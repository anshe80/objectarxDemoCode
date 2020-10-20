//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\SymbolTable\LayerUtil.h"
#include <dbsymtb.h>
#include <vector>
#include <acutmem.h>
#include "..\..\Common\Others\ConvertUtil.h"
#include "..\..\Common\Others\StringUtil.h"
#include "..\..\Common\IO\TextFileUtil.h"
#include <IO.H>

// This is command 'NEWLAYER'
void ZffChap4NewLayer()
{
	// ��ʾ�û�����ͼ������
	TCHAR layerName[100];
	if (acedGetString(Adesk::kFalse, TEXT("\n����ͼ�������:"), layerName) != RTNORM)
	{
		return;
	}

	// �½�ͼ��
	AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
	if (layerId.isValid())
	{
		acutPrintf(TEXT("\nͼ�����Ѿ�����ͬ����ͼ��."));
	}
	else
	{
		CLayerUtil::Add(layerName);
		acutPrintf(TEXT("\n�ɹ�����ͼ��."));
	}
}

// This is command 'LAYERCOLOR'
void ZffCHAP4LayerColor()
{
	// ��ʾ�û�����ͼ������
	TCHAR layerName[100];
	if (acedGetString(Adesk::kFalse, TEXT("\n����ͼ�������:"), layerName) != RTNORM)
	{
		return;
	}
	
	// �½�ͼ��
	AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
	if (layerId.isValid())
	{
		// ��ȡͼ�㵱ǰ����ɫ
		AcCmColor oldColor;
		AcDbLayerTableRecord *pLayerTblRcd = NULL;
		if (acdbOpenObject(pLayerTblRcd, layerId, AcDb::kForRead) == Acad::eOk)
		{
			oldColor = pLayerTblRcd->color();
			pLayerTblRcd->close();
		}
		int nCurColor = oldColor.colorIndex();		// ͼ���޸�ǰ����ɫ
		int nNewColor = oldColor.colorIndex();		// �û�ѡ�����ɫ

		// ����"��ɫ"�Ի���
		if (acedSetColorDialog(nNewColor, Adesk::kFalse, nCurColor))
		{
			CLayerUtil::SetColor(layerName, nNewColor);
		}
	}
	else
	{
		acutPrintf(TEXT("\nͼ���в�����ָ�����Ƶ�ͼ��."));
	}
}

// This is command 'DELETELAYER'
void ZffCHAP4DeleteLayer()
{
	// ��ʾ�û�����ͼ������
	TCHAR layerName[100];
	if (acedGetString(Adesk::kFalse, TEXT("\n����ͼ�������:"), layerName) != RTNORM)
	{
		return;
	}
	
	AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
	if (layerId.isValid())
	{
		// ɾ��ͼ��
		AcDbLayerTableRecord *pLayerTblRcd = NULL;
		if (acdbOpenObject(pLayerTblRcd, layerId, AcDb::kForWrite) == Acad::eOk)
		{
			pLayerTblRcd->erase();
			pLayerTblRcd->close();
		}
	}
	else
	{
		acutPrintf(TEXT("\nͼ���в�����ָ�����Ƶ�ͼ��."));
	}
}

// This is command 'EXPORTLAYER'
void ZffCHAP4ExportLayer()
{
	// ���ͼ���б�
	AcDbObjectIdArray layerIds;
	CLayerUtil::GetLayerList(layerIds);

	// ����ͼ���б�������Ҫд�뵽�ı��ļ����ַ�������
	std::vector<CString> lines;
	for (int i = 0; i < layerIds.length(); i++)
	{
		AcDbLayerTableRecord *pLayerTblRcd = NULL;
		if (acdbOpenObject(pLayerTblRcd, layerIds[i], AcDb::kForRead) == Acad::eOk)
		{
			// ���ͼ�����Ϣ
			std::vector<CString> layerInfos;
			TCHAR *szLayerName;		// ͼ������
			pLayerTblRcd->getName(szLayerName);
			layerInfos.push_back(szLayerName);
			acutDelString(szLayerName);			
											
			AcCmColor color = pLayerTblRcd->color();	// ͼ����ɫ	
			layerInfos.push_back(CConvertUtil::ToString(color.colorIndex()));
								
			AcDbLinetypeTableRecord *pLinetypeTblRcd = NULL;	// ͼ������
			acdbOpenObject(pLinetypeTblRcd, pLayerTblRcd->linetypeObjectId(), AcDb::kForRead);
			TCHAR *szLinetypeName;
			pLinetypeTblRcd->getName(szLinetypeName);
			pLinetypeTblRcd->close();
			layerInfos.push_back(szLinetypeName);
			acutDelString(szLinetypeName);
								
			AcDb::LineWeight lineWeight = pLayerTblRcd->lineWeight();	// ͼ����߿�
			int nVal = (int)lineWeight;		// ö��ת��Ϊ����
			layerInfos.push_back(CConvertUtil::ToString(nVal));

			CString strLayerInfo = CStringUtil::Join(layerInfos, TEXT(","));		// Ҫ�����ͼ�������Ϣ
			lines.push_back(strLayerInfo);

			pLayerTblRcd->close();
		}
	}

	// д���ı��ļ�
	CString fileName = TEXT("C:\\layers.txt");
	CTextFileUtil::Save(fileName, lines);
}

// This is command 'IMPORTLAYER'
void ZffCHAP4ImportLayer()
{
	// ��ȡ�ļ��е�����
	CString fileName = TEXT("C:\\layers.txt");
	if (_taccess(fileName, 0) != -1)
	{		
		std::vector<CString> lines;
		CTextFileUtil::Load(fileName, lines);

		// ����ÿһ���ַ���������ǰͼ���е�ͼ��
		for (int i = 0; i < lines.size(); i++)
		{
			std::vector<CString> layerInfos;
			CStringUtil::Split(lines[i], TEXT(","), layerInfos, false);
			if (layerInfos.size() == 4)
			{
				CString layerName = layerInfos[0];
				AcDbObjectId layerId = CLayerUtil::GetLayerId(layerName);
				if (layerId.isNull())
				{
					CLayerUtil::Add(layerName);
					layerId = CLayerUtil::GetLayerId(layerName);
				}

				AcDbLayerTableRecord *pLayerTblRcd = NULL;
				if (acdbOpenObject(pLayerTblRcd, layerId, AcDb::kForWrite) == Acad::eOk)
				{
					// ������ɫ
					AcCmColor color;
					Adesk::UInt16 colorIndex = _ttoi(layerInfos[1]);
					color.setColorIndex(colorIndex);
					pLayerTblRcd->setColor(color);
					
					// ��������
					AcDbLinetypeTable *pLinetypeTbl = NULL;
					AcDbObjectId linetypeId;
					acdbHostApplicationServices()->workingDatabase()
						->getLinetypeTable(pLinetypeTbl, AcDb::kForRead);
					if (pLinetypeTbl->has(layerInfos[2]))
					{
						pLinetypeTbl->getAt(layerInfos[2], linetypeId);	
						pLayerTblRcd->setLinetypeObjectId(linetypeId);
					}					
					pLinetypeTbl->close();
					
					// �����߿�
					AcDb::LineWeight lineWeight = (AcDb::LineWeight)_ttoi(layerInfos[3]);		
					pLayerTblRcd->setLineWeight(lineWeight);
					
					pLayerTblRcd->close();
				}				
			}
		}
	}
	else
	{
		acutPrintf(TEXT("\nδ�ҵ�ָ�����ļ�."));
	}
}

