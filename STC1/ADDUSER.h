#pragma once
#include "winsock.h"//��Ϊ���ݿ���ͨ���������ӵģ���������������ͷ�ļ�
#include "mysql.h"//���ûʲô��˵�ģ�mysqlͷ�ļ���ȻҪ����
#include <vector>
#include <string>

// ADDUSER �Ի���

class ADDUSER : public CDialogEx
{
	DECLARE_DYNAMIC(ADDUSER)

public:
	ADDUSER(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ADDUSER();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDUSERDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocususernameedit();

	MYSQL m_sqlCon;
	MYSQL_RES* m_res;         //��ѯ�ķ��ؽ����
	MYSQL_ROW m_row;          //��ȡÿһ����¼
	MYSQL_ROW m_row2;

	CString str;
	CString m_username;
	CString m_password;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedCancel2();
};
