#include <Grua.h>
#include <Parte.h>




Grua::Grua() {
	return;
}

// Pasarle un const VAO
// Hacer Clase VAO
Grua::Grua(GLfloat *angulo_camara, GLuint VAOEsfera, GLuint VAOCubo) {
	this->angulo_camara = angulo_camara;
	this->base				= Parte(0.f, 0.f, 0.15f, 0.3f, 0.2f, 0.2f, 36, VAOCubo);
	this->brazos[0].art		= Parte(0.f, 0.f, 0.10f, 0.07f, 0.07f, 0.07f, 1080, VAOEsfera);
	this->brazos[0].brazo	= Parte(0.f, 0.f, 0.10f, 0.05f, 0.05f, 0.3f, 36, VAOCubo);
	this->brazos[1].art		= Parte(0.f, 0.f, 0.15f, 0.05f, 0.05f, 0.05f, 1080, VAOEsfera);
	this->brazos[1].brazo	= Parte(0.f, 0.f, 0.11f, 0.05f, 0.05f, 0.3f, 36, VAOCubo);
}

void Grua::display(GLuint matrix_loc) {
	


	// Establece el tipo de poligono en la visualizacion
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Carga la matriz identidad
	glm::mat4 transform = glm::mat4();

	// Rota la base con respecto a la camara
	transform = glm::rotate(transform, glm::radians(*this->angulo_camara), EJE_X);
	
	this->base.display(&transform, matrix_loc);

	// Realiza el display de sus partes
	for (Brazo& p : this->brazos) {
		p.art.display(&transform, matrix_loc);
		p.brazo.display(&transform, matrix_loc);
	}
}

void Grua::move() {
	this->base.move();
}

void Grua::girar(direction dir) {
	this->base.girar(dir);
}

void Grua::rotar_art(int id_brazo, direction dir) {
	this->brazos[id_brazo].art.rotar(dir);
}

void Grua::acelerar() {
	this->base.acelerar();
}

void Grua::frenar() {
	this->base.frenar();
}

void Grua::handbrake() {
	this->base.handbrake();
}


const glm::vec3& Grua::position() const {
	return this->base.position();
}

const glm::vec3& Grua::angle() const {
	return this->base.angle();
}