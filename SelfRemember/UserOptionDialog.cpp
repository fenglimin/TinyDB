// UserOptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SelfRemember.h"
#include "UserOptionDialog.h"


// CUserOptionDialog dialog

IMPLEMENT_DYNAMIC(CUserOptionDialog, CDialog)

CUserOptionDialog::CUserOptionDialog(UserOptions* pUserOption, CWnd* pParent /*=NULL*/)
	: CDialog(CUserOptionDialog::IDD, pParent)
{
	m_pUserOption = pUserOption;
}

CUserOptionDialog::~CUserOptionDialog()
{
}

void CUserOptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER_OPTION, m_listUserOption);
}


BEGIN_MESSAGE_MAP(CUserOptionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CUserOptionDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserOptionDialog message handlers
BOOL CUserOptionDialog::OnInitDialog()
{	
	CDialog::OnInitDialog();

	CString strText;

	CustomColumn		gridColumn;
	CustomColumnList	gridColumnsList;

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Property Name");
	gridColumn.nWidth = 200;
	gridColumnsList.push_back(gridColumn);

	gridColumn.Reset();
	gridColumn.strHeaderCaption = _M("Property Value");
	gridColumn.nWidth = 200;
	gridColumnsList.push_back(gridColumn);

	CString strColumnOrder = "0;1;";
	m_listUserOption.SetColumnInfo(gridColumnsList, strColumnOrder);

	// Set Row Bk Color
	m_listUserOption.AddDefaultRowColor(RGB(239,239,239));

	// Height
	m_listUserOption.SetHeaderHeight(24);
	m_listUserOption.SetRowHeigt(24);

	m_listUserOption.EnableFilter(FALSE);
	m_listUserOption.EnableSort(FALSE);
	m_listUserOption.SetMultipleSelection(FALSE);
	m_listUserOption.SetShowSelection(FALSE);
	m_listUserOption.SetHighlightChangedCellText(TRUE);
	m_listUserOption.SetSelectThenEdit(FALSE);
	m_listUserOption.SetEditNextCellAfterReturnIsHit(FALSE);
	m_listUserOption.SetEditFirstCellAfterNewRowIsAdded(FALSE);
	m_listUserOption.SetEnablePopupMenu(FALSE);

	m_listUserOption.SetUser(this);
	

	m_listUserOption.m_defaultListFormat.cellType = cellTextReadOnly;

	// Language
	int nRow = m_listUserOption.AppendEmptyRow();
	CellFormat* pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->lfWeight = FW_BOLD;
	m_listUserOption.SetCell(nRow,0,_M("Language"), pCellFormat);

	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Language"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellTextCombo;
	m_listUserOption.SetCell(nRow,1, m_pUserOption->strLanguage, pCellFormat);

	// Query
	nRow = m_listUserOption.AppendEmptyRow();
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->lfWeight = FW_BOLD;
	m_listUserOption.SetCell(nRow,0,_M("Query"), pCellFormat);

	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Case Sensitive"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellCtrlCheckBox;
	pCellFormat->bChecked = m_pUserOption->bQueryCaseSensitive;
	m_listUserOption.SetCell(nRow,1, _T(""), pCellFormat);
	
	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Query while editing"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellCtrlCheckBox;
	pCellFormat->bChecked = m_pUserOption->bQueryWhenEditting;
	m_listUserOption.SetCell(nRow,1, _T(""), pCellFormat);

	// Auto Save
	nRow = m_listUserOption.AppendEmptyRow();
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->lfWeight = FW_BOLD;
	m_listUserOption.SetCell(nRow,0,_M("Auto Save"), pCellFormat);

	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Enable"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellCtrlCheckBox;
	pCellFormat->bChecked = m_pUserOption->bAutoSave;
	m_listUserOption.SetCell(nRow,1, _T(""), pCellFormat);

	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Time Interval(minutes)"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellTextEdit;
	pCellFormat->bEnabled = m_pUserOption->bAutoSave;
	strText.Format(_T("%d"), m_pUserOption->nAutoSaveInterval);
	m_listUserOption.SetCell(nRow,1, strText, pCellFormat);

	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Auto Save File"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellTextEdit;
	pCellFormat->bEnabled = m_pUserOption->bAutoSave;
	m_listUserOption.SetCell(nRow,1, m_pUserOption->strAutoSaveFile, pCellFormat);

	// Exit
	nRow = m_listUserOption.AppendEmptyRow();
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->lfWeight = FW_BOLD;
	m_listUserOption.SetCell(nRow,0,_M("Exit"), pCellFormat);

	nRow = m_listUserOption.AppendEmptyRow();
	m_listUserOption.SetCell(nRow,0,_T("  ") + _M("Prompt For Save"));
	pCellFormat = m_listUserOption.m_defaultListFormat.CopyFormat(levelCell);
	pCellFormat->cellType = cellCtrlCheckBox;
	pCellFormat->bChecked = m_pUserOption->bPromptForSave;
	m_listUserOption.SetCell(nRow,1, _T(""), pCellFormat);

	SetUIText();

	return TRUE;
}

void CUserOptionDialog::OnBnClickedOk()
{
	m_pUserOption->strLanguage = m_listUserOption.GetItemText(1,1);
	m_pUserOption->bQueryCaseSensitive = m_listUserOption.GetCellFormat(3,1)->bChecked;
	m_pUserOption->bQueryWhenEditting = m_listUserOption.GetCellFormat(4,1)->bChecked;
	m_pUserOption->bAutoSave = m_listUserOption.GetCellFormat(6,1)->bChecked;
	m_pUserOption->nAutoSaveInterval = atoi(m_listUserOption.GetItemText(7,1));
	m_pUserOption->strAutoSaveFile = m_listUserOption.GetItemText(8,1);
	m_pUserOption->bPromptForSave = m_listUserOption.GetCellFormat(10,1)->bChecked;

	OnOK();
}

BOOL CUserOptionDialog::OnCellComboDisplayed( CListCtrl* pListCtrl, int nRow, int nCol, CellFormat* pCellFormat, CStringList& strComboItem )
{
	if ( nRow == 1 && nCol == 1 )
	{
		strComboItem.AddTail(_T("English"));
		strComboItem.AddTail(_T("ÖÐÎÄ"));
	}

	return TRUE;
}

void CUserOptionDialog::SetUIText()
{
	SetDlgItemText(IDOK, _M("Save"));
	SetDlgItemText(IDCANCEL, _M("Cancel"));

	SetWindowText(_M("Options"));
}

BOOL CUserOptionDialog::OnCellCtrlClicked( CListCtrl* pListCtrl, int nRow, int nCol, CellFormat* pCellFormat )
{
	if ( nRow == 6 && nCol == 1 )
	{
		for ( int i = 1; i <= 2; i ++)
		{
			CellFormat* pFormat = m_listUserOption.GetCellFormat(nRow+i,nCol,TRUE);
			pFormat->bEnabled = pCellFormat->bChecked;
		}
	

		CRect rc;
		m_listUserOption.GetSubItemRect(nRow+1, nCol, LVIR_BOUNDS, rc);
		rc.bottom += rc.Width();

		m_listUserOption.InvalidateRect(rc);

		if ( pCellFormat->bChecked )
		{
			m_listUserOption.StartCellEdit(nRow+1,nCol);
		}
	}

	return TRUE;
}
