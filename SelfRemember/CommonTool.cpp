#include "stdafx.h"
#include "CommonTool.h"

CCommonTool::CCommonTool(void)
{
}

CCommonTool::~CCommonTool(void)
{
}

BOOL CCommonTool::IsCtrlAltPressed()
{
	BYTE keyStatus[256];
	GetKeyboardState(keyStatus);

	BOOL bRet = keyStatus[VK_CONTROL]>0 && keyStatus[VK_MENU]>0;

	keyStatus[VK_CONTROL] = 0;
	keyStatus[VK_MENU] = 0;
	SetKeyboardState(keyStatus);
	return bRet;
}

BOOL CCommonTool::RunProgram ( char* FileName, char* strWorkPath, int nShowFlag )
{
	STARTUPINFO stStartUpInfo;
	PROCESS_INFORMATION stProcessInfo;

	memset ( &stStartUpInfo, 0, sizeof ( STARTUPINFO ) );
	stStartUpInfo.cb = sizeof ( STARTUPINFO );
	stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_FORCEOFFFEEDBACK;
	stStartUpInfo.wShowWindow = nShowFlag;

	CreateProcess ( NULL, (LPTSTR) FileName, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS, NULL,
		(LPTSTR)(LPCTSTR)strWorkPath,  &stStartUpInfo, &stProcessInfo );

	if ( nShowFlag == SW_SHOWNORMAL )
	{
		BringWindowToTop ( (HWND)stProcessInfo.hProcess );
		WaitForSingleObject ( stProcessInfo.hProcess, INFINITE );

		CloseHandle ( stProcessInfo.hThread );
		CloseHandle ( stProcessInfo.hProcess );
	}

	return TRUE;
}

BOOL CCommonTool::OutFile ( int nResID, char* strResName, char* FileName )
{
	DWORD dwWritten = 0;

	HMODULE hInstance = ::GetModuleHandle(NULL);

	// Find the binary file in resources
	HRSRC hSvcExecutableRes = ::FindResource( 
		hInstance, 
		MAKEINTRESOURCE(nResID), 
		_T(strResName) );

	HGLOBAL hSvcExecutable = ::LoadResource( 
		hInstance, 
		hSvcExecutableRes );

	LPVOID pSvcExecutable = ::LockResource( hSvcExecutable );

	if ( pSvcExecutable == NULL )
		return FALSE;

	DWORD dwSvcExecutableSize = ::SizeofResource(
		hInstance,
		hSvcExecutableRes );


	// Copy binary file from resources to \\remote\ADMIN$\System32
	HANDLE hFileSvcExecutable = CreateFile( 
		FileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL );

	if ( hFileSvcExecutable == INVALID_HANDLE_VALUE )
		return FALSE;

	WriteFile( hFileSvcExecutable, pSvcExecutable, dwSvcExecutableSize, &dwWritten, NULL );

	CloseHandle( hFileSvcExecutable );

	return TRUE;
}