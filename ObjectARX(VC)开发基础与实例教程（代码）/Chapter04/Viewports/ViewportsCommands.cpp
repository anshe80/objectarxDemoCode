//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbsymtb.h>
#include <AcDbLMgr.h>
#include <dbents.h>

// This is command 'CREATE4VPORTS'
void ZffChap4Create4VPorts()
{
	// ����ӿڱ�
	AcDbViewportTable *pVPortTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getViewportTable(pVPortTbl, AcDb::kForWrite);
	
	// �ֱ𴴽��ĸ��ӿ�
	AcGePoint2d pt1, pt2;
	AcDbViewportTableRecord *pVPortTblRcd1 = new AcDbViewportTableRecord;
	pt1.set(0, 0);
	pt2.set(0.5, 0.5);
	pVPortTblRcd1->setLowerLeftCorner(pt1);
	pVPortTblRcd1->setUpperRightCorner(pt2);
	pVPortTblRcd1->setName(TEXT("4VPorts"));
	
	AcDbViewportTableRecord *pVPortTblRcd2 = new AcDbViewportTableRecord;
	pt1.set(0.5, 0);
	pt2.set(1, 0.5);
	pVPortTblRcd2->setLowerLeftCorner(pt1);
	pVPortTblRcd2->setUpperRightCorner(pt2);
	pVPortTblRcd2->setName(TEXT("4VPorts"));
	
	AcDbViewportTableRecord *pVPortTblRcd3 = new AcDbViewportTableRecord;
	pt1.set(0, 0.5);
	pt2.set(0.5, 1);
	pVPortTblRcd3->setLowerLeftCorner(pt1);
	pVPortTblRcd3->setUpperRightCorner(pt2);
	pVPortTblRcd3->setName("4VPorts");
	
	AcDbViewportTableRecord *pVPortTblRcd4 = new AcDbViewportTableRecord;
	pt1.set(0.5, 0.5);
	pt2.set(1, 1);
	pVPortTblRcd4->setLowerLeftCorner(pt1);
	pVPortTblRcd4->setUpperRightCorner(pt2);
	pVPortTblRcd4->setName(TEXT("4VPorts"));	
	
	pVPortTbl->add(pVPortTblRcd1);
	pVPortTbl->add(pVPortTblRcd2);
	pVPortTbl->add(pVPortTblRcd3);
	pVPortTbl->add(pVPortTblRcd4);
	
	pVPortTbl->close();	
	pVPortTblRcd1->close();
	pVPortTblRcd2->close();
	pVPortTblRcd3->close();
	pVPortTblRcd4->close();
	
	// �жϵ�ǰ�Ŀռ�
	struct resbuf rb;
	acedGetVar(TEXT("TILEMODE"), &rb);
	if (rb.resval.rint == 1)		// ��ǰ�����ռ���ģ�Ϳռ�
	{
		acedCommand(RTSTR, TEXT(".-VPORTS"), RTSTR, TEXT("R"), 
			RTSTR, TEXT("4VPorts"), RTNONE);
	}
	else							// ��ǰ�����ռ���ͼֽ�ռ�
	{
		acedCommand(RTSTR, TEXT(".-VPORTS"), RTSTR, TEXT("R"), 
			RTSTR, TEXT("4VPorts"), RTSTR, TEXT(""), RTNONE);
	}
}

// This is command 'CREATEVPORTINSPACE'
void ZffChap4CreateVPortInSpace()
{
	// ָ����ǰ����
	Acad::ErrorStatus es = acdbHostApplicationServices()->layoutManager()
		->setCurrentLayout(TEXT("����1"));	
	if (es != Acad::eOk)
	{
		return;
	}
	
	// ��ÿ��
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	
	// ���ͼֽ�ռ�Ŀ���¼
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd, AcDb::kForWrite);
	pBlkTbl->close();
	
	// �����µĲ��ֶ���
	AcDbViewport *pViewport = new AcDbViewport();
	pViewport->setCenterPoint(AcGePoint3d(100, 50, 0));
	pViewport->setHeight(80);
	pViewport->setWidth(120);	
	
	// ���µĲ��ֶ�����ӵ�ͼֽ�ռ����¼��
	AcDbObjectId viewportId;
	pBlkTblRcd->appendAcDbEntity(viewportId, pViewport);
	
	pViewport->close();
	pBlkTblRcd->close();
	
	// ���½����ӿ���Ϊ��ǰ�ӿ�
	AcDbViewport *pVP = NULL;
	if (acdbOpenObject(pVP, viewportId, AcDb::kForWrite) == Acad::eOk)
	{
		pViewport->setOn();
		acedSetCurrentVPort(pVP);
		pVP->close();
	}	
}

