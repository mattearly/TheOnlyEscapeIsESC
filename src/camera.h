#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "sound.h"
#include "mearly.h"
#include "game_modes.h"
#include "solidzone.h"

static struct Camera_Movement
{
  bool forward = false, back = false, left = false, right = false;
} CamMove;

// Default camera values
constexpr float YAW = -45.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 0.005f;
constexpr float SENSITIVITY = 0.0109f;
constexpr float ZOOM = 45.0f;
constexpr float CAMSTART[3] = {-3.0f, 1.0f, 8.7f};
constexpr float JUMPHEIGHT = CAMSTART[1] + 0.6f;
//constexpr float BOUNDRYSIZE  = 9.9;
constexpr float MAXLOOKANGLE = 35.0f;
constexpr float TIMEBETWEENFOOTSTEPS = 290.0f;

static float timeSinceLastStep = 0;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
  // Camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler Angles
  float Yaw;
  float Pitch;
  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;
  bool onGround;
  bool isFalling;

  // Constructor with vectors - default constructor
  Camera(glm::vec3 position = glm::vec3(CAMSTART[0], CAMSTART[1], CAMSTART[2]), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
                                MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
  {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    onGround = true;
    isFalling = false;
    updateCameraVectors();
  }
  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
  {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
  }

  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  // Processes input received keyboard
  void ProcessKeyboard(float deltaTime)
  {
    bool positionChanged = true;
    int PosX = Position.x;
    int PosY = Position.y;
    int PosZ = Position.z;
    float velocity = MovementSpeed * deltaTime;
    if (CamMove.forward)
    {
      Position += Front * velocity;
    }
    if (CamMove.back)
    {
      Position -= Front * velocity;
    }
    if (CamMove.left)
    {
      Position -= Right * velocity;
    }
    if (CamMove.right)
    {
      Position += Right * velocity;
    }

    // check collision
    if (Position.x > ZONESIZE || Position.z > ZONESIZE ||
        Position.x < -ZONESIZE || Position.z < -ZONESIZE)
    {
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check orange wall
    else if (Position.x > ZONEDX[0] && Position.z < ZONEDZ[0] &&
             Position.x < ZONEDX[1] && Position.z > ZONEDZ[1])
    {
      std::cout << "orange wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check blue wall (and right side of orange wall)
    else if (Position.x > ZONEDX[2] && Position.z < ZONEDZ[0] &&
             Position.x < ZONEDX[3] && Position.z > ZONEDZ[1])
    {
      std::cout << "blue wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check orange door
    else if (!orangeRoomOpen &&
             Position.x > ZONEDX[4] && Position.z < ZONEDZ[0] &&
             Position.x < ZONEDX[5] && Position.z > ZONEDZ[1])
    {
      std::cout << "door orange wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check blue door
    else if (!blueRoomOpen &&
             Position.x > ZONEDX[6] && Position.z < ZONEDZ[0] &&
             Position.x < ZONEDX[7] && Position.z > ZONEDZ[1])
    {
      std::cout << "door blue wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check purple wall
    else if (!purpleRoomOpen &&
             Position.x > ZONEDX[8] && Position.z < ZONEDZ[2] &&
             Position.x < ZONEDX[9] && Position.z > ZONEDZ[3])
    {
      std::cout << "purple wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    //purple room is open
    else if (Position.x > ZONEDX[8] + 1.3 && Position.z < ZONEDZ[2] &&
             Position.x < ZONEDX[9] && Position.z > ZONEDZ[3])
    {
      std::cout << "purple wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check in between orange/blue rooms wall
    else if (Position.x > ZONEDX[10] && Position.z < ZONEDZ[4] &&
             Position.x < ZONEDX[11] && Position.z > ZONEDZ[5])
    {
      std::cout << "in between wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check RED wall
    else if (Position.x > ZONEDX[12] && Position.z < ZONEDZ[6] &&
             Position.x < ZONEDX[13] && Position.z > ZONEDZ[7])
    {
      std::cout << "RED wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }
    // check REDDESTRUCTABLE wall
    else if (!redWallDestroyed &&
             Position.x > ZONEDX[14] && Position.z < ZONEDZ[8] &&
             Position.x < ZONEDX[15] && Position.z > ZONEDZ[9])
    {
      std::cout << "RED DESTRUCTABLE wall block!\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }

    //allow front door passage if it is destoyed
    else if (
        mainDoorHp == 0 && //allow passage through main door if destroyed
        Position.x > -2.0f && Position.z < HOUSESIZE &&
        Position.x < 2.0f && Position.z > HOUSESIZE - 0.4f)
    {
      // dont do anything, just allow it
    }

    // House Walls -- probably the most expensive collision check
    //    thus we are doing it last so it only executes if all the
    //    others pass
    else if (
        Position.x > HOUSESIZE - 0.4f && Position.z < HOUSESIZE && //right wall
            Position.x < HOUSESIZE && Position.z > -HOUSESIZE ||

        Position.x < -HOUSESIZE + 0.4f && Position.z < HOUSESIZE && //left wall
            Position.x > -HOUSESIZE && Position.z > -HOUSESIZE ||

        Position.x < HOUSESIZE && Position.z < -HOUSESIZE + 0.4f && //far wall
            Position.x > -HOUSESIZE && Position.z > -HOUSESIZE ||

        Position.x < HOUSESIZE && Position.z < HOUSESIZE && //front wall
            Position.x > -HOUSESIZE && Position.z > HOUSESIZE - 0.4f)
    {
      std::cout << "house wall block\n";
      if (CamMove.forward)
      {
        Position -= Front * velocity;
      }
      if (CamMove.back)
      {
        Position += Front * velocity;
      }
      if (CamMove.left)
      {
        Position += Right * velocity;
      }
      if (CamMove.right)
      {
        Position -= Right * velocity;
      }
      positionChanged = false;
    }

    if (timeSinceLastStep > TIMEBETWEENFOOTSTEPS)
    {
      if (positionChanged)
      {
        playfootstepsound();
        timeSinceLastStep = 0;
      }
    }
    else
    {
      timeSinceLastStep += deltaTime;
    }

    if (onGround)
    {
      // feet on the floor
      Position.y = CAMSTART[1];
    }
    else if (/*!onGround && */ !isFalling)
    {
      //rising
    }
    else
    {
      //falling
    }
    updateCameraVectors();
  }

  void Jump()
  {
    if (onGround)
    {
      isFalling = false;
      onGround = false;
      //      updateCameraVectors();
      playgruntsound();
    }
  }

  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
  {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
      if (Pitch > MAXLOOKANGLE)
      {
        Pitch = MAXLOOKANGLE;
      }
      if (Pitch < -MAXLOOKANGLE)
      {
        Pitch = -MAXLOOKANGLE;
      }
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
    glutPostRedisplay();
  }

  void Rise(float deltatime)
  {
    if (Position.y <= JUMPHEIGHT)
    {
      Position.y += 0.004 * deltatime;
    }
    else
    {
      isFalling = true;
    }
  }

  void Fall(float deltatime)
  {
    if (Position.y <= CAMSTART[1] + 0.05f)
    {
      Position.y = CAMSTART[1];
      onGround = true;
    }
    else
    {
      Position.y -= 0.004f * deltatime;
    }
    updateCameraVectors();
  }

  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset)
  {
    if (Zoom >= 20.0f && Zoom <= 50.0f)
      Zoom -= yoffset;
    if (Zoom <= 20.0f)
      Zoom = 20.0f;
    if (Zoom >= 50.0f)
      Zoom = 50.0f;
  }

  // Resets all the camera values back to the defaults
  void Reset()
  {
    Position = glm::vec3(CAMSTART[0], CAMSTART[1], CAMSTART[2]);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    Yaw = YAW;
    Pitch = PITCH;
    Front = glm::vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY;
    Zoom = ZOOM;
    updateCameraVectors();
  }

  // attempts to interact with enviornment, triggering game events
  void triggerAreaInteract()
  {

    //OPEN/CLOSE ORANGE ROOM DOOR
    if (Position.x < -0.5f && Position.x > -2.5f &&
        Position.z < -3.0f && Position.z > -7.3f)
    {
      if (!orangeRoomOpen)
      {
        playdoorsound();
        orangeRoomOpen = true;
      }
      else
      {
        playdoorclosesound();
        orangeRoomOpen = false;
      }
    }

    // OPEN/CLOSE BLUE ROOM DOOR
    else if (Position.x > 7.2f && Position.x < 9.9f &&
             Position.z < -3.0f && Position.z > -7.3f)
    {
      if (!blueRoomOpen)
      {
        playdoorsound();
        blueRoomOpen = true;
      }
      else
      {
        playdoorclosesound();
        blueRoomOpen = false;
      }
    }

    // PICK UP TRIANGLE TO OPEN DOOR
    else if (!orangeRoomItemCollected &&
             Position.x < -6.5f && Position.x > -9.9f &&
             Position.z < -6.2 && Position.z > -9.9f)
    {
      playpickupsound();
      //      playdemolitionsound();
      orangeRoomItemCollected = true;
    }

    // BLAST RED DOOR
    else if (!redWallDestroyed && orangeRoomItemCollected &&
             Position.x > 5.5 && Position.x < 9.7f &&
             Position.z < 2.3 && Position.z > -0.4)
    {
      playdestroysound();
      redWallDestroyed = true;
      playdemolitionsound();
    }

    // FLIP SWITCH RED DOOR
    else if (redWallDestroyed && orangeRoomItemCollected &&
             Position.x > 6.9f && Position.x < 9.9f &&
             Position.z < 9.9f && Position.z > 6.9f)
    {
      playswitchsound();
      switchFlipped = !switchFlipped;
      purpleRoomOpen = !purpleRoomOpen;
    }

    // PICKUP AXE
    else if (!AXECollected &&
             Position.x > 5.4f && Position.x < 8.9f &&
             Position.z < -8.0f && Position.z > -9.9f)
    {
      playpickupsound();
      AXECollected = true;
    }

    // SMASH FRONT DOOR
    else if (mainDoorHp != 0 && /*AXECollected &&*/
             Position.x > -2.f && Position.x < 2.f &&
             Position.z < 10.f && Position.z > 6.f)
    {
      if (AXECollected)
      {
        playsmashsound();
        mainDoorHp--;
        if (mainDoorHp == 0)
        {
          playwinsound();
          // playpickupsound();
        }
      }
      else
      {
        playknocksound();
      }
    }
  }

private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors()
  {
    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

#endif // CAMERA_H
