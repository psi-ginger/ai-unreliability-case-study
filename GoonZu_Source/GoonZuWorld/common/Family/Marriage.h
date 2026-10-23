#pragma once

#include "../common/CommonHeader.h"

const SI32 Const_Marriage_Min_Level			= 50;					// 결혼할수있는 최소 레벨

const SI32 Const_Max_Propose_Message		= 128;					// 프로포즈할때 쓰이는 최대 메시지 길이

const SI32 Const_Family_Item_Unique			= ITEMUNIQUE(24119);	// 가족이용권
const SI32 Const_Family_Item_Num			= 1;					// 가족이용권 사용시 필요한 아이템 갯수

const SI32 Const_WeddingAnniversary_Unique	= ITEMUNIQUE(8120);		// 결혼 기념일 지급 아이템 유니크
const SI32 Const_WeddingAnniversary_Num		= 1;					// 결혼 기념일 지급 아이템 갯수

const SI32 Const_Wedding_FullDress_Unique	= ITEMUNIQUE(13348);	// 결혼의복 상자
const SI32 Const_Wedding_FullDress_Num		= 1;

enum MARRIAGE_MATE_CONNECT_MODE
{
	MARRIAGE_MATE_CONNECT_MODE_NONE		= 0,
	MARRIAGE_MATE_CONNECT_MODE_CONNECT,
	MARRIAGE_MATE_CONNECT_MODE_SAMEMAP,
};
// 결혼시 애정 지수
enum MARRIAGE_LOVE_LEVEL
{
	MARRIAGE_LOVE_LEVEL_NONE = 0,	// 아무 등급도 아니다

	MARRIAGE_LOVE_LEVEL_01,			// 결혼하게 되면 기본적으로 받는 등급
	MARRIAGE_LOVE_LEVEL_02,
	MARRIAGE_LOVE_LEVEL_03,
	MARRIAGE_LOVE_LEVEL_04,
	MARRIAGE_LOVE_LEVEL_05,
	MARRIAGE_LOVE_LEVEL_06,
	MARRIAGE_LOVE_LEVEL_07,
	MARRIAGE_LOVE_LEVEL_08,
	MARRIAGE_LOVE_LEVEL_09,
	MARRIAGE_LOVE_LEVEL_10,

	MARRIAGE_LOVE_LEVEL_MAX
};

class CMarriage
{
private:
	SI32	m_siMatePersonID;				// 배우자(결혼 기념일은 0이고 배우자 아이디만 있으면 프로포즈하는 상대이다)
	TCHAR	m_szMateName[MAX_PLAYER_NAME];	// 배우자 이름

	SI32	m_siWeddingDateVary;			// 결혼일
	SI32	m_siItemGetDateVary;			// 결혼기념일 아이템 받은 날짜
	BOOL	m_bRequestItemGet;				// 결혼기념일 아이템을 받으라고 요청했다
    
	SI16	m_siLoveLevel;					// 애정도
	SI32	m_siUseHourVary;				// 애정도 아이템 최종 사용시간
	SI32	m_siLimitHourVary;				// 애정도 아이템 제한 시간

	SI16	m_siConnectMode;				// 접속 모드

public:
	CMarriage( void )		{	ZeroMemory(this, sizeof(CMarriage));	}
	~CMarriage( void )		{	}

	void	Init( void )
	{
		ZeroMemory(this, sizeof(CMarriage));
	}

	void	Set( CMarriage* pclMarriage )
	{
		if ( pclMarriage )
		{
			memcpy( this, pclMarriage, sizeof(CMarriage) );
		}
	}

	void	Set( SI32 siMatePersonID,	TCHAR* pszMateName, 
				SI32 siWeddingDateVary,	SI32 siItemGetDateVary,	BOOL bRequestItemGet,
				SI16 siLoveLevel,		SI32 siUseHourVary,		SI32 siLimitHourVary, 
				SI16 siConnectMode )
	{
		m_siMatePersonID	= siMatePersonID;
		if ( pszMateName )
		{
			StringCchCopy( m_szMateName, sizeof(m_szMateName), pszMateName );
		}

		m_siWeddingDateVary	= siWeddingDateVary;
		m_siItemGetDateVary	= siItemGetDateVary;
		m_bRequestItemGet	= bRequestItemGet;

		m_siLoveLevel		= siLoveLevel;
		m_siUseHourVary		= siUseHourVary;
		m_siLimitHourVary	= siLimitHourVary;

		m_siConnectMode		= siConnectMode;
	}

	// Interface Function
public:
	void	SetMatePersonID( SI32 siMatePersonID )				{	m_siMatePersonID = siMatePersonID;			}
	SI32	GetMatePersonID( void )								{	return m_siMatePersonID;					}

	void	SetMateName( TCHAR* pszMateName )					{	if ( pszMateName ) StringCchCopy( m_szMateName, sizeof(m_szMateName), pszMateName );	}
	TCHAR*	GetMateName( void )									{	return m_szMateName;						}

	void	SetWeddingDateVary( SI32 siWeddingDateVary )		{	m_siWeddingDateVary = siWeddingDateVary;	}
	SI32	GetWeddingDateVary( void )							{	return m_siWeddingDateVary;					}

	void	SetItemGetDateVary( SI32 siItemGetDateVary )		{	m_siItemGetDateVary = siItemGetDateVary;	}
	SI32	GetItemGetDateVary( void )							{	return m_siItemGetDateVary;					}

	void	SetRequestItemGet( BOOL bRequestItemGet )			{	m_bRequestItemGet = bRequestItemGet;		}
	BOOL	GetRequestItemGet( void )							{	return m_bRequestItemGet;					}

	void	SetLoveLevel( SI16 siLovelovel )					{	m_siLoveLevel = siLovelovel;				}
	SI16	GetLoveLevel( void )								{	return m_siLoveLevel;						}

	void	SetUseHourVary( SI32 siUseHourVary )				{	m_siUseHourVary = siUseHourVary;			}
	SI32	GetUseHourVary( void )								{	return m_siUseHourVary;						}

	void	SetLimitHourVary( SI32 siLimitHourVary )			{	m_siLimitHourVary = siLimitHourVary;		}
	SI32	GetLimitHourVary( void )							{	return m_siLimitHourVary;					}
	
	void	SetConnectMode( SI16 siConnectMode )				{	m_siConnectMode = siConnectMode;			}
	SI16	GetConnectMode( void )								{	return m_siConnectMode;						}

	// Utility Function
public:
	BOOL	IsMarried( void )
	{
		// 배우자퍼슨아이디와 결혼기념일 둘다 설정되어야 결혼한것이다
		if ( (0 < m_siMatePersonID) && (0 < m_siWeddingDateVary) )
		{
			return TRUE;
		}

		return FALSE;
	}

	SI16	CalcNowLoveLevel( SI32 siNowHourVary );

	// 결혼 기념일인가?
	BOOL	IsWeddingAnniversary( SI32 siNowDateVary );
	// 결혼 기념일 아이템을 받았는가?
	BOOL	IsWeddingItemGet( SI32 siNowDateVary );
	// 남은 시간을 가져온다
	SI32	GetRemainHourVary( SI32 siNowHourVary );


};