// DOFComm.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DOFComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CDOFComm::CDOFComm(BYTE nDofID,LPCWSTR sz,WORD port)//ʵ����ͨ�Ŷ���
{
	bEmergence = false;	//�Ƿ����״̬

	memset(&ToDOFBuf,0,sizeof(DataToDOF));		//����������˶�ƽ̨������,���ô洢�ռ�ToDOFBuf����ʼ��Ϊ0���洢�ռ�Ĵ�СΪDataToDOF��size
	memset(&FromDOFBuf,0,sizeof(DataToHost));	//������˶�ƽ̨��ȡ��������


	ToDOFBuf.nCheckID = nDofID;
	FromDOFBuf.nDOFStatus = dof_stop;

	wcscpy_s(szIP,40,sz);//wcscpy_s��ϵͳ����
	nPort = port;

	nSendCount = 0;
	nRecvCount = 0;
}

CDOFComm::~CDOFComm()
{
}

// CDOFComm ��Ա����
bool CDOFComm::Init(HWND hWnd)//����UDP�㲥������
{
	if (!AfxSocketInit())
		return false;

	if(!Create(SERVER_PORT,SOCK_DGRAM,FD_READ|FD_WRITE))	//SOCK_DGRAM:UDP,SOCK_STREAM :TCP
		return false;
	
	return true;
}

bool CDOFComm::Clean()
{	
	Close();
	return true;
}

bool CDOFComm::IsConnected()//�жϵ�ǰ�Ƿ�ͨ������
{
	return m_hSocket != SOCKET_ERROR;
}

BYTE CDOFComm::Status()	//��ȡ״̬
{
	if(!IsConnected())
		return dof_closed;

	if(bEmergence)
		return dof_emg;

    if(nRecvCount==0) FromDOFBuf.nDOFStatus=8;
	return FromDOFBuf.nDOFStatus;
}

bool CDOFComm::SendData(DataToDOF  *pData)//��������
{	
	if(pData != NULL && pData->nCheckID == ToDOFBuf.nCheckID && IsConnected())
	{
		if(bEmergence && pData->nCmd != S_CMD_Back2MID)//������ڽ���״̬����Ŀǰλ�ò����м�̬���򷵻ش���
			return false;

		if(pData != &ToDOFBuf)
		memcpy(&ToDOFBuf,pData,sizeof(DataToDOF));

		nSendCount++;
		int ii=0;
		
		ii=	SendTo(pData,sizeof(DataToDOF),CLIENT_PORT,TARGET_IP) ;//szIP,MSG_DONTROUTE�����øó�Ա����sendto�����ݷ��͵��ض�Ŀ�꣬���ɹ������򷵻�1

		return ii>0;
	}

	return false;
}

void CDOFComm::OnReceive(int nErrorCode)//��������
{
	DataToHost dh;
	int a=-1,b=-1;
	a=ReceiveFrom(&dh,sizeof(DataToHost),CString(szIP),nPort,0);

	if((b=ReceiveFrom(&dh,sizeof(DataToHost),CString(szIP),nPort,MSG_DONTROUTE))==int(-1))
	//if(a!=0)
	{
		nRecvCount++;
		ProcData(&dh);
	}
    a=0;
}

void CDOFComm::ProcData(DataToHost *pData)
{
	//if(pData == NULL || pData->nCheckID != ToDOFBuf.nCheckID)
		//return;

	if(pData != &FromDOFBuf)
		memcpy(&FromDOFBuf,pData,sizeof(DataToHost));//���ƻ�����֮����ֽڡ�
}

void CDOFComm::CheckDOF()
{
	ToDOFBuf.nCmd = S_CMD_Check;	

	SendData(&ToDOFBuf);
}

void CDOFComm::ToWorking()//�л�������״̬
{
	if(Status() != dof_neutral)	//ֻ��������λ�ò����л�������״̬
		return;

	ToDOFBuf.nCmd = S_CMD_RUN;
    for (int i=0;i<6;i++)
    {
		ToDOFBuf.DOFs[i]=0.f;
    }
    for (int i=0;i<3;i++)
    {
		ToDOFBuf.Vxyz[i]=0.f;	
		ToDOFBuf.Axyz[i]=0.f;
    }
	SendData(&ToDOFBuf);
}

void CDOFComm::ToNeutral()//�л�������״̬
{
	ToDOFBuf.nCmd = S_CMD_Back2MID;

	SendData(&ToDOFBuf);
}

bool CDOFComm::Emergence()//��ͣ
{
	ToDOFBuf.nCmd = S_CMD_ToMerg;

	bEmergence = true;

	SendData(&ToDOFBuf);

	return true;
}

bool CDOFComm::UnEmergence()//�����ͣ״̬
{
	if(!bEmergence)
		return true;
	
	bEmergence = false;

	ToWorking();

	return true;
}