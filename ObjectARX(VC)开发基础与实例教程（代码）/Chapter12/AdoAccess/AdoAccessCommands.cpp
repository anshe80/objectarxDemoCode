//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"
#include "..\..\Common\IO\AppDirectoryUtil.h"
#include "..\..\Common\Database\DbConnection.h"
#include "..\..\Common\Database\DbCommand.h"
#include "..\..\Common\Database\DbDataReader.h"

// ������ݿ������ַ���
static CString GetConnectionString()
{
	CString mdbFileName = CAppDirectoryUtil::GetParentDirectory() + TEXT("\\Book.mdb");
	return TEXT("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + mdbFileName;		
}

// This is command 'ADDBOOK'
void ZffChap12AddBook()
{
	// �����ݿ�����
	CDbConnection con(GetConnectionString());
	con.Open();

	// �����ѯ����
	CString sql;
	sql.Format(TEXT("Insert Into Book(Name, Author, Press, Price) Values('%s', '%s', '%s', %f)"), 
		TEXT("AutoCAD 2002�����̳�"), TEXT("���� �ŷ� ������"), TEXT("����ϣ�����ӳ�����"), 32.0);
	CDbCommand cmd(sql, &con);
	int rows = cmd.ExecuteNonQuery();

	CDbCommand cmd2(TEXT("Select @@Identity"), &con);
	int bookId = cmd2.ExecuteScalar().ToInteger();
	acutPrintf(TEXT("\n�½���Book���¼����:%d"), bookId);

	// �ر����ݿ�����
	con.Close();
}

// This is command 'EDITBOOK'
void ZffChap12EditBook()
{
	// �����ݿ�����
	CDbConnection con(GetConnectionString());
	con.Open();
	
	// �����ѯ����
	CString sql = TEXT("Update Book Set Name = @Name, Author = @Author, Press = @Press, Price = @Price Where BookID = @BookID");
	CDbCommand cmd(sql, &con);
	cmd.AddCharParameter(TEXT("@Name"), TEXT("����ͼ��"));
	cmd.AddCharParameter(TEXT("@Author"), TEXT("��������"));
	cmd.AddCharParameter(TEXT("@Press"), TEXT("���Գ�����"));
	cmd.AddDoubleParameter(TEXT("@Price"), 10.0);
	cmd.AddLongParameter(TEXT("@BookID"), 4);
	int rows = cmd.ExecuteNonQuery();
	if (rows > 0)
	{
		acutPrintf(TEXT("\n�޸ļ�¼�ɹ�."));
	}
	
	// �ر����ݿ�����
	con.Close();
}

// This is command 'DELETEBOOK'
void ZffCHAP12DeleteBook()
{
	// �����ݿ�����
	CDbConnection con(GetConnectionString());
	con.Open();
	
	// �����ѯ����
	CString sql = TEXT("Delete From Book Where BookID = @BookID");
	CDbCommand cmd(sql, &con);
	cmd.AddLongParameter(TEXT("@BookID"), 4);
	int rows = cmd.ExecuteNonQuery();
	if (rows > 0)
	{
		acutPrintf(TEXT("\nɾ����¼�ɹ�."));
	}
	
	// �ر����ݿ�����
	con.Close();
}

// This is command 'QUERYBOOK'
void ZffCHAP12QueryBook()
{
	// �����ݿ�����
	CDbConnection con(GetConnectionString());
	con.Open();
	
	// �����ѯ����
	CString sql = TEXT("Select * From Book Where Price > 20.0");
	CDbCommand cmd(sql, &con);
	CDbDataReader *dr = cmd.ExecuteReader();
	while (dr->Read())
	{
		long bookId = (*dr)[TEXT("BookID")].ToLong();
		CString name = (*dr)[TEXT("Name")].ToString();
		CString author = (*dr)[TEXT("Author")].ToString();
		CString press = (*dr)[TEXT("Press")].ToString();
		double price = (*dr)[TEXT("Price")].ToDouble();
		acutPrintf(TEXT("\nBookId:%d, Name:%s, Author:%s, Press:%s, Price:%.2f"), bookId, 
			name, author, press, price);
	}
	dr->Close();
	
	// �ر����ݿ�����
	con.Close();
}

