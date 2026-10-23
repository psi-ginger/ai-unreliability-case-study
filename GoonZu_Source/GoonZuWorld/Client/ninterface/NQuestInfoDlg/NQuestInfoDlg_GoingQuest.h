// [영훈] 개편된 퀘스트 알림창
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"

enum DAILY_QUESTTYPE
{
     DAILY_QUESTTYPE_1	= (MAX_QUEST_TYPE + 1)	// 젝퀘스트
	,DAILY_QUESTTYPE_2							// 데미안 퀘스트
	,DAILY_QUESTTYPE_3							// 터틀백 퀘스트
	,PARTY_QUESTTYPE							// all quest type 밑으로 추가했습니다. 기존의 퀘스트타입에 영향을 받지 않기 위해	

	,ALL_QUEST_TYPE								// MAX_QUEST_TYPE + DAILY_QUESTTYPE + 1
};


class cltDailyQuestInfo
{
public:
	SI16	m_siDailyQuestType;		// 데일리 퀘스트 1,2,3
	SI32	m_siCondition;			// 1,2일때는 몬스터/ 3일때는 아이템
	SI16	m_siNowAmount;			// 현재 내 갯수
	SI16	m_siNeedAmount;			// 필요한 갯수
	SI32	m_siLeftTime;			// 남은 시간
	bool	m_bHidden;				// 히든퀘스트 여부

	SI32	m_siRewardExp;			// 보상 경험치
	GMONEY	m_gmRewardMoney;		// 보상 돈
	SI32	m_siRewardItemUnique;	// 보상 아이템
	SI16	m_siRewardItemAmount;	// 보상 아이템 갯수	

public:
	cltDailyQuestInfo()
	{
		Init();
	}

	cltDailyQuestInfo( cltPDailyQuestInfo* pclDailyQuestInfo1 );
	cltDailyQuestInfo( cltPDailyQuest2Info* pclDailyQuestInfo2 );
	cltDailyQuestInfo( cltPDailyQuest3Info* pclDailyQuestInfo3 );

	void Init()
	{
		ZeroMemory( this, sizeof(cltDailyQuestInfo) );
	}

	bool GetDailyQuestInfo( OUT TCHAR* pszBuffer, IN SI32 siBufferSize, OUT SI32* siGimgIndex, OUT SI32* siFont );
	bool GetDailyQuestRewardText( OUT TCHAR* pszBuffer, IN SI32 siBufferSize );

	void DailyQuestCancel();

};

const SI32 Const_Max_Buf_Size = 2048;


class CNQuestInfoDlg_GoingQuest : public CDialogController
{

	// 변수영역
private:
	InterfaceFactory	m_InterfaceFactory;
	SI32				m_siGoingQuestList[ALL_QUEST_TYPE];
	

	// 함수영역
public:
	CNQuestInfoDlg_GoingQuest();
	~CNQuestInfoDlg_GoingQuest();

	void Create(CControllerMgr* pParent);
	
	static void CALLBACK StaticQuestInfoDlg_GoingQuestProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NQuestInfoDlg_GoingQuestProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Action();

	void Show();
	void Hide();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

private:
	void LoadGoingQuestList();

	bool AddDailyQuestInfo( cltDailyQuestInfo* pclDailyQuestInfo );

	bool AddQuestInfo( SI32 siQuestType, cltPersonQuestUnitInfo* pclPQuestInfo );

	bool AddPartyQuestInfo(TCHAR* szInfo);

	void SetQuestListBoxItemData( SI32 siGimgIndex, SI32 siFont, TCHAR* pszText );
	void SetQuestListBoxItemData( SI32 siGimgIndex, SI32 siFont, TCHAR* pszText, COLORREF colorRef);

	void SelectQuestInfo( SI32 siSelectedIndex );
	void GetMagicLampQuestInfoText( OUT TCHAR* pszBuffer, IN SI32 siBufferSize );

	void QuestContinue( SI32 siSelectedIndex );

	void ShowQuestCancelMsgBox( SI32 siSelectedIndex );
	void QuestCancel( SI32 siQuestType );
	
};
