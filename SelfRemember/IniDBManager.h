#pragma once

#include "IniDB.h"

class IIniDBManagerUser
{
public:
	virtual void OnListChanged(){};
};

class CIniDBManager : public ICustomListUser
{
public:
	CIniDBManager(void);
	virtual ~CIniDBManager(void);

	CIniDB	m_iniDB;

	CWinThread*	m_pAutoSaveThread;
	IIniDBManagerUser*	m_pUser;
	BOOL	m_bAppExiting;

public:
	BOOL	IsUIEnglish();
	void	StopAutoSaveThread();
	void	SetUser(IIniDBManagerUser* pUser) { m_pUser = pUser; }
	void	AutoSave();
	void	ApplySettings();

	void	Login(const CString& strUsername, const CString& strPassword);
	BOOL	LoadIniFile(const CString& strFileName, BOOL bEncrypt);

	CIniDB*	GetDBInstance() { return &m_iniDB; }

public:
	virtual BOOL OnFilterTextChanged(CListCtrl* pListCtrl, int nCol, const char* szNewText);
	virtual BOOL OnCellTextChanging(CListCtrl* pListCtrl, int nRow, int nCol, CellFormat* pCellFormat, const CString& strOldValue, CString& strNewValue);
	virtual BOOL OnRowAdded(CListCtrl* pListCtrl, int nRow);
	virtual BOOL OnRowDeleting(CListCtrl* pListCtrl, int nRow);
	virtual BOOL OnRowDeleted(CListCtrl* pListCtrl, int nRow);
};
