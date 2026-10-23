#ifndef _MESSAGE_H
#define _MESSAGE_H

#ifdef _CHINA
	#define MAX_MESSAGE_NUMBER     3				// 메시지를 표시할 수 있는 개수. 				
#else
	#define MAX_MESSAGE_NUMBER     10				// 메시지를 표시할 수 있는 개수. 				
#endif 

#define MAX_PERSONAL_MESSAGE_NUMBER     5				// 메시지를 표시할 수 있는 개수. 				

#define MAX_MESSAGE_DELAY      200			    // 메시지를 표시하는 시간. 

#define MAX_MESSAGE_LENGTH		256			    // 한 메시지의 최대 길이. 
#define MAX_WAR_MSG_SIZE		128 

#include <Directives.h>
#include "../CommonHeader.h"
#include "..\CommonLogic\WarRank\WarRank.h"
#include "..\Quest\Quest.h"

#include <ddraw.h>

class cltClient;
class cltItem;

class cltMessage{
public:
	SI32		MessageIndex;
	SI32		MessageDelay;
	TCHAR		Message[MAX_MESSAGE_NUMBER][MAX_MESSAGE_LENGTH];
#ifdef _SAFE_MEMORY
	NSafeTArray<COLORREF, MAX_MESSAGE_NUMBER>		MessageColor;
	NSafeTArray<COLORREF, MAX_MESSAGE_NUMBER>		MessageShdowColor;
#else
	COLORREF	MessageColor[MAX_MESSAGE_NUMBER];
	COLORREF	MessageShdowColor[MAX_MESSAGE_NUMBER];
#endif

	DWORD		dwLastWarMsgFrame;
	TCHAR		szWarMsg[MAX_WAR_MSG_SIZE];
	TCHAR		szKilledWar[MAX_WAR_MSG_SIZE];
	TCHAR		szKillPoint[MAX_WAR_MSG_SIZE];
	TCHAR		szWarPlayersMsg[MAX_WARRANK_PERSON_PER_MAP][MAX_WAR_MSG_SIZE];
	
	// [추가 : 황진성 2008. 1. 21 => 흑의 군단 마을 이름과 점수.]
	TCHAR		szWarVillageMsg[MAX_WARRANK_PERSON_PER_MAP][MAX_WAR_MSG_SIZE];
	
	// 수렵장 메시지 
	TCHAR		szHuntMsg[MAX_MESSAGE_LENGTH];
	bool		bHuntMsgSwitch;
	DWORD		dwLastHuntMsgClock;
	
	// 감옥 메시지 
	TCHAR		szPrisonMsg[MAX_MESSAGE_LENGTH];
	bool		bPrisonMsgSwitch;
	DWORD		dwLastPrisonMsgClock;

	
	// 불가사리 사냥 이벤트  메시지 
	TCHAR		szHuntEventMsg[MAX_MESSAGE_LENGTH];
	bool		bHuntEventMsgSwitch;
	DWORD		dwLastHuntEventMsgClock;

	// 모후모후 이벤트  메시지 
	TCHAR		szMofuMofuEventMsg[MAX_MESSAGE_LENGTH];
	bool		bMofuMofuEventMsgSwitch;
	DWORD		dwLastMofuMofuEventMsgClock;

	// 고구려 유적 방어 이벤트  메시지 
	TCHAR		szGoguEventMsg[MAX_MESSAGE_LENGTH];
	bool		bGoguEventMsgSwitch;
	DWORD		dwLastGoguEventMsgClock;

	// 오사카 이벤트  메시지 
	TCHAR		szOsakaEventMsg[MAX_MESSAGE_LENGTH];
	bool		bOsakaEventMsgSwitch;
	DWORD		dwLastOsakaEventMsgClock;

	// 강화도  메시지 
	TCHAR		szKanghwadoEventMsg[MAX_MESSAGE_LENGTH];
	bool		bKanghwadoEventMsgSwitch;
	DWORD		dwLastKanghwadoEventMsgClock;

	// Daily Quest 메시지 
	TCHAR		szDailyQuestMsg[MAX_MESSAGE_LENGTH];
	bool		bDailyQuestMsgSwitch;
	DWORD		dwLastDailyQuestMsgClock;

	// 마을 주민 채팅 메시지. 
	TCHAR		szVillageChatMsg[MAX_MESSAGE_LENGTH];
	bool		bVillageChatMsgSwitch;
	DWORD		dwLastVillageChatMsgClock;

	// 인스턴스 던전 메시지
	TCHAR		szInstanceMapMsg[MAX_MESSAGE_LENGTH];
	bool		bInstanceMapMsgSwitch;
	DWORD		dwLastInstanceMapMsgClock;

	// 퀘스트 진행 메세지
	DWORD		dwCurQuestInfoMsgClock[MAX_QUEST_TYPE];
	TCHAR		szCurQuestInfoMsgBuffer[MAX_QUEST_TYPE][MAX_MESSAGE_LENGTH];
	TCHAR		szQuestPrintBuffer[MAX_QUEST_TYPE][MAX_MESSAGE_LENGTH];
	bool		bQuestClearSwitch[MAX_QUEST_TYPE];
	bool		bCurQuestInfoMsgSwitch;

	//[진성] 개인정보 메시지 출력 위치 변경 변수 추가. => 2008-6-2
	TCHAR		PersonalMessage[MAX_PERSONAL_MESSAGE_NUMBER][MAX_MESSAGE_LENGTH];
	SI32		PersonalMessageIndex;
	SI32		PersonalMessageDelay;

	//[기형] 배틀로얄 모드 
	TCHAR		szBattleRoyalGameModeMsg[MAX_MESSAGE_LENGTH];
	bool		bBattleRoyalGameModeMsgSwitch;
	DWORD		dwLastBattleRoyalGameModeMsgClock;

	//	파티퀘스트 메세지
	TCHAR		m_szPartyQuestMessage[2][MAX_MESSAGE_LENGTH];	// 완료 메세지와 현재 진행중 메세지 두가지가 동시에 뿌려질수 있습니다.
	bool		m_bPartyQuestGameModeMsgSwitch;
	DWORD		m_dwLastPartyQuestGameModeMsgClock;

	//	풀스크린 여부를 가지고 온다.
	BOOL bFullscreen		;
	
	HFONT		hFont;
	// 배틀로얄용 폰트
	HFONT		m_hFontBattleRoyal;

	cltMessage();
	~cltMessage();


	void init();
	void SetMsg(TCHAR* szmessage, SI32 sieffect = 0, COLORREF rgb = RGB(220, 220, 220), COLORREF shadowrgb = RGB(0,0,0));
		//COLORREF rgb = RGB(250, 60, 50), COLORREF shadowrgb = RGB(100,30,30));

	void Scroll();
		
	void Show(cltClient* pclclient, SI32 sistartx, SI32 sistarty, LPDIRECTDRAWSURFACE7 psurface,	SI32 rx, SI32 ry);

	// 색상 코드가 들어간 문자열을 출력한다
	void DrawMultyColorText(HDC hdc, SI32 rx , SI32 ry, TCHAR * szmessage , COLORREF shadowrgb );
	
	// 
	SI16 ChangeText( HDC hDC, SI32 nPos , TCHAR * szmessage );

	// 전쟁 메시지를 설정한다. 
	void SetBlackArmyWarMsg(TCHAR* szmsg, TCHAR** sortplayernames, SI32 *playerscores  );
	void SetWarMsg(TCHAR* szmsg, TCHAR** sortplayernames, SI32 *playerscores, TCHAR** szSortVillageNames, SI32* siWarVillageScores );
	void ClearWarMsg();

	// 전쟁 메시지를 보여준다. 
	void ShowWarMessage(HDC hdc, SI32 rx, SI32 ry);

	// 전쟁 전과를 표시한다. 
	void ShowWarKill(LPDIRECTDRAWSURFACE7 surface, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 아이템 획득 메시지. 
	//----------------------------------------------

	void SetGetItemMsg(TCHAR* itemName, cltItem * pclItem,GMONEY price);
	
	//----------------------------------------------
	// 수렵장 메시지. 
	//----------------------------------------------
	void SetHuntMsg(TCHAR* szmessage);
	// 수렵장 메시지를 보여준다. 
	void ShowHuntMessage(HDC hdc, SI32 rx, SI32 ry);



	//----------------------------------------------
	// 퀘스트 진행 메시지
	//----------------------------------------------
	void ShowCurQuestInfo(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 감옥 메시지. 
	//----------------------------------------------
	void SetPrisonMsg(TCHAR* szmessage);
	// 감옥 메시지를 보여준다. 
	bool ShowPrisonMessage(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 불가사리 사냥 이벤트 메시지. 
	//----------------------------------------------
	void SetHuntEventMsg(SI32 leftsecond);
	// 불가사리 사냥 이벤트 메시지를 보여준다. 
	void ShowHuntEventMessage(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 모후모후 이벤트 메시지. 
	//----------------------------------------------
	void SetMofuMofuEventMsg(SI32 leftsecond, SI32 siMaMaMofuMofu, SI32 siKilledMofuMofu, SI32 siTotalMofuMofu);
	// 모후모후 사냥 이벤트 메시지를 보여준다. 
	void ShowMofuMofuEventMessage(HDC hdc, SI32 rx, SI32 ry);
	
	//----------------------------------------------
	// 고구려 유적 방어 이벤트 메시지. 
	//----------------------------------------------
	void SetGoguEventMsg(SI32 leftsecond);
	// 고구려 유적 방어  이벤트 메시지를 보여준다. 
	void ShowGoguEventMessage(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 오사카성 이벤트 메시지. 
	//----------------------------------------------
	void SetOsakaEventMsg(SI32 leftsecond);
	// 오사카성  이벤트 메시지를 보여준다. 
	void ShowOsakaEventMessage(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 강화도 이벤트 메시지. 
	//----------------------------------------------
	void SetKanghwadoEventMsg(SI32 leftsecond);
	// 강화도  이벤트 메시지를 보여준다. 
	void ShowKanghwadoEventMessage(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 수렵장 메시지. 
	//----------------------------------------------
	void SetDailyQuestMsg(TCHAR* szmessage);
	// 수렵장 메시지를 보여준다. 
	void ShowDailyQuestMessage(HDC hdc, SI32 rx, SI32 ry);

	//----------------------------------------------
	// 주민 채팅 메시지. 
	//----------------------------------------------
	void SetVillageChatMsg(TCHAR* szmessage);
	// 수렵장 메시지를 보여준다. 
	void ShowVillageChatMessage(HDC hdc, SI32 rx, SI32 ry);

	//-----------------------------------------------
	// 인스턴스 던전 메시지
	//----------------------------------------------
	void SetInstanceMapMsg(TCHAR* szmessage);
	void ShowInstanceMapMsg(HDC hdc, SI32 rx, SI32 ry);

	//[진성] 개인정보 메시지 출력 위치 변경 추가. => 2008-6-2
	void ShowPersonalMsg(HDC hdc, SI32 rx, SI32 ry);
	void SetPersonalMsg(TCHAR* szmessage);
	void PersonalScroll();

	//-----------------------------------------------
	// 배틀로얄 게임 모드
	//----------------------------------------------
	void SetBattleRoyalGameModeMsg(TCHAR* pszMessage);
	// 배틀로얄 게임 모드 메시지를 보여준다. 
	void ShowBattleRoyalGameModeMessage(HDC hdc, SI32 rx, SI32 ry);

	// 파티퀘스트 관련된 메세지를 출력합니다. 
	void ShowPartyQuestMsg(HDC hdc, SI32 rx, SI32 ry);
	void SetPartyQuestGameModeMsg(TCHAR* pszMessage, bool bClear);


};



#endif
