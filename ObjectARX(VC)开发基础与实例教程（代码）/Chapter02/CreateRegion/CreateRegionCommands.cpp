//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\RegionUtil.h"

// This is command 'ADDREGION'
void ZffChap2AddRegion()
{
	// ʹ��ѡ�񼯣���ʾ�û�ѡ����Ϊ����߽�Ķ���
	ads_name ss;
	int rt = acedSSGet(NULL, NULL, NULL, NULL, ss);		// ��ʾ�û�ѡ�����	
	
	// ����ѡ���еĶ��󹹽��߽����ߵ�ID����
	AcDbObjectIdArray objIds;
	if (rt == RTNORM)
	{		
		long length;
		acedSSLength(ss, &length);		// ���ѡ���еĶ������
		for (int i = 0; i < length; i++)
		{
			ads_name ent;
			acedSSName(ss, i, ent);
			AcDbObjectId objId;
			acdbGetObjectId(objId, ent);
			
			objIds.append(objId);
		}
	}
	acedSSFree(ss);			// ��ʱ�ͷ�ѡ��
	
	AcDbObjectIdArray regionIds;
	regionIds = CRegionUtil::Add(objIds);
	
	int number = regionIds.length();
	acutPrintf("\n�Ѿ�����%d������.", number);
}

