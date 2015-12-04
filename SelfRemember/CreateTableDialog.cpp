// CreateTableDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "CreateTableDialog.h"


// CCreateTableDialog dialog

IMPLEMENT_DYNAMIC(CCreateTableDialog, CDialog)

CCreateTableDialog::CCreateTableDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateTableDialog::IDD, pParent)
	, m_strTableName(_T(""))
	, m_nHeaderHeight(48)
	, m_nRowHeight(24)
	, m_bEnableFiilter(TRUE)
	, m_bLeftClickCellToEdit(TRUE)
	, m_bHighlightChangedCell(TRUE)
	, m_bAllowMultiSelection(FALSE)
	, m_bEditFirstCellAfterNewRowIsAdded(TRUE)
	, m_bEditNextCellAfterReturnIsHit(TRUE)
{
	m_bCreate = TRUE;
	m_pIniDB = NULL;
}

CCreateTableDialog::~CCreateTableDialog()
{
}

void CCreateTableDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TABLE_NAME, m_strTableName);
	DDX_Text(pDX, IDC_EDIT_HEADER_HEIGHT, m_nHeaderHeight);
	DDV_MinMaxInt(pDX, m_nHeaderHeight, 20, 60);
	DDX_Text(pDX, IDC_EDIT_ROW_HEIGHT, m_nRowHeight);
	DDV_MinMaxInt(pDX, m_nRowHeight, 20, 60);
	DDX_Check(pDX, IDC_CHECK_ENABLE_FILTER, m_bEnableFiilter);
	DDX_Check(pDX, IDC_CHECK_LEFT_CLICK_CELL_TO_EDIT, m_bLeftClickCellToEdit);
	DDX_Check(pDX, IDC_CHECK_HIGHLIGHT_CHANGED_CELL, m_bHighlightChangedCell);
	DDX_Check(pDX, IDC_CHECK_ALLOW_MULTI_SELECTION, m_bAllowMultiSelection);
	DDX_Check(pDX, IDC_CHECK_AUTO_EDIT_FIRST, m_bEditFirstCellAfterNewRowIsAdded);
	DDX_Control(pDX, IDC_LIST_COLUMN, m_listColumn);
	DDX_Check(pDX, IDC_CHECK_AUTO_EDIT_NEXT, m_bEditNextCellAfterReturnIsHit);
}


BEGIN_MESSAGE_MAP(CCreateTableDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateTableDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateTableDialog message handlers
BOOL CCreateTableDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CustomColumn		gridColumn;
	CustomColumnList	gridColumnsList;

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Name");
	gridColumn.nWidth = 100;
	gridColumn.strDefaultValue = _M("Column Name");
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Width");
	gridColumn.nWidth = 40;
	gridColumn.strDefaultValue = _T("60");
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Data Type");
	gridColumn.nWidth = 60;
	gridColumn.ctrlType = cellTextCombo;
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsNumber, _T("Number"), _T("Number")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsText, _T("Text"), _T("Text")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsTime, _T("Date"), _T("Date")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsDate, _T("Time"), _T("Time")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsDateTime, _T("DateTime"), _T("DateTime")));
	gridColumn.strDefaultValue = _T("Text");
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Edit Type");
	gridColumn.nWidth = 60;
	gridColumn.ctrlType = cellTextCombo;
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(cellTextEdit, _T("Edit"), _T("Edit")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(cellTextNotes, _T("Notes"), _T("Notes")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(cellTextCombo, _T("Combo"), _T("Combo")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsTime, _T("Date"), _T("Date")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsDate, _T("Time"), _T("Time")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(compareAsDateTime, _T("DateTime"), _T("DateTime")));
	gridColumn.strDefaultValue = _T("Edit");
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Align");
	gridColumn.nWidth = 60;
	gridColumn.ctrlType = cellTextCombo;
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(LVCFMT_LEFT, _T("Left"), _T("Left")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(LVCFMT_CENTER, _T("Center"), _T("Center")));
	gridColumn.dropdownValueList.push_back(CustomComboboxItem(LVCFMT_RIGHT, _T("Right"), _T("Right")));
	gridColumn.strDefaultValue = _T("Left");
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Default Value");
	gridColumn.nWidth = 80;
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Select List");
	gridColumn.nWidth = 150;
	gridColumnsList.push_back(gridColumn);

	CString strColumnOrder = "0;1;2;3;4;5;6";
	m_listColumn.SetColumnInfo(gridColumnsList, strColumnOrder);

	// Set Row Bk Color
	m_listColumn.AddDefaultRowColor(RGB(239,239,239));
	m_listColumn.AddDefaultRowColor(RGB(207,207,207));

	// Height
	m_listColumn.SetHeaderHeight(22);
	m_listColumn.SetRowHeigt(22);

	m_listColumn.EnableFilter(FALSE);
	m_listColumn.SetMultipleSelection(TRUE);
	m_listColumn.SetShowSelection(TRUE);
	m_listColumn.SetHighlightChangedCellText(TRUE);
	m_listColumn.SetSelectThenEdit(TRUE);
	m_listColumn.SetEditFirstCellAfterNewRowIsAdded(TRUE);
	m_listColumn.SetEditNextCellAfterReturnIsHit(TRUE);

	m_listColumn.m_defaultListFormat.cellType = cellTextEdit;

	CustomListContextMenu contectMenu;
	contectMenu.FromString(_M("08Copy*_*00*_*01Delete All Selected Columns*_*00*_*02Insert Column*_*03Append Column*_*00*_*04Move Top*_*05Move Up*_*06Move Down*_*07Move Bottom*_*00*_*09Select All*_*10Select None*_*"));
	m_listColumn.SetContextMenu(contectMenu);

	m_listColumn.SetUser(this);

	if ( !m_bCreate )
	{
		LoadTableStyle(m_pIniDB->m_pCurrentTable);
		m_pIniDB->m_pCurrentTable->pData->CacheStartColumnOperation();
	}

	SetUIText();
	
	return TRUE;
}

void CCreateTableDialog::OnBnClickedOk()
{
	if ( m_listColumn.GetItemCount() == 0 )
	{
		AfxMessageBox(_M("You must set at least one column!"));
		return;
	}

	CString strOldTableName = m_strTableName;
	UpdateData();

	m_strTableName.TrimLeft();
	m_strTableName.TrimRight();
	if ( m_strTableName.GetLength() == 0 ) 
	{
		AfxMessageBox(_M("Table name can NOT be empty!"));
		return;
	}

	if ( m_bCreate || strOldTableName.Compare(m_strTableName) != 0 )
	{
		if ( m_pIniDB->IsTableExist(m_strTableName) )
		{
			AfxMessageBox(_M("Table name already exist!"));
			return;
		}
	}

	if ( !m_bCreate )
	{
		m_pIniDB->m_pCurrentTable->pData->CacheEndColumnOperation();
	}

	SaveTableStyle();

	OnOK();
}

BOOL CCreateTableDialog::SaveTableStyle()
{
	CTable<CString>*	pTable;
	if ( m_bCreate )
	{
		pTable = m_pIniDB->CreateEmptyTable(m_strTableName,FALSE);
	}
	else
	{
		pTable = m_pIniDB->m_pCurrentTable;
		if ( pTable->strName != m_strTableName )
		{
			m_pIniDB->ChangeTableName(pTable, m_strTableName);
		}
	}

	pTable->listStyle.Reset();
	pTable->listStyle.nRowHeight = m_nRowHeight;
	pTable->listStyle.nHeaderHeight = m_nHeaderHeight;
	pTable->listStyle.bEnableFilter = m_bEnableFiilter;
	pTable->listStyle.bClickSelectedCellToEdit = m_bLeftClickCellToEdit;
	pTable->listStyle.bHighlightChangedCellText = m_bHighlightChangedCell;
	pTable->listStyle.bAllowMultlSelection = m_bAllowMultiSelection;
	pTable->listStyle.bEditFirstCellAfterNewRowIsAdded = m_bEditFirstCellAfterNewRowIsAdded;
	pTable->listStyle.bEditNextCellAfterReturnIsHit = m_bEditNextCellAfterReturnIsHit;

	// Set Row Bk Color
	pTable->listStyle.vecRowColors.push_back(RGB(239,239,239));
	pTable->listStyle.vecRowColors.push_back(RGB(207,207,207));

	CString strColumnInfo;
	int nCount = m_listColumn.GetItemCount();
	for ( int i = 0; i < nCount; i ++ )
	{
		strColumnInfo = _T("");

		for ( int j = 0; j < m_listColumn.m_ctrlHeader.GetItemCount(); j ++ )
		{
			strColumnInfo += m_listColumn.GetItemText(i,j) + _T("*_*");
		}

		pTable->listStyle.AddColumn(strColumnInfo);
	}

	// Temp code
	pTable->listStyle.contextMenu.CreateDefaultMenu();
	return TRUE;
}

BOOL CCreateTableDialog::LoadTableStyle(CTable<CString>* pTable)
{
	m_strTableName = pTable->strName;
	

	m_nRowHeight = pTable->listStyle.nRowHeight;
	m_nHeaderHeight = pTable->listStyle.nHeaderHeight;
	m_bEnableFiilter = pTable->listStyle.bEnableFilter;
	m_bLeftClickCellToEdit = pTable->listStyle.bClickSelectedCellToEdit;
	m_bHighlightChangedCell = pTable->listStyle.bHighlightChangedCellText;
	m_bAllowMultiSelection = pTable->listStyle.bAllowMultlSelection;
	m_bEditFirstCellAfterNewRowIsAdded = pTable->listStyle.bEditFirstCellAfterNewRowIsAdded;
	m_bEditNextCellAfterReturnIsHit = pTable->listStyle.bEditNextCellAfterReturnIsHit;

	UpdateData(FALSE);

	CString strKey, strColumnInfo, strTemp;
	int nCount = pTable->listStyle.gridColumnsList.size();
	int nPos, nCol;
	for ( int i = 0; i < nCount; i ++ )
	{
		m_listColumn.AppendDefaultRow();
		strColumnInfo = pTable->listStyle.gridColumnsList[i].ToString();

		nCol = 0;
		nPos = strColumnInfo.Find("*_*");
		while ( nPos != -1 )
		{
			strTemp = strColumnInfo.Left(nPos);
			m_listColumn.SetCellText(i, nCol++, strTemp, TRUE, FALSE);
			strColumnInfo = strColumnInfo.Right(strColumnInfo.GetLength()-nPos-3);
			nPos = strColumnInfo.Find("*_*");
		}
	}

	return TRUE;
}

BOOL CCreateTableDialog::OnRowDeleted(CListCtrl* pListCtrl, int nRow)
{ 
	if ( m_bCreate )
		return TRUE;

	m_pIniDB->m_pCurrentTable->pData->CacheDeleteColumn(nRow);
	return TRUE; 
}

BOOL CCreateTableDialog::OnRowSwapped(CListCtrl* pListCtrl, int nRow1, int nRow2)
{ 
	if ( m_bCreate )
		return TRUE;

	m_pIniDB->m_pCurrentTable->pData->CacheSwapColumn(nRow1, nRow2);
	return TRUE; 
}

BOOL CCreateTableDialog::OnRowAdded(CListCtrl* pListCtrl, int nRow)
{
	if ( m_bCreate )
		return TRUE;

	m_pIniDB->m_pCurrentTable->pData->CacheInsertColumn(nRow);
	return TRUE;
}

void CCreateTableDialog::SetUIText()
{
	SetDlgItemText(IDC_STATIC_TABLE_PROPERTY, _M("Table Property"));
	SetDlgItemText(IDC_STATIC_TABLE_NAME, _M("Table Name"));
	SetDlgItemText(IDC_STATIC_HEADER_HEIGHT, _M("Header Height"));
	SetDlgItemText(IDC_STATIC_ROW_HEIGHT, _M("Row Height"));
	SetDlgItemText(IDC_CHECK_ENABLE_FILTER, _M("Enable Filter"));
	SetDlgItemText(IDC_CHECK_LEFT_CLICK_CELL_TO_EDIT, _M("Select Then Edit"));
	SetDlgItemText(IDC_CHECK_HIGHLIGHT_CHANGED_CELL, _M("Highlight Changed Cell"));
	SetDlgItemText(IDC_CHECK_ALLOW_MULTI_SELECTION, _M("Allow Multi Selection"));
	SetDlgItemText(IDC_CHECK_AUTO_EDIT_FIRST, _M("Edit First Cell After New Row Added"));
	SetDlgItemText(IDC_CHECK_AUTO_EDIT_NEXT, _M("Edit Next Cell After Hit Return"));
	SetDlgItemText(IDC_STATIC_COLUMN_DEFINITION, _M("Column Definition"));
	SetDlgItemText(IDOK, _M("OK"));
	SetDlgItemText(IDCANCEL, _M("Cancel"));

	if ( m_bCreate )
	{
		SetWindowText(_M("Create Table"));
	}
	else
	{
		SetWindowText(_M("Edit Table"));
	}
}
