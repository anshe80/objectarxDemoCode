//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include <dbpl.h>
#include <gecomp2d.h>

// ���ݶ���߹�����ά��������
static bool PolyToGeCurve(AcDbPolyline *pPline, AcGeCurve2d *&pGeCurve)
{	
	int nSegs;							// ����ߵĶ���
	AcGeLineSeg2d line, *pLine;			// �������ߵ�ֱ�߶β���
	AcGeCircArc2d arc, *pArc;			// �������ߵ�Բ������
	AcGeVoidPointerArray geCurves;		// ָ����ɼ������߸��ֶε�ָ������										
	
	nSegs = pPline->numVerts() - 1;
	
	// ���ݶ���ߴ�����Ӧ�ķֶμ�������
	for (int i = 0; i < nSegs; i++)
	{
		if (pPline->segType(i) == AcDbPolyline::kLine)
		{
			pPline->getLineSegAt(i, line);
			pLine = new AcGeLineSeg2d(line);
			geCurves.append(pLine);
		}
		else if (pPline->segType(i) == AcDbPolyline::kArc)
		{
			pPline->getArcSegAt(i, arc);
			pArc = new AcGeCircArc2d(arc);
			geCurves.append(pArc);
		}		
	}
	
	// ����պ϶�������һ����Բ�������
	if (pPline->isClosed() && pPline->segType(nSegs) == AcDbPolyline::kArc)
	{
		pPline->getArcSegAt(nSegs, arc);
		pArc = new AcGeCircArc2d(arc);
		pArc->setAngles(arc.startAng(), arc.endAng() - 
			(arc.endAng() - arc.startAng()) / 100);
		geCurves.append(pArc);
	}
	
	// ���ݷֶεļ������ߴ�����Ӧ�ĸ�������
	if (geCurves.length() == 1)
	{
		pGeCurve = (AcGeCurve2d *)geCurves[0];
	}
	else
	{
		pGeCurve = new AcGeCompositeCurve2d(geCurves);
	}	
	
	// �ͷŶ�̬������ڴ�
	if (geCurves.length() > 1)		
	{
		for (i = 0; i < geCurves.length(); i++)
		{
			delete geCurves[i];
		}
	}
	
	return true;
}

// ����ָ����һ��㴴��һ���������������
struct resbuf* BuildRbFromPtArray(const AcGePoint2dArray &arrPoints)
{
	struct resbuf *retRb = NULL;
	int count = arrPoints.length();
	if (count <= 1)
	{
		acedAlert(TEXT("����BuildBbFromPtArray��, ���������Ԫ�ظ�������!"));
		return retRb;
	}
	
	// ʹ�õ�һ������������������������ͷ�ڵ�
	ads_point adsPt;
	adsPt[X] = arrPoints[0].x;
	adsPt[Y] = arrPoints[0].y;
	retRb = acutBuildList(RTPOINT, adsPt, RTNONE);
	
	struct resbuf *nextRb = retRb;		// ����ָ��
	
	for (int i = 1; i < count; i++)			// ע�⣺�����ǵ�һ��Ԫ�أ����i��1��ʼ
	{
		adsPt[X] = arrPoints[i].x;
		adsPt[Y] = arrPoints[i].y;
		// ��̬�����µĽڵ㣬���������ӵ�ԭ��������β��
		nextRb->rbnext = acutBuildList(RTPOINT, adsPt, RTNONE);		
		nextRb = nextRb->rbnext;		
	}	
	
	return retRb;
}

// ѡ��λ�ڶ�����ڲ�������ʵ��
static bool SelectEntInPoly(AcDbPolyline *pPline, 
								   AcDbObjectIdArray &ObjectIdArray, const char *selectMode, double approxEps)
{
	// �ж�selectMode����Ч��
	if (_tcscmp(selectMode, TEXT("CP")) != 0 && _tcscmp(selectMode, TEXT("WP")) != 0)
	{
		acedAlert(TEXT("����SelectEntInPline��, ָ������Ч��ѡ��ģʽ!"));
		return false;
	}	
	
	// ������������е�ObjectId
	for (int i = 0; i < ObjectIdArray.length(); i++)
	{
		ObjectIdArray.removeAt(i);
	}
	
	AcGeCurve2d *pGeCurve;						// ����߶�Ӧ�ļ�������
	Adesk::Boolean bClosed = pPline->isClosed();	// ������Ƿ�պ�
	if (bClosed != Adesk::kTrue)				// ȷ���������Ϊѡ��߽�ʱ�Ǳպϵ�
	{
		pPline->setClosed(!bClosed);
	}	
	
	// ������Ӧ�ļ���������
	PolyToGeCurve(pPline, pGeCurve);
	
	// ��ü������ߵ�������
	AcGePoint2dArray SamplePtArray;				// �洢���ߵ�������
	AcGeDoubleArray ParamArray;					// �洢�������Ӧ�Ĳ���ֵ
	AcGePoint2d ptStart, ptEnd;			// �������ߵ������յ�
	Adesk::Boolean bRet = pGeCurve->hasStartPoint(ptStart);
	bRet = pGeCurve->hasEndPoint(ptEnd);
	double valueSt = pGeCurve->paramOf(ptStart);
	double valueEn = pGeCurve->paramOf(ptEnd);
	pGeCurve->getSamplePoints(valueSt, valueEn, approxEps, 
		SamplePtArray, ParamArray);
	
	delete pGeCurve;		// �ں���PolyToGeCurve�з������ڴ�	
	
	// ȷ��������������յ㲻�غ�
	AcGeTol tol;
	tol.setEqualPoint(0.01);
	AcGePoint2d ptFirst = SamplePtArray[0];
	AcGePoint2d ptLast = SamplePtArray[SamplePtArray.length() - 1];	
	if (ptFirst.isEqualTo(ptLast))
	{
		SamplePtArray.removeLast();
	}	
	
	// ���������㴴���������������
	struct resbuf *rb;
	rb = BuildRbFromPtArray(SamplePtArray);	
	
	// ʹ��acedSSGet��������ѡ��
	ads_name ssName;			// ѡ������
	int rt = acedSSGet(selectMode, rb, NULL, NULL, ssName);	
	if (rt != RTNORM)
	{		
		acutRelRb(rb);			// �ͷŽ������������
		return false;
	}
	
	// ��ѡ�������еĶ�����ӵ�ObjectIdArray
	long length;	
	acedSSLength(ssName, &length);
	for (i = 0; i < length; i++)
	{
		// ���ָ��Ԫ�ص�ObjectId
		ads_name ent;
		acedSSName(ssName, i, ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId, ent);
		
		// ���ָ��ǰԪ�ص�ָ��
		AcDbEntity *pEnt;
		Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
		
		// ѡ����Ϊ�߽�Ķ�����ˣ�ֱ�������ô�ѭ��
		if (es == Acad::eWasOpenForWrite)		
		{
			continue;
		}
		
		ObjectIdArray.append(pEnt->objectId());
		
		pEnt->close();
	}	
	
	// �ͷ��ڴ�
	acutRelRb(rb);								// �ͷŽ������������
	acedSSFree(ssName);							// ɾ��ѡ��
	
	return true;
}

// This is command 'CREATESSET'
void ZffChap5CreateSSet()
{
	ads_name sset;	// ѡ������
	// ѡ��ͼ�����ݿ������е�ʵ��
	acedSSGet(TEXT("A"), NULL, NULL, NULL, sset);
	long length;
	acedSSLength(sset, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	// ������������
	
	acedSSFree(sset);
}

// This is command 'TEST'
void ZffChap5Test()
{
	struct resbuf *rb;	// �������������	
	ads_name ssname;	
	
	rb = acutBuildList(RTDXF0, TEXT("LINE"),		// ʵ������
		8, TEXT("0"),							// ͼ��
		RTNONE);
	
	// ѡ��ͼ����λ��0���ϵ�����ֱ��
	acedSSGet(TEXT("X"), NULL, NULL, rb, ssname); 
	
	acutRelRb(rb);	
	acedSSFree(ssname);
}

// This is command 'SELECTENT'
void ZffChap5SelectEnt()
{
	ads_point pt1, pt2, pt3, pt4; 
	struct resbuf *pointlist;		// �������������
	ads_name ssname;				// ѡ�񼯵�ͼԪ��
	pt1[X] = pt1[Y] = pt1[Z] = 0.0; 
	pt2[X] = pt2[Y] = 5.0; pt2[Z] = 0.0; 
	
	// ����Ѿ�ѡ����ʵ�壬�ͻ�õ�ǰ��PICKFIRSTѡ��
	// ������ʾ�û�ѡ��ʵ��
	acedSSGet(NULL, NULL, NULL, NULL, ssname); 
	
	// ������ڣ��ͻ�õ�ǰ��PickFirstѡ�� 
	acedSSGet(TEXT("I"), NULL, NULL, NULL, ssname); 
	
	// ѡ�����������ѡ�� 
	acedSSGet(TEXT("P"), NULL, NULL, NULL, ssname); 
	
	// ѡ�����һ�δ����Ŀɼ�ʵ��
	acedSSGet(TEXT("L"), NULL, NULL, NULL, ssname); 
	
	// ѡ��ͨ����(5,5)������ʵ��
	acedSSGet(NULL, pt2, NULL, NULL, ssname); 
	
	// ѡ��λ�ڽǵ�(0,0)��(5,5)��ɵĴ��������е�ʵ��
	acedSSGet(TEXT("W"), pt1, pt2, NULL, ssname); 
	
	// ѡ��ָ���Ķ���ΰ�Χ������ʵ�� 
	pt3[X] = 10.0; pt3[Y] = 5.0; pt3[Z] = 0.0; 
	pt4[X] = 5.0; pt4[Y] = pt4[Z] = 0.0; 
	pointlist = acutBuildList(RTPOINT, pt1, RTPOINT, pt2, 
		RTPOINT, pt3, RTPOINT, pt4, 0); 
	acedSSGet(TEXT("WP"), pointlist, NULL, NULL, ssname); 
	
	// ѡ����ǵ�(0,0)��(5,5)��ɵ������ཻ������ʵ�� 
	acedSSGet(TEXT("C"), pt1, pt2, NULL, ssname); 
	
	// ѡ����ָ������������ཻ������ʵ�� 
	acedSSGet(TEXT("CP"), pointlist, NULL, NULL, ssname); 
	acutRelRb(pointlist); 
	
	// ѡ����ѡ�����ཻ�����ж���
	pt4[Y] = 15.0; pt4[Z] = 0.0; 
	pointlist = acutBuildList(RTPOINT, pt1, RTPOINT, pt2, 
		RTPOINT, pt3, RTPOINT, pt4, 0); 
	acedSSGet(TEXT("F"), pointlist, NULL, NULL, ssname); 
	
	acutRelRb(pointlist);
	acedSSFree(ssname);
}

// This is command 'SELECTINPOLY'
void ZffChap5SelectInPoly()
{
	// ��ʾ�û�ѡ������
	ads_name entName;
	ads_point pt;
	if (acedEntSel(TEXT("\nѡ������:"), entName, pt) != RTNORM)
		return;
	
	AcDbObjectId entId;
	acdbGetObjectId(entId, entName);
	
	// �ж�ѡ���ʵ���Ƿ��Ƕ����
	AcDbEntity *pEnt;
	acdbOpenObject(pEnt, entId, AcDb::kForWrite);
	if (pEnt->isKindOf(AcDbPolyline::desc()))
	{
		AcDbPolyline *pPoly = AcDbPolyline::cast(pEnt);
		
		AcDbObjectIdArray ObjectIdArray;	// ѡ�񵽵�ʵ��ID����
		SelectEntInPoly(pPoly, ObjectIdArray, "CP", 1);
		acutPrintf(TEXT("\nѡ��%d��ʵ��."), ObjectIdArray.length());
	}
	
	pEnt->close();
}

// This is command 'FILTER1'
void ZffChap5Filter1()
{
	struct resbuf *rb;	// �������������	
	ads_name ssname;	
	
	rb = acutBuildList(RTDXF0, TEXT("TEXT"),		// ʵ������
		8, TEXT("0,ͼ��1"),							// ͼ��
		1, TEXT("*cadhelp*"),						// �������ַ���
		RTNONE);
	
	// ѡ�񸴺�Ҫ�������
	acedSSGet(TEXT("X"), NULL, NULL, rb, ssname); 
	long length;
	acedSSLength(ssname, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	acutRelRb(rb);	
	acedSSFree(ssname);
}

// This is command 'FILTER2'
void ZffChap5Filter2()
{
	struct resbuf *rb;	// �������������	
	ads_name ssname;	
	
	rb = acutBuildList(-4, TEXT("<OR"),		// �߼��������ʼ
		RTDXF0, TEXT("TEXT"),					// һ������
		RTDXF0, TEXT("MTEXT"),				// ��һ������
		-4, TEXT("OR>"),						// �߼����������
		RTNONE);
	
	// ѡ�񸴺�Ҫ�������
	acedSSGet(TEXT("A"), NULL, NULL, rb, ssname); 
	long length;
	acedSSLength(ssname, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	acutRelRb(rb);	
	acedSSFree(ssname);
}

// This is command 'FILTER3'
void ZffChap5Filter3()
{
	struct resbuf *rb;	// �������������	
	ads_name ssname;	
	
	rb = acutBuildList(RTDXF0, TEXT("CIRCLE"),	// ʵ������
		-4, TEXT(">="),							// ��ϵ�����
		40, 30,								// �뾶
		RTNONE);
	
	// ѡ�񸴺�Ҫ���ʵ��
	acedSSGet(TEXT("A"), NULL, NULL, rb, ssname); 
	long length;
	acedSSLength(ssname, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	acutRelRb(rb);	
	acedSSFree(ssname);
}

// This is command 'FILTER4'
void ZffChap5Filter4()
{
	struct resbuf *rb;	// �������������	
	ads_name ssname;	
	
	ads_point pt1, pt2;
	pt1[X] = pt1[Y] = pt1[Z] = 0;
	pt2[X] = pt2[Y] = 100;
	pt2[Z] = 0;
	
	rb = acutBuildList(RTDXF0, TEXT("CIRCLE"),	// ʵ������
		-4, TEXT(">,>,*"),						// ��ϵ�������ͨ���
		10, pt1,							// Բ��
		-4, TEXT("<,<,*"),						// ��ϵ�������ͨ���
		10, pt2,							// Բ��
		RTNONE);
	
	// ѡ�񸴺�Ҫ���ʵ��
	acedSSGet(TEXT("A"), NULL, NULL, rb, ssname); 
	long length;
	acedSSLength(ssname, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	acutRelRb(rb);	
	acedSSFree(ssname);
}

// This is command 'FILTER5'
void ZffChap5Filter5()
{
	struct resbuf *rb;	// �������������	
	ads_name ssname;	
	
	ads_point pt1, pt2;
	pt1[X] = pt1[Y] = pt1[Z] = 0;
	pt2[X] = pt2[Y] = 100;
	pt2[Z] = 0;
	
// 	rb = acutBuildList(1001, TEXT("XData"),	// ��չ���ݵ�Ӧ�ó�����		
// 		RTNONE);

	rb = acutBuildList(1000, TEXT("Road"),	// ��չ�����е�ASCII�ַ���	
		RTNONE);
	
	// ѡ�񸴺�Ҫ���ʵ��
	acedSSGet(TEXT("A"), NULL, NULL, rb, ssname); 
	long length;
	acedSSLength(ssname, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	acutRelRb(rb);	
	acedSSFree(ssname);
}

// This is command 'TEST2'
void ZffChap5Test2()
{
	ads_name ssname;	
	ads_point pt1, pt2;
	pt1[X] = pt1[Y] = pt1[Z] = 0;
	pt2[X] = pt2[Y] = 100;
	pt2[Z] = 0;	
	
	// ѡ��ͼ������pt1��pt2��ɵĴ����ཻ�����ж���
	acedSSGet(TEXT("C"), pt1, pt2, NULL, ssname); 
	long length;
	acedSSLength(ssname, &length);
	acutPrintf(TEXT("\nʵ����:%d"), length);
	
	acedSSFree(ssname);
}

// This is command 'ENTINFO'
void ZffChap5EntInfo()
{
	acDocManager->sendStringToExecute(acDocManager->curDocument(), 
		TEXT("(entget(car(entsel))) "));	// �ַ�����ĩβ����һ�����ַ�
}

