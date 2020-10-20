//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbsymtb.h>
#include <geassign.h>
#include <dbents.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include <acutmem.h>
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

	// �������գ�����ֵʹ��Ĭ��ֵ��
	AcDbObjectId blkRefId = CBlockUtil::InsertBlockRefWithAttribute(blkDefId, ptInsert, 1, 0);

	// ���ò���ֵ
	AcDbBlockReference *pBlkRef = NULL;
	if (acdbOpenObject(pBlkRef, blkRefId, AcDb::kForWrite) == Acad::eOk)
	{
		CBlockUtil::SetBlockRefAttribute(pBlkRef, TEXT("����"), TEXT("���ΰ�"));
		CBlockUtil::SetBlockRefAttribute(pBlkRef, TEXT("����"), TEXT("2.3Kg"));
		pBlkRef->close();
	}
}

