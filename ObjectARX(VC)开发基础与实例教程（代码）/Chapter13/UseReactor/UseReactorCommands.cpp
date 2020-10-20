//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <dbents.h>
#include "MyObjectReactor.h"

// This is command 'SETLINKENTS'
void ZffChap13SetLinkEnts()
{
	// ��ʾ�û�ѡ������Բ
	AcDbObjectIdArray circleIds;
	if (CSelectUtil::PromptSelectEnts(TEXT("\nѡ������Բ:"), AcDbCircle::desc(), circleIds))
	{
		if (circleIds.length() != 2)
		{
			acutPrintf(TEXT("\n����ѡ������Բ."));
			return;
		}

		// ��һ��Բ�ķ�Ӧ��
		AcDbDictionary *pNamedDict = NULL;
		CMyObjectReactor *pObjReactorA = new CMyObjectReactor();
		pObjReactorA->SetLinkEntId(circleIds[1]);
		AcDbObjectId objReactorAId;
		Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->
			getNamedObjectsDictionary(pNamedDict, AcDb::kForWrite);
		pNamedDict->setAt(TEXT("CircleA"), pObjReactorA, objReactorAId);
		AcDbCircle *pCircleA = NULL;
		if (acdbOpenObject(pCircleA, circleIds[0], AcDb::kForWrite) == Acad::eOk)
		{
			pCircleA->addPersistentReactor(objReactorAId);
			pCircleA->close();
		}

		// �ڶ���Բ�ķ�Ӧ��
		CMyObjectReactor *pObjReactorB = new CMyObjectReactor();
		pObjReactorB->SetLinkEntId(circleIds[0]);
		AcDbObjectId objReactorBId;
		pNamedDict->setAt(TEXT("CircleB"), pObjReactorB, objReactorBId);
		AcDbCircle *pCircleB = NULL;
		if (acdbOpenObject(pCircleB, circleIds[1], AcDb::kForWrite) == Acad::eOk)
		{
			pCircleB->addPersistentReactor(objReactorBId);
			pCircleB->close();
		}
		pNamedDict->close();
	}
}

