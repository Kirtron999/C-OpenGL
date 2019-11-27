// ���������� ������������ ����� ���������
#include "glew.h"
#include "glut.h"
void RenderScene(void)
{
	// ���� ��������� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT);
	// � ����� �������� ������� ���������
	glFlush();
}
/////////////////////////////////////////////////////////////////// 
// ��������������� ��������� ������������
void SetupRC(void)
{
	glClearColor(0.98f, 0.625f, 0.12f, 1.0f);
}
/////////////////////////////////////////////////////////////////// 
// ����� ����� �������� ���������
void main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Colorful window");
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
}
