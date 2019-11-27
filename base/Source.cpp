// ���������� ������������ ����� ���������
#include "glew.h"
#include "glut.h"
// �������� ��������� � ������ ��������������
GLfloat x1 = 0.0f;  GLfloat y1 = 0.0f; GLfloat rsize = 25;
// �������� ���� � ������������ � � � (����� ��������,
// �� ������� �� ������ ���� ������������ �������������)
GLfloat xstep = 1.0f;  GLfloat ystep = 1.0f; GLfloat angle = 0;
// ������������ ��������� ������ � ������ ����
GLfloat windowWidth;  GLfloat windowHeight;
// ������������ ���������������
GLfloat xCoefficient = 1.0f;  GLfloat yCoefficient = 1.0f;
// ���� ������������� ���������������
GLfloat xCoefficientStep = 0.0f;  GLfloat yCoefficientStep = 0.0f;
//���������� ��� ��������� ����� 
void RenderScene(void)
{
	// ������� ����,  ��������� ������� ���� ������� 
	glClear(GL_COLOR_BUFFER_BIT);
	// � �������� �������� ����� ��������� ������ ������� //RGB
	glColor3f(1.0f, 1.0f, 0.0f);
	// ������� ���������� �������

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

	// ������� ������� ������� ������ � ����������� ������
	glutSwapBuffers();
}
//���������� ����������� GLUT � �������� ���������  (���� �� ������ 	
//������� � �� ������������) 
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
//������ ��������� ������������ 
void SetupRC(void)
{
	// ������������� � �������� ����� ������� �����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
//���������� ����������� GLUT ��� ��������� �������� ���� 
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	// ������������� ������� �� ���� 
	if (h == 0) h = 1;
	// ������������� ���� ��������� � ��������� ����
	glViewport(0, 0, w, h);
	// ��������� ������� ���������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//� ������� ���������� ���������  (�����,  ������,  ������, 
	// �������,  �������,  �������)  ������������� ����� ��������� 	
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
//����� ����� �������� ��������� 
void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); glutCreateWindow("Bounce");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
}
