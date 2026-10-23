//#if (defined(__LEON_START_ALL__) || defined(__LEON_CNSOUNDONOFFDLG__)) && defined(_DEBUG)
/* ==========================================================================
	클래스 :		CNSoundOnOffDlg

	작성일 :		05/03/02
	작성자 :		정용래

	목  적 :		CNSoundOnOffDlg 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 환경 설정 다이얼로그를 생성한다.

	변경사항 :		없음
	
	차후작업 :		1. 전역 변수 extern CSoundOnOffDlg g_SoundOnOffDlg; 를 사용 하였음 => 차후 없애면 재작업 필요

   ========================================================================*/

#pragma once

#include "NSoundOnOffDlg.h"

#include "../../Lib/Sound/SoundMgr.h"
#include "../../Client/Music/music.h"

#include "../../Client/client.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Fishing.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Prompt/PromptMgr.h"

#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"
#include "../../InterfaceMgr/interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../../Ninterface/NChatbardlg/NChatBarDlg.h"

extern CSoundOnOffDlg g_SoundOnOffDlg;

extern SI32		gsiResolution;		// 게임의 해상도. 
extern BOOL		g_bFullScreen;		// 풀스크린 여부 
extern bool		SetGameModeInFile(SI32 gamemode, SI32 res, BOOL bFullScreen, SI32 runmode );
extern bool		SetFullScreenInFile( BOOL bFullScreen );
extern bool		GetFullScreenInFile( BOOL* bFullScreen );
extern SI16		g_siChannelingCode;

const SI16	Language_English = 0;
const SI16	Language_Chinese = 1;


CNSoundOnOffDlg::CNSoundOnOffDlg()
{	
	m_bShow = false;
	
	m_bFullScreenMode = true;		// default : 새인터페이스 사용
	m_bTmpFullScreenMode = true;	// 
	m_bEffectSound = true;
	m_bBackGroudnSound = true;
	m_bBigDisplayMode = true;
	m_bMiniMapShow = true;
	m_bMaxWindowMode = false;				// 최대창 크기 모드 사용
	m_bTmpMaxWindowMode = false;
	m_bShowCharName		= false;
	//m_bFriendRegisterReject = false;
	/*
	leon add - 2005-03-10-10-00
	* m_pNSoundOnOffDlg와 체크 상태 공유를 위해서.. 추가함
	*/
	m_bPrivateTradeReject = false;
	m_bWhisperChatReject = false;
	m_bJewelItemNotifyReject = false;
	m_bSummonEvolveNotifyReject = false;
	m_bPartyInviteNotifyReject = false;
	m_bFriendAddNotifyReject = false;
	m_bNotifyHeadChat = false;
	m_bNotifyMinigame = false;
	m_bRightMouseMentAllow = true;
	m_bChatFixed = false;

	//KHY - 0110 - 채팅 모드 고정
	m_bChatTabFixed = false;
	m_siChatTabNowIndex = 0;

	m_siSelectCaptionKindIdex = -1;

	m_bIsLoadLanguage = false;
	m_bIsLoadFps = false;

	m_bAloneDraw = false;

	m_pBackgroundCheckBox = NULL;              
	m_pEffectCheckBox = NULL;                  
	//m_pWindowmodeCheckBox = NULL;                
	m_pMinimapRejectCheckBox = NULL;           
	m_pNotifyFriendAddRejectCheckBox = NULL;   
	m_pNotifyHeadChatCheckBox = NULL;          
	m_pNotifyJewelItemRejectCheckBox = NULL;   
	m_pNotifyMiniGameCheckBox = NULL;
	m_pNotifySummonEvolveRejectCheckBox = NULL;
	m_pPartyInviteRejectCheckBox = NULL;       
	m_pPrivateTradeRejectCheckBox = NULL;      
	m_pRightMouseMenuAllowCheckBox = NULL;     
	m_pWhisperChatRejectCheckBox = NULL;
	m_pChatFixedCheckBox = NULL;
	m_pChatTabFixedCheckBox = NULL;
	m_pShowCharNameCheckBox = NULL;


	m_pSelectCaptionKindCombobox = NULL;       
	m_pTotalSpendTimeEdit		= NULL;              

	m_pSettingStatic			= NULL;          
	m_pEffect_BackgroundStatic	= NULL;
	m_pGameSettingStatic		= NULL;      
	m_pCaptionInfoStatic		= NULL;      
	m_pTotalSpendTimeStatic		= NULL;   

	m_pOutLine_NONAME1			= NULL; 
	m_pOutLine_NONAME2			= NULL;
	m_pOutLine_NONAME4			= NULL; 
	m_pCheckBox_maximization	= NULL; 

	m_pStaticShowCountry		= NULL;	
	m_pRadioShowCountryOn		= NULL;
	m_pRadioShowCountryOff		= NULL;

	m_pStaticLanguage			= NULL;
	m_pComboLanguage			= NULL;

	m_pStaticFps				= NULL;
	m_pComboFps					= NULL;

	m_pShowPuchikomiCheckBox	= NULL;

	// 신체기관 보여주기
	m_pShowOrganCheckBox		= NULL;
	m_bOrganPannelShow			= false;
	
	m_pMyKeyCheckBox			= NULL;
	m_bMykey					= false;

	m_pGameTipCheckBox			= NULL;
	m_bGameTip					= false;

	m_pHelperCheckBox			= NULL;
	m_bHelper					= false;

	m_pPVPModeCheckBox			= NULL;
	m_bPVP						= false;
	

	// 사운드 조절 관련

	m_siSelectedSoundButton = 0;
	m_siSelectedBGMButton = 0;

	m_pSoundMinusButton = NULL;
	m_pSoundPlusButton = NULL;
	m_pBGMMinusButton = NULL;
	m_pBGMPlusButton = NULL;

	

	for( SI32 i = 0; i < 5; i++ )
	{
		m_pEffVolumeButton[i] = NULL;
		m_pBGMVolumeButton[i] = NULL;
	}

	m_pCheckBoxAloneDraw = NULL;
	
		
	FILE *fp = _tfopen( TEXT("./Config/SoundConfig.dat"), TEXT("rt") );
	if ( fp != NULL )
	{
		SI32 EffectSound = 1;
		SI32 BackGroundSound = 1;
		SI32 DisplayMode = 0;
		SI32 MiniMapShow = 1;

		SI32 FullScreenMode = 1;
		SI32 MaxWindowMode = 0;				// 최대창 크기 모드 사용

		TCHAR Buffer[256] = TEXT("");
		_fgetts( Buffer, 255, fp );


		SI32 RightMouseMentAllow = 0;		// 사용자 메뉴 보여주기    
		                                                              
		SI32 WhisperChatReject = 0;			// 모든 귓속말 거부
		SI32 PrivateTradeReject = 0;		// 모든 거래 거부  
		
		SI32 JewelItemNotifyReject = 0;		// 명품 알림 거부          
		                                                              
		SI32 PartyInviteNotifyReject = 0;	// 파트 초대 거부          
		SI32 FriendAddNotifyReject = 0;		// 친구 추가 거부          
		                                                              
		SI32 SummonEvolveNotifyReject = 0;	// 소환수진화 알림 거부
		                                                              
		SI32 NotifyHeadChat = 0;			// 모든 광고창 거부
		SI32 NotifyMinigame = 0;			// 미니게임 거부   

		SI32 ChatFixed = 0;					// 채팅창 고정 ( non-floating )
		                                                              
		SI32 siSelectCaptionKindIdex = 0;	// 자막 정보 인덱스        
		
		SI32 siSoundVolume = 0; // 0~4
		SI32 siBGMVolume = 0; // 0~4
		SI32 ShowCharName = 0;

		SI32 siAloneDraw = 0;

		//KHY - 0110 - 채팅 모드 고정
		SI32 bChatTabFixed = 0;
		SI32 siChatTabNowIndex = 0;

		// 신체기관 창 보여주기 여부
		SI32 siOrganPannelShow	= 0;

		SI16 siSuccess = _stscanf( Buffer, TEXT("%d %d %d %d %d \
												 %d %d %d %d %d \
												 %d %d %d %d %d \
												 %d %d %d %d %d \
												 %d %d %d %d %d \
												 %d %d"				), 
									&EffectSound, 
									&BackGroundSound, 
									&MiniMapShow, 
									&FullScreenMode,
									&RightMouseMentAllow ,		
									&WhisperChatReject,
									&PrivateTradeReject,
									&JewelItemNotifyReject,		
									&PartyInviteNotifyReject ,
									&FriendAddNotifyReject,		
									&SummonEvolveNotifyReject ,
									&NotifyHeadChat ,
									&NotifyMinigame,			
									&siSelectCaptionKindIdex,
									&MaxWindowMode,
									&ChatFixed,
									&siSoundVolume,		
									&siBGMVolume,
									&ShowCharName,
									&siAloneDraw,
									&bChatTabFixed,					//KHY - 0110 - 채팅 모드 고정// 채팅 모드고정 여부 -> 일반, 마을, 길드등등.
									&siChatTabNowIndex,				// 현재 채팅 모드 정장.
									&siOrganPannelShow,				// 신체기관 보여주기 여부
									&m_bMykey,						// [진성] 나만의 메뉴.	=> 2008-9-3
									&m_bGameTip,					// [진성] 게임 팁.		=> 2008-10-6
									&m_bHelper,						// [진성] 도우미.		=> 2008-10-10
									&m_bPVP							// [성웅] PVP모드
 									);

		if( siSuccess <= 5)
			RightMouseMentAllow = 1;

		// 이전 버젼과의 호환성을 위해...
		if( siSuccess <= 16 )	// SoundVolume, BGMVolume 값이 '없다'
		{
			m_siSelectedBGMButton = 3;
			m_siSelectedSoundButton = 3;
		}
		else
		{
			if( siSoundVolume >= 0 && siSoundVolume <5 )	// 정상적인 사운드 크기 값이 들어가있따.
			{
				m_siSelectedSoundButton = siSoundVolume;
			}
			else
			{
				m_siSelectedSoundButton = 3;
			}


			if( siBGMVolume >= 0  && siBGMVolume < 5 )
			{
				m_siSelectedBGMButton = siBGMVolume;
			}
			else
			{
				m_siSelectedBGMButton = 3;
			}
		}	
		/*
		if( siSuccess <=18 )
		{
         //   ChatFixed = 1;		// 디폴트가 채팅창 고정
		}
		*/
		//if( siSuccess == EOF )
		//{
		//	return;
		//}

		if( ShowCharName > 0 )
			m_bShowCharName = true;
		else 
			m_bShowCharName = false;

		if(ChatFixed > 0 )
			m_bChatFixed = true;
		else
			m_bChatFixed = false;

		//KHY - 0110 - 채팅 모드 고정
		if(bChatTabFixed > 0 )
		{
			m_bChatTabFixed = true;
			m_siChatTabNowIndex = siChatTabNowIndex;
		}
		else
		{
			m_bChatTabFixed = false;
			m_siChatTabNowIndex = 0;
		}

		if( RightMouseMentAllow > 0 )	
			m_bRightMouseMentAllow  = true;
		else 
			m_bRightMouseMentAllow  = false;
		//if( WhisperChatReject > 0 )	
		//	m_bWhisperChatReject  = true; 
		//else 
		//	m_bWhisperChatReject  = false;
		// 시작시 귓속말 항상 허용!
		m_bWhisperChatReject = false;
		if( PrivateTradeReject > 0 )
			m_bPrivateTradeReject  = true;
		else 
			m_bPrivateTradeReject  = false;
		if( JewelItemNotifyReject > 0 )	
			m_bJewelItemNotifyReject  = true; 
		else 
			m_bJewelItemNotifyReject  = false;
		//if( PartyInviteNotifyReject > 0 )	
		//	m_bPartyInviteNotifyReject  = true;
		//else 
		//	m_bPartyInviteNotifyReject  = false;
		// 시작시 파티 초대 항상 허용!!
		m_bPartyInviteNotifyReject  = false;

		if( FriendAddNotifyReject > 0 )
			m_bFriendAddNotifyReject  = true;
		else 
			m_bFriendAddNotifyReject  = false;
		if( SummonEvolveNotifyReject > 0 )
			m_bSummonEvolveNotifyReject  = true; 
		else 
			m_bSummonEvolveNotifyReject  = false;
		if( NotifyHeadChat > 0 )	
			m_bNotifyHeadChat  = true; 
		else 
			m_bNotifyHeadChat  = false;
		if( NotifyMinigame > 0 )
			m_bNotifyMinigame  = true; 
		else 
			m_bNotifyMinigame  = false;
		if( siSelectCaptionKindIdex >= 0 &&  siSelectCaptionKindIdex < MAX_CAPTIONKIND_NUM )	
			m_siSelectCaptionKindIdex  = siSelectCaptionKindIdex; 
		else 
			m_siSelectCaptionKindIdex  = -1;

		//if( pclClient->siServiceArea != ConstServiceArea_Korea )
		//{
			if( FullScreenMode == 0)
				FullScreenMode = 1;
		//}
		
		if ( EffectSound > 0 )
		{
			m_bEffectSound = true;			
		}
		else
		{
			m_bEffectSound = false;			
		}
		
		if( BackGroundSound > 0 )
		{
			m_bBackGroudnSound = true;			
		}
		else
		{
			m_bBackGroudnSound = false;			
		}

		if ( MiniMapShow > 0 )
		{
			m_bMiniMapShow = true;
		}
		else
		{
			m_bMiniMapShow = false;
		}

		if ( FullScreenMode > 0 )
		{
			m_bFullScreenMode = true;
			m_bTmpFullScreenMode = true;
		}
		else
		{
			m_bFullScreenMode = false;
			m_bTmpFullScreenMode = false;
		}

		// 신체기관 창 보여주기 여부
		m_bOrganPannelShow	= DATA_TO_BOOL( siOrganPannelShow );

		fclose( fp );
		//if( MaxWindowMode > 0 ) // 최대창 크기 모드 사용
		if( g_bFullScreen )
		{
			m_bMaxWindowMode = true;
			m_bTmpMaxWindowMode = true;

			if( m_bFullScreenMode == false )
			{
				m_bFullScreenMode = true;
				m_bTmpFullScreenMode = true;
			}
		}
		else
		{
			m_bMaxWindowMode = false;
			m_bTmpMaxWindowMode = false;
		}

		if( siAloneDraw > 0 )
			m_bAloneDraw = true;
		else
			m_bAloneDraw = false; 

	}
}

CNSoundOnOffDlg::~CNSoundOnOffDlg()
{

	if( m_pCheckBoxAloneDraw)
	{
		delete m_pCheckBoxAloneDraw;
		m_pCheckBoxAloneDraw = NULL;
	}


	if(m_pChatFixedCheckBox)
	{
		delete m_pChatFixedCheckBox;
		m_pChatFixedCheckBox = NULL;
	}

	if(m_pChatTabFixedCheckBox)
	{
		delete m_pChatTabFixedCheckBox;
		m_pChatTabFixedCheckBox = NULL;
	}

	if(m_pBackgroundCheckBox)
	{
		delete m_pBackgroundCheckBox;              
		m_pBackgroundCheckBox = NULL;
	}
	if(m_pEffectCheckBox)
	{
		delete m_pEffectCheckBox;                  
		m_pEffectCheckBox = NULL;
	}
	
	/*
	if(m_pWindowmodeCheckBox)
	{
		delete m_pWindowmodeCheckBox;                
		m_pWindowmodeCheckBox = NULL;
	}
	*/

	SAFE_DELETE(m_pShowCharNameCheckBox);

	SAFE_DELETE(m_pShowPuchikomiCheckBox);

	if(m_pMinimapRejectCheckBox)
	{
		delete m_pMinimapRejectCheckBox;           
		m_pMinimapRejectCheckBox = NULL;
	}
	if(m_pNotifyFriendAddRejectCheckBox)
	{
		delete m_pNotifyFriendAddRejectCheckBox;   
		m_pNotifyFriendAddRejectCheckBox = NULL;
	}
	if(m_pNotifyHeadChatCheckBox)
	{
		delete m_pNotifyHeadChatCheckBox;          
		m_pNotifyHeadChatCheckBox = NULL;
	}
	if(m_pNotifyJewelItemRejectCheckBox)
	{
		delete m_pNotifyJewelItemRejectCheckBox;   
		m_pNotifyJewelItemRejectCheckBox = NULL;
	}
	if(m_pNotifyMiniGameCheckBox)
	{
		delete m_pNotifyMiniGameCheckBox;          
		m_pNotifyMiniGameCheckBox = NULL;
	}
	if(m_pNotifySummonEvolveRejectCheckBox)
	{
		delete m_pNotifySummonEvolveRejectCheckBox;
		m_pNotifySummonEvolveRejectCheckBox = NULL;
	}
	if(m_pPartyInviteRejectCheckBox)
	{
		delete m_pPartyInviteRejectCheckBox;       
		m_pPartyInviteRejectCheckBox = NULL;
	}
	if(m_pPrivateTradeRejectCheckBox)
	{
		delete m_pPrivateTradeRejectCheckBox;      
		m_pPrivateTradeRejectCheckBox = NULL;
	}
	if(m_pRightMouseMenuAllowCheckBox)
	{
		delete m_pRightMouseMenuAllowCheckBox;     
		m_pRightMouseMenuAllowCheckBox = NULL;
	}
	if(m_pWhisperChatRejectCheckBox)
	{
		delete m_pWhisperChatRejectCheckBox;       
		m_pWhisperChatRejectCheckBox = NULL;
	}
	if(m_pSelectCaptionKindCombobox)
	{
		delete m_pSelectCaptionKindCombobox;       
		m_pSelectCaptionKindCombobox = NULL;
	}
	if(m_pTotalSpendTimeEdit)
	{
		delete m_pTotalSpendTimeEdit;              
		m_pTotalSpendTimeEdit = NULL;
	}
	if(m_pSettingStatic)
	{
		delete m_pSettingStatic;          
		m_pSettingStatic = NULL;
	}
	if(m_pEffect_BackgroundStatic)
	{
		delete m_pEffect_BackgroundStatic;
		m_pEffect_BackgroundStatic = NULL;
	}
	if(m_pGameSettingStatic)
	{
		delete m_pGameSettingStatic;
		m_pGameSettingStatic = NULL;
	}
	if(m_pCaptionInfoStatic)
	{
		delete m_pCaptionInfoStatic;
		m_pCaptionInfoStatic = NULL;
	}
	if(m_pTotalSpendTimeStatic )
	{
		delete m_pTotalSpendTimeStatic;
		m_pTotalSpendTimeStatic = NULL;
	}

	if( 	m_pOutLine_NONAME1	 )  	delete 	m_pOutLine_NONAME1	;
	if( 	m_pOutLine_NONAME2	 )  	delete 	m_pOutLine_NONAME2	;
	if( 	m_pOutLine_NONAME4	 )  	delete 	m_pOutLine_NONAME4	;
	if( 	m_pCheckBox_maximization	 )  	delete 	m_pCheckBox_maximization	;


	if(m_pEffVolumeButton[4]) delete m_pEffVolumeButton[4] ;
	if(m_pBGMVolumeButton[4]) delete  m_pBGMVolumeButton[4] ;

	if(m_pEffVolumeButton[3]) delete m_pEffVolumeButton[3] ;
	if(m_pBGMVolumeButton[3]) delete  m_pBGMVolumeButton[3] ;

	if(m_pEffVolumeButton[2]) delete m_pEffVolumeButton[2] ;
	if(m_pBGMVolumeButton[2]) delete  m_pBGMVolumeButton[2] ;

	if(m_pEffVolumeButton[1]) delete m_pEffVolumeButton[1] ;
	if(m_pBGMVolumeButton[1]) delete  m_pBGMVolumeButton[1] ;

	if(m_pEffVolumeButton[0]) delete m_pEffVolumeButton[0] ;
	if(m_pBGMVolumeButton[0]) delete  m_pBGMVolumeButton[0] ;
	
	SAFE_DELETE(m_pSoundMinusButton );
	SAFE_DELETE(m_pSoundPlusButton );
	SAFE_DELETE(m_pBGMMinusButton );
	SAFE_DELETE(m_pBGMPlusButton );

	SAFE_DELETE(m_pStaticShowCountry );
	SAFE_DELETE(m_pRadioShowCountryOn);
	SAFE_DELETE(m_pRadioShowCountryOff);

	SAFE_DELETE(m_pStaticLanguage);
	SAFE_DELETE(m_pComboLanguage);

	SAFE_DELETE(m_pStaticFps);	
	SAFE_DELETE(m_pComboFps);

	SAFE_DELETE( m_pShowOrganCheckBox );
	SAFE_DELETE( m_pMyKeyCheckBox );
	SAFE_DELETE( m_pGameTipCheckBox );
	SAFE_DELETE( m_pHelperCheckBox );
	SAFE_DELETE( m_pPVPModeCheckBox );
}

void CNSoundOnOffDlg::Create( )
{
 	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR* pText = GetTxtFromMgr(3356);

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NSoundOnOff.ddf"));
	file.CreatePopupDialog(this, NSOUNDONOFF_DLG, TEXT("dialog_soundonoff"), StaticNSoundOnOffDlgProc);

	m_pSettingStatic = new CStatic( this );          
	m_pEffect_BackgroundStatic = new CStatic( this );
	m_pGameSettingStatic = new CStatic( this );      
	m_pCaptionInfoStatic = new CStatic( this );      
	m_pTotalSpendTimeStatic = new CStatic( this );   
	
	m_pShowCharNameCheckBox				= new CCheckBox( this );
	m_pShowPuchikomiCheckBox			= new CCheckBox( this );
	m_pBackgroundCheckBox				= new CCheckBox( this );              
	m_pEffectCheckBox					= new CCheckBox( this );                  
	//m_pWindowmodeCheckBox				= new CCheckBox( this );                
	m_pMinimapRejectCheckBox			= new CCheckBox( this );           
	m_pNotifyFriendAddRejectCheckBox	= new CCheckBox( this );   
	m_pNotifyHeadChatCheckBox			= new CCheckBox( this );          
	m_pNotifyJewelItemRejectCheckBox	= new CCheckBox( this );   
	m_pNotifyMiniGameCheckBox			= new CCheckBox( this );
	m_pNotifySummonEvolveRejectCheckBox = new CCheckBox( this );
	m_pPartyInviteRejectCheckBox		= new CCheckBox( this );       
	m_pPrivateTradeRejectCheckBox		= new CCheckBox( this );      
	m_pRightMouseMenuAllowCheckBox		= new CCheckBox( this );     
	m_pWhisperChatRejectCheckBox		= new CCheckBox( this );       
	m_pChatFixedCheckBox				= new CCheckBox( this );       
	m_pChatTabFixedCheckBox				= new CCheckBox( this );       
	m_pTotalSpendTimeEdit				= new CEdit( this );
	m_pSelectCaptionKindCombobox		= new CComboBox( this );       
						

	m_pOutLine_NONAME1	 = new 	COutLine	( this )	;
	m_pOutLine_NONAME2	 = new 	COutLine	( this )	;
	m_pOutLine_NONAME4	 = new 	COutLine	( this )	;
	m_pCheckBox_maximization	 = new 	CCheckBox	( this )	;
	
	m_pSoundMinusButton = new CButton( this );
	m_pSoundPlusButton = new CButton( this );
	m_pBGMMinusButton = new CButton( this );
	m_pBGMPlusButton = new CButton( this );

	for( SI32 i = 0; i < 5; i++ )
	{
		m_pEffVolumeButton[i] = new CImageStatic ( this )	;
		m_pBGMVolumeButton[i] = new CImageStatic ( this )	;
	}

	m_pStaticShowCountry = new CStatic( this );
	m_pRadioShowCountryOn = new CRadioButton( this );
	m_pRadioShowCountryOff = new CRadioButton( this );

	NEWCSTATIC( m_pStaticLanguage );
	NEWCCOMBOBOX( m_pComboLanguage );

	NEWCSTATIC( m_pStaticFps );
	NEWCCOMBOBOX( m_pComboFps );

	NEWCCHECKBOX( m_pCheckBoxAloneDraw );

	NEWCCHECKBOX( m_pShowOrganCheckBox );
	NEWCCHECKBOX( m_pMyKeyCheckBox );		// [진성] 나만의 메뉴.	=> 2008-9-3
	NEWCCHECKBOX( m_pGameTipCheckBox );		// [진성] 게임 팁.		=> 2008-10-6
	NEWCCHECKBOX( m_pHelperCheckBox );		// [진성] 도우미.		=> 2008-10-7

	NEWCCHECKBOX( m_pPVPModeCheckBox );		// [성웅] pvp 모드 

	file.CreateControl(m_pSoundMinusButton ,		NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME_MINUS ,	TEXT("button_soundminus")  );
	file.CreateControl(m_pSoundPlusButton ,			NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME_PLUS ,		TEXT("button_soundplus")  );
	file.CreateControl(m_pBGMMinusButton ,			NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME_MINUS ,	TEXT("button_bgmminus")  );
	file.CreateControl(m_pBGMPlusButton ,			NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME_PLUS ,		TEXT("button_bgmplus")  );

	file.CreateControl(m_pBGMVolumeButton[0]	,	NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME1 , 		TEXT("button_bgm1")	)	;
	file.CreateControl(m_pBGMVolumeButton[1]	,	NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME2 , 		TEXT("button_bgm2")	)	;
	file.CreateControl(m_pBGMVolumeButton[2]	,	NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME3 , 		TEXT("button_bgm3")	)	;
	file.CreateControl(m_pBGMVolumeButton[3]	,	NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME4 , 		TEXT("button_bgm4")	)	;
	file.CreateControl(m_pBGMVolumeButton[4]	,	NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME5 , 		TEXT("button_bgm5")	)	;

	file.CreateControl(m_pEffVolumeButton[0]	,	NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME1 , 		TEXT("button_eff1")	)	;
	file.CreateControl(m_pEffVolumeButton[1]	,	NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME2 , 		TEXT("button_eff2")	)	;
	file.CreateControl(m_pEffVolumeButton[2]	,	NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME3 , 		TEXT("button_eff3")	)	;
	file.CreateControl(m_pEffVolumeButton[3]	,	NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME4 , 		TEXT("button_eff4")	)	;
	file.CreateControl(m_pEffVolumeButton[4]	,	NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME5 , 		TEXT("button_eff5")	)	;


	file.CreateControl(m_pSettingStatic,          NSOUNDONOFF_DIALOG_SETTINGSTATIC,           TEXT("statictext_setting"));
	file.CreateControl(m_pEffect_BackgroundStatic,NSOUNDONOFF_DIALOG_EFFECT_BACKGROUNDSTATIC, TEXT("statictext_effect_background"));
	file.CreateControl(m_pGameSettingStatic,      NSOUNDONOFF_DIALOG_GAMESETTINGSTATIC,       TEXT("statictext_gamesetting"));
	file.CreateControl(m_pCaptionInfoStatic,      NSOUNDONOFF_DIALOG_CAPTIONINFOSTATIC,       TEXT("statictext_captioninfo"));
	file.CreateControl(m_pTotalSpendTimeStatic,   NSOUNDONOFF_DIALOG_TOTALSPENDTIMESTATIC,    TEXT("statictext_totalspendtime"));

	file.CreateControl(m_pBackgroundCheckBox,              	NSOUNDONOFF_DIALOG_BACKGROUNDCHECKBOX,              	TEXT("checkbox_background"));
	file.CreateControl(m_pEffectCheckBox,                  	NSOUNDONOFF_DIALOG_EFFECTCHECKBOX,                  	TEXT("checkbox_effect"));
	//file.CreateControl(m_pWindowmodeCheckBox,            	NSOUNDONOFF_DIALOG_WINDOWMODECHECKBOX,			   		TEXT("checkbox_fullmode"));
	file.CreateControl(m_pMinimapRejectCheckBox,           	NSOUNDONOFF_DIALOG_MINIMAPREJECTCHECKBOX,           	TEXT("checkbox_minimap_reject"));
	file.CreateControl(m_pNotifyFriendAddRejectCheckBox,   	NSOUNDONOFF_DIALOG_NOTIFYFRIENDADDREJECTCHECKBOX,   	TEXT("checkbox_notify_friendadd"));
	file.CreateControl(m_pNotifyHeadChatCheckBox,          	NSOUNDONOFF_DIALOG_NOTIFYHEADCHATCHECKBOX,          	TEXT("checkbox_notify_headchat"));
	file.CreateControl(m_pNotifyJewelItemRejectCheckBox,   	NSOUNDONOFF_DIALOG_NOTIFYJEWELITEMREJECTCHECKBOX,   	TEXT("checkbox_notify_jewelitem"));
	file.CreateControl(m_pNotifyMiniGameCheckBox,          	NSOUNDONOFF_DIALOG_NOTIFYMINIGAMECHECKBOX,          	TEXT("checkbox_notify_minigame"));
	file.CreateControl(m_pNotifySummonEvolveRejectCheckBox,	NSOUNDONOFF_DIALOG_NOTIFYSUMMONEVOLVEREJECTCHECKBOX,	TEXT("checkbox_notify_summonevolve"));
	file.CreateControl(m_pPartyInviteRejectCheckBox,       	NSOUNDONOFF_DIALOG_PARTYINVITEREJECTCHECKBOX,       	TEXT("checkbox_partyinvite_reject"));
	file.CreateControl(m_pPrivateTradeRejectCheckBox,      	NSOUNDONOFF_DIALOG_PRIVATETRADEREJECTCHECKBOX,      	TEXT("checkbox_privatetrade_reject"));
	file.CreateControl(m_pRightMouseMenuAllowCheckBox,     	NSOUNDONOFF_DIALOG_RIGHTMOUSEMENUALLOWCHECKBOX,     	TEXT("checkbox_right_mouse_menu_allow"));
	file.CreateControl(m_pWhisperChatRejectCheckBox,       	NSOUNDONOFF_DIALOG_WHISPERCHATREJECTCHECKBOX,       	TEXT("checkbox_whisperchat_reject"));
	file.CreateControl(m_pTotalSpendTimeEdit,              	NSOUNDONOFF_DIALOG_TOTALSPENDTIMEEDIT,              	TEXT("editbox_total_spend_time"));
	file.CreateControl(m_pSelectCaptionKindCombobox,       	NSOUNDONOFF_DIALOG_SELECTCAPTIONKINDCOMBOBOX,       	TEXT("combobox_selectcaptionkind"));
	file.CreateControl(m_pCheckBox_maximization	, 		   	NSOUNDONOFF_DIALOG_CHECKBOX_MAXIMIZATION	,			TEXT("checkbox_maximization") )	;
	file.CreateControl( m_pShowPuchikomiCheckBox,			NSOUNDONOFF_DIALOG_CHECKBOX_PUCHIKOMI, 					TEXT("checkbox_puchikomi"));
	file.CreateControl( m_pCheckBoxAloneDraw,				NSOUNDONOFF_DIALOG_CHECKBOX_ALONEDRAW, 					TEXT("checkbox_alonedraw"));
	file.CreateControl( m_pShowCharNameCheckBox,			NSOUNDONOFF_DIALOG_CHECKBOX_SHOWCHARNAME ,				TEXT("checkbox_showcharname"));
	file.CreateControl(m_pChatFixedCheckBox,				NSOUNDONOFF_DIALOG_CHATFIXEDCHECKBOX,					TEXT("checkbox_chatfixed"));

	//KHY - 0110 - 채팅 모드 고정
	file.CreateControl( m_pChatTabFixedCheckBox,			NSOUNDONOFF_DIALOG_CHATTABFIXEDCHECKBOX,				TEXT("checkbox_chattabfixed"));

	// [진성] 나만의 메뉴.	창 생성 => 2008-9-3
	file.CreateControl( m_pMyKeyCheckBox,					NSOUNDONOFF_DIALOG_CHECKBOX_MYKEY,						TEXT("checkbox_mykey"));
	file.CreateControl( m_pGameTipCheckBox,					NSOUNDONOFF_DIALOG_CHECKBOX_GAMETIP,					TEXT("checkbox_gametip"));
	file.CreateControl( m_pHelperCheckBox,					NSOUNDONOFF_DIALOG_CHECKBOX_HELPER,						TEXT("checkbox_helper"));

	// [ 성웅 ] pvp 거부 옵션 
	file.CreateControl( m_pPVPModeCheckBox,					NSOUNDONOFF_DIALOG_CHECKBOX_PVPMODE,						TEXT("checkbox_bPVPmode"));

	// 신체기관 보여주기 여부
	file.CreateControl( m_pShowOrganCheckBox,				NSOUNDONOFF_DIALOG_CHECKBOX_ORGANSHOW,					TEXT("checkbox_organshow") );

	file.CreateControl( m_pOutLine_NONAME1,					NSOUNDONOFF_DIALOG_OUTLINE_NONAME1,						TEXT("outline_NONAME1") )	;
	file.CreateControl( m_pOutLine_NONAME2,					NSOUNDONOFF_DIALOG_OUTLINE_NONAME2,						TEXT("outline_NONAME2") )	;
	file.CreateControl( m_pOutLine_NONAME4,					NSOUNDONOFF_DIALOG_OUTLINE_NONAME4,						TEXT("outline_NONAME4") )	;
	
	file.CreateControl( m_pStaticShowCountry,				NSOUNDONOFF_DIALOG_STATIC_SHOWCOUNTRY,					TEXT("statictext_showcountry" ) );
	file.CreateControl( m_pRadioShowCountryOn,				NSOUNDONOFF_DIALOG_RADIO_SHOWCOUNTRYON,					TEXT("radiobutton_showcountryon" ) );
	file.CreateControl( m_pRadioShowCountryOff,				NSOUNDONOFF_DIALOG_RADIO_SHOWCOUNTRYOFF,				TEXT("radiobutton_showcounrtyoff" ) );

	file.CreateControl( m_pStaticLanguage,					NSOUNDONOFF_DIALOG_STATIC_LANGUAGE,						TEXT("statictext_language") );
	file.CreateControl( m_pComboLanguage,					NSOUNDONOFF_DIALOG_COMBO_LANGUAGE,						TEXT("combobox_language") );
	file.CreateControl( m_pStaticFps,						NSOUNDONOFF_DIALOG_STATIC_FPS,							TEXT("statictext_fps") );
	file.CreateControl( m_pComboFps,						NSOUNDONOFF_DIALOG_COMBO_FPS,							TEXT("combobox_fps") );

	

	
	//m_pChatFixedCheckBox->Enable(false);
	m_pChatFixedCheckBox->SetTextPrintOption( DT_LEFT );
	m_pChatTabFixedCheckBox->SetTextPrintOption( DT_LEFT );	
	m_pBackgroundCheckBox->SetTextPrintOption( DT_LEFT );              
	m_pEffectCheckBox->SetTextPrintOption( DT_LEFT );                  
	//m_pWindowmodeCheckBox->SetTextPrintOption( DT_LEFT );                
	m_pMinimapRejectCheckBox->SetTextPrintOption( DT_LEFT );           
	m_pNotifyFriendAddRejectCheckBox->SetTextPrintOption( DT_LEFT );   
	m_pNotifyHeadChatCheckBox->SetTextPrintOption( DT_LEFT );          
	m_pNotifyJewelItemRejectCheckBox->SetTextPrintOption( DT_LEFT );   
	m_pNotifyMiniGameCheckBox->SetTextPrintOption( DT_LEFT );
	m_pNotifySummonEvolveRejectCheckBox->SetTextPrintOption( DT_LEFT );
	m_pPartyInviteRejectCheckBox->SetTextPrintOption( DT_LEFT );       
	m_pPrivateTradeRejectCheckBox->SetTextPrintOption( DT_LEFT );      
	m_pRightMouseMenuAllowCheckBox->SetTextPrintOption( DT_LEFT );     
	m_pWhisperChatRejectCheckBox->SetTextPrintOption( DT_LEFT );       
	m_pSelectCaptionKindCombobox->SetTextPrintOption( DT_LEFT );       
	m_pTotalSpendTimeEdit->SetTextPrintOption( DT_LEFT );              
	m_pChatFixedCheckBox->SetTextPrintOption( DT_LEFT );
	m_pChatTabFixedCheckBox->SetTextPrintOption( DT_LEFT );
	m_pStaticFps->SetTextPrintOption( DT_LEFT );
	m_pSettingStatic->SetTextPrintOption( DT_LEFT );          
	m_pEffect_BackgroundStatic->SetTextPrintOption( DT_LEFT );
	m_pGameSettingStatic->SetTextPrintOption( DT_LEFT );      
	m_pCaptionInfoStatic->SetTextPrintOption( DT_LEFT );      
	m_pTotalSpendTimeStatic->SetTextPrintOption( DT_LEFT );   

	m_pCheckBox_maximization->SetTextPrintOption( DT_LEFT ); 
	m_pRadioShowCountryOn->SetTextPrintOption( DT_LEFT );
	m_pRadioShowCountryOff->SetTextPrintOption( DT_LEFT );
	
	
	m_pShowPuchikomiCheckBox->SetTextPrintOption(DT_LEFT );

	if( g_siChannelingCode == CHANNELINGCODE_HANGAMEJP )
	{
		m_pShowPuchikomiCheckBox->Enable(false);
	}

	m_pShowCharNameCheckBox->SetTextPrintOption(DT_LEFT);

	m_pShowOrganCheckBox->SetTextPrintOption( DT_LEFT );

	m_pMyKeyCheckBox->SetTextPrintOption( DT_LEFT );
	m_pGameTipCheckBox->SetTextPrintOption( DT_LEFT );
	m_pHelperCheckBox->SetTextPrintOption( DT_LEFT );

	m_pPVPModeCheckBox->SetTextPrintOption( DT_LEFT );
	

	m_pShowCharNameCheckBox->Refresh();
	m_pShowPuchikomiCheckBox->Refresh();
	m_pBackgroundCheckBox->Refresh();              
	m_pEffectCheckBox->Refresh();                  
	//m_pWindowmodeCheckBox->Refresh();                
	m_pMinimapRejectCheckBox->Refresh();           
	m_pNotifyFriendAddRejectCheckBox->Refresh();   
	m_pNotifyHeadChatCheckBox->Refresh();          
	m_pNotifyJewelItemRejectCheckBox->Refresh();   
	m_pNotifyMiniGameCheckBox->Refresh();
	m_pNotifySummonEvolveRejectCheckBox->Refresh();
	m_pPartyInviteRejectCheckBox->Refresh();       
	m_pPrivateTradeRejectCheckBox->Refresh();      
	m_pRightMouseMenuAllowCheckBox->Refresh();     
	m_pWhisperChatRejectCheckBox->Refresh();       
	m_pSelectCaptionKindCombobox->Refresh();       
	m_pTotalSpendTimeEdit->Refresh();

	m_pSettingStatic->Refresh();          
	m_pEffect_BackgroundStatic->Refresh();
	m_pGameSettingStatic->Refresh();      
	m_pCaptionInfoStatic->Refresh();      
	m_pTotalSpendTimeStatic->Refresh();   

	m_pCheckBox_maximization->Refresh(); 
	m_pChatFixedCheckBox->Refresh();
	m_pChatTabFixedCheckBox->Refresh();
	m_pStaticFps->Refresh();

	m_pShowOrganCheckBox->Refresh();
	m_pMyKeyCheckBox->Refresh();
	m_pGameTipCheckBox->Refresh();
	m_pHelperCheckBox->Refresh();
	m_pPVPModeCheckBox->Refresh();
	//KHY - 0625 - 군주S 국가 관련 삭제.
	if( pclClient->siServiceArea != ConstServiceArea_Korea)
	{
		m_pRadioShowCountryOn->Refresh();
		m_pRadioShowCountryOff->Refresh();
	}
	//if( pclClient->siServiceArea != ConstServiceArea_Korea )
	//	m_pWindowmodeCheckBox->Show(FALSE);

	// 푸치코미 옵션은 일본에만 보여준다.
	//KHY - 0220 -  푸치코미 옵션 삭제.
	if(!pclClient->IsCountrySwitch(Switch_Puchikomi))
	{
		m_pShowPuchikomiCheckBox->Enable(false);
        m_pShowPuchikomiCheckBox->Show(false);
	}

	// 캐릭터 혼자 그려주기는 중국만 보여준다.
	if( ConstServiceArea_China != pclClient->siServiceArea )
	{
		m_pCheckBoxAloneDraw->Enable(false);
		m_pCheckBoxAloneDraw->Show(false);
	}

	//KHY - 0625 - 군주S 국가 관련 삭제.
	if( pclClient->siServiceArea == ConstServiceArea_Korea)
	{
		m_pStaticShowCountry->Show(false);
		m_pRadioShowCountryOn->Show(false);
		m_pRadioShowCountryOff->Show(false);
	}
	
	// [진성] 나만의 메뉴. show()	=> 2008-9-3
	if( !pclClient->IsCountrySwitch( Switch_Mykey ) )	
	{
		m_pMyKeyCheckBox->Show( false );
		m_pMyKeyCheckBox->Enable( false );
	}

	if( !pclClient->IsCountrySwitch( Switch_GameTip ) )	
	{
		m_pGameTipCheckBox->Show( false );
		m_pGameTipCheckBox->Enable( false );
	}
	
	if( !pclClient->IsCountrySwitch( Switch_Helper ) )	
	{
		m_pHelperCheckBox->Show( false );
		m_pHelperCheckBox->Enable( false );
	}


	// 자막광고 콤보 셋팅!
	stComboBoxItemData tmpComboBoxItemData;

	tmpComboBoxItemData.Init();
	pText = GetTxtFromMgr(2387);
	tmpComboBoxItemData.Set( pText );
	m_pSelectCaptionKindCombobox->AddItem( &tmpComboBoxItemData );

	tmpComboBoxItemData.Init();
	pText = GetTxtFromMgr(2388);
	tmpComboBoxItemData.Set( pText );
	m_pSelectCaptionKindCombobox->AddItem( &tmpComboBoxItemData );

	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set("English");
	m_pComboLanguage->AddItem( &tmpComboBoxItemData );

	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set("Chinese");
	m_pComboLanguage->AddItem( &tmpComboBoxItemData );

	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set("20 fps");
	m_pComboFps->AddItem( &tmpComboBoxItemData );

	tmpComboBoxItemData.Init();
	tmpComboBoxItemData.Set("40 fps");
	m_pComboFps->AddItem( &tmpComboBoxItemData );

	if( pclClient->siServiceArea == ConstServiceArea_English
		|| pclClient->siServiceArea == ConstServiceArea_NHNChina
		|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		LoadLanguageConfig();
	}
	LoadFpsConfig();
	LoadSoundConfig();

	SetSoundButton(m_siSelectedSoundButton);
	SetBGMButton(m_siSelectedBGMButton);

	Hide();
    
	return;
}

void CALLBACK CNSoundOnOffDlg::StaticNSoundOnOffDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNSoundOnOffDlg *pThis = (CNSoundOnOffDlg*) pControllerMgr;
	pThis->NSoundOnOffDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNSoundOnOffDlg::NSoundOnOffDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	bool bTmp = false;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME_MINUS:
		{
			if( EVENT_BUTTON_CLICKED == nEvent )
			{
				if( m_siSelectedSoundButton > 0)
				{
					SetSoundButton(--m_siSelectedSoundButton);
					WriteSoundConfig();
				}
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME_PLUS:
		{
			if( EVENT_BUTTON_CLICKED == nEvent )
			{
				if( m_siSelectedSoundButton < 4)
				{
					SetSoundButton(++m_siSelectedSoundButton);
					WriteSoundConfig();
				}
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME_MINUS:
		{
			if( EVENT_BUTTON_CLICKED == nEvent )
			{
				if( m_siSelectedBGMButton > 0)
				{
					SetBGMButton(--m_siSelectedBGMButton);
					WriteSoundConfig();
				}
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME_PLUS:
		{
			if( EVENT_BUTTON_CLICKED == nEvent )
			{
				if( m_siSelectedBGMButton < 4)
				{
					SetBGMButton(++m_siSelectedBGMButton);
					WriteSoundConfig();
				}
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME1:
		{ 		
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetSoundButton(0);   	
				WriteSoundConfig();
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME2:
		{		
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetSoundButton(1);	
				WriteSoundConfig();
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME3:
		{	
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetSoundButton(2);		
				WriteSoundConfig();
			}			
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME4:
		{	
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetSoundButton(3);	
				WriteSoundConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_EFF_VOLUME5:
		{ 	
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetSoundButton(4); 	
				WriteSoundConfig();
			}
		}
		break;		
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME1:
		{  		
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetBGMButton(0);
				WriteSoundConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME2:
		{		
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetBGMButton(1);
				WriteSoundConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME3:
		{	
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetBGMButton(2);
				WriteSoundConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME4:
		{	
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetBGMButton(3);	
				WriteSoundConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_BUTTON_BGM_VOLUME5:
		{		
			if(  EVENT_IMAGESTATIC_LBUTTONDOWN == nEvent )
			{
				SetBGMButton(4);				
				WriteSoundConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_CHATFIXEDCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pChatFixedCheckBox->IsCheck();

					m_bChatFixed = bTmp;
					g_SoundOnOffDlg.m_bChatFixed = bTmp;

					pclclient->pclMusic->pclSoundManager->EffectSoundOn(bTmp);

					WriteSoundConfig();
				}
				break;
			}
		}
		break;
	//KHY - 0110 - 채팅 모드 고정
	case NSOUNDONOFF_DIALOG_CHATTABFIXEDCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pChatTabFixedCheckBox->IsCheck();

					m_bChatTabFixed = bTmp;
					g_SoundOnOffDlg.m_bChatTabFixed = bTmp;

					if(bTmp == true)
					{
						if(pclclient->m_pDialog[ NCHATBAR_DLG ] != NULL)
						{
							g_SoundOnOffDlg.m_siChatTabNowIndex = (( CNChatBarDlg *)pclclient->m_pDialog[ NCHATBAR_DLG ] )->GetChatTabIndex();

							pclclient->pclMusic->pclSoundManager->EffectSoundOn(bTmp);

							WriteSoundConfig();
						}
					}
				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_NOTIFYMINIGAMECHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pNotifyMiniGameCheckBox->IsCheck();

					if( bTmp != m_bNotifyMinigame )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pNotifyMiniGameCheckBox->SetCheck( !bTmp );

						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						m_bNotifyMinigame = bTmp;
						g_SoundOnOffDlg.m_bNotifyMinigame = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_NOTIFYHEADCHATCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pNotifyHeadChatCheckBox->IsCheck();

					if ( bTmp != m_bNotifyHeadChat )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pNotifyHeadChatCheckBox->SetCheck( !bTmp );

						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						m_bNotifyHeadChat = bTmp;
						g_SoundOnOffDlg.m_bNotifyHeadChat = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;

	case NSOUNDONOFF_DIALOG_EFFECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pEffectCheckBox->IsCheck();

					m_bEffectSound = bTmp;
					g_SoundOnOffDlg.m_bEffectSound = bTmp;

					pclclient->pclMusic->pclSoundManager->EffectSoundOn(bTmp);

					WriteSoundConfig();
				}
				break;
			}
		}
		break;

	case NSOUNDONOFF_DIALOG_BACKGROUNDCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pBackgroundCheckBox->IsCheck();

					if ( bTmp != m_bBackGroudnSound )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pBackgroundCheckBox->SetCheck( !bTmp );
						//
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}
						m_bBackGroudnSound = bTmp;
						g_SoundOnOffDlg.m_bBackGroudnSound = bTmp;

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(bTmp);

							if(bTmp)
								pclclient->PlayBackgroundMusic( pclClient->pclCM->CR[1]->GetMapIndex() );
						}
					}

					WriteSoundConfig();
				}
				break;				
			}
		}
		break;

	case NSOUNDONOFF_DIALOG_MINIMAPREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
                    bTmp = m_pMinimapRejectCheckBox->IsCheck();
					m_bMiniMapShow = bTmp;
					g_SoundOnOffDlg.m_bMiniMapShow = bTmp;

					WriteSoundConfig();
				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_WHISPERCHATREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pWhisperChatRejectCheckBox->IsCheck();
					if ( bTmp != m_bWhisperChatReject )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pWhisperChatRejectCheckBox->SetCheck( !bTmp );

						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_WhisperChat_Reject clinfo( bTmp );
							cltMsg clMsg(GAMEMSG_REQUEST_WHISPERCHAT_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
						
						m_bWhisperChatReject = bTmp;
						g_SoundOnOffDlg.m_bWhisperChatReject = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;

	case NSOUNDONOFF_DIALOG_PRIVATETRADEREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pPrivateTradeRejectCheckBox->IsCheck();
					if ( bTmp != m_bPrivateTradeReject )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
      //                      m_pPrivateTradeRejectCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_PrivateTrade_Reject clinfo( bTmp );
							cltMsg clMsg(GAMEMSG_REQUEST_PRIVATETRADE_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}

						m_bPrivateTradeReject = bTmp;
						g_SoundOnOffDlg.m_bPrivateTradeReject = bTmp;

					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;	
	case NSOUNDONOFF_DIALOG_NOTIFYJEWELITEMREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pNotifyJewelItemRejectCheckBox->IsCheck();
					if ( bTmp != m_bJewelItemNotifyReject )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pNotifyJewelItemRejectCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}
						
						m_bJewelItemNotifyReject = bTmp;
						g_SoundOnOffDlg.m_bJewelItemNotifyReject = bTmp;
					}


					WriteSoundConfig();

				}
				break;
			}
		}
		break;

	
	case NSOUNDONOFF_DIALOG_NOTIFYSUMMONEVOLVEREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pNotifySummonEvolveRejectCheckBox->IsCheck();
					if ( bTmp != m_bSummonEvolveNotifyReject )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pNotifySummonEvolveRejectCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						m_bSummonEvolveNotifyReject = bTmp;
						g_SoundOnOffDlg.m_bSummonEvolveNotifyReject = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;
			
	case NSOUNDONOFF_DIALOG_PARTYINVITEREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pPartyInviteRejectCheckBox->IsCheck();
					if ( bTmp != m_bPartyInviteNotifyReject )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pPartyInviteRejectCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}
						
						m_bPartyInviteNotifyReject = bTmp;
						g_SoundOnOffDlg.m_bPartyInviteNotifyReject = bTmp;
						
						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_PartyInvite_Reject clinfo( bTmp );
							cltMsg clMsg(GAMEMSG_REQUEST_PARTYINVITE_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;
			
	case NSOUNDONOFF_DIALOG_NOTIFYFRIENDADDREJECTCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pNotifyFriendAddRejectCheckBox->IsCheck();
					if ( bTmp != m_bFriendAddNotifyReject )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
      //                      m_pNotifyFriendAddRejectCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}
						
						m_bFriendAddNotifyReject = bTmp;
						g_SoundOnOffDlg.m_bFriendAddNotifyReject = bTmp;

						if ( pclClient->GetGameStatus() == GS_GAME )
						{
							cltGameMsgRequest_FriendAdd_Reject clinfo( bTmp );
							cltMsg clMsg(GAMEMSG_REQUEST_FRIENDADD_REJECT, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}

					WriteSoundConfig();

				}
				break;			
			}
		}
		break;
			
	case NSOUNDONOFF_DIALOG_RIGHTMOUSEMENUALLOWCHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pRightMouseMenuAllowCheckBox->IsCheck();
					if ( bTmp != m_bRightMouseMentAllow )
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pRightMouseMenuAllowCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						m_bRightMouseMentAllow = bTmp;
						g_SoundOnOffDlg.m_bRightMouseMentAllow = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;


	case NSOUNDONOFF_DIALOG_CHECKBOX_SHOWCHARNAME:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pShowCharNameCheckBox->IsCheck();
					if ( bTmp != m_bShowCharName )
					{
					
						m_bShowCharName = bTmp;
						g_SoundOnOffDlg.m_bShowCharName = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;

	case NSOUNDONOFF_DIALOG_CHECKBOX_ALONEDRAW:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pCheckBoxAloneDraw->IsCheck();
					if ( bTmp != m_bAloneDraw )
					{

						m_bAloneDraw = bTmp;
						g_SoundOnOffDlg.m_bAloneDraw = bTmp;
					}

					WriteSoundConfig();

				}
				break;
			}
		}
		break;
			
	/*
	case NSOUNDONOFF_DIALOG_WINDOWMODECHECKBOX:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					// bTmp : 윈도우창 사용이므로 m_bTmpFullScreenMode와 개념이 반대임
					bTmp = m_pWindowmodeCheckBox->IsCheck();
					if ( bTmp == m_bTmpFullScreenMode)
					{
						//if ( pclClient->GetGameStatus() != GS_GAME )
						//{
						//	m_pWindowmodeCheckBox->SetCheck( !bTmp );
						//	TCHAR* pTitle = GetTxtFromMgr(2389);
						//	TCHAR* pText = GetTxtFromMgr(2390);
						//	g_ErrorMsg.Set( false, pTitle, pText);
						//	return ;
						//}

						//m_bFullScreenMode = bTmp;
						m_bTmpFullScreenMode = !bTmp;
						TCHAR* pTitle = GetTxtFromMgr(5162);
						TCHAR* pText = GetTxtFromMgr(2392);
						g_ErrorMsg.Set( false, pTitle, pText);
						g_SoundOnOffDlg.m_bTmpFullScreenMode = !bTmp;
						//g_SoundOnOffDlg.m_bFullScreenMode = bTmp; => 게임중에는 적용되지 않는다
					}
					WriteSoundConfig();
				}
				break;
			}
		}
		break;
		*/
	case NSOUNDONOFF_DIALOG_CHECKBOX_MAXIMIZATION:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					bTmp = m_pCheckBox_maximization->IsCheck();
					if ( bTmp != m_bTmpMaxWindowMode )
					{
						m_bTmpMaxWindowMode = bTmp;
						g_SoundOnOffDlg.m_bTmpMaxWindowMode = bTmp;

						if( bTmp == false )
						{
                            //m_pWindowmodeCheckBox->Enable( true );
							
							//if( m_pWindowmodeCheckBox->IsCheck() == false )
								//m_pWindowmodeCheckBox->SetCheck( true ); // <= 자동으로 윈도우 모드로 만들지 않는다.

							//if( m_pWindowmodeCheckBox->IsCheck() == true )
							//{
								TCHAR* pTitle = GetTxtFromMgr(5162);
								TCHAR* pText = GetTxtFromMgr(2392);
								g_ErrorMsg.Set( false, pTitle, pText);
							//}
						}
						else
						{
							//m_pWindowmodeCheckBox->Enable( false );
							//if( m_pWindowmodeCheckBox->IsCheck() == true )
								//m_pWindowmodeCheckBox->SetCheck( false );

							//if( m_pWindowmodeCheckBox->IsCheck() == false )
							//{
								TCHAR* pTitle = GetTxtFromMgr(5162);
								TCHAR* pText = GetTxtFromMgr(2392);
								g_ErrorMsg.Set( false, pTitle, pText);
							//}

						}
					}

					WriteSoundConfig();
					SetGameModeInFile( pclClient->GameMode, gsiResolution, m_bTmpMaxWindowMode, pclClient->siRunMode );
					SetFullScreenInFile(m_bTmpMaxWindowMode);	// FullScreen.txt에 저장
				}
				break;
			}
		}
		break;


	case NSOUNDONOFF_DIALOG_CHECKBOX_PUCHIKOMI:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					cltGameMsgRequest_SetShowPuchikomi pclinfo(m_pShowPuchikomiCheckBox->IsCheck());
					cltMsg clMsg(GAMEMSG_REQUEST_SETSHOWPUCHIKOMI , sizeof(pclinfo), (BYTE*)&pclinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;


	case NSOUNDONOFF_DIALOG_SELECTCAPTIONKINDCOMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					//if ( pclClient->GetGameStatus() != GS_GAME )
					//{
					//	TCHAR* pTitle = GetTxtFromMgr(2389);
					//	TCHAR* pText = GetTxtFromMgr(2390);
					//	g_ErrorMsg.Set( false, pTitle, pText);
					//	return ;
					//}

					SI32 SelectedIndex = m_pSelectCaptionKindCombobox->GetSelectedIndex();

					if ( SelectedIndex == -1 )
					{
						return ;
					}

					m_siSelectCaptionKindIdex = SelectedIndex;
					g_SoundOnOffDlg.m_siSelectCaptionKindIdex = SelectedIndex;
					
					if ( pclClient->GetGameStatus() == GS_GAME )
					{
						g_PromptMgr.DelAllPrompt();

						cltGameMsgRequest_CaptionKindChange clinfo( SelectedIndex );
						cltMsg clMsg(GAMEMSG_REQUEST_CAPTIONKINDCHANGE, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
					}


					WriteSoundConfig();

				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_RADIO_SHOWCOUNTRYON:
		{
			// 국가 정보 공개 여부 셋팅
			if( EVENT_RADIOBUTTON_CHANGED == nEvent )
			{
				// 국가 정보 셋팅
				WriteCountryConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_RADIO_SHOWCOUNTRYOFF:
		{
			// 국가 정보 공개 여부 셋팅
			if( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				// 국가 정보 셋팅
				WriteCountryConfig();
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_COMBO_LANGUAGE:
		{
			if( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				SI32 selectedindex = m_pComboLanguage->GetSelectedIndex();
				switch( selectedindex )
				{
				case Language_Chinese:
					pclClient->siLanguage = ConstLanguage_China;
					break;
				default:
					pclClient->siLanguage = ConstLanguage_English;
					break;
				}
				WriteLanguageConfig();

				if ( m_bIsLoadLanguage )
				{
					TCHAR* pText = GetTxtFromMgr( 6352 );
					TCHAR* pTitle = GetTxtFromMgr( 6353 );

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}
				else
				{
					m_bIsLoadLanguage = true;
				}
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_COMBO_FPS:
		{
			if ( EVENT_COMBOBOX_SELECTION_CHANGED == nEvent )
			{
				SI32 selectedindex = m_pComboFps->GetSelectedIndex();
				if ( selectedindex == 1 )
				{
					WriteFpsConfig( ConstFrameRate_High );
				}
				else
				{
					WriteFpsConfig( ConstFrameRate_Low );
				}

				if ( m_bIsLoadFps )
				{
					TCHAR* pText = GetTxtFromMgr( 6442 );
					TCHAR* pTitle = GetTxtFromMgr( 2045 );

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				}
				else
				{
					m_bIsLoadFps = true;
				}
				
			}
		}
		break;
	// 신체기관창 보여주기 여부
	case NSOUNDONOFF_DIALOG_CHECKBOX_ORGANSHOW:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					m_bOrganPannelShow = m_pShowOrganCheckBox->IsCheck();

					WriteSoundConfig();
				}
				break;
			}
		}
		break;
		// PVP거부 옵션 
	case NSOUNDONOFF_DIALOG_CHECKBOX_PVPMODE:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					m_bPVP = m_pPVPModeCheckBox->IsCheck()	;
					WriteSoundConfig();
				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_CHECKBOX_MYKEY:	// [진성] 나만의 메뉴.	=> 2008-9-3
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					// [진성] 나만의 메뉴. show()	=> 2008-9-3
					if( pclClient->IsCountrySwitch( Switch_Mykey ) )	
					{
						m_bMykey = m_pMyKeyCheckBox->IsCheck();

						if( pclclient->m_pDialog[NMYKEY] && pclclient->m_pDialog[NMYKEYMINI] )
						{
							if( true == m_bMykey )
							{
								(pclclient->m_pDialog[NMYKEY])->Show( false );
								(pclclient->m_pDialog[NMYKEYMINI])->Show( true ); 
							}
							else
							{
								(pclclient->m_pDialog[NMYKEY])->Show( false );
								(pclclient->m_pDialog[NMYKEYMINI])->Show( false ); 
							}
						}
						
						WriteSoundConfig();
					}
				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_CHECKBOX_GAMETIP:	// [진성] 게임 팁.	=> 2008-10-6
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					// [진성] 게임 팁	=> 2008-9-3
					if( pclClient->IsCountrySwitch( Switch_GameTip ) )	
					{
						m_bGameTip = m_pGameTipCheckBox->IsCheck();

						WriteSoundConfig();
					}
				}
				break;
			}
		}
		break;
	case NSOUNDONOFF_DIALOG_CHECKBOX_HELPER:	// [진성] 게임 도우미.	=> 2008-10-6
		{
			switch( nEvent )
			{
				case EVENT_CHECKBOX_CHANGED:
					{
						if( pclClient->IsCountrySwitch( Switch_Helper ) )	
						{				
							m_bHelper = m_pHelperCheckBox->IsCheck();
						
							WriteSoundConfig();
						}
					}
					break;
				}
		}
		break;
	}


	return;
}

bool CNSoundOnOffDlg::IsShow()
{
	return m_bShow;
}

void CNSoundOnOffDlg::Show()
{
	// 상태 저장 변수 셋팅
	m_bEffectSound = g_SoundOnOffDlg.m_bEffectSound;
	m_bBackGroudnSound = g_SoundOnOffDlg.m_bBackGroudnSound;
	m_bMiniMapShow = g_SoundOnOffDlg.m_bMiniMapShow;
	m_bFullScreenMode = g_SoundOnOffDlg.m_bFullScreenMode;
	m_bTmpFullScreenMode = g_SoundOnOffDlg.m_bTmpFullScreenMode; // 임시 풀스크린 모드 플래그

	m_bRightMouseMentAllow = g_SoundOnOffDlg.m_bRightMouseMentAllow;
	m_bWhisperChatReject = g_SoundOnOffDlg.m_bWhisperChatReject;
	m_bPrivateTradeReject = g_SoundOnOffDlg.m_bPrivateTradeReject;
	
	m_bJewelItemNotifyReject = g_SoundOnOffDlg.m_bJewelItemNotifyReject;

	m_bPartyInviteNotifyReject = g_SoundOnOffDlg.m_bPartyInviteNotifyReject;
	m_bFriendAddNotifyReject = g_SoundOnOffDlg.m_bFriendAddNotifyReject;

	m_bSummonEvolveNotifyReject = g_SoundOnOffDlg.m_bSummonEvolveNotifyReject;

	m_bNotifyHeadChat = g_SoundOnOffDlg.m_bNotifyHeadChat;
	m_bNotifyMinigame = g_SoundOnOffDlg.m_bNotifyMinigame;
	m_bChatFixed = g_SoundOnOffDlg.m_bChatFixed;

	m_siSelectCaptionKindIdex = g_SoundOnOffDlg.m_siSelectCaptionKindIdex ;

	m_bShowCharName			 = g_SoundOnOffDlg.m_bShowCharName ;
	//m_bFriendRegisterReject = g_SoundOnOffDlg.m_bFriendRegisterReject;

	//KHY - 0110 - 채팅 모드 고정
	m_bChatTabFixed = g_SoundOnOffDlg.m_bChatTabFixed;
	m_siChatTabNowIndex = g_SoundOnOffDlg.m_siChatTabNowIndex;
	
	// 컨트롤 상태 셋팅!!
	m_pEffectCheckBox->SetCheck( g_SoundOnOffDlg.m_bEffectSound );
	m_pBackgroundCheckBox->SetCheck( g_SoundOnOffDlg.m_bBackGroudnSound );
	m_pMinimapRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bMiniMapShow );
	//m_pWindowmodeCheckBox->SetCheck( !g_SoundOnOffDlg.m_bTmpFullScreenMode/*g_SoundOnOffDlg.m_bFullScreenMode*/ );
	m_pCheckBox_maximization->SetCheck( g_SoundOnOffDlg.m_bTmpMaxWindowMode ); // 최대창 크기 모드 사용

	m_pRightMouseMenuAllowCheckBox->SetCheck( g_SoundOnOffDlg.m_bRightMouseMentAllow );
	m_pWhisperChatRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bWhisperChatReject );
	m_pPrivateTradeRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bPrivateTradeReject );
	
	m_pNotifyJewelItemRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bJewelItemNotifyReject );

	m_pPartyInviteRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bPartyInviteNotifyReject );
	m_pNotifyFriendAddRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bFriendAddNotifyReject );

	m_pNotifySummonEvolveRejectCheckBox->SetCheck( g_SoundOnOffDlg.m_bSummonEvolveNotifyReject );
	
	m_pNotifyHeadChatCheckBox->SetCheck( g_SoundOnOffDlg.m_bNotifyHeadChat );
	m_pNotifyMiniGameCheckBox->SetCheck( g_SoundOnOffDlg.m_bNotifyMinigame );
	m_pChatFixedCheckBox->SetChange( g_SoundOnOffDlg.m_bChatFixed );

	//KHY - 0110 - 채팅 모드 고정
	m_pChatTabFixedCheckBox->SetChange( g_SoundOnOffDlg.m_bChatTabFixed );

	m_pShowCharNameCheckBox->SetChange( g_SoundOnOffDlg.m_bShowCharName );

	m_pCheckBoxAloneDraw->SetCheck( m_bAloneDraw );

	m_pShowOrganCheckBox->SetCheck( m_bOrganPannelShow );

	// [진성] 나만의 메뉴. show()	=> 2008-9-3
	if( !pclClient->IsCountrySwitch( Switch_Mykey ) )	
	{
		m_pMyKeyCheckBox->SetCheck( m_bMykey );
	}

	// [진성] 나만의 메뉴. show()	=> 2008-9-3
	if( !pclClient->IsCountrySwitch( Switch_GameTip ) )	
	{
		m_pGameTipCheckBox->SetCheck( m_bGameTip );
	}

	// [진성] 나만의 메뉴. show()	=> 2008-9-3
	if( !pclClient->IsCountrySwitch( Switch_Helper ) )	
	{
		m_pHelperCheckBox->SetCheck( m_bHelper );
	}

	m_pPVPModeCheckBox->SetCheck( m_bPVP );
			
	if(g_SoundOnOffDlg.m_siSelectCaptionKindIdex > -1)
		m_pSelectCaptionKindCombobox->SetCursel( g_SoundOnOffDlg.m_siSelectCaptionKindIdex );

	TCHAR Buffer[ 256 ] = TEXT("");
	Buffer[ 255 ] = '\0';
	GetWindowText( GetDlgItem( g_SoundOnOffDlg.m_hDlg, IDC_EDIT_TOTAL_SPEND_TIME ), Buffer, 254 );
	m_pTotalSpendTimeEdit->SetText( Buffer );
	
	// 보이는 중이면 안보이게 한다.
	if ( m_bShow )
	{
		m_bShow = false;

		CDialogController::Show( false );

		return;
	}

	// 새로 보이게 될 때만 서버에 시간을 요청한다.
	//------------------------------------
	// 게임서버로 플레이 시간을 요청한다. 
	//------------------------------------
	SI32 id  = 1;
	SI32 charunique = pclClient->pclCM->CR[id]->GetCharUnique();
	if(charunique)
	{
		cltGameMsgRequest_PlayTime clinfo(charunique);
		cltMsg clMsg(GAMEMSG_REQUEST_PLAYTIME, sizeof(clinfo), (BYTE*)&clinfo);

		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	// 국가 정보 공개 여부
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bShowCountry )
		m_pRadioShowCountryOn->SetNonEventCheck();
	else
		m_pRadioShowCountryOff->SetNonEventCheck();


	m_bShow = true;
	CDialogController::Show( true );

	return;

}

void CNSoundOnOffDlg::Hide()
{
	if ( ! m_bShow )
	{
		m_bShow = false;
		CDialogController::Show( false );

		return;
	}
	
	m_bShow = false;
	CDialogController::Show( false );

	return;
}

void CNSoundOnOffDlg::SetTotalSpendTime( SI32 TotalSpendTime , SI32 EventTime)
{
	SI32 Month = TotalSpendTime / ( 31 * 24 * 60 * 60 );
	SI32 Day = ( TotalSpendTime / ( 24 * 60 * 60 ) ) % 31;
	SI32 Hour = ( TotalSpendTime / ( 60 * 60 ) ) % 24;
	SI32 Min = ( TotalSpendTime / 60 ) % 60;
	SI32 Sec = TotalSpendTime % 60;

	SI32 eDay = ( EventTime / ( 24 * 60 * 60 ) ); 
	SI32 eHour = ( EventTime / ( 60 * 60 ) ) % 24;
	SI32 eMin = ( EventTime / 60 ) % 60;
	SI32 eSec = EventTime % 60;

	TCHAR Buffer[ 256 ] = TEXT("");
	TCHAR BufferEvent[ 256 ] = TEXT("");
	TCHAR BUfferTot[ 256 ] = TEXT("");

	TCHAR* pText = GetTxtFromMgr(2204);
	TCHAR* pTextEvent = GetTxtFromMgr(6079);

	StringCchPrintf( Buffer, 256, pText, Month, Day, Hour, Min, Sec );
	StringCchPrintf( BufferEvent, 256, pTextEvent, eDay, eHour, eMin, eSec );

	if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		StringCchPrintf( BUfferTot, 256, TEXT("%s%s"), Buffer, BufferEvent);
		m_pTotalSpendTimeEdit->SetText( BUfferTot );	// 이벤트 경과 타임까지 표시
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_English 
			|| pclClient->siServiceArea == ConstServiceArea_NHNChina 
			|| pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		_SYSTEMTIME currentTime = ((cltClient*)pclClient)->sTime;
		TCHAR szTimeText[128] = TEXT("");
		StringCchPrintf( szTimeText, 256, TEXT("%02d-%02d-%04d %02d:%02d"),
			currentTime.wMonth, currentTime.wDay, currentTime.wYear,
			currentTime.wHour, currentTime.wMinute);
		StringCchPrintf( BUfferTot, 256, TEXT("%sPST> %s"), Buffer, szTimeText );

		m_pTotalSpendTimeEdit->SetText( BUfferTot );	// 이벤트 경과 타임까지 표시
	}
	else
	{
		m_pTotalSpendTimeEdit->SetText( BufferEvent );
	}


	// 구 환경 설정 창의 EditBox 총 게임시간 셋팅
	SetWindowText( g_SoundOnOffDlg.m_hDlg, Buffer );
}

void CNSoundOnOffDlg::LoadSoundConfig()
{
	cltClient *pclclient = (cltClient *)pclClient;

	FILE *fp = _tfopen( TEXT("./Config/SoundConfig.dat"), TEXT("rt") );
	SI32 DisplayMode = 0;
	if ( fp == NULL )
	{
		m_bEffectSound = true;
		m_bBackGroudnSound = true;

		m_pEffectCheckBox->SetCheck( true, CONTROLLER_KIND_CHECKBOX );
		m_pBackgroundCheckBox->SetCheck( true, CONTROLLER_KIND_CHECKBOX );
	
		return;
	}

	SI32 EffectSound = 0;
	SI32 BackGroundSound = 0;
	

	
	SI32 MiniMapShow = 1;
	SI32 FullScreenMode = 1;				// default : 새인터페이스 사용
	SI32 MaxWindowMode = 0;				// 최대창 크기 모드 사용

	TCHAR Buffer[256] = TEXT("");
	_fgetts( Buffer, 255, fp );


	SI32 RightMouseMentAllow = 0;		// 사용자 메뉴 보여주기    
		                                                            
	SI32 WhisperChatReject = 0;			// 모든 귓속말 거부
	SI32 PrivateTradeReject = 0;		// 모든 거래 거부  
	
	SI32 JewelItemNotifyReject = 0;		// 명품 알림 거부          
		                                                            
	SI32 PartyInviteNotifyReject = 0;	// 파트 초대 거부          
	SI32 FriendAddNotifyReject = 0;		// 친구 추가 거부          
		                                                            
	SI32 SummonEvolveNotifyReject = 0;	// 소환수진화 알림 거부    
			                                                            
	SI32 NotifyHeadChat = 0;			// 모든 광고창 거부
	SI32 NotifyMinigame = 0;			// 미니게임 거부   

	SI32 ChatFixed = 0;					// 채팅창 고정 ( non-floating )
		                                                            
	SI32 siSelectCaptionKindIdex = 0;	// 자막 정보 인덱스        

	// 생성자에서 파일을 이미 읽었으므로 다시 읽지 않음 => 비정상 동작 막음
	m_pRightMouseMenuAllowCheckBox->SetCheck( m_bRightMouseMentAllow );

	//// 시작시 귓속말 항상 허용!
	m_bWhisperChatReject  = false;
	m_pWhisperChatRejectCheckBox->SetCheck( false );
	m_pPrivateTradeRejectCheckBox->SetCheck( m_bPrivateTradeReject );
	m_pNotifyJewelItemRejectCheckBox->SetCheck( m_bJewelItemNotifyReject );
	//// 시작시 파티 초대 항상 허용!!
	m_bPartyInviteNotifyReject  = false;
	m_pPartyInviteRejectCheckBox->SetCheck( false );
	m_pNotifyFriendAddRejectCheckBox->SetCheck( m_bFriendAddNotifyReject );
	m_pNotifySummonEvolveRejectCheckBox->SetCheck( m_bSummonEvolveNotifyReject );
	
	m_pNotifyHeadChatCheckBox->SetCheck( m_bNotifyHeadChat );
	m_pNotifyMiniGameCheckBox->SetCheck( m_bNotifyMinigame );
	m_pChatFixedCheckBox->SetCheck( m_bChatFixed );

	if(m_bChatTabFixed == true)
	{
		m_pChatTabFixedCheckBox->SetCheck( m_bChatTabFixed );
		g_SoundOnOffDlg.m_siChatTabNowIndex = m_siChatTabNowIndex;

	}
	m_pSelectCaptionKindCombobox->SetCursel( m_siSelectCaptionKindIdex );


	pclclient->pclMusic->pclSoundManager->EffectSoundOn( m_bEffectSound );
	m_pEffectCheckBox->SetCheck( m_bEffectSound );

	pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(m_bBackGroudnSound);
	m_pBackgroundCheckBox->SetCheck( m_bBackGroudnSound );
	
	m_pMinimapRejectCheckBox->SetCheck( m_bMiniMapShow, CONTROLLER_KIND_CHECKBOX );
	//if( m_pWindowmodeCheckBox->IsCheck() == m_bTmpFullScreenMode )
	//	m_pWindowmodeCheckBox->SetCheck( !m_bTmpFullScreenMode );
	m_pCheckBox_maximization->SetCheck( m_bTmpMaxWindowMode );
	
	m_pShowCharNameCheckBox->SetCheck( m_bShowCharName );

	m_pCheckBoxAloneDraw->SetCheck( m_bAloneDraw );

	SI32 bFullScreen = 0;

	GetFullScreenInFile( &bFullScreen );

	if(bFullScreen)
		m_bTmpMaxWindowMode = 1;
	else
		m_bTmpMaxWindowMode = 0;


	if( m_bTmpMaxWindowMode )
	{
		if( m_bFullScreenMode == false )
		{
			m_bFullScreenMode = true;
			m_bTmpFullScreenMode = true;
			//if( m_pWindowmodeCheckBox->IsCheck() == true )
			//	m_pWindowmodeCheckBox->SetCheck( false );
		}

		//m_pWindowmodeCheckBox->Enable( false );
	}	
	else
	{
		m_bMaxWindowMode = false;
		m_bTmpMaxWindowMode = false;
		//m_pCheckBox_maximization->SetCheck( false );
		
		//m_pWindowmodeCheckBox->Enable( true );
	}

	//m_pChatFixedCheckBox->SetCheck( TRUE, CONTROLLER_KIND_CHECKBOX );

	// 신체기관 창 보여주기 여부
	m_pShowOrganCheckBox->SetCheck( m_bOrganPannelShow );
	m_pShowOrganCheckBox->Enable( true );
	
	m_pMyKeyCheckBox->SetCheck( m_bMykey );
	m_pGameTipCheckBox->SetCheck( m_bGameTip );
	m_pHelperCheckBox->SetCheck( m_bHelper );
	m_pPVPModeCheckBox->SetCheck( m_bPVP );
	
	
	fclose( fp );

	return;
}

void CNSoundOnOffDlg::WriteSoundConfig()
{
	if(GetFileAttributes(TEXT("Config\\")) == (DWORD)-1)
	{
		// 디렉토리가 존재하지 않을경우
		CreateDirectory(TEXT("Config\\"),NULL);
	}

	FILE *fp = _tfopen( TEXT("./Config/SoundConfig.dat"), TEXT("wt") );
	if ( fp == NULL )	return;

	if ( m_bEffectSound )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}

	if ( m_bBackGroudnSound )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}

	if ( m_bMiniMapShow )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}

	//if( m_bFullScreenMode )
	if( m_bTmpFullScreenMode )
	{
		_ftprintf( fp, TEXT("%d "), 1 );
	}
	else
	{
		_ftprintf( fp, TEXT("%d "), 0 );
	}


	if( m_bRightMouseMentAllow		)	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bWhisperChatReject		)	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bPrivateTradeReject		)	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bJewelItemNotifyReject	) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bPartyInviteNotifyReject  ) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bFriendAddNotifyReject	) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bSummonEvolveNotifyReject )	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bNotifyHeadChat			) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );
	if( m_bNotifyMinigame			) 	_ftprintf( fp, TEXT("%d "), 1 );	 else 	_ftprintf( fp, TEXT("%d "), 0 );

	if( m_siSelectCaptionKindIdex >= 0 &&  m_siSelectCaptionKindIdex < MAX_CAPTIONKIND_NUM ) 	_ftprintf( fp, TEXT("%d "), m_siSelectCaptionKindIdex );	 
	else 						_ftprintf( fp, TEXT("%d "), -1 );

	if( m_bTmpMaxWindowMode ) 	_ftprintf( fp, TEXT("%d "), 1 );	 
	else 						_ftprintf( fp, TEXT("%d "), 0 );

	if( m_bChatFixed )			_ftprintf( fp, TEXT("%d "), 1 ); 
	else						_ftprintf( fp, TEXT("%d "), 0 );
	
	if( m_siSelectedSoundButton >= 0 && m_siSelectedSoundButton < 5 ) 	_ftprintf( fp, TEXT("%d "), m_siSelectedSoundButton);
	else						_ftprintf( fp, TEXT("%d "), 3 );

	if( m_siSelectedBGMButton >= 0 && m_siSelectedBGMButton < 5 ) 		_ftprintf( fp, TEXT("%d "), m_siSelectedBGMButton);
	else						_ftprintf( fp, TEXT("%d "), 3 );

	if( m_bShowCharName ) 		_ftprintf( fp, TEXT("%d "), 1 ); 
	else						_ftprintf( fp, TEXT("%d "), 0 );

	if( m_bAloneDraw )			_ftprintf( fp, TEXT("%d "), 1 ); 
	else						_ftprintf( fp, TEXT("%d "), 0 );

	if( m_bChatTabFixed )		_ftprintf( fp, TEXT("%d "), 1 ); 
	else						_ftprintf( fp, TEXT("%d "), 0 );
	
	if( m_siChatTabNowIndex )	_ftprintf( fp, TEXT("%d "), m_siChatTabNowIndex ); 
	else						_ftprintf( fp, TEXT("%d "), 0 );

	_ftprintf( fp, TEXT("%d "), ( m_bOrganPannelShow == true ? 1 : 0 ) );
	_ftprintf( fp, TEXT("%d "), ( m_bMykey			 == true ? 1 : 0 ) );
	_ftprintf( fp, TEXT("%d "), ( m_bGameTip		 == true ? 1 : 0 ) );
	_ftprintf( fp, TEXT("%d "), ( m_bHelper			 == true ? 1 : 0 ) );
	_ftprintf( fp, TEXT("%d "), ( m_bPVP			 == true ? 1 : 0 ) );
	
	fclose( fp );

	return;
}

void CNSoundOnOffDlg::SetSoundButton(SI16 selectedbutton )
{
	 cltClient* pclclient  = (cltClient*)pclClient;

	m_siSelectedSoundButton = selectedbutton;

 	for(SI16 i = 0; i < 5; i ++)
	{
		if( i <= selectedbutton)
			m_pEffVolumeButton[i]->SetFontIndex(i+5);
		else
			m_pEffVolumeButton[i]->SetFontIndex(i);
	}

	switch (selectedbutton)
	{
	case 0:
		pclclient->pclMusic->pclSoundManager->SetSoundVolume( 0.25f );
		break;
	case 1:
		pclclient->pclMusic->pclSoundManager->SetSoundVolume( 0.5f );
		break;
	case 2:
		pclclient->pclMusic->pclSoundManager->SetSoundVolume( 0.7f );
		break;
	case 3:
		pclclient->pclMusic->pclSoundManager->SetSoundVolume( 0.85f );
		break;
	case 4:
		pclclient->pclMusic->pclSoundManager->SetSoundVolume( 1.0f );
		break;
	}

}

void CNSoundOnOffDlg::SetBGMButton(SI16 selectedbutton )
{
	cltClient* pclclient  = (cltClient*)pclClient;
	m_siSelectedBGMButton = selectedbutton;

	for(SI16 i = 0; i < 5; i ++)
	{
		if( i <= selectedbutton)
			m_pBGMVolumeButton[i]->SetFontIndex(i+5);
		else
			m_pBGMVolumeButton[i]->SetFontIndex(i);
	}

	switch (selectedbutton)
	{
	case 0:
		pclclient->pclMusic->pclSoundManager->SetBackGroundSoundVolume( -2500 );
		break;
	case 1:
		pclclient->pclMusic->pclSoundManager->SetBackGroundSoundVolume( -1500 );
		break;
	case 2:
		pclclient->pclMusic->pclSoundManager->SetBackGroundSoundVolume( -1000 );
		break;
	case 3:
		pclclient->pclMusic->pclSoundManager->SetBackGroundSoundVolume( -700 );
		break;
	case 4:
		pclclient->pclMusic->pclSoundManager->SetBackGroundSoundVolume( -500 );
		break;
	}
}


void CNSoundOnOffDlg::SendCurrentSettingInfo()
{
	if( pclClient )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		if ( pclClient->GetGameStatus() == GS_GAME )
		{
			// Background Music On/Off => Client-Login.cpp의 DoMsg_GAMEMSG_RESPONSE_LOGIN에서 호출함
			if( pclclient->pclMusic && pclclient->pclMusic->pclSoundManager )
			{
				pclclient->pclMusic->pclSoundManager->BackGroundSoundOn(m_bBackGroudnSound);
				if(m_bBackGroudnSound) 
					pclclient->PlayBackgroundMusic( pclClient->pclCM->CR[1]->GetMapIndex() );
			}

			//  Send Msg to Server
			if( pclClient->pclCM && pclClient->pclCM->CR[1] )
			{
				// WhisperChat Reject On/Off
				cltGameMsgRequest_WhisperChat_Reject clinfo1( m_bWhisperChatReject );
				cltMsg clMsg1(GAMEMSG_REQUEST_WHISPERCHAT_REJECT, sizeof(clinfo1), (BYTE*)&clinfo1);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg1);

				// Private Trade Reject On/Off
				cltGameMsgRequest_PrivateTrade_Reject clinfo2( m_bPrivateTradeReject );
				cltMsg clMsg2(GAMEMSG_REQUEST_PRIVATETRADE_REJECT, sizeof(clinfo2), (BYTE*)&clinfo2);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg2);

				// PartyInvite Reject On/Off
				cltGameMsgRequest_PartyInvite_Reject clinfo3( m_bPartyInviteNotifyReject );
				cltMsg clMsg3(GAMEMSG_REQUEST_PARTYINVITE_REJECT, sizeof(clinfo3), (BYTE*)&clinfo3);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg3);

                // Friend Add Reject On/Off
				cltGameMsgRequest_FriendAdd_Reject clinfo4( m_bFriendAddNotifyReject );
				cltMsg clMsg4(GAMEMSG_REQUEST_FRIENDADD_REJECT, sizeof(clinfo4), (BYTE*)&clinfo4);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg4);



			} // if( pclClient->pclCM && pclClient->pclCM->CR[1] )
		} // if ( pclClient->GetGameStatus() == GS_GAME )
	} // if( pclClient )
}
void CNSoundOnOffDlg::WriteCountryConfig()
{
	// 국가 공개 여부
	bool showcountry = false;
	if( m_pRadioShowCountryOn->IsCheck() )
	{
		showcountry = true;
	}

	pclClient->pclCM->CR[1]->pclCI->clBI.bShowCountry = showcountry;

    cltGameMsgRequest_SetShowCountry clinfo( showcountry );
	cltMsg clMsg(GAMEMSG_REQUEST_SETSHOWCOUNTRY, sizeof(clinfo), (BYTE*)&clinfo );
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void CNSoundOnOffDlg::SetShowPuchikomiCheckBox(bool bCheck)
{
	m_pShowPuchikomiCheckBox->SetCheck(bCheck,-1);
}

void CNSoundOnOffDlg::LoadLanguageConfig()
{
	switch( pclClient->siLanguage )
	{
	case ConstLanguage_China:
		m_pComboLanguage->SetCursel(Language_Chinese);
		break;
	default:
		m_pComboLanguage->SetCursel(Language_English);
		break;
	}
}
void CNSoundOnOffDlg::WriteLanguageConfig()
{	
	if(GetFileAttributes(TEXT("Config\\")) == (DWORD)-1)
	{
		// 디렉토리가 존재하지 않을경우
		CreateDirectory(TEXT("Config\\"),NULL);
	}

	FILE *fp = _tfopen( TEXT("./Config/language.dat"), TEXT("wt") );
	if ( fp == NULL )
	{
		return;
	}

	_ftprintf( fp, TEXT("%d"), pclClient->siLanguage );

	fclose( fp );
}
void CNSoundOnOffDlg::LoadFpsConfig()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if( pclclient->GetFrameDelay() == 25 )
	{
		m_pComboFps->SetCursel( 1 );
	}
	else
	{
		m_pComboFps->SetCursel( 0 );
	}
}
void CNSoundOnOffDlg::WriteFpsConfig( SI16 fps )
{	
	if(GetFileAttributes(TEXT("Config\\")) == (DWORD)-1)
	{
		// 디렉토리가 존재하지 않을경우
		CreateDirectory(TEXT("Config\\"),NULL);
	}

	FILE *fp = _tfopen( TEXT("./Config/FrameRate.dat"), TEXT("wt") );
	if ( fp == NULL )
	{
		return;
	}

//	TCHAR szText[16]; 
	_ftprintf( fp, TEXT("%d"), fps );

	fclose( fp );
}
//#endif
