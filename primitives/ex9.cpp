#include "glew.h"	// System and OpenGL Stuff
#include "glut.h"
#include <math.h>


// Определяется константа со значением “пи”
#define GL_PI 3.1415f


// Значения углов
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLboolean bCull, bDepth, bOutline;

// Вызывается для рисования сцены
void SetupRC()
{
	// Черный фон
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Цвет рисования выбирается зеленым 
	glColor3f(0.0f, 1.0f, 0.0f);
	// Цвет модели затенения выбирается неструктурированным 
	glShadeModel(GL_FLAT);
	// Многоугольники с обходом по часовой стрелке считаются 
	// направленными вперед; поведение изменено на обратное, 
	// поскольку мы используем вееры треугольников
	glFrontFace(GL_CW);
}

// Вызывается для рисования сцены 
void RenderScene(void)
{
	GLfloat x, y, angle;  // Здесь хранятся координаты и углы
	int iPivot = 1;	 	// Используется, чтобы отмечать
						// чередующиеся цвета 
						// Очищаем окно и буфер глубины
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Включаем отбор, если установлена метка
	if (bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	// Если установлена метка, активизируем проверку глубины
	if (bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	// Если установлена метка, рисуем заднюю сторону
	// в форме каркаса 
	if (bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	// Записываем состояние матрицы и выполняем поворот 
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// Начинаем веер треугольников
	glBegin(GL_TRIANGLE_FAN);
	// Вершина конуса является общей вершиной веера. Перемещаясь 
	// вверх по оси z, вместо окружности получаем конус
	glVertex3f(0.0f, 0.0f, 75.0f);
	//По циклу проходим окружность и задаем четные точки вдоль
	// окружности как вершины веера треугольников
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// Рассчитываем положения x и у следующей вершины
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// Чередуем красный и зеленый цвет
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		// Увеличиваем pivot на 1, чтобы в следующий раз 
		// изменить цвет
		iPivot++;
		// Задаем следующую вершину веера треугольников
		glVertex2f(x, y);
	}
	// Рисуем веер, имитирующий конус 
	glEnd();
	// Начинаем новый веер треугольников, имитирующий основание
	// конуса
	glBegin(GL_TRIANGLE_FAN);
	// Центром веера является начало координат 
	glVertex2f(0.0f, 0.0f);
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// Рассчитываем координаты х и у следующей вершины
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// Чередуем красный и зеленый цвета
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		// Увеличиваем pivot на единицу, чтобы в следующий раз
		// поменять цвета 
		iPivot++;
		// Задаем следующую вершину веера треугольников
		glVertex2f(x, y);
	}
	// Рисуем веер, имитирующий основание конуса
	glEnd();
	// Восстанавливаем преобразования 
	glPopMatrix();
	// Очищаем стек команд рисования
	glutSwapBuffers();
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

void subMenuDepthTestCheck(int choice) {
	switch (choice) {
	case 0:
		bDepth = false;
		glutPostRedisplay();
		break;
	case 1:
		bDepth = true;
		glutPostRedisplay();
		break;
	}
}

void subMenuCullTestCheck(int choice) {
	switch (choice) {
	case 0:
		bCull = false;
		glutPostRedisplay();
		break;
	case 1:
		bCull = true;
		glutPostRedisplay();
		break;
	}
}

void subMenuOutlineTestCheck(int choice) {
	switch (choice) {
	case 0:
		bOutline = false;
		glutPostRedisplay();
		break;
	case 1:
		bOutline = true;
		glutPostRedisplay();
		break;
	}
}
void MenuInit() {
	int menu = glutCreateMenu(MenuCheck);
	int subMenuDepthTest = glutCreateMenu(subMenuDepthTestCheck);
	int subMenuCullTest = glutCreateMenu(subMenuCullTestCheck);
	int subMenuOutlineTest = glutCreateMenu(subMenuOutlineTestCheck);

	glutSetMenu(subMenuDepthTest);
	glutAddMenuEntry("Depth test OFF", 0);
	glutAddMenuEntry("Depth test ON", 1);

	glutSetMenu(subMenuCullTest);
	glutAddMenuEntry("Cull test OFF", 0);
	glutAddMenuEntry("Cull test ON", 1);

	glutSetMenu(subMenuOutlineTest);
	glutAddMenuEntry("Outline test OFF", 0);
	glutAddMenuEntry("Outline test ON", 1);

	glutSetMenu(menu);
	glutAddSubMenu("Depth Test", subMenuDepthTest);
	glutAddSubMenu("Cull Test", subMenuCullTest);
	glutAddSubMenu("Outline Test", subMenuOutlineTest);
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