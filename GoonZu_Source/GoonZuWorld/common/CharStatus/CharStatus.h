//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _CHARSTATUS_H
#define _CHARSTATUS_H

#include <windows.h>
#include <directives.h>

#include "CommonHeader.h"

#define MAX_CHARSTATUS_PAGE_NUMBER	5

#define WANTIED_ANIMAL_CHANGE_TYPE	2

class cltCharClient;
class cltParametaBox;
class cltSummonScroll;
class cltCharManager;
class cltItem;

class cltCharKindChange
{
public:
	HWND m_hDlg;
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;

	SI32 m_siSelectedUnique;

	HBITMAP m_hBitmap;

	cltCharKindChange();
	~cltCharKindChange();

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);

	static BOOL CALLBACK CharKindChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharKindChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void SetPortrait(HWND hDlg);
	void Close();

};

class cltCharNameChange
{
public:
	HWND m_hDlg;
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;

	HBRUSH m_hStaticBrush;
	HBRUSH m_hDlgBrush;

	cltCharNameChange();
	~cltCharNameChange();

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);

	static BOOL CALLBACK CharNameChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharNameChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Close();

};

class cltHorseTypeChange
{
public:
	HWND m_hDlg;
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;

	HBITMAP m_hHorseBitmap;

	SI32 m_siSelectedUnique;
	SI32 m_siOriUnique;
	SI32 m_siMaxUnique;

	cltHorseTypeChange();
	~cltHorseTypeChange();

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd,SI32 horseunique);

	static BOOL CALLBACK HorseTypeChangeStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK HorseTypeChangeProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void SetHorsePortrait(HWND hDlg);
	void Close();

};

/*
class cltCharStatusExplain
{
public:
	HWND m_hDlg;	
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;
	HBITMAP m_hHitBitmap;

	SI32 m_siWeaponType;

	cltCharStatusExplain();
	~cltCharStatusExplain();

	bool CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK CharStatusExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharStatusExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	SI32 GetWeaponType(char* pWeaponType);
	void ShowWeaponHit(HWND hDlg, SI32 siWeaponType);

};
*/

/*
class cltHorseBreedExplain
{
public:
	HWND m_hDlg;	
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;
	HBITMAP m_hHitBitmap;

	SI32 m_siBreedType;

	cltHorseBreedExplain();
	~cltHorseBreedExplain();

	bool CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK HorseBreedExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK HorseBreedExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	SI32 GetBreedType(char* pBreedType);
	void ShowBreedHit(HWND hDlg, SI32 siBreedType);

};
*/

/*
class cltSummonBreedExplain
{
public:
	HWND m_hDlg;	
	BOOL m_DlgRectInitSwitch;
	RECT m_DlgRect;
	HBITMAP m_hHitBitmap;

	SI32 m_siBreedType;

	cltSummonBreedExplain();
	~cltSummonBreedExplain();

	bool CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK SummonBreedExplainStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK SummonBreedExplainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	SI32 GetBreedType(char* pBreedType);
	void ShowBreedHit(HWND hDlg, SI32 siBreedType);

};
*/

class cltCharStatus
{
public:
	HWND m_hDlg;	
	HWND TabDlg[MAX_CHARSTATUS_PAGE_NUMBER];

	BOOL DlgRectInitSwitch;
	RECT DlgRect;
	HBITMAP m_hHorsePortraitBitmap;
	HBITMAP m_hSummonPortraitBitmap;
	HBITMAP m_hStatusHintHitBitmap;
	HBITMAP m_hHorseHintHitBitmap;
	HBITMAP m_hSummonHintHitBitmap;
	HBITMAP m_hSummonReviveBitmap;
	HBITMAP m_hStatusInitHitBitmap;
	HBITMAP m_hHorseInitHitBitmap;
	HBITMAP m_hHorseAddStatusHitBitmap;
	HBITMAP m_hHorseTypeHitBitmap;
	HBITMAP m_hStatusChangeNameBitmap;
	HBITMAP m_hStatusChangeKindBitmap;
	HBITMAP m_hSummonStatusInitBitmap;
	HBITMAP m_hNoLimitWarpTicketBitmap;

	char strHorseName[ MAX_HORSE_NAME ];
	char strSummonName[ MAX_SUMMON_NAME ];

	SI32 CharUnique;

	clock_t			m_LastTakeInOutTime;
	SI32 CurrentStatusPage;
	bool bBonusButtonEnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	bool bBonusButton2EnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	BOOL BonusSummonButtonEnableSwitch;		// 소환수 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	BOOL BonusHorseButtonEnableSwitch;		// 말 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수.
	
	BOOL m_bChangedInitSummonStatus;
	BOOL m_siLastSummonComboList;			// 마지막으로 Combo List로 설정된 소환수 
	SI32 m_siPortraitSummonKind;


	cltCharManager* pclCM;
	//cltCharStatusExplain* pclCharStatusExplain;
	//cltHorseBreedExplain* pclHorseBreedExplain;
	//cltSummonBreedExplain* pclSummonBreedExplain;
	cltHorseTypeChange* pclHorseTypeChange;
	cltCharNameChange* pclCharNameChange;
	cltCharKindChange* pclCharKindChange;

	HBRUSH m_hDlgBrush;
	HBRUSH m_hEditBrush;
	HBRUSH m_hStaticBrush;



	cltCharStatus(cltCharManager* pclCM);
	~cltCharStatus();

	BOOL CreateTabDlg(HWND hDlg, SI32 page, SI32 dlg, DLGPROC proc);


	BOOL CALLBACK CharStatusDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK CharStatusBattleStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharStatusBattleProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK CharStatusSummonStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharStatusSummonProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//------------------------------------------
	// 자산현황
	//------------------------------------------
	static BOOL CALLBACK CharStatusEssetStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharStatusEssetProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	void ShowMyGuildInfo( SI32 villageunique, char* guildname, char* guildrankname );	// 소속 길드 정보 표시



	//------------------------------------------
	// CharStatus-Horse.cpp
	//------------------------------------------
	// 말의 상태를 업데이트 한다.
	void UpdateHorseStatusInfo(HWND hDlg);

	static BOOL CALLBACK CharStatusHorseStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharStatusHorseProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	//------------------------------------------
	// CharStatus-Quest.cpp
	//------------------------------------------
	static BOOL CALLBACK CharStatusQuestStaticProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK CharStatusQuestProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	void UpdateCharStatusInfo(HWND hDlg);
	// 소환수의 상태를 업데이트 한다.
	void UpdateSummonStatusInfo(HWND hDlg);
	
	void SendUseBonusMessageToServer(SI32 pos, SI32 amount);
	void SendUseSummonBonusMessageToServer(HWND hDlg, SI16 data,bool bInit = false );
	void SendSummonTakeInOutMessageToServer(HWND hDlg,SI32 siSummonUnique = 0);
	void SendSummonAttackTypeMessageToServer(HWND hDlg, SI32 siAttackType);
	void SendSummonChangeNameMessageToServer(HWND hDlg);
	void SendSummonFreeMessageToServer(HWND hDlg);
	void SendSummonReviveMessageToServer(HWND hDlg);
	void SendUseHorseBonusMessageToServer(char data);
	void SendSummonStatusInitMessageToServer(HWND hDlg);
	void SendSummonAutoHealMessageToServer(HWND hDlg,SI16 data,bool autoheal);

	// 공격력 상세 정보를 설정한다. 
	void ShowDetailDamageInfo(HWND hDlg, cltParametaBox* pclPB, cltItem* pclitemlist, SI32 activesweapon);
	// 방어력 상세 정보를 설정한다. 
	void ShowDetailArmourInfo(HWND hDlg, cltParametaBox* pclPB);
	// 이동속도 상세 정보를 설정한다. 
	void ShowDetailSpeedInfo(HWND hDlg, cltParametaBox* pclPB, cltCharClient* pclchar);
	// 근력 상세 정보를 설정한다. 
	void ShowDetailStrInfo(HWND hDlg, cltParametaBox* pclPB);
	// 순발력 상세 정보를 설정한다. 
	void ShowDetailDexInfo(HWND hDlg, cltParametaBox* pclPB);
	// 도력 상세 정보를 설정한다. 
	void ShowDetailMagInfo(HWND hDlg, cltParametaBox* pclPB);
	// 생명력 상세 정보를 설정한다. 
	void ShowDetailVitInfo(HWND hDlg, cltParametaBox* pclPB);
	// 손재주 상세 정보를 설정한다. 
	void ShowDetailHndInfo(HWND hDlg, cltParametaBox* pclPB);
	// 소환수 스크롤 정보 
	void ShowDetailScrollInfo(HWND hDlg, cltSummonScroll *pScroll );
	
	// 현재 ComboList에 설정된 소환수 Index를 얻어온다.
	BOOL GetComboSummonNameIndex(HWND hDlg, char *CurrSummonName, SI32 *siSummonIndex);
	void InitSummonStatusInfo(HWND hDlg);
	void UpdateSummonPortrait(HWND hDlg);

};

#endif
