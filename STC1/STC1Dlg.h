
// STC1Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Cali.h"
#include "Corr.h"
#include "MainUI.h"
//#include "DOFComm.h"

#include "AddPatient.h"
#include "ADDPlan.h"
#include "ADDUSER.h"
enum
{
	NormalMode = 0,
	TreatmentMode = 1,
	QAMode = 2,
};

struct mystruct {
	float vector[4];
};

//�밲׿ͨ�ŵ����ݽṹ
struct DatatoAD {
	BYTE AD1;
	BYTE AD2;
	BYTE AD3;
	BYTE AD4;
	float ADstate[6];
	float AD360;
};

struct DataFromAD {
	BYTE AD1;
	BYTE AD2;
	BYTE AD3;
	BYTE AD4;
	float ADstate[6];
	float AD360;
};


// CSTC1Dlg �Ի���
class CSTC1Dlg : public CDialogEx
{
// ����
public:
	CSTC1Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	HACCEL  m_hAccel;
	virtual BOOL PreTranslateMessage(MSG *pMsg);
// �Ի�������
	enum { IDD = IDD_STC1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnOK();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	CMainUI m_mainui;
	CCali m_cali;
	CCorr m_corr; 
	CDOFComm m_dof;
	CMenu menu;

	Attitude attitude;//����λ��ָ��
	Userdata userdata;
	Onoff onoff;

	Dataexchange datachange;
	//Mode mode;
	Userdi userdi;
	CEdit m_infoedit;
	bool six_enable;//�����ɶ�ƽ̨ʹ�ܱ�ʶ��
	
	bool friststart;//���ο�����־λ
	char S_comMotion;//ѡ�����ָ����˶���ʽ
	float desir_speed[6];
	float desir_acc[6];
	float single_move_speed[6];
	float re_attitude_for_show[6];
	DWORD m_nRecvCount;
    //float single_move_speed_show[6];

	afx_msg void OnBnClickedCtrlbutton();//���ư�ť
	afx_msg void OnBnClickedStartbutton();//������ʱ��
	afx_msg void OnBnClickedResetbutton();//��λ
	afx_msg void OnBnClickedButton6();//��ʼ��
	afx_msg void OnBnClickedButton10();//����
	afx_msg void OnBnClickedButton9();//����
	afx_msg void OnBnClickedEstopbutton();//��ͣ
	afx_msg void timerout();
	afx_msg void GetRealData(float data[6])//��xy�����ƽ��У��
	{
		double x = data[3]; double y = data[4];
		if (x>0.2)
		{
			data[3] = 1.003*x+0.007*y;
		}
		else if (x<-0.2)
		{
			data[3] = 1.004*x+ 0.007*y;
		}
		if (y>0.2)
		{
			data[4] = 1.007*y-0.012*x;
		}
		else if (y<-0.2)
		{
			data[4] = 1.003*y-0.012*x;
		}
	}

	afx_msg void GetXYCORRData(float data[6])//��xy�����ƽ��У��
	{
		float x = data[3]; float y = data[4];

		data[3] = x - 0.007 *y;
		data[4] = y + 0.012*x;
	};

	bool IfMoveGet;
	CFont m_editFont;
	afx_msg void ChangeUI();

	UINT nMMTimer;

	UINT count1;
	CString result, str;
	UINT item;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);//�����л�����

	CString m_hJueDuiWeiZhi2;  //��λ��ǰ�����ݾ���λ��
	CString m_hJueDuiWeiZhi;  //����λ��

	bool lockflag;			//����������־	
	bool enableflag;		//ʹ�ܱ�־
	bool EthLinkOK;			//��̫�����ӱ�־
	bool EMSTOP;//����ͣ����־

	bool CanUse;
	
	//bool EStop;
	short Ret;				//PLC���ݽ���״̬��ʶλ	
	CString	Data[8];		//��PLC���ݴ洢��
	double fangxiang;          //��ת�����־λ
	bool LR;          //��ת�����־λ�����ʾ˳ʱ�룬�ٱ�ʾ��ʱ��



	afx_msg void RealAgleRealTime();
	afx_msg CString ToHdata(CString);
	afx_msg CString ToLdata(CString);
	CButton m_enbale_button;
	afx_msg void OnBnClickedEnbutton();
	afx_msg void beginmove();
	afx_msg void OnBnClickedLockbutton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ʹ�ܰ�ť���Ʊ���
	CColorButton m_Startcontrol;
	CColorButton m_Encontrol;
	CColorButton m_Lockcontrol;
	CColorButton m_ESTOPcontrol;


	afx_msg void CtrlSpeed();
	float maxtime ;//һ�θ����˶���Ҫ���ѵ����ʱ��
	float speed[6];//�����ٶȵ���ֵ�����Դ���˶���ǰ������ֵ


	afx_msg void JudgeAction();
	afx_msg void ActionTip();
	afx_msg void OnBnClickedLogoutbutton();
	afx_msg void OnBnClickedPdfbutton();
	afx_msg void On32775();
	afx_msg void OnSavereport();
	afx_msg void OnSoftwareinfo();

	//char* CString2char(CString str);
	const char* CString2char(CString str)
	{
		const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
		char c1[256] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* fname = c1;
		return c1;
	}
	afx_msg void OnMenuExit();
	afx_msg void OnStnClickedEnStatic();
	afx_msg void OnEditResetpassword();
	afx_msg void OnBnClickedButton13();
	MYSQL m_sqlCon;
	MYSQL_RES* m_res;         //��ѯ�ķ��ؽ����
	MYSQL_ROW m_row;          //��ȡÿһ����¼
	afx_msg void OnBnClickedButton2();
	afx_msg void OnInputpatientAddpatient();
	afx_msg void OnInputpatientEditpatient();
	afx_msg void OnInputplanAddplan();
	afx_msg void OnInputplanEditplan();
	afx_msg void OnUserAdduser();
	afx_msg void OnBnClickedmovebeginbutton();
};
