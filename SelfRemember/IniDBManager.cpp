#include "stdafx.h"
#include "IniDBManager.h"

static UINT AutoSaveThread(LPVOID pParam)
{
	CIniDBManager* pManager = reinterpret_cast<CIniDBManager*>(pParam);
	ASSERT(pManager);

	int i;
	int nInterval = 0;
	while ( pManager->m_iniDB.m_userOptions.bAutoSave && !pManager->m_bAppExiting)
	{
		nInterval = pManager->m_iniDB.m_userOptions.nAutoSaveInterval;
		for ( i = 0; i <  nInterval*60 && pManager->m_iniDB.m_userOptions.bAutoSave && !pManager->m_bAppExiting; i ++ )
		{
			// Auto save interval is changed, reset the loop
			if ( nInterval != pManager->m_iniDB.m_userOptions.nAutoSaveInterval )
			{
				i = 0;
				nInterval = pManager->m_iniDB.m_userOptions.nAutoSaveInterval;
			}
			Sleep(1000);
		}

		if ( i == nInterval*60 )
		{
			pManager->AutoSave();
		}
	}

	return 0;
}

CIniDBManager::CIniDBManager(void)
{
	m_pAutoSaveThread = NULL;
	m_pUser = NULL;
	m_bAppExiting = FALSE;
}

CIniDBManager::~CIniDBManager(void)
{
}

void CIniDBManager::AutoSave()
{
	if ( m_iniDB.IsAdminUser() )
	{
		m_iniDB.SaveIniFile(m_iniDB.m_userOptions.strAutoSaveFile, TRUE);
	}
	else
	{
		ImportExportSetting ieSetting;
		ieSetting.strIniFile = m_iniDB.m_userOptions.strAutoSaveFile;
		ieSetting.strPassword = m_iniDB.m_strCurrentUserPassword;
		ieSetting.bEncrypt = TRUE;

		for ( list<CTable<CString>*>::iterator it = m_iniDB.m_listTablesAccessible.begin(); it != m_iniDB.m_listTablesAccessible.end(); it ++ )
		{
			if ( m_iniDB.CanTableExport((*it)->strName) )
			{
				ieSetting.mapTables[(*it)->strName] = (*it)->strName;
			}
		}

		m_iniDB.ExportTables(ieSetting);
	}
}

void CIniDBManager::ApplySettings()
{
	if ( m_iniDB.m_userOptions.bAutoSave )
	{
		if ( m_pAutoSaveThread == NULL )
		{
			m_pAutoSaveThread =	AfxBeginThread(AutoSaveThread, this);
		}
	}
	else
	{
		if ( m_pAutoSaveThread != NULL )
		{
			WaitForSingleObject(m_pAutoSaveThread->m_hThread, INFINITE);
			m_pAutoSaveThread = NULL;
		}
	}

	CMultiLanguage::SetLanguage(IsUIEnglish());
	m_iniDB.ApplyQueryOptions();

	m_iniDB.UILanguageChanged();
}

void CIniDBManager::Login( const CString& strUsername, const CString& strPassword )
{
	m_iniDB.m_strCurrentUser = strUsername;
	m_iniDB.m_strCurrentUserPassword = strPassword;

	m_iniDB.PrepareForCurrentUser();
}

BOOL CIniDBManager::LoadIniFile( const CString& strFileName, BOOL bEncrypt )
{
	if ( !m_iniDB.LoadIniFile(strFileName, TRUE) )
		return FALSE;

	if ( !m_iniDB.LoadUserInfo() )
		return FALSE;

	return TRUE;
}

BOOL CIniDBManager::OnCellTextChanging(CListCtrl* pListCtrl, int nRow, int nCol, CellFormat* pCellFormat, const CString& strOldValue, CString& strNewValue)
{
	CString strCurrentTable = m_iniDB.GetCurrentTableName();

	// We are editing Users table
	if (  strCurrentTable == TABLE_NAME_USER )
	{
		// We are editing username
		if ( nCol == 0 )
		{
			return m_iniDB.ChangeUsername(strOldValue, strNewValue);			
		}
	}
	else if ( strCurrentTable == TABLE_NAME_USER_PRIVILEDGE )
	{
		if ( nCol == 0 )
		{
			return FALSE;
		}

		CTable<CString>* pTable = m_iniDB.FindTable(TABLE_NAME_USER_PRIVILEDGE);
		ASSERT(pTable);
		ASSERT(pTable->pData);
		if ( pTable->listStyle.gridColumnsList[nCol].strHeaderCaption == m_iniDB.m_strCurrentUser )
		{
			if ( strOldValue == TABLE_ACCESS_OWNER )
			{
				if ( AfxMessageBox(_T("You will lose the ownership of this table, are you sure to continue?"), MB_YESNO) == IDNO )
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CIniDBManager::OnFilterTextChanged(CListCtrl* pListCtrl, int nCol, const char* szNewText)
{
	if ( m_pUser )
	{
		m_pUser->OnListChanged();
	}

	return TRUE;
}

BOOL CIniDBManager::OnRowAdded(CListCtrl* pListCtrl, int nRow)
{
	if ( m_pUser )
	{
		m_pUser->OnListChanged();
	}

	// We are editing Users table
	if ( m_iniDB.GetCurrentTableName() == TABLE_NAME_USER )
	{
		return m_iniDB.AppendDefaultAccessInfoForUser(_T(""));			
	}

	return TRUE;
}

BOOL CIniDBManager::OnRowDeleting(CListCtrl* pListCtrl, int nRow)
{
	// We are editing Users table
	if ( m_iniDB.GetCurrentTableName() == TABLE_NAME_USER )
	{
		CString strUsername = pListCtrl->GetItemText(nRow,0);
		if ( !m_iniDB.DeleteAccessInfoOfUser(strUsername) )
			return FALSE;
	}

	return TRUE;
}

BOOL CIniDBManager::OnRowDeleted(CListCtrl* pListCtrl, int nRow)
{
	if ( m_pUser )
	{
		m_pUser->OnListChanged();
	}

	return TRUE;
}

void CIniDBManager::StopAutoSaveThread()
{
	if ( m_pAutoSaveThread != NULL )
	{
		m_bAppExiting = TRUE;
		WaitForSingleObject(m_pAutoSaveThread->m_hThread, INFINITE);
		m_pAutoSaveThread = NULL;
	}
}

BOOL CIniDBManager::IsUIEnglish()
{
	return (m_iniDB.m_userOptions.strLanguage.CompareNoCase(_T("English")) == 0);
}
