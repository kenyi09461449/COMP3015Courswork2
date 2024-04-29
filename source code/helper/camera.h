#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "glslprogram.h"
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    STAY,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    RISE,
    SINK
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 12.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position = {0.0,5.0,10.0};
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed = 30.0;
    float MouseSensitivity= 10.0;
    float Zoom = 60.f;
    bool ConstrainPitch = true;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    void Init();
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();


    void Update(float delta, glm::vec3 target, glm::vec3 target_velocity);
    void Reset();
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
    void ProcessKeyboard(float deltaTime);
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float delta, glm::vec3 target, glm::vec3 target_velocity);
    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll();
private:
    glm::mat4 projection = glm::mat4(1.0);
    
};

#endif
