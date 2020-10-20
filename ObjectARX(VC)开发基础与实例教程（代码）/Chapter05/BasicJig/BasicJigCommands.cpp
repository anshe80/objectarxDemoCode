//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "DrawSquareJig.h"
#include "..\..\Common\Interaction\GetInputUtil.h"
#include "..\..\Common\Entity\EntityUtil.h"

// This is command 'DRAWSEQUAREJIG'
void ZfgkBJDrawSequareJig()
{
	// ��ʾ�û��������ĵ�
	AcGePoint3d centerPoint;
	if (CGetInputUtil::GetPoint(TEXT("\nָ�������ε����ĵ�:"), centerPoint))
	{
		// �����϶�״̬
		CDrawSquareJig jig;
		AcDbObjectId polyId;
		if (jig.doIt(centerPoint, polyId))
		{
			// �ɹ�����֮�󣬿��Խ����������޸�
			CEntityUtil::SetColor(polyId, 1);
		}
		else
		{
			// �û�ȡ����ɾ���Ѿ�������ʵ��
			CEntityUtil::Erase(polyId);
		}
	}	
}

