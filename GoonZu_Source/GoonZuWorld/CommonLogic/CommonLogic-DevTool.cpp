//---------------------------------
// 2003/7/17 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\CommonLogic\CommonLogic.h"
#include "..\resource.h"
#include "..\Common\Bullet\Bullet.h"
#include "..\Common\Smoke\Smoke.h"
#include "..\Common\DevTool\DevCharInfo\DevCharInfo.h"
#include "..\Common\DevTool\DevNPCInfo\DevNPCInfo.h"
#include "..\Common\DevTool\DevInHereMapChar\DevInHereMapChar.h"

#include <Mouse.h>


extern cltCommonLogic*		pclClient;

//----------------------------------------------------------------
// DevCharInfo
//----------------------------------------------------------------
BOOL cltCommonLogic::CreateDevCharInfoDlg()
{

	if(pclDevTool[DEV_TOOL_CHARINFO] == NULL)return FALSE;
	
	// 대화상자가 없으면 만들고 있으면 없앤다. 
	return pclDevTool[DEV_TOOL_CHARINFO]->Create(GetInstance(), GetHwnd(), IDD_DIALOG_DEV_CHARINFO, StaticDevCharInfoDlgProc);

}

BOOL CALLBACK cltCommonLogic::StaticDevCharInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclDevTool[DEV_TOOL_CHARINFO]->DlgProc(hDlg, iMsg, wParam, lParam);
}


//----------------------------------------------------------------
// DevNPCInfo
//----------------------------------------------------------------
BOOL cltCommonLogic::CreateDevNPCInfoDlg()
{

	if(pclDevTool[DEV_TOOL_NPCINFO] == NULL)return FALSE;

	// 대화상자가 없으면 만들고 있으면 없앤다. 
	return pclDevTool[DEV_TOOL_NPCINFO]->Create(GetInstance(), GetHwnd(), IDD_DIALOG_DEV_CHARINFO, StaticDevNPCInfoDlgProc);

}

BOOL CALLBACK cltCommonLogic::StaticDevNPCInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclDevTool[DEV_TOOL_NPCINFO]->DlgProc(hDlg, iMsg, wParam, lParam);
}




//----------------------------------------------------------------
// DevProfileInfo
//----------------------------------------------------------------
BOOL cltCommonLogic::CreateDevProfileInfoDlg()
{

	if(pclDevTool[DEV_TOOL_PROFILEINFO] == NULL)return FALSE;

	// 대화상자가 없으면 만들고 있으면 없앤다. 
	return pclDevTool[DEV_TOOL_PROFILEINFO]->Create(GetInstance(), GetHwnd(), IDD_DIALOG_DEV_PROFILEINFO, StaticDevProfileDlgProc);

}

BOOL CALLBACK cltCommonLogic::StaticDevProfileDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclDevTool[DEV_TOOL_PROFILEINFO]->DlgProc(hDlg, iMsg, wParam, lParam);
}

//----------------------------------------------------------------
// DevServerInfo
//----------------------------------------------------------------
BOOL cltCommonLogic::CreateDevServerInfoDlg()
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		if(pclDevTool[DEV_TOOL_SERVERINFO] == NULL)return FALSE;

		BOOL bRet = FALSE;

		// 대화상자가 없으면 만들고 있으면 없앤다. 
		if(IsWhereServiceArea(ConstServiceArea_USA))
			bRet = pclDevTool[DEV_TOOL_SERVERINFO]->Create(GetInstance(), GetHwnd(), IDD_DIALOG_DEV_SERVERINFO, StaticDevServerDlgProc, false);
		else
			bRet = pclDevTool[DEV_TOOL_SERVERINFO]->Create(GetInstance(), GetHwnd(), IDD_DIALOG_DEV_SERVERINFO, StaticDevServerDlgProc);

		return bRet;
	}

	return FALSE;
}

BOOL CALLBACK cltCommonLogic::StaticDevServerDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if(pclClient->GameMode == GAMEMODE_SERVER)
	{
		return pclClient->pclDevTool[DEV_TOOL_SERVERINFO]->DlgProc(hDlg, iMsg, wParam, lParam);
	}

	return FALSE;
}

//---------------------------------------------
// DevInHereMapChar
//---------------------------------------------
BOOL cltCommonLogic::CreateDevInHereMapCharDlg()
{
	if ( GAMEMODE_CLIENT != pclClient->GameMode )
	{
		return FALSE;
	}

	if ( NULL == pclDevTool[DEV_TOOL_INHEREMAPCHAR] )
	{
		return FALSE;
	}

	// 대화상자가 없으면 만들고 있으면 없앤다. 
	return pclDevTool[DEV_TOOL_INHEREMAPCHAR]->Create(GetInstance(), GetHwnd(), IDD_DIALOG_DEV_INHEREMAPCHAR, StaticDevInHereMapCharDlgProc);
}

BOOL CALLBACK cltCommonLogic::StaticDevInHereMapCharDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	if ( GAMEMODE_CLIENT != pclClient->GameMode )
	{
		return FALSE;
	}

	return pclClient->pclDevTool[ DEV_TOOL_INHEREMAPCHAR ]->DlgProc(hDlg, iMsg, wParam, lParam);

}