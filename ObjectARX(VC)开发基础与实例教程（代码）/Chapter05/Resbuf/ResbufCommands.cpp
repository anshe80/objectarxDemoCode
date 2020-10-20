//
// ObjectARX defined commands

#include "StdAfx.h"
#include "StdArx.h"

// This is command 'ADDCIRCLE1'
void ZffChap5AddCircle1()
{
	// ����ADS����
	ads_point ptCenter = {0, 0, 0};		// Բ��
	ads_real radius = 10;				// �뾶
	
	// ����acedCommand��������Բ
	acedCommand(RTSTR, TEXT("Circle"),		// ����
		RTPOINT, ptCenter,	// Բ��
		RTREAL, radius,		// �뾶
		RTNONE);			// ��������
}

// This is command 'ADDCIRCLE2'
void ZffChap5AddCircle2()
{
	struct resbuf *rb;	// ���������
	int rc = RTNORM;	// ����ֵ
	
	// �����������������
	ads_point ptCenter = {30, 0, 0};
	ads_real radius = 10;
	rb = acutBuildList(RTSTR, TEXT("Circle"), 
		RTPOINT, ptCenter, 
		RTREAL, radius, 
		RTNONE);
	
	// ����Բ
	if (rb != NULL)
	{
		rc = acedCmd(rb);
	}
	
	// ���鷵��ֵ
	if (rc != RTNORM)
	{
		acutPrintf(TEXT("\n����Բʧ��!"));
	}
	
	acutRelRb(rb);
	
	// ��������
	acedCommand(RTSTR, TEXT("Zoom"), RTSTR, TEXT("E"), RTNONE);
}

// This is command 'ENTINFO'
void ZffChap5EntInfo()
{
	// ��ʾ�û�ѡ��ʵ��
	ads_name entName;
	ads_point pt;
	if (acedEntSel(TEXT("\nѡ��ʵ��:"), entName, pt) != RTNORM)
		return;
	
	struct resbuf *rbEnt;	// ����ʵ�����ݵĽ��������
	struct resbuf *rb;		// ���ڱ���rbEnt�Ľ��������
	
	// ��entName��ñ���ʵ�����ݵĽ��������
	rbEnt = acdbEntGet(entName);
	rb = rbEnt;
	
	while (rb != NULL)
	{
		switch (rb->restype) 
		{
		case -1:	// ͼԪ��
			acutPrintf(TEXT("\nͼԪ��: %x"), rb->resval.rstring);
			break;
		case 0:		// ͼԪ����
			acutPrintf(TEXT("\nͼԪ����: %s"), rb->resval.rstring);
			break;
		case 8:		// ͼ��
			acutPrintf(TEXT("\nͼ��:%s"), rb->resval.rstring);
			break;
		case 10:	// Բ��
			acutPrintf(TEXT("\nԲ��:(%.2f, %.2f, %.2f)"), 
				rb->resval.rpoint[X], 
				rb->resval.rpoint[Y], 
				rb->resval.rpoint[Z]);
			break;
		case 40:	// �뾶
			acutPrintf(TEXT("\n�뾶:%.4f"), rb->resval.rreal);
			break;
		case 210:	// Բ����ƽ��ķ���ʸ��
			acutPrintf(TEXT("\nƽ��ķ���ʸ��:(%.2f, %.2f, %.2f)"), 
				rb->resval.rpoint[X], 
				rb->resval.rpoint[Y], 
				rb->resval.rpoint[Z]);
			break;
		default:
			break;
		}	// switch
		
		rb = rb->rbnext;		// �л�����һ���ڵ�
	}	// while
	
	if (rbEnt != NULL)
	{
		acutRelRb(rbEnt);
	}
}

