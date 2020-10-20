//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\EllipseUtil.h"

// This is command 'ADDELLIPSE'
void ZffChap2AddEllipse()
{
	// ʹ�����ĵ㡢����ƽ�桢����ʸ���Ͷ̳��������������Բ
	AcGeVector3d vecNormal(0, 0, 1);
	AcGeVector3d majorAxis(40, 0, 0);
	AcDbObjectId entId;
	entId = CEllipseUtil::Add(AcGePoint3d::kOrigin, vecNormal, majorAxis, 0.5);
	
	// ʹ����Ӿ�����������Բ
	AcGePoint2d pt1(60, 80), pt2(140, 120);
	CEllipseUtil::Add(pt1, pt2);
}

// This is command 'ADDSPLINE'
void ZffCHAP2AddSpline()
{
	// ʹ��������ֱ�Ӵ�����������
	AcGePoint3d pt1(0, 0, 0), pt2(10, 30, 0), pt3(60, 80, 0), pt4(100, 100, 0);
	AcGePoint3dArray points;
	points.append(pt1);
	points.append(pt2);
	points.append(pt3);
	points.append(pt4);
	CSplineUtil::Add(points);	
	
	// ָ����ʼ�����ֹ������߷��򣬴�����������
	pt2.set(30, 10, 0);
	pt3.set(80, 60, 0);
	
	points.removeSubArray(0, 3);
	points.append(pt1);
	points.append(pt2);
	points.append(pt3);
	points.append(pt4);
	
	AcGeVector3d startTangent(5, 1, 0);
	AcGeVector3d endTangent(5, 1, 0);
	CSplineUtil::Add(points, startTangent, endTangent);
}

