//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\PolylineUtil.h"
#include "..\..\Common\Others\ConvertUtil.h"

// This is command 'ADDPOLYLINE'
void ZffChap2AddPolyline()
{
	// ����������һ��ֱ�ߵĶ����
	AcGePoint2d ptStart(0, 0), ptEnd(100, 100);
	CPolylineUtil::Add(ptStart, ptEnd, 1);
	
	// ������ά�����
	AcGePoint3d pt1(0, 0, 0), pt2(100, 0, 0), pt3(100, 100, 0);
	AcGePoint3dArray points;
	points.append(pt1);
	points.append(pt2);
	points.append(pt3);
	CPolylineUtil::Add3dPolyline(points);
	
	// �����������
	CPolylineUtil::AddPolygon(AcGePoint2d::kOrigin, 6, 30, 0, 1);
	
	// ��������
	AcGePoint2d pt(60, 70);
	CPolylineUtil::AddRectangle(pt, ptEnd, 1);
	
	// ����Բ
	pt.set(50, 50);
	CPolylineUtil::AddPolyCircle(pt, 30, 1);
	
	// ����Բ��
	CPolylineUtil::AddPolyArc(pt, 50, CConvertUtil::AngleToRadian(45), 
		CConvertUtil::AngleToRadian(225), 1);
}

