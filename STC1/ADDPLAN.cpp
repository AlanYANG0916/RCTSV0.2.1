// ADDPLAN.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "STC1.h"
#include "ADDPLAN.h"
#include "afxdialogex.h"

#include <websocket.h>
#include <stdio.h>

#pragma comment(lib,"libmysql.lib")//���������Ҳ�����ڹ�������������


// ADDPLAN �Ի���

IMPLEMENT_DYNAMIC(ADDPLAN, CDialogEx)

ADDPLAN::ADDPLAN(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDPLANDIALOG, pParent)
	, ID(_T(""))
	, PlanName(_T(""))
	, m_beamname(_T(""))
	, m_beamangle(_T(""))
{
	BeamNum = 0;
}

ADDPLAN::~ADDPLAN()
{
}

void ADDPLAN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IDEDIT, ID);
	DDX_Text(pDX, IDC_PLANNAMEEDIT, PlanName);
	DDX_Control(pDX, IDC_COMMLIST, ADDBeamList);
	DDX_Text(pDX, IDC_BEAMNAMEEDIT, m_beamname);
	DDX_Text(pDX, IDC_BEAMANGLEEDIT, m_beamangle);
	DDX_Control(pDX, IDC_EDIT5, m_edit);
	DDX_Control(pDX, IDC_PLANCOMBO, m_plancombo);
}


BEGIN_MESSAGE_MAP(ADDPLAN, CDialogEx)
	ON_BN_CLICKED(IDOK, &ADDPLAN::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &ADDPLAN::OnBnClickedButton1)
	ON_STN_CLICKED(IDC_FINDIDSTATIC, &ADDPLAN::OnStnClickedFindidstatic)
	ON_EN_KILLFOCUS(IDC_IDEDIT, &ADDPLAN::OnEnKillfocusIdedit)
	ON_EN_KILLFOCUS(IDC_PLANNAMEEDIT, &ADDPLAN::OnEnKillfocusPlannameedit)
	ON_NOTIFY(NM_RCLICK, IDC_COMMLIST, &ADDPLAN::OnNMRClickCommlist)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_COMMLIST, &ADDPLAN::OnNMCustomdrawCommlist)
	ON_EN_KILLFOCUS(IDC_EDIT5, &ADDPLAN::OnEnKillfocusEdit5)
	ON_NOTIFY(NM_DBLCLK, IDC_COMMLIST, &ADDPLAN::OnNMDblclkCommlist)
	ON_EN_SETFOCUS(IDC_EDIT5, &ADDPLAN::OnEnSetfocusEdit5)
	ON_STN_CLICKED(IDC_FINDNAMESTATIC, &ADDPLAN::OnStnClickedFindnamestatic)
END_MESSAGE_MAP()


// ADDPLAN ��Ϣ�������


BOOL ADDPLAN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

		CRect rect;
		// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
		ADDBeamList.GetClientRect(&rect);
		ADDBeamList.InsertColumn(0, _T("Beam Name"), LVCFMT_CENTER, rect.Width() / 3 * 2);
		ADDBeamList.InsertColumn(1, _T("Beam Angle/��"), LVCFMT_CENTER, rect.Width() / 3);

		if (isedit)
		{
			SetWindowText(L"EDIT PLAN");
			this->GetDlgItem(IDC_PLANNAMEEDIT)->ShowWindow(FALSE);
			this->GetDlgItem(IDC_PLANCOMBO)->ShowWindow(TRUE);

			this->GetDlgItem(IDOK)->EnableWindow(FALSE);
		}
		//GetDlgItem(IDC_EDIT5)->ShowWindow(FALSE);

	return TRUE;
}

void ADDPLAN::OnBnClickedOk()
{

	if (!isedit)//��ʾ�½�һ���ƻ�
	{
		if (PatientExist && !PlanExist)
		{
			UpdateData(TRUE);
			mysql_init(&m_sqlCon);//��ʼ�����ݿ����
			if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "chair001", 3306, NULL, 0))//localhost:��������ַ������ֱ������IP;root:�˺�;123:����;test:���ݿ���;3306:����˿�  
			{
				AfxMessageBox(_T("Database connection failed!"));
				return;
			}
			else//���ӳɹ�������������ݿ⣬֮�����ز�����������Ƿ����������
			{
			}

			//�鿴��ǰ�ƻ���
			str.Format(L"SELECT PlanNum FROM patient where id = \"%s\"", ID);
			const wchar_t* wstr = (LPCTSTR)str; //һ������unicode��������仰����
			char c1[500] = { 0 };
			wcstombs(c1, wstr, wcslen(wstr));
			const char* cc1 = c1;
			mysql_query(&m_sqlCon, cc1);
			m_res = mysql_store_result(&m_sqlCon);
			m_row = mysql_fetch_row(m_res);
			str = m_row[0];
			int PlanNum = _wtof(str.GetBuffer());
			//�����˲����¼ƻ�
			str.Format(L"UPDATE patient SET plan%d =\"%s\" where id = \"%s\"", PlanNum + 1, PlanName, ID);
			wstr = (LPCTSTR)str;//һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);
			str.Format(L"UPDATE patient SET PlanNum =\"%d\" where id = \"%s\"", PlanNum + 1, ID);
			wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);

			//��ƻ�������¼ƻ�
			str.Format(L"INSERT into plan (name) VALUES(\"%s%s\")", ID,PlanName);
			wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);
			str.Format(L"INSERT into plan (name) VALUES(\"%s%sN\")", ID,PlanName);
			wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);

			for (size_t i = 0; i < BeamNum; i++)
			{
				str.Format(L"UPDATE plan SET beam%d =\"%s\" where name = \"%s%s\"", i + 1, BeamAngle[i],ID, PlanName);
				wstr = (LPCTSTR)str;         //һ������unicode��������仰����
				for (int i = 0; i < 500; i++) { c1[i] = 0; }
				wcstombs(c1, wstr, wcslen(wstr));
				cc1 = c1;
				mysql_query(&m_sqlCon, cc1);

				str.Format(L"UPDATE plan SET beam%d =\"%s\" where name = \"%s%sN\"", i + 1, BeamName[i],ID, PlanName);
				wstr = (LPCTSTR)str;         //һ������unicode��������仰����
				for (int i = 0; i < 500; i++) { c1[i] = 0; }
				wcstombs(c1, wstr, wcslen(wstr));
				cc1 = c1;
				mysql_query(&m_sqlCon, cc1);
			}
			AfxMessageBox(_T("Add plan successful��"));
			mysql_close(&m_sqlCon);//�ر�Mysql����
			CDialogEx::OnOK();
		}
		else
		{
			return;
		}
	}
	else if(isedit&&PlanExist)//�༭���еļƻ�
	{
		m_plancombo.GetWindowText(PlanName);
		mysql_init(&m_sqlCon);//��ʼ�����ݿ����
		if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "chair001", 3306, NULL, 0))//localhost:��������ַ������ֱ������IP;root:�˺�;123:����;test:���ݿ���;3306:����˿�  
		{
			AfxMessageBox(_T("Database connection failed!"));
			return;
		}
		else//���ӳɹ�������������ݿ⣬֮�����ز�����������Ƿ����������
		{
			
		}

		for (size_t i = 0; i < BeamNum; i++)
		{
			str.Format(L"UPDATE plan SET beam%d =\"%s\" where name = \"%s%s\"", i + 1, BeamAngle[i],ID, PlanName);
			const wchar_t* wstr = (LPCTSTR)str; //һ������unicode��������仰����
			char c1[500] = { 0 };
			wcstombs(c1, wstr, wcslen(wstr));
			const char* cc1 = c1;
			mysql_query(&m_sqlCon, cc1);

			str.Format(L"UPDATE plan SET beam%d =\"%s\" where name = \"%s%sN\"", i + 1, BeamName[i], ID,PlanName);
			wstr = (LPCTSTR)str;         //һ������unicode��������仰����
			for (int i = 0; i < 500; i++) { c1[i] = 0; }
			wcstombs(c1, wstr, wcslen(wstr));
			cc1 = c1;
			mysql_query(&m_sqlCon, cc1);
		}
		AfxMessageBox(_T("edit plan successful��"));
		mysql_close(&m_sqlCon);//�ر�Mysql����
		CDialogEx::OnOK();
	}
}


void ADDPLAN::OnBnClickedButton1()//���beam��Ϣ
{
	if (editing)
	{
		UpdateData(TRUE);
		BeamName[p_edit] = m_beamname;
		BeamAngle[p_edit] = m_beamangle;

		ADDBeamList.DeleteItem(p_edit);
		ADDBeamList.InsertItem(p_edit, BeamName[p_edit]);// m_commattuide.QAName);
		ADDBeamList.SetItemText(p_edit, 1, BeamAngle[p_edit]);
		editing = false;
		ADDBeamList.SetItemData(p_edit, COLOR_DEFAULT);
		SetDlgItemText(IDC_BUTTON1, L"ADD beam");
	}
	else
	{
		UpdateData(TRUE);

		BeamName[BeamNum] = m_beamname;
		BeamAngle[BeamNum] = m_beamangle;

		ADDBeamList.InsertItem(BeamNum, BeamName[BeamNum]);// m_commattuide.QAName);
		ADDBeamList.SetItemText(BeamNum, 1, BeamAngle[BeamNum]);

		BeamNum++;
	}
}


void ADDPLAN::OnStnClickedFindidstatic()
{

}


void ADDPLAN::OnEnKillfocusIdedit()
{
	if (!isedit)
	{
		PatientExist = false;
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
		GetDlgItemText(IDC_IDEDIT, ID);
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
		if (!PatientExist)
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
			((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		}
		else if (PatientExist)
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����

			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		}
	}
	else if (isedit)
	{
		PlanExist = false;
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
		CString result;
		GetDlgItemText(IDC_IDEDIT, ID);
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
		if (PatientExist)
		{
			//mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		}
		else if (!PatientExist)
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����

			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
			((CStatic*)GetDlgItem(IDC_FINDIDSTATIC))->SetIcon(m_hIcon);
		}
		str.Format(L"SELECT PatientName,birth,sex,plan1,plan2,plan3,plan4,plan5,plan6,plan7,plan8,plan9,plan10 FROM patient where id = \"%s\"",ID );

		const wchar_t* wstr = (LPCTSTR)str;         //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		//������ת��
		const char* cc1 = c1;

		if (PatientExist)
		{
			mysql_query(&m_sqlCon, cc1);//��users�������ѯ����"SELECT password FROM users where id = 1000"								//��ȡ�����
			m_res = mysql_store_result(&m_sqlCon);
			while (m_row = mysql_fetch_row(m_res))
			{

				m_plancombo.ResetContent();
				//m_report.PlanName = m_row[3];
				for (size_t i = 0; i < 10; i++)
				{
					result = m_row[3 + i];
					if (result != "")
					{
						m_plancombo.AddString(result);
					}
					else if (result == "")
					{
						break;
					}
				}
			}
		}
		mysql_close(&m_sqlCon);//�ر�Mysql����

		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON9);
		((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
	}
	
}


void ADDPLAN::OnEnKillfocusPlannameedit()
{
	if (!isedit)
	{
		PlanExist = false;
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
		GetDlgItemText(IDC_PLANNAMEEDIT, PlanName);
		CString PlanName2;
		PlanName2.Format(L"%s%s",ID,PlanName);
		(mysql_query(&m_sqlCon, "SELECT name FROM plan"));//��users�������ѯid
		m_res = mysql_store_result(&m_sqlCon);
		while (m_row = mysql_fetch_row(m_res))
		{
			str = m_row[0];
			if (PlanName2 == str)
			{
				PlanExist = true;
			}
		}
		if (PlanExist|| PlanName =="")
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
			((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
		}
		else if (!PlanExist)
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����

			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
		}
	}
	else if (isedit)//�༭�ƻ�
	{
		PlanExist = false;
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
		GetDlgItemText(IDC_PLANNAMEEDIT, PlanName);
		(mysql_query(&m_sqlCon, "SELECT name FROM plan"));//��users�������ѯid
		m_res = mysql_store_result(&m_sqlCon);
		while (m_row = mysql_fetch_row(m_res))
		{
			str = m_row[0];
			if (PlanName == str)
			{
				PlanExist = true;
			}
		}
		if (PlanExist)
		{
			//mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
			this->GetDlgItem(IDOK)->EnableWindow(TRUE);
			ADDBeamList.DeleteAllItems();
		}
		else if (!PlanExist || PlanName == "")
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
			((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
			this->GetDlgItem(IDOK)->EnableWindow(FALSE);
			ADDBeamList.DeleteAllItems();
			return;
			
		}

		UpdateData(TRUE);
		//�鿴��ǰ�ƻ���


		str.Format(L"SELECT beam1,beam2,beam3,beam4,beam5,beam6,beam7,beam8,beam9,beam10 FROM plan where name = \"%s\"", PlanName);
		const wchar_t* wstr = (LPCTSTR)str; //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* cc1 = c1;
		mysql_query(&m_sqlCon, cc1);
		m_res = mysql_store_result(&m_sqlCon);
		m_row = mysql_fetch_row(m_res);

		for (size_t i = 0; i < 10; i++)
		{
			BeamAngle[i] = m_row[i];
		}

		str.Format(L"SELECT beam1,beam2,beam3,beam4,beam5,beam6,beam7,beam8,beam9,beam10 FROM plan where name = \"%sN\"", PlanName);
		wstr = (LPCTSTR)str; //һ������unicode��������仰����
		for (int i = 0; i < 500; i++) { c1[i] = 0; }
		wcstombs(c1, wstr, wcslen(wstr));
		cc1 = c1;
		mysql_query(&m_sqlCon, cc1);
		m_res = mysql_store_result(&m_sqlCon);
		m_row = mysql_fetch_row(m_res);

		for (size_t i = 0; i < 10; i++)
		{
			BeamName[i] = m_row[i];
			if (BeamName[i]=="")
			{
				BeamNum = i ;
				break;
			}
		}

		for (size_t i = 0; i < BeamNum; i++)
		{
			ADDBeamList.InsertItem(i, BeamName[i]);// m_commattuide.QAName);
			ADDBeamList.SetItemText(i, 1, BeamAngle[i]);
		}
		mysql_close(&m_sqlCon);//�ر�Mysql����
	}
}

void ADDPLAN::OnNMRClickCommlist(NMHDR *pNMHDR, LRESULT *pResult)//�б��Ҽ���Ӧ����
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if (index == -1)
		return;
	HMENU hMenu = ::CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, 10001, _T("Edit"));
	AppendMenu(hMenu, MF_STRING, 10002, _T("Delete"));
	CPoint pt;
	GetCursorPos(&pt); //��õ�ǰ���λ��
	UINT Cmd = (UINT)::TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL);//�����˵�

	POSITION pos = ADDBeamList.GetFirstSelectedItemPosition();
	int nItem = ADDBeamList.GetNextSelectedItem(pos);

	//CString str;
	switch (Cmd)//��Ӧ����Ĳ˵�
	{
	case 10001:

		ADDBeamList.SetItemData(nItem, COLOR_BLUE);

		editing = true;
		p_edit = nItem;
		m_beamname = BeamName[nItem];
		m_beamangle = BeamAngle[nItem];
		UpdateData(FALSE);
		SetDlgItemText(IDC_BUTTON1, L"Apply Edit");
		break;
	case 10002:
		ADDBeamList.DeleteItem(nItem);
		BeamNum--;

		mysql_init(&m_sqlCon);//��ʼ�����ݿ����
		if (!mysql_real_connect(&m_sqlCon, "localhost", "root", "123456", "chair001", 3306, NULL, 0))//localhost:��������ַ������ֱ������IP;root:�˺�;123:����;test:���ݿ���;3306:����˿�  
		{
			AfxMessageBox(_T("Database connection failed!"));
			return;
		}
		str.Format(L"UPDATE plan SET beam%d =\"%s\" where name = \"%s%s\"", BeamNum+1, "", ID, PlanName);
		const wchar_t* wstr = (LPCTSTR)str; //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* cc1 = c1;
		mysql_query(&m_sqlCon, cc1);

		str.Format(L"UPDATE plan SET beam%d =\"%s\" where name = \"%s%sN\"", BeamNum+1, "", ID, PlanName);
		wstr = (LPCTSTR)str;         //һ������unicode��������仰����
		for (int i = 0; i < 500; i++) { c1[i] = 0; }
		wcstombs(c1, wstr, wcslen(wstr));
		cc1 = c1;
		mysql_query(&m_sqlCon, cc1);
		break;	
	}
	*pResult = 0;
}

void ADDPLAN::OnNMCustomdrawCommlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMTVCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������  
	NMCUSTOMDRAW nmCustomDraw = pNMCD->nmcd;
	switch (nmCustomDraw.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
	{
		if (COLOR_BLUE == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(51, 153, 255);
			pNMCD->clrText = RGB(255, 255, 255);
		}
		else if (COLOR_RED == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 0, 0);       //������ɫ  
			pNMCD->clrText = RGB(255, 255, 255);     //������ɫ  
		}
		else if (COLOR_DEFAULT == nmCustomDraw.lItemlParam)
		{
			pNMCD->clrTextBk = RGB(255, 255, 255);
			pNMCD->clrText = RGB(0, 0, 0);
		}
		else
		{
			//  
		}
		break;
	}
	default:
	{
		break;
	}
	}

	*pResult = 0;
	*pResult |= CDRF_NOTIFYPOSTPAINT;       //�����У���Ȼ��û��Ч��  
	*pResult |= CDRF_NOTIFYITEMDRAW;        //�����У���Ȼ��û��Ч��  
	return;
}

void ADDPLAN::OnEnKillfocusEdit5()
{
	CString tem;
	m_edit.GetWindowText(tem);    //�õ��û�������µ����� 
	SetDlgItemText(IDC_EDIT5, L"123");
	ADDBeamList.SetItemText(m_Row, m_Col, tem);   //���ñ༭���������
	
	//m_edit.ShowWindow(FALSE);  //Ӧ�ر༭��
	
}

void ADDPLAN::OnNMDblclkCommlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//OnEnKillfocusEdit5();
	/*NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	m_Row = pNMListView->iItem;//���ѡ�е���  
	m_Col = pNMListView->iSubItem;//���ѡ����
	SetDlgItemText(IDC_EDIT5, L"123");

	CRect rc;
		ADDBeamList.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//��������RECT��  
		m_edit.SetParent(&ADDBeamList);//ת������Ϊ�б���е�����  
		m_edit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��; 
		//ssss = ADDBeamList.GetItemText(0, 1);
		//this->SetDlgItemTextW(IDC_EDIT5, ssss);
		//this->GetDlgItem(IDC_EDIT5)->SetDlgItemTextW();//���������е�ֵ����Edit�ؼ��� 
		//m_edit.ShowWindow(TRUE);
		//m_edit.SetFocus();//����Edit����  
		//m_edit.ShowCaret();//��ʾ���  
		//m_edit.SetSel(-1);//������ƶ������ 

	//}
	*pResult = 0;*/
}


void ADDPLAN::OnEnSetfocusEdit5()
{
	//str = ADDBeamList.GetItemText(m_Row, m_Col);
	//SetDlgItemText(IDC_EDIT5, L"");
}


void ADDPLAN::OnStnClickedFindnamestatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (isedit)
	{
		HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
		((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);

		PlanExist = false;
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
		//GetDlgItemText(IDC_PLANNAMEEDIT, PlanName);

		m_plancombo.GetWindowText(PlanName);
		CString PlanName2;
		PlanName2.Format(L"%s%s", ID, PlanName);
		(mysql_query(&m_sqlCon, "SELECT name FROM plan"));//��users�������ѯid
		m_res = mysql_store_result(&m_sqlCon);
		while (m_row = mysql_fetch_row(m_res))
		{
			str = m_row[0];
			if (PlanName2 == str)
			{
				PlanExist = true;
			}
		}
		if (PlanExist)
		{
			//mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON8);
			((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
			this->GetDlgItem(IDOK)->EnableWindow(TRUE);
			ADDBeamList.DeleteAllItems();
		}
		else if (!PlanExist || PlanName == "")
		{
			mysql_close(&m_sqlCon);//�ر�Mysql����
			HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICONcha);
			((CStatic*)GetDlgItem(IDC_FINDNAMESTATIC))->SetIcon(m_hIcon);
			this->GetDlgItem(IDOK)->EnableWindow(FALSE);
			ADDBeamList.DeleteAllItems();
			return;
		}

		//UpdateData(TRUE);
		//�鿴��ǰ�ƻ���


		str.Format(L"SELECT beam1,beam2,beam3,beam4,beam5,beam6,beam7,beam8,beam9,beam10 FROM plan where name = \"%s%s\"",ID, PlanName);
		const wchar_t* wstr = (LPCTSTR)str; //һ������unicode��������仰����
		char c1[500] = { 0 };
		wcstombs(c1, wstr, wcslen(wstr));
		const char* cc1 = c1;
		mysql_query(&m_sqlCon, cc1);
		m_res = mysql_store_result(&m_sqlCon);
		m_row = mysql_fetch_row(m_res);

		for (size_t i = 0; i < 10; i++)
		{
			BeamAngle[i] = m_row[i];
		}

		str.Format(L"SELECT beam1,beam2,beam3,beam4,beam5,beam6,beam7,beam8,beam9,beam10 FROM plan where name = \"%s%sN\"", ID,PlanName);
		wstr = (LPCTSTR)str; //һ������unicode��������仰����
		for (int i = 0; i < 500; i++) { c1[i] = 0; }
		wcstombs(c1, wstr, wcslen(wstr));
		cc1 = c1;
		mysql_query(&m_sqlCon, cc1);
		m_res = mysql_store_result(&m_sqlCon);
		m_row = mysql_fetch_row(m_res);

		for (size_t i = 0; i < 10; i++)
		{
			BeamName[i] = m_row[i];
			if (BeamName[i] == "")
			{
				BeamNum = i;
				break;
			}
		}

		for (size_t i = 0; i < BeamNum; i++)
		{
			ADDBeamList.InsertItem(i, BeamName[i]);// m_commattuide.QAName);
			ADDBeamList.SetItemText(i, 1, BeamAngle[i]);
		}
		mysql_close(&m_sqlCon);//�ر�Mysql����
	}
}
