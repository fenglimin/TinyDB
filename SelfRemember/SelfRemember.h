// SelfRemember.h : main header file for the SELFREMEMBER application
//

#if !defined(AFX_SELFREMEMBER_H__F74FFBF6_009B_4CC7_A3B2_DAFFBD31B9A9__INCLUDED_)
#define AFX_SELFREMEMBER_H__F74FFBF6_009B_4CC7_A3B2_DAFFBD31B9A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSelfRememberApp:
// See SelfRemember.cpp for the implementation of this class
//

class CSelfRememberApp : public CWinApp
{
public:
	CSelfRememberApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfRememberApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSelfRememberApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFREMEMBER_H__F74FFBF6_009B_4CC7_A3B2_DAFFBD31B9A9__INCLUDED_)
