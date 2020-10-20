//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include <dbents.h>
#include "..\..\Common\Entity\LineUtil.h"
#include "..\..\Common\Entity\circleutil.h"
#include "..\..\Common\IO\AppDirectoryUtil.h"

// This is command 'NEWDOCDRAWENTS'
void ZffChap7NewDocDrawEnts()
{
	// 1.��ǰͼ���еļ���
	// ���������ĵ����ܷ���ͼ�����ݿ������
	acDocManager->lockDocument(acDocManager->curDocument());

	// �ҳ�ͼ����ֱ�ߵ�����
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
	int lineCount = 0;
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbLine *pLine = NULL;
		if (acdbOpenObject(pLine, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			lineCount++;
			pLine->close();
		}
	}

	// �����ĵ�������Ӧ�ó���Χ
	acDocManager->unlockDocument(acDocManager->curDocument());

	// 2.����һ����ͼ���ĵ�����Ϊ��ǰ�ĵ�
	Acad::ErrorStatus es = acDocManager->appContextNewDocument(NULL);

	// 3.���µ�ͼ���ĵ��л�ͼ
	// ���������ĵ����ܷ���ͼ�����ݿ������
	acDocManager->lockDocument(acDocManager->curDocument());

	// ������ԭ��ͼ������ͬ������ֱ��
	for (i = 0; i < lineCount; i++)
	{
		double y = i * 20;
		AcGePoint3d startPoint(0, y, 0);
		AcGePoint3d endPoint(100, y, 0);
		CLineUtil::Add(startPoint, endPoint);
	}

	// �����ĵ�������Ӧ�ó���Χ
	acDocManager->unlockDocument(acDocManager->curDocument());
}

// This is command 'SAVEDWGOPENDOC'
void ZffChap7SaveDwgOpenDoc()
{
	// 1.��ǰͼ���еļ���
	// ���������ĵ����ܷ���ͼ�����ݿ������
	acDocManager->lockDocument(acDocManager->curDocument());
	
	// �ҳ�ͼ����ֱ�ߵ�����
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
	int lineCount = 0, circleCount = 0;
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbEntity *pEnt = NULL;
		if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				lineCount++;
			}
			else if (pEnt->isKindOf(AcDbCircle::desc()))
			{
				circleCount++;
			}
			pEnt->close();
		}
	}

	// ����������DWG�ļ�
	AcDbDatabase *pDb1 = new AcDbDatabase(true, false);
	for (i = 0; i < lineCount; i++)
	{
		double y = i * 20;
		AcGePoint3d startPoint(0, y, 0);
		AcGePoint3d endPoint(100, y, 0);
		CLineUtil::Add(startPoint, endPoint, pDb1);
	}
	CString fileName1 = CAppDirectoryUtil::GetCurrentDirectory() + TEXT("\\db1.dwg");
	pDb1->saveAs(fileName1);
	delete pDb1;
	AcDbDatabase *pDb2 = new AcDbDatabase(true, false);
	for (i = 0; i < circleCount; i++)
	{
		double y = i * 20;
		AcGePoint3d center(0, y, 0);
		CCircleUtil::Add(center, 5, pDb2);
	}
	CString fileName2 = CAppDirectoryUtil::GetCurrentDirectory() + TEXT("\\db2.dwg");
	pDb2->saveAs(fileName2);
	delete pDb2;
	
	// �����ĵ�������Ӧ�ó���Χ
	acDocManager->unlockDocument(acDocManager->curDocument());
	
	// 2.���Ѿ����ڵ�DWG�ļ������л�Ϊ��ǰ�ĵ�
	Acad::ErrorStatus es = acDocManager->appContextOpenDocument(fileName1);
	es = acDocManager->appContextOpenDocument(fileName2);
}

