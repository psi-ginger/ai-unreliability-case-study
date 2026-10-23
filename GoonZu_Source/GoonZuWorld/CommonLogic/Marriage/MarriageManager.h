#pragma once 

#include <Directives.h>
#include "../../Common/CommonHeader.h"
#include "../../common/Family/Marriage.h"

#include "../NLib/NUpdateTimer.h"

#include "WeddingManager.h"

#define MARRIAGE_WEDDING_CREATE_NPC_X				14
#define MARRIAGE_WEDDING_CREATE_NPC_Y				10

#define MARRIAGE_WEDDING_COMEIN_CONGRATULATOR_X		10
#define MARRIAGE_WEDDING_COMEIN_CONGRATULATOR_Y		16

#define MARRIAGE_WEDDING_COMEIN_MARRIAGE_PERSON_X	15
#define MARRIAGE_WEDDING_COMEIN_MARRIAGE_PERSON_Y	24

enum MARRIAGE_ABILITY_TYPE
{
	MARRIAGE_ABILITY_TYPE_NONE	= 0,

	MARRIAGE_ABILITY_TYPE_ATTACK,	// 공격력 +
	MARRIAGE_ABILITY_TYPE_DEFENSE,	// 방어력 +
	MARRIAGE_ABILITY_TYPE_LIFE,		// 생명력 +
	MARRIAGE_ABILITY_TYPE_EXP,		// 경험치 %

	MARRIAGE_ABILITY_TYPE_MAX
};

enum MARRIAGE_EFFECT_TYPE
{
	MARRIAGE_EFFECT_TYPE_NONE = 0,	// 없음

	MARRIAGE_EFFECT_TYPE_SMALL,		// 작음
	MARRIAGE_EFFECT_TYPE_MEDIUM,	// 중간
	MARRIAGE_EFFECT_TYPE_LARGE,		// 큰것

	MARRIAGE_EFFECT_TYPE_MAX
};

enum MARRIAGE_WEDDING_MESSAGE
{
	MARRIAGE_WEDDING_MESSAGE_05 = 0x0001,
	MARRIAGE_WEDDING_MESSAGE_15 = 0x0002,
	MARRIAGE_WEDDING_MESSAGE_25 = 0x0004,

	MARRIAGE_WEDDING_MESSAGE_35 = 0x0008,
	MARRIAGE_WEDDING_MESSAGE_45 = 0x0010,
	MARRIAGE_WEDDING_MESSAGE_55 = 0x0020,
};

enum MARRIAGE_WEDDING_STATUS
{
	MARRIAGE_WEDDING_STATUS_NONE = 0,


	MARRIAGE_WEDDING_STATUS_READY,					// 예식 준비
	MARRIAGE_WEDDING_STATUS_START,					// 예식 시작

	MARRIAGE_WEDDING_STATUS_COMEIN_WAIT,			// 부르기를 기다림

	MARRIAGE_WEDDING_STATUS_MATE1_COMEIN,			// 배우자1 입장
	MARRIAGE_WEDDING_STATUS_MATE1_COMEIN_WAIT,		// 배우자1 입장하기를 기다림

	MARRIAGE_WEDDING_STATUS_MATE2_COMEIN,			// 배우자2 입장
	MARRIAGE_WEDDING_STATUS_MATE2_COMEIN_WAIT,		// 배우자2 입장하기를 기다림

	MARRIAGE_WEDDING_STATUS_MATE1_QUESTION,			// 배우자1에게 질문
	MARRIAGE_WEDDING_STATUS_MATE1_QUESTION_WAIT,	// 배우자1의 답변을 기다림

	MARRIAGE_WEDDING_STATUS_MATE2_QUESTION,			// 배우자2에게 질문
	MARRIAGE_WEDDING_STATUS_MATE2_QUESTION_WAIT,	// 배우자2의 답변을 기다림

	MARRIAGE_WEDDING_STATUS_WEDDING_END,			// 결혼식 종료

	MARRIAGE_WEDDING_STATUS_GIVEITEM_QUESTION,		// 화관 아이템 증정 질문
	MARRIAGE_WEDDING_STATUS_GIVEITEM_WAIT,			// 화관 아이템 증정 답변 기다림
	MARRIAGE_WEDDING_STATUS_GIVEITEM,				// 화관 아이템 증정

	MARRIAGE_WEDDING_STATUS_END_READY,				// 끝 준비
	MARRIAGE_WEDDING_STATUS_END,					// 끝

	MARRIAGE_WEDDING_STATUS_MAX
};

enum MARRIAGE_WEDDING_CHAT_MESSAGE
{
	MARRIAGE_WEDDING_CHAT_MESSAGE_NONE = 0,

	MARRIAGE_WEDDING_CHAT_MESSAGE_READY1,			// 준비
	MARRIAGE_WEDDING_CHAT_MESSAGE_READY2,			// 준비
	MARRIAGE_WEDDING_CHAT_MESSAGE_READY3,			// 준비
	MARRIAGE_WEDDING_CHAT_MESSAGE_SAY1,				// 주례사
	MARRIAGE_WEDDING_CHAT_MESSAGE_SAY2,				// 주례사
	MARRIAGE_WEDDING_CHAT_MESSAGE_SAY3,				// 주례사
	MARRIAGE_WEDDING_CHAT_MESSAGE_SAY4,				// 주례사
	MARRIAGE_WEDDING_CHAT_MESSAGE_COMEIN,			// 입장해라
	MARRIAGE_WEDDING_CHAT_MESSAGE_QUESTION1,		// 신랑,신부에게 질문
	MARRIAGE_WEDDING_CHAT_MESSAGE_QUESTION2,		// 신랑,신부에게 질문
	MARRIAGE_WEDDING_CHAT_MESSAGE_QUESTION3,		// 신랑,신부에게 질문
	MARRIAGE_WEDDING_CHAT_MESSAGE_QUESTION4,		// 신랑,신부에게 질문
	MARRIAGE_WEDDING_CHAT_MESSAGE_END,				// 결혼 종료

};

// MarriageAbilityItem
class cltMarriageAbilityItem
{
private:
	SI32 m_siAbilityType;	// 능력치 타입
	SI32 m_siAbilityValue;	// 능력치 수치

	SI32 m_siEffectType;	// 보여지게 이팩트 타입

public:
	cltMarriageAbilityItem() : m_siAbilityType(0), m_siAbilityValue(0), m_siEffectType(0) 
	{
	}

	~cltMarriageAbilityItem()
	{
	}

	void Init( void )
	{
		ZeroMemory( this, sizeof(cltMarriageAbilityItem) );
	}

	void Set( cltMarriageAbilityItem* pclMarriageAbilityItem )
	{
		
	}

	void Set( SI32 siAbilityType, SI32 siAbilityValue, SI32 siEffectType )
	{
		m_siAbilityType		= siAbilityType;	
		m_siAbilityValue	= siAbilityValue;	

		m_siEffectType		= siEffectType;	
	}

// Utility Function
public:
	SI32 GetAbilityType( void )					{	return m_siAbilityType;				}
	void SetAbilityType( SI32 siAbilityType )	{	m_siAbilityType = siAbilityType;	}

	SI32 GetAbilityValue( void )				{	return m_siAbilityValue;			}
	void SetAbilityValue( SI32 siAbilityValue )	{	m_siAbilityValue = siAbilityValue;	}

	SI32 GetEffectType( void )					{	return m_siEffectType;				}
	void SetEffectType( SI32 siEffectType )		{	m_siEffectType = siEffectType;		}

	BOOL IsEmpty( void )
	{
		if ( 0 < m_siAbilityType )	return FALSE;
		if ( 0 < m_siAbilityValue )	return FALSE;
		if ( 0 < m_siEffectType	)	return FALSE;
		
		return TRUE;
	}

};

class cltMarriageAbilityManager
{
private:
	cltMarriageAbilityItem*		m_pclMarriageAbilityItem;

public:
	cltMarriageAbilityManager( void );
	~cltMarriageAbilityManager( );

// For Resource Function
public:
	BOOL LoadMarriageAbilityInfo( void );
private:
	SI32 ConvertAbilityTypeFromText( TCHAR* pszAbilityTypeText );
	SI32 ConvertEffectTypeFromText( TCHAR* pszEffectTypeText );

	void AddMarriageAbilityItem( SI32 siLoveLevel, SI32 siAbilityType, SI32 siAbilityValue, SI32 siEffectType );

// Utility Function
public:
	SI32 GetTotalAbilityValue( SI32 siAbilityType, SI32 siLoveLevel );
	SI32 GetEffectType( SI32 siLoveLevel );
}; 


class cltMarriageManager
{
private:
	cltMarriageAbilityManager	m_clAbilityMgr;

	NUpdateTimer				m_UpdateTimerWeddingAction;

private:
	SI32						m_siWeddingMessage;			// 웨딩 메시지 관련
	SI32						m_siWeddingStatus;			// 웨딩 진행상황 관련

	SI32						m_siOfficiatorID;			// 주례 NPC CharID

	SI32						m_siWeddintProgressCount;	// 웨딩 진행시 필요한 카운트

public:
	cltWeddingReserveList		m_clWeddingList;

public:
	cltMarriageManager( void );
	~cltMarriageManager();

	void Init( void );
	void InitForWeddingVariable( void );

	void ActionForWedding( void );

public:
	SI32 GetTotalAttackAbilityValue( SI32 siLoveLevel ); 
	SI32 GetTotalDefenseAbilityValue( SI32 siLoveLevel );
	SI32 GetTotalLifeAbilityValue( SI32 siLoveLevel );
	SI32 GetTotalExpAbilityValue( SI32 siLoveLevel );

	// 레벨에 따른 이팩트 타입을 가져온다
	SI32 GetEffectTypeFromLoveLevel( SI32 siLoveLevel );

public:
	// 현재 웨딩 상태를 받아온다
	SI32 GetWeddingStatus( void )	{	return m_siWeddingStatus;	};

	// 웨딩관련 자막을 넣는다
	void PushWeddingMessage( TCHAR* pszName1, TCHAR* pszName2, bool bFuture=false, SI16 siHour=0 );
	// 웨딩 메시지 상태가 Toogle되어있는 지 검사
	BOOL IsWeddingMessage( SI32 siWeddingMessage );
	BOOL CheckAndSetForProgressMessage( SI16 siMinute );
	BOOL CheckAndSetForFutureMessage( SI16 siMinute );

public:
	// 실질적으로 예식관련 프로세스를 수행할 함수
	void WeddingProcess( cltWeddingReserveUnit* pclUnit );
	// 예식장에 결혼식을 올릴 두 캐릭터가 있는지 검사
	BOOL IsInHearMapWeddingPersons( IN SI32 siMate1_PersonID, IN SI32 siMate2_PersonID, OUT SI32* psiMate1CharUnique, OUT SI32* psiMate2CharUnique );
	// 결혼식을 올릴 두 캐릭터를 예식장으로 불러온다
	BOOL ComeInMarriagePersons( SI32 siMate1_PersonID, SI32 siMate2_PersonID );
	// 퍼슨아이디가 예식맵 특정위치에 있는지 검사
	BOOL IsComeInMarriagePerson( SI32 siPersonID );
	// 주례 NPC관련
	BOOL CreateOfficiatorNPC( void );	// 주례 NPC생성
	void DeleteOfficiatorNPC( void );	// 주례 NPC삭제
	// 예식장에 있는 모든 캐릭터 들에게 채팅 메시지를 날린다
	void SendWeddingChatMessage( SI32 siMessageMode, TCHAR* pszName1=NULL, TCHAR* pszName2=NULL );
	// 현재 예식 상태를 변경한다
	void ChangeWeddingStatus( SI32 siChangeWeddingStatus );
	// 예식장에 있는 모든 사람들을 내보낸다
	void OutInHearMapAllPerson( void );
	// 신부에게 화관을 던질것인지 물어본다
	BOOL QuestionDropWeddingFlower( SI32 siMate1_PersonID, SI32 siMate2_PersonID );
	// 하객 혹은 결혼식을 올린 사람에게 화관 아이템을 준다
	void GiveWeddingFlowerItem( SI32 siMate1_PersonID, SI32 siMate2_PersonID );
	
};
