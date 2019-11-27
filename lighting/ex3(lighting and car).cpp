#include "glew.h"
#include "glut.h"
#include <math.h>

// ������������ ��������� �� ��������� ���
#define GL_PI 3.1415f

// ��������� ��������
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


//���������� ��� ��������� ����� 
void RenderScene(void)
{
	GLenum mode = GL_POLYGON;
	bool solid = true;
	GLUquadricObj *quadObj = gluNewQuadric();
	GLfloat left = 0.0f, up = 0.0f, right = 0.0f;
	// ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	glRotatef(left, 0.0, 1.0, 0.0);
	glRotatef(up, 1.0, 0.0, 0.0);
	glRotatef(right, 0.0, 0.0, 1.0);

	// �������� ����
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

	// ������ ����
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

	//�������� ������
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
	// �������
	if (solid)
		glutSolidTorus(0.05, 0.15, 3, 10);
	else
		glutWireTorus(0.05, 0.15, 3, 10);

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0.0, 0.9);
	glTranslatef(-0.1, -0.4, 1.0);

	// �����
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
	// ������� �������
	gluCylinder(quadObj, 0.03, 0.03, 1.0, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.1, 0.8);
	glRotatef(70, 1.0, 0.0, 0.0);
	gluCylinder(quadObj, 0.09, 0.09, 0.7, 10, 10);
	glPopMatrix();

	glPushMatrix();
	// ������
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

// ������� ��������� ����������� ������������� 
// � ��������� ������������ 
void SetupRC()
{
	// ��������� �����
	// ����� ����� ����
	//GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  ambientLight0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat  diffuseLight0[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	GLfloat  ambientLight1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat  diffuseLight1[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	GLfloat  ambientLight2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat  diffuseLight2[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	glEnable(GL_DEPTH_TEST);	// �������� ������� ������
	glFrontFace(GL_CCW);// �������������� � ������� ������ ������� ������� 
						//���������� ������

						// ��������� ���������
	glEnable(GL_LIGHTING);			// ����������� ���������	
	glEnable(GL_LIGHT0);     // �������� �������� ����� 0
							 // � ������ ��������� �������� ������������� ����������� �����,  //��������� � ������� ambientLight[]                  

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
	glEnable(GL_LIGHT2);


	glEnable(GL_COLOR_MATERIAL);	// �������������� ������������ ������

									//���������� � ��������� ����� �������� ������� �������� 
									//������������� ����, ��� ������� � glColor

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// ������-����� ����
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
	GLfloat lightPos1[] = { 5, 0, 0, 1.0f };
	GLfloat lightPos2[] = { -5, 0, 0, 1.0f };
	GLfloat lightPos3[] = { 0, 0, -5, 1.0f };
	// �������������� ������� �� ����
	if (h == 0)
		h = 1;

	// ������������� ���� ��������� �� �������� ����
	glViewport(0, 0, w, h);

	// ��������� ���� ������� ��������������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ������������� ����� ��������� � ������� ���������� 
	// ���������� (�����, ������, ������, �������, 
	// �������, �������) 
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);
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