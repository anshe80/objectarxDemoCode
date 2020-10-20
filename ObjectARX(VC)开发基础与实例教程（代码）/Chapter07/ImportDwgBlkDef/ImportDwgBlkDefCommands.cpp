//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\BlockUtil.h"
#include <IO.H>

// This is command 'INSERTBLKREFFROMOTHERDWG'
void ZffChap7InsertBlkRefFromOtherDwg()
{
	CString fileName = TEXT("C:\\����ͼ��.dwg");
	if (_taccess(fileName, 0) != -1)
	{
		AcDbObjectId blkDefId = CBlockUtil::CopyBlockDefFromOtherDwg(fileName, TEXT("����ͼ��"));
		if (blkDefId.isValid())
		{
			CBlockUtil::InsertBlockRef(blkDefId, AcGePoint3d(100, 100, 0), 1, 0);
		}
	}
	else
	{
		acutPrintf(TEXT("\nָ����ͼ���ļ�������."));
	}
}

// This is command 'INSERTDWGBLOCKREF'
void ZffChap7InsertDwgBlockRef()
{
	CString fileName = TEXT("C:\\��Ϊͼ����ļ�.dwg");
	if (_taccess(fileName, 0) != -1)
	{
		AcDbObjectId blkDefId = CBlockUtil::InsertDwgBlockDef(fileName, TEXT("�ⲿDWGͼ��"), true);
		if (blkDefId.isValid())
		{
			CBlockUtil::InsertBlockRef(blkDefId, AcGePoint3d(200, 100, 0), 1, 0);
		}
	}
	else
	{
		acutPrintf(TEXT("\nָ����ͼ���ļ�������."));
	}
}

