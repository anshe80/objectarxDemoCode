// EntInfoTipHandler.cpp: implementation of the CEntInfoTipHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EntInfoTipHandler.h"
#include "AdskDMgr.h"
#include "docdata.h"
#include <dbents.h>
#include "..\..\Common\Others\ConvertUtil.h"
#include "..\..\Common\Geometry\MathUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// �������ȫ�ֺ����Ķ�����������obj��
extern AsdkDataManager<CDocData> DocVars;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEntInfoTipHandler::CEntInfoTipHandler(const bool storeInDocVars, AcApDocument* pDoc)
{
	if (storeInDocVars) 
	{		
		assert(DocVars.docData(pDoc).m_pIPM == NULL);
		
		// �������������
		m_pDoc = pDoc;
		m_pDoc->inputPointManager()->addPointMonitor(this);
		DocVars.docData().m_pIPM = this;
	}
	else
	{
		m_pDoc = NULL;
	}
}

CEntInfoTipHandler::~CEntInfoTipHandler()
{
	if (m_pDoc)
	{
		if (DocVars.docData(m_pDoc).m_pIPM) 
		{
			// ɾ������������
			m_pDoc->inputPointManager()->removePointMonitor(this);
			DocVars.docData(m_pDoc).m_pIPM = NULL;
		}
	}
}

bool CEntInfoTipHandler::excludeFromOsnapCalculation( const AcArray<AcDbObjectId>& nestedEntity, int gsSelectionMark )
{
	return false;
}

Acad::ErrorStatus CEntInfoTipHandler::monitorInputPoint(
	 bool& bAppendToTooltipStr,
     TCHAR*& pAdditionalTooltipString,
     AcGiViewportDraw* pDrawContext,
     AcApDocument* pDocument,
     bool pointComputed,
     int history,
     const AcGePoint3d& lastPoint,
     const AcGePoint3d& rawPoint,
     const AcGePoint3d& grippedPoint,
     const AcGePoint3d& cartesianSnappedPoint,
     const AcGePoint3d& osnappedPoint,
     AcDb::OsnapMask osnapMask,
     const AcArray<AcDbCustomOsnapMode*>& customOsnapModes,
     AcDb::OsnapMask osnapOverrides,
     const AcArray<AcDbCustomOsnapMode*>& customOsnapOverrides,
     const AcArray<AcDbObjectId>& apertureEntities,
     const AcArray< AcDbObjectIdArray,
     AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& nestedApertureEntities,
     const AcArray<int>& gsSelectionMark,
     const AcArray<AcDbObjectId>& keyPointEntities,
     const AcArray< AcDbObjectIdArray,
     AcArrayObjectCopyReallocator< AcDbObjectIdArray > >& nestedKeyPointEntities,
     const AcArray<int>& keyPointGsSelectionMark,
     const AcArray<AcGeCurve3d*>& alignmentPaths,
     const AcGePoint3d& computedPoint,
     const TCHAR* pTooltipString)
{
	// һ��Ҫע���黺�����Ĵ�С������Խ�絼�µ�Acadֱ������
	TCHAR mtooltipStr[1024], tempStr[1024];			
	mtooltipStr[0] = '\0';

	Acad::ErrorStatus es;
	AcDbEntity* pEnt;
	AcDbObjectId highlightId = AcDbObjectId::kNull;

	if (pointComputed)
	{
		// ������������ǵ�ʵ��
		if (apertureEntities.length() > 0)
		{
			for (int i = 0; i < apertureEntities.length(); ++i)
			{
				// ������ʾ�����ʵ�壨������Ҫȷ���Ƿ���Ҫ��
				if (i > 0)
				{
					break;
				}

				if (Acad::eOk != (acdbOpenAcDbEntity(pEnt, apertureEntities[i], AcDb::kForRead)))
				{
					continue;
				}

				if (pEnt->isKindOf(AcDbLine::desc()))
				{					
					// ʵ��������Ϣ
					if (_tcslen(mtooltipStr) > 0)
					{
						_tcscpy(mtooltipStr, TEXT("ֱ��:"));
					}
					else
					{
						_tcscpy(mtooltipStr, TEXT("\nֱ��:"));
					}

					// ʵ����ϸ��Ϣ
					AcDbLine* pLine = AcDbLine::cast(pEnt);
					double length = pLine->startPoint().distanceTo(pLine->endPoint());
					AcGeVector3d vec = pLine->endPoint() - pLine->startPoint();
					double angle = vec.convert2d(AcGePlane::kXYPlane).angle();
					_stprintf(tempStr, TEXT("\n ����: %.2f \n ���: %.2f"), length, CConvertUtil::RadianToAngle(angle));
					_tcscat(mtooltipStr, tempStr);
				}
				else if (pEnt->isKindOf(AcDbCircle::desc()))
				{
					// ʵ��������Ϣ
					if (_tcslen(mtooltipStr) > 0)
					{
						_tcscpy(mtooltipStr, TEXT("Բ:"));
					}
					else
					{
						_tcscpy(mtooltipStr, TEXT("\nԲ:"));
					}
					
					// ʵ����ϸ��Ϣ
					AcDbCircle* pCircle = AcDbCircle::cast(pEnt);
					double radius = pCircle->radius();
					double area = CMathUtil::PI() * radius * radius;
					_stprintf(tempStr, TEXT("\n �뾶: %.2f \n ���: %.2f"), radius, area);
					_tcscat(mtooltipStr, tempStr);
				}

				pEnt->close();				
			}
			highlightId = apertureEntities[0];
		}			
	}

	// ִ�и�����ʾ��ֻ������ʾ����ʵ��ᱻ������ʾ
	static AcDbObjectId oldHighlightId = AcDbObjectId::kNull;
	if(highlightId != oldHighlightId)
	{
		if (AcDbObjectId::kNull != oldHighlightId)
		{
			es = acdbOpenAcDbEntity(pEnt, oldHighlightId, AcDb::kForRead);
			if (es == Acad::eOk)
			{
				es = pEnt->unhighlight();
				pEnt->close();
				oldHighlightId = AcDbObjectId::kNull;
			}
		}
		es = acdbOpenAcDbEntity(pEnt, highlightId, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			es = pEnt->highlight();
			pEnt->close();
			oldHighlightId = highlightId;
		}
	}	

	// ��ʾ��������ʾ����
	bAppendToTooltipStr = true;
	pAdditionalTooltipString = mtooltipStr;

	return Acad::eOk;
}