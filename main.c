//SISTEMA SOLAR 1.2
//DIBUJADO SOBRE EL PLANO X,Y

/*CONTROLES:
-f1-->zoom up
-f2--> zoom domn
-flechas-->mover camara
-click dereccho menu de telescopio
-s-->mostrar orbitas
-n-->no mostrar orbitas
r-->SS kiptoniano
b-->SS normal
*/

#include <windows.h>	//Inclusion de variables de windows
#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>		//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	
#include "planeta.h"

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

//para las texturas
GLuint texturas[11];

int W_WIDTH = 720;	 //Ancho de la ventana
int W_HEIGHT = 720;	 //Alto de la ventana

int rojo = 0; //para color del sol

//iluminacion
GLfloat ambiente[] = { 0.2f,0.2f,0.2f,0.0f };    //luz ambiental
GLfloat difusa[] = { 1.0f,1.0f,1.0f,1.0f };		//lamber o difusa;  Variacion asociada a cambios de intensidad o posicion del foco
GLfloat especular[] = { 1.0f,1.0f,1.0f,1.0f };  //especular o de phong; Variacion asociada a cambios de intensidad, posicion y PUNTO DE VISTA(OBSERVADOR) 

//falta la emisiva-->(APARIENCIA DE QUE EL OBJETO EMITE LUZ (se le prodira poner al sol))
GLfloat SpecRef[] = { 1.0f,1.0f,1.0f,1.0f }; //comportancion del material en funcoin de la componente especular

//para luz local ultima compionente vale 1
//posicion
GLfloat posicionLuz[] = { 0.0f,0.0f,0.0f,1.0f }; 
//direccion
GLfloat dir[] = { 1.0f,1.0f,1.0f };



//Están el segundo archivo
extern int myEsfera();
unsigned int indice;
unsigned int indice_ejes;

int camara=1; //para seleccionar el tipo de camara
int b = 1; //para ver o no las orbitas

planeta sol = { 0,0,0,10,0,8,0 };
planeta mercurio = { 11,10,0,30,0,1.5,0 };
planeta venus = { 22,7.5,0,25,0,2.5,0 };
planeta tierra = { 36,5,0,20,0,3.5,0};
planeta marte = { 50,3,0,17,0,3.7,0 };
planeta jupiter = { 58,3.5,0,14,0,4.8,0 };
planeta saturno = { 70,2.5,0,10,0,5.5,0 };
planeta urano = { 80,2,0,17,0,6.8,0 };
planeta neptuno = { 100,1,0,4,0,7.3,0 };
planeta luna = { 6,2,0,10,0,1,0 };
planeta iss = { 4,1,0,10,0,1,0 };

//vector director
float dirx = 0;
float diry = 0;
//posicion proyectil
float px = 0;
float py = 0;
boolean pr= FALSE;

void myEjes();				//dibuja los ejes
void dibujar(planeta p);	//dibuja un planeta
int myMovimiento();			//permite simular el movimiento de los planetas
void myDisplay(void);		//funcion de dibujo	
void openGlInit();			//inicializar carateristicas
void menu(void);			//menu
void onMenu(int opcion);    //para gestionar el menu


void myEjes() {

	indice_ejes = glGenLists(1);
	glNewList(indice_ejes, GL_COMPILE);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0, 10.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEndList();
}

void dibujar(planeta p,int i) {
	glPushMatrix();
		glRotatef(p.angulo_trans,0.0,0.0,1);				//para su traslacion
		glTranslatef(p.distancia, 0.0, 0.0);
		glPushMatrix();
			glRotatef(p.angulo_rotacion,0.0,0.0,1);		//para su rotacion
			//glCallList(indice_ejes);
			glScalef(p.tamanho,p.tamanho,p.tamanho);
			glBindTexture(GL_TEXTURE_2D, texturas[i]);
			glCallList(indice);
			glBindTexture(GL_TEXTURE_2D, 0);
			glEnd();
		glPopMatrix();
	glPopMatrix();
}

//hecho a partir de la parametrizacion de la circunferencia para calcular la trayectoria
//float b indica si se dibuja blanco o nrego
void dibujar_orbitas(planeta p) {
	
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	for (float i = 0; i < 360; i=i+0.01) {
		glVertex3f(p.distancia*sin(i), p.distancia*cos(i), 0.0f);
	}
	glEnd();
}

int  myMovimiento() {
	sol.angulo_trans += sol.velocidad_trans;
	if (sol.angulo_rotacion > 360) sol.angulo_rotacion -= 360;
	sol.angulo_rotacion += sol.velocidad_rotacion;
	if (sol.angulo_rotacion > 360) sol.angulo_rotacion -= 360;

	mercurio.angulo_trans += mercurio.velocidad_trans;
	if (mercurio.angulo_rotacion > 360) mercurio.angulo_rotacion -= 360;
	mercurio.angulo_rotacion += mercurio.velocidad_rotacion;
	if (mercurio.angulo_rotacion > 360) mercurio.angulo_rotacion -= 360;

	venus.angulo_trans += venus.velocidad_trans;
	if (venus.angulo_rotacion > 360) venus.angulo_rotacion -= 360;
	venus.angulo_rotacion += venus.velocidad_rotacion;
	if (venus.angulo_rotacion > 360) venus.angulo_rotacion -= 360;

	tierra.angulo_trans += tierra.velocidad_trans;
	if (tierra.angulo_rotacion > 360) tierra.angulo_rotacion -= 360;
	tierra.angulo_rotacion += tierra.velocidad_rotacion;
	if (tierra.angulo_rotacion > 360) tierra.angulo_rotacion -= 360;

	marte.angulo_trans += marte.velocidad_trans;
	if (marte.angulo_rotacion > 360) marte.angulo_rotacion -= 360;
	marte.angulo_rotacion += marte.velocidad_rotacion;
	if (marte.angulo_rotacion > 360) marte.angulo_rotacion -= 360;

	jupiter.angulo_trans += jupiter.velocidad_trans;
	if (jupiter.angulo_rotacion > 360) jupiter.angulo_rotacion -= 360;
	jupiter.angulo_rotacion += jupiter.velocidad_rotacion;
	if (jupiter.angulo_rotacion > 360) jupiter.angulo_rotacion -= 360;

	saturno.angulo_trans += saturno.velocidad_trans;
	if (saturno.angulo_rotacion > 360) saturno.angulo_rotacion -= 360;
	saturno.angulo_rotacion += saturno.velocidad_rotacion;
	if (saturno.angulo_rotacion > 360) saturno.angulo_rotacion -= 360;

	urano.angulo_trans += urano.velocidad_trans;
	if (urano.angulo_rotacion > 360) urano.angulo_rotacion -= 360;
	urano.angulo_rotacion += urano.velocidad_rotacion;
	if (urano.angulo_rotacion > 360) urano.angulo_rotacion -= 360;

	neptuno.angulo_trans += neptuno.velocidad_trans;
	if (neptuno.angulo_rotacion > 360) neptuno.angulo_rotacion -= 360;
	neptuno.angulo_rotacion += neptuno.velocidad_rotacion;
	if (neptuno.angulo_rotacion > 360) neptuno.angulo_rotacion -= 360;

	luna.angulo_trans += luna.velocidad_trans;
	if (luna.angulo_rotacion > 360) luna.angulo_rotacion -= 360;
	luna.angulo_rotacion += luna.velocidad_rotacion;
	if (luna.angulo_rotacion > 360) luna.angulo_rotacion -= 360;

	iss.angulo_trans += iss.velocidad_trans;
	if (iss.angulo_rotacion > 360) iss.angulo_rotacion -= 360;
	iss.angulo_rotacion += iss.velocidad_rotacion;
	if (iss.angulo_rotacion > 360) iss.angulo_rotacion -= 360;


	glutPostRedisplay();	//vuelve a llamar a display
	glutTimerFunc(TIEMPO, myMovimiento,0);
}


// Funcion de dibujo, funcion principal
void myDisplay(void) {

	//glViewport(0, 0, W_WIDTH, W_HEIGHT);
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//control de camara
	//libre
	if (camara == 1) {
		myCamara(W_WIDTH,W_HEIGHT);     
	}
	//sol
	if (camara == 2) {
		telescopio(tierra.distancia, tierra.angulo_trans, sol.distancia, sol.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	//marte
	if (camara == 3) {
		telescopio(tierra.distancia, tierra.angulo_trans, marte.distancia, marte.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	//neptuno
	if (camara == 4) {
		telescopio(tierra.distancia, tierra.angulo_trans, neptuno.distancia, neptuno.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	//venus
	if (camara == 5) {
		telescopio(tierra.distancia, tierra.angulo_trans, venus.distancia, venus.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	if (camara == 6) {
		telescopioSatelite(tierra.distancia, tierra.angulo_trans, luna.distancia, luna.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	if (camara == 7) {
		telescopio(tierra.distancia, tierra.angulo_trans, mercurio.distancia, mercurio.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	if (camara == 8) {
		telescopio(tierra.distancia, tierra.angulo_trans, jupiter.distancia, jupiter.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	if (camara == 9) {
		telescopio(tierra.distancia, tierra.angulo_trans, saturno.distancia, saturno.angulo_trans, W_WIDTH, W_HEIGHT);
	}
	if (camara == 10) {
		telescopio(tierra.distancia, tierra.angulo_trans, urano.distancia, urano.angulo_trans, W_WIDTH, W_HEIGHT);
	}

	// Inicializamos la matriz del modelo a la identidad
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);  //dibujamos el sol antes de encender las luces

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//SOL
	glPushMatrix();
		glRotatef(sol.angulo_trans, 0.0, 0.0, 0);				//para su traslacion
		glTranslatef(sol.distancia, 0.0, 0.0);
		glPushMatrix();
			glRotatef(sol.angulo_rotacion, 0.0, 0.0, 1);		//para su rotacion
			//glCallList(indice_ejes);
			if(rojo==0)
				glColor3f(1.0f, 1.0f, 1.0f);
			else
				glColor3f(1.0f, 0.0f, 0.0f);
			glScalef(sol.tamanho, sol.tamanho, sol.tamanho);
			glBindTexture(GL_TEXTURE_2D, texturas[0]);
			glCallList(indice);
			glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	glPopMatrix();

	glEnable(GL_LIGHTING);

	//resto de planteas
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);
	if(b==1)
		dibujar_orbitas(mercurio);
	dibujar(mercurio,1);
	if(b==1)
		dibujar_orbitas(venus);
	dibujar(venus,2);

	//tierra es especial proque tiene satelites
	if (b == 1)
		dibujar_orbitas(tierra);
	glPushMatrix();
		glRotatef(tierra.angulo_trans, 0.0, 0.0, 1.0);				//para su traslacion
		glTranslatef(tierra.distancia, 0.0, 0.0);
		glPushMatrix();
			glRotatef(tierra.angulo_rotacion, 0.0, 0.0, 1.0);		//para su rotacion
			//glCallList(indice_ejes);
			glColor3f(1.0f, 1.0f, 1.0f);
			glScalef(tierra.tamanho, tierra.tamanho, tierra.tamanho);
			glBindTexture(GL_TEXTURE_2D, texturas[3]);
			glCallList(indice);
			glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		dibujar(luna,9);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		dibujar(iss,10);
	glPopMatrix();

	if (b == 1)
		dibujar_orbitas(marte);
	dibujar(marte,4);
	if (b == 1)
		dibujar_orbitas(jupiter);
	dibujar(jupiter,5);
	if (b == 1)
		dibujar_orbitas(saturno);
	dibujar(saturno,6);
	if (b == 1)
		dibujar_orbitas(urano);
	dibujar(urano,7);
	if (b == 1)
		dibujar_orbitas(neptuno);
	dibujar(neptuno,8);

	//actualizamos la posicion del proyectil
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (pr == FALSE) {
		glPushMatrix();
			glRotatef(tierra.angulo_trans, 0.0, 0.0, 1);				//para su traslacion
			glTranslatef(tierra.distancia, 0.0, 0.0);
			glScalef(1, 1, 1);
			glCallList(indice);
		glPopMatrix();
	}
	else {
		px += dirx*0.005;
		py += diry*0.005;
		glPushMatrix();
			glTranslatef(px, py, 0.0);
			glScalef(1, 1, 1);
			glCallList(indice);
		glPopMatrix();
		//deteccion de colision con el sol
		if (sqrt(px*px + 0 + py * py) <=8) {
			//colision
			printf("colision");
			pr = FALSE;
		}
	}



	glFlush();
	glutSwapBuffers();

}
void onMenu(int opcion) {
	if (opcion == 1) {
		camara = 1;
	}
	if (opcion == 2) {
		camara = 2;
	}
	if (opcion == 3) {
		camara = 3;
	}
	if (opcion == 4) {
		camara = 4;
	}
	if (opcion == 5) {
		camara = 5;
	}
	if (opcion == 6) {
		camara = 6;
	}
	if (opcion == 7) {
		camara = 7;
	}
	if (opcion == 8) {
		camara = 8;
	}
	if (opcion == 9) {
		camara = 9;
	}
	if (opcion == 10) {
		camara = 10;
	}
	glutPostRedisplay();
}
void menu(void) {
	int menu;
	menu = glutCreateMenu(onMenu);   //le decimos que la funcion que lo gestiona es onMenu
	glutAddMenuEntry("Lbre", 1);
	glutAddMenuEntry("Sol", 2);
	glutAddMenuEntry("Marte",3);
	glutAddMenuEntry("Neptuno",4);
	glutAddMenuEntry("Venus",5);
	glutAddMenuEntry("Luna", 6);
	glutAddMenuEntry("Mercurio", 7);
	glutAddMenuEntry("Jupiter", 8);
	glutAddMenuEntry("Saturno", 9);
	glutAddMenuEntry("Urano", 10);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	glEnable(GL_NORMALIZE); // Normaliza las normales

	glEnable(GL_TEXTURE_2D);
}

void initLuz() {

	//las tres componentes
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular);

	//recordar que se trata de una luz local
	//posicion
	glLightfv(GL_LIGHT0, GL_POSITION, posicionLuz);
	//direccion
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);

	//como se trata de un foco ponemos el angulo de corte completo
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 179.0f);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL); 
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

	//carateristicas de los materiales
	glMaterialfv(GL_FRONT, GL_SPECULAR, SpecRef);	
	glMateriali(GL_FRONT, GL_SHININESS, 5);			

	glShadeModel(GL_SMOOTH);
}

void changeSize(GLint newWidth, GLint newHeight) {
	W_WIDTH = newWidth;
	W_HEIGHT = newHeight;
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)newWidth / (float)newHeight, 1.0, 2000.0);
	glutPostRedisplay();
}

void myTeclado(unsigned char tras, int x, int y)
{
	switch (tras) {
		case 'n':
			b = 0;
			break;
		case 's':
			b = 1;
			break;
		case 'r':
			difusa[1] = 0.0f; difusa[2] = 0.0f;
			especular[1] = 0.0f; especular[2] = 0.0f;
			SpecRef[1] = 0.0f; SpecRef[2] = 0.0f;
			initLuz();
			rojo = 1;
			break;
		case 'b':
			difusa[1] = 1.0f; difusa[2] = 1.0f;
			especular[1] = 1.0f; especular[2] = 1.0f;
			SpecRef[1] = 1.0f; SpecRef[2] = 1.0f;
			initLuz();
			rojo = 0;
			break;
		//proyectil
		case 'p':
			pr = TRUE;
			//guardamos posicion del proyectil al momento de lanzarse
			px = tierra.distancia*cos(tierra.angulo_trans);
			py = tierra.distancia*sin(tierra.angulo_trans);
			//creamos el vector director del movimineto
			dirx = 0.0 - px;
			diry = 0.0 - py;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void auxiliarTextura(char *path, int i) {
	int ancho, alto, canales;
	unsigned char *data = NULL;

	glBindTexture(GL_TEXTURE_2D, texturas[i]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(path, &ancho, &alto, &canales, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);
}

void cargaTextura() {

	glGenTextures(10, &texturas);
	auxiliarTextura("sol.jpg", 0);
	auxiliarTextura("mercurio.jpg", 1);
	auxiliarTextura("venus.jpg", 2);
	auxiliarTextura("tierra.jpg", 3);
	auxiliarTextura("marte.png", 4);
	auxiliarTextura("jupiter.jpg", 5);
	auxiliarTextura("saturno.jpg", 6);
	auxiliarTextura("urano.jpg", 7);
	auxiliarTextura("neptuno.jpg", 8);
	auxiliarTextura("luna.jpg", 9);
	auxiliarTextura("ss.jpg", 10);
	
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("SOLARIUM");

	openGlInit();
	initLuz();
	//Eventos teclado, teclas especiales //PARA LSO EVENTOS de usuario
	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);

	//creamos las 2 listas
	indice = myEsfera();
	myEjes();  //para los ejes
	//Evento de dibujo
	glutDisplayFunc(myDisplay);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//rescalado
	glutReshapeFunc(changeSize);

	myMovimiento();
	menu();
	cargaTextura();

	// Empieza en bucle principal
	glutMainLoop();
	return 0;
}

