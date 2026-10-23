// Char\CharFrame.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GMTOOL.h"
#include "CharFrame.h"
#include ".\charframe.h"

#include "../MainFrm.h"

#include "../../AdminSystem/MasterServer/MasterMsg.h"

// CCharFrame

extern CGMTOOLApp theApp;

IMPLEMENT_DYNCREATE(CCharFrame, CMDIChildWnd)

CCharFrame::CCharFrame()
{
}

CCharFrame::~CCharFrame()
{
}


BEGIN_MESSAGE_MAP(CCharFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCharFrame 메시지 처리기입니다.

int CCharFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_wndView.Create(IDD_CHARVIEW, this))	
	{
		return -1;
	}

	m_wndView.SetParent(this);

	RECT	rc;
	GetClientRect(&rc);
	m_wndView.MoveWindow(&rc, TRUE);

	return 0;
}

void CCharFrame::OnDestroy()
{
	CMDIChildWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMainFrame* pkMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pkMainFrame != NULL)
	{
		pkMainFrame->SetChild(CHILD_CHAR, NULL);
	}
}

void CCharFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_wndView.SetFocus();
}

void CCharFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_wndView.m_hWnd != NULL)
	{
		CRect	rc;
		GetClientRect(rc);

		m_wndView.MoveWindow(rc);
	}
}

BOOL CCharFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style =	WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX ;//| WS_MAXIMIZE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

void CCharFrame::AddCharName(TCHAR* pszServerName, TCHAR* pszCharName, TCHAR* pszAccountID)
{
	CListCtrl* pkListCtrl_ListName = (CListCtrl*)m_wndView.GetDlgItem(IDC_LISTNAME);
	if(pkListCtrl_ListName != NULL)
	{
		int nItem = pkListCtrl_ListName->GetItemCount();
		nItem = pkListCtrl_ListName->InsertItem(LVIF_TEXT, nItem, pszServerName, 0, 0, 0, 0);
		pkListCtrl_ListName->SetItemText(nItem, 1, pszCharName);
		pkListCtrl_ListName->SetItemText(nItem, 2, pszAccountID);
	}
}

TCHAR* CCharFrame::MakeItemName(SI32 siUnique)
{
	m_szItemName[0] = '\0';

	TCHAR* pszItemName = theApp.m_kDataManager.GetItemName(siUnique);
    if(pszItemName == NULL)
	{
		wsprintf(m_szItemName, "%d", siUnique);
	}
	else
	{
		_tcsncpy(m_szItemName, pszItemName, sizeof(m_szItemName));
	}

	return m_szItemName;
}

void CCharFrame::SetCharInfo(sResponse_GMC_UserCharInfo* pkInfo)
{
	if(pkInfo == NULL)				return;

	CEdit* pkEdit_CharInfo = (CEdit*)m_wndView.GetDlgItem(IDC_EDIT2);
	if(pkEdit_CharInfo == NULL)		return;

	CString	strFullInfo;
	CString strLine;
	int		i;

	strLine.Format("%s : %s \r\n", "AccountID",	pkInfo->m_szAccountID);			strFullInfo += strLine;
	strLine.Format("%s : %s \r\n", "Password",	pkInfo->m_szPassword);			strFullInfo += strLine;
	strLine.Format("%s : %d \r\n", "Login",		pkInfo->m_bConnectSwitch);		strFullInfo += strLine;
	strLine.Format("%s : %s \r\n", "CreateDate",	pkInfo->m_szMakeDate);		strFullInfo += strLine;
	strLine.Format("%s : %d \r\n", "Level",		pkInfo->m_siLevel);				strFullInfo += strLine;
	strLine.Format("%s : %I64d \r\n", "Money",	pkInfo->m_siMoney);				strFullInfo += strLine;

    strLine.Format("== Equipment ==\r\n");				strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "HELMET", MakeItemName(pkInfo->m_siItemUnique[0]), pkInfo->m_siItemCount[0]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "ARMOUR", MakeItemName(pkInfo->m_siItemUnique[1]), pkInfo->m_siItemCount[1]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "WEAPON1", MakeItemName(pkInfo->m_siItemUnique[2]), pkInfo->m_siItemCount[2]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "NECK", MakeItemName(pkInfo->m_siItemUnique[3]), pkInfo->m_siItemCount[3]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "RING1", MakeItemName(pkInfo->m_siItemUnique[4]), pkInfo->m_siItemCount[4]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "RING2", MakeItemName(pkInfo->m_siItemUnique[5]), pkInfo->m_siItemCount[5]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "BELT", MakeItemName(pkInfo->m_siItemUnique[6]), pkInfo->m_siItemCount[6]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "SHOES", MakeItemName(pkInfo->m_siItemUnique[7]), pkInfo->m_siItemCount[7]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "WEAPON2", MakeItemName(pkInfo->m_siItemUnique[8]), pkInfo->m_siItemCount[8]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "MANTLE", MakeItemName(pkInfo->m_siItemUnique[9]), pkInfo->m_siItemCount[9]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "BAG", MakeItemName(pkInfo->m_siItemUnique[10]), pkInfo->m_siItemCount[10]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "HAT", MakeItemName(pkInfo->m_siItemUnique[11]), pkInfo->m_siItemCount[11]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "DRESS", MakeItemName(pkInfo->m_siItemUnique[12]), pkInfo->m_siItemCount[12]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "BAG_SUB", MakeItemName(pkInfo->m_siItemUnique[13]), pkInfo->m_siItemCount[13]);	strFullInfo += strLine;
	strLine.Format("%10s : %20s [%4d] \r\n", "RESERVED1", MakeItemName(pkInfo->m_siItemUnique[14]), pkInfo->m_siItemCount[14]);	strFullInfo += strLine;

	strLine.Format("== Inventoty ==\r\n");				strFullInfo += strLine;
	for(i=15; i<35; i++)
	{
		strLine.Format("INV%3d : %20s [%4d]\r\n", i-15+1, MakeItemName(pkInfo->m_siItemUnique[i]), pkInfo->m_siItemCount[i]);	strFullInfo += strLine;
	}

	strLine.Format("== Horse ==\r\n");				strFullInfo += strLine;
	for(i=35; i<55; i++)
	{
		strLine.Format("HORSE%3d : %20s [%4d]\r\n", i-35+1, MakeItemName(pkInfo->m_siItemUnique[i]), pkInfo->m_siItemCount[i]);	strFullInfo += strLine;
	}

	strLine.Format("== Bag0 ==\r\n");				strFullInfo += strLine;
	for(i=55; i<67; i++)
	{
		strLine.Format("BAG0%3d : %20s [%4d]\r\n", i-55+1, MakeItemName(pkInfo->m_siItemUnique[i]), pkInfo->m_siItemCount[i]);	strFullInfo += strLine;
	}

	strLine.Format("== Bag1 ==\r\n");				strFullInfo += strLine;
	for(i=67; i<80; i++)
	{
		strLine.Format("BAG1%3d : %20s [%4d]\r\n", i-67+1, MakeItemName(pkInfo->m_siItemUnique[i]), pkInfo->m_siItemCount[i]);	strFullInfo += strLine;
	}

/*	strLine.Format("== Summon1Equip0 ==\r\n");				strFullInfo += strLine;
	for(i=80; i<95; i++)
	{
		strLine.Format("SUMMON1%3d : %5d [%4d]\r\n", i-80+1, pkInfo->m_siItemUnique[i], pkInfo->m_siItemCount[i]);	strFullInfo += strLine;
	}

	strLine.Format("== Summon2Equip0 ==\r\n");				strFullInfo += strLine;
	for(i=95; i<110; i++)
	{
		strLine.Format("SUMMON2%3d : %5d [%4d]\r\n", i-95+1, pkInfo->m_siItemUnique[i], pkInfo->m_siItemCount[i]);	strFullInfo += strLine;
	}
*/
	pkEdit_CharInfo->SetWindowText(strFullInfo);	
}
