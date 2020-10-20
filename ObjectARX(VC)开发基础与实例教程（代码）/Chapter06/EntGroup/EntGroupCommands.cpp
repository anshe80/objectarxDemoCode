//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <dbgroup.h>
#include <dbdict.h>

// This is command 'ADDGROUP'
void ZffChap6AddGroup()
{
	// ��ʾ�û�ѡ����ʵ��
	AcDbObjectIdArray entIds;
	if (CSelectUtil::PromptSelectEnts(TEXT("\nѡ��Ҫ�����ʵ��:"), AcDbEntity::desc(), entIds))
	{
		// ������
		AcDbGroup *pGroup = new AcDbGroup();
		for (int i = 0; i < entIds.length(); i++) 
		{
			pGroup->append(entIds[i]);
		}
		
		// ������ӵ����������ֵ�����ֵ���
		AcDbDictionary *pGroupDict = NULL;
		acdbHostApplicationServices()->workingDatabase()
			->getGroupDictionary(pGroupDict, AcDb::kForWrite);
		
		// ���������Ϊ*���ᴴ��������)
		CString groupName = TEXT("*");
		AcDbObjectId groupId;
		pGroupDict->setAt(groupName, pGroup, groupId);
		pGroupDict->close();
		pGroup->close();
	}
}

// This is command 'DELETEGROUP'
void ZffChap6DeleteGroup()
{
	AcDbObjectIdArray entIds;
	if (CSelectUtil::PromptSelectEnts(TEXT("\nѡ��Ҫ�ֽ�����е�ʵ��:"), AcDbEntity::desc(), entIds))
	{
		// �������ֵ䣬�ж��Ƿ�����˸�����ʵ��
		AcDbDictionary *pGroupDict = NULL;
		acdbHostApplicationServices()->workingDatabase()->getGroupDictionary(pGroupDict, AcDb::kForWrite);
		AcDbDictionaryIterator *it = pGroupDict->newIterator();
		for (; !it->done(); it->next())
		{
			AcDbGroup *pGroup = NULL;
			if (it->getObject((AcDbObject*&)pGroup, AcDb::kForWrite) == Acad::eOk)
			{
				AcDbObjectIdArray groupEntIds;
				pGroup->allEntityIds(groupEntIds);
				bool bFound = false;
				for (int i = 0; i < entIds.length(); i++)
				{
					if (groupEntIds.contains(entIds[i]))
					{
						bFound = true;
						break;
					}
				}
				// ɾ������ѡ����ʵ�����
				if (bFound)
				{
					pGroup->erase();
				}
				
				pGroup->close();
			}
		}		
		pGroupDict->close();
	}
}

