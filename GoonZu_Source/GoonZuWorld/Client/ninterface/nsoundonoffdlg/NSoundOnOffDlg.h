#pragma once

#include <Directives.h>
#include "..\..\..\Resource.h"

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"
#include <Graphic.h>

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Item/ItemCommon/ItemUnit.h"

#include "../../InterfaceMgr/DialogController.h"

class CCheckBox;
class CRadioButton;
class CComboBox;
class CStatic;
class CEdit;
class COutLine;
class CImageStatic;

class CNSoundOnOffDlg : public CDialogController
{
public:
	enum
	{
		MAX_CAPTIONKIND_NUM = 3,

		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_BIG_POS_X = 1024 - 280 + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 100,
		DLG_SMALL_POS_X = 800 - 280 + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 100,
	};

	CNSoundOnOffDlg();
	~CNSoundOnOffDlg();

	void Create();

	static	void CALLBACK StaticNSoundOnOffDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NSoundOnOffDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	bool IsShow();
	void Show();
	void Hide();

	void SetTotalSpendTime( SI32 TotalSpendTime, SI32 EventTime );
	void SendCurrentSettingInfo();

	void SetSoundButton(SI16 selectedbutton );
	void SetBGMButton(SI16 selectedbutton );

	void LoadSoundConfig();
	void WriteSoundConfig();

	void LoadLanguageConfig();
	void WriteLanguageConfig();

	void LoadFpsConfig();
	void WriteFpsConfig( SI16 fps );

	void WriteCountryConfig();

	void SetShowPuchikomiCheckBox(bool bCheck);

	bool IsAloneDraw() { return m_bAloneDraw; }

public:
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
	
	/*
	leon add - 2005-03-10-10-00
	* m_bPrivateTradeReject => m_pNSoundOnOffDlg와 체크 상태 공유를 위해서.. 추가함
	* m_siSelectCaptionKindIdex => combobox 현재 선택된 상태를 위해서 추가함
	*/
	// Extended Storable Settings
	bool m_bRightMouseMentAllow;		// 사용자 메뉴 보여주기
	bool m_bShowCharName;				// 캐릭터들의 이름 숨겨주기

	bool m_bWhisperChatReject;			// 모든 귓속말 거부
	bool m_bPrivateTradeReject;			// 모든 거래 거부
	
	bool m_bJewelItemNotifyReject;		// 명품 알림 거부

	bool m_bPartyInviteNotifyReject;	// 파트 초대 거부
	bool m_bFriendAddNotifyReject;		// 친구 추가 거부

	bool m_bSummonEvolveNotifyReject;	// 소환수진화 알림 거부
	

	bool m_bNotifyHeadChat;				// 모든 광고창 거부
	bool m_bNotifyMinigame;				// 미니게임 거부

	bool m_bChatFixed;					// 채팅창 고정 여부  -> 창위치 보존 여부 로 사용[영진] 
	
	SI16 m_siSelectCaptionKindIdex;		// 자막 정보 인덱스

	SI16 m_siSelectedSoundButton;
	SI16 m_siSelectedBGMButton;

	//bool m_bFriendRegisterReject;		// 친구 등록 거부

	bool m_bIsLoadLanguage;			// 처음 
	bool m_bIsLoadFps;

	bool m_bAloneDraw;	// 나혼자만 그려주기, npc 몬스터 포함한다.

	//KHY - 0110 - 채팅 모드 고정
	bool m_bChatTabFixed ;			// 채팅 모드고정 여부 -> 일반, 마을, 길드등등.
	SI16 m_siChatTabNowIndex;		// 현재 채팅 모드 정장.

	// [영훈] 신체기관 창 보여주기 여부
	bool m_bOrganPannelShow;

	bool m_bMykey;					// [진성] 나만의 메뉴.	=> 2008-9-3
	bool m_bGameTip;				// [진성] 게임 팁.		=> 2008-10-6
	bool m_bHelper;					// [진성] 도우미.		=> 2008-10-7

	bool m_bPVP;					// [성웅] PVP 모드 
	
	CCheckBox	 *m_pBackgroundCheckBox;
	CCheckBox	 *m_pEffectCheckBox;
	//CCheckBox	 *m_pWindowmodeCheckBox;
	CCheckBox	 *m_pMinimapRejectCheckBox;
	CCheckBox	 *m_pNotifyFriendAddRejectCheckBox;
	CCheckBox	 *m_pNotifyHeadChatCheckBox;
	CCheckBox	 *m_pNotifyJewelItemRejectCheckBox;
	CCheckBox	 *m_pNotifyMiniGameCheckBox;
	CCheckBox	 *m_pNotifySummonEvolveRejectCheckBox;
	CCheckBox	 *m_pPartyInviteRejectCheckBox;
	CCheckBox	 *m_pPrivateTradeRejectCheckBox;

	CCheckBox	 *m_pRightMouseMenuAllowCheckBox;
	CCheckBox	 *m_pWhisperChatRejectCheckBox;
	CCheckBox	 *m_pChatFixedCheckBox;					// 채팅창 고정
	CCheckBox	 *m_pChatTabFixedCheckBox;				// 채팅 모드 고정
	CCheckBox	 *m_pShowCharNameCheckBox;

	CCheckBox	 *m_pShowPuchikomiCheckBox;				// 푸치코미 공개 / 비공개

	CCheckBox	 *m_pShowOrganCheckBox;					// 신체 기관보기

	CComboBox	 *m_pSelectCaptionKindCombobox;

	CCheckBox	 *m_pMyKeyCheckBox;						// [진성] 나만의 메뉴.	=> 2008-9-3
	CCheckBox	 *m_pGameTipCheckBox;					// [진성] 게임 팁.		=> 2008-10-6
	CCheckBox	 *m_pHelperCheckBox;					// [진성] 도우미.		=> 2008-10-7

	CCheckBox	 *m_pPVPModeCheckBox;					// [성웅] PVP거부 모드

	CEdit		 *m_pTotalSpendTimeEdit;
	CStatic		 *m_pSettingStatic;
	CStatic		 *m_pEffect_BackgroundStatic;
	CStatic		 *m_pGameSettingStatic;
	CStatic		 *m_pCaptionInfoStatic;
	CStatic		 *m_pTotalSpendTimeStatic;

	COutLine	*m_pOutLine_NONAME1	;
	COutLine	*m_pOutLine_NONAME2	;
	COutLine	*m_pOutLine_NONAME4	;

	CCheckBox	*m_pCheckBox_maximization	;

	CImageStatic * m_pEffVolumeButton[5];
	CImageStatic * m_pBGMVolumeButton[5];
	
	CButton	*	m_pSoundMinusButton;
	CButton	*	m_pSoundPlusButton;
	CButton	*	m_pBGMMinusButton;
	CButton	*	m_pBGMPlusButton;

	CStatic*	m_pStaticShowCountry;
	CRadioButton*	m_pRadioShowCountryOn;
	CRadioButton*	m_pRadioShowCountryOff;

	CStatic*	m_pStaticLanguage;
	CComboBox*	m_pComboLanguage;

	CStatic*	m_pStaticFps;
	CComboBox*	m_pComboFps;

	CCheckBox	 *m_pCheckBoxAloneDraw;
};

//#endif
