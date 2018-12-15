#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <math.h>
#include "camera.h"
#include "shader.h"
#include "primitive_objects.h"
#include "sound.h"

extern float lastX, lastY;
extern bool firstMouse;
extern int win_w, win_h;

class Game
{
public:
  Game();
  ~Game();

  unsigned int VAO;
  unsigned int VBOs[2];

  GLuint aPos;  //position of currently being drawn object
  GLuint aColor;  // color of currently being drawn object
  GLuint aColorIndex;

  //  GLuint camera_view;  // camera-view matrix uniform shader variable location
  //  GLuint model_view;  // model-view matrix uniform shader variable location
  //  GLuint projection; // projection matrix uniform shader variable location

  Camera *camera;
  Shader *shader;

  float deltaTime = 0.0f;	// Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame

  void begin();
  void changeGameState();

  void updateDisplay();
  void idle();
  void mouseClick(int btn, int state, int x, int y);
  void clickHoldMotion(int x, int y);
  void reshapeWindow(int w, int h);
  void ASCIIKeyboardDown(unsigned char k, int x, int y);
  void ASCIIKeyboardUp(unsigned char k, int x, int y);
  void keyboardDownSpecial(int k, int x, int y);
  void keyboardUpSpecial(int k, int x, int y);
  void passiveMouseMotion(int x, int y);

private:
  enum STATE { MAIN_MENU, PLAY } State;

  void initAllCallbacks();

  void setupDisplayCallback();
  void setupIdleCallback();
  void setupMouseClickCallback();
  void setupClickHoldCallback();
  void setupReshapeCallback();
  void setupASCIIkbDownCallback();
  void setupASCIIkbUpCallback();
  void setupSpecialkbDownCallback();
  void setupSpecialkbUpCallback();
  void setupPassiveMouseCallback();
};


#endif // GAME_H
