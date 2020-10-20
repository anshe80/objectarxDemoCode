//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <dbxrecrd.h>
#include <dbdict.h>
#include <geassign.h>
#include <dbents.h>

// This is command 'ADDXRECORD'
void ZffChap6AddXRecord()
{
	// ��ʾ�û�ѡ����Ҫ�����չ��¼��ͼ�ζ���
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����Ҫ�����չ��¼��ʵ��:"), AcDbEntity::desc(), pEnt, pickPoint))
	{
		// ��ʵ���������չ�ֵ�
		pEnt->createExtensionDictionary();
		AcDbObjectId dictObjId = pEnt->extensionDictionary();
		pEnt->close();

		// ������չ��¼ʵ��
		AcDbXrecord *pXrec = new AcDbXrecord;
		
		// ����չ�ֵ������һ����¼
		AcDbObjectId xRecObjId;
		AcDbDictionary *pDict = NULL;
		if (acdbOpenObject(pDict, dictObjId, AcDb::kForWrite) == Acad::eOk)
		{
			pDict->setAt(TEXT("XRecord"), pXrec, xRecObjId);
			pDict->close();
		}
		
		// ������չ��¼������
		struct resbuf* pRb = acutBuildList(AcDb::kDxfText, TEXT("�����ַ�������"), 
			AcDb::kDxfInt32, 12, 
			AcDb::kDxfReal, 3.14, 
			AcDb::kDxfXCoord, asDblArray(pickPoint), 
			RTNONE);
		pXrec->setFromRbChain(*pRb);	// ��չ�������������չ��¼����
		pXrec->close();
		acutRelRb(pRb);
	}
}

// This is command 'VIEWXRECORD'
void ZffChap6ViewXRecord()
{
	// ��ʾ�û�ѡ����Ҫ�鿴��չ��¼��ͼ�ζ���
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����Ҫ�鿴��չ��¼��ʵ��:"), AcDbEntity::desc(), pEnt, pickPoint))
	{
		// ���ʵ�����չ�ֵ�
		AcDbObjectId dictObjId = pEnt->extensionDictionary(); 
		pEnt->close();		
		if (dictObjId.isNull())
		{
			acutPrintf(TEXT("\n��ѡ���ʵ�岻������չ�ֵ�!"));
			return;
		}

		// ����չ�ֵ䣬�����ؼ���"XRecord"��������չ��¼
		AcDbDictionary *pDict = NULL;
		AcDbXrecord *pXrec = NULL;
		struct resbuf *pRb = NULL;
		if (acdbOpenObject(pDict, dictObjId, AcDb::kForRead) == Acad::eOk)
		{
			pDict->getAt(TEXT("XRecord"), (AcDbObject*&)pXrec, AcDb::kForRead);
			
			// �����չ��¼�����������ر���չ���ݶ���			
			pXrec->rbChain(&pRb);
			pXrec->close();

			pDict->close();
		}
		
		if (pRb != NULL)
		{
			// ����������ʾ��չ��¼����
			struct resbuf *pTemp = pRb;
			
			acutPrintf(TEXT("\n�ַ������͵���չ�����ǣ�%s"), pTemp->resval.rstring);
			
			pTemp = pTemp->rbnext;
			acutPrintf(TEXT("\n�������͵���չ�����ǣ�%d"), pTemp->resval.rint);
			
			pTemp = pTemp->rbnext;
			acutPrintf(TEXT("\nʵ�����͵���չ�����ǣ�%.2f"), pTemp->resval.rreal);
			
			pTemp = pTemp->rbnext;
			acutPrintf(TEXT("\n���������͵���չ�����ǣ�(%.2f, %.2f, %.2f)"), 
				pTemp->resval.rpoint[X], pTemp->resval.rpoint[Y], 
				pTemp->resval.rpoint[Z]);
			
			acutRelRb(pRb);		
		}
	}	
}

// This is command 'ADDNAMEDICT'
void ZffChap6AddNameDict()
{
	// ������������ֵ䣬���������ָ�����ֵ���
	AcDbDictionary *pNameObjDict = NULL;//, 
	acdbHostApplicationServices()->workingDatabase()
		->getNamedObjectsDictionary(pNameObjDict, AcDb::kForWrite);
	// �����Ҫ��ӵ��ֵ����Ƿ��Ѿ�����
	AcDbObjectId dictObjId;
	AcDbDictionary *pDict = NULL;
	if (pNameObjDict->getAt(TEXT("MyDict"), (AcDbObject*&)pDict, AcDb::kForWrite) == Acad::eKeyNotFound)
	{
		pDict = new AcDbDictionary;
		pNameObjDict->setAt(TEXT("MyDict"), pDict, dictObjId);
		pDict->close();
	}
	pNameObjDict->close();
	
	// ���½����ֵ������һ��ͼ�����
	if (acdbOpenObject(pDict, dictObjId, AcDb::kForWrite) == Acad::eOk)
	{
		AcDbLayerTableRecord *pLayer = new AcDbLayerTableRecord();		
		AcCmColor color;
		color.setColorIndex(20);
		pLayer->setColor(color);
		pLayer->setIsOff(true);
		pLayer->setIsFrozen(true);
		AcDbObjectId layerId;
		Acad::ErrorStatus es = pDict->setAt(TEXT("LayerKey"), pLayer, layerId);
		pLayer->close();
		pDict->close();
	}
}

// This is command 'VIEWNAMEDICT'
void ZffChap6ViewNameDict()
{
	// ��ö��������ֵ���ָ�����ֵ���
	AcDbDictionary *pNameObjDict = NULL;
	Acad::ErrorStatus es;
	acdbHostApplicationServices()->workingDatabase()
		->getNamedObjectsDictionary(pNameObjDict, AcDb::kForRead);
	AcDbDictionary *pDict = NULL;
	es = pNameObjDict->getAt(TEXT("MyDict"), (AcDbObject*&)pDict, AcDb::kForRead);
	pNameObjDict->close();
	
	// ���������ָ�����ֵ���˳�����
	if (es == Acad::eKeyNotFound)
	{
		acutPrintf(TEXT("\n�������ֵ���MyDict."));
		return;
	}
	
	// ���ָ���Ķ����ֵ��еĶ���
	AcDbLayerTableRecord *pLayer = NULL;
	if (pDict->getAt(TEXT("LayerKey"), (AcDbObject*&)pLayer, AcDb::kForRead) == Acad::eOk)
	{
		// ���ʵ�����Ϣ
		int colorIndex = pLayer->color().colorIndex();
		bool bOff = pLayer->isOff();
		bool bFrozen = pLayer->isFrozen();
		pLayer->close();

		// �������д�ӡʵ����Ϣ
		acutPrintf(TEXT("\n�ֵ��е�ͼ����ɫ: %d"), colorIndex);
		acutPrintf(TEXT("\n�ֵ��е�ͼ���Ƿ��ǹر�״̬: %s"), bOff ? TEXT("��") : TEXT("��"));
		acutPrintf(TEXT("\n�ֵ��е�ͼ���Ƿ��ǹر�״̬: %s"), bFrozen ? TEXT("��") : TEXT("��"));
	}
	pDict->close();
}

