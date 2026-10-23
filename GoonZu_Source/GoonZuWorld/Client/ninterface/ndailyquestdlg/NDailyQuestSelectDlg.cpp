/* ==========================================================================
	클래스 :		CNDailyQuestSelectDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "./NDailyQuestSelectDlg.h"
#include <tchar.h>
#include "../../CommonLogic/Msg/MsgType-DailyQuest.h"
#include "../../Common/DailyQuest/DailyQuestObj.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../../Music/music.h"

#include "../../../Client/InterfaceMgr/Interface/OutLine.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"
#include "../../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"
#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/ListView.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNDailyQuestSelectDlg::CNDailyQuestSelectDlg() : CDialogController()
{
	m_siSelectedListIndex = -1;
	m_pDailyQuestObjDataList = new CDailyQuestObjData[ MAX_AVAILABLE_DAILYQUEST_NUM ];

	m_pImageStatic_portrait	 = NULL; 
	m_pListView_reward_item	 = NULL; 
	m_pStatic_NONAME1	 = NULL; 
	m_pStatic_NONAME2	 = NULL; 
	m_pStatic_NONAME3	 = NULL; 
	m_pStatic_NONAME4	 = NULL; 
	m_pButton_questrefresh	 = NULL; 
	m_pButton_queststart	 = NULL; 
	m_pButton_queststop	 = NULL; 
	m_pEdit_questexplain	 = NULL; 
	m_pEdit_condition_fee	 = NULL; 
	m_pEdit_condition_monster	 = NULL; 
	m_pEdit_monster_num	 = NULL; 
	m_pEdit_condition_time	 = NULL; 
	m_pEdit_reward_item	 = NULL; 
	m_pEdit_reward_num	 = NULL; 
	m_pEdit_reward_exp	 = NULL; 
	m_pList_quest_select	 = NULL; 
	m_pOutLine_NONAME1	 = NULL; 

	m_bMoneySelected = FALSE;

}

CNDailyQuestSelectDlg::~CNDailyQuestSelectDlg()
{
	if ( m_pDailyQuestObjDataList )
	{
		delete [] m_pDailyQuestObjDataList;
		m_pDailyQuestObjDataList = NULL;
	}
	
	if( 	m_pImageStatic_portrait	 )  	delete 	m_pImageStatic_portrait	;
	if( 	m_pListView_reward_item	 )  	delete 	m_pListView_reward_item	;
	if( 	m_pStatic_NONAME1	 )  	delete 	m_pStatic_NONAME1	;
	if( 	m_pStatic_NONAME2	 )  	delete 	m_pStatic_NONAME2	;
	if( 	m_pStatic_NONAME3	 )  	delete 	m_pStatic_NONAME3	;
	if( 	m_pStatic_NONAME4	 )  	delete 	m_pStatic_NONAME4	;
	if( 	m_pButton_questrefresh	 )  	delete 	m_pButton_questrefresh	;
	if( 	m_pButton_queststart	 )  	delete 	m_pButton_queststart	;
	if( 	m_pButton_queststop	 )  	delete 	m_pButton_queststop	;
	if( 	m_pEdit_questexplain	 )  	delete 	m_pEdit_questexplain	;
	if( 	m_pEdit_condition_fee	 )  	delete 	m_pEdit_condition_fee	;
	if( 	m_pEdit_condition_monster	 )  	delete 	m_pEdit_condition_monster	;
	if( 	m_pEdit_monster_num	 )  	delete 	m_pEdit_monster_num	;
	if( 	m_pEdit_condition_time	 )  	delete 	m_pEdit_condition_time	;
	if( 	m_pEdit_reward_item	 )  	delete 	m_pEdit_reward_item	;
	if( 	m_pEdit_reward_num	 )  	delete 	m_pEdit_reward_num	;
	if( 	m_pEdit_reward_exp	 )  	delete 	m_pEdit_reward_exp	;
	if( 	m_pList_quest_select	 )  	delete 	m_pList_quest_select	;
	if( 	m_pOutLine_NONAME1	 )  	delete 	m_pOutLine_NONAME1	;

	SetTimerOnOff( false );
}
	
void CNDailyQuestSelectDlg::Initialize()
{
	return;
}

void CNDailyQuestSelectDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient *)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NDailyQuestDlg/DLG_NDailyQuest.ddf"));

		file.CreatePopupDialog( this, NDAILYQUEST_DLG, TEXT("dialog_dailyquest"), NDailyQuestSelectStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_dailyquest") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pImageStatic_portrait	 = new 	CImageStatic	( this )	;
		m_pListView_reward_item	 = new 	CListView	( this )	;
		m_pStatic_NONAME1	 = new 	CStatic	( this )	;
		m_pStatic_NONAME2	 = new 	CStatic	( this )	;
		m_pStatic_NONAME3	 = new 	CStatic	( this )	;
		m_pStatic_NONAME4	 = new 	CStatic	( this )	;
		m_pButton_questrefresh	 = new 	CButton	( this )	;
		m_pButton_queststart	 = new 	CButton	( this )	;
		m_pButton_queststop	 = new 	CButton	( this )	;
		m_pEdit_questexplain	 = new 	CEdit	( this )	;
		m_pEdit_condition_fee	 = new 	CEdit	( this )	;
		m_pEdit_condition_monster	 = new 	CEdit	( this )	;
		m_pEdit_monster_num	 = new 	CEdit	( this )	;
		m_pEdit_condition_time	 = new 	CEdit	( this )	;
		m_pEdit_reward_item	 = new 	CEdit	( this )	;
		m_pEdit_reward_num	 = new 	CEdit	( this )	;
		m_pEdit_reward_exp	 = new 	CEdit	( this )	;
		m_pList_quest_select	 = new 	CList	( this )	;
		m_pOutLine_NONAME1	 = new 	COutLine	( this )	;

		file.CreateControl( 	m_pImageStatic_portrait	, 	NDAILYQUEST_IMAGESTATIC_PORTRAIT	, TEXT("imagestatic_portrait") )	;
		file.CreateControl( 	m_pListView_reward_item	, 	NDAILYQUEST_LISTVIEW_REWARD_ITEM	, TEXT("listview_reward_item") )	;
		file.CreateControl( 	m_pStatic_NONAME1	, 	NDAILYQUEST_STATICTEXT_NONAME1	, TEXT("statictext_NONAME1")	);
		file.CreateControl( 	m_pStatic_NONAME2	, 	NDAILYQUEST_STATICTEXT_NONAME2	, TEXT("statictext_NONAME2")	);
		file.CreateControl( 	m_pStatic_NONAME3	, 	NDAILYQUEST_STATICTEXT_NONAME3	, TEXT("statictext_NONAME3")	);
		file.CreateControl( 	m_pStatic_NONAME4	, 	NDAILYQUEST_STATICTEXT_NONAME4	, TEXT("statictext_NONAME4")	);
		file.CreateControl( 	m_pButton_questrefresh	, 	NDAILYQUEST_BUTTON_QUESTREFRESH	, TEXT("button_questrefresh" )	);
		file.CreateControl( 	m_pButton_queststart	, 	NDAILYQUEST_BUTTON_QUESTSTART	, TEXT("button_queststart")	);
		file.CreateControl( 	m_pButton_queststop	, 	NDAILYQUEST_BUTTON_QUESTSTOP	, TEXT("button_queststop")	);
		file.CreateControl( 	m_pEdit_questexplain	, 	NDAILYQUEST_EDITBOX_QUESTEXPLAIN	, TEXT("editbox_questexplain")	);
		file.CreateControl( 	m_pEdit_condition_fee	, 	NDAILYQUEST_EDITBOX_CONDITION_FEE	, TEXT("editbox_condition_fee")	);
		file.CreateControl( 	m_pEdit_condition_monster	, 	NDAILYQUEST_EDITBOX_CONDITION_MONSTER	, TEXT("editbox_condition_monster") )	;
		file.CreateControl( 	m_pEdit_monster_num	, 	NDAILYQUEST_EDITBOX_MONSTER_NUM	, TEXT("editbox_monster_num") )	;
		file.CreateControl( 	m_pEdit_condition_time	, 	NDAILYQUEST_EDITBOX_CONDITION_TIME	, TEXT("editbox_condition_time") )	;
		file.CreateControl( 	m_pEdit_reward_item	, 	NDAILYQUEST_EDITBOX_REWARD_ITEM	, TEXT("editbox_reward_item") )	;
		file.CreateControl( 	m_pEdit_reward_num	, 	NDAILYQUEST_EDITBOX_REWARD_NUM	, TEXT("editbox_reward_num") )	;
		file.CreateControl( 	m_pEdit_reward_exp	, 	NDAILYQUEST_EDITBOX_REWARD_EXP	, TEXT("editbox_reward_exp") )	;
		file.CreateControl( 	m_pList_quest_select	, 	NDAILYQUEST_LISTBOX_QUEST_SELECT	, TEXT("listbox_quest_select") )	;
		file.CreateControl( 	m_pOutLine_NONAME1	, 	NDAILYQUEST_OUTLINE_NONAME1	, TEXT("outline_NONAME1") )	;

		//  todo - item 연결
		//pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_REWARD_ITEM ), LVSIL_NORMAL );
		//m_hDailyQuestSelectListCtrl = GetDlgItem( hDlg, IDC_LIST_QUEST_SELECT );

		m_pListView_reward_item->SetBorder( true );

		TCHAR* pText = GetTxtFromMgr(1869);
		m_pList_quest_select->SetColumn( 0, 110, pText );
		pText = GetTxtFromMgr(1870);
		m_pList_quest_select->SetColumn( 1, 102, pText );
		pText = GetTxtFromMgr(1871);
		m_pList_quest_select->SetColumn( 2, 80, pText );
		m_pList_quest_select->SetColumn( 3, 18, TEXT("") );

		m_pList_quest_select->SetBKColorUse( true );
		m_pList_quest_select->SetBorder( true );
		m_pList_quest_select->Refresh();
		
		pText = GetTxtFromMgr(1872);
		m_pEdit_questexplain->SetText( pText );

		m_pButton_questrefresh->Enable( false );
		m_pButton_queststop->Enable( false );

		pText = GetTxtFromMgr(1256); // "수상한 모병관"
		pText = GetTxtFromMgr(2691); // "수상한모병관"

		// 얼굴 이미지 불러온다.

		SI16 tmpSelectedIndex = pclClient->pclKindInfoSet->FindKindFromName( pText );
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pImageStatic_portrait->SetFileName(szPortrait);
		m_pImageStatic_portrait->SetFontIndex( siPortraitFont );

		m_bInitStatus = true;
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CNDailyQuestSelectDlg::NDailyQuestSelectStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    CNDailyQuestSelectDlg *pThis = (CNDailyQuestSelectDlg*)pControllerMgr;
	pThis->NDailyQuestSelectProc( nEvent, nControlID, pControl );
}

void CALLBACK CNDailyQuestSelectDlg::NDailyQuestSelectProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NDAILYQUEST_LISTBOX_QUEST_SELECT:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					stListBoxItemData *pListBoxItemData = NULL; 
					SI16 siRow = -1, siCol = -1; 
					m_pList_quest_select->GetSelectedIndex( &siRow, &siCol ); 
					if( siRow == -1 || siCol == -1 ) return; 
					pListBoxItemData = m_pList_quest_select->GetItemData( siRow, siCol ); 
					if(pListBoxItemData == NULL) return; 
					m_siSelectedListIndex = pListBoxItemData->siParam[0]; 
					
					SI16 Count = 0;

					for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
					{
						if ( m_pDailyQuestObjDataList[ i ].m_siMonsterKind > 0 )
						{
							Count++;
							
							if ( Count == m_siSelectedListIndex + 1 )
							{
								TCHAR *MonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind );
								if ( MonsterName == NULL )
								{
									return;
								}

								TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pDailyQuestObjDataList[ i ].m_siRewardItemUnique );
								if ( ItemName == NULL )
								{
									return;
								}

								LoadDailyQuestListInfo( MonsterName, m_pDailyQuestObjDataList[ i ].m_siMonsterNum, m_pDailyQuestObjDataList[ i ].m_siRewardItemUnique, ItemName, m_pDailyQuestObjDataList[ i ].m_siRewardItemNum, m_pDailyQuestObjDataList[ i ].m_siRewardMoney, m_pDailyQuestObjDataList[ i ].m_siRewardExp, m_pDailyQuestObjDataList[ i ].m_siLeftTime, m_pDailyQuestObjDataList[ i ].m_siFee );
								break;
							}
							
						}
					}
				}
				break;
			}
		}
		break;
		
	case NDAILYQUEST_BUTTON_QUESTREFRESH	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:		// 퀘스트 리스트 다시 요청
				Refresh();
				return ;
			}
		}
		break;
	case NDAILYQUEST_BUTTON_QUESTSTART	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:			// 퀘스트 참가
				{
					if ( m_bInitStatus )
					{
						if ( pclClient->siServiceArea == ConstServiceArea_China )
						{
							if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
							{
								pclclient->CreateInterface( NCOMMONMSG_DLG );
							}
							((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_DAILYQUEST_REWARD);
						}
						else
						{
							SelectRewardMoney( FALSE );
						}
					}
					else
					{
						if  ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
						{
							TCHAR* pText = GetTxtFromMgr(1873);
							m_pEdit_questexplain->SetText( pText );
							return ;
						}
						
						if ( m_siSelectedListIndex > -1 )
						{
							SI16 Count = 0;
							for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
							{
								if ( m_pDailyQuestObjDataList[ i ].m_siMonsterKind > 0 )
								{
									Count++;
									
									if ( Count == m_siSelectedListIndex + 1 )
									{
										if ( m_pDailyQuestObjDataList[ i ].m_siFee > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
										{
											TCHAR* pTitle = GetTxtFromMgr(1874);
											TCHAR* pText = GetTxtFromMgr(1875);
											pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, m_pDailyQuestObjDataList[ i ].m_siFee );
											return ;
										}
										cltGameMsgRequest_DailyQuestSelect clDailyQuestSelect( &m_pDailyQuestObjDataList[ i ], m_bMoneySelected );
										cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTSELECT, sizeof( clDailyQuestSelect ), (BYTE*)&clDailyQuestSelect );
										pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
										
										pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0 );
										break;
									}
								}
							}
						}
					}
				}
				break;
			}
		}
		break;
	case NDAILYQUEST_BUTTON_QUESTSTOP	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:			// 퀘스트 중지
				{
					if  ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
					{
						cltGameMsgRequest_DailyQuestEnd clDailyQuestEnd;
						cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTEND, sizeof( clDailyQuestEnd ), (BYTE*)&clDailyQuestEnd );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

						pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_GIVEUP")), 0, 0 );
					}
				}
				break;
			}
		}
		break;
	}

	return ;
}

bool CNDailyQuestSelectDlg::IsShow()
{
	return m_bShow;
}

void CNDailyQuestSelectDlg::Show()
{
	if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
	{
		if ( m_bInitStatus )
		{
			m_bInitStatus = false;
			SetActionTimer( 5000 );
			SetTimerOnOff( true );
		}

		m_pButton_questrefresh->Enable( true );
		Refresh();
	}
	else if ( ! m_bInitStatus )
	{
		Refresh();
	}

	m_bDailyQuestSuccessEnd = false;
	
	m_bShow = true;
	CDialogController::Show( true );
	
	return;
}

void CNDailyQuestSelectDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );

	return;
}

void CNDailyQuestSelectDlg::LoadDailyQuestList()// 데일리 퀘스트를 읽어 온다.
{
	m_pEdit_questexplain->SetText( TEXT("") );
	m_pList_quest_select->Clear();

	m_siSelectedListIndex = -1;


	SI16 siCount = 0;
	SI32 siSortIndex = 0;

	TCHAR Buffer[ 512 ] = TEXT("");
	
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
	{
		if ( m_pDailyQuestObjDataList[ i ].m_siMonsterKind > 0 )
		{
			if ( pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind ) == NULL )
			{
				continue;
			}

			StringCchPrintf( Buffer, 512, TEXT("%s"), (TCHAR *)pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind) );

			SI32 monsterBattlePower = 0;
			SI32 exp = 0;
			SI32 kind = m_pDailyQuestObjDataList[ i ].m_siMonsterKind;
			pclClient->pclKindInfoSet->GetDailyQuestKindInfo( &kind, &monsterBattlePower, &exp );
			SI32 myBattlePower = pclClient->pclCM->CR[1]->pclCI->clIP.GetStr() + pclClient->pclCM->CR[1]->pclCI->clIP.GetDex() + pclClient->pclCM->CR[1]->pclCI->clIP.GetMag() + pclClient->pclCM->CR[1]->pclCI->clIP.GetVit();

			SI32 monsterWeakValue = ( 100 * monsterBattlePower ) / myBattlePower ;
			bool changeTextColor = false;
			if( 60 > monsterWeakValue || 150 < monsterWeakValue )
				changeTextColor = true;

			COLORREF textColor;
			if( changeTextColor )				
			{
				if( pclClient->IsCountrySwitch(Switch_DalilyQuestGrayList) )
				{
					textColor = RGB( 192, 192, 192 );
				}
				else
				{
					textColor = RGB( 0, 0, 0 );
				}
			}
			else
				textColor = RGB( 0, 0, 0 );
			
			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.siParam[0] = siCount;
			tmpListBoxItemData.Set(0, Buffer, textColor);

			StringCchPrintf( Buffer, 512, TEXT("%d"), m_pDailyQuestObjDataList[ i ].m_siMonsterNum );
			tmpListBoxItemData.Set(1, Buffer, textColor);
			
			if ( m_pDailyQuestObjDataList[ i ].m_siLeftTime > 60 )
			{
				SI16 Minutes = m_pDailyQuestObjDataList[ i ].m_siLeftTime / 60;
				SI16 Seconds = m_pDailyQuestObjDataList[ i ].m_siLeftTime % 60;

				if ( Seconds == 0 )	
				{
					TCHAR* pText = GetTxtFromMgr(1252);
					StringCchPrintf( Buffer, 512, pText, Minutes );		
				}
				else
				{	
					TCHAR* pText = GetTxtFromMgr(1253);
					StringCchPrintf( Buffer, 512, pText, Minutes, Seconds );	
				}
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(1254);
				StringCchPrintf( Buffer, 512, pText, m_pDailyQuestObjDataList[ i ].m_siLeftTime );
			}

			tmpListBoxItemData.Set(2, Buffer, textColor);
			m_pList_quest_select->AddItem( &tmpListBoxItemData );
			siCount++;
		}
	}

	NListSort(m_pList_quest_select, 0 );
	m_pList_quest_select->Refresh();

	return;
}

void CNDailyQuestSelectDlg::Refresh()
{
	m_siSelectedListIndex = -1;
	//ListView_DeleteAllItems( m_hDailyQuestSelectListCtrl );
	m_pList_quest_select->Clear();

	memset( m_pDailyQuestObjDataList, 0, sizeof( CDailyQuestObjData ) * MAX_AVAILABLE_DAILYQUEST_NUM );

	cltGameMsgRequest_DailyQuestList clDailyQuestList;
	cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTLIST, sizeof( clDailyQuestList ), (BYTE*)&clDailyQuestList );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	LoadCurrentDailyQuestSituationText();

	return;
}

void CNDailyQuestSelectDlg::SetList( CDailyQuestObjData *pDailyQuestObjDataList )
{	
	memcpy( m_pDailyQuestObjDataList, pDailyQuestObjDataList, sizeof( CDailyQuestObjData ) * MAX_AVAILABLE_DAILYQUEST_NUM );

	LoadDailyQuestList();
	LoadCurrentDailyQuestSituationText();
	
	return;
}

void CNDailyQuestSelectDlg::LoadCurrentDailyQuestSituationText()
{
	if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
	{
		TCHAR Buffer[ 512 ] = TEXT("");

		cltPDailyQuestInfo *pDailyQuestInfo = &pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo;
		if ( pDailyQuestInfo == NULL )
		{
			return;
		}
	
		TCHAR *MonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( pDailyQuestInfo->siMonsterKind );

		if ( MonsterName == NULL )
		{
			return;
		}
		
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pDailyQuestInfo->siRewardItemUnique );
		if ( ItemName == NULL )
		{
			return;
		}
		
		TCHAR Time[ 64 ] = TEXT("");

		if ( pDailyQuestInfo->siLeftTime > 60 )
		{
			SI16 Minutes = pDailyQuestInfo->siLeftTime / 60;
			SI16 Seconds = pDailyQuestInfo->siLeftTime % 60;
			
			if( Seconds == 0 )
			{
				TCHAR* pText = GetTxtFromMgr(1252);
				StringCchPrintf( Time, 64, pText, Minutes );
				pText = GetTxtFromMgr(1876);
				TCHAR num[128]= TEXT(" ") ;
				TCHAR n1[128]= TEXT(" ") ;
				TCHAR n2[128]= TEXT(" ") ;
				TCHAR itemnum[128]= TEXT(" ") ;
				TCHAR exp[128]= TEXT(" ") ;
				
				StringCchPrintf(num, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(n1, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(n2, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(itemnum, 128, TEXT("%d"),pDailyQuestInfo->siRewardItemNum);
				StringCchPrintf(exp, 128, TEXT("%d"),pDailyQuestInfo->siRewardExp );
				Nsprintf(Buffer,pText,TEXT("monster"),  MonsterName  ,TEXT("num"),  num  ,TEXT("n1"), n1   ,TEXT("n2"), n2   ,TEXT("time"),  Time,TEXT("item"),ItemName ,TEXT("itemnum"),itemnum, TEXT("exp"), exp ,NULL);

				
				//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum, Time, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(1253);
				StringCchPrintf( Time, 64, pText, Minutes, Seconds );
				pText = GetTxtFromMgr(1876);
				TCHAR num[128]= TEXT(" ") ;
				TCHAR n1[128]= TEXT(" ") ;
				TCHAR n2[128]= TEXT(" ") ;
				TCHAR itemnum[128]= TEXT(" ") ;
				TCHAR exp[128]= TEXT(" ") ;
				
				StringCchPrintf(num, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(n1, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(n2, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(itemnum, 128, TEXT("%d"),pDailyQuestInfo->siRewardItemNum);
				StringCchPrintf(exp, 128, TEXT("%d"),pDailyQuestInfo->siRewardExp );
				Nsprintf(Buffer,pText,TEXT("monster"),  MonsterName  ,TEXT("num"),  num  ,TEXT("n1"), n1   ,TEXT("n2"), n2   ,TEXT("time"),  Time,TEXT("item"),ItemName ,TEXT("itemnum"),itemnum, TEXT("exp"), exp ,NULL);

				
				//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum, Time, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );
			}
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1876);
				TCHAR num[128]= TEXT(" ") ;
				
				TCHAR n1[128]= TEXT(" ") ;
				TCHAR n2[128]= TEXT(" ") ;
				TCHAR itemnum[128]= TEXT(" ") ;
				TCHAR exp[128]= TEXT(" ") ;
				TCHAR time[128]= TEXT(" ") ;
				
				StringCchPrintf(num, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(n1, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(n2, 128, TEXT("%d"),pDailyQuestInfo->siMonsterNum);
				StringCchPrintf(itemnum, 128, TEXT("%d"),pDailyQuestInfo->siRewardItemNum);
				StringCchPrintf(exp, 128, TEXT("%d"),pDailyQuestInfo->siRewardExp );
				StringCchPrintf(time, 128, TEXT("%d"),pDailyQuestInfo->siLeftTime);
				Nsprintf(Buffer,pText,TEXT("monster"),  MonsterName  ,TEXT("num"),  num  ,TEXT("n1"), n1   ,TEXT("n2"), n2   ,TEXT("time"),  time ,TEXT("item"),ItemName ,TEXT("itemnum"),itemnum, TEXT("exp"), exp ,NULL);

				
			//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siLeftTime, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );
		}
		
		//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), Buffer );
		//EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTART ), FALSE );
		//EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTOP ), TRUE );
		m_pEdit_questexplain->SetText( Buffer );
		m_pButton_queststart->Enable( false );
		m_pButton_queststop->Enable( true );
	}
	else
	{
		//EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTART ), TRUE );
		//EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTOP ), FALSE );
		m_pButton_queststart->Enable( true );
		m_pButton_queststop->Enable( false );
		TCHAR* pText = GetTxtFromMgr(1877);
		//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), pText );
		m_pEdit_questexplain->SetText( pText );

	}

	return;
}

void CNDailyQuestSelectDlg::LoadDailyQuestListInfo( TCHAR *MonsterName, SI16 MonsterNum, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp, SI32 LeftTime, GMONEY Fee )
{
	SI16 RandIndex = rand() % 10;

	TCHAR Time[ 64 ] = TEXT("");

	if ( LeftTime > 60 )
	{
		SI16 Minutes = LeftTime / 60;
		SI16 Seconds = LeftTime % 60;
		
		if( Seconds == 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1252);
			StringCchPrintf( Time, 64, pText, Minutes );
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1253);
			StringCchPrintf( Time, 64, pText, Minutes, Seconds );
		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1254);
		StringCchPrintf( Time, 64, pText, LeftTime );
	}

	TCHAR Buffer[ 1024 ] = TEXT("");
	TCHAR* pText = GetTxtFromMgr( 1878 + RandIndex );
	TCHAR num[128]= TEXT(" ") ;		
	TCHAR itemnum[128]= TEXT(" ") ;
	TCHAR exp[128]= TEXT(" ") ;
	TCHAR fee[128]= TEXT(" ") ;
	TCHAR money[128]= TEXT(" ") ;

	StringCchPrintf(num, 128, TEXT("%d"),MonsterNum);	
	StringCchPrintf(itemnum, 128, TEXT("%d"),ItemNum);

	g_JYLibFunction.SetNumUnit( RewardExp, exp, 128);
	g_JYLibFunction.SetNumUnit( Fee, fee, 128);
	g_JYLibFunction.SetNumUnit( RewardMoney, money, 128);

	Nsprintf(Buffer,pText,TEXT("monster"), MonsterName   ,TEXT("num"),   num ,TEXT("item"),ItemName,TEXT("itemnum"),  itemnum  ,TEXT("exp"),  exp  ,TEXT("fee"),fee, TEXT("money"), money, NULL);

	
	//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), Buffer );
	m_pEdit_questexplain->SetText( Buffer );
	DrawDailQuestInfo(Fee, MonsterName, MonsterNum, LeftTime, ItemUnique, ItemName, ItemNum, RewardMoney, RewardExp);
			
	return;
}

void CNDailyQuestSelectDlg::DrawDailQuestInfo(GMONEY Fee , TCHAR *MonsterName, SI16 MonsterNum, SI32 LeftTime, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp)
{	
	TCHAR Buffer[256 ] = TEXT("");
	TCHAR* pText = TEXT("");

	/*pText = GetTxtFromMgr(1640);
	StringCchPrintf(Buffer, 256, pText, Fee);
	m_pEdit_condition_fee->SetText( Buffer );*/
	
	TCHAR		szPrice[64] = { '\0', };
	GMONEY		siPrice = Fee ;
	g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
	NTCHARString512	kText(GetTxtFromMgr(1640));
	kText.Replace("#money#",szPrice);
	m_pEdit_condition_fee->SetText( kText );
	
	
	StringCchPrintf(Buffer, 256, TEXT("%s"), MonsterName);
	m_pEdit_condition_monster->SetText( Buffer );

	pText = GetTxtFromMgr(3911);
	StringCchPrintf(Buffer, 256, pText, MonsterNum);
	m_pEdit_monster_num->SetText( Buffer );

	if ( LeftTime > 60 )
	{
		SI16 Minutes = LeftTime / 60;
		SI16 Seconds = LeftTime % 60;
		
		if( Seconds == 0 )	{	
			TCHAR* pText = GetTxtFromMgr(1252);
			StringCchPrintf( Buffer, 256, pText, Minutes );
		}
		else{	
			TCHAR* pText = GetTxtFromMgr(1253);
			StringCchPrintf( Buffer, 256, pText, Minutes, Seconds );		
		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1254);
		StringCchPrintf( Buffer, 256, pText, LeftTime );
	}

	m_pEdit_condition_time->SetText( Buffer );

	pText = GetTxtFromMgr(3910);
	StringCchPrintf(Buffer, 256, pText, RewardExp);
	m_pEdit_reward_exp->SetText( Buffer );

	if ( m_bMoneySelected )
	{
		TCHAR money[32] = {0,};
		g_JYLibFunction.SetNumUnit( RewardMoney, money, 128);
		pText = GetTxtFromMgr( 611 );

		StringCchCat( money, 32, pText );
		m_pEdit_reward_item->SetText( money );
	}
	else
	{
		StringCchPrintf(Buffer, 256, TEXT("%s"), ItemName);
		m_pEdit_reward_item->SetText( Buffer );

		pText = GetTxtFromMgr(2209);
		StringCchPrintf(Buffer, 256, pText, ItemNum);
		m_pEdit_reward_num->SetText( Buffer );

		cltItem clItem;
		clItem.siUnique = ItemUnique;
		clItem.siItemNum = 1;

		SetNInventoryInListView( m_pListView_reward_item, &clItem );
	}
}

void CNDailyQuestSelectDlg::DailyQuestComplete( bool bComplete )
{
	m_bDailyQuestSuccessEnd = bComplete;

	TCHAR Buffer[ 512 ] = TEXT("");
	m_siDailyQuestSuccessViewCounter = 0;
	
	if ( m_bDailyQuestSuccessEnd )
	{
		SI16 RandIndex = rand() % 4;
		TCHAR* pText = GetTxtFromMgr(3912);
		
		switch ( RandIndex )
		{
		case 0:
			pText = GetTxtFromMgr(3912);
			StringCchPrintf( Buffer, 512, pText );
			break;
		case 1:
			pText = GetTxtFromMgr(3913);
			StringCchPrintf( Buffer, 512, pText );
			break;		
		case 2:
			pText = GetTxtFromMgr(3914);
			StringCchPrintf( Buffer, 512, pText );
			break;
		case 3:
			pText = GetTxtFromMgr(3915);
			StringCchPrintf( Buffer, 512, pText );
			break;
		}

		m_pEdit_questexplain->SetText( Buffer );
	}
	else
	{	
		TCHAR* pText = GetTxtFromMgr(3916);
		m_pEdit_questexplain->SetText( pText );
	}
	
	m_pButton_queststart->Enable( true );
	m_pButton_queststop->Enable( false );

	return;
}

void CNDailyQuestSelectDlg::Action()
{
	if ( m_bDailyQuestSuccessEnd )
	{
		++m_siDailyQuestSuccessViewCounter;

		if ( m_siDailyQuestSuccessViewCounter > 1 )
		{
			m_bDailyQuestSuccessEnd = false;
			m_siDailyQuestSuccessViewCounter = 0;
			Refresh();
		}
	}
}

void CNDailyQuestSelectDlg::SetNInventoryInListView(CListView* in_pListView, cltItem* in_pCltItem )
{
	SI16 StartPos = 0;

	SI16 i = 0, j = 0;

	SI32 id = 1;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	in_pListView->DeleteAllItems();

	StartPos = 0;/*PERSONITEM_INV0 + ( j * MAX_ITEM_PER_INVENTORY );*/
	in_pListView->SetStartItemPos( StartPos );

	if ( in_pCltItem->siUnique > 0 )
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( in_pCltItem->siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( in_pCltItem->siUnique );

			if ( ItemName )
			{
				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
				pclClient->pclItemManager->GetExplain( in_pCltItem, ToolTipMsg );

				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, /*i - StartPos*/0, in_pCltItem->siItemNum, ItemName, ToolTipMsg );
				in_pListView->SetStartItemPos( 0 );
				in_pListView->InsertItem( &ListViewItemData );
			}
		}
	}
}

// _LEON_MADE_NLISTBOX_SORT
void CNDailyQuestSelectDlg::NListSort(CList* in_pList, SI16 in_SortColumn )
{
	if( in_pList == NULL || in_SortColumn < 0 || in_SortColumn >= MAX_LISTBOX_COLUMN_NUM ) return;

	stListBoxItemData	*tmpListBoxItemDatas = new stListBoxItemData[ MAX_LISTBOX_ITEM_NUM ];
	stListBoxItemData	*pTmpListBoxItemData = NULL;
	SI16 siCount = 0;

	SI32 i;

	for( i= 0; i<MAX_LISTBOX_ITEM_NUM; i++)
	{
        tmpListBoxItemDatas[i].Init();
	}

	pTmpListBoxItemData = in_pList->GetItemData( 0 );
	while( pTmpListBoxItemData != NULL )
	{
		memcpy(tmpListBoxItemDatas + siCount, pTmpListBoxItemData, sizeof(stListBoxItemData));
        siCount++;
		if(siCount >= MAX_LISTBOX_ITEM_NUM) break;
		pTmpListBoxItemData = in_pList->GetItemData( siCount );
	}

	//TCHAR buffer[256];
	TCHAR text[5000] =TEXT("");

	SI32 clearnum	= 0;
	SI32 totalnum	= 0;
	//데이터를 갱신하기 전에 깨끗이 지운다.
	in_pList->Clear();

	SI32  siSortIndexByName[ MAX_LISTBOX_ITEM_NUM ];

	memset( siSortIndexByName, -1, sizeof(siSortIndexByName));
 
	for( i = 0; i < siCount;i++ )
	{
		siSortIndexByName[i] = i;
	}

	//  i-1로 했으므로 0부터..
	for( i = 0; i < siCount; i++ )
	{
		if( siSortIndexByName[ 0 ] < 0  ) continue;
		const TCHAR* pBuffer =  tmpListBoxItemDatas[ siSortIndexByName[ 0 ] ].strText[in_SortColumn];

		for (SI32 j=1; j<siCount - i; j++)
		{
			if( siSortIndexByName[ j ] < 0 ) continue;
			const TCHAR* pname =  tmpListBoxItemDatas[ siSortIndexByName[ j ] ].strText[in_SortColumn];
            if( _tcscmp( pBuffer, pname) > 0 )
			{
				SI32 siTmp = 0;
				siTmp = siSortIndexByName[ j-1 ];
				siSortIndexByName[ j-1 ] = siSortIndexByName[ j ];
				siSortIndexByName[ j ] = siTmp;
			}

			pBuffer = tmpListBoxItemDatas[ siSortIndexByName[ j ] ].strText[in_SortColumn];
		}
	}

	for(i = 0; i < siCount; i++)
	{
		if( siSortIndexByName[ i ] < 0 ) continue;

		in_pList->AddItem( &(tmpListBoxItemDatas[ siSortIndexByName[ i ] ]) );
	}

	delete [] tmpListBoxItemDatas;

	in_pList->Refresh();

}

void CNDailyQuestSelectDlg::SelectRewardMoney( BOOL MoneySelected )
{
	m_bMoneySelected = MoneySelected;


	m_bInitStatus = false;
	Refresh();
	m_pButton_questrefresh->Enable( true );

	SetActionTimer( 5000 );
	SetTimerOnOff( true );

	if ( MoneySelected )
	{
		m_pStatic_NONAME4->MoveController( 160, 135 );
		m_pEdit_reward_item->MoveController( 211, 155 );

		m_pEdit_reward_num->Show( false );
		m_pListView_reward_item->Show( false );
	}
}

