// dlxiao.h : main header file for the DLXIAO application
//

#if !defined(AFX_DLXIAO_H__D57532F1_9571_4912_A8C7_9054C3300A29__INCLUDED_)
#define AFX_DLXIAO_H__D57532F1_9571_4912_A8C7_9054C3300A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDlxiaoApp:
// See dlxiao.cpp for the implementation of this class
//

class CDlxiaoApp : public CWinApp
{
public:
	CDlxiaoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlxiaoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDlxiaoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLXIAO_H__D57532F1_9571_4912_A8C7_9054C3300A29__INCLUDED_)
