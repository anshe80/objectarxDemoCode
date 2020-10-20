//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbsymtb.h>
#include <geassign.h>
#include <dbents.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include "..\..\Common\Entity\BlockUtil.h"

// This is command 'INSERTBLK'
void ZffChap3InsertBlk()
{
	// ����û�����Ŀ鶨������	
	TCHAR blkName[40];
	if (acedGetString(Adesk::kFalse, TEXT("\n����ͼ�������:"), blkName) != RTNORM)
	{			
		return;
	}	
	
	// ��õ�ǰ���ݿ�Ŀ��
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForWrite);
	
	// �����û�ָ���Ŀ鶨���Ƿ����
	CString strBlkDef;
	strBlkDef.Format(TEXT("%s"), blkName);
	if (!pBlkTbl->has(strBlkDef))
	{
		acutPrintf(TEXT("\n��ǰͼ����δ����ָ�����ƵĿ鶨��!"));
		pBlkTbl->close();
		return;
	}
	
	// ����û�����Ŀ���յĲ����
	ads_point pt;
	if (acedGetPoint(NULL, TEXT("\n�������յĲ����:"), pt) != RTNORM)
	{	
		pBlkTbl->close();
		return;
	}
	AcGePoint3d ptInsert = asPnt3d(pt);
	
	// ����û�ָ���Ŀ���¼	
	AcDbObjectId blkDefId;
	pBlkTbl->getAt(strBlkDef, blkDefId);
	pBlkTbl->close();
	
	// ��������ն���
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(ptInsert, blkDefId);
	
	// ���������ӵ�ģ�Ϳռ�
	CDwgDatabaseUtil::PostToModelSpace(pBlkRef);	
}

// This is command 'INSERTBLK2'
void ZffCHAP3InsertBlk2()
{
	// ����û�����Ŀ鶨������	
	TCHAR blkName[40];
	if (acedGetString(Adesk::kFalse, TEXT("\n����ͼ�������:"), blkName) != RTNORM)
	{			
		return;
	}	
	
	// �����û�ָ���Ŀ鶨���Ƿ����
	AcDbObjectId blkDefId = CBlockUtil::GetBlkDefId(blkName);
	if (blkDefId.isNull())
	{
		acutPrintf(TEXT("\n��ǰͼ����δ����ָ�����ƵĿ鶨��!"));
		return;
	}
	
	// ����û�����Ŀ���յĲ����
	ads_point pt;
	if (acedGetPoint(NULL, TEXT("\n�������յĲ����:"), pt) != RTNORM)
	{		
		return;
	}
	AcGePoint3d ptInsert = asPnt3d(pt);
	
	// ��������ն���
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(ptInsert, blkDefId);
	
	// ���������ӵ�ģ�Ϳռ�
	CDwgDatabaseUtil::PostToModelSpace(pBlkRef);
}

