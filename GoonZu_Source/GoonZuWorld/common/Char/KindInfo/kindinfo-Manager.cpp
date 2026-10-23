//---------------------------------
// 2004/4/17 ���°�
//---------------------------------

#include "../../../CommonLogic/CommonLogic.h"
#include "../../../common/CommonHeader.h"
#include "../../../Client/Client.h"
#include "../../../Server/TraceBoot.h"

#include "../../Client/Music/Music.h"

#include "../../Common/NPCManager/NPC.h"

#include "KindInfo.h"
#include "../../../CommonLogic/AttackType/AttackType.h"
#include "../../../CommonLogic/MagicMgr/MagicMgr.h"
#include "../../../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"
#include "../../../CommonLogic/CityHall/CityHall.h"
#include "../../../common/Skill/Skill-Manager.h"
#include "Char/CharCommon/Char-Common.h"
#include "Char/CharClient/Char-Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"


//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "../CharManager/CharManager.h"

#include "NHashTableString.h"
#include "NDataloader.h"

extern cltCommonLogic* pclClient;
extern TCHAR tokens[ MAX_TOKEN ][ MAX_STRING_LENGTH ];

//----------------------------------------------
// cltKindItemProbabilitySet
//----------------------------------------------
void cltKindItemProbabilitySet::Init()
{
	SI32 i;

	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		clKindItemProbability[i].Set(0, 0, 0,1);
	}
}

// �������� �߰��Ѵ�. 
//prbability : 1/10000 Ȯ��. 
BOOL cltKindItemProbabilitySet::Add(SI32 siitemunique, SI32 prbability, SI32 itemnum,SI32 minnum)
{
	SI32 i;

	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		if(clKindItemProbability[i].siItemUnique == 0)
		{
			clKindItemProbability[i].Set(siitemunique, prbability, itemnum,minnum) ;
			return TRUE;
		}
	}
	return FALSE;
}

// �����ϰ� �ϳ��� �������� ����ũ ���� �����Ѵ�. �ƹ��͵� �������� ���� ���� �ִ�.
bool cltKindItemProbabilitySet::GetRandDropItem(SI32 level, SI32 AddRate, SI32* punique, SI32* pitemnum)
{
	SI32 i, sum;

	sum	= 0;
	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		if(clKindItemProbability[i].siItemUnique)
		{
			sum +=  (clKindItemProbability[i].siProbability) 
				   +(clKindItemProbability[i].siProbability)*AddRate/100;
		}
	}

	SI32 randdata	= 0;
	
	if(sum >= 10000){ randdata = 	rand() %  sum; }
	else			{ randdata = 	rand() %  10000; }

	sum	= 0;
	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		if(clKindItemProbability[i].siItemUnique)
		{
			sum +=  (clKindItemProbability[i].siProbability) 
				   +(clKindItemProbability[i].siProbability)*AddRate/100;;
			
			if(randdata < sum)
			{
				*punique	=  clKindItemProbability[i].siItemUnique;
				*pitemnum	=  clKindItemProbability[i].siItemMinNum
					+ rand() % ( clKindItemProbability[i].siItemNum - clKindItemProbability[i].siItemMinNum + 1 ) ;

				return true;
			}
		}
	}

	return  false;
}

// �������� ����ũ�� �ٰŷ� �� ĳ���Ͱ� �� ������ �ִ� �Ͱ� ���õ� ������ ã�´�. 
cltKindItemProbability* cltKindItemProbabilitySet::FindItem(SI32 itemunique)
{
	SI32 i;

	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		if(clKindItemProbability[i].siItemUnique == itemunique)
		{
			return &clKindItemProbability[i];
		}
	}

	return NULL;

}

//---------------------------------------------
// cltKindInfoSet
//---------------------------------------------
cltKindInfoSet::cltKindInfoSet()
{
	// ��ȯ�� ���̺� �и� ���� �迭 NULL���� �ʱ�ȭ.
#ifdef _SAFE_MEMORY
	pclSummonKI.ZeroMem();
#else
	ZeroMemory(pclSummonKI, sizeof(pclSummonKI));
#endif

	SI32 i;
/*	cltKIDrawInfo				clKIDrawInfo;
	cltKIEffectInfo				clKIEffectInfo;
	cltKindHireCondition		clHireCond;
	cltBossItemInfo				clbossitem;
	cltAtkTypeTolerance			clAtkTypeTolerance;*/

	// ĳ���� ����
	NDataLoader dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("data/kindinfo_Unique.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("data/kindinfo_Unique.txt") );							
	}

	if (!bLoadFile)
		MsgBox(TEXT("Error"),TEXT("Not Found kindinfo_Unique.txt"));

	TraceBoot("KINDUNIQUE bLoadFile=%d hashTable=%p isWhereTextEnc=%d",
		(int)bLoadFile, (void*)pclClient->pclHashTableString,
		(int)pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ));

	TCHAR	szKindInfo[128] = TEXT("");
	SI32	siKindNum	= 0;

	NDataTypeInfo pKindInfo[]={ NDATA_INT32,	&siKindNum,	  4,
								NDATA_MBSTRING,	szKindInfo,	128,
								0,				0,			  0	};

	dataloader.RegDataType(pKindInfo);

	SI32 iKindRowsRead = 0;
	while (!dataloader.IsEndOfData())
	{
		siKindNum = 0;

		if (dataloader.ReadData())
		{
			iKindRowsRead++;
			if( MAX_KIND_NUMBER <= siKindNum || MAX_KINDINFO_NUMBER <= siKindNum )
			{
				MessageBox( NULL, "Kind Unique Over => MAX_KIND_NUMBER, MAX_KINDINFO_NUMBER Over", "Error", NULL );
			}

			pclClient->pclHashTableString->Insert( szKindInfo, siKindNum );

			if ( iKindRowsRead <= 3 || _tcscmp(szKindInfo, TEXT("KIND_SYSTEM")) == 0 )
			{
				TraceBoot("KINDUNIQUE row#%d num=%d name='%s'", iKindRowsRead, siKindNum, szKindInfo);
			}
		}
	}
	TraceBoot("KINDUNIQUE DONE rowsRead=%d", iKindRowsRead);

	// AniType
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ATTACK"),					ANITYPE_ATTACK);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_DYING"),					ANITYPE_DYING);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_HEAL"),						ANITYPE_HEAL);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_MOVE"),						ANITYPE_MOVE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_WAIT0"),					ANITYPE_WAIT0);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_WAIT1"),					ANITYPE_WAIT1);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_EXP"),						ANITYPE_EXP);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_HIT"),						ANITYPE_HIT);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_MAGIC"),					ANITYPE_MAGIC);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_FISH"),						ANITYPE_FISH);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_FARM"),						ANITYPE_FARM);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ATTACKSHADOW"), 			ANITYPE_ATTACKSHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ATTACK1"),					ANITYPE_ATTACK1);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ATTACK2"),					ANITYPE_ATTACK2);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_MAGICSHADOW"),				ANITYPE_MAGICSHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_DYINGSHADOW"),				ANITYPE_DYINGSHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_MOVESHADOW"),				ANITYPE_MOVESHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_WAIT0SHADOW"),				ANITYPE_WAIT0SHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_WAIT1SHADOW"),				ANITYPE_WAIT1SHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_HITSHADOW"),				ANITYPE_HITSHADOW);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_MINE"),						ANITYPE_MINE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ATTACKSHADOW1"),			ANITYPE_ATTACKSHADOW1);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SITDOWN"),					ANITYPE_SITDOWN);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_CRY"),						ANITYPE_CRY);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_VERYCRY"),					ANITYPE_VERYCRY);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_PLAY"),						ANITYPE_PLAY);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SMILE"),					ANITYPE_SMILE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SURPRISE"), 				ANITYPE_SURPRISE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ANGRY"),					ANITYPE_ANGRY);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_CATANGRY"), 				ANITYPE_CATANGRY);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_GOOD"),						ANITYPE_GOOD);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SIT"),						ANITYPE_SIT);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_HAND"),						ANITYPE_HAND);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_FEATHER"),					ANITYPE_FEATHER);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SLEEP"), 					ANITYPE_SLEEP);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_EAT"),	 					ANITYPE_EAT);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_EMIT"),  					ANITYPE_EMIT);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ROUDOLPHCRY"),				ANITYPE_ROUDOLPHCRY);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ROUDOLPHDANCE"),			ANITYPE_ROUDOLPHDANCE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ROUDOLPHSMILE"),			ANITYPE_ROUDOLPHSMILE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_ROUDOLPHSUPRISE"),			ANITYPE_ROUDOLPHSUPRISE);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SOULGUARDSF"),				ANITYPE_SOULGUARDSF);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SLEEPCAT"),					ANITYPE_SLEEPCAT);
	pclClient->pclHashTableString->Insert(TEXT("ANITYPE_SOULGUARDSFSHADOW"),		ANITYPE_SOULGUARDSFSHADOW);
	
	
	// TransParentMode
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_NORMAL"), TRANSPARENT_MODE_NORMAL);
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_OLDTRANS"), TRANSPARENT_MODE_OLDTRANS);
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_HALFTRANS"), TRANSPARENT_MODE_HALFTRANS);
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_SCREEN"), TRANSPARENT_MODE_SCREEN);
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_MULTIPLY"), TRANSPARENT_MODE_MULTIPLY);
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_ADD"), TRANSPARENT_MODE_ADD);
	pclClient->pclHashTableString->Insert(TEXT("TRANSPARENT_MODE_ALPHA"), TRANSPARENT_MODE_ALPHA);

	// ���Ӽ�
	pclClient->pclHashTableString->Insert(TEXT("DEFENCEATB_HUMAN"), DEFENCEATB_HUMAN );
	pclClient->pclHashTableString->Insert(TEXT("DEFENCEATB_SWORD"), DEFENCEATB_SWORD );
	pclClient->pclHashTableString->Insert(TEXT("DEFENCEATB_ARMOUR"), DEFENCEATB_ARMOUR);
	pclClient->pclHashTableString->Insert(TEXT("DEFENCEATB_HORSE"), DEFENCEATB_HORSE );
	pclClient->pclHashTableString->Insert(TEXT("DEFENCEATB_STONE"), DEFENCEATB_STONE );

	// ��������
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_Korea"),		ConstServiceArea_Korea);
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_China"),		ConstServiceArea_China);
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_Taiwan"),		ConstServiceArea_Taiwan);
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_Japan"),		ConstServiceArea_Japan);
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_English"),		ConstServiceArea_English);
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_Taiwan"),		ConstServiceArea_USA );
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_NHNChina"),	ConstServiceArea_NHNChina);
	pclClient->pclHashTableString->Insert(TEXT("ConstServiceArea_EUROPE"),		ConstServiceArea_EUROPE);

	// ��Index
	pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_MAINFIELD"), MAPINDEX_MAINFIELD);
	pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_BEGINNER1"), MAPINDEX_BEGINNER1);
	pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_PALACE"), MAPINDEX_PALACE);
	//pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_BATTLESEA"), MAPINDEX_BATTLESEA);
	//pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_DOCKDO	"), MAPINDEX_DOCKDO	);
	pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_MASTERWORLD"), MAPINDEX_MASTERWORLD);
	//pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_DAEMADOGATE"), MAPINDEX_DAEMADOGATE);
	//pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_DAEMADO"), MAPINDEX_DAEMADO);
	//pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_CHINABORDER"), MAPINDEX_CHINABORDER);
	//pclClient->pclHashTableString->Insert(TEXT("MAPINDEX_BEGINNER2"), MAPINDEX_BEGINNER2);

	// Sound �ؽ����̺��� ���
	// Tys

	
	bLoadFile=FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("data/SoundInfo_Unique.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("data/SoundInfo_Unique.txt") );
	}

	if (!bLoadFile)
		MsgBox(TEXT("Error"),TEXT("Not Found Soundinfo_Unique.txt"));

	TCHAR szSoundInfo[128] = TEXT("");
	SI32 siUnique	= 0;

	NDataTypeInfo pSoundInfo[]={	NDATA_INT32,	&siUnique,		4,
									NDATA_MBSTRING,	szSoundInfo,	128,
									0, 0, 0	};

	dataloader.RegDataType(pSoundInfo);


	while (!dataloader.IsEndOfData())
	{
		siUnique = 0;

		if (dataloader.ReadData())
			pclClient->pclHashTableString->Insert( szSoundInfo, siUnique );
	}


	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		pclKI[i] = NULL;
	}

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		pclSummonKI[i] = NULL;
	}

	for(i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
	{
		pclSummonChatCommand[i] = NULL;
	}	

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		pclGMCommandKindInfo[i] = NULL;
	}

	BasicBlock		=	BLOCK_NORMAL|BLOCK_SHALLOWWATER|BLOCK_DEEPWATER|BLOCK_CUSTOM;
	ShallowWaterBlock	=	BLOCK_NORMAL|BLOCK_LAND|BLOCK_DEEPWATER|BLOCK_CUSTOM;
	DeepWaterBlock	=	BLOCK_NORMAL|BLOCK_LAND|BLOCK_CUSTOM;

	// ĳ���͸� �����Ѵ�.

	AddCharKindInfoSetFromFile();



	//ReadPreferenceCharData();

	// ĳ������ ���� �Ӽ��� �����Ѵ�.
	if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{

		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoK.txt"));

		// ĳ������ ��Ÿ �Ӽ��� �����Ѵ�. - sortlevel , etc...
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoK.txt"));

		// ĳ������ �������� �����Ѵ�. 
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoK.txt"));

		//[����] ��ȯ�� ���̺� �и�. => 2008-6-18
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoK.txt"));
			//SetSummonEtcInfo(TEXT("CharInfo/CharEtcInfoK.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoK.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_English )
	{
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoE.txt"));
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoE.txt"));
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoE.txt"));

		//[����] ��ȯ�� ���̺� �и�. => 2008-6-18
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoE.txt"));
			//SetSummonEtcInfo(TEXT("CharInfo/CharEtcInfoE.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoE.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )
	{		
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoC.txt"));

		// ĳ������ ��Ÿ �Ӽ��� �����Ѵ�. - sortlevel , etc...
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoC.txt"));

		// ĳ������ �������� �����Ѵ�. 
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoC.txt"));

		//[����] ��ȯ�� ���̺� �и�. => 2008-6-18
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoC.txt"));
			//SetSummonEtcInfo(TEXT("CharInfo/CharEtcInfoC.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoC.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{		
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoJ.txt"));

		// ĳ������ ��Ÿ �Ӽ��� �����Ѵ�. - sortlevel , etc...
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoJ.txt"));

		// ĳ������ �������� �����Ѵ�. 
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoJ.txt"));

		//[����] ��ȯ�� ���̺� �и�. => 2008-6-18
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoJ.txt"));
			//SetSummonEtcInfo(TEXT("CharInfo/CharEtcInfoJ.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoJ.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )
	{		
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoT.txt"));

		// ĳ������ ��Ÿ �Ӽ��� �����Ѵ�. - sortlevel , etc...
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoT.txt"));

		// ĳ������ �������� �����Ѵ�. 
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoT.txt"));

		//[����] ��ȯ�� ���̺� �и�. => 2008-6-18
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoT.txt"));
			//SetSummonEtcInfo(TEXT("CharInfo/CharEtcInfoT.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoT.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_USA  )
	{		
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoU.txt"));

		// ĳ������ ��Ÿ �Ӽ��� �����Ѵ�. - sortlevel , etc...
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoU.txt"));

		// ĳ������ �������� �����Ѵ�. 
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoU.txt"));

		//[����] ��ȯ�� ���̺� �и�. => 2008-6-18
		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoU.txt"));
			//SetSummonEtcInfo(TEXT("CharInfo/CharEtcInfoU.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoU.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
	{
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoC.txt"));
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoC.txt"));
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoC.txt"));

		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoC.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoC.txt"));
		}
	}
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		SetCharAttackInfo(TEXT("CharInfo\\CharAttackInfoG.txt"));
		SetCharEtcInfo(TEXT("CharInfo\\CharEtcInfoG.txt"));
		SetCharItemInfo(TEXT("CharInfo\\CharItemInfoG.txt"));

		if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
		{
			SetSummonEtcInfo(TEXT("CharInfo/SummonEtcInfoG.txt"));
			SetSummonAttackInfo(TEXT("CharInfo/SummonAttackInfoG.txt"));
		}
	}



	// ĳ������ ��ȯ ������ �����Ѵ�. 
	SetDollInfo();

	// ���� ��ȯ���� Ư�� �ൿ ���̺��� �д´�.
	SetSummonChatCommand();

	/*	for ( SI32 d = 0 ; d < MAX_KINDINFO_NUMBER ; d ++ )
	{
	if ( pclKI[d] == NULL ) continue;
	if ( pclKI[d]->siSummonRank > 0 )
	{
	TCHAR buffer[256] = TEXT("") ;
	sprintf(buffer,TEXT("%s	%s	%d	%d\r\n"),pclKI[d]->GetName(),pclKI[d]->szCode,d,pclKI[d]->siSummonRank);
	OutputDebugStr(buffer);
	}
	}*/
}

cltKindInfoSet::~cltKindInfoSet()
{
	// ĳ���� ��ȣ�� üũ �ϱ� ���ؼ�.			// 
	//if (pclClient->GameMode == GAMEMODE_SERVER)
	//	WritePreferenceCharData();

	SI32 i;

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclKI[i])
		{
			delete pclKI[i];
			pclKI[i] = NULL;
		}
	}

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclSummonKI[i])
		{
			delete pclSummonKI[i];
			pclSummonKI[i] = NULL;
		}
	}

	for(i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
	{
		if(pclSummonChatCommand[i])
		{
			delete pclSummonChatCommand[i];
			pclSummonChatCommand[i] = NULL;
		}
	}

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclGMCommandKindInfo[i])
		{
			delete pclGMCommandKindInfo[i];
			pclGMCommandKindInfo[i] = NULL;
		}
	}
}

// NPC �̸��� �ٰŷ� Kind�� ���Ѵ�. 
SI32 cltKindInfoSet::FindKindFromName(TCHAR* npcname)
{
	SI32 i;
	if(npcname == NULL)return 0;

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclKI[i])
		{
			if(_tcscmp(pclKI[i]->GetName(), npcname) == 0)
			{
				return i;
			}
		}
	}

	return 0;
}

// NPC Code��  �ٰŷ� Kind�� ���Ѵ�. 
SI32 cltKindInfoSet::FindKindFromCode(TCHAR* npccode)
{
	SI32 i;
	if(npccode == NULL)return 0;

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclKI[i])
		{
			if(_tcscmp(pclKI[i]->szCode, npccode) == 0)
			{
				return i;
			}
		}
	}

	return 0;
}

// ĳ������ ���� �Ӽ��� �����Ѵ�.
void cltKindInfoSet::SetCharAttackInfo(TCHAR* datafilename)
{

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datafilename );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("%s %s"), datafilename, TEXT("Error"));
	}

	TCHAR	szcode[60];
	TCHAR	szname[60];
	SI32	siSortNum = 0;				// ��ȹ�ʿ��� ����ϴ� �����ͷ� ���α׷����� ������� ����
	TCHAR	szattacktype[60];
	TCHAR	szmagic[60];
	SI32	skill;
	SI32	aggressive;				// ȣ����. 
	SI32	range;
	SI32	mindmg;
	SI32	maxdmg;
	SI32	criticalrate;			// �ʻ�� Ȯ��(%)
	SI32	Attack_Interval;		// ���� ���� �ð� ���� ( milisecond ���� )
	SI32	ac;
	SI32	Str;
	SI32	Dex;
	SI32	Vit;
	SI32	Mag;
	SI32	Chm;
	SI32	Hnd;
	SI32	Luk;
	SI32	Std;
	SI64	Exp;// SI32->SI64 ����ġ �����÷ο춧���� ����
	SI32	AttackE;
	SI32	AttackR;
	SI32	DefenE;
	SI32	DefenR;
	//KHY - 0325 - ���� �߰�.
	SI32	Wis; 

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_MBSTRING,	szcode,				60,
			NDATA_MBSTRING,	szname,				60,
			NDATA_INT32,	&siSortNum,			4,		// ��ȹ�ʿ��� ����ϴ� �����ͷ� ���α׷����� ������� ����
			NDATA_MBSTRING,	szattacktype,		60,
			NDATA_MBSTRING,	szmagic,			60,
			NDATA_INT32,	&skill,				4,
			NDATA_INT32,	&aggressive,		4,
			NDATA_INT32, 	&range,				4,
			NDATA_INT32, 	&mindmg,			4,
			NDATA_INT32, 	&maxdmg,			4,
			NDATA_INT32, 	&criticalrate,		4,
			NDATA_INT32, 	&Attack_Interval,	4,
			NDATA_INT32, 	&ac,				4,
			NDATA_INT32, 	&Str, 				4,
			NDATA_INT32, 	&Dex, 				4,
			NDATA_INT32, 	&Vit, 				4,
			NDATA_INT32, 	&Mag, 				4,
			NDATA_INT32, 	&Chm, 				4,
			NDATA_INT32, 	&Hnd, 				4,
			NDATA_INT32, 	&Luk, 				4,
			NDATA_INT32, 	&Std, 				4,
			NDATA_INT64, 	&Exp, 				4,// SI32->SI64 ����ġ �����÷ο춧���� ����
			NDATA_INT32, 	&AttackE,			4,
			NDATA_INT32, 	&AttackR,			4,
			NDATA_INT32, 	&DefenE,			4,
			NDATA_INT32, 	&DefenR,			4,
			NDATA_INT32, 	&Wis,				4,
			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			if (szcode[0] != NULL)
			{
				SI32 kind = FindKindFromCode(szcode);
				if(kind > 0 && kind < MAX_KINDINFO_NUMBER)
				{
					SI32 attacktype = pclClient->pclAttackTypeInfoManager->FindAttackTypeFromCode(szattacktype);
					if(attacktype >= 0)
					{
						cltDefaultWeaponInfo		clDefaultWeaponInfo;
						clDefaultWeaponInfo.Set(attacktype, range, mindmg, maxdmg, ac);

						cltBasicAbility				clBA;
						clBA.Set(Str, Dex, Vit, Mag, Hnd, Luk,Wis);

						cltBasicParametaBox			clBPB;
						clBPB.Set(50, 0, 0 ,AttackE , AttackR, DefenE ,DefenR);


						pclKI[kind]->clDefaultWeaponInfo.Set(&clDefaultWeaponInfo);
						pclKI[kind]->clBA.Set(&clBA);
						pclKI[kind]->clBPB.Set(&clBPB);
						pclKI[kind]->Exp = Exp;								// ����ġ �ݿ�

						pclKI[kind]->dwAttackInterval	= Attack_Interval;	// ���� ���� ����

						pclKI[kind]->siCriticalRate		= criticalrate;		// �ʻ�� Ȯ�� 



						// ȣ����.
						pclKI[kind]->siAggressive	= aggressive;	 

						// ������ �߰��Ѵ�. 

						SI32 magickind = pMagicAttributePtr->FindRefFromKind(szmagic);

						if(magickind>=0)	{	pclKI[kind]->siMagic	= magickind;	}
						else				{	pclKI[kind]->siMagic	= 0;			}

						//else				{	MsgBox(TEXT("fd8vjdfsd"), TEXT("fdv8244:%s"), szmagic);		}

						// ������ų ������ �����Ѵ�.
						pclKI[kind]->siSkillLevel	= skill;

						// pclGMCommandKindInfo���� �߰��Ѵ�.
						pclGMCommandKindInfo[kind]->clDefaultWeaponInfo.Set(&clDefaultWeaponInfo);
						pclGMCommandKindInfo[kind]->clBA.Set(&clBA);
						pclGMCommandKindInfo[kind]->dwAttackInterval = Attack_Interval;
						pclGMCommandKindInfo[kind]->siCriticalRate = criticalrate;
						pclGMCommandKindInfo[kind]->siAggressive = aggressive;

					}
					else
					{
						#ifdef _DEBUG
							MsgBox(datafilename, TEXT("%s �ɸ��� �ڵ忡 %s Ÿ���� �߸�����ϴ�"), szcode, szattacktype);
						#else
							MsgBox(TEXT("vc8vjd"), TEXT("vc8763n:[%s %s]"), szcode, szattacktype);
						#endif
					}

				}
				else
				{
					#ifdef _DEBUG
						MsgBox(datafilename, TEXT("%s �ɸ��� �ڵ� Ÿ���� �߸�����ϴ�"), szcode);
					#else
						MsgBox(TEXT("vcnkkhd8"), TEXT("vc78:[%s]"), szcode);
					#endif
		
					SI32 a = 0;
				}
			}

		}
	}
}

// ĳ������ ��Ÿ �Ӽ��� �����Ѵ�. - sortlevel , etc...
void cltKindInfoSet::SetCharEtcInfo(TCHAR* datafilename)
{

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datafilename );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("%s %s"), datafilename, TEXT("Error"));
	}

	TCHAR	szcode[60];
	TCHAR	szname[60];
	SI32	sortlevel	= 0;
	SI32	summonrank	= 0;
	SI32	summonscroll[MAX_SCROLL_SUMMON_PER_KIND] = { 0, 0 };
	SI32	evolvelevel	= 0;
	SI32	portrait	= 0;
	TCHAR	szsummonjob[30];
	SI32	subskilllevel = 0 ;
	SI32	summonmall = 0 ;
	SI32	fire = 0 ;
	SI32	water = 0 ;
	SI32	land = 0 ;
	SI32	wind = 0 ;
	SI32	nature = 0 ;
	SI32	fireA = 0 ;
	SI32	waterA = 0 ;
	SI32	landA = 0 ;
	SI32	windA = 0 ;	
	SI32	magiclamp = 0;		// [����] ������� ���� ����Ʈ �� ���� ����.
	BOOL	bSummonTrade								= 0;
	SI32	siSummonEquip	= 0;
	SI32	SummonStamina								= 0;
	

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_MBSTRING,	szcode,				60,
			NDATA_MBSTRING,	szname,				60,
			NDATA_INT32, 	&sortlevel,			4,
			NDATA_INT32, 	&summonrank,		4,
			NDATA_INT32, 	&summonscroll[0],	4,
			NDATA_INT32, 	&summonscroll[1],	4,
			NDATA_INT32, 	&evolvelevel,		4,
			NDATA_INT32, 	&portrait,			4,
			NDATA_MBSTRING,	szsummonjob,		30,
			NDATA_INT32, 	&subskilllevel,		4,
			NDATA_INT32, 	&summonmall,		4,
			NDATA_INT32, 	&fire,				4,
			NDATA_INT32, 	&water,				4,
			NDATA_INT32, 	&land,				4,
			NDATA_INT32, 	&wind,				4,
			NDATA_INT32, 	&nature,			4,
			NDATA_INT32, 	&fireA,				4,
			NDATA_INT32, 	&waterA,			4,
			NDATA_INT32, 	&landA,				4,
			NDATA_INT32, 	&windA,				4,
			NDATA_INT32, 	&magiclamp,			4,
			NDATA_INT32, 	&bSummonTrade,		4,
			NDATA_INT32,	&siSummonEquip,		4,
			NDATA_INT32,	&SummonStamina,		4,
			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if (szcode[0] != NULL)
			{
				// szcode�� ��ġ�ϴ� ĳ���͸� ã�Ƽ� ���� Ư���� �����Ѵ�. 
				SI32 kind = FindKindFromCode(szcode);
				SI32 job  = clSummonJob.FindSummonJobFromJobCode(szsummonjob);
				if(kind > 0 && kind < MAX_KINDINFO_NUMBER)
				{
					pclKI[kind]->siSortLevel = sortlevel ;
#ifdef _DEBUG	
					if( pclKI[kind]->siSortLevel <= 0 || pclKI[kind]->siSortLevel >= MAX_SORT_LEVEL ) 
					{
						NTCHARString32 szError = "Sort Level Error : #min# ~ #max#"; 
						szError.Replace( "#min#", 0 );
						szError.Replace( "#max#", SI32ToString( MAX_SORT_LEVEL ) );
						MessageBox( NULL, szError, "Error", NULL );
					}
#endif

					pclKI[kind]->siSummonRank = summonrank ;
					pclKI[kind]->siMagicLamp = magiclamp ;

					for ( int i = 0 ; i < MAX_SCROLL_SUMMON_PER_KIND ; i ++ )
					{
						pclKI[kind]->siSummonScroll[i] = summonscroll[i] ;
					}
					pclKI[kind]->siEvolveLevel		= evolvelevel;
					pclKI[kind]->siSummonPortrait	= portrait;
					pclKI[kind]->siSummonJob		= job;
					pclKI[kind]->siSubSkillLevel	= subskilllevel;
					pclKI[kind]->bSummonMall		= DATA_TO_TYPE_BOOL(summonmall);
					pclKI[kind]->pclCharElemental.Set(fire,water,land,wind,nature,fireA,waterA,landA,windA);
					pclKI[kind]->bSummonTrade		= bSummonTrade;
					pclKI[kind]->siSummonEquip		= siSummonEquip;
					pclKI[kind]->siSummonStamina	= SummonStamina;	// ��ȯ�� ���¹̳� �߰� (PCK : 08.07.18)

					// pclGMCommandKindInfo���� �߰��Ѵ�.
					pclGMCommandKindInfo[kind]->clCharElemental.Set(fire,water,land,wind,nature,fireA,waterA,landA,windA);
				}
				else
				{
					MsgBox(TEXT("not Kind"), TEXT("Kind:[%s]"), szcode);
					SI32 a = 0;
				}
			}
		}
	}
}

void cltKindInfoSet::GetCharAttackInfo( SI32 siKind, TCHAR *pBuffer, SI16 txtSize )
{
    TCHAR temp[256];

	if ( pBuffer == NULL )
	{
		return;
	}

	cltClient *pclclient = NULL;
	cltNPCManagerClient* pclNPCManagerClient = NULL;

	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		pclclient = (cltClient*)pclClient;
		pclNPCManagerClient = (cltNPCManagerClient*)pclclient->pclNPCManager;
	}

	// ������� ��ȹ�� ������ ���Ѵ�. 
	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
#ifdef _SAFE_MEMORY
	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_CAPTURE, 0, 0, &pclchar->pclCI->clCharItem.clItem[0]);
#else
	SI32 skilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_CAPTURE, 0, 0, pclchar->pclCI->clCharItem.clItem);
#endif

	if( IsValidKind(siKind) == false )				return;

	SI32 rate = pclKI[siKind]->GetCaptureRate(skilllevel);
	float frate = (float)rate / 100.0f;

	BOOL bSummonRank = DATA_TO_TYPE_BOOL(pclKI[siKind]->siSummonRank);

	cltParametaBox clPB;	
	//cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[siKind];
	cltKindInfo* pclki = NULL;
	cltGMCommandKindInfo*	pclGMki = NULL;

	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclClient->pclKindInfoSet->pclSummonKI[siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[siKind];
	}
	else
	{
		pclki = pclClient->pclKindInfoSet->pclKI[siKind];
		pclGMki = pclClient->pclKindInfoSet->pclGMCommandKindInfo[siKind];
	}
	if(pclki == NULL )			return;
	if(pclGMki == NULL)			return;

	cltPIPInfo			clipinfo2;
	clipinfo2.SetStr(pclki->clBA.GetStr());
	clipinfo2.SetDex(pclki->clBA.GetDex());
	clipinfo2.SetVit(pclki->clBA.GetVit());
	clipinfo2.SetMag(pclki->clBA.GetMag());
	clipinfo2.SetLuk(pclki->clBA.GetLuk());
	clipinfo2.SetWis(pclki->clBA.GetWis());

	clipinfo2.siLevel = 1;

	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		if(pclNPCManagerClient != NULL)
		{
			clipinfo2.siLevel = clipinfo2.siLevel = pclNPCManagerClient->GetKindLevel(siKind);
			if(clipinfo2.siLevel <= 0)			clipinfo2.siLevel = 1;
		}
	}

	clPB.Init(pclClient->pclItemManager, &clipinfo2, NULL, pclki, pclGMki);
	clPB.Action(NULL, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL,  GAMEMODE_CLIENT, true, false);

	//KHY - ��ȯ�� ������ ��������.
	SI16 minDmg = 0;
	SI16 maxDmg = 0;

	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{
		minDmg = clPB.GetTotalPHysicalMinAttackDmg();
		maxDmg = clPB.GetTotalPHysicalMaxAttackDmg();
	}
	else
	{
		minDmg = clPB.GetTotalMinDamage();
		maxDmg = clPB.GetTotalMaxDamage();
	}

	if ( bSummonRank )
	{
		TCHAR* pText =GetTxtFromMgr(1287);
		TCHAR szAddScrollExplain[125];
		StringCchCopy(szAddScrollExplain, 125, pText);

		for(SI32 i = 0; i < MAX_SCROLL_SUMMON_PER_KIND; ++i)
		{
			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				if(pclSummonKI[siKind]->siSummonScroll[i])
				{
					if(i == 0)
					{
						if(pclClient->pclItemManager->GetName(pclSummonKI[siKind]->siSummonScroll[i]) == NULL)
						{
							StringCchCopy(szAddScrollExplain, 125, "");
						}
						else
						{
							StringCchCopy( szAddScrollExplain, 125, pclClient->pclItemManager->GetName(pclSummonKI[siKind]->siSummonScroll[i]) );
						}
					}
					else
					{
						StringCchCat(szAddScrollExplain, 125, TEXT(", "));
						if(pclClient->pclItemManager->GetName(pclSummonKI[siKind]->siSummonScroll[i]) == NULL)
						{
							StringCchCat(szAddScrollExplain, 125, "");
						}
						else
						{
							StringCchCat(szAddScrollExplain, 125, pclClient->pclItemManager->GetName(pclSummonKI[siKind]->siSummonScroll[i]));
						}	
					}
				}
			}
			else
			{
				if(pclKI[siKind]->siSummonScroll[i])
				{
					if(i == 0)
					{
						if(pclClient->pclItemManager->GetName(pclKI[siKind]->siSummonScroll[i]) == NULL)
						{
							StringCchCopy(szAddScrollExplain, 125, "");
						}
						else
						{
							StringCchCopy( szAddScrollExplain, 125, pclClient->pclItemManager->GetName(pclKI[siKind]->siSummonScroll[i]) );
						}
					}
					else
					{
						StringCchCat(szAddScrollExplain, 125, TEXT(", "));
						if(pclClient->pclItemManager->GetName(pclKI[siKind]->siSummonScroll[i]) == NULL)
						{
							StringCchCat(szAddScrollExplain, 125, "");
						}
						else
						{
							StringCchCat(szAddScrollExplain, 125, pclClient->pclItemManager->GetName(pclKI[siKind]->siSummonScroll[i]));
						}	
					}
				}
			}
		}

		TCHAR skillname[128]=TEXT("");
		pclClient->pclSkillManager->GetSkillName( clSummonJob.FindSummonJobSkillFromJob(pclKI[siKind]->siSummonJob), skillname );

		if(pclKI[siKind]->siSummonRank == 1)
		{
			TCHAR* pText = GetTxtFromMgr(2792);

			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				if(pclSummonKI[siKind]->siSummonRank == 1)
				{
					StringCchPrintf(	pBuffer, 
										txtSize, 
										pText,
										minDmg,
										maxDmg,
										pclKI[siKind]->Exp,
										pclKI[siKind]->siCaptureLevel,
										frate, '%',
										pclSummonKI[siKind]->siSummonRank,
										pclSummonKI[siKind]->siEvolveLevel,
										clSummonJob.FindSummonJobNameFromJob(pclSummonKI[siKind]->siSummonJob),
										skillname,
										pclKI[siKind]->siSubSkillLevel,
										LIMIT_LEVEL_HATCH_DOLL);
				}
			}
			else
			{
				StringCchPrintf(	pBuffer, 
									txtSize, 
									pText,
									minDmg,
									maxDmg,
									pclKI[siKind]->Exp,
									pclKI[siKind]->siCaptureLevel,
									frate, '%',
									pclKI[siKind]->siSummonRank,
									pclKI[siKind]->siEvolveLevel,
									clSummonJob.FindSummonJobNameFromJob(pclKI[siKind]->siSummonJob),
									skillname,
									pclKI[siKind]->siSubSkillLevel,
									LIMIT_LEVEL_HATCH_DOLL);
			}
		}
		else
		{
			TCHAR* pText	= GetTxtFromMgr(2793);
			TCHAR* pJobName	= clSummonJob.FindSummonJobNameFromJob(pclKI[siKind]->siSummonJob);

			if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
			{
				StringCchPrintf(	pBuffer, txtSize, 
									pText,
									minDmg,
									maxDmg,
									pclKI[siKind]->Exp,
									pclKI[siKind]->siCaptureLevel,
									frate, 
									'%',
									pclSummonKI[siKind]->siSummonRank,
									pclSummonKI[siKind]->siEvolveLevel,
									pJobName,
									skillname,
									pclKI[siKind]->siSubSkillLevel,
									szAddScrollExplain);
			}
			else
			{
				StringCchPrintf(	pBuffer, txtSize, 
									pText,
									minDmg,
									maxDmg,
									pclKI[siKind]->Exp,
									pclKI[siKind]->siCaptureLevel,
									frate, 
									'%',
									pclKI[siKind]->siSummonRank,
									pclKI[siKind]->siEvolveLevel,
									pJobName,
									skillname,
									pclKI[siKind]->siSubSkillLevel,
									szAddScrollExplain);
			}
		}
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(2794);
		StringCchPrintf( pBuffer, 512, 
			pText,
			minDmg,
			maxDmg,
			pclKI[siKind]->Exp,
			pclKI[siKind]->siCaptureLevel,
			frate, '%');
	}

	if(pclKI[siKind]->IsAtb(ATB_NOJJIM))
	{
		TCHAR* pText = GetTxtFromMgr(2795);
		StringCchPrintf(temp, 256, pText);
		StringCchCat(pBuffer, txtSize, temp);
	}
	if(pclKI[siKind]->IsAtb(ATB_WEAKENEMYNOATTACK))
	{
		TCHAR* pText = GetTxtFromMgr(2796);
		StringCchPrintf(temp, 256, pText);
		StringCchCat(pBuffer, txtSize, temp);
	}
	
	// ���� �Ӽ�
	SI32 fireA = pclKI[siKind]->pclCharElemental.GetFireA();
	SI32 windA = pclKI[siKind]->pclCharElemental.GetWindA();
	SI32 landA = pclKI[siKind]->pclCharElemental.GetLandA();
	SI32 waterA = pclKI[siKind]->pclCharElemental.GetWaterA();
	//SI32 fire = pclKI[siKind]->pclCharElemental.GetFire();
	//SI32 wind = pclKI[siKind]->pclCharElemental.GetWind();
	//SI32 land = pclKI[siKind]->pclCharElemental.GetLand();
	//SI32 water = pclKI[siKind]->pclCharElemental.GetWater();

	TCHAR* attBuffer;


	if ( fireA != 0 )
	{
		attBuffer = GetTxtFromMgr(5902);
		StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
		StringCchCat( pBuffer, txtSize, temp );
	}
	if ( windA != 0 )
	{
		attBuffer = GetTxtFromMgr(5906);
		StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
		StringCchCat( pBuffer, txtSize, temp );
	}
	if ( landA != 0 )
	{
		attBuffer = GetTxtFromMgr(5908);
		StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
		StringCchCat( pBuffer, txtSize, temp );
	}
	if ( waterA != 0 )
	{
		attBuffer = GetTxtFromMgr(5904);
		StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
		StringCchCat( pBuffer, txtSize, temp );
	}
	//if ( fire != 0 )
	//{
	//	attBuffer = GetTxtFromMgr(5902);
	//	StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
	//	StringCchCat( pBuffer, txtSize, temp );
	//}
	//if ( wind != 0 )
	//{
	//	attBuffer = GetTxtFromMgr(5906);
	//	StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
	//	StringCchCat( pBuffer, txtSize, temp );
	//}

	//if ( land != 0 )
	//{
	//	attBuffer = GetTxtFromMgr(5908);
	//	StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
	//	StringCchCat( pBuffer, txtSize, temp );
	//}

	//if ( water != 0 )
	//{
	//	attBuffer = GetTxtFromMgr(5904);
	//	StringCchPrintf( temp, 256, TEXT("%s\r\n"), attBuffer );
	//	StringCchCat( pBuffer, txtSize, temp );
	//}





	/*
	StringCchPrintf( pBuffer, 512, TEXT("�ٷ�:%d\r\n���߷�:%d\r\n������:%d\r\n����:%d\r\n���ݰŸ�:%d\r\n���ݷ�:%d~%d\r\n"),
	pclKI[siKind]->clBA.GetStr(),
	pclKI[siKind]->clBA.GetDex(),
	pclKI[siKind]->clBA.GetVit(),
	pclKI[siKind]->clBA.GetMag(),

	pclKI[siKind]->clDefaultWeaponInfo.GetAttackRange(),
	pclKI[siKind]->clDefaultWeaponInfo.GetDefaultMinDamage(),
	pclKI[siKind]->clDefaultWeaponInfo.GetDefaultMaxDamage()
	);
	*/

	return;
}

// ĳ������ �������� �����Ѵ�. 
void cltKindInfoSet::SetCharItemInfo(TCHAR* datafilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename);		
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datafilename);							
	}
	
	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("%s %s"), datafilename, TEXT("Error"));
	}

	TCHAR szcode[60];
	TCHAR szname[128];
	SI32 capturelevel;
	SI32 basicpbt;
	SI32 avdpbt;
	TCHAR szitemtext[512];


	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_MBSTRING,	szcode,		60,
			NDATA_MBSTRING,	szname,		128,
			NDATA_INT32, &capturelevel, 4,
			NDATA_INT32, &basicpbt,		4,
			NDATA_INT32, &avdpbt,		4,
			NDATA_MBSTRING,	szitemtext, 512,
			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if (szcode[0] != NULL)
			{
				// szcode�� ��ġ�ϴ� ĳ���͸� ã�Ƽ� ���� Ư���� �����Ѵ�. 
				SI32 kind = FindKindFromCode(szcode);
				if( IsValidKind(kind) )
				{
					pclKI[kind]->AnalyzeItemInfo(szitemtext);

					pclKI[kind]->siCaptureLevel = capturelevel;
					pclKI[kind]->siBasicPbt		= basicpbt;
					pclKI[kind]->siAdvPbt		= avdpbt;
				}
				else
				{
					#ifdef _DEBUG
						MsgBox(datafilename, TEXT("%s �ɸ��� �ڵ� Ÿ���� �߸�����ϴ�"), szcode);
					#else
						MsgBox(TEXT("11vcnkkhd8"), TEXT("vc78:[%s]"), szcode);
					#endif
				}
			}
		}
	}
}

// ��ȯ���� ��Ÿ �Ӽ��� �����Ѵ�.
void cltKindInfoSet::SetSummonEtcInfo(TCHAR* datafilename)
{

	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datafilename );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("%s %s"), datafilename, TEXT("Error"));
	}

	TCHAR	szcode[60]									= "";
	TCHAR	szname[60]									= "";
	SI32	sortlevel									= 0;
	SI32	summonrank									= 0;
	SI32	summonscroll[MAX_SCROLL_SUMMON_PER_KIND]	= { 0, 0 };
	SI32	evolvelevel									= 0;
	SI32	portrait									= 0;
	TCHAR	szsummonjob[30]								= "";
	SI32	subskilllevel								= 0;
	SI32	summonmall									= 0;
	SI32	fire										= 0;
	SI32	water 										= 0;
	SI32	land  										= 0;
	SI32	wind  										= 0;
	SI32	nature 										= 0;
	SI32	fireA  										= 0;
	SI32	waterA 										= 0;
	SI32	landA  										= 0;
	SI32	windA  										= 0;
	SI32	magiclamp									= 0; // ������� ���� ����Ʈ �� ���� ���� - �ش����Ͽ��� �о���� ����.
	BOOL	bSummonTrade								= 0;
	SI32	siSummonEquip								= 0;
	SI32	SummonStamina								= 0;
	

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szcode,				60,
		NDATA_MBSTRING,	szname,				60,
		NDATA_INT32, 	&sortlevel,			4,
		NDATA_INT32, 	&summonrank,		4,
		NDATA_INT32, 	&summonscroll[0],	4,
		NDATA_INT32, 	&summonscroll[1],	4,
		NDATA_INT32, 	&evolvelevel,		4,
		NDATA_INT32, 	&portrait,			4,
		NDATA_MBSTRING,	szsummonjob,		30,
		NDATA_INT32, 	&subskilllevel,		4,
		NDATA_INT32, 	&summonmall,		4,
		NDATA_INT32, 	&fire,				4,
		NDATA_INT32, 	&water,				4,
		NDATA_INT32, 	&land,				4,
		NDATA_INT32, 	&wind,				4,
		NDATA_INT32, 	&nature,			4,
		NDATA_INT32, 	&fireA,				4,
		NDATA_INT32, 	&waterA,			4,
		NDATA_INT32, 	&landA,				4,
		NDATA_INT32, 	&windA,				4,
		NDATA_INT32, 	&magiclamp,			4,
		NDATA_INT32, 	&bSummonTrade,		4,
		NDATA_INT32,	&siSummonEquip,		4,
		NDATA_INT32,	&SummonStamina,		4,
		0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if (szcode[0] != NULL)
			{
				// szcode�� ��ġ�ϴ� ĳ���͸� ã�Ƽ� ���� Ư���� �����Ѵ�. 
				SI32 kind = FindKindFromCode(szcode);
				SI32 job  = clSummonJob.FindSummonJobFromJobCode(szsummonjob);
				if(kind > 0 && kind < MAX_KINDINFO_NUMBER)
				{
					pclSummonKI[kind]->siSortLevel = sortlevel ;
#ifdef _DEBUG	
					if( pclSummonKI[kind]->siSortLevel <= 0 || pclSummonKI[kind]->siSortLevel >= MAX_SORT_LEVEL ) 
					{
						NTCHARString32 szError = "Sort Level Error : #min# ~ #max#"; 
						szError.Replace( "#min#", 0 );
						szError.Replace( "#max#", SI32ToString( MAX_SORT_LEVEL ) );
						MessageBox( NULL, szError, "Error", NULL );
					}
#endif

					pclSummonKI[kind]->siSummonRank = summonrank ;
					pclSummonKI[kind]->siMagicLamp = magiclamp ;

					for ( int i = 0 ; i < MAX_SCROLL_SUMMON_PER_KIND ; i ++ )
					{
						pclSummonKI[kind]->siSummonScroll[i] = summonscroll[i] ;
					}
					pclSummonKI[kind]->siEvolveLevel	= evolvelevel;
					pclSummonKI[kind]->siSummonPortrait	= portrait;
					pclSummonKI[kind]->siSummonJob		= job;
					pclSummonKI[kind]->siSubSkillLevel	= subskilllevel;
					pclSummonKI[kind]->bSummonMall		= DATA_TO_TYPE_BOOL(summonmall);
					pclSummonKI[kind]->pclCharElemental.Set(fire, water, land, wind, nature, fireA, waterA, landA, windA);
					pclSummonKI[kind]->bSummonTrade		= bSummonTrade;
					pclSummonKI[kind]->siSummonEquip	= siSummonEquip;
					pclSummonKI[kind]->siSummonStamina	= SummonStamina;	// ��ȯ�� ���¹̳� �߰� (PCK : 08.07.18)
				}
				else
				{
					MsgBox(TEXT("not Kind"), TEXT("Kind:[%s]"), szcode);
					SI32 a = 0;
				}
			}
		}
	}
}

void cltKindInfoSet::SetSummonAttackInfo(TCHAR* datafilename)
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( datafilename );
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("%s %s"), datafilename, TEXT("Error"));
	}

	TCHAR	szcode[60];
	TCHAR	szname[60];
	SI32	siSortNum = 0;				// ��ȹ�ʿ��� ����ϴ� �����ͷ� ���α׷����� ������� ����
	TCHAR	szattacktype[60];
	TCHAR	szmagic[60];
	SI32	skill;
	SI32	aggressive;				// ȣ����. 
	SI32	range;
	SI32	mindmg;
	SI32	maxdmg;
	SI32	criticalrate;			// �ʻ�� Ȯ��(%)
	SI32	Attack_Interval;		// ���� ���� �ð� ���� ( milisecond ���� )
	SI32	ac;
	SI32	Str;
	SI32	Dex;
	SI32	Vit;
	SI32	Mag;
	SI32	Chm;
	SI32	Hnd;
	SI32	Luk;
	SI32	Std;
	SI64	Exp;// SI32->SI64 ����ġ �����÷ο춧���� ����
	SI32	AttackE;
	SI32	AttackR;
	SI32	DefenE;
	SI32	DefenR;
	SI32	Wis;					//KHY - 0325 - ���� �߰�.

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szcode,				60,
		NDATA_MBSTRING,	szname,				60,
		NDATA_INT32,	&siSortNum,			4,		// ��ȹ�ʿ��� ����ϴ� �����ͷ� ���α׷����� ������� ����
		NDATA_MBSTRING,	szattacktype,		60,
		NDATA_MBSTRING,	szmagic,			60,
		NDATA_INT32,	&skill,				4,
		NDATA_INT32,	&aggressive,		4,
		NDATA_INT32, 	&range,				4,
		NDATA_INT32, 	&mindmg,			4,
		NDATA_INT32, 	&maxdmg,			4,
		NDATA_INT32, 	&criticalrate,		4,
		NDATA_INT32, 	&Attack_Interval,	4,
		NDATA_INT32, 	&ac,				4,
		NDATA_INT32, 	&Str, 				4,
		NDATA_INT32, 	&Dex, 				4,
		NDATA_INT32, 	&Vit, 				4,
		NDATA_INT32, 	&Mag, 				4,
		NDATA_INT32, 	&Chm, 				4,
		NDATA_INT32, 	&Hnd, 				4,
		NDATA_INT32, 	&Luk, 				4,
		NDATA_INT32, 	&Std, 				4,
		NDATA_INT64, 	&Exp, 				4,	// SI32->SI64 ����ġ �����÷ο춧���� ����
		NDATA_INT32, 	&AttackE,			4,
		NDATA_INT32, 	&AttackR,			4,
		NDATA_INT32, 	&DefenE,			4,
		NDATA_INT32, 	&DefenR,			4,
		NDATA_INT32, 	&Wis,				4,
		0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			if (szcode[0] != NULL)
			{
				SI32 kind = FindKindFromCode(szcode);
				if(kind > 0 && kind < MAX_KINDINFO_NUMBER)
				{
					SI32 attacktype = pclClient->pclAttackTypeInfoManager->FindAttackTypeFromCode(szattacktype);
					if(attacktype >= 0)
					{
						cltDefaultWeaponInfo		clDefaultWeaponInfo;
						clDefaultWeaponInfo.Set(attacktype, range, mindmg, maxdmg, ac);

						cltBasicAbility				clBA;
						clBA.Set(Str, Dex, Vit, Mag, Hnd, Luk,Wis);

						cltBasicParametaBox			clBPB;
						clBPB.Set(50, 0, 0 ,AttackE , AttackR, DefenE ,DefenR);


						pclSummonKI[kind]->clDefaultWeaponInfo.Set(&clDefaultWeaponInfo);
						pclSummonKI[kind]->clBA.Set(&clBA);
						pclSummonKI[kind]->clBPB.Set(&clBPB);
						pclSummonKI[kind]->Exp = Exp;							// ����ġ �ݿ�

						pclSummonKI[kind]->dwAttackInterval	= Attack_Interval;	// ���� ���� ����

						pclSummonKI[kind]->siCriticalRate	= criticalrate;		// �ʻ�� Ȯ�� 



						// ȣ����.
						pclSummonKI[kind]->siAggressive	= aggressive;	 

						// ������ �߰��Ѵ�. 

						SI32 magickind = pMagicAttributePtr->FindRefFromKind(szmagic);

						if(magickind>=0)	{	pclSummonKI[kind]->siMagic	= magickind;	}
						else				{	pclSummonKI[kind]->siMagic	= 0;			}

						//else				{	MsgBox(TEXT("fd8vjdfsd"), TEXT("fdv8244:%s"), szmagic);		}

						// ������ų ������ �����Ѵ�.
						pclSummonKI[kind]->siSkillLevel	= skill;

					}
					else
					{
#ifdef _DEBUG
						MsgBox(datafilename, TEXT("%s �ɸ��� �ڵ忡 %s Ÿ���� �߸�����ϴ�"), szcode, szattacktype);
#else
						MsgBox(TEXT("vc8vjd"), TEXT("vc8763n:[%s %s]"), szcode, szattacktype);
#endif
					}

				}
				else
				{
#ifdef _DEBUG
					MsgBox(datafilename, TEXT("%s �ɸ��� �ڵ� Ÿ���� �߸�����ϴ�"), szcode);
#else
					MsgBox(TEXT("vcnkkhd8"), TEXT("vc78:[%s]"), szcode);
#endif

					SI32 a = 0;
				}
			}

		}
	}
}


// ĳ������ ��ȯ �������� �����Ѵ�. 
void cltKindInfoSet::SetDollInfo()
{
	SI32 i;

	for(i = 0;i < MAX_KIND_NUMBER;i++)
	{
		if(pclKI[i] == NULL)continue;

		SI32 dollunique = pclClient->pclItemManager->GetItemDollUnique(i);
		pclKI[i]->siDollUnique = dollunique;
	}

}


// �⺻ Default ��Ȳ���� ���´�.
SI32 cltKindInfoSet::GetDefaultVillageGod()
{
	for(SI32 i = 0;i < MAX_KIND_NUMBER;++i)
	{
		if(pclKI[i] == NULL)continue;

		if(pclKI[i]->IsAtb(ATB_DEFAULTGOD)) return i;
	}


	return pclClient->GetUniqueFromHash(TEXT("KIND_RACCOON"));
}



// �⺻ Default ������ ���´�.
SI32 cltKindInfoSet::GetDefaultCastle()
{
	for(SI32 i = 0;i < MAX_KIND_NUMBER;++i)
	{
		if(pclKI[i] == NULL)continue;

		if(pclKI[i]->IsAtb(ATB_DEFAULTCASTLE)) return i;
	}


	return pclClient->GetUniqueFromHash(TEXT("KIND_CASTLE"));
}



// Ư�� ĳ���Ͱ� �ִ� �������� ������ ���´�.
bool cltKindInfoSet::GetItemFromKindExplain(SI32 kind, TCHAR* explaintext, SI16 txtSize)
{
	if( IsValidKind(kind) == false)			return false;

	SI32 i;

	StringCchCopy(explaintext, txtSize, TEXT(""));

	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		cltKindItemProbability* pclpbt = &pclKI[kind]->clItemPbtSet.clKindItemProbability[i];

		if(pclpbt)
		{
			TCHAR buffer[256];
			float pbt = (float)(pclpbt->siProbability / 100.0);

			const TCHAR* pitemname = pclClient->pclItemManager->GetName(pclpbt->siItemUnique);
			if(pitemname)
			{
				TCHAR* pText = GetTxtFromMgr(2797);
				StringCchPrintf(buffer, 256, pText, pitemname, pclpbt->siItemMinNum, pclpbt->siItemNum,  pbt, '%');
				StringCchCat(explaintext, txtSize, buffer);
			}
		}

	}

	return true;
}

// Ư�� ĳ���Ͱ� �ִ� �������� ������ ���´�.
bool cltKindInfoSet::GetItemFromKindExplain(SI32 kind, cltDropItem *dropitem, SI32 *siCount)
{
	if( IsValidKind(kind) == false )			return false;

	SI32 i;
	SI32 siAddCount = 0;

	for(i = 0;i < MAX_ITEM_PROBABILITY_PER_KIND;i++)
	{
		cltKindItemProbability* pclpbt = &pclKI[kind]->clItemPbtSet.clKindItemProbability[i];

		if(pclpbt)
		{
			TCHAR buffer[256];
			float pbt = (float)(pclpbt->siProbability / 100.0);

			const TCHAR* pitemname = pclClient->pclItemManager->GetName(pclpbt->siItemUnique);
			if(pitemname)
			{
				MStrCpy( dropitem[siAddCount].szName, pitemname, MAX_ITEM_NAME_LENGTH );

				TCHAR* pText = GetTxtFromMgr(2798);
				StringCchPrintf(buffer, 256, pText, pclpbt->siItemMinNum,pclpbt->siItemNum);
				MStrCpy( dropitem[siAddCount].szItemNum, buffer, 20);

				StringCchPrintf(buffer, 256, TEXT("(%6.2f%%)"),				pbt);
				MStrCpy( dropitem[siAddCount].szPercent,	buffer, 15 );
				siAddCount++;
			}
		}

	}

	*siCount = siAddCount;
	return true;
}



// Ư�� �������� �ִ� ������ ������ ���´�. 
bool cltKindInfoSet::GetItemMonsterExplain(SI32 itemunique, TCHAR* explaintext, SI16 txtSize )
{

	SI32 i;

	StringCchCopy(explaintext, txtSize, TEXT(""));

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclKI[i] == NULL)continue;

		cltKindItemProbability* pclpbt = pclKI[i]->clItemPbtSet.FindItem(itemunique);

		if(pclpbt)
		{
			TCHAR buffer[256];
			float pbt = (float)(pclpbt->siProbability / 100.0);

			TCHAR* pText = GetTxtFromMgr(2799);
			StringCchPrintf(buffer, 256, pText, pclKI[i]->GetName(), pclpbt->siItemMinNum, pclpbt->siItemNum, pbt, '%');
			StringCchCat(explaintext, txtSize, buffer);
		}
	}

	return true;
}


// Ư�� �������� �ִ� ������ ������ ���´�. 
bool cltKindInfoSet::GetItemMonsterExplain(SI32 itemunique, cltDropItem *dropitem, SI32 *siCount)
{

	SI32 i;
	SI32 siAddCount = 0;

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclKI[i] == NULL)continue;

		cltKindItemProbability* pclpbt = pclKI[i]->clItemPbtSet.FindItem(itemunique);

		if(pclpbt)
		{
			TCHAR buffer[256];
			float pbt = (float)(pclpbt->siProbability / 100.0);

			if ( ConstServiceArea_China == pclClient->siServiceArea )
			{
				pbt = max( 1.0f, pbt );
			}

			MStrCpy( dropitem[siAddCount].szName, pclKI[i]->GetName(), MAX_ITEM_NAME_LENGTH );

			TCHAR* pText = GetTxtFromMgr(2800);
			StringCchPrintf(buffer, 256, pText,	pclpbt->siItemMinNum ,pclpbt->siItemNum, pbt);
			MStrCpy(dropitem[siAddCount].szItemNum,	buffer, 20 );

			siAddCount++;
			if(siAddCount >= *siCount) break;
		}
	}

	*siCount = siAddCount;
	return true;
}


// kind�� �ٰŷ� �̸��� ���´�. 
const TCHAR* cltKindInfoSet::GetName(SI32 kind)
{
	if( IsValidKind(kind) == false )		return NULL;

	return pclKI[kind]->GetName();
}

void cltKindInfoSet::IncreasePreferenceChar( SI32 kind, SI32 lifetime)
{
	if( IsValidKind(kind) == false )		return;

	pclKI[kind]->IncreasePreferenceCharNum(lifetime);

	return;
}

void cltKindInfoSet::ReadPreferenceCharData(TCHAR * filename)
{
	FILE *fp = _tfopen( filename, TEXT("rt") );

	if ( fp == NULL )
	{
		return;
	}

	TCHAR buffer[1024];
	SI32 Kind;
	TCHAR Name[ 64 ];
	SI32 IncreaseNum;
	SI32 lifetime = 0;


	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		Kind			= 0;
		IncreaseNum		= 0;
		lifetime		= 0;

		_stscanf(buffer,TEXT("%d %s %d %d"), &Kind, Name, &IncreaseNum, &lifetime );
		if ( IsValidKind(Kind) )
		{
			pclKI[ Kind ]->siPreferenceCharNum = IncreaseNum;
			pclKI[ Kind ]->siKindLifeTime		= lifetime;
		}
	}

	fclose( fp );

	return;
}

void cltKindInfoSet::WritePreferenceCharData(_SYSTEMTIME sTime)
{

	TCHAR fileNameBuffer[256];
	fileNameBuffer[0] = 0;


	// ������ ������ ���� ����� �����͸� �о� �´�

	static bool backUpDataLoaded = false;
	static SI32 lastWrittenDay = 0;

	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\PreferenceChar_%04d%02d%02d.log") , sTime.wYear , sTime.wMonth , sTime.wDay );


	if( backUpDataLoaded == false )
	{
		//ReadStatisticBackUp( TEXT("Config\\Item_backup.bak") , sTime);

		
		ReadPreferenceCharData( fileNameBuffer );

		backUpDataLoaded = true;
		lastWrittenDay	= sTime.wDay; // �ʱ�ȭ�� ��¥�� ������ ������Ʈ 

		return;
	}

	// ��¥�� �ٲ�� �ϴ��� ��� �����͸� �ʱ�ȭ ���ش�.
	if( lastWrittenDay != sTime.wDay )
	{
		cltKindInfo* pclki = NULL;
		

		for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
		{
			pclki = pclKI[ i ];

			if( pclki == NULL ) continue;

            pclki->siKindLifeTime = 0;
			pclki->siPreferenceCharNum = 0;
		}
		
		lastWrittenDay = sTime.wDay;
	}

	FILE *fp = _tfopen( fileNameBuffer, TEXT("wt") );

	if ( fp == NULL )
	{
		return;
	}

	cltKindInfo* pclki = NULL;

	for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
	{
		pclki = pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_ATTACK ) == false )
			continue;

		SI32 avetime = 0;

		if(pclki->siPreferenceCharNum)
		{
			avetime = pclki->siKindLifeTime/pclki->siPreferenceCharNum;
		}

		_ftprintf( fp, TEXT("%d\t%s\t%d\t%d\t%d\n"), i, pclki->GetName(), pclki->siPreferenceCharNum, pclki->siKindLifeTime,  avetime);
	}

	fclose( fp );

	return;
}


// ���� ����Ʈ ��� ���Ϳ� �������� �����ϰ� ���Ѵ�. 
bool cltKindInfoSet::GetDailyQuestKindInfo(SI32* pkind, SI32*pbattlepower, SI32* pexp)
{
	SI32 i;
	SI32 sikindlist[MAX_KIND_NUMBER];
	SI32 sikindindex = 0;

	SI32 kind;

	// ��ȿ��  kind�� �Ѿ���� �����ϰ� ���Ѵ�. 
	if(*pkind == 0)
	{

		cltKindInfo* pclki = NULL;


		for (i = 10; i < MAX_KIND_NUMBER; ++i )
		{
			pclki = pclKI[ i ];

			if( pclki == NULL || pclki->IsAtb( ATB_DAILYQUEST ) == false )	continue;

			switch (pclClient->siServiceArea)
			{
			case ConstServiceArea_Japan :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_JAPAN))
						continue;
				}
				break;
			case ConstServiceArea_English :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_ENGLISH))
						continue;
				}
				break;
			case ConstServiceArea_China :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_CHINA))
						continue;
				}
				break;
			case ConstServiceArea_Korea :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_KOREA))
						continue;
				}
				break;
			case ConstServiceArea_Taiwan :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_TAIWAN))
						continue;
				}
				break;
			case ConstServiceArea_USA  :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_USA))
						continue;
				}
				break;
			case ConstServiceArea_NHNChina :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_NHNCHINA))
						continue;
				}
				break;

			case ConstServiceArea_EUROPE :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_EUROPE))
						continue;
				}
				break;

			}

			sikindlist[sikindindex] = i;
			sikindindex++;
		}

		if(sikindindex <= 0)return false;

		kind			= sikindlist[rand() % sikindindex];
	}
	// ��ȿ�� kind�� �Ѿ���� �� kind�� ����Ѵ�. 
	else
	{
		kind			= *pkind;
	}

	if( IsValidKind( kind ) == false )		return false;
	SI32 battlepower	= max(1, pclKI[ kind ]->clBA.GetStr() + pclKI[ kind ]->clBA.GetDex() + pclKI[ kind ]->clBA.GetVit() + pclKI[ kind ]->clBA.GetMag());


	*pkind			= kind;
	*pbattlepower	= battlepower;
	*pexp			= pclKI[ kind ]->Exp;// SI32->SI64 ����ġ �����÷ο춧���� ����

	return true;	
}
bool cltKindInfoSet::GetDailyQuest2KindInfo(SI32* pkind, SI32*pMonsterLevel, SI32* pexp)
{
	SI32 i;
	SI32 sikindlist[MAX_KIND_NUMBER];
	SI32 sikindindex = 0;
	SI32 kind;

	// ��ȿ��  kind�� �Ѿ���� �����ϰ� ���Ѵ�. 
	if(*pkind == 0)
	{
		cltKindInfo* pclki = NULL;
		for (i = 10; i < MAX_KIND_NUMBER; ++i )
		{
			pclki = pclKI[ i ];

			if( pclki == NULL || pclki->IsAtb( ATB_DAILYQUEST2 ) == false )	continue;

			switch (pclClient->siServiceArea)
			{
			case ConstServiceArea_Japan :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_JAPAN))
						continue;
				}
				break;
			case ConstServiceArea_English :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_ENGLISH))
						continue;
				}
				break;
			case ConstServiceArea_China :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_CHINA))
						continue;
				}
				break;
			case ConstServiceArea_Korea :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_KOREA))
						continue;
				}
				break;
			case ConstServiceArea_Taiwan :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_TAIWAN))
						continue;
				}
				break;
			case ConstServiceArea_USA  :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_USA))
						continue;
				}
				break;
			case ConstServiceArea_NHNChina :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_NHNCHINA))
						continue;
				}
				break;
			case ConstServiceArea_EUROPE :
				{
					if (!(pclki->siServiceAreaAtb & SERVICEAREA_ATB_EUROPE))
						continue;
				}
				break;
			}
			sikindlist[sikindindex] = i;
			sikindindex++;
		}

		if(sikindindex <= 0)return false;

		kind			= sikindlist[rand() % sikindindex];
	}
	// ��ȿ�� kind�� �Ѿ���� �� kind�� ����Ѵ�. 
	else
	{
		kind			= *pkind;
	}

	if( IsValidKind( kind ) == false )		return false;
	//���� ������ ����
    *pkind			= kind;
	*pMonsterLevel	= pclKI[ kind ]->clBA.GetBattleLevel();
	*pexp			= pclKI[ kind ]->Exp;// SI32->SI64 ����ġ �����÷ο춧���� ����

	return true;	
}
SI32 cltKindInfoSet::GetSortLevel(SI32 kind)
{
	if( IsValidKind(kind) == false )			return 0 ;

	return pclKI[kind]->siSortLevel ;
}


SI32 cltKindInfoSet::GetSubSkillLevel(SI32 kind,SI32 skill)
{
	if ( kind <= 0 || kind >= MAX_KIND_NUMBER ) return 0 ;
	if ( pclKI[kind] == NULL ) return 0 ;
	if ( skill <= SUMMON_JOB_NULL || skill >= MAX_SUMMON_JOB ) return 0 ;
	if ( pclKI[kind]->siSummonJob != skill ) return 0 ;

	return pclKI[kind]->siSubSkillLevel ;
}

// Ư�� �ӹ��� ��ɿ����� ������ ���͸� ���Ѵ�. 
bool cltKindInfoSet::FindGoodMonsterForSpecialQuest(SI32 level, SI32* pkind, SI32* pmonnum, cltQuestRewardInfo* pclrewardinfo)
{
	SI32 i;
//	SI32 levelrate		= level * 100 / max(100, (MAX_LEVEL - 30));
//	if(levelrate > 100)levelrate = 100;

	// �ڽ��� ���� +1 ~ +10
	//SI32 minmonlevel	= level + 1;
	//SI32 maxmonlevel	= level + 10;

	// �ڽ��� ���� -15 ~ +5
	SI32 minmonlevel	= level - 15;
	SI32 maxmonlevel	= level + 5;


	SI16 kindlist[MAX_KIND_NUMBER];
	SI32 kindlistindex	= 0;

	// ��ȿ ���� ���� ���� ���͸� ���ؼ� ����Ʈ�� �߰��Ѵ�. 
	for(i = 0;i < MAX_KIND_NUMBER;i++)
	{
		if(pclKI[i] == NULL)continue;
		if(pclKI[i]->IsAtb(ATB_MONSTER) == false)continue;	// ���Ͱ� �ƴϸ� ����
		if(pclKI[i]->IsAtb(ATB_BOSS) == true)continue;		// ���� ���� ����
		if(pclKI[i]->IsAtb(ATB_EVENT) == true)continue;		// �̺�Ʈ�� ���ʹ� ���� - by LEEKH 2007.07.03


		// ������� ���� ����Ʈ �� ���� ������ �� �Ͽ� �� ���� �˸��� ���͸� ������.
		if(pclKI[i]->siMagicLamp > maxmonlevel) continue;	
		if(pclKI[i]->siMagicLamp < minmonlevel) continue;

		//if(pclKI[i]->clBA.GetBattleLevel() > maxmonlevel)continue;
		//if(pclKI[i]->clBA.GetBattleLevel() < minmonlevel)continue;


		kindlist[kindlistindex] = i;
		kindlistindex++;
	}
	
	if(kindlistindex)
	{
		SI32 monkind	= kindlist[rand() % kindlistindex];

		SI32 monnum = 0;

		// 50% Ȯ��
		if ( rand()%2 )
		{
			monnum = rand() % 40 + 41; // 40 ~ 80 ����
		}
		else
		{
			monnum = 200 + rand() % 50; //200~249 ����
		}


		*pkind		= monkind;
		*pmonnum	= monnum;

		pclrewardinfo->siExp = max(100, ( level * monnum * 2 ) );
		return true;
	}

	return 0;
}

// 2010�ų��̺�Ʈ - ĳ���� ������ ���������� �´� ����, ����� ���� ������ ���Ѵ�.
bool cltKindInfoSet::Find_MakeLevel_ItemNum(SI32 level, SI32 makelevel)
{
	// �������� 31~60 �̻� �������� 1~20
	// �������� 61~90 �̻� �������� 20�̻�
	// �������� 91~120 �̻� �������� 40 �̻�

	if( level == 1 && level <= 30 )return false;	// ĳ���� ���� 1~30 ������ ������ ���� ����Ʈ�� ���� �� ����.

	if(level >= 31&& level <=	120) // 31~120 ���� ����鸸 ������ �� �ִ� ����Ʈ�� �־���.
	{
		SI32 makeitemnum = 0;

		if( makelevel >= 1 && makelevel <= 20 )
		{
			makeitemnum = 6 + rand() % 21;
		}	
	}

	return true;
}

// 2010�ų��̺�Ʈ - ��ɿ����� �ش� ĳ���Ϳ��� ������ ���͸� ���Ѵ�.
bool cltKindInfoSet::FindGoodMonsterForRemoveUnLuckStatusQuest( SI32 level, SI32* pkind, SI32* pmonnum, cltQuestRewardInfo* pclrewardinfo )
{
	// �ڽ��� ������� ���� ���Ͱ� ���´�.
	SI32 minmonlevel = 0;
	SI32 maxmonlevel = 0;

	// ĳ���� ������ ���� ã������ ������ ���� ���� �ִ�/�ּ� ���� ���Ѵ�
	if ( 1 <= level && 30 >= level )
	{
		minmonlevel = 1;
		maxmonlevel = 30;
	}
	else if ( 31 <= level && 60 >= level )
	{
		minmonlevel = 31;
		maxmonlevel = 60;
	}

	else if ( 61 <= level && 90 >= level )
	{
		minmonlevel = 61;
		maxmonlevel = 90;
	}

	else if ( 91 <= level && 100 >= level )
	{
		minmonlevel = 91;
		maxmonlevel = 100;
	}

	else if ( 101 <= level && 120 >= level )
	{
		minmonlevel = 100;
		maxmonlevel = 120;
	}

	SI16 kindlist[MAX_KIND_NUMBER];
	SI32 kindlistindex = 0;

	// ��ȿ���� ���� ���͸� ���ؼ� ����Ʈ�� �߰����ش�.
	for(SI32 i = 0; i<MAX_KIND_NUMBER; i++ )
	{
		if(pclKI[i] == NULL )continue;
		if(pclKI[i]->IsAtb(ATB_MONSTER) == false )continue;		// ���Ͱ� �ƴϸ� ����.
		if(pclKI[i]->IsAtb(ATB_BOSS) == true )continue;			// ���� ���� ����.
		if(pclKI[i]->IsAtb(ATB_EVENT) == true )continue;		// �̺�Ʈ ���ʹ� ����.

		// charetcinfoe.txt �� magiclamp �������� ���� ���� ����.
		if(pclKI[i]->siMagicLamp < minmonlevel) continue;
		if(pclKI[i]->siMagicLamp > maxmonlevel) continue;

		kindlist[kindlistindex] = i;
		kindlistindex++;
	}

	if(kindlistindex)
	{
		SI32 monkind	=	kindlist[rand() % kindlistindex];
		SI32 monnum	=	0;

		monnum = 10 + rand() % 41;	// 10 ~ 50����

		*pkind		= monkind;
		*pmonnum	= monnum;

		pclrewardinfo->bDeBuff = true;

		return true;
	}

	return false;
}

// ���ֵ��̹̼� - ��ɿ����� �ش� ĳ���Ϳ��� ������ ���͸� ���Ѵ�.
bool cltKindInfoSet::FindGoodMonsterForGoonzuDay( SI32 level, SI32* pkind, SI32* pmonnum )
{
	SI32 i;

	// �ڽ��� ���� -20 ������ ���Ͱ� ���� �� �ִ�. 
	SI32 minmonlevel	= level - 20;
	SI32 maxmonlevel	= level;

	SI16 kindlist[MAX_KIND_NUMBER];
	SI32 kindlistindex	= 0;

	// ��ȿ ���� ���� ���� ���͸� ���ؼ� ����Ʈ�� �߰��Ѵ�. 
	for(i = 0;i < MAX_KIND_NUMBER;i++)
	{
		if(pclKI[i] == NULL)continue;
		if(pclKI[i]->IsAtb(ATB_MONSTER) == false)continue;	// ���Ͱ� �ƴϸ� ����
		if(pclKI[i]->IsAtb(ATB_BOSS) == true)continue;		// ���� ���� ����
		if(pclKI[i]->IsAtb(ATB_EVENT) == true)continue;		// �̺�Ʈ�� ���ʹ� ���� - by LEEKH 2007.07.03

		// ������� ���� ����Ʈ �� ���� ������ �� �Ͽ� �� ���� �˸��� ���͸� ������.
		if(pclKI[i]->siMagicLamp > maxmonlevel) continue;	
		if(pclKI[i]->siMagicLamp < minmonlevel) continue;

		kindlist[kindlistindex] = i;
		kindlistindex++;
	}

	if(kindlistindex)
	{
		SI32 monkind	= kindlist[rand() % kindlistindex];

		SI32 monnum = 0;

		monnum = rand() % 6 + 25; // 6 ~ 30 ����
		
		*pkind		= monkind;
		*pmonnum	= monnum;

		return true;
	}

	return 0;
}

//KHY - 0822 - ���Ǻ� ����Ʈ �߰�.
bool cltKindInfoSet::FindGoodMonsterForHappyBeanQuest(SI32 level, SI32* pkind, SI32* pmonnum, cltQuestRewardInfo* pclrewardinfo)
{
	SI32 i;

	// �ڽ��� ���� -5 ~ 0
	SI32 minmonlevel	= level -5;
	SI32 maxmonlevel	= level ;

	SI32 siGetKind[400];
	ZeroMemory(siGetKind, sizeof(siGetKind));

	SI16 kindlist[MAX_KIND_NUMBER];
	SI32 kindlistindex	= 0;
	ZeroMemory(kindlist, sizeof(kindlist));

	cltNPCManagerServer* pclnpcmanager = (cltNPCManagerServer*)pclClient->pclNPCManager;
	pclnpcmanager->GetKindLevel(minmonlevel, maxmonlevel, siGetKind, 400);

	for(i = 0; i<MAX_KIND_NUMBER; i++)
	{
		SI32 siKind = siGetKind[i];

		if(IsValidKind(siKind) == false)			continue;
		if(pclKI[siKind]->IsAtb(ATB_MONSTER) == false)continue;	// ���Ͱ� �ƴϸ� ����
		if(pclKI[siKind]->IsAtb(ATB_BOSS) == true)continue;		// ���� ���� ����
		if(pclKI[siKind]->IsAtb(ATB_EVENT) == true)continue;		// �̺�Ʈ�� ���ʹ� ���� - by LEEKH 2007.07.03

		kindlist[kindlistindex] = siKind;
		kindlistindex++;
	}

	if(kindlistindex)
	{
		SI32 monkind	= kindlist[rand() % kindlistindex];
		SI32 monnum		= 20 + rand() % 30;

		*pkind		= monkind;
		*pmonnum	= monnum;

		pclrewardinfo->siItemUnique[0]	= ITEMUNIQUE(2770); //����Ÿ�� ������
		
		pclrewardinfo->siItemNum[0]		= 1;
//		pclrewardinfo->siExp = max(100, ( level * monnum * 2 ) );

		return true;
	}

	return 0;
}

// ���� ���� ���� ����
SI32 cltKindInfoSet::GetHireVillageLevel(SI32 kind)
{
	if( IsValidKind(kind) == false )		return 0;

	return (100 - pclKI[kind]->clHireCond.siMinVillageLevel)  * max(10, pclClient->siBestVillageLevel) / 100 ;
}


// ������ �ʴ� ������ �����Ѵ�.
void cltKindInfoSet::DeleteIfNotUse(DWORD curclock)
{
	SI32 i;

	for(i = 0;i < MAX_KINDINFO_NUMBER;i++)
	{
		if(pclKI[i] == NULL)continue;

		pclKI[i]->clCharAniManager.DeleteIfNotUse(curclock);
	}
}


bool cltKindInfoSet::IsSummonFromRank(SI32 rank)
{
	if ( rank <= 0 || rank > LIMIT_SUMMON_RANK_EVOLVE ) return false ;
	for ( SI32 i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
	{
		if ( pclKI[i] == NULL ) continue ;
		if ( pclKI[i]->bSummonMall == TRUE ) continue ;
		if ( pclKI[i]->siSummonRank == rank ) return true ;
	}

	return false ;
}

bool cltKindInfoSet::IsValidKind(SI32 kind)
{
	if(kind <= 0 || kind >= MAX_KIND_NUMBER)return false;
	if(pclKI[kind] == NULL)return false;

	return true;
}

// ���� �⺻ ��� ȹ��. 
GMONEY cltKindInfoSet::GetHireBasicMoney(SI32 kind)
{
	if( IsValidKind(kind) == false )			return 0;
	return max(20000, pclKI[kind]->clHireCond.siBasicMoney);
}

bool cltKindInfoSet::GetNPCImageFileAndFont(SI32 siKind, TCHAR* pszFileName, SI32 siLen, SI32* siFont)
{
	siKind--;

	StringCchPrintf(pszFileName, siLen, TEXT(""));
	*siFont = 0;

	*siFont = siKind % 256;
	SI32 siImgIndex;

	siImgIndex = siKind / 256;
	switch(siImgIndex)
	{
	case 0:		StringCchPrintf(pszFileName, siLen, TEXT("Interface/Portrait/PortraitIcon.SPR"));		break;
	case 1:		StringCchPrintf(pszFileName, siLen, TEXT("Interface/Portrait/PortraitIcon02.SPR"));		break;
	case 2:		StringCchPrintf(pszFileName, siLen, TEXT("Interface/Portrait/PortraitIcon03.SPR"));		break;
	}

	return true;
}

bool cltKindInfoSet::GetNPCGlobalImageIndexAndFont( IN SI32 siKind, OUT SI32* psiGimgIndex, OUT SI32* psiFont )
{
	if ( (NULL == psiGimgIndex) || (NULL == psiFont) )
	{
		return false;
	}

	SI32 siWorkKind		= (siKind - 1);
	SI32 siReturnFont	= (siWorkKind % 256);

	SI32 siImgIndex		= (siWorkKind / 256);
	SI32 siReturnIndex	= 0;

	switch ( siImgIndex )
	{
		case 0:	siReturnIndex = GIMG_PORTRAIT_SMALL01;	break;
		case 1:	siReturnIndex = GIMG_PORTRAIT_SMALL02;	break;
	}

	*psiGimgIndex	= siReturnIndex;
	*psiFont		= siReturnFont;

	return true;
}

bool cltKindInfoSet::GetNPCImageFileAndFontsmall(SI32 siKind, TCHAR* pszFileName, SI32 siLen, SI32* siFont)
{
	siKind--;
	StringCchPrintf(pszFileName, siLen, TEXT(""));
	*siFont = 0;
	*siFont = siKind;

	StringCchPrintf(pszFileName, siLen, TEXT("Interface/Pannel/IMAGE_33x30_00_000.SPR"));
	return true;
}

void cltKindInfoSet::SetSummonChatCommand()
{
	NDataLoader	dataloader;
	BOOL bLoadFile = FALSE;

	if(pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption))
	{
		bLoadFile = dataloader.LoadDataFromCryptFile(TEXT("Data/SummonChatCommand.txt"));
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(TEXT("Data/SummonChatCommand.txt"));
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("Error"), TEXT("Not Found SummonChatCommand.txt"));
	}

	TCHAR szSummonAniType[128]	=	TEXT("")	;
	SI32  siChatTextLine		=	0	;
	SI32  siLevel				=	0	;
	TCHAR szCharKind[128]		=	TEXT("")	;

	// SummonChatCommand ����
	NDataTypeInfo pSummonChatCommand[] = 
	{
		NDATA_INT32,	&siChatTextLine,	4,
		NDATA_MBSTRING,	szSummonAniType,	128,
		NDATA_INT32,	&siLevel,			4,
		NDATA_MBSTRING,	szCharKind,			128,
		0,0,0
	};

	dataloader.RegDataType(pSummonChatCommand);
	
	SI32 siIndex = 0;

	while(!dataloader.IsEndOfData())
	{
		if (siIndex >= MAX_SUMMON_CHATCOMMANDNUMBER) break;

		if(dataloader.ReadData())
		{
			pclSummonChatCommand[siIndex] = new cltSummonChatCommand();
			if(pclSummonChatCommand[siIndex] != NULL)
			{
				pclSummonChatCommand[siIndex]->siChatTextLine	=	siChatTextLine	;
				pclSummonChatCommand[siIndex]->siAniType		=	pclClient->GetUniqueFromHash(szSummonAniType)	;
				pclSummonChatCommand[siIndex]->siLevel			=	siLevel	;
				pclSummonChatCommand[siIndex]->siCharKind		=	pclClient->GetUniqueFromHash(szCharKind)	;
			}

			++siIndex;
		}
	}
}

SI16 cltKindInfoSet::GetAnitypeFromText(TCHAR* pszText, SI16 siCharKind, SI16 siLevel)
{
	TCHAR* pszCmd = NULL;

	for(SI32 i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
	{
		if( pclSummonChatCommand[i] == NULL )		continue;

		pszCmd = GetTxtFromMgr(pclSummonChatCommand[i]->siChatTextLine);
		if( pszCmd == NULL )						continue;
		if( _tcscmp( pszCmd, TEXT("NULL") ) == 0 )	continue;

		if( _tcsstr(pszText, pszCmd) != NULL )
		{
			if( pclSummonChatCommand[i]->siCharKind == siCharKind 
				&& pclSummonChatCommand[i]->siLevel <= siLevel )
			return pclSummonChatCommand[i]->siAniType;
		}
	}

	return 0;
}

SI32 cltKindInfoSet::GetTextMgrCanSummonChatCommand(SI16 siCharKind, SI16 siLevel)
{
	TCHAR* pszCmd = NULL;

	for(SI32 i = 0;i < MAX_SUMMON_CHATCOMMANDNUMBER;i++)
	{
		if( pclSummonChatCommand[i] == NULL )				continue	;

		if ( pclSummonChatCommand[i]->siChatTextLine <= 0)	continue	;
		pszCmd = GetTxtFromMgr(pclSummonChatCommand[i]->siChatTextLine);
		if( pszCmd == NULL )								continue	;
		if( _tcscmp( pszCmd, TEXT("NULL") ) == 0 )			continue	;

		if( pclSummonChatCommand[i]->siCharKind == siCharKind && 
			pclSummonChatCommand[i]->siLevel == siLevel )
		{
			return pclSummonChatCommand[i]->siChatTextLine;
		}
	}

	return 0;
}
