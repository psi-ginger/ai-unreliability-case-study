#ifndef _DBMSGVOTE_H_
#define _DBMSGVOTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

#define MAX_CANDIDATURE_NUM			20		// 최대 입후보자수

enum VOTE_STATUS_TYPE
{
	VOTE_STATUS_TYPE_RESULT			// 기존에 있던 최종 투표 현황
	,VOTE_STATUS_TYPE_ONAIR			// 새롭게 만든 현재 개표 현황 
	,VOTE_STATUS_TYPE_WRITE_RESULT	// 선거결과 파일로 기록
};

class sDBRequest_VoteInit
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siMasterPersonID;
	SI32				siMasterCharID;

	sDBRequest_VoteInit( SI32 MasterPersonID, SI32 MasterCharID )
	{
		packetHeader.usSize = sizeof( sDBRequest_VoteInit );
		packetHeader.usCmd	= DBMSG_REQUEST_VOTEINIT;

		siMasterPersonID = MasterPersonID;
		siMasterCharID = MasterCharID;
	}
};

class sDBResponse_VoteInit
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siResult;

	SI32				siMasterPersonID;
	SI32				siMasterCharID;

	sDBResponse_VoteInit()
	{
		ZeroMemory( this, sizeof( sDBResponse_VoteInit ) );
	}
};

class sDBRequest_VoteKingSelection
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID;

	SI16				siVoteListIndex;

	sDBRequest_VoteKingSelection( SI32 PersonID, SI32 CharID, SI16 VoteListIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_VoteKingSelection );
		packetHeader.usCmd	= DBMSG_REQUEST_VOTEKINGSELECTION;

		siPersonID = PersonID;
		siCharID = CharID;
		
		siVoteListIndex = VoteListIndex;		
	}
};

class sDBResponse_VoteKingSelection
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID;

	SI16				siVoteListIndex;

	sDBResponse_VoteKingSelection()
	{
		ZeroMemory( this, sizeof( sDBResponse_VoteKingSelection ) );
	}
};

class sDBRequest_VoteKingAggregate
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;

	SI08				m_siType;									// 어디에 쓰일것인지를 설정

	sDBRequest_VoteKingAggregate(SI32 siCharID, SI08 siType)
	{
		m_packetHeader.usSize	= sizeof( sDBRequest_VoteKingAggregate );
		m_packetHeader.usCmd	= DBMSG_REQUEST_VOTEKINGAGGREGATE;

		m_siCharID				= siCharID;
		m_siType				= siType;
	}
};

class sDBResponse_VoteKingAggregate
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;			// 결과값.

	SI32				m_siCharID;

	SI32				m_siCandidatureNum;						// 후보자 수
	//SI32				siPersonID[MAX_CANDIDATURE_NUM];		// 후보자들의 PersonID
	//SI32				siVoteIndex[MAX_CANDIDATURE_NUM];		// 후보자의 인덱스
	SI32				m_siVoteCount[MAX_CANDIDATURE_NUM];		// 후보자들의 득표수

	SI08				m_siType;								// 어디에 쓰일것인지를 설정

	sDBResponse_VoteKingAggregate()
	{
		ZeroMemory( this, sizeof( sDBResponse_VoteKingAggregate ) );
	}
};



#endif