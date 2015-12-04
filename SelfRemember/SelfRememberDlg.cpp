// SelfRememberDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "SelfRememberDlg.h"
#include "ImportExportDialog.h"
#include "Wininet.h"
#include "CreateTableDialog.h"
#include "TableCache.h"
#include "UserOptionDialog.h"
#include "ChangePasswordDialog.h"
#include "DynDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_M("About TinyDB"));
	SetDlgItemText(IDC_STATIC_VERSION, _M("TinyDB Version 1.0"));
	SetDlgItemText(IDC_STATIC_DEVELOPPER, _M("Developer : Sail Feng,  mailflm@163.com"));

	SetDlgItemText(IDOK, _M("OK"));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberDlg dialog

CSelfRememberDlg::CSelfRememberDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSelfRememberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelfRememberDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDBManager = NULL;
	m_pIniDB = NULL;
	m_pCommonTool = NULL;
}

void CSelfRememberDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelfRememberDlg)
	DDX_Control(pDX, IDC_LIST_LAUNCH, m_listTable);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_TABLES, m_comboTables);
	DDX_Control(pDX, IDC_BUTTON_CREATE_TABLE, m_btnCreateTable);
	DDX_Control(pDX, IDC_BUTTON_EDIT_TABLE, m_btnEditTable);
	DDX_Control(pDX, IDC_BUTTON_DELETE_TABLE, m_btnDeleteTable);
}

BEGIN_MESSAGE_MAP(CSelfRememberDlg, CDialog)
	//{{AFX_MSG_MAP(CSelfRememberDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CSelfRememberDlg::OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CSelfRememberDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_TABLE, &CSelfRememberDlg::OnBnClickedButtonCreateTable)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TABLE, &CSelfRememberDlg::OnBnClickedButtonEditTable)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TABLE, &CSelfRememberDlg::OnBnClickedButtonDeleteTable)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLES, &CSelfRememberDlg::OnCbnSelchangeComboTables)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PASSWORD, &CSelfRememberDlg::OnBnClickedButtonChangePassword)
	ON_BN_CLICKED(IDC_BUTTON_USER_OPTION, &CSelfRememberDlg::OnBnClickedButtonUserOption)
	ON_BN_CLICKED(IDOK, &CSelfRememberDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberDlg message handlers

BOOL CSelfRememberDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, _M(strAboutMenu));
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetUIText();

	ReloadTableList();

	if ( m_pIniDB->m_userOptions.strLastOpenedTable.GetLength() > 0 )
	{
		m_comboTables.SelectString(-1, m_pIniDB->m_userOptions.strLastOpenedTable);
		OnCbnSelchangeComboTables();
	}

#ifdef _DEBUG
	GetDlgItem(IDC_BUTTON_TEST)->ShowWindow(SW_SHOW);
#endif


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSelfRememberDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSelfRememberDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSelfRememberDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSelfRememberDlg::OnButtonTest() 
{
	//for ( int i = 0; i < 1000; i ++ )
	//{
	//	m_listTable.AppendDefaultRow();
	//}

	//ShowTableItemCount();

	//CDynDialogEx dlg(this);		//create the dynamic dialog, using this as parent window

	//dlg.SetWindowTitle(_T("Set Date Range"));

	//CRect rcFrom(10,10,30,20);
	//dlg.AddDlgControl(_T("STATIC"), _T("From :"), STYLE_STATIC, EXSTYLE_STATIC, rcFrom);

	//COleDateTime oldDateTimeStart;
	//oldDateTimeStart.SetDate(1975,12,29);
	//CRect rcStart(30,8,80,19);
	//dlg.AddDlgControl(_T("SysDateTimePick32"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | DTS_RIGHTALIGN, 0, rcStart, (void*)&oldDateTimeStart);

	//CRect rcTo(90,10,100,20);
	//dlg.AddDlgControl(_T("STATIC"), _T("To :"), STYLE_STATIC, EXSTYLE_STATIC, rcTo);

	//COleDateTime oldDateTimeEnd;
	//CRect rcEnd(110,8,160,19);
	//dlg.AddDlgControl(_T("SysDateTimePick32"), _T(""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | DTS_RIGHTALIGN, 0, rcEnd, (void*)&oldDateTimeEnd);

	//dlg.DoModal();

//	CString a = _M("Create Table");

//	GetClipboardData();

	m_listTable.PasteCellTextFromClipboard(0,0);
}

void CSelfRememberDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CSelfRememberDlg::UpdateIniFileInResource()
{
	char strFileName[MAX_PATH];
	GetModuleFileName ( NULL, strFileName, MAX_PATH );

	CString strCmdLine = "\"" + m_strRumCmdFile + "\" 2*";
	strCmdLine += strFileName;
	strCmdLine += "*IDR_EXE*134*";
	strCmdLine += m_strIniFile;

	m_pIniDB->SaveIniFile(m_strIniFile, TRUE);

	m_pCommonTool->OutFile( IDR_EXE_RUN_CMD, "IDR_EXE", m_strRumCmdFile.GetBuffer(0));
	m_pCommonTool->RunProgram(strCmdLine.GetBuffer(0), "C:\\", SW_HIDE);
}

void CSelfRememberDlg::OnBnClickedButtonExport()
{
	m_pIniDB->SaveAllTables();

	CImportExportDialog dlgImportExport;
	dlgImportExport.m_bImport = FALSE;
	dlgImportExport.m_pIniDB = m_pIniDB;
	dlgImportExport.DoModal();
}

void CSelfRememberDlg::OnBnClickedButtonImport()
{
	CImportExportDialog dlgImportExport;
	dlgImportExport.m_bImport = TRUE;
	dlgImportExport.m_pIniDB = m_pIniDB;
	if ( dlgImportExport.DoModal() != IDOK )
		return;

	ReloadTableList();

	if ( dlgImportExport.m_bImportAll )
	{
		m_listTable.SetEmptyTable(FALSE);
		m_comboTables.SelectString(-1, m_pIniDB->m_userOptions.strLastOpenedTable);
		OnCbnSelchangeComboTables();
	}
	else
	{
		m_comboTables.SelectString(-1, m_pIniDB->GetCurrentTableName());
	}
}

void CSelfRememberDlg::OnBnClickedButtonCreateTable()
{
	CCreateTableDialog dlg;
	dlg.m_pIniDB = m_pIniDB;
	if ( dlg.DoModal() != IDOK )
	{
		return;
	}

	ReloadTableList();
	m_comboTables.SelectString(-1, dlg.m_strTableName);
	OnCbnSelchangeComboTables();
}

void CSelfRememberDlg::OnBnClickedButtonEditTable()
{
	m_pIniDB->SaveCurrentTableStyleFromUI();

	int nSel = m_comboTables.GetCurSel();
	CCreateTableDialog dlg;
	dlg.m_pIniDB = m_pIniDB;
	dlg.m_bCreate = FALSE;

	if ( dlg.DoModal() != IDOK )
	{
		return;
	}	

	ReloadTableList();
	m_comboTables.SelectString(-1, dlg.m_strTableName);
	m_pIniDB->m_pCurrentTable = NULL; // Set the current table to NULL to force reload
	m_pIniDB->ShowTable(dlg.m_strTableName);
}

void CSelfRememberDlg::OnBnClickedButtonDeleteTable()
{
	if ( AfxMessageBox(_M("Are you sure to delete this table?"), MB_YESNO) != IDYES )
		return;

	int nItem = m_comboTables.GetCurSel();
	int nTableIndex = (int)m_comboTables.GetItemData(nItem);
	if ( !m_pIniDB->DeleteTable(m_pIniDB->m_pCurrentTable) )
		return;

	m_pIniDB->m_pCurrentTable = NULL; // Set the current table to NULL to force reload
	ReloadTableList();

	if ( m_comboTables.GetCount() > 0 )
	{
		m_comboTables.SetCurSel(0);
		OnCbnSelchangeComboTables();
	}
	else
	{
		m_pIniDB->m_pCacheList->SetEmptyTable(FALSE);
		Invalidate(TRUE);
	}

}

void CSelfRememberDlg::OnCbnSelchangeComboTables()
{
	m_pIniDB->SaveCurrentTableStyleFromUI();

	CString strNewTable;
	m_comboTables.GetWindowText(strNewTable);
	if ( m_pIniDB->GetCurrentTableName().Compare(strNewTable) == 0 )
	{
		return;
	}

	m_pIniDB->ShowTable(strNewTable);
	m_btnEditTable.EnableWindow(m_pIniDB->CanTableEdit(strNewTable));
	m_btnDeleteTable.EnableWindow(m_pIniDB->CanTableDelete(strNewTable));
	m_listTable.SetReadOnly(!m_pIniDB->CanTableWrite(strNewTable));

	ShowTableItemCount();
}

void CSelfRememberDlg::ReloadTableList()
{
	m_comboTables.ResetContent();
	int nItem;
	int nData = 1;

	for ( list<CTable<CString>*>::iterator it = m_pIniDB->m_listTablesAccessible.begin(); it != m_pIniDB->m_listTablesAccessible.end(); it ++ )
	{
		nItem = m_comboTables.AddString((*it)->strName);
		m_comboTables.SetItemData(nItem, (*it)->nIndex);
	}
}

void CSelfRememberDlg::ShowTableItemCount()
{
	CString strIndicator;
	strIndicator.Format(_T("%d / %d"), m_listTable.GetItemCount(), m_pIniDB->GetCurrentTableVisibleRowCount());
	SetDlgItemText(IDC_STATIC_INDICATOR, strIndicator);
}

void CSelfRememberDlg::OnBnClickedButtonChangePassword()
{
	CChangePasswordDialog dlg;
	dlg.m_pIniDB = m_pIniDB;
	if ( dlg.DoModal() != IDOK )
		return;

	if ( m_pIniDB->ChangePassword(m_pIniDB->m_strCurrentUser, dlg.m_strNewPassword ) )
		AfxMessageBox(_M("Change password successfully!"));
	else
		AfxMessageBox(_M("Change password failed!"));
}


void CSelfRememberDlg::OnBnClickedButtonUserOption()
{
	CUserOptionDialog dlg(&m_pIniDB->m_userOptions);
	if ( dlg.DoModal() != IDOK )
		return;

	m_pDBManager->ApplySettings();

	SetUIText();
}

void CSelfRememberDlg::SetDBManager( CIniDBManager* pDBManager )
{
	ASSERT(pDBManager);

	m_pDBManager = pDBManager;
	m_pIniDB = pDBManager->GetDBInstance();

	m_listTable.SetUser((ICustomListUser*)pDBManager);

	m_pIniDB->SetCacheList(&m_listTable);

	m_pDBManager->ApplySettings();
}

void CSelfRememberDlg::OnListChanged()
{
	ShowTableItemCount();	
}

void CSelfRememberDlg::SetUIText()
{
	SetDlgItemText(IDC_STATIC_TABLE, _M("Table"));
	SetDlgItemText(IDC_BUTTON_CREATE_TABLE, _M("Create Table"));
	SetDlgItemText(IDC_BUTTON_EDIT_TABLE, _M("Edit Table"));
	SetDlgItemText(IDC_BUTTON_DELETE_TABLE, _M("Delete Table"));
	SetDlgItemText(IDC_BUTTON_EXPORT, _M("Export"));
	SetDlgItemText(IDC_BUTTON_IMPORT, _M("Import"));
	SetDlgItemText(IDC_BUTTON_CHANGE_PASSWORD, _M("Change Password"));
	SetDlgItemText(IDC_BUTTON_USER_OPTION, _M("Options"));
	SetDlgItemText(IDOK, _M("Exit"));

	CString strWindowText;
	strWindowText.Format(_M("TinyDB - Welcome, %s"), m_pIniDB->m_strCurrentUser);
	SetWindowText(strWindowText);
}

void CSelfRememberDlg::OnClose()
{
	OnCloseDB();
}


void CSelfRememberDlg::OnBnClickedOk()
{
	OnCloseDB();
}

void CSelfRememberDlg::OnCloseDB()
{
	BOOL bSave = TRUE;
	if ( m_pDBManager->m_iniDB.m_userOptions.bPromptForSave )
	{
		UINT uRet = AfxMessageBox(_M("Do you want to save the changes?"), MB_YESNOCANCEL);
		if ( uRet == IDCANCEL )
			return;
		
		if ( uRet == IDNO )
		{
			bSave = FALSE;
		}
	}

	m_pDBManager->StopAutoSaveThread();
	if ( bSave)
	{
		UpdateIniFileInResource();
	}

	CMultiLanguage::Clear();

	CDialog::OnOK();
}

void CSelfRememberDlg::OnCancel()
{
	OnCloseDB();
}
