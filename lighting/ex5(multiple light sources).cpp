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
static GLenum mode = GL_LINE_STRIP;

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

	glColor3f(rComponent, gComponent, bComponent);

	// Рисуем точку
	glBegin(mode);
	z = -50.0f;
	for (angle = 0.0f; angle <= (2.0f*GL_PI)*3.0f; angle += step)
	{
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// Задаем точку и немного смещаем значение z 
		glVertex3f(x, y, z);
		z += (0.5f * (step / 0.1));
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

void subMenuApproximationCheck(int choice) {
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

void subMenuObjModeCheck(int choice) {
	switch (choice) {
	case 0:
		mode = GL_LINE_STRIP;
		glutPostRedisplay();
		break;
	case 1:
		mode = GL_LINE_LOOP;
		glutPostRedisplay();
		break;
	}
}

void subMenuColorCheck(int choice) {
	switch (choice) {
	case 0:
		rComponent = 1.0f;
		gComponent = 0.0f;
		bComponent = 0.0f;
		glutPostRedisplay();
		break;
	case 1:
		rComponent = 0.0f;
		gComponent = 1.0f;
		bComponent = 0.0f;
		glutPostRedisplay();
		break;
	case 2:
		rComponent = 0.0f;
		gComponent = 0.0f;
		bComponent = 1.0f;
		glutPostRedisplay();
		break;
	case 3:
		rComponent = 1.0f;
		gComponent = 1.0f;
		bComponent = 0.0f;
		glutPostRedisplay();
		break;
	case 4:
		rComponent = 1.0f;
		gComponent = 1.0f;
		bComponent = 1.0f;
		glutPostRedisplay();
		break;
	case 5:
		rComponent = 0.98f;
		gComponent = 0.625f;
		bComponent = 0.12f;
		glutPostRedisplay();
		break;
	}
}

void SubMenuCheck(int choice) {
	switch (choice) {
	case 0:
		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
		break;
	case 1:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
		break;
	case 2:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		break;
	}
	glutPostRedisplay();
}

void MenuInit() {
	int menu = glutCreateMenu(MenuCheck);
	int subMenuApproximation = glutCreateMenu(subMenuApproximationCheck);
	int subMenuObjMode = glutCreateMenu(subMenuObjModeCheck);
	int subMenuColor = glutCreateMenu(subMenuColorCheck);
	int subMenu = glutCreateMenu(SubMenuCheck);

	glutSetMenu(subMenu);
	glutAddMenuEntry("Without smoothing", 0);
	glutAddMenuEntry("Fast smoothing", 1);
	glutAddMenuEntry("Nicest smoothing", 2);

	glutSetMenu(subMenuApproximation);
	glutAddMenuEntry("Малая", 0);
	glutAddMenuEntry("Средняя", 1);
	glutAddMenuEntry("Высокая", 2);

	glutSetMenu(subMenuObjMode);
	glutAddMenuEntry("GL_LINE_STRIP", 0);
	glutAddMenuEntry("GL_LINE_LOOP", 1);

	glutSetMenu(subMenuColor);
	glutAddMenuEntry("Красный", 0);
	glutAddMenuEntry("Зеленый", 1);
	glutAddMenuEntry("Синий", 2);
	glutAddMenuEntry("Желтый", 3);
	glutAddMenuEntry("Белый", 4);
	glutAddMenuEntry("Оранжевый", 5);

	glutSetMenu(menu);
	glutAddSubMenu("Аппроксимация", subMenuApproximation);
	glutAddSubMenu("Выбор типа объекта", subMenuObjMode);
	glutAddSubMenu("Выбор цвета", subMenuColor);
	glutAddSubMenu("Smoothing", subMenu);
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