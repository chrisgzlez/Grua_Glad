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


// Vertices
#include <cubo.h>
#include <cuadrado.h>
#include <esfera.h>

#include <iostream>


using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;
GLfloat angulo = 0;
int eleccion_camara = 2; //Por defecto va a ser uno que llama a la camara exterior

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// unsigned int VBO, VAO, EBO;

// Vertex Array Object
GLuint VAOEjes;
GLuint VAOCuadrado;
GLuint VAOCubo;
GLuint VAOEsfera;

// Vertex Buffer Object
GLuint VBO;

// 
// GLuint EBO


//Sphere sphere1(1.0f, 18, 9);

/// DEFINICION DE LAS PARTES DE LA GRUA
typedef struct {
	GLfloat px, py, pz;					// Posiciones x, y, z
	GLfloat sx, sy, sz;					// Escalado del objeto (size)
	GLfloat ang_trans_x, ang_trans_z;	// Angulos de translacion sobre los ejes
	GLuint *render;						// VAO
	GLfloat vel;						// Velocidad
	GLuint num_vertices;				// Numero de vertices de la figura
} objeto;

//					{ px, py, pz, sx, sy, sz, ang_x, ang_z, render }
// Base de la Grua (rectangulo)
objeto base_grua =	{ 0.f, 0.f, 0.15f, 0.3f, 0.2f, 0.2f, 0.f, 0.f, &VAOCubo, 0, 36 };

// Punto de articulacion 1 (esfera)
objeto art_1 =		{ 0.f, 0.f, 0.10f, 0.07f, 0.07f, 0.07f, 0.f, 0.f, &VAOEsfera, 0, 1080 };

// Brazo 1 de la grua (rectangulo)
objeto brazo_1 =	{ 0.f, 0.f, 0.10f, 0.05f, 0.05f, 0.3f, 0.f, 0.f, &VAOCubo, 0, 36 };

// Punto de articulacion 2 (esfera)
objeto art_2 =		{ 0.f, 0.f, 0.15f, 0.05f, 0.05f, 0.05f, 0.f, 0.f, &VAOEsfera, 0, 1080 };

// Brazo 2 de la grua (rectangulo)
objeto brazo_2 =	{ 0.f, 0.f, 0.11f, 0.05f, 0.05f, 0.3f, 0.f, 0.f, &VAOCubo, 0, 36 };

/// FIN DEFINICION PARTES DE LA GRUA

/// TODO: Implementar
void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);

void cargar_ejes() {
	GLuint VBO, EBO;
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f, 1.f, 1.f, 1.f,	// Punto Origen 0
		0.5f, 0.0f, 0.0f, 1.f, 0.0f, 0.0f,	// Coord X 
		0.0f, 0.5f, 0.0f, 1.f, 0.0f, 0.0f,	// Coord Y 
		0.0f, 0.0f, 0.5f, 1.f, 0.0f, 0.0f,	// Coord Z 
		0.5f, 0.5f, 0.5f, 1.f, 1.f, 1.f		// Diagonal
	};

	GLuint indices[] = {
		0, 1,	// Eje x
		0, 2,	// Eje y
		0, 3,	// Eje z
		0, 4	// Diagonal
	};
	// Creamos buffers y array
	glGenVertexArrays(1, &VAOEjes);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAOEjes);

	// Lo trae al contexto actual. Lo convierte en el objeto actual
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Guardamos los vertices en el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Traemos al contexto principal el EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Cargamos los elementos en el EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	// Colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


}

void cargar_esfera() {
	GLuint VBO;

	// Creamos el array del VAO
	glGenVertexArrays(1, &VAOEsfera);

	// Creaa el buffer del objeto
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first.
	// Lo convertimos en el VAO actual
	glBindVertexArray(VAOEsfera);

	// Lo trae al contexto actual. Lo convierte en el objeto actual
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Guardamos los vertices en el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	/// CARGAR EN VAO
	// Comunica con el shader
	// Primero: layout del shader.
	// Segundo: Numero de vertices (3, x, y, z)
	// Tercero: Tipo de dato
	// Cuarto: Suda
	// Quinto: step entre vertices
	// Sexto:: Offset, donde empieza
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	// Esto lo activa. Le pasamos la posicion del VAO que queremos activar
	glEnableVertexAttribArray(0);

	// position normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);

	// position textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	/// FIN CARGAR EN VAO

	// Ponerlo a 0, evita problemas
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}


void cargar_cubo() {
	GLuint VBO;

	// Creamos el array del VAO
	glGenVertexArrays(1, &VAOCubo);

	// Creaa el buffer del objeto
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first.
	// Lo convertimos en el VAO actual
	glBindVertexArray(VAOCubo);

	// Lo trae al contexto actual. Lo convierte en el objeto actual
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Guardamos los vertices en el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cubo), vertices_cubo, GL_STATIC_DRAW);

	/// CARGAR EN VAO
	// Comunica con el shader
	// Primero: layout del shader.
	// Segundo: Numero de vertices (3, x, y, z)
	// Tercero: Tipo de dato
	// Cuarto: Suda
	// Quinto: step entre vertices
	// Sexto:: Offset, donde empieza
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

	// Esto lo activa. Le pasamos la posicion del VAO que queremos activar
	glEnableVertexAttribArray(0);

	// position Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	/// FIN CARGAR EN VAO

	// Ponerlo a 0, evita problemas
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}


void cargar_cuadrado() {
	GLuint VBO;

	// Creamos el array del VAO
	glGenVertexArrays(1, &VAOCuadrado);

	// Creaa el buffer del objeto
	glGenBuffers(1, &VBO);


	// bind the Vertex Array Object first.
	// Lo convertimos en el VAO actual
	glBindVertexArray(VAOCuadrado);

	// Lo trae al contexto actual. Lo convierte en el objeto actual
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Guardamos los vertices en el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cuadrado), vertices_cuadrado, GL_STATIC_DRAW);

	/// CARGAR EN VAO
	// Comunica con el shader
	// Primero: layout del shader.
	// Segundo: Numero de vertices (3, x, y, z)
	// Tercero: Tipo de dato
	// Cuarto: Suda
	// Quinto: step entre vertices
	// Sexto:: Offset, donde empieza
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

	// Esto lo activa. Le pasamos la posicion del VAO que queremos activar
	glEnableVertexAttribArray(0);

	// position Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	/// FIN CARGAR EN VAO

	// Ponerlo a 0, evita problemas
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.7265625f, 0.7421875f, 1.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
}


void display_suelo(GLuint transform_loc) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::mat4 transform;

	// Posicion de la matriz de transformacion en el shader
	

	float escala_suelo = 10;

	for (float x = -2; x <= 2; x += (1 / escala_suelo)) {
		for (float y = -2; y <= 2; y += (1 / escala_suelo)) {
			// Calculo la Matriz
			transform = glm::mat4(); // Matriz Identidad

			// Rota cada uno de los cuadrados antes de colocarlos
			transform = glm::rotate(transform, glm::radians(angulo), glm::vec3(1.0f, 0.f, 0.f));

			// Los colocamos en su posicion
			transform = glm::translate(transform, glm::vec3(x, y, 0.f));

			// Los escalamos al tamaño
			transform = glm::scale(transform, glm::vec3((GLfloat)(1 / escala_suelo), (GLfloat)(1 / escala_suelo), (GLfloat)(1 / escala_suelo)));

			// Le pasamos a la matriz del shader un vector con los valores de la matriz transform
			glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Cargamos el vao de la figura del cuadrado en el contexto principal
			glBindVertexArray(VAOCuadrado);

			// Dibujamos los cuadrados
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

	}
}

void _construir_partes_grua(glm::mat4* base, objeto obj, GLuint matrix_loc, bool isBase) {
	// Declaramos e Inicializamos matriz de transformacion
	glm::mat4 transform = glm::mat4();

	// Cargamos la matriz base
	transform = *base;

	// Si es la base de la grau
	if (isBase) {
		// Rota la base con respecto a la camara
		transform = glm::rotate(transform, glm::radians(angulo), glm::vec3(1.0f, 0.f, 0.f));
	}
	
	// La colocamos en su posicion
	transform = glm::translate(transform, glm::vec3(obj.px, obj.py, obj.pz));

	// Si son las articulaciones y brazos
	if (!isBase) {
		// Rotamos articulacion en el eje x
		transform = glm::rotate(transform, glm::radians(art_1.ang_trans_x), glm::vec3(1.f, 0.f, 0.f));

		// Rotamos articulacion en el eje z
		transform = glm::rotate(transform, glm::radians(art_1.ang_trans_z), glm::vec3(0.0f, 1.f, 0.f));
	}

	// Guardamos esta matriz de posicion y rotacion
	// Para usarla como base en las siguientes partes de la grua
	*base = transform;

	// Tamaño de la base de la grua
	transform = glm::scale(transform, glm::vec3(obj.sx, obj.sy, obj.sz));

	// Le pasamos a la matriz del shader un vector con los valores de la matriz transform
	glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, glm::value_ptr(transform));

	// Cargamos el vao de la figura del cuadrado en el contexto principal
	glBindVertexArray(*obj.render);

	// Dibujamos los cuadrados
	glDrawArrays(GL_TRIANGLES, 0, obj.num_vertices);
}


void grua(GLuint matrix_loc) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Inicializamos matrices
	glm::mat4 mat_base = glm::mat4();
	_construir_partes_grua(&mat_base, base_grua, matrix_loc, TRUE);
	_construir_partes_grua(&mat_base, art_1, matrix_loc, FALSE);
	_construir_partes_grua(&mat_base, brazo_1, matrix_loc, FALSE);
	_construir_partes_grua(&mat_base, art_2, matrix_loc, FALSE);
	_construir_partes_grua(&mat_base, brazo_2, matrix_loc, FALSE);
}



int main(int argc, char** argv) {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();

	// Indica a glfw que version estamos usando de OpenGl: (mayor)3.(minor)3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Estamos usando el perfil CORE
	// Signifca que solo usamos las funciones modernas
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//Creo la ventana
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Clases", NULL, NULL);

	// Procesa errores en creacion de ventanas
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	// Introduce la ventana al contexto actual
	glfwMakeContextCurrent(window);

	// TODO: Mirar que hace
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	
	glfwSetKeyCallback(window, keyCallback);

	openGlInit();

	// Genera el shader program a partir de los archivos
	shaderProgram = setShaders("resources\\shader.vert", "resources\\shader.frag");

	/// Cargar en VAO las figuras
	cargar_cuadrado();
	cargar_cubo();
	cargar_esfera();
	cargar_ejes();

	/// FIN CARGAR figuras en el VAO

	// Activamos o shader
	glUseProgram(shaderProgram);

	// Obtenemos la localizacion de la matriz en el shader
	GLuint transform_loc = glGetUniformLocation(shaderProgram, "transform");


	///MAIN LOOP
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// input
		// -----
		processInput(window);

		// Establecemos el color de limpieza del buffer
		glClearColor(0.1015625f, 0.15234375f, 0.30859375, 1.0f);

		// Limpiamos el buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/// SUELO
		// display_suelo(transform_loc);
		/// FIN SUELO

		/// DIBUJAR EJES
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
		glBindVertexArray(VAOEjes);
		glDrawElements(GL_LINES, 9, GL_UNSIGNED_INT, 0);
		/// FIN DIBUJAR EJES
		
		/// CONSTRUIR GRUA
		//grua(transform_loc);
		/// FIN CONSTRUIR GRUA
		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/// Liberar Memoria
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOEjes);
	glDeleteVertexArrays(1, &VAOCuadrado);


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

void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods) {

	//codigo asci de las teclas que estamos pulsando
	std::cout << key << std::endl;



	//subir y bajar la camara
	//k y l para subir y bajar
	
	/// subir camara
	if (key == GLFW_KEY_K) {//letra k
		angulo++;
	}

	/// bajar camara
	if (key == GLFW_KEY_L) {//letra l
		angulo--;
	}
	//movimineto de la base
	
	
	/// acelerar
	if (key == GLFW_KEY_W) {//letra w
		base_grua.vel += 0.001;
	}

	/// marcha atras/frenar
	if (key == GLFW_KEY_X) {//letra x
		base_grua.vel -= 0.001;
	}

	/// derecha
	if (key == GLFW_KEY_D) {//letra d
		base_grua.ang_trans_x++;
	}

	/// izquierda
	if (key == GLFW_KEY_A) {//letra a
		base_grua.ang_trans_x--;
	}
	
	//espacio para freno de mano
	/// marcha atras/frenar
	if (key == GLFW_KEY_SPACE) {//espacio
		base_grua.vel=0;
	}
	
	//primera articulacion
	
	/// subir brazo
	if (key == GLFW_KEY_UP) {//flechira arriba
		if (art_1.ang_trans_z < 90) {
			art_1.ang_trans_z++;
		}
	}

	/// bajar brazo
	if (key == GLFW_KEY_DOWN) {//flechita abajo
		if (art_1.ang_trans_z < -90) {
			art_1.ang_trans_z--;
		}
	}

	/// brazo a derecha
	if (key == GLFW_KEY_RIGHT) {//flechita derecha
		if (art_1.ang_trans_x < 90) {
			art_1.ang_trans_x++;
		}
	}
	
	/// brazo a izquierda
	if (key == GLFW_KEY_LEFT) {//flechita izquierda
		if (art_1.ang_trans_x < -90) {
			art_1.ang_trans_x++;
		}
	}
	
	//segunda articulacion
		
	/// subir brazo
	if (key == GLFW_KEY_T) {//flechira arriba
		if (art_2.ang_trans_z < 90) {
			art_2.ang_trans_z++;
		}
	}

	/// bajar brazo
	if (key == GLFW_KEY_G) {//flechita abajo
		if (art_2.ang_trans_z < -90) {
			art_2.ang_trans_z--;
		}
	}

	/// brazo a derecha
	if (key == GLFW_KEY_H) {//flechita derecha
		if (art_2.ang_trans_x < 90) {
			art_2.ang_trans_x++;
		}
	}

	/// brazo a izquierda
	if (key == GLFW_KEY_F) {//flechita izquierda
		if (art_2.ang_trans_x < -90) {
			art_2.ang_trans_x++;
		}
	}

	//cambio entre las camaras de la grua
	
	/// primera persona
	if (key == GLFW_KEY_1) {//espacio
		 eleccion_camara= 1;
	}

	/// camara exterior persona
	if (key == GLFW_KEY_2) {//espacio
		eleccion_camara = 2;
	}

	/// tercera persona
	if (key == GLFW_KEY_3) {//espacio
		eleccion_camara = 3;
	}
}
