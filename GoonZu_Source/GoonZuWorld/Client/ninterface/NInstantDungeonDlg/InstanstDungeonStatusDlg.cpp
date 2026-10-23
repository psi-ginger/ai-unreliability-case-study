#include "CommonHeader.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"

#include "../NLib/NTCHARString.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"

#include "../Client/ninterface/NInstantDungeonDlg/InstanstDungeonStatusDlg.h"
#include "../Client/MarkManager/MarkManager.h"

extern cltCommonLogic* pclClient;


InstanstDungeonStatusDlg::InstanstDungeonStatusDlg()
{
	
}

InstanstDungeonStatusDlg::~InstanstDungeonStatusDlg()
{
	
}

void InstanstDungeonStatusDlg::Destroy()
{
	
}

void InstanstDungeonStatusDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NInstantDungeonDlg/InstanstDungeonStatusDlg.ddf"));
	file.CreatePopupDialog( this, NINSTANST_DUNGEON_STATUS_DLG, TEXT("dialog_instants_dungeon"), StaticInstanstDungeonStatusDlgProc);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NINSTANTS_DUNGEON_STATUS_STATICTEXT_CONTENTS, this ),	NINSTANTS_DUNGEON_STATUS_STATICTEXT_CONTENTS, TEXT("statictext_contents"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,    NINSTANTS_DUNGEON_STATUS_LISTBOX_CONTENTS, this ),		NINSTANTS_DUNGEON_STATUS_LISTBOX_CONTENTS,	  TEXT("listbox_status_list"));	

	m_bIsInit = false;
 }


void CALLBACK InstanstDungeonStatusDlg::StaticInstanstDungeonStatusDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	InstanstDungeonStatusDlg *pThis = (InstanstDungeonStatusDlg*) pControllerMgr;
	pThis->NInstanstDungeonStatusDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK InstanstDungeonStatusDlg::NInstanstDungeonStatusDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;	
	}
}

void  InstanstDungeonStatusDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	//CList* pList = m_InterfaceFactory.GetList(NINSTANTS_DUNGEON_STATUS_LISTBOX_CONTENTS);
	
	/*for(SI32 Cnt = 0; Cnt < GUILD_MARK_CNT; ++Cnt)
	{
		GP.PutSpr(&m_Spr, GetX() + pList->GetX() + ScreenRX + 21, (GetY() + pList->GetY() + ScreenRY + 18) + (Cnt * 36) , Cnt );
	}*/
	// 길드 마크를 그린다.
	//DrawGuildMark(pList, ScreenRX, ScreenRY, 21, 18, 36, GUILD_MARK_CNT);

}


void InstanstDungeonStatusDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	if(false == RetValue)
	{
		return;
	}

	switch( MsgIndex )
	{
		default:	break;
	}
}

// test 용
void InstanstDungeonStatusDlg::InitList(void)
{
	CList* pList = m_InterfaceFactory.GetList(NINSTANTS_DUNGEON_STATUS_LISTBOX_CONTENTS);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = NULL; 
	pText = GetTxtFromMgr(7818);
	pList->SetColumn(0, 70, pText);
	pText = GetTxtFromMgr(10233);	
	pList->SetColumn(1, 120, pText);
	pText = GetTxtFromMgr(7819);	
	pList->SetColumn(2, 70, pText);
	pText = GetTxtFromMgr(7820);	
	pList->SetColumn(3, 100, pText);

	stListBoxItemData tempItemData;
	for(int Cnt = 0; Cnt < GUILD_MARK_CNT; ++Cnt)
	{
		tempItemData.Init();

		if(Cnt < 2)
		{	
			tempItemData.SetSmallIconGImg( 0, GIMG_VILLAGEMARK01, Cnt);
		}
		else
		{
			tempItemData.SetSmallIconGImg( 0, -1, -1);
		}
		
		tempItemData.Set( 1, "최강 황진성" );
		tempItemData.Set( 2, "5분 370초" );
		tempItemData.Set( 3, "오억~" );
	
		pList->AddItem( &tempItemData );
	}

	pList->Refresh();
}


void InstanstDungeonStatusDlg::InitList(cltGameMsgResponse_InstantsDungeonStatus* _GuildInfoManager)
{
	cltClient* pclclient = (cltClient*)pclClient;

	if(false == m_bIsInit)
	{
		CList* pList = m_InterfaceFactory.GetList(NINSTANTS_DUNGEON_STATUS_LISTBOX_CONTENTS);

		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = NULL; 
		pText = GetTxtFromMgr(7818);
		pList->SetColumn(0, 70, pText);
		pText = GetTxtFromMgr(10233);	
		pList->SetColumn(1, 120, pText);
		pText = GetTxtFromMgr(7819);	
		pList->SetColumn(2, 80, pText);
		pText = GetTxtFromMgr(7820);	
		pList->SetColumn(3, 100, pText);

		stListBoxItemData tempItemData;
		cInstantsDungeonStatusBaseInfo* pGuildInfo = NULL;
		TCHAR	szBuffer[32] = "";
		SI32	Minute		 = 0;
		SI32	Second		 = 0;
		for(int Cnt = 0; Cnt < GUILD_MARK_CNT; ++Cnt)
		{
			tempItemData.Init();

			pGuildInfo = &_GuildInfoManager->clGuildInfo[Cnt];
			if(strlen(pGuildInfo->szGuildName) <= 0 )
			{
				continue;
			}

			SI32  guildicon = -1 ;
			TSpr* guildspr	= NULL;
			
			pclclient->m_pMarkManager->GetMarkSprAndFontIndex( pGuildInfo->siGuildMarkIndex, &guildspr, &guildicon );
				
			if( guildicon >= 0 )
			{
				SI16 SprNum = -1;
				switch(pGuildInfo->siGuildMarkIndex / 256)
				{
					case 0: SprNum = GIMG_VILLAGEMARK01;	break;
					case 1: SprNum = GIMG_VILLAGEMARK02;	break;
					case 2: SprNum = GIMG_VILLAGEMARK03;	break;
				}	

				tempItemData.SetSmallIconGImg( 0, SprNum, guildicon);
			}

			tempItemData.Set( 1, pGuildInfo->szGuildName);
			
			if(0 != pGuildInfo->siRemainTime)
			{
				Minute =   pGuildInfo->siRemainTime / 60000;
				Second = ( pGuildInfo->siRemainTime % 60000 ) / 1000;
				//itoa(pGuildInfo->siRemainTime, szBuffer, 10);
				StringCchPrintf(szBuffer, sizeof(szBuffer), "%d : %d", Minute, Second);
				tempItemData.Set( 2, szBuffer);	
			}
			else
			{	
				StringCchPrintf(szBuffer, sizeof(szBuffer), "%d : %d", 0, 0);
				tempItemData.Set( 2, szBuffer);	
			}
			
			itoa(pGuildInfo->siScore, szBuffer, 10);
            tempItemData.Set( 3, szBuffer);

			pList->AddItem( &tempItemData );
		}

		pList->Refresh();

		m_bIsInit = true;
	}
}


// 길드 마크를 그린다.(세로로 배열.)
void InstanstDungeonStatusDlg::DrawGuildMark(CList* pList, SI32 _ScreenRX, SI32 _ScreenRY, SI32 _StartX, SI32 _StartY, SI32 _ColumnSize, SI32 _ColumnCnt)
{
	for(SI32 Cnt = 0; Cnt < _ColumnCnt; ++Cnt)
	{
		GP.PutSpr(&m_Spr, GetX() + pList->GetX() + _ScreenRX + _StartX, (GetY() + pList->GetY() + _ScreenRY + _StartY) + (Cnt * _ColumnSize), Cnt );
	}
}

void SetGuildInfo(cltGameMsgResponse_InstantsDungeonStatus* _GuildInfo)
{

}