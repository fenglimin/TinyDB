#pragma once


// CCreateTableDialog dialog
#include "CustomListCtrl.h"
#include "IniDB.h"

class CCreateTableDialog : public CDialog, public ICustomListUser
{
	DECLARE_DYNAMIC(CCreateTableDialog)

public:
	CCreateTableDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateTableDialog();

	void	SetUIText();
	BOOL	SaveTableStyle();
	BOOL	LoadTableStyle(CTable<CString>* pTable);

// Dialog Data
	enum { IDD = IDD_DIALOG_CREATE_TABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTableName;
	int m_nHeaderHeight;
	int m_nRowHeight;
	BOOL m_bEnableFiilter;
	BOOL m_bLeftClickCellToEdit;
	BOOL m_bHighlightChangedCell;
	BOOL m_bAllowMultiSelection;
	BOOL m_bEditFirstCellAfterNewRowIsAdded;
	CIniDB* m_pIniDB;

	BOOL m_bCreate;

	CCustomListCtrl m_listColumn;

	afx_msg void OnBnClickedOk();

public:
	virtual BOOL OnRowAdded(CListCtrl* pListCtrl, int nRow);
	virtual BOOL OnRowDeleted(CListCtrl* pListCtrl, int nRow);
	virtual BOOL OnRowSwapped(CListCtrl* pListCtrl, int nRow1, int nRow2);
	BOOL m_bEditNextCellAfterReturnIsHit;
};
