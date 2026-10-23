//////////////////////////////////////////////////////////////////////////
//-
//////////////////////////////////////////////////////////////////////////


#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CImageStatic;


class NGawiBawiBoDlg : public CDialogController
{
private:
	static const SI32	m_sMaxSelectButtonNum	= 4;
	static const SI32	m_sMaxStatus			= 2;


	enum ENUM_GAWIBAWIBO_SELECT		//	가위바위보 선택 인덱스
	{
		GAWIBAWIBO_NONE,
		GAWIBAWIBO_GAWI,
		GAWIBAWIBO_BAWI,
		GAWIBAWIBO_BO,
	};

	enum ENUM_GAWIBAWIBO_TEAM		//	팀 인덱스
	{
		GAWIBAWIBO_PLAYER,
		GAWIBAWIBO_NPC,
	};

	enum ENUM_GAWIBAWIBO_ICON_INDEX		//	각 팀 상태 아이콘 인덱스
	{
		GAWIBAWIBO_NPC_GAWI			= 0,
		GAWIBAWIBO_NPC_BO			= 1,
		GAWIBAWIBO_NPC_BAWI			= 2,
		
		GAWIBAWIBO_PLAYER_GAWI		= 3,
		GAWIBAWIBO_PLAYER_BO		= 4,
		GAWIBAWIBO_PLAYER_BAWI		= 5,

		GAWIBAWIBO_PLAYER_WIN_1		= 6,
		GAWIBAWIBO_PLAYER_WIN_2		= 7,
		GAWIBAWIBO_NPC_WIN_1		= 9,
		GAWIBAWIBO_NPC_WIN_2		= 10,
		GAWIBAWIBO_PLAYER_LOSE_1	= 12,
		GAWIBAWIBO_PLAYER_LOSE_2	= 13,
		GAWIBAWIBO_NPC_LOSE_1		= 15,
		GAWIBAWIBO_NPC_LOSE_2		= 16,

		GAWIBAWIBO_PLAYER_WAIT_1	= 18,
		GAWIBAWIBO_PLAYER_WAIT_2	= 19,

		GAWIBAWIBO_NPC_WAIT_1		= 21,
		GAWIBAWIBO_NPC_WAIT_2		= 22,
	};	

	enum ENUM_GAWIBAWIBO_STATUS		// 현재 상태
	{
		GAWIBAWIBO_STATUS_NONE,
		GAWIBAWIBO_STATUS_WAIT_SELECT,		// 선택 기다리는중.
		GAWIBAWIBO_STATUS_SELECT_COMPLETE,	// 선택 완료.
		GAWIBAWIBO_STATUS_RESULT,			// 선택 결과.
	};

	enum ENUM_GAWIBAWIBO_CALCULATE_RESULT
	{
		GAWIBAWIBO_RESULT_NONE,
		GAWIBAWIBO_RESULT_ABUSE_WIN,
		GAWIBAWIBO_RESULT_WIN,
		GAWIBAWIBO_RESULT_DRAW,
		GAWIBAWIBO_RESULT_LOSE,
		GAWIBAWIBO_RESULT_END,
	};

	InterfaceFactory	m_InterfaceFactory;

	CButton*			m_pSelectButton[m_sMaxSelectButtonNum];	//	가위바위보  선택 버튼

	CStatic*			m_pStcStatusIcon[m_sMaxStatus];			//	준비중 이미지
	CStatic*			m_pStcResult;							//	결과 이미지
	CStatic*			m_pStcCumulativeMoney;					//	누적금액
	CStatic*			m_pStcAbuseNPC[m_sMaxSelectButtonNum];	//  NPC 부정행위 시 활성화
	CImageStatic*		m_pImgStcBalloon;						//  NPC 말풍선
	CImageStatic*		m_pImgStcCount;							//  NPC 카운트

	TSpr*				m_pSprGawiBawiBo;
	TSpr*				m_pSprResult;

	SI16				m_siSelecteIndex;						//	선택 결과
	SI16				m_siStatus;								//	상태아이콘의 상태
	CCharacterDrawing*  m_pCharDraw;							
	cltAniInfo			m_clAniInfoPlayer;
	cltAniInfo			m_clAniInfoNPC;

	SI16				m_siAniDirection;
	SI16				m_siAniTypePlayer;						// 애니메이션 타입
	SI16				m_siAniTypeNPC;
	POINT				m_lnPositionPlayer;
	POINT				m_lnPositionNPC;


public:

	NGawiBawiBoDlg();
	~NGawiBawiBoDlg();

	void	Create();

	static	void CALLBACK StaticNGawiBawiBoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NGawiBawiBoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void	Action();	
	void	DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY);

	void	SetStatus(SI16 siStatus);

	void	SetGiveUpEnable();
	void	SetStatusIcon();
	void	SetStatusCharacter();

	void	WaitForSelect(){m_siStatus = GAWIBAWIBO_STATUS_WAIT_SELECT;}	//	플레이어의 패 선택을 기다리는중

	void	SeleceComplete(){m_siStatus = GAWIBAWIBO_STATUS_SELECT_COMPLETE;}	//	선택 완료
	void	ResultComplete(){m_siStatus = GAWIBAWIBO_STATUS_RESULT;}	//	선택 완료
	
	void	SetIcon(SI08 siPlayer, SI08 siIconIndex);			//	아이콘 설정

	void	SetCumulativeMoneyText(GMONEY siMoney);				//	누적금액 스트링 설정
	bool	SelectedBtn(SI16 siIndex);							//	선택한 버튼 설정
	void	SetPlayerDraw();	
	void	SetNPCDraw();

	void	Counting(clock_t nCount);							//	카운트 보여줌
	void	EndCount();											//	카운트 종료

	void	DrawPlayer(SI32 ScreenRX, SI32 ScreenRY);
	void	DrawNPC(SI32 ScreenRX, SI32 ScreenRY);

	void	RandomSelect(SI32 siIndex);										//	랜덤으로 패를 선택

	void	RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen);
};
