#include "DailyQuestSelectDlg.h"

#include "../../CommonLogic/Msg/MsgType-DailyQuest.h"
#include "../../Common/DailyQuest/DailyQuestObj.h"

#include "../ErrorMsgDlg/ErrorMsgDlg.h"
#include "../NotifyMsgDlg/NotifyMsgDlg.h"


#include "../../Client.h"
#include "../../Music/music.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CDailyQuestSelectDlg::CDailyQuestSelectDlg()
{
	m_hDlg = NULL;
	m_hPortraitBitmap = NULL;
	m_siSelectedListIndex = -1;
	m_hEditBrush	= NULL;
	m_pDailyQuestObjDataList = new CDailyQuestObjData[ MAX_AVAILABLE_DAILYQUEST_NUM ];
}

CDailyQuestSelectDlg::~CDailyQuestSelectDlg()
{
	if ( m_pDailyQuestObjDataList )
	{
		delete [] m_pDailyQuestObjDataList;
		m_pDailyQuestObjDataList = NULL;
	}
	
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}
	
void CDailyQuestSelectDlg::Initialize()
{
	return;
}

void CDailyQuestSelectDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_DAILYQUESTSELECT ), hWnd,  StaticDailyQuestSelectDlgProc );
	Hide();

	return;
}

BOOL CALLBACK CDailyQuestSelectDlg::StaticDailyQuestSelectDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	return pclclient->m_pDailyQuestSelectDlg->DailyQuestSelectDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CDailyQuestSelectDlg::DailyQuestSelectDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();

			m_hDlg = hDlg;

			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_REWARD_ITEM ), LVSIL_NORMAL );

			m_hDailyQuestSelectListCtrl = GetDlgItem( hDlg, IDC_LIST_QUEST_SELECT );
			if(m_hDailyQuestSelectListCtrl)
			{
				ListView_SetExtendedListViewStyle( m_hDailyQuestSelectListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				char* pText = GetTxtFromMgr(1869);
				ListViewInsertColumn( m_hDailyQuestSelectListCtrl, 110, pText, 0, 1);		
				pText = GetTxtFromMgr(1870);
				ListViewInsertColumn( m_hDailyQuestSelectListCtrl,  70, pText  , 1, 1);	
				pText = GetTxtFromMgr(1871);
				ListViewInsertColumn( m_hDailyQuestSelectListCtrl,  80, pText  , 2, 1);		
			}
			
			char* pText = GetTxtFromMgr(1872);

			SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ),pText );

			EnableWindow( GetDlgItem( hDlg, IDC_BUTTON_QUESTREFRESH ), FALSE );
			EnableWindow( GetDlgItem( hDlg, IDC_BUTTON_QUESTSTOP ), FALSE );


			if( m_hPortraitBitmap == NULL )
			{
				m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/Portrait/KIND_DAILYQUEST.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

				if ( m_hPortraitBitmap != NULL )
				{
					SendDlgItemMessage( hDlg, IDC_STATIC_PORTRAIT_DAILYQUEST, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap);
				}
			}

			m_bInitStatus = true;
			
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);
		}
		return TRUE;
	
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_QUESTEXPLAIN ) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_TIMER:
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

			/*
			if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
			{
				pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime -= 5;
				
				if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime < 0 )
				{
					pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siLeftTime = 0;
				}
				
				LoadCurrentDailyQuestSituationText();
			}
			*/
		}
		break;

	case WM_NOTIFY:
		{
			LPNMHDR						hdr;
			
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;
						

			if ( hdr->hwndFrom == m_hDailyQuestSelectListCtrl )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if ( nia->iItem >= 0 )
						{
							if ( nlv->uChanged == LVIF_STATE && nlv->uNewState == ( LVIS_SELECTED | LVIS_FOCUSED ))
							{
								m_siSelectedListIndex = nlv->lParam;
								
								SI16 Count = 0;
								for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
								{
									if ( m_pDailyQuestObjDataList[ i ].m_siMonsterKind > 0 )
									{
										Count++;
										
										if ( Count == m_siSelectedListIndex + 1 )
										{
											char *MonsterName = (char*)pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind );
											if ( MonsterName == NULL )
											{
												return FALSE;
											}

											char *ItemName = (char*)pclClient->pclItemManager->GetName( m_pDailyQuestObjDataList[ i ].m_siRewardItemUnique );
											if ( ItemName == NULL )
											{
												return FALSE;
											}

											LoadDailyQuestListInfo( MonsterName, m_pDailyQuestObjDataList[ i ].m_siMonsterNum, m_pDailyQuestObjDataList[ i ].m_siRewardItemUnique, ItemName, m_pDailyQuestObjDataList[ i ].m_siRewardItemNum, m_pDailyQuestObjDataList[ i ].m_siRewardExp, m_pDailyQuestObjDataList[ i ].m_siLeftTime, m_pDailyQuestObjDataList[ i ].m_siFee );
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
		}
		break;
	case WM_CLOSE:
		{
			Hide();
		}
		return FALSE;
		
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				return FALSE;

			case IDC_BUTTON_QUESTREFRESH:		// 퀘스트 리스트 다시 요청
				Refresh();
				return FALSE;
				
			case IDC_BUTTON_QUESTSTART:			// 퀘스트 참가
				{
					if ( m_bInitStatus )
					{
						m_bInitStatus = false;
						
						Refresh();

						EnableWindow( GetDlgItem( hDlg, IDC_BUTTON_QUESTREFRESH ), TRUE );
						
						SetTimer( m_hDlg, 0, 5000, NULL );
					}
					else
					{
						if  ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
						{
							char* pText = GetTxtFromMgr(1873);
							SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), pText);
							return FALSE;
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
											char* pTitle = GetTxtFromMgr(1874);
											char* pText = GetTxtFromMgr(1875);
											pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, m_pDailyQuestObjDataList[ i ].m_siFee );
											return FALSE;
										}
										
										cltGameMsgRequest_DailyQuestSelect clDailyQuestSelect( &m_pDailyQuestObjDataList[ i ] );
										cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTSELECT, sizeof( clDailyQuestSelect ), (char*)&clDailyQuestSelect );
										pclClient->SendMsgToServer((char*)&clMsg);
										
										pclClient->PushEffect( EFFECT_DAILYQUEST_OK, 0, 0 );
										break;
									}
								}
							}
						}
					}
				}
				break;
			case IDC_BUTTON_QUESTSTOP:			// 퀘스트 중지
				{
					if  ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
					{
						cltGameMsgRequest_DailyQuestEnd clDailyQuestEnd;
						cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTEND, sizeof( clDailyQuestEnd ), (char*)&clDailyQuestEnd );
						pclClient->SendMsgToServer((char*)&clMsg);

						pclClient->PushEffect( EFFECT_DAILYQUEST_GIVEUP, 0, 0 );
					}
				}
				break;
			}
			

		}
		break;

	case WM_DESTROY:
		{
			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
				m_hPortraitBitmap = NULL;
			}
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush= NULL;
			}

			KillTimer( m_hDlg, 0 );
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

bool CDailyQuestSelectDlg::IsShow()
{
	return m_bShow;
}

void CDailyQuestSelectDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
	{
		if ( m_bInitStatus )
		{
			m_bInitStatus = false;
			SetTimer( m_hDlg, 0, 5000, NULL );
		}
		
		EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTREFRESH ), TRUE );
		Refresh();
	}
	else if ( ! m_bInitStatus )
	{
		Refresh();
	}

	m_bDailyQuestSuccessEnd = false;
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );
	
	return;
}

void CDailyQuestSelectDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CDailyQuestSelectDlg::LoadDailyQuestList()
{
	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), "" );
	ListView_DeleteAllItems( m_hDailyQuestSelectListCtrl );
	m_siSelectedListIndex = -1;


	SI16 siCount = 0;
	SI32 siSortIndex = 0;

	char Buffer[ 512 ] = "";
	
	for ( SI16 i = 0; i < MAX_AVAILABLE_DAILYQUEST_NUM; ++i )
	{
		if ( m_pDailyQuestObjDataList[ i ].m_siMonsterKind > 0 )
		{
			if ( pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind ) == NULL )
			{
				continue;
			}

			// SORT를 위해서 
//			if( m_pDailyQuestObjDataList[ i ].m_bPCRoomQuest ) {
//				sprintf( Buffer, "%s(PC방)", (char *)pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind) );
//			} else {
				sprintf( Buffer, "%s", (char *)pclClient->pclKindInfoSet->GetName( m_pDailyQuestObjDataList[ i ].m_siMonsterKind) );
//			}
			 

			siSortIndex = ListViewInsertItem(m_hDailyQuestSelectListCtrl, -1, 0, 0, Buffer, (LPARAM)siCount);
			sprintf( Buffer, "%d", m_pDailyQuestObjDataList[ i ].m_siMonsterNum );
			ListViewInsertItem(m_hDailyQuestSelectListCtrl, -1, siSortIndex, 1, Buffer, -1);
			
			if ( m_pDailyQuestObjDataList[ i ].m_siLeftTime > 60 )
			{
				SI16 Minutes = m_pDailyQuestObjDataList[ i ].m_siLeftTime / 60;
				SI16 Seconds = m_pDailyQuestObjDataList[ i ].m_siLeftTime % 60;

				if ( Seconds == 0 )	
				{
					char* pText = GetTxtFromMgr(1252);
					sprintf( Buffer, pText, Minutes );		
				}
				else
				{	
					char* pText = GetTxtFromMgr(1253);
					sprintf( Buffer, pText, Minutes, Seconds );	
				}
			}
			else
			{
				char* pText = GetTxtFromMgr(1254);
				sprintf( Buffer, pText, m_pDailyQuestObjDataList[ i ].m_siLeftTime );
			}

			ListViewInsertItem(m_hDailyQuestSelectListCtrl, -1, siSortIndex, 2, Buffer, -1);
			siCount++;
		}
	}

	return;
}

void CDailyQuestSelectDlg::Refresh()
{
	m_siSelectedListIndex = -1;
	ListView_DeleteAllItems( m_hDailyQuestSelectListCtrl );
	memset( m_pDailyQuestObjDataList, 0, sizeof( CDailyQuestObjData ) * MAX_AVAILABLE_DAILYQUEST_NUM );

	cltGameMsgRequest_DailyQuestList clDailyQuestList;
	cltMsg clMsg( GAMEMSG_REQUEST_DAILYQUESTLIST, sizeof( clDailyQuestList ), (char*)&clDailyQuestList );
	pclClient->SendMsgToServer((char*)&clMsg);

	LoadCurrentDailyQuestSituationText();

	return;
}

void CDailyQuestSelectDlg::SetList( CDailyQuestObjData *pDailyQuestObjDataList )
{	
	memcpy( m_pDailyQuestObjDataList, pDailyQuestObjDataList, sizeof( CDailyQuestObjData ) * MAX_AVAILABLE_DAILYQUEST_NUM );

	LoadDailyQuestList();
	LoadCurrentDailyQuestSituationText();
	
	return;
}

void CDailyQuestSelectDlg::LoadCurrentDailyQuestSituationText()
{
	if ( pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo.siMonsterKind > 0 )
	{
		char Buffer[ 512 ] = "";

		cltPDailyQuestInfo *pDailyQuestInfo = &pclClient->pclCM->CR[1]->pclCI->clDailyQuestInfo;
		if ( pDailyQuestInfo == NULL )
		{
			return;
		}
	
		char *MonsterName = (char*)pclClient->pclKindInfoSet->GetName( pDailyQuestInfo->siMonsterKind );

		if ( MonsterName == NULL )
		{
			return;
		}
		
		char *ItemName = (char*)pclClient->pclItemManager->GetName( pDailyQuestInfo->siRewardItemUnique );
		if ( ItemName == NULL )
		{
			return;
		}
		
		char Time[ 64 ] = "";

		if ( pDailyQuestInfo->siLeftTime > 60 )
		{
			SI16 Minutes = pDailyQuestInfo->siLeftTime / 60;
			SI16 Seconds = pDailyQuestInfo->siLeftTime % 60;
			
			if( Seconds == 0 )
			{
				char* pText = GetTxtFromMgr(1252);
				sprintf( Time, pText, Minutes );

				pText = GetTxtFromMgr(1876);
				
				char num[128]= " " ;
				char n1[128]= " " ;
				char n2[128]= " " ;
				char itemnum[128]= " " ;
				char exp[128]= " " ;
				
				sprintf(num,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(n1,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(n2,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(itemnum,"%d",pDailyQuestInfo->siRewardItemNum);
				sprintf(exp,"%d",pDailyQuestInfo->siRewardExp );
				Nsprintf(Buffer,pText,"monster",  MonsterName  ,"num",  num  ,"n1", n1   ,"n2", n2   ,"time",  Time,"item",ItemName ,"itemnum",itemnum, "exp", exp ,NULL);

				
				//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum, Time, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );

			}
			else
			{
				char* pText = GetTxtFromMgr(1253);
				sprintf( Time, pText, Minutes, Seconds );
				pText = GetTxtFromMgr(1876);
					char num[128]= " " ;
				char n1[128]= " " ;
				char n2[128]= " " ;
				char itemnum[128]= " " ;
				char exp[128]= " " ;
				
				sprintf(num,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(n1,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(n2,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(itemnum,"%d",pDailyQuestInfo->siRewardItemNum);
				sprintf(exp,"%d",pDailyQuestInfo->siRewardExp );
				Nsprintf(Buffer,pText,"monster",  MonsterName  ,"num",  num  ,"n1", n1   ,"n2", n2   ,"time",  Time,"item",ItemName ,"itemnum",itemnum, "exp", exp ,NULL);

				
				//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum, Time, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );
			}
		}
		else
		{
			char* pText = GetTxtFromMgr(1876);
				char num[128]= " " ;
				char n1[128]= " " ;
				char n2[128]= " " ;
				char itemnum[128]= " " ;
				char exp[128]= " " ;
				char time[128]= " " ;
				
				sprintf(num,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(n1,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(n2,"%d",pDailyQuestInfo->siMonsterNum);
				sprintf(itemnum,"%d",pDailyQuestInfo->siRewardItemNum);
				sprintf(exp,"%d",pDailyQuestInfo->siRewardExp );
				sprintf(time,"%d",pDailyQuestInfo->siLeftTime);
				Nsprintf(Buffer,pText,"monster",  MonsterName  ,"num",  num  ,"n1", n1   ,"n2", n2   ,"time",  time ,"item",ItemName ,"itemnum",itemnum, "exp", exp ,NULL);

				
			//sprintf( Buffer, pText, MonsterName, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siKillMonsterNum, pDailyQuestInfo->siMonsterNum, pDailyQuestInfo->siLeftTime, ItemName, pDailyQuestInfo->siRewardItemNum, pDailyQuestInfo->siRewardExp );
		}
		
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), Buffer );
		
		EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTART ), FALSE );
		EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTOP ), TRUE );
	}
	else
	{
		EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTART ), TRUE );
		EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTOP ), FALSE );
		char* pText = GetTxtFromMgr(1877);
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), pText );
	}

	return;
}

void CDailyQuestSelectDlg::LoadDailyQuestListInfo( char *MonsterName, SI16 MonsterNum, SI16 ItemUnique, char *ItemName, SI16 ItemNum, SI32 RewardExp, SI32 LeftTime, SI32 Fee )
{
	SI16 RandIndex = rand() % 10;

	char Time[ 64 ] = "";

	if ( LeftTime > 60 )
	{
		SI16 Minutes = LeftTime / 60;
		SI16 Seconds = LeftTime % 60;
		
		if( Seconds == 0 )
		{
			char* pText = GetTxtFromMgr(1252);
			sprintf( Time, pText, Minutes );
		}
		else
		{
			char* pText = GetTxtFromMgr(1253);
			sprintf( Time, pText, Minutes, Seconds );
		}
	}
	else
	{
		char* pText = GetTxtFromMgr(1254);
		sprintf( Time, pText, LeftTime );
	}

	char Buffer[ 1024 ] = "";
	
	switch ( RandIndex )
	{
	case 0:
		{
		char* pText = GetTxtFromMgr(1878);
		
		char num[128]= " " ;		
		char itemnum[128]= " " ;
		char exp[128]= " " ;
		char fee[128]= " " ;
		
		sprintf(num,"%d",MonsterNum);	
		sprintf(itemnum,"%d",ItemNum);
		sprintf(exp,"%d",RewardExp);
		sprintf(fee,"%d",Fee);

		Nsprintf(Buffer,pText,"monster", MonsterName   ,"num",   num ,"item",ItemName,"itemnum",  itemnum  ,"exp",  exp  ,"fee",fee,NULL);

		//sprintf ( Buffer, pText, MonsterName, MonsterNum, ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 1:
		{
		char* pText = GetTxtFromMgr(1879);
		
		char num[128]= " " ;		
		char itemnum[128]= " " ;
		char exp[128]= " " ;
		char fee[128]= " " ;
		
		sprintf(num,"%d",MonsterNum);		
		sprintf(itemnum,"%d",ItemNum);
		sprintf(exp,"%d",RewardExp);
		sprintf(fee,"%d",Fee);
		Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);

		//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 2:
		{
			
			char num[128]= " " ;

			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
			char* pText = GetTxtFromMgr(1880);
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 3:
		{
			char* pText = GetTxtFromMgr(1881);
			char num[128]= " " ;

			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
			
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 4:
		{
			char* pText = GetTxtFromMgr(1882);
			char num[128]= " " ;

			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
		
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 5:
		{
			char* pText = GetTxtFromMgr(1883);
			char num[128]= " " ;

			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
			
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 6:
		{
			char* pText = GetTxtFromMgr(1884);
			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			char num[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
		
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 7:
		{
			char* pText = GetTxtFromMgr(1885);
			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			char num[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
		
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 8:
		{
			char* pText = GetTxtFromMgr(1886);
			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			char num[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
			
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	case 9:
		{
			char* pText = GetTxtFromMgr(1887);
			char itemnum[128]= " " ;
			char exp[128]= " " ;
			char fee[128]= " " ;
			char num[128]= " " ;
			sprintf(num,"%d",MonsterNum);		
			sprintf(itemnum,"%d",ItemNum);
			sprintf(exp,"%d",RewardExp);
			sprintf(fee,"%d",Fee);
		
            Nsprintf(Buffer,pText,"monster",MonsterName,"num", num   ,"item",  ItemName  ,"itemnum",  itemnum  ,"exp",exp,"fee",fee    ,NULL);
			//sprintf ( Buffer, pText, MonsterName, MonsterNum,  ItemName, ItemNum, RewardExp, Fee );
		}
		break;
	}

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), Buffer );

	DrawDailQuestInfo(Fee, MonsterName, MonsterNum, LeftTime, ItemUnique, ItemName, ItemNum, RewardExp);
			

	return;
}

void CDailyQuestSelectDlg::DrawDailQuestInfo(SI32 Fee , char *MonsterName, SI16 MonsterNum, SI32 LeftTime, SI16 ItemUnique, char *ItemName, SI16 ItemNum, SI32 RewardExp)
{	
	char Buffer[256 ] = "";
	HWND hwnd = NULL;

	char* pText = GetTxtFromMgr(1640);
	sprintf(Buffer, pText, Fee);
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_CONDITION_FEE );
	if(hwnd)	SetWindowText( hwnd , Buffer );
	
	sprintf(Buffer, "%s", MonsterName);
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_CONDITION_MONSTER );
	if(hwnd)	SetWindowText( hwnd , Buffer );

	pText = GetTxtFromMgr(3911);
	sprintf(Buffer, pText, MonsterNum);
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_CONDITION_MONSTER_NUM);
	if(hwnd)	SetWindowText( hwnd , Buffer );

	if ( LeftTime > 60 )
	{
		SI16 Minutes = LeftTime / 60;
		SI16 Seconds = LeftTime % 60;
		
		if( Seconds == 0 )	{	
			char* pText = GetTxtFromMgr(1252);
			sprintf( Buffer, pText, Minutes );
		}
		else{	
			char* pText = GetTxtFromMgr(1253);
			sprintf( Buffer, pText, Minutes, Seconds );		
		}
	}
	else
	{
		char* pText = GetTxtFromMgr(1254);
		sprintf( Buffer, pText, LeftTime );
	}
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_CONDITION_TIME );
	if(hwnd)	SetWindowText( hwnd , Buffer );

	sprintf(Buffer, "%s", ItemName);
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_REWARD_ITEM );
	if(hwnd)	SetWindowText( hwnd , Buffer );

	pText = GetTxtFromMgr(2209);
	sprintf(Buffer,pText, ItemNum);
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_REWARD_ITEM_NUM );
	if(hwnd)	SetWindowText( hwnd , Buffer );

	pText = GetTxtFromMgr(3910);
	sprintf(Buffer, pText, RewardExp);
	hwnd = GetDlgItem( m_hDlg, IDC_EDIT_REWARD_EXP );
	if(hwnd)	SetWindowText( hwnd , Buffer );

	cltItem clItem;
	clItem.siUnique = ItemUnique;
	clItem.siItemNum = 1;

	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_REWARD_ITEM, NULL, &clItem, 5, 5 );

}

void CDailyQuestSelectDlg::DailyQuestComplete( bool bComplete )
{
	m_bDailyQuestSuccessEnd = bComplete;

	char Buffer[ 512 ] = "";
	m_siDailyQuestSuccessViewCounter = 0;
	
	if ( m_bDailyQuestSuccessEnd )
	{
		SI16 RandIndex = rand() % 4;
		char* pText = GetTxtFromMgr(3912);
		
		switch ( RandIndex )
		{
		case 0:
			pText = GetTxtFromMgr(3912);
			sprintf( Buffer, pText );
			break;
		case 1:
			pText = GetTxtFromMgr(3913);
			sprintf( Buffer, pText );
			break;		
		case 2:
			pText = GetTxtFromMgr(3914);
			sprintf( Buffer, pText );
			break;
		case 3:
			pText = GetTxtFromMgr(3915);
			sprintf( Buffer, pText );
			break;
		}
		
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), Buffer );
	}
	else
	{	
		char* pText = GetTxtFromMgr(3916);
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_QUESTEXPLAIN ), pText );
	}
	
	EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTART ), TRUE );
	EnableWindow( GetDlgItem( m_hDlg, IDC_BUTTON_QUESTSTOP ), FALSE );

	return;
}