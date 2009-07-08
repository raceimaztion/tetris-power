/*
  The class definition for the main menu
*/

class MainMenu : public Screen, public ButtonCallback {
  private:
    //Mesh cube;
    Panel panel;
    Label title;
    Button start, options, quit;
    
  public:
    MainMenu(int screenID);
    
    void timerTick(float dTime);
    void prepareForShow();
    void prepareForHide();
    void screenPaint() const;
    void keyboard(const SDL_KeyboardEvent &key);
    void mouseButton(const SDL_MouseButtonEvent &mouse);
    void mouseMotion(const SDL_MouseMotionEvent &mouse);
    bool isOpaque() const;
    
    void buttonCallback(const Button& b);
};

