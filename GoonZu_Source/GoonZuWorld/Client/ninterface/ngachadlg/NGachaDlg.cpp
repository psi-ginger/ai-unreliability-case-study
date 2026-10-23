#include "NGachaDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Msg/msgType-Gacha.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../Client/Music/music.h"

#include "../../lib/MD5/md5.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

CNGachaDlg::CNGachaDlg()
{
	Initialize();
}

CNGachaDlg::~CNGachaDlg()
{
	Destroy();
}


void CNGachaDlg::Initialize()
{
	m_siCurrentStep = 0;
					
	for( SI16 i = 0; i < MAX_SHOW_GACHA_PRODUCT; i++ )
	{
		m_GachaProduct[ i ].Init();
		m_GachaProductPoint[ i ].x = 0;
		m_GachaProductPoint[ i ].y = 0;
	}


	m_RunRaccoonPoint.x = 0;
	m_RunRaccoonPoint.y = 0;
	
	m_bRaccoonDraw = false;

	m_GachaGetProduct.Init();

	m_FrameCnt = 0;

	m_myGachaGoldKeyNum = 0;
	m_myGachaSilverKeyNum = 0;
	m_myGachaBronzeKeyNum = 0;

	m_myGachaPieceNum = 0;

	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_22X27_00_000.SPR")   , &m_NumberImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_239X345_00_000.SPR")   , &m_RunRaccoonImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_204X144_00_000.SPR")   , &m_ClickImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_32X366_00_000.SPR")   , &m_GachaGageImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_44X378_00_000.SPR")   , &m_GachaGageOutLineImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_624X301_00_000.SPR")   , &m_GachaResultImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_652X535_00_000.SPR")   , &m_GachaFieldImg ); 

	m_myGachaUseKeyKind = 0;
}
void CNGachaDlg::Destroy()
{
	if( m_NumberImg.pImage) GP.FreeSpr( m_NumberImg );
	if( m_RunRaccoonImg.pImage) GP.FreeSpr( m_RunRaccoonImg );
	if( m_ClickImg.pImage) GP.FreeSpr( m_ClickImg );
	if( m_GachaGageImg.pImage) GP.FreeSpr( m_GachaGageImg );
	if( m_GachaGageOutLineImg.pImage) GP.FreeSpr( m_GachaGageOutLineImg );
	if( m_GachaResultImg.pImage) GP.FreeSpr( m_GachaResultImg );
	if( m_GachaFieldImg.pImage) GP.FreeSpr( m_GachaFieldImg );
	
	cltClient * pclclient = (cltClient*)pclClient;

	pclclient->PlayBackgroundMusic(pclclient->pclCM->CR[1]->GetMapIndex());
}

void CNGachaDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;
	
	file.LoadFile("Ninterface/Data/NGachaDlg/DLG_NGachaDlg.ddf");
	file.CreatePopupDialog( this , NGACHA_DLG , "dialog_Gacha" , StaticNGachaDlgProc );

	if( g_bFullScreen)
	{
		MovePos( 80, 40 );
	}
	else
	{
 		MovePos( 45, 20 );
	}
	
   	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_CLOSE , this)  , NGACHA_BUTTON_CLOSE , TEXT("button_close"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_GAMESTART , this)  , NGACHA_BUTTON_GAMESTART , TEXT("button_gamestart"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_ITEMMALL , this)  , NGACHA_BUTTON_ITEMMALL , TEXT("button_ItemMall"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_ONEMORE , this)  , NGACHA_BUTTON_ONEMORE , TEXT("button_Onemore"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_TOP , this)  , NGACHA_BUTTON_TOP , TEXT("button_top"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_BOX1 , this)  , NGACHA_BUTTON_BOX1 , TEXT("button_box1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_BOX2 , this)  , NGACHA_BUTTON_BOX2 , TEXT("button_box2"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_BOX3 , this)  , NGACHA_BUTTON_BOX3 , TEXT("button_box3"));
 	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_BOX4 , this)  , NGACHA_BUTTON_BOX4 , TEXT("button_box4"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_BOX5 , this)  , NGACHA_BUTTON_BOX5 , TEXT("button_box5"));
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_KEYPIECEUSE , this)  , NGACHA_BUTTON_KEYPIECEUSE , TEXT("button_keypieceuse"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGACHA_BUTTON_BUYKEY , this)  , NGACHA_BUTTON_BUYKEY , TEXT("button_buykey"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NGACHA_EDIT_EXPLAIN , this)  , NGACHA_EDIT_EXPLAIN , TEXT("editbox_Explain"));
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGACHA_BUTTON_CHOOSEKEYCLOSE , this) , NGACHA_BUTTON_CHOOSEKEYCLOSE , TEXT("button_choosekeyclose"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGACHA_BUTTON_CHOOSEGOLDKEY , this)  , NGACHA_BUTTON_CHOOSEGOLDKEY , TEXT("button_choosegoldkey"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGACHA_BUTTON_CHOOSESILVERKEY , this)  , NGACHA_BUTTON_CHOOSESILVERKEY , TEXT("button_choosesilverkey"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGACHA_BUTTON_CHOOSEKEYPIECE , this)  , NGACHA_BUTTON_CHOOSEKEYPIECE , TEXT("button_choosechoosekeypiece"));																																					  
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGACHA_BUTTON_CHOOSEBRONZEKEY , this)  , NGACHA_BUTTON_CHOOSEBRONZEKEY , TEXT("button_choosebronzekey"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NGACHA_BUTTON_STARTCHOOSENKEY , this)  , NGACHA_BUTTON_STARTCHOOSENKEY , TEXT("button_NONAME1"));

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC , NGACHA_STATIC_CHOOSEKEY , this)  , NGACHA_STATIC_CHOOSEKEY , TEXT("imagestatic_NONAME1"));
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NGACHA_EDIT_TOPNOTICE , this) , NGACHA_EDIT_TOPNOTICE , TEXT("editbox_topnotice"));

	

	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Enable(false);
  	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->SetChangeTextDrawMode( true );
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->SetFontSize(16);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->SetChangeTextDrawMode( true );
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->SetFontSize(16);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->SetText(GetTxtFromMgr(7077), RGB(255,255,255));
	cltClient *pclclient = (cltClient *)pclClient;
	 
	//cltGameMsgRequest_GachaProduct_List pclinfo();
	cltMsg clMsg( GAMEMSG_REQUEST_GACHAPRODUCT_LIST, 0 , NULL );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

	SetTopMostWindow( true );

	ChangeCurrentStepAndInit( GACHASTEP_TOP );	


	SetTimerOnOff( true );
	SetActionTimer( 150 );


	pclclient->pclMusic->pclSoundManager->BackGroundSoundStop();
	pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_TugOfWar.ogg"), true, false );

	// 각 아이템 이미지 좌표 설정
	for( SI16 i = 0; i < MAX_SHOW_GACHA_PRODUCT ; i ++ )
	{
		switch( i )
		{
		case 0 :
			{
				m_GachaProductPoint[i].x = 448;
				m_GachaProductPoint[i].y = 373;
			}
			break;
		case 1 :
			{
				m_GachaProductPoint[i].x = 634;
				m_GachaProductPoint[i].y = 319;
			}
			break;
		case 2 :
			{
				m_GachaProductPoint[i].x = 727;
				m_GachaProductPoint[i].y = 319;
			}
			break;
		case 3 :
			{
				m_GachaProductPoint[i].x = 815;
				m_GachaProductPoint[i].y = 319;
			}
			break;
		case 4 :
			{
				m_GachaProductPoint[i].x = 634;
				m_GachaProductPoint[i].y = 406;
			}
			break;
		case 5 :
			{
				m_GachaProductPoint[i].x = 727;
				m_GachaProductPoint[i].y = 406;
			}
			break;
		case 6 :
			{
				m_GachaProductPoint[i].x = 815;
				m_GachaProductPoint[i].y = 406;
			}
			break;
		}

	}
		

}

void CALLBACK CNGachaDlg::StaticNGachaDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGachaDlg *pThis = (CNGachaDlg *)pControllerMgr;
	pThis->NGachaDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNGachaDlg::NGachaDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NGACHA_BUTTON_CLOSE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}			
		}
		break;
	case NGACHA_BUTTON_GAMESTART:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					
					ChangeCurrentStepAndInit( GACHASTEP_CHOOSEKEY );

					
				}
				break;
			}
		}
		break;	
	case NGACHA_BUTTON_CHOOSEGOLDKEY:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( GACHA_USE_GOLDKEY ) == true )
					{
						m_myGachaUseKeyKind = GACHA_USE_GOLDKEY;
						//ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
					}					

				}
				break;
			}
		}
		break;	
	case NGACHA_BUTTON_CHOOSESILVERKEY:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( GACHA_USE_SILVERKEY ) == true )
					{
						m_myGachaUseKeyKind = GACHA_USE_SILVERKEY;
						//ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
					}					

				}
				break;
			}
		}
		break;	
	case NGACHA_BUTTON_CHOOSEBRONZEKEY:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( GACHA_USE_BRONZEKEY ) == true )
					{
						m_myGachaUseKeyKind = GACHA_USE_BRONZEKEY;
						//ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
					}					

				}
				break;
			}
		}
		break;	
	case NGACHA_BUTTON_CHOOSEKEYPIECE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( GACHA_USE_KEYPIECE ) == true )
					{
						m_myGachaUseKeyKind = GACHA_USE_KEYPIECE;
						//ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
					}					

				}
				break;
			}
		}
		break;	
	case NGACHA_BUTTON_STARTCHOOSENKEY:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( m_myGachaUseKeyKind ) == true )
					{						
						ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
					}					

				}
				break;
			}
		}
		break;
	case NGACHA_BUTTON_CHOOSEKEYCLOSE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
											
					ChangeCurrentStepAndInit(GACHASTEP_TOP);
					

				}
				break;
			}
		}
		break;

	case NGACHA_BUTTON_BOX1:
	case NGACHA_BUTTON_BOX2:
	case NGACHA_BUTTON_BOX3:
	case NGACHA_BUTTON_BOX4:
	case NGACHA_BUTTON_BOX5:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( m_myGachaUseKeyKind ) == true )
					{
						pclClient->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_OPEN_BOX"))    , 0, 0);
												
  						ChangeCurrentStepAndInit(GACHASTEP_OPENBOX);
                        						
					}
					else
					{
						ChangeCurrentStepAndInit(GACHASTEP_KEYNEED);
					}

				}
				break;
				
			}
		}
		break;
	case NGACHA_BUTTON_TOP:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					ChangeCurrentStepAndInit( GACHASTEP_TOP );
				}
				break;
			}
		}
		break;
	case NGACHA_BUTTON_ONEMORE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( CheckHaveKey( m_myGachaUseKeyKind ) == true )
					{
						ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
					}
					else
					{
						ChangeCurrentStepAndInit(GACHASTEP_KEYNEED);
					}					
				}
				break;
			}
		}
		break;
	case NGACHA_BUTTON_ITEMMALL:
	case NGACHA_BUTTON_BUYKEY:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
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
						((cltClient*)pclClient)->CreateInterface(NNEWITEMMALL_DLG);	
					}
				}
				break;
			}
		}
		break;
	}

}

void CNGachaDlg::Action()
{
	// 공통 액션
	
	m_FrameCnt++;

	// 자신이 가진 열쇠 갯수와 열쇠 조각 갯수를 구한다

	m_myGachaGoldKeyNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_GOLDKEY_ITEMUNIQUE );
	m_myGachaSilverKeyNum = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_SILVERKEY_ITEMUNIQUE );
	// 이벤트 동열쇠까지 개수에 추가한다.-------------------------------------------------
	m_myGachaBronzeKeyNum = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_BRONZEKEY_ITEMUNIQUE );
	m_myGachaBronzeKeyNum += pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_BRONZEKEY_EVENT_ITEMUNIQUE );
	// -----------------------------------------------------------------------------------
	m_myGachaPieceNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_KEYPIECE_ITEMUNIQUE );

	m_myGachaGoldKeyNum = min( 999 , m_myGachaGoldKeyNum );
	m_myGachaSilverKeyNum = min( 999 , m_myGachaSilverKeyNum );
	m_myGachaBronzeKeyNum = min( 999 , m_myGachaBronzeKeyNum );
	m_myGachaPieceNum = min( 999 , m_myGachaPieceNum );


	// 각 단계별 액션

	if( m_siCurrentStep == GACHASTEP_DROPBOX )
	{
		ActionStepDropBox();
	}
	else if( m_siCurrentStep == GACHASTEP_OPENBOX )
	{
		ActionStepOpenBox();
	}
}

void CNGachaDlg::ActionStepDropBox()
{

 	SI32 adjustX = 0;
	SI32 adjustY = - m_RunRaccoonImg.GetYSize() / 2;

	m_bRaccoonDraw = true;

	if( m_FrameCnt > 21  )
	{		
		ChangeCurrentStepAndInit( GACHASTEP_SELECTBOX );
		return;
	}
	else if( m_FrameCnt > 18  )
	{
		m_bRaccoonDraw = false;	
	}
	else if( m_FrameCnt > 15  )
	{
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(true);		
		m_RunRaccoonPoint.x = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->GetX();
		m_RunRaccoonPoint.y = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->GetY();
		m_RunRaccoonPoint.x += adjustX;
		m_RunRaccoonPoint.y += adjustY;

		if( m_FrameCnt == 16 )
		{
			pclClient->PushEffect(    pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_MOVE_MONSTER"))    , 0, 0);
			
		}

	}
	else if( m_FrameCnt > 12  )
	{
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(true);
		m_RunRaccoonPoint.x = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->GetX();
		m_RunRaccoonPoint.y = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->GetY();
		m_RunRaccoonPoint.x += adjustX;
		m_RunRaccoonPoint.y += adjustY;
		
		if( m_FrameCnt == 13 )
		{
			pclClient->PushEffect(    pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_MOVE_MONSTER"))    , 0, 0);
		}
	}
	else if( m_FrameCnt > 9  )
	{
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(true);
		m_RunRaccoonPoint.x = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->GetX();
		m_RunRaccoonPoint.y = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->GetY();
		m_RunRaccoonPoint.x += adjustX;
		m_RunRaccoonPoint.y += adjustY;

		if( m_FrameCnt == 10 )
		{
			pclClient->PushEffect(    pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_MOVE_MONSTER"))    , 0, 0);
		}
	}
	else if( m_FrameCnt > 6  )
	{
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(true);
		m_RunRaccoonPoint.x = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->GetX();
		m_RunRaccoonPoint.y = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->GetY();
		m_RunRaccoonPoint.x += adjustX;
		m_RunRaccoonPoint.y += adjustY;

		if( m_FrameCnt == 7 )
		{
			pclClient->PushEffect(    pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_MOVE_MONSTER"))    , 0, 0);
		}
	}
	else if( m_FrameCnt > 3  )
	{
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(true);
		m_RunRaccoonPoint.x = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->GetX();
		m_RunRaccoonPoint.y = m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->GetY();
		m_RunRaccoonPoint.x += adjustX;
		m_RunRaccoonPoint.y += adjustY;

		if( m_FrameCnt == 4 )
		{
			pclClient->PushEffect(    pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_MOVE_MONSTER"))    , 0, 0);
		}
	}
	else if( m_FrameCnt > 0  )
	{
		m_bRaccoonDraw = false;
	}
}

void CNGachaDlg::ActionStepOpenBox()
{

	if( m_FrameCnt > 100  )	// 오래기다려도 계속 OpenBox 단계라면 문제가있다. 종료된다.
	{		
		DeleteDialog();
	}

	if( m_FrameCnt == 5 ) // 5프레임 정도 딜레이 후 서버를 향해 키를 소모하고 가챠아이템을 뽑을것을 의뢰한다
	{
		if( CheckHaveKey( m_myGachaUseKeyKind ) == false )
		{
			ChangeCurrentStepAndInit(GACHASTEP_KEYNEED);
			return;
		}

		cltClient * pclclient = (cltClient*)pclClient;

		cltGameMsgRequest_GachaBuy pclinfo( m_myGachaUseKeyKind );
		cltMsg clMsg( GAMEMSG_REQUEST_GACHABUY, sizeof(pclinfo) , (BYTE*) &pclinfo );
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

void CNGachaDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	//////////////////////////////////////
	// 항상 보여지는 공통 UI Draw
	//////////////////////////////////////
	// 가차 열쇠, 조각 갯수 그려줌

	SI32 GoldKeyNum[3];
	GoldKeyNum[0] = m_myGachaGoldKeyNum  / 100;
	GoldKeyNum[1] = (m_myGachaGoldKeyNum %100)  / 10;
	GoldKeyNum[2] = m_myGachaGoldKeyNum %10;

	SI32 SilverKeyNum[3];
	SilverKeyNum[0] = m_myGachaSilverKeyNum  / 100;
	SilverKeyNum[1] = (m_myGachaSilverKeyNum %100)  / 10;
	SilverKeyNum[2] = m_myGachaSilverKeyNum %10;

	SI32 BronzeKeyNum[3];
	BronzeKeyNum[0] = m_myGachaBronzeKeyNum  / 100;
	BronzeKeyNum[1] = (m_myGachaBronzeKeyNum %100)  / 10;
	BronzeKeyNum[2] = m_myGachaBronzeKeyNum %10;

	SI32 KeyPieceNum[3];
	KeyPieceNum[0] = m_myGachaPieceNum / 100;
	KeyPieceNum[1] = (m_myGachaPieceNum %100)  / 10;
 	KeyPieceNum[2] = (m_myGachaPieceNum %10);


	for( SI16 i = 0 ; i < 3 ; i ++)
	{    
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX , 282 +GetY() + ScreenRY, GoldKeyNum[i] );
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX , 332 +GetY() + ScreenRY, SilverKeyNum[i] + 20 );
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX , 382 +GetY() + ScreenRY, BronzeKeyNum[i] + 30);
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX, 490 +GetY() + ScreenRY, KeyPieceNum[i] + 10 );

	}
	
	// 가차파워 게이지 출력

	SI32 GachaGageImgMaxNum = m_GachaGageImg.GetImageNum();
	SI32 GachaGageRate = 0;
	
 	GachaGageRate = min(MAX_GACHA_BONUS_VALUE , m_uiGachaCnt) * GachaGageImgMaxNum / MAX_GACHA_BONUS_VALUE;


  	if( GachaGageRate > 0 )
	{
		SI32 GageYSize =  m_GachaGageImg.GetYSize();

		for( SI16 i = 0 ; i < GachaGageRate ; i ++ )
		{
			SI32 imageFont = GachaGageImgMaxNum - i - 1;			
			GP.PutSpr( &m_GachaGageImg , 249 +GetX()  + ScreenRX , 237 + GageYSize * imageFont +GetY() + ScreenRY, imageFont );
		}

	}

	GP.PutSpr( &m_GachaGageOutLineImg , 243 +GetX() + ScreenRX , 231 +GetY() + ScreenRY, 0 );
	

	//////////////////////////////////////
	// 각 단계별 맞춤 드로우
    //////////////////////////////////////

	if( m_siCurrentStep == GACHASTEP_TOP )
	{
		DrawStepTop(  ScreenRX , ScreenRY);
	}
	else if( m_siCurrentStep == GACHASTEP_DROPBOX )
	{
		DrawStepDropBox( ScreenRX , ScreenRY);
	}
	else if( m_siCurrentStep == GACHASTEP_SELECTBOX )
	{
		DrawStepSelectBox( ScreenRX , ScreenRY);
	}
	else if( m_siCurrentStep == GACHASTEP_OPENBOX )
	{
		DrawStepOpentBox( ScreenRX , ScreenRY);
	}
	else if( m_siCurrentStep == GACHASTEP_ITEMGET )
	{
		DrawStepItemGet( ScreenRX , ScreenRY);
	}
	else if( m_siCurrentStep == GACHASTEP_RAREITEMGET )
	{
		DrawStepRareItemGet( ScreenRX , ScreenRY);
	}
	else if( m_siCurrentStep == GACHASTEP_KEYNEED )
	{
		DrawStepKeyNeed( ScreenRX , ScreenRY);
	}

}


void CNGachaDlg::DrawStepTop(  SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	SI32 imageType = 0 , imageIndex = 0; 

	for( SI16 i = 0 ; i < MAX_SHOW_GACHA_PRODUCT ; i ++ )
	{
		if( m_GachaProduct[i].siUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( m_GachaProduct[i].siUnique , &imageType, &imageIndex ) )
		{
			TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

			if( pSpr )
			{
				GP.PutSpr( pSpr ,ScreenRX + m_GachaProductPoint[i].x + GetX() ,ScreenRY + m_GachaProductPoint[i].y + GetY() , imageIndex);
			}
		}
	}
}
 
void CNGachaDlg::DrawStepDropBox(  SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	// 배경 드로우
	GP.PutSpr( &m_GachaFieldImg , ScreenRX + 318 + GetX() , ScreenRY + 192 + GetY(), 0  );
}

void CNGachaDlg::DrawStepSelectBox( SI32 ScreenRX, SI32 ScreenRY )
{
 	cltClient *pclclient = ( cltClient * )pclClient;
	// 배경 드로우
	GP.PutSpr( &m_GachaFieldImg , ScreenRX + 318 + GetX(), ScreenRY + 192 + GetY() , 0  );
	

	// 클릭 마크 표시

	//GP.PutSpr( &m_ClickImg, ScreenRX + 544 + GetX(), ScreenRY + 295+ GetY() , 0 );
	

}

void CNGachaDlg::DrawStepOpentBox( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	// 보물 상자 여는 구리댁 드로우
	GP.PutSpr( &m_GachaFieldImg , ScreenRX + 318 + GetX() , ScreenRY + 192 + GetY() , 1  );
}

void CNGachaDlg::DrawStepItemGet( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	// 배경 찍기
	GP.PutSpr( &m_GachaResultImg , ScreenRX + 318 + GetX(), ScreenRY + 192 + GetY() , 0  );

	// 아이템 그려줌

	SI32 imageType = 0 , imageIndex = 0; 

	if( m_GachaGetProduct.siUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( m_GachaGetProduct.siUnique , &imageType, &imageIndex ) )
	{
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

		if( pSpr )
		{
			GP.PutSpr( pSpr ,ScreenRX + 630 + GetX(),ScreenRY + 371 + GetY(), imageIndex);
		}
	}

}

void CNGachaDlg::DrawStepRareItemGet( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	// 배경 찍기
	GP.PutSpr( &m_GachaResultImg , ScreenRX + 318 + GetX(), ScreenRY + 192 + GetY(), 1  );

	SI32 imageType = 0 , imageIndex = 0; 

	if( m_GachaGetProduct.siUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( m_GachaGetProduct.siUnique , &imageType, &imageIndex ) )
	{
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

		if( pSpr )
		{
			GP.PutSpr( pSpr ,ScreenRX + 630 + GetX(),ScreenRY + 371 + GetY(), imageIndex);
		}
	}
}

void CNGachaDlg::DrawStepKeyNeed( SI32 ScreenRX, SI32 ScreenRY )
{

	
	cltClient *pclclient = ( cltClient * )pclClient;
	// 배경 그려줌
	GP.PutSpr( &m_GachaResultImg , ScreenRX + 318 + GetX(), ScreenRY + 192  + GetY(), 2  );
	
	/*
	SI32 imageType = 0 , imageIndex = 0; 

	SI32 GoldKeyItemUnique = GACHA_GOLDKEY_ITEMUNIQUE;
	SI32 SilverKeyItemUnique = GACHA_SILVERKEY_ITEMUNIQUE;
	SI32 BronzeKeyItemUnique = GACHA_BRONZEKEY_ITEMUNIQUE;
	SI32 KeyItemUnique = GACHA_KEYPIECE_ITEMUNIQUE;

	// 열쇠 그려줌
	if( GoldKeyItemUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( GoldKeyItemUnique , &imageType, &imageIndex ) )
	{
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

		if( pSpr )
		{
			GP.PutSpr( pSpr ,ScreenRX + 542 + GetX() ,ScreenRY + 371 +GetY(), imageIndex);
		}
	}
	// 열쇠 그려줌
	if( SilverKeyItemUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( SilverKeyItemUnique , &imageType, &imageIndex ) )
	{
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

		if( pSpr )
		{
			GP.PutSpr( pSpr ,ScreenRX + 562 + GetX() ,ScreenRY + 371 +GetY(), imageIndex);
		}
	}
	// 열쇠 그려줌
	if( BronzeKeyItemUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( BronzeKeyItemUnique , &imageType, &imageIndex ) )
	{
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

		if( pSpr )
		{
			GP.PutSpr( pSpr ,ScreenRX + 582 + GetX() ,ScreenRY + 371 +GetY(), imageIndex);
		}
	}


	// 열쇠 조각 그려줌
	if( KeyItemUnique && pclclient->pclItemManager->GetItemGImageFileIndexAndFont( KeyItemUnique , &imageType, &imageIndex ) )
	{
		TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( imageType );

		if( pSpr )
		{
			GP.PutSpr( pSpr ,ScreenRX + 676 + GetX() ,ScreenRY + 371 + GetY() , imageIndex);
		}
	}
	*/

}


void CNGachaDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;




	//////////////////////////////////////
	// 각 단계별 맞춤 드로우
	//////////////////////////////////////

	if( m_siCurrentStep == GACHASTEP_TOP )
	{
		DrawLastStepTop(  ScreenRX , ScreenRY );
	}
	else if( m_siCurrentStep == GACHASTEP_CHOOSEKEY )
	{
		DrawLastStepChooseKey( ScreenRX , ScreenRY );
	}
	else if( m_siCurrentStep == GACHASTEP_DROPBOX )
	{
		DrawLastStepDropBox( ScreenRX , ScreenRY );
	}
	else if( m_siCurrentStep == GACHASTEP_ITEMGET )
	{
		DrawLastStepItemGet( ScreenRX , ScreenRY );
	}
	else if( m_siCurrentStep == GACHASTEP_RAREITEMGET)
	{
		DrawLastStepRareItemGet( ScreenRX , ScreenRY );
	}
	else if( m_siCurrentStep == GACHASTEP_KEYNEED )
	{
		DrawLastStepKeyNeed( ScreenRX , ScreenRY );
	}

}




void CNGachaDlg::DrawLastStepTop( SI32 ScreenRX , SI32 ScreenRY )
{
	
 	cltClient *pclclient = ( cltClient * )pclClient;
	
	HDC hdc;
	HFONT hFont;

 	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	
	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// "이번 달 가챠 선물"
	{
  		TCHAR * pTitleText = GetTxtFromMgr(7022);
		RECT rect = { 463 , 260 , 685 , 297 };
		
		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(255, 255, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(50, 146, 255));
		DrawText( hdc, pTitleText, lstrlen(pTitleText), &rect, DT_CENTER | DT_VCENTER );

	}

	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	
	
 	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	// "최고물품 "
	{
		TCHAR * pText = GetTxtFromMgr(7025);
		RECT rect = { 426 , 338 , 506 , 362 };
		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		SetTextColor(hdc, RGB(255, 255, 255));

		DrawText( hdc, pText, lstrlen(pText), &rect, DT_CENTER | DT_VCENTER );
	}

	// 최고물품 아이템의 이름
  	if( m_GachaProduct[0].siUnique > 0 )
	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( m_GachaProduct[0].siUnique );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 426 , 428 , 504 , 459 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(255, 255, 255));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}

		// PCK : 가챠 레어아이템 소개 텍스트 수정 (09.03.03)
		NTCHARString256	kText( GetTxtFromMgr(7017) );
		NTCHARString64	kGachaRareItemName( TEXT("") );

		kGachaRareItemName = (TCHAR*)pclClient->pclItemManager->GetName(m_GachaProduct[0].siUnique);
		kText.Replace( TEXT("#gacharareitemname#"), kGachaRareItemName );

		m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->SetText(kText , RGB(255,255,255) );
	}
	
	// 다른 아이템 여섯개의 아이템 이름
	
	
	{
        for( SI16 i = 1 ; i < MAX_SHOW_GACHA_PRODUCT ; i++ )
		{
			if( m_GachaProduct[i].siUnique > 0 )
			{
  				const TCHAR * pText = pclClient->pclItemManager->GetName( m_GachaProduct[i].siUnique );
				if( pText != NULL)
				{
					RECT rect;
					rect.left =	m_GachaProductPoint[i].x - 17;
					rect.right = rect.left + 75;
					rect.top = m_GachaProductPoint[i].y + 48;
					rect.bottom = rect.top + 32;

					TCHAR szBuffer[256];
					//TCHAR * pSrcText = GetTxtFromMgr(676);
				 	StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

					OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

					SetTextColor(hdc, RGB(3, 28, 56));
					DrawText( hdc, szBuffer , lstrlen(szBuffer ), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
				}

			}
		}
	}


	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	pclclient->lpBackScreen->ReleaseDC(hdc);
	
}


void CNGachaDlg::DrawLastStepChooseKey( SI32 ScreenRX, SI32 ScreenRY )
{
	
	cltClient *pclclient = ( cltClient * )pclClient;

	HDC hdc;
	HFONT hFont;

	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	
	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// "골라주세요!"
	{
		TCHAR * pTitleText = GetTxtFromMgr(7078);
		RECT rect = { 453 , 260 , 725 , 317 };

		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(255, 255, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(50, 146, 255));
		DrawText( hdc, pTitleText, lstrlen(pTitleText), &rect, DT_CENTER | DT_VCENTER );

	}

 	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	

	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	// "선택하세요"
	{
   		TCHAR * pText = GetTxtFromMgr(7079);
		RECT rect = { 520 , 377 , 761 , 410 };
		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(50, 146, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(255, 255, 255));

		DrawText( hdc, pText, lstrlen(pText), &rect, DT_CENTER | DT_VCENTER );
	}

	// 아이템의 이름

	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_GOLDKEY_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

   			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 470 , 472 , 545 , 524 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}

	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_SILVERKEY_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

  			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 555 , 472 , 630 , 524 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}

	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_BRONZEKEY_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 655 , 472 , 730 , 524 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}


	{
  		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_KEYPIECE_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

  			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 745 , 472 , 820 , 504 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}


	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	pclclient->lpBackScreen->ReleaseDC(hdc);

}

void CNGachaDlg::DrawLastStepDropBox( SI32 ScreenRX, SI32 ScreenRY )
{
	// 움직이는 구리댁 드로우

	static SI16 aniFrame = 0;
	static SI16 deleyCnt = 0;

	if( m_bRaccoonDraw == true ) 
	{
		// 구리댁 에니메이션
		if( deleyCnt++ > 3 )
		{
			aniFrame++;

			if( aniFrame >= m_RunRaccoonImg.GetImageNum() )
			{
				aniFrame = 0;
			}

			deleyCnt = 0;		
		}

		GP.PutSpr( &m_RunRaccoonImg , ScreenRX + m_RunRaccoonPoint.x , ScreenRY + m_RunRaccoonPoint.y , aniFrame );
	}
}


void CNGachaDlg::DrawLastStepItemGet( SI32 ScreenRX , SI32 ScreenRY )
{
 	cltClient *pclclient = ( cltClient * )pclClient;

	HDC hdc;
	HFONT hFont;

	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}

	
	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// "축하드립니다!"
	{
 		TCHAR * pTitleText = GetTxtFromMgr(7023);
		RECT rect = { 463 , 260 , 685 , 297 };

		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(255, 255, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(50, 146, 255));
		DrawText( hdc, pTitleText, lstrlen(pTitleText), &rect, DT_CENTER | DT_VCENTER );

	}

	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	

	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	// "받은물품 "
	{
 		TCHAR * pText = GetTxtFromMgr(7026);
  		RECT rect = { 550 , 337 , 751 , 370 };
		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(50, 146, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(255, 255, 255));

		DrawText( hdc, pText, lstrlen(pText), &rect, DT_CENTER | DT_VCENTER );
	}

	// 받은물품 아이템의 이름
	if( m_GachaGetProduct.siUnique > 0 )
	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( m_GachaGetProduct.siUnique );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

 			RECT rect = { 593 , 429 , 708 , 461 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}

	if(hOldFont)
	{
 		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	pclclient->lpBackScreen->ReleaseDC(hdc);
}

void CNGachaDlg::DrawLastStepRareItemGet( SI32 ScreenRX , SI32 ScreenRY )
{
 	cltClient *pclclient = ( cltClient * )pclClient;

	HDC hdc;
	HFONT hFont;

	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	
	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// "축하드립니다!"
	{
		TCHAR * pTitleText = GetTxtFromMgr(7023);
		RECT rect = { 463 , 260 , 685 , 297 };

		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(255, 255, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(50, 146, 255));
		DrawText( hdc, pTitleText, lstrlen(pTitleText), &rect, DT_CENTER | DT_VCENTER );

	}

	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}
	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}

	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	// "최고물품 "
	{
		TCHAR * pText = GetTxtFromMgr(7025);
		RECT rect = { 550 , 337 , 751 , 370 };
		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(50, 146, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(255, 255, 255));

		DrawText( hdc, pText, lstrlen(pText), &rect, DT_CENTER | DT_VCENTER );
	}

	// 받은물품 아이템의 이름
	if( m_GachaGetProduct.siUnique > 0 )
	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( m_GachaGetProduct.siUnique );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 608 , 429 , 693 , 461 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}

	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	pclclient->lpBackScreen->ReleaseDC(hdc);
}

void CNGachaDlg::DrawLastStepKeyNeed( SI32 ScreenRX , SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	HDC hdc;
	HFONT hFont;

	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	
	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// "부족합니다!"
	{
 		TCHAR * pTitleText = GetTxtFromMgr(7024);
		RECT rect = { 463 , 260 , 685 , 297 };

		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(255, 255, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pTitleText, lstrlen(pTitleText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(50, 146, 255));
		DrawText( hdc, pTitleText, lstrlen(pTitleText), &rect, DT_CENTER | DT_VCENTER );

	}

	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else {
		hFont = CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}
	

	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	// "필요물품 "
	{
		TCHAR * pText = GetTxtFromMgr(7028);
		RECT rect = { 550 , 337 , 751 , 370 };
		OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

		RECT temprc;

		SetTextColor(hdc, RGB(50, 146, 255));

		// 외각선 그리기	
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );
		CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, pText, lstrlen(pText), &temprc, DT_CENTER | DT_VCENTER );

		SetTextColor(hdc, RGB(255, 255, 255));

		DrawText( hdc, pText, lstrlen(pText), &rect, DT_CENTER | DT_VCENTER );
	}

	// 아이템의 이름

	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_GOLDKEY_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

   	 		StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 470 , 430 , 545 , 482 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}

	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_SILVERKEY_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 555 , 430 , 630 , 482 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}

	{
		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_BRONZEKEY_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 655 , 430 , 730 , 482 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}
	

	{
   		const TCHAR * pText = pclClient->pclItemManager->GetName( GACHA_KEYPIECE_ITEMUNIQUE );
		if( pText != NULL)
		{
			TCHAR szBuffer[256];
			//TCHAR * pSrcText = GetTxtFromMgr(676);

 			StringCchPrintf( szBuffer , 256 , TEXT("%s") , pText );

			RECT rect = { 745 , 430 , 820 , 482 };
			OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY );

			SetTextColor(hdc, RGB(3, 28, 56));

			DrawText( hdc, szBuffer, lstrlen(szBuffer), &rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		}
	}


	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc,hOldFont)) ;
	}

	pclclient->lpBackScreen->ReleaseDC(hdc);
}


void CNGachaDlg::ChangeCurrentStepAndInit(SI16 Step )
{ 	
	m_siCurrentStep = Step; 
	m_FrameCnt = 0;

	if( m_siCurrentStep == GACHASTEP_TOP )
	{
		InitStepTop();
	}
	else if( m_siCurrentStep == GACHASTEP_CHOOSEKEY )
	{
		InitStepChooseKey();
	}
	else if( m_siCurrentStep == GACHASTEP_DROPBOX )
	{
		InitStepDropBox();
	}
	else if( m_siCurrentStep == GACHASTEP_OPENBOX )
	{
		InitStepOpenBox();
	}
	else if( m_siCurrentStep == GACHASTEP_ITEMGET )
	{
		InitStepItemGet();
	}
	else if( m_siCurrentStep == GACHASTEP_RAREITEMGET )
	{
		InitStepRareItemGet();
	}
	else if( m_siCurrentStep == GACHASTEP_KEYNEED )
	{
		InitStepKeyNeed();
	}

}

void CNGachaDlg::InitStepTop()
{
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(true);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);		
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(true);	
	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(false);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(true);
	
}

void CNGachaDlg::InitStepChooseKey()
{

	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(false);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(false);
 	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(false);

 	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(true);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(true);

	bool bHaveGoldKey = false, bHaveSilverKey = false , bHaveBronzeKey = false , bHaveKeyPiece = false;

	bHaveGoldKey = CheckHaveKey( GACHA_USE_GOLDKEY );
	bHaveSilverKey = CheckHaveKey( GACHA_USE_SILVERKEY );
	bHaveBronzeKey = CheckHaveKey( GACHA_USE_BRONZEKEY );
	bHaveKeyPiece = CheckHaveKey( GACHA_USE_KEYPIECE );

	if( bHaveGoldKey == true )
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Enable(true);
	else 
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Enable(false);
	
	if( bHaveSilverKey == true )
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Enable(true);
	else 
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Enable(false);
	
	if( bHaveBronzeKey == true )
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Enable(true);
	else 
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Enable(false);
	
	if( bHaveKeyPiece == true )
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Enable(true);
	else 
		m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Enable(false);

	if( bHaveGoldKey == false && bHaveSilverKey == false && bHaveBronzeKey == false && bHaveKeyPiece == false )
	{
		ChangeCurrentStepAndInit( GACHASTEP_KEYNEED );
	}
	
}

void CNGachaDlg::InitStepDropBox()
{
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(false);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(false);

	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(false);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(false);
}


void CNGachaDlg::InitStepOpenBox()
{
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(false);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(false);

	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(false);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(false);
}


void CNGachaDlg::InitStepItemGet()
{
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(false);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(true);
	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(false);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(true);
}

void CNGachaDlg::InitStepRareItemGet()
{
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(false);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(true);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(7056) , RGB(255,255,255) );

	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(false);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(true);
}

void CNGachaDlg::InitStepKeyNeed()
{
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_GAMESTART)->Show(false);
//	m_InterfaceFactory.GetButton(NGACHA_BUTTON_KEYPIECEUSE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ITEMMALL)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_ONEMORE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_TOP)->Show(true);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX1)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX2)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX3)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX4)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_BOX5)->Show(false);
	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->Show(true);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(7016) , RGB(255,255,255) );

	m_InterfaceFactory.GetImageStatic(NGACHA_STATIC_CHOOSEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEGOLDKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSESILVERKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYPIECE)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEBRONZEKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_STARTCHOOSENKEY)->Show(false);
	m_InterfaceFactory.GetButton(NGACHA_BUTTON_CHOOSEKEYCLOSE)->Show(false);

	m_InterfaceFactory.GetEdit(NGACHA_EDIT_TOPNOTICE)->Show(true);
}

bool CNGachaDlg::CheckHaveKey( SI32 GachaUseKeyKind )
{
	SI32 KeyItemUnique = 0;
	SI32 KeyNeedNum = 1;

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
	else 
	{
		return false;
	}

	SI32 keyItemNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( KeyItemUnique );

	if( GachaUseKeyKind == GACHA_USE_BRONZEKEY &&
		keyItemNum < KeyNeedNum )
	{
		// 이벤트 키 개수를 체크한다.
		KeyItemUnique = GACHA_BRONZEKEY_EVENT_ITEMUNIQUE;
		keyItemNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( KeyItemUnique );
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

bool CNGachaDlg::CheckHaveKeyPiece()
{
	m_myGachaPieceNum =	pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_KEYPIECE_ITEMUNIQUE );
	
	// 열쇠 조각이 모자르다면..
	if( m_myGachaPieceNum < GACHA_KEYPIECE_NEED_NUM )
	{
		return false;
	}
	else
	{
		return true;
	}

}

void CNGachaDlg::SetGachaProductAndCount( cltItem * pclItem , UI16 GachaCnt )
{
	m_uiGachaCnt = GachaCnt;
    
	for( SI16  i = 0 ; i < MAX_SHOW_GACHA_PRODUCT ; i++ )
	{
		m_GachaProduct[ i ].Set(&pclItem[i]);
	}

}


void CNGachaDlg::SetGachaGetProductAndCount( cltItem * pclItem , UI16 GachaCnt , bool BestProduct  )
{
	m_uiGachaCnt = GachaCnt;

	m_GachaGetProduct.Set( pclItem );

	if( BestProduct )
	{
		ChangeCurrentStepAndInit( GACHASTEP_RAREITEMGET );
	}
	else
	{
		ChangeCurrentStepAndInit( GACHASTEP_ITEMGET );
	}

}



void CNGachaDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{

	/*
	switch(MsgIndex)
	{
	case 0:
		{
			if(RetValue)
			{
				m_bUseCashKey = false;
				ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
			}
			else
			{
				ChangeCurrentStepAndInit(GACHASTEP_KEYNEED);
			}
		}
		break;
	case 1:
		{
			if(RetValue)
			{
				m_bUseCashKey = true;
				ChangeCurrentStepAndInit(GACHASTEP_DROPBOX);
			}
			else
			{
				ChangeCurrentStepAndInit(GACHASTEP_KEYNEED);
			}
		}
		break;
	default:
		break;
	}
*/
}