#include "NRightMouseMenuDlg.h"

#include "../../CommonLogic/Msg/MsgType-PrivateTrade.h"
#include "../../CommonLogic/Msg/MsgType-Party.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Item.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../CommonLogic/Msg/MsgType-PersonalShop.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"
#include "../../CommonLogic/Msg/MsgType-Friend.h"

#include "../Client/NInterface/NPartyDlg/NPartyDlg.h"

#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../Common/Char/CharCommon/Char-Common.h"
#include "../../Common/PrivateTradeOrderMgr/PrivateTradeOrderBaseInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../../Client/client.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../client/Client.h"
#include "../client/PartyQuestMgr/PartyQuestMgr_Client.h"
#include "../client/ninterface/NSelectItemCountDlg/NSelectItemCountDlg.h"

extern cltCommonLogic* pclClient;
extern SI16 g_siChannelingCode;

CNRightMouseMenuDlg::CNRightMouseMenuDlg()
 : m_pkImageStatic_Name(NULL),
   m_pkImageStatic_Back(NULL),
   m_pkStatic_Name(NULL)
{
	ZeroMemory(m_pkButton_Command, sizeof(m_pkButton_Command));
}

CNRightMouseMenuDlg::~CNRightMouseMenuDlg()
{
	SAFE_DELETE(m_pkImageStatic_Name);
	SAFE_DELETE(m_pkImageStatic_Back);
	SAFE_DELETE(m_pkStatic_Name);
}

void CNRightMouseMenuDlg::Create()
{
	SI32	i					= 0;
	SI32	siButtonIndex		= 0;
	SI32	siButtonControlID	= 0;
	SI32	siYPos				= 0;

	if ( IsCreate() )
	{
		DeleteDialog();
	}
	else
	{
		CDialogController::Create( NRIGHTMOUSEMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNRightMouseMenu, NULL, NULL, 0, false, 0, 0, 0, 72, 24 + 16, false, false, false );
		
		m_pkStatic_Name = new CStatic( this );
		m_pkStatic_Name->Create(NRIGHTMOUSEMENU_DIALOG_STATIC_CHARNAME, "", NULL, 0, false, 0, 0, 0, RIGHTMOUSEMENU_BASE_WIDTH_SIZE, 16, RIGHTMOUSEMENU_BASE_WIDTH_SIZE, 16);
		m_pkStatic_Name->SetTextPrintOption(DT_TOP | DT_CENTER);

		m_pkImageStatic_Name = new CImageStatic( this );
		m_pkImageStatic_Name->Create(NRIGHTMOUSEMENU_DIALOG_IMAGESTATIC_NAME, "NINTERFACE/IMAGE/IMAGE_8X16_00_000.SPR", 0, true, 3, 0, 0, RIGHTMOUSEMENU_BASE_WIDTH_SIZE, 16, RIGHTMOUSEMENU_BASE_WIDTH_SIZE, 16 );

		siYPos				= 19;
		siButtonControlID	= NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND0;

		TCHAR* pText = GetTxtFromMgr(8710);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(8713);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(8717);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(8714);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(8715);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(8706);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(8716);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

//		if ( pclClient->IsCountrySwitch(Switch_IKillU) )
//		{
			// NEW PVP - 손성웅 2008.10.08
			pText = GetTxtFromMgr(9569);		
			m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
			m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
			siButtonIndex++;	siButtonControlID++;	siYPos += 16;
//		}

		pText = GetTxtFromMgr(40126);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(6071);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		pText = GetTxtFromMgr(4513);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command = new CButton(this);
		m_pkButton_Command[siButtonIndex].m_pkButton_Command->Create(siButtonControlID, pText, "NINTERFACE/BUTTON/BUTTON_62X16_00_000.SPR", 0, false, 0, 5, siYPos, 62, 16, 62, 16);
		siButtonIndex++;	siButtonControlID++;	siYPos += 16;

		m_pkImageStatic_Back = new CImageStatic( this );
		m_pkImageStatic_Back->Create(NRIGHTMOUSEMENU_DIALOG_IMAGESTATIC_BACK, "NINTERFACE/IMAGE/IMAGE_8X8_00_000.SPR",  0, true, 9, 0, 15, 72, 24, 72, 24 );
	}

	return;
}

void CNRightMouseMenuDlg::Action()
{
	cltClient * pclclient = (cltClient*)pclClient;

	TCHAR * pTextCharName = (TCHAR*)m_pkStatic_Name->GetText();

	if( pTextCharName == NULL || pTextCharName[0] == '\0' )
	{
		// 상단에 캐릭터 이름이 나오도록
		TCHAR * pCharName =	(TCHAR*)pclclient->pclCM->CR[m_siUnderMouseID]->GetName();
		if( pCharName != NULL && pCharName[0] != '\0')
		{
			SIZE sizeString = {0, };
			
			m_pkStatic_Name->SetFontColor(RGB(255,255,255));
			m_pkStatic_Name->SetText( pCharName, DT_CENTER | DT_VCENTER | DT_SINGLELINE, &sizeString );
			
			CalcWidth( sizeString );
		}
	}
}

void CALLBACK CNRightMouseMenuDlg::StaticCallBackDialogNRightMouseMenu( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNRightMouseMenuDlg *pThis = (CNRightMouseMenuDlg*)pControllerMgr;
	pThis->CallBackDialogNRightMouseMenu( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNRightMouseMenuDlg::CallBackDialogNRightMouseMenu( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(nEvent)
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch(nControlID)
			{
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND0:		// 귓속말
				{
					if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
					{
						// 귓속말창 호출
						pclclient->RequestSecretChatByCharUnique(pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique());
					}

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND1:		// 개인거래
				{
					if ( ! pclClient->pclCM->IsValidID(m_siUnderMouseID) )
					{
						break;
					}
					else if ( m_siCharUnique < 1 )
					{
						break;
					}
					else if ( pclClient->pclCM->CR[m_siUnderMouseID]->pclCI->clIP.GetLevel() < 3 || pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 3 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2037);
						TCHAR* pText = GetTxtFromMgr(2361);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}
					// 캐릭터 배열의 위치와, 캐릭터 유니크 값이 일치할경우에만 그 유저가 선택되었다고 볼수 있음 ( 아닐 수도 있음. 메뉴를 억수로 오랜시간 띄워놨다가 실행하면, 우연히 같은 배열에 같은 unique 번호가 존재할수 있으므로... )
					else if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
					{
						// 거래창을 열기위해서 거래하기 원함을 서버로 요청을 함.
						//[영진]]
						SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();

						cltGameMsgRequest_PrivateTrade_Accept clPrivateTradeAccept( siCharUnique, PRIVATETRADE_TRADE_MODE);
						cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT,
							sizeof(clPrivateTradeAccept), (BYTE*)&clPrivateTradeAccept);

						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND2:		// 꼼지, 푸치코미, 홈페이지
				{
					// 미니 홈페이지 보기 요청
					if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
					{
						if(pclClient->siServiceArea == ConstServiceArea_Japan)
						{
							SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();

							cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( siCharUnique );
							cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
							pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
						}

						//-----------------------------------------------------------
						// 꼼지 가기 퀘스트가 진행중이었다면, 
						// 게임 서버로 퀘스트가 완료되었음을 알려준다.
						//-----------------------------------------------------------
						SI16 questtype, questunique, para1, para2;
						SI16 questclearcond =  QUEST_CLEAR_COND_OTHERMINIHOME;
						if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
						{
							cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
							cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND3:		// 친구추가
				{
					TCHAR * pCharName =	(TCHAR*)pclclient->pclCM->CR[m_siUnderMouseID]->GetName();

					if ( _tcscmp( pCharName, pclClient->pclCM->CR[1]->GetName() ) == 0 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2458);
						TCHAR* pText = GetTxtFromMgr(2459);
						((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return ;
					}

					cltGameMsgRequest_FriendAdd clFriendAdd( FRIEND_GROUP_UNKNOWN, pCharName );
					cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADD, sizeof(clFriendAdd), (BYTE*)&clFriendAdd );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND4:		// 파티초대
				{
					// 파티 초대 ( 파티중인지 그리고 파티장인지 체크 )
					if(g_SoundOnOffDlg.m_bFullScreenMode)
					{
						if( pclclient->m_pDialog[ NPARTY_DLG ] )
						{
							// 파티 초대 ( 파티중인지 그리고 파티장인지 체크 )
							if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyStatus() )
							{
								if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyLeader() )
								{
									if ( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
									{
										SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();

										if(pclClient->IsCountrySwitch(Switch_PartyQuest_PartyMatching) == true)
										{
											cltClient* pClient = (cltClient*)pclClient;
											
											// 파티퀘스트가 진행중이라면 퀘스트가 종료된다는 경고메세지를 우선 보여줍니다.
											if(pClient->m_pPartyQuestMgr->m_PartyQuestData.m_bGoingQuest == false)
											{
												cltGameMsgRequest_Party_Invite clPartyInvite( siCharUnique, ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->GetPartyIndex() );
												cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITE, sizeof( clPartyInvite ), (BYTE*)&clPartyInvite );
												pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
											}
											else
											{
												// 파티퀘스트가 종료되는데도 초대할건지 물어봅니다.

												((CNPartyDlg*)(pclclient->m_pDialog[ NPARTY_DLG ]))->SetInviteCharUnique(siCharUnique);
												((CNPartyDlg*)(pclclient->m_pDialog[ NPARTY_DLG ]))->StartPartyQuestMessage(PARTYQUEST_MESSAGE_ANSWER_FAILED);
											}
										}
										else
										{
											cltGameMsgRequest_Party_Invite clPartyInvite( siCharUnique, ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->GetPartyIndex() );
											cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITE, sizeof( clPartyInvite ), (BYTE*)&clPartyInvite );
											pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
										}
									}
								}
								else
								{
									TCHAR* pTitle = GetTxtFromMgr(316);
									TCHAR* pText = GetTxtFromMgr(5887);
									if ( !pTitle || !pText ) break ;

									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

								}
							}
							else		// 파티 중이 아니라면 파티 생성과 초대를 동시에 해야 됨.
							{
								if ( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
								{
									SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
									cltGameMsgRequest_Party_CreateInvite clPartyCreateInvite( siCharUnique );
									cltMsg clMsg( GAMEMSG_REQUEST_PARTY_CREATEINVITE, sizeof( clPartyCreateInvite ), (BYTE*)&clPartyCreateInvite );
									pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
								}
							}
						}
					}

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND5:		// 개인정보
				{
					SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
					cltGameMsgRequest_PersonPrivateInfo clinfo(siCharUnique);
					cltMsg clMsg(GAMEMSG_REQUEST_PERSONPRIVATEINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND6:		// 개인상점
				{
					SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
					cltGameMsgRequest_PersonalShopData	clinfo(siCharUnique,FALSE,FALSE);	// 만드는거 아니면 무조건 FALSE 
					cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND7:		// NEW PVP - 손성웅 2008.10.08
				{
					// 먼저 상대방의 아이디가 정상적인지 체크
					if ( pclClient->pclCM->IsValidID( m_siUnderMouseID ) == false )			return	;	// 정상ID 검사
					SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();	// pvp 대상이 되는 사람 케릭터 유니크

					if( pclClient->pclCM->CR[m_siUnderMouseID]->GetMapIndex() == MAPINDEX_PALACE )
					{
						TCHAR* pTitle	=	GetTxtFromMgr(428)	;
						TCHAR* pText	=	GetTxtFromMgr(9574)	;
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText )	;
						return	;
					}
					// pk 맵이면 신청 못하도록 한다,
					if ( pclClient->pclMapManager->IsPkMap( pclClient->pclCM->CR[m_siUnderMouseID]->GetMapIndex() ) == true) 
					{
						TCHAR* pTitle	=	GetTxtFromMgr(428)	;
						TCHAR* pText	=	GetTxtFromMgr(9578)	;
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText )	;
						return	;

					}
					cltGameMsgRequest_PVP_Request clinfo ( REQUEST_PVP ,siCharUnique);
					cltMsg clMsg(GAMEMSG_REQUEST_PVP_FILED, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					DeleteDialog();
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND8:		// 수리 의뢰
				{
					if ( pclClient->pclCM->IsValidID( m_siUnderMouseID ) == false )			return	;	// 정상ID 검사
					SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();	// pvp 대상이 되는 사람 케릭터 유니크

					cltGameMsgRequest_NewItemRepairRequestInfo clinfo(siCharUnique, NEWITEMREPAIR_INFO_ITEM);
					cltMsg clMsg(GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND9:		// 선물하기
				{
					if ( ((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG]) == NULL )
					{
						pclclient->CreateInterface( NSELECTITEMCOUNT_DLG );
					}
					if( NULL != pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])
						((CNSelectItemCountDlg*)pclclient->m_pDialog[NSELECTITEMCOUNT_DLG])->SetCountMode( COUNT_MODE_PRESENTITEM );
				}
				break;
			case NRIGHTMOUSEMENU_DIALOG_BUTTON_COMMAND10:		//  제조의뢰 
				{
					SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
					cltGameMsgRequest_PersonalShopData	clinfo(siCharUnique,FALSE,FALSE);	// 만드는거 아니면 무조건 FALSE 
					cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
				}break	;
			}
		}
		break;
	}
}

void CNRightMouseMenuDlg::Set(  SI32 UnderMouseID, SI32 CharUnique, SI32 MouseXPos, SI32 MouseYPos, SI16 Mode, cltCharClient* pclCharClient)
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32	i					= 0;
	SI32	siViewButtonCount	= 0;
	SI32	siWidth				= GetWidth();
	SI32	siHeight			= GetHeight();

	m_siUnderMouseID = UnderMouseID;
	m_siCharUnique = CharUnique;

	m_siMode = Mode;

	cltMapClient* pclmap = (cltMapClient*)pclClient->pclMapManager->pclCurrentMap;

	cltCharClient* pclChar = NULL;

	if( NULL == pclCharClient)
		pclChar = pclclient->pclCM->GetCharClient(m_siUnderMouseID);
	else
		pclChar = pclCharClient;

	if( NULL == pclChar)
		return;
	
	SHORT centerx, centery;
	pclmap->GetCenterXYByMapCenterXY(pclChar->GetX(), pclChar->GetY(), centerx, centery );

	// 상단에 캐릭터 이름이 나오도록
	TCHAR * pCharName =	(TCHAR*)pclChar->GetName();
	if( pCharName != NULL && pCharName[0] != '\0')
	{
		SIZE sizeString = {0, };

		m_pkStatic_Name->SetFontColor(RGB(255,255,255));
		m_pkStatic_Name->SetText(pCharName, DT_CENTER | DT_VCENTER | DT_SINGLELINE, &sizeString);

		CalcWidth( sizeString );
	}
	else
	{
		// 클라이언트가 캐릭터으 이름을 모르고 있따면 서버로 캐릭터의 이름을 요청
		cltGameMsgRequest_GetName clInfo( pclclient->pclCM->CR[UnderMouseID]->GetCharUnique() );
		cltMsg clMsg(GAMEMSG_REQUEST_RIGHTMOUSE_USERLIST_NAME, sizeof(clInfo), (BYTE*)&clInfo);

		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

	// 버튼 배열 설정
	m_pkButton_Command[0].SetUse(true);
	m_pkButton_Command[1].SetUse(true);
	m_pkButton_Command[2].SetUse(true);
	m_pkButton_Command[3].SetUse(true);
	m_pkButton_Command[4].SetUse(true);
	m_pkButton_Command[5].SetUse(true);
	m_pkButton_Command[6].SetUse(true);
	m_pkButton_Command[7].SetUse(true);
	m_pkButton_Command[9].SetUse(false);
	m_pkButton_Command[10].SetUse(false);	// 제조의뢰 

	NTCHARString64	kPuchikomi(GetTxtFromMgr(3304));
	NTCHARString64	kHomePage(GetTxtFromMgr(8717));
	
	// 기본적으로 "홈페이지" 로 표시된다.
	if(m_pkButton_Command[2].m_pkButton_Command)	m_pkButton_Command[2].m_pkButton_Command->SetText(kHomePage);

	//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
	if( pclClient->IsCountrySwitch(Switch_Puchikomi) )
	{
		// 게임팟 유저만 꼼지가기가 표시된다.
		if( g_siChannelingCode == CHANNELINGCODE_GAMEPOTJP )
		{
			if(m_pkButton_Command[2].m_pkButton_Command)	m_pkButton_Command[2].m_pkButton_Command->SetText(kPuchikomi);
			m_pkButton_Command[2].SetUse(true);
		}
		else
		{
			m_pkButton_Command[2].SetUse(false);
		}
	}
	else
	{
		m_pkButton_Command[2].SetUse(false);
	}

	

	if ( m_siMode == 0 )	// 일반
	{
		m_pkButton_Command[6].SetUse(false);
		m_pkButton_Command[8].SetUse(false);
	}
	else
	{
		// 일반 + 개인 상점 포함
		if ( m_siMode & RIGHTMOUSEMENU_ADD_PERSONSHOP )					
		{
			m_pkButton_Command[6].SetUse(true);
		}
		else
		{
			m_pkButton_Command[6].SetUse(false);
			
		}
		// 제조 의뢰 거나 제작 의뢰 거나
		if ( m_siMode & RIGHTMOUSEMENU_ONLY_MAKESHOP )					
		{
			m_pkButton_Command[10].SetUse(true);
		}
		else
		{
			m_pkButton_Command[10].SetUse(false);

		}
		

		// 일반 + 수리의뢰 포함
		if ( pclClient->IsCountrySwitch(Switch_NewDurability) && (m_siMode & RIGHTMOUSEMENU_ADD_REPAIRSHOP) )
		{
			m_pkButton_Command[8].SetUse(true);
		}
		else
		{
			m_pkButton_Command[8].SetUse(false);
		}

		// PVP메뉴 보이고 안보이고 설정
		if ( pclClient->IsCountrySwitch(Switch_IKillU) && (m_siMode & RIGHTMOUSEMENU_ADD_PVP))
		{
			m_pkButton_Command[7].SetUse(true);
		}
		else
		{
			m_pkButton_Command[7].SetUse(false);
		}

		if ( m_siMode & RIGHTMOUSEMENU_ONLY_GIVEPRESENT)
		{
			m_siMode = RIGHTMOUSEMENU_ONLY_GIVEPRESENT;

			m_pkButton_Command[0].SetUse(false);
			m_pkButton_Command[1].SetUse(false);
			m_pkButton_Command[2].SetUse(false);
			m_pkButton_Command[3].SetUse(false);
			m_pkButton_Command[4].SetUse(false);
			m_pkButton_Command[5].SetUse(false);
			m_pkButton_Command[6].SetUse(false);
			m_pkButton_Command[7].SetUse(false);
			m_pkButton_Command[8].SetUse(false);
			m_pkButton_Command[9].SetUse(true);
		}
		else
		{
			m_pkButton_Command[9].SetUse(false);
		}
	}

	

	SI32 siYPos				= 19;

	// 크기 설정 및 위치 재정리
	for(i=0; i<RIGHTMOUSEMENU_MAX_BUTTON_COUNT; i++)
	{
		CButton* pkButton = m_pkButton_Command[i].m_pkButton_Command;

		if(pkButton == NULL)						continue;
		if(m_pkButton_Command[i].m_bUse == false)	continue;
		
		siViewButtonCount++;
		pkButton->SetControllerPos(pkButton->GetX(), siYPos, pkButton->GetWidth(), pkButton->GetHeight(), pkButton->GetWidth(), pkButton->GetHeight());
		siYPos += 16;
	}

	siHeight = 16 + siViewButtonCount * 16;

	m_pkImageStatic_Back->SetControllerPos( m_pkImageStatic_Back->GetX(), m_pkImageStatic_Back->GetY(),
											m_pkImageStatic_Back->GetWidth(), siViewButtonCount * 16 + 8,
											m_pkImageStatic_Back->GetWidth(), siViewButtonCount * 16 + 8 );

	SetDialogPos( centerx, centery - 150, siWidth, siHeight );

	SetTimerOnOff(TRUE);
	SetActionTimer(100);

	Show(true);
}

void CNRightMouseMenuDlg::ClearFocus()
{
	CDialogController::ClearFocus();
	DeleteDialog();

	return;
}

void CNRightMouseMenuDlg::SetFocus( bool Focus )
{
	CDialogController::SetFocus( Focus );

	if ( ! Focus )
	{
		cltClient * pclclient = (cltClient*) pclClient;

		if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG] &&  pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow())
		{
			pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->Show(false);

		}

		DeleteDialog();
	}

	return;
}

void CNRightMouseMenuDlg::CalcWidth( SIZE sizeString )
{
	// <<4 >>4연산은 8의 배수를 만들기 위해, 8은 보정값, 16은 양쪽 백그라운드 8픽셀씩 여유값
	SI16 siCalcWidth = ((sizeString.cx >> 3) << 3) +8 +16;
	// 기본 크기보다 작다면 무조건 기본 크기로 변경
	if (RIGHTMOUSEMENU_BASE_WIDTH_SIZE > siCalcWidth )
	{
		siCalcWidth = RIGHTMOUSEMENU_BASE_WIDTH_SIZE;
	}

	// 원래 설정한 크기와 계산된 크기가 같다면 리턴
	if ( m_pkStatic_Name->GetWidth() == siCalcWidth )
	{
		return;
	}

	m_pkStatic_Name->SetSurfaceWidth( siCalcWidth );
	m_pkStatic_Name->SetWidth( siCalcWidth );

	m_pkImageStatic_Name->SetSurfaceWidth( siCalcWidth );
	m_pkImageStatic_Name->SetWidth( siCalcWidth );

	// 위치 조정
	SI16 siWidth	= ((siCalcWidth - m_pkImageStatic_Back->GetWidth()) / 2);
	SI16 siX		= m_pkImageStatic_Back->GetX() - siWidth;

	m_pkStatic_Name->SetX( siX );
	m_pkImageStatic_Name->SetX( siX );
}

/*
void CALLBACK CNRightMouseMenuDlg::CallBackDialogNRightMouseMenu( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nEvent )
	{
	case EVENT_IMAGESTATIC_LBUTTONUP:
		{
			POINT pt;
			m_pImageStatic->GetImageStaticPos( pt );

			pt.y -= m_siStartYPos;
			SI16 SelectedIndex = pt.y / m_siLineHeight;

			if ( SelectedIndex < 0 )
			{
				DeleteDialog();
				return;
			}

			if ( m_siMode == 0 )	// 일반
			{
				//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
				if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
				{
					if ( SelectedIndex > 4 )
					{
						DeleteDialog();
					}
					else
					{
						ExecuteCommandNoFuchikomi( SelectedIndex );
					}
				}
				else
				{
					if ( SelectedIndex > 5 )
					{
						DeleteDialog();
					}
					else
					{
						ExecuteCommand( SelectedIndex );
					}
				}					
			}
			else					// 일반 + 개인 상점
			{
				//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기	
				if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
				{
					if ( SelectedIndex > 6 )
					{
						DeleteDialog();
					}
					else
					{
						ExecuteCommandNoFuchikomi( SelectedIndex );
					}
				}
				else
				{
					if ( SelectedIndex > 7 )
					{
						DeleteDialog();
					}
					else
					{
						ExecuteCommand( SelectedIndex );
					}
				}					
			}
		}
		break;
	case EVENT_IMAGESTATIC_LBUTTONDRAG:
	case EVENT_IMAGESTATIC_MOUSEMOVE:
		{
			POINT pt;
			m_pImageStatic->GetImageStaticPos( pt );

			pt.y -= m_siStartYPos;
			SI16 SelectedIndex = pt.y / m_siLineHeight;
			if ( SelectedIndex < 0 )
			{
				m_pImageStatic->SetFontIndex( 0 );
				return;
			}

			if ( m_siMode == 0 )	// 일반
			{
				if ( SelectedIndex > 5 )
				{
					m_pImageStatic->SetFontIndex( 0 );
				}
				else
				{
					m_pImageStatic->SetFontIndex( SelectedIndex + 1 );
				}
			}
			else					// 일반 + 개인 상점
			{
				if ( SelectedIndex > 6 )
				{
					m_pImageStatic->SetFontIndex( 0 );
				}
				else
				{
					m_pImageStatic->SetFontIndex( SelectedIndex + 1 );
				}
			}
		}
		break;
	}

	return;
}

void CNRightMouseMenuDlg::Set(  SI32 UnderMouseID, SI32 CharUnique, SI32 MouseXPos, SI32 MouseYPos, SI16 Mode )
{

	
 	m_siUnderMouseID = UnderMouseID;
	m_siCharUnique = CharUnique;

	m_siMode = Mode;

	// 다이얼로그 위치를 Action에서 설정
//	Action();

	//cyj 캐릭터 팝업메뉴 상대 캐릭터 따라다니도록 수정
	cltClient* pclclient = (cltClient*)pclClient;
	cltMapClient* pclmap = (cltMapClient*)pclClient->pclMapManager->pclCurrentMap;

	SHORT centerx, centery;
	pclmap->GetCenterXYByMapCenterXY(pclclient->pclCM->CR[m_siUnderMouseID]->GetX(), pclclient->pclCM->CR[m_siUnderMouseID]->GetY(), centerx, centery );

	// 상단에 캐릭터 이름이 나오도록
	TCHAR * pCharName =	(TCHAR*)pclclient->pclCM->CR[m_siUnderMouseID]->GetName();
	if( pCharName != NULL && pCharName[0] != '\0')
	{
		//m_pStaticCharName->SetTextColor()
		m_pStaticCharName->SetFontColor(RGB(255,255,255));
		m_pStaticCharName->SetText(pCharName,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		// 클라이언트가 캐릭터으 이름을 모르고 있따면 서버로 캐릭터의 이름을 요청
		cltGameMsgRequest_GetName clInfo( pclclient->pclCM->CR[UnderMouseID]->GetCharUnique() );
		cltMsg clMsg(GAMEMSG_REQUEST_RIGHTMOUSE_USERLIST_NAME, sizeof(clInfo), (BYTE*)&clInfo);

		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

	

	//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
	if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
	{
		if ( m_siMode == 0 )	// 일반
		{
			SetDialogPos( centerx, centery - 150, 76, 127 );
		}
		else					// 일반 + 개인 상점 포함
		{
			SetDialogPos( centerx, centery - 150, 76, 142 );
		}

		if ( m_siMode == 0 )	// 일반
		{
			m_pImageStatic->SetFileName( TEXT("NInterface/Image/Image_76X97_00_000_NoPuchikomi.Spr") );

			//SetDialogPos( MouseXPos, MouseYPos, 44, 71 );
			m_pImageStatic->SetControllerPos( 0, 0, 76, 127, 76, 127 );
		}
		else					// 일반 + 개인 상점 포함
		{
			m_pImageStatic->SetFileName(  TEXT("NInterface/Image/Image_76X112_00_000_NoPuchikomi.Spr") );

			//SetDialogPos( MouseXPos, MouseYPos, 44, 80 );
			m_pImageStatic->SetControllerPos( 0, 0, 76, 142, 76, 142 );
		}
	}
	else
	{
		if ( m_siMode == 0 )	// 일반
		{
			SetDialogPos( centerx, centery - 150, 76, 127 );
		}
		else					// 일반 + 개인 상점 포함
		{
			SetDialogPos( centerx, centery - 150, 76, 142 );
		}

		if ( m_siMode == 0 )	// 일반
		{
			m_pImageStatic->SetFileName(  TEXT("NInterface/Image/Image_76X112_00_000_InPuchikomi.Spr") );

			//SetDialogPos( MouseXPos, MouseYPos, 44, 71 );
			m_pImageStatic->SetControllerPos( 0, 0, 76, 127, 76, 127 );
		}
		else					// 일반 + 개인 상점 포함
		{
			m_pImageStatic->SetFileName(  TEXT("NInterface/Image/Image_76X127_00_000_InPuchikomi.Spr") );

			//SetDialogPos( MouseXPos, MouseYPos, 44, 80 );
			m_pImageStatic->SetControllerPos( 0, 0, 76, 142, 76, 142 );
		}
	}

		

	SetTimerOnOff(TRUE);
	SetActionTimer(100);

	return;
}

void CNRightMouseMenuDlg::ClearFocus()
{
	CDialogController::ClearFocus();
	DeleteDialog();

	return;
}

void CNRightMouseMenuDlg::SetFocus( bool Focus )
{
	CDialogController::SetFocus( Focus );

	if ( ! Focus )
	{
		cltClient * pclclient = (cltClient*) pclClient;

		if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG] &&  pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow())
		{
			pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->Show(false);

		}

		DeleteDialog();
	}

	return;
}

void CNRightMouseMenuDlg::ExecuteCommand( SI16 CommandIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG] &&  pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow())
	{
		pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->Show(false);
	}

	switch( CommandIndex )
	{
	case 0:
		{
			// 캐릭터 배열의 위치와, 캐릭터 유니크 값이 일치할경우에만 그 유저가 선택되었다고 볼수 있음 ( 아닐 수도 있음. 메뉴를 억수로 오랜시간 띄워놨다가 실행하면, 우연히 같은 배열에 같은 unique 번호가 존재할수 있으므로... )
			if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
			{
				// 귓속말창 호출
				//pclclient->pclPrivateChatDlgMgr->Push(pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique(), (TCHAR*)pclClient->pclCM->CR[m_siUnderMouseID]->GetName(), TEXT(""));
				pclclient->RequestSecretChatByCharUnique(pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique());
			}
		}
		break;
	case 1:
		{
			if ( ! pclClient->pclCM->IsValidID(m_siUnderMouseID) )
			{
				break;
			}
			else if ( m_siCharUnique < 1 )
			{
				break;
			}
			else if ( pclClient->pclCM->CR[m_siUnderMouseID]->pclCI->clIP.GetLevel() < 3 || pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 3 )
			{
				TCHAR* pTitle = GetTxtFromMgr(2037);
				TCHAR* pText = GetTxtFromMgr(2361);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				break;
			}
			// 캐릭터 배열의 위치와, 캐릭터 유니크 값이 일치할경우에만 그 유저가 선택되었다고 볼수 있음 ( 아닐 수도 있음. 메뉴를 억수로 오랜시간 띄워놨다가 실행하면, 우연히 같은 배열에 같은 unique 번호가 존재할수 있으므로... )
			else if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
			{
				// 거래창을 열기위해서 거래하기 원함을 서버로 요청을 함.
				//[영진]]
				SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
				//cltGameMsgRequest_PrivateTrade clPrivateTrade( siCharUnique, PRIVATETRADE_TRADE_MODE );
				

				//cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE,
				//	sizeof(clPrivateTrade), (TCHAR*)&clPrivateTrade);
				cltGameMsgRequest_PrivateTrade_Accept clPrivateTradeAccept( siCharUnique, PRIVATETRADE_TRADE_MODE);
				cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT,
					sizeof(clPrivateTradeAccept), (BYTE*)&clPrivateTradeAccept);

				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 2:

		{
			// 미니 홈페이지 보기 요청
			if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() && pclClient->siServiceArea != ConstServiceArea_Korea)
			{

				if(pclClient->siServiceArea == ConstServiceArea_Japan)
				{

					SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();

					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( siCharUnique );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

				}


				
				//-----------------------------------------------------------
				// 꼼지 가기 퀘스트가 진행중이었다면, 
				// 게임 서버로 퀘스트가 완료되었음을 알려준다.
				//-----------------------------------------------------------
				SI16 questtype, questunique, para1, para2;
				SI16 questclearcond =  QUEST_CLEAR_COND_OTHERMINIHOME;
				if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
				{
					cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
					cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}
		}
		break;

	case 3:
		{
			TCHAR * pCharName =	(TCHAR*)pclclient->pclCM->CR[m_siUnderMouseID]->GetName();

			if ( _tcscmp( pCharName, pclClient->pclCM->CR[1]->GetName() ) == 0 )
			{
				TCHAR* pTitle = GetTxtFromMgr(2458);
				TCHAR* pText = GetTxtFromMgr(2459);
				((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return ;
			}

			cltGameMsgRequest_FriendAdd clFriendAdd( FRIEND_GROUP_UNKNOWN, pCharName );
			cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADD, sizeof(clFriendAdd), (BYTE*)&clFriendAdd );
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case 4:
		{
			// 파티 초대 ( 파티중인지 그리고 파티장인지 체크 )
			if(g_SoundOnOffDlg.m_bFullScreenMode)
			{
				if( pclclient->m_pDialog[ NPARTY_DLG ] )
				{
					// 파티 초대 ( 파티중인지 그리고 파티장인지 체크 )
					if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyStatus() )
					{
						if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyLeader() )
						{
							if ( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
							{
								SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();

								cltGameMsgRequest_Party_Invite clPartyInvite( siCharUnique, ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->GetPartyIndex() );
								cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITE, sizeof( clPartyInvite ), (BYTE*)&clPartyInvite );
								pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
						}
						else
						{
							TCHAR* pTitle = GetTxtFromMgr(316);
							TCHAR* pText = GetTxtFromMgr(5887);
							if ( !pTitle || !pText ) break ;

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

						}

					}
					else		// 파티 중이 아니라면 파티 생성과 초대를 동시에 해야 됨.
					{
						if ( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
						{
							SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
							cltGameMsgRequest_Party_CreateInvite clPartyCreateInvite( siCharUnique );
							cltMsg clMsg( GAMEMSG_REQUEST_PARTY_CREATEINVITE, sizeof( clPartyCreateInvite ), (BYTE*)&clPartyCreateInvite );
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}
				}
			}
		}
		break;

	case 5:
		{
			SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
			cltGameMsgRequest_PersonPrivateInfo clinfo(siCharUnique);
			cltMsg clMsg(GAMEMSG_REQUEST_PERSONPRIVATEINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case 6:
		{
			SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
			cltGameMsgRequest_PersonalShopData	clinfo(siCharUnique,FALSE);
			cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;		
	}

	DeleteDialog();

	return;
}


void CNRightMouseMenuDlg::ExecuteCommandNoFuchikomi( SI16 CommandIndex )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG] &&  pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->IsShow())
	{
		pclclient->m_pDialog[NRIGHTMOUSEUSERLIST_DLG]->Show(false);
	}

	switch( CommandIndex )
	{
	case 0:
		{
			// 캐릭터 배열의 위치와, 캐릭터 유니크 값이 일치할경우에만 그 유저가 선택되었다고 볼수 있음 ( 아닐 수도 있음. 메뉴를 억수로 오랜시간 띄워놨다가 실행하면, 우연히 같은 배열에 같은 unique 번호가 존재할수 있으므로... )
			if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
			{
				// 귓속말창 호출
				//pclclient->pclPrivateChatDlgMgr->Push(pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique(), (TCHAR*)pclClient->pclCM->CR[m_siUnderMouseID]->GetName(), TEXT(""));
				pclclient->RequestSecretChatByCharUnique(pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique());
			}
		}
		break;
	case 1:
		{
			if ( ! pclClient->pclCM->IsValidID(m_siUnderMouseID) )
			{
				break;
			}
			else if ( m_siCharUnique < 1 )
			{
				break;
			}
			else if ( pclClient->pclCM->CR[m_siUnderMouseID]->pclCI->clIP.GetLevel() < 3 || pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 3 )
			{
				TCHAR* pTitle = GetTxtFromMgr(2037);
				TCHAR* pText = GetTxtFromMgr(2361);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				break;
			}
			// 캐릭터 배열의 위치와, 캐릭터 유니크 값이 일치할경우에만 그 유저가 선택되었다고 볼수 있음 ( 아닐 수도 있음. 메뉴를 억수로 오랜시간 띄워놨다가 실행하면, 우연히 같은 배열에 같은 unique 번호가 존재할수 있으므로... )
			else if( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
			{
				// 거래창을 열기위해서 거래하기 원함을 서버로 요청을 함.
				//[영진]]
				SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
				//cltGameMsgRequest_PrivateTrade clPrivateTrade( siCharUnique, PRIVATETRADE_TRADE_MODE );


				//cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE,
				//	sizeof(clPrivateTrade), (TCHAR*)&clPrivateTrade);
				cltGameMsgRequest_PrivateTrade_Accept clPrivateTradeAccept( siCharUnique, PRIVATETRADE_TRADE_MODE);
				cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_ACCEPT,
					sizeof(clPrivateTradeAccept), (BYTE*)&clPrivateTradeAccept);

				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	
	case 2:
		{
			TCHAR * pCharName =	(TCHAR*)pclclient->pclCM->CR[m_siUnderMouseID]->GetName();

			if ( _tcscmp( pCharName, pclClient->pclCM->CR[1]->GetName() ) == 0 )
			{
				TCHAR* pTitle = GetTxtFromMgr(2458);
				TCHAR* pText = GetTxtFromMgr(2459);
				((cltClient*)pclClient)->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
				return ;
			}

			cltGameMsgRequest_FriendAdd clFriendAdd( FRIEND_GROUP_UNKNOWN, pCharName );
			cltMsg clMsg( GAMEMSG_REQUEST_FRIENDADD, sizeof(clFriendAdd), (BYTE*)&clFriendAdd );
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case 3:
		{
			// 파티 초대 ( 파티중인지 그리고 파티장인지 체크 )
			if(g_SoundOnOffDlg.m_bFullScreenMode)
			{
				if( pclclient->m_pDialog[ NPARTY_DLG ] )
				{
					// 파티 초대 ( 파티중인지 그리고 파티장인지 체크 )
					if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyStatus() )
					{
						if ( ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyLeader() )
						{
							if ( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
							{
								SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();

								cltGameMsgRequest_Party_Invite clPartyInvite( siCharUnique, ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->GetPartyIndex() );
								cltMsg clMsg( GAMEMSG_REQUEST_PARTY_INVITE, sizeof( clPartyInvite ), (BYTE*)&clPartyInvite );
								pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
						}
						else
						{
							TCHAR* pTitle = GetTxtFromMgr(316);
							TCHAR* pText = GetTxtFromMgr(5887);
							if ( !pTitle || !pText ) break ;

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

						}

					}
					else		// 파티 중이 아니라면 파티 생성과 초대를 동시에 해야 됨.
					{
						if ( pclClient->pclCM->IsValidID(m_siUnderMouseID) && m_siCharUnique == pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique() )
						{
							SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
							cltGameMsgRequest_Party_CreateInvite clPartyCreateInvite( siCharUnique );
							cltMsg clMsg( GAMEMSG_REQUEST_PARTY_CREATEINVITE, sizeof( clPartyCreateInvite ), (BYTE*)&clPartyCreateInvite );
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}
				}
			}
		}
		break;		
	case 4:
		{
			SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
			cltGameMsgRequest_PersonPrivateInfo clinfo(siCharUnique);
			cltMsg clMsg(GAMEMSG_REQUEST_PERSONPRIVATEINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	case 5:
		{
			SI32 siCharUnique = pclClient->pclCM->CR[m_siUnderMouseID]->GetCharUnique();
			cltGameMsgRequest_PersonalShopData	clinfo(siCharUnique,FALSE);
			cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;		
	}

	DeleteDialog();

	return;

}

void CNRightMouseMenuDlg::Action()
{
	cltClient * pclclient = (cltClient*)pclClient;

	TCHAR * pTextCharName = (TCHAR*)m_pStaticCharName->GetText();

	if( pTextCharName == NULL || pTextCharName[0] == '\0' )
	{
		// 상단에 캐릭터 이름이 나오도록
		TCHAR * pCharName =	(TCHAR*)pclclient->pclCM->CR[m_siUnderMouseID]->GetName();
		if( pCharName != NULL && pCharName[0] != '\0')
		{
			//m_pStaticCharName->SetTextColor()
			m_pStaticCharName->SetFontColor(RGB(255,255,255));
			m_pStaticCharName->SetText(pCharName,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}	
	}
}
*/
