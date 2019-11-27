#include "glew.h"	
#include "glut.h"	
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////
// Глобальные переменные модуля для записи исходных данных изображения
static GLubyte *pImage = NULL;
static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;

// Глобальные переменные для хранения режима рисования
static GLint    iRenderMode = 1;

// Определение заголовка targa
#pragma pack(1)
typedef struct
{
	GLbyte	identsize;    //Размер поля ID,который соответствует заголовку (0)
	GLbyte	colorMapType;           // 0 = None, 1 = paletted
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // Первая позици цветной карты
	unsigned short	colorMapLength;         // Количество цветов
	unsigned char 	colorMapBits;   		// Количество бит 
	unsigned short	xstart;                 // начало координаты X
	unsigned short	ystart;                 // начало координаты Y
	unsigned short	width;                  // ширина в пикселях
	unsigned short	height;                 // высота в пикселях
	GLbyte	bits;                   // количество бит на пикселе(8 16, 24, 32)
	GLbyte	descriptor;             // Дескриптор изображения
} TGAHEADER;

#pragma pack(8)

GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	FILE *pFile;			// Указатель файла
	TGAHEADER tgaHeader;		// Заголовок файла TGA
	unsigned long lImageSize;		// Размер изображения в байтах
	short sDepth;			// Размер пикселя
	;
	GLbyte	*pBits = NULL;          // Указатель на биты

									// Значения по умолчанию/значения	при сбое
	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_BGR_EXT;
	*iComponents = GL_RGB8;

	// Пытаемся открыть файл
	pFile = fopen(szFileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Считываем заголовок (двоичный)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

	// Обращение байтов при переходе между обратным и прямым 
	// порядком битов
#ifdef __APPLE__
	BYTE_SWAP(tgaHeader.colorMapStart);
	BYTE_SWAP(tgaHeader.colorMapLength);
	BYTE_SWAP(tgaHeader.xstart);
	BYTE_SWAP(tgaHeader.ystart);
	BYTE_SWAP(tgaHeader.width);
	BYTE_SWAP(tgaHeader.height);
#endif


	// Получаем ширину, высоту и глубину текстуры 
	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Проверки приемлемости. Очень просто: я понимаю только
	// 8-, 24- или 32-битовые файлы targa
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// Расчет размера буфера изображения
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Распределение памяти и проверка успешности 
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pBits == NULL)
		return NULL;

	// Считывание битов
	// Проверка на наличие ошибок чтения. Здесь должны // отлавливаться групповое кодирование или другие 
	// форматы, которые не нужно распознавать 
	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Устанавливается формат, ожидаемый OpenGL 
	switch (sDepth)
	{
	case 3:     // Наиболее вероятный случай
		*eFormat = GL_BGR_EXT;
		*iComponents = GL_RGB8;
		break;
	case 4:
		*eFormat = GL_BGRA_EXT;
		*iComponents = GL_RGBA8;
		break;
	case 1:
		*eFormat = GL_LUMINANCE;
		*iComponents = GL_LUMINANCE8;
		break;
	};


	// Работа с файлом закончена 
	fclose(pFile);

	// Возвращает указатель на данные изображения 
	return pBits;
}


//////////////////////////////////////////////////////////////////
// Эта функция выполняет необходимую инициализацию в контексте 
// визуализации.
void SetupRC(void)
{
	// Черный фон
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Загружаем изображение лошади
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	pImage = (GLubyte*)gltLoadTGA("C:\\Users\\Кирилл\\Desktop\\579450646.tga", &iWidth, &iHeight, &iComponents, &eFormat);
}

void ShutdownRC(void)
{
	// Освобождаем исходные данные изображения
	free(pImage);
}


///////////////////////////////////////////////////////////////////////////////
// Должным образом обновляем флаги в ответ на выбор позиции из меню

void ProcessMenu(int value)
{

		iRenderMode = value;

	// Активизируем перерисовывание изображения
	glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////////        
// Вызывается для рисования сцены 
void RenderScene(void)
{
	GLint iViewport[4];
	GLbyte *pModifiedBytes = NULL;
	GLfloat invertMap[256];
	GLint i;

	// Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT);

	// Текущее растровое положение всегда соответствует левому 
	// нижнему углу окна 
	glRasterPos2i(0, 0);

	//В зависимости от индекса режима визуализации выполняются 
	// необходимые операции с изображением 
	switch (iRenderMode)
	{
	case 1:
		// Вначале	рисуем	изображение	в	буфере цвета
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

		// Распределяем память для карты яркости
		pModifiedBytes = (GLbyte *)malloc(iWidth * iHeight);

		// Масштабируем цвета согласно стандарту NSTC
		glPixelTransferf(GL_RED_SCALE, 0.21f);
		glPixelTransferf(GL_GREEN_SCALE, 0.72f);
		glPixelTransferf(GL_BLUE_SCALE, 0.07f);

		// Считываем пиксели в буфер (будем применено увеличение)
		glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);

		// Масштабирование цвета возвращается в норму
		glPixelTransferf(GL_RED_SCALE, 1.0f);
		glPixelTransferf(GL_GREEN_SCALE, 1.0f);
		glPixelTransferf(GL_BLUE_SCALE, 1.0f);
		break;
	case 2:     // Переворачиваем пиксели
				// Вначале	рисуем	изображение	в	буфере цвета
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

		// Распределяем память для карты яркости
		pModifiedBytes = (GLbyte *)malloc(iWidth * iHeight);

		// Масштабируем цвета согласно стандарту NSTC
		glPixelTransferf(GL_RED_SCALE, 0.3f);
		glPixelTransferf(GL_GREEN_SCALE, 0.59f);
		glPixelTransferf(GL_BLUE_SCALE, 0.11f);

		// Считываем пиксели в буфер (будем применено увеличение)
		glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);

		// Масштабирование цвета возвращается в норму
		glPixelTransferf(GL_RED_SCALE, 1.0f);
		glPixelTransferf(GL_GREEN_SCALE, 1.0f);
		glPixelTransferf(GL_BLUE_SCALE, 1.0f);
		break;

	case 3:     
		// Вначале	рисуем	изображение	в	буфере цвета
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

		// Распределяем память для карты яркости
		pModifiedBytes = (GLbyte *)malloc(iWidth * iHeight);

		// Масштабируем цвета согласно стандарту NSTC
		glPixelTransferf(GL_RED_SCALE, 0.33f);
		glPixelTransferf(GL_GREEN_SCALE, 0.33f);
		glPixelTransferf(GL_BLUE_SCALE, 0.33f);

		// Считываем пиксели в буфер (будем применено увеличение)
		glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);

		// Масштабирование цвета возвращается в норму
		glPixelTransferf(GL_RED_SCALE, 1.0f);
		glPixelTransferf(GL_GREEN_SCALE, 1.0f);
		glPixelTransferf(GL_BLUE_SCALE, 1.0f);
		break;

	case 0:     // Просто копия старого изображения
	default:
		// Данная строка специально оставлена пустой
		break;
	}

	// Рисуются пиксели
	if (pModifiedBytes == NULL)
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);
	else
	{
		glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);
		free(pModifiedBytes);
	}


	// Обновление всего до настроек по умолчанию 
	glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glPixelTransferf(GL_RED_SCALE, 1.0f);
	glPixelTransferf(GL_GREEN_SCALE, 1.0f);
	glPixelTransferf(GL_BLUE_SCALE, 1.0f);
	glPixelZoom(1.0f, 1.0f);                    //Без	увеличения пикселей


											// Переключает буферы 
	glutSwapBuffers();
}



void ChangeSize(int w, int h)
{
	// Предотвращает деление на нуль, когда окно слишком маленькое 
	// (нельзя сделать окно нулевой ширины) 
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	// Система координат обновляется перед модификацией
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Устанавливается объем отсечения
	gluOrtho2D(0.0f, (GLfloat)w, 0.0, (GLfloat)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/////////////////////////////////////////////////////////////
// Точка входа основной программы 

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Image Operations");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	// Создается меню и добавляются опции выбора
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Original Image", 0);
	glutAddMenuEntry("HDTV standard", 1);
	glutAddMenuEntry("PAL NTSC standard", 2);
	glutAddMenuEntry("Equal coefficients(0.33)", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	SetupRC();          //	Настройка

	glutMainLoop();     //	Основной программный	цикл

	ShutdownRC();       //	Выключение


	return 0;
}