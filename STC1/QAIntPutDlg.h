#pragma once


// QAIntPutDlg 对话框

class QAIntPutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QAIntPutDlg)

public:
	QAIntPutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~QAIntPutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	int TTSorFIX;
	afx_msg void OnBnClickedCancel();
};
