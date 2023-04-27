#include <Parte.h>
#define ACCELERATION 0.001f
#define MAX_ANGULO_ROT 70

Parte::Parte() {
	return;
}


Parte::Parte(GLfloat px, GLfloat py, GLfloat pz, GLfloat sx, GLfloat sy, GLfloat sz, GLuint num_vertices, GLuint VAO) {
	this->_position = glm::vec3(px, py, pz);
	this->scale = glm::vec3(sx, sy, sz);
	this->ang_x = 0;
	this->ang_y = 0;
	this->ang_z = 0;
	this->vel = 0;
	this->VAO = VAO;
	this->num_vertices = num_vertices;
}



void Parte::display(glm::mat4* base, GLuint matrix_loc) {
	// Declaramos e Inicializamos matriz de transformacion
	glm::mat4 transform = glm::mat4();

	// Cargamos la matriz base
	transform = *base;

	// La colocamos en su posicion
	transform = glm::translate(transform, this->_position);

	// Rotamos articualcion en el eje x
	transform = glm::rotate(transform, glm::radians(this->ang_x), EJE_X);

	// Rotamos articulacion en el eje y
	transform = glm::rotate(transform, glm::radians(this->ang_y), EJE_Y);

	// Rotamos en el eje z (base, girar)
	transform = glm::rotate(transform, glm::radians(this->ang_z), EJE_Z);



	// Guardamos esta matriz de posicion y rotacion
	// Para usarla como base en las siguientes partes de la grua
	*base = transform;

	// Tamaño de la base de la grua
	transform = glm::scale(transform, this->scale);

	// Le pasamos a la matriz del shader un vector con los valores de la matriz transform
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, glm::value_ptr(transform));

	// Cargamos el vao de la figura del cuadrado en el contexto principal
	glBindVertexArray(this->VAO);

	// Dibujamos los cuadrados
	glDrawArrays(GL_TRIANGLES, 0, this->num_vertices);
}

void Parte::move() {
	// Move on the x axis
	this->_position.x += this->vel * cos(glm::radians(this->ang_z));

	// Move on the y axis
	this->_position.y += this->vel * sin(glm::radians(this->ang_z));
}

void Parte::rotar(direction dir) {
	switch (dir) {
		case FRONT:
			if (this->ang_y < MAX_ANGULO_ROT) {
				this->ang_y++;
			}
			break;

		case BACK:
			if (this->ang_y > -MAX_ANGULO_ROT) {
				this->ang_y--;
			}
			break;

		case RIGHT:
			if (this->ang_x < MAX_ANGULO_ROT) {
				this->ang_x++;
			}
			break;

		case LEFT:
			if (this->ang_x > -MAX_ANGULO_ROT) {
				this->ang_x--;
			}
			break;

	}
}

void Parte::girar(direction dir) {
	switch (dir) {
		case RIGHT:
			this->ang_z--;
			break;
		case LEFT:
			this->ang_z++;
			break;
		case FRONT: case BACK:
			break;
	}
}

void Parte::acelerar() {
	this->vel += ACCELERATION;
}

void Parte::frenar() {
	this->vel -= ACCELERATION;
}

void Parte::handbrake() {
	this->vel = 0;
}

const glm::vec3& Parte::position() const {
	return this->_position;
}

glm::vec3 Parte::angle() const {
	return glm::vec3(this->ang_x, this->ang_y, this->ang_z);
}