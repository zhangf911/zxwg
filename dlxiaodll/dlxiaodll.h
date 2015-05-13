// dlxiaodll.h : main header file for the DLXIAODLL DLL
//

#if !defined(AFX_DLXIAODLL_H__AF7B602C_0C62_48C0_B7DB_7FF5C282DFA9__INCLUDED_)
#define AFX_DLXIAODLL_H__AF7B602C_0C62_48C0_B7DB_7FF5C282DFA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#ifdef DLXIAODLL
#else
#define DLXIAODLL extern "C" __declspec(dllimport)
#endif

DLXIAODLL BOOL WINAPI UninstallHook();
DLXIAODLL BOOL WINAPI InstallHook();
/////////////////////////////////////////////////////////////////////////////
// CDlxiaodllApp
// See dlxiaodll.cpp for the implementation of this class
//

class CDlxiaodllApp : public CWinApp
{
public:
	CDlxiaodllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlxiaodllApp)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDlxiaodllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLXIAODLL_H__AF7B602C_0C62_48C0_B7DB_7FF5C282DFA9__INCLUDED_)
