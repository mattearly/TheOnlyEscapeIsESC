#include "game.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "game_modes.h"
#include <stdlib.h>
#include "camera.h"

int MOUSEX_WARP_OFFSET = win_w / 2;  //for keeping mouse centered on first person look
int MOUSEY_WARP_OFFSET = win_h / 2;
extern bool fullScreen;

/* live game functions */
void Game::updateDisplay()
{
  // per frame logic
  float currentFrame = glutGet(GLUT_ELAPSED_TIME);
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;


  // render

  // outside look control
  dayDeltaTime += deltaTime;
  if (dayDeltaTime > 100000.0f) { dayDeltaTime = 0.0f; };
  // switch to representation of the SKY
  if (dayDeltaTime > 90000.0f) {
    glClearColor(0.12f, 0.07f, 0.50f, 1.0f);  //dawn background
    shader->setInt("timeofday", 1);
  } else if (dayDeltaTime > 80000.0f) {
    glClearColor(0.30f, 0.00f, 0.10f, 1.0f);  //early dawn background
    shader->setInt("timeofday", 2);
  } else if (dayDeltaTime > 60000) {
    glClearColor(0.08f, 0.0f, 0.0f, 1.0f);  //late late night background
    shader->setInt("timeofday", 3);
  } else if (dayDeltaTime > 40000.0f) {
    glClearColor(0.15f, 0.10f, 0.10f, 1.0f);  //black background
    shader->setInt("timeofday", 4);
  } else if (dayDeltaTime > 30000.0f) {
    glClearColor(0.25f, 0.20f, 0.30f, 1.0f);  //dark grey background
    shader->setInt("timeofday", 5);
  } else if (dayDeltaTime > 20000.0f) {
    glClearColor(0.20f, 0.10f, 0.52f, 1.0f);  //late evening background
    shader->setInt("timeofday", 6);
  } else if (dayDeltaTime > 10000.0f) {
    glClearColor(0.14f, 0.07f, 0.62f, 1.0f);  //evening background
    shader->setInt("timeofday", 7);
  } else {
    glClearColor(0.07, 0.14f, 0.75f, 1.0f);  // day blue background
    shader->setInt("timeofday", 8);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // pass projection matrix to shader (note that in this case it could change every frame)
  glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom),
                                          (float)win_w / (float)win_h,
                                          0.01f, 300.0f);
  shader->setMat4("projection", projection);

  // camera/view transformation
  glm::mat4 view = camera->GetViewMatrix();
  shader->setMat4("view", view);

  //render box
  glm::mat4 model = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  if (!redWallDestroyed || !orangeRoomItemCollected) {
    shader->setInt("color", 0);  //turns off this mode to start
    //----------DRAW from VBOs[0]---------//
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);  //switch our first vbo
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);  // position attribute
    if (!orangeRoomItemCollected) {
      glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));  // color attribute

      model = {      1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f     };
      model = glm::translate(model, glm::vec3(-9.0f, 0.5f, -9.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, sizeof(coloredTriangle));
      shader->setInt("color", 99);  //black

    } else {
      shader->setInt("color", 98);  //white

    }

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(7.92f, 3.0f, 2.69f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(coloredTriangle));
  }

  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);   //switch to our second vbo
  glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);  //position to 0,0,0

  // --------------- DRAW OUTSIDE GROUND ---------//
  shader->setInt("color", 4);
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(0.0, 0.0f, 0.0));
  model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  //---------collapsing roof ceiling thing-------------//

  if (mainDoorHp > 0) {

    if (orangeRoomItemCollected && redWallDestroyed) {

      tillDeadTime += deltaTime;

      if (collapsingRoof > -0.3f)
//        collapsingRoof = (150000.f / (30000.f - tillDeadTime));
        collapsingRoof -= .00015f * deltaTime;

      if (!deadSoundPlayed && tillDeadTime > 27800.0f) {
        playdeadsound();
        deadSoundPlayed = true;
      }

      if (tillDeadTime > 30000.0f) {
        exit(0);
      }

    }

    if (!deadSoundPlayed) {
      shader->setInt("color", 10);
    } else {
      shader->setInt("color", 3);
    }

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(10.f, 5.0f, 10.f));  //outer
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(9.9f, 5.0f, 9.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(8.9f, 5.0f, 8.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(7.9f, 5.0f, 7.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(6.9f, 5.0f, 6.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(5.9f, 5.0f, 5.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(4.9f, 5.0f, 4.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(3.9f, 5.0f, 3.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(2.9f, 5.0f, 2.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(1.9f, 5.0f, 1.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0, collapsingRoof, 0.0));
    model = glm::scale(model, glm::vec3(0.9f, 5.0f, 0.9f));  //inner
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  }



  //----------DRAW OUR HOUSE BOX---------//
  shader->setInt("color", 2); //FLOOR/CEILING COLOR

  // FLOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(0.0, 0.005f, 0.0));
  model = glm::scale(model, glm::vec3(10.09f, 0.01f, 10.09f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // CEILING
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(0.0, 5.0, 0.0));
  model = glm::scale(model, glm::vec3(10.09f, 0.1f, 10.09f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  //wall colors, next 4
  shader->setInt("color", 6);
  // RIGHT WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(10.0f, 5.0f, 0.0f));
  model = glm::scale(model, glm::vec3(0.1f, 5.0f, 10.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // LEFT WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(-10.0f, 5.0f, 0.0f));
  model = glm::scale(model, glm::vec3(0.1f, 5.0f, 10.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // BEHIND WALL (HAS MAIN HOUSE DOOR ON IT)
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(6.0f, 5.0f, 10.0f)); //left side wall of front door (when looking at it)
  model = glm::scale(model, glm::vec3(4.0f, 5.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(-6.0f, 5.0f, 10.0f));  //right side wall of front door (when looking at it)
  model = glm::scale(model, glm::vec3(4.0f, 5.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(0.0f, 7.0f, 10.0f));  //above front door
  model = glm::scale(model, glm::vec3(2.0f, 3.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // FAR WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(0.0f, 5.0f, -10.0f));
  model = glm::scale(model, glm::vec3(10.1f, 5.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // orange room wall colors
  shader->setInt("color", 9);
  // ORANGE ROOM FONT WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(-6.20, 5.0f, -5.0f));
  model = glm::scale(model, glm::vec3(3.85f, 5.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // ABOVE ORANGE ROOM DOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(-1.26f, 6.08f, -5.0f));
  model = glm::scale(model, glm::vec3(1.16f, 3.83f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));


  // RIGHT OF ORANGE ROOM DOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(-0.425f, 0.75f, -5.0f));
  model = glm::scale(model, glm::vec3(.325f, 1.5f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // ORANGE/BLUE/PURPLE seperator wall
  shader->setInt("color", 6);
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(0.0f, 5.0f, -7.5f));
  model = glm::scale(model, glm::vec3(0.1f, 5.0f, 2.6f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // PURPLE ROOM
  // PURPLE room wall colors
  shader->setInt("color", 7);
  // PURPLE ROOM FONT WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  if (!purpleRoomOpen) {
    model = glm::translate(model, glm::vec3(5.0f, 5.0f, -8.1f));
  } else {
    model = glm::translate(model, glm::vec3(6.5f, 5.0f, -8.1f));
  }
  model = glm::scale(model, glm::vec3(5.0f, 5.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // BLUE ROOM
  // SET BLUE COLOR
  shader->setInt("color", 5);
  // FRONT BLUE ROOM WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(4.0f, 5.0f, -5.00f));
  model = glm::scale(model, glm::vec3(3.9f, 5.0f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  // ABOVE BLUE ROOM DOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(8.89f, 6.08f, -5.0f));
  model = glm::scale(model, glm::vec3(1.0f, 3.83f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  // RIGHT OF BLUE ROOM DOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(9.75f, 0.75f, -5.0f));
  model = glm::scale(model, glm::vec3(.25f, 1.5f, 0.1f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // RED ROOM
  // red colors
  shader->setInt("color", 3);
  // RED ROOM FONT WALL
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(6.00F, 5.0f, 6.4f));
  model = glm::scale(model, glm::vec3(0.1f, 5.0f, 3.5f));
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));


  // RED WALL destroyable wall
  if (!redWallDestroyed) {
    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(7.92f, 5.0f, 2.8f));
    model = glm::scale(model, glm::vec3(2.0f, 5.0f, 0.1f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  }

  shader->setInt("color", 1);  //DOOR COLOR

  // ORANGE ROOM DOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  if (!orangeRoomOpen) {
    model = glm::translate(model, glm::vec3(-1.55f, 0.75f, -5.0f));
    model = glm::scale(model, glm::vec3(0.8f, 1.5f, 0.1f));
  } else { //isopen
    //    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  //decided to just translate and scale
    model = glm::translate(model, glm::vec3(-2.45f, 0.75f, -5.9f));
    model = glm::scale(model, glm::vec3(0.1f, 1.5f, 0.8f));
  }
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // BLUE ROOM DOOR
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  if (!blueRoomOpen) {
    model = glm::translate(model, glm::vec3(8.7f, 0.75f, -5.0f));
    model = glm::scale(model, glm::vec3(0.8f, 1.5f, 0.1f));

  } else { //isopen
    //    model = glm::translate(model, glm::vec3(-2.55f, 0.75f, -5.9f));
    //    model = glm::scale(model, glm::vec3(0.1f, 1.5f, 0.8f))
    //    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(7.8f, 0.75f, -5.9f));
    model = glm::scale(model, glm::vec3(0.1f, 1.5f, 0.8f));
  }
  shader->setMat4("model", model);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // FRONT DOOR
  if (mainDoorHp != 0) {
    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0f, 2.0f, 10.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.1f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  } else {
    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(0.0f, .0f, 12.0f));
    model = glm::scale(model, glm::vec3(2.0f, 0.1f, 2.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  }

  // PURPLE ROOM SWITCH FLIPPER
  model = {      1.0f, 0.0f, 0.0f, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                 0.0f, 0.0f, 1.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, 1.0f     };
  model = glm::translate(model, glm::vec3(9.0f, 1.2f, 9.7f));
  model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.3f));
  if (switchFlipped) {
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  }
  shader->setMat4("model", model);
  shader->setInt("color", 7);
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));

  // AXE
  if (!AXECollected) {
    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(7.52f, 0.1f, -9.55f));
    model = glm::scale(model, glm::vec3(0.06f, 0.01f, 0.06f));
    shader->setMat4("model", model);
    shader->setInt("color", 10);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));  //axehead
    model = {      1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f     };
    model = glm::translate(model, glm::vec3(7.34f, 0.1f, -9.44f));
    model = glm::scale(model, glm::vec3(0.25f, 0.01f, 0.05f));
    shader->setMat4("model", model);
    shader->setInt("color", 1);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));  //axehandle
  }

  glutSwapBuffers();
}

void Game::idle(){
  if (!camera->onGround && !camera->isFalling) { //then rising
    camera->Rise(deltaTime);
  } else if (!camera->onGround) {
    camera->Fall(deltaTime);
  }
  if (CamMove.forward || CamMove.back || CamMove.left || CamMove.right){
    camera->ProcessKeyboard(deltaTime);
  }
  glutPostRedisplay();
}

void Game::mouseClick(int btn, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    switch(btn) {
    case GLUT_LEFT_BUTTON:
      camera->triggerAreaInteract();
      //      std::cout << "left click at: (" << x << ", " << y << ")\n";
      break;
    case GLUT_RIGHT_BUTTON:
      //      std::cout << "right click at: (" << x << ", " << y << ")\n";
      break;
    case GLUT_MIDDLE_BUTTON:
      //      std::cout << "middle click at: (" << x << ", " << y << ")\n";
      break;
    case 3:  //mouse wheel scrolls
      camera->ProcessMouseScroll(1.0f);
      break;
    case 4:
      camera->ProcessMouseScroll(-1.0f);
      break;
    default:
      break;
    }
  }
  glutPostRedisplay();
}

void Game::clickHoldMotion(int x, int y)
{
  std::cout << "drag at: (" << x << ", " << y << ")\n";
  glutPostRedisplay();
}

void Game::reshapeWindow(int w, int h)
{
  glViewport(0,0,w,h);
  win_w = w;
  win_h = h;
  MOUSEX_WARP_OFFSET = win_w / 2.4;  //for keeping mouse centered on first person movement
  MOUSEY_WARP_OFFSET = win_h / 2.4;
  glutWarpPointer(win_w/2, win_h/2);
  glutPostRedisplay();
}

void Game::ASCIIKeyboardDown(unsigned char k, int x, int y)
{
  switch (k) {
  case 033:
    exit(0);
  case 'w':
  case 'W':
    CamMove.forward = true;
    break;
  case 's':
  case 'S':
    CamMove.back = true;
    break;
  case 'a':
  case 'A':
    CamMove.left = true;
    break;
  case 'd':
  case 'D':
    CamMove.right = true;
    break;
  case 'q':
    break;
  case 'e':
  case 'E':
    camera->triggerAreaInteract();
    break;
  case ' ':
    camera->Jump();
    break;
  case 'l':
    camera->Reset();
    break;
    //  case '+':
    //    if (!fullScreen) {
    //      glutFullScreen();
    //      fullScreen = !fullScreen;
    //    }
    //    //    reshapeWindow(win_w, win_h);
    //  case '-':
    //    if (fullScreen) {
    //      glutPositionWindow(0,0);
    //      win_w = glutGet(GLUT_SCREEN_WIDTH);
    //      win_h = glutGet(GLUT_SCREEN_WIDTH);
    //      reshapeWindow(win_w, win_h);
    //      fullScreen = !fullScreen;
    //    } else {
    //      win_w -= 9;
    //      win_h -= 6;
    //      reshapeWindow(win_w, win_h);
    //    }
  default:
    break;
  }
  std::cout << "@worldloc: "
            << camera->Position.x << ", "
            << camera->Position.y << ", "
            << camera->Position.z << "\n";
  glutPostRedisplay();
}

void Game::ASCIIKeyboardUp(unsigned char k, int x, int y)
{
  switch (k) {
  case 'w':
  case 'W':
    CamMove.forward = false;
    break;
  case 's':
  case 'S':
    CamMove.back = false;
    break;
  case 'a':
  case 'A':
    CamMove.left = false;
    break;
  case 'd':
  case 'D':
    CamMove.right = false;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void Game::keyboardDownSpecial(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    break;
  case GLUT_KEY_DOWN:
    break;
  case GLUT_KEY_LEFT:
    break;
  case GLUT_KEY_RIGHT:
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void Game::keyboardUpSpecial(int k, int x, int y)
{
  switch (k) {
  case GLUT_KEY_UP:
    break;
  case GLUT_KEY_DOWN:
    break;
  case GLUT_KEY_LEFT:
    break;
  case GLUT_KEY_RIGHT:
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void Game::passiveMouseMotion(int x, int y)
{
  if (firstMouse) {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  float xoffset = x - lastX;
  float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top

  lastX = x;
  lastY = y;

  camera->ProcessMouseMovement(xoffset, yoffset);

  glutPostRedisplay();

  // this works most of the time rather seemlessly, except when running through cygwin startxwin func, works but hiccups
  if ( x < MOUSEX_WARP_OFFSET || x > win_w - MOUSEX_WARP_OFFSET) {  //center the mouse back based on window size
    lastX = win_w/2;
    lastY = win_h/2;
    glutWarpPointer(win_w/2, win_h/2);
  } else if (y < MOUSEY_WARP_OFFSET || y > win_h - MOUSEY_WARP_OFFSET) {  //center the mouse back based on window size
    lastX = win_w/2;
    lastY = win_h/2;
    glutWarpPointer(win_w/2, win_h/2);
  }
}
