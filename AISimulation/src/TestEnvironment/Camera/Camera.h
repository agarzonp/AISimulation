#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

class Camera
{
public:
	virtual ~Camera() {}
	virtual void OnMouseMove(float x, float y){}
	virtual void Update(float deltaTime){}

	void Init(const glm::vec3& pos, const glm::vec3& focusPos, float fov, float aspect, float zNear, float zFar)
	{
		this->pos = pos;
		this->focusPos = focusPos;
		this->fov = fov;
		this->aspect = aspect;
		this->zNear = zNear;
		this->zFar = zFar;

		Rotate(eulerAngles);
	}
	
	glm::mat4 ViewProjectionMatrix() const
	{
		glm::mat4 view = glm::mat4();
		
		view = glm::lookAt(pos, focusPos, up);

		glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
		return projection * view;
	}

	void Move(const glm::vec3& offset)
	{
		pos += offset;

		UpdateCameraVectors();
	}

	void MoveTo(const glm::vec3& position)
	{
		pos = position;

		UpdateCameraVectors();
	}

	void Rotate(const glm::vec3& angles)
	{
		this->eulerAngles += angles;

		// set the forward vector according to eulerAngles(spherical to cartesian coordinates)
		forward.x = cosf(eulerAngles.x) * sinf(eulerAngles.y);
		forward.y = sinf(eulerAngles.x);
		forward.z = cosf(eulerAngles.x) * cosf(eulerAngles.y);

		UpdateCameraVectors();
	}

	void RotateAroundAxis(const glm::vec3& axis, float angle)
	{
		// quaternion that defines a rotation around axis
		glm::quat rotQuaternion = glm::angleAxis(angle, axis);
		
		// forward quaternion
		glm::quat forwardQuaternion = glm::quat(0, forward.x, forward.y, forward.z);

		// rotate 
		glm::quat newForwardQuaternion = (rotQuaternion * forwardQuaternion) * glm::conjugate(rotQuaternion);
		
		// set the new forward vector
		forward.x = newForwardQuaternion.x;
		forward.y = newForwardQuaternion.y;
		forward.z = newForwardQuaternion.z;

		UpdateCameraVectors();
	}

	const glm::vec3& GetPosition()const  { return pos; }
	const glm::vec3& GetForward() const{ return forward; }
	const glm::vec3& GetUp() const { return up; }

protected:
	Camera() {}

	virtual void UpdateCameraVectors()
	{
		// normalize forward vector
		forward = glm::normalize(forward);

		// set the right vector by crossing the forward with the Y axis
		right = glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));

		// set the up vector by crossing the forward and right vector
		up = glm::cross(right, forward);

		// set the new focus position
		focusPos = pos + forward;
	}

	float fov = 45.0f; // in degrees
	float aspect = 1.33f;
	float zNear = 0.1f;
	float zFar = 1000.0f;

	glm::vec3 pos;
	glm::vec3 focusPos;

	// camera up, forward, right vectors
	glm::vec3 up;
	glm::vec3 forward;
	glm::vec3 right;

	// rotation by euler angles
	glm::vec3 eulerAngles;
};

#endif