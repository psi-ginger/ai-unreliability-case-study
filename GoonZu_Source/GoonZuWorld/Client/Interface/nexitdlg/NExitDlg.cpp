#include "../Client/Interface/NExitDlg/NExitDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../Client/QuitConfirm/QuitConfirm.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../Client/NInterface/NNewQuestDlg/NNewQuestDlg.h"
#include "../Client/NInterface/NQuestDlg/NQuestDlg.h"
#include "../Client/NInterface\NCommonMsgDlg\NCommonMsgDlg.h"

#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-ItemMall.h"
#include "Msg/MsgType-Web.h"

#include "../lib/MD5/md5.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;
extern SI16 g_siChannelingCode;

CNExitDlg::CNExitDlg() : CDialogController()
{
	m_siFontIndex = 0;
	m_pStatic = NULL;
	for(int i=0; i < 6; ++i)
		m_pBtn[i] = NULL;
}

CNExitDlg::~CNExitDlg()
{
	SAFE_DELETE(m_pStatic);
	for(int i=0; i< 6; ++i)
	{
		if(m_pBtn[i])
		{
			delete m_pBtn[i];
			m_pBtn[i] = NULL;
		}
	}
}

void CNExitDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NExitDlg/DLG_Exit.ddf"));
		file.CreatePopupDialog( this, NEXIT_DLG, TEXT("dialog_Exit"), StaticCallBackDialogNExitDlg );

		SI32 x = 0,y = 0;

		if(gsiResolution == 1) // 800 X 600
		{
			x = 260;
			y = 180;

			MovePos(x, y);
		}
		else if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			x = 340;
			y = 230;

			MovePos(x, y);
		}

		NEWCIMAGESTATIC(m_pStatic);

		NEWCBUTTON(m_pBtn[0]);
		NEWCBUTTON(m_pBtn[1]);
		NEWCBUTTON(m_pBtn[2]);
		NEWCBUTTON(m_pBtn[3]);
		NEWCBUTTON(m_pBtn[4]);
		NEWCBUTTON(m_pBtn[5]);

		file.CreateControl( m_pStatic, NEXIT_DIALOG_EXIT_STATIC, TEXT("statictext_NONAME1")); 
		//m_pStatic->SetFontIndex(m_siFontIndex);

		file.CreateControl( m_pBtn[0], NEXIT_DIALOG_ITEMMALL_GO_BUTTON, TEXT("button_NONAME1")); 
		file.CreateControl( m_pBtn[1], NEXIT_DIALOG_SEVER_SELECT_BUTTON, TEXT("button_NONAME2")); 
		file.CreateControl( m_pBtn[2], NEXIT_DIALOG_CHARACTER_SELECT_BUTTON, TEXT("button_NONAME3")); 
		file.CreateControl( m_pBtn[3], NEXIT_DIALOG_RETURN_GAME_BUTTON, TEXT("button_NONAME4")); 
		file.CreateControl( m_pBtn[4], NEXIT_DIALOG_EXIT_BUTTON, TEXT("button_NONAME5")); 
		file.CreateControl( m_pBtn[5], NEXIT_DIALOG_EVENT, TEXT("button_NONAME6")); 

		m_siFontIndex = 0;

		m_pStatic->SetFileName(TEXT("NInterface/Image/Image_300x300_00_000.SPR"));
		m_pStatic->SetFontIndex(m_siFontIndex);
		
		
		m_pBtn[5]->Show(false);


		//m_pBtn[1]->Enable(FALSE);
		
		SetTimerOnOff(TRUE);

		//cyj �����ӿ� ���������� ��������â�� �ߵ��� ����
		cltClient* pclclient = (cltClient*)pclClient;
		if (pclclient->GetGameStatus() < GS_GAME)
		{
			pclclient->CreateInterface( NEXITCONFIRM_DLG );
			DeleteDialog();
		}

		if (pclclient->m_pDialog[NCOMMONMSG_DLG])
		{
			if (((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->GetMode() == COMMONDLG_EXITCOUNT)
			{
				DeleteDialog();
			}
		}
	//	SetActionTimer( 1000 );
	}
	else
	{
		DeleteDialog();
	}
}

void CNExitDlg::Action()
{
/*	m_siFontIndex++;
	if(m_siFontIndex > 2)
		m_siFontIndex = 0;
	m_pStatic->SetFontIndex(m_siFontIndex);*/
}


void CALLBACK CNExitDlg::StaticCallBackDialogNExitDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNExitDlg *pThis = (CNExitDlg *)pControllerMgr;
	pThis->CallBackDialogNExitDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNExitDlg::CallBackDialogNExitDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NEXIT_DIALOG_ITEMMALL_GO_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{	

					//////////////////////////////////////////////////////////////////////////
					//////////////		GM�� �����ִ�		//////////////////////////////
					//////////////////////////////////////////////////////////////////////////

					//if(pclclient->pclCM->CR[1]->GetGameMasterLevel() != 101 )
					//	break;

					if ( pclClient->siServiceArea == ConstServiceArea_China )
					{
						pclclient->ShowHelpWindow("NULL");
					}
					else
					{
						if( pclClient->bItemMallUseSwitch == FALSE)
						{	

							TCHAR* pText = GetTxtFromMgr(962);

							if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
							{
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
							}
						}
						else
						{
							if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
							{
								if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
								{
									cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

									UI08 md5buf[256] = "" ;
									char buffermd5[256] = "" ;

									TCHAR	szURL[1024];
									TCHAR	szBeforeMD5[256];

									sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");

									memset(md5buf, 0, sizeof(md5buf));
									// MD5
									SI32 md5size = _tcslen(szBeforeMD5);
									memcpy(md5buf, szBeforeMD5, md5size);
									md5_string(md5buf, buffermd5, md5size);

									sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
										100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);

									g_LoginNoticeDlg.Set(szURL, 970, 756);
								}
							}
							else
							{
								pclclient->CreateInterface(NNEWITEMMALL_DLG);	
							}
						}
					}

					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NEXIT_DIALOG_SEVER_SELECT_BUTTON:
		{
			switch( nEvent )
			{

			case EVENT_BUTTON_CLICKED:
				{
/*
					if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
						pclclient->m_pDialog[ NNEWQUEST_DLG ]->DeleteDialog();
					if(pclclient->m_pDialog[ NQUEST_DLG ]->IsShow() == TRUE)
						((cltNQuestDlg*) pclclient->m_pDialog[ NQUEST_DLG ])->Hide();


					pclclient->m_pInterfaceMgr->CloseAllWindow();*/
					cltMsg clMsg(GAMEMSG_REQUEST_HOPETOFRONT, 0, NULL);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					// DEBUG �������� �ڵ��α����� �������� �ʴ´�.
#if defined(_DEBUG)
					StringCchCopy(pclclient->szID,			sizeof(pclclient->szID),		"");
					StringCchCopy(pclclient->szPassword,	sizeof(pclclient->szPassword),	"");
#endif

					// �Ϻ� NDE������ �ڵ��α����� �������� �ʴ´�. - �Ϻ����� ��û�� ���� ����
					if( GetChannelingUser(pclclient->pclCM->CR[1]->pclCI->clBI.szAccountID) == CHANNELING_USER_JAPAN_NDE )
					{
						StringCchCopy(pclclient->szID,			sizeof(pclclient->szID),		"");
						StringCchCopy(pclclient->szPassword,	sizeof(pclclient->szPassword),	"");
					}
				}
				break;
			}
		}
		break;

	case NEXIT_DIALOG_CHARACTER_SELECT_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient* pclclient = (cltClient*)pclClient;

					pclclient->bReceiveMyCharAllInfoSwitch	= false;
					pclclient->bBackUpSwitch				= false;

					pclClient->SetGameStatus(GS_INITCHARLIST);

					if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
						pclclient->m_pDialog[ NNEWQUEST_DLG ]->DeleteDialog();
					if(pclclient->m_pDialog[ NQUEST_DLG ]->IsShow() == TRUE)
						((cltNQuestDlg*) pclclient->m_pDialog[ NQUEST_DLG ])->Hide();
					
					//DeleteDialog();

					pclclient->m_pInterfaceMgr->CloseAllWindow();
				}
				break;
			}
		}
		break;

	case NEXIT_DIALOG_RETURN_GAME_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NEXIT_DIALOG_EXIT_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient* pclclient = (cltClient*)pclClient;

					pclclient->CreateInterface( NEXITCONFIRM_DLG );

					DeleteDialog();
					
				}
				break;
			}
		}
		break;

	case NEXIT_DIALOG_EVENT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR url[ MAX_PATH ];

					if ( pclclient->siServiceArea == ConstServiceArea_English )
					{
						// PCK : �۷ι��� ��� NHNȸ���� NDIȸ���� ���� �������� ��ũ�������� �ٸ���. (09.03.30)			
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nhn") >= 0 )		// NHN ȸ���̸�
						{
							StringCchCopy( url, MAX_PATH, TEXT("http://luminary.ijji.com/"));
						}
						else							// NDI ȸ���̸�
						{
							StringCchCopy( url, MAX_PATH, TEXT("http://luminary.ndoorsgames.com/center/default.asp"));
						}
					}
//KHY - 0609 - ����S Ȩ������ �߰�.
					else if ( pclclient->siServiceArea == ConstServiceArea_Japan )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://www.kunshu.jp/gameevent/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_Korea )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://goonzus.ndoors.com/"));
					}
////////////////////
					else if ( pclclient->siServiceArea == ConstServiceArea_Taiwan )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://gzs.hangame.com/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_USA )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://luminary.ijji.com/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_NHNChina )//NHN�߱� Ȩ������ ���� ����
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://www.newhljz.com/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_EUROPE )// Ȩ������ ���� ����
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://luminary.aeriagames.com"));
					}

					if ( pclclient->siServiceArea != ConstServiceArea_China )
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );
					}

					DeleteDialog();
				}
				break;
			}
		}
		break;
	case NEXIT_DIALOG_EXIT_STATIC:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONUP:
				{
					TCHAR url[ MAX_PATH ];

					if ( pclclient->siServiceArea == ConstServiceArea_English )
					{
						// PCK : �۷ι��� ��� NHNȸ���� NDIȸ���� ���� �������� ��ũ�������� �ٸ���. (09.03.30)			
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nh") >= 0 )		// NHN ȸ���̸�
						{
							StringCchCopy( url, MAX_PATH, TEXT("http://luminary.ijji.com/"));
						}
						else							// NDI ȸ���̸�
						{
							StringCchCopy( url, MAX_PATH, TEXT("http://luminary.ndoorsgames.com/center/default.asp"));
						}
					}
//KHY - 0612 - ����S Ȩ������ �߰�.
					else if ( pclclient->siServiceArea == ConstServiceArea_Japan )
					{
						break;

						//StringCchCopy( url, MAX_PATH, TEXT("http://www.kunshu.jp/gameevent/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_Korea )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://goonzus.ndoors.com/"));		// [��ȣ_NHN->NDOORS] �̰� ���� ��ũ������ ���� ���
					}
////////////////////
					else if ( pclclient->siServiceArea == ConstServiceArea_Taiwan )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://goonzu.omg.com.tw/index.aspx/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_USA )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://luminary.ijji.com/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_NHNChina )//NHN�߱� Ȩ������ ���� ����
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://www.newhljz.com/"));
					}
					else if ( pclclient->siServiceArea == ConstServiceArea_EUROPE )
					{
						StringCchCopy( url, MAX_PATH, TEXT("http://luminary.aeriagames.com/"));
					}

					if ( pclclient->siServiceArea != ConstServiceArea_China )
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );
					}


					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}
