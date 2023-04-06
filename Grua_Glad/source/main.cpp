#define _USE_MATH_DEFINES
#include <cmath>

#include <glad.h>
#include <glfw3.h>

/// Matriz de Transformacion
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// Shaders
#include <lecturaShader_0_9.h>


#include <iostream>

#define A_RADIANES M_PI / 180

using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;
GLfloat angulo = 0;






// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// unsigned int VBO, VAO, EBO;

// Vertex Array Object
GLuint VAO;
GLuint VAOCuadrado;

// Vertex Buffer Object
// GLuint VBO

// 
// GLuint EBO


//Sphere sphere1(1.0f, 18, 9);

/// DEFINICION DE LAS PARTES DE LA GRUA
typedef struct {
	GLfloat px, py, pz;					// Posiciones x, y, z
	GLfloat sx, sy, sz;					// Escalado del objeto (size)
	GLfloat ang_trans_x, ang_trans_z;	// Angulos de translacion sobre los ejes
	GLuint render;						// VAO

} objeto;

//					{ px, py, pz, sx, sy, sz, ang_x, ang_z, render }
// Base de la Grua (rectangulo)
objeto base_grua =	{ 0.f, 0.f, 0.15f, 0.3f, 0.2f, 0.2f, 0.f, 0.f,  0 };

// Punto de articulacion 1 (esfera)
objeto art_1 =		{ 0.f, 0.f, 0.10f, 0.7f, 0.7f, 0.7f, 0.f, 0.f,  0 };

// Brazo 1 de la grua (rectangulo)
objeto brazo_1 =	{ 0.f, 0.f, 0.10f, 0.5f, 0.5f, 0.3f, 0.f, 0.f,  0 };

// Punto de articulacion 2 (esfera)
objeto art_2 =		{ 0.f, 0.f, 0.15f, 0.05f, 0.05f, 0.05f, 0.f, 0.f,  0 };

// Brazo 2 de la grua (rectangulo)
objeto brazo_2 =	{ 0.f, 0.f, 0.11f, 0.05f, 0.05f, 0.3f, 0.f, 0.f,  0 };

/// FIN DEFINICION PARTES DE LA GRUA

/// TODO: Implementar
void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);



void dibujaCuadrado() {
	unsigned int VBO, EBO;


	float vertices[] = {
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,

	};

	glGenVertexArrays(1, &VAOCuadrado);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAOCuadrado);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// position Color

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.1015625f, 0.15234375f, 0.30859375, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
}

int main(int argc, char** argv) {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	//Creo la ventana
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clases", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	// generarShader();
	shaderProgram = setShaders("shader.vert", "shader.frag");

	openGlInit();


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Lazo de la ventana mientras no la cierre
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------

		glClearColor(0.1015625f, 0.15234375f, 0.30859375, 1.0f);  // valor limpieza buffer color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		/// DIBUJAR SUELO
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glm::mat4 transform, temp;

		// Posicion de la matriz de transformacion en el shader
		GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");

		float escala_suelo = 10;

		for (float x = -2; x <= 2; x += (1 / escala_suelo)) {
			for (float y = -2; y <= 2; y += (1 / escala_suelo)) {
				// Calculo la Matriz
				transform = glm::mat4(); // Identidad
				transform = glm::rotate(transform, (GLfloat)(angulo * A_RADIANES), glm::vec3(1.0f, 0.f, 0.f));
			}

		}

		/// FIN DIBUJAR SUELO

		// ESto no funciona
		// dibujo los ejes
		dibujaCuadrado();
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

		// Tipo de array, algo, numero de vertices 
		glDrawArrays(GL_LINES, 0, 900);

		glBindVertexArray(0); // no need to unbind it every time 




		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}