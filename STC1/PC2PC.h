#pragma once
#include "afxsock.h"

const UINT PC_SERVER_PORT = 1000;
const UINT PC_CLIENT_PORT = 2000;
#define PC_TARGET_IP L"192.168.0.131"

struct DataToPC
{
	BYTE nCheckID;
	BYTE nDOFStatus;
	BYTE nRev0;
	BYTE nRev1;
	
	float attitude[6];		//ʵ����̬
	float para[6];				//�������
	float motor_code[6];		//�����ֵ
	float pcctrl[6];//���ػ���ָ��

	int IsCtrl2Pc;
};

struct DataFromPC
{
	BYTE nCheckID;			//��ʶλ
	BYTE nCmd;				//״ָ̬��
	BYTE nAct;				//��Чָ��
	BYTE nReserved;	//����

	float DOFs[6];			//{��ҡ�����㣬����ǰ�򣬲�������}
	float Vxyz[3];			//{ǰ�򣬲�������}������Ϊ��������Ϊ�����ٶȣ�
	float Axyz[3];			//...�����ٶȣ�
};



class PC2PC :public CAsyncSocket
{
public:
	PC2PC(void);
	~PC2PC(void);

	wchar_t szIP[40];
	UINT nPort;

	bool Init(HWND hWnd);
	bool SendData(DataToPC  *pData);			
	void ProcData(DataFromPC *pData);
	bool IsConnected();
	virtual void OnReceive(int nErrorCode);

	
public:
	DataFromPC	FromPCBuf;				//����������˶�ƽ̨������(�ӹ���վ������վ)
	DataToPC	ToPCBuf;		//������˶�ƽ̨��ȡ��������(�ӹ���վ������վ)
	DWORD nSendCount;
	DWORD nRecvCount;
};

