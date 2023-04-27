#ifndef GRUA_H
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

typedef struct {
	Parte art;
	Parte brazo;
} Brazo;

class Grua {
private:
	
	GLfloat* angulo_camara;	// Angulo base de la grua
	Parte base;
	Brazo brazos[2];

	
public:
	// Default Constructor
	Grua();

	// Constructor
	Grua(GLfloat *angulo_base, GLuint VAOEsfera, GLuint VAOCubo);

	// Display
	void display(GLuint matrix);

	// Mover Grua
	void move();

	// Girar en una direccion
	void girar(direction dir);

	// Mover articulacion
	void rotar_art(int id_brazo, direction dir);

	// Incrementa la velocidad de la grua
	void acelerar();

	// Decrementa la velocidad de la grua/marcha atras
	void frenar();

	// Detiene la grua en la posicion actual
	void handbrake();

	// Get current position
	const glm::vec3& position() const;

	// Get current angle
	const glm::vec3& angle() const;



};


#endif // GRUA_h
