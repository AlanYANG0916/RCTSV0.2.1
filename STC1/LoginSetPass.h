#pragma once
#include "winsock.h"//��Ϊ���ݿ���ͨ���������ӵģ���������������ͷ�ļ�
#include "mysql.h"//���ûʲô��˵�ģ�mysqlͷ�ļ���ȻҪ����
#include <vector>
#include <string>

// LoginSetPass �Ի���

class LoginSetPass : public CDialogEx
{
	DECLARE_DYNAMIC(LoginSetPass)

public:
	LoginSetPass(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LoginSetPass();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CString m_username;
	CString m_password;
	MYSQL m_sqlCon;
	MYSQL_RES* m_res;         //��ѯ�ķ��ؽ����
	MYSQL_ROW m_row;          //��ȡÿһ����¼
public:
	afx_msg void OnBnClickedOk();
	CString NEWPass, NEWPass2;

	afx_msg void OnBnClickedCancel();
};
