#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Quest/Quest.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

#define SPR_NPCFACE1	TEXT("NInterface/Image/Image_414x406_00_000.SPR") 
#define SPR_NPCFACE2	TEXT("NInterface/Image/Image_414x406_00_001.SPR") 
#define NNEWQUEST_TEXTCOLOR	RGB(0,0,0)
#define NNEWQUEST_CONDITION_REWARD_COLOR	RGB(25,70,230)

class CNNewQuestDlg : public CDialogController
{
public:

	enum { SPR_NONE = 0, SPR_KIND1, SPR_KIND2 };

	CNNewQuestDlg();
	~CNNewQuestDlg();

	void Create();
	void Action();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StaticCallBackDialogNNewQuestDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewQuestDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetNewQuest(SI32 kind);
	void SetAcceptButtonEnable(SI16 type, bool flag);

    void SetEditCondition(TCHAR* pText);
	void SetEditStatus(TCHAR* pText);
	void SetEditQuestReward(TCHAR* pText);
	void SetEditExplain(TCHAR* pText);
	void SetQuestName(TCHAR* pText);
	void SetEditConversation(TCHAR* pText);
	void SetOpenedQuestDlg( BOOL Open ) ; // 퀘스트 숨기기 열기
	
	SI16 GetNpcKind()	{ return m_siNpcKind; }
	void InitNewQuest();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	inline void SetNpcFontIndex(SI32 index)
	{
		m_siNpcFontType = index;
	}

public:
	
	SI16		m_siSprKind;
	SI16		m_siNpcKind;
	SI16		m_siButtonType; // 0: 수락 1: 임무취소 2: 다음 임무 3: 다음
	SI16		m_siOldButtonType;	// 이전 버튼의 상태기억
	SI16		m_siCurrentState;
	SI16		m_siViewState;	// 0 : 대화	1: 임무관련
	SI32		m_siNpcFontType;
	BOOL		m_bOpened;
	SI32        QuestTwinkle;

	CStatic*	m_pStatic[7]; // 0: 조건 1: 임무 2: 보상 3: 퀘스트 제목 4: 설명 이미지 5: 구리댁 이미지 6: 퀘스트 제목

	CEdit*		m_pEditCondition;
	CEdit*		m_pEditStatus;
	CEdit*		m_pEditQuestReward;
	CEdit*		m_pEditExplain[2];	// 0:은 이미지 없는거 1:은 이미지 있을때
	CEdit*		m_pEditConversation;
	
	CButton*	m_pBtnClose;
	CButton*	m_pBtnHide;
	CButton*	m_pBtnAccept;

	TSpr	    m_SprNpcFace1;
	TSpr	    m_SprNpcFace2;

	// 루돌프 퀘스트 용 변수
	SI32		m_siQuestType_Rudolph;

};