#include "glew.h"	// System and OpenGL Stuff
#include "glut.h"
#include <math.h>
#include <cstdlib> // ��� ������� rand() � srand()
#include <ctime> // ��� ������� time()


// ������������ ��������� �� ��������� ���
#define GL_PI 3.1415f

// �������� �����
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat rComponent = 1.0f, gComponent = 0.0f, bComponent = 0.0f;
static GLfloat step = 0.1f;

void RenderScene(void)
{
	GLfloat x, y, z, angle; // ����� �������� ��������� � �����

						 // ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT);

	// ������������ ��������� ������� � ����������� ��������
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// ������ ������ ����� ����� ��������� ���������

	rComponent = (GLfloat)(rand() % 1000) / 1000;
	gComponent = (GLfloat)(rand() % 1000) / 1000;
	bComponent = (GLfloat)(rand() % 1000) / 1000;

	glColor3f(rComponent, gComponent, bComponent);

	// ������ �����
	glBegin(GL_LINE_STRIP);
	z = -50.0f;
	for (angle = 0.0f; angle <= (2.0f*GL_PI)*3.0f; angle += step)
	{
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// ������ ����� � ������� ������� �������� z 
		glVertex3f(x, y, z);
		z += (0.5f * (step/0.1));
	}
	// ������ ����� 
	glEnd();

	// ��������������� ��������������
	glPopMatrix();

	// ������� ���� ������ ���������
	glutSwapBuffers();
}

// ������� ��������� ����������� ������������� 
// � ��������� ������������ 
void SetupRC()
{
	// ������ ���
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

	// ��������� ����
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
	glutAddMenuEntry("�����", 0);
	glutAddMenuEntry("�������", 1);
	glutAddMenuEntry("�������", 2);
	glutSetMenu(menu);
	glutAddSubMenu("�������������", subMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f;

	// ������������� ������� �� ����
	if (h == 0)
		h = 1;

	// ������������� ������� ���� ��������� ������� �������� ����
	glViewport(0, 0, w, h);

	// ������������� ������������� ������� ���������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ������������� ����� ��������� � ������� ���������� 
	// ���������� (�����, ������, ������, �������, �������, �������) 

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// ����������� ���� ������ �������� ������
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