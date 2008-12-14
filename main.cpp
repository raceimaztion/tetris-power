#define MAIN_MODULE
#include "common.h"

void handleDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void handleTimer(int delay)
{
  glutTimerFunc(TIMER_TICK, handleTimer, 101);
}

void handleReshape(int width, int height)
{
  win_width = width;
  win_height = height;
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, win_width, 0, win_height, 0.0001f, 100000);
  
  glViewport(0, 0, width, height);
  
  glutPostRedisplay();
}

int main()
{
  srand(time(NULL));
  glutDisplayFunc(handleDisplay);
  glutReshapeFunc(handleReshape);
  
  // Start the timer just before we get to the main loop
  glutTimerFunc(TIMER_TICK, handleTimer, 101);
  
  glutMainLoop();
  return 0;
}
