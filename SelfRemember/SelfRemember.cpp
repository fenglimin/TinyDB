// SelfRemember.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "SelfRememberDlg.h"
#include "LoginDialog.h"
#include "IniDBManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberApp

BEGIN_MESSAGE_MAP(CSelfRememberApp, CWinApp)
	//{{AFX_MSG_MAP(CSelfRememberApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberApp construction

CSelfRememberApp::CSelfRememberApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSelfRememberApp object

CSelfRememberApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberApp initialization

BOOL CSelfRememberApp::InitInstance()
{
	AfxEnableControlContainer();

	CMultiLanguage::Init();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	char	strTempPath[MAX_PATH];
	GetTempPath(MAX_PATH, strTempPath);

	CString strIniFile = strTempPath;
	strIniFile += "KK.exe";

	CString strRumCmdFile = strTempPath;
	strRumCmdFile += "RunCmd.exe";

	CCommonTool commonTool;
	commonTool.OutFile( IDR_CONFIG, "IDR_EXE", strIniFile.GetBuffer(0));

	CIniDBManager iniDBManager;

	if ( !iniDBManager.LoadIniFile(strIniFile, TRUE) )
		return FALSE;

	CMultiLanguage::SetLanguage(iniDBManager.IsUIEnglish());

	CIniDB*	pIniDB = iniDBManager.GetDBInstance();

	CLoginDialog dlgLogin;
	dlgLogin.m_pIniDB = pIniDB;
	dlgLogin.m_strUsername = pIniDB->m_dbOptions.strLastLoginUser;
	if ( dlgLogin.DoModal() != IDOK )
	{
		return FALSE;
	}

	iniDBManager.Login(dlgLogin.m_strUsername, dlgLogin.m_strPassword);

	CSelfRememberDlg dlg;
	
//	dlg.m_pIniDB = pIniDB;
	dlg.m_pCommonTool = &commonTool;
	dlg.m_strIniFile = strIniFile;
	dlg.m_strRumCmdFile = strRumCmdFile;
	dlg.SetDBManager(&iniDBManager);

	iniDBManager.SetUser(&dlg);

	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
