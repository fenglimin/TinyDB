// ChangePasswordDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "ChangePasswordDialog.h"


// CChangePasswordDialog dialog

IMPLEMENT_DYNAMIC(CChangePasswordDialog, CDialog)

CChangePasswordDialog::CChangePasswordDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePasswordDialog::IDD, pParent)
	, m_strOldPassword(_T(""))
	, m_strNewPassword(_T(""))
	, m_strConfirmNewPassword(_T(""))
{

}

CChangePasswordDialog::~CChangePasswordDialog()
{
}

void CChangePasswordDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWORD, m_strOldPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_strNewPassword);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_NEW_PASSWORD, m_strConfirmNewPassword);
}


BEGIN_MESSAGE_MAP(CChangePasswordDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CChangePasswordDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangePasswordDialog message handlers

void CChangePasswordDialog::OnBnClickedOk()
{
	UpdateData();

	if ( !m_pIniDB->CanUserLogin(m_pIniDB->m_strCurrentUser, m_strOldPassword ) )
	{
		return;
	}

	if ( m_strNewPassword != m_strConfirmNewPassword )
	{
		AfxMessageBox(_M("New password mismatch!"));
		return;
	}
	
	OnOK();
}

void CChangePasswordDialog::SetUIText()
{
	SetDlgItemText(IDC_STATIC_OLD_PASSWORD, _M("Old Password"));
	SetDlgItemText(IDC_STATIC_NEW_PASSWORD, _M("New Password"));
	SetDlgItemText(IDC_STATIC_CONFIRM_PASSWORD, _M("Confirm Password"));

	SetDlgItemText(IDOK, _M("OK"));
	SetDlgItemText(IDCANCEL, _M("Cancel"));

	SetWindowText(_M("Change Password"));
}

BOOL CChangePasswordDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetUIText();

	return TRUE;
}
