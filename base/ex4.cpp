// подключаем заголовочные файлы библиотек
#include "glew.h"
#include "glut.h"
void RenderScene(void) {
	// Очищаем окно,  используя текущий цвет очистки                  	
	glClear(GL_COLOR_BUFFER_BIT);
	// В качестве текущего цвета рисования задает красный //RGB
	glColor3f(1.0f, 0.0f, 0.0f);
	// Рисует прямоугольник,   закрашенный текущим цветом 
	glRectf(-25.0f, 25.0f, 25.0f, -25.0f);
	// Очищает очередь текущих команд 
	glFlush();
}
///////////////////	Задает состояние визуализации 
void SetupRC(void)
{
	// Устанавливает в качестве цвета очистки синий
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}
///////Вызывается библиотекой GLUT при изменении размеров окна 
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat aspectRatio;
	// Предотвращает деление на нуль
	if (h == 0)
		h = 1;
	// Устанавливает поле просмотра с размерами окна
	glViewport(0, 0, w, h);
	// Обновляет систему координат
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// С помощью плоскостей отсечения (левая, правая, нижняя, 
	// верхняя, ближняя, дальняя) устанавливает объем отсечения 
	glOrtho(-100.0, 100.0, -100, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

///////////////////Точка входа основной программы 
void main(void) {
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("GLRect");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutMainLoop();
}
