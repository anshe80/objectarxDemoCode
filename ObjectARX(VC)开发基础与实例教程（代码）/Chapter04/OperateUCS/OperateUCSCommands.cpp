//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbsymtb.h>
#include <gemat3d.h>
#include <math.h>
#include <dbents.h>
#include <geassign.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"

// This is command 'NEWUCS'
void ZffChap4NewUcs()
{
	// ��õ�ǰͼ�ε�UCS��
	AcDbUCSTable *pUcsTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getUCSTable(pUcsTbl, AcDb::kForWrite);
	
	// ����UCS�Ĳ���
	AcGePoint3d ptOrigin(0, 0, 0);
	AcGeVector3d vecXAxis(1, 1, 0);
	AcGeVector3d vecYAxis(-1, 1, 0);
	
	// �����µ�UCS���¼
	AcDbUCSTableRecord *pUcsTblRcd = new AcDbUCSTableRecord();
	
	// ����UCS�Ĳ���
	Acad::ErrorStatus es = pUcsTblRcd->setName(TEXT("NewUcs"));
	if (es != Acad::eOk)
	{
		delete pUcsTblRcd;
		pUcsTbl->close();
		return;
	}
	pUcsTblRcd->setOrigin(ptOrigin);
	pUcsTblRcd->setXAxis(vecXAxis);
	pUcsTblRcd->setYAxis(vecYAxis);
	
	// ���½���UCS���¼��ӵ�UCS����
	es = pUcsTbl->add(pUcsTblRcd);
	if (es != Acad::eOk)
	{
		delete pUcsTblRcd;
		pUcsTbl->close();
		return;
	}
	
	// �رն���
	pUcsTblRcd->close();
	pUcsTbl->close();
}

// This is command 'SETCURUCS'
void ZffCHAP4SetCurUcs()
{
	// ��ʾ�û�����UCS������
	TCHAR ucsName[40];
	if (acedGetString(NULL, TEXT("\n�����û�����ϵ������:"), ucsName) != RTNORM)
		return;
	
	// ���ָ����UCS���¼
	AcDbUCSTable *pUcsTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getUCSTable(pUcsTbl, AcDb::kForRead);
	if (!pUcsTbl->has(ucsName))
	{
		pUcsTbl->close();
		return;
	}
	AcDbUCSTableRecord *pUcsTblRcd;
	pUcsTbl->getAt(ucsName, pUcsTblRcd, AcDb::kForRead);
	
	// ���UCS�ı任����
	AcGeMatrix3d mat;
	AcGeVector3d vecXAxis, vecYAxis, vecZAxis;
	vecXAxis = pUcsTblRcd->xAxis();
	vecYAxis = pUcsTblRcd->yAxis();
	vecZAxis = vecXAxis.crossProduct(vecYAxis);
	mat.setCoordSystem(pUcsTblRcd->origin(), vecXAxis, 
		vecYAxis, vecZAxis);
	
	// �ر�UCS���UCS���¼
	pUcsTblRcd->close();
	pUcsTbl->close();
	
	// ���õ�ǰ��UCS
	acedSetCurrentUCS(mat);
}

// This is command 'MOVEUCSORIGIN'
void ZffCHAP4MoveUcsOrigin()
{
	// ��õ�ǰUCS�ı任����
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	
	// ���ݱ任������UCS�Ĳ���
	AcGePoint3d ptOrigin;
	AcGeVector3d vecXAxis, vecYAxis, vecZAxis;
	mat.getCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);
	
	// �ƶ�UCS��ԭ��
	AcGeVector3d vec(100, 100, 0);
	ptOrigin += vec;
	
	// ���±任����
	mat.setCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);
	
	// Ӧ���µ�UCS
	acedSetCurrentUCS(mat);
}

// This is command 'ROTATEUCS'
void ZffCHAP4RotateUcs()
{
	// ��õ�ǰUCS�ı任����
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	
	// ���ݱ任������UCS�Ĳ���
	AcGePoint3d ptOrigin;
	AcGeVector3d vecXAxis, vecYAxis, vecZAxis;
	mat.getCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);
	
	// ��Z����ת60��
	vecXAxis.rotateBy(60 * atan(1) * 4 / 180, vecZAxis);
	vecYAxis.rotateBy(60 * atan(1) * 4 / 180, vecZAxis);
	
	// ���±任����
	mat.setCoordSystem(ptOrigin, vecXAxis, vecYAxis, vecZAxis);
	
	// Ӧ���µ�UCS
	acedSetCurrentUCS(mat);
}

// This is command 'ADDENTINUCS'
void ZffCHAP4AddEntInUcs()
{
	// ��ʾ�û�����ֱ�ߵ������յ�
	ads_point pt1, pt2;
	if (acedGetPoint(NULL, TEXT("\nʰȡֱ�ߵ����:"), pt1) != RTNORM)
		return;	
	if (acedGetPoint(pt1, TEXT("ʰȡֱ�ߵ��յ�:"), pt2) != RTNORM)
		return;

	// ����UCS���괴��ʵ��
	AcDbLine *pLine = new AcDbLine(asPnt3d(pt1), asPnt3d(pt2));

	// ��ʵ����м��α任
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	pLine->transformBy(mat);

	// ���뵽ģ�Ϳռ�
	CDwgDatabaseUtil::PostToModelSpace(pLine);
}

