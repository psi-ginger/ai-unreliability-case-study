#ifndef __MSGTYPEGAWIBAWIBO_H__
#define __MSGTYPEGAWIBAWIBO_H__


//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"

#pragma once 


//	[8/12/2009 종호_GAWIBAWIBO] Client -> Server	금액을 차감
class cltGameRequest_GawiBawiBo_ChargedMoney
{
public:
	cltGameRequest_GawiBawiBo_ChargedMoney()
	{
		ZeroMemory(this, sizeof(cltGameRequest_GawiBawiBo_ChargedMoney));
	}
};


//  [8/12/2009 종호_GAWIBAWIBO] 승무패 연산
class cltGameRequest_GawiBawiBo_CalculateResult
{
public:
	cltGameRequest_GawiBawiBo_CalculateResult()
	{
		ZeroMemory(this, sizeof(cltGameRequest_GawiBawiBo_CalculateResult));
	}
};

class cltGameResponse_GawiBawiBo_CalculateResult
{
public:
	SI08 m_siResult;		//  승패 결과
	SI08 m_siCount;			//	연승 정보
	SI08 m_siValidCount;	//	연승 값이 유효한값인가.

	cltGameResponse_GawiBawiBo_CalculateResult(SI08 siResult, SI08 siCount = 0, SI08 siValidCount = 0)
	{
		ZeroMemory(this, sizeof(cltGameResponse_GawiBawiBo_CalculateResult));

		m_siResult		= siResult;
		m_siCount		= siCount;
		m_siValidCount	= siValidCount;
	}
};

////	가위바위보 연승 카운트 증가
//class cltGameRequest_GawiBawiBo_GawiBawiBoCount_Increase
//{
//public:
//	SI08 m_siCount;	//	 연승 검증용 데이터
//
//	cltGameRequest_GawiBawiBo_GawiBawiBoCount_Increase(SI08 siCount)
//	{
//		ZeroMemory(this, sizeof(cltGameRequest_GawiBawiBo_GawiBawiBoCount_Increase));
//		m_siCount = siCount;
//	}
//};
//
////	가위바위보 연승 카운트 증가
//class cltGameResponse_GawiBawiBo_GawiBawiBoCount_Increase
//{
//public:
//	SI08 m_siCount;	//	 연승 검증용 데이터
//
//	cltGameResponse_GawiBawiBo_GawiBawiBoCount_Increase(SI08 siCount)
//	{
//		ZeroMemory(this, sizeof(cltGameResponse_GawiBawiBo_GawiBawiBoCount_Increase));
//		m_siCount = siCount;
//	}
//};


//	보상 패킷
class cltGameRequest_GawiBawiBo_Reward
{
public:
	GMONEY m_siMoney;	//	보상 머니 검증용 데이터
    
	cltGameRequest_GawiBawiBo_Reward(GMONEY siMoney)
	{
		ZeroMemory(this, sizeof(cltGameRequest_GawiBawiBo_Reward));
		m_siMoney = siMoney;
	}
};

//	보상 패킷
class cltGameResponse_GawiBawiBo_Reward
{
public:
	GMONEY m_siMoney;	//	보상 머니 검증용 데이터

	cltGameResponse_GawiBawiBo_Reward(GMONEY siMoney)
	{
		ZeroMemory(this, sizeof(cltGameResponse_GawiBawiBo_Reward));
		m_siMoney = siMoney;
	}
};


//	8연승 이상 알림 패킷
class cltGameResponse_GawiBawiBo_Notice_Reward
{
public:
	TCHAR	m_szName[256];	//	이름
	SI08	m_siCount;	//	연승
	GMONEY	m_siMoney;	//	보상 머니 검증용 데이터

	cltGameResponse_GawiBawiBo_Notice_Reward(TCHAR* szName, SI08 siCount, GMONEY siMoney)
	{
		ZeroMemory(this, sizeof(cltGameResponse_GawiBawiBo_Notice_Reward));
		StringCchCopy(m_szName, 256, szName);
		m_siCount	= siCount;
		m_siMoney	= siMoney;
	}
};
	
#endif

