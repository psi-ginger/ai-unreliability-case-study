#include "GeneralMeeting.h"
#include "..\..\Server\Server.h"

cltGeneralMeetingVoter::cltGeneralMeetingVoter()
{
	ZeroMemory( this, sizeof( cltGeneralMeetingVoter ) );
}

cltGeneralMeetingVoter::~cltGeneralMeetingVoter()
{

}

void cltGeneralMeetingVoter::Init()
{
	siPersonID = 0;
	szPersonName[ 0 ] = NULL;
	siStockAmount = 0;
	siVoteType = 0;
	siParticipate = 0;
}

void cltGeneralMeetingVoter::Set( cltGeneralMeetingVoter* pclVoter )
{
	siPersonID = pclVoter->siPersonID;
	memcpy( szPersonName, pclVoter->szPersonName, MAX_PLAYER_NAME );
	szPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
	siStockAmount = pclVoter->siStockAmount;
	siVoteType = pclVoter->siVoteType;
	siParticipate = pclVoter->siParticipate;
}


cltGeneralMeetingInfo::cltGeneralMeetingInfo()
{
	ZeroMemory( this, sizeof( cltGeneralMeetingInfo ) );
}

cltGeneralMeetingInfo::~cltGeneralMeetingInfo()
{

}

void cltGeneralMeetingInfo::Init()
{
	siVillageUnique = 0;
	siMatterType = 0;
	siPersonID = 0;
	siTotalStock = 0;
	siCandidatePersonID = 0;
	szCandidateName[ 0 ] = NULL;
	siAcceptCandidate = 0;

	for( int i = 0; i < 100; ++i ) clVoter[ i ].Init();
}

void cltGeneralMeetingInfo::SetVoter( cltGeneralMeetingVoter voter[] )
{
#ifdef _SAFE_MEMORY
	clVoter.MemCpy(voter);
#else
	memcpy( clVoter, voter, sizeof( cltGeneralMeetingVoter ) * 100 );
#endif
}

BOOL cltGeneralMeetingInfo::IsVoter( SI32 personid )
{
	for( int i = 0; i < 100; ++i ) {

		if( clVoter[ i ].siPersonID == personid &&
			clVoter[ i ].siParticipate != 2 ) return TRUE;
	}

	return FALSE;
}

BOOL cltGeneralMeetingInfo::IsAleadyVote( SI32 personid )
{
	for( int i = 0; i < 100; ++i ) {

		if( clVoter[ i ].siPersonID == personid &&
			clVoter[ i ].siVoteType != 0 ) return TRUE;
	}

	return FALSE;
}


BOOL cltGeneralMeetingInfo::Vote( SI32 personid, SI32 votetype )
{
	for( int i = 0; i < 100; ++i ) {
	
		if( clVoter[ i ].siPersonID == personid ) {
			clVoter[ i ].siVoteType = votetype;
			return TRUE;
		}

	}

	return FALSE;
}

SI32 cltGeneralMeetingInfo::GetResult()
{
	SI32 agree = 0, opposit = 0;

	for( int i = 0; i < 100; ++i ) {
	
		if( clVoter[ i ].siPersonID )
		{
			if( clVoter[ i ].siVoteType == 1 ) 
			{
				agree += clVoter[ i ].siStockAmount;
			} 
			else if( clVoter[ i ].siVoteType == 2 ) 
			{
				opposit += clVoter[ i ].siStockAmount;
			}
		}
	}

	return ( agree > opposit ) ? 1 : 2;

}

SI32 cltGeneralMeetingInfo::GetTotalAgree()
{
	int ret = 0;

	for( int i = 0; i < 100; ++i ) {
	
		if( clVoter[ i ].siPersonID ) {
			if( clVoter[ i ].siVoteType == 1 ) {
				ret += clVoter[ i ].siStockAmount;
			}
		}
	}

	return ret;
}

SI32 cltGeneralMeetingInfo::GetTotalOpposition()
{
	int ret = 0;

	for( int i = 0; i < 100; ++i ) {
	
		if( clVoter[ i ].siPersonID ) {
			if( clVoter[ i ].siVoteType == 2 ) {
				ret += clVoter[ i ].siStockAmount;
			}
		}
	}

	return ret;
}

SI32 cltGeneralMeetingInfo::GetTotalNotParticipate()//참여하지 않음을 누른 사람 수를 계산 [2007.07.11 성웅]
{
	int ret = 0;
	int i =0;

	for (i = 0; i<100; ++i)
	{
		if( clVoter[ i ].siPersonID )
		{
			if( clVoter[ i ].siParticipate == 2 )
			{
				ret += clVoter[ i ].siStockAmount;
			}
		}
	}
	return ret;
}

//void cltGeneralMeetingInfo::Action( SI32 _siVillageUnique )
//{
//	if( siMatterType > 0 ) 
//	{
//		cltDate cldate;
//		cldate.Set( &((cltServer*)pclClient)->pclTime->clDate );
//
//		// [진성] 주총 발의 기간이면.
//		if( ( clStartDate.GetDateVary() <= cldate.GetDateVary() ) &&
//			( clCloseDate.GetDateVary() >  cldate.GetDateVary() )		)
//		{
//			// 후보자가 동의하지 않음.
//			if( siAcceptCandidate <= 0 )
//			{
//				cltDate clAcceptClosedate;	// 동의 종료 시간.				
//				clAcceptClosedate.Set(&clStartDate);
//				clAcceptClosedate.AddMonth(1);
//
//				// 주총 발의 기간으로 부터 1달이 지났다면.
//				if(clAcceptClosedate.GetDateVary() < cldate.GetDateVary() )
//				{
//					sDBRequest_GeneralMeeting_CloseChief sendMsg( _siVillageUnique, 0, &((cltServer *)pclClient)->pclTime->clDate );
//					((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
//				}
//			}
//		}
//		// 주총 발의 시간(주총발의 후 3개월)에서 종료 시간 사이의 시간이 아니면.
//		else 
//		{
//			sDBRequest_GeneralMeeting_CloseChief sendMsg( _siVillageUnique, 0, &((cltServer *)pclClient)->pclTime->clDate );
//			((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
//		}
//	}
//}

