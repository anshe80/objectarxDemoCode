// DrawSquareJig.h: interface for the CDrawSquareJig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWSQUAREJIG_H__709CE93B_B69C_473B_9073_7360D39855A3__INCLUDED_)
#define AFX_DRAWSQUAREJIG_H__709CE93B_B69C_473B_9073_7360D39855A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dbpl.h>

class CDrawSquareJig : AcEdJig
{
public:
	CDrawSquareJig();
	virtual ~CDrawSquareJig();

	// �ⲿ���õĺ�����һ������Jig�ĳ�ʼ��
	bool doIt(const AcGePoint3d &centerPoint, AcDbObjectId &polyId);
	
	// �˺�������drag���������Ի���û�����
	virtual AcEdJig::DragStatus sampler();
	
	// ����Ҫ���϶������з����仯��ʵ������޸�
	virtual Adesk::Boolean update();
	
	// ָ����Jig�������Ķ���
	virtual AcDbEntity* entity() const;

private:
	AcDbPolyline* m_pPoly;		// �϶������ж�̬�仯��ʵ��
	AcGePoint3d m_curPoint;		// �洢�û�����ƶ�ʱ�����ʱλ��
	AcGePoint3d m_centerPoint;	// �����ε����ĵ�
};

#endif // !defined(AFX_DRAWSQUAREJIG_H__709CE93B_B69C_473B_9073_7360D39855A3__INCLUDED_)
