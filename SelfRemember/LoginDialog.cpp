// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "LoginDialog.h"


// CLoginDialog dialog

IMPLEMENT_DYNAMIC(CLoginDialog, CDialog)

CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
	, m_strUsername(_T(""))
	, m_strPassword(_T(""))
{

}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUsername);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDialog message handlers

void CLoginDialog::OnBnClickedOk()
{
	UpdateData();

	if ( !m_pIniDB->CanUserLogin(m_strUsername, m_strPassword ))
		return;

	OnOK();
}

void CLoginDialog::SetUIText()
{
	SetDlgItemText(IDC_STATIC_USERNAME, _M("Username"));
	SetDlgItemText(IDC_STATIC_PASSWORD, _M("Password"));
	SetDlgItemText(IDOK, _M("Login"));
	SetDlgItemText(IDCANCEL, _M("Exit"));
}

BOOL CLoginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetUIText();

	SetWindowText(_M("Login"));

	return TRUE;
}
