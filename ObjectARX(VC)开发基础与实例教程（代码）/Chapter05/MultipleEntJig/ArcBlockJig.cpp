// ArcBlockJig.cpp: implementation of the CArcBlockJig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ArcBlockJig.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArcBlockJig::CArcBlockJig()
{
	m_pJigEnt = NULL;
}

CArcBlockJig::~CArcBlockJig()
{
	if (m_pJigEnt)
	{
		delete m_pJigEnt;
		m_pJigEnt = NULL;
	}
}

bool CArcBlockJig::doIt( const AcGePoint3d &startPoint, AcGePoint3d &thirdPoint, AcGePoint3d &endPoint, 
						AcDbObjectId blkDefId, int blockCount )
{
	// �϶�֮ǰ�������Զ���ʵ��
	if (m_pJigEnt != NULL)
	{
		delete m_pJigEnt;
		m_pJigEnt = NULL;	
	}
	m_pJigEnt = new CArcBlockJigEntity(startPoint, thirdPoint, endPoint, blkDefId, blockCount);
	
	// ִ���϶�����
	CString prompt = TEXT("\nָ����һ��: ");
	setDispPrompt(prompt);
	AcEdJig::DragStatus stat = drag();
	
	// �϶�֮�󣺸�����Ҫȷ���Լ��Ĵ���ʽ
	bool bRet = false;
	if (stat == kNormal)
	{
		// �����ʵ�嵽ģ�Ϳռ�
		m_pJigEnt->PostToModelSpace();	
		bRet = true;
	}
	
	m_blkRefIds = m_pJigEnt->GetBlkRefIds();
	delete m_pJigEnt;
	m_pJigEnt = NULL;
	
	return bRet;
}

AcEdJig::DragStatus CArcBlockJig::sampler()
{
	setUserInputControls((UserInputControls)
        (AcEdJig::kAccept3dCoordinates
		| AcEdJig::kNoNegativeResponseAccepted
		| AcEdJig::kNullResponseAccepted));	
	
	// һ��Ҫ�ж�һ�µ��Ƿ����˱仯������update������ͣ�ر����ã�ʵ�巴�����ܱ����Ƴ���
	static AcGePoint3d pointTemp;
	DragStatus stat = acquirePoint(m_curPoint);
	if (pointTemp != m_curPoint)
	{
		pointTemp = m_curPoint;
	}
	else if (stat == AcEdJig::kNormal)
	{
		return AcEdJig::kNoChange;
	}
	
	return stat;
}

Adesk::Boolean CArcBlockJig::update()
{
	m_pJigEnt->SetEndPoint(m_curPoint);
	return Adesk::kTrue;
}

AcDbEntity* CArcBlockJig::entity() const
{
	return m_pJigEnt;
}

AcDbObjectIdArray CArcBlockJig::GetBlkRefIds()
{
	return m_blkRefIds;
}
