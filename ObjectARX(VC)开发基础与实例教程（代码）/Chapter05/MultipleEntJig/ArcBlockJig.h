// ArcBlockJig.h: interface for the CArcBlockJig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBLOCKJIG_H__C0A05BEA_FABF_41C9_9F06_A11DF104871D__INCLUDED_)
#define AFX_ARCBLOCKJIG_H__C0A05BEA_FABF_41C9_9F06_A11DF104871D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ArcBlockJigEntity.h"

class CArcBlockJig  : AcEdJig
{
public:
	CArcBlockJig();
	virtual ~CArcBlockJig();

	// ����startPoint:��ʼ��; endPoint:��ֹ��; thirdPoint:������; blkDefId:���Id; 
	// blockCount: �����ĸ���;
	bool doIt(const AcGePoint3d &startPoint, AcGePoint3d &thirdPoint, AcGePoint3d &endPoint, 
		AcDbObjectId blkDefId, int blockCount);
	virtual AcEdJig::DragStatus sampler();		// �˺�������drag���������Ի��һ������
	virtual Adesk::Boolean update();
	virtual AcDbEntity* entity() const;			// ָ����Jig�������Ķ���

	// ��ȡJig�����ɹ������Ŀ���ռ���
	AcDbObjectIdArray GetBlkRefIds();
	
private:
	CArcBlockJigEntity* m_pJigEnt;
	AcGePoint3d m_curPoint;
	AcDbObjectIdArray m_blkRefIds;
};

#endif // !defined(AFX_ARCBLOCKJIG_H__C0A05BEA_FABF_41C9_9F06_A11DF104871D__INCLUDED_)
