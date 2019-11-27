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
GLboolean bCull, bDepth, bOutline;

void RenderScene(void)
{
	GLfloat angle; // ����� �������� ��������� � �����
	GLfloat xA = -50.0f, yA = 0.0f, zA = 0.0f;
	GLfloat xB = 50.0f, yB = 0.0f, zB = -50.0f;
	GLfloat xC = 20.0f, yC = 0.0f, zC = 50.f;
	GLfloat xD = 0.0f, yD = 50.0f, zD = 0.0f;
						 // ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

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
	// ������������ ��������� ������� � ����������� ��������
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);


	// ������ ������������
	glFrontFace(GL_CCW);

	glBegin(GL_TRIANGLES);
	rComponent = 0.0f;
	gComponent = 0.0f;
	bComponent = 1.0f;
	glColor3f(rComponent, gComponent, bComponent);
	glVertex3f(xA, yA, zA);
	glVertex3f(xD, yD, zD);
	glVertex3f(xB, yB, zB);


		rComponent = 1.0f;
		gComponent = 0.0f;
		bComponent = 0.0f;
		glColor3f(rComponent, gComponent, bComponent);
		glVertex3f(xC, yC, zC);
		glVertex3f(xD, yD, zD);
		glVertex3f(xA, yA, zA);

		rComponent = 0.0f;
		gComponent = 1.0f;
		bComponent = 0.0f;
		glColor3f(rComponent, gComponent, bComponent);
		glVertex3f(xB, yB, zB);
		glVertex3f(xD, yD, zD);
		glVertex3f(xC, yC, zC);

		

		rComponent = 1.0f;
		gComponent = 1.0f;
		bComponent = 0.0f;
		glColor3f(rComponent, gComponent, bComponent);
		glVertex3f(xC, yC, zC);
		glVertex3f(xA, yA, zA);
		glVertex3f(xB, yB, zB);
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