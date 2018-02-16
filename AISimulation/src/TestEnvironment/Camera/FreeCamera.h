#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

#include "Camera.h"

class FreeCamera : public Camera
{
public:

	FreeCamera() {};
	~FreeCamera() {};

	void Update(float deltaTime) override
	{
		static const float speed = 0.01f;
		static const float angle = 0.0007f;

    if (Input::isKeyPressed(GLFW_KEY_W))
    {
      Move(forward * speed);
    }

    if (Input::isKeyPressed(GLFW_KEY_S))
    {
      Move(forward * -speed);
    }

    if (Input::isKeyPressed(GLFW_KEY_A))
    {
      Move(right * -speed);
    }

    if (Input::isKeyPressed(GLFW_KEY_D))
    {
      Move(right * speed);
    }

    if (Input::isKeyPressed(GLFW_KEY_Q))
    {
      Move(up * speed);
    }

    if (Input::isKeyPressed(GLFW_KEY_E))
    {
      Move(up * -speed);
    }

    if (Input::isKeyPressed(GLFW_KEY_LEFT))
    {
      //Rotate(glm::vec3(0.0f, angle, 0.0f));
      RotateAroundAxis(up, angle);
    }

    if (Input::isKeyPressed(GLFW_KEY_RIGHT))
    {
      //Rotate(glm::vec3(0.0f, -angle, 0.0f));
      RotateAroundAxis(up, -angle);
    }

    if (Input::isKeyPressed(GLFW_KEY_UP))
    {
      //Rotate(glm::vec3(angle, 0.0f, 0.0f));
      RotateAroundAxis(right, angle);
    }

    if (Input::isKeyPressed(GLFW_KEY_DOWN))
    {
      //Rotate(glm::vec3(-angle, 0.0f, 0.0f));
      RotateAroundAxis(right, -angle);
    }
	}

private:

};

#endif // !FREE_CAMERA_H

