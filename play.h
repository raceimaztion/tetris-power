/*
  The main play-Screen
*/

class PlayScreen : public Screen, public ButtonCallback {
  private:
    Mesh backdrop;
    Panel panel;
    Button menu;
    Light lamp;
    float angle;
  
  public:
    PlayScreen(int screenID);
    ~PlayScreen();
    
    void prepareForShow();
    void prepareForHide();
    void screenPaint() const;
    void timerTick();
    void keyboard(const SDL_KeyboardEvent& key);
    void mouseButton(const SDL_MouseButtonEvent& mouse);
    void mouseMotion(const SDL_MouseMotionEvent& mouse);
    bool isOpaque() const;
    
    void load();
    
    void buttonCallback(const Button& b);
};
