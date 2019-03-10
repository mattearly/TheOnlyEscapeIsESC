#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "filesystem.h"
#include <SDL2/SDL.h>
#include "sound.h"

using namespace std;


Game::Game()
{
  State = PLAY;
  camera = new Camera();
  shader = new Shader("shaders/vshader.glsl", "shaders/fshader.glsl");
}

Game::~Game()
{
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  //  glDeleteVertexArrays(2, &VAO);  // doing this doesn't seem to matter in our case
  //  glDeleteBuffers(2, &VBOs[0]);
  //  glDeleteVertexArrays(1, &VAOs[1]);
  //  glDeleteBuffers(1, &VBOs[0]);
}

void Game::begin()
{
  // for updating for different objects and points on the fly
  aPos = glGetAttribLocation(shader->ID, "aPos");
  aColor = glGetAttribLocation(shader->ID, "aColor");

  glEnableVertexAttribArray(aPos);   //already enabled
  glEnableVertexAttribArray(aColor);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(2, &VBOs[0]);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(coloredTriangle), coloredTriangle, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(aPos);
  glEnableVertexAttribArray(aColor);
  // tell openGL to use this shader
  shader->use();

  glEnable(GL_DEPTH_TEST);

  changeGameState();

  glutSetCursor(GLUT_CURSOR_NONE);

  // playdefaultmusic();
}

void Game::changeGameState()
{
  switch (State) {
  case MAIN_MENU:
    break;
  case PLAY:
    initAllCallbacks();
    break;
  }
}

void Game::initAllCallbacks()
{
  setupDisplayCallback();
  setupIdleCallback();
  setupMouseClickCallback();
  setupClickHoldCallback();
  setupReshapeCallback();
  setupASCIIkbDownCallback();
  setupASCIIkbUpCallback();
  setupSpecialkbDownCallback();
  setupSpecialkbUpCallback();
  setupPassiveMouseCallback();
  // playrunawaysound();
  playRunAwayVoice();
}


///
/// \brief g_CurrentInstance
///  for linking to callbacks from within the class
static Game * g_CurrentInstance;

extern "C" void displayCallback() {
  g_CurrentInstance->updateDisplay();
}

extern "C" void idleCallback() {
  g_CurrentInstance->idle();
}

extern "C" void clickCallback(int btn, int state, int x, int y) {
  g_CurrentInstance->mouseClick(btn, state, x, y);
}

extern "C" void clickHoldCallback(int x, int y) {
  g_CurrentInstance->clickHoldMotion(x, y);
}

extern "C" void reshapeCallback(int w, int h) {
  g_CurrentInstance->reshapeWindow(w, h);
}

extern "C" void keyDownCallback(unsigned char k, int x, int y) {
  g_CurrentInstance->ASCIIKeyboardDown(k, x, y);
}

extern "C" void keyUpCallback(unsigned char k, int x, int y) {
  g_CurrentInstance->ASCIIKeyboardUp(k, x, y);
}

extern "C" void specialDownCallback(int k, int x, int y) {
  g_CurrentInstance->keyboardDownSpecial(k, x, y);
}

extern "C" void specialUpCallback(int k, int x, int y) {
  g_CurrentInstance->keyboardUpSpecial(k, x, y);
}

extern "C" void passiveMouseCallback(int x, int y) {
  g_CurrentInstance->passiveMouseMotion(x, y);
}

void Game::setupDisplayCallback() {
  ::g_CurrentInstance = this;
  ::glutDisplayFunc(::displayCallback);
}

void Game::setupIdleCallback() {
  ::g_CurrentInstance = this;
  ::glutIdleFunc(::idleCallback);
}

void Game::setupMouseClickCallback() {
  ::g_CurrentInstance = this;
  ::glutMouseFunc(::clickCallback);
}

void Game::setupClickHoldCallback() {
  ::g_CurrentInstance = this;
  ::glutMotionFunc(::clickHoldCallback);
}

void Game::setupReshapeCallback() {
  ::g_CurrentInstance = this;
  ::glutReshapeFunc(::reshapeCallback);
}

void Game::setupASCIIkbDownCallback() {
  ::g_CurrentInstance = this;
  ::glutKeyboardFunc(::keyDownCallback);
}

void Game::setupASCIIkbUpCallback() {
  ::g_CurrentInstance = this;
  ::glutKeyboardUpFunc(::keyUpCallback);
}

void Game::setupSpecialkbDownCallback()
{
  ::g_CurrentInstance = this;
  ::glutSpecialFunc(::specialDownCallback);
}

void Game::setupSpecialkbUpCallback() {
  ::g_CurrentInstance = this;
  ::glutSpecialUpFunc(::specialUpCallback);
}

void Game::setupPassiveMouseCallback() {
  ::g_CurrentInstance = this;
  ::glutPassiveMotionFunc(::passiveMouseCallback);
}


