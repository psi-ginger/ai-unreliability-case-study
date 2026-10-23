//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _CLIENT_H
#define _CLIENT_H

#define JEWAN_FRAME_SKIP						// 12월 10일 작업 효율적인 프레임 스킵 ( 제완 )

#include "..\CommonLogic\CommonLogic.h"

#include "UnderMouseObject\UnderMouseObject.h"
#include "Logo\Logo.h"

#include "..\..\Server\FrontServer\FrontMsg.h"
#include "..\..\Server\FrontServer\WorldList.h"
#include "Network\StateBox.h"
#include "GlobalImgFile\GlobalImgFile.h"
#include "Interface\Pannel\Pannel.h"
#include "ITZNetOverlapped.h"
#include "Char\CharClient\Char-Client.h"
#include "Key\Key.h"

#include "DrawLoadingImg\DrawLoadingImg.h"

#include "InterfaceMgr/DialogResource.h"

#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "NUpdateTimer.h"

#ifdef USE_HANAUTH
	#ifndef _SERVER
		// - NHN ACM -
		#include "../HanAuth/HanAuthForClient.h"
		// - NHN ACM -
	#endif
#endif

#ifdef USE_HANREPORT
	#ifdef _SERVER

	#else
		#include "../HanAuth/HanReportForClient.h"

			#ifdef _DEBUG
				#pragma comment(lib, "HanReportForClientD.lib") 
			#else
				#pragma comment(lib, "HanReportForClient.lib") 
			#endif // _DEBUG
	
	#endif // _SERVER
#endif // USE_HANREPORT



// -HanAuth_Lib
//한게임 투표 모듈
#ifdef USE_HANPOLL
	#ifdef _SERVER
	#else
		// 한게임 설문 조사 모듈
		#include "../HanPollForClient/HanPollForClient.h"

		#ifdef _DEBUG
			#pragma comment(lib, "HanPollForClientD.lib") 
		#else
			#pragma comment(lib, "HanPollForClient.lib") 
		#endif
	#endif		
#endif


//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
	#ifndef _SERVER
		#include "../Porte/PorteAuthForClient.h"
		using namespace Porte::PorteAuthSystem;
	#endif
#endif

#include "../CommonLogic/BattleRoyal/CBattleRoyalClient.h"

//[추가 : 황진성 2007. 12. 3 오토프로그램 검색 헤더 추가.]
#include "../Client/AutoMacroMgr/clAutoMacroManager.h"
#include "../Client/CheckProcess/CheckProcess.h"
#include "ItemPeriodMgr/ItemPeriodMgr.h"


class cltMusic;
class CRegistryMgr;
class CDrawSequence;
class CDrawChatSurface;
class CDrawCharNameSurface;


class cltQuitConfirm;
class cltQuitMenu;
class CRememberWindowPosMgr;
class CCCDSurfaceMgr;
class CFocusManager;
class CClickEffect;
class cltGameContract;

class CaptureJPG;
class CFriendLoginDraw;
class CResidentLoginDraw;
class cltMiniHomeUpdates;
class cltPICAManagerCheck;
class cltPopupCheck;
class CVillageWarNotice;
class CNewYearEventNotice;

class CResidenceCheck;
class CVillageNotice;
class CDurability;
class CGameMasterDlg;
class CGameMasterPersonalLogDlg;
class CGMServerInfoDlg;
class CEventNotice;
class CWebEventDlg;
class cltHtmlDlgManager;

class CTradeMerchantDlg;
class CDecreePasswdDlg;
class cltGameMsgResponse_DelUserChar;
class cltGameMsgResponse_PreDelUserChar;
class cltHeroDataBackup;
class CTradeMerchantHorse;
class CTradeMerchantDish;
class cltHelpManager;

class CChatStatusBar;
class cltPCRoomIntro;

class CNPrivateChatDlgMgr;


class CAbuseFilter;

class CTestDlg;
class CDrawLogo;

class cltNVillageDetailInfoManager;

struct stMsgBox;
class CInterfaceMgr;

class CDialogController;
class CControllerMgr;
class CNFarmingDlg;

class CFeastEffectManager;

///////////////////////////////////////////////////////////////////
//// 미니 게임 추가
class CMiniGameFactory;
class CMiniGameManager;

class CEmoticonInfo;
class CBeginnerText;
class CLevelUpText;
class CGameTip;

class cltMarkManager;
class CGameClient;

class cltCheckProcess;
class CItemPeriodMgr;
class CGawiBawiBoMgr_Client;
class CClient_BBBMgr;
class CBattleRoyalClient;
//class CSearchDetailDlg;
class CClient_PartyMatchingMgr;
class CPartyQuestMgr_Client;

class	cltGMA;					// GZMacroAnalysis
class	cltGZFtp;


#define MAX_WORLD_NAME_SIZE		128

#define FIND_TREASUREEVENT_QUEST_UNIQUE	31500
#define FIND_TREASUREEVENT_QUEST_UNIQUE_SUB1	31505
#define FIND_TREASUREEVENT_QUEST_UNIQUE_SUB2	31510
#define FIND_TREASUREEVENT_QUEST_UNIQUE_SUB3	31515


enum // 이벤트 레벨 
{
	GAME_ROUND_1	=	0	,
	GAME_ROUND_2,
	GAME_ROUND_3,
	GAME_ROUND_4,
	GAME_ROUND_5,
	GAME_ROUND_6,
	GAME_ROUND_7,
	GAME_ROUND_8,
	GAME_ROUND_9,
	GAME_ROUND_10,
};


extern SI32		g_siContryCode;
extern BOOL		g_bFullScreen;

extern bool bIDPassswordInputSwitch;
extern bool loginswitch;
extern bool noticeswitch;
extern bool worldlistswitch;

//-------------------------------------------
// cltClient
//-------------------------------------------
class cltClient : public cltCommonLogic {
public:
	//[추가 : 황진성 2007. 12. 3 => 클라이언트가 오토프로그램으로 작동중인지 확인해줄 클레스]
	CAutoMacroManager clAutoMacroManager;


	cltMouseObject clMouseObject;

	SI32	siRX, siRY;				// 스크린의 RX, RY
	DWORD	m_dwGTick;
	bool	bDropTrySwitch;				// 아이템 버리기 시도 중인가.(이동 불가) 

	bool	bReceiveServerInfoSwitch;		// 서버의 기본 정보 수령 여부. 

public:

	SI32	siScreenMode;

	SI32	siScreenModeXOffSet;
	SI32	siScreenMode800X600XSize;

	SI32	siScreenModeYOffSet;
	SI32	siScreenMode800X600YSize;

	TCHAR	strTextFontFileName[ MAX_PATH ];

	//-------------------------------------------
	// 내자시 관련 이펙트
	//-------------------------------------------
	CFeastEffectManager*	m_pFeastEffectManager;
	//------------------------------------
	// 화면캡쳐용 Class 
	//------------------------------------
	CaptureJPG	*pclCapture;

	//-------------------------------------------
	// 디폴트 서비스 지역 
	//-------------------------------------------
	SI32 siServiceArea;							// 클라이언트가 처음 작동할때 어떤 서비스 지역을 근거로 하는가. 


	//-------------------------------------------
	// 사용자 아이디 패스워드. 
	//-------------------------------------------
	TCHAR	szID[FILE_NAME_SIZE];						// 사용자 아이디 
	TCHAR	szPassword[FILE_NAME_SIZE];					// 사용자 비밀번호.        

	bool	bWindowNotice;								// 윈도우 깜빡임 설정.

	cltLogo*	pclLogo;								// 로고 

	CDrawLoadingImg*	m_pLoadingImg;					// 로딩이미지

	//---------------------------------------------
	// 네트워크 관련 
	//---------------------------------------------

	BYTE									m_encRecvBuf[ 100000 ];  // 서버에서 받은 패킷 암호화 해제 버퍼

	// APEX-
	bool									m_bStartApex;
	CRITICAL_SECTION						m_CSSendToGameServer;
	// -APEX

//KHY - 0204 -  중국 PurpleAuth 인증 모듈
#ifdef USE_PORTEAUTH
#ifndef _SERVER //서버가 정의되어 있지 않다면.
	TCHAR									m_szPorteAuth_GameInfoString[SIZE_GAMEINFOSTRING];
	TCHAR									m_szPorteAuth_szAuthTICKET[SIZE_AUTHTICKET];
	TCHAR									m_szPorteAuth_szServiceCode[SIZE_GAMEINFOSTRING];	// 알파 값을 받아와야 한다 .
#endif
#endif // USE_PORTEAUTH
	SI08									m_siPorteAuth_TryCount;  // 3까지 제접속을 시도한다.

#ifdef _USA_NEW_AUTH_
	TCHAR									m_szUSAAuth_String[4096];
#endif


#ifdef USE_HANAUTH
	// -HanAuth
	TCHAR									m_szHanAuth_GameString[SIZE_GAMESTRING];
	TCHAR									m_szHanAuth_AuthString[SIZE_AUTHSTRING];
	// -HanAuth
#endif // USE_HANAUTH
	SI32									m_siHanAuth_ServiceTypeCode;
	TCHAR									m_szHanAuth_UserID[MAX_PLAYER_NAME];
	SI08									m_siHanAuth_TryCount;

	TCHAR									m_szPorteAuth_UserID[13]; // User ID size is 12byte 	// 중국 유저 아이디

// -HanAuth_Lib
	void									HanAuth_Lib_HanReportSendPosLog_Client(TCHAR* pszID, TCHAR* value);
// -HanAuth_Lib
	void									HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(SI32 siserverID, TCHAR* pszUserID, TCHAR* pszValue);

	//NUpdateTimer							kHanAuth_UpdateTimer;
//	NUpdateTimer							kHanAuth_UpdateTimer_GameRunAuth;

	ITZSession*								pGameSession;
	ITZSession*								pFrontSession;
	SI32									siAuthKey;							//	Front Server로 부터 받은 인증키. 

	UI08									cSecretBuffer[ PACKET_MAX_SIZE ];	// 서버와의 통신을 위해 암호화된 데이터를 저장할 버퍼. 
	bool									bFrontServerSecretKeySwitch;		// 프런트 서버로부터 암호화키 를 받았음을 확인한다.
	bool									bGameServerSecretKeySwitch;			// 게임서버로부터 암화화키를 받았음을 확인한다.ㅏ

	BOOL									bLoginRequest;
	DWORD									dwLastLoginTick;
	NUpdateTimer							m_kTimer_WaitGameLogin;


	ITZSessionFactory< cltCharClient >		sf;
	ITZSessionManager						sm;
	ITZNetOverlapped						overlapped;

	cltWorldList							clWorldList;						// 서버로 부터 받은 월드 리스트. 
	WORLDINFO								clSelectedWorldInfo;				// 선택된 월드 정보. 

	cltStateBox								clUserState;

	cltGZFtp		*m_pclGZFtp;
	//-------------------------------
	// GZMacroAnalysis
	//-------------------------------
	cltGMA				*m_pclGMA;
	SI32				m_siGMA_ByAdminPersonID;
	BOOL				m_bGMA_ReportedByAdminCommand;

	bool bFrontServerActionSwitch;
	bool bGameServerActionSwitch;

	//--------------------------------------------
	// 포커스 상태 관리
	//--------------------------------------------
	CFocusManager	*pclFocusManager;

	//--------------------------------------------
	// 레지스트리 관리 
	//--------------------------------------------
	CRegistryMgr*	pclRegistry;

	//--------------------------------------------
	// 툴바. 
	//--------------------------------------------
	cltToolBar*				pclToolBar;

	//--------------------------------------------
	// 윈도우 위치 기억
	//--------------------------------------------
	//CRememberWindowPosMgr	*pclRememberWindowPosMgr;

	//-------------------------------------------
	// 판넬 
	//------------------------------------------
	cltPannel*				pclPannel;


	//-------------------------------------------
	// 내구도 표시
	//------------------------------------------
	CDurability*			pclDurability;

	//CSearchDetailDlg*		m_pSearchDetailDlg;

	//-------------------------------------------
	// 마을 안내문
	//------------------------------------------
	CVillageNotice			*pclVillageNotice;

	//-----------------------------------------
	// 수리 모드 
	//-----------------------------------------
	bool	bRepairSwitch;

	//-----------------------------------------
	// 자막 모드 
	//-----------------------------------------
	bool	bPromptSwitch;

	//-----------------------------------------
	// SMALL MAP 모드
	//-----------------------------------------
	bool	bSmallMapSwitch;

	//----------------------------------------
	// 마을 건물 정보 수신 확인 스위치. 
	//----------------------------------------
	bool bWaitVillageStructureInfoSwitch;

	//----------------------------------------
	// 주인공 캐릭터가 최초로 설정되었는지 여부 
	//----------------------------------------
	//bool	bFirstHeroCharInitSwitch;

	//----------------------------------------
	// 관전 모드 여부 
	//----------------------------------------
	bool bWatchModeSwitch;
	SI32 siWatchModeDetailX;
	SI32 siWatchModeDetailY;

	//----------------------------------------
	// 작업 중지 여부 스위치(광산, 농장)
	//----------------------------------------
	bool bStopDoingSwitch;

	bool bPassFirstOrder; 

public:
	//-----------------------------------
	// 키 입력. 
	//-----------------------------------
	cltKeyManager	clKeyManager;

	//----------------------------------------
	// 서피스 
	//----------------------------------------
	LPDIRECTDRAWSURFACE7		lpBackScreen;	// 게임이 그려질 화면 뒤의 서피스. 


	//----------------------------------------
	// 출력 순서 정렬 
	//----------------------------------------
	CDrawSequence* pclDrawSequence;

	cltMusic* pclMusic;

	cltGlobalImgFile* pclGImg;

	//---------------------------------------
	// 서버와의 마지막 교신 시간. 
	//---------------------------------------
	DWORD	dwLastReceiveFromServerClock;


	//--------------------------------------
	// 서버 점검 중에도 접속할 권한이 있는가?
	//---------------------------------------
	bool bLoginWhileServerDownSwitch;

	//--------------------------------------
	// 마지막으로 접속했던 서버가 어디인가?
	//--------------------------------------
	TCHAR szLastConnectServerName[20];

	//----------------------------------------
	// 클라이언트 영역 크기. 
	// 화면 사이즈보다 커야 부드러운 스크롤이 구현가능하다. 
	//----------------------------------------
	SI32 siClientScreenXsize;
	SI32 siClientScreenYsize;
	SI32 siClientScreenHalfSize;

	// 화면에 찍을 지도를 임시로 저장하는 곳 
	UI08* pScreenMapBuffer;
	LPDIRECTDRAWSURFACE7     lpScreenMapBuffer;

	SHORT	GameMouseX;
	SHORT	GameMouseY;


public:
	SI32 siSelectedOrder;				// 선택된 명령 
	SI16 siSelectedOrderPara1;			// 선택된 명령의 부속 파라메타. 
	SI16 siSelectedOrderPara2;			// 선택된 명령의 부속 파라메타.

public:
	SI32 ObjectTempSelectedSwitch;

	//private: // 05년 03월 22일에 광명이 바꿈(임시로)
public:
	// 캐릭터 선택관련 변수 //
	SI32    MyCharUnique;
	SI32	Char_flag;

	SI32 CurrentCharInfoList;
	// 서버로 부터 받을 사용자 캐릭터들의 정보를 저장할 공간.
	cltCharListInfo clCharListInfo[MAX_TOTAL_CHAR_PER_USER];

public:
	SHORT   IDPara1; // 각 아이디 값의 파라메타 
	SHORT   IDPara2; // 각 아이디 값의 파라메타 

	cltUnderMouseObject* pclUnderMouseObject;		// 마우스 포이터 밑에 있는 캐릭터. 


	HWND SelectCharDlg;					// 캐릭터 선택 다이얼로그 박스 핸들. 
	HWND MakeCharDlg;					// 캐릭터 만들기 다이얼로그 박스 핸들. 
	HWND SelectWorldListDlg;			// 월드 리스트 선택 다이얼로그 박스 핸들. 

	CResidenceCheck				*m_pResidenceCheck; // 주민등록번호 뒷자리 입력
	//CSiegeWinDlg				*m_pSiegeWinDlg; // 공성 성공 결과 알림창

	// 웹 이벤트
//	CEventNotice				*m_pEventNotice; // 선물 이벤트 결과창
//	CWebEventDlg				*m_pWebEventDlg;
//	cltHtmlDlgManager			*m_pHtmlDlgManager;

	// 성공한 이벤트 저장
	SI32						m_siSucceedEventType;
	
	// 고스톱, 포커 나누어서 이벤트 저장 (PCK - 2008.09.18)
	// 1 = 포커, 2 = 고스톱
	SI32						m_siGoStopPockerEventType;

	CFriendLoginDraw			*m_pFriendLoginDraw;
	CResidentLoginDraw			*m_pResidentLoginDraw;
	CVillageWarNotice			*m_pVillageWarNotice;
	CNewYearEventNotice			*m_pNewYearEventNotice;

//	CAbuseFilter				*m_pCAbuseFilter;
	//CNFarmingDlg				*m_pNFarmingDlg;

	CDrawLogo					*m_DrawLogo;

	cltDate clClientDate;				// 현재 시간. 

	_SYSTEMTIME sTime;					// 서버의 실제시간

	BOOL ScrollProcSwitch;				// 스크롤 변수. 
	

	//-------------------------------------
	// 인터페이스 관련
	//-------------------------------------
	CInterfaceMgr				*m_pInterfaceMgr;
#ifdef _SAFE_MEMORY
	NSafeTArray<CDialogController*, DIALOG_END>		m_pDialog;
#else
	CDialogController			*m_pDialog[ DIALOG_END ];
#endif

	//-------------------------------------
//	cltPCRoomIntro				*m_pPCRoomIntro;

public:

	//-------------------------------------
	// 꼼지 최신글 읽어오기
	//-------------------------------------
//	cltMiniHomeUpdates*	pclMiniHomeUpdates;

	//-------------------------------------
	// 한게임 PICA매니져 체크
	//-------------------------------------
	cltPICAManagerCheck*	m_pclPICAManagerCheck;

	//-------------------------------------
	// [진성] 팝업창 띄우기 체크. 2008-5-27
	//-------------------------------------
	cltPopupCheck*			m_pPopupCheck;
	NUpdateTimer			m_kUpdateTimer_PopupCheck;
	
	//-------------------------------------
	// 채팅 말풍선 관련. 
	//-------------------------------------
	CDrawChatSurface*	pclDrawChatSurface;

	//-------------------------------------
	// 캐릭터 이름 출력 관련. 
	//-------------------------------------
	CDrawCharNameSurface*	pclDrawCharNameSurface;

	//------------------------------------
	// DB이상 여부.
	//------------------------------------
	bool bDBFailSwitch;

	//------------------------------------
	// 이동 표식. 
	//------------------------------------
	CClickEffect*	pclClickManager;

	//--------------------------------
	// 주인공 캐릭터 정보 백업 장치. 
	//--------------------------------
	bool bReceiveMyCharAllInfoSwitch;		// 주인공 캐릭터의 정보를 서버로부터 모두 받았는지 여부. 
	bool bBackUpSwitch;						// 주인공 캐릭터의 정보를 클라이언트에서 별도 백업 받았는지 여부, 
	cltHeroDataBackup* pclHeroDataBackup;

	//--------------------------------
	// 게임 약관. 
	//--------------------------------
//	cltGameContract* pclGameContract;


	//-------------------------------
	// 불가사리 사냥 이벤트. 
	//-------------------------------
	bool bHuntingEventSwitch;

	//-------------------------------
	// 모후모후 던젼 이벤트 진행 국가.
	//-------------------------------
	SI16 siMofuMofuEvent_VilliageUnique;

	//-------------------------------
	// 고구려 유적 방어  이벤트. 
	//-------------------------------
	bool bGoguEventSwitch;

	//-------------------------------
	// 오사카성 이벤트 
	//-------------------------------
	bool bOsakaEventSwitch;

	//-------------------------------
	// 강화도 이벤트 
	//-------------------------------
	bool bKanghwadoEventSwitch;

	//-------------------------------
	// 서버와의 연결이 단절되었을때의 스위치 by cyj
	//-------------------------------
	bool bServerNotConnectSwitch;

	//-------------------------------
	// 부활 메세지 수신 여부 스위치 by cyj
	//-------------------------------
	bool bReviveRequestSwitch;

	//------------------------------
	// 마지막으로 이모티콘 보낸시간..제한필요한다.
	//------------------------------
	clock_t	LastSendEmoticonClock;

	//-------------------------------
	// 등급표시
	//-------------------------------
	NUpdateTimer	m_kTimer_ShowRank_Interval;	// 게임 중 1시간에 한번씩 화면에 출력
	NUpdateTimer	m_kTimer_ShowRank_Time;		// 3초동안 화면에 표시
	SI08			m_siDrawMode;				// 표시 위치 0: 우상단, 1: 미니맵 아래

	//-------------------------------
	// 게임가드 정기적으로 체크
	//-------------------------------
	NUpdateTimer	m_kTimer_Check_GameGuard_Interval;	// 게임 중 1시간에 한번씩 화면에 출력

	//-------------------------------
	// 길드전 관련 변수
	//-------------------------------
	cltGuildWarClient*	pclGuildWarClient;

	//-------------------------------
	// 도움말 
	//-------------------------------
	cltHelpManager*		pclHelpManager;

	// 로그인공지창
	bool bLoginNotice;

	CNPrivateChatDlgMgr		*m_pNPrivateChatDlgMgr;

	//---------------------------------
	// 이모티콘
	//---------------------------------
	CEmoticonInfo		clEmoticonInfo;

	//
	CBeginnerText*		m_pBeginnerText;
	CLevelUpText*		m_pLevelUpText;
	CGameTip*			m_pclGameTip;

	cltMarkManager*		m_pMarkManager;


	//  [7/1/2009 종호_ITEMPERIOD] 기간제 아이템 매니저
	CItemPeriodMgr*		m_pItemPeriodMgr;

	//	축제 - 가위바위보
	CGawiBawiBoMgr_Client*	m_pGawiBawiBoMgr;

	// [지연] - 복불복 매니저
	CClient_BBBMgr* m_pClient_BBBMgr;

	// [기형] - 배틀로얄
	CBattleRoyalClient* m_pclBattleRoyal;

	// [지연] - 초보자 파티 퀘스트 시스템(파티매칭).
	CClient_PartyMatchingMgr* m_pClient_PartyMatchingMgr;

	CPartyQuestMgr_Client*	m_pPartyQuestMgr;

	//---------------------------------
	// 캐릭터 조합 이미지 해제 마지막 체크 시간
	//---------------------------------
	clock_t				m_CCDLastCheckClock;
	//---------------------------------
	// 필드오브젝트 삭제
	//---------------------------------
	clock_t				m_DeleteFOLastCheckClock;
	//---------------------------------
	// HeartBeat
	//---------------------------------
	clock_t				m_SentHeartBeatClock;

	//-----------------------
	// 인증부분 암호화 dhm-
	//-----------------------
	CGameClient*		m_pClientDiffHellMan;

	//-----------------------
	// 피로방지 시스템
	//-----------------------
	UI08 uiTiredState;

	//--------------------------
	// 이름 과 그림파일 인덱스
	//--------------------------
#ifdef _SAFE_MEMORY
	NSafeTArray< NTCHARString64, MAX_NPCKINDFORRANKMARK >		m_NPCName;
	NSafeTArray<SI32, MAX_NPCKINDFORRANKMARK>					m_NPCImgIndex;
#else
	TCHAR m_NPCName[MAX_NPCKINDFORRANKMARK][64];
//	SI32 m_NPCKindIndex[MAX_NPCKINDFORRANKMARK];
	SI32 m_NPCImgIndex[MAX_NPCKINDFORRANKMARK];
#endif

	//--------------------------
	// 멀티클라이언트 체크
	//--------------------------
	cltCheckProcess*	m_pclCheckProcess;

	BYTE				m_bufCompress[ sizeof(cltMsg) ];	// 서버와의 통신을 위해 암호화된 데이터를 저장할 버퍼. 

public:


	cltClient( TCHAR* appname, HWND hwnd , HINSTANCE hinst, SI32 gamemode, SI32 defaultservicearea, TCHAR* pszHanAuth_GameString, SI32 runmode);
	virtual ~cltClient() ;

	virtual void Create();
	void CreateBaseInterface();

	void CreateInterface( SI16 DialogID, bool ShowHideMethod = false );
	void DestroyInterface( CControllerMgr *pDialogController );

	void InitMembers();


	//	const unsigned char *GetThunderPalette()const{return ThunderPalette;}

	void SetTitleText();

public:
	//Client.cpp
	// 화면 출력 관련.

	void SetFlashWindowNotice( bool bNotice );

	void DrawGovernMentRankMark(SI32 siDrawMode);

	// 타임 카운트 하는 화면을 보여 준다 .
	void DrawTimeCount(SI32 siTime,LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry);	// siTimeCount를 한다.

	// 라운드를 보여준다
	void DrawRound(SI32 siRound,LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry);// 라운드를 보여준다.


private:

public:

	// Client.cpp
	BOOL InBufferMessageOperation(cltMsg* pclMsg, SI32 id);

	BOOL ModalessDlgProc(MSG* pmsg);

	// 에러 박스를 사용하여 표시되는 메시지. 
	void OperateByServerResponseValueGlobal(const cltServerResponseValue* pclreturn);

	// 서버로부터의 결과값을 분석하여 적절한 곳에 통보한다. 
	BOOL OperateByServerResponseValue(const cltServerResponseValue* pclreturn);

	//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
	BOOL OperateByServerResponseCharValue(const cltServerResponseCharValue2* pclreturn);

	//----------------------------------------------
	// MyDPID
	//----------------------------------------------
	// dpid가 사용자인지 확인한다.
	BOOL IsUser(SI32 charunique)
	{
		if(MyCharUnique && MyCharUnique == charunique)return TRUE;
		return FALSE;
	}


	// 명령 처리 함수. 
	BOOL OrderOperation(cltOrderData order);
	BOOL OrderAction();
	void SetSiSelectedOrder(SI32 seletedorder) { siSelectedOrder = seletedorder; };

	// 맵 에디터 관련 
	virtual void MapEditAction(){}


	// 채팅 
	// 서버로 채트 메시지를 보낸다. 
	BOOL SendChatMessage(SI16 chatmode, TCHAR* pmsg,SI32 systemtxt = 0 );

	// DirectX.cpp 
	// 게임에 사용될 팔레트를 불러온다. 
	//	void LoadPalette();
	//	void SetGamePalette(const unsigned char *pal);

	void UpdateScreen(HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, bool waitrefresh);

	// 클라이언트로 하여금 FrontServer에 접속하게 만든다. 
	bool InitConnectToFrontServer();

	// Front Server와 접속을 시도한다. 
	SI32 ConnectToFrontServer();

	// 프론트 서버에 군주 DB사용자를 웹DB로 이전하라고 요청한다. 
	void RequestJoinGame();

	// PCK : IOVATION 인증 
	bool Registeriovation();

	// 클라이언트로 하여금 GameServer에 접속하게 만든다. 
	bool InitConnectToGameServer();

	// Game Server 와 접속을 시도한다. 
	SI32 ConnectToGameServer();

	void GetMacAddress( TCHAR *szAddress , SI16 txtSize );
	void GetMacAddress( SOCKET hSocket, TCHAR *szAddress , SI16 txtSize );

	COLORREF	GetColorByPrice( GMONEY price );
	
	TCHAR* GetServerNameFromUnique(SI16 ServerUnique);

	//----------------------------------------
	// SelectChar 
	// SelectChar.cpp
	//----------------------------------------
	BOOL SelectChar();

	// 캐릭터 선택 다이얼로그 상자의 캐릭터 정보란에 slot에 해당하는 캐릭터의 정보를 보여준다. 
	BOOL SetSelectCharInfoText(HWND hDlg, SI32 slot );

	SI32 GetCurrentCharInfoList()
	{
		if ( CurrentCharInfoList < 0 || CurrentCharInfoList > MAX_CHAR_PER_USER ) return -1 ;
		return CurrentCharInfoList ;
	}
	cltCharListInfo* GetCharListInfo(SI32 index)
	{
		if ( index < 0 || index > MAX_TOTAL_CHAR_PER_USER ) return NULL ;
		return &clCharListInfo[index];
	}

	WORD GetPrimaryLanguage( HKL hKeyboardLayout );
	WORD GetSubLanguage( HKL hKeyboardLayout );
	DWORD GetCharSet();
	BOOL IsCorrectOS();

	//--------------------------------------------
	// SelectWorldList
	//--------------------------------------------
	// 적절한 월드가  선택되었는지 확인한다. 
	bool SelectWorldList();

	bool CreateSelectWorldListDlg(bool bShowBanner);

	// Draw.cpp
	void  GameDraw();   // 그리기 관련 부분 

	bool GameDrawHeroViewOperate();
	bool GameDrawMap();
	bool GamaDrawInterface();
	void GameDrawUpdateScreen();

	// MouseOperation.cpp
	// 키보드 처리를 포함한 루틴 
	BOOL GameOverHead();
	// 지도상에서 마우스의 위치와 움직임을 체크한다. 
	BOOL GetMouseMode();
	// 메뉴 입력등을 처리한다. 
	void OperateMenu();
	BOOL SetMousePos(SI32* psix, SI32* psiy);
	// 마우스 버튼에 의한 자동 명령을 처리한다. (왼쪽 마우스 버튼)
	bool OperateAutoLeftOrder();
	// 마우스 버튼에 의한 자동 명령을 처리한다. (오른쪽 마우스 버튼)
	bool OperateAutoRightOrder();
	bool OperateAutoDoubleLeftOrder();

	void ShowMessageForItemEquipFail( SI32 siFailCode );


	//----------------------------------------
	// Char Information Update
	//----------------------------------------
	BOOL SendCharUpdate(SI32 charunique, bool bmode){return FALSE;}

	//---------------------------------------
	// GameData 정리 출력
	//---------------------------------------
	// 던젼별 몬스터 정보를 텍스트로 정리해서 출력한다.
	void MakeDungeonDataTxt();

	void MakeNPCDataTxt();

	void MakeMonsterDataTxt();

	void MakeMonsterItemDataTxt();

	void MakeItemMakeDataTxt();

	void MakeItemMaterialTxt();

	void MakeMonsterBasicStatusDataTxt();

	//---------------------------------------
	// 클라이언트 실행 명령어
	//---------------------------------------
	void ExecFile( TCHAR* pszCommand );

	//----------------------------------------
	// SMall Map
	//----------------------------------------
	// 작은 지도에서 사용자의 위치를 설정한다. 
	void SetSmallMapUserPos(cltSmallMap* pclSmallMap);

	
	//---------------------------------------
	// Hero View Operate
	//---------------------------------------
	void HeroViewOperate();

	//---------------------------------------
	// User Char List
	//---------------------------------------

	// 사용자의 캐릭터 정보를 리스트에 추가한다. 
	// 이렇게 확보된 캐릭터 정보는 사용자로부터 선택될 수 있다.
	BOOL PutCharList(cltCharListInfo* pcharinfo);

	// 캐릭터 정보를 삭제한다.
	BOOL DeleteCharList(cltGameMsgResponse_DelUserChar* pcharinfo);
	//케릭터삭제유예기간
	BOOL PreDeleteCharList(cltGameMsgResponse_PreDelUserChar* pcharinfo);


	// 캐릭터 리스트에 3개의 캐릭터 정보가 모두 들어왔는지 확인한다.
	BOOL IsReadyCharList();

	//------------------------------------------
	// CharStatus
	//------------------------------------------
	BOOL CreateCharStatusDlg();


	//-------------------------------------------
	// Skill
	//-------------------------------------------
	// 스킬 다이얼로그 박스를 만든다. 
	BOOL CreateSkillDlg(SI32 charunique);

	BOOL CreateStrDlg(SI32 charunique, cltStructureMngBasic* pclmgr, SI32 dlgitem, DLGPROC dlgproc);

	//------------------------------------------
	// Stock
	//------------------------------------------
	// 주가 정보를 일정 시간마다 화면에 보여준다. 
	void ShowStockPrice();


	//------------------------------------------
	// CityHall
	//------------------------------------------
	BOOL CreateCityHallDlg(SI32 charunique);

	//------------------------------------------
	// SetChar
	//-------------------------------------------
	SI32					siSetCharBufferIndex;
	cltMakeCharInfo			cltSetCharBuffer[MAX_SETCHAR_RESERVE_NUMBER];
	// 캐릭터를 안전할 때 만들 수 있도록 버퍼에 넣는다. 
	BOOL PushChar(cltMakeCharInfo* pclMsgLogIn);
	BOOL PopChar(cltMakeCharInfo* pclMsgLogIn);
	BOOL SetCharReservedChar();
	// 생산 예약된 캐릭터에서 특정 캐릭터를 삭제한다. 
	bool DeleteReservedChar(SI32 charunique);

	//npc 의 이름을 text 파일에서 얻어온다
	void NPCMgr();

	
	inline CGawiBawiBoMgr_Client* GetGawiBawiBoMgr()
	{
		if(m_pGawiBawiBoMgr != NULL)
			return m_pGawiBawiBoMgr;
		else
			return NULL;
	}
	
	//-----------------------------------------------
	// 사운드 관련 
	//-----------------------------------------------
	BOOL PushEffect(SI32 effect, SI32 x, SI32 y);
	void PopEffect();


	//----------------------------------------------
	// 스크롤 관련 
	//----------------------------------------------
	void SetScrollProcSwitch(BOOL mode);
	void ScrollAction();

	//--------------------------------------
	// Action
	//---------------------------------------
	SHORT GameAction();

	BOOL ClientAction1();
	BOOL ClientAction2();

	//----------------------------------------
	// WinSock
	//----------------------------------------
	void InitNetwork();
	void WinSockConnect();
	virtual void WinSockAction();
	// 세션캐릭터를 pclCM과 연결한다. 
	void  SetSessionChar();

	// 프런트 서버로 메시지를 보낸다. 
	BOOL SendFrontServerMsg(sPacketHeader* psendmsg);

	// 메시지를 서버로 보낸다. 
	BOOL SendMsgToServer(sPacketHeader* pmsg);
	void FrontServerOperation(sPacketHeader *pPacket);

	bool FrontServerAction();
	bool GameServerAction();

	void NetworkOperation();

	void Init();												// 초기화. 
	TSpr* GetGlobalSpr(SI32 index);								// GlobalImgFile

	//--------------------------------------------
	// 툴바 제작 관련.
	//--------------------------------------------
	// 툴바 관련 
	BOOL CreateToolBar();
	TCHAR* GetToolTip(SI32 idcommand);

	BOOL GetClientSize(RECT* prect);							// 윈도우 사이즈 관련 함수 

	//-------------------------------------------
	// RX, RY
	//------------------------------------------
	void SetScreenRXY(SI32 rx, SI32 ry)
	{
		siRX	= rx;
		siRY	= ry;
	}

	SI32 GetScreenRX()const {return siRX;}
	SI32 GetScreenRY()const {return siRY;}

	SI32 GetDrawStartX(){return GetScreenRX() + MAP_TILE_XSIZE;}
	SI32 GetDrawStartY(){return GetScreenRY() + MAP_TILE_YSIZE;}


	//-----------------------------------------
	// 수리 모드 
	//-----------------------------------------
	void SetRepairSwitch(bool mode)
	{
		bRepairSwitch = mode;
	}

	bool GetRepairSwitch()const
	{
		return bRepairSwitch;
	}

	//-----------------------------------
	// WM_COMMAND메시지 처리 함수. 
	//-----------------------------------
	virtual bool DoMsg_WM_COMMAND(WPARAM wParam);


	//----------------------------------
	// 게임 속도 조정. 
	//---------------------------------
	// 게임의 속도를 일정하게 만든다. 
	BOOL GameDelay();

	double m_dwNextFrameTick;
	double m_dwDelayTick;

	// 파일에서 Server의 IP주소를 얻어온다.
	bool GetServeIPInFile(TCHAR* ip1, SI16 ip1txtSize, TCHAR* ip2, SI16 ip2txtSize);

	//----------------------------------
	// 디렉토리 경로 받아오기
	//---------------------------------
	bool GetDirectoryPath( TCHAR *DirectoryPath );

	SI32 GetScreenXSize();
	SI32 GetScreenYSize();
	SI32 GetScreenStatusBarYSize();

	void SetMsgBox( stMsgBox *pMsgBox, BYTE *pData, SI16 DataLen );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );


	//---------------------------------------
	// CLient-WinMain.cpp
	//--------------------------------------
	// WInMain안에서의 클라이언트 기능. 
	void ClientWinMain();

	void GsInitConnectFrontServer();
	void GsConnectFrontServer();
	void GsInitCharList();

	void GsInitConnectGameServer();
	void GsConnectGameServer();

	void GsCharList();
	void GsGame();
	void GsQuit();
	void GsInitSelectWorldList();
	void GsSelectWorldList();

	void GsShowGovernment();

	// APEX-
	void DoMsg_GAMEMSG_RESPONSE_APEXDATA(cltMsg* pclMsg, SI32 id);
	// -APEX

	void ShowHelpWindow( TCHAR* szFileName );

	//------------------------------------------------
	// Client-Person.cpp
	//------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_INCLIFE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_INCMANA(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_REPONSE_INCHUNGRY(cltMsg* pclMsg, SI32 id);	// 음식 섭취로 인한 포만감 증가. 
	void DoMsg_GAMEMSG_RESPONSE_HIT(cltMsg* pclMsg, SI32 id);		// 맞았다. 
	void DoMsg_GAMEMSG_RESPONSE_GETEXP(cltMsg* pclMsg, SI32 id);		// 경험치를 얻었음을 처리
	void DoMsg_GAMEMSG_RESPONSE_SPECIALQUESTREWARD(cltMsg* pclMsg, SI32 id);		// 특수 임무 완수후에 보상된 내용을 통보받음. 
	void DoMsg_GAMEMSG_RESPONSE_SPECIALQUESTMADE(cltMsg* pclMsg, SI32 id);		// 특수 임무 설정되었음을 통보받음. 
	void DoMsg_GAMEMSG_RESPONSE_QUESTUSERINFO(cltMsg* pclMsg, SI32 id);			// 특정 유저의 퀘스트 정보를 얻어온다. 
	void DoMsg_GAMEMSG_RESPONSE_HORSENAMECHANGE(cltMsg* pclMsg, SI32 id);		// 말 이름 변경하기. 
	void DoMsg_GAMEMSG_RESPONSE_CLEAR_SPECIALWORD(cltMsg* pclMsg, SI32 id);		// 단어 조합 완성 수를 얻어온다.
	void DoMsg_GAMEMSG_RESPONSE_SETPERSONNAMINGINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETPPERSONCURRENTNAMING(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_PRIZE_GAMEEVENT(cltMsg* pclMsg, SI32 id);		// 이벤트 결과를 얻어온다.
	//void DoMsg_GAMEMSG_RESPONSE_COUNTATTACK_INFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_EVENTNPC(cltMsg* pclMsg, SI32 id);				// 외부 이벤트 천군
	void DoMsg_GAMEMSG_RESPONSE_GETGREETING(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GET_WEAPONSKILLCOST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SET_WEAPONSKILL(cltMsg* pclMsg, SI32 id);

	//--------------------------------------------------------
	//KHY - 0822 - 해피빈 퀘스트 추가.
	void DoMsg_GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_NO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_OK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HAPPYBEANQUEST_REWARD(cltMsg* pclMsg, SI32 id);
	//--------------------------------------------------------

	// [지연] 군주데이 미션 완료에 대한 보상을 알림
	void DoMsg_GAMEMSG_RESPONSE_GOONZUDAYMISSION_CLEARREWARD(cltMsg* pclMsg, SI32 id);

	// 미션퀘스트 시작정보 받음.
	void DoMsg_GAMEMSG_RESPONSE_MISSIONQUEST_START(cltMsg* pclMsg, SI32 id);		

	//KHY - 0906 - 게임방 이벤트 쿠폰.
	void DoMsg_GAMEMSG_RESPONSE_EVENTCOUPON(cltMsg* pclMsg, SI32 id);
	//옥션이벤트 쿠폰 [2007.09.10 손성웅]
	void DoMsg_GAMEMSG_RESPONSE_AUCTIONEVENTCOUPON(cltMsg* pclMsg, SI32 id);

	//--------------------------------------------------------
	
	//--------------------------------------------------------
	// 캐릭터의 특정 정보 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_QUEST(cltMsg* pclMsg, SI32 id);		// Person의 퀘스트 정보 전체를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_PERSON_HORSE(cltMsg* pclMsg, SI32 id);		// Person의 말 정보 전체를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_PERSON_IP(cltMsg* pclMsg, SI32 id);			// Person의 IP정보를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_ALLITEM(cltMsg* pclMsg, SI32 id);		// 모든 물품 정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_SKILL(cltMsg* pclMsg, SI32 id);		// 모든 기술 정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_ONESKILL(cltMsg* pclMsg, SI32 id);	// 한가지 기술 정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_HEALTH(cltMsg* pclMsg, SI32 id);		// 건강 정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_STOCKORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PEERSON_MARKET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PERSON_STOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PERSON_BANK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PERSON_WORLDMONEY(cltMsg*pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_PERSONITEMINFO(cltMsg* pclMsg, SI32 id);		// 특정 아이템 한개의 정보를 받는다. 

	void DoMsg_GAMEMSG_RESPONSE_PERSONENCHANTITEMINFO(cltMsg* pclMsg, SI32 id);	//

	void DoMsg_GAMEMSG_RESPONSE_CHARUPDATE(cltMsg* pclMsg, SI32 id);			// 캐릭터 기본 정보를 업데이트 한다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_GUILTY(cltMsg* pclMsg, SI32 id);		// Person의 죄 정보를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_CHANGESTATUSINFO(cltMsg* pclMsg, SI32 id);	// 캐릭터의 상태 변경 정보를 받음 
	void DoMsg_GAMEMSG_RESPONSE_PERSON_INNERSTATUS(cltMsg* pclMsg, SI32 id);	// InnerStatus를 전달한다.

	void DoMsg_GAMEMSG_RESPONSE_PERSONNAME_FROMPERSONID(cltMsg* pclMsg, SI32 id);	// PersonID를 가지고 Person 이름을 받아옴
	void DoMsg_GAMEMSG_RESPONSE_RIGHTMOUSE_USERLIST_NAME(cltMsg* pclMsg, SI32 id);  // 마우스 오른쪽 키를 눌렀을때 팝업되는 유저 리스트에 필요한 이름을 얻어온다.
	void DoMsg_GAMEMSG_RESPONSE_TIRED_STATUS(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_DONATEVILLAGEMONEY(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_DONATEMEMBERLIST(cltMsg* pclMsg, SI32 id );

	//--------------------------------------------------------


	void DoMsg_GAMEMSG_RESPONSE_WARPEFFECT(cltMsg* pclMsg, SI32 id);			// 유료아이템을 이용하여 워프 했음을 알린다. 
	void DoMsg_GAMEMSG_RESPONSE_HORSEFREE(cltMsg* pclMsg, SI32 id);			// Person의 말을 풀어준다.
	//KHY - 1001 - 기승동물 5마리 보유 수정.
	void DoMsg_GAMEMSG_RESPONSE_CHANGEHORSE(cltMsg* pclMsg, SI32 id);			// Person의 말을 바꾼다.
	void DoMsg_GAMEMSG_RESPONSE_ALLCHARINFO(cltMsg* pclMsg, SI32 id);		// Person의 모든 정보를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_ALLCHARINFOONE(cltMsg* pclMsg, SI32 id);	// Person의 정보1를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_ALLCHARINFOTWO(cltMsg* pclMsg, SI32 id);	// Person의 정보2를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_CHANGEMONEY(cltMsg* pclMsg, SI32 id);		// Person의 소지금 정보를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_USERID(cltMsg* pclMsg, SI32 id);				// Person의 군주 계정을 받는다.(미니홈페이지 때문에)
	void DoMsg_GAMEMSG_RESPONSE_WARKILL(cltMsg* pclMsg, SI32 id);			// 전과를 통보 받는다.
	void DoMsg_GAMEMSG_RESPONSE_JIM(cltMsg* pclMsg, SI32 id);				// Person의 찜 정보를 통보받는다.
	void DoMsg_GAMEMSG_RESPONSE_HOMEVILLAGE(cltMsg* pclMsg, SI32 id);		// Person의 주소지 정보를 통보받는다.
	//	void DoMsg_GAMEMSG_RESPONSE_SCHOOL(cltMsg* pclMsg, SI32 id);			// Person의 학교 정보를 받는다. 
	//	void DoMsg_GAMEMSG_RESPONSE_SCHOOLUSERNUM(cltMsg* pclMsg, SI32 id);	// 학교 사용자 수를 통보 받음.
	void DoMsg_GAMEMSG_RESPONSE_PLAYTIME(cltMsg* pclMsg, SI32 id);			// Person의 플레이 시간을 구한다. 
	void DoMsg_GAMEMSG_RESPONSE_GENERALEFT(cltMsg* pclMsg, SI32 id);			// 지휘효과를 얻는다. 
	void DoMsg_GAMEMSG_RESPONSE_MAKEITEMDELAY(cltMsg* pclMsg, SI32 id);		// 물품 생산에 필요한 지연 시간을 얻어온다.
	void DoMsg_GAMEMSG_RESPONSE_ROULETTEGAME_RESULT(cltMsg* pclMsg, SI32 id);// 박대박 게임 결과를 얻는다
	void DoMsg_GAMEMSG_RESPONSE_QUESTEFFECT(cltMsg* pclMsg, SI32 id);		// 퀘스트 이펙트 메시지
	void DoMsg_GAMEMSG_RESPONSE_SETFATHER(cltMsg* pclMsg, SI32 id);			// 스승님 설정. 
	void DoMsg_GAMEMSG_RESPONSE_SETSHOWPUCHIKOMI(cltMsg* pclMsg, SI32 id);			// 푸치코미 설정.
	void DoMsg_GAMEMSG_RESPONSE_PERSONPRIVATEINFO(cltMsg* pclMsg, SI32 id);	// 사용자 개인정보 통보.
	void DoMsg_GAMEMSG_RESPONSE_CHILDINFO(cltMsg* pclMsg, SI32 id);			// 스승  정보 통보. 
	void DoMsg_GAMEMSG_RESPONSE_FATHERAPPLYERLIST(cltMsg* pclMsg, SI32 id);	// 스승 신청자 정보 통보. 
	void DoMsg_GAMEMSG_RESPONSE_UPDATECHILDLIST(cltMsg* pclMsg, SI32 id);	// 제자 리스트 정보 통보. 
	void DoMsg_GAMEMSG_RESPONSE_RENOUNCEPUPIL(cltMsg* pclMsg, SI32 id);		// 스승이 제자를 짜른 정보 통보
	void DoMsg_GAMEMSG_RESPONSE_OVERTAKETEACHERLEVEL(cltMsg* pclMsg, SI32 id);	// 제자가 스승레벨을 따라잡았다
	void DoMsg_GAMEMSG_RESPONSE_REQUEST_SETFATHER(cltMsg* pclMsg, SI32 id);			// 스승님 설정. 
	void DoMsg_GAMEMSG_RESPONSE_PVPRESULT(cltMsg* pclMsg, SI32 id);			// 스승님 설정. DoMsg_GAMEMSG_RESPONSE_PVPRESULT

	void DoMsg_GAMEMSG_RESPONSE_STOCKBIDVILLAGELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STOCKBIDDERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CANCEL_STOCKORDER(cltMsg* pclMsg, SI32 id);  // 매수주문 취소

	void DoMsg_GAMEMSG_RESPONSE_PERSONALLSHAPEINFO(cltMsg* pclMsg, SI32 id);	// PERSON의 외향정보를 통보받음
	void DoMsg_GAMEMSG_RESPONSE_ADDINFO(cltMsg* pclMsg, SI32 id);			// 캐릭터의 추가정보를 받음 
	void DoMsg_GAMEMSG_RESPONSE_CHANGESUMMONSTATUS(cltMsg* pclMsg, SI32 id);	// 내가 가진 소환수의 정보가 변경되었다.


	void DoMsg_GAMEMSG_RESPONSE_CHANGEMAGIC(cltMsg* pclMsg, SI32 id);		// 캐릭터의 MAGIC을 변경된다.
	void DoMsg_GAMEMSG_RESPONSE_EVENTINFO(cltMsg* pclMsg, SI32 id);			// 캐릭터의 이벤트 정보를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_NEIGHBOURHORSE(cltMsg* pclMsg, SI32 id);		// Person의 말 정보를 받아서 업데이트. (이웃의 말 정보) 
	void DoMsg_GAMEMSG_RESPONSE_WINWARPRIZE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_STOCKDISTRIBUTIONSET(cltMsg* pclMsg, SI32 id);	// 주식 공모 알림
	void DoMsg_GAMEMSG_RESPONSE_STOCKDISTRIBUTIONORDER(cltMsg* pclMsg, SI32 id);	// 주식 공모 참여 결과

	// NPC고용 
	void DoMsg_GAMEMSG_RESPONSE_HIRENPC(cltMsg* pclMsg, SI32 id);			
	void DoMsg_GAMEMSG_RESPONSE_NPCPAYMENT(cltMsg* pclMsg, SI32 id);
	
	void DoMsg_DoMsg_GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC(cltMsg* pclMsg, SI32 id );
		
	// Client-SystemNPC.cpp
	void DoMsg_GAMEMSG_RESPONSE_SELLNPC(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BUYNPC(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NPCSELLITEMINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRESELLTOFOREIGNMERCHANT(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_INVESTMENTGETINFO(cltMsg* pclMsg, SI32 id);

	// 교역 상인 NPC ( 청, 일본 )
	void DoMsg_GAMEMSG_RESPONSE_FOREIGNMERCHANTNPCBUYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEMINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FOREIGNMERCHANTBUYCASHITEM(cltMsg* pclMsg, SI32 id);


	// 추천 포인트 상인 NPC
	void DoMsg_GAMEMSG_RESPONSE_GETRECOMMANDPOINT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BUYRECOMMANDPRODUCT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RECOMMANDPOINTLIST(cltMsg* pclMsg, SI32 id);


	// 마을 이벤트 NPC
	void DoMsg_GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC(cltMsg* pclMsg, SI32 id);

	// 무역 상인 NPC
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMFROMPERSON(cltMsg* pclMsg, SI32 id);

	// 상인이 말 구입
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTHORSEBUYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYHORSEFROMPERSON(cltMsg* pclMsg, SI32 id);

	// 상인이 아이템을 구입하는 정보
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEMINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYITEM(cltMsg* pclMsg, SI32 id);


	//--------------------------------------------------------------------
	// 개인 거래
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ACCEPT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_CANCEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_DELLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_DELITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDPRICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDSTOCK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDHOUSEUNIT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_ADDHORSE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_MYACCEPT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_OTHERACCEPT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_END(cltMsg* pclMsg, SI32 id);
	//--------------------------------------------------------------------



	//--------------------------------------------------------------------
	// 파티 시스템
	//void DoMsg_GAMEMSG_RESPONSE_PARTY_CREATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_CREATEINVITE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_CREATEJOIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_INVITE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_INVITEREJECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_JOIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_DELETE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_ADDUSER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_DELUSER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_FORCEDELUSER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_CHANGELEADER(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_PARTY_NOTIFYEXP(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_PARTY_CHANGEITEMGETPATTERN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_NOTIFYITEMGET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_PARTYUSERPOS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_NOTIFYLEVELUP(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_PARTY_WANTJOIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTY_WANTJOINRESULT(cltMsg* pclMsg, SI32 id);
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	// 친구
	void DoMsg_GAMEMSG_RESPONSE_FRIENDADD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDADDACCEPT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDDEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDLOGIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDLOGOUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDMEMO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FRIENDMEMOSET(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_FRIENDCHANGEGROUP(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_FRIENDCHANGEGROUPNAME(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_FRIENDALLLIST(cltMsg* pclMsg, SI32 id );	
	//--------------------------------------------------------------------


	//--------------------------------------------------------------------
	// 투표
	void DoMsg_GAMEMSG_RESPONSE_VOTEKINGAVAILABLE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VOTEKINGSELECTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VOTEKINGAGGREGATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STARTELECTIONSTUMP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ENDELECTIONSTUMP(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_STARTVOTESTATUS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VOTESTATUS(cltMsg* pclMsg, SI32 id);
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	// Daily Quest
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUESTLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUESTSELECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUESTSITUATION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUESTCHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUESTEND(cltMsg* pclMsg, SI32 id);	
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	// Daily Quest2
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2SELECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2SITUATION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2CHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST2END(cltMsg* pclMsg, SI32 id);	
	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	// Daily Quest3
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3SELECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3SITUATION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3CHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DAILYQUEST3END(cltMsg* pclMsg, SI32 id);	
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	// ItemMall
	void DoMsg_GAMEMSG_RESPONSE_ITEMMALLEXISTENCE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ITEMMALLITEMLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ITEMMALLGETITEM(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_NOLIMITTICKET_WARP(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_GET_SEL_PGLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GET_ITEMMALL_ITEMINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GET_MYCASH_INFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ITEMMALL_BUYITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ITEMMALL_GIFTITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CHECK_ACCOUNTID(cltMsg* pclMsg, SI32 id);
	void DoMag_GAMEMSG_RESPONSE_BILLPAGE_URL(cltMsg* pclMsg, SI32 id);
	void DoMag_GAMEMSG_RESPONSE_LOGINPAGE_URL(cltMsg* pclMsg, SI32 id);
	void DoMsa_GAMEMSG_RESPONSE_GACHAPAGE_URL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SET_CHANGEHERO(cltMsg* pclMsg, SI32 id);

	//--------------------------------------------------------------------

	//--------------------------------------------------------------------
	// 개인상점 
	void DoMsg_GAMEMSG_RESPONSE_PERSONALSHOPBANNER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PERSONALSHOPDATA(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STARTPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STOPPERSONALSHOP(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_UPDATEPERSONALSHOPBANNER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_UPDATEPERSONALSHOPDATA(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BUYPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SELLPERSONALSHOP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SEARCHPERSONALSHOPITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MAKEPERSONALSHOP(cltMsg* pclMsg, SI32 id);

	//--------------------------------------------------------------------
	// 소환수 
	//--------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_SUMMONFREE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONNAMECHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONREVIVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONATTACKCHANGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONEVOLVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONSCROLL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMON_SETAUTOHEAL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADEMERCHANTBUYSUMMONINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONTRADE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SELECT_SUMMONCHAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SELECT_SUMMONUSESTON(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BASESUMMONINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONSLOTIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONSLOTOUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMON_TRANSFORM(cltMsg* pclMsg, SI32 id);	//[진성] 소환수 변신.
	
	//---------------------------------------------------------------------
	// 신분 입후보자 정보를 받음. 
	void DoMsg_GAMEMSG_RESPONSE_CANDIDATELIST(cltMsg* pclMsg, SI32 id);

	//---------------------------------------------------------------------


	// Client-Stock.cpp
	void DoMsg_GAMEMSG_RESPONSE_STOCKORDERINFO(cltMsg* pclMsg, SI32 id);
	// 여각에 올려진 사자, 팔자 주문의 정보 통보. 
	void DoMsg_GAMEMSG_RESPONSE_STOCKTRADEINFO(cltMsg* pclMsg, SI32 id);


	//--------------------------------------
	// 시전
	//--------------------------------------
	// Client-House.cpp
	void DoMsg_GAMEMSG_RESPONSE_MYHOUSEUNITLIST(cltMsg* pclMsg, SI32 id);			// 시전의 기본정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITLIST(cltMsg* pclMsg, SI32 id);			// 시전의 기본정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITLIST_DETAIL(cltMsg* pclMsg, SI32 id);	// 시전의 상세정보를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_HOUSECANCELLATIONPENALTYMONEY(cltMsg* pclMsg, SI32 id);	// 해약하는데 얼마를 지불해야 하는지 정보를 받는다.


	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE(cltMsg* pclMsg, SI32 id);	// 고물상 생성
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO(cltMsg* pclMsg, SI32 id);	// 고물 해체 정보 받기
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS(cltMsg* pclMsg, SI32 id);		// 고물 해체
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSNOTIFY(cltMsg* pclMsg, SI32 id);	// 고물 해체 되었음을 주위 사람들에게 알림
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITHORSEIN(cltMsg* pclMsg, SI32 id);		// 사용자의 말을 시전에 넣는다. 
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITHORSEOUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITNAMECHANGE(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// 농업, 어업, 축산업 공통 사용
	//--------------------------------------
	// Client-Agriculture.cpp
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTUREAUTOGAIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTUREGETPRODUCTS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTUREGETPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTUREPAYRENTFEE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTURESTART(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTUREFORNOTRENTMAN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AGRICULTURELEASELEFTTIME(cltMsg* pclMsg, SI32 id);



	//--------------------------------------
	// 낚시
	//--------------------------------------
	// Client-Fishing.cpp
	void DoMsg_GAMEMSG_RESPONSE_FISHINGSTART(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FISHINGGETMARINEPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// 농경
	//--------------------------------------
	// Client-Farming.cpp
	void DoMsg_GAMEMSG_RESPONSE_FARMINGGETPRODUCTS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FARMINGGETPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// 채굴
	//--------------------------------------
	// Client-Mining.cpp
	void DoMsg_GAMEMSG_RESPONSE_MININGGETPRODUCTS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MININGGETPRODUCTSNOTIFY(cltMsg* pclMsg, SI32 id);




	void DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_RECVITEMLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_RECVMSGLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_GETRECORD(cltMsg* pclMsg, SI32 id);

	//[진성] 단체 메시지 보내기. => 2008-8-4
	void DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_USER_LIST_GROUP(cltMsg* pclMsg , SI32 id);	//[진성] 단체 메시지 보내기. 단체로 보낼 유저리스트를 얻음. => 2008-8-4 
	void DoMsg_GAMEMSG_RESPONSE_CHAR_NAME_CHECK(cltMsg* pclMsg , SI32 id);				//[진성] 단체 메시지 보내기. 단체로 보낼 리스트에 추가할 1명의 유저를 얻음. => 2008-8-4 

	//--------------------------------------
	// 유저의 시전물품을 찾는다
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_GET_SAVEUSERITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GET_SAVEUSERITEMLIST(cltMsg* pclMsg, SI32 id);


	void DoMsg_GAMEMSG_RESPONSE_LIST_DELETEALL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_LIST_DELITEM(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// [희영] 시세확인 2008-07-18
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_MARKETCONDITIONS(cltMsg* pclMsg, SI32 id);


	//--------------------------------------
	// 수렵장 
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_HUNTMONSTERFIELDLISTADD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HUNTMONSTERFIELDLISTDEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HUNTMONSTERIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HUNTMONSTEROUT(cltMsg* pclMsg, SI32 id);
	// 수렵장 어드벤티지를 구한다. 
	void DoMsg_GAMEMSG_RESPONSE_HUNTADDRATE(cltMsg* pclMsg, SI32 id);


	//-------------------------------------
	// 객주 
	//-------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_MARKETORDER(cltMsg* pclMsg, SI32 id);



	//--------------------------------------
	// 사복시
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_OUTHORSE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_MYACCOUNT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_SELLHORSE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HORSEMARKET_BUYHORSE(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// 거간 
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_MYORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_SELL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_BUY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REALESTATEMARKET_DETAILINFO(cltMsg* pclMsg, SI32 id);


	//--------------------------------------
	// 장예원
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_MYORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_SELL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONMARKET_BUY(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// 비변사
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_CANCELORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_MYORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_ORDERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_SELL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SUMMONHEROMARKET_BUY(cltMsg* pclMsg, SI32 id);


	//--------------------------------------
	// 농장
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_LAND_UNITLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_LAND_UNITDETAIL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_LAND_APPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_LAND_RENT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_LAND_DELAPPLYER(cltMsg* pclMsg, SI32 id);


	//--------------------------------------
	// 광산
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_MINE_UNITLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MINE_UNITDETAIL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MINE_APPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MINE_RENT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MINE_DELAPPLYER(cltMsg* pclMsg, SI32 id);

	//--------------------------------------
	// 내자시
	//--------------------------------------
	//#if defined(_LEON_FEAST_EFFECT)
	void DoMsg_GAMEMSG_RESPONSE_FEAST_USEEFFECT(cltMsg* pclMsg, SI32 id); // 폭죽이펙트 사용하기
	//#endif
	//[진성] 파티홀.
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_TOTAL_INFO(cltMsg* pclMsg, SI32 id);									// 파티장 전체 정보.	=> 2008. 3. 15
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_RESERVATION(cltMsg* pclMsg, SI32 id);									// 파티장 예약.			=> 2008. 3. 15
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ADD_PRESENT(cltMsg* pclMsg, SI32 id);									// 파티장 선물 등록.	=> 2008. 3. 15
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_GET_COSTUME(cltMsg* pclMsg, SI32 id);				
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION_LIST(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ADD_DECORATION(cltMsg* pclMsg, SI32 id);			
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_GIVEITEM(cltMsg* pclMsg, SI32 id);									// 선물주기.
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_USEEFFECT(cltMsg* pclMsg, SI32 id);									// 폭죽.
	
	void DoMsg_GAMEMSG_RESPONSE_PARTYHALL_DECORATION_LIST(cltMsg* pclMsg, SI32 id);								// 장식물 리스트 주기.



	//--------------------------------------
	// 주주총회
	//--------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_INFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_SUGGEST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_VOTE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_ACCEPTCANDIDATE(cltMsg* pclMsg, SI32 id);


	void DoMsg_GAMEMSG_NOTICE_GENERALMEETING(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_SETVILLAGEUPGRADE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_STOCKOFFERINGINFO(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_EXCHANGESTOCK(cltMsg* pclMsg, SI32 id);

	//---------------------------------------
	// 시스템 
	//---------------------------------------
	// Client-System.cpp
	void DoMsg_GAMEMSG_RESPONSE_USERNUM(cltMsg* pclMsg, SI32 id);			// 동접 수를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_SECRETCHATRESULT(cltMsg* pclMsg, SI32 id);	// 귓말 채팅 결과를 받는다. 
	void DoMsg_GAMEMSG_RESPONSE_CHAT(cltMsg* pclMsg, SI32 id);				// 채팅 정보를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_HEADCHAT(cltMsg* pclMsg, SI32 id);			// 머릿말을 정보를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_MAKESECRETCHATCHANNEL(cltMsg* pclMsg, SI32 id);	// 채팅 채널 개통 정보를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_ACCEPTSECRETCHATCHANNEL(cltMsg* pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NPCPOS(cltMsg* pclMsg, SI32 id);				// NPC위치를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_USERCHARINFO(cltMsg* pclMsg, SI32 id);		// 사용자 간단 정보. 
	void DoMsg_GAMEMSG_RESPONSE_USERCHARITEMINFO(cltMsg* pclMsg, SI32 id);	// 사용자 아이템 정보. 
	void DoMsg_GAMEMSG_RESPONSE_USERCHARITEMINFO2(cltMsg* pclMsg, SI32 id);	// 사용자 아이템 정보. 
	void DoMsg_GAMEMSG_RESPONSE_ACCOUNTINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRODUCTPRICE(cltMsg* pclMsg, SI32 id);		// 물품 가격 정보 처리. 
	void DoMsg_GAMEMSG_RESPONSE_GMGIVEMONEY(cltMsg* pclMsg, SI32 id);		// 운영자의 돈 주기 처리 
	void DoMsg_GAMEMSG_RESPONSE_NEWS(cltMsg* pclMsg, SI32 id);				// 뉴스 처리. 
	void DoMsg_GAMEMSG_RESPONSE_ASKLIST(cltMsg* pclMsg, SI32 id);			// 질문 리스트 처리. 
	void DoMsg_GAMEMSG_RESPONSE_ENEMYCAPTURE(cltMsg* pclMsg, SI32 id);		// 적의 점령 정보 
	void DoMsg_GAMEMSG_RESPONSE_ENEMYCAPTUREALL(cltMsg* pclMsg, SI32 id);	// 적의 모든 점령 정보 
	void DoMsg_GAMEMSG_RESPONSE_NOTICELOGIN(cltMsg* pclMsg, SI32 id);		// 로그인 공지.
	void DoMsg_GAMEMSG_RESPONSE_SKILLMASTERLIST(cltMsg* pclMsg, SI32 id);	// 장인 명단 통보. 
	void DoMsg_GAMEMSG_RESPONSE_VILLAGEDETAILINFO(cltMsg* pclMsg, SI32 id);	// 마을 상세 정보 통보.  
	void DoMsg_GAMEMSG_RESPONSE_RAREITEMMADE(cltMsg* pclMsg, SI32 id);		// 명품 생산 정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_RECOMMENDREWARD(cltMsg* pclMsg, SI32 id);	// 추천인 포상 정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_FAMELEVELREWARD(cltMsg* pclMsg, SI32 id);	// 명성레벨 포상 정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_BIGWARITEM(cltMsg* pclMsg, SI32 id);			// 대박 전리품 생산 정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_GOODHORSE(cltMsg* pclMsg, SI32 id);			// 명마 생산 정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_NOADDHORSE(cltMsg* pclMsg, SI32 id);			// 말추가 실패 통보.
	void DoMsg_GAMEMSG_RESPONSE_HIGHSUMMON(cltMsg* pclMsg, SI32 id);			// 높은 랭크의 소환수 정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_HOPETOQUIT(cltMsg* pclMsg, SI32 id);			// 로그아웃 희망에 대한 응답
	void DoMsg_GAMEMSG_RESPONSE_HOPETOFRONT(cltMsg* pclMsg, SI32 id);		// 프론트로 되돌아가기

	void DoMsg_GAMEMSG_RESPONSE_VILLAGEGENERALMEETING(cltMsg* pclMsg, SI32 id);		//[추가 : 황진성 2008. 2. 21 => 마을 주주총회 발동 관련 정보 통보.]
	

	void DoMsg_GAMEMSG_RESPONSE_USERLEVELUP(cltMsg* pclMsg, SI32 id);		// 다른 사용자 레벨업정보 통보 
	void DoMsg_GAMEMSG_RESPONSE_SERVERINFO(cltMsg* pclMsg, SI32 id);			// 서버정보를 받는다.
	void DoMsg_GAMEMSG_RESPONSE_WARVILLAGE(cltMsg* pclMsg, SI32 id);			// 공성전의 시작과 종료를 알린다.
	void DoMsg_GAMEMSG_RESPONSE_BONUSTIME(cltMsg* pclMsg, SI32 id);			// 보너스 타임의  시작과 종료를 알린다.
	void DoMsg_GAMEMSG_RESPONSE_SETGAMEDELAY(cltMsg* pclMsg, SI32 id);		// 서버의 게임 속도를 구해온다. 
	void DoMsg_GAMEMSG_RESPONSE_LETTER(cltMsg* pclMsg, SI32 id);				// 쪽지를 받았다. 
	void DoMsg_GAMEMSG_RESPONSE_ACCOUNTLETTER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY(cltMsg* pclMsg, SI32 id); // 아이템 사용기한이 3일 미만임을 통보 받았다.
	void DoMsg_GAMEMSG_RESPONSE_HUNTINGINFO(cltMsg* pclMsg, SI32 id);		// 불가사리 사냥 이벤트 정보. 
	void DoMsg_GAMEMSG_RESPONSE_HUNTINGREWARD(cltMsg* pclMsg, SI32 id);		// 불가사리 사냥 성공시 보상 
	void DoMsg_GAMEMSG_RESPONSE_GOGUINFO(cltMsg* pclMsg, SI32 id);			// 고구려 유적 방어 이벤트 정보. 
	void DoMsg_GAMEMSG_RESPONSE_GOGUREWARD(cltMsg* pclMsg, SI32 id);			// 고구려 유적 방어  성공시 보상 
	void DoMsg_GAMEMSG_RESPONSE_OSAKAINFO(cltMsg* pclMsg, SI32 id);			// 오사카성 정보. 
	void DoMsg_GAMEMSG_RESPONSE_OSAKAREWARD(cltMsg* pclMsg, SI32 id);		// 오사카성 보상 
	void DoMsg_GAMEMSG_RESPONSE_KANGHWADOINFO(cltMsg* pclMsg, SI32 id);		// 강화도 정보. 
	void DoMsg_GAMEMSG_RESPONSE_KANGHWADOREWARD(cltMsg* pclMsg, SI32 id);	// 강화도 보상 

	// 육조판서 
	void DoMsg_GAMEMSG_RESPONSE_GETBYUNGZOINFO(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_GETGONGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETHYUNGZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETHOZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETIZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETYEZOINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETGOONZUINFO(cltMsg* pclMsg, SI32 id);
	
	void DoMsg_GAMEMSG_RESPONSE_GOONZU_GETBONUSTIME(cltMsg* pclMsg, SI32 id);	// 군주가 설정해 놓은 보너스 타임 정보

	void DoMsg_GAMEMSG_RESPONSE_GETANGELAPPLYERLIST(cltMsg* pclMsg, SI32 id); // 수호천사 신청자 목록 요청
	void DoMsg_GAMEMSG_RESPONSE_GETANGELLIST(cltMsg* pclMsg, SI32 id); // 수호천사 목록 요청
	void DoMsg_GAMEMSG_RESPONSE_SETANGELAPPLYER(cltMsg* pclMsg, SI32 id); // 수호천사 등록 요청
	void DoMsg_GAMEMSG_RESPONSE_SETANGEL(cltMsg* pclMsg, SI32 id); // 수호천사 임명 요청

	void DoMsg_GAMEMSG_RESPONSE_GETPOLICELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETPOLICE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_GETCRIMINALLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETCRIMINAL(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_GETCHANGEDNAME(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_AUTOHUNTING(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AUTOHUNTING_ASK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AUTOHUNTING_TIMEUPDATE(cltMsg* pclMsg, SI32 id);


	void DoMsg_GAMEMSG_RESPONSE_ENCHANTITEMSTARTOREND(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ENCHANTOTHERSHOW(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ENCHANTOTHERPRICE(cltMsg* pclMsg, SI32 id);


	void DoMsg_GAMEMSG_RESPONSE_SERVEREVENTINFO(cltMsg* pclMsg, SI32 id);	// 서버의 이벤트 정보를 수령한다. 
	void DoMsg_GAMEMSG_RESPONSE_BOSSAPPEAR(cltMsg* pclMsg, SI32 id);			// 보스 출현 정보를 알려준다. 
	void DoMsg_GAMEMSG_RESPONSE_PERSONALSHOPMAKEINFO(cltMsg* pclMsg, SI32 id);	// 개인 상점 제조의뢰 내용을 방송한다. 
	void DoMsg_GAMEMSG_RESPONSE_LEVELUP(cltMsg* pclMsg, SI32 id);			// 특정 사용자의 레벨이 올랐음을 알린다. 
	void DoMsg_GAMEMSG_RESPONSE_LEVELUPNOTICE(cltMsg* pclMsg, SI32 id); //KHY - 1205 
	void DoMsg_GAMEMSG_RESPONSE_PERSONAL_LOG(cltMsg* pclMsg, SI32 id);		// 운영자가 사용자의 로그를 요청한다.
	void DoMsg_GAMEMSG_RESPONSE_NOTICEADMIN(cltMsg* pclMsg, SI32 id);		// 중요공지한다
	void DoMsg_GAMEMSG_RESPONSE_INSTANCEMAPINFO(cltMsg* pclMsg, SI32 id);

	//	void DoMsg_GAMEMSG_RESPONSE_ITEMINFOGET(cltMsg* pclMsg, SI32 id);	// 아이템의 정보를 얻어온다. 유니크 및 바이너리로

	// Client-LogIn.cpp
	void DoMsg_GAMEMSG_RESPONSE_LOGIN(cltMsg* pclMsg, SI32 id);			// 로그인. 
	void DoMsg_GAMEMSG_RESPONSE_LOGINMANY(cltMsg* pclMsg, SI32 id);		// 로그인. (복수 캐릭터. ) 
	void DoMsg_GAMEMSG_RESPONSE_LOGOUT(cltMsg* pclMsg, SI32 id);			// 로그아웃.  

	void DoMsg_GAMEMSG_RESPONSE_RECOMMENDPERSON(cltMsg* pclMsg, SI32 id);	// 추천인 응답
	void DoMsg_GAMEMSG_RESPONSE_RECOMMENDPERSONAVAILABLE(cltMsg* pclMsg, SI32 id);	// 추천인 추천 가능

	void DoMsg_GAMEMSG_RESPONSE_BLACKARMYAPPLY(cltMsg* pclMsg, SI32 id);	// 흑의군대 이벤트 신청
	void DoMsg_GAMEMSG_RESPONSE_BLACKARMYWAR(cltMsg* pclMsg, SI32 id);	// 흑의군대 이벤트 신청

	void DoMsg_GAMEMSG_RESPONSE_BLACKARMY_RANKINFO(cltMsg* pclMsg, SI32 id);	// 흑의군대 이벤트 신청

	void DoMsg_GAMEMSG_RESPONSE_BLACKARMY_MVPEFFECT(cltMsg* pclMsg, SI32 id);	// 흑의군대 이벤트 신청

	void DoMsg_GAMEMSG_RESPONSE_BLACKWAR(cltMsg* pclMsg, SI32 id);	// 흑의군대 전체이벤트

	void DoMsg_GAMEMSG_RESPONSE_DISASSEMBLEITEM(cltMsg* pclMsg, SI32 id);	// PersonID를 가지고 Person 이름을 받아옴
	void DoMsg_GAMEMSG_RESPONSE_DISASSEMBLEITEM_LIST(cltMsg* pclMsg, SI32 id);	// PersonID를 가지고 Person 이름을 받아옴
	void DoMsg_GAMEMSG_RESPONSE_DISASSEMBLEITEM_RECIEVE(cltMsg* pclMsg, SI32 id);	// PersonID를 가지고 Person 이름을 받아옴
	void DoMsg_GAMEMSG_RESPONSE_OPENTREASUREBOX(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_KINGGURIINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SANTARACCOONINFO(cltMsg* pclMsg, SI32 id);		//[추가 : 황진성 2007. 11. 29 산타라쿤 이벤트 메시지.]
	void DoMsg_GAMEMSG_RESPONSE_KITERACCOONINFO(cltMsg* pclMsg, SI32 id);		// [영훈] 2008.01.13 설날 이벤트 카이트 라쿤 메시지
	void DoMsg_GAMEMSG_RESPONSE_RAREITEMFROMTREASUREBOX(cltMsg* pclMsg, SI32 id);	// [영훈] 렌덤 상자를 열었을때 레어아이템이 나왔다
	

	//------------------------------------------------
	// Item
	//------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_NPCREPAIRPRICE(cltMsg* pclMsg, SI32 id);			// NPC수리 가격 응답. 
	void DoMsg_GAMEMSG_RESPONSE_MAKEITEMSWITCH(cltMsg* pclMsg, SI32 id);			// 제조에니메이션을 시작하고 끝낸다.
	void DoMsg_GAMEMSG_RESPONSE_EXCHANGEWEAPON(cltMsg* pclMsg, SI32 id);			// Weapon의 Active 상태를 바꾼다.
	void DoMsg_GAMEMSG_RESPONSE_EXCHANGECLOTHES(cltMsg* pclMsg, SI32 id);			// Clothes 의 Active 상태를 바꾼다

	void DoMsg_GAMEMSG_RESPONSE_UPGRADEITEM(cltMsg* pclMsg, SI32 id);				// 아이템 업그레이드에 대한 응답( 일반 -> 호품 -> 명품 -> 신기 )  - 혹부리 영감 이용권을 사용해서.
	void DoMsg_GAMEMSG_RESPONSE_PERFORMANCEUPGRADEITEM(cltMsg* pclMsg, SI32 id);	// 아이템 성능 향상 시킴 ( 부적 붙혀서 )

	void DoMsg_GAMEMSG_RESPONSE_ENCHANTITEMDLG(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PERSONMAKEITEMSWITCH(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MAKEITEMDLG(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ISSKILLITEM(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_PREMIUM_INFO(cltMsg* pclMsg, SI32 id);				// 프리미엄 정보를 받다.

	void DoMsg_GAMEMSG_RESPONSE_CRYSTALITEMINFO(cltMsg* pclMsg, SI32 id);			// 결정체 정보를 업데이트 한다.( 일단 내구도만)

	void DoMsg_GAMEMSG_RESPONSE_SUMMONCHANGEITEM(cltMsg* pclMsg, SI32 id);			// [영훈] 소환수 장비착용 : 소환수 장비가 변경되었다

	//------------------------------------------------
	// 건물정보
	//------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_VILLAGEINFOSTRINFO(cltMsg* pclMsg, SI32 id);	// 마을 상세 정보용 건물 정보 통보. 


	//------------------------------------------------
	// 상품 주기 (XBOX, 자전거, 플스 등등 )
	//------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_GIFTNT(cltMsg* pclMsg, SI32 id);

	//#if defined(_CITYHALL_BANKRUPTCY)
	//------------------------------------------------
	// 마을 선전포고에 대한 응답 받음
	//------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_NOTIFYCLIENTABOUTCITYHALLSTATE(cltMsg* pclMsg, SI32 id);
	//#endif

	//cyj 메세지 핸들러 추가
	void DoMsg_GAMEMSG_RESPONSE_USERORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CLIENTCHARUNIQUE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NOTICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CHARLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DELUSERCHAR(cltMsg* pclMsg, SI32 id);//케릭터삭제
	void DoMsg_GAMEMSG_RESPONSE_PREDELUSERCHAR(cltMsg* pclMsg, SI32 id);//케릭터삭제유예기간
	void DoMsg_GAMEMSG_RESPONSE_MAKENEWCHAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGESTRUCT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BUILDSTRUCTURE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DESTROYSTRUCTURE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGENOTICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGELEVELDETAILINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RANK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REALESTATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STRINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETTAXLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETDIVIDENDLIST(cltMsg* pclMsg, SI32 id);
	//void DoMsg_GAMEMSG_RESPONSE_HIRENPC(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RESIDENTAPPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RESIDENTLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RESIDENTINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_UPDATERESIDENTAPPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ALLSTRCHIEFINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STOCKPRICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETVILLAGEINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETNOWVILLAGEWARINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETVILLAGEMARK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RESIDENTSLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RESIDENTLOGIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DECLAREWAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CANCELDECLAREWAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETSYMBOLSTATUS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETCASTLESTATUS(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGEWARRESULT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGEWARRESULT_FOR_VILLAGEMEMBER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETSTRNOTICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ENDVILLAGEWAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_INVESTLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CHIEFINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REPAIRSTRUCTURE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGEBOARD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_EVENT_INSERTMATERIAL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_EVENT_MATERIALLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_EVENT_MATERIALCOMPLETE(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_EVENT_COMPLETEBUILD(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_SERVERRTN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NOTICE_QUEST(cltMsg* pclMsg, SI32 id);

	//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
	void DoMsg_GAMEMSG_RESPONSE_CHARSERVERRTN(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_DATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_LOWPRICESTOCKORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HOUSEAPPLYERLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STGITEMINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_STGINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITSTABLECREATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITHORSEINFOSET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HOUSEUNITHORSEEATFOOD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WEATHER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GAMEMASTERLEVEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RIDEHORSE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_UNRIDEHORSE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DELHOUSEAPPLYER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MARKETSELLINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DEALTHPENALTYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_REVIVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ITEMDROP(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WHISPERCHAT_REJECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PRIVATETRADE_REJECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ANOTHERAUTH(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NOTGAMEPOTAUTH(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CHANGE_CHARNAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CHANGE_CHARKIND(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CANNOTLOGIN_AGE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CANNOTLOGIN_ERROR(cltMsg* pclMsg, SI32 id);	
	void DoMsg_GAMEMSG_RESPONSE_GAMEDISCONNECT_AGE(cltMsg* pclMsg, SI32 id);	// 게임중에 나이제한 체크함 (PCK - 2007.09.19)
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETINGLETTER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_PARTICIPATE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_CHAT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_CHATJOIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GENERALMEETING_CHATOUT(cltMsg* pclMsg, SI32 id);

	//Client-WorldMoney
	void DoMsg_GAMEMSG_RESPONSE_SETWORLDMONEY_BUYDLG(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DISTRIBUTION_WORLDMONEY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WORLDMONEY_TRADELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WORLDMONEY_SELLORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WORLDMONEY_BUYORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WORLDMONEY_CANCELORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WORLDMONEY_SET_CHAR(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_SET_SELLORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_CANCEL_SELLORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_SET_BUYORDER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_GET_LISTINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_GET_SELLINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_GET_MYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_GET_SAVEDACU_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRADESERVER_GET_WITHDRAW_ACU(cltMsg* pclMsg, SI32 id);

	// WorldPassport( 무역 여권 )
	void DoMsg_GAMEMSG_RESPONSE_WORLDPASSPORT_SET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WORLDPASSPORT_EXTEND(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_COUNTRYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_USERLISTBYCOUNTRY(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_SERVERTIME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NOTICECOUNTRYUSERMAKE(cltMsg* pclMsg, SI32 id);

	// 20미만 레벨 유저 통보. 
	void DoMsg_GAMEMSG_RESPONSE_LOWLEVELLIST(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_ADDBUF(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DELETEBUF( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SETTEACHERPOINT( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_SETITEMTYPEINFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SETEMOTICON( cltMsg* pclMsg, SI32 id );

	void DoMSg_GAMEMSG_RESPONSE_GAME_GUARD_AUTH( cltMsg * pclMsg , SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_MYPUCHIKOMIURL( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_USERPUCHIKOMIURL( cltMsg* pclMsg, SI32 id );
	// Guild
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CREATEGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_DESTROYGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_JOINGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CONFIRMJOINGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_LEAVEGUILD( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGEVILLAGE( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_GETGUILDLIST( cltMsg*pclMsg,SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_GETGUILDINFO(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_SETSECONDMASTER(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_GIVEGUILDMONEY(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_SETSTRFEERATE(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGESTATUS(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_SETMARK(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGENAME(cltMsg*pclMsg,SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILD_CHANGEMASTER( cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_RESERVELIST( cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_APPLYRESERVE( cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_APPLYTOENEMY( cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_START( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_START( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_ADDKILLPOINT( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_MAPUSERLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_END( cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_KILLPOINT( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDDUNGEON_APPLYLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDDUNGEON_APPLY( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDDUNGEON_STATUS( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_MEMBERLIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILD_GETVILLAGELIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDWAR_RECOMMANDLIST( cltMsg* pclMsg, SI32 id );
	//void DoMsg_GAMEMSG_RESPONSE_GUILD_CLOSEGUILD( cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GACHAPRODUCT_LIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GACHABUY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GACHA2PRODUCT_LIST( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_CARRYOVER_GACHA_LIST( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_CARRYOVER_SELECTPRODUCT_LIST( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_RESERVELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_START(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_END(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_SETINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_INFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_MOFUMOFUEVENT_MAMAMOFUMOFU(cltMsg* pclMsg, SI32 id);

	// [진성] 마을 이벤트 - 몬스터 습격
	void DoMsg_GAMEMSG_RESPONSE_MONSTER_ATTACK_START(cltMsg* pclMsg, SI32 id);
	// [지연] 마을 이벤트 - 몬스터 습격 - 보상 내역에 대한 통보용
	void DoMsg_GAMEMSG_RESPONSE_MONSTER_ATTACK_REWARD(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_SYSTEMREWARDLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RECIEVESYSTEMREWARD(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_DEMANDATTEND(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_ATTEND(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_CREATEEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_DESTROYEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_INVITEEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_JOINEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SECEDEEMPIRE(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_UPDATEEMPIREINFO(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_EMPIREINFO(cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SETEMPIREMONEY(cltMsg* pclMsg, SI32 id );
	
	
	void DoMsg_GAMEMSG_RESPONSE_AUTOMACRO_CHECK(cltMsg* pclMsg, SI32 id );		//[추가 : 2007. 12. 3 => 4차오토프로그램 방지 클라이언트 메시지 추가.]
	void DoMsg_GAMEMSG_RESPONSE_AUTOMACRO_ON_OTHER(cltMsg* pclMsg, SI32 id);	//[추가 : 황진성 2007. 12. 10 => 오토프로그램 발견 공지 메시지.]

	void DoMsg_GAMEMSG_RESPONSE_NEWYEAR_BTN(cltMsg* pclMsg, SI32 id);			//[추가 : 황진성 2007. 12. 11 => 신년 버튼 뜨우라는 메시지 처리.]

	void DoMsg_GAMEMSG_RESPONSE_TRANSFORM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_TRANSFORMINFO(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_MAXSYNCRO_ATTAINMENT(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_TRANSFORM_LIMIT_TIME(cltMsg* pclMsg, SI32 id);		//[추가 : 황진성 2008. 1. 9 => 변신한 시간이 30초가 안되서 해제 실패 메시지 띄우게.]	

	void DoMsg_GAMEMSG_RESPONSE_CREATEBANKACCOUNT_CANCEL(cltMsg* pclMsg, SI32 id);	//[추가 : 황진성 2008. 1. 9 => 은행 내구도가 0이어서 계좌만들기 실패 메시지 띄우게.]	
	
	//----------------------------------------------------------------------------------
	//	추가 : 황진성 2008. 1. 9 => 인스턴트던젼.
	//----------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_INSTANTSDUNGEON_STATUS(cltMsg* pclMsg, SI32 id);		//[추가 : 황진성 2008. 1. 10 => 인던 현재 상태 정보.]	

	void DoMsg_GAMEMSG_RESPONSE_INSTANTSDUNGEON_RUN_STATUS(cltMsg* pclMsg, SI32 id);	//[추가 : 황진성 2008. 1. 10 => 인던 진행 상태 정보.]	

	void DoMsg_GAMEMSG_RESPONSE_INSTANTSDUNGEON_REWARD(cltMsg* pclMsg, SI32 id);		//[추가 : 황진성 2008. 1. 11 => 인던 보상 정보.]	
	
	void DoMsg_GAMEMSG_RESPONSE_INSTANTSDUNGEON_TOP_CHANGE(cltMsg* pclMsg, SI32 id);	//[추가 : 황진성 2008. 1. 17 => 인던 최고 순위 변경 알림.]	
	//--------------------------------------------------------------------------------------
	//명예의 전당 
	//--------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_HOFList(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RakingList(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_INSTANTMAP_GATEINFO(cltMsg* pclMsg, SI32 id);

	//[추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 이벤트 종료 후 결과를 알릴 메시지.]  
	void DoMsg_GAMEMSG_RESPONSE_BLACK_WAR_REFORM_CHAR_RESULT(cltMsg* pclMsg, SI32 id);
	
	//[추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 이벤트 종료 후 결과를 알릴 메시지.]  
	void DoMsg_GAMEMSG_RESPONSE_BLACK_WAR_REFORM_VILLAGE_RESULT(cltMsg* pclMsg, SI32 id);

	//*******************************************************************************************
	//[추가 : 황진성 2008. 1. 29 =>  공간 상자 메시지. S->C]
	void DoMsg_GAMEMSG_RESPONSE_SPACE_BOX_LIST( cltMsg* pclMsg, SI32 id );
	
	//[추가 : 황진성 2008. 1. 29 =>  공간 상자(보물) 메시지. S->C]
	void DoMsg_GAMEMSG_RESPONSE_TRESURE_BOX_LIST( cltMsg* pclMsg, SI32 id );

	//[추가 : 황진성 2008. 1. 30 =>  공간 상자 저장 메시지. S->C]
	void DoMsg_GAMEMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM( cltMsg* pclMsg, SI32 id );

	//[추가 : 황진성 2008. 1. 30 =>  공간 상자 회수 메시지 처리. S->C]
	void DoMsg_GAMEMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM( cltMsg* pclMsg, SI32 id );

	// [영훈] 공간상자 아이템 사용 추가
	void DoMsg_GAMEMSG_RESPONSE_USE_SPACEBOX_ITEM( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_USE_TREASURE_ITEM( cltMsg* pclMsg, SI32 id );
	//*******************************************************************************************	
	
	//[추가 : 황진성 2008. 2. 28 =>  아티펫트 메시지 처리. S->C]
	void DoMsg_GAMEMSG_RESPONSE_ARTIFACT_MIXTURE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_ARTIFACT_REMOVE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_ARTIFACT_SLOT_CREATE( cltMsg* pclMsg, SI32 id );
	//*******************************************************************************************	
	
	// [영훈] CRC체크관련 서버에서 체크 메시지를 보내라는 페킷이 들어있음
	void DoMsg_GAMEMSG_RESPONSE_FILECRCCHECK( cltMsg* pclMsg, SI32 id );
	// [영훈] 해적선 : 클리어 보너스로 아이템을 획득했다는 메시지
	void DoMsg_GAMEMSG_RESPONSE_INSTANTDUNGEON_PARTY_CLEAR( cltMsg* pclMsg, SI32 id );
	// [영훈] 해적선 : 해적선이 나타났다는 메시지
	void DoMsg_GAMEMSG_RESPONSE_PIRATESHIP_APPEAR( cltMsg* pclMsg, SI32 id );
	
	//[진성] 건강정보를 변경 시킨다. => 2008-4-6
	void DoMsg_GAMEMSG_RESPONSE_HEALTH_CHANGE( cltMsg* pclMsg, SI32 id );

	//-----------------------------------------------------------------------
	// 케릭터 옵션 정뵤
	//---------------	---------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_CHAR_OPTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CHAR_GET_OPTION(cltMsg* pclMsg, SI32 id );	

	// [진성] 영웅의 동상 이벤트 버프룰렛 돌릴때 클라 처리. => 2008-6-10
	void DoMsg_GAMEMSG_RESPONSE_BUFF_ROULETTE( cltMsg* pclMsg, SI32 id );

	//	[종호_BALANCECURRENCY] 응답 패킷
	void DoMsg_GAMEMSG_RESPONSE_BALANCECURRENCY_BUFF( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_SET_HOMEPAGE_URL( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GET_HOMEPAGE_URL( cltMsg* pclMsg, SI32 id );

	// [진성] 서버 이벤트 처리 함수. (S->C) => 2008-6-19 
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_RANK_INFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_SERVER_INFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_DAILY_EVENT_SERVER_INFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_PERSONAL_INFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_INSTANCEMAP_BONUS_FIGURE( cltMsg* pclMsg, SI32 id );

	// [영훈] 서버 이벤트 퀘스트 관련 함수
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_QUEST_START( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_QUEST_RUNINFO( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_QUEST_CLEARSTATUS( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_SERVER_EVENT_INSTANCEDUNGEON_RUNINFO( cltMsg* pclMsg, SI32 id );

	// [성웅] 2차 가챠 아이템 탄광소녀
	/*void DoMsg_GAMEMSG_RESPONSE_TANWANGGIRL(cltMsg* pclMsg, SI32 id);*/
	
	// [춘기] 소환수 스태미너 충전
	void DoMsg_GAMEMSG_RESPONSE_SUMMON_STPCHARGE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_REQUEST_EVENTTIME_UPDATE( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_CHECKPROCESS( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_CHECKPROCESS_RESULT( cltMsg* pclMsg, SI32 id );

	//[진성] 호박상자 => 2008-9-16
	void DoMsg_GAMEMSG_RESPONSE_AMBER_BOX_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_CHAR_ROULETTE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VILLAGE_RESIDENT_INVITE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_PVP_FILED(cltMsg* pclMsg, SI32 id);// NEW PVP - 손성웅 2008.10.08
	void DoMsg_GAMEMSG_RESPONSE_PVP_LEAGUE_INFOGET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PVP_LEAGUE_UI_INFOGET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PVP_LEAGUE_BATTLE_CHANGEGRADE(cltMsg* pclMsg, SI32 id);
	
	void DoMsg_GAMEMSG_RESPONSE_HELP_ME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HELP_ME_OK(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_HELP_CHAT_MSG(cltMsg* pclMsg, SI32 id);
	
	//[진성] 도우미 => 2008-11-05
	void DoMsg_GAMEMSG_RESPONSE_QUESTION_LIST_GET( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_QUESTION_CONTENTS_GET( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_ADD_ANSWER( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_ADD_ANSWER_COMPLETE_RESULT( cltMsg* pclMsg, SI32 id );

	void DoMsg_GAMEMSG_RESPONSE_SETMUTE( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_MUTELIST( cltMsg* pclMsg, SI32 id );

	//[춘기] 신년운세 룰렛
	void DoMsg_GAMEMSG_RESPONSE_DIVINATION_ROULETTE( cltMsg* pclMsg, SI32 id );

	//[진성] 엘릭서 npc에게 타입별 아이템의 정보를 리스트로 받는다.
	void DoMsg_GAMEMSG_RESPONSE_ELIXIR_NPC_ITEM_TYPE_LIST_GET( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_ELIXIR_SET( cltMsg* pclMsg, SI32 id );

	//발렌타인 데이 전쟁 정보
	void DoMsg_GAMEMSG_RESPONSE_VALENTINE_INFO_GET(cltMsg* pclMsg, SI32 id);
	//발렌타인 데이 전쟁 커플정보
	void DoMsg_GAMEMSG_RESPONSE_VALENTINE_REQUEST(cltMsg* pclMsg, SI32 id);
	// 정각이 되었어요 자 너 커플이냐? 솔로냐 물어보아보아요 ㅋㅋ
	void DoMsg_GAMEMSG_RESPONSE_VALENTINE_RUSOLO_COUPLE_ONTIME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINE_OPENDLG(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINE_PRIZEGET(cltMsg* pclMsg, SI32 id);

	// [영훈] 새로운 선택상자 아이템을 받았다
	void DoMsg_GAMEMSG_RESPONSE_NEWSELECTBOX_SELECT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NEWSELECTBOX_GETITEM(cltMsg* pclMsg, SI32 id);

	
	//=============================================================================================
	// [진성] 통합마켓.
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_SELL_NEWMARKET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BUY_NEWMARKET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RECALL_NEWMARKET(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_NEWMARKETACCOUNT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NEWMARKETITEMLIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GETFROMNEWMARKET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NEWSELLITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NEWBUYITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SETNEWMARKETFEE(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_PERSONNAME_SERIALID(cltMsg* pclMsg, SI32 id);		// SerialID를 가지고 Person 이름을 받아옴

	//=============================================================================================
	// [성웅] 길드맵 획득방식 변경
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_Request_Warp(cltMsg* pclMsg, SI32 id);	// 길드사냥터 길드전 워프 여부를 물어 본다.
	void DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_Top10( cltMsg* pclMsg, SI32 id );		// // 길드사냥터 길드전 맵별 탑10
	void DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_END( cltMsg* pclMsg, SI32 id);
	
	void DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_TIEM( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GuildHuntMapWar_HaveMap( cltMsg* pclMsg, SI32 id );
	void DoMsg_GAMEMSG_RESPONSE_GUILDHUNTMAPGUILDWAR_SENDMATCHINGRESULT( cltMsg* pclMsg, SI32 id );

	
	
	//=============================================================================================
	// [진성] 프로모터
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_PROMOTERNPC_GIVE_ITEM_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PROMOTER_INFO_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PROMOTER_URL_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PROMOTER_SET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RECOMMEND_INTRODUCE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RECOMMENDEE_INTRODUCE(cltMsg* pclMsg, SI32 id);	

	//=============================================================================================
	// [영훈] 결혼 시스템
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_PROPOSE_QUESTION(cltMsg* pclMsg, SI32 id);	// 프로포즈 질문
	void DoMsg_GAMEMSG_RESPONSE_SET_MARRY(cltMsg* pclMsg, SI32 id);			// 결혼했다
	void DoMsg_GAMEMSG_RESPONSE_GET_MARRY_INFO(cltMsg* pclMsg, SI32 id);	// 결혼관련 정보 받기
	void DoMsg_GAMEMSG_RESPONSE_MARRY_ITEMGET_SET(cltMsg* pclMsg, SI32 id);	// 결혼기념일 아이템 받은 날짜 설정
	void DoMsg_GAMEMSG_RESPONSE_MARRY_LOVE_SET(cltMsg* pclMsg, SI32 id);	// 결혼 애정도 관련 설정

	void DoMsg_GAMEMSG_RESPONSE_WEDDING_RESERVE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WEDDING_SIMPLELIST_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WEDDING_LIST_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_WEDDING_RESERVE_CANCEL(cltMsg* pclMsg, SI32 id);


	//	[종호] 통화량 조절 - 사용가능 금액 응답
	void DoMsg_GAMEMSG_RESPONSE_BALANCECANUSECURRENCY(cltMsg* pclMsg, SI32 id);
	//	[종호] GM이 요청한 사용 가능 통화량
	void DoMsg_GAMEMSG_RESPONSE_GETBALANCECANUSECURRENCY(cltMsg* pclMsg, SI32 id);
	
	//=============================================================================================
	void DoMsg_GAMEMSG_RESPONSE_FAMILY_REQUEST_ADD_CHILDRENN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GET_FAMILYINFO(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FAMILY_UPDATE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_FAMILY_CHAT(cltMsg* pclMsg, SI32 id);


	//=============================================================================================
	// [영훈] 수호 정령
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_STATUSINOUT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_FELLOWSHIPSET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_CHANGENAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_DEAD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_GET(cltMsg* pclMsg, SI32 id);

	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_HOUSE_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SOULGUARD_HOUSE_CREATE(cltMsg* pclMsg, SI32 id);


	//=============================================================================================
	// [성웅] 라쿤 피하기 이벤트 //Switch_10MonthEvent_RacoonDodge
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_RESERVATION(cltMsg* pclMsg, SI32 id);			// 예약에 대한 답변-> 현제 대답해줄건 리스트를 보내주는거임.현제 만들어진 작업 없음. 추가 해야함
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_WAIT5SECOND(cltMsg* pclMsg, SI32 id);			// 5초 카운트 해라
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_REQUEST_PLAYEVENT(cltMsg* pclMsg, SI32 id);			// 선정 되었다. 참여 할꺼니?
	
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_30SECOND(cltMsg* pclMsg, SI32 id);			// 30초 카운트 
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_ROUNDCOUNT(cltMsg* pclMsg, SI32 id);			// 화면에 해당 라운드를 보여준다.
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_30SECOND_CLEAR(cltMsg* pclMsg, SI32 id);			// 30초 카운트  중지 
	void DoMsg_GAMEMSG_RESPONSE_RACOONDODGE_EVENT_ROUNDCOUNT_CLEAR(cltMsg* pclMsg, SI32 id);			// 화면에 해당 라운드 중지 

	//=============================================================================================
	// [기형] 휴면 계정 시스템
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_DORMANCY_TAKEOUTEQUIPITEMINBOX(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_DORMANCY_REFRESHPOINTINFO(cltMsg* pclMsg, SI32 id);
	
	//  [8/12/2009 종호_GAWIBAWIBO] 
	void DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_CHARGED_MONEY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_CALCULATE_RESULT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_NOTICE_RESULT(cltMsg* pclMsg, SI32 id);
	//void DoMsg_GAMEMSG_RESPONSE_GAWIBAWIBO_COUNT(cltMsg* pclMsg, SI32 id);

	//=============================================================================================
	// [지연] 복불복 게임
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_BOKBULBOK_CHECK_ENTERMAP_ENABLE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BOKBULBOK_CHECK_WAITLIST_ENABLE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BOKBULBOK_CHECK_ENTRYREQUEST_FAIL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_WAITLIST_ANSWER(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_WAITLIST_CANCEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_STAGESTART(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_STAGEFAIL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_WARPMAPQUESTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_NOTENOUGHMONEY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_STAGECLEAR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_GM_SHOWMONSTERTYPE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_10CLEARNOTICE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BBB_ALREADYRESERVED(cltMsg* pclMsg, SI32 id);
	
	//=============================================================================================
	// [강형] 새로운 내구도 시스템
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_NEWNPCREPAIR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NEWITEMREPAIR(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_NEWITEMREPAIRREQUESTINFO(cltMsg* pclMsg, SI32 id);
	//-------------------------------------------------------------------------------------------------------------------
	//[성웅] 라쿤이벤트 군주 이벤트로 변경
	//-------------------------------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE__RACCOONEVENT_TOGOONZU(cltMsg* pclMsg, SI32 id);//[성웅] 라쿤이벤트 군주 이벤트로 변경
	void DoMsg_GAMEMSG_RESPONSE__GOONZURACCOONEVENT_SETINFO(cltMsg* pclMsg, SI32 id);//[성웅] 라쿤이벤트 군주 이벤트로 변경

	
	//-------------------------------------------------------------------------------------------------------------------
	//[진성] 스킬북
	//-------------------------------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_SKILLBOOK_MAGICINFO_GET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SKILLBOOK_MAGIC_SET(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SKILLBOOK_MAGIC_DEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGIC_ADD(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGIC_DEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CURSEINFO(cltMsg* pclMsg, SI32 id);
	

	//=============================================================================================
	// [기형] 배틀로얄
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_NOTICERESULT(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_REMAINEDTIME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_STARTGAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_ENDGAME(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_POWERUPMODE(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_BATTLEROYAL_OPENSTADIUM(cltMsg* pclMsg, SI32 id);

	//=============================================================================================
	// [기형] 발렌타인데이 이벤트
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_BUF(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_CHANGEBUF(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_START(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_END(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_GIVE_PRESENT_TONPC(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEEVENT_ISHOLDING(cltMsg* pclMsg, SI32 id);

	// [지연] 2010신년이벤트
	//---------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_FORTUNERESULT(cltMsg* pclMsg, SI32 id);			// 나온 운세에 대한 정보 (서버->클라)

	// [지연] 군주데이 - 이벤트 아이템으로 생성된 미션에 대한 정보 통보
	void DoMsg_GAMEMSG_RESPONSE_GOONZUDAYMISSION( cltMsg* pclMsg, SI32 id );

	//-------------------------------------------------------------------------------------------------------------------
	// [성웅] 메크로 유저 정보 분석
	//-------------------------------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_REQUEST_RECORD_ACTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_RECORD_ACTION_FINISHED(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_GZFTP_PROTOCOL(cltMsg* pclMsg, SI32 id);
	
	//-------------------------------------------------------------------------------------------------------------------
	// [지연] 파티매칭 클라쪽 응답 패킷 처리 함수들
	//-------------------------------------------------------------------------------------------------------------------
	void DoMsg_GAMEMSG_RESPONSE_PARTYMATCHING_LEVEL(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_APPLYTOPARTYJOIN(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CREATEMYPARTY(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CREATECOMPLETEPARTY(cltMsg* pclMsg, SI32 id); 
	void DoMsg_GAMEMSG_RESPONSE_GETPARTYMEMBERNAMELIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_PARTYMATCHINGEXCEPTION(cltMsg* pclMsg, SI32 id);

	// [종호] 파티퀘스트
	void DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_CHECKSTART(cltMsg* pclMsg, SI32 id); 
	void DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_START(cltMsg* pclMsg, SI32 id); 
	void DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_UPDATE(cltMsg* pclMsg, SI32 id); 
	void DoMsg_GAMEMSG_RESPONSE_PARTY_QUEST_GIVEUP(cltMsg* pclMsg, SI32 id);
	
	


	// [진성] 기부 관련 정보 얻기.
	void DoMsg_GAMEMSG_RESPONSE_CONTRIBUTION_ITEM_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_CONTRIBUTION_INPUT_ITEM_LIST(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_GIANTRACCOON_RESURRECT_EVENT_START(cltMsg* pclMsg, SI32 id);

	//[성웅] 제작의뢰 찾기 
	void DoMsg_GAMEMSG_RESPONSE_SEARCHMANUFACTURE(cltMsg* pclMsg, SI32 id);

	//[진성] 발렌타인데이. 
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEDAY_ITEM_PACKING(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEDAY_BUY_ITEM(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEDAY_CONFESSION_QUESTION(cltMsg* pclMsg, SI32 id);
	void DoMsg_GAMEMSG_RESPONSE_VALENTINEDAY_CONFESSION_ANSWER(cltMsg* pclMsg, SI32 id);
	

	
	
	//---------------------------
	// Key입력. 
	//---------------------------
	void PushKey(cltKey* pclkey)
	{
		clKeyManager.Push(pclkey);
	}

	void HorseStatusShow(TCHAR* pnormaltext);		// 말의 불만 사항을 표시한다. 
	void HorseStatusHide();		// 말의 불만 사항을 표시를 삭제한다.

	bool RequestSecretChatByPersonID(SI32 personid,TCHAR * buf = NULL );		// personid에게 귓말을 요청한다. 
	bool RequestSecretChatByCharUnique(SI32 charunique,TCHAR * buf = NULL );	// charunique에게 귓말을 요청한다. 
	bool RequestSecretChatByCharName(TCHAR * szName,TCHAR * buf = NULL );	// 캐릭터 이름을 근거로 귓말을 요청한다.

	// 배경 음악을 연주한다. 
	void PlayBackgroundMusic(SI32 mapindex);

	// 맵들의 주변 위치 정보를 가져온다.
	void GetNearMapInfo(SI32 mapindex,SI32 x,SI32 y,TCHAR * buf, SI16 txtSize );
	bool GetDirectionStr(SI32 sx, SI32 sy, SI32 dx, SI32 dy, TCHAR *szText, SI16 txtSize);


	// 지도 이동시 불필요한 인터페이스를 지운다. 
	void DestroyNeedlessInterface();

	// 장비의 내구도 검사. 
	void CheckuserEquipDur();

	//-----------------------------------------------
	// 아이템 기간 관련
	//-----------------------------------------------
	// 아이템 기간 관련 처리
	BOOL GetItemUseAbleDay(cltItem* clItem, SI32 * outUseAbleDay); // 사용기간이 지난 아이템이면 참을 반환

	// 배경 그림 그리기
	void DrawInitBack();

	// 주인공이 말을 타거나 내리게 한다. 
	void RideOrUnrideHorse();

	// 클라이언트에서 서비스 지역을 설정하고 적절한 조치를 취한다. 
	void SetServiceArea(SI32 area);

	SI32 GetScreenMouseX();
	SI32 GetScreenMouseY();

	// 마우스를 움직일때의 처리. 
	void MouseMoveAction(SI32 x, SI32 y);

	void ChangeTextFont( TCHAR *FileName, TCHAR *FontKind );

	//------------------------------------------
	// 포그 관련 
	//------------------------------------------
	void InitFogNode();
	// 타일 출력 속도 향상을 위한 마스크 정보 획득
	void GetTileImgMask();

	BOOL CreateDirectDraw();

	// 출력 속도 향상을 위한 타일 마스트. 
	SI32 TileMaskTransArray[MAP_TILE_YSIZE];
	SI32 TileMaskImgArray[MAP_TILE_YSIZE];

	bool GetFameLevelName( SI16 FameLevel, TCHAR *FameLevelName );

	//======================================================
	// 메시지맵
	//======================================================
	// 클라이언트메시지 처리 함수포인터
	class stClientMsgPtr
	{
	public:
		void (cltClient::*ClientMsgFunc)(cltMsg* pclMsg, SI32 id);
	};

	stClientMsgPtr *m_pClientMsgPtr;
	void	ClientMsgNullFunc(cltMsg* pclMsg, SI32 id);
	void	RegisterClientMsgMap();

	//------------------------------------------
	// 포그 관련 
	//------------------------------------------
	bool IsEnterGate( SI32 gateunique );

	SI16 GetLanguageFromFile();
	SI16 GetFpsFromFile();

	void SendEmoticon( SI16 index );

	//------------------------------------------
	// 행동이 가능한 시간인가 ( second 마다 행동할 경우 )
	// 프레임이 가변적임에 따라.. 만듬..
	//------------------------------------------
	bool CanActionBySecond( SI32 second );

	bool IsOpenInterface( SI32 index );

	//[진성] 버프룰렛이 돌아가는 중이면 해당 버프를 룰렛이 끝난후에 그린다. => 2008-6-17
	bool IsBuffRouletteAction(SI16 _siBuffKind);
};

#include <vector>
#include <mmsystem.h>
#include <winbase.h>

struct XPerformanceGuard
{
	XPerformanceGuard( const char *szName, int nLimit ) : m_szName( szName )
	{   
		m_nStartTime    = timeGetTime();            // 코드 시작 시간 세팅
		m_nTimeLimit    = m_nStartTime + nLimit;
		m_nEndTime      = 0;
	}

	~XPerformanceGuard()
	{
		m_nEndTime = timeGetTime();

		// 수행된 시간이 허용치를 초과했다면 퍼포먼스 경고 출력
		if( m_nEndTime > m_nTimeLimit ) printPerformanceWarning();
	}

	void Profile( const char *szName )
	{
		m_vResult.push_back( _TAG( szName, timeGetTime() ) );
	}

protected:

	void oprint( const char *str, ... )
	{
		char szBuf[1024];
		va_list va;
		va_start( va, str );
		_vsnprintf( szBuf, sizeof(szBuf), str, va );
		va_end( va );
		OutputDebugString( szBuf );
	}

	// 각각의 수행 시간을 출력
	void printPerformanceWarning()
	{
		int nTmp = m_nStartTime;
		char szBar[29] = "============================";
		memcpy( szBar, m_szName, strlen(m_szName) > sizeof(szBar)-1 ? sizeof(szBar)-1 : strlen(m_szName) );
		if( strlen(m_szName) < sizeof(szBar)-1 ) szBar[strlen(m_szName)] = ' ';
		oprint( "\n== PERFOMANCE CAUTION : %s\n", szBar );
		for( size_t i = 0; i < m_vResult.size(); ++i ) 
		{
			oprint( "%3d. %-25s : %d ms\n", i, m_vResult[i].szName, m_vResult[i].nTime - nTmp );
			nTmp = m_vResult[i].nTime;
		}
		oprint( "     %-25s : %d ms\n", "TOTAL", m_nEndTime - m_nStartTime );
		oprint( "====================================================\n\n" );
	}

	struct _TAG
	{
		_TAG( const char *name, int t ) : szName( name ), nTime( t )    {}

		const char *    szName;
		int             nTime;
	};

	const char*         m_szName;
	int                 m_nStartTime;
	int                 m_nEndTime;
	int                 m_nTimeLimit;
	int                 m_nTM;
	std::vector< _TAG >   m_vResult;
};

#define XPG_SET( x,y )  XPerformanceGuard   ___x_performance_guard( x,y )
#define XPG_DO( x )     ___x_performance_guard.Profile( x )

#endif

