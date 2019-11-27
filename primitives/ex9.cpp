#include "glew.h"	// System and OpenGL Stuff
#include "glut.h"
#include <math.h>


// ������������ ��������� �� ��������� ���
#define GL_PI 3.1415f


// �������� �����
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLboolean bCull, bDepth, bOutline;

// ���������� ��� ��������� �����
void SetupRC()
{
	// ������ ���
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// ���� ��������� ���������� ������� 
	glColor3f(0.0f, 1.0f, 0.0f);
	// ���� ������ ��������� ���������� ������������������� 
	glShadeModel(GL_FLAT);
	// �������������� � ������� �� ������� ������� ��������� 
	// ������������� ������; ��������� �������� �� ��������, 
	// ��������� �� ���������� ����� �������������
	glFrontFace(GL_CW);
}

// ���������� ��� ��������� ����� 
void RenderScene(void)
{
	GLfloat x, y, angle;  // ����� �������� ���������� � ����
	int iPivot = 1;	 	// ������������, ����� ��������
						// ������������ ����� 
						// ������� ���� � ����� �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// �������� �����, ���� ����������� �����
	if (bCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	// ���� ����������� �����, ������������ �������� �������
	if (bDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	// ���� ����������� �����, ������ ������ �������
	// � ����� ������� 
	if (bOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	// ���������� ��������� ������� � ��������� ������� 
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// �������� ���� �������������
	glBegin(GL_TRIANGLE_FAN);
	// ������� ������ �������� ����� �������� �����. ����������� 
	// ����� �� ��� z, ������ ���������� �������� �����
	glVertex3f(0.0f, 0.0f, 75.0f);
	//�� ����� �������� ���������� � ������ ������ ����� �����
	// ���������� ��� ������� ����� �������������
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// ������������ ��������� x � � ��������� �������
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// �������� ������� � ������� ����
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		// ����������� pivot �� 1, ����� � ��������� ��� 
		// �������� ����
		iPivot++;
		// ������ ��������� ������� ����� �������������
		glVertex2f(x, y);
	}
	// ������ ����, ����������� ����� 
	glEnd();
	// �������� ����� ���� �������������, ����������� ���������
	// ������
	glBegin(GL_TRIANGLE_FAN);
	// ������� ����� �������� ������ ��������� 
	glVertex2f(0.0f, 0.0f);
	for (angle = 0.0f; angle < (2.0f*GL_PI); angle += (GL_PI / 8.0f))
	{
		// ������������ ���������� � � � ��������� �������
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);
		// �������� ������� � ������� �����
		if ((iPivot % 2) == 0)
			glColor3f(0.0f, 1.0f, 0.0f);
		else
			glColor3f(1.0f, 0.0f, 0.0f);
		// ����������� pivot �� �������, ����� � ��������� ���
		// �������� ����� 
		iPivot++;
		// ������ ��������� ������� ����� �������������
		glVertex2f(x, y);
	}
	// ������ ����, ����������� ��������� ������
	glEnd();
	// ��������������� �������������� 
	glPopMatrix();
	// ������� ���� ������ ���������
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

	// ��������� ����
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