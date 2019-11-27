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

void RenderScene(void)
{
	// ���������� ���� ��� ��� ���� ���������� ����� 
	GLfloat x, y, z, angle; // ����� �������� ���������� � ����
	GLfloat fSizes[2]; // ������� ��������� ������ �����
	GLfloat fCurrSize; // ������ �������� ���������
	GLint factor = 1; 	// ��������� ������� 
	GLushort pattern = 0x5555; // ������ �������


	// ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glGetFloatv(GL_LINE_WIDTH_RANGE, fSizes);

	// �������������� ������� 
	glEnable(GL_LINE_STIPPLE);

	fCurrSize = fSizes[0];
	z = -50.0f;
	// ��������� ������ ��� � �� 20 ������ �� ���
	for (y = -90.0f; y < 90.0f; y += 19.0f)
	{
		// �������� ������ ����� 
		glLineWidth(fCurrSize);
		glLineStipple(factor, pattern);
		
		//�������� ���� ��������� �����
		rComponent = (GLfloat)(rand() % 1000) / 1000;
		gComponent = (GLfloat)(rand() % 1000) / 1000;
		bComponent = (GLfloat)(rand() % 1000) / 1000;

		glColor3f(rComponent, gComponent, bComponent);

		// �������� ����� 
		glBegin(GL_LINES);
		glVertex3f(-80.0f, y, z);
		glVertex3f(80.0f, y, z);
		glEnd();
		// ������������� ������ �����
		fCurrSize += 1.0f;
		factor++;
		pattern -= 1000;
		z += 5.0f;
	}
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
	glutAddMenuEntry("�������", 0);
	glutAddMenuEntry("�������", 1);
	glutAddMenuEntry("�����", 2);
	glutAddMenuEntry("������", 3);
	glutAddMenuEntry("�����", 4);
	glutAddMenuEntry("���������", 5);
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
	//MenuInit();
	glutMainLoop();

	return 0;
}