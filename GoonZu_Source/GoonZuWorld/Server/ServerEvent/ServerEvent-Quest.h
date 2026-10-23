/*

*/
#pragma once

#include "Directives.h"
#include "CommonLogic.h"


// 총 서버 퀘스트가 갖고 있을 수 있는 문자수
const SI32 Const_ServerEvent_Max_QuestCharacter		= 15;
const SI32 Const_ServerEvent_MulNum					= 2;
// 총 퀘스트 데이터 수
const SI32 Const_ServerEvent_Max_QuestData			= 50;

const SI32 Const_ServerEvent_RewardItemGetMinLevel	= 20;

enum SERVEREVENT_MESSAGETYPE
{
	SERVEREVENT_MESSAGETYPE_NONE	= 0,	// 이런타입은 없다,

	SERVEREVENT_MESSAGETYPE_QUEST_FAIL,		// 시간이 다되었는데도 퀘스트를 성공하지 못해 실패했다
	SERVEREVENT_MESSAGETYPE_GETCHAR,		// 문자를 받는다(퀘스트 성공과 같다)
	SERVEREVENT_MESSAGETYPE_CHAR_FAIL,		// 문자 받기를 실패했다.(퀘스트는 성공했지만 문자만 받지 못함)
	SERVEREVENT_MESSAGETYPE_QUEST_ALLCLEAR,	// 모든 퀘스트를 클리어 했다.
	
	/*------------------------------------------------------------------------------
	//
	//	새로운 메시지 타입은 이 위에 추가해 주세요
	//
	------------------------------------------------------------------------------*/
	SERVEREVENT_MESSAGETYPE_END
};

enum SERVEREVENT_TYPE
{
	SERVEREVENT_TYPE_NONE = 0,			// 이런타입은 없다

	SERVEREVENT_TYPE_HUNTLEVEL,			// 01 특정 레벨의 몬스터 잡기(Condition1=이 레벨부터, Condition2=이 레벨까지)
	SERVEREVENT_TYPE_HUNTKIND,			// 02 특정 KIND 몬스터 잡기(Condition1=ATB, Condition2=Kind)
	SERVEREVENT_TYPE_HUNTBOSS,			// 03 특정 보스 KIND 몬스터 잡기(Condition1=Kind)
	
	SERVEREVENT_TYPE_QUESTCLEAR,		// 04 퀘스트 클리어(Condition1=Quest Type)
	
	SERVEREVENT_TYPE_USEITEM,			// 05 아이템 사용하기(Condition1=Item Type, Condition2=Item Unique)
	
	SERVEREVENT_TYPE_MAKELEVELITEM,		// 06 특정 레벨의 아이템 제조(Condition1=이 레벨부터, Condition2=이 레벨까지)
	SERVEREVENT_TYPE_MAKERAREITEM,		// 07 신기 아이템 이상 제조하기
	
	SERVEREVENT_TYPE_INSTANCEMAPCLEAR,	// 08 인던 클리어(Condition1=인스턴스 멥 타입)
	
	SERVEREVENT_TYPE_FISH,				// 09 낚시해서 아이템 얻기
	SERVEREVENT_TYPE_FARM,				// 10 농사해서	아이템 얻기
	SERVEREVENT_TYPE_MINE,				// 11 채광해서 아이템 얻기
	
	SERVEREVENT_TYPE_SKILLUP,			// 12 아무 스킬포인트 올리기

	SERVEREVENT_TYPE_BLACKWARCLEAR,		// 13 흑의군단 승리하기

	SERVEREVENT_TYPE_RESIDENTVILLAGE,	// 14 마을 주민으로 가입하기(Condition1=마을 인덱스)
	
	SERVEREVENT_TYPE_OPEN_GVG,			// 15 길드전을 개최해야 한다
	
	SERVEREVENT_TYPE_COUNTUSER_INMAP,	// 16 특정 맵 인덱스의 유저가 몇명이상 있어야한다

	SERVEREVENT_TYPE_LEVELUP,			// 17 레벨업하기(Condition1=이 레벨이상 올려야한다)


	/*------------------------------------------------------------------------------
	//
	//	새로운 서버 이벤트 타입은 이 위에 추가해 주세요
	//
	------------------------------------------------------------------------------*/
	SERVEREVENT_TYPE_END
};

class CNServerEvent_QuestData
{
protected:
	SI32	m_siEventType;			// 메인 이벤트 타입

	SI32	m_siCondition1;			// 클리어 하기 위해 필요한 조건 1
	SI32	m_siCondition2;			// 클리어 하기 위해 필요한 조건 2
	
	SI32	m_siNeedClearAmount;	// 클리어 하기 위해 필요한 갯수
	
	SI32	m_siRewardItemUnique;	// 클리어시 보상되는 아이템(유니크)
	SI32	m_siRewardItemAmount;	// 보상되는 아이템 갯수

	SI32	m_siLimitMinute;		// 제한시간

public:
	CNServerEvent_QuestData( void );
	CNServerEvent_QuestData( CNServerEvent_QuestData* pclSEQuestData );

	~CNServerEvent_QuestData();

public:
	void	Init( void );
	// 인터페이스 함수
public:
	bool	SetServerEvent_QuestData( 
		SI32	siEventType,		// 서버 이벤트 퀘스트 타입
		
		SI32	siCondition1,		// 조건1(퀘스트를 생성(부여)하기 위한 조건)
		SI32	siCondition2,		// 조건2(실질적으로 검사 조건이 된다)

		SI32	siNeedClearAmount,	// 퀘스트를 클리어 하기 위해 필요한 횟수

		SI32	siRewardItemUnique,	// 보상 아이템
		SI32	siRewardItemAmount,	// 보상 아이템 갯수
	
		SI32	siLimitMinute		// 제한 시간(분)
	);

	bool	SetServerEvent_QuestData( CNServerEvent_QuestData* pclSEQuestData );


	SI32	GetEventType( void ) const						{ return m_siEventType; } 
	void	SetEventType( SI32 siEventType );

	SI32	GetCondition1( void ) const						{ return m_siCondition1; }
	void	SetCondition1( SI32 siCondition1 )				{ m_siCondition1 = siCondition1; }

	SI32	GetCondition2( void ) const						{ return m_siCondition2; }
	void	SetCondition2( SI32 siCondition2 )				{ m_siCondition2 = siCondition2; }

	SI32	GetNeedClearAmount( void )	const				{ return m_siNeedClearAmount; }
	void	SetNeedClearAmount( SI32 siNeedClearAmount )	{ m_siNeedClearAmount = siNeedClearAmount; }

	SI32	GetRewardItemUnique( void )	const				{ return m_siRewardItemUnique; }
	void	SetRewardItemUnique( SI32 siRewardItemUnique )	{ m_siRewardItemUnique = siRewardItemUnique; }

	SI32	GetRewardItemAmount( void )	const				{ return m_siRewardItemAmount; }
	void	SetRewardItemAmount( SI32 siRewardItemAmount )	{ m_siRewardItemAmount = siRewardItemAmount; }

	SI32	GetLimitMinute( void ) const					{ return m_siLimitMinute; }
	void	SetLimitMinute( SI32 siLimitMinute )			{ m_siLimitMinute = siLimitMinute; }
	SI32	DecLimitMinute( SI32 siDecMinute );

};

/**************************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------------------*/
/*                                                    경계선 입니다                                                       */
/*------------------------------------------------------------------------------------------------------------------------*/
/**************************************************************************************************************************/

class CNServerEvent_QuestData_Manager
{
public:
enum SEQD_MANAGER_VARIABLE
{
	SMV_FORHUNTKIND_MAXMAPNUM = 50,
	SMV_FORHUNTKIND_MAXNPCNUM = 30,
};
protected:
#ifdef _SAFE_MEMORY
	NSafeTArray<CNServerEvent_QuestData*, Const_ServerEvent_Max_QuestData>		m_pcltSEQuestData;
#else
	// 서버이벤트가 기록될 클레스
	CNServerEvent_QuestData*	m_pcltSEQuestData[Const_ServerEvent_Max_QuestData];
#endif
	// 현재 서버 이벤트가 등록된 수
	SI32						m_siServerEventDataCount;


public:
	CNServerEvent_QuestData_Manager( void );
	~CNServerEvent_QuestData_Manager();

	void	Init( void );


public:
	bool	LoadQuestDataFile( SI16 siServerUnique );

	SI32	EventTypeStringToInteger( IN TCHAR* pszEventTypeString );
	bool	ConditionToInteger( IN SI32 siEventType, IN TCHAR* pszCondition1, IN TCHAR* pszCondition2, OUT SI32 &rsiCondition1, OUT SI32 &rsiCondition2 );
	bool	GetRandQuestData( OUT CNServerEvent_QuestData* pclSEQuestData, IN SI32 siSelectIndex=-1 );
	SI32	GetIndexFromType( SI32 siEventType, SI32 siStartPos=0 );
	
protected:
	bool	AddServerEvent_QuestData( 
		SI32	siEventType,

		SI32	siCondition1,
		SI32	siCondition2,

		SI32	siNeedClearAmount,

		SI32	siRewardItemUnique,
		SI32	siRewardItemAmount,

		SI32	siLimitMinute
		);

	bool	AddServerEvent_QuestData( IN CNServerEvent_QuestData* pclSEQuestData );

	void	ShowMessageBox( TCHAR* pszEventType, SI32 siCondition1=0, SI32 siCondition2=0 );

	SI32	LoadHuntParentMapIndex( SI32 psiMapIndex[] );
	SI32	FindHuntChildMapIndex( IN SI32 siParentMapIndex ,OUT SI32 psiMapIndex[] );
	SI32	RandHuntMonsterKind( SI32 siMapIndex, SI32 siATB=0 );

	SI32	GetRandMonsterKindForHuntKind( SI32 siATB=0 );
	SI32	GetRandItemUinqueForUseItem( SI32 siItemType );
	SI32	GetRandMapIndexForUserCount( void );
	
};

/**************************************************************************************************************************/
/*------------------------------------------------------------------------------------------------------------------------*/
/*                                                    경계선 입니다                                                       */
/*------------------------------------------------------------------------------------------------------------------------*/
/**************************************************************************************************************************/

enum SERVEREVENT_QUEST_PACKETTYPE
{
	SERVEREVENT_QUEST_PACKETTYPE_NONE			= 0,// 이런 타입은 없다

	// 클라이언트용
	SERVEREVENT_QUEST_PACKETTYPE_QUESTSTART,		// 퀘스트가 시작되었다
	SERVEREVENT_QUEST_PACKETTYPE_RUNINFO,			// 퀘스트 진행 정보
	SERVEREVENT_QUEST_PACKETTYPE_GETCHAR,			// 퀘스트를 클리어 해서 문자를 하나 받았다
	SERVEREVENT_QUEST_PACKETTYPE_QUEST_FAIL,		// 퀘스트를 실패했다
	SERVEREVENT_QUEST_PACKETTYPE_CHAR_FAIL,			// 퀘스트는 클리어 했지만 문자를 받지 못했다
	SERVEREVENT_QUEST_PACKETTYPE_ALLCLEAR,			// 모든 퀘스트를 클리어 했다

	// DB용
	SERVEREVENT_QUEST_PACKETTYPE_GET_QUESTINFO,		// 퀘스트 정보 요청
	SERVEREVENT_QUEST_PACKETTYPE_SET_QUESTINFO,		// 퀘스트 정보 저장

	SERVEREVENT_QUEST_PACKETTYPE_END			
};

class CNServerEvent_Quest
{
protected:
	CNServerEvent_QuestData			m_clSEQuestData;
	CNServerEvent_QuestData_Manager	m_clSEQD_Manager;

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, Const_ServerEvent_Max_QuestCharacter>									m_bCharacter;
#else
	// 실패를 포함한 문자
	bool							m_bCharacter[ Const_ServerEvent_Max_QuestCharacter ];
#endif
	SI16							m_siInFailCharacter[ Const_ServerEvent_Max_QuestCharacter * Const_ServerEvent_MulNum ];

	SI16							m_siCharacterLimit;			// 순수한 문자 갯수
	SI16							m_siFailAddCharacterLimit;	// 실패를 포함한 문자 갯수

	SI16							m_siCharacterIndex;			// 현재 m_bInFailCharacter 접근해야하는 인덱스

	SI32							m_siNowAmount;				// 얼마만큼의 퀘스트 조건을 얻었는가?

	DWORD							m_dwServerEventQuestColck;


public:
	CNServerEvent_Quest( void );
	~CNServerEvent_Quest();

public:
	void						Action( void );

	bool						LoadQuestData( SI16 siServerUnique );

	void						Init( void );
	void						InitCharacter( void );

	void						IncServerEventQuestCondition( SI32 siEventType, SI32 siCondition, SI32 siAmount );

	bool						IsClearServerEventQuest( void );
	bool						ServerEventQuestClearProcess( SI32 siEventType=0 );
	bool						AllClearServerEventQuest( void );
	
	void						MakeNewServerEventQuest( SI32 siEventType=0 );

	CNServerEvent_QuestData*	GetServerEnent_QuestData( void )	{ return &m_clSEQuestData; }
	
	SI16						GetCharacterLimit( void )			{ return m_siCharacterLimit; }
	void						SetCharacterLimit( SI16 siCharacterLimit );

	SI32						GetNowAmount( void )				{ return m_siNowAmount; }
	void						SetNowAmount( SI32 siNowAmount );

	SI16						GetCharacterIndex( void )			{ return m_siCharacterIndex; }
	void						SetCharacterIndex( SI16 siCharacterIndex );

	bool*						GetCharacter( void )				{ return &m_bCharacter[0]; }
	void						SetCharacter( bool bCharacter[] );

	SI16*						GetInFailCharacter( void )			{ return &m_siInFailCharacter[0]; }
	void						SetInFailCharacter( SI16 siInFailCharacter[] );

	DWORD						GetServerEventQuestColck( void )	{ return m_dwServerEventQuestColck; }
	void						SetServerEventQuestColck( DWORD dwServerEventQuestColck );

	void						StartServerEventQuest( void );
	void						SendMessageToDB( SI32 siPacketType );
	
	// 운영자 명령어 serverquestclear 을 위한 강제 퀘스트 클리어 용 함수
	void						QuestClearForGM( void );

protected:
	void						MakeCharacter( void );
	void						RewardCharacter( void );

	void						IncAmount( SI32 siAmount );
	void						RewardItem( SI32 siItemUnique, SI32 siItemAmount );

	void						SendMessageToClient( SI32 siPacketType, SI32 siPara1=0, SI32 siPara2=0 );
	

};