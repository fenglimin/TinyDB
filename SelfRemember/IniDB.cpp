#include "stdafx.h"
#include "IniDB.h"

CIniDB::CIniDB(void)
{
	m_pWorkingIniFile = &m_iniMainFile;
	m_pCacheList = NULL;
	m_pCurrentTable = NULL;

	m_strAdminUsername = _T("admin");

	m_mapTableAccessPriviledge[TABLE_ACCESS_ERROR]				= tableAccessError;
	m_mapTableAccessPriviledge[TABLE_ACCESS_NO]					= tableAccessNo;
	m_mapTableAccessPriviledge[TABLE_ACCESS_READONLY]			= tableAccessRead;
	m_mapTableAccessPriviledge[TABLE_ACCESS_READWRITE]			= tableAccessReadWrite;
	m_mapTableAccessPriviledge[TABLE_ACCESS_READWRITEEXPORT]	= tableAccessReadWriteExport;
	m_mapTableAccessPriviledge[TABLE_ACCESS_OWNER]				= tableAccessOwner;

}

CIniDB::~CIniDB(void)
{
	ClearMemory();
}

CString CIniDB::GetTableSectionName(const CString& strTableName)
{
	CString strSectionName;
	strSectionName.Format(_T("%s_%s"), SECTION_NAME_TABLE, strTableName);
	return strSectionName;
}

void CIniDB::ClearMemory()
{
	CTable<CString>* pTable;
	for ( list<CTable<CString>*>::iterator it = m_listTables.begin(); it != m_listTables.end(); it++ )
	{
		pTable = *it;
		delete pTable;
	}

	m_listTables.clear();
	m_listTablesAccessible.clear();
}

CString CIniDB::GetCurrentTableName()
{
	if ( m_pCurrentTable == NULL )
	{
		return _T("");
	}

	return m_pCurrentTable->strName;
}

int CIniDB::GetCurrentTableVisibleRowCount()
{
	if ( m_pCurrentTable == NULL )
	{
		return 0;
	}

	ASSERT(m_pCurrentTable->pData);
	int nCount = m_pCurrentTable->pData->CacheGetRowCount();
	int nRet = nCount;
	RowStatus rowstatus;
	for ( int i = 0; i < nCount; i ++ )
	{
		rowstatus = m_pCurrentTable->pData->CacheGetRowstatus(i);
		if ( rowstatus != rowNew && rowstatus != rowLoaded )
		{
			nRet--;
		}
	}

	return nRet;
}

BOOL CIniDB::LoadIniFile(const CString& strFileName, BOOL bEncrypt)
{
	m_pWorkingIniFile->Clear();
	ClearMemory();
	m_pCurrentTable = NULL;

	if ( !m_pWorkingIniFile->Read ( strFileName, bEncrypt ) )
	{
		AfxMessageBox(_M("Error loading database file!"));
		return FALSE;
	}

	m_strIniFileName = strFileName;

	if ( !LoadDBOptions() )
		return FALSE;

	if ( !LoadUserOptions() )
		return FALSE;

	if ( !LoadAllTableStyle() )
		return FALSE;

	return TRUE;
}

BOOL CIniDB::LoadAllTableStyle()
{
	int nTableCount = 0;
	if ( !m_pWorkingIniFile->GetValue(SECTION_NAME_TABLE, KEY_TABLE_COUNT, nTableCount) )
	{
		AfxMessageBox(_M("Error loading table information!"));
		return FALSE;
	}

	CTable<CString>* pTable = NULL;
	m_listTables.clear();
	CString strKey, strValue;
	for ( int i = 0; i < nTableCount; i ++ )
	{
		strKey.Format(KEY_FORMATTER_TABLE, i+1);
		m_pWorkingIniFile->GetValue(SECTION_NAME_TABLE, strKey, strValue);

		pTable = CreateEmptyTable(strValue, TRUE);
		LoadTableStyle(pTable);
	}

	return TRUE;
}

BOOL CIniDB::SaveCurrentTableStyleFromUI()
{
	// User may change the column width
	if ( m_pCurrentTable )
	{
		m_pCacheList->UpdateTableSytle(m_pCurrentTable->listStyle);
	}

	return TRUE;
}

BOOL CIniDB::ShowTable(const CString& strTableName)
{
	if ( strTableName.GetLength() == 0 )
	{
		AfxMessageBox(_T("Internal error, empty table name!"));
		return FALSE;
	}

	BOOL bSameTable = strTableName.Compare(GetCurrentTableName()) == 0;
	if ( bSameTable )
	{
		return TRUE;
	}

	m_pCurrentTable = FindTable(strTableName);
	ASSERT(m_pCurrentTable);

	if ( !LoadTableData(m_pCurrentTable) )
		return FALSE;

	m_pCacheList->SetCacheData(m_pCurrentTable->pData);
	m_pCacheList->SetTableStyle(m_pCurrentTable->listStyle);
	m_pCacheList->FilterDataFromCache();



	return TRUE;
}

BOOL CIniDB::SaveIniFile(const CString& strFileName, BOOL bEncrypt)
{
	SaveUserOptions();
	SaveDBOptions();
	SaveAllTables();

	return m_pWorkingIniFile->Write(strFileName, bEncrypt);
}

BOOL CIniDB::ExportTables(ImportExportSetting& ieSetting)
{
	CTable<CString>* pTable;
	for ( map<CString,CString>::iterator it = ieSetting.mapTables.begin(); it != ieSetting.mapTables.end(); it++ )
	{
		pTable = FindTable(it->second);
		ASSERT(pTable);
		LoadTableData(pTable);
	}

	m_iniExportFile.Clear();
	m_pWorkingIniFile = &m_iniExportFile;

	CString strKey;
	int nIndex = 1;
	for ( map<CString,CString>::iterator it = ieSetting.mapTables.begin(); it != ieSetting.mapTables.end(); it++ )
	{
		pTable = FindTable(it->second);

		pTable->strName = it->first;
		strKey.Format(KEY_FORMATTER_TABLE, nIndex++);
		m_pWorkingIniFile->SetValue(SECTION_NAME_TABLE, strKey, pTable->strName);

		SaveTableStyle(pTable);
		SaveTableData(pTable);

		pTable->strName = it->second;
	}

	m_pWorkingIniFile->SetValue(SECTION_NAME_TABLE, KEY_TABLE_COUNT, (int)ieSetting.mapTables.size());
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_ADMIN_PASSWORD, ieSetting.strPassword);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_IS_COMPLETE_DB, FALSE);

	BOOL bOk = m_pWorkingIniFile->Write(ieSetting.strIniFile, ieSetting.bEncrypt);

	m_pWorkingIniFile = &m_iniMainFile;

	return bOk;
}

BOOL CIniDB::ImportTables(CIniDB* pIniImport, ImportExportSetting& ieSetting)
{
	CTable<CString>* pTable;
	for ( map<CString,CString>::iterator it = ieSetting.mapTables.begin(); it != ieSetting.mapTables.end(); it++ )
	{
		pTable = pIniImport->FindTable(it->second);
		ASSERT(pTable);
		pIniImport->LoadTableData(pTable);

		pTable->strName = it->first;
		InsertTable(pTable);
		pIniImport->m_listTables.remove(pTable);
	}

	return TRUE;
}

BOOL CIniDB::LoadTableStyle(CTable<CString>* pTable)
{
	ASSERT ( pTable->strName.GetLength() != 0 );
	CString strSectionName = GetTableSectionName(pTable->strName);

	pTable->listStyle.Reset();
	m_pWorkingIniFile->GetValue(strSectionName, _T("RowHeight"), pTable->listStyle.nRowHeight);
	m_pWorkingIniFile->GetValue(strSectionName, _T("HeaderHeight"), pTable->listStyle.nHeaderHeight);
	m_pWorkingIniFile->GetValue(strSectionName, _T("EnableFilter"), pTable->listStyle.bEnableFilter);
	m_pWorkingIniFile->GetValue(strSectionName, _T("LeftClickCellToEdit"), pTable->listStyle.bClickSelectedCellToEdit);
	m_pWorkingIniFile->GetValue(strSectionName, _T("HighlightChangedCell"), pTable->listStyle.bHighlightChangedCellText);
	m_pWorkingIniFile->GetValue(strSectionName, _T("AllowMultiSelection"), pTable->listStyle.bAllowMultlSelection);
	m_pWorkingIniFile->GetValue(strSectionName, _T("EditFirstCellAfterNewRowIsAdded"), pTable->listStyle.bEditFirstCellAfterNewRowIsAdded);
	m_pWorkingIniFile->GetValue(strSectionName, _T("EditNextCellAfterReturnIsHit"), pTable->listStyle.bEditNextCellAfterReturnIsHit);

	CString strValue;
	m_pWorkingIniFile->GetValue(strSectionName, _T("ContextMenu"), strValue);
	pTable->listStyle.contextMenu.FromString(strValue);

	// Set Row Bk Color
	pTable->listStyle.vecRowColors.push_back(RGB(239,239,239));
	pTable->listStyle.vecRowColors.push_back(RGB(207,207,207));

	CustomColumn		gridColumn;
	int	nValue = 0;

	CString strKey, strColumnInfo, strTemp, strColumnOrder, strOrder, strComboItem; 
	m_pWorkingIniFile->GetValue(strSectionName, KEY_COLUMN_COUNT, nValue);
	for ( int i = 0; i < nValue; i ++ )
	{
		gridColumn.Reset();

		strKey.Format(KEY_FORMATTER_COLUMN, i+1);
		m_pWorkingIniFile->GetValue(strSectionName, strKey, strColumnInfo);

		pTable->listStyle.AddColumn(strColumnInfo);
	}

	return TRUE;
}


BOOL CIniDB::LoadTableData(CTable<CString>* pTable)
{
	ASSERT(pTable);
	ASSERT(pTable->strName.GetLength() != 0);

	if ( pTable->pData != NULL )
	{
		return TRUE;
	}

	CTableCache<CString>* pTableCache = new CTableCache<CString>;
	pTableCache->CacheSetEmptyValue(_T(""));
	pTableCache->CacheSetColumnCount(pTable->listStyle.gridColumnsList.size());
	pTable->pData = pTableCache;

	CString strSectionName = GetTableSectionName(pTable->strName);

	// Load data
	CString strDataInfo, strKey;
	int nPos, nCol, nValue, nCacheIndex;

	nValue = 0;
	m_pWorkingIniFile->GetValue(strSectionName, KEY_DATA_COUNT, nValue);
	for ( int i = 0; i < nValue; i ++ )
	{
		strKey.Format(KEY_FORMATTER_DATA, i+1);
		m_pWorkingIniFile->GetValue(strSectionName, strKey, strDataInfo);

		nCacheIndex = pTable->pData->CacheAppendEmptyRow(rowLoaded);

		nCol = 0;
		nPos = strDataInfo.Find("*_*");
		while ( nPos != -1 )
		{
			pTable->pData->CacheSetItemText(nCacheIndex, nCol++, strDataInfo.Left(nPos));
			strDataInfo = strDataInfo.Right(strDataInfo.GetLength()-nPos-3);
			nPos = strDataInfo.Find("*_*");
		}
	}

	return TRUE;
}

BOOL CIniDB::SaveAllTables()
{
	CString strKey;
	CTable<CString>* pTable;
	int nIndex = 1;
	for ( list<CTable<CString>*>::iterator it = m_listTables.begin(); it != m_listTables.end(); it++ )
	{
		pTable = *it;
		strKey.Format(KEY_FORMATTER_TABLE, nIndex++);
		m_pWorkingIniFile->SetValue(SECTION_NAME_TABLE, strKey, pTable->strName);

		SaveTableStyle(pTable);
		SaveTableData(pTable);
	}

	m_pWorkingIniFile->SetValue(SECTION_NAME_TABLE, KEY_TABLE_COUNT, (int)m_listTables.size());

	return TRUE;
}

BOOL CIniDB::SaveTableStyle(CTable<CString>* pTable)
{
	CString strKey;
	CString strSectionName = GetTableSectionName(pTable->strName);

	m_pWorkingIniFile->SetValue(strSectionName, _T("RowHeight"), pTable->listStyle.nRowHeight);
	m_pWorkingIniFile->SetValue(strSectionName, _T("HeaderHeight"), pTable->listStyle.nHeaderHeight);
	m_pWorkingIniFile->SetValue(strSectionName, _T("EnableFilter"), pTable->listStyle.bEnableFilter);
	m_pWorkingIniFile->SetValue(strSectionName, _T("LeftClickCellToEdit"), pTable->listStyle.bClickSelectedCellToEdit);
	m_pWorkingIniFile->SetValue(strSectionName, _T("HighlightChangedCell"), pTable->listStyle.bHighlightChangedCellText);
	m_pWorkingIniFile->SetValue(strSectionName, _T("AllowMultiSelection"), pTable->listStyle.bAllowMultlSelection);
	m_pWorkingIniFile->SetValue(strSectionName, _T("EditFirstCellAfterNewRowIsAdded"), pTable->listStyle.bEditFirstCellAfterNewRowIsAdded);
	m_pWorkingIniFile->SetValue(strSectionName, _T("EditNextCellAfterReturnIsHit"), pTable->listStyle.bEditNextCellAfterReturnIsHit);

	m_pWorkingIniFile->SetValue(strSectionName, _T("ContextMenu"), pTable->listStyle.contextMenu.ToString());

	if ( pTable == m_pCurrentTable )
	{
		SaveCurrentTableStyleFromUI();
	}

	int nColCount = pTable->listStyle.gridColumnsList.size();
	for ( int i = 0; i < nColCount; i ++ )
	{
		strKey.Format(KEY_FORMATTER_COLUMN, i+1);
		m_pWorkingIniFile->SetValue(strSectionName, strKey, pTable->listStyle.gridColumnsList[i].ToString());
	}
	m_pWorkingIniFile->SetValue(strSectionName, KEY_COLUMN_COUNT, nColCount);

	return TRUE;
}

BOOL CIniDB::SaveTableData(CTable<CString>* pTable)
{
	if ( pTable->strName.GetLength() == 0 )
		return FALSE;

	if ( !pTable->pData )
		return FALSE;

	CString strSectionName = GetTableSectionName(pTable->strName);
	int nDataCount = pTable->pData->CacheGetRowCount();
	int nColumnCount = pTable->pData->CacheGetColCount();

	CString strData, strKey;
	int nSaveCount = 1;
	for ( int i = 0; i < nDataCount; i ++ )
	{
		strData = _T("");
		if ( pTable->pData->CacheGetRowstatus(i) == rowDeleted )
			continue;

		for ( int j = 0; j < nColumnCount; j ++ )
		{
			strData += pTable->pData->CacheGetItemText(i, j) + _T("*_*");
		}

		strKey.Format(KEY_FORMATTER_DATA, nSaveCount++);
		m_pWorkingIniFile->SetValue(strSectionName, strKey, strData);
	}

	m_pWorkingIniFile->SetValue(strSectionName, KEY_DATA_COUNT, nSaveCount-1);

	return TRUE;
}

BOOL CIniDB::DeleteTable(CTable<CString>* pTable)
{
	if ( !DeleteAccessInfoOfTable(pTable->strName) )
		return FALSE;

	CString strSectionName = GetTableSectionName(pTable->strName);
	m_pWorkingIniFile->DeleteSection(strSectionName);

	m_listTablesAccessible.remove(pTable);
	m_listTables.remove(pTable);
	delete pTable;
	pTable = NULL;

	return TRUE;
}

CTable<CString>* CIniDB::CreateEmptyTable(const CString& strTableName, BOOL bCreateFromIniFile)
{
	CTable<CString>* pTable = new CTable<CString>;
	m_listTables.insert(m_listTables.end(), pTable);
	pTable->strName = strTableName;
	pTable->nIndex = m_listTables.size();

	if ( !bCreateFromIniFile)
	{
		m_listTablesAccessible.insert(m_listTablesAccessible.end(), pTable);
		AppendDefaultAccessInfoForTable(strTableName);
	}

	return pTable;
}

// The table is created by user or imported by current user, so the current user is the owner of this table
void CIniDB::InsertTable(CTable<CString>* pTable)
{
	ASSERT(pTable);
	ASSERT(pTable->pData);

	m_listTables.insert(m_listTables.end(), pTable);
	pTable->nIndex = m_listTables.size();

	m_listTablesAccessible.insert(m_listTablesAccessible.end(), pTable);
	AppendDefaultAccessInfoForTable(pTable->strName);
}

CTable<CString>* CIniDB::FindTable(int nIndex)
{
	for ( list<CTable<CString>*>::iterator it = m_listTables.begin(); it != m_listTables.end(); it++ )
	{
		if ( (*it)->nIndex == nIndex )
			return *it;
	}

	return NULL;
}

CTable<CString>* CIniDB::FindTable(const CString& strName)
{
	for ( list<CTable<CString>*>::iterator it = m_listTables.begin(); it != m_listTables.end(); it++ )
	{
		if ( (*it)->strName == strName )
			return *it;
	}

	return NULL;
}

BOOL CIniDB::ChangeTableName(CTable<CString>* pTable, const CString& strNewTableName)
{
	ASSERT(IsTableExist(pTable->strName));
	ASSERT(!IsTableExist(strNewTableName));

	CTable<CString>* pUserTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pUserTable);
	ASSERT(pUserTable->pData);

	int	nCount = pUserTable->pData->CacheGetRowCount();
	for ( int i = 0 ; i < nCount; i ++ )
	{
		if ( pUserTable->pData->CacheGetItemText(i,0) == pTable->strName )
		{
			pUserTable->pData->CacheSetItemText(i,0,strNewTableName);
			pTable->strName = strNewTableName;
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format(_M("Can NOT find table %s!"), strNewTableName);
	AfxMessageBox(strMsg + _T("  ") + _M("Update table access information failed!"));
	return FALSE;
}

BOOL CIniDB::LoadUserInfo()
{
	CTable<CString>* pTable = FindTable(TABLE_NAME_USER);
	if ( pTable == NULL )
	{
		AfxMessageBox(_M("Error loading user information!"));
		return FALSE;
	}
	LoadTableData(pTable);

	return TRUE;
}

void CIniDB::PrepareForCurrentUser()
{
	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_LAST_OPENED_TABLE+m_strCurrentUser, m_userOptions.strLastOpenedTable);
	m_userOptions.strLastOpenedTable.TrimLeft();
	m_userOptions.strLastOpenedTable.TrimRight();

	CTable<CString>* pTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	if ( pTable == NULL )
	{
		AfxMessageBox(_M("Error loading user table access information!"));
		return;
	}
	LoadTableData(pTable);
	int nRowCount = pTable->pData->CacheGetRowCount();

	BOOL bFind = FALSE;

	m_listTablesAccessible.clear();
	TableAccessPriviledge access;
	for ( list<CTable<CString>*>::iterator it = m_listTables.begin(); it != m_listTables.end(); it ++ )
	{
		access = GetTableAccessPriviledge((*it)->strName);
		if (  access >= tableAccessRead )
		{
			m_listTablesAccessible.insert(m_listTablesAccessible.end(), *it);
			if ( (*it)->strName == m_userOptions.strLastOpenedTable )
			{
				bFind = TRUE;
			}
		}
		
		if ( access != tableAccessOwner )
		{
			// You are not the owner of this table, so mark as Hidden in UserPriviledge
			for ( int i = 0; i < nRowCount; i++ )
			{
				if ( pTable->pData->CacheGetItemText(i,0) == (*it)->strName )
				{
					pTable->pData->CacheSetRowstatus(i,rowHidden);
					break;
				}
			}
		}
	}

	if ( !bFind )
	{
		m_userOptions.strLastOpenedTable = _T("");
	}
}

TableAccessPriviledge CIniDB::GetTableAccessPriviledge(const CString& strTableName)
{
	if ( IsAdminUser() )
		return tableAccessOwner;

	CTable<CString>* pTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pTable);
	ASSERT(pTable->pData);

	int nRowCount = pTable->pData->CacheGetRowCount();
	int nColCount = pTable->pData->CacheGetColCount();

	for ( int i = 0; i < nRowCount; i ++ )
	{
		if ( pTable->pData->CacheGetItemText(i,0) == strTableName )
		{
			for ( int j = 1; j < nColCount; j ++ )
			{
				if ( pTable->listStyle.gridColumnsList[j].strHeaderCaption == m_strCurrentUser )
				{
					CString strAccess = pTable->pData->CacheGetItemText(i,j);
					return m_mapTableAccessPriviledge[strAccess];
				}

			}
		}
	}

	return tableAccessError;
}

BOOL CIniDB::IsAdminUser()
{
	return m_strCurrentUser == m_strAdminUsername;
}

BOOL CIniDB::CanUserLogin(const CString& strUsername, const CString& strPassword)
{
	CTable<CString>* pTable = FindTable(TABLE_NAME_USER);
	ASSERT(pTable);
	ASSERT(pTable->pData);

	if ( strUsername == m_strAdminUsername )
	{
		if ( strPassword != m_dbOptions.strAdminPassword )
		{
			AfxMessageBox(_M("Wrong password!"));
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	int nCount = pTable->pData->CacheGetRowCount();
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( pTable->pData->CacheGetItemText(i,0) == strUsername )
		{
			if ( pTable->pData->CacheGetItemText(i,1) == strPassword )
			{
				return TRUE;
			}
			else
			{
				AfxMessageBox(_M("Wrong password!"));
				return FALSE;
			}
		}
	}

	AfxMessageBox(_M("User does NOT exist!"));
	return FALSE;
}

BOOL CIniDB::ChangeUsername(const CString& strOldName, const CString& strNewName)
{
	if ( strOldName == m_strAdminUsername )
	{
		AfxMessageBox(_T("Can NOT change admin's name!"));
		return FALSE;
	}

	if ( strNewName == m_strAdminUsername )
	{
		CString strMsg;
		strMsg.Format(_M("The user name %s is reserved!"), strNewName);
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if ( strNewName == strOldName )
	{
		return FALSE;
	}

	CTable<CString>* pTable = FindTable(TABLE_NAME_USER);
	ASSERT(pTable);
	ASSERT(pTable->pData);

	int nCount = pTable->pData->CacheGetRowCount();
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( pTable->pData->CacheGetItemText(i,0) == strNewName )
		{
			CString strMsg;
			strMsg.Format(_M("The user name %s is already exist!"), strNewName);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}

	pTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pTable);

	nCount = pTable->listStyle.gridColumnsList.size();
	for ( int i = 0 ; i < nCount; i ++ )
	{
		if ( pTable->listStyle.gridColumnsList[i].strHeaderCaption == strOldName )
		{
			pTable->listStyle.gridColumnsList[i].strHeaderCaption = strNewName;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CIniDB::ChangePassword(const CString& strUsername, const CString& strNewPassword)
{
	if ( strUsername == m_strAdminUsername )
	{
		m_dbOptions.strAdminPassword = strNewPassword;
		return TRUE;
	}

	m_strCurrentUserPassword = strNewPassword;

	CTable<CString>* pTable = FindTable(TABLE_NAME_USER);
	ASSERT(pTable);
	ASSERT(pTable->pData);

	int nCount = pTable->pData->CacheGetRowCount();
	for ( int i = 0; i < nCount; i ++ )
	{
		if ( pTable->pData->CacheGetItemText(i,0) == strUsername )
		{
			pTable->pData->CacheSetItemText(i,1,strNewPassword);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CIniDB::AppendDefaultAccessInfoForTable(const CString& strTableName)
{
	CTable<CString>* pUserTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pUserTable);
	ASSERT(pUserTable->pData);

	int nCount = pUserTable->listStyle.gridColumnsList.size();
	ASSERT(nCount>0);
	int nRow = pUserTable->pData->CacheAppendEmptyRow(rowLoaded);
	pUserTable->pData->CacheSetItemText(nRow,0,strTableName);

	for ( int i = 1; i < nCount; i ++ )
	{
		if ( pUserTable->listStyle.gridColumnsList[i].strHeaderCaption == m_strCurrentUser )
			pUserTable->pData->CacheSetItemText(nRow,i,TABLE_ACCESS_OWNER);
		else
			pUserTable->pData->CacheSetItemText(nRow,i,pUserTable->listStyle.gridColumnsList[i].strDefaultValue);
	}

	return TRUE;
}

BOOL CIniDB::AppendDefaultAccessInfoForUser(const CString& strUsername)
{
	CTable<CString>* pUserTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pUserTable);
	ASSERT(pUserTable->pData);

	pUserTable->listStyle.AddColumn(strUsername+DEFAULT_ACCESS_INFO_COLUMN);
	int nColumnCount = pUserTable->listStyle.gridColumnsList.size();
	pUserTable->pData->CacheStartColumnOperation();
	pUserTable->pData->CacheInsertColumn(nColumnCount-1);
	pUserTable->pData->CacheEndColumnOperation();
	
	int nRowCount = pUserTable->pData->CacheGetRowCount();
	for ( int i = 0; i < nRowCount; i ++ )
	{
		if ( pUserTable->pData->CacheGetItemText(i,0) == TABLE_NAME_USER_PRIVILEDGE )
			pUserTable->pData->CacheSetItemText(i,nColumnCount-1,TABLE_ACCESS_READWRITE);
		else
			pUserTable->pData->CacheSetItemText(i,nColumnCount-1,TABLE_ACCESS_NO);
	}

	return TRUE;

}

BOOL CIniDB::DeleteAccessInfoOfTable(const CString& strTableName)
{
	CTable<CString>* pUserTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pUserTable);
	ASSERT(pUserTable->pData);

	int nCount = pUserTable->pData->CacheGetRowCount();
	ASSERT(nCount>0);

	for ( int i = 0; i < nCount; i ++ )
	{
		if ( pUserTable->pData->CacheGetItemText(i,0).Compare(strTableName) == 0 )
		{
			pUserTable->pData->CacheDeleteRow(i);
			return TRUE;
		}
	}

	CString strMsg;
	strMsg.Format(_M("Can NOT find table %s!"), strTableName);

	AfxMessageBox(strMsg + _T("  ") + _M("Remove table access information failed!"));
	return FALSE;
}

BOOL CIniDB::DeleteAccessInfoOfUser(const CString& strUsername)
{
	CTable<CString>* pUserTable = FindTable(TABLE_NAME_USER_PRIVILEDGE);
	ASSERT(pUserTable);
	ASSERT(pUserTable->pData);

	int i;
	int nColumnCount = pUserTable->listStyle.gridColumnsList.size();
	// Start from second column , the first column is "Table"
	for ( i = 1; i < nColumnCount; i ++ )
	{
		if ( pUserTable->listStyle.gridColumnsList[i].strHeaderCaption == strUsername )
			break;
	}

	if ( i == nColumnCount )
	{
		AfxMessageBox(_T("Internal error! Can NOT find user ") + strUsername + _T(" in UserTable!"));
		return FALSE;
	}

	int nDeleteColumn = i;
	for ( ; i < nColumnCount-1; i ++ )
	{
		pUserTable->listStyle.gridColumnsList[i] = pUserTable->listStyle.gridColumnsList[i+1];
	}
	pUserTable->listStyle.gridColumnsList.pop_back();

	pUserTable->pData->CacheStartColumnOperation();
	pUserTable->pData->CacheDeleteColumn(nDeleteColumn);
	pUserTable->pData->CacheEndColumnOperation();

	return TRUE;

}

BOOL CIniDB::LoadDBOptions()
{
	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_IS_COMPLETE_DB, m_dbOptions.bIsCompleteDB);

	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_ADMIN_PASSWORD, m_dbOptions.strAdminPassword);
	m_dbOptions.strAdminPassword.TrimLeft();
	m_dbOptions.strAdminPassword.TrimRight();

	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_LAST_LOGIN_USER, m_dbOptions.strLastLoginUser);
	m_dbOptions.strLastLoginUser.TrimLeft();
	m_dbOptions.strLastLoginUser.TrimRight();

	return TRUE;
}

BOOL CIniDB::LoadUserOptions()
{
	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_LANGUAGE, m_userOptions.strLanguage);
	m_userOptions.strLanguage.TrimLeft();
	m_userOptions.strLanguage.TrimRight();

	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_QUERY_CASE_SENSITIVE, m_userOptions.bQueryCaseSensitive);
	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_QUERY_WHEN_EDITTING, m_userOptions.bQueryWhenEditting);
	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_AUTO_SAVE, m_userOptions.bAutoSave);
	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_AUTO_SAVE_INTERVAL, m_userOptions.nAutoSaveInterval);

	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_AUTO_SAVE_FILE, m_userOptions.strAutoSaveFile);
	m_userOptions.strAutoSaveFile.TrimLeft();
	m_userOptions.strAutoSaveFile.TrimRight();

	m_pWorkingIniFile->GetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_PROMPT_FOR_SAVE, m_userOptions.bPromptForSave);

	return TRUE;
}

BOOL CIniDB::SaveDBOptions()
{
	if ( !IsAdminUser())
	{
		m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_LAST_LOGIN_USER, m_strCurrentUser);
	}

	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_IS_COMPLETE_DB, m_dbOptions.bIsCompleteDB);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_ADMIN_PASSWORD, m_dbOptions.strAdminPassword);

	return TRUE;
}

BOOL CIniDB::CanTableEdit(const CString& strTableName)
{
	if ( strTableName == TABLE_NAME_USER_PRIVILEDGE || strTableName == TABLE_NAME_USER )
		return FALSE;

	TableAccessPriviledge access = GetTableAccessPriviledge(strTableName);

	return access == tableAccessOwner;
}

BOOL CIniDB::CanTableDelete(const CString& strTableName)
{
	return CanTableEdit(strTableName);
}

BOOL CIniDB::CanTableExport(const CString& strTableName)
{
	if ( strTableName == TABLE_NAME_USER_PRIVILEDGE || strTableName == TABLE_NAME_USER )
		return FALSE;

	TableAccessPriviledge access = GetTableAccessPriviledge(strTableName);

	return access >= tableAccessReadWriteExport;
}

BOOL CIniDB::CanTableWrite(const CString& strTableName)
{
	TableAccessPriviledge access = GetTableAccessPriviledge(strTableName);

	return access >= tableAccessReadWrite;
}

BOOL CIniDB::SaveUserOptions()
{
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_LAST_OPENED_TABLE+m_strCurrentUser, GetCurrentTableName());
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_LANGUAGE, m_userOptions.strLanguage);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_QUERY_CASE_SENSITIVE, m_userOptions.bQueryCaseSensitive);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_QUERY_WHEN_EDITTING, m_userOptions.bQueryWhenEditting);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_AUTO_SAVE, m_userOptions.bAutoSave);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_AUTO_SAVE_INTERVAL, m_userOptions.nAutoSaveInterval);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_AUTO_SAVE_FILE, m_userOptions.strAutoSaveFile);
	m_pWorkingIniFile->SetValue(SECTION_NAME_OPTION, KEY_USER_OPTIONS_PROMPT_FOR_SAVE, m_userOptions.bPromptForSave);

	return TRUE;
}

void CIniDB::ApplyQueryOptions()
{
	m_pCacheList->SetQueryStyle(m_userOptions.bQueryCaseSensitive,m_userOptions.bQueryWhenEditting);
}

void CIniDB::UILanguageChanged()
{
	if ( m_pCacheList && m_pCurrentTable )
	{
		m_pCacheList->RebuildContextMenu(m_pCurrentTable->listStyle.contextMenu);
	}
}
