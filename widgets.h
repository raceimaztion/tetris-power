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
  protected:
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
class ButtonListener;
class Button : public Label {
  protected:
    int tag, state;
    list<void (*)(const Button&)> listenerFunctions;
    list<ButtonListener*> listenerClasses;
    void trigger();
    
  public:
    Button(int x, int y, int width, int height, Colour c, const string& label, Font* font, int tag);
    Button(const Button& b);
    
    int getTag() const;
    
    void addListener(void (*listener)(const Button&));
    void addListener(ButtonListener *c);
    void removeListener(void (*listener)(const Button&));
    
    void paint() const;
    void timerTick(float dTime);
    void mouse(const SDL_MouseButtonEvent& mouse);
    void mouse(const SDL_MouseMotionEvent& mouse);
};

class ButtonListener {
  public:
    virtual void buttonListener(const Button& b);
};

// Button states:
#define BUTTON_NORMAL 0x10
#define BUTTON_HOVERED 0x11
#define BUTTON_PRESSED 0x12
#define BUTTON_BLANKED 0x13

// Panel widget class
class Panel : public Widget {
  protected:
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

// Progress bar widget class
class ProgressMeter : public Widget {
  protected:
    float percentage;
    
  public:
    ProgressMeter(int x, int y, int width, int height, Colour c, float percentage=0.0f);
    ProgressMeter(const ProgressMeter& pm);
    
    float getPercentage() const;
    void setPercentage(float percentage);
    void paint() const;
    void timerTick(float dTime);
};

// A label that floats upwards, fading out as it rises
class FloatyLabel : public Label {
  protected:
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
    
    bool operator==(const FloatyLabel& fl);
};

class SpinArrowsListener;
class SpinArrows : public Widget {
  protected:
    list<SpinArrowsListener*> listeners;
    int tag;
    bool upHovered, downHovered;
  
  public:
    SpinArrows(int x, int y, int width, int height, Colour c, int tag);
    SpinArrows(const SpinArrows& sa);
};

class SpinArrowsListener {
  public:
    virtual void spinArrowsListener(const SpinArrows& sa, bool 
};

