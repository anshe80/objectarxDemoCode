//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbsymtb.h>
#include <dbents.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include "..\..\Common\SymbolTable\ViewUtil.h"

// ���AutoCADӦ�ó����ȫ·������
bool GetAcadPath(CString &acadPath)
{		
	DWORD dwRet = ::GetModuleFileName(acedGetAcadWinApp()->m_hInstance, 
		acadPath.GetBuffer(_MAX_PATH) , _MAX_PATH);
	acadPath.ReleaseBuffer();
	
	if (dwRet == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// This is command 'CREATEDWG'
void ZffChap7CreateDwg()
{
	// �����µ�ͼ�����ݿ⣬�����ڴ�ռ�
    AcDbDatabase *pDb = new AcDbDatabase(true, false);
	
    AcDbBlockTable *pBlkTbl = NULL;
    pDb->getSymbolTable(pBlkTbl, AcDb::kForRead);
	
    AcDbBlockTableRecord *pBlkTblRcd = NULL;
    pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
        AcDb::kForWrite);
    pBlkTbl->close();
	
    // ��������Բ
    AcDbCircle *pCir1 = new AcDbCircle(AcGePoint3d(1,1,1), AcGeVector3d(0,0,1), 1.0);
    pBlkTblRcd->appendAcDbEntity(pCir1);
    pCir1->close();    
	AcDbCircle *pCir2 = new AcDbCircle(AcGePoint3d(4,4,4), AcGeVector3d(0,0,1), 2.0);
	pBlkTblRcd->appendAcDbEntity(pCir2);
    pCir2->close();
    pBlkTblRcd->close();
	
	// ���acad.exe��λ��
    CString acadPath;	
    GetAcadPath(acadPath);

    // ȥ��·������"acad.exe"�ַ������õ�AutoCAD��װ·��
    acadPath = acadPath.Left(acadPath.GetLength() - 8);
    CString filePath = acadPath + TEXT("test.dwg");

    // ʹ��saveAs��Ա����ʱ������ָ������dwg��չ�����ļ����� 
    pDb->saveAs(filePath);

    delete pDb;		// pDb�������ݿ�ĳ�פ���󣬱����ֹ�����
}

// This is command 'READDWG'
void ZffChap7ReadDwg()
{
	// ʹ��false��Ϊ���캯���Ĳ���������һ���յ�ͼ�����ݿ�
    // ������֤ͼ�����ݿ�����������������    
    AcDbDatabase *pDb = new AcDbDatabase(false);
	
    // AcDbDatabase::readDwgFile()���������Զ����dwg��չ�� 
    CString acadPath;	
    GetAcadPath(acadPath);
    // ȥ��·������"acad.exe"�ַ���
    acadPath = acadPath.Left(acadPath.GetLength() - 8);
    CString filePath = acadPath + "test.dwg";
    pDb->readDwgFile(filePath, _SH_DENYWR);
	
	// ���ģ�Ϳռ������ʵ��
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds(NULL, pDb);
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbEntity *pEnt = NULL;
		if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			acutPrintf(TEXT("\n������: %s"), (pEnt->isA())->name());
			pEnt->close();
		}
	}

	// ɾ��ͼ�����ݿ�
    delete pDb;
}

// This is command 'CREATEDWG2'
void ZffChap7CreateDwg2()
{
	// �����µ�ͼ�����ݿ⣬�����ڴ�ռ�
    AcDbDatabase *pDb = new AcDbDatabase(true, false);
	
    // ��������Բ
    AcDbCircle *pCir1 = new AcDbCircle(AcGePoint3d(1,1,1), AcGeVector3d(0,0,1), 1.0);
	CDwgDatabaseUtil::PostToModelSpace(pCir1, pDb);   
	AcDbCircle *pCir2 = new AcDbCircle(AcGePoint3d(4,4,4), AcGeVector3d(0,0,1), 2.0);
	CDwgDatabaseUtil::PostToModelSpace(pCir2, pDb);

	// ����DWG�ļ�����ͼ
	CViewUtil::DwgZoomExtent(pDb);
	
	// ���acad.exe��λ��
    CString acadPath;	
    GetAcadPath(acadPath);
	
    // ȥ��·������"acad.exe"�ַ������õ�AutoCAD��װ·��
    acadPath = acadPath.Left(acadPath.GetLength() - 8);
    CString filePath = acadPath + TEXT("test2.dwg");
	
    // ʹ��saveAs��Ա����ʱ������ָ������dwg��չ�����ļ����� 
    pDb->saveAs(filePath);
	
    delete pDb;		// pDb�������ݿ�ĳ�פ���󣬱����ֹ�����
}

