#ifndef _SOUNDONOFFDLG_H_
#define _SOUNDONOFFDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>

class CSoundOnOffDlg
{
public:
	enum 
	{
		MAX_CAPTIONKIND_NUM = 3,
	};

	CSoundOnOffDlg();
	~CSoundOnOffDlg();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticSoundOnOffDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK SoundOnOffDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	
	bool IsShow();
	void Show();
	void Hide();

	void SetTotalSpendTime( SI32 TotalSpendTime );
	void SendCurrentSettingInfo();

	void LoadSoundConfig();
	void WriteSoundConfig();

public:
	HWND m_hDlg;
	bool m_bShow;

	// Previous Storable Setting
	bool m_bEffectSound;				// 효과음 사용
	bool m_bBackGroudnSound;			// 배경음 사용
	bool m_bBigDisplayMode;				// 1024x768 : true or 800x600 : false
	bool m_bFullScreenMode;				// 풀스크린 모드 사용
	bool m_bTmpFullScreenMode;			// 임시 풀스크린 모드 플래그
	bool m_bMaxWindowMode;				// 최대창 크기 모드 사용
	bool m_bTmpMaxWindowMode;			// 임시 최대창 크기 모드 플래그
	bool m_bMiniMapShow;				// 미니맵 보기
	
	// Extended Storable Settings
	bool m_bRightMouseMentAllow;		// 사용자 메뉴 보여주기
	bool m_bShowCharName;				// 

	bool m_bWhisperChatReject;			// 모든 귓속말 거부
	bool m_bPrivateTradeReject;			// 모든 거래 거부
	
	bool m_bJewelItemNotifyReject;		// 명품 알림 거부

	bool m_bPartyInviteNotifyReject;	// 파트 초대 거부
	bool m_bFriendAddNotifyReject;		// 친구 추가 거부

	bool m_bSummonEvolveNotifyReject;	// 소환수진화 알림 거부

	bool m_bNotifyHeadChat;				// 모든 광고창 거부
	bool m_bNotifyMinigame;				// 미니게임 거부

	bool m_bChatFixed;					// 채팅창 고정 여부 -> 창위치 기억 여부 [영진]

	//KHY - 0110 - 채팅 모드 고정
	bool m_bChatTabFixed;					// 채팅 모드고정 여부 -> 일반, 마을, 길드등등.
	SI16 m_siChatTabNowIndex;				// 현재 채팅 모드 정장.
	
	SI16 m_siSelectCaptionKindIdex;		// 자막 정보 인덱스

	SI32 m_siSelectedBGMButton;
	SI32 m_siSelectedSoundButton;



	//bool m_bFriendRegisterReject;		// 친구 등록 거부

	bool m_bAloneDraw;
	
};

extern CSoundOnOffDlg g_SoundOnOffDlg;



#endif
