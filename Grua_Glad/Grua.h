#ifndef GRUA_h
#define GRUA_H

#define _USE_MATH_DEFINES
#include <cmath>

#include <glad.h>
#include <glfw3.h>

/// Matriz de Transformacion
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Parte.h>

class Grua {
private:



	void _builder(glm::mat4* base, objeto obj, GLuint matrix_loc, bool isBase);

public:
	// Constructor
	Grua(GLuint matrix);


};


#endif // GRUA_h
