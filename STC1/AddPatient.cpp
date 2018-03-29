// AddPatient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "STC1.h"
#include "AddPatient.h"
#include "afxdialogex.h"

#include <websocket.h>
#include <stdio.h>

#pragma comment(lib,"libmysql.lib")//���������Ҳ�����ڹ�������������

// AddPatient �Ի���

IMPLEMENT_DYNAMIC(AddPatient, CDialogEx)

AddPatient::AddPatient(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AddPatientDIALOG, pParent)
	, ID(_T(""))
	, Name(_T(""))
	, Birth(_T(""))
	, Gender(_T(""))
{

}

AddPatient::~AddPatient()
{
}

void AddPatient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GenderCOMBO, patientgender);
	DDX_Text(pDX, IDC_patientIDEDIT, ID);
	DDX_Text(pDX, IDC_patientNameEDIT, Name);
	DDX_Text(pDX, IDC_patientBirthEDIT, Birth);
	DDX_CBString(pDX, IDC_GenderCOMBO, Gender);
}


BEGIN_MESSAGE_MAP(AddPatient, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddPatient::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_patientIDEDIT, &AddPatient::OnEnKillfocuspatientidedit)
	ON_BN_CLICKED(IDC_searchBUTTON, &AddPatient::OnBnClickedsearchbutton)
END_MESSAGE_MAP()


// AddPatient ��Ϣ�������

BOOL AddPatient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	patientgender.AddString(L"Male");
	patientgender.AddString(L"Female");
	patientgender.AddString(L"Other");

	if (edit)
	{
		SetWindowText(L"EDIT PATIENT");
		this->GetDlgItem(IDC_patientNameEDIT)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_patientBirthEDIT)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_GenderCOMBO)->EnableWindow(FALSE);
	}

	return TRUE;
}


void AddPatient::OnBnClickedOk()
{
	mysql_init(&m_sqlCon);//��ʼ�����ݿ����
	if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "chair001", 3306, NULL, 0))//localhost:��������ַ������ֱ������IP;root:�˺�;123:����;test:���ݿ���;3306:����˿�  
	{
		AfxMessageBox(_T("Database connection failed!"));
		return;
	}
	else//���ӳɹ�������������ݿ⣬֮�����ز�����������Ƿ����������
	{
		//AfxMessageBox(_T("���ݿ����ӳɹ�!"));
	}

	GetDlgItemText(IDC_patientIDEDIT, ID);
	(mysql_query(&m_sqlCon, "SELECT id FROM patient"));//��users�������ѯid
	m_res = mysql_store_result(&m_sqlCon);
	while (m_row = mysql_fetch_row(m_res))
	{
		str = m_row[0];
		if (ID == str)
		{
			PatientExist = true;
		}
	}
	if (PatientExist&&!edit)//���޸�
	{
		//HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
		//((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		//AfxMessageBox(_T("Can't add, There EXIST information about the patient!"));
		return;
	}
	if (!PatientExist && edit)
	{
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
		((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		//AfxMessageBox(_T("Can't Edit, There NO information about the patient!"));
		return;
	}

	if (!PatientExist && !edit)//tianjai
	{
		hadedit = false;
		UpdateData(TRUE);
		str.Format(L"INSERT into patient (id,PatientName,birth,sex,PlanNum) VALUES(\"%s\",\"%s\",\"%s\",\"%s\",\"0\")", ID, Name, Birth, Gender);
		const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* cc1 = c1;
		mysql_query(&m_sqlCon, cc1);

		AfxMessageBox(_T("Edit Patient successful��"));
		mysql_close(&m_sqlCon);//�ر�Mysql����
		CDialogEx::OnOK();
				
	}

	if (PatientExist && edit)//xiugai
	{
		if (!hadedit)
		{
			str.Format(L"SELECT PatientName,birth,sex FROM patient where id = \"%s\"", ID);
			const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			char c1[500] = { 0 };
			wcstombs(c1, wstr, wcslen(wstr));
			const char* cc1 = c1;
			mysql_query(&m_sqlCon, cc1);//��users�������ѯ����"SELECT password FROM users where id = 1000"								//��ȡ�����
			m_res = mysql_store_result(&m_sqlCon);
			m_row = mysql_fetch_row(m_res);
			Name = m_row[0];
			Birth = m_row[1];
			Gender = m_row[2];

			UpdateData(FALSE);
			this->GetDlgItem(IDC_patientIDEDIT)->EnableWindow(FALSE);
			hadedit = true;
			mysql_close(&m_sqlCon);//�ر�Mysql����

			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
							
			this->GetDlgItem(IDC_searchBUTTON)->ShowWindow(FALSE);
			this->GetDlgItem(IDC_patientNameEDIT)->EnableWindow(TRUE);
			this->GetDlgItem(IDC_patientBirthEDIT)->EnableWindow(TRUE);
			this->GetDlgItem(IDC_GenderCOMBO)->EnableWindow(TRUE);
		}
		else if (hadedit)
		{
			UpdateData(TRUE);
			//str.Format(L"UPDATE into patient (id,PatientName,birth,sex) VALUES(\"%s\",\"%s\",\"%s\",\"%s\")", ID, Name, Birth, Gender);
			str.Format(L"UPDATE patient SET PatientName = \"%s\" where id = \"%s\"", Name, ID);
			const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			char c1[500] = { 0 };
			wcstombs(c1, wstr, wcslen(wstr));
			const char* cc1 = c1;
			mysql_query(&m_sqlCon, cc1);

			str.Format(L"UPDATE patient SET birth = \"%s\" where id = \"%s\"", Birth, ID);
			wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);

			str.Format(L"UPDATE patient SET sex = \"%s\" where id = \"%s\"", Gender, ID);
			wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);

			AfxMessageBox(_T("Edit Patient successful��"));
			mysql_close(&m_sqlCon);//�ر�Mysql����
			CDialogEx::OnOK();
		}
	}

}

void AddPatient::OnEnKillfocuspatientidedit()
{
	//PatientExist = false;
	mysql_init(&m_sqlCon);//��ʼ�����ݿ����
	if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "chair001", 3306, NULL, 0))//localhost:��������ַ������ֱ������IP;root:�˺�;123:����;test:���ݿ���;3306:����˿�  
	{
		AfxMessageBox(_T("Database connection failed!"));
		return;
	}
	else//���ӳɹ�������������ݿ⣬֮�����ز�����������Ƿ����������
	{
		//AfxMessageBox(_T("���ݿ����ӳɹ�!"));
	}
	UpdateData(TRUE);
	//GetDlgItemText(IDC_IDEDIT, ID);
	(mysql_query(&m_sqlCon, "SELECT id FROM patient"));//��users�������ѯid
	m_res = mysql_store_result(&m_sqlCon);
	while (m_row = mysql_fetch_row(m_res))
	{
		str = m_row[0];
		if (ID == str)
		{
			PatientExist = true;
			break;
		}
		else
		{
			PatientExist = false;
		}
	}

	if ((!PatientExist)&&(!edit))
	{
		mysql_close(&m_sqlCon);//�ر�Mysql����
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
		((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
	}
	else if (PatientExist && !edit)
	{
		mysql_close(&m_sqlCon);//�ر�Mysql����

		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
		((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
	}
	/*else if (!PatientExist && edit)
	{
		mysql_close(&m_sqlCon);//�ر�Mysql����
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
		((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
	}
	else if (PatientExist && edit)
	{
		str.Format(L"SELECT PatientName,birth,sex FROM patient where id = \"%s\"", ID);
		const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* cc1 = c1;
		mysql_query(&m_sqlCon, cc1);//��users�������ѯ����"SELECT password FROM users where id = 1000"								//��ȡ�����
		m_res = mysql_store_result(&m_sqlCon);
		m_row = mysql_fetch_row(m_res);
		Name = m_row[0];
		Birth = m_row[1];
		Gender = m_row[2];

		UpdateData(FALSE);
		//this->GetDlgItem(IDC_patientIDEDIT)->EnableWindow(FALSE);
		hadedit = true;
		mysql_close(&m_sqlCon);//�ر�Mysql����

		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
		((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
	}*/
}


void AddPatient::OnBnClickedsearchbutton()
{
	if (!PatientExist && edit)
	{
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
		((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		//AfxMessageBox(_T("Can't Edit, There NO information about the patient!"));
		return;
	}
	if (PatientExist && edit)//xiugai
	{
		if (!hadedit)
		{
			str.Format(L"SELECT PatientName,birth,sex FROM patient where id = \"%s\"", ID);
			const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			char c1[500] = { 0 };
			wcstombs(c1, wstr, wcslen(wstr));
			const char* cc1 = c1;
			mysql_query(&m_sqlCon, cc1);//��users�������ѯ����"SELECT password FROM users where id = 1000"								//��ȡ�����
			m_res = mysql_store_result(&m_sqlCon);
			m_row = mysql_fetch_row(m_res);
			Name = m_row[0];
			Birth = m_row[1];
			Gender = m_row[2];

			UpdateData(FALSE);
			this->GetDlgItem(IDC_patientIDEDIT)->EnableWindow(FALSE);
			hadedit = true;
			mysql_close(&m_sqlCon);//�ر�Mysql����

			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		}
		this->GetDlgItem(IDC_searchBUTTON)->ShowWindow(FALSE);
		this->GetDlgItem(IDC_patientNameEDIT)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_patientBirthEDIT)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_GenderCOMBO)->EnableWindow(TRUE);
	}
}
