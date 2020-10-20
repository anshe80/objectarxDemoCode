//-----------------------------------------------------------------------------
#if !defined(ARX__GETCOLORDLG_H__20140203_203523)
#define ARX__GETCOLORDLG_H__20140203_203523

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ��cpp�ļ����ƶ����������������ļ��������ͷ�ļ�ʱ�Ҳ���IDD_GET_COLOR
#include "resource.h"

//-----------------------------------------------------------------------------
class CGetColorDlg : public CAcUiDialog {
	DECLARE_DYNAMIC (CGetColorDlg)

public:
	CGetColorDlg (CWnd* pParent =NULL, HINSTANCE hInstance =NULL) ;

	//{{AFX_DATA(CGetColorDlg)
	enum { IDD = IDD_GET_COLOR };
	int		m_colorIndex;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CGetColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGetColorDlg)
	afx_msg LONG OnAcadKeepFocus(UINT, UINT);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	// ����ԭ������ɫ
	void SetParams(int colorIndex);
	// ����û��������ɫ
	void GetParams(int &colorIndex);
} ;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //----- !defined(ARX__GETCOLORDLG_H__20140203_203523)
