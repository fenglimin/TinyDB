// SelfRememberDlg.h : header file
//

#if !defined(AFX_SELFREMEMBERDLG_H__D6C2DB64_5490_4778_98DF_7385C26753A7__INCLUDED_)
#define AFX_SELFREMEMBERDLG_H__D6C2DB64_5490_4778_98DF_7385C26753A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ini.h"
#include "CustomListCtrl.h"
#include "CommonTool.h"
#include "afxwin.h"
#include "IniDBManager.h"

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberDlg dialog

class CSelfRememberDlg : public CDialog, public IIniDBManagerUser
{
	// Construction
public:
	CIniDBManager*	m_pDBManager;
	CIniDB*  m_pIniDB;
	CCommonTool* m_pCommonTool;
	CString	m_strIniFile;
	CString m_strRumCmdFile;

	void SetUIText();
	void SetDBManager(CIniDBManager* pDBManager);
	void ShowTableItemCount();
	void ReloadTableList();
	void UpdateIniFileInResource();
	CSelfRememberDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(CSelfRememberDlg)
	enum { IDD = IDD_SELFREMEMBER_DIALOG };
	CCustomListCtrl	m_listTable;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfRememberDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSelfRememberDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonTest();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InitList();
	void OnCloseDB();
public:
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonCreateTable();
	afx_msg void OnBnClickedButtonEditTable();
	afx_msg void OnBnClickedButtonDeleteTable();
	CComboBox m_comboTables;
	afx_msg void OnCbnSelchangeComboTables();
	afx_msg void OnBnClickedButtonChangePassword();
	CButton m_btnCreateTable;
	CButton m_btnEditTable;
	CButton m_btnDeleteTable;
	afx_msg void OnBnClickedButtonUserOption();

public:
	virtual void OnListChanged();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFREMEMBERDLG_H__D6C2DB64_5490_4778_98DF_7385C26753A7__INCLUDED_)
