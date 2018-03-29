// DOFComm.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <deque> ///////////////////////////////////////////////////////////////////////////

const UINT SERVER_PORT = 10000;
const UINT CLIENT_PORT = 5000;
#define TARGET_IP L"192.168.0.125"
// const UINT CLIENT_PORT = 5500;		//������λ����
// #define TARGET_IP L"192.168.0.131"	//������λ����
#define MIN_PAR			0.000001f
#define ToDeg(rad)		(rad * 180.0 / PI)
#define ToRad(deg)		(deg / 180.0 * PI)
#define PI                    3.1416f
#define TIMER             10
#define MAXSPEED		30.0f
#define MAXANGV		10.f
#define att_send_flag	1.f
#define ljg	                0.f       //���ݲɼ�



struct Userdi
{
	BYTE pass_di_1;
	BYTE pass_di_2;
};

struct Mode
{
	int m_nVarIdx;
	int  flag_count;		//��ʶ�����ֹ�����ʽ
	int  m_nAct;
};

struct Onoff
{
	char n_solution;	//�Ƿ�����
	char data_save;		//�Ƿ񱣴�����
	char set_button;	//��ʶ��ť�Ƿ���Ч
	char back2mid;		//�Ƿ����λ
};

struct Sinmovetime
{
	float max;
	float test;
};
struct Userdata
{
	float send_do[6];		//��������ؼ�����
	float m_para[18];		//������̬�˶��ؼ�����
	float eRR[6];			//���������ʾ
	float m_force[6];		//ʵ�ʸ���
	float m_eDOF[6];		//ʵ����̬
	BYTE ca_send_do[6];
	BYTE total_send_do;
};

enum ComMotion
{
	ComMotion_1=1,//�����˶�һ
	ComMotion_2=2,//�����˶���
	ComMotion_3=3,//�����˶���
	ComMotion_4=4 //�����˶���
};

/////////////////////////////////////////////////
typedef struct PlatAttPara
{
	float attu0;   //��̬һ
	float attu1;   //��̬��
	float attu2;   //��̬��
	float attu3;   //��̬��
	float attu4;   //��̬��
	float attu5;   //��̬��
}PlatAttParaStruct;

typedef std::deque<PlatAttParaStruct>PlatAttParaDeque;
/////////////////////////////////////////////////

struct Dataexchange
{
	double eX;
	double eY;
	double eZ;
	double eH;
	double eP;
	double eR;
	
	double eAx;
	double eAy;
	double eAz;
	double eVx;
	double eVy;
	double eVz;
};

struct Attitude
{
	float end[6];		//�����˶��У���һ����̬�л�����һ����̬ʱ�ĵ�ǰ��̬
	float mid[6];		//�����˶��У���һ����̬�л�����һ����̬ʱ��Ŀ����̬
	float sen[6];		//�����˶��е�ָ����̬

	float iec[6];
};


struct DataToHost
{
	BYTE nCheckID;
	BYTE nDOFStatus;
	BYTE nRev0;
	BYTE nRev1;
	
	float attitude[6];		//
	float para[6];				//�������
	float motor_code[6];		//�����ֵ,//[0,1,2]������̬��[3,4,5]ʵ����̬
};

//�˶�ƽ̨״̬����
enum
{
	dof_stop		= 0,		//ֹͣ״̬
	dof_sys_moving	= 1,		//�˶�������״̬
	dof_neutral		= 2,		//����״̬
	dof_working		= 3,		//����״̬
	dof_setconf		= 3,		//�޸Ĳ���

	dof_select=8,

	dof_check_id = 55,

	dof_closed = 253,
	dof_emg = 254,
	dof_err = 255
};

struct DataToDOF
{
	BYTE nCheckID;			//��ʶλ
	BYTE nCmd;				//״ָ̬��
	BYTE nAct;				//��Чָ��
	BYTE nReserved;	//����

	float DOFs[6];			//{��ҡ�����㣬����ǰ�򣬲�������}
	float Vxyz[3];			//{ǰ�򣬲�������}������Ϊ��������Ϊ�����ٶȣ�
	float Axyz[3];			//...�����ٶȣ�
};

enum COUNT_FLAG
{
	F_None=0,  
    F_SinMove=1,		//������������
	F_SinMotion=2,		//���������˶�	
	F_ComMotion=3,		//�����˶�һ       
	F_DataMotion=4,		//�ļ��˶�       
	F_Jog=5,		//�����ֶ� 
};

//ƽ̨����״ָ̬�
enum M_nCmd
{
	S_CMD_RUN=0,			//��������
	S_CMD_Check=1,
	S_CMD_Back2MID=2,		// �׻�����λ
	S_CMD_ToMerg=3,			//����ͣ��
	S_CMD_ToWork=4,			// ����Э��
	S_CMD_JOG=5,			//�����ֶ�
	S_CMD_Work=6,			//�ɵ�λ��������λ
	S_CMD_Stop=7,			//����λ���䵽��λ
	S_CMD_ChaConf=8,		//������������Ϣ
	S_CMD_HOM=9,         
};

enum
{
	S_OFF = 0,
	S_ON  = 1
};

enum
{
	MOTOR_NONE = 0,
	MOTOR_LXM23 = 1,
	MOTOR_LXM32 = 2
};
//���⶯������


class CDOFComm : public CAsyncSocket
{
public:
	CDOFComm(BYTE nDofID = 55,LPCWSTR szIPs = TARGET_IP,WORD nPort = CLIENT_PORT);
	virtual ~CDOFComm();
	bool Init(HWND hWnd);				//��ʼ��������������
	bool Clean();								
	bool IsConnected();
	BYTE Status();								//��ȡ״̬
	wchar_t szIP[40];
	UINT nPort;

public:
	bool SendData(DataToDOF  *pData);			
	void ProcData(DataToHost *pData);
	void CheckDOF();							//���ͼ������
	void ToNeutral();							//������λ
	void ToWorking();
	bool Emergence();
	bool UnEmergence();	
	virtual void OnReceive(int nErrorCode);
protected:
	bool bEmergence;							//�Ƿ����״̬
public:
	DataToDOF	ToDOFBuf;				//����������˶�ƽ̨������
	DataToHost	FromDOFBuf;			//������˶�ƽ̨��ȡ��������
	DWORD nSendCount;
	DWORD nRecvCount;


	static double DOFdata;
};


