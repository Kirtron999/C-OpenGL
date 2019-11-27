#include "glew.h"	
#include "glut.h"	
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////
// ���������� ���������� ������ ��� ������ �������� ������ �����������
static GLubyte *pImage = NULL;
static GLint iWidth, iHeight, iComponents;
static GLenum eFormat;
static GLfloat coeff = 0.2;

// ����������� ��������� targa
#pragma pack(1)
typedef struct
{
	GLbyte	identsize;    //������ ���� ID,������� ������������� ��������� (0)
	GLbyte	colorMapType;           // 0 = None, 1 = paletted
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // ������ ������ ������� �����
	unsigned short	colorMapLength;         // ���������� ������
	unsigned char 	colorMapBits;   		// ���������� ��� 
	unsigned short	xstart;                 // ������ ���������� X
	unsigned short	ystart;                 // ������ ���������� Y
	unsigned short	width;                  // ������ � ��������
	unsigned short	height;                 // ������ � ��������
	GLbyte	bits;                   // ���������� ��� �� �������(8 16, 24, 32)
	GLbyte	descriptor;             // ���������� �����������
} TGAHEADER;

#pragma pack(8)

GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	FILE *pFile;			// ��������� �����
	TGAHEADER tgaHeader;		// ��������� ����� TGA
	unsigned long lImageSize;		// ������ ����������� � ������
	short sDepth;			// ������ �������
	;
	GLbyte	*pBits = NULL;          // ��������� �� ����

									// �������� �� ���������/��������	��� ����
	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_BGR_EXT;
	*iComponents = GL_RGB8;

	// �������� ������� ����
	pFile = fopen(szFileName, "rb");
	if (pFile == NULL)
		return NULL;

	// ��������� ��������� (��������)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

	// ��������� ������ ��� �������� ����� �������� � ������ 
	// �������� �����
#ifdef __APPLE__
	BYTE_SWAP(tgaHeader.colorMapStart);
	BYTE_SWAP(tgaHeader.colorMapLength);
	BYTE_SWAP(tgaHeader.xstart);
	BYTE_SWAP(tgaHeader.ystart);
	BYTE_SWAP(tgaHeader.width);
	BYTE_SWAP(tgaHeader.height);
#endif


	// �������� ������, ������ � ������� �������� 
	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// �������� ������������. ����� ������: � ������� ������
	// 8-, 24- ��� 32-������� ����� targa
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// ������ ������� ������ �����������
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// ������������� ������ � �������� ���������� 
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pBits == NULL)
		return NULL;

	// ���������� �����
	// �������� �� ������� ������ ������. ����� ������ // ������������� ��������� ����������� ��� ������ 
	// �������, ������� �� ����� ������������ 
	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// ��������������� ������, ��������� OpenGL 
	switch (sDepth)
	{
	case 3:     // �������� ��������� ������
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


	// ������ � ������ ��������� 
	fclose(pFile);

	// ���������� ��������� �� ������ ����������� 
	return pBits;
}

//////////////////////////////////////////////////////////////////
// ��� ������� ��������� ����������� ������������� � ��������� 
// ������������.
void SetupRC(void)
{
	// ������ ���
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// ��������� ����������� ������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	pImage = (GLubyte*)gltLoadTGA("C:\\Users\\������\\Desktop\\2.tga", &iWidth, &iHeight, &iComponents, &eFormat);
}

void ShutdownRC(void)
{
	// ����������� �������� ������ �����������
	free(pImage);
}


///////////////////////////////////////////////////////////////////////////////
// ������� ������� ��������� ����� � ����� �� ����� ������� �� ����

void ProcessMenu(int value)
{
	switch (value) {
	case 0:
		if (coeff < 1.0)
			coeff += 0.2;
		break;
	case 1:
		if (coeff > 0.1)
			coeff -= 0.2;
		break;
	}
	// ������������ ��������������� �����������
	glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////////        
// ���������� ��� ��������� ����� 
void RenderScene(void)
{
	// ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT);

	glPixelZoom(1.0f, -1.0f);
	glRasterPos2i(0, iHeight);

	
	glDrawPixels(iWidth, iHeight*coeff, eFormat, GL_UNSIGNED_BYTE, pImage);
	
												// ����������� ������ 
	glutSwapBuffers();
}



void ChangeSize(int w, int h)
{
	// ������������� ������� �� ����, ����� ���� ������� ��������� 
	// (������ ������� ���� ������� ������) 
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	// ������� ��������� ����������� ����� ������������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ��������������� ����� ���������
	gluOrtho2D(0.0f, (GLfloat)w, 0.0, (GLfloat)h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/////////////////////////////////////////////////////////////
// ����� ����� �������� ��������� 

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GL_DOUBLE);
	glutInitWindowSize(960, 600);
	glutCreateWindow("OpenGL Image Operations");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	// ��������� ���� � ����������� ����� ������
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("+20%", 0);
	glutAddMenuEntry("-20%", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	SetupRC();          //	���������

	glutMainLoop();     //	�������� �����������	����

	ShutdownRC();       //	����������


	return 0;
}
