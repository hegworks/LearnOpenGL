﻿#pragma once

#include "../Common.h"
#include "Camera.h"

class FPSCamera : public Camera
{
public:
	FPSCamera();
	~FPSCamera();

	virtual void MouseCallback(double xPos, double yPos);
	virtual void ProcessInput(GLFWwindow* window);
	virtual void Update(float deltaTime);
	virtual void KeyDown(int glfwKey);
	virtual void KeyUp(int glfwKey);

	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const;
	virtual float GetFov() const { return m_fov; }

private:
	//------settings
	const float YAW_DEFAULT = -90.0f;
	const float FOV_DEFAULT = 45.0f;
	const float SPEED_DEFAULT = 10.0f;
	const float SPEED_BOOSTED = 20.0f;
	const glm::vec3 POS_INITIAL = glm::vec3(0.0f, 2.0f, 0.0f);
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 FRONT_INITIAL = glm::vec3(0.0f, 1.0f, 0.0f);
	const float PITCH_MAX = 89.0f;
	const float MOUSE_SENSITIVITY = 0.05f;

	const int UP_KEY = GLFW_KEY_W;
	const int DOWN_KEY = GLFW_KEY_S;
	const int RIGHT_KEY = GLFW_KEY_D;
	const int LEFT_KEY = GLFW_KEY_A;
	const int ASCEND_KEY = GLFW_KEY_Q;
	const int DESCEND_KEY = GLFW_KEY_E;
	const int SPEED_BOOST_KEY = GLFW_KEY_LEFT_SHIFT;
	static constexpr int JUMP_KEY = GLFW_KEY_SPACE;

	// jump
	const float JUMP_HEIGHT = 5.0f;
	const float GRAVITY = -9.8f;
	const int MULTI_JUMP_COUNT = 2;
   //=====settings


   //-----variables
	double m_yaw = YAW_DEFAULT;
	double m_pitch = 0;
	float m_fov = FOV_DEFAULT;

	bool m_isFirstMouse = true;
	double m_lastX = SCRWIDTH / 2.0;
	double m_lastY = SCRHEIGHT / 2.0;

	glm::vec3 m_pos = POS_INITIAL;
	glm::vec3 m_front = FRONT_INITIAL;

	bool m_isUpKeyPressed = false;
	bool m_isDownKeyPressed = false;
	bool m_isLeftKeyPressed = false;
	bool m_isRightKeyPressed = false;
	bool m_isBoostKeyPressed = false;
	bool m_isAscendKeyPressed = false;
	bool m_isDescendKeyPressed = false;

	// jump
	bool m_isJumpKeyPressed = false;
	bool m_isGrounded = true;
	float m_velocityY = 0.0f;
	int m_multiJumpsJumped = 0;
	//======variables
};

inline FPSCamera::FPSCamera()
{
}

inline FPSCamera::~FPSCamera()
{
}

inline void FPSCamera::MouseCallback(double xPos, double yPos)
{
	if(m_isFirstMouse) // initially set to true
	{
		m_lastX = xPos;
		m_lastY = yPos;
		m_isFirstMouse = false;
	}

	double xOffset = xPos - m_lastX;
	double yOffset = m_lastY - yPos; // reversed since y-coordinates range from bottom to top
	m_lastX = xPos;
	m_lastY = yPos;

	xOffset *= MOUSE_SENSITIVITY;
	yOffset *= MOUSE_SENSITIVITY;

	m_yaw += xOffset;
	m_pitch += yOffset;

	if(m_pitch > PITCH_MAX)
		m_pitch = PITCH_MAX;
	if(m_pitch < -PITCH_MAX)
		m_pitch = -PITCH_MAX;
}

inline void FPSCamera::ProcessInput(GLFWwindow* window)
{
	m_isUpKeyPressed = glfwGetKey(window, UP_KEY) == GLFW_PRESS;
	m_isDownKeyPressed = glfwGetKey(window, DOWN_KEY) == GLFW_PRESS;
	m_isLeftKeyPressed = glfwGetKey(window, LEFT_KEY) == GLFW_PRESS;
	m_isRightKeyPressed = glfwGetKey(window, RIGHT_KEY) == GLFW_PRESS;
	m_isAscendKeyPressed = glfwGetKey(window, ASCEND_KEY) == GLFW_PRESS;
	m_isDescendKeyPressed = glfwGetKey(window, DESCEND_KEY) == GLFW_PRESS;
	m_isBoostKeyPressed = glfwGetKey(window, SPEED_BOOST_KEY) == GLFW_PRESS;
}

inline void FPSCamera::KeyDown(int glfwKey)
{
	switch(glfwKey)
	{
		case JUMP_KEY:
			m_isJumpKeyPressed = true;
			break;
		default:
			break;
	}
}

inline void FPSCamera::KeyUp(int glfwKey)
{
	switch(glfwKey)
	{
		case JUMP_KEY:
			m_isJumpKeyPressed = false;
			break;
		default:
			break;
	}
}

inline void FPSCamera::Update(float deltaTime)
{
	const float cameraSpeed = m_isBoostKeyPressed ? SPEED_BOOSTED : SPEED_DEFAULT;
	if(m_isUpKeyPressed)
		m_pos += cameraSpeed * glm::normalize(glm::vec3(m_front.x, 0, m_front.z)) * deltaTime;
	if(m_isDownKeyPressed)
		m_pos -= cameraSpeed * glm::normalize(glm::vec3(m_front.x, 0, m_front.z)) * deltaTime;
	if(m_isLeftKeyPressed)
		m_pos -= glm::normalize(glm::cross(m_front, UP)) * cameraSpeed * deltaTime;
	if(m_isRightKeyPressed)
		m_pos += glm::normalize(glm::cross(m_front, UP)) * cameraSpeed * deltaTime;


	//-----jump
	if(m_isJumpKeyPressed && (m_isGrounded || m_multiJumpsJumped < MULTI_JUMP_COUNT))
	{
		m_velocityY = JUMP_HEIGHT;
		m_isJumpKeyPressed = false;
		m_isGrounded = false;
		m_multiJumpsJumped++;
	}
	// gravity
	m_velocityY += GRAVITY * deltaTime;

	// apply velocity
	m_pos.y += m_velocityY * deltaTime;

	// check ground
	if(m_pos.y < POS_INITIAL.y)
	{
		m_pos.y = POS_INITIAL.y;
		m_isGrounded = true;
		m_multiJumpsJumped = 0;
	}
	//=====jump

	glm::vec3 direction;
	const float yawF = static_cast<float>(m_yaw);
	const float pitchF = static_cast<float>(m_pitch);
	direction.x = cos(glm::radians(yawF)) * cos(glm::radians(pitchF));
	direction.y = sin(glm::radians(pitchF));
	direction.z = sin(glm::radians(yawF)) * cos(glm::radians(pitchF));
	m_front = glm::normalize(direction);
}

inline void FPSCamera::GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const
{
	pos = m_pos;
	front = m_front;
	up = UP;
}
