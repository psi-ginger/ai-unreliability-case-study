#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../Common/VoteMgr/VoteBaseInfo.h"
#include "../../Common/CommonHeader.h"

#include "../NLib/NUpdateTimer.h"


class CImageStatic;
class CStatic;
class CButton;
class CEdit;


class CNVoteStatusDlg : public CDialogController
{
enum VOTESTATUS_VARIABLE
{
	VOTESTATUS_BASE_DIALOG_WIDTH		= 250	// 다이얼로그 기본 넓이
	,VOTESTATUS_BASE_DIALOG_X			= 83	// 다이얼로그 기본 시작 X
	,VOTESTATUS_BASE_DIALOG_Y			= 260	// 다이얼로그 기본 시작 Y

	,VOTESTATUS_CANDIDATE_START_X		= 0		// 후보자가 그려질 처음 시작 위치(X)
	,VOTESTATUS_CANDIDATE_START_Y		= 0		// 후보자가 그려질 처음 시작 위치(Y)

	,VOTESTATUS_CANDIDATE_WIDTH			= 30	// 후보자 그림 크기(넓이)
	,VOTESTATUS_CANDIDATE_HEIGHT		= 40	// 후보자 그림 크기(높이)

	,VOTESTATUS_MODE_MINIMUM			= 0		// 최소화 모드
	,VOTESTATUS_MODE_MAXIMUM			= 1		// 최대화 모드

	,VOTESTATUS_MAX_FRAME				= 2		// 한 에니당 차지하는 프레임 수

	,VOTESTATUS_DRAWMODE_RUN			= 0		// 달리는 그림
	,VOTESTATUS_DRAWMODE_LOSE			= 1 * VOTESTATUS_MAX_FRAME	// 졌을때 그림
	,VOTESTATUS_DRAWMODE_WIN			= 2 * VOTESTATUS_MAX_FRAME	// 당선됬을 때 그림

	,VOTESTATUS_VOTE_READY				= 0		// 개표 상태 - 대기중
	,VOTESTATUS_VOTE_RUN				= 1		// 개표 상태 - 진행중
	,VOTESTATUS_VOTE_END				= 2		// 개표 상태 - 종료

};

public:
	CNVoteStatusDlg();
	~CNVoteStatusDlg();

	void Create();
	void Init();

	static void CALLBACK StaticVoteStatusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NVoteStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

//-- 데이터 설정관련
public:
	void SetStartVoteStatus( TCHAR** pptszCnadidateName, SI32 siTotalVote,  SI08 siCandidateNum, SI32 siTopVoteNum );
	void SetVoteStatus( SI32 *psiNowVote );

//-- 표시관련
public:
	void DrawCandidate( SI32 siRX, SI32 siRY );
	void WriteCandidate( SI08 siShowCandidateNumber );
	
//- 윈도우 모드 관련
public:
	void WindowModeChange( bool bWindowMode );

private:
	void SaveNowDialog();
	void EndVote();
	void VoteCount();
	SI08 SearchTopCandidate();
	

//-- 컨트롤용 변수
private:
	//CImageStatic		*m_pcisBack;
	CStatic				*m_pcsVote;
	CStatic				*m_pcsPercentage;
	CButton				*m_pcbMinimum;
	CButton				*m_pcbMaximum;
	CStatic				*m_pcsDisplay;
//-- 후보자 관련 변수
	TCHAR				m_tszCandidateName[MAX_CANDIDATURE_NUM][MAX_PLAYER_NAME];
	SI32				m_siVote[MAX_CANDIDATURE_NUM];			// 서버로 부터 얻어온 현재 득표수
	SI32				m_siDrawVote[MAX_CANDIDATURE_NUM];		// 케릭터 Draw를 위한 저장변수
	SI08				m_siDrawMode[MAX_CANDIDATURE_NUM];		// 드로우 상태

	SI32				m_siDrawOrder[MAX_CANDIDATURE_NUM];		// 후보자 그리는 순서

	SI32				m_siTotalVote;		// 총 투표수
	SI32				m_siNowTotalVote;	// 현재 투표수

	SI08				m_siCandidateNum;	// 후보자 수
	SI08				m_siNowShowNum;		// 현재 보여지는 후보자 번호
	SI32				m_siTopVoteNum;		// 그래프의 최대 넓이(최대 득표자수의 득표수)

	SI08				m_siNowFrame;		// 현재 그리는 프레임

	SI08				m_siVoteStatus;		// 개표 현황

	TSpr				m_sprCandidate;		// 후보자  그림

//-- 윈도우모드 관련 변수
private:
	bool				m_bWindowMode;		// 현재 윈도우 모드

	SI16				m_siWindowPosX;		// 최대모드일때 윈도우 포지션 X
	SI16				m_siWindowPosY;		// 최대모드일때 윈도우 포지션 X

	SI16				m_siWindowWidth;	// 최대모드일때 윈도우 넓이
	SI16				m_siWindowHeight;	// 최대모두일때 윈도우 높이

	bool				m_bLock;			// 데이터 변경여부저장


	NUpdateTimer		m_kUpdateTimer_WriteCandidate;	// 후보자 이름이 보여지는 부분
	NUpdateTimer		m_kUpdateTimer_DrawCandidate;	// 후보자 그림이 그려지는 부분
	NUpdateTimer		m_kUpdateTimer_End;				// 끝났을 때 종료키는 타이머
};
