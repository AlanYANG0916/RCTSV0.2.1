#pragma once
#include "winsock.h"//��Ϊ���ݿ���ͨ���������ӵģ���������������ͷ�ļ�
#include "mysql.h"//���ûʲô��˵�ģ�mysqlͷ�ļ���ȻҪ����
#include <vector>
#include <string>


// CLogin �Ի���

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	MYSQL m_sqlCon;
	MYSQL_RES* m_res;         //��ѯ�ķ��ؽ����
	MYSQL_ROW m_row;          //��ȡÿһ����¼
	MYSQL_ROW m_row2;

	afx_msg void OnBnClickedOk();
	CString str;
	CString m_username;
	CString m_password;
	CString m_real_password;
	bool NameExist;
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL SelectDB();
	afx_msg void GetDataFromDB();

	//vector <int> m_data[100];  //������ݿ��¼�����Ϊ100��
	afx_msg void OnEnSetfocuspasswordedit();
	int keyid;
	afx_msg void OnEnSetfocususernameedit();

	bool CanClose = true;
	bool showWaring= false;
	bool ChangePass=false;
	bool moveingorlogin;//0login,1move

	BOOL OnInitDialog();
	CFont m_editFont;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
