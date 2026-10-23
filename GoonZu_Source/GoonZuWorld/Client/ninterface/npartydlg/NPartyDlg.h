#ifndef _NPARTY_H_
#define _NPARTY_H_

#include "../../InterfaceMgr/DialogController.h"
#include "..\..\..\Common\PartyMgr\PartyObj.h"
#include "..\..\..\Common\CommonHeader.h"
//#include "../Client/Interface/PartyDlg/PartyDlg.h"

#define MAX_RICHEDIT_OBJ_SIZE							500		// 최대 500 Byte를 데이터에 할당 함.

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class CChatEdit;
class CRadioButton;
class COutLine;

class CLibListMgr;
class cltItem;

#define MAX_MINIMIZE_SLOT 7

enum PARTYQUEST_MESSAGE
{
	PARTYQUEST_MESSAGE_CAN_START,
	PARTYQUEST_MESSAGE_CANNOT_START,
	PARTYQUEST_MESSAGE_FAILED_LESS_USER,
	PARTYQUEST_MESSAGE_FAILED_ADDED_USER,
	PARTYQUEST_MESSAGE_ANSWER_FAILED,
	PARTYQUEST_MESSAGE_TIMEOUT,
};

struct stPartyChatSend
{	
	TCHAR strChatData[ MAX_CHAT_DATA_LENGTH ];

	stPartyChatSend()
	{
		memset( this, 0, sizeof( stPartyChatSend ) );
	}

	stPartyChatSend( TCHAR *ChatData )
	{
		MStrCpy( strChatData, ChatData, MAX_CHAT_DATA_LENGTH );
	}
};

struct stClientPartyUser
{
	bool bPartyStatus;						// 파티에 참여한 유저냐, 아니면 빈 리스트의 유저이냐를 구분짓기 위해서...
	bool bIsPartyLeader;					// 파티장이냐, 파티원이냐	
	SI32 siCharUnique;						// 캐릭터 unique 값
	TCHAR strCharName[ MAX_PLAYER_NAME ];	// 캐릭터 명
	SI32 siExp;								// 획득 경험치
	SI32 siLevel;							// 레벨
	cltCharPos clCharPos;					// 위치 (맵인덱스, 맵좌표)

	void Init()
	{
		bPartyStatus = false;
		bIsPartyLeader = false;
		siCharUnique = 0;		
		memset( strCharName, 0, MAX_PLAYER_NAME );
		siExp = 0;
		siLevel = 0;
		clCharPos.Init();
	}

	void Set( stClientPartyUser *pClientPartyUser )
	{
		memcpy ( this, pClientPartyUser, sizeof( stClientPartyUser ) );
	}
};

class CNPartyDlg : public CDialogController
{
public:

	CNPartyDlg();
	~CNPartyDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNPartyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPartyDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();
	void Action();
//	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

	void EnableButton(CButton* pButton, TCHAR* pText, BOOL Flag = false);	// 버튼 활성화 비활성화

	void Initialize();
	bool IsPartyStatus();
	bool IsPartyLeader();
	bool IsPartyUser( SI32 CharUnique );

	SI32 GetPartyIndex();

//	void PartyCreate( SI32 PartyIndex );
	void PartyCreateJoin( SI32 PartyIndex, SI32 LeaderPersonID, SI32 LeaderCharUnique, TCHAR *strLeaderChatName, SI32 LeaderLevel, cltCharPos* LeaderCharPos , SI32 MemberPersonID, SI32 MemberCharUnique, TCHAR *strMemberCharName, SI32 MemberLevel, cltCharPos* MemberCharPos );	
	void PartyJoin( SI32 PartyIndex, SI16 UserListIndex,SI16 ItemGetPattern );
	void PartyDelete( SI32 PartyIndex );
	void PartyAddUser( bool PartyLeader, SI32 CharUnique, TCHAR *CharName, SI16 UserListIndex, SI32 Level, cltCharPos* clCharPos );
	void PartyDelUser( SI32 CharUnique, SI16 UserListIndex );
	void PartyForceDelUser( SI32 CharUnique, SI16 UserListIndex );
	void PartyChangeLeader( SI32 CharUnique, SI16 UserListIndex );
	void PartyNotifyExp( SI32 CharUnique, SI16 UserListIndex, SI32 Exp );

	void AddChat( TCHAR *strUserID, TCHAR *strChatData);
	void AddChat( TCHAR *strChatData);
	void AddName( TCHAR *PlayerName);
	void AddText( TCHAR *ChatText);
//	void SetFont( COLORREF Color, DWORD dwMask = CFM_COLOR|CFM_SIZE|CFM_BOLD|CFM_ITALIC|CFM_FACE, DWORD dwEffects=~(CFE_AUTOCOLOR), TCHAR *FontKind=TEXT("굴림"), SI16 YHeight=RICHEDIT_FONT_HEIGHT );
	void SendPartyChat();

	SI32 FindUserSelectedIndex( TCHAR *CharName );

	void PrintUserNum();

	void ReloadPartyUserList();

	bool PopPartyChatSendData( stPartyChatSend *pPartyChatSend );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
	void ItemGetPatternButtonEnable(bool bEnable);
	void ChangeItemGetPattern( SI16 ItemGetPattern );
	
	void PartyNotifyItemGet(SI32 CharUnique, cltItem* pclItem);
	void SetPartyUserPos(SI32 CharUnique, cltCharPos* pclCharPos);
	void SetPartyUserLevel(SI32 CharUnique, SI32 Level);
	cltCharPos* GetPartyUserPos(SI16 partyuserindex);
	void Minimi(BOOL bMini);	// 최소화 루틴. TRUE = 최소화 시킨다. FALSE = 다시 되돌린다
	BOOL IsMinimi(){	return m_bMini; };
	SI16 GetMinimizedSlot(){return m_siMinimizedSlot;};

		// [영훈] 해적선 : 해적선안에서는 파티를 떠날 수 없다.(버튼 막음)
	void DoNotLeaveParty();

	bool IsPartyMember( const SI32 siFindCharID );

	void StartPartyQuestMessage(SI32 siMessage);	// 파티퀘스트 시작시 메세지 관련
	void SetInviteCharUnique(SI32 siInviteCharUnique){m_siInviteCharUniqueForPartyQuest = siInviteCharUnique;}

	void UpdatePartyQuestBtn();

	BOOL m_bMini;
	SI16 m_siOldPosX;
	SI16 m_siOldPosY;
	SI16 m_siMinimizedSlot; //[영진] 최소화 버튼이 위치한곳의 슬롯 미니맵 밑에 [3][2][1]  0이면 미니가 아니다. 

	bool m_bPartyStatus;			// 파티중이냐, 파티중이 아니냐
	bool m_bIsPartyLeader;			// 파티장이냐, 파티원이냐

	SI32 m_siPartyIndex;	
 
	COLORREF ColorIndex[6];

	stClientPartyUser m_stClientPartyUser[ MAX_PARTY_USER_NUM ];
	SI16 m_siPartyUserNum;

	SI32 m_siSelectedUserListIndex;
	SI16 m_siItemGetPattern; // 아이템 획득 방식 기억 변수

	TCHAR *m_strViewData;
	TCHAR *m_strChatData;

	CLibListMgr *m_pChatDataList;

	CStatic*	m_pStatic[3]; // 0: 인원 표시, // 1: 파티채팅, //2: 획득방식 글자
	CList*		m_pList;
	CButton*	m_pButton;
	CButton*	m_pMoveButton;
	CEdit*		m_pEdit;  //   파티말 쓰기
	CRadioButton* m_pRadioItemGetPattern[4]; // 0 : 각자획득, 1: 순환획득, 2: 막타획득, 3: 랜덤획득
		
	CImageStatic* m_pImageMinimized;
	
	CButton*	m_pButtonMinimize;
	
	COutLine*	m_pOutLine[2];
	
	CChatEdit*  m_pChatEdit; //  채팅 표시

	SI32		m_siInviteCharUniqueForPartyQuest;	// 파티퀘스트에 사용되는 초대상대유니크
	CButton*	m_pBtnStartQuest;	// 파티 퀘스트 시작

	// 파티매칭
	bool		m_bIsPartyQuestParty;	// 파티퀘스트를 위한 파티인가?				// 아래 세 값은 처음 파티가 만들어질때 서버에서 받은 파티장인 사람의 정보를 바탕으로 채워짐.
	SI32		m_siCategory;		// 파티매칭 파티일경우 카테고리에 (0 <=) 값이 들어옴
	SI32		m_siMission;		// 파티매칭 파티일경우 미션에 (0 <=) 값이 들어옴
	void		Set_PartyQuestInfo(bool isPartyQuest, SI32 siCategory, SI32 siMission);
};

#endif