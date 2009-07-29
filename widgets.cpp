#include "common.h"

// Bevel controls:
//#define BEVEL_TOP_LEFT
#define BEVEL_TOP_RIGHT
//#define BEVEL_BOTTOM_RIGHT
#define BEVEL_BOTTOM_LEFT
#define BEVEL_MAX 20

void _wDrawBeveledRect(int x, int y, int width, int height)
{
  int bevel = min(min(width, height), BEVEL_MAX) / 2;
  
#ifdef BEVEL_TOP_LEFT
  glVertex2i(x + bevel, y);
  glVertex2i(x, y + bevel);
#else
  glVertex2i(x, y);
#endif

#ifdef BEVEL_BOTTOM_LEFT
  glVertex2i(x, y+height-bevel-1);
  glVertex2i(x+bevel, y+height-1);
#else
  glVertex2i(x, y+height-1);
#endif

#ifdef BEVEL_BOTTOM_RIGHT
  glVertex2i(x+width-bevel-1, y+height-1);
  glVertex2i(x+width-1, y+height-bevel-1);
#else
  glVertex2i(x+width-1, y+height-1);
#endif

#ifdef BEVEL_TOP_RIGHT
  glVertex2i(x+width-1, y+bevel);
  glVertex2i(x+width-bevel-1, y);
#else
  glVertex2i(x+width-1, y);
#endif
} // end _wDrawBeveledRect()

void _wDrawGradientRect(int x, int y, int width, int height, Colour c)
{
  c.brighter(0.1f).apply();
  glVertex2i(x+width-1, y);
  glVertex2i(x, y);
  c.darker(0.1f).apply();
  glVertex2i(x, y+height-1);
  glVertex2i(x+width-1, y+height-1);
}

// Handy rectangle-drawing functions
void wDrawRect(int x, int y, int width, int height, Colour c)
{
  glBegin(GL_LINE_LOOP);
//  _wDrawBeveledRect(x, y, width, height);
  _wDrawGradientRect(x, y, width, height, c);
  glEnd();
}

void wFillRect(int x, int y, int width, int height, Colour c)
{
  glBegin(GL_POLYGON);
//  _wDrawBeveledRect(x, y, width, height);
  _wDrawGradientRect(x, y, width, height, c);
  glEnd();
}

/* ***************** *
 * base Widget class *
 * ***************** */
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
void Widget::timerTick(float dTime) { }
void Widget::mouse(const SDL_MouseButtonEvent& mouse) { }
void Widget::mouse(const SDL_MouseMotionEvent& mouse) { }

/* ****************** *
 * Label widget class *
 * ****************** */
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
  
  glPushMatrix();
  if (x < 0) glTranslatef(getWidth(), 0, 0);
  if (y < 0) glTranslatef(0, getHeight(), 0);
  
  int lineWidth = fStringWidth(font, label.c_str()), lineHeight = fFontHeight(font);
  float px = x + align_x*(width - lineWidth), py = y + height - align_y*(height - lineHeight);
  
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  
  glRasterPos2f(px, py);
  c.apply();
  
  fDrawString(font, label.c_str());
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
  glPopMatrix();
}

void Label::timerTick(float dTime) { }

/* ******************* *
 * Button widget class *
 * ******************* */
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
  callbackFunctions.remove(callback);
}

void Button::trigger()
{
  for (list<void (*)(const Button& b)>::iterator cur = callbackFunctions.begin();
                                                 cur != callbackFunctions.end();
                                                 cur++)
    (*cur)(*this);
  for (list<ButtonCallback*>::iterator cur = callbackClasses.begin();
                                       cur != callbackClasses.end();
                                       cur ++)
    (*cur)->buttonCallback(*this);
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
  {
    wFillRect(x, y+height-1, width, -height, c);
    glColor3f(0, 0, 0);
  }
  else
  {
    wDrawRect(x, y, width, height, c);
    if (state == BUTTON_HOVERED)
    {
      wFillRect(x+1, y+1, width-3, height-3, c);
      glColor3f(0, 0, 0);
    }
  }
  
  glPopMatrix();
  Label::paint();
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void Button::timerTick(float dTime)
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

/* ****************** *
 * Panel widget class *
 * ****************** */
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
  children.remove(child);
}

void Panel::setFont(Font* font)
{
  for (list<Widget*>::iterator cur = children.begin();
                               cur != children.end();
                               cur++)
    (*cur)->setFont(font);
}

void Panel::paint() const
{
#ifdef DEBUG
  printf("Panel::paint(): Drawing panel and %d %s.\n", children.size(), (children.size()==1 ? "child": "children"));
#endif
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  
  for (list<Widget*>::const_iterator cur = children.begin();
                               cur != children.end();
                               cur++)
    (*cur)->paint();
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void Panel::timerTick(float dTime)
{
  for (list<Widget*>::iterator cur = children.begin();
                               cur != children.end();
                               cur++)
    (*cur)->timerTick(dTime);
}

void Panel::mouse(const SDL_MouseButtonEvent& mouse)
{
  for (list<Widget*>::iterator cur = children.begin();
                               cur != children.end();
                               cur++)
    (*cur)->mouse(mouse);
}

void Panel::mouse(const SDL_MouseMotionEvent& mouse)
{
  for (list<Widget*>::iterator cur = children.begin();
                               cur != children.end();
                               cur++)
    (*cur)->mouse(mouse);
}

/* ************************** *
 * ProgressMeter widget class *
 * ************************** */
ProgressMeter::ProgressMeter(int x, int y, int width, int height, Colour c, float percentage) :
                                                    Widget(x, y, width, height, c)
{
  this->percentage = percentage;
}

ProgressMeter::ProgressMeter(const ProgressMeter& pm) : Widget(pm)
{
  percentage = pm.percentage;
}

float ProgressMeter::getPercentage() const
{
  return percentage;
}

void ProgressMeter::setPercentage(float percentage)
{
  this->percentage = percentage;
}

void ProgressMeter::paint() const
{
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  c.apply();
  
  // Draw the outline
  wDrawRect(x, y, width, height, c);
  // Draw the progress
  wFillRect(x+1, y+1, (int)((width-3)*min(1.0f, percentage)), height-3, c);
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void ProgressMeter::timerTick(float dTime)
{
  // Not using this yet
  // Maybe use it to change a stipple pattern used on the fill meter?
}

/* ***************** *
 * FloatyLabel class *
 * ***************** */
FloatyLabel::FloatyLabel(int x, int y, Colour c, string label, Font* font, float totalTime, float vertSpeed) :
                                                       Label(x, y, 0, 0, c, label, font)
{
  curTime = 0.0f;
  this->totalTime = totalTime;
  this->vertSpeed = vertSpeed;
}

FloatyLabel::FloatyLabel(int x, int y, int width, int height, Colour c, string label, Font* font, float totalTime, float vertSpeed) :
                                                       Label(x, y, width, height, c, label, font)
{
  curTime = 0.0f;
  this->totalTime = totalTime;
  this->vertSpeed = vertSpeed;
}

FloatyLabel::FloatyLabel(const FloatyLabel& fl) : Label(fl)
{
  curTime = fl.curTime;
  totalTime = fl.totalTime;
}

void FloatyLabel::paint() const
{
  if (!visible) return;
  
  c.apply(1.0f - curTime/totalTime);
  
  int lineWidth = fStringWidth(getFont(), getLabel().c_str()), lineHeight = fFontHeight(getFont());
  float px = x + getAlignmentX()*(width - lineWidth), py = y + height - getAlignmentY()*(height - lineHeight) + curTime*vertSpeed;
  
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  
  glRasterPos2f(px, py);
  c.apply();
  
  fDrawString(getFont(), getLabel().c_str());
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void FloatyLabel::timerTick(float dTime)
{
  curTime += dTime;
  triggerRepaint();
}

bool FloatyLabel::isDone() const
{
  return curTime >= totalTime;
}

float FloatyLabel::getCurTime() const
{
  return curTime;
}

float FloatyLabel::getTotalTime() const
{
  return totalTime;
}

float FloatyLabel::getVertSpeed() const
{
  return vertSpeed;
}


