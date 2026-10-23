#include "NOpenBoxDlg.h"
#include "NGachaDlgNew.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Msg/msgType-Gacha.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../Client/Music/music.h"

#include "../../lib/MD5/md5.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

CNOpenBoxDlg::CNOpenBoxDlg()
{
	m_siFontIndex = 0;
	m_siAniStep   = ANISTEP_FISH;
	m_bFishClick  = false;

	m_siSelectGachaProductIndex		= -1;
	m_siSelectGachaProductUnique	= 0;

	ZeroMemory( m_siFrameTime, sizeof(m_siFrameTime) );
}

CNOpenBoxDlg::~CNOpenBoxDlg()
{
	Destroy();
}

void CNOpenBoxDlg::Init()
{ 
	m_siFrameTime[0] = 400;		m_siFrameTime[5] = 100;
	m_siFrameTime[1] = 400;		m_siFrameTime[6] = 100;
	m_siFrameTime[2] = 400;		m_siFrameTime[7] = 100;
	m_siFrameTime[3] = 100;		m_siFrameTime[8] = 100;
	m_siFrameTime[4] = 100;		m_siFrameTime[9] = 100;
		
	m_siFrameTime[10] = 900;	m_siFrameTime[15] = 400;
	m_siFrameTime[11] = 400;	m_siFrameTime[16] = 1000;  
	m_siFrameTime[12] = 400;	m_siFrameTime[17] = 500; 
	m_siFrameTime[13] = 400;	m_siFrameTime[18] = 300;
	m_siFrameTime[14] = 400;	m_siFrameTime[19] = 300; 

	m_siFrameTime[20] = 400;
	m_siFrameTime[21] = 800; 
	m_siFrameTime[22] = 1500;  
	m_siFrameTime[23] = 1200;
						
	m_dwPrevTime = GetTickCount();
}

void CNOpenBoxDlg::Destroy()
{


}

void CNOpenBoxDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	SetTopMostWindow( true );

	CInterfaceFile file;
	
	file.LoadFile("Ninterface/Data/NGachaDlg/DLG_NOpenBoxDlg.ddf");
	file.CreatePopupDialog( this , NOPENBOX, "dialog_openbox" , StaticNOpenBoxDlgProc );

	if( g_bFullScreen)	MovePos( 80, 40 );
	else		 		MovePos( 45, 20 );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, OPENBOX_BUTTON_SKIP,		this), OPENBOX_BUTTON_SKIP,		"button_skip");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, OPENBOX_BUTTON_CLOSE,	this), OPENBOX_BUTTON_CLOSE,	"button_close");

	TCHAR szControlName[128] = "";
	for(SI32 Count = 0; Count < SELECT_FISH_NUM; ++Count)
	{
		StringCchPrintf(szControlName, sizeof(szControlName),  "button_fish%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, OPENBOX_BUTTON_SELECT_FISH0 + Count,	this), OPENBOX_BUTTON_SELECT_FISH0 + Count, szControlName);

		m_InterfaceFactory.GetButton( OPENBOX_BUTTON_SELECT_FISH0 + Count )->Show( false );
		m_InterfaceFactory.GetButton( OPENBOX_BUTTON_SELECT_FISH0 + Count )->SetImageArea( true );
	}

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, OPENBOX_IMAGESTATIC_FISHBOX, this), OPENBOX_IMAGESTATIC_FISHBOX, TEXT("imagestatic_fishbox"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC, OPENBOX_IMAGESTATIC_OPENBOX, this), OPENBOX_IMAGESTATIC_OPENBOX, TEXT("imagestatic_boxopen"));
 
	m_InterfaceFactory.GetImageStatic(OPENBOX_IMAGESTATIC_FISHBOX)->Show( true  );
	m_InterfaceFactory.GetImageStatic(OPENBOX_IMAGESTATIC_OPENBOX)->Show( false );
	m_InterfaceFactory.GetImageStatic(OPENBOX_BUTTON_SKIP)->Show( false );

	SetTimerOnOff( true );
	SetActionTimer( 1 ); 
	
	SetTopMostWindow( true );

	Init();
}

void CALLBACK CNOpenBoxDlg::StaticNOpenBoxDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNOpenBoxDlg *pThis = (CNOpenBoxDlg *)pControllerMgr;
	pThis->NOpenBoxDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNOpenBoxDlg::NOpenBoxDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case OPENBOX_BUTTON_SELECT_FISH0:
		case OPENBOX_BUTTON_SELECT_FISH1:
		case OPENBOX_BUTTON_SELECT_FISH2:
		case OPENBOX_BUTTON_SELECT_FISH3:
		case OPENBOX_BUTTON_SELECT_FISH4:
			{
				//사운드
				pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_ROLLOVER_BOX")), 0, 0);

				m_bFishClick = true; 
			}
			break;
		case OPENBOX_IMAGESTATIC_FISHBOX:			
			{	
				switch(nEvent)
				{
					case EVENT_IMAGESTATIC_LBUTTONUP:
						{
							//  스킵
							if( 11 < m_siFontIndex && 22 > m_siFontIndex )  
							{
								if( false == m_InterfaceFactory.GetImageStatic(OPENBOX_BUTTON_SKIP)->IsShow() )
									return; 
								
								m_InterfaceFactory.GetImageStatic(OPENBOX_BUTTON_SKIP)->Show( false );

								m_siFontIndex = 22; 
								m_dwPrevTime  = GetTickCount();
								/*cltGameMsgRequest_GachaBuy pclinfo( m_siGachaUseKeyKind );
								cltMsg clMsg( GAMEMSG_REQUEST_GACHABUY, sizeof(pclinfo) , (BYTE*) &pclinfo );
								pclclient->SendMsgToServer((sPacketHeader*)&clMsg);*/
							}
						}
				}
			}
			break;
		case OPENBOX_BUTTON_SKIP:
			{
				switch(nEvent)
				{
					case EVENT_BUTTON_CLICKED:
						{
							if( 11 < m_siFontIndex && 22 > m_siFontIndex )  
							{
                                m_InterfaceFactory.GetImageStatic(OPENBOX_BUTTON_SKIP)->Show( false );

								m_siFontIndex = 22;  
								m_dwPrevTime  = GetTickCount();

								/*cltGameMsgRequest_GachaBuy pclinfo( m_siGachaUseKeyKind );
								cltMsg clMsg( GAMEMSG_REQUEST_GACHABUY, sizeof(pclinfo) , (BYTE*) &pclinfo );
								pclclient->SendMsgToServer((sPacketHeader*)&clMsg);*/
								//DeleteDialog();
							}
						}
				}
			}
			break;
		case OPENBOX_BUTTON_CLOSE:
			{
				if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
				{
					if ( pclclient->m_pDialog[NCARRYOVERGACHA_DLG] )
					{
						pclclient->m_pDialog[NCARRYOVERGACHA_DLG]->DeleteDialog();
					}
				}
				else
				{
					if( pclclient->m_pDialog[ NGACHANEW_DLG ] )
						((CNGachaDlgNew*)pclclient->m_pDialog[ NGACHANEW_DLG ])->DeleteDialog();
				}
			}	
			break;
		default:
			{
			}
			break;
	}
}

void CNOpenBoxDlg::Action()
{
	cltClient * pclclient = (cltClient*)pclClient;

	CImageStatic* pImageStatic  = NULL;
	SI32		  siAniImageNum = 0;
	if( ANISTEP_FISH == m_siAniStep )
	{
		pImageStatic  = m_InterfaceFactory.GetImageStatic(OPENBOX_IMAGESTATIC_FISHBOX);
		pImageStatic->Show( true );

		m_InterfaceFactory.GetImageStatic(OPENBOX_IMAGESTATIC_OPENBOX)->Show( false );

		siAniImageNum = ANIMATION_FISH_NUM; 
	}
	else
	{
		pImageStatic  = m_InterfaceFactory.GetImageStatic(OPENBOX_IMAGESTATIC_OPENBOX);
		pImageStatic->Show( true );  

		m_InterfaceFactory.GetImageStatic(OPENBOX_IMAGESTATIC_FISHBOX)->Show( false );

		siAniImageNum = ANIMATION_OPENBOX_NUM; 
	}

	pImageStatic->SetFontIndex( m_siFontIndex );

	if( ANIMATION_FISH_NUM == siAniImageNum)
	{
		// 11번 이미지에서 물고기 출현, 그리고 선택. 
		if( 11 == m_siFontIndex && false == m_bFishClick )	
		{
			for(SI32 Count = 0; Count < SELECT_FISH_NUM; ++Count)
				m_InterfaceFactory.GetButton( OPENBOX_BUTTON_SELECT_FISH0 + Count )->Show( true );

			return;
		}
		else 
		{
			for(SI32 Count = 0; Count < SELECT_FISH_NUM; ++Count)
				m_InterfaceFactory.GetButton( OPENBOX_BUTTON_SELECT_FISH0 + Count )->Show( false );
		} 

		// 물고기 선택 후에는 해당창에 있는 종료버튼이 사라진다.
		if(12 <= m_siFontIndex)
		{
			m_InterfaceFactory.GetButton(OPENBOX_BUTTON_CLOSE)->Enable( false );
		}

		//  스킵버튼 출현 시점.
		if( 11 < m_siFontIndex && 13 > m_siFontIndex )  
			m_InterfaceFactory.GetImageStatic(OPENBOX_BUTTON_SKIP)->Show( true );
		
		if( 22 < m_siFontIndex )
			m_InterfaceFactory.GetImageStatic(OPENBOX_BUTTON_SKIP)->Show( false );
		

		// 프레임별 시간확인 후 이미지 변경.
		if( siAniImageNum > m_siFontIndex && ((GetTickCount() - m_dwPrevTime) > m_siFrameTime[m_siFontIndex]) )
		{
			m_siFontIndex++; 
			m_dwPrevTime = GetTickCount();
			if( siAniImageNum <= m_siFontIndex )  
			{
				if( true == CheckHaveKey( m_siGachaUseKeyKind ) )
				{
					m_siFontIndex = 0; 
					m_siAniStep   = ANISTEP_OPENBOX;
					pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BGM_GAME_START")), 0, 0);
				}
				else
				{
					DeleteDialog();
				}
			} 
		} 
	}
	else if( ANISTEP_OPENBOX == m_siAniStep )
	{
		// 프레임별 시간확인 후 이미지 변경. 
		if( siAniImageNum > m_siFontIndex && ((GetTickCount() - m_dwPrevTime) > 150) )
		{ 
			m_siFontIndex++; 
			m_dwPrevTime = GetTickCount();
		}
		// 마지막 애니메이션이면. 
		else if( (GetTickCount() - m_dwPrevTime) > 500 ) 
		{
			if( true == CheckHaveKey( m_siGachaUseKeyKind ) )
			{
				m_siAniStep = NONE;

				if ( pclClient->IsCountrySwitch(Switch_CarryOverGacha) )
				{
					cltGameMsgRequest_CarryOver_GachaBuy pclinfo( m_siSelectGachaProductIndex, m_siSelectGachaProductUnique, m_siGachaUseKeyKind );
					cltMsg clMsg( GAMEMSG_REQUEST_CARRYOVER_GACHABUY, sizeof(pclinfo), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}
				else
				{
					cltGameMsgRequest_GachaBuy pclinfo( m_siGachaUseKeyKind );
					cltMsg clMsg( GAMEMSG_REQUEST_GACHABUY, sizeof(pclinfo) , (BYTE*) &pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				//DeleteDialog();
			}
			else
			{
				DeleteDialog();
			}
		}
	}
}

void CNOpenBoxDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
}


void CNOpenBoxDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;


}

void CNOpenBoxDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}

bool CNOpenBoxDlg::CheckHaveKey( SI32 GachaUseKeyKind )
{
	SI32 KeyItemUnique = 0;
	SI32 KeyNeedNum    = 1;

	if( GachaUseKeyKind == GACHA_USE_GOLDKEY )
	{
		KeyItemUnique = GACHA_GOLDKEY_ITEMUNIQUE;
	}
	else if( GachaUseKeyKind == GACHA_USE_SILVERKEY )
	{
		KeyItemUnique = GACHA_SILVERKEY_ITEMUNIQUE;
	}
	else if( GachaUseKeyKind == GACHA_USE_BRONZEKEY )
	{
		KeyItemUnique = GACHA_BRONZEKEY_ITEMUNIQUE;
	}
	else if( GachaUseKeyKind == GACHA_USE_KEYPIECE )
	{
		KeyItemUnique = GACHA_KEYPIECE_ITEMUNIQUE;
		KeyNeedNum = 10;
	}
	else if ( GachaUseKeyKind == CARRYOVER_USE_GOLDKEY )
	{
		KeyItemUnique = CARRYOVER_GOLDKEY_ITEMUNIQUE;
	}
	else if ( GachaUseKeyKind == CARRYOVER_USE_SILVERKEY )
	{
		KeyItemUnique = CARRYOVER_SILVERKEY_ITEMUNIQUE;
	}
	else if ( GachaUseKeyKind == CARRYOVER_USE_BRONZEKEY )
	{
		KeyItemUnique = CARRYOVER_BRONZEKEY_ITEMUNIQUE;
	}
	else 
	{
		return false;
	}

	SI32 keyItemNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( KeyItemUnique );

	if( GachaUseKeyKind == GACHA_USE_BRONZEKEY && keyItemNum < KeyNeedNum )
	{
		// 이벤트 키 개수를 체크한다.
		KeyItemUnique = GACHA_BRONZEKEY_EVENT_ITEMUNIQUE;
		keyItemNum	  =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( KeyItemUnique );
	}

	// 열쇠가 모자르다면..
	if( keyItemNum < KeyNeedNum )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CNOpenBoxDlg::SetGachaUseKeyKind( SI32 _siGachaUseKeyKind )
{
	m_siGachaUseKeyKind = _siGachaUseKeyKind;	
}

void CNOpenBoxDlg::SetCarryOverInfo( SI32 siGachaUseKeyKind, SI32 siSelectGachaProductIndex, SI32 siSelectGachaProductUnique )
{
	m_siGachaUseKeyKind				= siGachaUseKeyKind;

	m_siSelectGachaProductIndex		= siSelectGachaProductIndex;
	m_siSelectGachaProductUnique	= siSelectGachaProductUnique;
}
