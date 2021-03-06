/*
  the class definition for the Splash screen
*/

#pragma implementation
class SplashScreen : public Screen {
  private:
    Panel panel;
    Label status, title;
    
  public:
    SplashScreen(int screenID);
    ~SplashScreen();
    
    void timerTick(float dTime);
    void prepareForShow();
    void prepareForHide();
    void screenPaint() const;
    void keyboard(const SDL_KeyboardEvent &key);
    void mouseButton(const SDL_MouseButtonEvent &mouse);
    void mouseMotion(const SDL_MouseMotionEvent &mouse);
    bool isOpaque() const;
};

