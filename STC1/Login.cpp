// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "STC1.h"
#include "Login.h"
#include "afxdialogex.h"

#include "afxdialogex.h"
#include "math.h"
#include "mmsystem.h"
#include<ATLBASE.H>
#include<iomanip>
#include <io.h>

//#include <SkinPPWTL.h>

//#pragma comment(lib,"SkinPPWTL.lib")
#pragma comment(lib,"libmysql.lib")//���������Ҳ�����ڹ�������������
// CLogin �Ի���
CString UserName;
CString permission;

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_password(_T(""))
{
	m_real_password = L"��";
	keyid = -1;
	NameExist = false;
	m_username= UserName;
	
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_usernameEDIT, m_username);
	DDV_MaxChars(pDX, m_username, 32);
	DDX_Text(pDX, IDC_passwordEDIT, m_password);
	DDV_MaxChars(pDX, m_password, 32);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_passwordEDIT, &CLogin::OnEnSetfocuspasswordedit)
	ON_EN_SETFOCUS(IDC_usernameEDIT, &CLogin::OnEnSetfocususernameedit)
END_MESSAGE_MAP()


// CLogin ��Ϣ�������


BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (showWaring)
	{
		m_editFont.CreateFont(
			20,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
			_T("Arial"));                 // lpszFacename
		this->GetDlgItem(IDC_WaringSTATIC)->SetFont(&m_editFont);
		this->GetDlgItem(IDC_WaringSTATIC)->ShowWindow(TRUE);
	}
	if (moveingorlogin==1)
	{
		SetWindowText(L"Confirm Movement");
	}
	return true;
}

void CLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (ChangePass)//�Ƿ����޸�����
	{
		CString NEWPass;
		GetDlgItemText(IDC_passwordEDIT,NEWPass);
		str.Format(L"UPDATE user2 SET password=AES_ENCRYPT(\"%s\", 'scuyf130916') where id = %s", NEWPass,m_username);
		const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* cc1 = c1;

		if (mysql_query(&m_sqlCon, cc1))
		{
			AfxMessageBox(TEXT("ʧ�ܣ�"));
			return;
		}//��user2�������ѯ����"SELECT password FROM user2 where id = 1000"
										//��ȡ�����
		m_res = mysql_store_result(&m_sqlCon);
		//m_row = mysql_fetch_row(m_res);

		mysql_close(&m_sqlCon);//�ر�Mysql����
		CDialogEx::OnOK();

	}
	else if(!ChangePass){
		if (m_password == m_real_password)
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����
			UserName = m_username;
			CDialogEx::OnOK();
		}
		else
		{
			AfxMessageBox(_T("Password error, please check your username and password��"));
			SetDlgItemTextW(IDC_passwordEDIT, L"");
		}
	}
}


void CLogin::OnBnClickedCancel()
{
	if (CanClose)
	{
		CDialogEx::OnCancel();
	}
}

BOOL CLogin::SelectDB()
{
	//��ѯ����
	if (mysql_query(&m_sqlCon, "SELECT id FROM user2"))//��user2�������ѯid
		return FALSE;
	//��ȡ�����
	m_res = mysql_store_result(&m_sqlCon);
	return TRUE;
}

void CLogin::GetDataFromDB()
{
	//��ȡ��¼
	int i = 0;
	int id = 0;
	while (m_row = mysql_fetch_row(m_res))
	{
		str = m_row[0];
		if (_wtof(m_username.GetBuffer()) == _wtof(str.GetBuffer()))
		{
			NameExist = true;
		}
		i++;
	}
	if (!NameExist)
	{
		//AfxMessageBox(_T("û�и��û��ĵǼ���Ϣ"));
		return;
	}
}


void CLogin::OnEnSetfocuspasswordedit()//��ȡ��ʵ������
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

	UpdateData(TRUE);
	SelectDB();
	GetDataFromDB();

	//str.Format(L"SELECT password,permission FROM user2 where id = %s", m_username);
	str.Format(L"SELECT AES_DECRYPT(password, 'scuyf130916'),permission FROM user2 where id = %s", m_username);
	
	const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
	char c1[500] = { 0 };
	wcstombs(c1, wstr, wcslen(wstr));
	//������ת��
	const char* cc1 = c1;

	if (NameExist)
	{

		mysql_query(&m_sqlCon, cc1);//��user2�������ѯ����"SELECT password FROM user2 where id = 1000"
										//��ȡ�����
			m_res = mysql_store_result(&m_sqlCon);
			while (m_row = mysql_fetch_row(m_res))
			{
				m_real_password = m_row[0];
				permission = m_row[1];
			}
		
	}

	/*str.Format(L"SELECT permission FROM user2 where id = %s", m_username);

	wstr = (LPCTSTR)str;         //һ������unicode��������仰����
	wcstombs(c1, wstr, wcslen(wstr));
	cc1 = c1;
	if (NameExist)
	{
		mysql_query(&m_sqlCon, cc1);//��user2�������ѯ����"SELECT password FROM user2 where id = 1000"
										 //��ȡ�����
		m_res = mysql_store_result(&m_sqlCon);
		while (m_row2 = mysql_fetch_row(m_res))
		{
			permission = m_row2[0];
		}
	}*/
}

void CLogin::OnEnSetfocususernameedit()
{
	SetDlgItemTextW(IDC_passwordEDIT, L"");
}

HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  Change any attributes of the DC here   

		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_WaringSTATIC:
			pDC->SetTextColor(RGB(255, 0, 0)); //�����ı���ɫΪ
			pDC->SetBkColor(RGB(255, 255, 255));//�����ı�������ɫ
			pDC->SetBkMode(TRANSPARENT);//OPAQUE
			//return (HBRUSH)::GetStockObject(RGB(255, 255, 255));//���ÿؼ�������ɫ
			break;
		default:
			break;
		}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
