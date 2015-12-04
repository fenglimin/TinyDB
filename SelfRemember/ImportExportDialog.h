#pragma once

#include "CustomListCtrl.h"
#include "IniDB.h"
// CImportExportDialog dialog

class CImportExportDialog : public CDialog, public ICustomListUser
{
	DECLARE_DYNAMIC(CImportExportDialog)

public:
	CImportExportDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CImportExportDialog();

	CIniDB*				m_pIniDB;
	CIniDB				m_iniImportDB;
	CCustomListCtrl		m_listTable;
	BOOL				m_bImportAll;
	BOOL				m_bImport;
	ImportExportSetting	m_ieSetting;

	void	SetUIText();
	void	LoadExportTables();
	void	AddTableToList(const CString& strTableName);

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_EXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnCellCtrlClicked(CListCtrl* pListCtrl, int nRow, int nCol, CellFormat* pCellFormat);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonBrowse();
	CString m_strPassword;
	CString m_strPasswordConfirm;
	CString m_strIniFile;
	BOOL m_bEncrypt;
	afx_msg void OnBnClickedButtonAll();
	afx_msg void OnBnClickedButtonReload();
};
