// dlxiaoDlg.h : header file
//

#if !defined(AFX_DLXIAODLG_H__66757FED_E5BF_4D69_ABCE_BE409332C3D3__INCLUDED_)
#define AFX_DLXIAODLG_H__66757FED_E5BF_4D69_ABCE_BE409332C3D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlxiaoDlg dialog

class CDlxiaoDlg : public CDialog
{
// Construction
public:
	CDlxiaoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlxiaoDlg)
	enum { IDD = IDD_DLXIAO_DIALOG };
	CEdit	m_sn;
	CEdit	m_key;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlxiaoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDlxiaoDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLXIAODLG_H__66757FED_E5BF_4D69_ABCE_BE409332C3D3__INCLUDED_)
