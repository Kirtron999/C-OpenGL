// ���������� ������������ ����� ���������
#include "glew.h"
#include "glut.h"
// ������� ���������� ���������
void display()
{
	// ����������� ������ ������
	glutSwapBuffers();
}
// ��������� ������� �������. ������������� ������������
// ������ ����� ������ �� ���������
void main(int argc, char* argv[])
{

	// �������������� ����� � ������ RGBA � ������� �������
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	// ������� ���� �� ���������
	glutCreateWindow("Test Window");
	// ������������� ���������� ����������� ����
	glutDisplayFunc(display);
	// ��������� �������� ���� ����
	glutMainLoop();
}
