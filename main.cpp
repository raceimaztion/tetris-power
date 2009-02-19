#define MAIN_MODULE
#include "common.h"

SDL_Surface *screen; // This is the backbuffer

/* **************************** *
 * SDL initialization functions *
 * **************************** */
bool initVideo(Uint32 flags = SDL_OPENGL)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Could not initialize SDL video! Error was:\n%s\n", SDL_GetError());
    return false;
  }
  atexit(SDL_Quit);
  
  // Set up pre-window OpenGL parameters
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  
  // Now set the video mode
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
  if (screen == NULL)
  {
    fprintf(stderr, "Unable to set video mode! Error was:\n%s\n", SDL_GetError());
    return false;
  }
  
  // Set up post-window OpenGL parameters
  glEnable(GL_MULTISAMPLE);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
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
  
  lastTime = curTime;
}

void handleKeyboardEvent(const SDL_keysym& key)
{
  
}

/* ******************* *
 * Main entry function *
 * ******************* */
int main(int argc, char **argv)
{
  // Global initialization
  srand(time(NULL));
  if (!initVideo())
    exit(1);
  for (int i=0; i < 323; i++)
    keys_pressed[i] = 0;
  num_keys_pressed = 0;
  
  bool running = true;
  SDL_Event event;
  
  // The main game loop
  while (running)
  {
    // Process SDL events
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          handleKeyboardEvent(event.key.keysym);
          break;
        
        case SDL_QUIT:
          running = false;
      }
    }
    
    // Wait ten milliseconds and do it all again
    SDL_Delay(10);
  }
  
  return 0;
}
