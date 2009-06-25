/*
  The class definition for the main menu
*/

#define MAINMENU_TAG_QUIT 0x21

class MainMenu : public Screen, public ButtonCallback {
  private:
    //Mesh cube;
    Panel panel;
    Button quit;
    Label label;
    
  public:
    MainMenu(int screenID);
    
    void timerTick();
    void prepareForShow();
    void prepareForHide();
    void screenPaint();
    void keyboard(const SDL_KeyboardEvent &key);
    void mouseButton(const SDL_MouseButtonEvent &mouse);
    void mouseMotion(const SDL_MouseMotionEvent &mouse);
    bool isOpaque();
    
    void buttonCallback(const Button& b);
};

