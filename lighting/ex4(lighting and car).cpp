#include "glew.h"
#include "glut.h"
#include <math.h>

// Определяется константа со значением “пи”
#define GL_PI 3.1415f

// Параметры поворота
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


//Вызывается для рисования сцены 
void RenderScene(void)
{
	GLenum mode = GL_POLYGON;
	bool solid = true;
	GLUquadricObj *quadObj = gluNewQuadric();
	GLfloat left = 0.0f, up = 0.0f, right = 0.0f;
	// Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	glRotatef(left, 0.0, 1.0, 0.0);
	glRotatef(up, 1.0, 0.0, 0.0);
	glRotatef(right, 0.0, 0.0, 1.0);

	// передние фары
	glPushMatrix();
	glTranslatef(0.75, 0.0, 2.01);
	glColor3f(1.0, 1.0, 1.0);
	gluDisk(quadObj, 0.0, 0.2, 10, 10);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.5, 0.0);
	gluDisk(quadObj, 0.0, 0.1, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, 0.0, 2.01);
	glColor3f(1.0, 1.0, 1.0);
	gluDisk(quadObj, 0.0, 0.2, 10, 10);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.5, 0.0);
	gluDisk(quadObj, 0.0, 0.1, 4, 4);
	glPopMatrix();

	// задние фары
	if (solid)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	glTranslatef(-0.75, -0.3, -2.01);
	glColor3f(1.0, 0.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRectf(0.1, 0.1, -0.1, -0.1);
	glTranslatef(1.5, 0.0, 0.0);
	glRectf(0.1, 0.1, -0.1, -0.1);
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glColor3f(0.5, 0.0, 0.3);
	glBegin(mode);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 0.5, 2.0);
	glVertex3f(-1.0, 0.5, 2.0);
	glVertex3f(-1.0, -0.5, 2.0);
	glVertex3f(1.0, -0.5, 2.0);
	glEnd();

	glBegin(mode);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(1.0, 0.5, -2.0);
	glVertex3f(1.0, -0.5, -2.0);
	glVertex3f(-1.0, -0.5, -2.0);
	glVertex3f(-1.0, 0.5, -2.0);
	glEnd();

	glBegin(mode);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.5, 2.0);
	glVertex3f(-1.0, 0.5, -2.0);
	glVertex3f(-1.0, -0.5, -2.0);
	glVertex3f(-1.0, -0.5, 2.0);
	glEnd();

	glBegin(mode);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.5, 2.0);
	glVertex3f(1.0, -0.5, 2.0);
	glVertex3f(1.0, -0.5, -2.0);
	glVertex3f(1.0, 0.5, -2.0);
	glEnd();

	glBegin(mode);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 0.5, -2.0);
	glVertex3f(-1.0, 0.5, 2.0);
	glVertex3f(1.0, 0.5, 2.0);
	glVertex3f(1.0, 0.5, -2.0);
	glEnd();

	glBegin(mode);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-1.0, -0.5, -2.0);
	glVertex3f(1.0, -0.5, -2.0);
	glVertex3f(1.0, -0.5, 2.0);
	glVertex3f(-1.0, -0.5, 2.0);
	glEnd();


	glPushMatrix();

	//переднее стекло
	glBegin(mode);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 0.5, 1.5);
	glVertex3f(-1.0, 0.5, 1.5);
	glVertex3f(-1.0, 1.0, 0.7);
	glVertex3f(1.0, 1.0, 0.7);
	glEnd();

	glBegin(mode);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(1.0, 1.0, -1.4);
	glVertex3f(1.0, 0.5, -1.8);
	glVertex3f(-1.0, 0.5, -1.8);
	glVertex3f(-1.0, 1.0, -1.4);
	glEnd();

	glBegin(mode);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.0, 1.0, 0.7);
	glVertex3f(-1.0, 0.5, 1.5);
	glVertex3f(-1.0, 0.5, -1.8);
	glVertex3f(-1.0, 1.0, -1.4);
	glEnd();

	glBegin(mode);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 1.0, 0.7);
	glVertex3f(1.0, 0.5, 1.5);
	glVertex3f(1.0, 0.5, -1.8);
	glVertex3f(1.0, 1.0, -1.4);
	glEnd();

	glBegin(mode);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, 1.0, -1.4);
	glVertex3f(-1.0, 1.0, 0.7);
	glVertex3f(1.0, 1.0, 0.7);
	glVertex3f(1.0, 1.0, -1.4);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, -0.5, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.01);
	glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glTranslatef(0.0, 0.0, -0.3);
	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.5, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.5, 1.0, 0.0);
	glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);

	glTranslatef(0.0, 0.0, -0.29);
	glRotatef(180, 0.5, 1.0, 0.0);
	glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.5, -1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.0, 1.0, 0.0);
	glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);

	glTranslatef(0.0, 0.0, -0.29);
	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, -0.5, -1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.5, 1.0, 0.0);
	glColor3f(0.1, 1.0, 0.5);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.01);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glTranslatef(0.0, 0.0, -0.3);
	glRotatef(180, 0.5, 1.0, 0.0);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, -0.5, -1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glColor3f(0.9, 0.5, 0.5);
	gluCylinder(quadObj, 0.05, 0.05, 1.4, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7, -0.5, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(quadObj, 0.05, 0.05, 1.4, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.5, -1.0);
	gluCylinder(quadObj, 0.05, 0.05, 2.0, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.5, -1.0);
	glColor3f(0.0, 0.0, 1.0);
	gluSphere(quadObj, 0.17, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 1.0, 1.0);
	glTranslatef(0.5, 0.5, 0.5);
	glRotatef(30, 1.0, 0.0, 0.0);
	// баранка
	if (solid)
		glutSolidTorus(0.05, 0.15, 3, 10);
	else
		glutWireTorus(0.05, 0.15, 3, 10);

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.0, 0.9);
	glTranslatef(-0.1, -0.4, 1.0);

	// мотор
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.2, -0.2, 0.3);
	glVertex3f(-0.2, -0.2, 0.3);
	glVertex3f(-0.2, 0.2, 0.3);
	glVertex3f(0.2, 0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.2, -0.2, -0.3);
	glVertex3f(0.2, 0.2, -0.3);
	glVertex3f(-0.2, 0.2, -0.3);
	glVertex3f(-0.2, -0.2, -0.3);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-0.2, 0.2, 0.3);
	glVertex3f(-0.2, 0.2, -0.3);
	glVertex3f(-0.2, -0.2, -0.3);
	glVertex3f(-0.2, -0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(0.2, 0.2, -0.3);
	glVertex3f(0.2, -0.2, -0.3);
	glVertex3f(0.2, -0.2, 0.3);
	glVertex3f(0.2, 0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-0.2, 0.2, 0.3);
	glVertex3f(-0.2, 0.2, -0.3);
	glVertex3f(0.2, 0.2, -0.3);
	glVertex3f(0.2, 0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-0.2, -0.2, -0.3);
	glVertex3f(-0.2, -0.2, 0.3);
	glVertex3f(0.2, -0.2, 0.3);
	glVertex3f(0.2, -0.2, -0.3);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.5, 0.5);
	glRotatef(60, 1.0, 0.0, 0.0);
	glColor3f(0.7, 0.7, 0.7);
	// рулевая колонка
	gluCylinder(quadObj, 0.03, 0.03, 1.0, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.1, 0.8);
	glRotatef(70, 1.0, 0.0, 0.0);
	gluCylinder(quadObj, 0.09, 0.09, 0.7, 10, 10);
	glPopMatrix();

	glPushMatrix();
	// кресло
	glColor3f(0.8, 0.0, 0.3);
	glTranslatef(0.5, -0.6, 0.22);
	glRotatef(70, 1.0, 0.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.3, 0.2, 0.4);
	glVertex3f(-0.3, 0.2, 0.4);
	glVertex3f(-0.3, -0.3, 0.4);
	glVertex3f(0.3, -0.3, 0.4);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.2, -0.7);
	glRotatef(-20, 1.0, 0.0, 0.0);

	glBegin(GL_POLYGON);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.3, 0.2, 0.4);
	glVertex3f(-0.3, 0.2, 0.4);
	glVertex3f(-0.3, -0.65, 0.4);
	glVertex3f(0.3, -0.65, 0.4);
	glEnd();
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

// Функция выполняет необходимую инициализацию 
// в контексте визуализации 
void SetupRC()
{
	// Параметры света
	// Яркий белый свет
	//GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat ambientLight[] = { 0.3f, 0.3f,  0.3f,  1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f,  0.7f,  1.0f };
	GLfloat specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };
	GLfloat specref[] = { 1.0f,   1.0f,  1.0f,   1.0f };

	glEnable(GL_DEPTH_TEST);	// Удаление скрытых частей
	glFrontFace(GL_CCW);// Многоугольники с обходом против часовой стрелки 
						//направлены наружу

						// Освещение материала
	glEnable(GL_LIGHTING);			// Активизация освещения	                

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glEnable(GL_LIGHT0);


	glEnable(GL_COLOR_MATERIAL);	// Активизируется согласование цветов

									//Рассеянный и диффузный цвета передней стороны объектов 
									//соответствуют тому, что указано в glColor

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Светло-синий цвет
	glClearColor(0.0f, 0.0f, 05.f, 1.0f);
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
	GLfloat nRange = 5.0f;
	// Предотвращение деления на ноль
	if (h == 0)
		h = 1;

	// Устанавливает поле просмотра по размерам окна
	glViewport(0, 0, w, h);

	// Обновляет стек матрицы проектирования
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Устанавливает объем отсечения с помощью отсекающих 
	// плоскостей (левая, правая, нижняя, верхняя, 
	// ближняя, дальняя) 
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Ambient Light Jet");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();

	return 0;
}
