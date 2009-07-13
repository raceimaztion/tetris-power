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
    
    void timerTick(float dTime);
    float getRepeatTime() const;
    void setRepeatTime(float repeatTime);
};

enum PlayState { PLAYING, DROPPING_BLOCK, WAITING };

class PlayScreen : public Screen, public ButtonCallback {
  private:
    Mesh backdrop;
    Panel panel;
    Button menu;
    Light lamp;
    Camera camera;
    Shape shape;
    Controls controls;
    Grid grid;
    PlayState state;
  
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

