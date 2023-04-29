#ifndef PARTE_H
#define PARTE_H

#define EJE_X glm::vec3(1.0f, 0.f, 0.f)
#define EJE_Y glm::vec3(0.f, 1.0f, 0.f)
#define EJE_Z glm::vec3(0.0f, 0.f, 1.0f)

#include <glad.h>
#include <glfw3.h>

/// Matriz de Transformacion
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <texturas.h>

enum direction {
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

class Parte {
private:
	glm::vec3	_position;					// Posiciones x, y, z
	glm::vec3	scale;						// Escalado del objeto (size)
	GLfloat		ang_x, ang_y, ang_z;		// Angulos de translacion sobre los ejes
	GLfloat		vel;						// Velocidad

	// Hacer Clase VAO
	GLuint		VAO;						// VAO
	GLuint		num_vertices;				// Numero de vertices de la figura

	GLuint		_textura;					// Textura de la parte


public:

	// Default Constructor
	Parte();

	// Constructor
	// TODO: Hacer clase VAO, guardar ahí numero de vertices
	Parte(GLfloat px, GLfloat py, GLfloat pz, GLfloat sx, GLfloat sy, GLfloat sz, GLuint num_vertices, GLuint VAO, const char* texture_path);


	// Display
	void display(glm::mat4* base, GLuint matrix_loc);

	// Moverse en funcion de su velocidad actual
	void move();

	// Gira con respecto al plano de movimiento
	void girar(direction dir);

	// Rota sobre su posicion actual en la direccion indicada
	void rotar(direction dir);

	// Get current position
	const glm::vec3& position() const;

	// Get current angle
	glm::vec3 angle() const;

	// Incrementa la velocidad de la grua
	void acelerar();

	// Decrementa la velocidad de la grua/marcha atras
	void frenar();

	// Detiene la grua en la posicion actual
	void handbrake();

	// Getter textura 
	GLuint* textura();
};

#endif // PARTE_H