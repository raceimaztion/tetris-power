#define MAIN_MODULE
#include "common.h"

#define MESSAGE_REPAINT 201

SDL_Surface *screen; // This is the backbuffer
bool needsRepaint = false;

/* **************************** *
 * SDL initialization functions *
 * **************************** */
bool initVideo(Uint32 flags = SDL_OPENGL)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Error: Could not initialize SDL video! Error was:\n%s\n", SDL_GetError());
    return false;
  }
  atexit(SDL_Quit);
  
  // Set up pre-window OpenGL parameters
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  //8-bits for each Red, Green, Blue, and Alpha plane
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  
  //24-bits for the depth buffer and enable double-buffering
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  // Try to enable multisampling (anti-aliasing)
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  
  // Now set the video mode
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
  if (screen == NULL)
  {
    // If we don't get 4 samples, try 2
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
    
    if (screen == NULL)
    {
      printf("Warning: Failed to create screen with anti-aliasing, falling back to regular rendering.\n");
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    
      screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
      if (screen == NULL)
      {
        fprintf(stderr, "Error: Unable to set video mode! Error was:\n%s\n", SDL_GetError());
        return false;
      } // end if no samples
    } // end if try 2 samples
  } // end if try 4 samples
  
  // Set up post-window OpenGL parameters
  glEnable(GL_MULTISAMPLE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
  glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
  
  return true;
}

bool initAudio()
{
  if (SDL_Init(SDL_INIT_AUDIO) < 0)
  {
    fprintf(stderr, "Warning: Unable to initialize audio! Another program may be using it! Reason was:\n%s\n", SDL_GetError());
    return false;
  }
  
  if (Mix_OpenAudio(11025, AUDIO_S16, 2, 512) < 0)
  {
    fprintf(stderr, "Warning: Unable to set up 11kHz 16-bit stereo. Reason was:\n%s\n", SDL_GetError());
    return false;
  }
  
  return true;
}

/* ***************************************** *
 * Functions for doing things from callbacks *
 * ***************************************** */
void triggerRepaint()
{
  needsRepaint = true;
/*
  SDL_Event event;
  event.type = SDL_USEREVENT;
  event.user.code = MESSAGE_REPAINT;
  event.user.data1 = 0;
  event.user.data2 = 0;
  
  SDL_PushEvent(&event);
*/
}

/* ********* *
 * Callbacks *
 * ********* */
void handleDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  SDL_GL_SwapBuffers();
}

void gameStep()
{
  static Uint32 lastTime = 0;
  static float currentTime = 0;
  if (lastTime == 0)
    lastTime = SDL_GetTicks();
  
  Uint32 curTime = SDL_GetTicks();
  float dTime = 0.001f*(curTime - lastTime);
  currentTime += dTime;
  
  // Process stuff here
  screenTimerTick(dTime);
  
  lastTime = curTime;
}

void handleKeyboardEvent(const SDL_KeyboardEvent& key)
{
  screenKeyboard(key);
}

void handleMouseButton(const SDL_MouseButtonEvent &mouse)
{
  screenMouseButton(mouse);
}

void handleMouseMotion(const SDL_MouseMotionEvent &mouse)
{
  screenMouseMotion(mouse);
}

Font *loadFont(const string& fileName)
{
  SDL_Surface *fontSurface = IMG_Load(fileName.c_str());
  if (fontSurface == NULL)
  {
    fprintf(stderr, "Runtime warning: Failed to load font from file '%s', you might not get some text in-game!\n", fileName.c_str());
    return NULL;
  }
  Font *font = fParseFont(fontSurface);
  SDL_FreeSurface(fontSurface);
  if (font == NULL)
  {
    fprintf(stderr, "Runtime warning: Failed to parse font from file '%s', you might not get some text in-game!\n", fileName.c_str());
    return NULL;
  }
  return font;
}

void loadFonts()
{
  // Try to load the fallback font
  if (fallbackFont == NULL)
  {
    fallbackFont = loadFont("fonts/main_font.png");
  }
  
  // Try to load the large font
  if (largeFont == NULL)
  {
    largeFont = loadFont("fonts/large_font.png");
  }
}

/* *************** *
 * Thread function *
 * *************** */
Uint32 timerHandler(Uint32 interval, void* unused)
{
//  screenTimerTick();
  gameStep();
  
  // Check to see if we need to load anything
  if (!loaderDoneLoading())
  {
#ifdef DEBUG
    printf("main.cpp: timerHandler: Running loader.\n");
#endif
    loaderRunLoader();
  }
  
  return interval;
}


/* ******************* *
 * Main entry function *
 * ******************* */
int main(int argc, char **argv)
{
  printf("Starting Tetris-Power...\n");
  
  // Global initialization
  srand(time(NULL));
  
  if (!initVideo())
    exit(1);
  if (SDL_Init(SDL_INIT_TIMER) < 0)
  {
    fprintf(stderr, "Error: Failed to initialize SDL timer system.\nError was: %s.\n", SDL_GetError());
    exit(1);
  }
  
  for (int i=0; i < 323; i++)
    keys_pressed[i] = 0;
  num_keys_pressed = 0;
  win_width = SCREEN_WIDTH;
  win_height = SCREEN_HEIGHT;
  
  SDL_WM_SetCaption("Tetris Power", "Tetris Power");
  
  // Start the internal loaders
  loadFonts();
  comInit();
  shInit();
  
  // Create the splash screen
  SplashScreen splash(SPLASH_SCREEN);
  screenAddNew(splash, SPLASH_SCREEN);
  screenActivate(SPLASH_SCREEN);
  
  // Create the main menu
  MainMenu mainMenu(MAIN_MENU_SCREEN);
  screenAddNew(mainMenu, MAIN_MENU_SCREEN);
  
  // Create the play screen
  PlayScreen playScreen(PLAY_SCREEN);
  screenAddNew(playScreen, PLAY_SCREEN);
  
  bool running = true;
  SDL_Event event;
  
  // Set up our timer system
  SDL_AddTimer(TIMER_TICK, timerHandler, NULL);
  
  // The main game loop
  while (running)
  {
    // Process SDL events
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_USEREVENT:
          if (event.user.code == MESSAGE_REPAINT)
            screenPaint(true);
          break;
        
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          handleKeyboardEvent(event.key);
          break;
        
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          handleMouseButton(event.button);
          break;
        
        case SDL_MOUSEMOTION:
          handleMouseMotion(event.motion);
          break;
        
        case SDL_VIDEOEXPOSE:
        case SDL_VIDEORESIZE:
          screenPaint(true);
          break;
        
        case SDL_QUIT:
          running = false;
          break;
      }
    }
    
    // Wait ten milliseconds and do it all again
    SDL_Delay(10);
    
    // If we're out of screens, quit
    if (screenNumStackedScreens() < 1)
    {
      fprintf(stderr, "Runtime warning: No screens left in stack, quitting...\n");
      running = false;
    }
    else
    { // Repaint things if we need it
      screenPaint(needsRepaint);
      needsRepaint = false;
    }
  }
  
  // We're done, quit
  return 0;
}

