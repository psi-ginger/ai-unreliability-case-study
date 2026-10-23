#include "WarRank.h"
#include "CommonLogic.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

void cltWarRankInfo::Init()
{
	ZeroMemory( this, sizeof( cltWarRankInfo ) );
}

void cltWarRankInfo::SetRankPerson( int ranknum, int personid, char *name, int score )
{
	if( ranknum < 1 || ranknum > MAX_WARRANK_PERSON_PER_MAP )
	{
		return;
	}

	int index = ranknum - 1;

	clRankPerson[ index ].m_ranknum = ranknum;
	clRankPerson[ index ].m_personid = personid;
	MStrCpy( clRankPerson[ index ].m_szPlayerName, name, MAX_PLAYER_NAME );
	clRankPerson[ index ].m_score = score;
}

cltWarRankPersonInfo* cltWarRankInfo::GetRankPerson( int ranknum )
{
	if( ranknum < 1 || ranknum > MAX_WARRANK_PERSON_PER_MAP )
	{
		return NULL;
	}

	return &clRankPerson[ ranknum - 1 ];
}


void cltWarRankManager::SetWarRankInfo( cltWarRankInfo *pWarRankInfo )
{
	int warmapindex = pWarRankInfo->m_warmapindex;

	if( warmapindex < 0 || warmapindex >= MAX_BLACKARMY_MAP_CNT )
	{
		return;
	}

	memcpy( &clRankInfo[ warmapindex ], pWarRankInfo, sizeof( cltWarRankInfo ) );

	SetRankMVP( warmapindex );

}

cltWarRankInfo* cltWarRankManager::GetWarRankInfo( int warmapindex )
{
	if( warmapindex < 0 || warmapindex >= MAX_BLACKARMY_MAP_CNT )
	{
		return NULL;
	}

	return &clRankInfo[ warmapindex ];
}

bool cltWarRankManager::SetRankMVP( int warmapindex )
{
	for (int i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
	{
		if (clRankInfo[ warmapindex ].clRankPerson[i].m_ranknum == 1)
		{
			int personid = clRankInfo[ warmapindex ].clRankPerson[i].m_personid;
			int id = pclClient->pclCM->GetIDFromPersonID(personid);

			if (pclClient->pclCM->IsValidID(id))
			{
				pclClient->pclCM->CR[id]->pclCI->clBI.uiGMMode = GMMODE_ATB_WARMVP;
				pclClient->pclCM->CR[id]->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );

				return true;
			}
		}
	}

	return false;
}

bool cltWarRankManager::IsRankMVPFromPersonID( SI32 personid  )
{
	for (int i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{
		for (int j = 0; j < MAX_WARRANK_PERSON_PER_MAP; j++)
		{
			if (clRankInfo[ i ].clRankPerson[j].m_ranknum == 1)
			{
				if (clRankInfo[ i ].clRankPerson[j].m_personid == personid)
				{
					return true;
				}
			}

		}
	}

	return false;
}

