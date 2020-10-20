// MyObjectReactor.h: interface for the CMyObjectReactor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYOBJECTREACTOR_H__89798AC8_DB70_4D32_A5C1_44F934EF70BA__INCLUDED_)
#define AFX_MYOBJECTREACTOR_H__89798AC8_DB70_4D32_A5C1_44F934EF70BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyObjectReactor : public AcDbObject
{
public:
	// ���û�����򵼴��������һ���ǵü�����仰
	ACRX_DECLARE_MEMBERS(CMyObjectReactor);

	CMyObjectReactor();
	virtual ~CMyObjectReactor();

	Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
    Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;

	void modified(const AcDbObject* pObj);
	virtual void erased(const AcDbObject* dbObj, Adesk::Boolean pErasing = true);

	// �������������ʵ��
	void SetLinkEntId(AcDbObjectId entId);
    
private:
	AcDbObjectId m_entId;		// ��֪ͨ���������ʵ��ID
};

#endif // !defined(AFX_MYOBJECTREACTOR_H__89798AC8_DB70_4D32_A5C1_44F934EF70BA__INCLUDED_)
