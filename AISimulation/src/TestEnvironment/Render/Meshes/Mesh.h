#ifndef MESH_H
#define MESH_H

class Mesh
{
protected:

	// vbo, vao, ibo 
	GLuint vertexBufferObject{ 0 };
	GLuint indexBufferObject{ 0 };
	GLuint vertexArrayObject{ 0 };

	GLsizei numVBO{ 1 };
	GLsizei numIBO{ 1 };
	GLsizei numVAO{ 1 };

public:
	virtual ~Mesh() { Terminate(); }

	// Init
	virtual void Init() = 0;

	// Render
	virtual void Render() = 0;

protected:

	// Terminate
	void Terminate()
	{
		if (vertexArrayObject)
		{
			glDeleteVertexArrays(numVAO, &vertexArrayObject);
		}
		
		if (indexBufferObject)
		{
			glDeleteBuffers(numIBO, &indexBufferObject);
		}
		
		if (vertexBufferObject)
		{
			glDeleteBuffers(numVBO, &vertexBufferObject);
		}
	}
};

#endif // !MESH_H

