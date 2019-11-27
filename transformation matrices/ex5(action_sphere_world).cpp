
#include "glew.h"
#include "glut.h"
#include <windows.h>
#include <math.h>

///////////////////////////////////////////////////////
// ������������ ���������
#define GLT_PI	3.14159265358979323846
#define GLT_PI_DIV_180 0.017453292519943296
#define GLT_INV_PI_DIV_180 57.2957795130823229

///////////////////////////////////////////////////////////////////////////////
// �������� ������� �������� ������ � �������
// ��������� � ��������, �� ��� ����� ������, ����� �������� �������
#define gltDegToRad(x)	((x)*GLT_PI_DIV_180)
#define gltRadToDeg(x)	((x)*GLT_INV_PI_DIV_180)
// ��������� ���� ������
typedef GLfloat GLTVector2[2];//��������������� ������ � ��������� �������
typedef GLfloat GLTVector3[3];//��������������� ������ � ��������� �������
typedef GLfloat GLTVector4[4];//������������������� ������ � ��������� //�������
typedef GLfloat GLTMatrix[16];// �������� ������� ������� 4�4 � ��������� //�������


typedef struct {                     // ����� ��� ���������� ������    GLTVector3 vLocation;
	GLTVector3 vUp;
	GLTVector3 vForward;
	GLTVector3 vLocation;
	GLfloat aColor[3];
} GLTFrame;

#define NUM_SPHERES      50
#define NUM_TEAPOTS 30
#define NUM_CUBES 30
GLTFrame    spheres[NUM_SPHERES];
GLTFrame    teapots[NUM_TEAPOTS];
GLTFrame    cubes[NUM_CUBES];
GLTFrame    frameCamera;
// ��������� ��������� ������������ ���� ��������
void gltVectorCrossProduct(const GLTVector3 vU, const GLTVector3 vV, GLTVector3 vResult)
{
	vResult[0] = vU[1] * vV[2] - vV[1] * vU[2];
	vResult[1] = -vU[0] * vV[2] + vV[0] * vU[2];
	vResult[2] = vU[0] * vV[1] - vV[0] * vU[1];
}
void gltLoadIdentityMatrix(GLTMatrix m)
{
	static GLTMatrix identity = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	memcpy(m, identity, sizeof(GLTMatrix));
}
// ������� ������� �������� 4x4, ������������ �������, � �� � �������
void gltRotationMatrix(float angle, float x, float y, float z, GLTMatrix mMatrix)
{
	float vecLength, sinSave, cosSave, oneMinusCos;
	float xx, yy, zz, xy, yz, zx, xs, ys, zs;

	// ���� ������� ������ ��������, ��...
	if (x == 0.0f && y == 0.0f && z == 0.0f)
	{
		gltLoadIdentityMatrix(mMatrix);
		return;
	}

	// ��������������� �������
	vecLength = (float)sqrt(x*x + y*y + z*z);

	// ������������� ������� ��������
	x /= vecLength;
	y /= vecLength;
	z /= vecLength;

	sinSave = (float)sin(angle);
	cosSave = (float)cos(angle);
	oneMinusCos = 1.0f - cosSave;

	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * sinSave;
	ys = y * sinSave;
	zs = z * sinSave;

	mMatrix[0] = (oneMinusCos * xx) + cosSave;
	mMatrix[4] = (oneMinusCos * xy) - zs;
	mMatrix[8] = (oneMinusCos * zx) + ys;
	mMatrix[12] = 0.0f;

	mMatrix[1] = (oneMinusCos * xy) + zs;
	mMatrix[5] = (oneMinusCos * yy) + cosSave;
	mMatrix[9] = (oneMinusCos * yz) - xs;
	mMatrix[13] = 0.0f;

	mMatrix[2] = (oneMinusCos * zx) - ys;
	mMatrix[6] = (oneMinusCos * yz) + xs;
	mMatrix[10] = (oneMinusCos * zz) + cosSave;
	mMatrix[14] = 0.0f;

	mMatrix[3] = 0.0f;
	mMatrix[7] = 0.0f;
	mMatrix[11] = 0.0f;
	mMatrix[15] = 1.0f;
}

// ������� ������� � �������������� ������� 4�4. ������� ������� ������������ 
void gltRotateVector(const GLTVector3 vSrcVector, const GLTMatrix mMatrix, GLTVector3 vOut)
{
	vOut[0] = mMatrix[0] * vSrcVector[0] + mMatrix[4] * vSrcVector[1] + mMatrix[8] * vSrcVector[2];
	vOut[1] = mMatrix[1] * vSrcVector[0] + mMatrix[5] * vSrcVector[1] + mMatrix[9] * vSrcVector[2];
	vOut[2] = mMatrix[2] * vSrcVector[0] + mMatrix[6] * vSrcVector[1] + mMatrix[10] * vSrcVector[2];
}
/////////////////////////////////////////////////////////
//��� ������� ����������� ������ ����� ��������� �������.
void gltInitFrame(GLTFrame *pFrame)
{
	pFrame->vLocation[0] = 0.0f;
	pFrame->vLocation[1] = 0.0f;
	pFrame->vLocation[2] = 0.0f;

	pFrame->vUp[0] = 0.0f;
	pFrame->vUp[1] = 1.0f;
	pFrame->vUp[2] = 0.0f;

	pFrame->vForward[0] = 0.0f;
	pFrame->vForward[1] = 0.0f;
	pFrame->vForward[2] = -1.0f;
}

///////////////////////////////////////////////////////////////////
// ������� ������� �������������� 4x4 �� ������� �������
void gltGetMatrixFromFrame(GLTFrame *pFrame, GLTMatrix mMatrix)
{
	GLTVector3 vXAxis;       // ����������� ��� X

							 // ���������� ��� X
	gltVectorCrossProduct(pFrame->vUp, pFrame->vForward, vXAxis);

	// ������ ���������� ������� �������� ������� X
	memcpy(mMatrix, vXAxis, sizeof(GLTVector3));
	mMatrix[3] = 0.0f;

	// ������� ������� Y
	memcpy(mMatrix + 4, pFrame->vUp, sizeof(GLTVector3));
	mMatrix[7] = 0.0f;

	// ������� ������� Z
	memcpy(mMatrix + 8, pFrame->vForward, sizeof(GLTVector3));
	mMatrix[11] = 0.0f;

	// �������/������������ �������
	memcpy(mMatrix + 12, pFrame->vLocation, sizeof(GLTVector3));
	mMatrix[15] = 1.0f;
}

////////////////////////////////////////////////////////////////////
// ������������ ����������� ����, �������� ��� �������������� ������� �������
void gltApplyActorTransform(GLTFrame *pFrame)
{
	GLTMatrix mTransform;
	gltGetMatrixFromFrame(pFrame, mTransform);
	glMultMatrixf(mTransform);
}

//////////////////////////////////////////////////////////////////
// ���������� ������ �������������� ������ ������� �������. ��� � //������������ ������� �������������� ���������� gluLookAt ������������ ��� //float ������ double � ������� ������ ������ ������ ���������.
void gltApplyCameraTransform(GLTFrame *pCamera)
{
	GLTMatrix mMatrix;
	GLTVector3 vAxisX;
	GLTVector3 zFlipped;

	zFlipped[0] = -pCamera->vForward[0];
	zFlipped[1] = -pCamera->vForward[1];
	zFlipped[2] = -pCamera->vForward[2];

	// ������� X ������
	gltVectorCrossProduct(pCamera->vUp, zFlipped, vAxisX);

	// ���������� �������, �������� ��������, ��� ������� � �������
	mMatrix[0] = vAxisX[0];
	mMatrix[4] = vAxisX[1];
	mMatrix[8] = vAxisX[2];
	mMatrix[12] = 0.0f;

	mMatrix[1] = pCamera->vUp[0];
	mMatrix[5] = pCamera->vUp[1];
	mMatrix[9] = pCamera->vUp[2];
	mMatrix[13] = 0.0f;

	mMatrix[2] = zFlipped[0];
	mMatrix[6] = zFlipped[1];
	mMatrix[10] = zFlipped[2];
	mMatrix[14] = 0.0f;

	mMatrix[3] = 0.0f;
	mMatrix[7] = 0.0f;
	mMatrix[11] = 0.0f;
	mMatrix[15] = 1.0f;

	// ������ ������ �������
	glMultMatrixf(mMatrix);

	// �������� ��������������
	glTranslatef(-pCamera->vLocation[0], -pCamera->vLocation[1], -pCamera->vLocation[2]);
}
void gltMoveFrameForward(GLTFrame *pFrame, GLfloat fStep)
{
	pFrame->vLocation[0] += pFrame->vForward[0] * fStep;
	pFrame->vLocation[1] += pFrame->vForward[1] * fStep;
	pFrame->vLocation[2] += pFrame->vForward[2] * fStep;
}

/////////////////////////////////////////////////////////
// ����������� ������� ������� ����� ��� Y
void gltMoveFrameUp(GLTFrame *pFrame, GLfloat fStep)
{
	pFrame->vLocation[0] += pFrame->vUp[0] * fStep;
	pFrame->vLocation[1] += pFrame->vUp[1] * fStep;
	pFrame->vLocation[2] += pFrame->vUp[2] * fStep;
}

////////////////////////////////////////////////////////
// ����������� ������� ������� ����� ��� X
void gltMoveFrameRight(GLTFrame *pFrame, GLfloat fStep)
{
	GLTVector3 vCross;

	gltVectorCrossProduct(pFrame->vUp, pFrame->vForward, vCross);
	pFrame->vLocation[0] += vCross[0] * fStep;
	pFrame->vLocation[1] += vCross[1] * fStep;
	pFrame->vLocation[2] += vCross[2] * fStep;
}

/////////////////////////////////////////////////////////
// ������� ����� � ������� �����������
void gltTranslateFrameWorld(GLTFrame *pFrame, GLfloat x, GLfloat y, GLfloat z)
{
	pFrame->vLocation[0] += x; pFrame->vLocation[1] += y; pFrame->vLocation[2] += z;
}

/////////////////////////////////////////////////////////
// ������� ����� � ��������� �����������
void gltTranslateFrameLocal(GLTFrame *pFrame, GLfloat x, GLfloat y, GLfloat z)
{
	gltMoveFrameRight(pFrame, x);
	gltMoveFrameUp(pFrame, y);
	gltMoveFrameForward(pFrame, z);
}

/////////////////////////////////////////////////////////
// ������� ����� ������ ��������� ��� Y
void gltRotateFrameLocalY(GLTFrame *pFrame, GLfloat fAngle)
{
	GLTMatrix mRotation;
	GLTVector3 vNewForward;

	gltRotationMatrix((float)gltDegToRad(fAngle), 0.0f, 1.0f, 0.0f, mRotation);
	gltRotationMatrix(fAngle, pFrame->vUp[0], pFrame->vUp[1], pFrame->vUp[2], mRotation);

	gltRotateVector(pFrame->vForward, mRotation, vNewForward);
	memcpy(pFrame->vForward, vNewForward, sizeof(GLTVector3));
}

//////////////////////////////////////////////////////////
//  ������� ����� ������ ��������� ��� X
void gltRotateFrameLocalX(GLTFrame *pFrame, GLfloat fAngle)
{
	GLTMatrix mRotation;
	GLTVector3 vCross;

	gltVectorCrossProduct(vCross, pFrame->vUp, pFrame->vForward);
	gltRotationMatrix(fAngle, vCross[0], vCross[1], vCross[2], mRotation);

	GLTVector3 vNewVect;
	// ��������� ������� 3x3 �������� ��� ��������
	vNewVect[0] = mRotation[0] * pFrame->vForward[0] + mRotation[4] * pFrame->vForward[1] + mRotation[8] * pFrame->vForward[2];
	vNewVect[1] = mRotation[1] * pFrame->vForward[0] + mRotation[5] * pFrame->vForward[1] + mRotation[9] * pFrame->vForward[2];
	vNewVect[2] = mRotation[2] * pFrame->vForward[0] + mRotation[6] * pFrame->vForward[1] + mRotation[10] * pFrame->vForward[2];
	memcpy(pFrame->vForward, vNewVect, sizeof(GLfloat) * 3);

	// ���������� �������, ������������� �����
	vNewVect[0] = mRotation[0] * pFrame->vUp[0] + mRotation[4] * pFrame->vUp[1] + mRotation[8] * pFrame->vUp[2];
	vNewVect[1] = mRotation[1] * pFrame->vUp[0] + mRotation[5] * pFrame->vUp[1] + mRotation[9] * pFrame->vUp[2];
	vNewVect[2] = mRotation[2] * pFrame->vUp[0] + mRotation[6] * pFrame->vUp[1] + mRotation[10] * pFrame->vUp[2];
	memcpy(pFrame->vUp, vNewVect, sizeof(GLfloat) * 3);
}

/////////////////////////////////////////////////////////////
// ������� ����� ������ ��������� ��� Z
void gltRotateFrameLocalZ(GLTFrame *pFrame, GLfloat fAngle)
{
	GLTMatrix mRotation;

	// ��������� ��������� ������ ������, ������������ �����
	gltRotationMatrix(fAngle, pFrame->vForward[0], pFrame->vForward[1], pFrame->vForward[2], mRotation);

	GLTVector3 vNewVect;
	vNewVect[0] = mRotation[0] * pFrame->vUp[0] + mRotation[4] * pFrame->vUp[1] + mRotation[8] * pFrame->vUp[2];
	vNewVect[1] = mRotation[1] * pFrame->vUp[0] + mRotation[5] * pFrame->vUp[1] + mRotation[9] * pFrame->vUp[2];
	vNewVect[2] = mRotation[2] * pFrame->vUp[0] + mRotation[6] * pFrame->vUp[1] + mRotation[10] * pFrame->vUp[2];
	memcpy(pFrame->vUp, vNewVect, sizeof(GLfloat) * 3);
}
// ��������� ��������������� �������
void gltScaleVector(GLTVector3 vVector, const GLfloat fScale)
{
	vVector[0] *= fScale; vVector[1] *= fScale; vVector[2] *= fScale;
}

// ���������� ����� ������� � ��������
GLfloat gltGetVectorLengthSqrd(const GLTVector3 vVector)
{
	return (vVector[0] * vVector[0]) + (vVector[1] * vVector[1]) + (vVector[2] * vVector[2]);
}

// ���������� ����� �������
GLfloat gltGetVectorLength(const GLTVector3 vVector)
{
	return (GLfloat)sqrt(gltGetVectorLengthSqrd(vVector));
}
// ��������������� ������� �� ����� - �������� ����������� �������
void gltNormalizeVector(GLTVector3 vNormal)
{
	GLfloat fLength = 1.0f / gltGetVectorLength(vNormal);
	gltScaleVector(vNormal, fLength);
}
void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor)
{
	GLTVector3 vNormal;
	double majorStep = 2.0f*GLT_PI / numMajor;
	double minorStep = 2.0f*GLT_PI / numMinor;
	int i, j;

	for (i = 0; i<numMajor; ++i)
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat)cos(a0);
		GLfloat y0 = (GLfloat)sin(a0);
		GLfloat x1 = (GLfloat)cos(a1);
		GLfloat y1 = (GLfloat)sin(a1);

		glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j <= numMinor; ++j)
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat)cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat)sin(b);

			// First point
			glTexCoord2f((float)(i) / (float)(numMajor), (float)(j) / (float)(numMinor));
			vNormal[0] = x0*c;
			vNormal[1] = y0*c;
			vNormal[2] = z / minorRadius;
			gltNormalizeVector(vNormal);
			glNormal3fv(vNormal);
			glVertex3f(x0*r, y0*r, z);

			glTexCoord2f((float)(i + 1) / (float)(numMajor), (float)(j) / (float)(numMinor));
			vNormal[0] = x1*c;
			vNormal[1] = y1*c;
			vNormal[2] = z / minorRadius;
			glNormal3fv(vNormal);
			glVertex3f(x1*r, y1*r, z);
		}
		glEnd();
	}
}

//////////////////////////////////////////////////////////////////
// ��� ������� ��������� ����������� ������������� � ��������� 
// ������������ 
void SetupRC()
{
	int iSphere;

	// ����������� ���
	glClearColor(0.0f, 0.0f, .50f, 1.0f);

	gltInitFrame(&frameCamera);  // ���������������� ������

								 // ��������� ������� ����������� ������-�����
	for (iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
	{
		gltInitFrame(&spheres[iSphere]);    // ���������������� ������        
											// ���������� ��������� ��������� ����� -20 � 20 � ����� 0.1
		spheres[iSphere].vLocation[0] = (float)((rand() % 400) - 200) * 0.1f;
		spheres[iSphere].vLocation[1] = 0.0f;
		spheres[iSphere].vLocation[2] = (float)((rand() % 400) - 200) * 0.1f;
	}

	for (iSphere = 0; iSphere < NUM_TEAPOTS; iSphere++)
	{
		gltInitFrame(&teapots[iSphere]);    // ���������������� ������        
											// ���������� ��������� ��������� ����� -20 � 20 � ����� 0.1
		teapots[iSphere].vLocation[0] = (float)((rand() % 400) - 200) * 0.1f;
		teapots[iSphere].vLocation[1] = 0.0f;
		teapots[iSphere].vLocation[2] = (float)((rand() % 400) - 200) * 0.1f;
		teapots[iSphere].aColor[0] = (GLfloat)(rand() % 1000) / 1000;
		teapots[iSphere].aColor[1] = (GLfloat)(rand() % 1000) / 1000;
		teapots[iSphere].aColor[2] = (GLfloat)(rand() % 1000) / 1000;
	}

	for (iSphere = 0; iSphere < NUM_CUBES; iSphere++)
	{
		gltInitFrame(&cubes[iSphere]);    // ���������������� ������        
											// ���������� ��������� ��������� ����� -20 � 20 � ����� 0.1
		cubes[iSphere].vLocation[0] = (float)((rand() % 400) - 200) * 0.1f;
		cubes[iSphere].vLocation[1] = 0.0f;
		cubes[iSphere].vLocation[2] = (float)((rand() % 400) - 200) * 0.1f;
		cubes[iSphere].aColor[0] = (GLfloat)(rand() % 1000) / 1000;
		cubes[iSphere].aColor[1] = (GLfloat)(rand() % 1000) / 1000;
		cubes[iSphere].aColor[2] = (GLfloat)(rand() % 1000) / 1000;
	}
}


///////////////////////////////////////////////////////////
// �������� ����� � ������������ ������
void DrawGround(void)
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iLine;

	glBegin(GL_LINES);
	for (iLine = -fExtent; iLine <= fExtent; iLine += fStep)
	{
		glVertex3f(iLine, y, fExtent);    // �������� z-�������
		glVertex3f(iLine, y, -fExtent);

		glVertex3f(fExtent, y, iLine);
		glVertex3f(-fExtent, y, iLine);
	}

	glEnd();
}


// ���������� ��� ��������� �����
void RenderScene(void)
{
	int i;
	static GLfloat yRot = 0.0f; // ������������ � �������� ���� ��������
	static GLfloat rComponent, bComponent, gComponent;
	yRot += 0.5f;

	// ������� ���� ������� ������ �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gltApplyCameraTransform(&frameCamera);

	// ������ �����
	DrawGround();

	// ��� �������� � ��������� ����
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ������ ��������� ������� ������������� �����
	for (i = 0; i < NUM_SPHERES; i++)
	{
		glPushMatrix();
		gltApplyActorTransform(&spheres[i]);
		glutSolidSphere(0.1f, 13, 26);
		//glutSolidTeapot(0.1);
		glPopMatrix();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// ������ ��������� ������� ������������� ������� ���
	for (i = 0; i < NUM_TEAPOTS; i++) {

		glColor3f(teapots[i].aColor[0], teapots[i].aColor[1], teapots[i].aColor[2]);

		glPushMatrix();
		gltApplyActorTransform(&teapots[i]);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glutSolidTeapot(0.1);
		glPopMatrix();
	}

	// ������ �������� ������������� ����
	for (i = 0; i < NUM_CUBES; i++) {
		

		glColor3f(cubes[i].aColor[0], cubes[i].aColor[1], cubes[i].aColor[2]);

		glPushMatrix();
		gltApplyActorTransform(&cubes[i]);
		glRotatef(yRot, 1.0f, 0.0f, 0.0f);
		glutSolidCube(0.1);
		glPopMatrix();
	}

	glColor3f(1, 1, 1);
	// ��� �������� � ��������� ����
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.5f);

	glPushMatrix();
	glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.1f, 13, 26);
	glPopMatrix();

	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	gltDrawTorus(0.35, 0.15, 40, 20);
	glPopMatrix();
	glPopMatrix();

	// ������������ �������
	glutSwapBuffers();
}



// ��������� �� ������� �� ���������, ������ ������� ������� ������
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		gltMoveFrameForward(&frameCamera, 0.1f);

	if (key == GLUT_KEY_DOWN)
		gltMoveFrameForward(&frameCamera, -0.1f);

	if (key == GLUT_KEY_LEFT)
		gltRotateFrameLocalY(&frameCamera, 0.1);

	if (key == GLUT_KEY_RIGHT)
		gltRotateFrameLocalY(&frameCamera, -0.1);

	// ���������� ����
	glutPostRedisplay();
}


///////////////////////////////////////////////////////////
//���������� ����������� GLUT � �������� ���������  (���� �� ������ 
// ������� � �� ������������) 
void TimerFunction(int value)
{
	// �������������� ����� � ������ ������������
	glutPostRedisplay();
	glutTimerFunc(3, TimerFunction, 1);
}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// ������������� ������� �� ����, ����� ���� ������� ��������� 
	// (������ ������� ���� ������� ������). 
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	// ������� ��������� ����������� ����� ������������
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ��������������� ����� �������
	gluPerspective(35.0f, fAspect, 1.0f, 50.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL NotOnlySphereWorld Demo");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);

	SetupRC();
	glutTimerFunc(33, TimerFunction, 1);

	glutMainLoop();

	return 0;
}

