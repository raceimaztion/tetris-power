/* ********************************** *
 * Screen-related classes and methods *
 * ********************************** */
class Screen {
  private:
    // Private variables
    bool needRepaint;
    bool visible;
    int screenID;
  
  public:
    Screen(int screenID);
    ~Screen();
    
    // Useful methods
    void end(); // Ends this screen and returns to the previous screen on the stack
    bool replaceWith(const int screenID); // Replace this screen with another one
    bool call(const int screenID); // Call up another screen on top of this one
    void markRepaint(); // Indicate that we need to repaint next time around
    void clearRepaint();
    bool needsRepaint(); // Returns true if we need a redraw
    void setVisible(bool visible);
    bool isVisible() const;
    static int getWidth();   // Return the size of the window
    static int getHeight();
    int getScreenID() const;
    
    // Virtual methods:
    virtual void timerTick(float dTime); // Called for each tick of the game system timer
    virtual void prepareForShow(); // Called every time this screen becomes visible
    virtual void prepareForHide(); // Called every time this screen becomes hidden
    virtual void screenPaint() const; // Called to redraw this screen. This MUST leave the OpenGL matrices as it found them!
    virtual void keyboard(const SDL_KeyboardEvent &key); // Called whenever a key is pressed or released
    virtual void mouseButton(const SDL_MouseButtonEvent &mouse);
    virtual void mouseMotion(const SDL_MouseMotionEvent &mouse);
    virtual bool isOpaque() const;
};

/*
  Checks to see if a screen has the specified ID
    Returns true if there is
*/
bool screenExists(int screenID);

/*
  Add a new screen with the specified ID
    Returns false if it didn't work
*/
bool screenAddNew(Screen &next, int screenID);

/*
  Activate a screen with the given ID
    Returns false if there's no screen by that code
*/
bool screenActivate(int screenID);

/*
  Pop the current screen off the "screen stack"
    If there are no screens on the stack, nothing happens
*/
void screenPop();

/*
  Send a timer tick to the current screen
    Returns true if the screens need a repaint
*/
bool screenTimerTick(float dTime);

// Send a repaint command to the screen stack, and force repaint if required
void screenPaint(bool force=false);

// Check to see if we need to repaint the screen
bool screenNeedsRepaint();

// Send a keyboard event to the current screen
void screenKeyboard(const SDL_KeyboardEvent &key);

// Send a mouse button event to the current screen
void screenMouseButton(const SDL_MouseButtonEvent &mouse);

// Send a mouse motion event to the current screen
void screenMouseMotion(const SDL_MouseMotionEvent &mouse);

// Find out how many screens are in the current stack
int screenNumStackedScreens();

