// DlxiaoDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dlxiaodll.h"
#include "DlxiaoDllDlg.h"
#include <math.h>
#include <stdio.h>
#include <afxcoll.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlxiaoDllDlg dialog


CDlxiaoDllDlg::CDlxiaoDllDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDlxiaoDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlxiaoDllDlg)
	m_sleeptime = 400;
	//}}AFX_DATA_INIT
}


void CDlxiaoDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlxiaoDllDlg)
	DDX_Control(pDX, IDC_BUTTON_RUN, m_button_run);
	DDX_Control(pDX, IDC_STATIC_SET, m_static_set);
	DDX_Control(pDX, IDC_BUTTON_GJ, m_button_start);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_button_stop);
	DDX_Control(pDX, IDC_COMBO_FZJN, m_fzJnList);
	DDX_Control(pDX, IDC_COMBO_JNADDHP, m_jnAddHpList);
	DDX_Control(pDX, IDC_COMBO_JNLIST, m_jnList);
	DDX_Text(pDX, IDC_EDIT_SLEEPTIME, m_sleeptime);
	DDV_MinMaxInt(pDX, m_sleeptime, 300, 900);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlxiaoDllDlg, CDialog)
//{{AFX_MSG_MAP(CDlxiaoDllDlg)
ON_BN_CLICKED(IDC_BUTTON_GJ, OnStart)
ON_BN_CLICKED(IDC_BUTTON_STOP, OnStop)
ON_CBN_SELCHANGE(IDC_COMBO_JNLIST, OnSelchangeComboJnlist)
ON_CBN_SELCHANGE(IDC_COMBO_JNADDHP, OnSelchangeComboJnaddhp)
ON_CBN_SELCHANGE(IDC_COMBO_FZJN, OnSelchangeComboFzjn)
	ON_BN_CLICKED(IDC_BUTTON_RUN, OnButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, OnButtonTest2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlxiaoDllDlg message handlers






typedef struct
{
    float fx;
    float fz;
    float fy;
} COODI, *PCOODI;  

extern CDlxiaoDllDlg* dlxiaoDllDlg;


CString sellNpcName =  "名医";
CString storageNpcName = "仓库老板";
DWORD saveItemId = 685;
//DWORD saveItemId = 55;








CDWordArray  jnPointerArray;//每个技能的指针

DWORD rwBaseAddr; //人物的基址

DWORD gwBaseAddr;//怪物的基址

DWORD dmBaseAddr; //地面物品的基址

DWORD bgBaseAddr;//包裹的基址

DWORD jnBaseAddr;//技能的基址

DWORD storageBaseAddr;//仓库的基址


CByteArray bagArray;//发包的内容

float guajix; //挂机的x坐标

float guajiy;//挂机的y坐标
float guajiz;//挂机的z坐标
bool isGjFlag;//是否挂机
DWORD rwId;//人物ID
void Initialization(); //初始化上面



float rwX;//人物x
float rwY;//人物y

BOOL isFullStorage;//仓库满了


CStringList pickItemNameList;


int addhpjnidx;//加血技能数组的位置
int usejnidx;//使用技能数组的位置
int fzjnidx;//辅助技能的位置


bool isheordz;//是喝还是打坐


HANDLE PID;//游戏进程

void readRwXY();//读人物的x,y







DWORD getStorageEmptyNum();//等到仓库空的格子序号
DWORD getGwPoint();//等到怪物的指针
DWORD getGwIdByCString(CString NpcName);//根据名字找ID
void setGameInfo(LPCSTR s);//运行状态
void addHpMp(bool isheordz);//加HP,MP



void AddRed();
void AddBlue();
void run(float x,float y,float z);//跑到目的地
void Pick();//捡物品
void dead();//死亡的流程
void bagfull();//包裹满的流程
void dazuo();//打坐
void sell();//卖东东
void addfz();//加辅助
void selectGw();//选怪
void killGw();//攻击怪物
void dingdian();//定点打怪
void savestorage();//存仓库
BOOL IsSell(DWORD id,int type);//是否要卖
BOOL IsPick(CString itemName);//是否捡
BOOL IsBagFull();//包裹是否满
BOOL IsBagSave();//是否要保存物品

void bagsave();//包裹有要保存的物品


void setByteToArray(int tem,CByteArray* arr);
void setDwordToArray(int tem,CByteArray* arr);
void setDwordToArrayP(int tem,CByteArray* arr);
void setShortToArrayP(int tem,CByteArray* arr);

void CallPick(DWORD id, DWORD id0);//捡物品
void CallActiveNPC(DWORD id);//击活NPC
void CallUseItem(DWORD id, DWORD no);//使用物品
void CallRunmap(float x,float y,float z);//自动寻路
void CallDazuo();//打坐
void CallUnDazuo();//取消打坐
void CallNormal(); //普通打怪
void CallDeadBack();//死亡回城
void CallSelectGw(DWORD gwId);//选怪
void CallSell(DWORD dwNum, DWORD dwID,DWORD dwPos);//买物品
void CallCloseWindow();//关闭窗口
void CallUserSkill(DWORD jnId,DWORD gwId);//使用技能
void CallSaveStorage(DWORD bagNum,DWORD storageNum);//保存仓库
void CallOpenStorage();//开启仓库
void CallCloseStorage();//关闭仓库
void CallSendBag(CByteArray* arr);//发包

void wgdebug(LPCSTR s, int a);//debug

UINT GjThreadProc(LPVOID lpParamter);//挂机线程
UINT RunThreadProc(LPVOID lpParamter);//跑路线程
UINT TestThreadProc(LPVOID lpParamter);

BOOL NotPick(DWORD item);



void readRwXY()
{
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x3d8), &rwX, sizeof(FLOAT), 0);
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x3e0), &rwY, sizeof(FLOAT), 0);
}


UINT RunThreadProc(LPVOID lpParamter)
{

	float x ,y;
	CString xs,ys;
	if(dlxiaoDllDlg->GetDlgItemText(IDC_EDIT_X,xs) && dlxiaoDllDlg->GetDlgItemText(IDC_EDIT_Y,ys))
	{
		x = (float)atof(xs);
	    y = (float)atof(ys);
		CallRunmap(x,y,0);
	}
	return (0);
}

UINT TestThreadProc(LPVOID lpParamter)
{

 
	//sell();
 CallCloseWindow();
	return (0);
}

UINT GjThreadProc(LPVOID lpParamter)
{ 
    DWORD isSelectGw = 0;
	DWORD gwPoint,one;
	int gwId =0;
	int hp = 0;
	int jnID;
	float rwgwjl;//人物与怪物的距离
	BYTE isUsejn;//技能是否能使用
	
	float gwx,gwy;
	while (isGjFlag)
    {
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x254), &hp, 4, 0);
		if(hp == 0){
			dead();
		}else{
			addHpMp(false);
			ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x7b8), &isSelectGw, 4, 0);
			if (isSelectGw == 0)
			{
				if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISSELL)){
					bagfull();
				}
				Pick();
				addHpMp(true);
				addfz();
				dingdian();
				gwPoint = getGwPoint();
				if(gwPoint != 0){
					ReadProcessMemory(PID,(LPCVOID)(gwPoint+8),&gwId,4,0);
					if (gwId != 0 )
					{
						CallSelectGw(gwId);
					}
				}
				
			}
			else
			{
				CallNormal();
				readRwXY();
				ReadProcessMemory(PID, (LPCVOID)(gwPoint + 0x4), &one, 4, 0);
                ReadProcessMemory(PID, (LPCVOID)(one + 0x3C), &gwx, sizeof(FLOAT), 0);
                ReadProcessMemory(PID, (LPCVOID)(one + 0x44), &gwy, sizeof(FLOAT), 0);
				rwgwjl = sqrtf(powf((rwX - gwx), 2) + powf((rwY - gwy), 2));
				if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISUSERJN) && (rwgwjl <= 3) &&  usejnidx > -1){
					ReadProcessMemory(PID, (LPCVOID)(jnPointerArray.GetAt(usejnidx) + 0x8), &jnID, 4, 0);
					if(jnID){
						ReadProcessMemory(PID, (LPCVOID)(jnPointerArray.GetAt(usejnidx) + 0xC), &isUsejn, 1, 0);
						if(isUsejn == 0){	
							CallUserSkill(jnID,gwId);
						}
					}
				}
				Sleep(500);
			}
		}
	}
 
return (0);
}



void dingdian(){
	int ddbj;
	if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISDINGDIAN)){
		readRwXY();
		ddbj = (int) sqrtf(powf((rwX - guajix), 2) + powf((rwY - guajiy), 2));
		if(ddbj > dlxiaoDllDlg->GetDlgItemInt(IDC_EDIT_DINGDIAN)){
			run(guajix,guajiy,guajiz);
		}
		
		
	}
	
}

void addfz()
{
	int fzjnCount =0;
	int jnID;
	if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISFZJN)){
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x118), &fzjnCount, 4, 0);
		if(fzjnCount == 0){
			if( fzjnidx > -1 ){
				ReadProcessMemory(PID, (LPCVOID)(jnPointerArray.GetAt(fzjnidx) + 0x8), &jnID, 4, 0);
				if(jnID){
					CallUserSkill(jnID,rwId);
				}
			}
		}
	}
}



//加HP，MP
void addHpMp(bool isheordz)
{
	//wgdebug("====================加HP MP=======================%d\n",0);
    int hp, mp;
	int addHpValue,addMpValue;
	DWORD jnID;
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x254), &hp, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x258), &mp, 4, 0);
    if (dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_JH))
    {
		addHpValue =  dlxiaoDllDlg->GetDlgItemInt(IDC_EDIT_HONG);
        if (hp <= addHpValue)
        {
			if(isheordz){
				if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISJNADDHP) && mp > 10 ){ 
					if( addhpjnidx > -1 ){
						ReadProcessMemory(PID, (LPCVOID)(jnPointerArray.GetAt(addhpjnidx) + 0x8), &jnID, 4, 0);
						if(jnID){
							CallUserSkill(jnID,rwId);
							Sleep(300);
						}else{
							if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISDZ)){
								dazuo();
							}
						}
					}else{
						if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISDZ)){
							dazuo();
						}
					}	
				}else if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISDZ)){
					dazuo();
				}
			}else{
				AddRed();
			}
        }
    }
	
    if (dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_JL))
    {
		addMpValue =  dlxiaoDllDlg->GetDlgItemInt(IDC_EDIT_LANG);
        if (mp <= addMpValue)
        {
			if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_ISDZ) && isheordz){
				dazuo();
			}else{
				AddBlue();
			}
		}
    }
	
	//wgdebug("====================END 加HP MP=======================%d\n",0);
}



void bagfull(){
	int dituID = 0; //地图
	float x = 169;
	float y = 66;
	float z =0;
	
	if(IsBagFull()){
		ReadProcessMemory(PID, (LPCVOID)(0x00903C7C), &dituID, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(dituID+0x1C), &dituID, 4, 0);			 
		ReadProcessMemory(PID, (LPCVOID)(dituID+0x8), &dituID, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(dituID+0x88), &dituID, 4, 0);
		
		if(dituID == 1){
			x = 169;
			y = 66;
		}else if(dituID == 2){
			x = 58;
			y = 112;
		}else if(dituID == 3){
			x = -80;
			y = -145;
		}	
		run(x,y,z);
		Sleep(100);
		CallActiveNPC(getGwIdByCString(sellNpcName));
		sell();	
		Sleep(100);
		CallCloseWindow();
		run(guajix,guajiy,guajiz);
	}
}




void bagsave()
{
	int dituID = 0; //地图
	float x = 169;
	float y = 66;
	float z =0;
	if(IsBagSave())
	{
		ReadProcessMemory(PID, (LPCVOID)(0x00903C7C), &dituID, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(dituID+0x1C), &dituID, 4, 0);			 
		ReadProcessMemory(PID, (LPCVOID)(dituID+0x8), &dituID, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(dituID+0x88), &dituID, 4, 0);
		
		if(dituID == 1){
			x = 211;
			y = 72;
		}else if(dituID == 2){
			x = 58;
			y = 112;
		}else if(dituID == 3){
			x = -80;
			y = -145;
		}	
		run(x,y,z);
		CallActiveNPC(getGwIdByCString(storageNpcName));
		CallOpenStorage();
		savestorage();	
		CallCloseStorage();
		CallCloseWindow();
		run(guajix,guajiy,guajiz);
	}
}


void savestorage()
{
	int max;
	DWORD base;
	DWORD ckNum;
	DWORD d1,dwID,dwNum,dwMaxNum;
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0x10), &max, 4, 0);//包裹的最大容量
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0xC), &base, 4, 0);
	for(int i=0;i<max;i++){
		ReadProcessMemory(PID, (LPCVOID)(base + (i*4)), &d1, 4, 0);
		if(d1)
		{
			ReadProcessMemory(PID, (LPCVOID)(d1+0x8), &dwID, 4, 0);//ID
			if(dwID == saveItemId)
			{
				ReadProcessMemory(PID, (LPCVOID)(d1+0x14), &dwNum, 4, 0);//数量
				ReadProcessMemory(PID, (LPCVOID)(d1+0x18), &dwMaxNum, 4, 0);//最大数量
				if(dwNum == dwMaxNum)
				{
					ckNum =getStorageEmptyNum();
					if(ckNum == -1)
					{
						isFullStorage = TRUE;
						return;
					}else{
						
						CallSaveStorage(i,ckNum);
					}
					
				}				
			}
			
		}
	}
	
	
	
	
}



void dead(){
	CallDeadBack();
	Sleep(10000);
	dazuo();
	Sleep(500);
	run(guajix,guajiy,guajiz);
}


void dazuo(){
	int hp=0;
	int maxHp = 0;
	int rwzt2=-1;
	DWORD isSelectGw;
	CallDazuo();
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x254), &hp, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x26c), &maxHp, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x7b8), &isSelectGw, 4, 0);
	while(hp < maxHp && hp >0 && (isSelectGw ==0))
	{
		Sleep(1000);
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x254), &hp, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x7b8), &isSelectGw, 4, 0);
	}
	CallUnDazuo();
}

void run(float x,float y,float z){
	int rwzt1 = -1;//人物状态
	int rwzt2 = -1;//人物状态
	CallRunmap(x,y,z);
	Sleep(5000);
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x8E4), &rwzt1, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(rwzt1 + 0x18), &rwzt1, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x8E4), &rwzt2, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(rwzt2 + 0xF), &rwzt2, 4, 0);
	while(rwzt1 != 0 && rwzt2 != 0){
		Sleep(2000);
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x8E4), &rwzt1, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(rwzt1 + 0x18), &rwzt1, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x8E4), &rwzt2, 4, 0);
		ReadProcessMemory(PID, (LPCVOID)(rwzt2 + 0xF), &rwzt2, 4, 0);
	}
}

void Pick()
{
    DWORD itemnum;
    DWORD two;
    DWORD baseitem;
    DWORD baseitem0;
    DWORD itemid;
    DWORD itemid0;
	CString itemName;
	WCHAR name[32];
    float x2, y2, d;
	
	readRwXY();
	
    ReadProcessMemory(PID, (LPCVOID)(dmBaseAddr + 0x18), &baseitem, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(dmBaseAddr + 0x20), &itemnum, 4, 0);
	
    if (itemnum != 0)
    {
        for (int i = 0;i < 769;i++)
        {
            ReadProcessMemory(PID, (LPCVOID)(baseitem + i*4), &two, 4, 0);
            if (two != 0)
            {
                ReadProcessMemory(PID, (LPCVOID)(two + 4), &baseitem0, 4, 0);
                ReadProcessMemory(PID, (LPCVOID)(baseitem0 + 0x110), &itemid, 4, 0);
                ReadProcessMemory(PID, (LPCVOID)(baseitem0 + 0x10C), &itemid0, 4, 0);
                ReadProcessMemory(PID, (LPCVOID)(baseitem0 + 0x3C), &x2, sizeof(FLOAT), 0);
                ReadProcessMemory(PID, (LPCVOID)(baseitem0 + 0x44), &y2, sizeof(FLOAT), 0);
				d =  sqrtf(powf((rwX - x2), 2) + powf((rwY - y2), 2));
				if (d < 10)
				{
					ReadProcessMemory(PID, (LPCVOID)(baseitem0 + 0x164), &itemnum, 4, 0);
					ReadProcessMemory(PID,(LPCVOID)(itemnum),&name,32,0);
					itemName = name;
					if(IsPick(name)){
						CallPick(itemid,itemid0);
					}
				}
            }
        }
    }
}

void sell(){
	int max;
	int type;
	DWORD base;
	DWORD d1,dwID,dwNum;
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0x10), &max, 4, 0);//包裹的最大容量
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0xC), &base, 4, 0);
	for(int i=0;i<max;i++){
		ReadProcessMemory(PID, (LPCVOID)(base + (i*4)), &d1, 4, 0);
		if(d1)
		{
			ReadProcessMemory(PID, (LPCVOID)(d1+0x8), &dwID, 4, 0);//ID
			ReadProcessMemory(PID, (LPCVOID)(d1+0x4), &type, 4, 0);//类型
			if(IsSell(dwID,type)){
				ReadProcessMemory(PID, (LPCVOID)(d1+0x14), &dwNum, 4, 0);//数量
				CallSell(dwNum,dwID,i);
			}
		}
	}	
}





void AddRed()
{
    DWORD itemnum;
    DWORD one;
    DWORD two;
    DWORD baseitem;
    DWORD itemid;
    ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x874), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x10), &itemnum, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0xC), &baseitem, 4, 0);
	
    for (DWORD i = 0;i < itemnum;i++)
    {
        ReadProcessMemory(PID, (LPCVOID)(baseitem + i*4), &two, 4, 0);
		
        if (two != 0)
        {
            ReadProcessMemory(PID, (LPCVOID)(two + 8), &itemid, 4, 0);
            if ((itemid >= 1861 && itemid <= 1865)   ||  itemid == 22 || itemid == 3999 || itemid == 4001 || itemid == 4000)
            {
                CallUseItem(itemid, i);
                break;
            }
        }
    }
	
}

void AddBlue()
{
	//wgdebug("====================加蓝=======================%d\n",0);
    DWORD itemnum;
    DWORD one;
    DWORD two;
    DWORD baseitem;
    DWORD itemid;
	
    ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x874), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x10), &itemnum, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0xC), &baseitem, 4, 0);
	
    for (DWORD i = 0;i < itemnum;i++)
    {
        ReadProcessMemory(PID, (LPCVOID)(baseitem + i*4), &two, 4, 0);	
        if (two != 0)
        {
            ReadProcessMemory(PID, (LPCVOID)(two + 8), &itemid, 4, 0);	
            if ((itemid >= 1871 && itemid <= 1876) || itemid == 4003 || itemid == 4004 )
            {
                CallUseItem(itemid, i);
                break;
            }
        }
    }
}


DWORD getGwPoint()
{
	
	float x1, y1, x2, y2, dl;
    float d = 100000;
    DWORD point =0;
    DWORD one, two;
    DWORD isgw;
	
    ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x3d8), &x1, sizeof(FLOAT), 0);
    ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x3e0), &y1, sizeof(FLOAT), 0);
    for (int i = 0;i < 769;i++)
    {
        ReadProcessMemory(PID, (LPCVOID)(gwBaseAddr + i*4), &two, 4, 0);
        if (two != 0)
        {
            ReadProcessMemory(PID, (LPCVOID)(two + 0x4), &one, 4, 0);
            ReadProcessMemory(PID, (LPCVOID)(one + 0xb4), &isgw, 4, 0);			
            if (isgw == 6 )
            {
                ReadProcessMemory(PID, (LPCVOID)(one + 0x3C), &x2, sizeof(FLOAT), 0);
                ReadProcessMemory(PID, (LPCVOID)(one + 0x44), &y2, sizeof(FLOAT), 0);
                dl = sqrtf(powf((x1 - x2), 2) + powf((y1 - y2), 2));
				if(d > dl)
				{
					d = dl;
					point = two;
				}
            }
        }
    }	
    return point;
}

DWORD getGwIdByCString(CString NpcName){
	
    DWORD id = 0;
    DWORD one, two,three;
	WCHAR name[32];
	CString cname;
	
    for (int i = 0;i < 770;i++)
    {
        ReadProcessMemory(PID, (LPCVOID)(gwBaseAddr + i*4), &two, 4, 0);
        if (two != 0)
        {
            ReadProcessMemory(PID, (LPCVOID)(two + 0x4), &one, 4, 0);//怪物指针
			ReadProcessMemory(PID,(LPCVOID)(one+0x208),&three,4,0);
			ReadProcessMemory(PID,(LPCVOID)(three),&name,32,0);
			cname = name;
			if( cname.Find(NpcName) == 0){
				ReadProcessMemory(PID,(LPCVOID)(two+0x8),&id,4,0);
			}
		}
    }
    return id;
}


DWORD getStorageEmptyNum()
{
	wgdebug("====================storageBaseAddr=====================%d\n",storageBaseAddr);
	int max;
	DWORD one,two;
	ReadProcessMemory(PID, (LPCVOID)(storageBaseAddr + 0x10), &max, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(storageBaseAddr + 0xC), &one, 4, 0);
	wgdebug("====================max=====================%d\n",max);
	for(int i=0;i<max;i++)
	{
		ReadProcessMemory(PID, (LPCVOID)(one + i*4), &two, 4, 0);
		if(!two)
		{
			return i;
		}
	}
	return -1;//仓库满了。return -1
}



void Initialization()
{
    //PID = GetCurrentProcess();
    DWORD BaseAddr = 0x00903C7C;
    DWORD one;
    ReadProcessMemory(PID, (LPCVOID)BaseAddr, &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x1c), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x28), &rwBaseAddr, 4, 0);
	
    ReadProcessMemory(PID, (LPCVOID)BaseAddr, &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x1c), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x8), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x20), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x18), &gwBaseAddr, 4, 0);
	
    ReadProcessMemory(PID, (LPCVOID)BaseAddr, &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x1c), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x8), &one, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(one + 0x24), &dmBaseAddr, 4, 0);
	
	
	ReadProcessMemory(PID, (LPCVOID)BaseAddr, &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x1c), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x28), &one, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(one + 0x874), &bgBaseAddr, 4, 0);
	
	
	
	
	ReadProcessMemory(PID, (LPCVOID)BaseAddr, &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x1c), &one, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(one + 0x28), &one, 4, 0);
	ReadProcessMemory(PID, (LPCVOID)(one + 0x8b0), &storageBaseAddr, 4, 0);
	

	ReadProcessMemory(PID, (LPCVOID)BaseAddr, &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x1c), &one, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(one + 0x28), &jnBaseAddr, 4, 0);
	
	
	

	
	
	ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x240), &rwId, 4, 0);
	guajiz = 0;
	
}


void CDlxiaoDllDlg::OnStart() 
{
	if(!isGjFlag){	
		GetDlgItem(IDC_BUTTON_GJ)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(FALSE);
		isGjFlag = true;

		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x3d8), &guajix, sizeof(FLOAT), 0);
		ReadProcessMemory(PID, (LPCVOID)(rwBaseAddr + 0x3e0), &guajiy, sizeof(FLOAT), 0);

		::AfxBeginThread(GjThreadProc, NULL);
	}
}

void CDlxiaoDllDlg::OnStop() 
{
    isGjFlag = false;	
	Sleep(1000);
	GetDlgItem(IDC_BUTTON_GJ)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(TRUE);
}

BOOL CDlxiaoDllDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_button_stop.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_STOP)));
	m_button_start.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_START)));
	m_static_set.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_SZ)));
	m_button_run.SetBitmap(::LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_PL)));
	
    PID = GetCurrentProcess();
	Initialization();
	readJn();	
	/*	if(pickItemNameList.GetCount() == 0){		
	char pBuf[MAX_PATH];//存放路径的变量
	//::GetCurrentDirectory(MAX_PATH,pBuf);                   //获取程序的当前目录
	//strcat(pBuf,"\\");
	//strcat(pBuf,"pickItem.txt");   
	HMODULE   hmode = GetModuleHandle("dlxiaodll.dll");
	GetModuleFileName(hmode,pBuf,MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(pBuf, drive, dir, NULL,NULL);
	CString strPath;
	strPath.Format("%s%s", drive, dir);
	strPath += "pickItem.txt";
	OutputDebugString(strPath);
	CStdioFile  file;
	CString strLine;
	if( file.Open(strPath,CFile.modeRead)){
	while(file.ReadString(strLine)){
				OutputDebugString(strLine);
				pickItemNameList.AddHead(strLine);
				}
				file.Close();
				}
}*/
	pickItemNameList.AddHead("炼器符"); 
	pickItemNameList.AddHead("粗木");  	 
	pickItemNameList.AddHead("碎石"); 		
	pickItemNameList.AddHead("香草"); 		
	pickItemNameList.AddHead("青田石"); 		
	pickItemNameList.AddHead("丹木"); 		
	pickItemNameList.AddHead("坚石矿"); 		
	pickItemNameList.AddHead("芒草"); 		
	pickItemNameList.AddHead("水碧"); 		
	pickItemNameList.AddHead("刚木"); 		
	pickItemNameList.AddHead("铁矿石"); 		
	pickItemNameList.AddHead("蛇衔草"); 		
	pickItemNameList.AddHead("水玉"); 		
	pickItemNameList.AddHead("甘华木"); 		
	pickItemNameList.AddHead("铜矿石"); 		
	pickItemNameList.AddHead("洞冥草"); 		
	pickItemNameList.AddHead("青碧"); 		
	pickItemNameList.AddHead("指星木"); 		
	pickItemNameList.AddHead("怀梦草"); 		
	pickItemNameList.AddHead("瑰玉"); 		
	pickItemNameList.AddHead("银矿石"); 		
	pickItemNameList.AddHead("灵寿木"); 		 
	pickItemNameList.AddHead("金矿石"); 		
	pickItemNameList.AddHead("不死草"); 		
	pickItemNameList.AddHead("璇玉"); 		
	pickItemNameList.AddHead("天婴木"); 		
	pickItemNameList.AddHead("赤银矿石"); 
	pickItemNameList.AddHead("龙须草"); 		 
	pickItemNameList.AddHead("龙公木"); 		
	pickItemNameList.AddHead("乌金矿石"); 
	pickItemNameList.AddHead("车马芝"); 		
	pickItemNameList.AddHead("仓玉"); 		
	pickItemNameList.AddHead("扶桑之木");  
	pickItemNameList.AddHead("玄铁矿石"); 
	pickItemNameList.AddHead("九穗禾"); 		 
	pickItemNameList.AddHead("天瑜"); 		 
	pickItemNameList.AddHead("嘉禾之木"); 
	pickItemNameList.AddHead("五色珍铁");  
	pickItemNameList.AddHead("天宫莲"); 		
	pickItemNameList.AddHead("帝玉");

	return TRUE; 
	
}


void CDlxiaoDllDlg::OnSelchangeComboJnlist() 
{
	usejnidx = m_jnList.GetCurSel();
	
}

void CDlxiaoDllDlg::OnSelchangeComboJnaddhp() 
{
	addhpjnidx  = m_jnAddHpList.GetCurSel();
}

void CDlxiaoDllDlg::OnSelchangeComboFzjn() 
{
	
	fzjnidx = m_fzJnList.GetCurSel();
}


void wgdebug(LPCSTR s, int a)
{
#ifdef _DEBUG
    char debug[4096];
    wsprintf(debug, s, a);
    OutputDebugString(debug);
#endif
}



BOOL IsSell(DWORD id,int type){
	
	if(type == 2 || type == 8 || type == 7){
		return FALSE;
	}
	if(type == 23){
		return FALSE;
	}
	if(id >= 1871 && id <=1875){
		return FALSE;
	}
	if(id >= 1861 && id <=1864){
		return FALSE;
	}
	if(id >= 3999 && id <=4004){
		return FALSE;
	}
	if(id == 22){
		return FALSE;
	}
	if(id == 6360){
		return FALSE;
	}
	if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_SELLALL))
	{
		return TRUE;
	}
	return TRUE;
	
}

BOOL IsBagSave()
{
	if(isFullStorage){
		return FALSE;
	}
	
	int max;
	DWORD base;
	DWORD d1,dwID,dwNum,dwMaxNum;
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0x10), &max, 4, 0);//包裹的最大容量
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0xC), &base, 4, 0);
	for(int i=0;i<max;i++){
		ReadProcessMemory(PID, (LPCVOID)(base + (i*4)), &d1, 4, 0);
		if(d1)
		{
			ReadProcessMemory(PID, (LPCVOID)(d1+0x8), &dwID, 4, 0);//ID
			if(dwID == saveItemId)
			{
				ReadProcessMemory(PID, (LPCVOID)(d1+0x14), &dwNum, 4, 0);//数量
				ReadProcessMemory(PID, (LPCVOID)(d1+0x18), &dwMaxNum, 4, 0);//最大数量
				if(dwNum == dwMaxNum )
				{
					return TRUE;
				}
				
			}
			
		}
	}	
	return FALSE;
}

BOOL IsPick(CString itemName){

	if(dlxiaoDllDlg->IsDlgButtonChecked(IDC_CHECK_QJ)){
		return TRUE;
	}

	if ( pickItemNameList.IsEmpty()){		
		return false;
	}else if(pickItemNameList.Find(itemName)){
		return true;
	}
	return false;
}

BOOL IsBagFull(){
	DWORD base;
	int max=0;
	DWORD d1;
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0x10), &max, 4, 0);//包裹的最大容量
	ReadProcessMemory(PID, (LPCVOID)(bgBaseAddr + 0xC), &base, 4, 0);
	for(int i=0;i<max;i++){
		ReadProcessMemory(PID, (LPCVOID)(base + (i*4)), &d1, 4, 0);
		if(!d1)
		{
			return FALSE;
		}
	}
	return TRUE;
	
}

void setDwordToArray(int tem,CByteArray* arr)
{
	arr->Add((BYTE)(tem >> 24));
	arr->Add((BYTE)(tem >> 16));
	arr->Add((BYTE)(tem >> 8));
	arr->Add((BYTE)(tem >> 0));
}

void setDwordToArrayP(int tem,CByteArray* arr)
{
	arr->Add((BYTE)(tem >> 0));
	arr->Add((BYTE)(tem >> 8));
	arr->Add((BYTE)(tem >> 16));
	arr->Add((BYTE)(tem >> 24));
	
}

void setShortToArrayP(int tem,CByteArray* arr)
{
	arr->Add((BYTE)(tem >> 0));
	arr->Add((BYTE)(tem >> 8));
}

void setByteToArray(int tem,CByteArray* arr)
{
	arr->Add((BYTE)(tem >> 0));
}

void CallUseItem(DWORD id, DWORD no)
{
	bagArray.RemoveAll();
	bagArray.Add(0x28);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(1);
	setShortToArrayP(no,&bagArray);
	setDwordToArrayP(id,&bagArray);
	CallSendBag(&bagArray);
}

void CallNormal()
{
	bagArray.RemoveAll();
	bagArray.Add(3);
	bagArray.Add(0);
	bagArray.Add(0);
	CallSendBag(&bagArray);
}

void CallSelectGw(DWORD gwId)
{
	bagArray.RemoveAll();
	bagArray.Add(2);
	bagArray.Add(0);
	setDwordToArrayP(gwId,&bagArray);
	CallSendBag(&bagArray);
	
}

void CallPick(DWORD id, DWORD id0)
{
	bagArray.RemoveAll();
	bagArray.Add(6);
	bagArray.Add(0);
	setDwordToArrayP(id0,&bagArray);
	setDwordToArrayP(id,&bagArray);
	CallSendBag(&bagArray);
	
}

void CallUserSkill(DWORD jnId,DWORD gwId)
{
	bagArray.RemoveAll();
	bagArray.Add(0x29);
	bagArray.Add(0);
	setDwordToArrayP(jnId,&bagArray);
	bagArray.Add(0);
	bagArray.Add(1);
	setDwordToArrayP(gwId,&bagArray);
	CallSendBag(&bagArray);
}

void CallDeadBack()
{
	bagArray.RemoveAll();
	bagArray.Add(4);
	bagArray.Add(0);
	CallSendBag(&bagArray);
}

void CallDazuo()
{
	bagArray.RemoveAll();
	bagArray.Add(0x2e);
	bagArray.Add(0);
	CallSendBag(&bagArray);
}
void CallUnDazuo()
{
	bagArray.RemoveAll();
	bagArray.Add(0x2a);
	bagArray.Add(0);
	CallSendBag(&bagArray);
}


void CallRunmap(float x,float y,float z)
{
	DWORD addr = 0x0042ABB0;
	_asm
	{
		pushad	
			mov     edx, dword ptr ds:[0x00903C7C]
			mov     eax, dword ptr ds:[edx+0x1C]
			mov     ecx, dword ptr ds:[eax+0x8]
			mov     esi, dword ptr ds:[ecx+0x88]
			push	esi
			mov		eax, x
			mov		ecx, 0x00909BC8
			mov		[ecx], eax
			mov		eax, z
			mov		[ecx + 4], eax
			mov		eax, y
			mov		[ecx + 8], eax
			push	0x00909BC8
			mov		eax, dword ptr DS:[0x00903C7C]
			mov		eax, dword ptr DS:[eax + 0x1C]
			mov		eax, dword ptr DS:[eax + 0x28]
			lea		eax, dword ptr DS:[eax + 0x3C]
			push	eax
			mov		ecx, 0x00903C30
			call	addr
			popad
	}
}


void CallActiveNPC(DWORD id)
{
	bagArray.RemoveAll();
	bagArray.Add(0x23);
	bagArray.Add(0);
	setDwordToArrayP(id,&bagArray);
	CallSendBag(&bagArray);
	
}

void CallBuy(DWORD dwNum, DWORD dwID,DWORD dwPos)
{
/*	PRES_STR pRes;
pRes.dwID=dwID;
pRes.dwPos=dwPos;
pRes.dwNum=dwNum;

  DWORD addr = 0x0059FB80;
  __asm
  {
		pushad
		push pRes
		push 1
		call addr
		popad
		}
	Sleep(150);*/
}

void CallSell(DWORD dwNum, DWORD dwID,DWORD dwPos)
{
	bagArray.RemoveAll();
	bagArray.Add(0x25);
	bagArray.Add(0);
	bagArray.Add(2);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0x10);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(1);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0);
	setDwordToArrayP(dwID,&bagArray);
	setDwordToArrayP(dwPos,&bagArray);
	setDwordToArrayP(dwNum,&bagArray);
	CallSendBag(&bagArray);
	
}
void CallOpenStorage()
{
	bagArray.RemoveAll();
	bagArray.Add(0x25);
	bagArray.Add(0);
	bagArray.Add(0x0F);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(4);
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0); 
	bagArray.Add(0);
	bagArray.Add(0);
	bagArray.Add(0); 
	bagArray.Add(0);
	CallSendBag(&bagArray);
}
void CallSaveStorage(DWORD bagNum,DWORD storageNum)
{
	bagArray.RemoveAll();
	bagArray.Add(0x3A);
	bagArray.Add(0);
	setByteToArray(storageNum,&bagArray);
	setByteToArray(bagNum,&bagArray);
	CallSendBag(&bagArray);
}

void CallCloseStorage()
{
	bagArray.RemoveAll();
	bagArray.Add(0x2A);
	bagArray.Add(0);
	CallSendBag(&bagArray);
	
}

void CallCloseWindow()
{
	DWORD addr = 0x006B28D0;
	__asm{	
		pushad
			push 0x008C1C0C
			mov eax,dword ptr ds:[0x00903C7C]
			mov eax,dword ptr ds:[eax+0x1C]
			mov eax,dword ptr ds:[eax+0x4]
			mov eax,dword ptr ds:[eax+0x8]
			mov eax,dword ptr ds:[eax+0x14]
			mov esi,eax
			mov ecx,esi
			call addr
			popad
	}
	Sleep(150);
}


void CallSendBag(CByteArray* arr){
	int sleeptime = 400;
	DWORD addr = 0x0057EDC0;
	DWORD bagSize = arr->GetSize();
	BYTE *buff = arr->GetData();
	__asm{
		pushad
			mov ecx,dword ptr ds:[0x00903C7C]
			push bagSize
			mov ebx,buff
			push ebx
			mov ecx,dword ptr ds:[ecx+0x20]
			call addr
			popad
	}
	if(dlxiaoDllDlg->GetDlgItemInt(IDC_EDIT_SLEEPTIME))
	{
		sleeptime = dlxiaoDllDlg->GetDlgItemInt(IDC_EDIT_SLEEPTIME);
	}
	Sleep(sleeptime);
}

void CDlxiaoDllDlg::readJn()
{
	
	jnPointerArray.RemoveAll();
	DWORD jnMax,one,two,three;
	WCHAR jnName[32];

	ReadProcessMemory(PID, (LPCVOID)(jnBaseAddr + 0x910), &jnMax, 4, 0);
    ReadProcessMemory(PID, (LPCVOID)(jnBaseAddr + 0x90C), &one, 4, 0);
	
	for(DWORD i=0;i<jnMax;i++){
		ReadProcessMemory(PID, (LPCVOID)(one + i*4), &two, 4, 0);
		if(two)
		{
			jnPointerArray.Add(two);
			ReadProcessMemory(PID, (LPCVOID)(two + 0x4), &three,4,0);
			ReadProcessMemory(PID, (LPCVOID)(three + 0x4), &three,4,0);
			ReadProcessMemory(PID, (LPCVOID)(three + 0x63), &three,4,0);
			ReadProcessMemory(PID, (LPCVOID)(three),&jnName,32,0);
			m_jnList.AddString((CString)jnName);
			m_fzJnList.AddString((CString)jnName);
			m_jnAddHpList.AddString((CString)jnName);
		}
		
	}
}

void CDlxiaoDllDlg::OnButtonRun() 
{
	 
	::AfxBeginThread(RunThreadProc, NULL);
}


 

void CDlxiaoDllDlg::OnButtonTest2() 
{
	::AfxBeginThread(TestThreadProc, NULL);
	
}
