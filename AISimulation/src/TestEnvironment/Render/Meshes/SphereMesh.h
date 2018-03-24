#ifndef SPHERE_MESH_H
#define SPHERE_MESH_H

#include "Mesh.h"

class SphereMesh : public Mesh
{
	int latitudes{ 50 };
	int longitudes{ 50 };

public:

	// Init
	void Init() final
	{

	}

	// Render
	void Render() final
	{
		int i, j;
		for (i = 0; i <= latitudes; i++) 
		{
			float lat0 = float(MathGeom::PI) * (-0.5f + (float)(i - 1) / latitudes);
			float z0 = sinf(lat0);
			float zr0 = cosf(lat0);

			float lat1 = float(MathGeom::PI) * (-0.5f + (float)i / latitudes);
			float z1 = sinf(lat1);
			float zr1 = cosf(lat1);

			glBegin(GL_QUAD_STRIP);

			for (j = 0; j <= longitudes; j++)
			{
				float lng = 2 * float(MathGeom::PI) * (float)(j - 1) / longitudes;
				float x = cosf(lng);
				float y = sinf(lng);

				glNormal3f(x * zr0, y * zr0, z0);
				glVertex3f(x * zr0, y * zr0, z0);
				glNormal3f(x * zr1, y * zr1, z1);
				glVertex3f(x * zr1, y * zr1, z1);
			}

			glEnd();
		}
	}
};

#endif // !SPHERE_MESH_H

