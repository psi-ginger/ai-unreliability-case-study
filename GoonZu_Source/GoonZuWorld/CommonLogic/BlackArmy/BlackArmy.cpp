//---------------------------------
// 2004/2/18 ���°�
//---------------------------------

#include "BlackArmy.h"

#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Music\music.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharServer\Char-Server.h"
#include "Char\CharManager\CharManager.h"
#include "MsgType-System.h"
#include "MsgType-Person.h"
#include "MsgRval-Define.h"

#include "..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "NDataLoader.h"

#include "WarRank\WarRank.h"
#include "PartyMgr\PartyObj.h"
#include "PartyMgr\PartyMgr.h"

#include "Minister\MinisterMgr.h"
#include "Minister\Minister-Byungzo.h"

extern cltCommonLogic* pclClient;

//---------------------------------------
// cltBackArmyApplyPerson
//---------------------------------------
cltBlackArmyApplyPerson::cltBlackArmyApplyPerson()
{
	Init();
}

cltBlackArmyApplyPerson::cltBlackArmyApplyPerson(cltSimplePerson applyperson)
{
	Init();

	clApplyPerson.Set(&applyperson);
//	siPartyIndex = partyindex;
//	bPartyLearder = partyleader;
}

cltBlackArmyApplyPerson::~cltBlackArmyApplyPerson()
{
}

void cltBlackArmyApplyPerson::Init()
{
	clApplyPerson.Init();
	siWarMapUnique = 0;
//	bPartyLearder = false;
//	siPartyIndex = 0;
	bWarMapAlloc = false;
}


void cltBlackArmyApplyPerson::Set(cltBlackArmyApplyPerson *pinfo)
{
	clApplyPerson.Set(&pinfo->clApplyPerson);
	siWarMapUnique = pinfo->siWarMapUnique;
//	bPartyLearder = pinfo->bPartyLearder;
//	siPartyIndex = pinfo->siPartyIndex;
	bWarMapAlloc = pinfo->bWarMapAlloc;
}

//---------------------------------------
// cltBackArmyApplyList
//---------------------------------------
cltBlackArmyApplyList::cltBlackArmyApplyList()
{
	siPersonIndex = 0;
	InitList();
}

cltBlackArmyApplyList::~cltBlackArmyApplyList()
{
}


SI32 cltBlackArmyApplyList::AddApplyPerson(cltBlackArmyApplyPerson *pPerson)
{
	SI32 retPersonIndex = -1;

	if (siPersonIndex >= 0 && siPersonIndex < MAX_BLACKARMY_APPLY_CNT)
	{
		clApplyPersonList[siPersonIndex].Set(pPerson);

		retPersonIndex = siPersonIndex;
		siPersonIndex++;

	}

	return retPersonIndex;
}

void cltBlackArmyApplyList::InitList()
{
	for (int i = 0; i < MAX_BLACKARMY_APPLY_CNT; i++)
	{
		clApplyPersonList[i].Init();
	}

	siPersonIndex = 0;
}

SI32 cltBlackArmyApplyList::FindApplyFromPersonid(SI32 personid)
{
	SI32 retApplyIndex = -1;

	for (int i = 0; i < siPersonIndex; i++)
	{
		if (clApplyPersonList[i].clApplyPerson.GetPersonID() == personid)
			retApplyIndex = i;
	}

	return	retApplyIndex;

}


//---------------------------------------
// cltBackArmyWarMap
//---------------------------------------
cltBlackArmyWarMap::cltBlackArmyWarMap()
{
	Init();
}

cltBlackArmyWarMap::~cltBlackArmyWarMap()
{
}

void cltBlackArmyWarMap::Init()
{
	siPersonIndex = 0;
	siMapIndex = 0;

	siGateUnique = 0;

	siMapUnitNum = 0;
	siMapCurUnitNum = 0;
	siMapMadeNum = 0;

	//siWarStep = 0;

	siWarcmd = 0;

	bWarDoing = false;

	for (SI32 i = 0; i < MAX_BLACKARMY_UNITKIND; i++)
	{
		clBlackArmyUnit[i].Init();
	}

	InitList();
}


SI32 cltBlackArmyWarMap::AddWarPerson(cltBlackArmyApplyPerson *pPerson)
{
	SI32 retPersonIndex = -1;

	if (siPersonIndex >= 0 && siPersonIndex < MAX_BLACKARMY_MAP_PERSON_CNT)
	{
		clWarPersonList[siPersonIndex].Set(pPerson);

		retPersonIndex = siPersonIndex;
		siPersonIndex++;

	}

	return retPersonIndex;

}

void cltBlackArmyWarMap::InitList()
{
	for (int i = 0; i < MAX_BLACKARMY_MAP_PERSON_CNT; i++)
	{
		clWarPersonList[i].Init();
	}

	siPersonIndex = 0;
}

//---------------------------------------
// cltBlackArmyUnit
//---------------------------------------
cltBlackArmyUnit::cltBlackArmyUnit()
{
	Init();
}

cltBlackArmyUnit::cltBlackArmyUnit(SI32 kind, SI32 num, SI32 curnum, SI32 madenum, SI32 basic, SI32 rand)
{
	siKind		= kind;
	siNum		= num;
	siCurNum	= curnum;
	siMadeNum	= madenum;
	siBasicLevel= basic;
	siRandLevel	= rand;
}

void cltBlackArmyUnit::Init()
{
	siKind		= 0;
	siNum		= 0;
	siCurNum	= 0;
	siMadeNum	= 0;
	siBasicLevel= 0;
	siRandLevel	= 0;
}

void cltBlackArmyUnit::Set(cltBlackArmyUnit *blackarmyunit)
{
	siKind		= blackarmyunit->siKind;
	siNum		= blackarmyunit->siNum;
	siCurNum	= blackarmyunit->siCurNum;
	siMadeNum	= blackarmyunit->siMadeNum;
	siBasicLevel= blackarmyunit->siBasicLevel;
	siRandLevel	= blackarmyunit->siRandLevel;
}


//---------------------------------------
// cltBlackArmyManager
//---------------------------------------
cltBlackArmyManager::cltBlackArmyManager()
{
	bWarSwitch	= false;
	bApplySwitch = false;
	bAutoBlackWarSwitch = false;
	
	//siTotalUnitNum= 0;		// ��ü �����. 
	//siTotalCurUnitNum = 0;		// ���� �����. 

	siSettingMonNum = 0;

	siUnitKind = 0;				// ����Ʈ�� �� ���� �ִ°�?
	uiLeftTime = 0;				// ���� �ð�. (ms) 
	dwWarStartClock = 0;

	bDrawSwitch = false;
	siDrawCmdFont = 0;
	dwDrawStartClock = 0;
}


cltBlackArmyManager::~cltBlackArmyManager()
{
}

void cltBlackArmyManager::Init()
{
	LoadWarInfoFile();
}

void cltBlackArmyManager::LoadWarInfoFile()
{
	// ���� ������ �ʱ�ȭ
	siUnitKind	= 0;

	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromFile( TEXT("ServerInfo\\WarInfo.txt") );
	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("Don't LoadFile: WarInfo.txt"));
		return;
	}

	SI32 num		= 0;
	SI32 basic		= 0;
	SI32 randlevel	= 0;
	TCHAR charname[128] = { '\0' };

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	charname,		128,
		NDATA_INT32,	&num,			4,		
		NDATA_INT32,	&basic,			4,		
		NDATA_INT32,	&randlevel,		4,		
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if(siUnitKind >= MAX_BLACKARMY_UNITKIND)	{	MsgBox("fdj82jfd", "dsif83jf");		continue;	}

			// ������ �߰����� �ʴ´�.
			if( strncmp(charname, "KIND_BLACKBOSS", strlen("KIND_BLACKBOSS")) == 0 )			continue;

			SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode(charname);
			if(kind)
			{
				oneMapUnit[siUnitKind].Init();
				oneMapUnit[siUnitKind].siKind		= kind;
				oneMapUnit[siUnitKind].siNum 		= num;
				oneMapUnit[siUnitKind].siBasicLevel	= basic;
				oneMapUnit[siUnitKind].siRandLevel	= randlevel;

				siUnitKind++;
			}
		}
	}

	bLoadFile = dataloader.LoadDataFromFile( TEXT("ServerInfo\\WarInfo.txt") );
	SI32 BossKind = siUnitKind;

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if(BossKind >= MAX_BLACKARMY_UNITKIND)	{	MsgBox("fdj82jfd", "dsif83jf");		continue;	}

			// ������ �߰��Ѵ�.
			if( strncmp(charname, "KIND_BLACKBOSS", strlen("KIND_BLACKBOSS")) != 0 )			continue;

			SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode(charname);
			if(kind)
			{
				oneMapUnit[BossKind].Init();
				oneMapUnit[BossKind].siKind			= kind;
				oneMapUnit[BossKind].siNum 			= num;
				oneMapUnit[BossKind].siBasicLevel	= basic;
				oneMapUnit[BossKind].siRandLevel	= randlevel;

				BossKind++;
			}
		}
	}

}

SI32 cltBlackArmyManager::ApplyPerson(cltBlackArmyApplyPerson *pPerson)
{
	if (clApplyList.FindApplyFromPersonid(pPerson->clApplyPerson.GetPersonID()) >= 0)
	{
		return -2;
	}

	return clApplyList.AddApplyPerson(pPerson);
}

void cltBlackArmyManager::JoinWarMap(SI16 ApplyIndex)
{
	SI16 retWapMapUnique = -1;
	SI16 i = 0;
	SI32 id = 0;
	cltCharServer* pclchar = NULL;

	cltBlackArmyApplyPerson clPerson = clApplyList.clApplyPersonList[ApplyIndex];

	if (clPerson.bWarMapAlloc)									return;
	if (clPerson.clApplyPerson.GetPersonID() <= 0)				return;

	id = pclClient->pclCM->GetIDFromPersonID(clPerson.clApplyPerson.GetPersonID());
	if (id <= 0 || pclClient->pclCM->IsValidID(id) == false)	return;
    pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
//	SI32 max_warmap_usernum = MAX_BLACKARMY_MAP_PERSON_CNT;

	// ��Ƽ���� ���⼭ ó������ �ʴ´�.
	if( pclchar->siPartyIndex > 0 )								return;

	// �ʿ� �Ҵ��Ѵ�.
//    for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	i = siNextJoinMap;
	SI32 siCheckMapCount = 0;

	while ( siCheckMapCount < MAX_BLACKARMY_MAP_CNT)
	{
		if( MAX_BLACKARMY_MAP_PERSON_CNT - clWarMap[i].siPersonIndex > 0 )
//		if (clWarMap[i].siPersonIndex < max_warmap_usernum )
		{
			SI32 siApplyIndex = clWarMap[i].AddWarPerson(&clPerson);
			if( siApplyIndex >= 0 )
			{
				SI32 id = pclClient->pclCM->GetIDFromPersonID(clPerson.clApplyPerson.GetPersonID());
				if (id > 0 && pclClient->pclCM->IsValidID(id))
				{
					cltCharServer *pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
					pclchar->siBlackArmyWarMapIndex = i;
					clApplyList.clApplyPersonList[ApplyIndex].siWarMapUnique = clWarMap[i].siMapIndex;
					clApplyList.clApplyPersonList[ApplyIndex].bWarMapAlloc = true;

					// 5�� �����ŭ ������ ä��� ���� ������ �����Ѵ�.
					if( clWarMap[i].siPersonIndex % 5 == 0)				siNextJoinMap = (i+1) % MAX_BLACKARMY_MAP_CNT;
				}
				break;
			}
		}
		i = (i+1) % MAX_BLACKARMY_MAP_CNT;		// ������ üũ
		siCheckMapCount++;						// üũ �� ����
	}
}

// ��Ƽ ���� �ο� �Ҵ�
void cltBlackArmyManager::JoinWarMapWithParty(SI16 ApplyIndex)
{
	SI32 totalPartyMembers = 0; 
	SI32 partyindex = 0;
	SI32 i = 0, j = 0;

	SI32 id	 = 0;
	cltCharServer *pclchar = NULL;

	cltBlackArmyApplyPerson clPerson = clApplyList.clApplyPersonList[ApplyIndex];

	// �̹� �ʿ� �Ҵ���� ������ ó������ �ʴ´�.
	if (clPerson.bWarMapAlloc)									return;
	if (clPerson.clApplyPerson.GetPersonID() <= 0)				return;

	// ���� ���� �ʴ� ������ ����
	id = pclClient->pclCM->GetIDFromPersonID(clPerson.clApplyPerson.GetPersonID());
	if (id <= 0 || pclClient->pclCM->IsValidID(id) == false)	return;
	pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

	//SI32 max_warmap_usernum = MAX_BLACKARMY_MAP_PERSON_CNT;

	SI32 PartyApplyIndex[MAX_PARTY_USER_NUM];
	for (i = 0; i < MAX_PARTY_USER_NUM; i++)	{	PartyApplyIndex[i] = -1;	}

	// ��Ƽ���� �ƴ� ������ ó������ �ʴ´�.
	partyindex = pclchar->siPartyIndex;
	if( partyindex <= 0 )										return;

	// ��û�ο��߿� ��Ƽ�ο��� ã�´�.
	for (i = 0; i < clApplyList.siPersonIndex; i++)
	{
		id = pclClient->pclCM->GetIDFromPersonID(clApplyList.clApplyPersonList[i].clApplyPerson.GetPersonID());
		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
			if (pclchar->siPartyIndex == partyindex && clApplyList.clApplyPersonList[i].bWarMapAlloc == false)
			{
				PartyApplyIndex[totalPartyMembers] = i;
				totalPartyMembers++;
			}
		}
	}

	// �ʿ� �Ҵ��Ѵ�.
	//for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	i = siNextJoinMap;
	SI32 siCheckMapCount = 0;

	while ( siCheckMapCount < MAX_BLACKARMY_MAP_CNT)
	{
		if( MAX_BLACKARMY_MAP_PERSON_CNT - clWarMap[i].siPersonIndex >= totalPartyMembers )
		{
			for (j = 0; j < totalPartyMembers; j++)
			{
				cltBlackArmyApplyPerson clPartyPerson = clApplyList.clApplyPersonList[PartyApplyIndex[j]];
				SI32 siApplyIndex = clWarMap[i].AddWarPerson(&clPartyPerson);

				if( siApplyIndex >= 0 )
				{
					id = pclClient->pclCM->GetIDFromPersonID(clPartyPerson.clApplyPerson.GetPersonID());
					if (id > 0 && pclClient->pclCM->IsValidID(id))
					{
						pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
						pclchar->siBlackArmyWarMapIndex = i;

						clApplyList.clApplyPersonList[PartyApplyIndex[j]].siWarMapUnique = clWarMap[i].siMapIndex;
						clApplyList.clApplyPersonList[PartyApplyIndex[j]].bWarMapAlloc = true;
					}
				}
			}
			// ���� ������ ����
			siNextJoinMap = (i+1) % MAX_BLACKARMY_MAP_CNT;
			break;
		}
		i = (i+1) % MAX_BLACKARMY_MAP_CNT;		// ������ üũ
		siCheckMapCount++;						// üũ �� ����
	}
}

// ������ ��ȹ�Ѵ�. 
bool cltBlackArmyManager::MakeWar(DWORD clock)
{
	// �������̶��, ������ �� ����.
	if (bWarSwitch)			return false;

	// ���￡ ������ ������� ������ �����Ѵ� 
	SI32 i;

	// ���� �������� ��������
	cltServer* pclserver = (cltServer*)pclClient;
	CMinisterByungzo* pclminister = (CMinisterByungzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_BYUNGZO);

	// ������ ������ ��
	siSettingMonNum = pclminister->m_siBlackArmyNum;

	// ������ �� ���� ��ȿ�� ������ ����
	if (siSettingMonNum < 100 && siSettingMonNum > 600)		{	siSettingMonNum = 100;	}

	// ���� ������ �ʱ�ȭ
/*	siUnitKind	= 0;
	siTotalUnitNum	= 0;

	// ���� ������ ���� ����
	cltBlackArmyUnit oneMapUnit[MAX_BLACKARMY_UNITKIND];

	// ���� ���� ���� �ε�
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	bLoadFile = dataloader.LoadDataFromFile( TEXT("ServerInfo\\WarInfo.txt") );
	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("Don't LoadFile: WarInfo.txt"));
		return false;
	}

	SI32 num		= 0;
	SI32 basic		= 0;
	SI32 randlevel	= 0;
	TCHAR charname[128] = { '\0' };

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	charname,		128,
		NDATA_INT32,	&num,			4,		
		NDATA_INT32,	&basic,			4,		
		NDATA_INT32,	&randlevel,		4,		
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if(siUnitKind >= MAX_BLACKARMY_UNITKIND)
			{
				MsgBox("fdj82jfd", "dsif83jf");
				continue;
			}

			// ������ �߰����� �ʴ´�.
			//if( strncmp(charname, "KIND_BLACKBOSS", strlen("KIND_BLACKBOSS")) == 0 )		continue;

			SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode(charname);
			if(kind)
			{
				oneMapUnit[siUnitKind].Init();
				oneMapUnit[siUnitKind].siKind		= kind;
				oneMapUnit[siUnitKind].siNum 		= num;
				oneMapUnit[siUnitKind].siBasicLevel	= basic;
				oneMapUnit[siUnitKind].siRandLevel	= randlevel;

				siUnitKind++;
			}
		}
	}*/

	// ���� ���� �߸���
	if (siUnitKind <= 0)		return false;

//	siTotalUnitNum = 0;

	// ���� ���� �̺�Ʈ ��Ʋ �ʵ� �� ���� �ʱ�ȭ
	SI32 j;
	for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{
		clWarMap[i].Init();
		clWarMap[i].siMapUnitNum = siSettingMonNum;		// ������ ������ ���� ��ġ
		
		for (j = 0; j < MAX_BLACKARMY_UNITKIND; j++)
			clWarMap[i].clBlackArmyUnit[j].Set(&oneMapUnit[j]);

		//siTotalUnitNum += clWarMap[i].siMapUnitNum;

		clWarMap[i].siMapIndex = BLACKARMY_MAP_STARTINDEX + i;		// �ʵ�� ����
		clWarMap[i].siGateUnique = BLACKARMY_GATEUNIQUE_START + i;	// �ʵ�� ����

		clWarMap[i].bWarDoing = true;
		clWarMap[i].bBossMaked = false;
	}

	// ��� �������� ���� ������ �ʱ�ȭ�Ѵ�.
	pclClient->pclCM->DeleteAllWarKill();

	// ���� ���� - ������ 30�� ���� ����ȴ�.
	bWarSwitch			= true;
	uiLeftTime			= 1800000 ;// 30��. 
	dwWarStartClock		= clock;

	// ���� ��û ����
	bApplySwitch = false;

	SI32 id = 0;

	siNextJoinMap = 0;
	// ����ʿ� ���� �й� - 1. ��Ƽ����
	for (i = 0; i < clApplyList.siPersonIndex; i++)
	{
		id = pclClient->pclCM->GetIDFromPersonID(clApplyList.clApplyPersonList[i].clApplyPerson.GetPersonID());
		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			if (pclClient->pclCM->CR[id]->siPartyIndex > 0)
			{
				JoinWarMapWithParty(i);
			}
		}
	}

    siNextJoinMap = 0;
	// ����ʿ� ���� �й� - 2. ��������
	for (i = 0; i < clApplyList.siPersonIndex; i++)
	{
		id = pclClient->pclCM->GetIDFromPersonID(clApplyList.clApplyPersonList[i].clApplyPerson.GetPersonID());
		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			if (pclClient->pclCM->CR[id]->siPartyIndex <= 0)
			{
				JoinWarMap(i);
			}
		}
	}

	// ������� �ο����� ���� ��û�� ó�� �� �α� ����
	for (i = 0; i < clApplyList.siPersonIndex; i++)
	{
		if (clApplyList.clApplyPersonList[i].bWarMapAlloc == false)
		{
			// �������� ���� ��û�ڿ��� �޼��� ������, ������ �����ֵ��� 
			id = pclClient->pclCM->GetIDFromPersonID(clApplyList.clApplyPersonList[i].clApplyPerson.GetPersonID());
			if (id > 0 && pclClient->pclCM->IsValidID(id))
			{
				// ������ �ϰ� �ִ� ����
				((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_BLACKARMY_FAILALLOC, 0, 0, id);

				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				sDBRequest_ChangeMoney clMsg(pclchar->GetID(), pclchar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_BLACKARMYAPPLY, BLACKARMY_APPLY_MONEY );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				pclserver->pclDelMoneyStatistics->Del(DELMONEY_ENTEREVENT, BLACKARMY_APPLY_MONEY);
			}
			else
			{
				// ������ �ϰ� �ִ� ���� ����
				sDBRequest_ChangeMoney clMsg(0, clApplyList.clApplyPersonList[i].clApplyPerson.GetPersonID(), CHANGE_MONEY_REASON_BLACKARMYAPPLY, BLACKARMY_APPLY_MONEY );
				pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				pclserver->pclDelMoneyStatistics->Del(DELMONEY_ENTEREVENT, BLACKARMY_APPLY_MONEY);
			}
		}

		// �� �Ҵ� ������ �α׷� ����
		// param1 = ������ ��ȣ
		// param2 = ������ PersonID
		// param3 = ���� �� �ε���
		// param4 = �� �Ҵ� ����
		cltCharServer* pclchar = pclClient->pclCM->GetCharServer(id);
		((cltServer*)pclClient)->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BLACKARMY, LOGCOMMAND_INDEX_BLACKARMY_APPLYMAP, 0, pclchar, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								i, clApplyList.clApplyPersonList[i].clApplyPerson.GetPersonID(), clApplyList.clApplyPersonList[i].siWarMapUnique, clApplyList.clApplyPersonList[i].bWarMapAlloc, 0,
								NULL, NULL);
	}

/*	for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{
		//--------------------------------
		// ������� ��ġ�Ѵ�. 
		//--------------------------------
		while(clWarMap[i].siMapMadeNum < siSettingMonNum) 
		{
			SI32 j = rand() % siUnitKind;
			SI32 kind = clWarMap[i].clBlackArmyUnit[ j ].siKind;

			// ���������� ���� ���� �ݼ��� �پ��� ��� ����
			if (kind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")) )
				continue; 

			SI32 pos = 0 ; 
			SI32 x, y;
			SI32 tempx, tempy, level;
			SI32 mapindex = 0;

			mapindex = clWarMap[i].siMapIndex;

			tempx = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetXsize();
			tempy = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetYsize();

			// ���͸� ��ġ�� ������ ���� ã�´�. 
			if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false)
			{
				continue;
			}

			level = clWarMap[i].clBlackArmyUnit[ j ].siBasicLevel + rand() % clWarMap[i].clBlackArmyUnit[ j ].siRandLevel;

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
			if(id > 0 && pclClient->pclCM->IsValidID(id))
			{
				clWarMap[i].siMapMadeNum ++;
				clWarMap[i].siMapCurUnitNum++;
				clWarMap[i].clBlackArmyUnit[ j ].siCurNum++;
				clWarMap[i].clBlackArmyUnit[ j ].siMadeNum++;

				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
				
				pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);
			}
		}

		// ù��° �ܰ� ���� ����
		clWarMap[i].siWarStep = 1;
	}*/

	// ������ ���۵Ǹ� ��û�� �ߴ��Ѵ�.

	//--------------------------------------------------
	// ��� Ŭ���̾�Ʈ�鿡�� ������ ���۵Ǿ����� �˸���. 
	//--------------------------------------------------
	for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{
		// �αױ��
		// param1 = �� ��� ��ġ
		// param2 = ���� �� �ε���
		// param3 = ���� �ʿ� �Ҵ�� ĳ���� ��
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BLACKARMY, LOGCOMMAND_INDEX_BLACKARMY_START, 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										i, clWarMap[i].siMapIndex, clWarMap[i].siPersonIndex, 0, 0, NULL, NULL);
		SendWarMsg(WAR_CMD_BREAK, i);
	}

	// ������ �߹� ��Ų��. 
	return true;
}

void cltBlackArmyManager::Action()
{
	SI32 i;
	cltServer* pclserver = (cltServer*)pclClient;

	//------------------------------------------------------------------------------------------
	// MVP ���� ó��
	list<cltMVPEffectList>::iterator itor;
	list<cltMVPEffectList>::iterator deleteitor = listMvpEffect.end();

	for (itor = listMvpEffect.begin(); itor != listMvpEffect.end(); itor++)
	{
		cltMVPEffectList mvpperson = (cltMVPEffectList)*itor;

		if (TABS(pclClient->CurrentClock - mvpperson.dwSelectMVPClock) >= 10000)
		{
			if (mvpperson.siID > 0 && pclClient->pclCM->IsValidID(mvpperson.siID))
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[mvpperson.siID];

				// Ŭ���̾�Ʈ�� MVP ����Ʈ �ѷ��ش�.
				cltGameMsgResponse_BlackArmy_MVPEffect clinfo(pclchar->GetCharUnique());
				cltMsg clMsg(GAMEMSG_RESPONSE_BLACKARMY_MVPEFFECT, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNeighbourMsg(&clMsg, true);
			}

			deleteitor = itor;
		}

	}
	if (deleteitor != listMvpEffect.end())
	{
		listMvpEffect.erase(deleteitor);
	}
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	// ��û & ���� ���� ó��
	if (pclserver->pclMinisterMgr)
	{
		// �����Ǽ�(û���) ������ ���� ����ǽ�
		CMinisterByungzo *pclminister = (CMinisterByungzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_BYUNGZO);
		if (pclminister)
		{
			if (pclminister->m_siWarStartTime > 0)
			{
				_SYSTEMTIME startTime = pclClient->GetTimeFromsMinute( (UI32) pclminister->m_siWarStartTime );

				// ������ �ð����� ��û����
				if (startTime.wYear == pclserver->sTime.wYear &&
					startTime.wMonth == pclserver->sTime.wMonth &&
					startTime.wDay == pclserver->sTime.wDay &&
					startTime.wHour == pclserver->sTime.wHour )
				{
					bApplySwitch = true;
				}

				// ��û������ 59�� �Ŀ� ���ﰳ�� ��.��;;
				if ( bApplySwitch == true && bAutoBlackWarSwitch == false )
				{
					if (startTime.wYear == pclserver->sTime.wYear &&
						startTime.wMonth == pclserver->sTime.wMonth &&
						startTime.wDay == pclserver->sTime.wDay &&
						startTime.wHour == pclserver->sTime.wHour &&
						pclserver->sTime.wMinute == 59 )
					{
						MakeWar(pclserver->CurrentClock);
						bAutoBlackWarSwitch = true;

						return;
					}
				}
			}
		}
	}
	//------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------
	// ������� ó��
	if(bWarSwitch == false)					return ;
		
	//--------------------------------------
	// �����ð����� ������� ��Ȳ�� üũ�Ѵ�. 
	//---------------------------------------
	static DWORD dwLastWarCheckClock = 0;
	SI32 cmd = 0;
	if(TABS(pclClient->CurrentClock - dwLastWarCheckClock ) > 10000)
	{
		dwLastWarCheckClock = pclClient->CurrentClock;

		//---------------------------
		// ���� �ð��� Ȯ���Ѵ�. 
		//---------------------------
		DWORD lastclock = TABS(pclClient->CurrentClock - dwWarStartClock);
		dwWarStartClock	= pclClient->CurrentClock;

		if(uiLeftTime > lastclock)
		{
			uiLeftTime -= lastclock;

			for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
			{
				if (clWarMap[i].siWarcmd != WAR_CMD_WIN)
					clWarMap[i].siWarcmd = WAR_CMD_GOING;
			}
		}
		else
		{
			uiLeftTime = 0;

			AllWarEnd();
			return;
		}

		// ��� ������ �������� Ȯ���Ѵ�.
		bool bAllWarEnd = true;
		for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
		{
			if (clWarMap[i].bWarDoing == true)
				bAllWarEnd = false;
		}

		// ��� ������ �����ٸ�, �������� �Ѵ�.
		if (bAllWarEnd == true)
		{
			AllWarEnd();
			return;
		}

		//siTotalCurUnitNum = 0;
        for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
		{
			if( clWarMap[i].bWarDoing == false )		continue;		// �������� ���� �ƴϴ�.

			if( clWarMap[i].siMapMadeNum >= clWarMap[i].siMapUnitNum )
			{
				// ��� ���簡 �� ����� ����. - ���� ��ȯ
				if( clWarMap[i].siMapCurUnitNum <= MAX_BLACKARMY_MAP_REGEN_CNT &&
					clWarMap[i].bBossMaked == false )
				{
					SI32 unitindex = -1;
					SI32 pos = 0 ;
					SI32 x, y;
					SI32 tempx, tempy, level;
					SI32 mapindex = 0;

					for (SI32 j = 0; j < MAX_BLACKARMY_UNITKIND; j++)
					{
						if (clWarMap[i].clBlackArmyUnit[j].siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")) )
							unitindex = j;
					}

					// ���� ��ȯ
					if (unitindex == -1 || clWarMap[i].clBlackArmyUnit[unitindex].siMadeNum >= clWarMap[i].clBlackArmyUnit[unitindex].siNum)
						continue;

					mapindex = clWarMap[i].siMapIndex;

					tempx = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetXsize();
					tempy = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetYsize();

					// ���͸� ��ġ�� ������ ���� ã�´�. 
					if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")), tempx, tempy, 10, &x, &y, true) == false)
					{
						continue;
					}

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

					// ������ ���� ��ġ�� ���𰡵带 �����.
					SI32 id;
					for (SI32 k = 0; k < BLACKARMY_BOSS_BODYGUARD; k++)
					{
						level = BLACKARMY_INC_LEVEL * 3;
						id = pclserver->SetNPCChar( pclClient->GetUniqueFromHash( TEXT("KIND_DAEGUM")), VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
						if(id > 0 && pclClient->pclCM->IsValidID(id))
						{
							clWarMap[i].siMapMadeNum ++;
							clWarMap[i].siMapCurUnitNum++;
							clWarMap[i].clBlackArmyUnit[ unitindex ].siCurNum++;
							clWarMap[i].clBlackArmyUnit[ unitindex ].siMadeNum++;

							cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
							pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);
						}
					}

					level = clWarMap[i].clBlackArmyUnit[ unitindex ].siBasicLevel + rand() % clWarMap[i].clBlackArmyUnit[ unitindex ].siRandLevel;
					id = pclserver->SetNPCChar(pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")), VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if(id > 0 && pclClient->pclCM->IsValidID(id))
					{
						clWarMap[i].siMapMadeNum ++;
						clWarMap[i].siMapCurUnitNum++;
						clWarMap[i].clBlackArmyUnit[ unitindex ].siCurNum++;
						clWarMap[i].clBlackArmyUnit[ unitindex ].siMadeNum++;

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
						pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);

						// param1 = �� ���� �ε���
						// param2 = �� �ε���
						((cltServer*)pclClient)->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BLACKARMY, LOGCOMMAND_INDEX_BLACKARMY_BOSS, 0, NULL, NULL, 0, 0, 0, 0, 0, pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")), 0, 0, 0, 0,
													i, clWarMap[i].siMapIndex, 0, 0, 0, NULL, NULL);

						// ������ ������.
						clWarMap[i].bBossMaked = true;

						// ���������� �޼��� �ѷ���
						for (j = 0; j < clWarMap[i].siPersonIndex; j++)
						{
							SI32 userid = pclClient->pclCM->GetIDFromPersonID(clWarMap[i].clWarPersonList[j].clApplyPerson.GetPersonID());

							if (userid > 0 && pclClient->pclCM->IsValidID(userid))
							{
								TCHAR* pTitle = GetTxtFromMgr(5857);
								TCHAR* pText = GetTxtFromMgr(5858);

								cltGameMsgResponse_Notice clNotice(pTitle, pText,true);
								cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

								pclClient->pclCM->CR[userid]->SendNetMsg((sPacketHeader*)&clMsg);
							}
						}
					}
				}
			}
			else
			{
				// ���� ���� ���簡 ���Ҵ�.
				if( clWarMap[i].siMapCurUnitNum <= MAX_BLACKARMY_MAP_REGEN_CNT )
				{
					// �ִ� �ʿ� ������ �� �ִ� ��������� �����Ѵ�.
					while( clWarMap[i].siMapCurUnitNum < MAX_BLACKARMY_MAP_ARMY_CNT &&
						   clWarMap[i].siMapMadeNum < clWarMap[i].siMapUnitNum)
					{
						SI32 j = rand() % siUnitKind;
						SI32 kind = clWarMap[i].clBlackArmyUnit[ j ].siKind;

						SI32 pos = 0 ;
						SI32 x, y;
						SI32 tempx, tempy, level;
						SI32 mapindex = 0;

						mapindex = clWarMap[i].siMapIndex;
						tempx = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetXsize();
						tempy = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetYsize();

						// ���͸� ��ġ�� ������ ���� ã�´�. 
						if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false)
						{
							continue;
						}

						level = clWarMap[i].clBlackArmyUnit[ j ].siBasicLevel + rand() % clWarMap[i].clBlackArmyUnit[ j ].siRandLevel;

						cltInnerStatusInfo clinnerstatus;
						clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

						SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
						if(id > 0 && pclClient->pclCM->IsValidID(id))
						{
							clWarMap[i].siMapMadeNum ++;
							clWarMap[i].siMapCurUnitNum++;
							clWarMap[i].clBlackArmyUnit[ j ].siCurNum++;
							clWarMap[i].clBlackArmyUnit[ j ].siMadeNum++;

							cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

							pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);

						}
					}
				}
			}
/*			//--------------------------------
			// ������ ���� ��ġ
			//--------------------------------
			// BlackBoss ���� ����
			if (clWarMap[i].siMapCurUnitNum <= siSettingMonNum / 10 && clWarMap[i].siWarStep > 2)
			{
				SI32 unitindex = -1;
				SI32 pos = 0 ;
				SI32 x, y;
				SI32 tempx, tempy, level;
				SI32 mapindex = 0;

				for (SI32 j = 0; j < MAX_BLACKARMY_UNITKIND; j++)
				{
					if (clWarMap[i].clBlackArmyUnit[j].siKind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")) )
						unitindex = j;
				}

				if (unitindex == -1 || clWarMap[i].clBlackArmyUnit[unitindex].siMadeNum >= clWarMap[i].clBlackArmyUnit[unitindex].siNum)
					continue;

				mapindex = clWarMap[i].siMapIndex;

				tempx = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetXsize();
				tempy = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetYsize();

				// ���͸� ��ġ�� ������ ���� ã�´�. 
				if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")), tempx, tempy, 10, &x, &y, true) == false)
				{
					continue;
				}

				cltInnerStatusInfo clinnerstatus;
				clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

				SI32 id;

				for (SI32 k = 0; k < BLACKARMY_BOSS_BODYGUARD; k++)
				{
					level = BLACKARMY_INC_LEVEL * 3;
					id = pclserver->SetNPCChar( pclClient->GetUniqueFromHash( TEXT("KIND_DAEGUM")), VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if(id > 0 && pclClient->pclCM->IsValidID(id))
					{
						clWarMap[i].siMapMadeNum ++;
						clWarMap[i].siMapCurUnitNum++;
						clWarMap[i].clBlackArmyUnit[ unitindex ].siCurNum++;
						clWarMap[i].clBlackArmyUnit[ unitindex ].siMadeNum++;

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
						pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);
					}
				}

				level = clWarMap[i].clBlackArmyUnit[ unitindex ].siBasicLevel + rand() % clWarMap[i].clBlackArmyUnit[ unitindex ].siRandLevel;

				id = pclserver->SetNPCChar(pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")), VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
				if(id > 0 && pclClient->pclCM->IsValidID(id))
				{
					clWarMap[i].siMapMadeNum ++;
					clWarMap[i].siMapCurUnitNum++;
					clWarMap[i].clBlackArmyUnit[ unitindex ].siCurNum++;
					clWarMap[i].clBlackArmyUnit[ unitindex ].siMadeNum++;

					cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
					pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);

					// ���������� �޼��� �ѷ���
					for (j = 0; j < clWarMap[i].siPersonIndex; j++)
					{
						SI32 userid = pclClient->pclCM->GetIDFromPersonID(clWarMap[i].clWarPersonList[j].clApplyPerson.GetPersonID());

						if (userid > 0 && pclClient->pclCM->IsValidID(userid))
						{

							TCHAR* pTitle = GetTxtFromMgr(5857);
							TCHAR* pText = GetTxtFromMgr(5858);

							cltGameMsgResponse_Notice clNotice(pTitle, pText,true);
							cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

							pclClient->pclCM->CR[userid]->SendNetMsg((sPacketHeader*)&clMsg);
						}
					}
				}
			}
			// siSettingMonNum / 20 ���Ϸ� �� ������ ������
			else if(clWarMap[i].bWarDoing == true && clWarMap[i].siMapCurUnitNum < siSettingMonNum / 20)
			{
				SI32 rejenMonsterCnt;

				// �ι�° �ܰ迡���� siSettingMonNum / 3 * 2 ��ŭ
				if (clWarMap[i].siWarStep == 1)
				{
					rejenMonsterCnt = siSettingMonNum / 3 * 2;
				}
				// ���������� ������ ���
				else
				{
					rejenMonsterCnt = siSettingMonNum;
				}

				pclClient->pclLog->LogPrint("clWarMap[i].siMapMadeNum < rejenMonsterCnt\n", clWarMap[i].siMapMadeNum, rejenMonsterCnt);

				while(clWarMap[i].siMapMadeNum < rejenMonsterCnt)
				{
					SI32 j = rand() % siUnitKind;
					SI32 kind = clWarMap[i].clBlackArmyUnit[ j ].siKind;

					// ���������� ���� ���� 50���� �پ��� ��� ����
					if (kind == pclClient->GetUniqueFromHash(TEXT("KIND_BLACKBOSS")))
						continue;


					SI32 pos = 0 ;
					SI32 x, y;
					SI32 tempx, tempy, level;
					SI32 mapindex = 0;

					mapindex = clWarMap[i].siMapIndex;
					tempx = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetXsize();
					tempy = 1 + rand() % pclClient->pclMapManager->pclMapArray[mapindex]->GetYsize();

					// ���͸� ��ġ�� ������ ���� ã�´�. 
					if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, tempx, tempy, 10, &x, &y, true) == false)
					{
						continue;
					}

					level = clWarMap[i].clBlackArmyUnit[ j ].siBasicLevel + rand() % clWarMap[i].clBlackArmyUnit[ j ].siRandLevel;

					if (clWarMap[i].siWarStep == 1)
					{
						level = BLACKARMY_INC_LEVEL;
					}
					else
					{
						level = BLACKARMY_INC_LEVEL * 2;
					}

					cltInnerStatusInfo clinnerstatus;
					clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

					SI32 id = pclserver->SetNPCChar(kind, VILLAGE_NONE, NULL, level, 0, x, y, mapindex, NPCITEMMODE_NORMAL, &clinnerstatus );
					if(id > 0 && pclClient->pclCM->IsValidID(id))
					{
						clWarMap[i].siMapMadeNum ++;
						clWarMap[i].siMapCurUnitNum++;
						clWarMap[i].clBlackArmyUnit[ j ].siCurNum++;
						clWarMap[i].clBlackArmyUnit[ j ].siMadeNum++;

						cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

						pclchar->clIdentity.MakeBlackArmy(IDENTITY_BLACKARMY, i, clWarMap[i].siMapIndex);

					}

				}
				
				// �������� �޼��� �ѷ���
				if (clWarMap[i].siWarStep <= 2)
				{
					for (SI32 j = 0; j < clWarMap[i].siPersonIndex; j++)
					{
						SI32 userid = pclClient->pclCM->GetIDFromPersonID(clWarMap[i].clWarPersonList[j].clApplyPerson.GetPersonID());

						if (userid > 0 && pclClient->pclCM->IsValidID(userid))
						{

							TCHAR* pTitle = GetTxtFromMgr(4903);
							TCHAR* pText = GetTxtFromMgr(5897);

							cltGameMsgResponse_Notice clNotice(pTitle, pText,true);
							cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

							pclClient->pclCM->CR[userid]->SendNetMsg((sPacketHeader*)&clMsg);

						}

					}
				}

				clWarMap[i].siWarStep++;
				pclClient->pclLog->LogPrint("clWarMap[i].siWarStep = %d\n", i, clWarMap[i].siWarStep);
			}
			siTotalCurUnitNum += clWarMap[i].siMapCurUnitNum;*/
		}

		for (i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
		{
			//������ �¸��ϰų� �ð��� ���������� �������� ����
			if(clWarMap[i].siWarcmd == WAR_CMD_WIN)
			{
				WarEnd(i);
				continue;
			}		

			SendWarMsg(clWarMap[i].siWarcmd, i);
		}		
	}
}

SI16 cltBlackArmyManager::FindWarMapIndex(SI32 mapindex)
{
	SI16 retWarMapIndex = -1;
	for (int i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{
		if (clWarMap[i].siMapIndex == mapindex)
			retWarMapIndex = i;
	}

	return retWarMapIndex;
}

SI16 cltBlackArmyManager::FindWarUnitIndex(SI32 kind)
{
	SI16 retWarUnitIndex = -1;
	for (int i = 0; i < MAX_BLACKARMY_UNITKIND; i++)
	{
		if (clWarMap[0].clBlackArmyUnit[i].siKind == kind)
			retWarUnitIndex = i;
	}

	return retWarUnitIndex;
}

void cltBlackArmyManager::AllWarEnd()
{
	if (bWarSwitch == false) return;

	for (SI32 i = 0; i < MAX_BLACKARMY_MAP_CNT; i++)
	{
		if (clWarMap[i].siWarcmd != WAR_CMD_WIN)
			clWarMap[i].siWarcmd = WAR_CMD_LOSE;

		WarEnd(i);
	}

	clApplyList.InitList();

	bWarSwitch = false;
	bApplySwitch = false;

	if (bAutoBlackWarSwitch)
	{
		cltServer *pclserver = (cltServer*)pclClient;
		CMinisterByungzo *pclminister = (CMinisterByungzo*)pclserver->pclMinisterMgr->GetMinister(MINISTER_BYUNGZO);
		if (pclminister)
		{
			SI32 siID = 0;

			// ���� �������� ���Ｓ��ī��Ʈ ����, ������۽ð� �ʱ�ȭ
			pclminister->m_siSetWarCount++;
			pclminister->m_siWarStartTime = 0;

			sDBRequest_SetByungzoInfo clMsg(siID, pclminister->m_siPersonID,
				pclminister->m_szGreeting, pclminister->m_siVillageWarTime , pclminister->m_siSetTime, pclminister->m_siWarStartTime, 
				pclminister->m_siSetWarCount, pclminister->m_siBlackArmyNum);

			((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
	}

	bAutoBlackWarSwitch = false;
}


void cltBlackArmyManager::WarEnd(SI32 warmapindex)
{
	SI32 i, j;
	SI32 statistic[10];

	if (clWarMap[warmapindex].bWarDoing == false)
		return;

	for(i = 0;i < 10;i++)
	{
		statistic[i] = 0;
	}

	//----------------------------------
	// ���� ������� ���� ��ǥ�� ���ش�.
	//----------------------------------
	
	SI32 index = 0;
	SI32 id;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;
		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
			if (pclchar->clIdentity.siIdentity == IDENTITY_BLACKARMY)
			{
				if (pclchar->clIdentity.clBlackArmyInfo.siWarMapIndex == warmapindex)
				{
					// �� �������� ������ �����Ѵ�. 
					pclClient->pclCM->CR[id]->DeleteInCell();
					if(pclClient->pclCM->DeleteChar( id ) == TRUE)
					{
						index--;
					}
				}
			}
		}
	}

	//----------------------------------
	// ������ ���� ���� MAX_WARRANK_PERSON_PER_MAP ���� ��Ʈ�Ѵ�. 
	//----------------------------------
	SI32 siWarSortRecordChar[MAX_WARRANK_PERSON_PER_MAP];
	ZeroMemory(siWarSortRecordChar, sizeof(SI32) * MAX_WARRANK_PERSON_PER_MAP);
	pclClient->pclCM->SortWarRecord(siWarSortRecordChar, MAX_WARRANK_PERSON_PER_MAP, warmapindex);

	SI32 personid, score;
	TCHAR charname[256];

	cltWarRankInfo warrank;
	warrank.m_warmapindex = warmapindex;

	for (i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
	{
		if (clWarMap[warmapindex].siWarcmd == WAR_CMD_WIN)
		{
			SI32 id = siWarSortRecordChar[i];
			if (id > 0 && pclClient->pclCM->IsValidID(id))
			{
				personid = pclClient->pclCM->CR[id]->pclCI->clBI.clPerson.GetPersonID();
				StringCchCopy(charname, 256, pclClient->pclCM->CR[id]->GetName());
				score = pclClient->pclCM->CR[id]->siWarEnemyKillNumber;
			}
			else
			{
				personid = -1;
				StringCchCopy(charname, 256, TEXT(""));
				score = -1;
			}

		}
		else
		{
			personid = -1;
			StringCchCopy(charname, 256, TEXT(""));
			score = -1;
		}

		warrank.SetRankPerson( i + 1, personid, charname, score);
	}

	sDBRequest_SetWarRankInfo clRankMsg(&warrank);
	pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clRankMsg);

	for (i = 0; i < clWarMap[warmapindex].siPersonIndex; i++)
	{
		id = pclClient->pclCM->GetIDFromPersonID(clWarMap[warmapindex].clWarPersonList[i].clApplyPerson.GetPersonID());
		// ������� �Ŀ��Ը� �����Ѵ�. 
		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			if(pclClient->pclCM->CR[id]->IsValidConnection() == true)
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
				if (pclchar->siBlackArmyWarMapIndex == warmapindex )
				{
					if (clWarMap[warmapindex].siGateUnique > 0 && pclchar->GetMapIndex() == clWarMap[warmapindex].siMapIndex && pclClient->pclCM->IsAlive(id))
					{
						cltGate gate = pclClient->pclMapManager->pclGateManager->clGate[clWarMap[warmapindex].siGateUnique];

						cltWarpInfo clinfo(true, gate.SrcMapIndex, false, false, gate.SrcX, gate.SrcY, -1, -1);
						pclchar->SetWarpInfo(&clinfo,56);
					}

					if (clWarMap[warmapindex].siWarcmd == WAR_CMD_WIN)
					{
						//---------------------------------------------
						// �̱���� ����ڵ鿡�� ����ǰ�� ����
						//---------------------------------------------
						GMONEY money = pclchar->siWarEnemyKillNumber * 50; 
						SI32 personalfameexp = 0;
						SI32 rewarditemnum = 0;
						SI32 rewarditemunique = ITEMUNIQUE(5045); //ITEMUNIQUE(5316);

						SI32 ranking = 0;
						for (j = 0; j < MAX_WARRANK_PERSON_PER_MAP; j++)
						{
							if (pclchar->siID == siWarSortRecordChar[j])
							{
								ranking = j + 1;
							}
						}

						switch (ranking)
						{
						case 1 : 
							personalfameexp = 500;
							rewarditemnum = 50;
							break;
						case 2 : 
							personalfameexp = 400;
							rewarditemnum = 40;
							break;
						case 3 : 
							personalfameexp = 300;
							rewarditemnum = 30;
							break;
						case 4 : 
							personalfameexp = 200;
							rewarditemnum = 20;
							break;
						case 5 : 
							personalfameexp = 100;
							rewarditemnum = 10;
							break;
						}

						// �Ƿι��� �ý��� ����
						rewarditemnum = pclchar->CalcTiredPoint( rewarditemnum );
						money = pclchar->CalcTiredPoint( money );

						if ( rewarditemnum > 0 )
						{
							if ( pclchar->GiveItem(rewarditemunique, rewarditemnum,INPUTITEMPERSON_OPTION_BLACKARMY, 0) == false )
							{
								((cltServer*)pclClient)->SendLog_InventoryFull(pclchar->pclCI->GetPersonID(),rewarditemunique,rewarditemnum,INPUTITEMPERSON_OPTION_WAR);
							}
						}

						// Ŭ���̾�Ʈ���� ���� ����ġ�� �޾����� �뺸�Ѵ�. 
						cltGameMsgResponse_WinWarPrize clinfo(-1, money, 0, 0, 0, rewarditemunique, rewarditemnum);
						cltMsg clMsg(GAMEMSG_RESPONSE_WINWARPRIZE, sizeof(clinfo), (BYTE*)&clinfo);
						pclchar->SendNetMsg((sPacketHeader*)&clMsg);

						sDBRequest_ChangeMoney clMoneyMsg(id, pclClient->pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_WARWIN, money);
						pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMoneyMsg);

						((cltServer*)pclClient)->pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_EVENT, money);

						// ��ȭ�� �����Ѵ�. 
						cltServer* pclserver = (cltServer*)pclClient;
						cltMoney clmoney;
						clmoney.Set(money);
						pclserver->clCurrencyInfo.clInputMoney_War.Increase(&clmoney);

					}
				}
			}
		}
	}

	SendWarMsg(clWarMap[warmapindex].siWarcmd, warmapindex);

	if (clWarMap[warmapindex].siWarcmd == WAR_CMD_WIN)
	{
		TCHAR mvpname[MAX_PLAYER_NAME];
		if (warrank.GetRankPerson(1)->m_score > 0)
		{
			StringCchCopy(mvpname, MAX_PLAYER_NAME, warrank.GetRankPerson(1)->m_szPlayerName);

			TCHAR *MapName = pclClient->pclMapManager->GetMapName(clWarMap[warmapindex].siMapIndex);
			TCHAR *pTitle = GetTxtFromMgr(5884);
			TCHAR *pText = GetTxtFromMgr(5883);
			TCHAR buf[256];
			StringCchPrintf(buf, 256, pText, MapName, MapName, mvpname);

			((cltServer*)pclClient)->PushSpecialNews( buf, pTitle , NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);

			id = pclClient->pclCM->GetIDFromPersonID(warrank.GetRankPerson(1)->m_personid);
			
			if (id > 0 && pclClient->pclCM->IsValidID(id))
			{
				if(pclClient->pclCM->CR[id]->IsValidConnection() == true)
				{
					// ����Ʈ �ѷ��� ĳ��id ����
					cltMVPEffectList mvpperson(pclClient->CurrentClock, id);

                    listMvpEffect.push_back(mvpperson);
				}
			}

		}
	}
	else
	{
		TCHAR *MapName = pclClient->pclMapManager->GetMapName(clWarMap[warmapindex].siMapIndex);
		TCHAR *pTitle = GetTxtFromMgr(5884);
		TCHAR *pText = GetTxtFromMgr(5885);
		TCHAR buf[256];
		StringCchPrintf(buf, 256, pText, MapName);

		((cltServer*)pclClient)->PushSpecialNews( buf, pTitle ,NULL, CAPTIONKIND_NEWS,CAPTIONKIND_NEWSCHAT);

	}

	((cltServer*)pclClient)->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BLACKARMY, LOGCOMMAND_INDEX_BLACKARMY_END, 0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
												0, 0, 0, 0, 0, NULL, NULL);
	clWarMap[warmapindex].bWarDoing = false;
	clWarMap[warmapindex].bBossMaked = false;
	clWarMap[warmapindex].InitList();
}

void cltBlackArmyManager::SendWarMsg(SI32 cmd, SI32 warmapindex)
{

	SI32 index = 0;
	SI32 id;
	SI32 i;
    
	//----------------------------------
	// ������ ���� ���� MAX_WARRANK_PERSON_PER_MAP���� ��Ʈ�Ѵ�. 
	//----------------------------------
	SI32 siWarSortRecordChar[MAX_WARRANK_PERSON_PER_MAP];
	TCHAR szWarSortCharName[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];
	SI32 siWarScore[MAX_WARRANK_PERSON_PER_MAP];

	ZeroMemory(siWarSortRecordChar, sizeof(SI32) * MAX_WARRANK_PERSON_PER_MAP);
	pclClient->pclCM->SortWarRecord(siWarSortRecordChar, MAX_WARRANK_PERSON_PER_MAP, warmapindex);

	for (i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
	{
		id = siWarSortRecordChar[i];

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			const TCHAR *pName = pclClient->pclCM->CR[siWarSortRecordChar[i]]->GetName();

			if (pName != NULL && _tcscmp(pName, "") != 0)
			{
				StringCchPrintf(szWarSortCharName[i], MAX_PLAYER_NAME, TEXT("%s"), pName);
				siWarScore[i] = pclClient->pclCM->CR[siWarSortRecordChar[i]]->siWarEnemyKillNumber;
			}
			else
			{
				StringCchPrintf(szWarSortCharName[i], MAX_PLAYER_NAME, TEXT(""));
				siWarScore[i] = 0;
			}
		}
		else
		{
			StringCchPrintf(szWarSortCharName[i], MAX_PLAYER_NAME, TEXT(""));
			siWarScore[i] = 0;
		}
		
	}

	for (i = 0; i < clWarMap[warmapindex].siPersonIndex; i++)
	{
		id = pclClient->pclCM->GetIDFromPersonID(clWarMap[warmapindex].clWarPersonList[i].clApplyPerson.GetPersonID());

		if (id > 0 && pclClient->pclCM->IsValidID(id))
		{
			// ������� �Ŀ��Ը� �����Ѵ�. 
			if(pclClient->pclCM->CR[id]->IsValidConnection() == true)
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				if (pclchar->siBlackArmyWarMapIndex == warmapindex)
				{
					cltGameMsgResponse_BlackArmy_War clwar(cmd, uiLeftTime, warmapindex, clWarMap[warmapindex].siMapCurUnitNum, (TCHAR**)szWarSortCharName, (SI32*)siWarScore);
					cltMsg clMsg(GAMEMSG_RESPONSE_BLACKARMYWAR, sizeof(clwar), (BYTE*)&clwar);

					pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				}

				if (cmd == WAR_CMD_BREAK)
				{
					// [����] ���λ����� ������ ���¿����� �޽����� ������ ����
					if ( ( true == pclClient->pclCM->CR[id]->bPersonalShopSwitch )
						|| ( true == pclClient->pclCM->CR[id]->bPremiumPersonalShop )
					)
					{
						continue;
					}

					((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_BLACKARMY_MAPPOSITION,  
						clWarMap[warmapindex].siGateUnique, 0, pclchar->GetCharUnique());
				}

			}


		}

	}


}

void cltBlackArmyManager::SetDraw(SI32 cmd)
{
	bDrawSwitch = true;

	switch(cmd)
	{
	case WAR_CMD_BREAK:
		{
			siDrawCmdFont	= 0;
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WARBREAK")), 0, 0);
			((cltClient*)pclClient)->PlayBackgroundMusic(0);
		}
		break;
	case WAR_CMD_WIN:	
		siDrawCmdFont	= 1;	
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WARWIN")), 0, 0);
		break;
	case WAR_CMD_LOSE:	
		siDrawCmdFont	= 2;	
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_WARLOSE")), 0, 0);	
		break;	
	}

	if ( cmd == WAR_CMD_WIN || cmd == WAR_CMD_LOSE )
	{
		((cltClient*)pclClient)->PlayBackgroundMusic(pclClient->pclCM->CR[1]->GetMapIndex());
	}

	dwDrawStartClock = pclClient->CurrentClock;
}

// ��Ȳ�� ǥ���Ѵ�. 
void cltBlackArmyManager::Draw(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry)
{
	if(bDrawSwitch == false)return ;

	if( TABS(pclClient->CurrentClock - dwDrawStartClock) > 10000)
	{
		bDrawSwitch = false;
	}

	TSpr* pspr = pclClient->GetGlobalSpr(GIMG_MAGIC_WAR);
	if(pspr == NULL)return ;

	cltClient* pclclient = (cltClient*)pclClient;

	SI32 yindent = 0 ;

	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		yindent = 30 ;
	}

	if(GP.LockSurface(lpsurface)==TRUE)
	{
		GP.PutSpr( pspr, (pclclient->siClientScreenXsize - pspr->GetXSize())/2 + rx, 
			50  + ry + yindent , siDrawCmdFont);

		GP.UnlockSurface(lpsurface);
	}
}
