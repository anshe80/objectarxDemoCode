//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <math.h>
#include "..\..\..\..\..\..\Autodesk\ARX�������\inc\Common\Geometry\MathUtil.h"
#include <gemat3d.h>
#include <dbents.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include "..\..\Common\Interaction\GetInputUtil.h"

// �����û����������������㷨����
static AcGeVector3d GetNormalByInVector(const AcGeVector3d &inVector)
{
    double tol = 1.0E-7;
    if (fabs(inVector.x) < tol && fabs(inVector.y) < tol)
    {
        if (inVector.z >= 0)
        {
            return AcGeVector3d(-1, 0, 0);
        }
        else
        {
            return AcGeVector3d::kXAxis;
        }
    }
    else
    {
        AcGeVector2d yAxis2d(inVector.x, inVector.y);
        yAxis2d = yAxis2d.rotateBy(CMathUtil::PI() * 0.5);
        AcGeVector3d yAxis(yAxis2d.x, yAxis2d.y, 0);
        AcGeVector3d normal = yAxis;
        normal = normal.rotateBy(CMathUtil::PI() * 0.5, inVector);
        return normal;
    }
}

// ��ô�WCS��UCS�ı任����
static AcGeMatrix3d GetTranslateMatrix(const AcGePoint3d &inPoint, const AcGeVector3d &inVector, const AcGeVector3d &normal)
{
    AcGeVector3d xAxis = inVector;
    xAxis.normalize();
    AcGeVector3d zAxis = normal;
    zAxis.normalize();
    AcGeVector3d yAxis(xAxis.x, xAxis.y, xAxis.z);
    yAxis = yAxis.rotateBy(CMathUtil::PI() * 0.5, zAxis);

	AcGeMatrix3d mat = AcGeMatrix3d::alignCoordSys(AcGePoint3d::kOrigin, AcGeVector3d::kXAxis, AcGeVector3d::kYAxis, AcGeVector3d::kZAxis, 
		inPoint, xAxis, yAxis, zAxis);

	return mat;
}

// ��������Ĺܵ���㡢�յ㡢�����������ܵ�
static void DrawPipe(const AcGePoint3d &startPoint, const AcGePoint3d &endPoint, double width, double height)
{
    // ��ñ任����
    AcGeVector3d inVector = endPoint - startPoint;      // �������
    AcGeVector3d normal = GetNormalByInVector(inVector);       // ������
    AcGeMatrix3d mat = GetTranslateMatrix(startPoint, inVector, normal);
	
    // ����
	int colorIndex = 54;
	double z = 0.5 * height;
	double length = startPoint.distanceTo(endPoint);
	AcDbFace *pTopFace = new AcDbFace(AcGePoint3d(0, -0.5 * width, z), AcGePoint3d(length, -0.5 * width, z), 
		AcGePoint3d(length, 0.5 * width, z), AcGePoint3d(0, 0.5 * width, z));
	pTopFace->transformBy(mat);
	pTopFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pTopFace);
	
	// ����
	z = -0.5 * height;
	AcDbFace *pBottomFace = new AcDbFace(AcGePoint3d(0, -0.5 * width, z), AcGePoint3d(length, -0.5 * width, z), 
		AcGePoint3d(length, 0.5 * width, z), AcGePoint3d(0, 0.5 * width, z));
	pBottomFace->transformBy(mat);
	pBottomFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pBottomFace);
	
	// �����
	double y = 0.5 * width;
	AcDbFace *pLeftSideFace = new AcDbFace(AcGePoint3d(0, y, 0.5 * height), AcGePoint3d(length, y, 0.5 * height), 
		AcGePoint3d(length, y, -0.5 * height), AcGePoint3d(0, y, -0.5 * height));
	pLeftSideFace->transformBy(mat);
	pLeftSideFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pLeftSideFace);
	
	// �����
	y = -0.5 * width;
	AcDbFace *pRightSideFace = new AcDbFace(AcGePoint3d(0, y, 0.5 * height), AcGePoint3d(length, y, 0.5 * height), 
		AcGePoint3d(length, y, -0.5 * height), AcGePoint3d(0, y, -0.5 * height));
	pRightSideFace->transformBy(mat);
	pRightSideFace->setColorIndex(54);
	CDwgDatabaseUtil::PostToModelSpace(pRightSideFace);
}

// This is command 'DRAWPIPE'
void ZffChap8DrawPipe()
{
	// ��ʾ�û�������㡢�յ�
	AcGePoint3d startPoint, endPoint;
    if (CGetInputUtil::GetPoint(TEXT("\n�������:"), startPoint) && 
		CGetInputUtil::GetPoint(startPoint, TEXT("\n�����յ�:"), endPoint))
    {
        // ���ƹܵ�
        DrawPipe(startPoint, endPoint, 100, 70);
    }
}

