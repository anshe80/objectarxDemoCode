//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbsymtb.h>
#include <dbents.h>

// This is command 'ADDBLK'
void ZffChap3AddBlk()
{
	// ��õ�ǰͼ�����ݿ�Ŀ��
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForWrite);
	
	// �����µĿ���¼
	AcDbBlockTableRecord *pBlkTblRcd = new AcDbBlockTableRecord();
	
	// �����û����������ÿ���¼������
	TCHAR blkName[40];
	if (acedGetString(Adesk::kFalse, TEXT("\n����ͼ�������:"), blkName) != RTNORM)
	{
		pBlkTbl->close();
		delete pBlkTblRcd;
		return;
	}
	pBlkTblRcd->setName(blkName);
	
	// ������¼��ӵ������
	AcDbObjectId blkDefId;
	pBlkTbl->add(blkDefId, pBlkTblRcd);
	pBlkTbl->close();
	
	// �����¼�����ʵ��
	AcGePoint3d ptStart(-10, 0, 0), ptEnd(10, 0, 0);
	AcDbLine *pLine1 = new AcDbLine(ptStart, ptEnd);	// ����һ��ֱ��
	ptStart.set(0, -10, 0);
	ptEnd.set(0, 10, 0);
	AcDbLine *pLine2 = new AcDbLine(ptStart, ptEnd);	// ����һ��ֱ��
	AcGeVector3d vecNormal(0, 0, 1);
	AcDbCircle *pCircle = new AcDbCircle(AcGePoint3d::kOrigin, vecNormal, 6);
	
	// ����һ������  ����ֱ��
	AcDbAttributeDefinition *pAttDef = new AcDbAttributeDefinition(
		ptEnd, TEXT("20"), TEXT("ֱ��"), TEXT("����ֱ��"));	
	
	AcDbObjectId entId;
	pBlkTblRcd->appendAcDbEntity(entId, pLine1);
	pBlkTblRcd->appendAcDbEntity(entId, pLine2);
	pBlkTblRcd->appendAcDbEntity(entId, pCircle);
	pBlkTblRcd->appendAcDbEntity(entId, pAttDef);
	
	// �ر�ʵ��Ϳ���¼
	pLine1->close();
	pLine2->close();
	pCircle->close();
	pAttDef->close();
	pBlkTblRcd->close();
}

