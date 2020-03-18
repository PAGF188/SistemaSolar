/*CONTROLES:
-f1-->zoom up
-f2--> zoom domn
-flechas-->mover camara
-click dereccho menu de telescopio
*/

#include <windows.h>	//Inclusion de variables de windows
#include <glut.h>	//Inclusion de GLUT
#include <GL/gl.h>	//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>	//Inclusion de librerias auxiliares	


#define INCREMENTO .01
#define DISTANCIA 50
#define PI 3.1416

float alpha = 0;
float beta = 0;
float fov = 45;

void myCamara(int w,int h) {

	/**/
	glMatrixMode(GL_PROJECTION);//Configuración de la matriz de proyeccion
	glLoadIdentity();
	gluPerspective(fov, (float)w/(float)h, 1.0, 2000.0);
	glTranslatef(0.0f, 0.0f, -200.f);
	gluLookAt(((float)DISTANCIA*(float)sin(alpha)*cos(beta)), ((float)DISTANCIA*(float)sin(beta)), ((float)DISTANCIA*cos(alpha)*cos(beta)), 0, 0, 0, 0, 1, 0);
}

//1 es respecto al obheto donde se encuentra la camara 2 a donde mira
void telescopio(float d1,float a1,float d2,float a2,int w, int h) {
	glMatrixMode(GL_PROJECTION);//Configuración de la matriz de proyeccion
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h,1.0,2000.0);
	gluLookAt(d1*cos(a1*PI / 180), d1*sin(a1*PI / 180), 0.0, d2*cos(a2*PI / 180), d2*sin(a2*PI / 180), 0.0, 0,0,1 );
}

//lo primero es la tierra, lo segundo el satelite
void telescopioSatelite(float d1, float a1, float d2, float a2,int w, int h) { 
	glMatrixMode(GL_PROJECTION);//Configuración de la matriz de proyeccion
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 2000.0);
	gluLookAt(d1*cos(a1*PI / 180), d1*sin(a1*PI / 180), 0.0,				//desde la tierra
		d2*cos(3.5*a2*PI / 180)+d1*cos(a1*PI/180), d2*sin(3.5*a2*PI / 180)+d1*sin(a1*PI/180), 0.0,	//a la luna		
		0, 0, 1);
}


/**************************************
FUNCION DE CONTROL DE TECLAS ESPECIAIS:
***************************************
*/
void myTeclasespeciales(int cursor, int x, int y)
{
	switch (cursor)
	{
		//Traslaciones:
	case GLUT_KEY_F1:
		fov -= 1.0;
		break;
	case GLUT_KEY_F2:
		fov += 1.0;
		break;
	case GLUT_KEY_F3:

		break;
	case GLUT_KEY_F4:
		break;
	case GLUT_KEY_F5:

		break;
	case GLUT_KEY_F6:

		break;
		//Giros:
	case GLUT_KEY_UP:
		beta += INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta -= INCREMENTO;
		break;
	case GLUT_KEY_RIGHT:
		alpha -= INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alpha += INCREMENTO;
		break;
	default:
		break;
	}

	if (alpha >= PI * 2.0 && alpha <= 0) alpha = 0;
	if (beta >= PI * 2.0 && beta <= 0) beta = 0; //hay que repasarlo para evitar el salto
	glutPostRedisplay();
}