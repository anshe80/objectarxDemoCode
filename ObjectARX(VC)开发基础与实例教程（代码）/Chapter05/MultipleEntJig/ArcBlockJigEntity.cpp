// ArcBlockJigEntity.cpp: implementation of the CArcBlockJigEntity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ArcBlockJigEntity.h"
#include "..\..\Common\Entity\LineUtil.h"
#include "..\..\Common\Others\ConvertUtil.h"
#include <acgi.h>
#include <dbents.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include "..\..\Common\Geometry\MathUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArcBlockJigEntity::CArcBlockJigEntity(const AcGePoint3d &startPoint, const AcGePoint3d &thirdPoint, 
									   const AcGePoint3d &endPoint, AcDbObjectId blkDefId, int count)
{
	m_startPoint = startPoint;
	m_thirdPoint = thirdPoint;
	m_endPoint = endPoint;
	m_blkDefId = blkDefId;
	m_blockCount = count;
}

CArcBlockJigEntity::~CArcBlockJigEntity()
{

}

Adesk::Boolean CArcBlockJigEntity::worldDraw( AcGiWorldDraw* mode )
{
	DrawOrAddSubEnts(mode);

	return Adesk::kTrue;
}

void CArcBlockJigEntity::SetEndPoint( const AcGePoint3d &pt )
{
	// ������������worldDraw�����ĵ���
	assertWriteEnabled();

	m_endPoint = pt;
}

void CArcBlockJigEntity::PostToModelSpace()
{
	DrawOrAddSubEnts(NULL);
}

void CArcBlockJigEntity::DrawOrAddSubEnts( AcGiWorldDraw* mode )
{
	// ����Բ��
	AcDbCurve *pCurve = NULL;		// ����ȷֵ������
	AcGePoint2d startPoint2d = CConvertUtil::ToPoint2d(m_startPoint);
	AcGePoint2d thirdPoint2d = CConvertUtil::ToPoint2d(m_thirdPoint);
	AcGePoint2d endPoint2d = CConvertUtil::ToPoint2d(m_endPoint);
	if (CLineUtil::ThreePointIsCollinear(startPoint2d, thirdPoint2d, endPoint2d))	// ���㹲�ߣ�����ֱ��
	{
		AcGePoint3d verts[2];
		verts[0] = m_startPoint;
		verts[1] = m_endPoint;
		if (mode != NULL)
		{
			mode->geometry().polyline(2, verts);
		}
		
		pCurve = new AcDbLine(m_startPoint, m_endPoint);
	}
	else	// ����Բ��
	{
		if (mode != NULL)
		{
			mode->geometry().circularArc(m_startPoint, m_thirdPoint, m_endPoint);
		}
		
		AcGeCircArc2d geArc(startPoint2d, thirdPoint2d, endPoint2d);
		// geArc����ʼ�Ƕ�ʼ����0����˱��뵥��������ʼ�ǶȺ���ֹ�Ƕ�
		AcGeVector2d vecStart = startPoint2d - geArc.center();
		AcGeVector2d vecEnd = endPoint2d - geArc.center();
		// AcDbArc��������ʱ��ģ������Ҫ�����������ת����ȷ����ȷ����ʼ�Ƕ�
		double startAngle = 0;
		if (CLineUtil::PtInLeftOfLine(startPoint2d, thirdPoint2d, endPoint2d) > 0)	// ��ʱ��
		{
			startAngle = vecStart.angle();
		}
		else
		{
			startAngle = vecEnd.angle();
		}
		double endAngle = startAngle + (geArc.endAng() - geArc.startAng());
		pCurve = new AcDbArc(CConvertUtil::ToPoint3d(geArc.center()), geArc.radius(), startAngle, endAngle);
	}
	
	// ����ȷֵ㣬��ÿ���ղ����λ��
	double startParam = 0, endParam = 0;		// ���ߵ������յ����
	pCurve->getStartParam(startParam);
	pCurve->getEndParam(endParam);
	int intervalCount = m_blockCount + 1;		// �ȷּ������ȿ����������1
	double paramInterval = (endParam - startParam) / intervalCount;
	AcGePoint3dArray blkRefPoints;		// ����յĲ���㼯��
	for (int i = 1; i < intervalCount; i++)		// ���ߺ������յ㲻��Ҫ����ͼ��
	{
		double param = startParam + i * paramInterval;
		AcGePoint3d pt;
		pCurve->getPointAtParam(param, pt);
		blkRefPoints.append(pt);
	}

	if (mode != NULL)	// ��ʾ��ʵ��
	{
		delete pCurve;		// ��̬�����ʵ�壬������ģ�Ϳռ䣬ʹ�����֮����Ҫ�ͷ�
	}
	else	// �����ʵ��ķ�ʽ
	{
		CDwgDatabaseUtil::PostToModelSpace(pCurve);
	}
	
	// ���Ƽ���ͼ��
	m_blkRefIds.setLogicalLength(0);
	for (i = 0; i < blkRefPoints.length(); i++)
	{
		AcDbBlockReference *pBlkRef = new AcDbBlockReference(blkRefPoints[i], m_blkDefId);
		if (mode != NULL)
		{
			pBlkRef->worldDraw(mode);
			delete pBlkRef;
		}
		else
		{
			m_blkRefIds.append(CDwgDatabaseUtil::PostToModelSpace(pBlkRef));
		}
	}
}

AcDbObjectIdArray CArcBlockJigEntity::GetBlkRefIds()
{
	return m_blkRefIds;
}
