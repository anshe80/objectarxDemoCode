//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "MfcDialog.h"
#include "ArxDialog.h"

// This is command 'MFCMODAL'
void ZffCHAP11MfcModal()
{
	// ��ʾMFC��ģ̬�Ի���
	CMfcDialog theDialog;
	if (theDialog.DoModal() == IDOK)
	{
		AfxMessageBox(_T("�رնԻ���"));
	}	
}

// This is command 'ARXMODAL'
void ZffCHAP11ArxModal()
{
	// ��ֹ��Դ��ͻ
	CAcModuleResourceOverride resOverride;
	
	// ��ʾObjectARX��ģ̬�Ի���
	CArxDialog theDialog;
	theDialog.DoModal();		
}

