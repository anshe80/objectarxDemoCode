//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "dbents.h"
#include "..\..\Common\Entity\LineUtil.h"
#include "..\..\Common\Entity\EntityUtil.h"

// ����ֱ�ߵĺ���
AcDbObjectId CreateLine()
{	
	AcGePoint3d ptStart(0, 0, 0);
	AcGePoint3d ptEnd(100, 100, 0);
	AcDbLine *pLine = new AcDbLine(ptStart, ptEnd);	
	
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlockTable, AcDb::kForRead);	
	
	AcDbBlockTableRecord *pBlockTableRecord;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
        AcDb::kForWrite);   	
	
	AcDbObjectId lineId;
    pBlockTableRecord->appendAcDbEntity(lineId, pLine);	
	
	pBlockTable->close();
    pBlockTableRecord->close();
    pLine->close();	

	return lineId;
}

// �޸�ָ��ʵ�����ɫ
Acad::ErrorStatus ChangeColor(AcDbObjectId entId, Adesk::UInt16 colorIndex)
{
	AcDbEntity *pEntity;
	// ��ͼ�����ݿ��еĶ���
    acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	
	// �޸�ʵ�����ɫ
    pEntity->setColorIndex(colorIndex);
	
	// ����֮�󣬼�ʱ�ر�
    pEntity->close();
	
    return Acad::eOk;	
}

// This is command 'CHANGECOLOR'
void ZffCHAP2ChangeColor()
{
	// ����ֱ��
	AcDbObjectId lineId;
	lineId = CreateLine();

	// ��ֱ�ߵ���ɫ�޸�Ϊ��ɫ
	ChangeColor(lineId, 1);
}

// This is command 'CHANGECOLOR2'
void ZffCHAP2ChangeColor2()
{
	// ����ֱ��
	AcGePoint3d startPoint(0, 0, 0);
	AcGePoint3d endPoint(100, 100, 0);
	AcDbObjectId lineId = CLineUtil::Add(startPoint, endPoint);

	// �޸���ɫΪ��ɫ
	CEntityUtil::SetColor(lineId, 1);

	// �޸�ʵ���ͼ�������
	CEntityUtil::SetLayer(lineId, TEXT("������"));
	CEntityUtil::SetLinetype(lineId, TEXT("HIDDEN"));
}

