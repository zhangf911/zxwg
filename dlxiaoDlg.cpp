// dlxiaoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dlxiao.h"
#include "dlxiaoDlg.h"
#include "dlxiaodll/dlxiaodll.h"

extern "C"
{
#include "miracl.h"
#include "mirdef.h"
}



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_MY_MESSAGE (WM_USER+108)
NOTIFYICONDATA tnd;
CWnd* wnd;
CMenu menu;
CMenu* pMenu;
/////////////////////////////////////////////////////////////////////////////
// CDlxiaoDlg dialog

CDlxiaoDlg::CDlxiaoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDlxiaoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlxiaoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlxiaoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlxiaoDlg)
	DDX_Control(pDX, IDC_EDIT_SN, m_sn);
	DDX_Control(pDX, IDC_EDIT_KEY, m_key);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlxiaoDlg, CDialog)
//{{AFX_MSG_MAP(CDlxiaoDlg)
ON_MESSAGE(WM_MY_MESSAGE, OnMyMessage)
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_SIZE()
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlxiaoDlg message handlers

BOOL CDlxiaoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	LPCTSTR lpRootPathName="c:\\"; 
	DWORD VolumeSerialNumber;
	::GetVolumeInformation(lpRootPathName,NULL, NULL,&VolumeSerialNumber, NULL,NULL,NULL,NULL);
	CString snstr;
	snstr.Format("%lx ",VolumeSerialNumber);
	snstr.TrimRight();
	this->SetDlgItemText(IDC_EDIT_SN,snstr);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDlxiaoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlxiaoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDlxiaoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(nType == SIZE_MINIMIZED){
		tnd.cbSize=sizeof(NOTIFYICONDATA);
		tnd.hWnd=this->m_hWnd;
		tnd.uID=IDR_MAINFRAME;
		tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
		tnd.uCallbackMessage=WM_MY_MESSAGE;	//用户自定义的消息，即鼠标在任务栏上程序图标上动作时图标发送的消息
		tnd.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
		strcpy(tnd.szTip,"");//图标提示为"冬天的林"
		Shell_NotifyIcon(NIM_ADD,&tnd);//向任务栏添加图标		
		this->ShowWindow(SW_HIDE);
	}
	
}

LRESULT CDlxiaoDlg::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
	UINT uID;//发出该消息的图标的ID
	UINT uMouseMsg;//鼠标动作
	uID=(UINT) wParam;
	uMouseMsg=(UINT) lParam;
	CPoint pos;
	
	
	switch(uMouseMsg)
	{
	case WM_LBUTTONDOWN:
		this->ShowWindow(SW_SHOW);//显示程序窗口
		break;
	case WM_RBUTTONDOWN:
		this->ShowWindow(SW_SHOW);//显示程序窗口
		break;		
	}
	
	return 0;
}

void CDlxiaoDlg::OnClose() 
{
	Shell_NotifyIcon(NIM_DELETE,&tnd);
	//UninstallHook();	
	CDialog::OnClose();
}

void CDlxiaoDlg::OnButtonStart() 
{

	miracl *mip=mirsys(100,0);
	mip->IOBASE=16;	//16进制模式
	
	//定义并初始化变量
	big m=mirvar(0);	//m 放明文：注册码SN
	big c=mirvar(0);	//c 放密文：用户名Name
	big n=mirvar(0);	//n 模数
	big e=mirvar(0);	//e 公钥
	TCHAR SN[256]={0};
	TCHAR KEY[256]={0};
	TCHAR temp[256]={0};
	int len=0;
	int i,j;
	
	//获取SN
	len=m_sn.GetWindowTextLength ();
	m_sn.GetWindowText (SN,len+1);
	
	//获取KEY
	len=m_key.GetWindowTextLength ();
	m_key.GetWindowText (KEY,len+1);
	
	//检查KEY是否为16进制
	for (i=0,j=0;i<len;i++)
	{
		if(isxdigit(KEY[i])==0)
		{
			j=1;
			break;
		}
	}
	
	//如果输入的KEY为16进制且长度不为0
	if (j!=1&&len!=0)
	{
		
		cinstr(m,KEY);									//初始化明文m
		cinstr(n,"963251DC5A9C90D9F203A03C363BA411");	//初始化模数n	 
		cinstr(e,"10001");								//初始化公钥e
		
		//当m<n时
		if(compare(m,n)==-1)
		{
			powmod(m,e,n,c);//计算c=m^e mod n
			big_to_bytes(256,c,temp,FALSE);//将c转换成数组写入temp
	
			//释放内存
			mirkill(m);
			mirkill(c);
			mirkill(n);
			mirkill(e);
			mirexit();
		}
		else
			j=1;
		
	}
	
	
	//对SN、temp， m、n， KEY的长度进行检查
	if(lstrcmp(SN,temp)!=0||j==1||len==0)
	{
			MessageBox("请找QQ:42256281要钥匙!!!");
	}else{
		this->ShowWindow(SW_MINIMIZE);
			InstallHook();
		
	}



}

void CDlxiaoDlg::OnButtonTest() 
{
		this->ShowWindow(SW_MINIMIZE);
			InstallHook();
	
}

