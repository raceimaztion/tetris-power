#include "common.h"

// Bevel controls:
//#define BEVEL_TOP_LEFT
#define BEVEL_TOP_RIGHT
//#define BEVEL_BOTTOM_RIGHT
#define BEVEL_BOTTOM_LEFT
#define BEVEL_MAX 20

#define _BORDER_THICKNESS 3.0f
#define _BORDER_INSET 4

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
  comDrawRoundRect(x, y, width, height, _BORDER_THICKNESS, c);
//  glBegin(GL_LINE_LOOP);
//  _wDrawBeveledRect(x, y, width, height);
//  _wDrawGradientRect(x, y, width, height, c);
//  glEnd();
}

void wFillRect(int x, int y, int width, int height, Colour c)
{
  comFillRoundRect(x, y, width, height, _BORDER_THICKNESS, c);
//  glBegin(GL_POLYGON);
//  _wDrawBeveledRect(x, y, width, height);
//  _wDrawGradientRect(x, y, width, height, c);
//  glEnd();
}

void wDrawText(float x, float y, float width, float height, float align_x, float align_y, const string& text, Font* font)
{
  if (font == NULL)
  {
    printf("Runtime warning: wDrawText(): No font is selected, aborting paint() method.\n");
    return;
  }
  
  int lineWidth = fStringWidth(font, text.c_str()), lineHeight = fFontHeight(font);
  float px = x + align_x*(width - lineWidth), py = y + height - align_y*(height - lineHeight);
  
  bool usedDepth = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  
  glRasterPos2f(px, py);
  fDrawString(font, text.c_str());
  
  if (usedDepth) glEnable(GL_DEPTH_TEST);
}

void wDrawText(float x, float y, const string& text, Font* font)
{
  wDrawText(x, y, 0, 0, 0.5, 0.5f, text, font);
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
  needRepaint = false;
}

Widget::Widget(const Widget& w)
{
  x = w.x;
  y = w.y;
  width = w.width;
  height = w.height;
  c = w.c;
  visible = w.visible;
  needRepaint = w.needRepaint;
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

void Widget::repaint()
{
  needRepaint = true;
}

void Widget::clearRepaint()
{
  needRepaint = false;
}

bool Widget::needsRepaint() const
{
  return needRepaint;
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
  if (!visible)
    return;
  
  c.apply();
  wDrawText(x, y, width, height, 0.5f, 0.5f, label, font);
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
  listenerFunctions = b.listenerFunctions;
  listenerClasses = b.listenerClasses;
}

int Button::getTag() const
{
  return tag;
}

void Button::addListener(void (*callback)(const Button&))
{
  listenerFunctions.push_back(callback);
}

void Button::addListener(ButtonListener *c)
{
  listenerClasses.push_back(c);
}

void Button::removeListener(void (*callback)(const Button&))
{
  listenerFunctions.remove(callback);
}

void Button::trigger()
{
  for (list<void (*)(const Button& b)>::iterator cur = listenerFunctions.begin();
                                                 cur != listenerFunctions.end();
                                                 cur++)
    (*cur)(*this);
  for (list<ButtonListener*>::iterator cur = listenerClasses.begin();
                                       cur != listenerClasses.end();
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
    wFillRect(x, y, width, height, c);
    glColor3f(0, 0, 0);
  }
  else
  {
    if (state == BUTTON_HOVERED)
    {
      wFillRect(x+_BORDER_INSET, y+_BORDER_INSET, width-2*_BORDER_INSET, height-2*_BORDER_INSET, c);
      wDrawRect(x, y, width, height, c);
      glColor3f(0, 0, 0);
    }
    else
      wDrawRect(x, y, width, height, c);
  }
  
  glPopMatrix();
  wDrawText(x, y, width, height, 0.5f, 0.5f, label, font);
  
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
      repaint();
    }
  }
  else if (mouse.type == SDL_MOUSEBUTTONUP)
  {
    if (isIn(mouse.x, mouse.y))
      trigger();
    if (state != BUTTON_NORMAL)
    {
      state = BUTTON_NORMAL;
      repaint();
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
void ButtonListener::buttonCallback(const Button& b) { }

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
  if (!visible) return;
  
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

void Panel::clearRepaint()
{
  for (list<Widget*>::iterator cur = children.begin();
                               cur != children.end();
                               cur++)
    (*cur)->clearRepaint();
}

bool Panel::needsRepaint() const
{
  for (list<Widget*>::const_iterator cur = children.begin();
                                     cur != children.end();
                                     cur++)
    if ((*cur)->needsRepaint())
      return true;
  
  return needRepaint;
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
  wFillRect(x+_BORDER_INSET, y+_BORDER_INSET, (int)((width-2*_BORDER_INSET)*min(1.0f, percentage)), height-2*_BORDER_INSET, c);
  
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
  vertSpeed = fl.vertSpeed;
}

void FloatyLabel::paint() const
{
  // If we don't have a font or aren't visible, don't draw anything
  if (font == NULL)
  {
    printf("Runtime warning: FloatyLabel::paint(): No font is selected, aborting paint() method.\n");
    return;
  }
  if (!visible)
    return;
  
  c.apply(1.0f - (curTime/totalTime));
  wDrawText(x, y + vertSpeed*curTime, width, height, 0.5f, 0.5f, label, font);
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

bool FloatyLabel::operator==(const FloatyLabel& fl)
{
  return (x == fl.x) && (y == fl.y) &&
         (width == fl.width) && (height == fl.height) &&
         (getLabel() == fl.getLabel()) && (curTime == fl.curTime);
}

/* *********************** *
 * SpinArrows widget class *
 * *********************** */
SpinArrows::SpinArrows(int x, int y, int width, int height, Colour c, int tag)
										: Widget(x, y, width, height, c)
{
  this->tag = tag;
  upHovered = downHovered = false;
}

SpinArrows::SpinArrows(const SpinArrows& sa) : Widget(sa),
                                               listeners(sa.listeners)
{
  tag = sa.tag;
  upHovered = sa.upHovered;
  downHovered = sa.downHovered;
}

void SpinArrows::trigger(bool up)
{
  for (list<SpinArrowsListener*>::iterator cur = listeners.begin(); cur != listeners.end(); cur++)
    (*cur)->spinArrowsCallback(*this, up);
}

int SpinArrows::getTag() const
{
  return tag;
}

void SpinArrows::paint() const
{
  if (upHovered)
  {
    c.apply();
    glBegin(GL_QUADS);
      glVertex2i(x, y);
      glVertex2i(x + width, y);
      glVertex2i(x + width, y + height/2);
      glVertex2i(x, y + height/2);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
      glVertex2i(x, y + height/2);
      glVertex2i(x + width/2, y);
      glVertex2i(x + width, y + height/2);
      glVertex2i(x + width/2, y + height/4);
    glEnd();
  }
  else
  {
    c.apply();
    glBegin(GL_QUADS);
      glVertex2i(x, y + height/2);
      glVertex2i(x + width/2, y);
      glVertex2i(x + width, y + height/2);
      glVertex2i(x + width/2, y + height/4);
    glEnd();
  }
  
  if (downHovered)
  {
    c.apply();
    glBegin(GL_QUADS);
      glVertex2i(x, y + height/2);
      glVertex2i(x + width, y + height/2);
      glVertex2i(x + width, y + height);
      glVertex2i(x, y + height);
    glEnd();
    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
      glVertex2i(x, y + height/2);
      glVertex2i(x + width/2, y + 3*height/4);
      glVertex2i(x + width, y + height/2);
      glVertex2i(x + width/2, y + height);
    glEnd();
  }
  else
  {
    c.apply();
    glBegin(GL_QUADS);
      glVertex2i(x, y + height/2);
      glVertex2i(x + width/2, y + 3*height/4);
      glVertex2i(x + width, y + height/2);
      glVertex2i(x + width/2, y + height);
    glEnd();
  }
}

void SpinArrows::timerTick(float dTime)
{
  // Nothing much to do here
}

void SpinArrows::mouse(const SDL_MouseButtonEvent& mouse)
{
  if (mouse.type != SDL_MOUSEBUTTONDOWN) return;
  
  if (upHovered)
    trigger(true);
  if (downHovered)
    trigger(false);
}

void SpinArrows::mouse(const SDL_MouseMotionEvent& mouse)
{
  if (!isIn(mouse.x, mouse.y))
  {
    if (upHovered || downHovered)
      repaint();
    upHovered = downHovered = false;
  }
  else
  {
    if (2*(mouse.y - y) > height) // Down
    {
      if (!downHovered || upHovered)
        repaint();
      downHovered = true;
      upHovered = false;
    }
    else // Up
    {
      if (!upHovered || downHovered)
        repaint();
      upHovered = true;
      downHovered = false;
    }
  }
} // end mouse(SDL_MouseMotionEvent)

void SpinArrows::addListener(SpinArrowsListener* listener)
{
  listeners.push_back(listener);
}

void SpinArrows::removeListener(SpinArrowsListener* listener)
{
  listeners.remove(listener);
}

void SpinArrowsListener::spinArrowsCallback(const SpinArrows& sa, bool up)
{
 // Nothing to do here
}

/* ************************** *
 * OptionSpinner widget class *
 * ************************** */
OptionSpinner::OptionSpinner(int x, int y, int width, int height, Colour c, int tag) : Widget(x, y, width, height, c),
                                                                                       spinner(x + width - height/2, y, height/2, height, c, tag),
                                                                                       label(x, y, width - height/2, height, c, "", fallbackFont)
{
  this->tag = tag;
  currentOption = -1;
  spinner.addListener(this);
}

OptionSpinner::OptionSpinner(const OptionSpinner& os) : Widget(os),
                                                        options(os.options),
                                                        listeners(os.listeners),
                                                        spinner(os.spinner),
                                                        label(os.label)
{
  tag = os.tag;
  currentOption = os.currentOption;
}

void OptionSpinner::trigger(string option, int index)
{
  for (list<OptionSpinnerListener*>::iterator cur = listeners.begin();
                                              cur != listeners.end();
                                              cur++)
    (*cur)->optionSpinnerCallback(*this, option, index);
}

int OptionSpinner::getTag() const
{
  return tag;
}

void OptionSpinner::paint() const
{
  label.paint();
  spinner.paint();
}

void OptionSpinner::timerTick(float dTime)
{
  // Nothing much to do here
  label.timerTick(dTime);
  spinner.timerTick(dTime);
}

void OptionSpinner::mouse(const SDL_MouseButtonEvent& mouse)
{
  label.mouse(mouse);
  spinner.mouse(mouse);
}

void OptionSpinner::mouse(const SDL_MouseMotionEvent& mouse)
{
  label.mouse(mouse);
  spinner.mouse(mouse);
}

void OptionSpinner::clearRepaint()
{
  Widget::clearRepaint();
  label.clearRepaint();
  spinner.clearRepaint();
}

bool OptionSpinner::needsRepaint() const
{
  return needRepaint || label.needsRepaint() || spinner.needsRepaint();
}

void OptionSpinner::addListener(OptionSpinnerListener* listener)
{
  listeners.push_back(listener);
}

void OptionSpinner::removeListener(OptionSpinnerListener* listener)
{
  listeners.remove(listener);
}

void OptionSpinner::addOption(string option)
{
  options.push_back(option);
  if (currentOption < 0)
  {
    currentOption = 0;
    label.setLabel(options.front());
    repaint();
  }
}

void OptionSpinner::removeOption(string option)
{
  options.remove(option);
  if (currentOption >= (int)options.size())
  {
    currentOption = (int)options.size() - 1;
    if (currentOption < 0)
      label.setLabel("");
    else
      label.setLabel(options.back());
    repaint();
  }
}

void OptionSpinner::spinArrowsCallback(const SpinArrows& sa, bool up)
{
  if (up)
  {
    if (options.empty())
    {
      currentOption = -1;
      label.setLabel("");
    }
    else
    {
      currentOption --;
      if (currentOption < 0)
      {
        currentOption = 0;
        label.setLabel(options.front());
      }
      else
      {
        list<string>::iterator cur = options.begin();
        for (int i=currentOption; i > 0; i--, cur++) ;
        label.setLabel(*cur);
      }
    }
  }
  else // Down
  {
    currentOption ++;
    if (currentOption >= (int)options.size())
    {
      currentOption = (int)options.size() - 1;
      if (currentOption < 0)
        label.setLabel("");
      else
        label.setLabel(options.back());
    }
    else
    {
      list<string>::iterator cur = options.begin();
      for (int i=currentOption; i > 0; i--, cur++) ;
      label.setLabel(*cur);
    }
  }
  
  trigger(label.getLabel(), currentOption);
  repaint();
}

void OptionSpinnerListener::optionSpinnerCallback(const OptionSpinner& os, string option, int index)
{
  // Nothing to do here
}

