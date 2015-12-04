#pragma once
#include "Ini.h"
#include "CustomListStruct.h"
#include "TableCache.h"

const char	TABLE_NAME_USER_PRIVILEDGE[]	= _T("UserPriviledge");
const char	TABLE_NAME_USER[]				= _T("User");

const char	SECTION_NAME_TABLE[]			= _T("Table");
const char	SECTION_NAME_OPTION[]			= _T("Option");

const char	KEY_FORMATTER_TABLE[]			= _T("Table%02d");	// Max 99 tables
const char	KEY_FORMATTER_COLUMN[]			= _T("Column%02d");	// Max 99 columns per table
const char	KEY_FORMATTER_DATA[]			= _T("Data%04d");	// Max 9999 datas per table

const char	DEFAULT_ACCESS_INFO_COLUMN[]	= _T("*_*120*_*Text*_*Combo*_*Left*_*NoAccess*_*NoAccess;ReadOnly;ReadWrite;ReadWriteExport;Owner;*_*");

const char	TABLE_ACCESS_ERROR[]			= _T("Error");
const char	TABLE_ACCESS_NO[]				= _T("NoAccess");
const char	TABLE_ACCESS_READONLY[]			= _T("ReadOnly");
const char	TABLE_ACCESS_READWRITE[]		= _T("ReadWrite");
const char	TABLE_ACCESS_READWRITEEXPORT[]	= _T("ReadWriteExport");
const char	TABLE_ACCESS_OWNER[]			= _T("Owner");

const char	KEY_LAST_LOGIN_USER[]			= _T("LastLoginUser");
const char	KEY_TABLE_COUNT[]				= _T("TableCount");
const char	KEY_COLUMN_COUNT[]				= _T("ColumnCount");
const char	KEY_DATA_COUNT[]				= _T("DataCount");
const char	KEY_ADMIN_PASSWORD[]			= _T("AdminPassword");
const char	KEY_ADMIN_OPTION[]				= _T("AdminOption");
const char  KEY_IS_COMPLETE_DB[]			= _T("IsCompleteDB");

const char	KEY_USER_OPTIONS_LAST_OPENED_TABLE[]	= _T("LastOpenedTable_");
const char	KEY_USER_OPTIONS_LANGUAGE[]				= _T("Language");
const char	KEY_USER_OPTIONS_QUERY_CASE_SENSITIVE[]	= _T("QueryCaseSensitive");
const char	KEY_USER_OPTIONS_QUERY_WHEN_EDITTING[]	= _T("QueryWhenEditting");
const char	KEY_USER_OPTIONS_AUTO_SAVE[]			= _T("AutoSaveEnabled");
const char	KEY_USER_OPTIONS_AUTO_SAVE_INTERVAL[]	= _T("AutoSaveInterval");
const char	KEY_USER_OPTIONS_AUTO_SAVE_FILE[]		= _T("AutoSaveFile");
const char	KEY_USER_OPTIONS_PROMPT_FOR_SAVE[]		= _T("PromptForSave");


template <typename T>
class CTable
{
public:
	CTable()
	{
		strName = _T("");
		nIndex	= -1;		// Start from 1, consistent with ini file
		listStyle.Reset();
		pData = NULL;
	}

	~CTable()
	{
		if ( pData )
		{
			delete pData;
			pData = NULL;
		}
	}

	CString		strName;
	int			nIndex;
	CustomListStyle listStyle;
	I2DCache<T>*	pData;
};

enum TableAccessPriviledge
{
	tableAccessError			= 0,
	tableAccessNo				= 1,
	tableAccessRead				= 2,
	tableAccessReadWrite		= 3,
	tableAccessReadWriteExport	= 4,
	tableAccessOwner			= 5,
};

struct ImportExportSetting
{
	CString		strIniFile;
	CString		strPassword;
	BOOL		bEncrypt;
	map<CString,CString>	mapTables;
};

struct DBOptions
{
	CString		strAdminPassword;
	CString		strLastLoginUser;
	BOOL		bIsCompleteDB;
};

struct UserOptions
{
	CString		strLastOpenedTable;
	CString		strLanguage;
	BOOL		bQueryCaseSensitive;
	BOOL		bQueryWhenEditting;
	BOOL		bAutoSave;
	int			nAutoSaveInterval;
	CString		strAutoSaveFile;
	BOOL		bPromptForSave;
};
//
//class CUser
//{
//public:
//	CUser()
//	{
//		nIndex = -1;
//	}
//
//	BOOL FromString(CString strUserInfo)
//	{
//		int nPos = strUserInfo.Find("*_*");
//		if ( nPos < 0 )
//			return FALSE;
//		strUsername = strUserInfo.Left(nPos);
//		strUserInfo = strUserInfo.Right(strUserInfo.GetLength()-nPos-3);
//
//		nPos = strUserInfo.Find("*_*");
//		if ( nPos < 0 )
//			return FALSE;
//		strPassword = strUserInfo.Left(nPos);
//
//		strDescription = strUserInfo.Right(strUserInfo.GetLength()-nPos-3);
//
//		return TRUE;
//	}
//
//	CString ToString()
//	{
//		CString strUserInfo;
//		strUserInfo.Format(_T("%s*_*%s*_*%s"), strUsername, strPassword, strDescription);
//		return strUserInfo;
//	}
//
//	CString	strUsername;
//	CString strPassword;
//	CString strDescription;
//
//	int		nIndex;
//
//};

class CIniDB
{
public:
	CIniDB(void);
	virtual ~CIniDB(void);

public:
	CIni*			m_pWorkingIniFile;
	CIni			m_iniMainFile;
	CIni			m_iniExportFile;

	list<CTable<CString>*>	m_listTables;
	list<CTable<CString>*>	m_listTablesAccessible;

	CString			m_strIniFileName;
	CString			m_strCurrentUser;
	CString			m_strCurrentUserPassword;
	CString			m_strAdminUsername;


	ICacheList*			m_pCacheList;
	CTable<CString>*	m_pCurrentTable;

	map<CString,TableAccessPriviledge>	m_mapTableAccessPriviledge;

	DBOptions		m_dbOptions;
	UserOptions		m_userOptions;
	

public:
	// Ini operation
	CString	GetTableSectionName(const CString& strTableName);
	BOOL SaveIniFile(const CString& strFileName, BOOL bEncrypt);
	BOOL LoadIniFile(const CString& strFileName, BOOL bEncrypt);
	BOOL ExportTables(ImportExportSetting& ieSetting);
	BOOL ImportTables(CIniDB* pIniImport, ImportExportSetting& ieSetting);


	// Common operation
	void ApplyQueryOptions();
	void PrepareForCurrentUser();
	BOOL LoadUserInfo();
	void ClearMemory();
	void SetCacheList(ICacheList* pCacheList) { m_pCacheList = pCacheList; }
	void UILanguageChanged();

	// Table Operation
	BOOL SaveCurrentTableStyleFromUI();
	int	 GetCurrentTableVisibleRowCount();
	CString	GetCurrentTableName();
	BOOL IsTableExist(const CString& strName){ return FindTable(strName) != NULL; }
	CTable<CString>* FindTable(int nIndex);
	CTable<CString>* FindTable(const CString& strName);
	BOOL ShowTable(const CString& strTableName);
	CTable<CString>* CreateEmptyTable(const CString& strTableName, BOOL bCreateFromIniFile);
	void InsertTable(CTable<CString>* pTable);
	BOOL DeleteTable(CTable<CString>* pTable);
	BOOL ChangeTableName(CTable<CString>* pTable, const CString& strNewTableName);

	BOOL LoadAllTableStyle();
	BOOL SaveAllTables();

	BOOL SaveTableData(CTable<CString>* pTable);
	BOOL SaveTableStyle(CTable<CString>* pTable);

	BOOL LoadTableData(CTable<CString>* pTable);
	BOOL LoadTableStyle(CTable<CString>* pTable);

	BOOL CanTableEdit(const CString& strTableName);
	BOOL CanTableDelete(const CString& strTableName);
	BOOL CanTableExport(const CString& strTableName);
	BOOL CanTableWrite(const CString& strTableName);

	TableAccessPriviledge	GetTableAccessPriviledge(const CString& strTableName);

	// Users operation
	BOOL IsAdminUser();
	BOOL CanUserLogin(const CString& strUsername, const CString& strPassword);
	BOOL ChangePassword(const CString& strUsername, const CString& strNewPassword);
	BOOL ChangeUsername(const CString& strOldName, const CString& strNewName);


	// UserTable
	BOOL AppendDefaultAccessInfoForTable(const CString& strTableName);
	BOOL AppendDefaultAccessInfoForUser(const CString& strUsername);
	BOOL DeleteAccessInfoOfTable(const CString& strTableName);
	BOOL DeleteAccessInfoOfUser(const CString& strUsername);
	
	// Option
	BOOL LoadDBOptions();
	BOOL LoadUserOptions();
	BOOL SaveDBOptions();
	BOOL SaveUserOptions();

};

