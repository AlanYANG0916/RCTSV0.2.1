// ADDUSER.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "STC1.h"
#include "ADDUSER.h"
#include "afxdialogex.h"

#pragma comment(lib,"libmysql.lib")//���������Ҳ�����ڹ�������������
// ADDUSER �Ի���

IMPLEMENT_DYNAMIC(ADDUSER, CDialogEx)

ADDUSER::ADDUSER(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDUSERDIALOG, pParent)
{

}

ADDUSER::~ADDUSER()
{
}

void ADDUSER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ADDUSER, CDialogEx)
	ON_EN_KILLFOCUS(IDC_usernameEDIT, &ADDUSER::OnEnKillfocususernameedit)
	ON_BN_CLICKED(IDOK2, &ADDUSER::OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL2, &ADDUSER::OnBnClickedCancel2)
END_MESSAGE_MAP()


// ADDUSER ��Ϣ�������


void ADDUSER::OnEnKillfocususernameedit()
{
	bool NameExist=false;
	GetDlgItemText(IDC_usernameEDIT, m_username);
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

	//UpdateData(TRUE);
	mysql_query(&m_sqlCon, "SELECT id FROM user2");//��user2�������ѯid
	//��ȡ�����
	m_res = mysql_store_result(&m_sqlCon);

	while (m_row = mysql_fetch_row(m_res))
	{
		str = m_row[0];
		if (m_username == str)
		{
			NameExist = true;
		}
	}
	if (NameExist)
	{
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
		((CStatic*)GetDlgItem(IDC_STATIC111))->SetIcon(m_hIcon);
	}
	if (!NameExist)
	{
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
		((CStatic*)GetDlgItem(IDC_STATIC111))->SetIcon(m_hIcon);
	}
}


void ADDUSER::OnBnClickedOk2()
{
	CString NEWPass, NEWPass2;
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
	if (NEWPass != NEWPass2)
	{
		AfxMessageBox(_T("Please try again , the two passwords are not consistent.!"));
		return;
	}

	
	str.Format(L"INSERT into user2(id) VALUES(%s)", m_username);
	const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
	char c1[500] = { 0 };
	wcstombs(c1, wstr, wcslen(wstr));
	const char* cc1 = c1;

	mysql_query(&m_sqlCon, cc1);


	str.Format(L"UPDATE user2 SET password=AES_ENCRYPT(\"%s\", 'scuyf130916') where id = %s", NEWPass, m_username);
	const wchar_t* wstr2 = (LPCTSTR)str;         //һ������unicode��������仰����
	char c2[500] = { 0 };
	wcstombs(c2, wstr2, wcslen(wstr2));
	const char* cc2 = c2;

	mysql_query(&m_sqlCon, cc2);

	mysql_close(&m_sqlCon);//�ر�Mysql����

	//mysql_close(&m_sqlCon);//�ر�Mysql����
	CDialogEx::OnOK();
}


void ADDUSER::OnBnClickedCancel2()
{
	CDialogEx::OnCancel();
}
