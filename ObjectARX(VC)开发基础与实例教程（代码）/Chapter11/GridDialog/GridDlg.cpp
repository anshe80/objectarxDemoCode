//-----------------------------------------------------------------------------
//----- GridDlg.cpp : implementation file
#include "StdAfx.h"
#include "GridDlg.h"
#include "..\..\Common\MfcGridCtrl\GridCtrlUtil.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CGridDlg, CAcUiDialog)

BEGIN_MESSAGE_MAP(CGridDlg, CAcUiDialog)
	//{{AFX_MSG_MAP(CGridDlg)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)    // Needed for modeless dialog.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CGridDlg::CGridDlg (CWnd* pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CGridDlg::IDD, pParent, hInstance) {
	//{{AFX_DATA_INIT(CGridDlg)
	//}}AFX_DATA_INIT
}

void CGridDlg::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	//{{AFX_DATA_MAP(CGridDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_grid);
}

// Needed for modeless dialogs to keep focus.
// Return FALSE to not keep the focus, return TRUE to keep the focus
LONG CGridDlg::OnAcadKeepFocus(UINT, UINT)
{
  return TRUE;
}

//-----------------------------------------------------------------------------

void CGridDlg::UpdateOutColGrid()
{
	m_grid.SetRowCount(1);
	
	int row = 1, col = 0;
	std::vector<CString> colNames = GetAllColumnName();
	m_grid.SetRowCount(colNames.size() + 1);
	for (int i = 0; i < colNames.size(); i++)
	{
		m_grid.SetItemText(row + i, col, colNames[i]);
	}
	
	// ���������������
	std::vector<CString> precisionStrs;
	CString val = TEXT("0");
	int precisionCount = 9;
	for (i = 0; i < precisionCount; i++)
	{
		precisionStrs.push_back(val);
		if (i == 0)
		{
			val += TEXT(".");
		}
		val += TEXT("0");
	}
	
	// ���ֶ��뷽ʽ��ѡ��ֵ
	std::vector<CString> textAlignments;
	textAlignments.push_back(TEXT("��"));
	textAlignments.push_back(TEXT("��"));
	textAlignments.push_back(TEXT("��"));
	
	int defaultAligmentIndex = 2;
	// ���õ�Ԫ������ݺ�״̬
	int colWidth = 80;
	for (i = 1; i < m_grid.GetRowCount(); i++)
	{
		// ��һ��Ϊ���ɱ༭
		m_grid.SetItemState(i, 0, GVIS_READONLY);
		
		// ���õڶ���Ϊ��ѡ��
		int col = 1;
		bool cellCheck = true;
		CGridCtrlUtil::SetCellCheckType(cellCheck, m_grid, i, col);
		m_grid.SetItemData(i, col, 1);
		
		// ���õ�����Ϊ������
		col = 2;		
		if (i != m_grid.GetRowCount() - 2)
		{
			m_grid.SetItemState(i, col, GVIS_READONLY);
		}
		else
		{
			CGridCtrlUtil::SetCellComboType(m_grid, i, col);
			CGridCtrlUtil::SetCellComboDwStyle(m_grid, i, col, CBS_DROPDOWNLIST);
			CGridCtrlUtil::SetCellComboOptions(m_grid, i, col, precisionStrs);
			m_grid.SetColumnWidth(col, colWidth);
			m_grid.SetItemText(i, col, TEXT("0.00"));
		}
		
		// ���ֶ��뷽ʽ��
		col = 3;
		CGridCtrlUtil::SetCellComboType(m_grid, i, col);
		CGridCtrlUtil::SetCellComboDwStyle(m_grid, i, col, CBS_DROPDOWNLIST);
		CGridCtrlUtil::SetCellComboOptions(m_grid, i, col, textAlignments);
		// ����Ĭ�ϵĶ��뷽ʽ
		if (i == 1 || (i == m_grid.GetRowCount() - 1))	// ����С���ע��Ϊ�����У�Ĭ�϶��뷽ʽΪ�����
		{
			defaultAligmentIndex = 0;		// �����
		}
		else
		{
			defaultAligmentIndex = 1;		// ����
		}
		m_grid.SetItemText(i, col, textAlignments[defaultAligmentIndex]);
	}
	
	m_grid.AutoSizeRows();
}

BOOL CGridDlg::OnInitDialog() 
{
	CAcUiDialog::OnInitDialog();
	
	// �����Grid��ʼ��
	m_grid.SetRowCount(GetAllColumnName().size());
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(4);	
	m_grid.SetItemText(0, 0, TEXT("����"));
	m_grid.SetItemText(0, 1, TEXT("���"));
	m_grid.SetItemText(0, 2, TEXT("����"));
	m_grid.SetItemText(0, 3, TEXT("���ֶ���"));
	m_grid.SetColumnWidth(0, 160);
	m_grid.SetColumnWidth(1, 50);
	m_grid.SetColumnWidth(2, 75);
	m_grid.SetColumnWidth(3, 75);
	
	UpdateOutColGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGridDlg::OnOK() 
{
	if (!UpdateData())
	{
		return;
	}

	// ��ȡ��������е�����
	for (int i = 1; i < m_grid.GetRowCount(); i++)
	{
		if (CGridCtrlUtil::GetCellChecked(m_grid, i, 1) && m_grid.GetRowHeight(i) > 0)		// �Ƿ����
		{
			int index = i - 1;
			CString colName = m_grid.GetItemText(i, 0);	// ����
			CString precisionStr = m_grid.GetItemText(i, 2);		// ��ʾ����
			CString strTextAlignment = m_grid.GetItemText(i, 3);		// ����ˮƽ���뷽ʽ
			int textAlignment = 0;
			if (strTextAlignment.CompareNoCase(TEXT("��")) == 0)
			{
				textAlignment = 0;
			}
			else if (strTextAlignment.CompareNoCase(TEXT("��")) == 0)
			{
				textAlignment = 1;
			}
			else	//strTextAlignment.CompareNoCase(TEXT("��")) == 0
			{
				textAlignment = 2;
			}
		}
	}
	
	CAcUiDialog::OnOK();
}

std::vector<CString> CGridDlg::GetAllColumnName()
{
	std::vector<CString> colNames;
	colNames.push_back(TEXT("���"));
	colNames.push_back(TEXT("ͼ��"));
	colNames.push_back(TEXT("�豸����������"));
	colNames.push_back(TEXT("����ͺ�"));
	colNames.push_back(TEXT("��λ"));
	colNames.push_back(TEXT("����"));
	colNames.push_back(TEXT("��ע"));
	return colNames;
}
