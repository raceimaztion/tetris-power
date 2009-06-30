#include "common.h"

// Widget base class
Widget::Widget(int x, int y, int width, int height, Colour c)
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->c = c;
  
  visible = true;
}

Widget::Widget(const Widget& w)
{
  x = w.x;
  y = w.y;
  width = w.width;
  height = w.height;
  c = w.c;
  visible = w.visible;
}

int Widget::getX() const
{
  return x;
}

int Widget::getY() const
{
  return y;
}

int Widget::getWidth() const
{
  return width;
}

int Widget::getHeight() const
{
  return height;
}

Colour Widget::getColour() const
{
  return c;
}

bool Widget::isVisible() const
{
  return visible;
}

void Widget::setVisible(bool show)
{
  visible = show;
}

void Widget::show()
{
  visible = true;
}

void Widget::hide()
{
  visible = false;
}

bool Widget::isIn(int px, int py) const
{
  if (px < x || py < y) return false;
  if (px > x+width || py > y+height) return false;
  return true;
}

void Widget::setFont(Font* font) { }
void Widget::paint() const { printf("Runtime warning: Using raw Widget::paint() method!\n"); }
void Widget::timerTick() { }
void Widget::mouse(const SDL_MouseButtonEvent& mouse) { }
void Widget::mouse(const SDL_MouseMotionEvent& mouse) { }

// Label widget class
Label::Label(int x, int y, int width, int height, Colour c, const string& label, Font* font) : Widget(x, y, width, height, c)
{
  this->label = label;
  this->font = font;
  align_x = align_y = 0.5f;
}

Label::Label(const Label& l) : Widget(l)
{
  label = l.label;
  font = l.font;
  align_x = l.align_x;
  align_y = l.align_y;
}

string Label::getLabel() const
{
  return label;
}

void Label::setLabel(string label)
{
  this->label = label;
}

Font* Label::getFont() const
{
  return font;
}

void Label::setFont(Font *font)
{
  this->font = font;
}

float Label::getAlignmentX() const
{
  return align_x;
}

void Label::setAlignmentX(float x)
{
  align_x = x;
}

float Label::getAlignmentY() const
{
  return align_y;
}

void Label::setAlignmentY(float y)
{
  align_y = y;
}

void Label::setAlignment(float x, float y)
{
  align_x = x;
  align_y = y;
}

void Label::paint() const
{
  // If we don't have a font or aren't visible, don't draw anything
  if (font == NULL)
  {
    printf("Runtime warning: Label::paint(): No font is selected, aborting paint() method.\n");
    return;
  }
  if (!visible)
    return;
  
  int lineWidth = fStringWidth(font, label.c_str()), lineHeight = fFontHeight(font);
  float px = x + align_x*(width - lineWidth), py = y + height - align_y*(height - lineHeight);
  
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  
  glRasterPos2f(px, py);
  c.apply();
  fDrawString(font, label.c_str());
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void Label::timerTick() { }

// Button widget
Button::Button(int x, int y, int width, int height, Colour c,
               const string& label, Font* font, int tag)
               : Label(x, y, width, height, c, label, font)
{
  this->tag = tag;
  state = BUTTON_NORMAL;
}

Button::Button(const Button& b) : Label(b)
{
  tag = b.tag;
  state = b.state;
  callbackFunctions = b.callbackFunctions;
  callbackClasses = b.callbackClasses;
}

int Button::getTag() const
{
  return tag;
}

void Button::addCallback(void (*callback)(const Button&))
{
  callbackFunctions.push_back(callback);
}

void Button::addCallback(ButtonCallback *c)
{
  callbackClasses.push_back(c);
}

void Button::removeCallback(void (*callback)(const Button&))
{
  // TODO: Remove reference to callback
}

void Button::trigger()
{
  for (unsigned int i=0; i < callbackFunctions.size(); i++)
    (*(callbackFunctions.at(i)))(*this);
  for (unsigned int i=0; i < callbackClasses.size(); i++)
    callbackClasses.at(i)->buttonCallback(*this);
}

void Button::paint() const
{
#ifdef DEBUG
  printf("Button::paint(): Drawing button.\n");
#endif
  if (!visible) return;
  
  glPushMatrix();
  if (x < 0) glTranslatef(width, 0, 0);
  if (y < 0) glTranslatef(0, height, 0);
  
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  
  c.apply();
  if (state == BUTTON_PRESSED)
    glBegin(GL_POLYGON);
  else
    glBegin(GL_LINE_LOOP);
  
    glVertex2f(x, y);
    glVertex2f(x, y + 0.5f*height);
    glVertex2f(x + 0.5f*height, y+height);
    glVertex2f(x+width, y+height);
    glVertex2f(x+width, y + 0.5f*height);
    glVertex2f(x + width - 0.5f*height, y);
  glEnd();
  
  if (state == BUTTON_HOVERED)
  {
    glBegin(GL_POLYGON);
      glVertex2f(x+1, y+1);
      glVertex2f(x+1, y + 0.5f*height - 1);
      glVertex2f(x + 0.5f*height + 1, y+height - 2);
      glVertex2f(x+width - 2, y+height - 2);
      glVertex2f(x+width - 2, y + 0.5f*height + 1);
      glVertex2f(x + width - 0.5f*height - 1, y+1);
    glEnd();
  }
  
  if (state == BUTTON_HOVERED || state == BUTTON_PRESSED)
    glColor3f(0, 0, 0);
  
  glPopMatrix();
  
  Label::paint();
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void Button::timerTick()
{
  
}

void Button::mouse(const SDL_MouseButtonEvent& mouse)
{
  if (mouse.type == SDL_MOUSEBUTTONDOWN)
  {
    if (isIn(mouse.x, mouse.y))
    {
      state = BUTTON_PRESSED;
      triggerRepaint();
    }
  }
  else if (mouse.type == SDL_MOUSEBUTTONUP)
  {
    if (isIn(mouse.x, mouse.y))
      trigger();
    if (state != BUTTON_NORMAL)
    {
      state = BUTTON_NORMAL;
      triggerRepaint();
    }
  }
}

void Button::mouse(const SDL_MouseMotionEvent& mouse)
{
  switch (state)
  {
    case BUTTON_NORMAL:
      if (isIn(mouse.x, mouse.y))
      {
        state = BUTTON_HOVERED;
        triggerRepaint();
      }
      break;
    case BUTTON_HOVERED:
      if (!isIn(mouse.x, mouse.y))
      {
        state = BUTTON_NORMAL;
        triggerRepaint();
      }
      break;
    case BUTTON_PRESSED:
      if (!isIn(mouse.x, mouse.y))
      {
        state = BUTTON_BLANKED;
        triggerRepaint();
      }
      break;
    case BUTTON_BLANKED:
      if (isIn(mouse.x, mouse.y))
      {
        state = BUTTON_PRESSED;
        triggerRepaint();
      }
      break;
    default:
      state = BUTTON_NORMAL;
      break;
  }
}

// Button callback class:
void ButtonCallback::buttonCallback(const Button& b) { }

// Panel widget class:
Panel::Panel(int x, int y, int width, int height, Colour c) : Widget(x, y, width, height, c)
{
  // Nothing much to do here
}

void Panel::addChild(Widget *child)
{
  children.push_back(child);
}

void Panel::removeChild(Widget *child)
{
  // TODO: remove the child
}

void Panel::setFont(Font* font)
{
  for (unsigned int i=0; i < children.size(); i++)
    children.at(i)->setFont(font);
}

void Panel::paint() const
{
#ifdef DEBUG
  printf("Panel::paint(): Drawing panel and %d %s.\n", children.size(), (children.size()==1 ? "child": "children"));
#endif
  for (unsigned int i=0; i < children.size(); i++)
    children.at(i)->paint();
}

void Panel::timerTick()
{
  for (unsigned int i=0; i < children.size(); i++)
    children.at(i)->timerTick();
}

void Panel::mouse(const SDL_MouseButtonEvent& mouse)
{
  for (unsigned int i=0; i < children.size(); i++)
    children.at(i)->mouse(mouse);
}

void Panel::mouse(const SDL_MouseMotionEvent& mouse)
{
  for (unsigned int i=0; i < children.size(); i++)
    children.at(i)->mouse(mouse);
}
