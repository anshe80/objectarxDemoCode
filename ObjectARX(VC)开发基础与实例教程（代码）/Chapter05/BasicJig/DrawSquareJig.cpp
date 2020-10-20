// DrawSquareJig.cpp: implementation of the CDrawSquareJig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawSquareJig.h"
#include "..\..\Common\Others\ConvertUtil.h"
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include "..\..\Common\Geometry\MathUtil.h"
#include "..\..\Common\Geometry\GePointUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawSquareJig::CDrawSquareJig()
{
	m_pPoly = NULL;
}

CDrawSquareJig::~CDrawSquareJig()
{

}

bool CDrawSquareJig::doIt( const AcGePoint3d &centerPoint, AcDbObjectId &polyId )
{
	m_centerPoint = centerPoint;

	// �϶���ʼ֮ǰ��������ߴ�������
	m_pPoly = new AcDbPolyline();
	for (int i = 0; i < 4; i++)
	{
		m_pPoly->addVertexAt(i, CConvertUtil::ToPoint2d(m_centerPoint));
	}
	m_pPoly->setClosed(Adesk::kTrue);

	// �����϶�����
	CString prompt = TEXT("\nָ����ע�����:");	
	setDispPrompt(prompt);
	AcEdJig::DragStatus stat = drag();

	// �϶��������������ز���
	if (stat == kNormal)
	{
		polyId = CDwgDatabaseUtil::PostToModelSpace(m_pPoly);		
		return true;
	}
	else
	{
		delete m_pPoly;
		return false;
	}
}

AcEdJig::DragStatus CDrawSquareJig::sampler()
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

Adesk::Boolean CDrawSquareJig::update()
{
	// ʵ����ĸ��²�������������µ���m_pPoly
	double dist = CConvertUtil::ToPoint2d(m_centerPoint).distanceTo(CConvertUtil::ToPoint2d(m_curPoint));
	for (int i = 0; i < 4; i++)
	{
		double angle = i * CMathUtil::PI() * 0.5 + CMathUtil::PI() * 0.25;
		AcGePoint2d pt = CGePointUtil::PolarPoint(CConvertUtil::ToPoint2d(m_centerPoint), angle, dist);
		m_pPoly->setPointAt(i, pt);
	}

	return Adesk::kTrue;
}

AcDbEntity* CDrawSquareJig::entity() const
{
	return m_pPoly;
}
