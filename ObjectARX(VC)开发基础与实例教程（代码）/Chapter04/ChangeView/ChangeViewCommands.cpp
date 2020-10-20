//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Others\ConvertUtil.h"
#include "..\..\Common\SymbolTable\ViewUtil.h"
#include <geassign.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include <dbents.h>

// This is command 'ZOOMWINDOW'
void ZffChap4ZoomWindow()
{
	// ��ʾ�û�ѡ�������Ŵ��ڵ������ǵ�
	ads_point pt1, pt2;
	if (acedGetPoint(NULL, TEXT("\n�����һ���ǵ�:"), pt1) != RTNORM)
		return;
	if (acedGetCorner(pt1, TEXT("\n����ڶ����ǵ�:"), pt2) != RTNORM)
		return;

	// �������ǵ������ת������������ϵ
	AcGePoint3d pt1Wcs = CConvertUtil::UcsToWcsPoint(asPnt3d(pt1));
	AcGePoint3d pt2Wcs = CConvertUtil::UcsToWcsPoint(asPnt3d(pt2));
	
	CViewUtil::Set(pt1Wcs, pt2Wcs, 1.0);
}

// This is command 'ZOOMEXTENT'
void ZffCHAP4ZoomExtent()
{
	CViewUtil::ZoomExtent();
}

// This is command 'ZOOMENTITY'
void ZffCHAP4ZoomEntity()
{
	// Ѱ��ͼ���е�Բ�εİ�Χ��
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
	bool bFound = false;
	AcDbExtents ext;
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbCircle *pCircle = NULL;
		if (acdbOpenObject(pCircle, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			pCircle->getGeomExtents(ext);
			bFound = true;

			pCircle->close();
		}
	}

	// ��ͼ���е�Բ����ʾ��ͼ�δ��ڵ�����
	if (bFound)
	{
		CViewUtil::Set(ext.minPoint(), ext.maxPoint(), 5.0);
	}
}

