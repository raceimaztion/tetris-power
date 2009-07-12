/*
  The main play-Screen
*/

class Control {
  private:
    bool keyHeld,      // keyHeld indicates if the key is currently being pressed,
         keyPressed;   // keyPressed indicates if the key has been pressed since the last request
    float repeatTime,  // The amount of time between repeats
          timeLeft;    // The amount of time left until the next "press"
  
  public:
    Control(); // Default constructor with no time between keypresses
    Control(float repeatTime); // Constructor that specifies the amount of time between keypresses
    
    void keyEvent(bool pressed); // Called when the key gets pressed or released
    bool isPressed(); // Returns true if the key should be considered pressed
    void timerTick(float dTime); // Advances the current idea of time
    float getRepeatTime() const; // Find out how much time between key presses
    void setRepeatTime(float time); // Set the time between key presses
};

class Controls {
  private:
    bool moveLeft, holdLeft;
    bool moveRight, holdRight;
    bool moveUp, holdUp;
    bool moveDown, holdDown;
    bool moveDrop, holdDrop;
    bool moveSpinLeft, holdSpinLeft;
    bool moveSpinRight, holdSpinRight;
  
  public:
    Controls();
    
    void key(const SDL_KeyboardEvent& key);
    bool holdingLeft();
    bool holdingRight();
    bool holdingUp();
    bool holdingDown();
    bool holdingDrop();
    bool holdingSpinLeft();
    bool holdingSpinRight();
};

class PlayScreen : public Screen, public ButtonCallback {
  private:
    Mesh backdrop;
    Panel panel;
    Button menu;
    Light lamp;
    Camera camera;
    Shape shape;
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
    
    void load();
    
    void buttonCallback(const Button& b);
};

