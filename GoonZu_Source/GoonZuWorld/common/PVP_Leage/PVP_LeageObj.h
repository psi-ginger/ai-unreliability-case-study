//////////////////////////////////////////////////////////////////////////
//
//	PVP_League를 할때 만들 리스트의 내용을 클래스로 만들어 둔다.
//
//	매칭 리스트의 내용
//	
//////////////////////////////////////////////////////////////////////////
#ifndef _PVP_LEAGUEOBG_H_
#define _PVP_LEAGUEOBG_H_

#include "NDataTypes.h"

class CPVP_Leage_ListData
{
private:
	SI32 m_siPersonID	;	// 등록한 사람 PersonID
	SI08 m_siGrade	;		// 등록한 사람 등급
	SI08 m_siStatus	;		// 리스트에 있는 상태 변수 0: 일반 예약 상태, 1: 지난예약 매칭 실패한 상태
public:
	CPVP_Leage_ListData()
	{
		ZeroMemory( this, sizeof(CPVP_Leage_ListData) );
	}

	CPVP_Leage_ListData( CPVP_Leage_ListData* pList )
	{
		if ( pList )
		{
			memcpy( this, pList, sizeof(CPVP_Leage_ListData) )	;
		}
	}

	CPVP_Leage_ListData( SI32 siPersonID, SI08 siGrade, SI08 siStatus )
	{
		m_siPersonID	= siPersonID	;
		m_siGrade		= siGrade		;
		m_siStatus		= siStatus		;
	}

	~CPVP_Leage_ListData()
	{

	}
	//--
	void SetPersonID( SI32 siPersonID )	{	m_siPersonID = siPersonID	;	}
	SI32 GetPersonID( void )			{	return m_siPersonID		;		}

	void SetGrade( SI32 siGrade )		{	m_siGrade = siGrade		;		}
	SI32 GetGrade( void )				{	return m_siGrade		;		}

	void SetSatus( SI32 siSatus )		{	m_siStatus = siSatus	;		}
	SI32 GetSatus( void )				{	return m_siStatus		;		}
	//--

	// 데이터를 통쨰로 받아서 셋팅
	void SetPVP_LeagueData( CPVP_Leage_ListData* pList )
	{
		if ( pList )
		{
			memcpy( this,pList,sizeof(CPVP_Leage_ListData) )	;
		}
	}

	void SetPVP_LeagueData( SI32 siPersonID, SI08 siGrade, SI08 siStatus )
	{
		m_siPersonID	= siPersonID	;
		m_siGrade		= siGrade		;
		m_siStatus		= siStatus		;
	}
	// 데이터 초기화
	void InitPVP_LeagueData( )
	{
		ZeroMemory(this, sizeof( CPVP_Leage_ListData ))	;
	}

	// 비었는지 비지 않았는지 확인(personid만 검사)
	bool IsEmpty( void )
	{
		if ( 0 >= m_siPersonID )
		{
			return true	;
		}
		return false	;
	}

	// 같은 PersonID인지 확인
	bool IsSamePersonID( SI32 siPersonID )
	{
		if ( (siPersonID) && (siPersonID == m_siPersonID) )
		{
			return true;
		}

		return false;
	}

};

class CPVP_Leage_MatchingListData
{
private:
	SI32 m_siBattlePersonIDHome	;	// 매칭 기준이 되었던 사람의 personID
	SI32 m_siBattlePersonIDAway	;	// 매칭 된 personID

	SI32 m_siScore;					// 경기 등급에 의한 특설링 설정을 위해 두 사람의 조건값을 갖고 있는다

public:
	CPVP_Leage_MatchingListData()
	{
		ZeroMemory( this, sizeof(CPVP_Leage_MatchingListData) );
	}

	CPVP_Leage_MatchingListData( CPVP_Leage_MatchingListData* pList )
	{
		if ( pList )
		{
			memcpy( this, pList, sizeof(CPVP_Leage_MatchingListData) )	;
		}
	}

	CPVP_Leage_MatchingListData( SI32 siBattlePersonIDHome, SI32 siBattlePersonIDAway, SI32 siScore )
	{
		m_siBattlePersonIDHome	=	siBattlePersonIDHome	;
		m_siBattlePersonIDAway	=	siBattlePersonIDAway	;
		m_siScore				=	siScore;
	}

	~CPVP_Leage_MatchingListData()
	{

	}
	//--
	void SetBattlePersonIDHome( SI32 siBattlePersonIDHome )	{	m_siBattlePersonIDHome = siBattlePersonIDHome	;	}
	SI32 GetBattlePersonIDHome( void )						{	return m_siBattlePersonIDHome					;	}

	void SetBattlePersonIDAway( SI32 siBattlePersonIDAway )	{	m_siBattlePersonIDAway = siBattlePersonIDAway	;	}
	SI32 GetBattlePersonIDAway( void )						{	return m_siBattlePersonIDAway					;	}

	void SetScore( SI32 siScore )							{	m_siScore = siScore;								}
	SI32 GetScore( void )									{	return m_siScore;									}
	//--

	// 데이터를 통쨰로 받아서 셋팅
	void SetPVP_Leage_MatchingListData( CPVP_Leage_MatchingListData* pList )
	{
		if ( pList )
		{
			memcpy( this, pList, sizeof(CPVP_Leage_MatchingListData) )	;
		}
	}

	void SetPVP_Leage_MatchingListData( SI32 siBattlePersonIDHome, SI32 siBattlePersonIDAway, SI32 siScore )
	{
		m_siBattlePersonIDHome	=	siBattlePersonIDHome	;
		m_siBattlePersonIDAway	=	siBattlePersonIDAway	;
		m_siScore				=	siScore;

	}
	// 데이터 초기화
	void InitPVP_Leage_MatchingListData( )
	{
		ZeroMemory( this, sizeof(CPVP_Leage_MatchingListData) )	;
	}

	// 비었는지 비지 않았는지 확인
	bool IsEmpty( void )
	{
		if ( (0 >= m_siBattlePersonIDHome) && (0 >= m_siBattlePersonIDAway) )
		{
			return true	;
		}
		return false	;
	}

	bool IsMatch( SI32 siSourcePersonID, SI32 siDestPersonID )
	{
		if( siSourcePersonID < 0 || siDestPersonID < 0 )		return false;
		if( siSourcePersonID == siDestPersonID )				return false;

		if( m_siBattlePersonIDHome == siSourcePersonID && m_siBattlePersonIDAway == siDestPersonID )		return true;
		if( m_siBattlePersonIDHome == siDestPersonID && m_siBattlePersonIDAway == siSourcePersonID )		return true;

		return false;
	}	
};

class CPVP_Leage_EndGameListData
{
private:
	SI32 m_siBattlePersonIDHome	;	// 매칭 기준이 되었던 사람의 personID
	SI32 m_siBattlePersonIDAway	;	// 매칭 된 personID

public:
	CPVP_Leage_EndGameListData()
	{
		ZeroMemory( this, sizeof(CPVP_Leage_EndGameListData) );
	}

	CPVP_Leage_EndGameListData( CPVP_Leage_EndGameListData* pList )
	{
		if ( pList )
		{
			memcpy( this, pList, sizeof(CPVP_Leage_EndGameListData) )	;
		}
	}

	CPVP_Leage_EndGameListData( SI32 siBattlePersonIDHome, SI32 siBattlePersonIDAway, SI32 siScore )
	{
		m_siBattlePersonIDHome	=	siBattlePersonIDHome	;
		m_siBattlePersonIDAway	=	siBattlePersonIDAway	;
	}

	~CPVP_Leage_EndGameListData()
	{

	}
	//--
	void SetBattlePersonIDWin( SI32 siBattlePersonIDHome )	{	m_siBattlePersonIDHome = siBattlePersonIDHome	;	}
	SI32 GetBattlePersonIDWin( void )						{	return m_siBattlePersonIDHome					;	}

	void SetBattlePersonIDLose( SI32 siBattlePersonIDAway )	{	m_siBattlePersonIDAway = siBattlePersonIDAway	;	}
	SI32 GetBattlePersonIDLose( void )						{	return m_siBattlePersonIDAway					;	}
	//--

	// 데이터를 통쨰로 받아서 셋팅
	void SetPVP_Leage_EndGameListData( CPVP_Leage_EndGameListData* pList )
	{
		if ( pList )
		{
			memcpy( this, pList, sizeof(CPVP_Leage_EndGameListData) )	;
		}
	}

	void SetPVP_Leage_EndGameListData( SI32 siBattlePersonIDHome, SI32 siBattlePersonIDAway, SI32 siScore )
	{
		m_siBattlePersonIDHome	=	siBattlePersonIDHome	;
		m_siBattlePersonIDAway	=	siBattlePersonIDAway	;
	}
	// 데이터 초기화
	void InitPVP_Leage_EndGameListData( )
	{
		ZeroMemory( this, sizeof(CPVP_Leage_EndGameListData) )	;
	}

	// 비었는지 비지 않았는지 확인
	bool IsEmpty( void )
	{
		if ( (0 >= m_siBattlePersonIDHome) && (0 >= m_siBattlePersonIDAway) )
		{
			return true	;
		}
		return false	;
	}
};

#endif