//-----------------------------------------------------------------------------
//----- ArxDialog.cpp : implementation file
#include "StdAfx.h"
#include "resource.h"
#include "ArxDialog.h"
#include <math.h>
#include "..\..\Common\Others\ConvertUtil.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CArxDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(CArxDialog, CAcUiDialog)
	//{{AFX_MSG_MAP(CArxDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)    // Needed for modeless dialog.
	ON_BN_CLICKED(IDC_BUTTON_ANGLE, OnButtonAngle)
	ON_BN_CLICKED(IDC_BUTTON_POINT, OnButtonPoint)
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_EDIT_ANGLE, OnKillfocusEditAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_XPT, OnKillfocusEditXpt)
	ON_EN_KILLFOCUS(IDC_EDIT_YPT, OnKillfocusEditYpt)
	ON_EN_KILLFOCUS(IDC_EDIT_ZPT, OnKillfocusEditZpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CArxDialog::CArxDialog (CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CArxDialog::IDD, pParent, hInstance) {
	//{{AFX_DATA_INIT(CArxDialog)
	//}}AFX_DATA_INIT
}

void CArxDialog::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	//{{AFX_DATA_MAP(CArxDialog)
	DDX_Control(pDX, IDC_BUTTON_ANGLE, m_btnAngle);
	DDX_Control(pDX, IDC_BUTTON_POINT, m_btnPick);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_editAngle);
	DDX_Control(pDX, IDC_EDIT_XPT, m_editXPt);
	DDX_Control(pDX, IDC_EDIT_YPT, m_editYPt);
	DDX_Control(pDX, IDC_EDIT_ZPT, m_editZPt);
	//}}AFX_DATA_MAP
}

// Needed for modeless dialogs to keep focus.
// Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CArxDialog::OnAcadKeepFocus(WPARAM, LPARAM)
{
  return TRUE;
}

//-----------------------------------------------------------------------------

BOOL CArxDialog::OnInitDialog() 
{
	CAcUiDialog::OnInitDialog();
	
	// ���������ķ�Χ
	m_editXPt.SetRange(-100.0, 100.0);
	m_editYPt.SetRange(-100.0, 100.0);
	m_editZPt.SetRange(-100.0, 100.0);

	// ���ýǶȵ����뷶Χ
	m_editAngle.SetRange(0.0, 90.0);

	// ����Ĭ�ϵ�λͼ
	m_btnPick.AutoLoad();
	m_btnAngle.AutoLoad();
	
	// �����ı����Ĭ��ֵ
	m_strAngle = "0.0";
	m_strXPt = "0.0";
	m_strYPt = "0.0";
	m_strZPt = "0.0";

	// ��ʾ��ʼ�������ͽǶ�ֵ
	DisplayPoint();
	DisplayAngle();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArxDialog::OnButtonAngle() 
{
	// ���ضԻ���ѿ���Ȩ����AutoCAD
    BeginEditorCommand();
	
    // ����ǰѡ��ĵ��λ����Ϊ����
    ads_point pt; 
    acdbDisToF(m_strXPt, -1, &pt[X]);
    acdbDisToF(m_strYPt, -1, &pt[Y]);
    acdbDisToF(m_strZPt, -1, &pt[Z]);    
	
    // ��ʾ�û�����һ��
	double angle;
	const double PI = 4 * atan(1.0);
    if (acedGetAngle(pt, TEXT("\n����Ƕ�:"), &angle) == RTNORM) 
	{
        // �����ȷ��ýǶȣ����ضԻ���
        CompleteEditorCommand();
        // ���Ƕ�ֵת��Ϊ����ֵ
        m_strAngle.Format(TEXT("%.2f"), angle * (180.0/PI));

		// ��ʾ�Ƕ�ֵ
        DisplayAngle();
    } 
	else 
	{
        // ����ȡ����������Ի���
        CancelEditorCommand();
    }
}

void CArxDialog::OnButtonPoint() 
{
	// ���ضԻ���ѿ���Ȩ����AutoCAD
	BeginEditorCommand();
	
	// ��ʾ�û�����һ����
	ads_point pt;
	if (acedGetPoint(NULL, TEXT("\n����һ����:"), pt) == RTNORM)
	{
		// �������Ч������ִ��
		CompleteEditorCommand();
		m_strXPt.Format(TEXT("%.2f"), pt[X]);
		m_strYPt.Format(TEXT("%.2f"), pt[Y]);
		m_strZPt.Format(TEXT("%.2f"), pt[Z]);

		// ��ʾ�������
		DisplayPoint();
	}
	else
	{
		// ����ȡ����������Ի���
		CancelEditorCommand();
	}
}

void CArxDialog::DisplayPoint()
{
	// �ڶԻ�������ʾ�������
	m_editXPt.SetWindowText(m_strXPt);
	m_editXPt.Convert();	// ���¿ؼ���������ĳ�Ա����
	m_editYPt.SetWindowText(m_strYPt);
	m_editYPt.Convert();
	m_editZPt.SetWindowText(m_strZPt);
	m_editZPt.Convert();
}


void CArxDialog::DisplayAngle()
{
	// �ڶԻ�������ʾ�Ƕ�ֵ
	m_editAngle.SetWindowText(m_strAngle);
	m_editAngle.Convert();
}

void CArxDialog::OnClose() 
{
	// ��AutoCAD��������ѡ���Ľ��
	double x = CConvertUtil::ToDouble(m_strXPt);
	double y = CConvertUtil::ToDouble(m_strYPt);
	double z = CConvertUtil::ToDouble(m_strZPt);
	acutPrintf(TEXT("\nѡ��ĵ�����Ϊ(%.2f, %.2f, %.2f)."), x, y, z);
	
	CAcUiDialog::OnClose();
}

void CArxDialog::OnKillfocusEditAngle() 
{
	// ��ò������û������ֵ
	m_editAngle.Convert();		
	m_editAngle.GetWindowText(m_strAngle);
}

void CArxDialog::OnKillfocusEditXpt() 
{
	// ��ò������û������ֵ
	m_editXPt.Convert();		
	m_editXPt.GetWindowText(m_strXPt);
}

void CArxDialog::OnKillfocusEditYpt() 
{
	// ��ò������û������ֵ
	m_editYPt.Convert();		
	m_editYPt.GetWindowText(m_strYPt);
}

void CArxDialog::OnKillfocusEditZpt() 
{
	// ��ò������û������ֵ
	m_editZPt.Convert();		
	m_editZPt.GetWindowText(m_strZPt);	
}

