
#define CR 0.0175
#define TIEMPO 41   //41

typedef struct {
	float distancia;
	float velocidad_trans;
	float angulo_trans;
	float velocidad_rotacion;
	float angulo_rotacion;
	int tamanho;
	int lista_render;
} planeta;

extern void myCamara(int w, int h);
extern void telescopio(float d1, float a1, float d2, float a2,int w, int h);
extern void telescopioSatelite(float d1, float a1, float d2, float a2,int w, int h);
extern void myTeclado(unsigned char tras, int x, int y);
extern void myTeclasespeciales(int cursor, int x, int y);