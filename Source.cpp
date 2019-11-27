// подключаем заголовочные файлы библиотек
#include "glew.h"
#include "glut.h"
// Исходное положение и размер прямоугольника
GLfloat x1 = 0.0f;  GLfloat y1 = 0.0f; GLfloat rsize = 25;
// Величина шага в направлениях х и у (число пикселей,
// на которые на каждом шаге перемещается прямоугольник)
GLfloat xstep = 1.0f;  GLfloat ystep = 1.0f; GLfloat angle = 0;
// Отслеживание изменений ширины и высоты окна
GLfloat windowWidth;  GLfloat windowHeight;
// Коэффициенты масштабирования
GLfloat xCoefficient = 1.0f;  GLfloat yCoefficient = 1.0f;
// Шаги коэффициентов масштабирования
GLfloat xCoefficientStep = 0.0f;  GLfloat yCoefficientStep = 0.0f;
//Вызывается для рисования сцены 
void RenderScene(void)
{
	// Очищаем окно,  используя текущий цвет очистки 
	glClear(GL_COLOR_BUFFER_BIT);
	// В качестве текущего цвета рисования задает красный //RGB
	glColor3f(1.0f, 1.0f, 0.0f);
	// Пробуем нарисовать квадрат

	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(angle, 0, 0, 1);
	glScalef(xCoefficient, yCoefficient, 0);
	glBegin(GL_QUADS);
		glVertex3f(-15.0f, -15.0f, 0.0f);
		glVertex3f(15.0f, -15.0f, 0.0f);
		glVertex3f(15.0f, 15.0f, 0.0f);
		glVertex3f(-15.0f, 15.0f, 0.0f);
	glEnd();
	glLoadIdentity();

	// Очищает очередь текущих команд и переключает буферы
	glutSwapBuffers();
}
//Вызывается библиотекой GLUT в холостом состоянии  (окно не меняет 	
//размера и не перемещается) 
void TimerFunction(int value)
{
	angle += 3;
	if (xCoefficient >= 2){
		xCoefficientStep = -0.05f;
		yCoefficientStep = -0.05f;
	}
	else if (xCoefficient <= 1) {
		xCoefficientStep = 0.05f;
		yCoefficientStep = 0.05f;
	}
	xCoefficient += xCoefficientStep;
	yCoefficient += yCoefficientStep;
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}
//Задает состояние визуализации 
void SetupRC(void)
{
	// Устанавливает в качестве цвета очистки синий
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
//Вызывается библиотекой GLUT при изменении размеров окна 
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	// Предотвращает деление на нуль 
	if (h == 0) h = 1;
	// Устанавливает поле просмотра с размерами окна
	glViewport(0, 0, w, h);
	// Обновляет систему координат
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//С помощью плоскостей отсечения  (левая,  правая,  нижняя, 
	// верхняя,  ближняя,  дальняя)  устанавливает объем отсечения 	
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
	{
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho(-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else
	{
		windowWidth = 100 * aspectRatio; windowHeight = 100;
		glOrtho(-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//Точка входа основной программы 
void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); glutCreateWindow("Bounce");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
}
