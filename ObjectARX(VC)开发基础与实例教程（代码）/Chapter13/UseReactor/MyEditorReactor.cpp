// MyEditorReactor.cpp: implementation of the CMyEditorReactor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyEditorReactor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyEditorReactor::CMyEditorReactor()
{

}

CMyEditorReactor::~CMyEditorReactor()
{

}

void CMyEditorReactor::commandEnded( const TCHAR* cmdStr )
{
	acutPrintf(TEXT("\n%s����ִ�н�����."), cmdStr);
}

void CMyEditorReactor::commandWillStart( const char* cmdStr )
{
	acutPrintf(TEXT("\n%s���Ҫ��ʼִ��."), cmdStr);
}
