//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\CustomObjectDB\ZffDwgScale.h"
#include <dbdict.h>

const CString DWG_SCALE_KEY = TEXT("DwgScale");

// This is command 'SETDWGSCALE'
void ZffChap9SetDwgScale()
{
	// ������������ֵ�
	AcDbDictionary *pNameObjDict = NULL;
	acdbHostApplicationServices()->workingDatabase()
		->getNamedObjectsDictionary(pNameObjDict, AcDb::kForWrite);

	// ����ֵ������ZffDwgScale����
	AcDbObjectId dictObjId;
	ZffDwgScale *pDwgScale = new ZffDwgScale();
	pDwgScale->Set(1, 100);
	pNameObjDict->setAt(DWG_SCALE_KEY, pDwgScale, dictObjId);
	pDwgScale->close();
	pNameObjDict->close();
}

// This is command 'GETDWGSCALE'
void ZffChap9GetDwgScale()
{
	// ��ö��������ֵ���ָ�����ֵ���
	AcDbDictionary *pNameObjDict = NULL;
	Acad::ErrorStatus es;
	acdbHostApplicationServices()->workingDatabase()
		->getNamedObjectsDictionary(pNameObjDict, AcDb::kForRead);
	ZffDwgScale *pDwgScale = NULL;
	es = pNameObjDict->getAt(DWG_SCALE_KEY, (AcDbObject*&)pDwgScale, AcDb::kForRead);
	pNameObjDict->close();
	
	// ���������ָ�����ֵ���˳�����
	if (es == Acad::eKeyNotFound)
	{
		acutPrintf(TEXT("\n�������ֵ���%s."), DWG_SCALE_KEY);
		return;
	}
	
	// ���ָ���Ķ����ֵ��еĶ���
	acutPrintf(TEXT("\n��ǰͼ������ͼ����:%d, ��ͼ����:%d"), pDwgScale->GetInfoScale(), pDwgScale->GetLabelScale());
	pDwgScale->close();
}

