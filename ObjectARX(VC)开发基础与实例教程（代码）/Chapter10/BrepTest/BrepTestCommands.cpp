//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <dbsol3d.h>
#include <brbrep.h>
#include <brbftrav.h>
#include <brface.h>
#include <gexbndsf.h>
#include <gecylndr.h>
#include <brbetrav.h>
#include <bredge.h>
#include <geextc3d.h>
#include "..\..\Common\Entity\PolylineUtil.h"

// This is command 'GETCYLINDERINFO'
void ZffChap10GetCylinderInfo()
{
	// ��ʾ�û�ѡ��ʵ��
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����άʵ��:"), AcDb3dSolid::desc(), pEnt, pickPoint))
	{
		// ����һ��brep����
		AcBrBrep brepEnt;
		brepEnt.set(*pEnt);

		// ����һ����ı�����
		AcBr::ErrorStatus returnValue = AcBr::eOk;
		AcBrBrepFaceTraverser brepFaceTrav;
		returnValue = brepFaceTrav.setBrep(brepEnt);

		// ������
		while (!brepFaceTrav.done() && (returnValue == AcBr::eOk))
		{
			// ��������
			AcBrFace brFace;
			AcBr::ErrorStatus es =	brepFaceTrav.getFace(brFace);
			if (es == AcBr::eOk)
			{
				// ��ü��������
				AcGeSurface* pAcGeSurface;
				brFace.getSurface(pAcGeSurface);
				
				// ת���ɱ߽������
				AcGeExternalBoundedSurface* ebSurf = (AcGeExternalBoundedSurface*)pAcGeSurface;
				if (ebSurf != NULL && ebSurf->isCylinder())
				{
					AcGeCylinder* pCylinder = (AcGeCylinder*)ebSurf;
					acutPrintf(TEXT("\nԲ������Ϣ:"));
					acutPrintf(TEXT("\n�뾶:%.2f"), pCylinder->radius());
					AcGePoint3d center = pCylinder->origin();
					acutPrintf(TEXT("\n���ĵ�:(%.2f, %.2f, %.2f)"), center.x, center.y, center.z);
					AcGeVector3d axis = pCylinder->axisOfSymmetry();
					acutPrintf(TEXT("\n��������:(%.2f, %.2f, %.2f)"), axis.x, axis.y, axis.z);
				}
				delete pAcGeSurface;
			}
			returnValue = brepFaceTrav.next();
		}

		pEnt->close();
	}
}

// This is command 'GETBOUNDAYCURVES'
void ZffChap10GetBoundayCurves()
{
	// ��ʾ�û�ѡ��ʵ��
	AcDbEntity *pEnt = NULL;
	AcGePoint3d pickPoint;
	if (CSelectUtil::PromptSelectEntity(TEXT("\nѡ����άʵ��:"), AcDb3dSolid::desc(), pEnt, pickPoint))
	{
		// ����һ��brep����
		AcBrBrep brepEnt;
		brepEnt.set(*pEnt);
		
		// ����һ���ߵı�����
		AcBrBrepEdgeTraverser brepEdgeTrav;
		AcBr::ErrorStatus returnValue = brepEdgeTrav.setBrep(brepEnt);
		if (returnValue == AcBr::eOk) 
		{
			while (!brepEdgeTrav.done() && (returnValue == AcBr::eOk)) 
			{
				// ��ȡ��ǰ�ߵ���Ϣ
				AcBrEdge brEdge;
				brepEdgeTrav.getEdge(brEdge);
				AcGeCurve3d *pGeCurve = NULL;
				if (brEdge.getCurve(pGeCurve) == AcBr::eOk)
				{
					// ����ת��Ϊ��ͨ�ļ�������
					AcGeCurve3d *pNativeCurve = NULL;
					Adesk::Boolean bRet = ((AcGeExternalCurve3d*)pGeCurve)->isNativeCurve(pNativeCurve);

					int numSample = 180;	// �򻯲�����ֱ�ӵȷ�Ϊ180��
					AcGePoint3dArray points;
					pNativeCurve->getSamplePoints(numSample, points);
					if (pNativeCurve->isClosed() && points.length() > 0)
					{
						points.append(points[0]);
					}
					delete pNativeCurve;
					delete pGeCurve;
					
					if (points.length() > 2)
					{
						CPolylineUtil::Add3dPolyline(points);
					}
				}
				
				returnValue = brepEdgeTrav.next();				
			} 
		}

		pEnt->close();
	}
}

