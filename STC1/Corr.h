#pragma once


// CCorr �Ի���

class CCorr : public CDialogEx
{
	DECLARE_DYNAMIC(CCorr)

public:
	CCorr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCorr();

// �Ի�������
	enum { IDD = IDD_CORRDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg void ReadConfig();
	double FLimitInTTS[6];
	double ULimitInTTS[6];
	double StepOn[6];
	double RefPoint[3];
	double CORR[3];

public:
	afx_msg void OnBnClickedChangemotionrangebutton();
	void OnOK();

	int m_radiogroup;
};
