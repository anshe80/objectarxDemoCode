//-----------------------------------------------------------------------------
//----- ChildDlg.cpp : implementation file
#include "StdAfx.h"
#include "resource.h"
#include "ChildDlg.h"
#include "dbents.h"
#include "..\..\Common\Entity\LineUtil.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CChildDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CChildDlg, CAcUiDialog)
	//{{AFX_MSG_MAP(CChildDlg)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)    // Needed for modeless dialog.
	ON_BN_CLICKED(IDC_LINE, OnLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CChildDlg::CChildDlg (CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CChildDlg::IDD, pParent, hInstance) {
	//{{AFX_DATA_INIT(CChildDlg)
	//}}AFX_DATA_INIT
}

void CChildDlg::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	//{{AFX_DATA_MAP(CChildDlg)
	//}}AFX_DATA_MAP
}

// Needed for modeless dialogs to keep focus.
// Return FALSE to not keep the focus, return TRUE to keep the focus
LONG CChildDlg::OnAcadKeepFocus(UINT, UINT)
{
  return TRUE;
}

//-----------------------------------------------------------------------------

void CChildDlg::OnLine() 
{
	// �����ڷ�ģ̬�Ի�����Ҫ�����ͽ����ĵ�
	acDocManager->lockDocument(acDocManager->curDocument());
	
	// ����һ��ֱ��
	CLineUtil::Add(AcGePoint3d(4.0, 2.0, 0.0), AcGePoint3d(10.0, 7.0, 0.0));
	
	acDocManager->unlockDocument(acDocManager->curDocument());
}
