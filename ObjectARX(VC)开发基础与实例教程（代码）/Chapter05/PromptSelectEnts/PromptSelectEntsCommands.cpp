//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <dbents.h>

// This is command 'SELECTENT'
void ZffChap5SelectEnt()
{
	// ��ʾ�û�ѡ��һ��Բ��������ɫ�޸�Ϊ��ɫ
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ��Բ:"), AcDbCircle::desc(), pEnt, pickPoint))
	{
		pEnt->setColorIndex(1);
		pEnt->close();
	}
}

// This is command 'SELECTENTS'
void ZffChap5SelectEnts()
{
	// ��ʾ�û�ѡ����Բ��ֱ�ߣ�������ɫ�޸�Ϊ��ɫ
	std::vector<AcRxClass*> descs;
	descs.push_back(AcDbLine::desc());
	descs.push_back(AcDbCircle::desc());
	AcDbObjectIdArray entIds;
	if (CSelectUtil::PromptSelectEnts(TEXT("\nѡ��Բ��ֱ��:"), descs, entIds))
	{
		for (int i = 0; i < entIds.length(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (acdbOpenObject(pEnt, entIds[i], AcDb::kForWrite) == Acad::eOk)
			{
				pEnt->setColorIndex(1);
				pEnt->close();
			}
		}
	}
}

