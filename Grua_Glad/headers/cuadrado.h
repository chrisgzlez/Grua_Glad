#pragma once
#include <glfw3.h>

GLfloat vertices_cuadrado[] = {
		/// POSICION				NORMALES		TEXTURAS ///
		-0.5f, -0.5f, 0.5f,	 	0.0f, 0.0f, 1.0f,	 0, 0,
		0.5f, -0.5f, 0.5f, 		0.0f, 0.0f, 1.0f, 	 1, 0,
		0.5f, 0.5f, 0.5f, 		0.0f, 0.0f, 1.0f, 	 1, 1,

		0.5f, 0.5f, 0.5f, 		0.0f, 0.0f, 1.0f, 	 1, 1,
		-0.5f, 0.5f, 0.5f, 		0.0f, 0.0f, 1.0f, 	 0, 1.0,
		-0.5f, -0.5f, 0.5f, 	0.0f, 0.0f, 1.0f, 	 0, 0
};