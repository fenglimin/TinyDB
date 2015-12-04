#pragma once

#include "IniDB.h"
// CLoginDialog dialog

class CLoginDialog : public CDialog
{
	DECLARE_DYNAMIC(CLoginDialog)

public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDialog();

	CIniDB* m_pIniDB;

	void SetUIText();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUsername;
	CString m_strPassword;
	afx_msg void OnBnClickedOk();
};
