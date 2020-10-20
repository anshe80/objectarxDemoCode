//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "dbents.h"
//#include "dbapserv.h"

// This is command 'CREATELINE'
void ZffCHAP2CreateLine()
{
	// ���ڴ��ϴ���һ���µ�AcDbLine����
	AcGePoint3d ptStart(0, 0, 0);
	AcGePoint3d ptEnd(100, 100, 0);
	AcDbLine *pLine = new AcDbLine(ptStart, ptEnd);
	
	// ���ָ�����ָ��
	AcDbBlockTable *pBlockTable = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlockTable, AcDb::kForRead);

	// ���ָ���ض��Ŀ���¼��ģ�Ϳռ䣩��ָ��
	AcDbBlockTableRecord *pBlockTableRecord = NULL;
    pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
        AcDb::kForWrite);    
	
	// ��AcDbLine��Ķ�����ӵ�����¼��
	AcDbObjectId lineId;
    pBlockTableRecord->appendAcDbEntity(lineId, pLine);
	
	// �ر�ͼ�����ݿ�ĸ��ֶ���
	pBlockTable->close();
    pBlockTableRecord->close();
    pLine->close();	
}






















