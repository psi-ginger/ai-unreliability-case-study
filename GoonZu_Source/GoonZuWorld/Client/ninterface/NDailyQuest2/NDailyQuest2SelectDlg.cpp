/* ==========================================================================
	클래스 :		CNDailyQuestSelectDlg

	작성일 :		05/05/10
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "./NDailyQuest2SelectDlg.h"
#include <tchar.h>
#include "../../CommonLogic/Msg/MsgType-DailyQuest2.h"
#include "../../Common/DailyQuest2/DailyQuest2Obj.h"
//#include "../../Common/DailyQuest/DailyQuestObj.h"//임시;;;

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
#include "../../NInterface/NHiddenQuestDlg/NHiddenQuestDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CDailyQuest2SelectDlg::CDailyQuest2SelectDlg() : CDialogController()
{
	m_siSelectedListIndex = -1;
	m_pDailyQuest2ObjDataList = new CDailyQuest2ObjData[ MAX_AVAILABLE_DAILYQUEST2_NUM ];

	m_pImageStatic_portrait = NULL;
    m_pEdit_questexplain= NULL;

	m_pStatic_NONAME1= NULL;
	m_pListView_Reward_Item= NULL;

	m_pStatic_NONAME3= NULL;
	m_pStatic_NONAME11= NULL;
	m_pStatic_NONAME2= NULL;
	m_pStatic_NONAME12= NULL;

	m_pEdit_Reward_Item= NULL;
	m_pEdit_Reward_EXP= NULL;
	m_pEdit_Reward_Amount= NULL;
	m_pEdit_Reward_Guild_Point= NULL;

	m_pOutLine_NONAME1= NULL;

	m_pStatic_NONAME10= NULL;
	m_pListView_Monster_Picture= NULL;

	m_pStatic_NONAME5= NULL;
	m_pStatic_NONAME6= NULL;
	/*m_pStatic_NONAME7= NULL;*/
	m_pStatic_NONAME8= NULL;
	m_pStatic_NONAME9= NULL;

	m_pEdit_Quest_Time= NULL;
	m_pEdit_Moster_Name= NULL;
	//m_pEdit_Quest_Fee= NULL;
	m_pEdit_Moster_Num= NULL;
	m_pEdit_Limit_Time= NULL;

	m_pOutLine_NONAME4= NULL;

	m_pList_quest_select= NULL;
	m_pButton_questrefresh= NULL;
	m_pButton_queststart= NULL;
	m_pButton_queststop= NULL;

	m_bMoneySelected = FALSE;

}

CDailyQuest2SelectDlg::~CDailyQuest2SelectDlg()
{
	if ( m_pDailyQuest2ObjDataList )
	{
		delete [] m_pDailyQuest2ObjDataList;
		m_pDailyQuest2ObjDataList = NULL;
	}

	if(m_pImageStatic_portrait)  	delete 	m_pImageStatic_portrait;
	if(m_pEdit_questexplain)  		delete 	m_pEdit_questexplain;
	 	
	if(m_pStatic_NONAME1)  			delete 	m_pStatic_NONAME1;
	if(m_pListView_Reward_Item)  	delete 	m_pListView_Reward_Item;
	 	
	if(m_pStatic_NONAME3)  			delete 	m_pStatic_NONAME3;
	if(m_pStatic_NONAME11)  		delete 	m_pStatic_NONAME11;
	if(m_pStatic_NONAME2)  			delete 	m_pStatic_NONAME2;
	if(m_pStatic_NONAME12)  		delete 	m_pStatic_NONAME12;
	 	
	if(m_pEdit_Reward_Item)  		delete 	m_pEdit_Reward_Item;
	if(m_pEdit_Reward_EXP)  		delete 	m_pEdit_Reward_EXP;
	if(m_pEdit_Reward_Amount)  		delete 	m_pEdit_Reward_Amount;
	if(m_pEdit_Reward_Guild_Point) 	delete 	m_pEdit_Reward_Guild_Point;
	 	
	if(m_pOutLine_NONAME1)			delete 	m_pOutLine_NONAME1;
	 		
	if(m_pStatic_NONAME10)			delete 	m_pStatic_NONAME10;
	if(m_pListView_Monster_Picture)	delete	m_pListView_Monster_Picture;
						
	if(m_pStatic_NONAME5)		 	delete	m_pStatic_NONAME5;
	if(m_pStatic_NONAME6)  			delete	m_pStatic_NONAME6;
	//if(m_pStatic_NONAME7)	 		delete	m_pStatic_NONAME7;
	if(m_pStatic_NONAME8)	 		delete	m_pStatic_NONAME8;
	if(m_pStatic_NONAME9)  			delete	m_pStatic_NONAME9;
						
	if(m_pEdit_Quest_Time)			delete	m_pEdit_Quest_Time;
	if(m_pEdit_Moster_Name)			delete	m_pEdit_Moster_Name;
	//if(m_pEdit_Quest_Fee)  			delete	m_pEdit_Quest_Fee;
	if(m_pEdit_Moster_Num)  		delete	m_pEdit_Moster_Num;
	if(m_pEdit_Limit_Time)			delete	m_pEdit_Limit_Time;
						
	if(m_pOutLine_NONAME4)  		delete	m_pOutLine_NONAME4;
					
	if(m_pList_quest_select)		delete	m_pList_quest_select;
	if(m_pButton_questrefresh)		delete	m_pButton_questrefresh;
	if(m_pButton_queststart)		delete	m_pButton_queststart;
	if(m_pButton_queststop)  		delete	m_pButton_queststop;

	SetTimerOnOff( false );
}
	
void CDailyQuest2SelectDlg::Initialize()
{
	return;
}

void CDailyQuest2SelectDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient *)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NDailyQuestDlg2/DLG_NDailyQuest2.ddf"));

		file.CreatePopupDialog( this, NDAILYQUEST2_DLG, TEXT("dialog_dailyquest2"), DailyQuest2SelectStaticProc);

		m_pImageStatic_portrait	 = new 	CImageStatic	( this )	;
		m_pEdit_questexplain	 = new 	CEdit	( this )	;

		m_pStatic_NONAME1	 = new 	CStatic	( this )	;
		m_pListView_Reward_Item	 = new 	CListView	( this )	;

		m_pStatic_NONAME3	 = new 	CStatic	( this )	;
		m_pStatic_NONAME11	 = new 	CStatic	( this )	;
		m_pStatic_NONAME2	 = new 	CStatic	( this )	;
		m_pStatic_NONAME12	 = new 	CStatic	( this )	;
		
		m_pEdit_Reward_Item	 = new 	CEdit	( this )	;
		m_pEdit_Reward_EXP	 = new 	CEdit	( this )	;
		m_pEdit_Reward_Amount	 = new 	CEdit	( this )	;
		m_pEdit_Reward_Guild_Point	 = new 	CEdit	( this )	;
		
		m_pOutLine_NONAME1	 = new 	COutLine	( this )	;

		m_pStatic_NONAME10	 = new 	CStatic	( this )	;
		m_pListView_Monster_Picture	 = new 	CListView	( this )	;

		m_pStatic_NONAME5	 = new 	CStatic	( this )	;
		m_pStatic_NONAME6	 = new 	CStatic	( this )	;
		//m_pStatic_NONAME7	 = new 	CStatic	( this )	;
		m_pStatic_NONAME8	 = new 	CStatic	( this )	;
		m_pStatic_NONAME9	 = new 	CStatic	( this )	;
			
		m_pEdit_Quest_Time	 = new 	CEdit	( this )	;
		m_pEdit_Moster_Name	 = new 	CEdit	( this )	;
		//m_pEdit_Quest_Fee	 = new 	CEdit	( this )	;
		m_pEdit_Moster_Num	 = new 	CEdit	( this )	;
		m_pEdit_Limit_Time	 = new 	CEdit	( this )	;

		m_pOutLine_NONAME4	 = new 	COutLine	( this )	;

		m_pList_quest_select	 = new 	CList	( this )	;
		m_pButton_questrefresh	 = new 	CButton	( this )	;
		m_pButton_queststart	 = new 	CButton	( this )	;
		m_pButton_queststop		= new 	CButton	( this )	;
	

		file.CreateControl( 	m_pImageStatic_portrait	, 	NDAILYQUEST2_IMAGESTATIC_PORTRAIT	, TEXT("imagestatic_portrait") );	// 데미안 얼굴
		file.CreateControl( 	m_pEdit_questexplain	, 	NDAILYQUEST2_EDITBOX_QUESTEXPLAIN	, TEXT("editbox_questexplain")	);	// 퀘스트 설명

		file.CreateControl( 	m_pStatic_NONAME1	, 	NDAILYQUEST2_STATICTEXT_NONAME1	, TEXT("statictext_NONAME1")	);			// [보상]
		file.CreateControl( 	m_pListView_Reward_Item	, 	NDAILYQUEST2_LISTVIEW_REWARD_ITEM	, TEXT("listview_reward_item") );	// 보상아이템 그림

		file.CreateControl( 	m_pStatic_NONAME3	, 	NDAILYQUEST2_STATICTEXT_NONAME3	, TEXT("statictext_NONAME3")	);			// [아이템]
		file.CreateControl( 	m_pStatic_NONAME11	, 	NDAILYQUEST2_STATICTEXT_NONAME11	, TEXT("statictext_NONAME11")	);		// [경험치]
		file.CreateControl( 	m_pStatic_NONAME2	, 	NDAILYQUEST2_STATICTEXT_NONAME2	, TEXT("statictext_NONAME2")	);			// [조건]
		file.CreateControl( 	m_pStatic_NONAME12	, 	NDAILYQUEST2_STATICTEXT_NONAME12	, TEXT("statictext_NONAME12")	);		// [길드 포인트]
		
		file.CreateControl( 	m_pEdit_Reward_Item	, 	NDAILYQUEST2_EDITBOX_REWARD_ITEM	, TEXT("editbox_reward_item") )	;		// 보상아이템 이름
		file.CreateControl( 	m_pEdit_Reward_EXP	, 	NDAILYQUEST2_EDITBOX_REWARD_EXP	, TEXT("editbox_reward_EXP")	);				// 보상경험치양
		file.CreateControl( 	m_pEdit_Reward_Amount	, 	NDAILYQUEST2_EDITBOX_REWARD_AMOUNT	, TEXT("editbox_reward_amount")	);	// 보상 수량
		file.CreateControl( 	m_pEdit_Reward_Guild_Point	, 	NDAILYQUEST2_EDITBOX_REWARD_GUILD_POINT	, TEXT("editbox_guild_Point") );	// 보상 길드 포인트양
		
		file.CreateControl( 	m_pOutLine_NONAME1	, 	NDAILYQUEST2_OUTLINE_NONAME1	, TEXT("outline_NONAME1") )	;					// [보상] 아웃라인

		file.CreateControl( 	m_pStatic_NONAME10	, 	NDAILYQUEST2_STATICTEXT_NONAME10	, TEXT("statictext_NONAME10")	);			// [조건]
		file.CreateControl( 	m_pListView_Monster_Picture	, 	NDAILYQUEST2_LISTVIEW_MONSTER_PICTURE	, TEXT("listview_monsterpicture") );// 잡을 몬스터 얼굴

		file.CreateControl( 	m_pStatic_NONAME5	, 	NDAILYQUEST2_STATICTEXT_NONAME5	, TEXT("statictext_NONAME5")	);			// [쿼스트아이템]
		file.CreateControl( 	m_pStatic_NONAME6	, 	NDAILYQUEST2_STATICTEXT_NONAME6	, TEXT("statictext_NONAME6")	);			// [몬스터명]
		//file.CreateControl( 	m_pStatic_NONAME7	, 	NDAILYQUEST2_STATICTEXT_NONAME7	, TEXT("statictext_NONAME7")	);			// [퀘스트비용]
		file.CreateControl( 	m_pStatic_NONAME8	, 	NDAILYQUEST2_STATICTEXT_NONAME8	, TEXT("statictext_NONAME8")	);			// [몬스터 수]
		file.CreateControl( 	m_pStatic_NONAME9	, 	NDAILYQUEST2_STATICTEXT_NONAME9	, TEXT("statictext_NONAME9")	);			// [제한 시간]
		
		file.CreateControl( 	m_pEdit_Quest_Time	, 	NDAILYQUEST2_EDITBOX_QUEST_ITEM	, TEXT("editbox_quest_item") )	;			// 퀘스트아이템 이름
		file.CreateControl( 	m_pEdit_Moster_Name	, 	NDAILYQUEST2_EDITBOX_MONSTER_NAME	, TEXT("editbox_monster_name") )	;			// 잡을 몬스터 이름
		//file.CreateControl( 	m_pEdit_Quest_Fee	, 	NDAILYQUEST2_EDITBOX_QUEST_FEE	, TEXT("editbox_quest_fee") )	;			// 퀘스트 수락 비용
		file.CreateControl( 	m_pEdit_Moster_Num	, 	NDAILYQUEST2_EDITBOX_MOSTER_NUM	, TEXT("editbox_monster_num") )	;			// 잡을 몬스터 숫자
		file.CreateControl( 	m_pEdit_Limit_Time	, 	NDAILYQUEST2_EDITBOX_LIMIT_TIME	, TEXT("editbox_limit_time") )	;			// 퀘스트 제한 시간

		file.CreateControl( 	m_pOutLine_NONAME4	, 	NDAILYQUEST2_OUTLINE_NONAME4	, TEXT("outline_NONAME4") )	;					// [조건] 아웃라인
		
		file.CreateControl( 	m_pList_quest_select	, 	NDAILYQUEST2_LISTBOX_QUEST_SELECT	, TEXT("listbox_quest_select") )	;
		file.CreateControl( 	m_pButton_questrefresh	, 	NDAILYQUEST2_BUTTON_QUESTREFRESH	, TEXT("button_questrefresh" )	);
		file.CreateControl( 	m_pButton_queststart	, 	NDAILYQUEST2_BUTTON_QUESTSTART	, TEXT("button_queststart")	);
		file.CreateControl( 	m_pButton_queststop	, 	NDAILYQUEST2_BUTTON_QUESTSTOP	, TEXT("button_queststop")	);

		//  todo - item 연결
		/*pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_REWARD_ITEM ), LVSIL_NORMAL );
		m_hDailyQuestSelectListCtrl = GetDlgItem( hDlg, IDC_LIST_QUEST_SELECT );*/

		m_pListView_Reward_Item->SetBorder( true );//보상아이템 이미지
		m_pListView_Monster_Picture->SetBorder(true);//몬스터 얼굴
		//퀘스트 선택 창 칼럼 셋팅		
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
		//퀘스트 설명 창
		pText = GetTxtFromMgr(8365);//퀘스트 창을 열면 나오는 화면 
		m_pEdit_questexplain->SetText( pText );

		m_pButton_questrefresh->Enable( false );
		m_pButton_queststop->Enable( false );

		pText = GetTxtFromMgr(8058); // 데미안.// 사진을 이름으로 가지고 온다.
		//pText = GetTxtFromMgr(2691); // "수상한모병관"

		SI16 tmpSelectedIndex = pclClient->pclKindInfoSet->FindKindFromName( pText );
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pImageStatic_portrait->SetFileName(szPortrait);
		m_pImageStatic_portrait->SetFontIndex( siPortraitFont );
		TCHAR* pSampleText = GetTxtFromMgr(8152);
		m_pEdit_Quest_Time->SetText(pSampleText);
		m_bInitStatus = true;
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CDailyQuest2SelectDlg::DailyQuest2SelectStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    CDailyQuest2SelectDlg *pThis = (CDailyQuest2SelectDlg*)pControllerMgr;
	pThis->DailyQuest2SelectProc( nEvent, nControlID, pControl );
}

void CALLBACK CDailyQuest2SelectDlg::DailyQuest2SelectProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NDAILYQUEST2_LISTBOX_QUEST_SELECT:
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

					for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; ++i )
					{
						if ( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind > 0 )
						{
							Count++;
							
							if ( Count == m_siSelectedListIndex + 1 )
							{
								TCHAR *MonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind );
								if ( MonsterName == NULL )
								{
									return;
								}

								TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pDailyQuest2ObjDataList[ i ].m_siRewardItemUnique );
								if ( ItemName == NULL )
								{
									return;
								}
									;
								SI16 siMonsterNum = m_pDailyQuest2ObjDataList[ i ].m_siMonsterNum	; 
								SI16 siRewardItemUnique = m_pDailyQuest2ObjDataList[ i ].m_siRewardItemUnique	;
								ItemName	;
								SI16 siRewardItemNum = m_pDailyQuest2ObjDataList[ i ].m_siRewardItemNum	;
								GMONEY gRewardMoney = m_pDailyQuest2ObjDataList[ i ].m_siRewardMoney	;
								SI32 siLeftTime = m_pDailyQuest2ObjDataList[ i ].m_siLeftTime	;
								GMONEY gFee = m_pDailyQuest2ObjDataList[ i ].m_siFee	;

								SI32 siRewardExp = m_pDailyQuest2ObjDataList[ i ].m_siRewardExp	;	
								SI32 siCharLeve = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel()	;

								if ( siCharLeve >= 120 ) 
								{
									SI32 siMonserKind = m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind	;
									if ( siMonserKind > 0 ) 
									{
										cltKindInfo* pclki	=	pclClient->pclKindInfoSet->pclKI[siMonserKind]	;
										if(pclki)
										{
											SI32 MonsterExp = pclki->Exp;
											siRewardExp	=	((MonsterExp *  siMonsterNum * 580) / 100) + 100 ;
											// 100 단위 절삭 
											siRewardExp	=	(siRewardExp / 100) * 100	;

										}
										
									}								
								}
								LoadDailyQuest2ListInfo(MonsterName,siMonsterNum ,siRewardItemUnique ,ItemName,siRewardItemNum,  gRewardMoney,siRewardExp, siLeftTime ,gFee );
								break;
							}
							
						}
					}
				}
				break;
			}
		}
		break;
		
	case NDAILYQUEST2_BUTTON_QUESTREFRESH	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:		// 퀘스트 리스트 다시 요청
				Refresh();
				return ;
			}
		}
		break;
	case NDAILYQUEST2_BUTTON_QUESTSTART	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:			// 퀘스트 참가
				{//레벨 90 이상이고,해당 아이템이 없으면 퀘스트를 사용할수 없다고 알림창이 뜬다.
					SI16	charlevel = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();//케릭터 Level
					SI32	itemunique = ITEMUNIQUE(24082);
					if (charlevel >= 90)
					{
						if (FindItemFromInventory(itemunique) == true){
							
							if ( m_bInitStatus )
							{
								if ( pclClient->siServiceArea == ConstServiceArea_China )
								{
									if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
									{
										pclclient->CreateInterface( NCOMMONMSG_DLG );
									}
									((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_DAILYQUEST2_REWARD);
								}
								else
								{
									SelectRewardMoney( FALSE );
								}
							}
							else
							{
								if  ( pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind > 0 )
								{
									TCHAR* pText = GetTxtFromMgr(1873);
									m_pEdit_questexplain->SetText( pText );
									return ;
								}

								if ( m_siSelectedListIndex > -1 )
								{
									SI16 Count = 0;
									for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; ++i )
									{
										//여기에서 보상을 만들어 낸다.								

										if ( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind > 0 )
										{
											Count++;

											if ( Count == m_siSelectedListIndex + 1 )
											{
												if ( m_pDailyQuest2ObjDataList[ i ].m_siFee > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
												{
													TCHAR* pTitle = GetTxtFromMgr(1874);
													TCHAR* pText = GetTxtFromMgr(1875);
													pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, m_pDailyQuest2ObjDataList[ i ].m_siFee );
													break;
												}

												if (rand()%100 == 45 || rand()%100 == 76){
													cltClient *pclclient = (cltClient*)pclClient;
													pclclient->CreateInterface( HIDDENQUEST2_DLG );	
													((NHiddenQuestDlg*)pclclient->m_pDialog[HIDDENQUEST2_DLG])->SetPramformDailyQuest2(i,m_pDailyQuest2ObjDataList);
													DeleteDialog();
												}
												else
												{
													cltGameMsgRequest_DailyQuest2Select clDailyQuestSelect( &m_pDailyQuest2ObjDataList[ i ], m_bMoneySelected,i );
													cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2SELECT, sizeof( clDailyQuestSelect ), (BYTE*)&clDailyQuestSelect );
													pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

													pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_DAILYQUEST_OK")), 0, 0 );
												}
											}
										}
									}
								}
							}
						}
						else{
							TCHAR* pTitle = GetTxtFromMgr(512);//인벤토리에 아이템이 없다고 뜬다. 텍스트 내용은 추가 해야함.
							TCHAR* pText = GetTxtFromMgr(8074);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						}
					}
					else{
						TCHAR* pTitle = GetTxtFromMgr(5598);//레벨 90이 안된다고 뜬다 텍스트 내용은 추가 해야함
						TCHAR* pText = GetTxtFromMgr(8073);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
					}
				}
				break;
			}
		}
		break;
	case NDAILYQUEST2_BUTTON_QUESTSTOP	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:			// 퀘스트 중지
				{
					if  ( pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind > 0 )
					{
						cltGameMsgRequest_DailyQuest2End clDailyQuestEnd;
						cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2END, sizeof( clDailyQuestEnd ), (BYTE*)&clDailyQuestEnd );
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

bool CDailyQuest2SelectDlg::IsShow()
{
	return m_bShow;
}

void CDailyQuest2SelectDlg::Show()
{
	if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind > 0 )
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

void CDailyQuest2SelectDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );

	return;
}

void CDailyQuest2SelectDlg::LoadDailyQuest2List()// 리스트를 읽어 들인다.
{
	m_pEdit_questexplain->SetText( TEXT("") );
	m_pList_quest_select->Clear();

	m_siSelectedListIndex = -1;

	SI16 siCount = 0;
	SI32 siSortIndex = 0;

	TCHAR Buffer[ 512 ] = TEXT("");
	
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST2_NUM; ++i )
	{
		if ( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind > 0 )
		{
			if ( pclClient->pclKindInfoSet->GetName( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind ) == NULL )//몬스터의 이름을 얻어 온다.
			{
				continue;
			}

			StringCchPrintf( Buffer, 512, TEXT("%s"), (TCHAR *)pclClient->pclKindInfoSet->GetName( m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind) );

			SI32 monsterBattlePower = 0;
			SI32 exp = 0;
			SI32 kind = m_pDailyQuest2ObjDataList[ i ].m_siMonsterKind;
			pclClient->pclKindInfoSet->GetDailyQuest2KindInfo( &kind, &monsterBattlePower, &exp );
			SI32 myBattlePower = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();

			SI32 monsterWeakValue = myBattlePower - monsterBattlePower ;//몬스터 레벨
			bool changeTextColor = false;
			if( monsterWeakValue > 40 ){
				changeTextColor = true;
			}

			COLORREF textColor;
			if( changeTextColor )				
			{
				if (pclClient->IsCountrySwitch(Switch_DalilyQuestGrayList	) )
					
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

			StringCchPrintf( Buffer, 512, TEXT("%d"), m_pDailyQuest2ObjDataList[ i ].m_siMonsterNum );
			tmpListBoxItemData.Set(1, Buffer, textColor);
			
			if ( m_pDailyQuest2ObjDataList[ i ].m_siLeftTime > 60 )
			{
				SI16 Minutes = m_pDailyQuest2ObjDataList[ i ].m_siLeftTime / 60;
				SI16 Seconds = m_pDailyQuest2ObjDataList[ i ].m_siLeftTime % 60;

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
				StringCchPrintf( Buffer, 512, pText, m_pDailyQuest2ObjDataList[ i ].m_siLeftTime );
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

void CDailyQuest2SelectDlg::Refresh()
{
	m_siSelectedListIndex = -1;
	m_pList_quest_select->Clear();

	memset( m_pDailyQuest2ObjDataList, 0, sizeof( CDailyQuest2ObjData ) * MAX_AVAILABLE_DAILYQUEST2_NUM );

	cltGameMsgRequest_DailyQuest2List clDailyQuest2List;
	cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUEST2LIST, sizeof( clDailyQuest2List ), (BYTE*)&clDailyQuest2List );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	LoadCurrentDailyQuest2SituationText();

	return;
}

void CDailyQuest2SelectDlg::SetList( CDailyQuest2ObjData *pDailyQuest2ObjDataList )
{	
	memcpy( m_pDailyQuest2ObjDataList, pDailyQuest2ObjDataList, sizeof( CDailyQuest2ObjData ) * MAX_AVAILABLE_DAILYQUEST2_NUM );

	LoadDailyQuest2List();
	LoadCurrentDailyQuest2SituationText();

	return;
}

void CDailyQuest2SelectDlg::LoadCurrentDailyQuest2SituationText()
{
	if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info.siMonsterKind > 0 )
	{
		TCHAR Buffer[ 512 ] = TEXT("");

		cltPDailyQuest2Info *pDailyQuest2Info = &pclClient->pclCM->CR[1]->pclCI->clDailyQuest2Info;
		if ( pDailyQuest2Info == NULL )
		{
			return;
		}
	
		TCHAR *MonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( pDailyQuest2Info->siMonsterKind );

		if ( MonsterName == NULL )
		{
			return;
		}
		
		TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( pDailyQuest2Info->siRewardItemUnique );
		if ( ItemName == NULL )
		{
			return;
		}
		
		TCHAR Time[ 64 ] = TEXT("");

		if ( pDailyQuest2Info->siLeftTime > 60 )
		{
			SI16 Minutes = pDailyQuest2Info->siLeftTime / 60;
			SI16 Seconds = pDailyQuest2Info->siLeftTime % 60;
			
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
				
				StringCchPrintf(num, 128, TEXT("%d"),pDailyQuest2Info->siMonsterNum);
				StringCchPrintf(n1, 128, TEXT("%d"),pDailyQuest2Info->siKillMonsterNum);
				StringCchPrintf(n2, 128, TEXT("%d"),pDailyQuest2Info->siMonsterNum);
				StringCchPrintf(itemnum, 128, TEXT("%d"),pDailyQuest2Info->siRewardItemNum);
				StringCchPrintf(exp, 128, TEXT("%d"),pDailyQuest2Info->siRewardExp );
				Nsprintf(Buffer,pText,TEXT("monster"),  MonsterName  ,TEXT("num"),  num  ,TEXT("n1"), n1   ,TEXT("n2"), n2   ,TEXT("time"),  Time,TEXT("item"),ItemName ,TEXT("itemnum"),itemnum, TEXT("exp"), exp ,NULL);
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
				
				StringCchPrintf(num, 128, TEXT("%d"),pDailyQuest2Info->siMonsterNum);
				StringCchPrintf(n1, 128, TEXT("%d"),pDailyQuest2Info->siKillMonsterNum);
				StringCchPrintf(n2, 128, TEXT("%d"),pDailyQuest2Info->siMonsterNum);
				StringCchPrintf(itemnum, 128, TEXT("%d"),pDailyQuest2Info->siRewardItemNum);
				StringCchPrintf(exp, 128, TEXT("%d"),pDailyQuest2Info->siRewardExp );
				Nsprintf(Buffer,pText,TEXT("monster"),  MonsterName  ,TEXT("num"),  num  ,TEXT("n1"), n1   ,TEXT("n2"), n2   ,TEXT("time"),  Time,TEXT("item"),ItemName ,TEXT("itemnum"),itemnum, TEXT("exp"), exp ,NULL);				
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
				
				StringCchPrintf(num, 128, TEXT("%d"),pDailyQuest2Info->siMonsterNum);
				StringCchPrintf(n1, 128, TEXT("%d"),pDailyQuest2Info->siKillMonsterNum);
				StringCchPrintf(n2, 128, TEXT("%d"),pDailyQuest2Info->siMonsterNum);
				StringCchPrintf(itemnum, 128, TEXT("%d"),pDailyQuest2Info->siRewardItemNum);
				StringCchPrintf(exp, 128, TEXT("%d"),pDailyQuest2Info->siRewardExp );
				StringCchPrintf(time, 128, TEXT("%d"),pDailyQuest2Info->siLeftTime);
				Nsprintf(Buffer,pText,TEXT("monster"),  MonsterName  ,TEXT("num"),  num  ,TEXT("n1"), n1   ,TEXT("n2"), n2   ,TEXT("time"),  time ,TEXT("item"),ItemName ,TEXT("itemnum"),itemnum, TEXT("exp"), exp ,NULL);				
		}
		
		m_pEdit_questexplain->SetText( Buffer );
		m_pButton_queststart->Enable( false );
		m_pButton_queststop->Enable( true );
	}
	else
	{
		m_pButton_queststart->Enable( true );
		m_pButton_queststop->Enable( false );
		TCHAR* pText = GetTxtFromMgr(8396);
		m_pEdit_questexplain->SetText( pText );

	}

	return;
}

void CDailyQuest2SelectDlg::LoadDailyQuest2ListInfo( TCHAR *MonsterName, SI16 MonsterNum, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp, SI32 LeftTime, GMONEY Fee )
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
	TCHAR* pText = GetTxtFromMgr( 8172 + RandIndex );
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
	m_pEdit_questexplain->SetText( Buffer );
	DrawDailQuest2Info(Fee, MonsterName, MonsterNum, LeftTime, ItemUnique, ItemName, ItemNum, RewardMoney, RewardExp);
			
	return;
}

void CDailyQuest2SelectDlg::DrawDailQuest2Info(GMONEY Fee , TCHAR *MonsterName, SI16 MonsterNum, SI32 LeftTime, SI16 ItemUnique, TCHAR *ItemName, SI16 ItemNum, GMONEY RewardMoney, SI32 RewardExp)
{
	SI32 charlevel = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();
	//텍스트를 뿌려준다.
	TCHAR Buffer[256 ] = TEXT("");
	TCHAR Buffer1[256 ] = TEXT("");

	TCHAR* pText = TEXT("");
	StringCchPrintf(Buffer, 256, TEXT("%s"), MonsterName);
	m_pEdit_Moster_Name->SetText( Buffer );

	pText = GetTxtFromMgr(3911);
	StringCchPrintf(Buffer, 256, pText, MonsterNum);
	m_pEdit_Moster_Num->SetText( Buffer );

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

	m_pEdit_Limit_Time->SetText( Buffer );
	
	pText =GetTxtFromMgr(2052);
	m_pEdit_Reward_EXP->SetText( SI32ToString(RewardExp) );//보상경험치

	StringCchPrintf(Buffer1, 256,pText,RewardExp/GuildPoint_Rate);//보상 길드 포인트
	m_pEdit_Reward_Guild_Point->SetText(Buffer1);
	SI32 QuestFee = 0 ;

	if ( m_bMoneySelected )
	{
		TCHAR money[32] = {0,};
		g_JYLibFunction.SetNumUnit( RewardMoney, money, 128);
		pText = GetTxtFromMgr( 611 );

		StringCchCat( money, 32, pText );
		m_pEdit_Reward_Item->SetText( money );
	}
	else
	{
		StringCchPrintf(Buffer, 256, TEXT("%s"), ItemName);
		m_pEdit_Reward_Item->SetText( Buffer );

		pText = GetTxtFromMgr(2209);
		StringCchPrintf(Buffer, 256, pText, ItemNum);
		m_pEdit_Reward_Amount->SetText( Buffer );

		cltItem clItem;
		clItem.siUnique = ItemUnique;
		clItem.siItemNum = 1;

		SetNInventoryInListView( m_pListView_Reward_Item, &clItem );
		//몬스터를 그린다.
		SetMonsterPicture(MonsterName);

	}
}

void CDailyQuest2SelectDlg::DailyQuest2Complete( bool bComplete )
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
		TCHAR* pText = GetTxtFromMgr(8076);
		m_pEdit_questexplain->SetText( pText );
	}
	
	m_pButton_queststart->Enable( true );
	m_pButton_queststop->Enable( false );

	return;
}

void CDailyQuest2SelectDlg::Action()
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

void CDailyQuest2SelectDlg::SetNInventoryInListView(CListView* in_pListView, cltItem* in_pCltItem )
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
void CDailyQuest2SelectDlg::NListSort(CList* in_pList, SI16 in_SortColumn )
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

void CDailyQuest2SelectDlg::SelectRewardMoney( BOOL MoneySelected )
{
	m_bMoneySelected = MoneySelected;


	m_bInitStatus = false;
	Refresh();
	m_pButton_questrefresh->Enable( true );

	SetActionTimer( 5000 );
	SetTimerOnOff( true );

	if ( MoneySelected )
	{
		//m_pStatic_NONAME1->MoveController( 160, 135 );
		m_pEdit_Reward_Item->MoveController( 211, 155 );

		m_pEdit_Reward_Amount->Show( false );
		m_pListView_Reward_Item->Show( false );
	}
}
bool CDailyQuest2SelectDlg:: FindItemFromInventory( SI32 ItemUnique)//해당 아이템이 인벤토리에 있는지 확인한다.
{
	int		Inventory;// 인벤토리, 해당 아이템 
	for( Inventory = PERSONITEM_INV0; Inventory < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; Inventory++ )
	{
		cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[Inventory];
		if(pclItem == NULL)					continue;

		if( pclItem->siUnique == ItemUnique)
		{

			return true;
		}
	}
	return false;
}

//히든 퀘스트 나올때 할 행동.
void CDailyQuest2SelectDlg :: RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
}
void CDailyQuest2SelectDlg :: SetMonsterPicture(TCHAR *MonsterName){
	TCHAR strFileName[ 128 ] = TEXT("");
	SI16 tmpSelectedIndex = pclClient->pclKindInfoSet->FindKindFromName( MonsterName );
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pListView_Monster_Picture->SetFileName(szPortrait);
	m_pListView_Monster_Picture->SetFontIndex( siPortraitFont );
	m_pListView_Monster_Picture->Refresh();
}