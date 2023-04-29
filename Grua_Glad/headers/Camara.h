#ifndef CAMARA_H
#define CAMARA_H

/// Glad
#include <glad.h>
#include <glfw3.h>

/// Matriz de Transformacion
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define EJE_X glm::vec3(1.0f, 0.f, 0.f)
#define EJE_Y glm::vec3(0.f, 1.0f, 0.f)
#define EJE_Z glm::vec3(0.0f, 0.f, 1.0f)

class Camara {
	private:
		GLfloat angle_x; // Angulo con respecto al eje x
		GLsizei *view_width, *view_height;		// Viewport Width and Height
		GLuint shader;							// Shader Program

	public:
		// Default Constructor
		Camara();

		// Constructor
		Camara(GLsizei*view_width, GLsizei *view_height, GLuint shader_program);

		// Camara por defecto
		void default_view();

		// Primera Persona
		void first_person(glm::vec3 pos, glm::vec3 angle);

		// Tercera Persona
		void third_person(glm::vec3 pos, glm::vec3 angle);

		// Get angle
		const GLfloat& angle() const;

		// Set angle
		GLfloat& angle();

};

#endif // CAMARA_H