#ifndef _CLIENT_BOKBULBOK_MANAGER_H_
#define _CLIENT_BOKBULBOK_MANAGER_H_

#include "../../Common/CommonHeader.h"




class CClient_BBBMgr
{
public:
	CClient_BBBMgr();
	~CClient_BBBMgr();


protected:
	bool m_bWaitListResult;
	DWORD m_dwStageStartTime;
	bool m_bStageStartFlag;

	SI16 m_siCurrentStage;

	// 서버와 같은 파일에서 읽어들인 시간 설정들
public:
	SI32 m_siStageStartDelay;				// 스테이지 시작때 카운트 시간 (카운트 시간 + 스타트1초)
	SI32 m_siStageLoseDelay;				// 스테이지 실패 표시 시간
	SI32 m_siStageWinDelay;					// 매 스테이지 윈 표시 시간
	SI32 m_siStagePlayDelay;				// 스테이지 플레이 시간
	SI32 m_siTotalStageWinDelay;			// 최종 스테이지 완료 후 윈 표시 시간
	bool m_b3secDelay;						// 이모티콘 띄우는 딜레이 체크를 위한 플래그
	


public:
	bool m_bGMShowMonsterType;				// 지엠 명령어로 몬스터 위에 타입별로 표시 띄워주기

public:
	// 1. 서버에서 받은 메세지가 [맵에 입장 가능] 일때에 대한 처리- 맵으로 바로이동.
	// 2. 서버에서 받은 메세지가 [대기 리스트에 등록 가능] 일때에 대한 처리
	// 3. 서버에서 받은 메세지가 [복불복 게임 참여 불가] 일때에 대한 처리

	void Action();

	void Warp_GameStage(SI32 siPersonID);				// 해당 캐릭터를 게임할 맵으로 이동 시킨다.

	void Pop_WaitListDlg();
	void Pop_EntryFailDlg();
	void Pop_DetailWaitListDlg(TCHAR** ppWaitList, bool bResult, GMONEY siTotalMoney);
	void Pop_WaitListCancelResult();
	void Pop_WarpMapQuestionDlg();
	void Pop_NotEnoughMoneyDlg();

	bool IsReserved() { return m_bWaitListResult; }							// 내가 대기 리스트에 등록 되어 있는지에 대한 체크 (매번 서버에 물어볼순 없으므로)
	void SetReserveStatus(bool bResult) { m_bWaitListResult = bResult; }	// 나의 대기자 리스트 등록 상태를 변경하는 함수. (클라쪽만.. 서버엔 패킷 따로 보내야 함)
	void RefreshWaitList(TCHAR** ppWaitList);

	void StageStart(SI32 siPersonID, SI16 siCurrentStage, DWORD dwStartTime);
	void StageFail(SI32 siPersonID, DWORD dwStartTime);
	void StageClear();
	void TotalStageClear();

	void Check_AlreadyReserved();
	void Receive_AlreadyReserved(TCHAR** ppWaitList, bool bResult, GMONEY siTotalMoney );

	void Notice_10ClearNotice(SI32 siRewardMoney, TCHAR* pszWinnerName);
	
	void DrawWinImage_ForMe();

protected:
	void _Load_BBBGameInfo();
	void _Clear_BBBStatus();
};

#endif				//_CLIENT_BOKBULBOK_MANAGER_H_