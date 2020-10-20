//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\Document\DwgDatabaseUtil.h"
#include <dbents.h>
#include "..\..\Common\IO\AppDirectoryUtil.h"
#include "..\..\Common\Entity\LineUtil.h"
#include "..\..\Common\Entity\circleutil.h"

// ����Excel����
// �����ڵ��ó����InitApplication�����г�ʼ��COM��̬���ӿ⣺::CoInitialize(NULL);
// ͬʱ��UnloadApplication�����йر�COM���֧�֣�::CoUninitialize();
static bool RunExcelApp(_Application &excelApp, bool bVisible = true, bool bUserControl = true);

// This is command 'SAVEFILE'
void ZffChap12SaveFile()
{
	// ��ȡֱ�ߡ�Բ�μ���
	AcDbObjectIdArray allEntIds = CDwgDatabaseUtil::GetAllEntityIds();
	AcDbObjectIdArray lineIds, circleIds;
	for (int i = 0; i < allEntIds.length(); i++)
	{
		AcDbEntity *pEnt = NULL;
		if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk)
		{
			if (pEnt->isKindOf(AcDbLine::desc()))
			{
				lineIds.append(pEnt->objectId());
			}
			else if (pEnt->isKindOf(AcDbCircle::desc()))
			{
				circleIds.append(pEnt->objectId());
			}

			pEnt->close();
		}
	}

	// д�������ļ�
	CString fileName = CAppDirectoryUtil::GetCurrentDirectory() + TEXT("\\�������ļ�.bnf");
	FILE *pFile = _tfopen(fileName, TEXT("wb"));
	if (pFile)
	{
		// �ļ����ͣ��ļ�����ʶ���ַ�����4���ֽڵ��ַ���
		char szFileDescription[4] = "bnf";
		fwrite(szFileDescription, sizeof(char), 4, pFile);

		// �ļ��汾
		int version = 1;		// ��ǰ�ǵ�1��
		fwrite(&version, sizeof(int), 1, pFile);

		// ֱ������
		int lineCount = lineIds.length();
		fwrite(&lineCount, sizeof(int), 1, pFile);
		for (int i = 0; i < lineCount; i++)
		{
			AcDbLine *pLine = NULL;
			if (acdbOpenObject(pLine, lineIds[i], AcDb::kForRead) == Acad::eOk)
			{
				AcGePoint3d startPoint = pLine->startPoint();
				AcGePoint3d endPoint = pLine->endPoint();
				double stp[3];
				for (int j = 0; j < 3; j++)
				{
					stp[j] = startPoint[j];
				}
				fwrite(stp, sizeof(double), 3, pFile);
				double enp[3];
				for (j = 0; j < 3; j++)
				{
					enp[j] = endPoint[j];
				}
				fwrite(enp, sizeof(double), 3, pFile);

				pLine->close();
			}
		}

		// Բ����
		int circleCount = circleIds.length();
		fwrite(&circleCount, sizeof(int), 1, pFile);
		for (i = 0; i < circleCount; i++)
		{
			AcDbCircle *pCircle = NULL;
			if (acdbOpenObject(pCircle, circleIds[i], AcDb::kForRead) == Acad::eOk)
			{
				AcGePoint3d centerPoint = pCircle->center();
				double radius = pCircle->radius();
				double cep[3];
				for (int j = 0; j < 3; j++)
				{
					cep[j] = centerPoint[j];
				}
				fwrite(cep, sizeof(double), 3, pFile);
				
				fwrite(&radius, sizeof(double), 1, pFile);
				
				pCircle->close();
			}
		}

		// �ر��ļ�
		fclose(pFile);
	}
}

// This is command 'LOADFILE'
void ZffCHAP12LoadFile()
{
	// ���������ļ�
	CString fileName = CAppDirectoryUtil::GetCurrentDirectory() + TEXT("\\�������ļ�.bnf");
	FILE *pFile = _tfopen(fileName, TEXT("rb"));
	if (pFile)
	{
		// �ļ����ͣ��ļ�����ʶ���ַ�����4���ֽڵ��ַ���
		char szFileDescription[4] = {0};
		fread(szFileDescription, sizeof(char), 4, pFile);
		if (strcmp(szFileDescription, "bnf") != 0)
		{
			AfxMessageBox(TEXT("������Ч��BNF�ļ�!"));
			fclose(pFile);
			return;
		}
		
		// �ļ��汾
		int version = 1;		// ��ǰ�ǵ�1��
		fread(&version, sizeof(int), 1, pFile);
		
		// ֱ������
		int lineCount = 0;
		fread(&lineCount, sizeof(int), 1, pFile);
		for (int i = 0; i < lineCount; i++)
		{
			AcGePoint3d startPoint;
			double stp[3];
			fread(stp, sizeof(double), 3, pFile);
			for (int j = 0; j < 3; j++)
			{
				startPoint[j] = stp[j];
			}

			AcGePoint3d endPoint;
			double enp[3];
			fread(enp, sizeof(double), 3, pFile);
			for (j = 0; j < 3; j++)
			{
				endPoint[j] = enp[j];
			}

			CLineUtil::Add(startPoint, endPoint);
		}
		
		// Բ����
		int circleCount = 0;
		fread(&circleCount, sizeof(int), 1, pFile);
		for (i = 0; i < circleCount; i++)
		{
			AcGePoint3d centerPoint;
			double cep[3];
			fread(cep, sizeof(double), 3, pFile);
			for (int j = 0; j < 3; j++)
			{
				centerPoint[j] = cep[j];
			}

			double radius = 0;			
			fread(&radius, sizeof(double), 1, pFile);
			
			CCircleUtil::Add(centerPoint, radius);
		}

		// �ر��ļ�
		fclose(pFile);
	}
}

public static List<Catalog> GetChildCatalogs(string number)
{
	try
	{
		List<Catalog> catalogs = new List<Catalog>();
		
		// ���ݿ�����
		OleDbConnection con = new OleDbConnection(DatabaseUtil.GetSysConnectionString());
		using (con)
		{
			con.Open();
			
			string sql = "SELECT * FROM [Catalog] WHERE Len([Number]) = " + (number.Length + 3) +
				" And [Number] Like '" + number + "%'";
			
			// ִ�����ݿ��ѯ����
			OleDbCommand cmd = new OleDbCommand(sql, con);
			using (OleDbDataReader dr = cmd.ExecuteReader())
			{
				while (dr.Read())
				{
					Catalog c = new Catalog();
					c.name = dr["Name"].ToString();
					c.number = dr["Number"].ToString();
					c.catalogId = Convert.ToInt64(dr["CatalogId"]);
					catalogs.Add(c);
				}
			}
			
			return catalogs;
		}
	}
	catch (Exception ex)
	{
		MessageBox.Show(ex.Message, "����", MessageBoxButtons.OK, MessageBoxIcon.Error);
		return null;
	}
}