/*
  the class definition for the Splash screen
*/

#pragma implementation
class SplashScreen : public Screen {
  private:
    float progress;
    bool done;
    
  public:
    SplashScreen(int screenID);
    ~SplashScreen();
    
    void timerTick();
    void prepareForShow();
    void prepareForHide();
    void screenPaint();
    void keyboard(const SDL_KeyboardEvent &key);
    void mouseButton(const SDL_MouseButtonEvent &mouse);
    void mouseMotion(const SDL_MouseMotionEvent &mouse);
    bool isOpaque();
};

