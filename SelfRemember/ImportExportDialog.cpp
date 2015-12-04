// ImportExportDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "ImportExportDialog.h"


// CImportExportDialog dialog

IMPLEMENT_DYNAMIC(CImportExportDialog, CDialog)

CImportExportDialog::CImportExportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CImportExportDialog::IDD, pParent)
	, m_strPassword(_T(""))
	, m_strPasswordConfirm(_T(""))
	, m_strIniFile(_T(""))
	, m_bEncrypt(TRUE)
{
	m_bImport = FALSE;
	m_pIniDB = NULL;
	m_bEncrypt = TRUE;

	m_bImportAll = FALSE;
}

CImportExportDialog::~CImportExportDialog()
{
}

void CImportExportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMPORT_EXPORT, m_listTable);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_CONFIRM, m_strPasswordConfirm);
	DDX_Text(pDX, IDC_EDIT_INI_FILE, m_strIniFile);
	DDX_Check(pDX, IDC_CHECK_ENCRYPT, m_bEncrypt);
}


BEGIN_MESSAGE_MAP(CImportExportDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CImportExportDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CImportExportDialog::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_ALL, &CImportExportDialog::OnBnClickedButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CImportExportDialog::OnBnClickedButtonReload)
END_MESSAGE_MAP()


// CImportExportDialog message handlers
BOOL CImportExportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strColumnName1, strColumnName2;
	if ( m_bImport )
	{
		strColumnName1 = _M("Table Name In The File");
		strColumnName2 = _M("Table Name Imported(Editable)");
	}
	else
	{
		strColumnName1 = _M("Table Name In The System");
		strColumnName2 = _M("Table Name Exported(Editable)");
	}

	CustomColumn		gridColumn;
	CustomColumnList	gridColumnsList;

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _T("");
	gridColumn.nWidth = 30;
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = strColumnName1;
	gridColumn.nWidth = 160;
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = strColumnName2;
	gridColumn.nWidth = 190;
	gridColumnsList.push_back(gridColumn);


	CString strColumnOrder = "0;1;2;";
	m_listTable.SetColumnInfo(gridColumnsList, strColumnOrder);

	// Set Row Bk Color
	m_listTable.AddDefaultRowColor(RGB(239,239,239));
	m_listTable.AddDefaultRowColor(RGB(207,207,207));

	// Height
	m_listTable.SetHeaderHeight(24);
	m_listTable.SetRowHeigt(24);

	m_listTable.EnableFilter(FALSE);
	m_listTable.SetMultipleSelection(TRUE);
	m_listTable.SetShowSelection(FALSE);
	m_listTable.SetHighlightChangedCellText(TRUE);
	m_listTable.SetSelectThenEdit(FALSE);
	m_listTable.SetEnablePopupMenu(FALSE);

	m_listTable.SetUser(this);

	if ( !m_pIniDB->IsAdminUser() )
	{
		GetDlgItem(IDC_CHECK_ENCRYPT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ALL)->ShowWindow(SW_HIDE);
	}

	if ( m_bImport )
	{
		m_strIniFile = m_pIniDB->m_userOptions.strAutoSaveFile;

		GetDlgItem(IDC_STATIC_CONFIRM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PASSWORD_CONFIRM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_RELOAD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_RELOAD)->EnableWindow(TRUE);
	}
	else
	{
		CTime dt = CTime::GetCurrentTime();
		m_strIniFile.Format(_T("C:\\TinyDB_Export_%s_%d-%02d-%02d %2d-%02d-%02d.ini"), m_pIniDB->m_strCurrentUser, dt.GetYear(), dt.GetMonth(),
			dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
	
		LoadExportTables();
		GetDlgItem(IDC_BUTTON_ALL)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	SetUIText();

	return TRUE;
}

void CImportExportDialog::LoadExportTables()
{
	for ( list<CTable<CString>*>::iterator it = m_pIniDB->m_listTablesAccessible.begin(); it != m_pIniDB->m_listTablesAccessible.end(); it ++ )
	{
		if ( m_pIniDB->CanTableExport((*it)->strName) )
		{
			AddTableToList((*it)->strName);
		}
	}
}

void CImportExportDialog::OnBnClickedOk()
{
	UpdateData();

	if ( m_strIniFile.IsEmpty() )
	{
		AfxMessageBox(_M("Please set the export file!"));
		return;
	}

	if ( !m_bImport && m_strPassword != m_strPasswordConfirm )
	{
		AfxMessageBox(_M("Password mismatch!"));
		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PASSWORD);
		pEdit->SetFocus();
		pEdit->SetSel(0, m_strPassword.GetLength());
		return;
	}

	m_ieSetting.mapTables.clear();
	CString strText1, strText2, strMsg;
	int nCount = m_listTable.GetItemCount();
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( !m_listTable.GetCellFormat(i,0)->bChecked )
			continue;

		strText1 = m_listTable.GetItemText(i,1);
		strText2 = m_listTable.GetItemText(i,2);
		if ( m_ieSetting.mapTables[strText2] != _T("") )
		{
			strMsg.Format(_M("Table name %s is duplicated!"), strText2);
			AfxMessageBox(strMsg);
			m_listTable.StartCellEdit(i,2);
			return;
		}

		if ( m_bImport )
		{
			if ( m_pIniDB->FindTable(strText2) != NULL )
			{
				strMsg.Format(_M("Table name %s is already existed!"), strText2);
				AfxMessageBox(strMsg);
				m_listTable.StartCellEdit(i,2);
				return;
			}
		}

		m_ieSetting.mapTables[strText2] = strText1;
	}

	if ( m_ieSetting.mapTables.size() == 0 )
	{
		AfxMessageBox(_M("Please select at least one table!"));
		return;
	}

	m_ieSetting.bEncrypt = m_bEncrypt;
	m_ieSetting.strIniFile = m_strIniFile;
	m_ieSetting.strPassword = m_strPassword;

	if ( m_bImport )
	{
		m_pIniDB->ImportTables(&m_iniImportDB, m_ieSetting);
	}
	else
	{	
		BOOL bOk = m_pIniDB->ExportTables(m_ieSetting);
		if ( !bOk )
		{
			AfxMessageBox(_M("Export failed!"));
			return;
		}
		else
		{
			CString strMsg;
			strMsg.Format(_M("Export to file %s successfully!"), m_ieSetting.strIniFile);
			AfxMessageBox(strMsg);
		}
	}

	CDialog::OnOK();
}

BOOL CImportExportDialog::OnCellCtrlClicked(CListCtrl* pListCtrl, int nRow, int nCol, CellFormat* pCellFormat)
{
	return TRUE;
}

void CImportExportDialog::OnBnClickedButtonBrowse()
{
	CFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY,
		"All Files (*.*)|*.*||" );
	if ( dlg.DoModal() != IDOK )
		return;

	SetDlgItemText(IDC_EDIT_INI_FILE, dlg.GetPathName());

	if ( m_bImport )
	{
		GetDlgItem(IDC_BUTTON_RELOAD)->EnableWindow(TRUE);
		OnBnClickedButtonReload();
	}

	
	GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
}

void CImportExportDialog::AddTableToList(const CString& strTableName)
{
	int nRow = m_listTable.AppendEmptyRow();

	CellFormat* pCellFormat = m_listTable.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellCtrlCheckBox;
	pCellFormat->bChecked = TRUE;
	m_listTable.SetCell(nRow, 0, _T(""), pCellFormat);

	m_listTable.SetCell(nRow, 1, strTableName, NULL);

	pCellFormat = m_listTable.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellTextEdit;
	m_listTable.SetCell(nRow, 2, strTableName, pCellFormat);
}

void CImportExportDialog::OnBnClickedButtonAll()
{
	UpdateData();
	ASSERT(m_pIniDB->IsAdminUser());

	if ( m_bImport )
	{
		if ( !m_pIniDB->LoadIniFile(m_strIniFile, m_bEncrypt) )
			return;

		if ( !m_pIniDB->LoadUserInfo() )
			return;

		m_pIniDB->PrepareForCurrentUser();
		m_bImportAll = TRUE;
	}
	else
	{
		CString strAdminPassword = m_pIniDB->m_dbOptions.strAdminPassword;
		m_pIniDB->m_dbOptions.strAdminPassword = m_strPassword;
		BOOL bOk = m_pIniDB->SaveIniFile(m_strIniFile, m_bEncrypt);
		m_pIniDB->m_dbOptions.strAdminPassword = strAdminPassword;

		if ( !bOk )
		{
			AfxMessageBox(_M("Export failed!"));
			return;
		}
		else
		{
			CString strMsg;
			strMsg.Format(_M("Export to file %s successfully!"), m_strIniFile);
			AfxMessageBox(strMsg);
		}
	}

	CDialog::OnOK();
}


void CImportExportDialog::OnBnClickedButtonReload()
{
	UpdateData();

	m_listTable.DeleteAllItems(FALSE);
	GetDlgItem(IDC_BUTTON_ALL)->EnableWindow(FALSE);

	if ( !m_iniImportDB.LoadIniFile(m_strIniFile, m_bEncrypt) )
		return;

	if ( m_iniImportDB.m_dbOptions.strAdminPassword != m_strPassword )
	{
		AfxMessageBox(_M("Password mismatch!"));
		CEdit* pEdit = (CEdit*) GetDlgItem(IDC_EDIT_PASSWORD);
		pEdit->SetFocus();
		pEdit->SetSel(0, m_strPassword.GetLength());
		return;
	}

	if ( m_iniImportDB.m_dbOptions.bIsCompleteDB && !m_pIniDB->IsAdminUser())
	{
		AfxMessageBox(_M("This file contains entire database information, you can NOT import it!"));
		return;
	}

	for ( list<CTable<CString>*>::iterator it = m_iniImportDB.m_listTables.begin(); it != m_iniImportDB.m_listTables.end(); it ++ )
	{
		AddTableToList((*it)->strName);
	}

	GetDlgItem(IDC_BUTTON_ALL)->EnableWindow(m_iniImportDB.m_dbOptions.bIsCompleteDB);

}

void CImportExportDialog::SetUIText()
{
	if ( m_bImport )
	{
		SetWindowText(_M("Import"));
		SetDlgItemText(IDC_BUTTON_ALL, _M("Import All"));
		SetDlgItemText(IDOK, _M("Import Tables"));
	}
	else
	{
		SetWindowText(_M("Export"));
		SetDlgItemText(IDC_BUTTON_ALL, _M("Export All"));
		SetDlgItemText(IDOK, _M("Export Tables"));
	}

	SetDlgItemText(IDC_STATIC_FILE, _M("File"));
	SetDlgItemText(IDC_STATIC_PASSWORD_PROTECT, _M("Password Protect"));
	SetDlgItemText(IDC_STATIC_CONFIRM, _M("Confirm Password"));
	SetDlgItemText(IDC_BUTTON_RELOAD, _M("Reload"));
	SetDlgItemText(IDC_CHECK_ENCRYPT, _M("Encrypt"));

	
	SetDlgItemText(IDCANCEL, _M("Cancel"));
}

