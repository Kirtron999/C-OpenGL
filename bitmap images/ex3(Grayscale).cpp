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

// ���������� ���������� ��� �������� ������ ���������
static GLint    iRenderMode = 1;

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
	pImage = (GLubyte*)gltLoadTGA("C:\\Users\\������\\Desktop\\579450646.tga", &iWidth, &iHeight, &iComponents, &eFormat);
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

		iRenderMode = value;

	// ������������ ��������������� �����������
	glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////////        
// ���������� ��� ��������� ����� 
void RenderScene(void)
{
	GLint iViewport[4];
	GLbyte *pModifiedBytes = NULL;
	GLfloat invertMap[256];
	GLint i;

	// ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT);

	// ������� ��������� ��������� ������ ������������� ������ 
	// ������� ���� ���� 
	glRasterPos2i(0, 0);

	//� ����������� �� ������� ������ ������������ ����������� 
	// ����������� �������� � ������������ 
	switch (iRenderMode)
	{
	case 1:
		// �������	������	�����������	�	������ �����
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

		// ������������ ������ ��� ����� �������
		pModifiedBytes = (GLbyte *)malloc(iWidth * iHeight);

		// ������������ ����� �������� ��������� NSTC
		glPixelTransferf(GL_RED_SCALE, 0.21f);
		glPixelTransferf(GL_GREEN_SCALE, 0.72f);
		glPixelTransferf(GL_BLUE_SCALE, 0.07f);

		// ��������� ������� � ����� (����� ��������� ����������)
		glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);

		// ��������������� ����� ������������ � �����
		glPixelTransferf(GL_RED_SCALE, 1.0f);
		glPixelTransferf(GL_GREEN_SCALE, 1.0f);
		glPixelTransferf(GL_BLUE_SCALE, 1.0f);
		break;
	case 2:     // �������������� �������
				// �������	������	�����������	�	������ �����
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

		// ������������ ������ ��� ����� �������
		pModifiedBytes = (GLbyte *)malloc(iWidth * iHeight);

		// ������������ ����� �������� ��������� NSTC
		glPixelTransferf(GL_RED_SCALE, 0.3f);
		glPixelTransferf(GL_GREEN_SCALE, 0.59f);
		glPixelTransferf(GL_BLUE_SCALE, 0.11f);

		// ��������� ������� � ����� (����� ��������� ����������)
		glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);

		// ��������������� ����� ������������ � �����
		glPixelTransferf(GL_RED_SCALE, 1.0f);
		glPixelTransferf(GL_GREEN_SCALE, 1.0f);
		glPixelTransferf(GL_BLUE_SCALE, 1.0f);
		break;

	case 3:     
		// �������	������	�����������	�	������ �����
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);

		// ������������ ������ ��� ����� �������
		pModifiedBytes = (GLbyte *)malloc(iWidth * iHeight);

		// ������������ ����� �������� ��������� NSTC
		glPixelTransferf(GL_RED_SCALE, 0.33f);
		glPixelTransferf(GL_GREEN_SCALE, 0.33f);
		glPixelTransferf(GL_BLUE_SCALE, 0.33f);

		// ��������� ������� � ����� (����� ��������� ����������)
		glReadPixels(0, 0, iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);

		// ��������������� ����� ������������ � �����
		glPixelTransferf(GL_RED_SCALE, 1.0f);
		glPixelTransferf(GL_GREEN_SCALE, 1.0f);
		glPixelTransferf(GL_BLUE_SCALE, 1.0f);
		break;

	case 0:     // ������ ����� ������� �����������
	default:
		// ������ ������ ���������� ��������� ������
		break;
	}

	// �������� �������
	if (pModifiedBytes == NULL)
		glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);
	else
	{
		glDrawPixels(iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, pModifiedBytes);
		free(pModifiedBytes);
	}


	// ���������� ����� �� �������� �� ��������� 
	glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
	glPixelTransferf(GL_RED_SCALE, 1.0f);
	glPixelTransferf(GL_GREEN_SCALE, 1.0f);
	glPixelTransferf(GL_BLUE_SCALE, 1.0f);
	glPixelZoom(1.0f, 1.0f);                    //���	���������� ��������


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
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Image Operations");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	// ��������� ���� � ����������� ����� ������
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Original Image", 0);
	glutAddMenuEntry("HDTV standard", 1);
	glutAddMenuEntry("PAL NTSC standard", 2);
	glutAddMenuEntry("Equal coefficients(0.33)", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	SetupRC();          //	���������

	glutMainLoop();     //	�������� �����������	����

	ShutdownRC();       //	����������


	return 0;
}