// подключаем заголовочные файлы библиотек
#include "glew.h"
#include "glut.h"
// создаем обработчик отрисовки
void display()
{
	// переключаем буферы экрана
	glutSwapBuffers();
}
// описываем главную функцию. рекомендуется использовать
// именно такой формат ее заголовка
void main(int argc, char* argv[])
{

	// инициализируем экран в режиме RGBA и двойным буфером
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	// создаем окно по умолчанию
	glutCreateWindow("Test Window");
	// устанавливаем обработчик перерисовки окна
	glutDisplayFunc(display);
	// запускаем основной цикл окна
	glutMainLoop();
}
