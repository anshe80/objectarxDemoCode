//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\HatchUtil.h"

// This is command 'ADDHATCH'
void ZffChap2AddHatch()
{
	// ��ʾ�û�ѡ�����߽�
	ads_name ss;
	int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);
	AcDbObjectIdArray objIds;
	
	// ��ʼ�����߽��ID����
	if (rt == RTNORM)
	{		
		long length;
		acedSSLength(ss, &length);
		for (int i = 0; i < length; i++)
		{
			ads_name ent;
			acedSSName(ss, i, ent);
			AcDbObjectId objId;
			acdbGetObjectId(objId, ent);
			
			objIds.append(objId);
		}
	}
	acedSSFree(ss);			// �ͷ�ѡ��
	
	CHatchUtil::Add(objIds, TEXT("SOLID"), 1.0);	
}

