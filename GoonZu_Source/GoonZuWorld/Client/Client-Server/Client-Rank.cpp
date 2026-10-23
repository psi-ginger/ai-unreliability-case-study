//---------------------------------
// 2004/5/19 김태곤
//---------------------------------
#include "..\Common\CommonHeader.h"
#include "..\Client.h"

#include "..\Server\Candidate\Candidate.h"

#include "..\..\CommonLogic\Cityhall\Cityhall.h"
#include "Msg\MsgType-Rank.h"

extern cltCommonLogic* pclClient;

// 신분 입후보자 정보를 받음. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_CANDIDATELIST(cltMsg* pclMsg, SI32 id)
{
	// 받은 대행수 정보를 CityHallManager에게 전달한다. 
	cltGameMsgResponse_CandidateList* pclPacket = (cltGameMsgResponse_CandidateList*)pclMsg->cData;
	
	pclCityHallManager->pclClientCandidateManager->SetCandidate(pclPacket->siIndex, pclPacket->clRank.GetRankType(), &pclPacket->clPerson);
	
	switch(pclPacket->clRank.GetRankType())
	{
	case RANKTYPE_CITYHALL:
		// 대행수 후보  정보를 업데이트 할 것을 요구한다. 
		pclCityHallManager->UpdateMayorCandidateSwitch = TRUE;
		break;
	case RANKTYPE_BANK:
	case RANKTYPE_STOCK:
	case RANKTYPE_MARKET:
	case RANKTYPE_HOUSE:
	case RANKTYPE_HUNT:
	case RANKTYPE_POSTOFFICE:
	case RANKTYPE_HORSEMARKET:
	case RANKTYPE_REALESTATEMARKET:
	case RANKTYPE_SUMMONMARKET:
	case RANKTYPE_LAND:
	case RANKTYPE_FEAST:
	case RANKTYPE_SUMMONHEROMARKET:
	case RANKTYPE_MINE:
	case RANKTYPE_GUILD:
	case RANKTYPE_NEWMARKET:
		// 각종 후보  정보를 업데이트 할 것을 요구한다. 
		pclCityHallManager->UpdateChiefCandidateSwitch = TRUE;
		break;
		
	default:
		MsgBox(TEXT("fd9872j"), TEXT("f8f:%d"), pclPacket->clRank.GetRankType());
		break;
	}

}
