#include "../Server/Server.h"
#include "Char\CharCommon\Char-Common.h"
#include "..\resource.h"
#include "../CommonLogic/CommonLogic.h"
#include "DevServerInfo.h"

extern cltCommonLogic* pclClient;

cltDevServerInfo::cltDevServerInfo() : cltDevTool()
{
	pkProfileIterator	= NProfileSystem::Get_Iterator();
	m_siMaxCalls		= 0;
	m_bReset			= FALSE;
}

cltDevServerInfo::~cltDevServerInfo()
{
	NProfileSystem::Release_Iterator(pkProfileIterator);
}

BOOL CALLBACK cltDevServerInfo::DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[256] = TEXT("");

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				OnInitDialog(hDlg);
			}
		}
		return TRUE;

	case WM_TIMER:
		{
			//OnTimerDialog(hDlg, );
		}
		return TRUE;

	case WM_DESTROY:
		// 타이머를 해제한다. 
		//KillTimer(hDlg, 1);

		// 윈도우의 위치를 기억한다. 
		GetWindowRect(m_hDlg, &DlgRect);

		DlgRectInitSwitch = TRUE;

		m_hDlg = NULL;
		return TRUE;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_RESET:			{	m_bReset = TRUE;		}	return TRUE;
			case IDOK:				{	DestroyWindow(m_hDlg);	}	return TRUE;
			case IDCANCEL:			{	DestroyWindow(m_hDlg);	}	return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

BOOL cltDevServerInfo::OnInitDialog(HWND hDlg)
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		m_hDlg = hDlg;
		m_hServerInfoListCtrl = GetDlgItem( hDlg, IDC_LIST_SERVERINFO);

		LVCOLUMN Col;			

		Col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		Col.fmt = LVCFMT_RIGHT;

		Col.cx = 100;	Col.pszText = "Time";					Col.iSubItem = 0;
		SendMessage( m_hServerInfoListCtrl, LVM_INSERTCOLUMN, 0, (LPARAM)&Col );

		Col.cx = 400;	Col.pszText = "COMMAND";				Col.iSubItem = 1;
		SendMessage( m_hServerInfoListCtrl, LVM_INSERTCOLUMN, 1, (LPARAM)&Col );

		return TRUE;
	}

	return FALSE;
}

#ifdef _DEBUG
	#define	SERVERINFO_DONOT_WRITEFILE
#endif

void cltDevServerInfo::OnServerInfoDialog(TCHAR* Command)
{
	#ifdef _SERVER

	// DB초기화가 종료되면 더이상 로그를 찍지 않는다. - by LEEKH 2008.03.07
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		cltServer* pclserver = (cltServer*)pclClient;
		if (pclserver->bCompleteDBInitAll == TRUE)
			return;
	}
	
	TCHAR szBuffer0[128];
	TCHAR szBuffer1[128];
	TCHAR fileNameBuffer[256];
	int idx;
	//슬롯에 찍을 시간을 텍스트로 완성한다.
	TCHAR szTime[128];
	SYSTEMTIME stTimeInfo;
	GetLocalTime(&stTimeInfo);
	StringCchPrintf( szTime,  sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);

	StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
	StringCchPrintf(szBuffer1, sizeof(szBuffer1),"%s",Command);
	//-----------------------------------------------------------------------------------
	
	LVITEM Li;
	Li.mask			= LVIF_TEXT;
	Li.state		= 0;
	Li.stateMask	= 0;
	Li.iImage		= 0;
	Li.iSubItem 	= 0;
	idx				= ListView_GetItemCount(m_hServerInfoListCtrl);
	Li.iItem		= idx;	
	Li.iSubItem = 0;	Li.pszText = szBuffer0;	SendMessage( m_hServerInfoListCtrl, LVM_INSERTITEM, 0, (LPARAM)&Li );
	Li.iSubItem = 1;	Li.pszText = szBuffer1;	SendMessage( m_hServerInfoListCtrl, LVM_SETITEM, 0, (LPARAM)&Li );
	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\ServerStartInfo_%04d%02d%02d.log") , stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );

		#ifndef SERVERINFO_DONOT_WRITEFILE
			pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), TEXT("Command : \t[%s]\t "),szBuffer1);
		#endif

	#endif
}
