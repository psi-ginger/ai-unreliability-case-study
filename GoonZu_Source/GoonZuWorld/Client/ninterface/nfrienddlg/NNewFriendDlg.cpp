/* ==========================================================================
	클래스 :		CNNewFriendDlg

	작성일 :		07/02/21
	작성자 :		이강형

	목  적 :		새로운 FriendDlg

	용  법 :		

	변경사항 :

	차후작업 :

========================================================================*/
//#if (defined(__LEON_START_ALL) || defined(__LEON_NFRIENDDLG__)) && defined(_DEBUG)

#include "NNewFriendDlg.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../ImageInterface/FriendLoginDraw/FriendLoginDraw.h"

#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/Msg/MsgType-ItemMall.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "NFriendMemoDlg.h"
#include "NFriendChangeGroupNameDlg.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;
#define FRIENDDLG_TIMER_VALUE			10000

CNNewFriendDlg::CNNewFriendDlg()
{
	m_pFriendList = NULL;             
	m_pFriendNameStatic = NULL;       
	m_pFriendNameEdit = NULL;         
	m_pFriendDelButton = NULL;        
	m_pFriendWhisperChatButton = NULL;
	m_pFriendAddButton = NULL;        
	m_pFriendMinhomeButton = NULL;    
	m_pFriendWarpButton = NULL;       
	m_pFriendProfileImage = NULL;
	m_pFriendIntroStatic = NULL;
	m_pFriendMemoButton = NULL;

	m_pFriendGroupAllButton = NULL;
	m_pFriendGroup1Button = NULL;
	m_pFriendGroup2Button = NULL;
	m_pFriendGroup3Button = NULL;
	m_pFriendGroup4Button = NULL;

	m_pFriendMoveGroupButton = NULL;
	m_pFriendChangeGroupNameButton = NULL;

	m_bMovingFriendGroup = FALSE;

	m_bShow = false;

	m_pFriendInfo = new cltFriendInfo();
	m_pFriendInfo->Init();

	m_siCurrentGroupID = FRIEND_GROUP_UNKNOWN;

	Initialize();
}

CNNewFriendDlg::~CNNewFriendDlg()
{
	Destroy();
}

void CNNewFriendDlg::Initialize()
{
	for ( int i = 0 ; i < MAX_FRIEND_NUMBER ; i ++ )
		m_szAccountID[i][0] = NULL;
		//StringCchCopy(m_szAccountID[i], MAX_PLAYER_NAME, TEXT(""));
}

void CNNewFriendDlg::Destroy()
{
	if ( m_pFriendInfo )
	{
		delete m_pFriendInfo;
		m_pFriendInfo = NULL;
	}

	if(m_pFriendList)
	{
		delete m_pFriendList;             
		m_pFriendList = NULL;
	}
	if(m_pFriendNameStatic)
	{
		delete m_pFriendNameStatic;       
		m_pFriendNameStatic = NULL;
	}

	if(m_pFriendNameEdit)
	{
		delete m_pFriendNameEdit;         
		m_pFriendNameEdit = NULL;
	}
	if(m_pFriendDelButton)
	{
		delete m_pFriendDelButton;        
		m_pFriendDelButton = NULL;
	}
	if(m_pFriendWhisperChatButton)
	{
		delete m_pFriendWhisperChatButton;
		m_pFriendWhisperChatButton = NULL;
	}

	if(m_pFriendAddButton)
	{
		delete m_pFriendAddButton;        
		m_pFriendAddButton = NULL;
	}
	if(m_pFriendMinhomeButton)
	{
		delete m_pFriendMinhomeButton;    
		m_pFriendMinhomeButton = NULL;
	}
	if(m_pFriendWarpButton)
	{
		delete m_pFriendWarpButton;       
		m_pFriendWarpButton = NULL;
	}
	if(m_pFriendProfileImage)
	{
		delete m_pFriendProfileImage;
		m_pFriendProfileImage = NULL;
	}
	if(m_pFriendIntroStatic)
	{
		delete m_pFriendIntroStatic;
		m_pFriendIntroStatic = NULL;
	}

	SAFE_DELETE( m_pFriendMemoButton );

	SAFE_DELETE( m_pFriendGroupAllButton );
	SAFE_DELETE( m_pFriendGroup1Button );
	SAFE_DELETE( m_pFriendGroup2Button );
	SAFE_DELETE( m_pFriendGroup3Button );
	SAFE_DELETE( m_pFriendGroup4Button );

	SAFE_DELETE( m_pFriendMoveGroupButton );
	SAFE_DELETE( m_pFriendChangeGroupNameButton );

	SAFE_DELETE( m_pAutoAnswerButton );

	SetTimerOnOff( false );
}

void CNNewFriendDlg::Create()
{
cltClient *pclclient = (cltClient *)pclClient;

CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NNewFriend.ddf"));
	file.CreatePopupDialog( this, NNEWFRIEND_DLG, TEXT("dialog_NNewFriend"), StaticNNewFriendDlgProc);
	
	m_pFriendList = new CList( this );             
	m_pFriendNameStatic = new CStatic( this );       
	m_pFriendNameEdit = new CEdit( this );         
	m_pFriendDelButton = new CButton( this );        
	m_pFriendWhisperChatButton = new CButton( this );
	m_pFriendAddButton = new CButton( this );        
	m_pFriendMinhomeButton = new CButton( this );    
	m_pFriendWarpButton = new CButton( this );   

	m_pFriendProfileImage = new CImageStatic( this );
	m_pFriendIntroStatic = new CStatic( this );

	m_pFriendGroupAllButton = new CButton( this );    
	m_pFriendGroup1Button = new CButton( this );    
	m_pFriendGroup2Button = new CButton( this );    
	m_pFriendGroup3Button = new CButton( this );    
	m_pFriendGroup4Button = new CButton( this );    

	m_pFriendMoveGroupButton = new CButton( this );    
	m_pFriendChangeGroupNameButton = new CButton( this );    

	NEWCBUTTON( m_pFriendMemoButton );
	NEWCBUTTON( m_pAutoAnswerButton );

	file.CreateControl(m_pFriendList,             NFRIEND_DIALOG_FRIENDLIST,             TEXT("listbox_friend"));
	file.CreateControl(m_pFriendNameStatic,       NFRIEND_DIALOG_FRIENDNAMESTATIC,       TEXT("statictext_friend_name"));
	file.CreateControl(m_pFriendNameEdit,         NFRIEND_DIALOG_FRIENDNAMEEDIT,         TEXT("editbox_friend_name"));
	file.CreateControl(m_pFriendDelButton,        NFRIEND_DIALOG_FRIENDDELBUTTON,        TEXT("button_friend_del"));
	file.CreateControl(m_pFriendWhisperChatButton,NFRIEND_DIALOG_FRIENDWHISPERCHATBUTTON,TEXT("button_friend_whisper_chat"));
	file.CreateControl(m_pFriendAddButton,        NFRIEND_DIALOG_FRIENDADDBUTTON,        TEXT("button_friend_add"));
	file.CreateControl(m_pFriendMinhomeButton,    NFRIEND_DIALOG_FRIENDMINHOMEBUTTON,    TEXT("button_friend_minhome"));
	file.CreateControl(m_pFriendWarpButton,       NFRIEND_DIALOG_FRIENDWARPBUTTON,       TEXT("button_friend_warp"));
	file.CreateControl(m_pFriendMemoButton,		  NFRIEND_DIALOG_FRIENDMEMOBUTTON,		 TEXT("button_friend_memo"));

	file.CreateControl(m_pFriendGroupAllButton,	  NNEWFRIEND_DIALOG_FRIENDGROUPALLBUTTON,	 TEXT("button_friend_groupall"));
	file.CreateControl(m_pFriendGroup1Button,	  NNEWFRIEND_DIALOG_FRIENDGROUP1BUTTON,		 TEXT("button_friend_group1"));
	file.CreateControl(m_pFriendGroup2Button,	  NNEWFRIEND_DIALOG_FRIENDGROUP2BUTTON,		 TEXT("button_friend_group2"));
	file.CreateControl(m_pFriendGroup3Button,	  NNEWFRIEND_DIALOG_FRIENDGROUP3BUTTON,		 TEXT("button_friend_group3"));
	file.CreateControl(m_pFriendGroup4Button,	  NNEWFRIEND_DIALOG_FRIENDGROUP4BUTTON,		 TEXT("button_friend_group4"));

	file.CreateControl(m_pFriendMoveGroupButton,		NNEWFRIEND_DIALOG_FRIENDMOVEGROUPBUTTON,		TEXT("button_friend_movegroup"));
	file.CreateControl(m_pFriendChangeGroupNameButton,	NNEWFRIEND_DIALOG_FRIENDCHANGEGROUPNAMEBUTTON,	TEXT("button_friend_changegroupname"));
	file.CreateControl(m_pAutoAnswerButton,		  NFRIEND_DIALOG_AUTOANSWERBUTTON,		 TEXT("button_autoanswer"));


	//file.CreateControl(m_pFriendProfileImage,       NFRIEND_DIALOG_FRIENDPROFILE,       "imagestatic_friend_profile");
	//file.CreateControl(m_pFriendIntroStatic,       NFRIEND_DIALOG_FRIENDINTRO,       "statictext_friend_intro");

	// leon to - bmp 로드 차후 추가 필요!!
	//m_hLogInOutImg = ImageList_LoadBitmap( pclClient->GetInstance(), MAKEINTRESOURCE( IDB_BITMAP_FRIEND_LOGINOUT ), 16, 1, 0 );
	//SendMessage( m_hFriendListCtrl, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)m_hLogInOutImg );
	//m_hFriendWarpBtnBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), TEXT("Interface/FriendDlg/FriendWarpBtn.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	//=> m_pFriendProfileImage->SetFileName(TEXT("..."));

	TCHAR* pText = GetTxtFromMgr(1556);
	m_pFriendList->SetColumn( 0, 128, pText );
	pText = GetTxtFromMgr(1611);
	m_pFriendList->SetColumn( 1, 100, pText );
	pText = GetTxtFromMgr(1506);
	m_pFriendList->SetColumn( 2, 36, pText );
	pText = GetTxtFromMgr(3101);
	m_pFriendList->SetColumn( 3, 80, pText );
	pText = GetTxtFromMgr(6881);
	m_pFriendList->SetColumn( 4, 30, pText );
	pText = GetTxtFromMgr(4030);
	m_pFriendList->SetColumn( 5, 90, pText );

	m_pFriendList->SetBorder( true );
	m_pFriendList->SetBKColorUse( true );
	m_pFriendList->SetDrawLine( true );
	m_pFriendList->Refresh();

	// 기본 설정
	m_pFriendGroupAllButton->SetFontWeight(FW_BOLD);
	m_pFriendGroupAllButton->Refresh();

	////---------------------------------
	//// HTML 을 초기화한다.
	////---------------------------------
	////==================================			
	//HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
	//if ( hwnd )
	//{
	//	EmbedBrowserObject(hwnd);
	//}

	//MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 2, 213, 73, 73, TRUE );

	//hwnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
	//if ( hwnd )
	//{
	//	EmbedBrowserObject(hwnd);
	//}

	//SetTimerOnOff( true );

	SetActionTimer( 200 );
	
	//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
	if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
	{
		m_pFriendMinhomeButton->Show(false);
	}

	Hide();
	return;
}

void CALLBACK CNNewFriendDlg::StaticNNewFriendDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewFriendDlg *pThis = (CNNewFriendDlg*) pControllerMgr;
	pThis->NNewFriendDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNewFriendDlg::NNewFriendDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	BOOL bBeforeMovingFriendGroup = m_bMovingFriendGroup;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
	case EVENT_LISTBOX_SELECTION:
		{
			// 그룹이동을 위한 모드를 끈다.
			m_bMovingFriendGroup = FALSE;
		}
		break;
	}

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;

	case NFRIEND_DIALOG_FRIENDLIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if ( m_pFriendList )
					{
						SI16 siSelectedIndex = m_pFriendList->GetSelectedIndex();
						if ( siSelectedIndex >= 0 )
						{
							//leon todo - 아래 인자 0을 siSelectedIndex가 Row값이므로 Column값 얻어오는 함수로 바꿔야함
							TCHAR Buffer[256 ] = TEXT("");
							TCHAR *tmpText = NULL;
							tmpText = m_pFriendList->GetText(siSelectedIndex, 0);
							if(tmpText)
							{
	                            StringCchCopy( Buffer, 256, tmpText);
								m_pFriendNameEdit->SetText( Buffer );
							}

							//TCHAR buffer[1024] = TEXT("") ;

							//TCHAR ProfileURL[MAX_PATH];

							//pclClient->GetCZIntroUrl( ProfileURL, m_szAccountID[siSelectedIndex] );

							// leon tod - html 컨트롤 변환 처리 추가 요망
							//hWnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
							//if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);
						}
					}
				}
				break;
			}
		}
		break;
	case NFRIEND_DIALOG_FRIENDADDBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");

					TCHAR *tmpText = NULL;
					tmpText = m_pFriendNameEdit->GetText();
					if(tmpText)
					{
						if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
						StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
					}

					Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					if ( _tcslen( Buffer ) < 1 )
					{
						return ;
					}					
					
					if ( _tcscmp( Buffer, pclClient->pclCM->CR[1]->GetName() ) == 0 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2458);
						TCHAR* pText = GetTxtFromMgr(2459);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return ;
					}

					cltGameMsgRequest_FriendAdd clFriendAdd( m_siCurrentGroupID, Buffer );
					cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADD, sizeof(clFriendAdd), (BYTE*)&clFriendAdd );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NFRIEND_DIALOG_FRIENDDELBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
					TCHAR *tmpText = NULL;
					tmpText = m_pFriendNameEdit->GetText();
					if(tmpText)
					{
						if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
						StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
					}

					Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

					if ( _tcslen( Buffer ) < 1 )
					{
						return ;
					}
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), Buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					if ( PersonID > 0 )
					{
						cltGameMsgRequest_FriendDel clFriendDel( PersonID );
						cltMsg clMsg( GAMEMSG_REQUEST_FRIENDDEL, sizeof(clFriendDel), (BYTE*)&clFriendDel );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			}
		}
		break;

	case NFRIEND_DIALOG_FRIENDWHISPERCHATBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
					TCHAR *tmpText = NULL;
					tmpText = m_pFriendNameEdit->GetText();
					if(tmpText)
					{
						if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
						StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
					}

					Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), Buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(PersonID);
				}
				break;
			}
		}
		break;
	case NFRIEND_DIALOG_FRIENDMINHOMEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] =TEXT("");
					TCHAR *tmpText = NULL;
					tmpText = m_pFriendNameEdit->GetText();
					if(tmpText)
					{
						if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
						StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
					}
					Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), Buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					if ( PersonID > 0 )
					{
						cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( Buffer );
						cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
						pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
					}
				}
				break;
			}
		}
		break;
	case NFRIEND_DIALOG_FRIENDWARPBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
					TCHAR *tmpText = NULL;
					tmpText = m_pFriendNameEdit->GetText();
					if(tmpText)
					{
						if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
						StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
					}
					Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';
					
					SI32 PersonID = 0;
					
					for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
					{
						cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[i];
						
						if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
						{
							if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), Buffer ) == 0 )
							{
								PersonID = pSimplePersonInfo->GetPersonID();
								break;
							}
						}
					}
					
					if ( PersonID > 0 )
					{
						cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

						TCHAR TEMP[256];
						TCHAR* pTitle = NULL;
						TCHAR* pText = NULL;

						// 캐릭터가 워프를 할 수 있는가?
						if( pclchar->CanWarpFrom() == false )
							return;
						
                        if( pclchar->CanFreeWarp( 0 ) == false )
						{
							// 순간이동이용권을 보유하고 있는지 확인한다. 
							SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
							SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
							if(pos < 0)
							{
								pTitle = GetTxtFromMgr(1413);
								pText = GetTxtFromMgr(2460);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

								return ;
							}
							pTitle = GetTxtFromMgr(1418);
							TCHAR* pText = GetTxtFromMgr(2461);
							StringCchPrintf(TEMP, 256, pText, Buffer);
						}
						else
						{
							pTitle = GetTxtFromMgr(3189);
							pText = GetTxtFromMgr(3191);
							StringCchPrintf(TEMP, 256, pText, Buffer);
						}

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, TEMP, MSGBOX_TYPE_YESNO, 0 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( PersonID );
						memcpy( TempBuffer, &PersonID, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
					}
				}
				break;
			}
		}
		break;
	case NFRIEND_DIALOG_FRIENDMEMOBUTTON:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
                ShowFriendMemo();

				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_MEMOCLICK );
			}
		}
		break;

	case NNEWFRIEND_DIALOG_FRIENDGROUPALLBUTTON:
	case NNEWFRIEND_DIALOG_FRIENDGROUP1BUTTON:
	case NNEWFRIEND_DIALOG_FRIENDGROUP2BUTTON:
	case NNEWFRIEND_DIALOG_FRIENDGROUP3BUTTON:
	case NNEWFRIEND_DIALOG_FRIENDGROUP4BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 siGroupID = nControlID - NNEWFRIEND_DIALOG_FRIENDGROUPALLBUTTON;
					if(siGroupID < FRIEND_GROUP_UNKNOWN || siGroupID > FRIEND_GROUP_4)		return;
					
					if(bBeforeMovingFriendGroup)
					{
						TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");

						TCHAR *tmpText = NULL;
						tmpText = m_pFriendNameEdit->GetText();
						if(tmpText)
						{
							if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
							StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
						}

						Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

						if ( _tcslen( Buffer ) < 1 )
						{
							return ;
						}

						SI32 PersonID = 0;

						for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
						{
							cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[i];

							if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
							{
								if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), Buffer ) == 0 )
								{
									PersonID = pSimplePersonInfo->GetPersonID();
									break;
								}
							}
						}

						if ( PersonID > 0 )
						{
							cltGameMsgRequest_FriendChangeGroup clFriendChangeGroup( siGroupID, PersonID );
							cltMsg clMsg( GAMEMSG_REQUEST_FRIENDCHANGEGROUP, sizeof(cltGameMsgRequest_FriendChangeGroup), (BYTE*)&clFriendChangeGroup );
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}

					// 현재 보이는 그룹을 변경한다.
					SetGroupList(siGroupID);
					LoadFriendList();
	
				}
				break;
			}
		}
		break;

	case NNEWFRIEND_DIALOG_FRIENDMOVEGROUPBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_MOVETOGROUPCLICK );

					if(bBeforeMovingFriendGroup)
					{
						// 그룹이동모드를 끈다.
						m_bMovingFriendGroup = FALSE;
						return;
					}
					TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");

					TCHAR *tmpText = NULL;
					tmpText = m_pFriendNameEdit->GetText();
					if(tmpText)
					{
						if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
						StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
					}

					Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

					if ( _tcslen( Buffer ) < 1 )
					{
						return ;
					}

					m_bMovingFriendGroup = TRUE;
				}
				break;
			}

		}
		break;
	case NNEWFRIEND_DIALOG_FRIENDCHANGEGROUPNAMEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowChangeGroupName();
				}
				break;
			}
		}
		break;
	case NFRIEND_DIALOG_AUTOANSWERBUTTON:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->CreateInterface( NAUTOANSWER_DLG );
			}
		}
		break;
	default:
		break;
	}
}

bool CNNewFriendDlg::IsShow()
{
	return m_bShow;
}

void CNNewFriendDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	/*
	아래 코드 필요 없음. ( pclClient->친구 리스트 ) 거기꺼 가죠 오면 됨.
	cltGameMsgRequest_Friend_List clFriendList();
	cltMsg clMsg( GAMEMSG_REQUEST_FRIEND_LIST, sizeof(clFriendList), (BYTE*)&clFriendList );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	*/

	// 친구 정보는 PersonID 랑 캐릭터명 만 있으면 됨.

	LoadFriendList();

	m_bShow = true;

	SetTimerOnOff( true );

	CDialogController::Show( true );

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENFRIEND);

	return;
}

void CNNewFriendDlg::Hide()
{
	SetTimerOnOff( false );
	m_bShow = false;
	m_bMovingFriendGroup = FALSE;

	CDialogController::Show( false );

	return;
}

bool CNNewFriendDlg::IsChanged()
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return false;

	BYTE *src = (BYTE*)m_pFriendInfo;
	BYTE *dest = (BYTE*)pclClient->pclCM->CR[1]->pclFriendInfo;

	for ( SI32 i = 0; i < sizeof( cltFriendInfo ); ++i )
	{
		if(src[i] != dest[i])
		{
			return true;
		}
	}
	
	return false;
}

void CNNewFriendDlg::LoadFriendList()
{
	// cltFriendInfo 에는 현재 위치한 맵인덱스가 없다..!!
	// 서버에 새로 요청!

	cltGameMsgRequest_FriendList clinfo; 
	cltMsg clMsg(GAMEMSG_REQUEST_FRIENDLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	
	return;
}

// 친구가 추가 된 경우
void CNNewFriendDlg::FriendAddList( SI16 ListIndex, SI08 GroupID, SI32 PersonID, SI32 CharID, TCHAR *FriendName ,TCHAR *AccountID)
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	if ( PersonID < 1 || FriendName == NULL || _tcslen( FriendName ) < 1 )
	{
		return;
	}

	if ( CharID > 0 )
	{
		pclClient->pclCM->CR[1]->pclFriendInfo->Set( ListIndex, GroupID, PersonID, CharID, FriendName, true ,AccountID);
	}
	else
	{
		pclClient->pclCM->CR[1]->pclFriendInfo->Set( ListIndex, GroupID, PersonID, CharID, FriendName, false ,AccountID);
	}
	
	LoadFriendList();

	return;
}

void CNNewFriendDlg::FriendDelList( SI16 ListIndex, SI32 PersonID )
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	if ( pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[ListIndex].GetPersonID() == PersonID )
	{
		pclClient->pclCM->CR[1]->pclFriendInfo->Init( ListIndex );
		LoadFriendList();
	}
	
	return;
}

void CNNewFriendDlg::FriendLogin( SI08 GroupID, SI32 PersonID, SI32 CharID ,TCHAR * AccountID)
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		if ( pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[ i ].GetPersonID() == PersonID )
		{
			pclClient->pclCM->CR[1]->pclFriendInfo->Set( i, GroupID, CharID, true ,AccountID);
			
			cltClient *pclclient = (cltClient*)pclClient;
			pclclient->m_pFriendLoginDraw->Set( (TCHAR*)pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[ i ].GetName() );
			
			LoadFriendList();
			break;
		}
	}

	return;
}

void CNNewFriendDlg::FriendLogOut( SI32 PersonID )
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		if ( pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[ i ].GetPersonID() == PersonID )
		{
			pclClient->pclCM->CR[1]->pclFriendInfo->bConnectStatus[i] = false;
			pclClient->pclCM->CR[1]->pclFriendInfo->siCharID[i] = 0;
			LoadFriendList();
			break;
		}
	}

	return;
}

// WM_TIMER 메세지 처리 루틴
void CNNewFriendDlg::Action()
{
	if ( IsChanged() )
	{
		LoadFriendList();
	}

	if(m_bMovingFriendGroup)
	{
		SI32 siFontIndex = m_pFriendGroupAllButton->GetFontIndex();
		{
			if(siFontIndex == 0)		siFontIndex = 12;
			else						siFontIndex = 0;

			m_pFriendGroupAllButton->SetFontIndex(siFontIndex);
			m_pFriendGroup1Button->SetFontIndex(siFontIndex);
			m_pFriendGroup2Button->SetFontIndex(siFontIndex);
			m_pFriendGroup3Button->SetFontIndex(siFontIndex);
			m_pFriendGroup4Button->SetFontIndex(siFontIndex);
		}
	}
	else
	{
		m_pFriendGroupAllButton->SetFontIndex(0);
		m_pFriendGroup1Button->SetFontIndex(0);
		m_pFriendGroup2Button->SetFontIndex(0);
		m_pFriendGroup3Button->SetFontIndex(0);
		m_pFriendGroup4Button->SetFontIndex(0);
	}
}

void CNNewFriendDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 친구로 이동
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 PersonID = *((SI32*)pData);
				cltGameMsgRequest_FriendWarp clFriendWarp( PersonID, FRIENDWARP_MODE_FRIEND );
				cltMsg clMsg( GAMEMSG_REQUEST_FRIENDWARP, sizeof( clFriendWarp ), (BYTE*)&clFriendWarp );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}
void CNNewFriendDlg::ShowFriendMemo()
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	TCHAR Buffer[ MAX_PLAYER_NAME ] = TEXT("");
	TCHAR *tmpText = NULL;
	tmpText = m_pFriendNameEdit->GetText();
	if(tmpText)
	{
		if(_tcslen(tmpText) >= MAX_PLAYER_NAME ) return;
		StringCchCopy(Buffer, MAX_PLAYER_NAME, tmpText);
	}
	Buffer[ MAX_PLAYER_NAME - 1 ] = '\0';

	SI32 PersonID = 0;

	for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		cltSimplePerson *pSimplePersonInfo = &pclClient->pclCM->CR[1]->pclFriendInfo->clFriend[i];

		if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
		{
			if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), Buffer ) == 0 )
			{
				PersonID = pSimplePersonInfo->GetPersonID();
				break;
			}
		}
	}
	if ( PersonID > 0 )
	{
		cltClient* pclclient = (cltClient*)pclClient;

		pclclient->CreateInterface( NFRIENDMEMO_DLG );
		CNFriendMemoDlg* pDialog = (CNFriendMemoDlg*)pclclient->m_pDialog[NFRIENDMEMO_DLG];
		if ( pDialog )
		{
            pDialog->SetFriendPersonID(PersonID);
			pDialog->SetTitle(Buffer);

		}

		cltGameMsgRequest_FriendMemo clInfo( PersonID );
		cltMsg clMsg( GAMEMSG_REQUEST_FRIENDMEMO , sizeof( clInfo ), (BYTE*)&clInfo );
		((cltClient*)pclClient)->SendMsgToServer( (sPacketHeader*)&clMsg );
	}
}

void CNNewFriendDlg::SetGroupList(SI32 siGroupID)
{
	m_pFriendGroupAllButton->SetFontWeight(FW_NORMAL);
	m_pFriendGroup1Button->SetFontWeight(FW_NORMAL);
	m_pFriendGroup2Button->SetFontWeight(FW_NORMAL);
	m_pFriendGroup3Button->SetFontWeight(FW_NORMAL);
	m_pFriendGroup4Button->SetFontWeight(FW_NORMAL);

	m_siCurrentGroupID = siGroupID;

    switch(siGroupID)
	{
	case FRIEND_GROUP_UNKNOWN:
		{
			m_pFriendGroupAllButton->SetFontWeight(FW_BOLD);
		}
		break;
	case FRIEND_GROUP_1:
		{
			m_pFriendGroup1Button->SetFontWeight(FW_BOLD);
		}
		break;
	case FRIEND_GROUP_2:
		{
			m_pFriendGroup2Button->SetFontWeight(FW_BOLD);
		}
		break;
	case FRIEND_GROUP_3:
		{
			m_pFriendGroup3Button->SetFontWeight(FW_BOLD);
		}
		break;
	case FRIEND_GROUP_4:
		{
			m_pFriendGroup4Button->SetFontWeight(FW_BOLD);
		}
		break;
	}

	m_pFriendGroupAllButton->Refresh();
	m_pFriendGroup1Button->Refresh();
	m_pFriendGroup2Button->Refresh();
	m_pFriendGroup3Button->Refresh();
	m_pFriendGroup4Button->Refresh();
}

void CNNewFriendDlg::SetGroupName()
{
	if(pclClient->pclCM->CR[1]->pclFriendInfo == NULL)		return;

	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_1]) > 0)
	{
		m_pFriendGroup1Button->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_1]);
		m_pFriendGroup1Button->Refresh();
	}
	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_2]) > 0)
	{
		m_pFriendGroup2Button->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_2]);
		m_pFriendGroup2Button->Refresh();
	}
	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_3]) > 0)
	{
		m_pFriendGroup3Button->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_3]);
		m_pFriendGroup3Button->Refresh();
	}
	if(strlen(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_4]) > 0)
	{
		m_pFriendGroup4Button->SetText(pclClient->pclCM->CR[1]->pclFriendInfo->szGroupName[FRIEND_GROUP_4]);
		m_pFriendGroup4Button->Refresh();
	}
}

void CNNewFriendDlg::ShowChangeGroupName()
{
	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->CreateInterface( NFRIENDCHANGEGROUPNAME_DLG );
	CNFriendChangeGroupNameDlg* pDialog = (CNFriendChangeGroupNameDlg*)pclclient->m_pDialog[NFRIENDCHANGEGROUPNAME_DLG];
	if ( pDialog )
	{
		pDialog->SetGroupName();
		pDialog->Show( TRUE );
	}
}

//#endif
