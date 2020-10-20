// DoubleClickCircle.cpp: implementation of the CDoubleClickCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoubleClickCircle.h"
#include <dbents.h>
#include <actrans.h>
#include "GetColorDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoubleClickCircle::CDoubleClickCircle()
{

}

CDoubleClickCircle::~CDoubleClickCircle()
{

}

void CDoubleClickCircle::startEdit( AcDbEntity *pEnt, AcGePoint3d pt )
{
	// ��õ�ǰ�ĵ�
	AcApDocument *pDoc = acDocManager->curDocument();
	
	// ��AcDbEntityָ��ת��ΪAcDbLineָ��
	if (pEnt->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *pCircle = AcDbCircle::cast(pEnt);

		// �����ĵ�
		acDocManager->lockDocument(pDoc);
		
		// ��ʵ��Ĵ�״̬����Ϊ��д״̬
		pCircle->upgradeOpen();
		
		// ��ʾ�û�ָ���µ���ɫ
		int oldColorIndex = pCircle->colorIndex();
		CAcModuleResourceOverride myResource;	// ��ֹ��Դ��ͻ
		CGetColorDlg dlg;
		dlg.GetParams(oldColorIndex);
		if (dlg.DoModal() == IDOK)
		{
			int newColorIndex = 0;
			dlg.GetParams(newColorIndex);
			pCircle->setColorIndex(newColorIndex);
		}
		//pCircle->setColorIndex(1);
		pCircle->close();
		
		// �����ĵ�
		acDocManager->unlockDocument(pDoc);
		
		// ����PickFirstѡ���е�����
		acedSSSetFirst(NULL, NULL);
		
		// ����ͼ�ε���ʾ
		pCircle->draw();		// Ϊʲô���ڹر�֮����ʹ��������Ҳ��̫���
		actrTransactionManager->flushGraphics();
		acedUpdateDisplay();		
	}
	else
	{
		acutPrintf("\n��Ч��AcDbCircle����...");
		pEnt->close();
		return;
	}
}

void CDoubleClickCircle::finishEdit( void )
{
	// �˴���ʹ��
}