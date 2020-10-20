//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include <dbsol3d.h>
#include <gemat3d.h>
#include <math.h>
#include <dbents.h>
#include "..\..\Common\Entity\RegionUtil.h"
#include <dbregion.h>

// This is command 'ADDBOX'
void ZffChap10AddBox()
{
	AcDb3dSolid *pSolid = new AcDb3dSolid();
	Acad::ErrorStatus es = pSolid->createBox(40, 50, 30);
	if (es != Acad::eOk)
	{
		acedAlert("����������ʧ��!");
		delete pSolid;
		return;
	}
	
	// ʹ�ü��α任�����ƶ�������
	AcGeMatrix3d xform;
	AcGeVector3d vec(100, 100, 100);
	xform.setToTranslation(vec);
	es = pSolid->transformBy(xform);
	
	// ����������ӵ�ģ�Ϳռ�
	CDwgDatabaseUtil::PostToModelSpace(pSolid);
}

// This is command 'ADDCYLINDER'
void ZffChap10AddCylinder()
{
	// �����ض�������Բ���壨ʵ������һ��Բ׶�壩
	AcDb3dSolid *pSolid = new AcDb3dSolid();
	pSolid->createFrustum(30, 10, 10, 0);
	
	// ��Բ׶����ӵ�ģ�Ϳռ�
	CDwgDatabaseUtil::PostToModelSpace(pSolid);
}

// This is command 'ADDSPIRE'
void ZffChap10AddSpire()
{
	// ָ�����������ߵĲ���
	double radius, deltaVertical;	// �뾶��ÿһ���ڴ�ֱ���������
	double number, segment;			// �����ߵ���תȦ�������һȦ�ķֶ���
	
	radius = 30, deltaVertical = 12;
	number = 5, segment = 30;
	
	// �����ĸ����ͽǶȼ��
	int n = number * segment;	// ��ĸ���ʵ������n+1
	double angle = 8 * atan(1) / segment;	// ����֮�����ת�Ƕ�
	
	// ������Ƶ������
	AcGePoint3dArray points;	// ���Ƶ���������
	for (int i = 0; i < n+1; i++)
	{
		AcGePoint3d vertex;
		vertex[X] = radius * cos(8 * i * atan(1) / segment);
		vertex[Y] = radius * sin(8 * i * atan(1) / segment);
		vertex[Z] = i * deltaVertical / segment;
		points.append(vertex);
	}
	
	// ����������·��
	AcDb3dPolyline *p3dPoly = new AcDb3dPolyline(AcDb::k3dSimplePoly, points);
	
	// ��·����ӵ�ģ�Ϳռ�
	AcDbObjectId spireId = CDwgDatabaseUtil::PostToModelSpace(p3dPoly);
	
	// ����һ��Բ��Ϊ����Ľ���
	AcGeVector3d vec(0, 1, 0);			// Բ����ƽ��ķ�ʸ��
	AcGePoint3d ptCenter(30, 0, 0);		// Բ��λ����뾶�Ĵ�С�й�
	AcDbCircle *pCircle = new AcDbCircle(ptCenter, vec, 3);
	AcDbObjectId circleId = CDwgDatabaseUtil::PostToModelSpace(pCircle);
	
	// ����Բ����һ������
	AcDbObjectIdArray boundaryIds, regionIds;
	boundaryIds.append(circleId);
	regionIds = CRegionUtil::Add(boundaryIds);
	
	// ��������������·��
	AcDbRegion *pRegion = NULL;
	if (acdbOpenObject(pRegion, regionIds.at(0), AcDb::kForRead) == Acad::eOk)
	{
		AcDb3dPolyline *pPoly = NULL;
		if (acdbOpenObject(pPoly, spireId, AcDb::kForRead) == Acad::eOk)
		{
			// �����������
			AcDb3dSolid *pSolid = new AcDb3dSolid();
			pSolid->extrudeAlongPath(pRegion, pPoly);
			CDwgDatabaseUtil::PostToModelSpace(pSolid);
			
			pPoly->close();
		}
		
		pRegion->close();
	}
}

// This is command 'REVOLVEENT'
void ZffChap10RevolveEnt()
{
	// ���ö��������
	AcGePoint3d vertex[5];
	vertex[0] = AcGePoint3d(15, 0, 0);
	vertex[1] = AcGePoint3d(45, 0, 0);
	vertex[2] = AcGePoint3d(35, 9, 0);
	vertex[3] = AcGePoint3d(41, 18, 0);
	vertex[4] = AcGePoint3d(15, 18, 0);
	AcGePoint3dArray points;
	for (int i = 0; i <= 4; i++)
	{
		points.append(vertex[i]);
	}
	
	// ������Ϊ��ת����Ķ����
	AcDb3dPolyline *p3dPoly = new AcDb3dPolyline(AcDb::k3dSimplePoly, 
		points, true);
	AcDbObjectId polyId = CDwgDatabaseUtil::PostToModelSpace(p3dPoly);
	
	// ���պϵĶ����ת��������
	AcDbObjectIdArray boundaryIds, regionIds;
	boundaryIds.append(polyId);
	regionIds = CRegionUtil::Add(boundaryIds);
	
	// ������ת����
	AcDbRegion *pRegion = NULL;
	if (acdbOpenObject(pRegion, regionIds.at(0), AcDb::kForRead) == Acad::eOk)
	{
		AcDb3dSolid *pSolid = new AcDb3dSolid();
		pSolid->revolve(pRegion, AcGePoint3d::kOrigin, AcGeVector3d(0, 1, 0), 8 * atan(1));
		CDwgDatabaseUtil::PostToModelSpace(pSolid);
		
		pRegion->close();
	}
}

// This is command 'BOOLEAN'
void ZffChap10Boolean()
{
	// ��������������
	AcDb3dSolid *pSolid1 = new AcDb3dSolid();
	pSolid1->createBox(40, 50, 30);
	AcDb3dSolid *pSolid2 = new AcDb3dSolid();
	pSolid2->createBox(40, 50, 30);
	
	// ʹ�ü��α任�����ƶ�������
	AcGeMatrix3d xform;
	AcGeVector3d vec(20, 25, 15);
	xform.setToTranslation(vec);
	pSolid1->transformBy(xform);
	
	// ����������ӵ�ģ�Ϳռ�
	AcDbObjectId solidId1 = CDwgDatabaseUtil::PostToModelSpace(pSolid1);
	AcDbObjectId solidId2 = CDwgDatabaseUtil::PostToModelSpace(pSolid2);
	
	// ���в������㣬�����µ�ʵ��
	acdbOpenObject(pSolid1, solidId1, AcDb::kForWrite);
	acdbOpenObject(pSolid2, solidId2, AcDb::kForWrite);
	
	Acad::ErrorStatus es = pSolid1->booleanOper(AcDb::kBoolUnite, pSolid2);
	assert(pSolid2->isNull());
	pSolid2->erase();	// �ֹ�����ɾ��
	
	pSolid2->close();	// ɾ��֮������Ҫ�رո�ʵ��
	pSolid1->close();
}

