/*
  The main play-Screen
*/

class Control {
  private:
    bool keyHeld,      // keyHeld indicates if the key is currently being pressed,
         keyPressed;   // keyPressed indicates if the key has been pressed since the last request
    float repeatTime,  // The amount of time between repeats
          timeLeft;    // The amount of time left until the next "press"
    list<FloatyLabel> scoreLabels;
  
  public:
    Control(); // Default constructor with no time between keypresses
    Control(float repeatTime); // Constructor that specifies the amount of time between keypresses
    
    void keyEvent(bool pressed); // Called when the key gets pressed or released
    bool isPressed(); // Returns true if the key should be considered pressed
    void timerTick(float dTime); // Advances the current idea of time
    float getRepeatTime() const; // Find out how much time between key presses
    void setRepeatTime(float time); // Set the time between key presses
    void flush(); // Stop considering the key previously-pressed
};

class Controls {
  private:
    Control left, right, up, down, drop, spinLeft, spinRight;
    int numPressed;
  
  public:
    Controls();
    Controls(float repeatTime);
    
    void keyEvent(const SDL_KeyboardEvent& key);
    bool holdingLeft();
    bool holdingRight();
    bool holdingUp();
    bool holdingDown();
    bool holdingDrop();
    bool holdingSpinLeft();
    bool holdingSpinRight();
    bool anyKeyPressed();
    void flushKeys(); // Stop considering all keys as previously-pressed
    
    void timerTick(float dTime);
    float getRepeatTime() const;
    void setRepeatTime(float repeatTime);
};

enum PlayState { PLAYING, DROPPING_BLOCK, WAITING };

class PlayScreen : public Screen, public ButtonCallback, public Loadable, public GridListener {
  private:
    // Widgets
    Panel panel, floatables;
    list<FloatyLabel> floatingLabels;
    Button menu;
    ScoreView scoreView;
    // 3D view-related
    Light lamp;
    Camera camera;
    // Playing-area related
    Mesh backdrop;
    Grid grid;
    // Block-related:
    Shape shape, nextShape;
    float dropTime;
    PlayState state;
    // Input-related
    Controls controls;
  
  public:
    PlayScreen(int screenID);
    ~PlayScreen();
    
    void prepareForShow();
    void prepareForHide();
    void screenPaint() const;
    void timerTick(float dTime);
    void keyboard(const SDL_KeyboardEvent& key);
    void mouseButton(const SDL_MouseButtonEvent& mouse);
    void mouseMotion(const SDL_MouseMotionEvent& mouse);
    bool isOpaque() const;
    
    void dropShape();
    void putShapeInGrid(int distance=0);
    
    void rowRemoved(int row);
    void gridEmpty();
    
    void load();
    
    void buttonCallback(const Button& b);
};

