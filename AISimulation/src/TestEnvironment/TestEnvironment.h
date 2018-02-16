#ifndef TEST_ENVIRONMENT_H
#define TEST_ENVIRONMENT_H

#include <cassert>
#include <memory>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../Input/Input.h"
#include "../Shaders/Shader.h"
#include "Camera/FreeCamera.h"

class TestEnvironment : public InputListener
{

public:
  TestEnvironment()
	{
		Init();
	}

	~TestEnvironment()
	{
		Terminate();
	}

	void OnKeyPressed(int key) override 
	{
	}

	void OnKeyReleased(int key) override { }
	void OnMouseButtonPressed(int button, double x, double y) override { }
	void OnMouseButtonReleased(int button, double x, double y) override { }
	void OnMouseScroll(double xoffset, double yoffset) override { }
	
	void OnMouseMove(double x, double y) { }

	void Update(float deltaTime) 
	{
    camera.Update(deltaTime);
	}

	void Render() 
	{
		DrawCubes();
	}
	
protected:

	void Init()
	{
		// init the vertex buffer object
		InitVBO();

		// init the indeces buffer object
		InitIBO();

		// init the vertex array object
		InitVAO();

		// load shader
		shader.Load("assets/Shaders/basic.vert", "assets/Shaders/basic.frag");

		// init cubes
		InitCubes();

    // init camera
    camera.Init(glm::vec3(0.0f, 1.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, 1024.0f / 768.0f, 0.1f, 1000000.0f);
	}
		
	void InitVBO()
	{
		// create one buffer in the GPU, use it as an array buffer and set the data
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	}

	void InitIBO()
	{
		// create one buffer in the GPU, use it as an element array buffer and set the data
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	void InitVAO()
	{
		// create one vertex array object for drawing and use it. This vertexArrayObject is the one who will deal with the vertex buffer
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		// Tell the vertex shader how to interpret the buffer data. This information is needed for the active vertexArrayObject
		// The 0 attribute(pos) has 3 elements (x,y,z) of type GL_FLOAT
		// The stride to the next 0 attribute is zero bytes because there are no other attributes in between
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Enable the 0 attribute
		glEnableVertexAttribArray(0);
	}

	void InitCubes()
	{
		// floor
		cubes[0].pos = glm::vec3(0.0f, 0.0f, 10.0f);
		cubes[0].scale = glm::vec3(40.0f, 0.0001f, 40.f);
		cubes[0].color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		cubes[0].enabled = true;

		// walls
		cubes[1].pos = glm::vec3(40.0f, 5.0f, 10.0f);
		cubes[1].scale = glm::vec3(0.5f, 5.0f, 40.f);
		cubes[1].color = glm::vec4(0.11f, 0.11f, 0.11f, 1.0f);
		cubes[1].enabled = true;

		cubes[2].pos = glm::vec3(-40.0f, 5.0f, 10.0f);
		cubes[2].scale = glm::vec3(0.5f, 5.0f, 40.f);
		cubes[2].color = glm::vec4(0.11f, 0.11f, 0.11f, 1.0f);
		cubes[2].enabled = true;

		cubes[3].pos = glm::vec3(0.0f, 5.0f, 50.0f);
		cubes[3].scale = glm::vec3(40.0f, 5.0f, 0.5f);
		cubes[3].color = glm::vec4(0.11f, 0.11f, 0.11f, 1.0f);
		cubes[3].enabled = true;

		cubes[4].pos = glm::vec3(0.0f, 5.0f, -30.0f);
		cubes[4].scale = glm::vec3(40.0f, 5.0f, 0.5f);
		cubes[4].color = glm::vec4(0.11f, 0.11f, 0.11f, 1.0f);
		cubes[4].enabled = true;

		// red cubes
		cubes[5].pos = glm::vec3(-20.0f, 2.5f, -18.0f);
		cubes[5].rotation = glm::vec3(0.5f, 0.5f, 0.0f);
		cubes[5].scale = glm::vec3(2.5f, 2.5f, 2.5f);
		cubes[5].color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		cubes[5].enabled = true;

		cubes[6].pos = glm::vec3(25.0f, 2.5f, 40.0f);
		cubes[6].rotation = glm::vec3(0.5f, 0.5f, 0.0f);
		cubes[6].scale = glm::vec3(2.5f, 2.5f, 2.5f);
		cubes[6].color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		cubes[6].enabled = true;

		// blue tower
		cubes[7].pos = glm::vec3(-10.0f, 3.5f, 20.0f);
		cubes[7].rotation = glm::vec3(0.0f, 0.5f, 0.0f);
		cubes[7].scale = glm::vec3(1.0f, 3.5f, 1.0f);
		cubes[7].color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
		cubes[7].enabled = true;

		cubes[8].pos = glm::vec3(-15.0f, 11.5f, 22.7f);
		cubes[8].rotation = glm::vec3(0.0f, 0.5f, 0.0f);
		cubes[8].scale = glm::vec3(1.0f, 5.0f, 1.0f);
		cubes[8].color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
		cubes[8].enabled = true;

		cubes[9].pos = glm::vec3(-5.0f, 11.5f, 17.3f);
		cubes[9].rotation = glm::vec3(0.0f, 0.5f, 0.0f);
		cubes[9].scale = glm::vec3(1.0f, 5.0f, 1.0f);
		cubes[9].color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
		cubes[9].enabled = true;

		cubes[10].pos = glm::vec3(-10.0f, 7.5f, 20.0f);
		cubes[10].rotation = glm::vec3(0.0f, 0.5f, 0.0f);
		cubes[10].scale = glm::vec3(5.0f, 1.0f, 1.0f);
		cubes[10].color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
		cubes[10].enabled = true;

		cubes[11].pos = glm::vec3(-10.0f, 15.5f, 20.0f);
		cubes[11].rotation = glm::vec3(0.0f, 0.5f, 0.0f);
		cubes[11].scale = glm::vec3(5.0f, 1.0f, 1.0f);
		cubes[11].color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
		cubes[11].enabled = true;

		// green walls

		cubes[12].pos = glm::vec3(25.0f, 1.0f, 0.0f);
		cubes[12].rotation = glm::vec3(0.0f, -1.23f, 0.0f);
		cubes[12].scale = glm::vec3(5.0f, 1.0f, 0.25f);
		cubes[12].color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
		cubes[12].enabled = true;

		cubes[13].pos = glm::vec3(30.0f, 1.0f, -10.0f);
		cubes[13].rotation = glm::vec3(0.0f, -1.23f, 0.0f);
		cubes[13].scale = glm::vec3(5.0f, 1.0f, 0.25f);
		cubes[13].color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
		cubes[13].enabled = true;
	}

	void DrawCubes()
	{
		const glm::mat4& viewProjection = camera.ViewProjectionMatrix();

		// use the shader
		shader.Use();	

		// tell the vertexArrayObject to be used
		glBindVertexArray(vertexArrayObject);

		glm::mat4 model;
		for (int i = 0; i < NUM_CUBES; i++)
		{
			Cube cube = cubes[i];
			if (cube.enabled)
			{
				model = glm::mat4();
				model = glm::translate(model, cube.pos) 
					  * glm::rotate(model, cube.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) 
					  * glm::rotate(model, cube.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) 
					  * glm::rotate(model, cube.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
					  * glm::scale(model, cube.scale);
				shader.SetUniform("modelViewProjection", viewProjection * model);
				shader.SetUniform("color", cube.color);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)indices); // tell to draw cube by using the IBO
			}
		}

		// do not use the vertexArrayObject anymore
		glBindVertexArray(0);
	}

	void Terminate()
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
		glDeleteBuffers(1, &vertexBufferObject);
		glDeleteBuffers(1, &indexBufferObject);
	}

private:

	// vertices of the cube
	GLfloat vertices[24] =
	{
		-1.0f,	1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,	1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};


	// indices to cube vertices
	GLuint indices[36] =
	{
	   // tri 1      tri 2
		0, 2, 1,	0, 3, 2,	// front face
		4, 6, 5,    4, 7, 6,	// back face	
		4, 3, 0,    4, 7, 3,	// left face	
		1, 6, 5,    1, 2, 6,	// right face	
		4, 1, 5,    4, 0, 1,	// top face	
		7, 2, 6,    7, 3, 2		// bottom face	
	};
	
	// vbo, vao, ibo 
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	GLuint vertexArrayObject; 
	
	// shader
	Shader shader;

	// cubes
	struct Cube
	{
		bool enabled = false;

		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	};

	static const int NUM_CUBES = 64;
	Cube cubes[NUM_CUBES];

  // Camera
  FreeCamera camera;
};

#endif