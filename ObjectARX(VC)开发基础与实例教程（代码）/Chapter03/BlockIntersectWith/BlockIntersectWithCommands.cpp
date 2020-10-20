//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include "..\..\Common\Entity\CircleUtil.h"
#include "..\..\Common\Entity\EntityUtil.h"
#include <dbents.h>
#include "..\..\Common\Entity\BlockUtil.h"

// This is command 'BLOCKINTERSECTWITH'
void ZffChap3BlockIntersectWith()
{
	// ���ͼ�����ݿ�����Ҫ����Ŀ����
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
	AcDbObjectId blkRefId;
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbBlockReference *pBlkRef = NULL;
		if (acdbOpenObject(pBlkRef, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			blkRefId = pBlkRef->objectId();
			pBlkRef->close();
		}
	}

	// ����պ�ֱ�߼�����ʵ����
	AcDbBlockReference *pBlkRef = NULL;
	if (acdbOpenObject(pBlkRef, blkRefId, AcDb::kForRead) == Acad::eOk)
	{
		for (i = 0; i < allEntIds.length(); i++)
		{
			if (allEntIds[i] != blkRefId)
			{
				AcDbLine *pLine = NULL;
				if (acdbOpenObject(pLine, allEntIds[i], AcDb::kForRead) == Acad::eOk)
				{
					AcGePoint3dArray intPoints;
					CBlockUtil::IntersectWith(pBlkRef, pLine, intPoints, false, false, false);

					// �������ú�ɫ��Բ���Ƴ���
					for (int j = 0; j < intPoints.length(); j++)
					{
						double radius = 4;
						AcDbObjectId circleId = CCircleUtil::Add(intPoints[j], radius);
						CEntityUtil::SetColor(circleId, 1);
					}

					pLine->close();
				}
			}
		}

		pBlkRef->close();
	}
}

