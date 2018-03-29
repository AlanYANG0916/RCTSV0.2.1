// LoginSetPass.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "STC1.h"
#include "LoginSetPass.h"
#include "afxdialogex.h"

#pragma comment(lib,"libmysql.lib")//���������Ҳ�����ڹ�������������

extern CString UserName;

// LoginSetPass �Ի���

IMPLEMENT_DYNAMIC(LoginSetPass, CDialogEx)

LoginSetPass::LoginSetPass(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	m_username = UserName;
	NEWPass = "";
	NEWPass2 = "";
}

LoginSetPass::~LoginSetPass()
{
}

void LoginSetPass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_usernameEDIT, m_username);
	DDV_MaxChars(pDX, m_username, 32);
	//DDX_Text(pDX, IDC_passwordEDIT, m_password);
	//DDV_MaxChars(pDX, m_password, 32);
}


BEGIN_MESSAGE_MAP(LoginSetPass, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoginSetPass::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LoginSetPass::OnBnClickedCancel)
END_MESSAGE_MAP()


// LoginSetPass ��Ϣ�������


void LoginSetPass::OnBnClickedOk()
{
	mysql_init(&m_sqlCon);//��ʼ�����ݿ����
	if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "chair001", 3306, NULL, 0))//localhost:��������ַ������ֱ������IP;root:�˺�;123:����;test:���ݿ���;3306:����˿�  
	{
		AfxMessageBox(_T("���ݿ�����ʧ��!"));
		return;
	}
	else//���ӳɹ�������������ݿ⣬֮�����ز�����������Ƿ����������
	{
		//AfxMessageBox(_T("���ݿ����ӳɹ�!"));
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItemText(IDC_passwordEDIT, NEWPass);
	GetDlgItemText(IDC_passwordEDIT2, NEWPass2);
	if (NEWPass!= NEWPass2)
	{
		AfxMessageBox(_T("Please try again , the two passwords are not consistent.!"));
		return;
	}
	
	str.Format(L"UPDATE user2 SET password=AES_ENCRYPT(\"%s\", 'scuyf130916') where id = %s", NEWPass, m_username);
	const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
	char c1[500] = { 0 };
	wcstombs(c1, wstr, wcslen(wstr));
	const char* cc1 = c1;

	mysql_query(&m_sqlCon, cc1);

	mysql_close(&m_sqlCon);//�ر�Mysql����
	CDialogEx::OnOK();
}


void LoginSetPass::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
