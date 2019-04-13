/// Programmer: Matthew Early
/// Title: The Only Escape... is ESC!
/// Genre: 3D Haunted House Adventure Game

#include "game.h"

using namespace std;

int win_w = 800, win_h = 600;
float lastX = win_w / 2.0f;
float lastY = win_h / 2.0f;
bool firstMouse = true;
//bool fullScreen = true;

int main(int argc, char **argv)
{

  glutInit(&argc, argv);
  //  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  //black background
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  //debug testing screen size function stuff
  //std::cout << "glut screen width(DEFAULT): " << GLUT_SCREEN_WIDTH << std::endl;
  //std::cout << "glut screen height(DEFAULT): " << GLUT_SCREEN_HEIGHT << std::endl;
  //std::cout << "glut screen width: " << glutGet(GLUT_SCREEN_WIDTH) << std::endl;
  //std::cout << "glut screen height: " << glutGet(GLUT_SCREEN_HEIGHT) << std::endl;

  win_w = glutGet(GLUT_SCREEN_WIDTH) / 1.2;
  win_h = glutGet(GLUT_SCREEN_HEIGHT);
  glutInitWindowSize(win_w, win_h);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("THE ONLY ESCAPE... IS ESC!");
  glewInit();
  glutFullScreen();

  Game game;
  game.begin();

  glutMainLoop();

  return (EXIT_SUCCESS);
}
