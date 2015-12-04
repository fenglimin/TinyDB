#pragma once

#include "IniDB.h"
// CChangePasswordDialog dialog

class CChangePasswordDialog : public CDialog
{
	DECLARE_DYNAMIC(CChangePasswordDialog)

public:

	void	SetUIText();
	CIniDB*		m_pIniDB;
	CChangePasswordDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChangePasswordDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGE_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPassword;
	CString m_strNewPassword;
	CString m_strConfirmNewPassword;
	afx_msg void OnBnClickedOk();
};
