#include "glew.h"
#include "glut.h"
#include <cstdlib>
#include <ctime>

// Величина поворота
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


void SetRandomColor() {
	static GLfloat rComponent = 1.0f, gComponent = 0.0f, bComponent = 0.0f;

	rComponent = (GLfloat)(rand() % 1000) / 1000;
	gComponent = (GLfloat)(rand() % 1000) / 1000;
	bComponent = (GLfloat)(rand() % 1000) / 1000;

	glColor3f(rComponent, gComponent, bComponent);
}

// Вызывается для рисования сцены 
void RenderScene(void)
{
	// Очистка окна текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Восстанавливаем нормальное состояние рисования
	glShadeModel(GL_SMOOTH);

	// Рисуем параллелепипед
	
	glBegin(GL_QUADS);
	//перед
	SetRandomColor();
	glVertex3f(-40.0f, -30.0f, 40.0f);
	SetRandomColor();
	glVertex3f(0.0f, -30.0f, 40.0f);
	SetRandomColor();
	glVertex3f(40.0f, 30.0f, 40.0f);
	SetRandomColor();
	glVertex3f(0.0f, 30.0f, 40.0f);

	//верх
	SetRandomColor();
	glVertex3f(0, 30, 40);
	SetRandomColor();
	glVertex3f(40, 30, 40);
	SetRandomColor();
	glVertex3f(40, 30, 0);
	SetRandomColor();
	glVertex3f(0, 30, 0);

	//правый бок
	SetRandomColor();
	glVertex3f(0, -30, 0);
	SetRandomColor();
	glVertex3f(40, 30, 0);
	SetRandomColor();
	glVertex3f(40, 30, 40);
	SetRandomColor();
	glVertex3f(0, -30, 40);
	
	

	//зад
	SetRandomColor();
	glVertex3f(-40, -30, 0);
	SetRandomColor();
	glVertex3f(0, 30, 0);
	SetRandomColor();
	glVertex3f(40, 30, 0);
	SetRandomColor();
	glVertex3f(0, -30, 0);

	//левый бок
	SetRandomColor();
	glVertex3f(-40, -30, 0);
	SetRandomColor();
	glVertex3f(-40, -30, 40);
	SetRandomColor();
	glVertex3f(0, 30, 40);
	SetRandomColor();
	glVertex3f(0, 30, 0);

	//низ
	SetRandomColor();
	glVertex3f(-40, -30, 40);
	SetRandomColor();
	glVertex3f(-40, -30, 0);
	SetRandomColor();
	glVertex3f(0, -30, 0);
	SetRandomColor();
	glVertex3f(0, -30, 40);
	glEnd();

	glPopMatrix();
	// Очищает очередь текущих команд
	glutSwapBuffers();
}
// Эта функция выполняет необходимую инициализацию в контексте 
// визуализации 
void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	// Черный фон
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	if (key > 356.0f)
		xRot = 0.0f;

	if (key < -1.0f)
		xRot = 355.0f;

	if (key > 356.0f)
		yRot = 0.0f;

	if (key < -1.0f)
		yRot = 355.0f;

	// Refresh the Window
	glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f;

	// Предотвращает деление на нуль
	if (h == 0)
		h = 1;

	// Устанавливаем размеры поля просмотра равными размерам окна
	glViewport(0, 0, w, h);

	// Устанавливаем перспективную систему координат
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Устанавливает объем отсечения с помощью отсекающих 
	// плоскостей (левая, правая, нижняя, верхняя, ближняя, дальняя) 

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Обновляется стек матриц проекции модели
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(0)));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("RGB Triangle");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();

	return 0;
}
