#pragma once


// QAIntPutDlg �Ի���

class QAIntPutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QAIntPutDlg)

public:
	QAIntPutDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~QAIntPutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	int TTSorFIX;
	afx_msg void OnBnClickedCancel();
};
