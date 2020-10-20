//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include "..\..\Common\Interaction\GetInputUtil.h"
#include "ArcBlockJig.h"
#include <dbents.h>

// This is command 'ARCBLOCKJIG'
void ZffChap5ArcBlockJig()
{
	// ѡ��һ������գ�������Բ������
	AcDbEntity *pEnt = NULL;
	AcDbObjectId blkDefId;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ��һ�������������Բ������:"), AcDbBlockReference::desc(), pEnt, pickPoint))
	{
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
		blkDefId = pBlkRef->blockTableRecord();
		pEnt->close();
	}
	if (blkDefId.isNull())
	{
		return;
	}

	// ��ʾ�û�ʰȡ��һ��
	AcGePoint3d startPoint;
	if (!CGetInputUtil::GetPoint(TEXT("\nʰȡ��һ��:"), startPoint))
	{
		return;
	}

	// ��ʾ�û�ʰȡ�ڶ���
	AcGePoint3d secondPoint;
	if (!CGetInputUtil::GetPoint(startPoint, TEXT("\nʰȡ�ڶ���:"), secondPoint))
	{
		return;
	}

	// ��ʼ�϶�
	CArcBlockJig jig;
	int blockCount = 4;
	jig.doIt(startPoint, secondPoint, secondPoint, blkDefId, blockCount);
}

