/* ************** *
 * Widget classes *
 * ************** */

// Base widget class
class Widget {
  protected:
    int x, y, width, height;
    Colour c;
    bool visible;
  
  public:
    Widget(int x, int y, int width, int height, Colour c);
    Widget(const Widget& w);
    
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    Colour getColour() const;
    bool isVisible() const;
    void setVisible(bool show);
    void show();
    void hide();
    bool isIn(int x, int y) const;
    
    virtual void setFont(Font* font);
    virtual void paint() const;
    virtual void timerTick(float dTime);
    virtual void mouse(const SDL_MouseButtonEvent& mouse);
    virtual void mouse(const SDL_MouseMotionEvent& mouse);
};

// Label widget class
class Label : public Widget {
  private:
    string label;
    Font *font;
    float align_x, align_y;
  
  public:
    Label(int x, int y, int width, int height, Colour c, const string& label, Font *font);
    Label(const Label& l);
    
    string getLabel() const;
    void setLabel(string label);
    Font* getFont() const;
    void setFont(Font* font);
    float getAlignmentX() const;
    void setAlignmentX(float x);
    float getAlignmentY() const;
    void setAlignmentY(float y);
    void setAlignment(float x, float y);
    
    virtual void paint() const;
    virtual void timerTick(float dTime);
};

// Button widget class
class ButtonCallback;
class Button : public Label {
  private:
    int tag, state;
    list<void (*)(const Button&)> callbackFunctions;
    list<ButtonCallback*> callbackClasses;
    void trigger();
    
  public:
    Button(int x, int y, int width, int height, Colour c, const string& label, Font* font, int tag);
    Button(const Button& b);
    
    int getTag() const;
    
    void addCallback(void (*callback)(const Button&));
    void addCallback(ButtonCallback *c);
    void removeCallback(void (*callback)(const Button&));
    
    void paint() const;
    void timerTick(float dTime);
    void mouse(const SDL_MouseButtonEvent& mouse);
    void mouse(const SDL_MouseMotionEvent& mouse);
};

class ButtonCallback {
  public:
    virtual void buttonCallback(const Button& b);
};

// Button states:
#define BUTTON_NORMAL 0x10
#define BUTTON_HOVERED 0x11
#define BUTTON_PRESSED 0x12
#define BUTTON_BLANKED 0x13

// Panel widget class
class Panel : public Widget {
  private:
    list<Widget*> children;
    
  public:
    Panel(int x, int y, int width, int height, Colour c);
    
    void addChild(Widget *child);
    void removeChild(Widget *child);
    
    void setFont(Font* font);
    
    void paint() const;
    void timerTick(float dTime);
    void mouse(const SDL_MouseButtonEvent& mouse);
    void mouse(const SDL_MouseMotionEvent& mouse);
};

class ProgressMeter : public Widget {
  private:
    float percentage;
    
  public:
    ProgressMeter(int x, int y, int width, int height, Colour c, float percentage=0.0f);
    ProgressMeter(const ProgressMeter& pm);
    
    float getPercentage() const;
    void setPercentage(float percentage);
    void paint() const;
    void timerTick(float dTime);
};

class FloatyLabel : public Label {
  private:
    float curTime, totalTime, vertSpeed;
  
  public:
    FloatyLabel(int x, int y, Colour c, string label, Font* font, float totalTime, float vertSpeed);
    FloatyLabel(int x, int y, int width, int height, Colour c, string label, Font* font, float totalTime, float vertSpeed);
    FloatyLabel(const FloatyLabel& fl);
    
    void paint() const;
    void timerTick(float dTime);
    bool isDone() const;
    
    float getCurTime() const;
    float getTotalTime() const;
    float getVertSpeed() const;
};

