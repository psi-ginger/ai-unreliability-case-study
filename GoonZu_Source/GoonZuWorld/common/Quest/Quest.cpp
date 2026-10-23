//---------------------------------
// 2003/11/24 김태곤
//---------------------------------
#include "CommonLogic.h"
#include "Char\CharCommon\Char-Common.h"

#include "Quest.h"
#include "..\resource.h"
#include "MsgType-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\Server\GameDBProcess\GameDBProcess.h"
#include "../Server/Server.h"
#include "../common/Skill/Skill-Manager.h"

extern cltCommonLogic* pclClient;

// 보상 내용을 텍스트에 담아낸다. 
bool cltQuestRewardInfo::GetExplainText(TCHAR* ptext, SI16 txtSize)
{
	TCHAR temp[256];

	StringCchCopy(ptext, txtSize, TEXT(""));

	if(siExp)
	{
		TCHAR* pText= GetTxtFromMgr(3090);
		StringCchPrintf(temp, 256, pText, siExp);
		StringCchCat(ptext, txtSize, temp);
	}
	if(siMoney)
	{
		GMONEY siGetMoney = siMoney;

		// 돈을 보상받을때 NPC Rate에따라 변경된다면
		if ( true == bApplyNpcRate )
		{
			SI64 siNPCPriceRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
			siGetMoney = siGetMoney * siNPCPriceRate / 100;
		}

		// 화폐단위 콤마 표시
		TCHAR szPrice[64] = { '\0', };
		g_JYLibFunction.SetNumUnit(siGetMoney, szPrice, sizeof(szPrice));

		// 실제 화폐 입력
		NTCHARString128	strMoney( GetTxtFromMgr(3091) );
		strMoney.Replace( TEXT("#money#"), szPrice );

		StringCchCat(ptext, txtSize, strMoney);
	}
	if(siItemUnique[0])
	{
		// [영훈] 재료 아이템을 받을때는 메시지를 다르게 한다
		const TCHAR* pitemname = pclClient->pclItemManager->GetName(siItemUnique[0]);

		if ( bIsGetMaterial == false )
		{
			TCHAR* pText= GetTxtFromMgr(3092);

			StringCchPrintf( temp, 256, pText, pitemname, siItemNum[0] );
			StringCchCat( ptext, txtSize, temp );
		}
		else
		{
			NTCHARString64 kText( GetTxtFromMgr(8077) );
			
			kText.Replace( TEXT("#itemname#"), pitemname );
			StringCchCat( ptext, txtSize, kText );
		}

	}
	if(siItemUnique[1])
	{
		const TCHAR* pitemname = pclClient->pclItemManager->GetName(siItemUnique[1]);
		TCHAR* pText= GetTxtFromMgr(3092);
		StringCchPrintf(temp, 256, pText, pitemname, siItemNum[1]);
		StringCchCat(ptext, txtSize, temp);
	}
	if(siItemUnique[2])
	{
		const TCHAR* pitemname = pclClient->pclItemManager->GetName(siItemUnique[2]);
		TCHAR* pText= GetTxtFromMgr(3092);
		StringCchPrintf(temp, 256, pText, pitemname, siItemNum[2]);
		StringCchCat(ptext, txtSize, temp);
	}
	if(siItemUnique[3])
	{
		const TCHAR* pitemname = pclClient->pclItemManager->GetName(siItemUnique[3]);
		TCHAR* pText= GetTxtFromMgr(3092);
		StringCchPrintf(temp, 256, pText, pitemname, siItemNum[3]);
		StringCchCat(ptext, txtSize, temp);
	}
	if(siItemUnique[4])
	{
		const TCHAR* pitemname = pclClient->pclItemManager->GetName(siItemUnique[4]);
		TCHAR* pText= GetTxtFromMgr(3092);
		StringCchPrintf(temp, 256, pText, pitemname, siItemNum[4]);
		StringCchCat(ptext, txtSize, temp);
	}
	if(siFameExp)
	{
		TCHAR* pText= GetTxtFromMgr(3093);
		StringCchPrintf(temp, 256, pText, siFameExp);
		StringCchCat(ptext, txtSize, temp);
	}
	if(siNCoin)
	{
		TCHAR* pText= GetTxtFromMgr(3094);
		StringCchPrintf(temp, 256, pText, siNCoin);
		StringCchCat(ptext, txtSize, temp);
	}
	// 스킬 경험치
	if ( (siSkillExp>0) && (siSkillType>0) )
	{
		NTCHARString128	kSkillExplain( GetTxtFromMgr(8362) );
		TCHAR			szSkillName[MAX_SKILL_NAME_LENGTH] = {'\0',};

		if ( pclClient->pclSkillManager->GetSkillName( siSkillType, szSkillName ) == true )
		{
			kSkillExplain.Replace( TEXT("#skilltype#"), szSkillName );
			kSkillExplain.Replace( TEXT("#skillexp#"), SI32ToString(siSkillExp) );

			StringCchCat( ptext, txtSize, kSkillExplain );
		}
	}
	// 이동속도
	if(bMoveBuf)
	{
		TCHAR* pText= GetTxtFromMgr(6409);		
		StringCchCat(ptext, txtSize, pText);
	}
	// [지연] : 디버프 보상
	if ( true == bDeBuff )
	{
		// 보상 내역에 대한 뭔가 메세지가 있어야 할지도 모르지만 일단은 다른 대화상자에서 모든 메세지를 찍어주기 때문에 여긴 제외한다
		// 		TCHAR* pText = GetTxtFromMgr();
		// 		StringCchCat( ptext, txtSize, pText );
	}

	return true;

}


//--------------------------------------------
// cltPQuestInfo
//--------------------------------------------
// 특정 타입의 퀘스트가 진행중이거나 수행된 것이 있는지 확인한다. 
bool cltPQuestInfo::IsQuestClearOrGoing(SI32 questtype)
{

	if(questtype < 0 || questtype >= MAX_QUEST_TYPE)return false;
	
	//Clear한 것이 있따. 
	if(clUnit[questtype].siQuestState)return true;

	// 현재 진행중인 것이 있따. 
	if(clUnit[questtype].siCurQuestUnique)return true;

	return false;

}

// 특정 퀘스트가 진행중인지 확인한다 
bool cltPQuestInfo::IsQuestGoing(SI32 clearcond, SI16* ptype, SI16* punique, SI16* ppara1, SI16* ppara2, SI32* pstartindex)
{
	SI32 i;
	cltQuestInfoCommon* pclquestinfo ;

	SI32 startpos = 0;
	if(pstartindex)			{		startpos = *pstartindex;		}

	for(i = startpos;i < MAX_QUEST_TYPE;i++)
	{
		if(clUnit[i].siCurQuestUnique == 0)									continue;

		switch(i)
		{
		case QUEST_TYPE_SPECIAL:		pclquestinfo = &clSpecialQuestInfo;				break;
		case QUEST_TYPE_NAMING:			pclquestinfo = &clNamingQuestInfo;				break;
		case QUEST_TYPE_HAPPYBEAN:		pclquestinfo = &clHappyBeanQuestInfo;			break;
		case QUEST_TYPE_MISSION1:		pclquestinfo = &clMissionQuestInfo;				break;
		default:						pclquestinfo = pclClient->pclQuestManager->GetQuestInfo( clUnit[i].siCurQuestUnique );	break;
		}
		
		if(pclquestinfo == NULL)											continue;
		if(pclquestinfo->clQuestClearConditionInfo.siFlag != clearcond)		continue;

		if(ptype)	*ptype		= i;
		if(punique)	*punique	= pclquestinfo->siUnique;
		if(ppara1)	*ppara1		= pclquestinfo->clQuestClearConditionInfo.siPara1;
		if(ppara1)	*ppara2		= pclquestinfo->clQuestClearConditionInfo.siPara2;

		if(pstartindex)		{		*pstartindex = i + 1;			}

		return true;
	}

	if(pstartindex)			{		*pstartindex = 0;				}

	return false;
}

bool cltPQuestInfo::IsQuestGoing( SI32 siClearCondition )
{
	SI16 siType		= 0;
	SI16 siUnique	= 0;
	SI16 siPara1	= 0;
	SI16 siPara2	= 0;

	return IsQuestGoing( siClearCondition, &siType, &siUnique, &siPara1, &siPara2 );
}

bool cltPQuestInfo::IsSameParaQuestGoing( SI32 siClearCond, SI16 siPara1, SI16 siPara2/* =0  */)
{
	cltQuestInfoCommon* pclQInfo = NULL;

	for ( SI32 siIndex=0; siIndex<MAX_QUEST_TYPE; siIndex++ )
	{
		// 현재 퀘스트가 진행중이 아니라면
		if ( 0 == clUnit[siIndex].siCurQuestUnique )
		{
			continue;
		}

		// 퀘스트 단계가 무조건 Going에서만 작동된다
		if ( QUEST_STEP_MIDDLE != clUnit[siIndex].siCurQuestStep )
		{
			continue;
		}

		switch ( siIndex )
		{
		case QUEST_TYPE_SPECIAL:	pclQInfo = &clSpecialQuestInfo;																break;
		case QUEST_TYPE_NAMING:		pclQInfo = &clNamingQuestInfo;																break;
		case QUEST_TYPE_HAPPYBEAN:	pclQInfo = &clHappyBeanQuestInfo;															break;
		case QUEST_TYPE_MISSION1:	pclQInfo = &clMissionQuestInfo;																break;
		default:					pclQInfo = pclClient->pclQuestManager->GetQuestInfo( clUnit[siIndex].siCurQuestUnique );	break;
		}

		if ( NULL == pclQInfo )
		{
			continue;
		}

		if ( siClearCond != pclQInfo->clQuestClearConditionInfo.siFlag )
		{
			continue;
		}

		if ( siPara1 != pclQInfo->clQuestClearConditionInfo.siPara1 )
		{
			continue;
		}

		if ( (0 < siPara2) && (siPara2 != pclQInfo->clQuestClearConditionInfo.siPara2) )
		{
			continue;
		}

		return true;
	}

	return false;
}


// 파라메타를 증가시킨다. 
bool cltPQuestInfo::IncPara(SI32 questtype, SI32 para1, SI32 incnum)
{
	cltQuestInfoCommon* pclquestinfo ;

	if(clUnit[questtype].siCurQuestUnique == 0)return false;
	
	// 특수 퀘스트이면, 
	switch( questtype )
	{
	case QUEST_TYPE_SPECIAL:		pclquestinfo = &clSpecialQuestInfo;			break;
	case QUEST_TYPE_NAMING:			pclquestinfo = &clNamingQuestInfo;			break;
	case QUEST_TYPE_HAPPYBEAN:		pclquestinfo = &clHappyBeanQuestInfo;		break;
	case QUEST_TYPE_MISSION1:		pclquestinfo = &clMissionQuestInfo;			break;
	default:						pclquestinfo = pclClient->pclQuestManager->GetQuestInfo( clUnit[questtype].siCurQuestUnique );	break;
	}
	
	if(pclquestinfo == NULL)return false;
	
	// 대상 몬스터와 일치해야 한다. 
	// 퀘스트 완료 조건의 대상 유니크가 없을경우(0) 조건 무시

	if ( pclquestinfo->clQuestClearConditionInfo.siPara1 != 0 && pclquestinfo->clQuestClearConditionInfo.siPara1 != para1 ) return false;
	
	if ( MAX_SI16 < (clUnit[questtype].siQuestPara2 + incnum) )
	{
		clUnit[questtype].siQuestPara2 = MAX_SI16;
	}
	else
	{
		clUnit[questtype].siQuestPara2 += incnum;
	}
	
	return true;
}


// 진행중인 퀘스트를 DB에 저장한다. 
bool cltPQuestInfo::SaveGoingQuestToDB(cltCharCommon* pclchar)
{
	SI32 i;
	cltQuestInfoCommon* pclquestinfo = NULL;

	for(i = 0;i < MAX_QUEST_TYPE;i++)
	{
		if(clUnit[i].siCurQuestUnique == 0)continue;

		switch( i )
		{
		case QUEST_TYPE_SPECIAL:		pclquestinfo = &clSpecialQuestInfo;		break;
		case QUEST_TYPE_NAMING:			pclquestinfo = &clNamingQuestInfo;		break;
		case QUEST_TYPE_HAPPYBEAN:		pclquestinfo = &clHappyBeanQuestInfo;	break;
		case QUEST_TYPE_MISSION1:		pclquestinfo = &clMissionQuestInfo;		break;
		default:						pclquestinfo = pclClient->pclQuestManager->GetQuestInfo( clUnit[i].siCurQuestUnique );		break;
		}

		if(pclquestinfo == NULL)continue;

		// 퀘스트의 추가 정보를 함께 저장한다.
		switch( i )
		{
		case QUEST_TYPE_NAMING:
			{
				// 명칭 퀘스트 정보를 DB에 저장한다.
				sDBRequest_SetNamingQuestInfo clMsg1(pclchar->pclCI->GetPersonID(), &clNamingQuestInfo );
				pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMsg1);
			}
			break;
		case QUEST_TYPE_MISSION1:
			{
				// 미션 퀘스트 정보를 DB에 저장한다.
				sDBRequest_SetMissionQuestInfo clMissionQuestInfo(pclchar->pclCI->GetPersonID(), &clMissionQuestInfo );
				pclClient->SendDBMsg(DBSELECT_SUB, (sPacketHeader*)&clMissionQuestInfo);
			}
			break;
		}

		// DB에 퀘스트의 상태를 저장한다. 
		sDBRequest_SetQuestInfo clMsg(pclchar->pclCI->GetPersonID(), i,	&pclchar->pclCI->clQuestInfo.clUnit[i]);		
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
	}

	return true;
}

// 특정 타입의 퀘스트가 모두 완료되었는지 확인한다. 
bool cltPQuestInfo::IsAllClear(cltQuestManager* pclquestmanager, SI32 questtype)
{
	SI32 i;
	SI32 count = 0;

	for(i = 0;i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		if(pclquestmanager->GetQuestUniqueFromBit(questtype, i))
		{
			SI32 data = clUnit[questtype].GetBit(i);
			if(data == 0)
			{
				return false;
			}

		}
	}

	return true;
}

// 완료된 퀘스트의 수를 구한다. 
SI32 cltPQuestInfo::GetClearQuestNum(cltQuestManager* pclquestmanager, SI32 questtype)
{
	SI32 i;
	SI32 count = 0;

	for(i = 0;i < MAX_QUEST_PER_TYPE_NUMBER;i++)
	{
		SI32 data = clUnit[questtype].GetBit(i);
		if(data)
		{
			if(pclquestmanager->GetQuestUniqueFromBit(questtype, i))
			{
				count++;
			}
		}
	}

	return count;
}

// 특정 유니크의 퀘스트를 종료했는가. 
bool cltPQuestInfo::IsClear(cltQuestManager* pclquestmanager, SI32 questunique)
{
	cltQuestInfo* pclquestinfo = pclquestmanager->GetQuestInfo(questunique);
	if(pclquestinfo == NULL)return false;

	// 퀘스트의 타입을 구한다. 
	SI32 questtype = pclquestinfo->siType;

	// 해당 퀘스트 타입에서의 비트를 구한다. 
	SI32 bit  = pclquestmanager->GetBitFromUnique(questtype, questunique);

	if(clUnit[questtype].GetBit(bit))return true;

	return false;
}

// 흥부박 퀘스트의 단어 맞추기가  완료되었다면, 
bool cltPQuestInfo::IsWordAssembleCompleted(cltQuestManager* pclQuestManager)
{
	if(pclQuestManager->siWordAssemblyNumber <= 0)return false;

	for(SI32 i = 0;i < pclQuestManager->siWordAssemblyNumber;i++)
	{
		if(bWordAssemblySwitch[i] == false)return false;
	}

	return true;
}

// 단어 조합 정보를 초기화한다. 
void cltPQuestInfo::InitWordAssemble()
{
	for(SI32 i = 0;i < MAX_WORD_ASSEMBLY_CHAR_NUMBER;i++)
	{
		bWordAssemblySwitch[i] = false;
	}
}

bool cltPQuestInfo::GetWordAssembleText( cltQuestManager* pclQuestManager, TCHAR* pszBuffer, SI32 siBufferSize )
{
	if ( (NULL == pclQuestManager) || (NULL == pszBuffer) || (0 >= siBufferSize) )
	{
		return false;
	}

	TCHAR	szWord[256]		= { '\0', };
	SI32	siWordNumber	= pclQuestManager->siWordAssemblyNumber;

	for ( SI32 siIndex=0 ; siIndex<siWordNumber; ++siIndex )
	{
		if ( bWordAssemblySwitch[siIndex] )
		{
			StringCchCat( szWord, 256, pclQuestManager->szWordAssemblyString[siIndex] );
		}
		else
		{
			StringCchCat( szWord, 256, "?" );
		}

		StringCchCat( szWord, 256, " " );
	}


	TCHAR* pText = GetTxtFromMgr( 4383 );
	StringCchPrintf( pszBuffer, siBufferSize, pText, szWord );

	return true;
}

//--------------------------------------------
// cltQuestInfo
//--------------------------------------------
void cltQuestInfo::Set(SI32 type, SI32 unique, 
					   cltQuestInitConditionInfo* pclinitcond, 
					   cltQuestClearConditionInfo* pclclearcond, 
					   cltQuestRewardInfo* pclreward, 
					   TCHAR* titletext, 
					   TCHAR* pimagefilename)
{
	siType		= type;
	siUnique	= unique;

	// 초기조건. 
	clInitConditionInfo.Set(pclinitcond);

	// 완료 조건 설정. 
	clQuestClearConditionInfo.Set(pclclearcond);

	// 보상 조건. 
	clQuestRewardInfo.Set(pclreward);

	MStrCpy( szTitleText, titletext,	MAX_TITLE_TEXT_SIZE );

	if(pimagefilename)
	{
		pImageFileName = new TCHAR[_tcslen(pimagefilename) + 1];
		StringCchCopy(pImageFileName, _tcslen(pimagefilename) + 1, pimagefilename);
	}

}



