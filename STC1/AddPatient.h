#pragma once
#include "afxwin.h"
#include "mysql.h"//���ûʲô��˵�ģ�mysqlͷ�ļ���ȻҪ����

// AddPatient �Ի���

class AddPatient : public CDialogEx
{
	DECLARE_DYNAMIC(AddPatient)

public:
	AddPatient(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AddPatient();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddPatientDIALOG };
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

	afx_msg void OnBnClickedOk();
	CComboBox patientgender;
	CString ID;
	CString Name;
	CString Birth;

	bool PatientExist=false;
	CString Gender;

	bool edit=false;
	bool hadedit = false;
	afx_msg void OnEnKillfocuspatientidedit();
	afx_msg void OnBnClickedsearchbutton();
};
