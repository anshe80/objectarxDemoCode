//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\ArcUtil.h"
#include <complex>

// This is command 'ADDARC'
void ZffChap2AddArc()
{
	// ����λ��XOYƽ���ϵ�Բ��
	AcGePoint2d ptCenter(50, 50);
	CArcUtil::Add(ptCenter, 100 * sqrt(2) / 2, 5 * CMathUtil::PI() / 4, 
		7 * CMathUtil::PI() / 4);
	
	// ���㷨����Բ��
	AcGePoint2d ptStart(100, 0);
	AcGePoint2d ptOnArc(120, 50);
	AcGePoint2d ptEnd(100, 100);
	CArcUtil::Add(ptStart, ptOnArc, ptEnd);
	
	// "��㡢Բ�ġ��յ�"����Բ��
	ptStart.set(100, 100);
	ptCenter.set(50, 50);
	ptEnd.set(0, 100);
	CArcUtil::AddBySCE(ptStart, ptCenter, ptEnd);
	
	// "��㡢Բ�ġ�Բ���Ƕ�"����Բ��
	ptStart.set(0, 100);
	ptCenter.set(50, 50);
	CArcUtil::Add(ptStart, ptCenter, CMathUtil::PI() / 2);
}

