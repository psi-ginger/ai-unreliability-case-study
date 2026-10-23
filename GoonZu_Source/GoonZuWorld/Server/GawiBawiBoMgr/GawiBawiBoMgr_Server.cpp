#include "GawiBawiBoMgr_Server.h"
#include "Char/CharManager/CharManager.h"
#include "../CommonLogic/CommonLogic.h"
#include "../Lib/dxutil.h"
#include "../Client/Client.h"

extern cltCommonLogic* pclClient;

CGawiBawiBoMgr_Server::CGawiBawiBoMgr_Server()
{
	m_siStatus = 0;	
	ZeroMemory(m_siWInPercentage, sizeof(m_siWInPercentage));

	m_bAlwaysWin = false;
	m_siLimitWin = 0;
}


CGawiBawiBoMgr_Server::~CGawiBawiBoMgr_Server()
{
	
}


bool CGawiBawiBoMgr_Server::Initialize()			//	매니저 초기화
{
	bool bResult = false;

	//	매니저 초기화 작업.. 파일로드 등등
	bResult = LoadGawiBawiBoFiles();

	return bResult;
}

bool CGawiBawiBoMgr_Server::LoadGawiBawiBoFiles()
{
	NDataLoader	dataloader;

	bool bResult = false;

	if( pclClient->IsWhereServiceArea(ConstSwitch_TextEncryption))
		bResult = dataloader.LoadDataFromCryptFile(TEXT("Data/SRPInfo.txt"));
	else
		bResult = dataloader.LoadDataFromFile(TEXT("Data/SRPInfo.txt"));

	if (bResult == false)
		return false;

	TCHAR	szName[MAX_PATH] = TEXT("");
	SI32	siValue = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szName,		MAX_PATH,
		NDATA_INT32,	&siValue,	4,
		0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo);

	SI16 siCount = 1;

	while(dataloader.IsEndOfData() != true) 
	{
		if (dataloader.ReadData() == true)	
		{
			if(siCount <= 4)		//	데이터파일의 2번 인덱스까지는 승률 데이터
				m_siWInPercentage[siCount] = siValue;
			else				//	데이터파일의 3번인덱스 부터는 누적포인트 데이터 (누적포인트는 누적카운트가 기획상 더 늘어날수 있으므로 벡터로)
			{
				m_vecStraightWins.push_back(siValue);	
				m_siLimitWin++;
			}
			siCount++;
		}
		else
			return true;
	}
	return false;	
}

void CGawiBawiBoMgr_Server::Action()
{

}


GMONEY	CGawiBawiBoMgr_Server::GetChargedMoney()	//	차감된 금액을 리턴
{
	GMONEY siMoney = 0;

	siMoney	= (pclClient->pclItemManager->clItemStatistic.siNPCPriceRate) * 200;	//	 비용은 시세 * 200

	return siMoney;
}

//	승패 결정
SI08 CGawiBawiBoMgr_Server::GetCalculateResult()
{
	SI32 siRand = rand() % 100;
	
	if(siRand < m_siWInPercentage[GAWIBAWIBO_RESULT_ABUSE_WIN]) // NPC 부정행위로 무조건 승리
		return GAWIBAWIBO_RESULT_ABUSE_WIN;
	
	else if(siRand < m_siWInPercentage[GAWIBAWIBO_RESULT_WIN])
		return GAWIBAWIBO_RESULT_WIN;
	
	else if(siRand < m_siWInPercentage[GAWIBAWIBO_RESULT_WIN] + m_siWInPercentage[GAWIBAWIBO_RESULT_DRAW])
		return GAWIBAWIBO_RESULT_DRAW;
	
	else
		return GAWIBAWIBO_RESULT_LOSE;
}


// 넘어온 인자값이 연승수. 해당 연승시 받을 수있는 누적금액을  구한다.
GMONEY CGawiBawiBoMgr_Server::GetCalculateCumulativeMoney(SI08 siStraightWins)						//	누적금액 계산
{
	//	서버에서는 넘어온 카운트 인자만을 기준으로 연승누적값을 구해야 한다.
	GMONEY siMoneyCumulativeMoney = 0;

	//	siCount 연승을 했을시 받을 금액을 리턴
	
	if(IsValidCount(siStraightWins) == false)
	{
		assert(0);
		return 0;
	}

	SI32	siNpcrate	= pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
	GMONEY	siUnitMoney	= siNpcrate * 200;	//	 비용은 시세 * 10			

	//	 siIndex의 초기값을 1로 설정한 이유는 리소스상에서 1승의 index 가 1이다.
	//for(SI08 siIndex = 1; siIndex <= siStraightWins; siIndex++)
	//{
	//if(siIndex == 1)	// 아직 누적금액이 없는 경우.
	//	siMoneyCumulativeMoney = (siUnitMoney * m_vecStraightWins.at(siIndex)) / 100;	
	//else
	//	siMoneyCumulativeMoney = (siMoneyCumulativeMoney * m_vecStraightWins.at(siIndex)) / 100;

	// 누적금액 공식 변경
	//}

	siMoneyCumulativeMoney = (siUnitMoney * m_vecStraightWins.at(siStraightWins)) / 100;	

	return siMoneyCumulativeMoney;
}

SI08 CGawiBawiBoMgr_Server::IsValidCount(SI08 siCount)
{
	if(siCount <= 0 || siCount > (SI08)m_vecStraightWins.size())
		return GAWIBAWIBO_RESULT_VALIDCOUNT_INVALID;
	else if(siCount == (SI08)(m_vecStraightWins.size()-1))
		return GAWIBAWIBO_RESULT_VALIDCOUNT_MAXWIN;
	else
        return GAWIBAWIBO_RESULT_VALIDCOUNT_VALID;
}

void CGawiBawiBoMgr_Server::SetLimitWin(SI32 siLimitWin)
{
	if((size_t)siLimitWin >= m_vecStraightWins.size()-1)
		m_siLimitWin = m_vecStraightWins.size()-1;
	else
		m_siLimitWin = siLimitWin;
}
