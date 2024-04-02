//Librerías
#include <windows.h>	// Variables de windows
#include <glut.h>		// GLUT
#include <GL/gl.h>		// GL		 	
#include <GL/glu.h>	

//Librerías Auxiliares
#include <stdio.h>	// Standard Input-Output
#include <math.h>	// Librería Matemática
#include <stdarg.h> // Standard Arguments

//Constantes
#define GL_PI 3.14f		// Valor de Pi
#define myTiempo 30		// Contador de Tiempo

// Tamaño de la ventana
int W_WIDTH = 600;		//Ancho de la ventana
int W_HEIGHT = 600;		//Alto de la ventana

// Ángulos de rotación para la cámara
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

// Camara, teclado y teclas especiales
extern myCamara();
extern void myTeclado(unsigned char tras,int x,int y);
extern void myTeclasespeciales(int cursor,int x,int y) ;

// Opciones
int movimiento = 1;
int modoDibujo = 0;

// Esfera
extern int myEsfera();	// Función para generar una esfera
int esfera;				// Índice para lista de visualización

// Camara (indicador)
int camara = 0;
 
//Definimos la estructura de los planetas:
typedef struct {

	// Identificador
	GLint idPlaneta;

	// Propiedades
	GLint tamanoPlaneta;
	GLfloat anguloTranslacion;
	GLfloat velocidadTranslacion;
	GLfloat anguloRotacion;
	GLfloat velocidadRotacion;
	GLfloat distancia;

	// Color
	GLfloat  colorR, colorG, colorB;

} Planeta;

// Inicializamos los planetas:
//				ID,tam,angT,velT,angR,vR,distancia
Planeta sol = { 1, 100, 0.f, 0.f, 0.f, 1.f, 0.f,/*Color*/ 1.f, 1.f, 0.f};
Planeta mercurio = { 2, 15, 0.f, 5.f, 0.f, 7.f, 150.f,/*Color*/ 1.f, 1.f, 0.f};
Planeta venus = { 3, 25, 0.f, 4.5f, 0.f, 10.f, 200.f,/*Color*/ 1.f, 0.f, 1.f};
Planeta tierra = { 4, 30, 0.f, 4.f, 0.f, 7.f, 300.f,/*Color*/ 0.f, 0.5f, 1.f };
Planeta luna = { 5, 7, 0.f, 5.f, 0.f, 10.f, 80.f,/*Color*/ 1.f, 1.f, 1.f};
Planeta ISS = { 6, 5, 0.f, 10.f, 0.f, 20.f, 50.f,/*Color*/ 0.5f, 0.5f, 0.5f };
Planeta marte = { 7, 25, 0.f, 3.5f, 0.f, 6.f, 400.f,/*Color*/ 1.f, 0.5f, 0.f };
Planeta jupiter = { 8, 60, 0.f, 2.f, 0.f, 9.f, 550.f,/*Color*/ 0.8f, 0.6f, 0.4f };
Planeta saturno = { 9, 55, 0.f, 1.5f, 0.f, 5.f, 670.f,/*Color*/ 1.f, 0.8f, 0.4f };
Planeta urano = { 10, 50, 0.f, 1.f, 0.f, 4.f, 800.f,/*Color*/ 0.5f, 1.f, 0.5f };
Planeta neptuno = { 11, 45, 0.f, 0.8f, 0.f, 3.f, 930.f,/*Color*/ 0.f, 0.f, 1.f };


// ================================================================================//
// myTelescopio: recibe como argumentos un planeta desde el qué enfocar, y un planeta
// al que enfocar.
// ================================================================================//
void myTelescopio(float distancia, float angulo, float distanciaObj, float anguloObj) {

	//Configuracion de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);

	//La ponemos a uno
	glLoadIdentity();

	gluPerspective(45.0, 1, 1.0, 2000.0);
	gluLookAt(distancia * cos(angulo * GL_PI / 180.0), 0, -1 * distancia * sin(angulo * GL_PI / 180.0),
		distanciaObj * cos(anguloObj * GL_PI / 180.0), 0, -1 * distanciaObj * sin(anguloObj * GL_PI / 180.0),
		0, 1, 0);
}
void myTelescopioSatelite(float distancia, float angulo, float distanciaObj, float anguloObj) {

	//Configuracion de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);

	//La ponemos a uno
	glLoadIdentity();

	float PosicionLunaX = distancia * cos(angulo * GL_PI / 180.0) + distanciaObj * cos(anguloObj * GL_PI / 180.0 + angulo * GL_PI / 180.0);
	float PosicionLunaY = distancia * sin(angulo * GL_PI / 180.0) + distanciaObj * sin(anguloObj * GL_PI / 180.0 + angulo * GL_PI / 180.);

	gluPerspective(45.0, 1, 1.0, 2000.0);
	gluLookAt(PosicionLunaX, 0, -1 * PosicionLunaY, //posicion ojos
		distanciaObj * cos(anguloObj * GL_PI / 180.0), 0, -1 * distanciaObj * sin(anguloObj * GL_PI / 180.0), //posicion que enfoca la camara
		0, 1, 0);
}



// ================================================================================//
// menuFun: iguala la variable camara a una de las 15 posibilidades				   ||
// dependiendo de la opción escogida. Se le pasa a glutCreateMenu()				   ||
// ================================================================================//
void menuFun(int opcion) {
	if (camara >= 0 && camara <= 14)
		camara = opcion;
	else camara = 1; //Por defecto deja la exterior
}

// ================================================================================//
// myMenu: función usada en openGlInit() para inicializar un menú. Se abre con el
// click derecho, y sale en pantalla las diferentes opciones.
// ================================================================================//
void myMenu(void) {
	int menuFondo = glutCreateMenu(menuFun);

	//Opciones visibles
	glutAddMenuEntry("Sonda Exterior", 1);
	glutAddMenuEntry("Tierra desde el Sol", 2);
	glutAddMenuEntry("Tierra desde la Luna", 3);
	glutAddMenuEntry("Tierra desde el ISS", 4);
	glutAddMenuEntry("Mercurio desde la Tierra", 5);
	glutAddMenuEntry("Venus desde la Tierra", 6);
	glutAddMenuEntry("Luna desde la Tierra", 7);
	glutAddMenuEntry("ISS desde la Tierra", 8);
	glutAddMenuEntry("Marte desde la Tierra", 9);
	glutAddMenuEntry("Jupiter desde la Tierra", 10);
	glutAddMenuEntry("Saturno desde la Tierra", 11);
	glutAddMenuEntry("Urano desde la Tierra", 12);
	glutAddMenuEntry("Neptuno desde la Tierra", 13);
	glutAddMenuEntry("Sol desde la Tierra", 14);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// ================================================================================//
// myEjes: recibe como argumento una estructura planeta, y dibuja por pantalla
// los ejes de dicho planeta, incluyendo la diagonal. No lo hemos usado, pero
// permanece comentado para su dibujo cuando sea pertinente.
// ================================================================================//
void myEjes (Planeta planeta) {

	glColor3f (0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(planeta.tamanoPlaneta, 0.0f, 0.0f);
	glEnd();

	
	glColor3f (1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f, planeta.tamanoPlaneta, 0.0f);
	glEnd();

	
	glColor3f (0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0,planeta.tamanoPlaneta);
	glEnd();

	//Diagonal
	glColor3f (1.0f,1.0f,1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(planeta.tamanoPlaneta/3, planeta.tamanoPlaneta / 3, planeta.tamanoPlaneta / 3);
	glEnd();
	
}

// ================================================================================//
// myDibujarOrbita: recibe como argumento una estructura planeta, y dibuja su
// respectiva órbita en color blanco.
// ================================================================================//
void myDibujarOrbita(Planeta planeta) {
	//Dibujo de la orbita del planeta
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f); //Blanco
	for (float ang = 0; ang < 2 * GL_PI; ang += 0.01) {
		float x = cos(ang) * planeta.distancia;
		float z = sin(ang) * planeta.distancia;
		glVertex3f(x, 0.0f, z);
	}
	glEnd();
}
// ================================================================================//
// myDibujarAnillos: recibe como argumento una estructura planeta, y dibuja anillos
// a su alrededor proporcionales a su tamaño.
// ================================================================================//
void myDibujarAnillos(Planeta planeta) {
	glColor3f(0.549, 0.549, 0.549);
	glLineWidth(3); //Lineas mas anchas que las orbitas
	for (float i = 1.1; i < 1.9; i += 0.25) {
		//Anillos del planeta
		glBegin(GL_LINE_STRIP);
		for (float ang = 0; ang < 2 * GL_PI; ang += 0.01) {
			float x = cos(ang) * planeta.tamanoPlaneta * i;
			float z = sin(ang) * planeta.tamanoPlaneta * i;
			glVertex3f(x, 0.0f, z);
		}
		glEnd();
	}
	glLineWidth(1);
}


// ================================================================================//
// myDibujarPlanetas: recibe como argumentos como mínimo una estructura planeta y la
// dibuja. Puede recibir más argumentos, en cuyo caso se interpretan como satélites
// y también se dibujan pero orbitando a la estructura planeta del primer argumento.
// ================================================================================//
void myDibujarPlanetas(Planeta planeta, ...) {

	//Lista de argumentos variables
	va_list args;
	va_start(args, planeta);

	//Translacion
	glPushMatrix();
	glRotatef(planeta.anguloTranslacion, 0.0f, 1.0f, 0.0f);
	glTranslatef(planeta.distancia, 0.0f, 0.0f);

	//Rotacion	
	glPushMatrix();
	glRotatef(planeta.anguloRotacion, 0.f, 1.f, 0.f);
	//myEjes(planeta);

	//Dibujamos el objeto
	glColor3f(planeta.colorR, planeta.colorG, planeta.colorB);
	glScalef(planeta.tamanoPlaneta, planeta.tamanoPlaneta, planeta.tamanoPlaneta);
	glCallList(esfera);

	//se recupera la matriz de la pila 
	glPopMatrix(); //Translacion

	// Si el planeta es Saturno, dibujamos sus anillos:
	if (planeta.idPlaneta == 9) {
		myDibujarAnillos(planeta);
	}

	glPopMatrix(); //Rotacion

	// Dibujamos órbita:
	myDibujarOrbita(planeta);

	
	// Procesamos argumentos variables (satélites)
	Planeta satelite;
	while ((satelite = va_arg(args, Planeta)).idPlaneta != 0) {
		// Translación del satélite
		glPushMatrix();
		glRotatef(planeta.anguloTranslacion, 0.0f, 1.0f, 0.0f);
		glTranslatef(planeta.distancia, 0.0f, 0.0f);

		// Rotación del satélite
		glPushMatrix();
		glRotatef(planeta.anguloRotacion, 0.f, 1.f, 0.f);
		glScalef(planeta.tamanoPlaneta, planeta.tamanoPlaneta, planeta.tamanoPlaneta);

		// Recuperamos matriz
		glPopMatrix(); // Rotación 1

		// Satélite
		//Translacion
		glPushMatrix();
		glRotatef(satelite.anguloTranslacion, 0.0f, 1.0f, 0.0f);
		glTranslatef(satelite.distancia, 0.0f, 0.0f);

		//Rotacion	
		glPushMatrix();
		//myEjes(satelite);
		glRotatef(satelite.anguloRotacion, 0.f, 1.f, 0.f);

		//Dibujamos el objeto
		glColor3f(satelite.colorR, satelite.colorG, satelite.colorB);
		glScalef(satelite.tamanoPlaneta, satelite.tamanoPlaneta, satelite.tamanoPlaneta);
		glCallList(esfera);

		//se recupera la matriz de la pila 
		glPopMatrix(); //Rotacion 2
		glPopMatrix(); //Translacion 2

		glPopMatrix(); //Translacion 1
	}

	// Finalizamos la lista de argumentos variables
	va_end(args);
}

// ================================================================================//
// corregirAngulo: esta función auxiliar recibe un puntero a un ángulo, y si es superior a
// 360 se pasa a -360.
// ================================================================================//
void corregirAngulo(float* angulo) {
	if (*angulo > 360) *angulo -= 360;
}

// ================================================================================//
// actualizarPosicion: esta función recibe un puntero a un planeta, y actualiza sus
// ángulos tanto de rotación como de translación sumándoles sus respectivas
// velocidades. Llamamos a la función auxiliar corregirAngulo para lograr realizar
// una circunferencia continua.
// ================================================================================//
void actualizarPosicion(Planeta* planeta) {
	planeta->anguloTranslacion += planeta->velocidadTranslacion;
	planeta->anguloRotacion += planeta->velocidadRotacion;
	corregirAngulo(&planeta->anguloTranslacion);
	corregirAngulo(&planeta->anguloRotacion);
}

// ================================================================================//
// myMovimiento: esta función hace uso de la función actualizarPosicion para
// actualizar las posiciones de cada cuerpo celeste (es decir, moverlos). Además,
// se llama a glutPostRedisplay() para mostrar los posibles cambios y a
// glutTimerFunc() para actualizar el contador de tiempo.
// ================================================================================//
void myMovimiento() {

	// Actualizamos las posiciones de cada planeta:
	actualizarPosicion(&sol);
	actualizarPosicion(&mercurio);
	actualizarPosicion(&venus);
	actualizarPosicion(&tierra);
	actualizarPosicion(&luna);
	actualizarPosicion(&ISS);
	actualizarPosicion(&marte);
	actualizarPosicion(&jupiter);
	actualizarPosicion(&saturno);
	actualizarPosicion(&urano);
	actualizarPosicion(&neptuno);

	// Redibujamos para mostrar cambios:
	glutPostRedisplay();

	// Aumentamos el contador de tiempo:
	glutTimerFunc(myTiempo, myMovimiento, 30);
}


// ================================================================================//
// myDisplay: función que se le pasará a glutDisplayFunction, encargada del dibujo
// cada uno de los planetas con la función myDibujarPlanetas. Además, realiza otras
// tareas auxiliares como limpiar la ventana, activar la matriz del modelador y el
// modo de dibujo, y llamar a glFlush y a glutSwapBuffers para actualizar la pantalla.
// ================================================================================//
void myDisplay (void) {

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	myCamara(); // para colocar la camara

	// Cambiamos la camara dependiendo del planeta:
	switch (camara) {
	case 1:
		myCamara();
		break;
	case 2:
		myTelescopio(sol.distancia, sol.anguloTranslacion, tierra.distancia, tierra.anguloTranslacion);
		break;
	case 3:
		myTelescopioSatelite(tierra.distancia, tierra.anguloTranslacion, luna.distancia, luna.anguloTranslacion);
		break;
	case 4:
		myTelescopioSatelite(tierra.distancia, tierra.anguloTranslacion, ISS.distancia, ISS.anguloTranslacion);
		break;
	case 5:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, mercurio.distancia, mercurio.anguloTranslacion);
		break;
	case 6:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, venus.distancia, venus.anguloTranslacion);
		break;
	case 7:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, luna.distancia, luna.anguloTranslacion);
		break;
	case 8:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, ISS.distancia, ISS.anguloTranslacion);
		break;
	case 9:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, marte.distancia, marte.anguloTranslacion);
		break;
	case 10:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, jupiter.distancia, jupiter.anguloTranslacion);
		break;
	case 11:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, saturno.distancia, saturno.anguloTranslacion);
		break;
	case 12:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, urano.distancia, urano.anguloTranslacion);
		break;
	case 13:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, neptuno.distancia, neptuno.anguloTranslacion);
		break;
	case 14:
		myTelescopio(tierra.distancia, tierra.anguloTranslacion, sol.distancia, sol.anguloTranslacion);
		break;
	default:
		break;
	}

	// Activamos la matriz del modelador
	glMatrixMode(GL_MODELVIEW); //Matriz del Modelo
	glLoadIdentity(); // Inicializamos la matriz del modelo a la identidad
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glColor3f (1.0f,1.0f,50.0f);

	//Dibujamos los planetas con la función:
	myDibujarPlanetas(sol);
	myDibujarPlanetas(mercurio);
	myDibujarPlanetas(venus);
	myDibujarPlanetas(tierra, luna, ISS);
	myDibujarPlanetas(marte);
	myDibujarPlanetas(jupiter);
	myDibujarPlanetas(saturno);
	myDibujarPlanetas(urano);
	myDibujarPlanetas(neptuno);


	//Actualizamos
	glFlush();
	glutSwapBuffers();
		
}	
// ================================================================================//
// reescalado: función auxiliar para ayudar con el reescalado.
// ================================================================================//
void reescalado(GLint ancho, GLint alto) {
	W_WIDTH = ancho;
	W_HEIGHT = alto;
}

// ================================================================================//
// openGlInit: función encargada de realizar inicializaciones de openGlInit. Entre
// ellas se encuentran el z-buffer, la limpieza del buffer, la ocultación de las 
// caras, normalizaciones, e inicializaciones de teclado, display y reescalado.
// ================================================================================//
void openGlInit() {
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color

	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	glEnable(GL_NORMALIZE);

	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(reescalado);

	myMenu();
}

// ================================================================================//
// main: función encargada de iniciar el programa. Crea una ventana, llama a
// openGlInit, y entra en el bucle principal.
// ================================================================================//
int main(int argc, char **argv) {

	glutInit(&argc, argv);
	
	glutInitWindowPosition(0,0);  
	glutInitWindowSize (W_WIDTH, W_HEIGHT);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE |GLUT_DEPTH);
	glutCreateWindow("Sistema Solar en OpenGL 1.2.");

	// Identificador lista de visualización esfera
	esfera = myEsfera();

	//Inicializamos OpenGL
	openGlInit();
	
	// Empieza en bucle principal¡
	myMovimiento();


	glutMainLoop();
	

	return 0;
}