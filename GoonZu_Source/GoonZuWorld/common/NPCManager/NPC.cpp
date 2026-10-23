//---------------------------------
// 2003/5/17 ���°�
//---------------------------------

#include <CommonLogic.h>
#include "../../Client/Client.h"
#include "../../Server/TraceBoot.h"

#include "NPC.h"
#include "../../CommonLogic/CommonLogic.h"
#include "Char/CharServer/Char-Server.h"
#include "Char/CharManager/CharManager.h"
#include "../../Common/Order/Order.h"
#include "../../Common/util/util.h"

#include "../../common/PartyMgr/PartyObj.h"
#include "../../common/PartyMgr/PartyMgr.h"
#include "../../common/PartyMgr/PartyBaseInfo.h"

#include "../../Common/Char/MonsterGroupManager/MonsterGroupManager.h"

#include "../../CommonLogic/MsgType-System.h"

#include "../../CommonLogic/MsgRval-Define.h"

extern cltCommonLogic* pclClient;

cltNPCGroup::cltNPCGroup( SI32 id, SI32 maxnpcnumber, SI32 mapindex, SI32 x, SI32 y, SI32 radius, DWORD interval, bool bossGroup )
{
	SI32 i;

	ID					= id;
	MaxNPCNumber		= maxnpcnumber;
	siMapIndex			= mapindex;
	siX					= x;
	siY					= y;
	siRadius			= radius;

	bBossGroup			= bossGroup ;

	if(siRadius == 0)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("f97343"), TEXT("Fd0383"), MB_OK|MB_TOPMOST);
	}
	dwSetIntervalClock	= interval;
	dwLastCheckClock	= 0;
	dwLastNoticeClock	= 0;


	NPCKindNumber		= 0;
	CurrentNPCNumber	= 0;

	for(i = 0;i < MAX_NPC_KIND_PER_GROUP;i++)
	{
		clUnit[i].Init();
	}
}

cltNPCGroup::~cltNPCGroup()
{

}

// �׷쿡 ���� NPC�� ������ �� ���� �߰��Ѵ�.
BOOL cltNPCGroup::AddNPCKind(SI32 kind, SI32 num, SI32 level)
{

	if(NPCKindNumber >= MAX_NPC_KIND_PER_GROUP)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("NPC Kind Exceed"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}

	clUnit[NPCKindNumber].Set(kind, num, level);
	NPCKindNumber++;

	return TRUE;
}


bool cltNPCGroup::AddChar(SI32 id, SI32 kindindex)
{
	if(CurrentNPCNumber >= MAX_NPC_PER_GROUP)return false;

	NPCList[CurrentNPCNumber]		= id;
	NPCKindList[CurrentNPCNumber]	= kindindex;

	CurrentNPCNumber++;

	return true;
}

bool cltNPCGroup::SubChar(SI32 idindex)
{
	if(CurrentNPCNumber == 0)return false;

	NPCList[idindex]		= NPCList[CurrentNPCNumber-1];
	NPCKindList[idindex]	= NPCKindList[CurrentNPCNumber-1];

	CurrentNPCNumber--;

	return true;

}

// NPCGroup�����ȿ� �����ϴ°� ?
bool cltNPCGroup::IsInNPCGroup(SI32 mapindex, SI32 x, SI32 y)
{
	if(siMapIndex == mapindex
		&& x >= (siX - siRadius) && x <= (siX + siRadius)
		&& y >= (siY - siRadius) && y <= (siY + siRadius) )

	{
		return true;
	}

	return false;
}

// Ư�� kind�� �� �׷쿡 �����ϴ°�?
bool cltNPCGroup::IsInKind(SI32 kind)
{
	SI32 i;

	for(i = 0;i < MAX_NPC_KIND_PER_GROUP;i++)
	{
		if(clUnit[i].siKind == kind)return true;
	}

	return false;
}

//-------------------------------------------------------------------
//cltNPCManagerCommon
//-------------------------------------------------------------------
cltNPCManagerCommon::cltNPCManagerCommon(cltCharManager* pclcm)
{
	SI32 i;

	// ĳ���� �Ŵ��� ����. 
	pclCM	= pclcm;

	for(i = 0;i < MAX_NPC_GROUP_NUMBER;i++)
	{
		pclNPCGroup[i]= NULL;
	}


	SetNPCGroup();
}

cltNPCManagerCommon::~cltNPCManagerCommon()
{
	SI32 i;

	for(i = 0;i < MAX_NPC_GROUP_NUMBER;i++)
	{
		if(pclNPCGroup[i])
		{
			delete pclNPCGroup[i];
			pclNPCGroup[i]= NULL;
		}
	}

}

BOOL cltNPCManagerCommon::SetNPCGroup(SI32& group, TCHAR* npcgroupname, bool clonemap, SI32 clonemapindex )
{

	SI32 i;
	TCHAR buffer[1024];
	FILE* fp;

	TCHAR filename[128];

	// Hard bounds check: pclNPCGroup is only MAX_NPC_GROUP_NUMBER entries and this
	// function writes pclNPCGroup[group] below.
	if( group < 0 || group >= MAX_NPC_GROUP_NUMBER )
	{
		TraceBoot("NPCGROUP SetNPCGroup rejected out-of-range group=%d (max=%d) name='%s'",
			group, MAX_NPC_GROUP_NUMBER, npcgroupname ? npcgroupname : TEXT("(null)"));
		return FALSE;
	}

	StringCchPrintf(filename, 128, TEXT("NPCGroup\\%s.txt"), npcgroupname);

	fp = _tfopen(filename, TEXT("rt"));
	if(fp)
	{
		// ù 6���� �����Ѵ�. 
		for(i= 0;i < 6;i++)
		{
			_fgetts(buffer, 100, fp);
		}

		// ������ �о����.
		_fgetts(buffer, 100, fp);


		// ������ �м��ϱ�. 
		SI32 groupnum;
		SI32 maxnum;
		SI32 mapindex;
		SI32 centerx;
		SI32 centery;
		SI32 radius;
		SI32 interval;
		SI32 boss = 0 ;

		_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d"),
			&groupnum, &maxnum, &mapindex, &centerx, &centery, &radius, &interval, &boss);

		bool bossGroup = DATA_TO_BOOL(boss);

		if ( pclNPCGroup[group] == NULL )
		{
			if( clonemap  )
			{
				mapindex = clonemapindex;
			}
			pclNPCGroup[group] = new cltNPCGroup(group, maxnum, mapindex, centerx, centery, radius, interval * 1000, bossGroup );

			// ù 3���� �����Ѵ�. 
			for(i= 0;i < 3;i++)
			{
				_fgetts(buffer, 100, fp);
			}

			i = 0 ;
			SI32 pKind[MAX_MONSTER_KIND_PER_GROUP] = {0,};
			SI32 pLevel[MAX_MONSTER_KIND_PER_GROUP] = {0,};
			SI32 pValue[MAX_MONSTER_KIND_PER_GROUP] = {0,};
			while(_fgetts(buffer, 100, fp) > 0)
			{
				TCHAR npccode[64];
				SI32 num;
				SI32 level;
				SI32 value = 0 ;

				_stscanf(buffer, TEXT("%s %d %d %d"), npccode, &num, &level,&value);

				if(_tcscmp(npccode, TEXT("NULL")) == 0)continue;

				SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode(npccode);
				if(kind >0)
				{
					pclNPCGroup[group]->AddNPCKind(kind, num, level);
				}
				else
				{
					MsgBox(TEXT("fsdf8vjc"), TEXT("fSDfdsf:%s %s"), npcgroupname, npccode);
				}
			}

			// �׷��� ���ʹ� �� ���� �ּ�ó�� KKM 2007 / 08 / 22
			/*	if ( bGroup == true && pclClient->pclMonsterGroupManager )
			{
			if ( pclClient->pclMonsterGroupManager->SetInitFromFile(groupnum,mapindex,centerx,centery,radius,
			pKind,pLevel,pValue) == false )
			{
			MsgBox(TEXT("fsjyvvc"), TEXT("fdf3fdjy:%d %s"), groupnum, filename);
			}
			}*/

			// Ŭ�� ���� �ƴϰ� Ŭ�� ���� ������ ������ NPC������ �����Ѵ�.
			if ( clonemap == false && pclClient->pclMapManager->HaveCloneMap(mapindex) )
			{
				SI32 cloneindex = pclClient->pclMapManager->clUnitMapInfo[mapindex].siCloneIndex;
				for ( SI32 i=0; i < CLONEMAP_NUM; ++i )
				{
					SI32 dstMapIndex = pclClient->pclMapManager->clCloneMapInfo[cloneindex].siDstMapIndex[i];
					if ( dstMapIndex > 0 )
					{
						++group;
						SetNPCGroup( group , npcgroupname, true, dstMapIndex );
					}
				}
			}
		}
		else
		{
			MsgBox(TEXT("fdsfvvc"), TEXT("fdf3fdfd:%d %s"), group, filename);
		}
	}
	else
	{
		MsgBox(TEXT("vc8cjdf"), TEXT("fej8:%s"), filename);
	}

	if(fp)
		fclose(fp);

	return TRUE;
}


// NPC �׷��� �����Ѵ�.
BOOL cltNPCManagerCommon::SetNPCGroup()
{
	SI32 i;
	FILE* fp = NULL;
	SI32 index = 0;
	TCHAR buffer[256];

	//-----------------------------------
	// NPCGroup�� �̸��� Ȯ���Ѵ�. 
	//-----------------------------------

	if( pclClient->siServiceArea == ConstServiceArea_English )
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupListE.txt"), TEXT("rt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupListK.txt"), TEXT("rt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupListT.txt"), TEXT("rt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA )
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupListU.txt"), TEXT("rt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupListC.txt"), TEXT("rt"));
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupListG.txt"), TEXT("rt"));
	}

	else
	{
		fp = _tfopen(TEXT("NPCGroup\\NPCGroupList.txt"), TEXT("rt"));
	}

	if(fp == NULL)
	{
		MsgBox(TEXT("fdsfk"), TEXT("vefd"));
		return FALSE;
	}

	for(i = 0;i < 5;i++)
	{
		_fgetts(buffer, 100, fp);
	}

	SI32 group =  1;
	while(_fgetts(buffer, 100, fp) > 0)
	{
		TCHAR list[128];
		_stscanf(buffer, TEXT("%127s"), list);
		if(_tcscmp(list, TEXT("END")) == 0)break;

		// NPCGroupListE.txt ships 1140 entries but MAX_NPC_GROUP_NUMBER is only 1000.
		// Unbounded "group++" indexed pclNPCGroup[] past the end of the array
		// (NSafeTArray raises EXCEPTION_ARRAY_BOUNDS_EXCEEDED / corrupts adjacent
		// members), killing the server during cltNPCManagerServer construction.
		if( group >= MAX_NPC_GROUP_NUMBER )
		{
			TraceBoot("NPCGROUP list truncated: group=%d reached MAX_NPC_GROUP_NUMBER=%d",
				group, MAX_NPC_GROUP_NUMBER);
			break;
		}

		SetNPCGroup(group, list);
		group++;
	}


	fclose(fp);

	return TRUE;

	//BOOL bLoadFile = FALSE;
	//NDataLoader	dataloader;
	//bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/NPCGroup.txt") );

	//if (!bLoadFile)
	//{
	//       MsgBox(TEXT("Failed Open File"), TEXT("Data/NPCGroup.txt") );
	//	return FALSE;
	//}

	//TCHAR	szGroupName[MAX_PATH] = TEXT("");
	//SI32	siMaxNum = 0;
	//SI32	siMapIndex = 0;
	//SI32	siCenterX = 0;
	//SI32	siCenterY = 0;
	//SI32	siRadius = 0;
	//SI32	siInterval = 0;
	//SI32	siIsGroup = 0;
	//SI32	siIsBoss = 0;
	//TCHAR	szNPCCode[MAX_MONSTER_KIND_PER_GROUP][64];
	//SI32	siMinNum[MAX_MONSTER_KIND_PER_GROUP] = {0,};
	//SI32	siLevel[MAX_MONSTER_KIND_PER_GROUP] = {0,};
	//SI32	siValue[MAX_MONSTER_KIND_PER_GROUP] = {0,};

	//NDataTypeInfo pDataTypeInfo[] =
	//{
	//	NDATA_MBSTRING,	szGroupName,	MAX_PATH,
	//	NDATA_INT32,	&siMaxNum,		4,
	//	NDATA_INT32,	&siMapIndex,	4,
	//	NDATA_INT32,	&siCenterX,		4,
	//	NDATA_INT32,	&siCenterY,		4,
	//	NDATA_INT32,	&siRadius,		4,
	//	NDATA_INT32,	&siInterval,	4,
	//	NDATA_INT32,	&siIsGroup,		4,
	//	NDATA_INT32,	&siIsBoss,		4,
	//	NDATA_MBSTRING,	szNPCCode[0],	64,
	//	NDATA_INT32,	&siMinNum[0],	4,
	//	NDATA_INT32,	&siLevel[0],	4,
	//	NDATA_INT32,	&siValue[0],	4,
	//	NDATA_MBSTRING,	szNPCCode[1],	64,
	//	NDATA_INT32,	&siMinNum[1],	4,
	//	NDATA_INT32,	&siLevel[1],	4,
	//	NDATA_INT32,	&siValue[1],	4,
	//	NDATA_MBSTRING,	szNPCCode[2],	64,
	//	NDATA_INT32,	&siMinNum[2],	4,
	//	NDATA_INT32,	&siLevel[2],	4,
	//	NDATA_INT32,	&siValue[2],	4,
	//	NDATA_MBSTRING,	szNPCCode[3],	64,
	//	NDATA_INT32,	&siMinNum[3],	4,
	//	NDATA_INT32,	&siLevel[3],	4,
	//	NDATA_INT32,	&siValue[3],	4,
	//	NDATA_MBSTRING,	szNPCCode[4],	64,
	//	NDATA_INT32,	&siMinNum[4],	4,
	//	NDATA_INT32,	&siLevel[4],	4,
	//	NDATA_INT32,	&siValue[4],	4,
	//	0, 0, 0
	//};

	//dataloader.RegDataType(pDataTypeInfo );

	//bool	bGroup;
	//bool	bBoss;
	//SI32	id = 1;
	//SI32	group = 1;

	//while( !dataloader.IsEndOfData() ) 
	//{
	//	if (dataloader.ReadData())	
	//	{

	//		bGroup = DATA_TO_BOOL(siIsGroup);
	//		bBoss = DATA_TO_BOOL(siIsBoss);

	//		if ( !szGroupName[0] )
	//			continue;

	//		pclNPCGroup[id] = new cltNPCGroup( id, siMaxNum, siMapIndex, siCenterX, siCenterY, siRadius, siInterval*1000 );

	//		SI32 pKind[MAX_MONSTER_KIND_PER_GROUP] = {0,};
	//		SI32 pLevel[MAX_MONSTER_KIND_PER_GROUP] = {0,};
	//		SI32 pValue[MAX_MONSTER_KIND_PER_GROUP] = {0,};

	//		SI32 k = 0;

	//		for ( SI16 i = 0; i< MAX_MONSTER_KIND_PER_GROUP; i++ )
	//		{
	//			if ( ! szNPCCode[i][0] )
	//			{
	//				continue;
	//			}

	//			SI32 kind = pclClient->pclKindInfoSet->FindKindFromCode( szNPCCode[i] );

	//			if ( kind > 0 )
	//			{
	//				if ( bGroup == true )
	//				{
	//					pKind[k] = kind ;
	//					pLevel[k] = siLevel[i] ;
	//					pValue[k] = siValue[i] ;
	//					siMinNum[i] = 0 ;
	//					++k;
	//				}

	//				pclNPCGroup[id]->AddNPCKind( kind, siMinNum[i], siLevel[i] );
	//			}
	//			else
	//			{
	//				MsgBox(TEXT("NPCGroup Set Error"), TEXT("Fault CharacterKind:%s %s"), szGroupName, szNPCCode[i]);
	//			}
	//		}

	//		if ( bGroup == true && pclClient->pclMonsterGroupManager )
	//		{
	//			if ( pclClient->pclMonsterGroupManager->SetInitFromFile(group, siMapIndex, siCenterX, siCenterY, siRadius, pKind, pLevel, pValue ) == true )
	//			{
	//				++group;
	//			}
	//			else
	//			{
	//				MsgBox(TEXT("fsjyvvc"), TEXT("fdf3fdjy:%s"), szGroupName);
	//			}
	//		}
	//		++id;
	//	}
	//}
	//return TRUE;
}

// NPCGroup�����ȿ� �����ϴ°� ?
bool cltNPCManagerCommon::IsInNPCGroup(SI32 npcgroup, SI32 mapindex, SI32 x, SI32 y)
{
	if(npcgroup <= 0 || npcgroup >= MAX_NPC_GROUP_NUMBER)return false;
	if(pclNPCGroup[npcgroup] == NULL)return false;

	return pclNPCGroup[npcgroup]->IsInNPCGroup(mapindex, x, y);
}

SI32 cltNPCManagerCommon::GetKindLevel(SI32 kind)
{
	SI32 siLevel = 0;
	SI32 i, j;

	for(i = 0;i < MAX_NPC_GROUP_NUMBER; i++)
	{
		if(pclNPCGroup[i] == NULL)continue;

		for(j = 0;j < MAX_NPC_KIND_PER_GROUP;j++)
		{
			if(pclNPCGroup[i]->clUnit[j].siKind == kind)
			{
				siLevel = pclNPCGroup[i]->clUnit[j].siKindLevel;
				return siLevel;
			}
		}
	}

	return siLevel;
}

SI32 cltNPCManagerCommon::GetKindMaxLevel(SI32 kind)
{
	SI32 siMaxLevel = 0;
	SI32 i, j;

	for(i = 0;i < MAX_NPC_GROUP_NUMBER; i++)
	{
		if(pclNPCGroup[i] == NULL)continue;

		for(j = 0;j < MAX_NPC_KIND_PER_GROUP;j++)
		{
			if(pclNPCGroup[i]->clUnit[j].siKind == kind)
			{
				if(siMaxLevel < pclNPCGroup[i]->clUnit[j].siKindLevel)
					siMaxLevel = pclNPCGroup[i]->clUnit[j].siKindLevel;
			}
		}
	}

	return siMaxLevel;
}

//---------------------------------------------------------
// cltNPCManagerClient
//----------------------------------------------------------
cltNPCManagerClient::cltNPCManagerClient(cltCharManager* pclcm) : cltNPCManagerCommon(pclcm)
{


}

cltNPCManagerClient::~cltNPCManagerClient()
{

}

// npc �׷쿡�� Ư�� kind�� ��ġ ���� text�� ���´�.
bool cltNPCManagerClient::GetKindPos(SI32 kind, TCHAR* ptext, SI16 txtSize)
{
	SI32 i;
	TCHAR buffer[256];

	StringCchCopy(ptext, txtSize, TEXT(""));

	bool returnval = false;


	for(i = 0;i < MAX_NPC_GROUP_NUMBER;i++)
	{
		if(pclNPCGroup[i] == NULL)continue;

		// kind�� ���� �׷��� ã�´�. 
		if(pclNPCGroup[i]->IsInKind(kind) == true)
		{
			SI32 mapindex = pclNPCGroup[i]->siMapIndex;

			TCHAR* pmapname = pclClient->pclMapManager->GetMapName(mapindex);

			StringCchPrintf(buffer, 256, TEXT("%s(%d, %d)\r\n"), pmapname, pclNPCGroup[i]->siX, pclNPCGroup[i]->siY);
			StringCchCat(ptext, txtSize, buffer);

			returnval = true;

		}
	}

	return returnval;
}


// npc �׷쿡�� Ư�� kind�� ��ġ ���� text�� ���´�.
bool cltNPCManagerClient::GetKindPos(SI32 kind, cltRegenPos *regenpos, SI32 *siCount)
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 i;
	TCHAR buffer[256];

	bool returnval = false;
	SI32 siAddCount = 0;


	for(i = 0;i < MAX_NPC_GROUP_NUMBER;i++)
	{
		if(pclNPCGroup[i] == NULL)continue;

		// kind�� ���� �׷��� ã�´�. 
		if(pclNPCGroup[i]->IsInKind(kind) == true)
		{
			SI32 mapindex = pclNPCGroup[i]->siMapIndex;
			TCHAR* pmapname = pclClient->pclMapManager->GetMapName(mapindex);

			StringCchPrintf(buffer, 256, TEXT("%s"),			pmapname);
			StringCchCopy(regenpos[siAddCount].szMapName, 50, buffer);

			StringCchPrintf(buffer, 256, TEXT("(%3d, %3d)"),	pclNPCGroup[i]->siX, pclNPCGroup[i]->siY);
			StringCchCopy(regenpos[siAddCount].szMapPos, 50, buffer);

			pclclient->GetNearMapInfo(mapindex,pclNPCGroup[i]->siX,pclNPCGroup[i]->siY,regenpos[siAddCount].szNearMapInfo, 50);

			/*			SI32 siVillageUnique;
			TCHAR strdir[30] = TEXT("") ;

			// �߿����� 
			if(mapindex == 0)
			{
			siVillageUnique = pclClient->pclVillageManager->FindNearVillage(pclNPCGroup[i]->siX, pclNPCGroup[i]->siY);
			SI32 x = pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->PosX;
			SI32 y = pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->PosY;

			GetDirectionStr(x, y, pclNPCGroup[i]->siX, pclNPCGroup[i]->siY, strdir);
			pmapname = pclClient->pclMapManager->GetMapName(siVillageUnique);
			sprintf(buffer, TEXT("%s %s"),		pmapname, strdir);
			_tcscpy(regenpos[siAddCount].szNearMapInfo, buffer);
			}
			else
			{
			_tcscpy(strdir, TEXT(""));
			if(_tcsstr(pmapname,      "�������"))		{	_tcscpy(strdir, TEXT("���ѻ꼺 ���"));	}
			else if(_tcsstr(pmapname, TEXT("���ϰ���")))		{	_tcscpy(strdir, TEXT("���ѻ꼺 �Ϻ�"));	}
			else if(_tcsstr(pmapname, TEXT("���üұ�")))		{	_tcscpy(strdir, TEXT("���ൿ�� ����"));	}
			else if(_tcsstr(pmapname, TEXT("ȭ�漺")))			{	_tcscpy(strdir, TEXT("�����ұ� ����"));	}
			else if(_tcsstr(pmapname, TEXT("������")))			{	_tcscpy(strdir, TEXT("������ ����"));	}
			else if(_tcsstr(pmapname, TEXT("�ʺ��ڻ糶��")))	{	_tcscpy(strdir, TEXT("�Ѿ� ������ "));		}

			// ���� �ٲܶ� �� Ȯ���Ҳ� 
			else if(_tcsstr(pmapname, TEXT("���ѻ꼺")))		{	_tcscpy(strdir, TEXT("���� ����"));		}
			else if(_tcsstr(pmapname, TEXT("���ൿ��")))		{	_tcscpy(strdir, TEXT("ȫõ ����"));		}
			else if(_tcsstr(pmapname, TEXT("�����ұ�")))		{	_tcscpy(strdir, TEXT("��õ ����"));		}
			else if(_tcsstr(pmapname, TEXT("����Ǯ��")))		{	_tcscpy(strdir, TEXT("�뱸 ����"));		}
			else if(_tcsstr(pmapname, TEXT("������")))		{	_tcscpy(strdir, TEXT("�Ծ� �ϼ���"));		}
			else if(_tcsstr(pmapname, TEXT("��Ȳ��")))			{	_tcscpy(strdir, TEXT("���� ������"));		}

			else if(_tcsstr(pmapname, TEXT("�������")))		{	_tcscpy(strdir, TEXT("�뿵 ����"));		}
			else if(_tcsstr(pmapname, TEXT("�Ͱ����")))		{	_tcscpy(strdir, TEXT("��ȵ���õ ����"));		}
			else if(_tcsstr(pmapname, TEXT("���ϱ�")))			{	_tcscpy(strdir, TEXT("���� ������"));		}
			else if(_tcsstr(pmapname, TEXT("���")))			{	_tcscpy(strdir, TEXT("���� �ϼ���"));		}
			else if(_tcsstr(pmapname, TEXT("��Ÿ�۾���")))		{	_tcscpy(strdir, TEXT("�߰��� ����"));		}

			else if(_tcsstr(pmapname, TEXT("�Ѱ�����")))		{	_tcscpy(strdir, TEXT("��õ ����"));		}
			else if(_tcsstr(pmapname, TEXT("��ȸ����")))		{	_tcscpy(strdir, TEXT("�ȵ� ����"));		}
			else if(_tcsstr(pmapname, TEXT("������")))			{	_tcscpy(strdir, TEXT("�߰��� ����"));		}
			else if(_tcsstr(pmapname, TEXT("��λ�")))			{	_tcscpy(strdir, TEXT("���� �ϵ���,����� �ϼ���"));		}

			if(_tcslen(strdir))	sprintf(buffer, TEXT("%s"),		strdir);
			else				sprintf(buffer, TEXT("%s"),		pmapname);
			_tcscpy(regenpos[siAddCount].szNearMapInfo, buffer);
			}*/

			returnval = true;
			siAddCount++;

		}
	}

	*siCount = siAddCount;
	return returnval;
}

/*bool cltNPCManagerClient::GetDirectionStr(SI32 sx, SI32 sy, SI32 dx, SI32 dy, TCHAR *szText)
{
SI32  rsx, rsy, rdx, rdy;
rsx = sx*2-sy*2;	rsy = sy*1+sx*1;
rdx = dx*2-dy*2;	rdy = dy*1+dx*1;

switch(DecideDirection8(rsx, rsy, rdx, rdy))
{
case SOUTH|EAST:	_tcscpy(szText, TEXT("������"));	break;
case SOUTH:			_tcscpy(szText, TEXT("����"));		break;
case SOUTH|WEST:	_tcscpy(szText, TEXT("������"));	break;
case WEST:			_tcscpy(szText, TEXT("����"));		break;
case WEST|NORTH:	_tcscpy(szText, TEXT("�ϼ���"));	break;
case NORTH:			_tcscpy(szText, TEXT("����"));		break;
case NORTH|EAST:	_tcscpy(szText, TEXT("�ϵ���"));	break;
case EAST:			_tcscpy(szText, TEXT("����"));		break;
default:			_tcscpy(szText, TEXT("��ó"));		break;
}

return true;
}*/

// npc �׷쿡�� Ư�� ��(index)�� �ִ� ���͵��� kind�� �����´�.
bool cltNPCManagerClient::GetKindMapInx(SI32 mapindex, SI32 siKind[], SI32 siKindLevel[], SI32 siMaxSize)
{
	SI32 kindnumber = 0 ;

	for(SI32 i = 0 ; i < MAX_NPC_GROUP_NUMBER ; i++ )
	{
		if(pclNPCGroup[i] == NULL)continue;

		if ( pclNPCGroup[i]->siMapIndex == mapindex )
		{
			for ( SI32 j = 0 ; j < MAX_NPC_KIND_PER_GROUP ; j ++ )
			{
				if ( pclNPCGroup[i]->clUnit[j].siKind > 0 )
				{
					if ( FindKindData(siKind, siMaxSize, pclNPCGroup[i]->clUnit[j].siKind ) )	continue ;
					//if ( kindnumber >= MAX_NPC_KIND_PER_GROUP ) continue ;
					if( kindnumber >=  siMaxSize)											continue;
					siKind[kindnumber] = pclNPCGroup[i]->clUnit[j].siKind ;
					siKindLevel[kindnumber] = pclNPCGroup[i]->clUnit[j].siKindLevel ;
					kindnumber++ ;
					//if ( ConstServiceArea_China == pclClient->siServiceArea )
					//{
					//	siKindLevel[kindnumber] = pclClient->pclKindInfoSet->pclKI[pclNPCGroup[i]->clUnit[j].siKind]->clBA.GetBattleLevel();
					//}
					//else
					//{

					//}
				}
			}
		}
	}
	return true;
}

bool cltNPCManagerClient::FindKindData(SI32 siKind[], SI32 siCount, SI32 npckind)
{
	if ( npckind <= 0 ) return true ;

	//for ( SI32 i = 0 ; i < MAX_NPC_KIND_PER_GROUP ; i ++ )
	for ( SI32 i = 0 ; i < siCount ; i ++ )
	{
		if ( siKind[i] == npckind )
			return true ;
	}
	return false ;
}

//---------------------------------------------------------
// cltNPCManagerServer
//----------------------------------------------------------
cltNPCManagerServer::cltNPCManagerServer(cltCharManager* pclcm) : cltNPCManagerCommon(pclcm)
{


}

cltNPCManagerServer::~cltNPCManagerServer()
{
}

bool cltNPCManagerServer::FindKindData(SI32 siKind[], SI32 siCount, SI32 npckind)
{
	if ( npckind <= 0 ) return true ;

	//for ( SI32 i = 0 ; i < MAX_NPC_KIND_PER_GROUP ; i ++ )
	for ( SI32 i = 0 ; i < siCount ; i ++ )
	{
		if ( siKind[i] == npckind )
			return true ;
	}
	return false ;
}

bool cltNPCManagerServer::GetKindLevel(SI32 sLevel, SI32 eLevel, SI32 siKind[], SI32 maxsize)
{
	SI32 kindnumber = 0 ;

	for(SI32 i = 0 ; i < MAX_NPC_GROUP_NUMBER ; i++ )
	{
		if(pclNPCGroup[i] == NULL)	continue;

		for ( SI32 j = 0 ; j < MAX_NPC_KIND_PER_GROUP ; j ++ )
		{
			if ( sLevel <= pclNPCGroup[i]->clUnit[j].siKindLevel && eLevel >= pclNPCGroup[i]->clUnit[j].siKindLevel )
			{
				if ( FindKindData(siKind, maxsize, pclNPCGroup[i]->clUnit[j].siKind ) )		continue ;
				if ( kindnumber >= maxsize )												continue ;
				siKind[kindnumber] = pclNPCGroup[i]->clUnit[j].siKind ;

				if (pclNPCGroup[i]->clUnit[j].siKindLevel == 1)
				{
					int a= 0;
				}
				kindnumber++ ;
			}
		}
	}
	return true;
}

// NPC �׷� ������ ���� ĳ���͸� �߰��Ѵ�.
BOOL cltNPCManagerServer::SetChar()
{
	SI32 i, j, k, z;
	SI32 id;

	if(pclClient->GetFrame() % 20)return FALSE;


	for(i = 0; i < MAX_NPC_GROUP_NUMBER;i++)
	{
		if(pclNPCGroup[i] == NULL)
			continue;

		// �ð� �������� ������ �׷��� ���, 
		SI32 maxnumber		= pclNPCGroup[i]->MaxNPCNumber;
		SI32 npckindnumber	= pclNPCGroup[i]->NPCKindNumber;
		SI32 mapindex		= pclNPCGroup[i]->siMapIndex;

		// 30 NPCGroup files reference map indices 400..465, but pclMapArray only has
		// MAX_MAP_NUMBER (400) entries. Indexing it out of range makes NSafeTArray
		// raise EXCEPTION_ARRAY_BOUNDS_EXCEEDED and hand back a NULL reference, so
		// the caller's ->FindEmptyAreaForCharLocate() faults inside cltMapCommon
		// (0xC0000005 in cltMapCommon::CalcCollInfo). Skip those groups entirely.
		if( mapindex < 0 || mapindex >= MAX_MAP_NUMBER )
		{
			// This runs every tick for every group, so throttle the trace to keep
			// TraceBoot.log from exploding (it reached 337k lines in minutes).
			static SI32 s_iBadMapGroupTraceCount = 0;
			if( s_iBadMapGroupTraceCount < 40 )
			{
				s_iBadMapGroupTraceCount++;
				TraceBoot("NPCGROUP group=%d skipped: out-of-range mapindex=%d (MAX_MAP_NUMBER=%d)",
					i, mapindex, MAX_MAP_NUMBER);
			}
			continue;
		}

		if ( pclClient->pclMapManager->IsInstanceMap(mapindex) == false )
		{
			if(pclNPCGroup[i]->dwSetIntervalClock && (DWORD)TABS(pclClient->CurrentClock - pclNPCGroup[i]->dwLastCheckClock) < pclNPCGroup[i]->dwSetIntervalClock)
			{
				if( pclNPCGroup[i]->bBossGroup )
				{
					DWORD leftsecond = pclNPCGroup[i]->dwSetIntervalClock - TABS(pclClient->CurrentClock - pclNPCGroup[i]->dwLastCheckClock);

					// �ش� ������ �ִ� ����ڵ鿡�� �������� ���� �ð��� �˷��ش�. 
					if(TABS(pclClient->CurrentClock - pclNPCGroup[i]->dwLastNoticeClock) >= 60000)
					{
						pclNPCGroup[i]->dwLastNoticeClock = pclClient->CurrentClock;

						cltGameMsgResponse_BossAppear clinfo(pclNPCGroup[i]->clUnit[0].siKind, leftsecond / 1000);
						cltMsg clMsg(GAMEMSG_RESPONSE_BOSSAPPEAR, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->pclCM->SendAllPersonInMapMsg((sPacketHeader*)&clMsg, pclNPCGroup[i]->siMapIndex);
					}
				}

				continue;
			}

			pclNPCGroup[i]->dwLastCheckClock = pclClient->CurrentClock;

			//---------------------------------
			// NPC�׷��� ���� ������ �ʱ�ȭ�Ѵ�.
			//---------------------------------
			for(j = 0;j < npckindnumber;j++)
			{
				pclNPCGroup[i]->clUnit[j].siKindNum = 0;
			}

			//--------------------------------
			// NPC�׷��� ������ �� �����Ѵ�. 
			//--------------------------------

			// NPC�� ��ȿȭ�� �͵��� �����Ѵ�. 
			for(j = 0;j < pclNPCGroup[i]->CurrentNPCNumber;j++)
			{
				id = pclNPCGroup[i]->NPCList[j];
				// ĳ���Ͱ� �׾��ų� ���� NPC�׷��� �ƴѰ�쿡. 
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
				if(pclClient->pclCM->IsAlive(id) == FALSE || pclchar->GetNPCGroup() != i)
				{
					pclNPCGroup[i]->SubChar(j);
				}
				else
				{
					//-------------------------------------
					// NPC�׷쿡 �߰��Ѵ�.
					//-------------------------------------
					for(k =0;k < npckindnumber;k++)
					{
						if(pclNPCGroup[i]->clUnit[k].siKind == pclchar->GetKind())
						{
							pclNPCGroup[i]->clUnit[k].siKindNum++;
						}
					}
				}

			}

			SI32 currentnumber	= pclNPCGroup[i]->CurrentNPCNumber;

			// ������ NPC���� �ִ� ������ ���ų� ������ NPC�� �߰����� �ʴ´�.
			if(currentnumber >= maxnumber)continue;

			// �ѹ��� ������ ���͵�
			if( pclNPCGroup[i]->dwSetIntervalClock > 0 && false == pclNPCGroup[i]->bBossGroup )
			{
				for(j = 0; j < npckindnumber;j++)
				{
					SI32 kind		= pclNPCGroup[i]->clUnit[j].siKind;
					SI32 kindmaxnum	= pclNPCGroup[i]->clUnit[j].siKindMaxNum;
					SI32 kindnumber	= pclNPCGroup[i]->clUnit[j].siKindNum;
					SI32 level		= pclNPCGroup[i]->clUnit[j].siKindLevel;

					// ĳ���� ���ڰ� �������� ������ �������� �ʴ´�. 
					for( z = kindnumber; z < kindmaxnum; ++z )
					{
						//---------------------------
						// ĳ���͸� �����. 
						//---------------------------
						SI32 x = pclNPCGroup[i]->siX - pclNPCGroup[i]->siRadius/2 + (rand() % pclNPCGroup[i]->siRadius);
						SI32 y = pclNPCGroup[i]->siY - pclNPCGroup[i]->siRadius/2 + (rand() % pclNPCGroup[i]->siRadius);

						SI32 dx, dy;

					#ifdef _DEBUG
						if( NULL == pclClient->pclMapManager->pclMapArray[mapindex] )
						{
							TCHAR ErrorBuffer[128] = "";
							StringCchPrintf( ErrorBuffer, sizeof(ErrorBuffer), "Npc Group [ %d ]�� ������ ����!!", mapindex );
							MessageBox( NULL, ErrorBuffer, "����!", 0 );
							continue;
						}
					#else
						if( NULL == pclClient->pclMapManager->pclMapArray[mapindex] ) continue;;
					#endif
						

						if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, x, y, 5, &dx, &dy, false) == false)continue;

						// ĳ���͸� �����Ѵ�. 
						cltOrderDataInfo clOrder;
						clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);

						cltSimpleRank clrank;
						clrank.Init();

						cltIdentity			clIdentity;
						cltInnerStatusInfo	clInnerStatus;
						clInnerStatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);	// ��� ����ڿ��� �������̴�. 

						SI32 charunique;
						SI32 mode = 0;
						charunique = cltCharServer::FindEmptyCharUnique(mode);

						SI32 childcharuique = 0;
						SI32 cursummonindex	= 0;

						cltBufInfo clBufInfo;
						clBufInfo.Init();
						cltMakeCharInfo clMakeCharInfo( kind, 
														charunique, 
														level, 
														0, 
														dx, 
														dy, 
														mapindex, 
														childcharuique, 
														cursummonindex, 
														0, 
														0, 
														0, 
														0, 
														0, 
														0,
														0, 
														0,
														1,
														NULL,
														0, 
														0,
														0,
														0,
														0, 
														0,
														0,
														0,
														&clrank,
														0,
														0,
														&clOrder,
														&clIdentity,
														&clInnerStatus,
														0,
														0,
														0,
														false,
														0,
														NULL,
														NULL,
														false, 
														0, 
														false, 
														false, 
														false, 
														false, 
														0, 
														0,
														NULL, 
														false
														,NULL
														,0
														,0
														,0
														,0
														,0
														,0
														,0
														,0
														,0
														,0
														,false
														,&clBufInfo );

						id = pclCM->pclIDManager->GetID(mode);


						id = pclClient->SetChar(id, &clMakeCharInfo);
						if(id)
						{
							if(pclNPCGroup[i]->AddChar(id, j) == TRUE)
							{
								pclCM->CR[id]->SetNPCGroup(i);
							}

							//-------------------------------
							// �� ĳ���Ϳ��� �������� �ش�. 
							//-------------------------------
							// KindInfo�� ������ ���� �������� �����. 
							cltItem clItem;

							if(pclClient->pclItemManager->MakeRandItem(level, 0, &pclClient->pclKindInfoSet->pclKI[kind]->clItemPbtSet, &clItem) == true)
							{
								// ������� �������� ĳ���Ϳ��� �ش�.
								pclCM->CR[id]->AddItemToInventory(&clItem);
							}
						}
					}
				}
			}
			else
			{
				for(j = 0; j < npckindnumber;j++)
				{
					SI32 kind		= pclNPCGroup[i]->clUnit[j].siKind;
					SI32 kindmaxnum	= pclNPCGroup[i]->clUnit[j].siKindMaxNum;
					SI32 kindnumber	= pclNPCGroup[i]->clUnit[j].siKindNum;
					SI32 level		= pclNPCGroup[i]->clUnit[j].siKindLevel;

					// ĳ���� ���ڰ� �������� ������ �������� �ʴ´�. 
					if(kindnumber >= kindmaxnum)continue;

					//---------------------------
					// ĳ���͸� �����. 
					//---------------------------

					// ĳ���͸� ������ ������ ��ġ�� ã�´�. 
					SI32 x = pclNPCGroup[i]->siX - pclNPCGroup[i]->siRadius/2 + (rand() % pclNPCGroup[i]->siRadius);
					SI32 y = pclNPCGroup[i]->siY - pclNPCGroup[i]->siRadius/2 + (rand() % pclNPCGroup[i]->siRadius);

					SI32 dx, dy;
					if(pclClient->pclMapManager->pclMapArray[mapindex]->FindEmptyAreaForCharLocate(kind, x, y, 5, &dx, &dy, false) == false)continue;


					// ĳ���͸� �����Ѵ�. 
					cltOrderDataInfo clOrder;
					clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);

					cltSimpleRank clrank;
					clrank.Init();

					cltIdentity			clIdentity;
					cltInnerStatusInfo	clInnerStatus;
					clInnerStatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);	// ��� ����ڿ��� �������̴�. 

					SI32 charunique;
					SI32 mode = 0;
					charunique = cltCharServer::FindEmptyCharUnique(mode);

					SI32 childcharuique = 0;
					SI32 cursummonindex	= 0;

					cltBufInfo clBufInfo;
					clBufInfo.Init();
					cltMakeCharInfo clMakeCharInfo(kind, charunique, level, 0, dx, dy, mapindex, 
													childcharuique, 
													cursummonindex, 
													0, 0, 0, 0, 0, 0, 0, 0, 
													1, NULL, 0, 0, 0, 0, 0, 
													0, 0, 0, &clrank, 0, 0,
													&clOrder, &clIdentity, &clInnerStatus, 
													0, 0, 0, false, 0, NULL, NULL, 
													false, 0, false, false, false,	
													false, 0, 0, NULL, false,NULL
													,0,0,0,0,0,0,0,0,0,0,false, &clBufInfo
													);

					id = pclCM->pclIDManager->GetID(mode);


					id = pclClient->SetChar(id, &clMakeCharInfo);
					if(id)
					{
						if(pclNPCGroup[i]->AddChar(id, j) == TRUE)
						{
							pclCM->CR[id]->SetNPCGroup(i);
						}

						//-------------------------------
						// �� ĳ���Ϳ��� �������� �ش�. 
						//-------------------------------
						// KindInfo�� ������ ���� �������� �����. 
						cltItem clItem;

						if(pclClient->pclItemManager->MakeRandItem(level, 0, &pclClient->pclKindInfoSet->pclKI[kind]->clItemPbtSet, &clItem) == true)
						{
							// ������� �������� ĳ���Ϳ��� �ش�.
							pclCM->CR[id]->AddItemToInventory(&clItem);
						}
					}
				}
			}
		}
		else
		{
			SetInstanceChar(mapindex) ;
		}
	}

	return TRUE;
}

//[�߰� : Ȳ���� 2007. 10. 31 ������ ������ �̸� ������ ������ ���� �̸��� ã������ �߰�.]
// npc �׷쿡�� Ư�� ��(index)�� �ִ� ���͵��� kind�� �����´�.
bool cltNPCManagerServer::GetKindMapInx(SI32 mapindex, SI32 siKind[], SI32 siKindLevel[], SI32 siMaxSize)
{
	SI32 kindnumber = 0 ;

	for(SI32 i = 0 ; i < MAX_NPC_GROUP_NUMBER ; i++ )
	{
		if(pclNPCGroup[i] == NULL)continue;

		if ( pclNPCGroup[i]->siMapIndex == mapindex )
		{
			for ( SI32 j = 0 ; j < MAX_NPC_KIND_PER_GROUP ; j ++ )
			{
				if ( pclNPCGroup[i]->clUnit[j].siKind > 0 )
				{
					if ( FindKindData(siKind, siMaxSize, pclNPCGroup[i]->clUnit[j].siKind ) )	continue ;
					//if ( kindnumber >= MAX_NPC_KIND_PER_GROUP ) continue ;
					if( kindnumber >=  siMaxSize)											continue;
					siKind[kindnumber] = pclNPCGroup[i]->clUnit[j].siKind ;
					siKindLevel[kindnumber] = pclNPCGroup[i]->clUnit[j].siKindLevel ;
					kindnumber++ ;
					//if ( ConstServiceArea_China == pclClient->siServiceArea )
					//{
					//	siKindLevel[kindnumber] = pclClient->pclKindInfoSet->pclKI[pclNPCGroup[i]->clUnit[j].siKind]->clBA.GetBattleLevel();
					//}
					//else
					//{

					//}
				}
			}
		}
	}
	return true;
}
