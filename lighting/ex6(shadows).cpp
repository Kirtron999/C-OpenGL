#include "glew.h"
#include "glut.h"
#include <math.h>

// Определяется константа со значением “пи”
#define GL_PI 3.1415f

// Параметры поворота
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
GLfloat	 lightPos1[] = { -150.0f, 150.0f, -50.0f, 0.0f };
GLfloat	 lightPos2[] = { 150.0f, 150.0f, -50.0f, 0.0f };
static GLfloat  ambientLight[] = { 0.15f, 0.15f, 0.15f, 1.0f };
static GLfloat  diffuseLight[] = { 0.35f, 0.35f, 0.35f, 1.0f };
static GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };


typedef GLfloat GLTVector3[3];
typedef GLfloat GLTVector2[2];//Двукомпонентный вектор с плавающей запятой
typedef GLfloat GLTVector3[3];//Трехомпонентный вектор с плавающей запятой
typedef GLfloat GLTVector4[4];//Четырехкомпонентный вектор с плавающей //запятой
typedef GLfloat GLTMatrix[16];// Основноц столбец матрицы 4х4 с плавающей //запятой

// Матрица преобразования, дающая проекцию тени
GLTMatrix shadowMat1, shadowMat2;

void gltScaleVector(GLTVector3 vVector, const GLfloat fScale)
{
	vVector[0] *= fScale; vVector[1] *= fScale; vVector[2] *= fScale;
}
// Возвращает длину вектора в квадрате
GLfloat gltGetVectorLengthSqrd(const GLTVector3 vVector)
{
	return (vVector[0] * vVector[0]) + (vVector[1] * vVector[1]) + (vVector[2] * vVector[2]);
}
// Возвращает длину вектора 
GLfloat gltGetVectorLength(const GLTVector3 vVector)
{
	return (GLfloat)sqrt(gltGetVectorLengthSqrd(vVector));
}
// Вычитание одного вектора из другого
void gltSubtractVectors(const GLTVector3 vFirst, const GLTVector3 vSecond, GLTVector3 vResult)
{
	vResult[0] = vFirst[0] - vSecond[0];
	vResult[1] = vFirst[1] - vSecond[1];
	vResult[2] = vFirst[2] - vSecond[2];
}
// Вычислить векторное произведение двух векторов
void gltVectorCrossProduct(const GLTVector3 vU, const GLTVector3 vV, GLTVector3 vResult)
{
	vResult[0] = vU[1] * vV[2] - vV[1] * vU[2];
	vResult[1] = -vU[0] * vV[2] + vV[0] * vU[2];
	vResult[2] = vU[0] * vV[1] - vV[0] * vU[1];
}
// Масштабирование вектора по длине - создание единичсного вектора
void gltNormalizeVector(GLTVector3 vNormal)
{
	GLfloat fLength = 1.0f / gltGetVectorLength(vNormal);
	gltScaleVector(vNormal, fLength);
}
// Три точки на плоскости расположены против часовой стрелки, вычисление 
// нормали
void gltGetNormalVector(const GLTVector3 vP1, const GLTVector3 vP2, const GLTVector3 vP3, GLTVector3 vNormal)
{
	GLTVector3 vV1, vV2;

	gltSubtractVectors(vP2, vP1, vV1);
	gltSubtractVectors(vP3, vP1, vV2);

	gltVectorCrossProduct(vV1, vV2, vNormal);
	gltNormalizeVector(vNormal);
}



// Полученные три коэффициента уравнения плоскости дают три точки на поверхности
void gltGetPlaneEquation(GLTVector3 vPoint1, GLTVector3 vPoint2, GLTVector3 vPoint3, GLTVector3 vPlane)
{
	// Получение нормали из трех точек. Нормаль – первые три коэффициента 
	// уравнения плоскости
	gltGetNormalVector(vPoint1, vPoint2, vPoint3, vPlane);

	// Итоговый коэффициент находится обратной подстановкой
	vPlane[3] = -(vPlane[0] * vPoint3[0] + vPlane[1] * vPoint3[1] + vPlane[2] * vPoint3[2]);
}
// Создание матрицы теневой проекции из коэффициентов уравнения плоскости и // положение света. Возвращаемое значение хранится в destMat
void gltMakeShadowMatrix(GLTVector3 vPoints[3], GLTVector4 vLightPos, GLTMatrix destMat)
{
	GLTVector4 vPlaneEquation;
	GLfloat dot;

	gltGetPlaneEquation(vPoints[0], vPoints[1], vPoints[2], vPlaneEquation);

	// Скалярное произведение положение самолета и света
	dot = vPlaneEquation[0] * vLightPos[0] +
		vPlaneEquation[1] * vLightPos[1] +
		vPlaneEquation[2] * vLightPos[2] +
		vPlaneEquation[3] * vLightPos[3];


	// Проецируем
	// Первый столбец
	destMat[0] = dot - vLightPos[0] * vPlaneEquation[0];
	destMat[4] = 0.0f - vLightPos[0] * vPlaneEquation[1];
	destMat[8] = 0.0f - vLightPos[0] * vPlaneEquation[2];
	destMat[12] = 0.0f - vLightPos[0] * vPlaneEquation[3];

	// Второй столбец
	destMat[1] = 0.0f - vLightPos[1] * vPlaneEquation[0];
	destMat[5] = dot - vLightPos[1] * vPlaneEquation[1];
	destMat[9] = 0.0f - vLightPos[1] * vPlaneEquation[2];
	destMat[13] = 0.0f - vLightPos[1] * vPlaneEquation[3];

	// Третий столбец
	destMat[2] = 0.0f - vLightPos[2] * vPlaneEquation[0];
	destMat[6] = 0.0f - vLightPos[2] * vPlaneEquation[1];
	destMat[10] = dot - vLightPos[2] * vPlaneEquation[2];
	destMat[14] = 0.0f - vLightPos[2] * vPlaneEquation[3];

	// Четвертый столбец
	destMat[3] = 0.0f - vLightPos[3] * vPlaneEquation[0];
	destMat[7] = 0.0f - vLightPos[3] * vPlaneEquation[1];
	destMat[11] = 0.0f - vLightPos[3] * vPlaneEquation[2];
	destMat[15] = dot - vLightPos[3] * vPlaneEquation[3];
}

void DrawCar(int nShadow) {

	GLenum mode = GL_POLYGON;
	bool solid = true;
	GLUquadricObj *quadObj = gluNewQuadric();


	
	glPushMatrix();
	// передние фары
	glPushMatrix();
	glTranslatef(0.75, 0.0, 2.01);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(1.0, 1.0, 1.0);
	gluDisk(quadObj, 0.0, 0.2, 10, 10);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(1.0, 0.5, 0.0);
	gluDisk(quadObj, 0.0, 0.1, 4, 4);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.75, 0.0, 2.01);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(1.0, 1.0, 1.0);
	gluDisk(quadObj, 0.0, 0.2, 10, 10);
	glTranslatef(0.0, -0.4, 0.0);
	glRotatef(45, 0.0, 0.0, 1.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(1.0, 0.5, 0.0);
	gluDisk(quadObj, 0.0, 0.1, 4, 4);
	glPopMatrix();

	// задние фары
	if (solid)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();
	glTranslatef(-0.75, -0.3, -2.01);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(1.0, 0.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glRectf(0.1, 0.1, -0.1, -0.1);
	glTranslatef(1.5, 0.0, 0.0);
	glRectf(0.1, 0.1, -0.1, -0.1);
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
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

	//переднее стекло
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
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.01);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
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
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.5, 1.0, 0.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);

	glTranslatef(0.0, 0.0, -0.29);
	glRotatef(180, 0.5, 1.0, 0.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7, -0.5, -1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.0, 1.0, 0.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.1, 1.0, 0.5);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);

	glTranslatef(0.0, 0.0, -0.29);
	glRotatef(180, 0.0, 1.0, 0.0);
	gluDisk(quadObj, 0.0, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, -0.5, -1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.5, 0.6, 0.5);
	gluCylinder(quadObj, 0.4, 0.4, 0.29, 20, 10);

	glRotatef(180, 0.5, 1.0, 0.0);
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
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
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
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
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.0, 0.0, 1.0);
	gluSphere(quadObj, 0.17, 20, 20);
	glPopMatrix();

	glPushMatrix();
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.1, 1.0, 1.0);
	glTranslatef(0.5, 0.5, 0.5);
	glRotatef(30, 1.0, 0.0, 0.0);
	// баранка
	if (solid)
		glutSolidTorus(0.05, 0.15, 3, 10);
	else
		glutWireTorus(0.05, 0.15, 3, 10);

	glPopMatrix();

	glPushMatrix();
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.5, 0.0, 0.9);
	glTranslatef(-0.1, -0.4, 1.0);

	// мотор
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
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
		glColor3f(0.7, 0.7, 0.7);
	// рулевая колонка
	gluCylinder(quadObj, 0.03, 0.03, 1.0, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.1, 0.8);
	glRotatef(70, 1.0, 0.0, 0.0);
	gluCylinder(quadObj, 0.09, 0.09, 0.7, 10, 10);
	glPopMatrix();

	glPushMatrix();
	// кресло
	if (nShadow == 0)
		glColor3ub(0, 0, 0);
	else
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
}

//Вызывается для рисования сцены 
void RenderScene(void)
{
	GLenum mode = GL_POLYGON;
	bool solid = true;
	GLUquadricObj *quadObj = gluNewQuadric();
	// Очищаем окно текущим цветом очистки
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	//glNormal3f(0, 1, 0);
	
	glColor3ub(0, 255, 0);
	glVertex3f(400.0f, -150.0f, -200.0f);
	glVertex3f(-400.0f, -150.0f, -200.0f);

	glColor3ub(0, 32, 0);
	glVertex3f(-400.0f, -150.0f, 200.0f);
	glVertex3f(400.0f, -150.0f, 200.0f);

	glEnd();

	glPushMatrix();

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glScalef(30, 30, 30);
	DrawCar(1);
	glScalef(0.3, 0.3, 0.3);

	glPopMatrix();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glPushMatrix();

	// Текущая матрица множится на матрицу проекции тени 
	glMultMatrixf((GLfloat *)shadowMat1);
	

	// Машинка поворачивается в новом плоском пространстве 
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Чтобы обозначить рисование тени, функции передается 
	// значение 1 
	glScalef(30, 30, 30);
	DrawCar(0);
	glScalef(0.3, 0.3, 0.3);

	// Восстанавливается нормальная проекция 
	glPopMatrix();

	glPushMatrix();

	// Текущая матрица множится на матрицу проекции тени 
	glMultMatrixf((GLfloat *)shadowMat2);


	// Машинка поворачивается в новом плоском пространстве 
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Чтобы обозначить рисование тени, функции передается 
	// значение 1 
	glScalef(30, 30, 30);
	DrawCar(0);
	glScalef(0.3, 0.3, 0.3);

	// Восстанавливается нормальная проекция 
	glPopMatrix();

	// Рисуется источник света 
	glPushMatrix();
	glTranslatef(lightPos1[0], lightPos1[1], lightPos1[2]);
	glColor3ub(255, 255, 0);
	glutSolidSphere(5.0f, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lightPos2[0], lightPos2[1], lightPos2[2]);
	glColor3ub(255, 255, 0);
	glutSolidSphere(5.0f, 10, 10);
	glPopMatrix();

	// Восстанавливаются переменные состояния освещения 
	glEnable(GL_DEPTH_TEST);

	// Отображаются результаты
	glutSwapBuffers();
}

// Функция выполняет необходимую инициализацию 
// в контексте визуализации 
void SetupRC()
{
	// Параметры света
	// Яркий белый свет
	//GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLTVector3 points[3] = { { -30.0f, -149.0f, -20.0f },
							 { -30.0f, -149.0f, 20.0f },
							 { 40.0f, -149.0f, 20.0f } };



	glEnable(GL_DEPTH_TEST);	// Удаление скрытых частей
	glFrontFace(GL_CCW);// Многоугольники с обходом против часовой стрелки 
						//направлены наружу

						// Освещение материала
	glEnable(GL_LIGHTING);			// Активизация освещения	

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glEnable(GL_LIGHT1);



	glEnable(GL_COLOR_MATERIAL);	// Активизируется согласование цветов

									//Рассеянный и диффузный цвета передней стороны объектов 
									//соответствуют тому, что указано в glColor

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Светло-синий цвет
	glClearColor(0.0f, 0.0f, 05.f, 1.0f);

	gltMakeShadowMatrix(points, lightPos1, shadowMat1);
	gltMakeShadowMatrix(points, lightPos2, shadowMat2);
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
	GLfloat fAspect;

	// Предотвращение деления на ноль
	if (h == 0)
		h = 1;


	// Устанавливает размеры поля просмотра равны размерам окна
	glViewport(0, 0, w, h);

	// Обновляет систему координат
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(60.0f, fAspect, 200.0, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Перемшаемся вдоль оси Z в поле зрения
	glTranslatef(0.0f, 0.0f, -400.0f);


	glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Shadows");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();

	return 0;
}