#include "../Common/CommonHeader.h"
#include "../../../Client/Client.h"

//--------------------------------
// Client
//--------------------------------
//#include "../../Client/ColorTable/ColorTable.h"
#include "../../Client/Music/Music.h"

// 채팅 관련. 
#include "../Client/Interface/DrawChat/DrawChatSurface.h"
#include "../Client/Interface/DrawChat/DrawChat.h"

// 캐릭터 이름 출력 관련
#include "../Client/Interface/DrawCharName/DrawCharName.h"

// 경험치 관련 
#include "../../Client/Effect/ExpCounter/ExpCounter.h"
// 치료 관련 
#include "../../Client/Effect/HealCounter/HealCounter.h"

// 농경술
#include "../../Client/NInterface/NFarmingDlg/NFarmingDlg.h"

//쪽지 관련
#include "../../Client/NInterface/NLetterDlg/NLetterDlg.h"

#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
//#include "../../Client/Interface/PartyDlg/PartyDlg.h"
#include "../../Client/NInterface/NPartyDlg/NPartyDlg.h"

#include "../../Client/InterfaceMgr/InterfaceMgr.h"

#include "CharCriticalEffect/CharCriticalEffect.h"
#include "../../Client/Effect/Emoticon/EmoticonMgr.h"

// 파티 관련 동료 표시
#include "CharPartyEmoticon/CharPartyEmoticon.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Personalshop.h"
//#include "../Client/Interface/PersonalShop/PersonalShopDlg.h"

#include "../../CommonLogic/Msg/MsgType-RacoonDodgeEvent.h"

// 스틸 방지를 위한 찜 표시
#include "CharJim/CharJim.h"

//--------------------------------
// Common
//--------------------------------
#include "../Common/Bullet/Bullet.h"
#include "Util/Util.h"
#include "PersonInfo.h"
#include "../CharManager/CharManager.h"

// 명령 관련 
#include "../../Common/Order/Order.h"

//--------------------------------
// CommonLogic
//--------------------------------
#include "../CommonLogic/Msg/MsgType-Quest.h"

extern cltCommonLogic* pclClient;

extern SI32 AttackAtbInfo[];

cltCharClient::cltCharClient() : cltCharCommon()
{

	pImgSpr	= NULL;
	Font	= 0;
	siEffectFont = -1;

	siShadowAni = 0 ;
	siShadowFont = -1 ;

	DrawSwitch			=  TRUE;
	bDrawReadySwitch	=  false;

	RealImageXPercent=RealImageYPercent=100;
	dwDrawFrame			=  0;                         // 이전에 그림을 그린 프레임 
	dwLastWaitFrame		=  0;

	TransMode			=  TRANSPARENT_MODE_NORMAL;

	ImageXsize	= 0;
	ImageYsize	= 0;

	VibrateY		= 0;
	VibrateSwitch	= FALSE;

	bMouseOverSwitch = false;
	bNewSelectedSwitch	= false;
	NewSelectedDelay	= 0;
	MouseOverDelay		= 0;

	DrawStartX	=	DrawStartY	= 0;

	//----------------------------
	// 애니메이션 
	//---------------------------
	StringCchCopy(szHelmetCode,	32,	TEXT(""));
	StringCchCopy(szArmourCode,	32,	TEXT(""));
	StringCchCopy(szWeaponCode,	32, TEXT(""));
	StringCchCopy(szMantleCode,	32,	TEXT(""));
	StringCchCopy(szHorseCode,	32,	TEXT(""));


	//------------------------------
	// 미러 출력 관련 
	//-----------------------------
	bReverseSwitch		= FALSE;                				// 뒤집어서 찍을 것인가?

	//-----------------------------
	// 이미지 사이즈 관련 
	//-----------------------------
	RealImageStartX	=	RealImageEndX	= 0;
	RealImageStartY	=	RealImageEndY	= 0; 

	//------------------------------
	// 사망관련 
	//------------------------------
	bDyingOperationSwitch	= false;
	bGhostShowSwitch		= false;

	//----------------------------
	// 이동 관련 
	//----------------------------
	siRX	=	siRY	= 0;
	MOX	=	MOY	= 0;
	MDX	=	MDY	= 0;

	//------------------------
	// 대행수, 행수 동작 처리. ( 바닥에 원 그리는 것 )
	//-------------------------
	bBoss		= false;							// 행수임을 알려줘야 하는가?
	pclBoss		= NULL;

	//------------------------
	// Regeneration 관련 
	//-------------------------
	bRegeneration	 = false;					// 리젠 되었음을 알려줘야 하는가?
	pclRegeneration = NULL;

	//-----------------------------------------
	//자기 마을에 트리가 건설되어 있는지 
	//-----------------------------------------
#ifdef _SAFE_MEMORY
	siEventNPCKind.ZeroMem();
#else
	ZeroMemory(siEventNPCKind,sizeof(siEventNPCKind));
#endif

	//------------------------
	// 개인상점 모드 
	//------------------------
	siShopMode      = 0;
	StringCchCopy(szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
	StringCchCopy(szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));


	//------------------------
	// 피격 이미지 관련 
	//-------------------------
	bHitDrawSwitch	= false;					// 피격되었음을 알려줘야 하는가?
	pclCharCut		= NULL;

	//------------------------
	// 레벨업 관련 
	//-------------------------
	bLevelUpDrawSwitch = false;				// 레벨업되었음을 알려줘야 하는가?
	pclLevelUp			= NULL;

	//------------------------
	// 하트 이펙트 관련 
	//-------------------------	 
	pclHeartEffect		= NULL;

	//------------------------
	// 네잎 클로버 이펙트 관련 
	//-------------------------	 
	pclFourLeafEffect = NULL;

	//------------------------
	// 쿠션 이펙트 관련 
	//-------------------------	 
	pclCushionEffect = NULL;
	pclCushionCatEffect = NULL;

	siCushionUnique = 0;

	//------------------------
	// MVP이펙트 관련 
	//-------------------------
	bMVPEffectDrawSwitch = false;				// 레벨업되었음을 알려줘야 하는가?
	pclMVPEffect			= NULL;

	//------------------------
	// Enchant 이펙트 관련
	//-------------------------	 	
	bEnchantEffDrawSwitch = false;
	pclEnchantEff			= NULL;

	//------------------------
	// Quest 이펙트 관련
	//-------------------------	 	
	//bQuestEffDrawSwitch = false;
	pclQuestEff			= NULL;


	//------------------------
	// 스킬 레벨업 관련 
	//-------------------------
	bSkillLevelUpDrawSwitch	= false;		// 스킬 레벨업되었음을 알려줘야 하는가?
	pclSkillLevelUp			= NULL;


	//------------------------
	// 이모티콘 관련
	//-------------------------
	pclEmoticonEff			= NULL;

	//------------------------
	// 발렌타인 이모티콘 관련
	//-------------------------
	m_pclValentineEmoticonEff			= NULL;

	//------------------------
	// 경험치 표시 
	//------------------------
	pclExpCounter		= NULL;					// 경험치 표시. 

	//------------------------
	// 치료   표시 
	//------------------------
	pclHealCounter		= NULL;					// HEAL 표시 

	//------------------------
	// 필살기 표시 
	//------------------------
	pclCriticalEffect	= NULL;					// 필살기 표시

	//------------------------
	// 스틸 방지를 위한 찜 표시
	//------------------------
	pclJim = NULL;


	//------------------------
	//	파티 동료 표시를 위한 것.
	//------------------------
	pclPartyEmoticon = NULL;

	//-----------------------
	// 말풍선 관련 
	//-----------------------
	pclDrawChat = NULL;

	//-----------------------
	// 캐릭터 이름 출력 관련
	//-----------------------
	pclDrawCharName = NULL;

	//---------------------------------
	// 조합형 캐릭터용 서피스 인덱스. 
	//--------------------------------
	siHeroSurfaceIndex	= -1;

	//-----------------------------
	// 추종자 캐릭터 정보.
	//-----------------------------
	siHostID	= 0;
	siSlaveID	= 0;

	//--------------------------
	// 캐릭터 이름 정보 표시 
	//--------------------------
	dwLastMakeCharNameFrame = 0;
	StringCchCopy(szCharNameInfo, 128, TEXT(""));

	//--------------------------
	// 치료 효과. 
	//--------------------------
	dwHealClock	= 0;

	//------------------------
	// 물품 생산 효과. 
	//------------------------
	bStartMakeItemSwitch	= false;
	dwStartMakeItemClock	= 0;
	dwDelayMakeItemClock	= 0;

	clMyShopData.Init();
	bMyShopStart = FALSE;

	m_RankMarkImgEncBuffer	= NULL;
	m_RankMarkEncHres = 0;
	m_RankMarkEncVres = 0;

	m_siSetItemImgAnimationCounter1 = 0;
	m_siSetItemImgAnimationCounter2 = 0;

	m_siWeddingImgAnimationCounter = 0;

	//--------------------------
	// 수호정령 특수모션 이펙트
	//--------------------------
	dwSoulGuardSF_Clock	= 0;
	siSoulGuardSF_Kind		= 0;
}

cltCharClient::~cltCharClient()
{

	SAFE_DELETE( pclDrawCharName  );
	SAFE_DELETE( pclDrawChat );
	SAFE_DELETE( pclBoss );
	SAFE_DELETE( pclRegeneration );
	SAFE_DELETE( pclCharCut );
	SAFE_DELETE( pclLevelUp );
	SAFE_DELETE( pclHeartEffect );
	SAFE_DELETE( pclFourLeafEffect );
	SAFE_DELETE( pclCushionEffect );
	SAFE_DELETE( pclCushionCatEffect );
	SAFE_DELETE( pclMVPEffect );
	SAFE_DELETE( pclEnchantEff);
	SAFE_DELETE( pclQuestEff );
	SAFE_DELETE( pclEmoticonEff );
	SAFE_DELETE( m_pclValentineEmoticonEff );

	SAFE_DELETE( pclSkillLevelUp );
	SAFE_DELETE( pclHealCounter );
	SAFE_DELETE( pclExpCounter );
	SAFE_DELETE( pclCriticalEffect );
	SAFE_DELETE( pclJim );
	SAFE_DELETE( pclPartyEmoticon );
	SAFE_DELETE( m_RankMarkImgEncBuffer );

}

void cltCharClient::InitCharClient()
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 출력의 준비가 되어 있지 않다. 
	SetDrawReadySwitch(false);

	//----------------------------
	// 애니메이션 
	//---------------------------
	StringCchCopy(szHelmetCode,	32,	TEXT("KOR01"));
	StringCchCopy(szArmourCode,	32,	TEXT("KOR01"));
	StringCchCopy(szWeaponCode,	32,	TEXT("KOR01"));
	StringCchCopy(szMantleCode,	32,	TEXT(""));
	StringCchCopy(szHorseCode,	32,	TEXT(""));

	//------------------------------
	// 사망관련 
	//------------------------------
	bDyingOperationSwitch	= false;
	bGhostShowSwitch		= false;

	pImgSpr	= NULL;
	Font	= 0;
	siEffectFont = -1;

	siShadowAni = 0 ;
	siShadowFont = -1 ;

	DrawSwitch=TRUE;
	RealImageXPercent=RealImageYPercent=100;
	dwDrawFrame	= 0;                         // 이전에 그림을 그린 프레임 
	dwLastWaitFrame		=  0;

	//------------------------------
	// 미러 출력 관련 
	//-----------------------------
	bReverseSwitch		= FALSE;                				// 뒤집어서 찍을 것인가?

	//-----------------------------
	// 이미지 사이즈 관련 
	//-----------------------------
	RealImageStartX	=	RealImageEndX	= 0;
	RealImageStartY	=	RealImageEndY	= 0; 

	//---------------------------------
	// 조합형 캐릭터용 서피스 인덱스. 
	//--------------------------------
	siHeroSurfaceIndex	= -1;

	//------------------------
	// 대행수, 행수 동작 처리. ( 바닥에 원 그리는 것 )
	//-------------------------
	bBoss	= false;							// 행수임을 알려줘야 하는가?
	if(pclBoss)
	{
		delete pclBoss;
		pclBoss = NULL;
	}

	pclBoss	= new CBossEffect( pclClient->GetGlobalSpr(GIMG_BIGBOSS), pclClient->GetGlobalSpr(GIMG_SMALLBOSS) );

	//------------------------
	// Regeneration 이미지 관련 
	//-------------------------
	bRegeneration	= false;					// 피격되었음을 알려줘야 하는가?
	if(pclRegeneration)
	{
		delete pclRegeneration;
		pclRegeneration = NULL;
	}
	//pclRegeneration = new CRegeneration( pclClient->GetGlobalSpr(GIMG_REGENERATION) );
	pclRegeneration = new CRegeneration( pclClient->GetGlobalSpr(GIMG_BEGINWARP) );

	//-----------------------------------------
	//자기 마을에 트리가 건설되어 있는지 
	//-----------------------------------------
	//ZeroMemory(siEventNPCKind,sizeof(siEventNPCKind));


	//------------------------
	// 개인상점 모드 
	//------------------------
	siShopMode      = 0;
	StringCchCopy(szPersonalShopText, MAX_PERSONAL_SHOP_TEXT_SIZE, TEXT(""));
	StringCchCopy(szPersonalShopTitle, MAX_SHOP_TITLE_LENGTH, TEXT(""));
	//------------------------
	// 피격 이미지 관련 
	//-------------------------
	bHitDrawSwitch	= false;					// 피격되었음을 알려줘야 하는가?
	if(pclCharCut)
	{
		SAFE_DELETE(pclCharCut)
	}
	pclCharCut = new CCharCut();

	//------------------------
	// 레벨업 이미지 관련 
	//-------------------------
	bLevelUpDrawSwitch = false;
	if(pclLevelUp)
	{
		SAFE_DELETE(pclLevelUp);		 
	}
	pclLevelUp = new CLevelUpEffect( pclClient->GetGlobalSpr( GIMG_LEVELUP ), pclClient->GetGlobalSpr( GIMG_LEVELUP_EFF ));

	//-----------------------
	// 하트 이펙트 관련
	//-----------------------
	if(pclHeartEffect)
	{
		SAFE_DELETE(pclHeartEffect);
	}
	pclHeartEffect = new CHeartEffect( pclClient->GetGlobalSpr( GIMG_HEART_EFFECT ) );

	//------------------------
	// 네잎 클로버 이펙트 관련 
	//-------------------------
	if(pclFourLeafEffect)
	{
		SAFE_DELETE(pclFourLeafEffect);
	}
	pclFourLeafEffect = new CFourLeafEffect( pclClient->GetGlobalSpr( GIMG_FOURLEAF ) );


	// 쿠션
	//KHY - 1109 - 가챠레어- 강아지방석추가
	siCushionUnique = 0;

	if(pclCushionEffect)
	{
		SAFE_DELETE(pclCushionEffect);
	}
	pclCushionEffect = new CCushionEffect( pclClient->GetGlobalSpr( GIMG_CUSHION_1  ) , pclClient->GetGlobalSpr( GIMG_CUSHION_2 ),
		pclClient->GetGlobalSpr( GIMG_CUSHION_3  ) , pclClient->GetGlobalSpr( GIMG_CUSHION_4 ),
		pclClient->GetGlobalSpr( GIMG_CUSHIOND_1 ) , pclClient->GetGlobalSpr( GIMG_CUSHIOND_2),
		pclClient->GetGlobalSpr( GIMG_CUSHIOND_3 ) , pclClient->GetGlobalSpr( GIMG_CUSHIOND_4));
	if(pclCushionCatEffect)
	{
		SAFE_DELETE(pclCushionCatEffect);
	}									
	pclCushionCatEffect = new CCushionCatEffect( pclClient->GetGlobalSpr( GIMG_CUSHION_CAT_1 ) , pclClient->GetGlobalSpr( GIMG_CUSHION_CAT_2 ),
		pclClient->GetGlobalSpr( GIMG_CUSHION_CAT_3 ) , pclClient->GetGlobalSpr( GIMG_CUSHION_CAT_4 ),
		pclClient->GetGlobalSpr( GIMG_CUSHION_DOG_1 ) , pclClient->GetGlobalSpr( GIMG_CUSHION_DOG_2 ),
		pclClient->GetGlobalSpr( GIMG_CUSHION_DOG_3 ) , pclClient->GetGlobalSpr( GIMG_CUSHION_DOG_4 ));

	//------------------------
	// MVP 이펙트 관련
	//-------------------------
	bMVPEffectDrawSwitch = false;
	SAFE_DELETE(pclMVPEffect);
	pclMVPEffect= new CMVPEffect( pclClient->GetGlobalSpr( GIMG_MVP_EFFECT ), pclClient->GetGlobalSpr( GIMG_MVP_AROUND_EFFECT ));

	//------------------------
	// Enchant 이펙트 관련
	//-------------------------
	bEnchantEffDrawSwitch = false;
	SAFE_DELETE(pclEnchantEff);
	pclEnchantEff= new CEnchantEff( pclClient->GetGlobalSpr( GIMG_ENCHANT_EFFECT_HERO_WE ),  pclClient->GetGlobalSpr( GIMG_ENCHANT_EFFECT_HERO_AM ), pclClient->GetGlobalSpr( GIMG_ENCHANT_EFFECT_HEROINE_WE ), pclClient->GetGlobalSpr( GIMG_ENCHANT_EFFECT_HEROINE_AM ));

	//------------------------
	// Quest 이펙트 관련
	//-------------------------
	//bQuestEffDrawSwitch = false;
	SAFE_DELETE(pclQuestEff);
	pclQuestEff= new CQuestEff( pclClient->GetGlobalSpr( GIMG_QUEST_EFFECT_RECEIVE ),  pclClient->GetGlobalSpr( GIMG_QUEST_EFFECT_CLEAR ));


	//------------------------
	// 스킬 레벨업 이미지 관련 
	//-------------------------
	bSkillLevelUpDrawSwitch = false;
	if(pclSkillLevelUp)
	{
		delete pclSkillLevelUp;
		pclSkillLevelUp = NULL;
	}
	pclSkillLevelUp = new CSkillLevelUpEffect( pclClient->GetGlobalSpr( GIMG_SKILLLEVELUP ) , pclClient->GetGlobalSpr( GIMG_SKILLLEVELUP_EFF ) );


	//------------------------
	// 이모티콘 이미지 관련
	//------------------------
	if( pclEmoticonEff )
	{
		delete pclEmoticonEff;
		pclEmoticonEff = NULL;
	}

	pclEmoticonEff = new CEmoticonEffect( pclClient->GetGlobalSpr( GIMG_EMOTICON) );

	//------------------------
	// 발렌타인 이모티콘 이미지 관련
	//------------------------
	if( m_pclValentineEmoticonEff )
	{
		delete m_pclValentineEmoticonEff;
		m_pclValentineEmoticonEff = NULL;
	}

	m_pclValentineEmoticonEff = new CEmoticonEffect( pclClient->GetGlobalSpr( GIMG_VALENTINEEMOTICON) );

	//------------------------
	// 경험치 표시 
	//------------------------
	if(pclExpCounter)
	{
		delete pclExpCounter;
		pclExpCounter = NULL;
	}
	pclExpCounter		= new CExpCounter;

	//------------------------
	// 치료 표시 
	//------------------------
	if(pclHealCounter)
	{
		delete pclHealCounter;
		pclHealCounter = NULL;
	}
	pclHealCounter		= new CHealCounter;

	if ( pclCriticalEffect )
	{
		delete pclCriticalEffect;
		pclCriticalEffect = NULL;
	}

	pclCriticalEffect	= new CCharCriticalEffect( pclClient->GetGlobalSpr( GIMG_CRITICALEFFECT ) );


	//------------------------
	// 스틸 방지를 위한 찜 표시
	//------------------------
	if ( pclJim )
	{
		delete pclJim;
		pclJim = NULL;
	}

	pclJim = new CCharJim( pclClient->GetGlobalSpr( GIMG_JIM ) );

	//------------------------
	//	파티 동료 표시를 위한 것.
	//------------------------
	if ( pclPartyEmoticon )
	{
		pclPartyEmoticon->Initialize();
	}
	else
	{
		pclPartyEmoticon = new CCharPartyEmoticon();
	}

	//-----------------------
	// 말 풍선 관련 
	//-----------------------
	if(pclDrawChat)
	{
		pclDrawChat->Initialize( GetCharUnique() );
	}
	else
	{
		pclDrawChat = new CDrawChat();
		pclDrawChat->Initialize( GetCharUnique() );
	}

	//-----------------------
	// 캐릭터 이름 출력 관련
	//-----------------------
	if ( pclDrawCharName )
	{
		//pclDrawCharName->Initialize();
		delete pclDrawCharName;
		pclDrawCharName = NULL;
		pclDrawCharName	= new CDrawCharName();
	}
	else
	{
		pclDrawCharName	= new CDrawCharName();
	}

	// 주인공 캐릭터의 크기는 정해져 있다. 
	bool selectKind = false;

	// anytype에 따라서, 주인공의 여부를 알아보는 함수가 틀리다.
	if(selectRealAnyType( ))
	{
		selectKind = pclKI->IsAtb(ATB_HERO);
	}
	else
	{
		selectKind = GetKindInfo(true)->IsAtb(ATB_HERO);
	}


	if(selectKind)
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		TransMode = pclKI->clKIDrawInfo.siTransparentMode;
		AlphaValue = pclKI->clKIDrawInfo.siAlphaValue ;

		ImageXsize = 320;
		ImageYsize = 240;

		// by LEEKH 2007.12.24 - TRANSFORM
		VibrateY		= 0;
		VibrateSwitch	=	pclKI->VibrateSwitch;
	}
	else
	{
		// by LEEKH 2007.12.24 - TRANSFORM
		TransMode = GetKindInfo()->clKIDrawInfo.siTransparentMode;
		AlphaValue = GetKindInfo()->clKIDrawInfo.siAlphaValue ;

		// by LEEKH 2007.12.24 - TRANSFORM
		SI32 ref = GetKindInfo(true)->clCharAniManager.FindFirstAni();
		if(ref <= 0)MsgBox(TEXT("fd9jfd"), TEXT("fd9jf:%s"), GetName());

		// by LEEKH 2007.12.24 - TRANSFORM
		ImageXsize = GetKindInfo(true)->clCharAniManager.GetImageXsize(ref, pclClient->CurrentClock);
		ImageYsize = GetKindInfo(true)->clCharAniManager.GetImageYsize(ref, pclClient->CurrentClock);

		// by LEEKH 2007.12.24 - TRANSFORM
		VibrateY		= 0;
		VibrateSwitch	=	GetKindInfo()->VibrateSwitch;
	}

	if(IsPC(CharUnique))
	{
		SetName(TEXT(""));

		// 주인공 캐릭터인 경우 
		if(GetID() == 1)
		{
			// 클라이언트가 준비되었음을 통보한다. 
			{
				cltMsg clMsg(GAMEMSG_REQUEST_READY, 0, NULL);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}

			//if(((cltClient*)pclClient)->bFirstHeroCharInitSwitch == false)
			{
				//	((cltClient*)pclClient)->bFirstHeroCharInitSwitch = true;

				// 서버에 이름을 요청한다. 
				{
					cltGameMsgRequest_GetName clInfo( CharUnique );
					cltMsg clNormalMsg(GAMEMSG_REQUEST_NAME, sizeof(clInfo), (BYTE*)&clInfo);
					pclclient->SendMsgToServer((sPacketHeader*)&clNormalMsg);
				}

				// 서버에 선전포고 상황을 요청한다.
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETVILLAGEINFO, 0);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				//// 아이템 몰 아이템이 있는지 요청한다.
				//{
				//	pclclient->pclCM->CR[1]->pclCI->clItemMall.Init();
				//	cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLEXISTENCE,0);
				//	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				//}   // 캐릭터 선택시마다 요청할수 있도록 client-Login 으로 이사보냈습니다. [영진]

			}
		}
	}

	//-----------------------------
	// 추종자 캐릭터 정보.
	//-----------------------------
	siHostID		= 0;
	siSlaveID		= 0;

	//--------------------------
	// 캐릭터 이름 정보 표시 
	//--------------------------
	dwLastMakeCharNameFrame = 0;
	StringCchCopy(szCharNameInfo, 128, TEXT(""));

	//--------------------------
	// 대기동작이 마지막으로 바뀐시간
	//--------------------------
	lastWaitAniChanged = pclclient->CurrentClock;

	//--------------------------
	// 치료 효과. 
	//--------------------------
	dwHealClock	= 0;

	//------------------------
	// 물품 생산 효과. 
	//------------------------
	bStartMakeItemSwitch	= false;
	dwStartMakeItemClock	= 0;
	dwDelayMakeItemClock	= 0;


	//-------------------------
	// 포그 처리를 한다.
	//------------------------
	FogAction();

	//--------------------------
	// 수호정령 특수모션 이펙트
	//--------------------------
	dwSoulGuardSF_Clock	= 0;
	siSoulGuardSF_Kind		= 0;

}



BOOL cltCharClient::OperateWaitClient()
{
	if(clInnerStatus.IsStatus(INNERSTATUS_ICE) == false || clInnerStatus.IsStatus(INNERSTATUS_FROZEN) == false || clInnerStatus.IsStatus(INNERSTATUS_STUN) == false )
	{
		// 정지시 가만히 있는 캐릭터는 조치를 취하지 않는다.
		// 동작에 관계없이 일정한 애니를 갖는 유닛은 안된다.
		SetAnimation(WaitType);
		// 애니메이션이 업데이트 되면, 
		if(DoAnimationDelay() == TRUE)
		{
			// 최초 이미지를 보여주는 것이라면, 
			if(clAniInfo.GetAniStep() == 0 )//&& clAniInfo.siRetryNum > 0 )
			{
				BOOL bRideMotorcycle = FALSE;
				SI32 siHorseUnique = 0;
				if( pclCI->clHorseInfo.GetCurrentHorseRIDEHORSE() )
				{
					cltHorse* pclHorse = pclCI->clHorseInfo.GetCurrentShowHorse();
					if(pclHorse && pclHorse->siHorseUnique == HORSEUNIQUE_MOTORCYCLE)
					{
						bRideMotorcycle = TRUE;
					}

					siHorseUnique = pclHorse->siHorseUnique;
				}

				if( GetKindInfo(true)->IsAtb(ATB_HERO) == false )		bRideMotorcycle = false;

				if(bRideMotorcycle)
				{
					WaitType = ANITYPE_WAIT0;
					SetAnimation(WaitType);
					DoAnimationDelay();
				}
				else
				{
					if(WaitType == ANITYPE_WAIT0)
					{
						// 정지 2번이 있는지 확인한다. 
						// by LEEKH 2007.12.24 - TRANSFORM
						SI32 aniref			= GetKindInfo(true)->clCharAniManager.FindRefFromAnitype(ANITYPE_WAIT1);

						//cyj 밑에 코드에서 프레임이 홀수로 증가할때는 대기 동작이 안바뀌므로 수정
						//if((pclClient->GetFrame() % 10 == 0) && aniref >= 0)
						SI32 siRand = 0;

						// 사수리의 경우 날개짓 모양이 멈춰 있는 것처럼 보임으로 WAIT0을 줄인다.
						if( siHorseUnique == HORSEUNIQUE_SASURI)
						{
							siRand = 1000;
						}
						else
						{
							siRand = (rand()%7 + 3)*1000;
						}

						if( (TABS( pclClient->CurrentClock - lastWaitAniChanged ) > siRand) && aniref >= 0 )
						{
							lastWaitAniChanged = pclClient->CurrentClock;						dwLastMakeCharNameFrame = pclClient->GetFrame();

							WaitType = ANITYPE_WAIT1;
							SetAnimation(WaitType);
							DoAnimationDelay();
						}
					}
					else
					{
						WaitType = ANITYPE_WAIT0;
						SetAnimation(WaitType);
						DoAnimationDelay();
					}
				}
			}
		}
	}

	// 대사를 혼자 지껄인다. 
	if((SI32)pclClient->GetFrame() % 400 == (GetID() % 400))
	{
		if ( pclKI->pclWarChatInfo )
		{
			if (  GetMapIndex() != pclKI->pclWarChatInfo->siMapindex )
			{
				TCHAR* ptext = NULL ;

				ptext = pclKI->GetWarRandChatWinText(GetMapIndex());

				if(ptext)
				{
					cltClient* pclclient = (cltClient*)pclClient;
					pclDrawChat->Set(pclclient->pclDrawChatSurface, ptext, false, NPC_CHATTING);
				}
			}
			else
			{
				TCHAR* ptext = pclKI->GetRandChatText() ;

				if(ptext)
				{
					cltClient* pclclient = (cltClient*)pclClient;
					pclDrawChat->Set(pclclient->pclDrawChatSurface, ptext, false, NPC_CHATTING);
				}
			}
		}
		else
		{
			TCHAR* ptext = pclKI->GetRandChatText() ;

			if(ptext)
			{
				cltClient* pclclient = (cltClient*)pclClient;
				pclDrawChat->Set(pclclient->pclDrawChatSurface, ptext, false, NPC_CHATTING);
			}
		}
	}

	// 주인 캐릭터가 있는 경우, 
	if(siHostID)
	{
		if(pclCM->IsAlive(siHostID))
		{
			cltCharClient* pclchar = (cltCharClient*)pclCM->CR[siHostID];
			SI32 length = max( TABS(pclchar->GetX() - GetX()) , TABS(pclchar->GetY() - GetY()) );

			if(length >= 3)
			{
				SI32 xvary = 1 - (rand() % 3);
				SI32 yvary = 1 - (rand() % 3);

				OrderMove(GetID(), pclchar->GetX() + xvary, pclchar->GetY() + yvary, BY_COMPUTER);
			}
		}
	}

	// 정지시에는 바쁠게 없다.
	bBusySwitch = false;


	return TRUE;
}

void cltCharClient::GetBottomPoint(cltPos* pclpos)
{
	pclpos->siX = siCenterX;
	pclpos->siY = siCenterY + (RealImageEndY-RealImageStartY)/2;
}

// 찍을 위치를 계산한다. 
void cltCharClient::CalculateRealXY()
{

	int   FlyVibrateData[MAX_FLY_VIBRATE] = {0, 1, 2, 3, 4, 3, 2, 1};

	SHORT x=GetX();
	SHORT y=GetY();

	//흔들림을 넣는다. 
	if(VibrateSwitch==TRUE)
	{
		VibrateY=FlyVibrateData[((pclClient->GetFrame() + siID)/4)%MAX_FLY_VIBRATE];
	}

	SI32 mapcenterx = 0, mapcentery = 0;
	pclMap->GetXYDotFromXYCord(GetX(), GetY(), mapcenterx, mapcentery);

	// 캐릭터의 중심을 구한다
	siCenterX=mapcenterx + siRX;

	SI32 mapcenterx2 = 0, mapcentery2 = 0;
	pclMap->GetXYDotFromXYCord(x, y, mapcenterx2, mapcentery2);

	siCenterY=mapcentery+5 + siRY-(RealImageEndY-RealImageStartY)/2;

	RealRect.left=  mapcenterx2 + siRX-(RealImageEndX-RealImageStartX)/2;
	RealRect.right= RealRect.left + (RealImageEndX-RealImageStartX);
	RealRect.bottom=mapcentery2 + siRY+20;//pclKI->clKIDrawInfo.siBottomIndent;
	RealRect.top=   RealRect.bottom-(RealImageEndY-RealImageStartY); 
}

void cltCharClient::NewSelectedAction()
{
	if(bNewSelectedSwitch == true)
	{
		//if(pclClient->GetFrame() % 2 ==0)
		if((pclClient->CurrentClock / 50) % 2 ==0)
		{
			if(NewSelectedDelay>0)
			{
				NewSelectedDelay--;
			}
			else
			{
				SetNewSelectedSwitch(false);
			}
		}
	}


	if(bMouseOverSwitch == true )
	{
		//if(pclClient->GetFrame() % 2 ==0)
		if((pclClient->CurrentClock / 50) % 2 ==0)
		{
			if(MouseOverDelay>0)
			{
				MouseOverDelay--;					
			}
			else
			{
				SetMouseOverSwitch(false);
			}
		}		
	}

}

void cltCharClient::SetNewSelectedSwitch(bool mode)
{

	switch(mode)
	{
	case true:
		bNewSelectedSwitch	=	true;
		NewSelectedDelay	=	NEW_SELECTED_DELAY;
		break;
	case false:
		bNewSelectedSwitch	=	false;
		break;
	}
}

void cltCharClient::SetMouseOverSwitch(bool mode)
{	
	switch(mode)
	{
	case true:
		{
			if( bNewSelectedSwitch == false ) // 깜박이는 중이 아니라면
			{
				bMouseOverSwitch = true;	// 계속 허옇게 빛나게끔 
				MouseOverDelay = MOUSE_OVER_DELAY;
			}
			else
				bMouseOverSwitch = false;
		}
		break;
	case false:
		{
			bMouseOverSwitch = false;
		}
		break;
	}
}

// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
// length: 중심과의 거리 
BOOL cltCharClient::IsAbsPtInObj(SHORT x, SHORT y, SHORT& length)
{

	if(RealRect.left <x && x<RealRect.right
		&& RealRect.top  <y && y<RealRect.bottom)
	{
		length=max(TABS(siCenterX-x),TABS(siCenterY-y));
		return TRUE;
	}

	return FALSE;
}



///----------------------------------
// Session 생성시 호출 함수. 
//-----------------------------------
void cltCharClient::Init()
{
	ITZPerson::Init();

	//pclClient->MakeSimpleChar(m_uiCharID, 0);
}

// Logout시 호출되는 함수. 
void cltCharClient::Close()
{
	ITZPerson::Close();

	//cyj 접속 끊어져도 캐릭 정보 지우지 않음
	//pclClient->pclCM->DeleteChar( GetID() );
}
//----------------------------
// 클라이언트 캐릭터의 동작. 
//----------------------------
void cltCharClient::CharClientAction()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( siPVPWaitTime > 0 )
	{
		if (TABS(pclClient->CurrentClock - siPVPWaitTime) >= 7000)// 셋팅한지 6초가 지나면 
		{
			// 기다리는 시간이 다 끝났으므로 스타트 날린다.
			cltGameMsgRequest_PVP_Request clinfo ( START_PVP , pclclient->pclCM->CR[1]->pclCI->GetPersonID() );
			cltMsg clMsg(GAMEMSG_REQUEST_PVP_FILED, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			siPVPWaitTime = 0	;
		}
	}
	if ( siPVPTime > 0 )
	{
		if (TABS(pclClient->CurrentClock - siPVPTime) >= (1000*60)*3)// 3분이 지나면 Draw
		{	
			if ( pclclient->pclCM->CR[1]->GetPVP_LeagueEnemyPersonID() <= 0 ) // PVP리그가 아닐 때만 클라이언트에서 보낸다
			{
				cltGameMsgRequest_PVP_Request clinfo ( DRAW_PVP , pclclient->pclCM->CR[1]->siEnemUnique );
				cltMsg clMsg(GAMEMSG_REQUEST_PVP_FILED, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				siPVPTime = 0	;
			}
		}
	}
	if ( siRacoonDodgeWaitTime > 0 )//Switch_10MonthEvent_RacoonDodge
	{
		if (TABS(pclClient->CurrentClock - siRacoonDodgeWaitTime) >= 7000)// 셋팅한지 6초가 지나면 
		{
			// 기다리는 시간이 다 끝났으므로 스타트 날린다.
			cltGameMsgRequest_RacoonDodgeEveent_StartGame clinfo (pclclient->pclCM->CR[1]->pclCI->GetPersonID() );
			cltMsg clMsg(GAMEMSG_REQUEST_RACOONDODGE_EVENT_OVER5SECOND_START, sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			siRacoonDodgeWaitTime = 0	;
		}
	}
	// Regenertaion 동작 처리
	CharRegenerationAction();

	// 베기 동작 처리. 
	CharCutAction();

	// 신분 표시 처리. 
	CharBossAction();

	if( pclLevelUp )
	{
		pclLevelUp->Action();
	}

	if( pclHeartEffect )
	{
		pclHeartEffect->Action( bHeartEffectSwitch );
	}

	if( pclFourLeafEffect )
	{
		pclFourLeafEffect->Action( bFourLeafSwitch );
	}

	// 쿠션 정보 설정 - by LEEKH 2007.11.13
	SI32 siCushion = 0;

	if(GetCurrentOrder() == ORDER_SITDOWN)
		siCushion = siCushionUnique;
	else
		siCushion = 0;

	//KHY - 1109 - 가챠레어- 강아지방석추가
	if( pclCushionEffect )
	{
		pclCushionEffect->Action( siCushion, GetKind() , siDirection );
	}
	if( pclCushionCatEffect )
	{
		pclCushionCatEffect->Action( siCushion, GetKind() );
	}
	/*
	if( pclCushionEffect )
	{	
	pclCushionEffect->Action( 
	(GetCurrentOrder() == ORDER_SITDOWN)
	&& bHaveCushion
	, GetKind() , siDirection );
	}
	if( pclCushionCatEffect )
	{
	pclCushionCatEffect->Action( (GetCurrentOrder() == ORDER_SITDOWN)
	&& bHaveCushion
	, GetKind() );
	}
	*/
	if( pclMVPEffect )
	{
		pclMVPEffect->Action();
	}

	if( pclEnchantEff)
	{
		pclEnchantEff->Action();
	}

	if( pclQuestEff)
	{
		pclQuestEff->Action();
	}

	if( pclSkillLevelUp )
	{
		pclSkillLevelUp->Action();
	}

	if( pclEmoticonEff )
	{
		pclEmoticonEff->Action();
	}

	if( m_pclValentineEmoticonEff )
	{
		m_pclValentineEmoticonEff->Action();
	}

	if(pclExpCounter)
	{
		pclExpCounter->Action();
	}

	if(pclHealCounter)
	{
		pclHealCounter->Action();
	}

	if ( pclJim )
	{
		pclJim->Action();
	}

	if(pclDrawChat)
	{
		pclDrawChat->Action();

		if ( bPersonalShopSwitch == true && pclDrawChat->IsDrawStatus() == false )
		{
			if(bPremiumPersonalShop == true)
			{				
				pclDrawChat->Set(((cltClient*)pclClient)->pclDrawChatSurface, szPersonalShopText, true, PERSONALSHOP_BANNER_PREMIUM, siShopMode,szPersonalShopTitle);
			}
			else
			{
				pclDrawChat->Set(((cltClient*)pclClient)->pclDrawChatSurface, szPersonalShopText, true, PERSONALSHOP_BANNER, siShopMode);
			}
		}
		else if( (bIsElectionStump == true) && (pclDrawChat->IsDrawStatus() == false) )
		{
			pclDrawChat->Set(((cltClient*)pclClient)->pclDrawChatSurface, szMaintenance, true, ELECTIONSTUMP, PSHOP_MODE_SELL, szCandidate);
		}
		else if ( bRepairShopSwitch == true )
		{
		}

	}


	if ( pclCriticalEffect )
	{
		pclCriticalEffect->Action();
	}

	// 마우스로 캐릭터를 선택했을 경우의 조치.
	NewSelectedAction();

	// 말이 존재한다면 말을 대동하게 한다. 
	if(IsPC(GetCharUnique()))
	{
		m_clMagicBufMgr.DecFrame( 0 );

		cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

		// 추종마가 무효하다면, 
		if(pclCM->IsValidID(siSlaveID) == false)
		{

			// 현재 타고 있는 말이 아니어야 한다. 
			//			if(pclCI->clHorseInfo.siCurrentHorseIndex != 0)
			if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == false) // siCurrentHorseRIDEHORSE == false
			{
				if(pclhorse != NULL && pclhorse->siHorseUnique > 0 && pclhorse->siHorseUnique < MAX_HORSE_TYPE_INFO)
				{
					SI32 id		= pclCM->pclIDManager->GetID(0);
					SI32 charunique		= 0;

					// horseunique를 가지고 적절한 kind를 찾는다. 
					SI32 horsekind = pclClient->pclHorseManager->pclHorseTypeInfo[pclhorse->siHorseUnique]->siKind;

					// 말은 InnerStatus 가 필요없다.
					cltInnerStatusInfo	clEmptyInnerStatus;

					cltBufInfo clBufInfo;
					clBufInfo.Init();
					cltMakeCharInfo clinfo(horsekind, charunique, 1, 0, GetX(), GetY(), GetMapIndex(),
						0, 
						0, 
						false, 
						false,
						false, 
						false, 
						false,
						false,
						false,
						false,
						0, 
						NULL, 
						pclCI->clHorseInfo.siCurrentShowHorse, 
						0,
						0, 0, 0, 							
						0, 
						0, 0, 
						NULL,0,0,
						NULL, 
						NULL, 
						&clEmptyInnerStatus,
						false, 
						false,
						0,
						false,
						0,
						NULL,
						NULL,
						pclCI->clHorseInfo.siCurrentHorseRIDEHORSE,
						0,
						false,
						false,	// [추가 : 황진성 2008. 1. 14 => 자신의 길드가 최강인가.]
						false,
						false,
						0,
						0,
						NULL,
						false
						,NULL
						,0
						,0
						,0
						,0
						,0
						,0
						,0
						,0
						,0
						,0
						,false
						,&clBufInfo
						);
					siSlaveID = pclClient->SetChar(id, &clinfo);
					if(pclCM->IsValidID(siSlaveID))
					{
						cltCharClient* pclchar = (cltCharClient*)pclCM->CR[ siSlaveID ] ;
						pclchar->siHostID = GetID();
					}

				}
			}
		}
		// 추종마가 유효하다면, 
		else
		{
			cltCharClient* pclchar = NULL;
			if( pclCM->IsValidID(siSlaveID) )
			{
				pclchar = (cltCharClient*)pclCM->CR[ siSlaveID ];
			}
			if(pclchar)
			{
				// 추종마를 지워야 하는지 여부, 
				bool bdelhorseswitch = false;

				//그 말을 타고 있는 것이라면 추종마를 삭제한다. 
				if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == TRUE)
				{
					bdelhorseswitch = true;
				}
				// 말이 사라진것이라면 삭제한다. 
				if(pclhorse->siHorseUnique == 0)
				{
					bdelhorseswitch = true;
				}

				if(bdelhorseswitch)
				{
					if(pclchar->siHostID == GetID())
					{
						pclCM->DeleteChar(siSlaveID);
						siSlaveID = 0;
					}
				}

				// 추종마의 호스트와 내 아이디가 일치하지 않으면 실제적인 추종마가 아니다. 
				if(pclchar->siHostID != GetID())
				{
					siSlaveID = 0;
				}
			}
		}

		//-----------------------------------
		// 특수 임무 점검 
		//-----------------------------------
		if(GetID() == 1 && (pclClient->GetFrame() % 100) == 0)
		{
			SI32 questtype	= QUEST_TYPE_SPECIAL;
			SI32 questunique = pclCI->clQuestInfo.clUnit[questtype].siCurQuestUnique;
			if(questunique)
			{
				cltQuestInfoCommon* pclinfo = &pclCI->clQuestInfo.clSpecialQuestInfo;
				if(pclinfo)
				{
					// 퀘스트 완료 조건을 충족시키는지 확인한다. 
					if(IsClearCondition(questtype, &pclinfo->clQuestClearConditionInfo) == true)
					{
						cltGameMsgRequest_QuestGoing clInfo(questtype, questunique, QUEST_STEP_CLEAR);
						cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
						SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}

			questtype	= QUEST_TYPE_NAMING;
			questunique = pclCI->clQuestInfo.clUnit[questtype].siCurQuestUnique;
			if(questunique)
			{
				cltQuestInfoCommon* pclinfo = &pclCI->clQuestInfo.clNamingQuestInfo;
				if(pclinfo)
				{
					// 퀘스트 완료 조건을 충족시키는지 확인한다. 
					if(IsClearCondition(questtype, &pclinfo->clQuestClearConditionInfo) == true)
					{
						cltGameMsgRequest_QuestGoing clInfo(questtype, questunique, QUEST_STEP_CLEAR);
						cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
						SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
			//KHY - 0822 - 해피빈 퀘스트 추가.
			questtype	= QUEST_TYPE_HAPPYBEAN;
			questunique = pclCI->clQuestInfo.clUnit[questtype].siCurQuestUnique;
			if(questunique)
			{
				cltQuestInfoCommon* pclinfo = &pclCI->clQuestInfo.clHappyBeanQuestInfo;
				if(pclinfo)
				{
					// 퀘스트 완료 조건을 충족시키는지 확인한다. 
					if(IsClearCondition(questtype, &pclinfo->clQuestClearConditionInfo) == true)
					{
						cltGameMsgRequest_QuestGoing clInfo(questtype, questunique, QUEST_STEP_CLEAR);
						cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
						SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}

			// 미션 퀘스트 추가 - by LEEKH 2008-09-18
			questtype	= QUEST_TYPE_MISSION1;
			questunique = pclCI->clQuestInfo.clUnit[questtype].siCurQuestUnique;
			if(questunique)
			{
				cltQuestInfoCommon* pclinfo = &pclCI->clQuestInfo.clMissionQuestInfo;
				if(pclinfo)
				{
					// 퀘스트 완료 조건을 충족시키는지 확인한다. 
					if(IsClearCondition(questtype, &pclinfo->clQuestClearConditionInfo) == true)
					{
						cltGameMsgRequest_QuestGoing clInfo(questtype, questunique, QUEST_STEP_CLEAR);
						cltMsg clMsg(GAMEMSG_REQUEST_SETQUESTGOING, sizeof(clInfo), (BYTE*)&clInfo);
						SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
		}

		// 농경술 검사
		if ( pclCM->CR[1]->GetCurrentOrder() == ORDER_FARM )
		{
			CNFarmingDlg* FarmDlg = (CNFarmingDlg*)pclclient->m_pDialog[NFARMING_DLG] ;
			if ( FarmDlg == NULL )							
			{        
				pclclient->CreateInterface( NFARMING_DLG );
			}
		}
	}

	if( bMyShopStart )
	{
		SI64 siTotalPrice = 0;
		SI32 i;
		for ( i = 0; i < MAX_SHOP_ITEM_NUMBER; ++i )
		{
			if(clMyShopData.m_clItem[i].siUnique > 0)
				siTotalPrice += (clMyShopData.m_clItem[i].siItemNum * (SI64)clMyShopData.m_siPrice[i]);
		}

		// 내가 갖고 있는돈을 가져와 물건의 가격과 비교한다.
		SI64 BuyerMoney   = (SI64)pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();

		if ( BuyerMoney  < siTotalPrice )
		{
			TCHAR* pTitle = GetTxtFromMgr(5118);
			TCHAR* pText = GetTxtFromMgr(5119);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

			cltGameMsgRequest_StopPersonalShop clshopdata( pclClient->pclCM->CR[1]->GetCharUnique());
			cltMsg clMsg( GAMEMSG_REQUEST_STOPPERSONALSHOP, sizeof( clshopdata ), (BYTE*)&clshopdata );
			cltClient *pclclient = ( cltClient * )pclClient;
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

			if(pclclient->m_pDialog[ NPERSONALSHOP_DLG ])			
				pclclient->m_pDialog[ NPERSONALSHOP_DLG ]->DeleteDialog();

			clMyShopData.Init();
			bMyShopStart = FALSE;
		}
	}

	// [기형] 2009.09.02 프리미엄 선공 방지 스프가 활성화 상태라면 소환수의 상태를 버프 상태로 바꿔주서야 한다.
	SI32 siSummonID = GetSummonID();
	cltCharClient *pclSummon = pclCM->GetCharClient(siSummonID);
	if( NULL != pclSummon)
	{
		if( pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum)
		{
			pclSummon->bNoFirstAttackSwitch = true;
		}
	}
}



// 말을 탄다. 
bool cltCharClient::RideHorse()
{
	if(cltCharCommon::RideHorse()== false)return false;

	// 타게 된 말의 정보에 따라 필요한 이미지를 로딩한다. 
	cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();
	if(pclhorse == NULL)			return false;

	// PCK : 축제음식 솜사탕을 먹으면 탈것중 랜덤하게 하나로 이미지가 바뀐다. (09.08.11)
	if(pclCI->clBI.clBufInfo.IsHaveBuf(BUF_TRANSFORM_HORSE))
	{
		SI32 siTransFormHorseUnique = pclCI->clBI.clBufInfo.GetBufPercent(BUF_TRANSFORM_HORSE);
		ChangeHorseCode(siTransFormHorseUnique);
	}
	else
	{
		ChangeHorseCode( pclhorse->siHorseUnique );
	}

	return true;
}

// 말에서 내린다.
bool cltCharClient::UnrideHorse()
{
	if(cltCharCommon::UnrideHorse()== false)return false;

	// 타게 된 말의 정보에 따라 필요한 이미지를 로딩한다.

	ChangeHorseCode( 0 );

	return true;
}

// 삭제에 필요한 조치를 취한다. 
void cltCharClient::DeleteOperation()
{
	// 추종자를 삭제한다. 
	DeleteSlave();
}

// 추종자를 삭제한다. 
void cltCharClient::DeleteSlave()
{

	// 추종자 캐릭터를 삭제한다. 
	if(siSlaveID)
	{
		if(pclCM->IsAlive( siSlaveID )  == TRUE)
		{
			pclCM->DeleteChar( siSlaveID );
			siSlaveID	= 0;
		}
	}
}

bool cltCharClient::AfterCharAction()
{
	// 출력 준비가 되어있다.
	SetDrawReadySwitch(true);

	return false;
}


// 찜정보에 따라 화면에 표시하거나 표시하지 않는다. 
void cltCharClient::SetJimShow()
{
	if(pclJim == NULL)return ;

	if(clJim.siJimHostCharUnique)
	{
		bool bhostjimswitch = false;

		SI32 jimcharunique = clJim.siJimHostCharUnique;
		if(pclClient->IsUser(jimcharunique))
		{
			bhostjimswitch = true;
		}

		SI32 id = pclCM->GetIDFromCharUnique(jimcharunique);
		if(pclCM->IsValidID(id))
		{
			if(pclCM->CR[id]->GetSummonParentID() == 1)
			{
				bhostjimswitch = true;
			}
		}

		//cyh 파티원일때는 찜표시 안나오도록 수정
		cltClient *pclclient = (cltClient*)pclClient;
		if(pclclient->m_pDialog[ NPARTY_DLG ])
			if (((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyUser(jimcharunique))
				bhostjimswitch = true;

		if(bhostjimswitch == true)
		{
			pclJim->Set(true, true);
		}
		else
		{
			pclJim->Set(true, false);
		}
	}
	else
	{
		pclJim->Set(false, true);
	}

}


// 캐릭터가 진행하고 있는 퀘스트를 점검하여 종결시킨다. 
void cltCharClient::DoQuestCheckAndAction(SI32 questclearcond)
{
	SI16 questtype, questunique, para1, para2;
	if(pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}


}

void FogOp(cltMapClient* pclmap, SI32 index, SI32 charx, SI32 chary);

// 포그 처리를 한다. 
void cltCharClient::FogAction()
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 charunique = GetCharUnique() ;

	BOOL TempCheck = FALSE;
	if(pclclient->m_pDialog[ NPARTY_DLG ])
		TempCheck = ((CNPartyDlg*)pclclient->m_pDialog[ NPARTY_DLG ])->IsPartyUser(charunique);

	if ( GetID() == 1 || GetID() == pclCM->CR[1]->GetSummonID() || TempCheck == TRUE )
	{
		cltMapClient* pclmap = (cltMapClient*)pclMap;

		SI32 fogindex = 0;
		FogOp(pclmap, fogindex, GetX(), GetY());
	}
}


void cltCharClient::MakeRankMarkEncBuffer()
{
	if ( m_RankMarkImgEncBuffer )
	{
		delete [] m_RankMarkImgEncBuffer;
		m_RankMarkImgEncBuffer = NULL;
	}

	SI32 NpcKindKey = -1;


	if ( pclKI->clKIDrawInfo.siHeadNameFont > -1 )
	{
		NpcKindKey = pclKI->clKIDrawInfo.siHeadNameFont;   //npc 번호

	}
	else if( clIdentity.siIdentity == IDENTITY_VILLAGEGOD )
	{
		NpcKindKey = 43;
	}

	//여기가 맞는데 소환수 flag
	else if(clIdentity.siIdentity ==IDENTITY_PERSON_SUMMON )
	{
		//if (!pclKI->bSummonMall )
		NpcKindKey = 44;
		//else
		//	NpcKindKey = 56;

	}
	else
	{
		SI32 ItemUnique = 0;

		if ( pclCI->clCharItem.clItem[ PERSONITEM_HAT ].siUnique == 0 )
		{
			ItemUnique = clPB.clShapeEquip.clHelmet.uiUnique;
		}
		else
		{
			ItemUnique = pclCI->clCharItem.clItem[ PERSONITEM_HAT ].siUnique;
		}


		if ( ItemUnique )
		{
			SI32 Ref = pclClient->pclItemManager->FindItemRefFromUnique( ItemUnique );

			if ( Ref > 0 )
			{
				switch( pclClient->pclItemManager->pclItemInfo[Ref]->siNeedRank )
				{
				case 1:
					NpcKindKey = 0;
					break;
				case 4:
					NpcKindKey = 42;
					break;
				case 5:
					NpcKindKey = 54;
					break;
				case 6:
					NpcKindKey = 57;
					break;
				case 7:
					NpcKindKey = 62;
					break;
				default:
					NpcKindKey = -1;
					break;
				}
			}
		}
		else
		{
			NpcKindKey = -1;
		}

		// [영훈] 기자단 모자 (2008. 06 .05)
		if ( CONST_JOURNALIST_HAT_ITEMUNIQUE == ItemUnique )
		{
			NpcKindKey = 0;
		}

	}	


	if ( NpcKindKey < 0 )
	{
		return;
	}




	//NPCMgr();    //npc 의 인덱스와 이미지 파일을 불러온다.

	cltClient* pclclient = (cltClient*)pclClient;

	m_RankMarkEncHres = 0;
	m_RankMarkEncVres = 0;
	int flag_key=pclclient->m_NPCImgIndex[NpcKindKey]-1;        //npc 종류 플래그 (0, 1, 2)


	LPDIRECTDRAWSURFACE7 lpSurface = NULL;
	//cltTSpr RankMarkImage;
	TSpr RankMarkImage;

	if ( g_graphics.CreateSurface16( &lpSurface, MAX_RANKMARK_WIDTH, MAX_RANKMARK_HEIGHT, TRUE, RGB16_TRANS ) == FALSE )
	{
		MsgBox( TEXT("CreateOffScreen Error, MakeRankMarkEncBuffer()"), TEXT("sdfsd") );
	}

	// 메모리 보안코드 - by LEEKH
	if(lpSurface == NULL)				return;

	g_graphics.ClearSurface( lpSurface, RGB16_TRANS );


	//-------------------------------------------------
	// 배경 이미지 출력 부분
	//-------------------------------------------------


	TCHAR szName[256]=TEXT(" ");
	StringCchCopy(szName, 256, pclclient->m_NPCName[NpcKindKey]);

	//if( lpSurface->GetDC(&hdc) == DD_OK)

	SIZE size = { 0, 0 };
	HDC hdc;

	//  npc 이름을 서피스에 쓴다. 
	if(lpSurface->GetDC(&hdc)== DD_OK)
	{
		HFONT hFont;
		//if( pclClient->IsWhereServiceArea( ConstSwitch_NONATIALIASED_QUALITY ) ){
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			hFont = CreateFont( 11, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, pclclient->m_pInterfaceMgr->GetFontKind() );
		}
		else {
			hFont = CreateFont( 11, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, pclclient->m_pInterfaceMgr->GetFontKind() );
		}


		HFONT hOldFont = NULL;

		hOldFont = (HFONT)SelectObject( hdc, hFont );

		GetTextExtentPoint32(hdc, szName, _tcslen(szName), &size);

		SelectObject( hdc, hOldFont );

		if ( hFont )
		{
			DeleteObject( hFont );
			hFont = NULL;
		}

		lpSurface->ReleaseDC( hdc );
	}

	if(size.cx && size.cy )//이름위에 FLAG
	{
		// npc 이미지를 버퍼에 넣는다. 


		GP.LoadSprFromFile( TEXT("NInterface/Image/Image_48x18_00_000.SPR")  ,&RankMarkImage );	//그림파일을 불러 오고

		if (RankMarkImage.GetXSize() == 0)//그림파일 사이즈가 0 이면 리턴
			return;
		SI32 xnum = size.cx / RankMarkImage.GetXSize()  +  ( ( size.cx % RankMarkImage.GetXSize() ) ? 1:0 );
		m_RankMarkEncHres	= ( xnum + 2 ) * RankMarkImage.GetXSize();
		m_RankMarkEncVres	= RankMarkImage.GetYSize();

		m_RankMarkImgEncBuffer = new RGB16[ MAX_RANKMARK_WIDTH * MAX_RANKMARK_HEIGHT * 2 ];


		if( GP.LockSurface( lpSurface ) )
		{
			GP.PutSpr( &RankMarkImage, 0, 0, 0 + (flag_key*3) );

			for(SI32 i =0 ;i < xnum  ;i++)
			{
				GP.PutSpr( &RankMarkImage, RankMarkImage.GetXSize() +i * RankMarkImage.GetXSize(), 0 , ( 1+(flag_key*3) ));
			}

			GP.PutSpr( &RankMarkImage, RankMarkImage.GetXSize() + xnum * RankMarkImage.GetXSize(), 0 , ( 2+(flag_key*3) ));

			GP.UnlockSurface( lpSurface );
		}				

		//-------------------------------------------------
		// 폰트 이미지 출력 부분
		//-------------------------------------------------

		// npc 이름을 출력한다. 
		if( lpSurface->GetDC(&hdc)== DD_OK)//flag 을 위에 글을 올린다.

		{
			HFONT hFont;

			if ( pclclient->siServiceArea == ConstServiceArea_Japan)
			{
				//if( pclClient->IsWhereServiceArea( ConstSwitch_NONATIALIASED_QUALITY ) ){
				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( 11, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("돋움") );
				}
				else {
					hFont = CreateFont( 11, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("돋움") );
				}


			}
			else
			{
				//if( pclClient->IsWhereServiceArea( ConstSwitch_NONATIALIASED_QUALITY ) ){
				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( 13, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("Tahoma") );
				}
				else {
					hFont = CreateFont( 13, 0, 0, 0, 0, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("Tahoma") );
				}

			}

			HFONT hOldFont = NULL;

			hOldFont = (HFONT)SelectObject( hdc, hFont );

			SetTextColor(hdc,COLOR_DIALOG_BLACK);


			SetBkMode(hdc, TRANSPARENT );
			//SetFont();

			TextOut(
				hdc, 
				RankMarkImage.GetXSize(), 
				( m_RankMarkEncVres - size.cy ) / 2 + 2, 
				szName, lstrlen(szName) );

			SelectObject( hdc, hOldFont );

			if ( hFont )
			{
				DeleteObject( hFont );
				hFont = NULL;
			}

			lpSurface->ReleaseDC( hdc );
		}


		if ( GP.LockSurface( lpSurface ) == TRUE )
		{
			int size;
			// 메모리 보안 코드 추가
			if ( m_RankMarkEncHres > MAX_RANKMARK_WIDTH )		m_RankMarkEncHres = MAX_RANKMARK_WIDTH;
			if ( m_RankMarkEncVres > MAX_RANKMARK_HEIGHT )		m_RankMarkEncVres = MAX_RANKMARK_HEIGHT;

			if(m_RankMarkImgEncBuffer)
			{
				g_graphics.EncodeXpr( (RGB16*)m_RankMarkImgEncBuffer, 
					//MAX_RANKMARK_WIDTH,
					//MAX_RANKMARK_HEIGHT,
					m_RankMarkEncHres, m_RankMarkEncVres,
					RGB16_TRANS,
					&size );
			}

			GP.UnlockSurface( lpSurface );
		}
	}

	if( lpSurface )
	{
		lpSurface->Release();
		lpSurface = NULL;
	}



	return;
}



void cltCharClient::NPCMgr( )
{
	// 안씁니다!! 변수 모두 cltClient로 이동.

	//TCHAR buffer[1024] = TEXT("");
	//FILE* fp= NULL;

	//if( pclClient->siServiceArea ==  ConstServiceArea_Korea )	// 한국
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrK.txt"), TEXT("rt"));
	//}
	//else if( pclClient->siServiceArea ==  ConstServiceArea_English )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrE.txt"), TEXT("rt"));
	//}
	//else if(pclClient->siServiceArea ==  ConstServiceArea_China )
	//{
	//       fp = _tfopen(TEXT("TextMgr\\NPCMgrC.txt"), TEXT("rt"));
	//}
	//else if(pclClient->siServiceArea ==  ConstServiceArea_Japan )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrJ.txt"), TEXT("rt"));
	//}
	//else if( pclClient->siServiceArea ==  ConstServiceArea_Taiwan )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrT.txt"), TEXT("rt"));
	//}
	//else if( pclClient->siServiceArea ==  ConstServiceArea_USA )
	//{
	//	fp = _tfopen(TEXT("TextMgr\\NPCMgrU.txt"), TEXT("rt"));
	//}


	//if(fp == NULL)
	//{
	//	MsgBox(TEXT("fileError"), TEXT("not NPCMgrtextFile"));
	//	return ;
	//}

	//// 엔피씨 텍스트의 4줄은 주석으로 무시한다.
	//for(SI32 i = 0;i < 4;i++)
	//	_fgetts(buffer, 1024, fp);

	//for ( i = 0 ; i < MAX_NPCKINDFORRANKMARK ; i ++ )
	//{
	//	StringCchCopy(m_NPCName[i], 64, TEXT(""));
	//	m_NPCImgIndex[i]=0;		
	//}


	//i = 0 ;

	//while( _fgetts(buffer, 1024, fp) != NULL)
	//{
	//	if ( i >= MAX_NPCKINDFORRANKMARK ) break ;
	//	
	//	TCHAR * szindex;
	//	TCHAR * szname;
	//	TCHAR * szkind;

	//	szname = _tcstok(buffer, TEXT("\t"));
	//	szkind = _tcstok(NULL,TEXT("\r\n"));

	//	if ( !szname || !szkind ) break ;
	//	{

	//		szindex= _tcstok(szname,TEXT(":"));
	//		szname =_tcstok(NULL,TEXT(" ") );
	//		StringCchCopy(m_NPCName[i], 64, szname);
	//	}
	//	m_NPCImgIndex[i] = _tstoi(szkind);
	//	i ++ ;
	//}

	//fclose(fp);	
}

SI32 cltCharClient::GetRX() const 
{
	return siRX;
}

SI32 cltCharClient::GetRY() const 
{
	return siRY;
}

DWORD cltCharClient::GetDrawFrame()
{
	return dwDrawFrame;
}

void cltCharClient::SetDrawReadySwitch(bool mode)
{
	bDrawReadySwitch = mode;
}

//------------------------
// 피격 이미지 관련 
//-------------------------
void cltCharClient::SetHitSwitch(bool mode)
{
	cltCharCommon::SetHitSwitch(mode);

	// 피격 이미지 출력하게 하기.
	SetHitDrawSwitch(mode);
}


void cltCharClient::SetHitDrawSwitch(bool mode)
{
	bHitDrawSwitch = mode;
}

//--------------------------------
// 대행수, 행수 동작 처리. ( 바닥에 원 그리는 것 )
//--------------------------------
void cltCharClient::CharBossAction()
{
	if( pclBoss )
	{
		pclBoss->Action();
	}
}


//--------------------------------
// Regeneration 동작 처리. 
//--------------------------------
void cltCharClient::CharRegenerationAction()
{
	if(pclRegeneration)
	{
		pclRegeneration->Action();
	}
}

void cltCharClient::SetCharRegeneration()
{
	if(pclRegeneration)
	{
		pclRegeneration->Set();
	}
}

//--------------------------------
// 베기 동작 처리. 
//--------------------------------
void cltCharClient::CharCutAction()
{
	if(pclCharCut)
	{
		pclCharCut->Action();
	}
}

void cltCharClient::SetCharCut(SI32 mode)
{
	if(pclCharCut)
	{
		pclCharCut->SetAction(mode);
	}
}

//--------------------------------
// 레벨업 동작 처리. 
//--------------------------------
void cltCharClient::SetLevelUp()
{
	if( pclLevelUp )
	{
		pclLevelUp->Set();
	}

	// 캐릭터를 레벨업 동작으로 세팅

	bLevelUpDrawSwitch = true;
	clAniInfo.SetAniStep(0);

}

//--------------------------------
// MVP 이펙트 동작 처리
//--------------------------------
void cltCharClient::SetMVPEffect()
{
	if( pclMVPEffect )
	{
		pclMVPEffect->Set();
	}
}


//--------------------------------
// Enchant 이펙트 동작 처리
//--------------------------------
void cltCharClient::SetEnchantEffect(SI08 AniType)
{
	if( pclEnchantEff )
	{	
		if( 0 == AniType)
		{
			pclEnchantEff->Set(ENCHANT_EFFECT_NONE);
		}

		if( KIND_HERO3 == GetKind() || KIND_HERO4 == GetKind()  )
		{
			if( ENCHANT_ANI_ARMOR == AniType)
			{
				pclEnchantEff->Set(ENCHANT_EFFECT_HERO_AMOR);
			}
			else if(ENCHANT_ANI_WEPONE == AniType)
			{
				pclEnchantEff->Set(ENCHANT_EFFECT_HERO_WEPONE);
			}
		}
		else if( KIND_HEROINE3 == GetKind() || KIND_HEROINE4 == GetKind() )
		{
			if( ENCHANT_ANI_ARMOR == AniType)
			{
				pclEnchantEff->Set(ENCHANT_EFFECT_HEROINE_AMOR);
			}
			else if(ENCHANT_ANI_WEPONE == AniType)
			{
				pclEnchantEff->Set(ENCHANT_EFFECT_HEROINE_WEPONE);
			}
		}
	}
}

//--------------------------------
// Quest 이펙트 동작 처리
//--------------------------------
void cltCharClient::SetQuestEffectReceive()
{
	if( pclQuestEff)
	{
		pclQuestEff->SetReceive();
	}
}

void cltCharClient::SetQuestEffectClear()
{
	if( pclQuestEff)
	{
		pclQuestEff->SetClear();
	}
}

//--------------------------------
// 스킬 레벨업 동작 처리. 
//--------------------------------
void cltCharClient::SetSkillLevelUp()
{
	if( pclSkillLevelUp )
	{
		pclSkillLevelUp->Set();
	}

	bLevelUpDrawSwitch = true;
	clAniInfo.SetAniStep(0);
}

//--------------------------------
// 이모티콘 동작 처리
//--------------------------------
void cltCharClient::SetEmoticon( SI16 index )
{
	if ( pclEmoticonEff )
	{
		pclEmoticonEff->Set( index );
	}
}

//--------------------------------
// 발렌타인 이모티콘 동작 처리
//--------------------------------
void cltCharClient::SetValentineEmoticon( SI16 index )
{
	if ( m_pclValentineEmoticonEff )
	{
		m_pclValentineEmoticonEff->Set( index );
	}
}

bool cltCharClient::IsBlankInv(SI32 invnum,SI32 invmode)
{
	// ALL모드이면 전체를 하나 하나 체크한다.
	if(invmode == INVMODE_ALL)
	{
		bool	ret;

		// 사람 인벤토리 검색
		ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_PERSON);
		if(ret)			return ret;
		// 말 인벤토리 검색
		if(HaveHorse() == true)
		{
			ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_HORSE);
			if(ret)			return ret;
		}
		// 가방1 인벤토리 검색
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG].siUnique > 0)
		{
			ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_BAG);
			if(ret)			return ret;
		}
		// 가방2 인벤토리 검색
		if(pclCI->clCharItem.clItem[PERSONITEM_BAG_SUB].siUnique > 0)
		{
			ret = pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager, invnum, INVMODE_BAG_SUB);
			if(ret)			return ret;
		}

		// INVMODE_ALL 모드는 여기서 끝낸다.
		return ret;
	}

	return pclCI->clCharItem.IsBlankInv(pclClient->pclItemManager,invnum,invmode);
}
