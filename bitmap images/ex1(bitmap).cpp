#include "glew.h"	
#include "glut.h"
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()


static GLfloat rComponent, bComponent, gComponent;

//битовый образ 1
GLubyte cross[128] = {
	0x00, 0x00, 
	0x00, 0x00, 
	0x07, 0xe0, 
	0x07, 0xe0, 
	0x07, 0xe0, 
	0x3f, 0xfc, 
	0x3f, 0xfc, 
	0x3f, 0xfc, 
	0x3f, 0xfc, 
	0x3f, 0xfc, 
	0x3f, 0xfc, 
	0x07, 0xe0, 
	0x07, 0xe0, 
	0x07, 0xe0, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00, 
	0x00, 0x00 };


GLubyte bi2[] = {
	0x00, 0x00,
	0x00, 0x00,
	0x3f, 0x00,
	0x7f, 0x00,
	0x40, 0x80,
	0x00, 0x00,
	0x00, 0x00,
	0x33, 0x00,
	0x33, 0x00,
	0x33, 0x00
};


//////////////////////////////////////////////////////////////////
// Функция выполняет всю необходимую инициализацию в контексте визуализации
void SetupRC()
{
	// Черный фон
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


//////////////////////////////////////////////////////////
// Устанавливается система координат, согласованная с координатами
void ChangeSize(int w, int h)
{
	// Предотвращает деление на нуль, когда окно слишком маленькое 
	// (нельзя сделать окно нулевой ширины). 
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	// Система координат обновляется перед модификацией
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Псевдокоординаты окна
	gluOrtho2D(0.0, (GLfloat)w, 0.0f, (GLfloat)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/////////////////////////////////////////////////////////////////
// Вызывается для рисования сцены
void RenderScene(void)
{
	int x, y;

	srand(time(0));
	// Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT);

	// Устанавливается белый цвет
	glColor3f(1.0f, 1.0f, 1.0f);
	// Цикл из 16 строк и столбцов
	for (y = 0; y < 8; y++)
	{
		rComponent = (GLfloat)(rand() % 1000) / 1000;
		gComponent = (GLfloat)(rand() % 1000) / 1000;
		bComponent = (GLfloat)(rand() % 1000) / 1000;
		glColor3f(rComponent, bComponent, gComponent);
		// Устанавливается растровое положение данного "квадрата”
		glRasterPos2i(0, y * 32);
		for (x = 0; x < 16 - y; x++)

			// Рисуется битовый образ, меняется растровое положение
			glBitmap(16, 20, 0.0, 0.0, 32.0, 0.0, cross);
	}


	for (y = 0; y < 8; y++)
	{
		rComponent = (GLfloat)(rand() % 1000) / 1000;
		gComponent = (GLfloat)(rand() % 1000) / 1000;
		bComponent = (GLfloat)(rand() % 1000) / 1000;
		glColor3f(rComponent, bComponent, gComponent);
		// Устанавливается растровое положение данного "квадрата”
		glRasterPos2i(0, y * 32 + 8 * 32);
		for (x = 16; x > 7 - y; x--)

			// Рисуется битовый образ , меняется растровое положение
			glBitmap(10, 10, 0.0, 0.0, 32.0, 0.0, bi2);
	}


	// Переключает буферы
	glutSwapBuffers();
}


/////////////////////////////////////////////////////////////
// Точка входа основной программы
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutCreateWindow("OpenGL Bitmaps");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	SetupRC();
	glutMainLoop();

	return 0;
}
