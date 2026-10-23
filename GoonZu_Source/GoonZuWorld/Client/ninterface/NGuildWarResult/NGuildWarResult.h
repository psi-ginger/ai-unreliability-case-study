#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../Common/SystemReward/SystemReward.h"

class CNGuildWarResult : public CDialogController
{
public:
	enum 
	{
		INPUT_HOME,		//	홈팀 데이터 input
		INPUT_AWAY,		//	어웨이 데이터 input
		INPUT_LINE,		//	리스트에 한 라인 데이터 input
	};

	//	길드 멤버 각각이 가져야 할 정보들
	struct	GuildMember	
	{
		TCHAR	m_szUserName[MAX_PLAYER_NAME];	//	유저 이름
		SI32	m_siKillCount;					//	킬수
		SI32	m_siDeathCount;					//	뎃수
		SI32	m_siTeamIndex;					//	Home?Away?
	};


	stListBoxItemData	m_tempItemData;
	GuildMember			m_clGulidMember;						//	리스트에 데이터를 넣을 구조체
	SI32				m_siInputCount;							//	리스트에 데이터 넣을떄 쓰일 카운트
	SI32				m_siActionCount;						//	결과창을 안보여줄때 일정 카운트 후에 show(false) 시켜준다.


public:
	CNGuildWarResult();
	~CNGuildWarResult();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticGuildWarResultDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildWarResultDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void Action();
	void setWinLose(SI32 siWinLose);
	void InputData(SI32 siTeamIndex, TCHAR* pUserName, SI32 siKillPoint, SI32 siDeathPoint);
	void SetList();
};