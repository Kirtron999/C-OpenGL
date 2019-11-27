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

void RenderScene(void)
{
	GLfloat x, y, z, angle; // Место хранения координат и углов
	GLfloat sizes[2];	 //Запоминаем диапазон размеров поддерживаемых точек

	GLfloat step;		 // Запоминаем поддерживаемый инкремент размеров точек
	GLfloat curSize;	 // Записываем размер текущих точек

						 // Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT);

	// Записывается состояние матрицы и выполняются повороты
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Цвет рисования
	glColor3f(rComponent, gComponent, bComponent);

	// Получаем диапазон размеров поддерживаемых точек и размер шага

	glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);

	// Задаем исходный размер точки
	curSize = sizes[0];

	// Задаем начальную координату z
	z = -50.0f;

	// Циклический проход по окружности три раза
	for (angle = 0.0f; angle <= (2.0f*3.1415f)*3.0f; angle += 0.1f)
	{
		// Расчет значений x и у точек окружности
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);

		// Задаем размер точки перед указанием примитива
		glPointSize(curSize);

		rComponent = (GLfloat)(rand() % 1000) / 1000;
		gComponent = (GLfloat)(rand() % 1000) / 1000;
		bComponent = (GLfloat)(rand() % 1000) / 1000;

		glColor3f(rComponent, gComponent, bComponent);

		// Рисуем точку
		glBegin(GL_POINTS);
		glVertex3f(x, y, z);
		glEnd();

		// Увеличиваем значение z и размер точки
		z += 0.5f;
		curSize += (step/2);
	}

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

void MenuInit() {
	int menu = glutCreateMenu(MenuCheck);
	glutSetMenu(menu);
	glutAddMenuEntry("Красный", 0);
	glutAddMenuEntry("Зеленый", 1);
	glutAddMenuEntry("Синий", 2);
	glutAddMenuEntry("Желтый", 3);
	glutAddMenuEntry("Белый", 4);
	glutAddMenuEntry("Оранжевый", 5);
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
	//MenuInit();
	glutMainLoop();

	return 0;
}