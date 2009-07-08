/*
  The main play-Screen
*/

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

