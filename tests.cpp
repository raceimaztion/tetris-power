#define MAIN_MODULE
#define TESTS
#include "common.h"

SDL_Surface *screen; // This is the backbuffer
bool needsRepaint = false;

Texture tex;
Mesh mesh;
const Colour CUBE_COLOUR(0.3f, 0.5f, 0.5f);
Light light(Position(2, -5, 3), 1, Colour(0.75f));
float rotation = 0.0f;

void initGL(int samples);

/* **************** *
 * Loader functions *
 * **************** */
void loadImage()
{
  printf("Loading texture(s)...\n");
//  tex = texLoadTexture("textures/nehe.bmp");
  tex = texLoadTexture("textures/block-normals.png");
//  tex = texMakeCurveBackground(128, 128, false);
//  tex = texMakeCurveBorder(256, 256, true);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  
  if (glIsTexture(tex.getTextureIndex()))
    printf("Test texture seems fine.\n");
  else
    printf("Test texture does not seem to exist!\n");
  
  printf("Finished loading texture(s).\n");
}


/* **************************** *
 * SDL initialization functions *
 * **************************** */
bool initVideo()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Error: Could not initialize SDL video! Error was:\n%s\n", SDL_GetError());
    return false;
  }
  atexit(SDL_Quit);
  
  const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
  if (!videoInfo)
  {
    printf("Failed to obtain the SDL Video Info. Error was:\n%s\n", SDL_GetError());
    exit(1);
  }
  
  Uint32 flags = SDL_OPENGL |
                 SDL_GL_DOUBLEBUFFER |
                 SDL_HWPALETTE;
  if (videoInfo->hw_available)
  {
    printf("Trying to use a hardware surface for rendering.\n");
    flags |= SDL_HWSURFACE;
  }
  else
  {
    printf("Using a software surface for rendering.\n");
    flags |= SDL_SWSURFACE;
  }
  
  if (videoInfo->blit_hw)
  {
    printf("Trying to use hardware acceleration.\n");
    flags |= SDL_HWACCEL;
  }
  else
    printf("Using software acceleration.\n");
  
  // Set up pre-window OpenGL parameters
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  //8-bits for each Red, Green, Blue, and Alpha plane
//  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
//  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
//  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
//  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  
  //24-bits for the depth buffer and enable double-buffering
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  // Try to enable multisampling (anti-aliasing)
  int samples = 4;
/*  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  
  // Now set the video mode
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
  if (screen == NULL)
  {
    // If we don't get 4 samples, try 2
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
    samples = 2;
    
    if (screen == NULL)
    {
      printf("Warning: Failed to create screen with anti-aliasing, falling back to regular rendering.\n");
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);*/
      samples = 0;
    
      screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, flags);
      if (screen == NULL)
      {
        fprintf(stderr, "Error: Unable to set video mode! Error was:\n%s\n", SDL_GetError());
        return false;
      } // end if no samples
/*    } // end if try 2 samples
  } // end if try 4 samples*/
  
//  loadImage();
  
  // Set up post-window OpenGL parameters
  initGL(samples);
  
  return true;
} // end initVideo()

void initGL(int samples)
{
  // enable multisampling
  if (samples != 0)
    glEnable(GL_MULTISAMPLE);
  
  // set the background colour
  glClearColor(0.0f, 0.1f, 0.2f, 0.5f);
  
  // set the viewport to fill the window
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  
  // clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // enable alpha blending
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // set things up for our font engine
//  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//  glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
//  glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
  
  // Cull backfaces
//  glCullFace(GL_BACK);
//  glEnable(GL_CULL_FACE);
  
  // Make sure our specular highlights are visible
//  glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
  
//  glEnable(GL_TEXTURE_2D);
  
  // Set up depth testing
  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LEQUAL);
//  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void toggleFullscreen()
{
  SDL_WM_ToggleFullScreen(screen);
}

/* ******************** *
 * Test render function *
 * ******************** */
void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 1, 1);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(50, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.001f, 500.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  
  gluLookAt(0, 0, 0,   // Eye location
            0, 1, 0,     // Target
            0, 0, 1);  // Up
  glTranslatef(0, 5, 0);
  
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  
//  tex.applyTexture();
//  mesh.render(true);
  /*
  glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 0.0f, -1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 0.0f,  1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 0.0f,  1.0f);
  glEnd();
  //*/
//  glDisable(GL_TEXTURE_2D);
  
//  comFillRoundRect(-1, -1, 2, 2, 1.0f, Colour(0.5f, 0.8f, 0.4f));
  comDrawTexturedCube(0, 0, 0.5f, rotation);
  
  glFlush();
  SDL_GL_SwapBuffers();
  
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
} // end render()

int main()
{
  if (!initVideo())
    exit(1);
  
  // *** Load textures here ***
  loadImage();
  comInit();
  
  // ****** Load all needed stuff here ******
//  Mesh::loadWavefrontObjectFile(&mesh, "objects/plane.obj");
  Mesh::loadWavefrontObjectFile(&mesh, "objects/block8.obj");
//  Mesh::loadWavefrontObjectFile(&mesh, "objects/block9.obj");
  
  // ****** Message loop ******
  bool running = true;
  SDL_Event event;
  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      switch (event.type)
      {
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
          {
            running = false;
            break;
          }
          else if (event.key.keysym.sym == SDLK_F11)
            toggleFullscreen();
        case SDL_VIDEORESIZE:
        case SDL_VIDEOEXPOSE:
          render();
          break;
        
        case SDL_QUIT:
          running = false;
          break;
      }
    } // end while(event)
    
    SDL_Delay(10);
    rotation += 1.0f;
    render();
    
  } // end while (running)
  
  return 0;
} // end main()

