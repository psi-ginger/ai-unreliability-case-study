//=============================================================================
//
//=============================================================================

#include "NQuestTypeGameDataManager.h"

NQuestTypeGameDataManager::NQuestTypeGameDataManager()
{
	for(SI32 i=0; i<MAX_QUEST_TYPE; i++)
	{
		m_pkData[i] = NULL;
	}
}

NQuestTypeGameDataManager::~NQuestTypeGameDataManager()
{
	for(SI32 i=0; i<MAX_QUEST_TYPE; i++)
	{
		if(m_pkData[i] != NULL)
			delete m_pkData[i];
	}
}

BOOL NQuestTypeGameDataManager::Init()
{
	return LoadQuestTypeGameData();
}

BOOL NQuestTypeGameDataManager::LoadQuestTypeGameData()
{
	SI32 i;
	FILE* fp;
	TCHAR buffer[1024];
	SI32 questtype = QUEST_TYPE_BATTLE1;	

	TCHAR QuestFileName[][128] =
	{
		TEXT("Special"),		// 사용되지 않음.
		TEXT("Battle1"),
		TEXT("MakeHeal1"),
		TEXT("MakeSword1"),
		TEXT("MakeBow1"),
		TEXT("MakeHelmet1"),
		TEXT("Beginner3"),
		TEXT("MakeSpear1"),
		TEXT("MakeArmor1"),
		TEXT("MakeAx1"),
		TEXT("MakeGun1"),
		TEXT("MakeRod1"),
		TEXT("MakeBelt1"),
		TEXT("MakeShoes1"),
		TEXT("Beginner2"),
		TEXT("Battle2"),
		TEXT("Basic"),
		TEXT("Beginner4"),
		TEXT("MakeBeginner1"),
		TEXT("NULL")
	};

	while(_tcscmp(QuestFileName[questtype], TEXT("NULL")) != 0)
	{
		StringCchPrintf(buffer, 1024, TEXT("Quest\\%s.txt"), QuestFileName[questtype]);

		fp = _tfopen(buffer, TEXT("rt"));
		if(fp == NULL )			return FALSE;

		NQuestTypeGameData* pkNewData = new NQuestTypeGameData;
		if(pkNewData)
		{
			// 퀘스트 타입을 확보한다. 
			pkNewData->m_siQuestType = questtype;
			// 퀘스트 타입 이름을 확보한다. 
			StringCchCopy(pkNewData->m_szQuestName, sizeof(pkNewData->m_szQuestName), QuestFileName[questtype]);
			// 첫번째 8은 무시한다. 
			for(i = 0;i < 8;i++)
			{
				_fgetts(buffer, 1024, fp);
			}

			SI32 bit;
			SI32 unique;

			for(i = 0;i <MAX_QUEST_PER_TYPE_NUMBER;i++)
			{
				_fgetts(buffer, 1024, fp);

				_stscanf(buffer, TEXT("%d %d"), &bit, &unique);

				pkNewData->m_siQuestUnique[bit]	= unique;
			}
			fclose(fp);

			if(questtype >= MAX_QUEST_TYPE)			return FALSE;
			m_pkData[questtype] = pkNewData;
		}

		questtype++;
	}

	return TRUE;
}
