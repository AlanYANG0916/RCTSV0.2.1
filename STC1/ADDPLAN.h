#pragma once
#include "afxwin.h"
#include "mysql.h"//���ûʲô��˵�ģ�mysqlͷ�ļ���ȻҪ����
#include "afxcmn.h"


#define COLOR_DEFAULT 0 //Ĭ����ɫ
#define COLOR_RED 1
  //��ɫ
#define COLOR_BLUE 2
  //��ɫ

// ADDPLAN �Ի���

class ADDPLAN : public CDialogEx
{
	DECLARE_DYNAMIC(ADDPLAN)

public:
	ADDPLAN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ADDPLAN();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDPLANDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	BOOL OnInitDialog();
public:

	MYSQL m_sqlCon;
	MYSQL_RES* m_res;         //��ѯ�ķ��ؽ����
	MYSQL_ROW m_row;          //��ȡÿһ����¼

	CString str;
public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnStnClickedFindidstatic();

	BOOL PatientExist=false;
	BOOL PlanExist = false;
	CString ID;
	afx_msg void OnEnKillfocusIdedit();
	afx_msg void OnEnKillfocusPlannameedit();
	CString PlanName;
	CListCtrl ADDBeamList;

	CString BeamName[20];
	CString BeamAngle[20];
	int BeamNum;
	CString m_beamname;
	CString m_beamangle;

	
	CEdit m_edit;
	bool editing=false;
	bool isedit=false;//�޸ļƻ��ı�ʶ��
	int p_edit;
	afx_msg void OnNMCustomdrawCommlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickCommlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit5();

	int m_Row=0; 
	int m_Col=0;
	afx_msg void OnNMDblclkCommlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnSetfocusEdit5();
	CComboBox m_plancombo;
	afx_msg void OnStnClickedFindnamestatic();
};
