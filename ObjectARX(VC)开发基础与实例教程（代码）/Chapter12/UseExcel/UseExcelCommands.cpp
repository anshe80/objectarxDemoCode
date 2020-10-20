//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include <dbents.h>
#include "excel.h"
#include "..\..\Common\Others\ConvertUtil.h"
#include <comdef.h>
#include "..\..\Common\Entity\LineUtil.h"
#include "..\..\Common\Entity\circleutil.h"

// ����Excel����
// �����ڵ��ó����InitApplication�����г�ʼ��COM��̬���ӿ⣺::CoInitialize(NULL);
// ͬʱ��UnloadApplication�����йر�COM���֧�֣�::CoUninitialize();
static bool RunExcelApp( _Application &excelApp, bool bVisible, bool bUserControl )
{
	// ����Excel
	// �ȼ���Ƿ��Ѿ�����
	CLSID clsId;
	::CLSIDFromProgID(L"Excel.Application", &clsId); // from registry
	IUnknown* pUnk = NULL;
	if (::GetActiveObject(clsId, NULL, &pUnk) == S_OK) 		
	{
		LPDISPATCH pDispatch;
		VERIFY(pUnk->QueryInterface(IID_IDispatch,(void**) &pDispatch) == S_OK);
		excelApp.AttachDispatch(pDispatch);
		pUnk->Release();
	} 
	else		
	{
		if(!excelApp.CreateDispatch(TEXT("Excel.Application")))
		{
			AfxMessageBox(TEXT("��ǰ�����û�а�װExcel!"));
			return false;
		}
	}
	
	excelApp.SetVisible(bVisible);
	excelApp.SetUserControl(bUserControl);	// �Ƿ����������û�����Excel
	
	return true;
}

// �½�һ��workbook
static void NewWorkBook( _Application excelApp, _Workbook &excelBook )
{
	// ���Workbooks
	LPDISPATCH pDispatch = excelApp.GetWorkbooks();
	Workbooks excelBooks;
	excelBooks.AttachDispatch(pDispatch);
	
	// ��õ�ǰ���Workbook
	COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	pDispatch = excelBooks.Add(vOptional);
	excelBook.AttachDispatch(pDispatch);
}

// ���ָ��λ�õ�workSheet
// i����1��ʼ
static bool GetWorkSheet( _Workbook excelBook, int i, _Worksheet &excelSheet)
{
	LPDISPATCH pDispatch = excelBook.GetWorksheets();
	Worksheets excelSheets;
	excelSheets.AttachDispatch(pDispatch);

	if (i > 0 && i <= excelSheets.GetCount())
	{
		LPDISPATCH pDispatch = excelSheets.GetItem(COleVariant(long(i)));
		excelSheet.AttachDispatch(pDispatch);

		return true;
	}
	else
	{
		return false;
	}
}

// ��ʾ�û�ѡ��һ���ļ�
// fileDesc:�ļ��������������硰ͼƬ�ļ�(*.jpg)���еġ�ͼƬ�ļ���
// fileExtName:��չ��
static bool SelectFileToOpen(const TCHAR* filedesc, const TCHAR* fileExtName, CString &selectedFile)
{
	CString extensionNameHaveDot = TEXT(".");
	extensionNameHaveDot += fileExtName;
	
	// �����򿪶Ի���
	CString filter;
	filter.Format(TEXT("%s(*.%s)|*.%s"), filedesc, fileExtName, fileExtName);
	filter += TEXT("|�����ļ�(*.*)|*.*||");
	
	// 	TRUE/FALSE�򿪻����Ի���   
	BOOL bOpenFile = TRUE;
	CFileDialog cFileDlg(bOpenFile, extensionNameHaveDot, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	cFileDlg.m_ofn.lStructSize = 88;  // ��Office���Ĵ��ļ��Ի���
	
	// ȡ��ȫ·���ļ���
	if (cFileDlg.DoModal() == IDOK)
	{
		selectedFile = cFileDlg.GetPathName();		
		return true;
	}
	else
	{   
		return false;
	}
}

// ��excel�ļ������workbook
static bool OpenWorkBook(_Application excelApp, const TCHAR* fileName, _Workbook &excelBook, bool bIsReadOnly = true)
{
	try
	{
		COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  //��ʾһ��Ϊoptional��ֵ
		COleVariant vIsReadOnly((short)bIsReadOnly);	// ֻ��		
		
		// ���Workbooks
		LPDISPATCH pDispatch = excelApp.GetWorkbooks();
		Workbooks excelBooks;
		excelBooks.AttachDispatch(pDispatch);
		
		//��Excel�ļ�
		pDispatch = excelBooks.Open(fileName, vOptional, vIsReadOnly, vOptional, vOptional, 
			vOptional, vOptional, vOptional, vOptional, vOptional, vOptional, 
			vOptional, vOptional, vOptional, vOptional); 
		
		excelBook.AttachDispatch(pDispatch);
	}
	catch (_com_error e)
	{
		assert(false);
		AfxMessageBox(e.ErrorMessage());
		return false;
	}
	
	return true;
}

// ȡ������
static long GetRowCount(_Worksheet excelSheet)
{
	Range excelRange;
	excelRange.AttachDispatch(excelSheet.GetUsedRange());
	
	Range rowRange;
	rowRange.AttachDispatch(excelRange.GetRows());
	return rowRange.GetCount();
}

// This is command 'EXPORTTOEXCEL'
void ZffChap12ExportToExcel()
{
	// 1.��ȡֱ�ߡ�Բ�μ���
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
	AcDbObjectIdArray lineIds, circleIds;
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbEntity *pEnt = NULL;
		if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				lineIds.append(pEnt->objectId());
			}
			else if (pEnt->isKindOf(AcDbCircle::desc()))
			{
				circleIds.append(pEnt->objectId());
			}
			
			pEnt->close();
		}
	}

	// 2.����Excel
	_Application excelApp;
	RunExcelApp(excelApp, true, true);

	// 3.�½�һ��WorkBook
	_Workbook excelBook;
	NewWorkBook(excelApp, excelBook);

	// 4.����ֱ������
	// ���Sheet1
	_Worksheet excelSheet1;
	GetWorkSheet(excelBook, 1, excelSheet1);
	Range cells1;
	cells1.AttachDispatch(excelSheet1.GetCells());
	// ��һ�е�����
	long row = 1, col = 1;
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("���")));
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("���X")));
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("���Y")));
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("���Z")));
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("�յ�X")));
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("�յ�Y")));
	cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("�յ�Z")));
	// �����е�����
	for (i = 0; i < lineIds.length(); i++)
	{
		AcDbLine *pLine = NULL;
		if (acdbOpenObject(pLine, lineIds[i], AcDb::kForRead) == Acad::eOk)
		{
			AcGePoint3d startPoint = pLine->startPoint();
			AcGePoint3d endPoint = pLine->endPoint();

			row = i + 2;
			col = 1;
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(row - 1)));
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(startPoint.x)));
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(startPoint.y)));
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(startPoint.z)));
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(endPoint.x)));
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(endPoint.y)));
			cells1.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(endPoint.z)));

			pLine->close();
		}
	}

	// 5.����Բ����
	// ���Sheet2
	_Worksheet excelSheet2;
	GetWorkSheet(excelBook, 2, excelSheet2);
	Range cells2;
	cells2.AttachDispatch(excelSheet2.GetCells());
	// ��һ�е�����
	row = 1, col = 1;
	cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("���")));
	cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("Բ��X")));
	cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("Բ��Y")));
	cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("Բ��Z")));
	cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(TEXT("�뾶")));
	// �����е�����
	for (i = 0; i < circleIds.length(); i++)
	{
		AcDbCircle *pCircle = NULL;
		if (acdbOpenObject(pCircle, circleIds[i], AcDb::kForRead) == Acad::eOk)
		{
			AcGePoint3d center = pCircle->center();
			double radius = pCircle->radius();
			
			row = i + 2;
			col = 1;
			cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(row - 1)));
			cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(center.x)));
			cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(center.y)));
			cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(center.z)));
			cells2.SetItem(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(radius)));
			
			pCircle->close();
		}
	}
}

// ��õ�Ԫ���ֵ
static bool GetCellValue( Range &excelRange, long rowIndex, long columnIndex, CString &strValue )
{
	assert(rowIndex > 0);
	assert(columnIndex > 0);
	
	COleVariant vValue;
	vValue = excelRange.GetItem(COleVariant(rowIndex), COleVariant(columnIndex));
	strValue = (LPCTSTR)_bstr_t(vValue);
	
	return true;
}

// ��õ�Ԫ���ֵ
static bool GetCellValue( Range &excelRange, long rowIndex, long columnIndex, double &val )
{
	CString strValue;
	bool bRet = GetCellValue(excelRange, rowIndex, columnIndex, strValue);
	val = CConvertUtil::ToDouble(strValue);

	return bRet;
}

// This is command 'IMPORTFROMEXCEL'
void ZffCHAP12ImportFromExcel()
{
	// 1.��ʾ�û�ѡ��Excel�ļ�
	CString excelFileName;
	if (!SelectFileToOpen(TEXT("Excel�ļ�"), TEXT("xls"), excelFileName))
	{
		return;
	}

	// 2.����Excel
	_Application excelApp;
	RunExcelApp(excelApp, true, true);
	
	// 3.��ָ�����ļ�
	_Workbook excelBook;
	OpenWorkBook(excelApp, excelFileName, excelBook);
	
	// 4.����ֱ������
	// ���Sheet1
	_Worksheet excelSheet1;
	GetWorkSheet(excelBook, 1, excelSheet1);
	long rowCount = GetRowCount(excelSheet1);
	Range cells1;
	cells1.AttachDispatch(excelSheet1.GetCells());
	// �����е����ݣ��ӵڶ��п�ʼ��
	for (long row = 2; row <= rowCount; row++)
	{
		AcGePoint3d startPoint, endPoint;
		long col = 2;
		GetCellValue(cells1, row, col++, startPoint.x);
		GetCellValue(cells1, row, col++, startPoint.y);
		GetCellValue(cells1, row, col++, startPoint.z);
		GetCellValue(cells1, row, col++, endPoint.x);
		GetCellValue(cells1, row, col++, endPoint.y);
		GetCellValue(cells1, row, col++, endPoint.z);
		CLineUtil::Add(startPoint, endPoint);
	}
	
	// 5.����Բ����
	// ���Sheet2
	_Worksheet excelSheet2;
	GetWorkSheet(excelBook, 2, excelSheet2);
	rowCount = GetRowCount(excelSheet2);
	Range cells2;
	cells2.AttachDispatch(excelSheet2.GetCells());
	// �����е����ݣ��ӵڶ��п�ʼ��
	for (row = 2; row <= rowCount; row++)
	{
		AcGePoint3d center;
		double radius = 0;
		long col = 2;
		GetCellValue(cells2, row, col++, center.x);
		GetCellValue(cells2, row, col++, center.y);
		GetCellValue(cells2, row, col++, center.z);
		GetCellValue(cells2, row, col++, radius);
		CCircleUtil::Add(center, radius);
	}
}

