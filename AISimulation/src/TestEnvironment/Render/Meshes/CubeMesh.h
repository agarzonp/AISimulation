#ifndef CUBE_MESH_H
#define CUBE_MESH_H

#include "Mesh.h"

class CubeMesh : public Mesh
{
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

public:

	void Init() final
	{
		// create numVBO buffers in the GPU, use it as an array buffer and set the data
		glGenBuffers(numVBO, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// create numIBO buffer in the GPU, use it as an element array buffer and set the data
		glGenBuffers(numIBO, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// create one vertex array object for drawing and use it. This vertexArrayObject is the one who will deal with the vertex buffer
		glGenVertexArrays(numVAO, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		// Tell the vertex shader how to interpret the buffer data. This information is needed for the active vertexArrayObject
		// The 0 attribute(pos) has 3 elements (x,y,z) of type GL_FLOAT
		// The stride to the next 0 attribute is zero bytes because there are no other attributes in between
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Enable the 0 attribute
		glEnableVertexAttribArray(0);
	}

	// Render
	void Render() final
	{
		// tell the vertexArrayObject to be used
		glBindVertexArray(vertexArrayObject);

		// tell to draw cube by using the IBO
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)indices);

		// do not use the vertexArrayObject anymore
		glBindVertexArray(0);
	}
};
#endif // !CUBE_MESH_H

