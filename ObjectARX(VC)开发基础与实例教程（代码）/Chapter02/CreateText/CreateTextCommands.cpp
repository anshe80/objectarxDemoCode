//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Entity\TextUtil.h"

// This is command 'ADDTEXT'
void ZffChap2AddText()
{
	// ������������
	AcGePoint3d ptInsert(0, 18, 0);
	CTextUtil::AddText(ptInsert, TEXT("�����Ƿ��߿�"));
	
	// ������������
	ptInsert.set(0, 0, 0);
	CTextUtil::AddMText(ptInsert, TEXT("ѧϰ��ʹ��ObjectARX��Ҫ�������Ŭ��."));
}

