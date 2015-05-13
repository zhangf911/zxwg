#if !defined(AFX_DLXIAODLLDLG_H__CA721EF3_39D9_4229_A3E7_2077E82D1C48__INCLUDED_)
#define AFX_DLXIAODLLDLG_H__CA721EF3_39D9_4229_A3E7_2077E82D1C48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlxiaoDllDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlxiaoDllDlg dialog

class CDlxiaoDllDlg : public CDialog
{
// Construction
public:
	void readJn();
	CDlxiaoDllDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlxiaoDllDlg)
	enum { IDD = IDD_DLXIAODLLDLG_DIALOG };
	CButton	m_button_run;
	CButton	m_static_set;
	CButton	m_button_start;
	CButton	m_button_stop;
	CComboBox	m_fzJnList;
	CComboBox	m_jnAddHpList;
	CComboBox	m_jnList;
	int		m_sleeptime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlxiaoDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlxiaoDllDlg)
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSelchangeComboJnlist();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboJnaddhp();
	afx_msg void OnSelchangeComboFzjn();
	afx_msg void OnButtonRun();
	afx_msg void OnButtonTest2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLXIAODLLDLG_H__CA721EF3_39D9_4229_A3E7_2077E82D1C48__INCLUDED_)
