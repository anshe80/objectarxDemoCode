//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "ModelessDlg.h"

extern CModelessDlg *pDialog;

// This is command 'MODELESSDLG'
void ZffCHAP11ModelessDlg()
{
	CAcModuleResourceOverride resOverride;
	
	// �Է�ģ̬��ʽ�����Ի���
	if (pDialog == NULL)
	{
		pDialog = new CModelessDlg(acedGetAcadFrame());
		pDialog->Create(IDD_DIALOG_MODELESS);
		pDialog->ShowWindow(SW_SHOW);
	}
	else
	{
		pDialog->ShowWindow(SW_SHOW);
	}	
}

