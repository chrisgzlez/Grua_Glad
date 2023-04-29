#include <Camara.h>

#include <iostream>

Camara::Camara() {
	return;
}

Camara::Camara(GLsizei* view_width, GLsizei* view_height, GLuint shader_program) {
	this->angle_x = 90;
	this->view_width = view_width;
	this->view_height = view_height;
	this->shader = shader_program;
}

const GLfloat& Camara::angle() const {
	return this->angle_x;
}

GLfloat& Camara::angle(){
	return this->angle_x;
}

void Camara::default_view() {

	//matriz de view
	glm::mat4 view;
	view = glm::mat4();

	//hacemos el lookat para posicionar la camara
	// la posicionamos en el 0 0 y le damos un poco de altura
	// miramos al centro de la pantalla
	// y en el caso de la normal que apunte hacia el eje y

	view = glm::lookAt(
		glm::vec3(0.0, (GLfloat)cos(glm::radians(this->angle_x))*5, (GLfloat)sin(glm::radians(this->angle_x))*5),
		glm::vec3(0.f, 0.f, 0.f),
		EJE_Y
	);

	// view = glm::lookAt(
	// 	glm::vec3(0.0f, 0.0f, 5.0f),
	// 	glm::vec3(0.0f, 0.0f, 0.0f), 
	// 	glm::vec3(0.0f, 1.0f, 0.0f)
	// );

	unsigned int viewLoc = glGetUniformLocation(this->shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//matriz de proyeccion
	glm::mat4 projection;
	projection = glm::mat4();

	//añadimos la perspectiva a la ventana
	projection = glm::perspective(glm::radians(45.0f), (GLfloat) *this->view_width/ (GLfloat)*this->view_height, 0.01f, 8.0f);
	unsigned int proyectionLoc = glGetUniformLocation(this->shader, "projection");
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

}

void Camara::first_person(glm::vec3 pos, glm::vec3 angle) {

	//matriz para la view
	glm::mat4 view;
	view = glm::mat4();

	// miramos como si fueramos el conductor de la grua (parte de alante del cubo
	// miramos un poco mas alejado de ese punto

	view = glm::lookAt(
		pos + glm::vec3(0.f, 0.f, 0.2f),
		pos + glm::vec3(1.5f * cos(glm::radians(angle.z)), 1.5f * sin(glm::radians(angle.z)), 0.2f),
		EJE_Z
	);

	GLuint viewLoc = glGetUniformLocation(this->shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//matriz de projeccion
	glm::mat4 projection;
	projection = glm::mat4();

	//perpectiva
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)*this->view_width / (GLfloat)*this->view_height, 0.1f, 5.0f);
	GLuint proyectionLoc = glGetUniformLocation(this->shader, "projection");
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

}

void Camara::third_person(glm::vec3 pos, glm::vec3 angle) {

	//matriz de view

	glm::mat4 view;
	view = glm::mat4();

	// hacemos el lookat para posicionar la camara detras de la grua
	//la posicionamos detras de la grua mirando hacia un punto lejano
	// y en el caso de la normal que apunte hacia el eje y

	view = glm::lookAt(
		pos + glm::vec3(-0.5f * cos(glm::radians(angle.z)), -0.5f * sin(glm::radians(angle.z)), 0.4f),
		pos + glm::vec3(10 * cos(glm::radians(angle.z)), 10 * sin((glm::radians(angle.z))), 0.4f),
		EJE_Z
	);


	GLuint viewLoc = glGetUniformLocation(this->shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//matriz de proyeccion
	glm::mat4 projection;
	projection = glm::mat4();

	//perspectiva
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)*this->view_width / (GLfloat)*this->view_height, 0.01f, 5.0f);
	GLuint proyectionLoc = glGetUniformLocation(this->shader, "projection");
	glUniformMatrix4fv(proyectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

}