//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\circleutil.h"

// This is command 'ADDCIRCLE'
void ZffChap2AddCircle()
{
	// "Բ�ġ��뾶"������һ��Բ
	AcGePoint3d ptCenter(100, 100, 0);
	CCircleUtil::Add(ptCenter, 20);
	
	// ���㷨����һ��Բ
	AcGePoint2d pt1(70, 100);
	AcGePoint2d pt2(130, 100);
	CCircleUtil::Add(pt1, pt2);
	
	// ���㷨����һ��Բ
	pt1.set(60, 100);
	pt2.set(140, 100);
	AcGePoint2d pt3(100, 60);
	CCircleUtil::Add(pt1, pt2, pt3);
}

