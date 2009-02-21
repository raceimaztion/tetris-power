/*
  the class definition for the Intro or Splash screen
*/

#pragma implementation
class IntroScreen : public Screen {
  public:
    IntroScreen();
    ~IntroScreen();
    
    void timerTick();
    void prepareForShow();
    void prepareForHide();
    void screenPaint();
    void keyboard(const SDL_keysym &key);
    void mouseButton(const SDL_MouseButtonEvent &mouse);
    void mouseMotion(const SDL_MouseMotionEvent &mouse);
    bool isOpaque();
};

