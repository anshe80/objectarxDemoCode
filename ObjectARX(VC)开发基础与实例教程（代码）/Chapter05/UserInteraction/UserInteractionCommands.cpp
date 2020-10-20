//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\GetInputUtil.h"
#include "..\..\Common\Entity\PolylineUtil.h"
#include "..\..\Common\Others\ConvertUtil.h"
#include <dbpl.h>

// ��ʾ�û������߿�
ads_real GetWidth()
{
	ads_real width = 0;
	if (acedGetReal(TEXT("\n�����߿�:"), &width) == RTNORM)
	{
		return width;
	}
	else
	{
		return 0;
	}
}

// ��ʾ�û�������ɫ����ֵ
int GetColorIndex()
{
	int colorIndex = 0;
	if (acedGetInt(TEXT("\n������ɫ����ֵ(0~256):"), &colorIndex) != RTNORM)
	{
		return 0;
	}
	
	// ������ɫ����ֵ��Ч�����
	while (colorIndex < 0 || colorIndex > 256)
	{
		acedPrompt(TEXT("\n��������Ч����ɫ����."));
		if (acedGetInt(TEXT("\n������ɫ����ֵ(0~256):"), &colorIndex) != RTNORM)
		{
			return 0;
		}
	}
	
	return colorIndex;
}

// This is command 'ADDPOLYBASIC'
void ZffChap5AddPolyBasic()
{
	int index = 2;				// ��ǰ�����Ĵ���
	AcGePoint3d ptStart;			// ���
	if (!CGetInputUtil::GetPoint(TEXT("\n�����һ��:"), ptStart))
	{
		return;
	}
	
	AcGePoint3d ptPrevious, ptCurrent;	// ǰһ���ο��㣬��ǰʰȡ�ĵ�
	ptPrevious = ptStart;
	AcDbObjectId polyId;				// ����ߵ�ID
	while (CGetInputUtil::GetPoint(ptPrevious, TEXT("\n������һ��:"), ptCurrent))
	{
		if (index == 2)
		{
			// ���������
			polyId = CPolylineUtil::Add(CConvertUtil::ToPoint2d(ptPrevious), CConvertUtil::ToPoint2d(ptCurrent));
		}
		else if (index > 2)
		{
			// �޸Ķ���ߣ�������һ������
			AcDbPolyline *pPoly = NULL;
			if (acdbOpenObject(pPoly, polyId, AcDb::kForWrite) == Acad::eOk)
			{
				pPoly->addVertexAt(index - 1, CConvertUtil::ToPoint2d(ptCurrent));				
				pPoly->close();
			}
		}
		
		ptPrevious = ptCurrent;
		index++;
	}
}

// This is command 'GETPOINTKEYWORD'
void ZffChap5GetPointKeyword()
{
	int rc;		// ����ֵ
	TCHAR kword[20];		// �ؼ���
	AcGePoint3d pt;
	acedInitGet(RSG_NONULL, TEXT("K W"));
	rc = CGetInputUtil::GetPointReturnCode(TEXT("\n����һ�����[Keyword1/keyWord2]:"), pt);
	
	switch (rc)
	{
	case RTKWORD:	// �����˹ؼ���
		if (acedGetInput(kword) != RTNORM)
		{
			return;
		}
		if (_tcscmp(kword, TEXT("K")) == 0)
		{
			acutPrintf(TEXT("\nѡ��Ĺؼ�����Keyword1!"));
		}
		else if (_tcscmp(kword, TEXT("W")) == 0)
		{
			acutPrintf(TEXT("\nѡ��Ĺؼ�����keyWord2!"));
		}
		break;
	case RTNORM:
		acutPrintf(TEXT("\n������������(%.2f, %.2f, %.2f)"), pt.x, pt.y, pt.z);
		break;
	default:
		break;
	}
}

// This is command 'ADDPOLY'
void ZffChap5AddPoly()
{
	int colorIndex = 0;			// ��ɫ����ֵ
	ads_real width = 0;			// ����ߵ��߿�	
	int index = 2;				// ��ǰ�����Ĵ���	
	
	// ��ʾ�û��������
	AcGePoint3d ptStart;			// ���
	if (!CGetInputUtil::GetPoint(TEXT("\n�����һ��:"), ptStart))
	{
		return;
	}
	
	AcGePoint3d ptPrevious, ptCurrent;	// ǰһ���ο��㣬��ǰʰȡ�ĵ�
	ptPrevious = ptStart;
	AcDbObjectId polyId;				// ����ߵ�ID
	
	// ����ڶ���
	acedInitGet(NULL, TEXT("W C O"));
	int rc = CGetInputUtil::GetPointReturnCode(ptPrevious, 
		TEXT("\n������һ��[���(W)/��ɫ(C)]<���(O)>:"), ptCurrent);
	while (rc == RTNORM || rc == RTKWORD)
	{
		if (rc == RTKWORD)		// ����û������˹ؼ���
		{
			TCHAR kword[20];
			if (acedGetInput(kword) != RTNORM)
				return;
			if (_tcscmp(kword, TEXT("W")) == 0)
			{
				width = GetWidth();				
			}
			else if (_tcscmp(kword, TEXT("C")) == 0)
			{
				colorIndex = GetColorIndex();
			}
			else if (_tcscmp(kword, TEXT("O")) == 0)
			{
				return;
			}
			else
			{
				acutPrintf(TEXT("\n��Ч�Ĺؼ���."));
			}
		}
		else if (rc == RTNORM)	// �û������˵�
		{
			if (index == 2)
			{
				// ���������
				polyId = CPolylineUtil::Add(CConvertUtil::ToPoint2d(ptPrevious), CConvertUtil::ToPoint2d(ptCurrent));
				AcDbPolyline *pPoly = NULL;
				if (acdbOpenObject(pPoly, polyId, AcDb::kForWrite) == Acad::eOk)
				{
					// �޸Ķ���ߵ���ɫ���߿�
					pPoly->setConstantWidth(width);
					pPoly->setColorIndex(colorIndex);
					pPoly->close();
				}
			}
			else if (index > 2)
			{
				// �޸Ķ���ߣ�������һ������
				AcDbPolyline *pPoly = NULL;
				if (acdbOpenObject(pPoly, polyId, AcDb::kForWrite) == Acad::eOk)
				{
					pPoly->addVertexAt(index - 1, CConvertUtil::ToPoint2d(ptCurrent));	
					// �޸Ķ���ߵ���ɫ���߿�
					pPoly->setConstantWidth(width);
					pPoly->setColorIndex(colorIndex);

					pPoly->close();
				}
			}
			
			ptPrevious = ptCurrent;
			index++;
		}
		
		// ��ʾ�û������µĽڵ�
		acedInitGet(NULL, TEXT("W C O"));
		rc = CGetInputUtil::GetPointReturnCode(ptPrevious, TEXT("\n������һ�� [���(W)/��ɫ(C)]<���(O)>:"), ptCurrent);
	}
}

// This is command 'SELECTFILE'
void ZffChap5SelectFile()
{
	CString title = TEXT("ѡ��ͼ���ļ�");
	CString path = TEXT("C:\\");
	struct resbuf *fileName = acutNewRb(RTSTR);
	
	if (acedGetFileD(title, path, TEXT("dwg;dxf"), 0, fileName) == RTNORM)
	{
		acedAlert(fileName->resval.rstring);
	}
	
	acutRelRb(fileName);
}

