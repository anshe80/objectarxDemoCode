//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <geassign.h>

// This is command 'ADDXDATA'
void ZffChap6AddXData()
{
	// ��ʾ�û�ѡ����Ҫ�����չ���ݵ�ͼ�ζ���
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����Ҫ�����չ���ݵ�ʵ��:"), AcDbEntity::desc(), pEnt, pickPoint))
	{
		CString appName = TEXT("XDataApp");
		
		// ע��Ӧ�ó�������
		acdbRegApp(appName);

		// �����������������
		struct resbuf* rb = acutBuildList(AcDb::kDxfRegAppName, appName,		// Ӧ�ó�������
			AcDb::kDxfXdAsciiString, TEXT("�ַ�����������"),	// �ַ���
			AcDb::kDxfXdInteger32, 2,							// ����
			AcDb::kDxfXdReal, 3.14,								// ʵ��
			AcDb::kDxfXdWorldXCoord, asDblArray(pickPoint),						// ������ֵ
			RTNONE);
		
		// Ϊѡ���ʵ�������չ����
		pEnt->setXData(rb);
		acutRelRb(rb);

		acutPrintf(TEXT("\n�ɹ�Ϊʵ���������չ����."));

		pEnt->close();
	}
}

// This is command 'VIEWXDATA'
void ZffChap6ViewXData()
{
	// ��ʾ�û�ѡ����Ҫ�鿴��չ���ݵ�ͼ�ζ���
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����Ҫ�����չ���ݵ�ʵ��:"), AcDbEntity::desc(), pEnt, pickPoint))
	{
		CString appName = TEXT("XDataApp");
		
		// ��ȡʵ�����չ����
		struct resbuf *pRb = pEnt->xData(appName);		
		if (pRb != NULL)
		{
			// ����������ʾ���е���չ����
			struct resbuf *pTemp = pRb;		// ʹ����ʱ�Ľ��������ָ����б�����pRb���޸ı����ͷ�
			
			// ����Ҫ����Ӧ�ó����������һ��
			pTemp = pTemp->rbnext;
			acutPrintf("\n�ַ������͵���չ������: %s", pTemp->resval.rstring);
			
			pTemp = pTemp->rbnext;
			acutPrintf("\n�������͵���չ������: %d", pTemp->resval.rint);
			
			pTemp = pTemp->rbnext;
			acutPrintf("\nʵ�����͵���չ������: %.2f", pTemp->resval.rreal);
			
			pTemp = pTemp->rbnext;
			acutPrintf("\n���������͵���չ������: (%.2f, %.2f, %.2f)", 
				pTemp->resval.rpoint[X], pTemp->resval.rpoint[Y], 
				pTemp->resval.rpoint[Z]);
			
			acutRelRb(pRb);		
		}
		else
		{
			acutPrintf("\n��ѡ���ʵ�岻�����κε���չ����!");
		}
		
		pEnt->close();
	}
}

// This is command 'DELETEXDATA'
void ZffCHAP6DeleteXData()
{
	// ��ʾ�û�ѡ����Ҫɾ����չ���ݵ�ͼ�ζ���
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����Ҫɾ����չ���ݵ�ʵ��:"), AcDbEntity::desc(), pEnt, pickPoint))
	{
		CString appName = TEXT("XDataApp");
		
		// ע��Ӧ�ó�������
		acdbRegApp(appName);
		
		// �����������������
		struct resbuf* rb = acutBuildList(AcDb::kDxfRegAppName, appName,		// Ӧ�ó�������			
			RTNONE);
		
		// ��������Ϊ�յĽ���������������ɾ����չ����
		pEnt->setXData(rb);
		acutRelRb(rb);

		acutPrintf(TEXT("\n�ɹ�Ϊʵ��ɾ������չ����."));
		
		pEnt->close();
	}
}

