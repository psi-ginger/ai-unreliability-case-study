
#define NOSUSPICTION_PRISON_TIME	5	// 매크로 사용자 허위 신고시 수감 시간(분) 
//---------------------------------
// 2004/9/01 성준엽
//---------------------------------

#ifndef _AUTOHUNTING_H
#define _AUTOHUNTING_H

#include <Directives.h>

#define MAX_SUSPECT_NUM		3000

#define PRISON_TIME			300

#define CLIENT_UPDATE_TIME			2
#define NOSUSPICTION_PRISON_TIME	5	// 매크로 사용자 허위 신고시 수감 시간(분) 

class cltAutoHuntingManager{
public:
	cltAutoHuntingManager();
	~cltAutoHuntingManager();

public: // Client
/*	void Create( HINSTANCE hInst, HWND hParent );

	static BOOL CALLBACK StaticAutoHuntingDlgMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK AutoHuntingDlgMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// Txt : 요구단어,request : 의뢰자 이름
	void Set(TCHAR * Txt,SI32 slot,SI32 minuts,SI16 prisontime,SI16 prisonerpenalty);
	void Hide();

	void SetLeftTime(SI32 time)
	{
		m_siLeftTime = time;
	}
	SI32 GetLeftTime()
	{
		return m_siLeftTime ;
	}*/

public: // Server
	void Init();
	bool Set(const TCHAR * Txt,TCHAR * request,TCHAR * suspect,SI32* slot,SI32 requestid);
	void SetStaticTime(SI32 totaltime);
	void Action();
	void DeletePerson(const TCHAR* szSuspectName);
	void DeletePerson(const SI32 siSlot);
	bool CheckAnswer(const TCHAR* szSuspectName,const TCHAR* szAnswer,SI32 siSlot,SI32* siCheck);
	bool IsRequest(const TCHAR* name);
	void PrisonUser(TCHAR* name,SI32 requestid,SI32 time,TCHAR * reason);

private: // Client
/*	HWND		m_hWnd;
	HINSTANCE	m_hInst;

	HWND	m_hDlg;

	HBRUSH m_hStaticBrush;
	HBRUSH m_hDlgBrush;

	SI32	m_siLeftTime;
	SI32	m_siSlot;*/

protected: // Server
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_SUSPECT_NUM>		m_bActionSwitch;

	NSafeTArray<SI32, MAX_SUSPECT_NUM>		m_szRequestPersonID; // 의뢰인의 personid
	NSafeTArray<SI32, MAX_SUSPECT_NUM>		m_siCheckNum;		// 오답이 몇번째인가

	NSafeTArray<DWORD, MAX_SUSPECT_NUM>		m_dwWatingClock;
	NSafeTArray<DWORD, MAX_SUSPECT_NUM>		m_dwUpdateColok;
#else
	bool	m_bActionSwitch[MAX_SUSPECT_NUM];

	SI32	m_szRequestPersonID[MAX_SUSPECT_NUM]; // 의뢰인의 personid
	SI32	m_siCheckNum[MAX_SUSPECT_NUM]; // 오답이 몇번째인가

	DWORD	m_dwWatingClock[MAX_SUSPECT_NUM];
	DWORD	m_dwUpdateColok[MAX_SUSPECT_NUM];
#endif

	TCHAR	m_szRequestName[MAX_SUSPECT_NUM][MAX_PLAYER_NAME]; // 의뢰인
	TCHAR	m_szSuspectName[MAX_SUSPECT_NUM][MAX_PLAYER_NAME]; // 용의자

	TCHAR	m_szRequestTxt[MAX_SUSPECT_NUM][MAX_ITEM_NAME]; // 정답
};

#endif
