// UseReactor.cpp : Initialization functions
#include "StdAfx.h"
#include "StdArx.h"
#include "resource.h"
#include <afxdllx.h>
#include "MyEditorReactor.h"
#include "MyDocReactor.h"
#include "MyDbReactor.h"
#include "MyObjectReactor.h"

HINSTANCE _hdllInstance =NULL ;
CMyEditorReactor *g_editorReactor = NULL;		// �༭����Ӧ��
CMyDocReactor *g_docReactor = NULL;		// �ĵ���Ӧ��
CMyDbReactor *g_dbReactor = NULL;		// ͼ�����ݿⷴӦ��

// This command registers an ARX command.
void AddCommand(const TCHAR* cmdGroup, const TCHAR* cmdInt, const TCHAR* cmdLoc,
				const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal = -1);


// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_MSG
void InitApplication();
void UnloadApplication();
//}}AFX_ARX_MSG

// NOTE: DO NOT edit the following lines.
//{{AFX_ARX_ADDIN_FUNCS
//}}AFX_ARX_ADDIN_FUNCS


////////////////////////////////////////////////////////////////////////////
//
// Define the sole extension module object.
AC_IMPLEMENT_EXTENSION_MODULE(UseReactorDLL);

// Now you can use the CAcModuleResourceOverride class in
// your application to switch to the correct resource instance.
// Please see the ObjectARX Documentation for more details

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point
extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
        _hdllInstance = hInstance;
		// Extension DLL one time initialization
		UseReactorDLL.AttachInstance(hInstance);
		InitAcUiDLL();
	} else if (dwReason == DLL_PROCESS_DETACH) {
		// Terminate the library before destructors are called
		UseReactorDLL.DetachInstance();

	}
	return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// ObjectARX EntryPoint
extern "C" AcRx::AppRetCode 
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg) {
	case AcRx::kInitAppMsg:
		// Comment out the following line if your
		// application should be locked into memory
		acrxDynamicLinker->unlockApplication(pkt);
		acrxDynamicLinker->registerAppMDIAware(pkt);
		InitApplication();
		break;
	case AcRx::kUnloadAppMsg:
		UnloadApplication();
		break;
	}
	return AcRx::kRetOK;
}

// Init this application. Register your
// commands, reactors...
void InitApplication()
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_INIT
	AddCommand(TEXT("ZFFCHAP13"), TEXT("SETLINKENTS"), TEXT("SETLINKENTS"), ACRX_CMD_MODAL, ZffChap13SetLinkEnts);
	//}}AFX_ARX_INIT
	
	// �Զ�����ĳ�ʼ��
	CMyObjectReactor::rxInit();
	acrxBuildClassHierarchy();

	// ��ӱ༭����Ӧ��
	g_editorReactor = new CMyEditorReactor();
	acedEditor->addReactor(g_editorReactor);

	// ����ĵ���Ӧ��
	g_docReactor = new CMyDocReactor();
	acDocManager->addReactor(g_docReactor);

	// ���ͼ�����ݿⷴӦ��
	g_dbReactor = new CMyDbReactor();

	// ���Ѿ����ڵ�ͼ�����ݿ���ӷ�Ӧ��
	AcApDocumentIterator *it = acDocManager->newAcApDocumentIterator();
	for (; !it->done(); it->step())
	{
		it->document()->database()->addReactor(g_dbReactor);
	}	
	delete it;
}

// Unload this application. Unregister all objects
// registered in InitApplication.
void UnloadApplication()
{
	// NOTE: DO NOT edit the following lines.
	//{{AFX_ARX_EXIT
	acedRegCmds->removeGroup(TEXT("ZFFCHAP13"));
	//}}AFX_ARX_EXIT

	// ɾ���༭����Ӧ��
	acedEditor->removeReactor(g_editorReactor);
	delete g_editorReactor;

	// ɾ���ĵ���Ӧ��
	acDocManager->removeReactor(g_docReactor);
	delete g_docReactor;

	// ��ObjectARX���ϵ��ɾ���Զ���ʵ��
	deleteAcRxClass(CMyObjectReactor::desc());	
}

// This functions registers an ARX command.
// It can be used to read the localized command name
// from a string table stored in the resources.
void AddCommand(const TCHAR* cmdGroup, const TCHAR* cmdInt, const TCHAR* cmdLoc,
				const int cmdFlags, const AcRxFunctionPtr cmdProc, const int idLocal)
{
	TCHAR cmdLocRes[65];

	// If idLocal is not -1, it's treated as an ID for
	// a string stored in the resources.
	if (idLocal != -1) {

		// Load strings from the string table and register the command.
		::LoadString(_hdllInstance, idLocal, cmdLocRes, 64);
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLocRes, cmdFlags, cmdProc);

	} else
		// idLocal is -1, so the 'hard coded'
		// localized function name is used.
		acedRegCmds->addCommand(cmdGroup, cmdInt, cmdLoc, cmdFlags, cmdProc);
}
