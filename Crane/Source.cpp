
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "esfera.h"
#include "lecturaShader_0_9.h"

#define RADIANES 3.14/180

void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

extern GLuint setShaders(const char* nVertx, const char* nFrag);
GLuint shaderProgram;


// VAO
unsigned int VAO;
unsigned int VAOCuadrado;
unsigned int VAOCubo;
unsigned int VAO_esfera;
float angulox=0.0f;
float anguloy;

// estructura para la grua
typedef struct {
	float px, py, pz; //posicion inicial
	float angulo_trans; //angulo giro x
	float angulo_trans_2; //angulo giro z
	float sx, sy, sz; //escalado en los dos ejes
	unsigned int listarender; //VAO
}objeto;

//definicion de las partes de la grua
objeto base = { 0.0f,0.0f,-0.10f,0.0f,0.0f,.5f,.3f,.3f,0 };//base
objeto articulacion1 = { 0.0f,0.0f,-0.15f,0.0f,0.0f,.05f,.05f,.05f,0 };//articulacion1
objeto brazo1 = { 0.f,0.0f,-0.20f,0.0f,0.0f,.05f,.05f,.3f,0 };//brazo1
objeto articulacion2 = {0.0f,0.0f,-0.18f,0.0f,0.0f,.04f,.04f,.04f,0};//articulacion2
objeto brazo2 = { 0.0f,0.0f,-0.16f,0.0f,0.0f,.04f,.04f,.25f,0 };//base2

// variable global que almacena el modo de la cámara
int modo_camara = 1; // 0: 1ra persona, 1: 3ra persona
void keyCallBack(GLFWwindow* window, int key, int scanColor, int action, int mods);

void crearCubo() {
	unsigned int VBO,VBOColor;

	float vertices[] = {
		//Frontal
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 

		//Trasera
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 
		 0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 
		-0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 

		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f, 
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 

		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
	};

	//Geberanmos VAO
	glGenVertexArrays(1, &VAOCubo);
	glBindVertexArray(VAOCubo);

	// Generamos un VBO para los vértices y lo enlazamos al VAO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// atributo posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//VBO color
	glGenBuffers(1, &VBOColor);
	glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//atributo color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);


	// Desenlazamos los buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void dibujaCuadrado() {
	unsigned int VBO;


	float vertices[] = {
		//primer triangulo
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		 //segundo triangulo
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		 -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
	};

	glGenVertexArrays(1, &VAOCuadrado);
	glGenBuffers(1, &VBO);
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

}

void dibujaEjes() {
	unsigned int VBO, EBO;
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//Vertices          //Colores
		0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,  // 0
		.5f, 0.0f, 0.0f,	 1.0f, 0.0f, 0.0f, //x
		0.0f, .5f, 0.0f,	 0.0f, 1.0f, 0.0f,// y
		0.0f, 0.0f, .5f,	 0.0f, 0.0f, 1.0f, // z  
		.5f , .5f, 0.5f,	1.0f, 1.0f, 1.0f // 1,1,1 bueno realmente la mitad
	};
	unsigned int indices[] = {  // empieza desde cero
		0, 1,
		0, 2,
		0, 3,
		0, 4
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
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


void dibuja_esfera() {
	unsigned int VBO;

	glGenVertexArrays(1, &VAO_esfera);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAO_esfera);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_esfera), vertices_esfera, GL_STATIC_DRAW);

	//Vertices

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(0);

	// Normales

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//Texturas

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

}


void dibujaSuelo() {
	//declaro matrices de transformacion
	glm::mat4 transform;
	//esta matriz almacena la matriz de transformacion actual
	glm::mat4 transformtemp;

	//buscamos la matriz en el shader
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	float i, j;
	float escalasuelo = 10;
	for (i = -2.0f; i <= 2.0f; i += (1.0f / escalasuelo)) {
		for (j = -2.0f; j <= 2.0f; j += (1.0f / escalasuelo)) {
			//Calculo la matriz
			transform = glm::mat4(); //matriz identidad
			transform = glm::rotate(transform, (float)(angulox * RADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::translate(transform, glm::vec3(i, j, 0.0f));
			transform = glm::scale(transform, glm::vec3((1 / escalasuelo), (1 / escalasuelo), (1 / escalasuelo)));

			//cargo la matriz
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
			glBindVertexArray(VAOCuadrado);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

//partes de la grua
glm::mat4 dibujaBasePrincipal() {
	//declaro matrices de transformacion
	glm::mat4 transform;
	//esta matriz almacena la matriz de transformacion actual
	glm::mat4 transformtemp;

	//buscamos la matriz en el shader
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	//BASE
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Calculo la matriz
	transform = glm::mat4(); //matriz identidad
	transform = glm::rotate(transform, (float)(angulox * RADIANES), glm::vec3(1.0f, 0.0f, 0.0f));

	transform = glm::translate(transform, glm::vec3(base.px, base.py, base.pz));
	transform = glm::rotate(transform, (float)(base.angulo_trans * RADIANES), glm::vec3(0.0f, 0.0f, 1.0f));
	transformtemp = transform;
	transform = glm::scale(transform, glm::vec3(base.sx, base.sy, base.sz));

	//cargo la matriz
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAOCubo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(VAO);									//hacemos un bind al VAO que queremos emplear en el dibujo
	glDrawArrays(GL_LINES, 0, 6);

	return transformtemp;
}
glm::mat4 dibujaArticulacion1(glm::mat4 transformtemp,unsigned int transformLoc) {
	//declaro matrices de transformacion
	glm::mat4 transform;
	//esta matriz almacena la matriz de transformacion actual
	glm::mat4 transformtempArt1;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Calculo la matriz
	transform = glm::mat4(); //matriz identidad
	transform = transformtemp;
	transform = glm::translate(transform, glm::vec3(articulacion1.px, articulacion1.py, articulacion1.pz));
	//hacer las dos rotaciones
	transform = glm::rotate(transform, (float)(articulacion1.angulo_trans * RADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)(articulacion1.angulo_trans_2 * RADIANES), glm::vec3(0.0f, 1.0f, 0.0f));
	transformtempArt1 = transform;
	transform = glm::scale(transform, glm::vec3(articulacion1.sx, articulacion1.sy, articulacion1.sz));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAO_esfera);
	glDrawArrays(GL_TRIANGLES, 0, 1080);
	glBindVertexArray(VAO);									//hacemos un bind al VAO que queremos emplear en el dibujo
	glDrawArrays(GL_LINES, 0, 6);
	return transformtempArt1;
}
glm::mat4 dibujaBrazo1(glm::mat4 transformtemp, unsigned int transformLoc) {
	//declaro matrices de transformacion
	glm::mat4 transform;
	//esta matriz almacena la matriz de transformacion actual
	glm::mat4 transformtempBrazo1;
	transform = transformtemp;
	transform = glm::translate(transform, glm::vec3(brazo1.px, brazo1.py, brazo1.pz));
	transform = glm::rotate(transform, (float)(brazo1.angulo_trans * RADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)(brazo1.angulo_trans_2 * RADIANES), glm::vec3(0.0f, 1.0f, 0.0f));
	transformtempBrazo1 = transform;
	transform = glm::scale(transform, glm::vec3(brazo1.sx, brazo1.sy, brazo1.sz));

	//cargo la matriz
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAOCubo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	return transformtempBrazo1;
}

glm::mat4 dibujaArticulacion2(glm::mat4 transformtemp, unsigned int transformLoc) {
	//declaro matrices de transformacion
	glm::mat4 transform;
	//esta matriz almacena la matriz de transformacion actual
	glm::mat4 transformtempArt2;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Calculo la matriz
	transform = glm::mat4(); //matriz identidad
	transform = transformtemp;
	transform = glm::translate(transform, glm::vec3(articulacion2.px, articulacion2.py, articulacion2.pz));
	//hacer las dos rotaciones
	transform = glm::rotate(transform, (float)(articulacion2.angulo_trans * RADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)(articulacion2.angulo_trans_2 * RADIANES), glm::vec3(0.0f, 1.0f, 0.0f));
	transformtempArt2 = transform;
	transform = glm::scale(transform, glm::vec3(articulacion2.sx, articulacion2.sy, articulacion2.sz));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAO_esfera);
	glDrawArrays(GL_TRIANGLES, 0, 1080);
	return transformtempArt2;
}

glm::mat4 dibujarbrazo2(glm::mat4 transformtemp, unsigned int transformLoc) {
	//declaro matrices de transformacion
	glm::mat4 transform;
	//esta matriz almacena la matriz de transformacion actual
	glm::mat4 transformtempBrazo2;
	transform = transformtemp;
	transform = glm::translate(transform, glm::vec3(brazo2.px, brazo2.py, brazo2.pz));
	transform = glm::rotate(transform, (float)(brazo2.angulo_trans * RADIANES), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)(brazo2.angulo_trans_2 * RADIANES), glm::vec3(0.0f, 1.0f, 0.0f));
	transformtempBrazo2= transform;
	transform = glm::scale(transform, glm::vec3(brazo2.sx, brazo2.sy, brazo2.sz));

	//cargo la matriz
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glBindVertexArray(VAOCubo);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	return transformtempBrazo2;
}

//propiedades de la camara
glm::vec3 cameraPos   = glm::vec3(-3.f, -.5f,  -1.0f);
glm::vec3 cameraFront = glm::vec3(5.f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);

//camara
void terceraPersona(float posicionx,float posiciony,float posicionz, float angulo) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//Matriz de Vista
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(
		glm::vec3(posicionx -0.5f * cos(angulo * RADIANES), posiciony-0.5f * sin(angulo * RADIANES), posicionz -0.6f),
		glm::vec3(posicionx+5*cos(angulo * RADIANES), posiciony+5*sin(angulo * RADIANES),posicionz+0),
		glm::vec3(0.0f, 0.0f, -1));
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Matriz de proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
void primeraPersona(float posicionx, float posiciony, float posicionz, float angulo) {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//Matriz de Vista
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(
		glm::vec3(posicionx +0.0*cos(angulo*RADIANES), posiciony + 0.00f * sin(angulo * RADIANES), posicionz-0.3),
		glm::vec3(posicionx + 5*cos(angulo*RADIANES), posiciony+ 5 * sin(angulo * RADIANES),posicionz+ 0),
		glm::vec3(0.0f, 0.0f, -1));
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Matriz de proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
void camaraLibre() {
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	//Matriz de Vista
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//Matriz de proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	//glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);

}

int main()
{
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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetKeyCallback(window, keyCallBack);
	openGlInit();

	//generarShader();
	shaderProgram = setShaders("shader.vert", "shader.frag");
	glUseProgram(shaderProgram);

	dibujaEjes();
	dibujaCuadrado();
	crearCubo();
	dibujaSuelo();
	dibuja_esfera();

	// Lazo de la ventana mientras no la cierre
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
		glUseProgram(shaderProgram);

		//camara
		if (modo_camara == 0) {
			terceraPersona(base.px,base.py,base.pz,base.angulo_trans);
		}
		if (modo_camara == 1){
			primeraPersona(base.px, base.py, base.pz, base.angulo_trans);
		}
		if (modo_camara == 2) {
			camaraLibre();
		}

		//declaro matrices
		glm::mat4 transform;
		//esta matriz almacena la matriz de transformacion actual
		glm::mat4 transformtemp;
		//buscamos la matriz en el shader
		unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
		
		//ejes para orientarme
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//Calculo la matriz
		transform = glm::mat4(); //matriz identidad

		//cargo la matriz
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		glBindVertexArray(VAO);									//hacemos un bind al VAO que queremos emplear en el dibujo
		glDrawArrays(GL_LINES, 0, 6);

		//suelo
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		dibujaSuelo();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//BASE
		transformtemp = dibujaBasePrincipal();

		//Articulacion1
		transformtemp = dibujaArticulacion1(transformtemp,transformLoc);
		
		/*transformtemp = dibujaOBJ(transformtemp, transformLoc, articulacion1);
		glBindVertexArray(VAO_esfera);
		glDrawArrays(GL_TRIANGLES, 0, 1080);*/

		//brazo1
		transformtemp = dibujaBrazo1(transformtemp,transformLoc);

		//articulacion2
		transformtemp = dibujaArticulacion2(transformtemp, transformLoc);
		//transformtemp = dibujaOBJ(transformtemp, transformLoc,articulacion2);

		//base2
		transformtemp = dibujarbrazo2(transformtemp, transformLoc);


		// glfw: swap 
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO);



	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void keyCallBack(GLFWwindow* window, int key, int scanColor, int action, int mods) {
	std::cout << key << std::endl;
	float velocidad_camara=0.05f;
	//suelo
	//K
	if (key == 75) {
		angulox++;
	}
	//L
	if (key == 76) {
		angulox--;
	}
	//base
	//D
	if (key == 68) {
		base.angulo_trans+=2;
	}
	//A
	if (key == 65) {
		base.angulo_trans-=2;
	}
	//articulacion1
	//E
	if (key == 69) {
		articulacion1.angulo_trans++;
	}
	//R
	if (key == 82) {
		articulacion1.angulo_trans--;
	}
	//T
	if (key == 84) {
		articulacion1.angulo_trans_2++;
	}
	//Y
	if (key == 89) {
		articulacion1.angulo_trans_2--;
	}
	//articulacion2
	//O
	if (key == 79) {
		articulacion2.angulo_trans++;
	}
	//P
	if (key == 80) {
		articulacion2.angulo_trans--;
	}
	//I
	if (key == 73) {
		articulacion2.angulo_trans_2++;
	}
	//U
	if (key == 85) {
		articulacion2.angulo_trans_2--;
	}

	//movimiento grua
	//W
	if (key == 87) {
		base.px += 0.1f * cos(base.angulo_trans * RADIANES);
		base.py += 0.1f * sin(base.angulo_trans * RADIANES);
	}
	//S
	if (key == 83) {
		base.px-=0.1f * cos(base.angulo_trans * RADIANES);
		base.py -=0.1f * sin(base.angulo_trans * RADIANES);
	}

	//cambiar camara
	//C
	if (key == 67) {
		modo_camara = 1;
	}
	//V
	if (key == 86) {
		modo_camara = 0;
	}
	//B
	if (key == 66) {
		modo_camara = 2;
	}

	//movimiento de la camara
	if (key == GLFW_KEY_UP) {
		cameraPos += velocidad_camara * cameraFront;
	}
	if (key == GLFW_KEY_DOWN) {
		cameraPos -= velocidad_camara * cameraFront;
	}
	if (key == GLFW_KEY_RIGHT) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocidad_camara;
	}
	if (key == GLFW_KEY_LEFT) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocidad_camara;

	}
}