//////////////////////////////////////////////////////////////////////////
// Hello.cpp
// by �ŷ�
//////////////////////////////////////////////////////////////////////////
// ����ͷ�ļ�
#include <aced.h>
#include <rxregsvc.h>

// ������ʼ��������ж�غ���
void InitApp();
void UnloadApp();

// ���������ִ�к���
void HelloWorld();

// ����Ӧ�ó���ʱ�����õĺ���
void InitApp()
{
	//ʹ��AutoCAD�������ע��һ��������
	acedRegCmds->addCommand(TEXT("Hello1"),
		TEXT("Hello"),	//�����������������ƾ�������
		TEXT("Hello"),	//AutoCAD�����иó���
		ACRX_CMD_MODAL,
		HelloWorld);
}

// ж��Ӧ�ó���ʱ�����õĺ���
void UnloadApp()
{
	//ɾ��������
	acedRegCmds->removeGroup(TEXT("Hello1"));
}

// ʵ��Hello����ĺ���
void HelloWorld()
{
	acutPrintf(TEXT("\nHello, World!"));
}

// ��ڵ㺯��
extern "C" AcRx::AppRetCode 
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg) 
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		InitApp();
		break;
	case AcRx::kUnloadAppMsg:
		UnloadApp();
		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}
