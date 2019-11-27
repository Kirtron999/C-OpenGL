#include "glew.h"	// System and OpenGL Stuff
#include "glut.h"
#include <math.h>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()


// Определяется константа со значением “пи”
#define GL_PI 3.1415f

// Значения углов
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat rComponent = 1.0f, gComponent = 0.0f, bComponent = 0.0f;
static GLfloat step = 0.1f;

void RenderScene(void)
{
	GLfloat x, y, z, angle; // Место хранения координат и углов

						 // Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT);

	// Записывается состояние матрицы и выполняются повороты
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Задаем размер точки перед указанием примитива

	rComponent = (GLfloat)(rand() % 1000) / 1000;
	gComponent = (GLfloat)(rand() % 1000) / 1000;
	bComponent = (GLfloat)(rand() % 1000) / 1000;

	glColor3f(rComponent, gComponent, bComponent);

	// Рисуем точку
	glBegin(GL_LINE_STRIP);
	z = -50.0f;
	for (angle = 0.0f; angle <= (2.0f*GL_PI)*3.0f; angle += step)
	{
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// Задаем точку и немного смещаем значение z 
		glVertex3f(x, y, z);
		z += (0.5f * (step/0.1));
	}
	// Рисует точки 
	glEnd();

	// Восстанавливаем преобразования
	glPopMatrix();

	// Очищаем стек команд рисования
	glutSwapBuffers();
}

// Функция выполняет необходимую инициализацию 
// в контексте визуализации 
void SetupRC()
{
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

	// Обновляем окно
	glutPostRedisplay();
}

void MenuCheck(int choice) {
	switch (choice) {

	}
}

void SubMenuCheck(int choice) {
	switch (choice) {
	case 0:
		step = 1.0f;
		glutPostRedisplay();
		break;
	case 1:
		step = 0.5f;
		glutPostRedisplay();
		break;
	case 2:
		step = 0.1f;
		glutPostRedisplay();
		break;
	}
}

void MenuInit() {
	int menu = glutCreateMenu(MenuCheck);
	int subMenu = glutCreateMenu(SubMenuCheck);
	glutSetMenu(subMenu);
	glutAddMenuEntry("Малая", 0);
	glutAddMenuEntry("Средняя", 1);
	glutAddMenuEntry("Высокая", 2);
	glutSetMenu(menu);
	glutAddSubMenu("Аппроксимация", subMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	glutCreateWindow("Points Example");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	MenuInit();
	glutMainLoop();

	return 0;
}