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


/// Vertices
#include <cubo.h>
#include <cuadrado.h>
#include <esfera.h>


/// Para las texturas
#include <texturas.h>

#include <iostream>
#include <sstream>

#include <Grua.h>
#include <Camara.h>

#define SIZE_SUELO 4

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;
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

GLsizei w = SCR_WIDTH;
GLsizei h = SCR_HEIGHT;

// Vertex Buffer Object
GLuint VBO;

Camara camara;
Grua grua;


GLuint textura_suelo;

typedef struct {
	GLfloat x, y, z; // Coordenadas
} punto;


punto pview = { 0.0f, 0.0f, 0.0f };
punto pluz = { 0.0f, 0.0f, 10.0f };

typedef struct {
	int texture_index;
	int timer;
	GLuint textures[16];
	bool reverse;
} Piscina;

Piscina piscina;


// Maneja los inputs
void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);

/// CARGAR MODELOS 
void cargar_ejes() {
	GLuint VBO, EBO;
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f,	// Punto Origen 0
		0.5f, 0.0f, 0.0f, 1.f, .0f, 0.0f,	// Coord X 
		0.0f, 0.5f, 0.0f, 0.f, 1.0f, 0.0f,	// Coord Y 
		0.0f, 0.0f, 0.5f, 0.f, 0.0f, 1.0f,	// Coord Z 
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

	// Esto lo activa. Le pasamos la posicion del VAO que queremos activar
	glEnableVertexAttribArray(0);

	// position Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// position Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

	// Esto lo activa. Le pasamos la posicion del VAO que queremos activar
	glEnableVertexAttribArray(0);

	// position Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// position Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/// FIN CARGAR EN VAO

	// Ponerlo a 0, evita problemas
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void cargar_piscina() {
	piscina.reverse = false;
	piscina.texture_index = 0;

	for (int i = 0; i < 16; i++) {
		stringstream texture_path;
		texture_path << "resources\\texturas\\piscina" << i << ".png";
		cargar_textura(texture_path.str().c_str(), &piscina.textures[i]);
	}
}

/// FIN CARGAR MODELOS


// Realiza el display de las piscinas
void display_piscina(GLuint transform_loc) {
	// Activamos el modo Blend para las texturas
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendColor(1.0f, 1.f, 1.f, 0.5f);


	// Carga Matriz identidad
	glm::mat4 transform = glm::mat4();
	
	// Trasalada a la posicion de la piscina
	transform = glm::translate(transform, glm::vec3(0.3f, 0.0f, 0.01f));

	// Escala el tamaño de la piscina
	transform = glm::scale(transform, glm::vec3(0.9, 0.9, 0.1));

	// Selecciona la textura de piscina a usar
	glBindTexture(GL_TEXTURE_2D, piscina.textures[piscina.texture_index]);

	// Le pasamos a la matriz del shader un vector con los valores de la matriz transform
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

	// Cargamos el vao de la figura del cuadrado en el contexto actual
	glBindVertexArray(VAOCuadrado);

	// Dibujamos los cuadrados
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Desactivamos Blend y liberamos textura
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	// Le sumo uno a la variable random para que vaya alternando entre las 15 texturas diferentes
	if (!(piscina.timer % 5)) {

		if (!piscina.reverse){
			piscina.texture_index++;
			if (piscina.texture_index == 15) piscina.reverse = true;
		}
		else {
			piscina.texture_index--;
			if (piscina.texture_index == 0) piscina.reverse = false;
		}
		// piscina.texture_index %= 16;
	}

	piscina.timer++;

}


// Realiza el display del suelo
void display_suelo(GLuint transform_loc) {
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::mat4 transform;

	// Posicion de la matriz de transformacion en el shader


	float escala_suelo = 10;

	for (float x = -SIZE_SUELO; x <= SIZE_SUELO; x += (1 / escala_suelo)) {
		for (float y = -SIZE_SUELO; y <= SIZE_SUELO; y += (1 / escala_suelo)) {
			// Calculo la Matriz
			transform = glm::mat4(); // Matriz Identidad

			// Los colocamos en su posicion
			transform = glm::translate(transform, glm::vec3(x, y, 0.f));

			// Los escalamos al tamaño
			transform = glm::scale(transform, glm::vec3((GLfloat)(1 / escala_suelo), (GLfloat)(1 / escala_suelo), (GLfloat)(1 / escala_suelo)));

			// Traemos textura al contexto actual
			glBindTexture(GL_TEXTURE_2D, textura_suelo);

			// Le pasamos a la matriz del shader un vector con los valores de la matriz transform
			glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Cargamos el vao de la figura del cuadrado en el contexto principal
			glBindVertexArray(VAOCubo);

			// Dibujamos los cuadrados
			glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

	}
}


void iluminacion() {
	//el color del objeto
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
	//glUniform3f(colorLoc, 1.0, 1.0f, 0.0f);

	//el color de la luz ambiente 
	unsigned int lightLoc = glGetUniformLocation(shaderProgram, "lightColor");
	glUniform3f(lightLoc, 0.5f, 0.5f, 0.5f);

	//luz difusa 
	unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, (GLfloat)grua.foco().x, (GLfloat)grua.foco().y, (GLfloat)grua.foco().z + 2);


	//luz especularf
	//la posicion del usuario/camara (0,0,10) en nuestro caso
	unsigned int viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

}


// Inicializa OpenGL
void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.7265625f, 0.7421875f, 1.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	//glEnable(GL_CULL_FACE); //ocultacion caras back
	//glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_2D); // Habilita el mapeado de texturas 
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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grua", NULL, NULL);

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
	
	// Set function to manage inputs
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

	/// Cargamos Texturas de Suelo y Piscina
	cargar_piscina();

	cargar_textura("resources\\texturas\\suelo.jpg", &textura_suelo);
	///

	/// CREAMOS GRUA
	grua = Grua(VAOEsfera, VAOCubo);
	/// FIN CREAR GRUA

	/// CREAMOS CAMARA
	camara = Camara(&w, &h, shaderProgram);
	/// FIN CREAMOS CAMARA
	
	// Activamos o shader
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);


	// Obtenemos las localizacion de los vectores
	unsigned int lightLoc = glGetUniformLocation(shaderProgram, "lightColor");
	unsigned int objectColor = glGetUniformLocation(shaderProgram, "objectColor");
	unsigned int lightPos = glGetUniformLocation(shaderProgram, "lightPos");
	unsigned int lightPoint = glGetUniformLocation(shaderProgram, "lightPoint");
	unsigned int viewPos = glGetUniformLocation(shaderProgram, "viewPos");

	

	// Obtenemos la localizacion de la matriz en el shader
	GLuint transform_loc = glGetUniformLocation(shaderProgram, "transform");


	///MAIN LOOP
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		// Establecemos el color de limpieza del buffer
		glClearColor(0.1015625f, 0.15234375f, 0.30859375, 1.0f);

		// Limpiamos el buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//si es 2 camara libre
		if (eleccion_camara==2) {
			camara.default_view();
		}else if(eleccion_camara==1){ //si es 1 es camara en primera persona
			camara.first_person(grua.position(), grua.angle());
			
		} else if(eleccion_camara==3){ // si es 3 es tercera persona
			camara.third_person(grua.position(), grua.angle());
		}

		// Activo las texturas

		glActiveTexture(GL_TEXTURE0);

		// Inicializa vectores
		glUniform3f(lightLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(objectColor, 1.0f, 1.0f, 1.0f);


		/// SUELO
		display_suelo(transform_loc);
		/// FIN SUELO

		/// PISCINA
		display_piscina(transform_loc);
		/// FIN PISCINA

		/// DIBUJAR EJES
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// glm::rotate(glm::mat4(), glm::radians(angulo_camara), glm::vec3(1.0f, 0.f, 0.f));
		// 
		// glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(), glm::radians(angulo_camara), glm::vec3(1.0f, 0.f, 0.f))));
		// glBindVertexArray(VAOEjes);
		// glDrawElements(GL_LINES, 9, GL_UNSIGNED_INT, 0);
		/// FIN DIBUJAR EJES
		


		/// GRUA

		grua.move();
		grua.display(transform_loc);


		/// FIN GRUA

		iluminacion();


		
		
		

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


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	w = width;
	h = height;
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods) {
	
	// Cierra la ventana al pulsar esc
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}

	//subir y bajar la camara
	//k y l para subir y bajar
	
	if (eleccion_camara == 2){
		/// subir camara
		if (key == GLFW_KEY_K) {//letra k

			if (camara.angle() < 175) {
				camara.angle()++;
			}
		}

		/// bajar camara
		if (key == GLFW_KEY_L) {//letra l
			if (camara.angle() > 90) {
				camara.angle()--;
			}
		}
	}

	//movimineto de la base
	
	/// acelerar
	if (key == GLFW_KEY_W) {//letra w
		grua.acelerar();
	}

	/// marcha atras/frenar
	if (key == GLFW_KEY_X) {//letra x
		grua.frenar();
	}

	/// derecha
	if (key == GLFW_KEY_D) {//letra d
		grua.girar(RIGHT);
	}

	/// izquierda
	if (key == GLFW_KEY_A) {//letra a
		grua.girar(LEFT);
	}
	
	//espacio para freno de mano
	/// freno de mano
	if (key == GLFW_KEY_SPACE) {//espacio
		grua.handbrake();
	}
	
	//primera articulacion
	
	/// adelante brazo
	if (key == GLFW_KEY_UP) {//flechira arriba
		grua.rotar_art(0, FRONT);
	}

	/// atras brazo
	if (key == GLFW_KEY_DOWN) {//flechita abajo
		grua.rotar_art(0, BACK);
	}

	/// brazo a derecha
	if (key == GLFW_KEY_RIGHT) {//flechita derecha
		grua.rotar_art(0, RIGHT);
	}
	
	/// brazo a izquierda
	if (key == GLFW_KEY_LEFT) {//flechita izquierda
		grua.rotar_art(0, LEFT);
	}
	
	//segunda articulacion
		
	/// adelante brazo
	if (key == GLFW_KEY_T) {//flechira arriba
		grua.rotar_art(1, FRONT);
	}

	/// atras brazo
	if (key == GLFW_KEY_G) {//flechita abajo
		grua.rotar_art(1, BACK);
	}

	/// brazo a derecha
	if (key == GLFW_KEY_H) {//flechita derecha
		grua.rotar_art(1, RIGHT);
	}

	/// brazo a izquierda
	if (key == GLFW_KEY_F) {//flechita izquierda
		grua.rotar_art(1, LEFT);
	}

	//cambio entre las camaras de la grua
	
	/// primera persona
	if (key == GLFW_KEY_1) { // numero 1
		 eleccion_camara= 1;
	}

	/// camara exterior persona
	if (key == GLFW_KEY_2) { //numero 2
		eleccion_camara = 2;
	}

	/// tercera persona
	if (key == GLFW_KEY_3) { //numero 3
		eleccion_camara = 3;
	}
}

