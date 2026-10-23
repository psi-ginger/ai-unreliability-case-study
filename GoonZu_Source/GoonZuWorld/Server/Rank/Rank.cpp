//---------------------------------
// 2003/6/12 김태곤
//---------------------------------


#include "Rank.h"
#include <Stdio.h>
#include "..\..\CommonLogic\CommonLogic.h"

extern cltCommonLogic* pclClient;

//---------------------------------
// cltRank
//---------------------------------
void cltRank::Set(const cltSimpleRank* pclrank, const cltSimplePerson* pclperson)
{
	clRank.Set(pclrank);
	clPerson.Set(pclperson);
	
}


//---------------------------------
// cltRankManager
//---------------------------------
cltRankManager::cltRankManager()
{

}

cltRankManager::~cltRankManager()
{
}


cltRank* cltRankManager::GetRank(SI32 ranktype, SI32 villageunique)
{
	if(ranktype < 0 || ranktype >= MAX_RANKTYPE_NUMBER)return NULL;
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return NULL;

	return &clRank[ranktype][villageunique];	
}

// 직급을 갖고 있는지 확인한다
cltRank* cltRankManager::GetRankInfo(SI32 personid)
{
	SI32 i;
	SI32 rank;

	for(rank = 0;rank < MAX_RANKTYPE_NUMBER; rank++)
		for(i  = 0;i < MAX_VILLAGE_NUMBER;i++)
		{
			if(clRank[rank][i].GetRankType())
			{
				if(clRank[rank][i].GetPersonID() == personid)
				{
					return &clRank[rank][i];
				}
			}
		}

		return NULL;
}

// 신분을 갖는 사람을 등록한다. 
SI32 cltRankManager::SetRank(const cltSimpleRank* pclrank, const cltSimplePerson* pclperson )
{
	if(pclrank == NULL)return -1;
	if(pclperson == NULL)return -1;

	SI32 ranktype = pclrank->GetRankType();
	SI32 villageunique = pclrank->GetVillageUnique();
	if(ranktype < 0 || ranktype >= MAX_RANKTYPE_NUMBER)return 0;
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return 0;

	clRank[ranktype][villageunique].Set(pclrank, pclperson);

	return 0;
}


// personid가 특정 신분인지 확인한다. 
BOOL cltRankManager::IsRank(SI32 personid, cltSimpleRank* pclrank)
{
	if(pclrank == NULL)return FALSE;

	SI32 ranktype = pclrank->GetRankType();
	SI32 villageunique = pclrank->GetVillageUnique();
	if(ranktype < 0 || ranktype >= MAX_RANKTYPE_NUMBER)return false;
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return false;

	if(clRank[ranktype][villageunique].GetPersonID() == personid 
	&& clRank[ranktype][villageunique].IsSame(pclrank) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

// personid의 특정 신분을 삭제한다. 
BOOL cltRankManager::DeleteRank(SI32 personid, cltSimpleRank* pclrank)
{
	if(pclrank == NULL)return FALSE;

	SI32 ranktype = pclrank->GetRankType();
	SI32 villageunique = pclrank->GetVillageUnique();
	if(ranktype < 0 || ranktype >= MAX_RANKTYPE_NUMBER)return false;
	if(villageunique < 0 || villageunique >= MAX_VILLAGE_NUMBER)return false;


	if(clRank[ranktype][villageunique].GetPersonID() == personid 
	&& clRank[ranktype][villageunique].IsSame(pclrank) == TRUE)
	{
		pclClient->pclLog->FilePrint(TEXT("ResignLog.txt"), TEXT("DeleteRank0 Person[%s %d] Vill[%d] rank[%d]"), 
			clRank[ranktype][villageunique].clPerson.GetName(), 
			clRank[ranktype][villageunique].clPerson.siPersonID, 
			villageunique, ranktype);

		clRank[ranktype][villageunique].Init();

		pclClient->pclLog->FilePrint(TEXT("ResignLog.txt"), TEXT("DeleteRank1 Person[%s %d] Vill[%d] rank[%d]"), 
			clRank[ranktype][villageunique].clPerson.GetName(), 
			clRank[ranktype][villageunique].clPerson.siPersonID, 
			villageunique, ranktype);

		
		return TRUE;
	}

	return FALSE;
}


// 신분의 이름을 얻어온다. 
BOOL GetRankName(cltSimpleRank* pclrank, TCHAR* buffer, SI16 txtSize, UI08 grank)
{
	TCHAR* pText = GetTxtFromMgr(3924);
	if(pclrank == NULL && grank == 0 )return FALSE;
	if(pclrank->GetRankType() == 0 && grank == 0 )
	{
		pText = GetTxtFromMgr(3924);
		StringCchCopy(buffer, txtSize, pText);
		return TRUE;
	}

	if ( grank )
	{
		switch(grank)
		{
		case GRANK_KING: // 군주
			{
				pText = GetTxtFromMgr(599);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		case GRANK_MINISTER1: // 이조
			{
				pText = GetTxtFromMgr(1124);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		case GRANK_MINISTER2: // 호조
			{
				pText = GetTxtFromMgr(1123);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		case GRANK_MINISTER3: // 예조
			{
				pText = GetTxtFromMgr(1125);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		case GRANK_MINISTER4: // 병조
			{
				pText = GetTxtFromMgr(1120);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		case GRANK_MINISTER5: // 형조
			{
				pText = GetTxtFromMgr(1122);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		case GRANK_MINISTER6: // 공조
			{
				pText = GetTxtFromMgr(1121);
				StringCchCopy(buffer, txtSize, pText);
				return TRUE;
			}
			break;
		}
	}

	TCHAR* pvillagename = NULL;

	// 마을의 이름을 얻어온다. 
	if( pclrank->GetVillageUnique() >= 0 && pclrank->GetVillageUnique() < MAX_VILLAGE_NUMBER )
	{
		pvillagename = pclClient->pclVillageManager->pclVillageInfo[pclrank->GetVillageUnique()]->szName;
	}


	switch(pclrank->GetRankType())
	{
	case RANKTYPE_CITYHALL:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(1947);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;
	case RANKTYPE_BANK:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3793);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;
	case RANKTYPE_STOCK:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3794);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;
	case RANKTYPE_HUNT:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3795);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;
	case RANKTYPE_MARKET:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3796);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;
	case RANKTYPE_HOUSE:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3797);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;
	
	case RANKTYPE_POSTOFFICE:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3798);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;


	case RANKTYPE_HORSEMARKET:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3800);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;		

	case RANKTYPE_REALESTATEMARKET:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3801);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;		

	case RANKTYPE_SUMMONMARKET:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3802);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;		

	case RANKTYPE_LAND:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3804);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;		

	case RANKTYPE_FEAST:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3805);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename,pText);
			return TRUE;
		}
		break;		
		
	case RANKTYPE_SUMMONHEROMARKET:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3806);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename,  pText);
			return TRUE;
		}
		break;		

	case RANKTYPE_MINE:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(3807);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;		

	case RANKTYPE_GUILD:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(5361);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
			return TRUE;
		}
		break;		
	case RANKTYPE_NEWMARKET:
		if(pvillagename)
		{
			TCHAR* pText = GetTxtFromMgr(5361);
			StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, GetTxtFromMgr(9832)); 
			return TRUE;
		}
		break;		


	default:
		MsgBox(TEXT("fd9872j"), TEXT("fd9872h:%d"), pclrank->GetRankType());
		break;

	}

	return FALSE;
}

// 관직신분의 이름을 얻어온다. 
bool GetMisterRankName(UI08 grank, TCHAR* buffer, SI16 txtSize)
{
	if(0 == grank)
		return false;
	
	TCHAR* pText = NULL;

	switch(grank)
	{
		case GRANK_KING: // 군주
			{
				pText = GetTxtFromMgr(599);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
		case GRANK_MINISTER1: // 이조
			{
				pText = GetTxtFromMgr(1124);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
		case GRANK_MINISTER2: // 호조
			{
				pText = GetTxtFromMgr(1123);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
		case GRANK_MINISTER3: // 예조
			{
				pText = GetTxtFromMgr(1125);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
		case GRANK_MINISTER4: // 병조
			{
				pText = GetTxtFromMgr(1120);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
		case GRANK_MINISTER5: // 형조
			{
				pText = GetTxtFromMgr(1122);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
		case GRANK_MINISTER6: // 공조
			{
				pText = GetTxtFromMgr(1121);
				StringCchCopy(buffer, txtSize, pText);
				return true;
			}
			break;
	}
	

	return false;
}

bool GetVillageRankName(cltSimpleRank* pclrank, TCHAR* buffer, SI16 txtSize)
{
	TCHAR* pvillagename = NULL;

	// 마을의 이름을 얻어온다. 
	if( pclrank->GetVillageUnique() >= 0 && pclrank->GetVillageUnique() < MAX_VILLAGE_NUMBER )
	{
		pvillagename = pclClient->pclVillageManager->pclVillageInfo[pclrank->GetVillageUnique()]->szName;
	}

	switch(pclrank->GetRankType())
	{
		case RANKTYPE_CITYHALL:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(1947);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;
		case RANKTYPE_BANK:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3793);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;
		case RANKTYPE_STOCK:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3794);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;
		case RANKTYPE_HUNT:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3795);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;
		case RANKTYPE_MARKET:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3796);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;
		case RANKTYPE_HOUSE:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3797);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;

		case RANKTYPE_POSTOFFICE:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3798);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;


		case RANKTYPE_HORSEMARKET:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3800);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;		

		case RANKTYPE_REALESTATEMARKET:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3801);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;		

		case RANKTYPE_SUMMONMARKET:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3802);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;		

		case RANKTYPE_LAND:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3804);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;		

		case RANKTYPE_FEAST:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3805);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename,pText);
				return true;
			}
			break;		

		case RANKTYPE_SUMMONHEROMARKET:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3806);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename,  pText);
				return true;
			}
			break;		

		case RANKTYPE_MINE:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(3807);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText);
				return true;
			}
			break;		

		case RANKTYPE_GUILD:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(5361);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText); 
				return true;
			}
			break;	
		case RANKTYPE_NEWMARKET:
			if(pvillagename)
			{
				TCHAR* pText = GetTxtFromMgr(9835);
				StringCchPrintf(buffer, txtSize, TEXT("%s %s"), pvillagename, pText); 
				return true;
			}
			break;	
	}


	return false;
}