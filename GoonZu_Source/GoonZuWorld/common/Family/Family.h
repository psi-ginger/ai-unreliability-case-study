//------------------------------------------------------------------------------
//	제목 : 가족 시스템
//	내용 : 가족관계에 대한 정보를 담은 class
//	최초 작성일 : 2009-05-20
//	작성자 : 손 성웅
//
//------------------------------------------------------------------------------
#pragma once

#include "../common/CommonHeader.h"
#define MAX_CHILDREN_NUM	10	// 가족안에서 가질수 있는 자식들의 수
#define MAX_PARENT_NUM		2	// 가족안에서 가질수 있는 부모의 수
#define MAX_MONTH_FAMILY			12	// 최대 개월수
#define MAX_DAY_FOR_FAMILY			31	// 최대 날짜

enum	// 가족내에서 자신의 신분을 정한다.
{
	MYCLASS_IN_FAMILY_NONE	=	0,			// 가족따윈 없다.
	MYCLASS_IN_FAMILY_PARANTS,				// 난 두명의 부모중에 한명이다.
	MYCLASS_IN_FAMILY_CHILDREN,				// 난 가족의 구성원중 아이들이다 
};
enum	// 간식용 쿠키 효과 레벨
{
	USE_COOKIE_LEVEL_NONE	=	0,			// 간식용 쿠키 사용 안함
	USE_COOKIE_LEVEL_1,			// 간식용 쿠키 레벨 1 
	USE_COOKIE_LEVEL_2,			// 간식용 쿠키 레벨 2
	USE_COOKIE_LEVEL_3,			// 간식용 쿠키 레벨 3
	USE_COOKIE_LEVEL_4,			// 간식용 쿠키 레벨 4
	USE_COOKIE_LEVEL_5,			// 간식용 쿠키 레벨 5
};
#define  COOKIE_TIME	24	// datavary에 24를 더하면 현실시간 48
#define  COOKIE_COOLTIME	24	// datavary에 10를 더하면 현실시간 20
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CChildrenData
{
public:
	CChildrenData()		{	ZeroMemory(this, sizeof(CChildrenData));	}
	~CChildrenData()	{ };
	void	Init( void ){	ZeroMemory(this, sizeof(CChildrenData));	}
	void	Set( CChildrenData* pclParentData )
	{
		if ( pclParentData )	{	memcpy( this, pclParentData, sizeof(CChildrenData) );	}
	}

	void	Set_Children_PersonID( SI32 siPersonID_Children )		{	m_siPersonID_Children = siPersonID_Children;			}
	SI32	Get_Children_PersonID( void )							{	return m_siPersonID_Children;					}
	void	Set_Children_Name( TCHAR* szName_Children )				{	if ( szName_Children ) StringCchCopy( m_szName_Children, sizeof(m_szName_Children), szName_Children );	}
	TCHAR*	Get_Children_Name( void )								{	return m_szName_Children;						}
	//	기간체크를 용의하게 하기 위해서 변경 
	void	SetFamilyEffectTime( SYSTEMTIME stFamilyEffectTime)		{m_stFamilyEffectTiem = stFamilyEffectTime;};/*{ memcpy( &m_stFamilyEffectTiem, &stFamilyEffectTime, sizeof( SYSTEMTIME) ) ; }*/
	SYSTEMTIME GetFamilyEffectTime()								{	return m_stFamilyEffectTiem		;			}

	void	InGame(){ m_bOnGame = true; };
	void	OutGame(){ m_bOnGame = false; };
	bool	GetGame(){ return m_bOnGame; };
	bool	GetOnePlayer(){ return m_bOnGame; };

	void	SetCookie( SI32 siPerson, SI32 siCookieLevel, SI32 EndDatevary, SI32 siUseLimitDateVary)	;	// 쿠키를 사용 셋팅
	void	DelCookie();
	SI32	GetCookieLevel(){ return m_siFamilyEffectLevel; }	;
	void	LoseCookieLevel(){ m_siFamilyEffectLevel = m_siFamilyEffectLevel -1 ;}	;
	SI32	GetCookie_Effect_EndTime(){ return m_siFamilyEffectDate_EndDateVary; }	;
	SI32	GetCookieEffect_LimitTime(){ return m_siFamilyEffectDate_LimitDateVary; }	;
protected:
private:
	bool	m_bOnGame		;		//접속유무

	SI32	m_siPersonID_Children ;					// 자식 personID
	TCHAR	m_szName_Children[MAX_PLAYER_NAME] ;	// 자식 이름
	SYSTEMTIME m_stFamilyEffectTiem	;				// 가족효과


	SI16	m_siFamilyEffectLevel;								// 간식용 쿠키 사용 레벨
	SI32	m_siFamilyEffectDate_EndDateVary;					// 간식용 쿠키 사용 종료시간 단계별로 48분 
	SI32	m_siFamilyEffectDate_LimitDateVary;					// 간식용 쿠키 연속 사용 제한시간 20분
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CParentData
{
public:
	CParentData()	{ ZeroMemory( this, sizeof( CParentData) ); }
	~CParentData()	{ };
	void	Init( void ){	ZeroMemory(this, sizeof(CParentData));	}
	void	Set( CParentData* pclChildrenData )
	{
		if ( pclChildrenData )	{	memcpy( this, pclChildrenData, sizeof(CChildrenData) );	}
	}

	void	SetParent_PersonID( SI32 siPersonID_Children )				{	m_siParant_PersonID = siPersonID_Children;			}
	SI32	GetParent_PersonID( void )								{	return m_siParant_PersonID;					}
	void	SetParent_Name( TCHAR* szParant_Name )					{	if ( szParant_Name ) StringCchCopy( m_szParant_Name, sizeof(m_szParant_Name), szParant_Name );	}
	TCHAR*	GetParent_Name( void )									{	return m_szParant_Name;						}

	void	InGame(){ m_onGame = true; };
	void	OutGame(){ m_onGame = false; };
	bool	GetGame(){ return m_onGame; };
protected:
private:
	SI32	m_siParant_PersonID ;	
	TCHAR	m_szParant_Name[MAX_PLAYER_NAME];
	bool	m_onGame	;

};

class CFamily
{
private:
	SI32			m_MyClass_In_Family ;					// 나의 신분을 가지고 있고 가족의 전체 정보를 가지고 있는다.	
	CParentData		pclParentData[ MAX_PARENT_NUM ]		;	//부모 케릭터 정보
	CChildrenData	pclChildrenData[MAX_CHILDREN_NUM]	;	//자식 케릭터 정보
	SI32			m_NowParentNum		;					// 현제 접속한 부모 수
	SI32			m_NowChildrenNum	;					// 현제 접속한 자식 수

public:
	CFamily( void )		{	ZeroMemory(this, sizeof(CFamily));	}
	~CFamily( void )		{	}
	void	Init( void ){	ZeroMemory(this, sizeof(CFamily));	}
	void	Set( CFamily* pclFamily ){		if ( pclFamily )	{	memcpy( this, pclFamily, sizeof(CFamily) );	}	}
	void	Set( SI32 siMyFamilyClass , CParentData* pclParentData, CChildrenData*	pclChildrenData )
	{
		m_MyClass_In_Family	=	siMyFamilyClass	;
		memcpy( pclParentData, pclParentData, sizeof(CParentData) );	
		memcpy( pclChildrenData, pclChildrenData, sizeof(CChildrenData) );
	}
	void SetMyClass_InFamily( SI32 siClass ){  m_MyClass_In_Family = siClass; }	// 가족안에서 나의 위치를 나타낸다.
	SI32 GetMyClass_InFamily( void ){ return m_MyClass_In_Family; }				// 가족안에서 나의 위치를 셋팅 한다.
	
	void	GetParentName( TCHAR* pParentName_SLot1 , TCHAR* pParentName_SLot2 )	;
	TCHAR*	GetChildrenName( SI32 siSLot ) ;
	
	SYSTEMTIME GetChild_Add_Date(SI32 siSLot )	;
	SYSTEMTIME FInd_Add_Date( SI32 PersonID )	;

	void	SetChildren( SI32 siPersonID, TCHAR* szCharName,SYSTEMTIME stFamilyEffectDate);
	void	SetParent( SI32 siPersonID, TCHAR* szCharName)	;

	SI32	GetParent_1()	;
	SI32	GetParent_2()	;

	SI32	GetMatePersonID ( SI32 siParentID)	;	// 해당 부모의 배우자 아이디를 가지고 온다
	SI32	GetChildPersonID( SI32 siSLot)	;

	SI32	GetChildPersonIDfromName( TCHAR* charname)	;	// 자식이름으로 personId를 찾는다.
	SI32	GetParentPersonIDfromName( TCHAR* charname)	;	// 부모 이름으로 personID를 찾는다.

	void	DelFamilyMember( SI32 siParentID )	;		// 해당 personid의 가족맴버를 지운다.

	SI32	GetParent_PersonId(SI32 siSlot)	;
	TCHAR*	GetParent_Name(SI32 siSlot)	;
	SI32	IsParent( SI32 siPersonID)	;

	void	AddOnParentNum(){ m_NowParentNum = m_NowParentNum + 1	;}	;		/// 접속하면 이함수로 접속인원을 늘린다	- 부모
	void	LoseOnParentNum(){ m_NowParentNum = m_NowParentNum - 1	; }	;		/// 접속해제하면 이함수로 접속인원을 줄인다.- 부모

	void	AddOnChildrenNum(){ m_NowChildrenNum = m_NowChildrenNum + 1	;}	;		/// 접속하면 이함수로 접속인원을 늘린다 - 자식
	void	LoseOnChildrenNum(){ m_NowChildrenNum = m_NowChildrenNum - 1	; }	;	/// 접속해제하면 이함수로 접속인원을 줄인다. -자식

	SI32	GetOnParentNum(){ return m_NowParentNum; }	;					/// 현제 접속한 부모의 수
	SI32	GetOnChildrenNum(){ return m_NowChildrenNum; }	;				/// 현제 접속한 자식들의 수

	void	ConnectIn( SI32 siPersonID , SI32 siFamilyClass )	;	// 해당 케릭터가 접속 했다
	void	ConnectOut( SI32 siPersonID ,SI32 siFamilyClass )	;	// 해당 케릭터가 게임을 나갔다.

	bool	IsConnectParent( SI32 siSLot)	{ return pclParentData[siSLot].GetGame(); }; // 해당 부모가 접속 해있는지
	bool	IsConnectChildren( SI32 siSLot)	{ return pclChildrenData[siSLot].GetGame(); }; // 해당 자식이 접속 해있는지

	// 쿠키 사용을 위해 만들자.
	void	SetCookie(  SI32 siPersonID , SI32 siCookelevel, SI32 EndDatevary, SI32 siUseLimitDateVary, SI32 siMyClass )		;	// 쿠키를 사용 했다
	void	LoseCookieLevel( SI32 siPersonID )	;	// 쿠키 레벨을 1단계 줄인다.
	SI32	GetMyCookieLevel(SI32 siPersonID)	;	// 나의 쿠키 레벨을 가지고 온다.
	SI32	GetMyCookie_EndTime(SI32 siPersonID)	;	// 나의 쿠키 종료 타임
	SI32	GetMyCookie_Limitime(SI32 siPersonID)	;	// 나의 쿠키 사용 제한 타임

	bool	GetOnGame_Children( SI32 siSlot )	;			// 게임에 접속 했는지 확인
	bool	GetOnGame_Parent( SI32 siSlot )	;				// 게임에 접속 했는지 확인 

	SI32	GetCookieEffect( SI32 siCookieLevel )	;		// 쿠키 효과 를 가지고 온다.

};