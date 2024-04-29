#include "Camera.h"
#include "input.h"

#include<iostream>
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, 1.0f))
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
	//Init();
}

void Camera::Reset() {
	Position = {10,10,10};
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
	//Init();
}
//
//void Camera::Init()
//{
//	lastX = Input::Instance().GetMouseX();
//	lastY = Input::Instance().GetMouseY();
//}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(Zoom), (float)800 / (float)600, 0.1f, 100.0f);;
}

void Camera::ProcessKeyboard(float deltaTime)
{
	Camera_Movement direction = STAY;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_W) == GLFW_PRESS)
		direction = FORWARD;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_S) == GLFW_PRESS)
		direction = BACKWARD;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_A) == GLFW_PRESS)
		direction = LEFT;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_D) == GLFW_PRESS)
		direction = RIGHT;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_SPACE) == GLFW_PRESS)
		direction = RISE;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_C) == GLFW_PRESS)
		direction = SINK;
	if (Input::Instance().IsKeyPressed(GLFW_KEY_B) == GLFW_PRESS)
		Reset();

	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == RISE)
		Position += Up * velocity;
	if (direction == SINK)
		Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float delta, glm::vec3 target, glm::vec3 target_velocity)
{
	//float curX = Input::Instance().GetMouseX();
	//float curY = Input::Instance().GetMouseY();
	Front = glm::normalize(target - Position);
	Position += target_velocity * delta;
	float xoffset = Input::Instance().GetMouseX_move();
	float yoffset = Input::Instance().GetMouseY_move();
	//lastX = curX;
	//lastY = curY;

	xoffset *= -MouseSensitivity * delta;
	yoffset *= MouseSensitivity * delta;
	float distance = glm::length(Position-target);
	distance = distance > 20 ? 20 : distance;
	distance = distance < 10 ? 10 : distance;
	glm::vec3 dir_x = glm::cross(Front, Up);
	glm::vec3 dir_y = glm::cross(Front, -Right);
	if (glm::length(dir_x) > 0.2 ) {
		dir_x = glm::normalize(dir_x);
		dir_y = glm::normalize(dir_y);
		Position += (xoffset * dir_x + yoffset * dir_y);
	}
	Position = distance * glm::normalize(Position - target) + target;
	/*Yaw += xoffset;
	Pitch += yoffset;

	if (ConstrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}*/

	updateCameraVectors();
}

void Camera::ProcessMouseScroll()
{

	//float yoffset = Input::Instance().G();
	/*Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;*/
}

void Camera::Update(float delta, glm::vec3 target, glm::vec3 target_velocity)
{
	ProcessMouseMovement(delta, target, target_velocity);
	//ProcessKeyboard(delta);
	//ProcessMouseScroll();

	//updateCameraVectors();
	//projection = glm::perspective(glm::radians(Zoom), windowRadio, 0.1f, 10000.0f);
	//glm::mat4 view = GetViewMatrix();
	//shader.setUniform("projection", projection);
	//shader.setUniform("view", view);
	//shader.setUniform("viewPos", Position);
}

void Camera::updateCameraVectors()
{
	/*glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);*/
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
