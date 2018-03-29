#include "stdafx.h"
#include "PC2PC.h"


PC2PC::PC2PC(void)
{
	memset(&FromPCBuf,0,sizeof(DataFromPC));		//����������˶�ƽ̨������,���ô洢�ռ�ToDOFBuf����ʼ��Ϊ0���洢�ռ�Ĵ�СΪDataToDOF��size
	memset(&ToPCBuf,0,sizeof(DataToPC));	//������˶�ƽ̨��ȡ��������

}


PC2PC::~PC2PC(void)
{
}

bool PC2PC::Init(HWND hWnd)
{
	if (!AfxSocketInit())
		return false;
	//tcp������
	//if(!Create(PC_SERVER_PORT,SOCK_STREAM,FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE))	//SOCK_DGRAM:UDP,SOCK_STREAM :TCP
		//return false;

	//Listen();
	//udp������
	if(!Create(PC_SERVER_PORT,SOCK_DGRAM,FD_READ|FD_WRITE))	//SOCK_DGRAM:UDP,SOCK_STREAM :TCP
		return false;

	
	return true;
}


bool PC2PC::SendData(DataToPC  *pData)//��������
{	
	if(pData != NULL && pData->nCheckID == ToPCBuf.nCheckID && IsConnected())
	{
		//if(bEmergence && pData->nCmd != S_CMD_Back2MID)//������ڽ���״̬����Ŀǰλ�ò����м�̬���򷵻ش���
			//return false;

		if(pData != &ToPCBuf)
		memcpy(&ToPCBuf,pData,sizeof(DataToPC));

		//nSendCount++;
		int ii=0;
		
			
		ii=	SendTo(pData,sizeof(DataToPC),PC_CLIENT_PORT,PC_TARGET_IP) ;//szIP,MSG_DONTROUTE�����øó�Ա����sendto�����ݷ��͵��ض�Ŀ�꣬���ɹ������򷵻�1

		return ii>0;
	}

	return false;
}

void PC2PC::OnReceive(int nErrorCode)//��������
{
	DataFromPC dh;
	int a=-1,b=-1;
	a=ReceiveFrom(&dh,sizeof(DataFromPC),CString(szIP),nPort,0);

	if((b=ReceiveFrom(&dh,sizeof(DataFromPC),CString(szIP),nPort,MSG_DONTROUTE))==int(-1))
	//if(a!=0)
	{
		nRecvCount++;
		ProcData(&dh);
	}
    a=0;
}

void PC2PC::ProcData(DataFromPC *pData)
{
	//if(pData == NULL || pData->nCheckID != ToDOFBuf.nCheckID)
		//return;

	if(pData != &FromPCBuf)
		memcpy(&FromPCBuf,pData,sizeof(DataFromPC));//���ƻ�����֮����ֽڡ�
}

bool PC2PC::IsConnected()
{
	return m_hSocket != SOCKET_ERROR;
}
