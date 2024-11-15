#pragma once
class Camera
{
public:
	Camera() = default;
	virtual ~Camera() = default;

	virtual void MouseCallback(double xPos, double yPos) = 0;
	virtual void KeyDown(int glfwKey) = 0;
	virtual void KeyUp(int glfwKey) = 0;
	virtual void ProcessInput(GLFWwindow* window) = 0;
	virtual void Update(float deltaTime) = 0;

	virtual void GetCameraProperties(glm::vec3& pos, glm::vec3& front, glm::vec3& up) const = 0;
	virtual float GetFov() const = 0;
};
