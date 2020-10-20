//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <gearc2d.h>
#include <geline2d.h>
#include <gelnsg2d.h>
#include <dbents.h>
#include "..\..\Common\Others\ConvertUtil.h"
#include "..\..\Common\Interaction\SelectUtil.h"
#include <dbpl.h>
#include "..\..\Common\Document\DwgDatabaseUtil.h"

// This is command 'INTERSECTWITH'
void ZffChap8IntersectWith()
{
	// ������Ҫ���㽻��ļ��������
	AcGeCircArc2d geArc(AcGePoint2d::kOrigin, 50, 0, 6);
	AcGeLine2d geLine(AcGePoint2d::kOrigin, AcGePoint2d(10, 10));
	
	// ���㲢�������
	AcGePoint2d point1, point2;
	int num;
	if (geArc.intersectWith(geLine, num, point1, point2))
	{
		acutPrintf(TEXT("\nֱ�ߺ�Բ����%d������."), num);
		if (num > 0)
		{
			acutPrintf(TEXT("\n����1����: (%.4f, %.4f)."), point1.x, point1.y);
		}
		if (num > 1)
		{
			acutPrintf(TEXT("\n����2����: (%.4f, %.4f)."), point2.x, point2.y);
		}
	}
}

// ����ֱ��DB�����ü��������
static AcGeLineSeg2d GetGeLineObj(AcDbObjectId lineId)
{
	AcGeLineSeg2d geLine;
	AcDbLine *pLine = NULL;
	if (acdbOpenObject(pLine, lineId, AcDb::kForRead) == Acad::eOk)
	{
		geLine.set(CConvertUtil::ToPoint2d(pLine->startPoint()), CConvertUtil::ToPoint2d(pLine->endPoint()));
		pLine->close();
	}
	
	return geLine;
}

// This is command 'LINEDISTANCE'
void ZffChap8LineDistance()
{
	// ��ʾ�û�ѡ����Ҫ������������ֱ��
	AcDbObjectIdArray lineIds;
	if (CSelectUtil::PromptSelectEnts(TEXT("\nѡ������ֱ��:"), AcDbLine::desc(), lineIds))
	{
		if (lineIds.length() != 2)
		{
			acutPrintf(TEXT("\n����ѡ������ֱ��."));
			return;
		}

		// ��ֱ��ת���ɶ�Ӧ�ļ��������
		AcGeLineSeg2d geLine1 = GetGeLineObj(lineIds[0]);
		AcGeLineSeg2d geLine2 = GetGeLineObj(lineIds[1]);
		
		// ���㲢�������֮�����̾���
		double distance = geLine1.distanceTo(geLine2);
		acutPrintf("\n����ֱ��֮�����̾���Ϊ: %.4f.", distance);
	}	
}

// �����������֮�������
void GetCurveBetweenIntPoints(AcDbPolyline *pPoly, const AcGePoint3dArray &intPoints)
{
	AcDbVoidPtrArray curves;
	pPoly->getSplitCurves(intPoints, curves);
	for (int i = 0; i < curves.length(); i++)
	{
		AcDbCurve *pCurve = static_cast<AcDbCurve*>(curves[i]);		
		
		// ɾ����β�������ߣ������ε�������ӵ�ģ�Ϳռ�
		if (i == 0 || i == curves.length() - 1)
		{
			delete pCurve;
		}
		else
		{
			AcDbObjectId curveId = CDwgDatabaseUtil::PostToModelSpace(pCurve);
		}
	}
}

// This is command 'CURVEBOOLEAN'
void ZffChap8CurveBoolean()
{
	// ѡ����Ҫ���������������
	AcDbObjectIdArray polyIds;
	if (CSelectUtil::PromptSelectEnts(TEXT("\nѡ�����������:"), AcDbPolyline::desc(), polyIds))
	{
		if (polyIds.length() != 2)
		{
			acutPrintf(TEXT("\n����ѡ�����������."));
			return;
		}
		
		// �����������ߵĽ���
		bool bOk = false;
		AcDbPolyline *pPoly1 = NULL, *pPoly2 = NULL;
		if (acdbOpenObject(pPoly1, polyIds[0], AcDb::kForWrite) == Acad::eOk)
		{
			if (acdbOpenObject(pPoly2, polyIds[1], AcDb::kForWrite) == Acad::eOk)
			{
				AcGePoint3dArray intPoints;
				pPoly1->intersectWith(pPoly2, AcDb::kOnBothOperands, intPoints);
				if (intPoints.length() >= 2)
				{
					bOk = true;
				}
				else
				{
					acutPrintf("\n�����֮�佻������2��, �޷����м���.");
				}
				
				// ���ݽ���Ͳ���ֵ��ý���֮�������
				if (bOk)
				{
					GetCurveBetweenIntPoints(pPoly1, intPoints);
					GetCurveBetweenIntPoints(pPoly2, intPoints);

					pPoly2->erase();
				}

				pPoly2->close();
			}

			if (bOk)
			{
				pPoly1->erase();
			}

			pPoly1->close();
		}
	}
}

