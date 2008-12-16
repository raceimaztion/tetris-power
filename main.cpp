#define MAIN_MODULE
#include "common.h"

SDL_Surface *screen; // This is the backbuffer

/* **************************** *
 * SDL initialization functions *
 * **************************** */
bool initVideo(Uint32 flags = SDL_DOUBLEBUF | SDL_OPENGL)
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

void handleTimer(int delay)
{
//  glutTimerFunc(TIMER_TICK, handleTimer, 101);
}

void handleReshape(int width, int height)
{
  win_width = width;
  win_height = height;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, win_width, 0, win_height, 0.0001f, 100000);
  
  glViewport(0, 0, width, height);
  
  //glutPostRedisplay();
}

int main(int argc, char **argv)
{
  // Global initialization
  srand(time(NULL));
  if (!initVideo())
    exit(1);
  //glutInit(&argc, argv);
  
  // Application-specific initialization
  //glutDisplayFunc(handleDisplay);
  //glutReshapeFunc(handleReshape);
  
  // Start the timer just before we get to the main loop
  //glutTimerFunc(TIMER_TICK, handleTimer, 101);
  
  //glutMainLoop();
  return 0;
}
