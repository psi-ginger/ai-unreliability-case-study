//---------------------------------
// 2003/11/26 ���°�
//---------------------------------
#include "CommonLogic.h"
#include "Char\CharCommon\Char-Common.h"

#include "Quest.h"
#include "..\resource.h"
#include "MsgType-Person.h"

//------------------------------------
// Lib
//------------------------------------
#include "HashTable/HashTable_StringInt.h"

extern cltCommonLogic* pclClient;

// ����Ʈ �ؽ�Ʈ�� �ҷ��´�. 
bool cltQuestManager::LoadQuestText(SI32 questtype, SI32 questunique)
{ 
	SI32 i;
	FILE* fp;
	TCHAR buffer[2048];
	TCHAR filename[1024];
	TCHAR temp[256] = {0,};

	cltQuestClearConditionInfo	clClearCond;
	cltQuestRewardInfo			clReward;
	cltQuestInitConditionInfo	clinitcond;

	SI32 unique ;

	if( pclClient->siServiceArea == ConstServiceArea_Korea )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dK.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_English )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dE.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_China )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dC.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_Japan )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dJ.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dT.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_USA )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dU.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dC.txt"), szQuestTypeName[questtype], questunique);
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
		StringCchPrintf(buffer, sizeof(buffer), TEXT("Quest\\%s\\%03dG.txt"), szQuestTypeName[questtype], questunique);

	_tcscpy(filename, buffer);

	//��� ������ ���� ���� �ε�
	//if( pclClient->siLanguage == ConstLanguage_China )
	//{
	//	StringCchPrintf(buffer, 1024, TEXT("Quest\\%s\\%03dC.txt"), szQuestTypeName[questtype], questunique);
	//}


	//	if( questtype == 15 ) {
	//		int a = 10;
	//	}

	fp = _tfopen(buffer, TEXT("rb"));

	if(fp == NULL)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("LoadQuestText"), buffer, MB_OK|MB_TOPMOST);
		return false;
	}

	//-------------------------------------
	// 5�� ����. 
	//-------------------------------------
	for(i = 0;i < 5;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);

#ifdef _DEBUG
		if( i == 4 )
		{
			if(_tcsncmp(buffer, "Unique", 6) != 0)
			{
				MessageBox(NULL, TEXT("Line : 5"), filename, MB_OK|MB_TOPMOST);
			}
		}
#endif
	}

	// ����ũ�� ���Ѵ�.
	_fgetts(buffer, sizeof(buffer), fp);

	_stscanf(buffer, TEXT("%d"), &unique);

	//-------------------------------------
	// 2�� ����. 
	//-------------------------------------
	for(i = 0;i < 2;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if( i == 1 )
		{
			if(_tcsncmp(buffer, "Title", 5) != 0)
			{
				MessageBox(NULL, TEXT("Line : 8"), filename, MB_OK|MB_TOPMOST);
			}
		}
#endif
	}

	// Ÿ��Ʋ�� ���Ѵ�. 
	_fgetts(buffer, sizeof(buffer), fp);
	/*TCHAR sztitle[128];

	_tcsncpy(sztitle, buffer, 127);
	for(i = 0;i < 128;i++)
	{
		if(sztitle[i] == '\t')
		{
			sztitle[i] ='\0';
		}
	}*/

#ifdef _DEBUG
	if( m_pQuestText == NULL )		MessageBox(NULL, TEXT("m_pQuestText is NULL"), filename, MB_OK|MB_TOPMOST);
#endif

	NTCHARString<128>	kTitle;
	NTCHARString<64>	kKey_Title;
	kKey_Title  = SI32ToString( questunique );
	kKey_Title += "_Title";
	kTitle = m_pQuestText->GetText( kKey_Title );
	kTitle.ReplaceAll( "\t", "\0" );

	//-------------------------------------
	// 3�� ����. 
	//-------------------------------------
	for(i = 0;i < 3;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if( i == 1 )
		{
			if(_tcsncmp(buffer, "Middle", 6) != 0)
			{
				MessageBox(NULL, TEXT("Line : 11"), filename, MB_OK|MB_TOPMOST);
			}
		}
		if( i == 2 )
		{
			if(_tcsncmp(buffer, "Money", 5) != 0)
			{
				MessageBox(NULL, TEXT("Line : 12"), filename, MB_OK|MB_TOPMOST);
			}
		}
#endif
	}


	// �߰� ������ ���Ѵ�. 
	SI32 middlemoney;
	SI32 middleitemunique;
	SI32 middleitemnum;

	_fgetts(buffer, sizeof(buffer), fp);

	_stscanf(buffer, TEXT("%d %d %d"), &middlemoney, &middleitemunique, &middleitemnum);


	//-------------------------------------
	// 1�� ����. 
	//-------------------------------------
	for(i = 0;i < 1;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
	}

	// �Ϸ������� Ȯ���Ѵ�. 
	TCHAR text[128];
	TCHAR clearcondtext[128];

	_fgetts(buffer, sizeof(buffer), fp);

	_stscanf(buffer, TEXT("%s %s"), text, clearcondtext);

#ifdef _DEBUG
	if(_tcsncmp(text, "Condition", 9) != 0)
	{
		MessageBox(NULL, TEXT("Line : 15"), filename, MB_OK|MB_TOPMOST);
	}

	if (_tcsncmp(clearcondtext, "QUEST_CLEAR_COND_", 17) != 0)
	{
		MessageBox(NULL, TEXT("Do Not Have ClearCondition"), filename, MB_OK|MB_TOPMOST);
	}
#endif

	

	// ���� ���ξ� 17����Ʈ�� �����Ѵ�. 
	StringCchCopy(temp, 256, &clearcondtext[17]);
	StringCchCopy(clearcondtext, 128, temp);

	//-------------------------------------
	// 1�� ����. 
	//-------------------------------------
	for(i = 0;i < 1;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if( i == 0 )
		{
			if(_tcsncmp(buffer, "Para1", 5) != 0)
			{
				MessageBox(NULL, TEXT("Line : 16"), filename, MB_OK|MB_TOPMOST);
			}
		}
#endif
	}

	SI32 siclearcond = 0;
	SI32 data	= 0;

	pclHashTable->Find(clearcondtext, siclearcond, data, 400);

	

	clClearCond.siFlag = siclearcond;

	switch(siclearcond)
	{
	case QUEST_CLEAR_COND_ITEM:
	case QUEST_CLEAR_COND_MAKEITEM:
	case QUEST_CLEAR_COND_USEITEM:
	case QUEST_CLEAR_COND_SELLMARKET:
	case QUEST_CLEAR_COND_EQUIP:
	case QUEST_CLEAR_COND_SELLSWORDNPC:
	case QUEST_CLEAR_COND_TUTORIALMAKEITEM:
	case QUEST_CLEAR_COND_ITEMCHECK:
	case QUEST_CLEAR_COND_UPITEMTOMARKET:
	case QUEST_CLEAR_COND_SELLTRADER:
	case QUEST_CLEAR_COND_ENCHANTITEM:
	case QUEST_CLEAR_COND_HUNT_ITEM:
	case QUEST_CLEAR_COND_TAKEITEM_MATERIAL:
		{
			// �Ϸ� ���� �� ������ Ȯ���Ѵ�. 
			//TCHAR szclearconditemname[128];
			SI32 clearconditemunique;
			SI32 cleatconditemnum;

			_fgetts(buffer, sizeof(buffer), fp);

			//_stscanf(buffer, TEXT("%s %d"), szclearconditemname, &cleatconditemnum);
			_stscanf(buffer, TEXT("%d %d"), &clearconditemunique, &cleatconditemnum);

			// �������� �̸��� �ٰŷ� ����ũ�� Ȯ���Ѵ�. 
			//SI32 clearconditemunique = pclClient->pclItemManager->FindUniqueFromName(szclearconditemname);
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(clearconditemunique);
			if(ref > 0)
				//if ( clearconditemunique > 0 )
			{
				clClearCond.siPara1 = clearconditemunique;
				clClearCond.siPara2 = cleatconditemnum;
			}
			else
			{
				#ifdef _DEBUG
					MsgBox(filename, TEXT("QUEST_CLEAR_COND_ERROR:[%d] unique:%d"), clearconditemunique, questunique);
				#else
					MsgBox(TEXT("fd83fd"), TEXT("F34:[%d] unique:%d"), clearconditemunique, questunique);
				#endif
			}
		}
		break;

	// ä������ ������ ����ũ ������̵� �����ϵ��� ����.
	case QUEST_CLEAR_COND_FISH:
	case QUEST_CLEAR_COND_FISHNOSKILL:
	case QUEST_CLEAR_COND_FARM:
	case QUEST_CLEAR_COND_FARMNOSKILL:
	case QUEST_CLEAR_COND_MINE:
	case QUEST_CLEAR_COND_MINENOSKILL:
	
	case QUEST_CLEAR_COND_MAKE_RAREITEM:

	case QUEST_CLEAR_COND_BUYMARKET:
		{
			// �Ϸ� ���� �� ������ Ȯ���Ѵ�. 
			//TCHAR szclearconditemname[128];
			SI32 clearconditemunique;
			SI32 cleatconditemnum;

			_fgetts(buffer, sizeof(buffer), fp);

			//_stscanf(buffer, TEXT("%s %d"), szclearconditemname, &cleatconditemnum);
			_stscanf(buffer, TEXT("%d %d"), &clearconditemunique, &cleatconditemnum);

			// �������� �̸��� �ٰŷ� ����ũ�� Ȯ���Ѵ�. 
			//SI32 clearconditemunique = pclClient->pclItemManager->FindUniqueFromName(szclearconditemname);
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(clearconditemunique);
			if(ref > 0)
				//if ( clearconditemunique > 0 )
			{
				clClearCond.siPara1 = clearconditemunique;
				clClearCond.siPara2 = cleatconditemnum;
			}
			else
			{
				clClearCond.siPara1 = 0;
				clClearCond.siPara2 = cleatconditemnum;
			}
		}
		break;


	case QUEST_CLEAR_COND_KILLMON:
	case QUEST_CLEAR_COND_SUMMONBATTLE:
	case QUEST_CLEAR_COND_SELLNPC:
	case QUEST_CLEAR_COND_GONPC:
	case QUEST_CLEAR_COND_GETQUEST:
	case QUEST_CLEAR_COND_SEARCHMONSTERINFO:
	case QUEST_CLEAR_COND_PARTYKILLMON:
	case QUEST_CLEAR_COND_HITMON:
	case QUEST_CLEAR_COND_NPCINFO:
	case QUEST_CLEAR_COND_TWOWAYKILLMON:
	case QUEST_CLEAR_COND_TWOWAYKILLMONBONUS:
	case QUEST_CLEAR_COND_GONPC_INFORMATION:
	case QUEST_CLEAR_COND_TRANSMONSTERTRICERAMONS:
		{
			// �Ϸ� ���� �� ������ Ȯ���Ѵ�. 
			TCHAR szclearcondmonname[128];
			//SI32 clearcondkind;
			SI32 cleatcondmonnum;

			_fgetts(buffer, sizeof(buffer), fp);

			_stscanf(buffer, TEXT("%s %d"), szclearcondmonname, &cleatcondmonnum);
			//_stscanf(buffer, TEXT("%d %d"), &clearcondkind, &cleatcondmonnum);

			// ������ �̸��� �ٰŷ� Kind�� Ȯ���Ѵ�. 
			SI32 clearcondkind = pclClient->pclKindInfoSet->FindKindFromCode(szclearcondmonname);
			//if ( pclClient->pclKindInfoSet->IsValidKind(clearcondkind) == true )
			if(clearcondkind > 0)
			{
				clClearCond.siPara1 = clearcondkind;
				clClearCond.siPara2 = cleatcondmonnum;
			}
			else
			{
				#ifdef _DEBUG
				MsgBox(filename, TEXT("%s\n�ش� NPC ��ȣ�� �������� �ʽ��ϴ�"), szclearcondmonname);
				#else
					MsgBox(TEXT("123fd83fd"), TEXT("F34454:%d"), clearcondkind);
				#endif
			}

		}
		break;
		/*	case QUEST_CLEAR_COND_GETQUEST:
		{
		TCHAR para1[128];
		//SI32 para1;
		SI32 para2;

		_fgetts(buffer, 1024, fp);
		_stscanf(buffer, TEXT("%s %d"), para1, &para2);
		//_stscanf(buffer, TEXT("%d %d"), para1, &para2);

		clClearCond.siPara1	= pclClient->pclKindInfoSet->FindKindFromCode(para1);
		//clClearCond.siPara1 = para2;
		clClearCond.siPara2	= para2;
		}
		break;*/
	case QUEST_CLEAR_COND_ITEMINFO:
		{
			//TCHAR szclearconditemname[128];
			SI32 clearconditemunique;
			SI32 para2;

			_fgetts(buffer, sizeof(buffer), fp);

			//_stscanf(buffer, TEXT("%s %d"), szclearconditemname, &para2);
			_stscanf(buffer, TEXT("%d %d"), &clearconditemunique, &para2);

			// �������� �̸��� �ٰŷ� ����ũ�� Ȯ���Ѵ�. 
			//SI32 clearconditemunique = pclClient->pclItemManager->FindUniqueFromName(szclearconditemname);
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(clearconditemunique);
			if(ref > 0)
				//if ( clearconditemunique > 0 )
			{
				clClearCond.siPara1 = clearconditemunique;
				clClearCond.siPara2 =  para2;
			}
			else
			{
				#ifdef _DEBUG
					MsgBox(filename, TEXT("QUEST_CLEAR_COND_ITEMINFO:%d unique:%d"), clearconditemunique, questunique);
				#else
					MsgBox(TEXT("11123fd83fd"), TEXT("F34:%d unique:%d"), clearconditemunique, questunique);
				#endif
			}
		}
		break;
	case QUEST_CLEAR_COND_SKILL:		
	case QUEST_CLEAR_COND_BONUSZERO:
	case QUEST_CLEAR_COND_SUMMONBONUS:
	case QUEST_CLEAR_COND_SKILLBONUSZERO:
	case QUEST_CLEAR_COND_SCROLL:
	case QUEST_CLEAR_COND_USEWENNYOLDMAN2:
	case QUEST_CLEAR_COND_FEEDHORSE1:
	case QUEST_CLEAR_COND_FEEDHORSE2:
	case QUEST_CLEAR_COND_HORSESPEED:
	case QUEST_CLEAR_COND_MONEY:
	case QUEST_CLEAR_COND_OPENHEADCHAT:
	case QUEST_CLEAR_COND_BANK:
	case QUEST_CLEAR_COND_HUNGRY:
	case QUEST_CLEAR_COND_REPAIRWEAPON:
	case QUEST_CLEAR_COND_OPENCHARSTATUS:
	case QUEST_CLEAR_COND_OPENSTOCKMARKET:
	case QUEST_CLEAR_COND_LOOKPANSEO:
	case QUEST_CLEAR_COND_OPENVILLAGEBOARD:
	case QUEST_CLEAR_COND_OPENCITYHALL:
	case QUEST_CLEAR_COND_OPENHUNT:
	case QUEST_CLEAR_COND_SKILLMASTER:
	case QUEST_CLEAR_COND_BROADCAST:
	case QUEST_CLEAR_COND_NEWS:
	case QUEST_CLEAR_COND_OTHERMINIHOME:
	case QUEST_CLEAR_COND_BUYFROMPBSTORE:
	case QUEST_CLEAR_COND_APPLYVILLAGE:
	case QUEST_CLEAR_COND_USEFAN:
	case QUEST_CLEAR_COND_SENDITEMBYPOST:
	case QUEST_CLEAR_COND_REVIVESUMMON:
	case QUEST_CLEAR_COND_USEWARPTICKET1:
	case QUEST_CLEAR_COND_USEWARPTICKET2:
	case QUEST_CLEAR_COND_USEWENNYOLDMAN:
	case QUEST_CLEAR_COND_OPENPBSTORE:
	case QUEST_CLEAR_COND_STRUCTUREOPEN:
	case QUEST_CLEAR_COND_ITEMINFODLG:
	case QUEST_CLEAR_COND_OPENSKILL:
	case QUEST_CLEAR_COND_GOMAP:
	case QUEST_CLEAR_COND_LAMPGOMAP:		// ������� ���� ��ȸ
	case QUEST_CLEAR_COND_LEVEL:
	case QUEST_CLEAR_COND_FAMELEVEL:
	case QUEST_CLEAR_COND_FULLLIFE:
	case QUEST_CLEAR_COND_MAKEFRIEND:
	case QUEST_CLEAR_COND_STOCK:
	case QUEST_CLEAR_COND_BAG:
	case QUEST_CLEAR_COND_HORSE:
	case QUEST_CLEAR_COND_SUMMON:
	case QUEST_CLEAR_COND_SUMMON2:
	case QUEST_CLEAR_COND_BASICWEAPONSKILL:
	case QUEST_CLEAR_COND_CLEARQUEST:
	case QUEST_CLEAR_COND_EQUIPPOS:
	case QUEST_CLEAR_COND_DAILYQUEST:
	case QUEST_CLEAR_COND_SPECIALQUEST:
	case QUEST_CLEAR_COND_SUMMONEVOLUTION:
	case QUEST_CLEAR_COND_SUMMONSCROLL:
	case QUEST_CLEAR_COND_ITEMMALL:	
	case QUEST_CLEAR_COND_SETFATHER:
	case QUEST_CLEAR_COND_CLICKITEMINFO:
	case QUEST_CLEAR_COND_OPENMAKEITEM:
	case QUEST_CLEAR_COND_CLICKMAKEBUTTON:
	case QUEST_CLEAR_COND_OPENENCHANTITEM:
	case QUEST_CLEAR_COND_SETENCHANTITEM:
	case QUEST_CLEAR_COND_SETENCHANTELEMENT:
	case QUEST_CLEAR_COND_WARPTOPALACE:
	case QUEST_CLEAR_COND_SELLFROMPBSTORE:
	case QUEST_CLEAR_COND_PRIVATETRADE:	
	case QUEST_CLEAR_COND_MAKEBOOK:
	case QUEST_CLEAR_COND_HUNTMAPINFODLG:
	case QUEST_CLEAR_COND_PARTYHUNTING:
	case QUEST_CLEAR_COND_MAGICINFODLG:
	case QUEST_CLEAR_COND_OPENHOUSE:
	case QUEST_CLEAR_COND_OPENREALESTATEMARKET:
	case QUEST_CLEAR_COND_BLACKARMY:
	case QUEST_CLEAR_COND_OPENHORSEMARKET:
	case QUEST_CLEAR_COND_SUMMONEVOLUTIONRANK:
	case QUEST_CLEAR_COND_REVIVESUMMONBYDOLL:
	case QUEST_CLEAR_COND_OPENHOZODLG:
	case QUEST_CLEAR_COND_OPENBYUNGZODLG:
	case QUEST_CLEAR_COND_OPENGONGZODLG:
	case QUEST_CLEAR_COND_OPENIZODLG:
	case QUEST_CLEAR_COND_OPENGOONZUDLG:
	case QUEST_CLEAR_COND_VOTEKING:		
	case QUEST_CLEAR_COND_USEWARPTICKET3:
	case QUEST_CLEAR_COND_VILLAGEDETAILINFO:
	case QUEST_CLEAR_COND_USEMAGIC:
	case QUEST_CLEAR_COND_BUYTOPBSTORE:
	case QUEST_CLEAR_COND_NONE:
	case QUEST_CLEAR_COND_ENCHANTANYITEM:
	case QUEST_CLEAR_COND_OPENFRIEND:
	case QUEST_CLEAR_COND_OPENMASTER:
	case QUEST_CLEAR_COND_OPENMAKEREQUEST:
	case QUEST_CLEAR_COND_MAKEREQUESTSTART:
	case QUEST_CLEAR_COND_OPENNAMINGQUEST:
	case QUEST_CLEAR_COND_DISASSEMBLEITEM:
	case QUEST_CLEAR_COND_SUMMONACTIVE:
	case QUEST_CLEAR_COND_SUMMONDIE:
	case QUEST_CLEAR_COND_SUMMONCHANGESTATUS:
	case QUEST_CLEAR_COND_SUMMONATTACK:
	case QUEST_CLEAR_COND_SUMMONLEVELUP:
	case QUEST_CLEAR_COND_MAKEINTBOOK:
	case QUEST_CLEAR_COND_SETGREETING:
	case QUEST_CLEAR_COND_GOHUNTMAP:
	case QUEST_CLEAR_COND_MAGICACTIVE:
	case QUEST_CLEAR_COND_OPENGUILD:
	case QUEST_CLEAR_COND_APPLYGUILD:		
	case QUEST_CLEAR_COND_OPENGUILDUSER:
	case QUEST_CLEAR_COND_LOOKPRISON:
	case QUEST_CLEAR_COND_OPENGOONZUDLG_NOTICE:
	case QUEST_CLEAR_COND_MAKEPARTY:
	case QUEST_CLEAR_COND_SORTRESIDENT:		
	case QUEST_CLEAR_COND_CLICKITEMPROPERTY:
	case QUEST_CLEAR_COND_OPENSKILLETC:
	case QUEST_CLEAR_COND_SITDOWN:
	case QUEST_CLEAR_COND_SEARCHSTOCKINFO:
	case QUEST_CLEAR_COND_CLICKSTOCKOFFER:
	case QUEST_CLEAR_COND_CLICKCITYHALLFUND:
	case QUEST_CLEAR_COND_CLICKGENERALMETTING:
	case QUEST_CLEAR_COND_CLICKPERSONALSHOP:
	case QUEST_CLEAR_COND_OPENITEMMALL:
	case QUEST_CLEAR_COND_ANGELLISTCLICK:
	case QUEST_CLEAR_COND_POLICESETCLICK:
	case QUEST_CLEAR_COND_SEARCHFARM:
	case QUEST_CLEAR_COND_LEASECLICK:
	case QUEST_CLEAR_COND_GETMAGICLAMPQUEST:
	case QUEST_CLEAR_COND_OPENQUESTDIALOG:
	case QUEST_CLEAR_COND_ISLEARNED_MANUFACTURE:
	case QUEST_CLEAR_COND_ISGET_MANUFACTUREQUEST:
	case QUEST_CLEAR_COND_REQUESTMENTORSUCCESS:
	case QUEST_CLEAR_COND_TAKEPUPILS:
	case QUEST_CLEAR_COND_CHATTOBEGGINER:
	case QUEST_CLEAR_COND_ALLWEAPONSKILL:
	case QUEST_CLEAR_COND_MEMOCLICK:
	case QUEST_CLEAR_COND_MOVETOGROUPCLICK:
	case QUEST_CLEAR_COND_EQUIPBAG:
	case QUEST_CLEAR_COND_KILLDARKSOLDIER:
	case QUEST_CLEAR_COND_FULL_ORGAN:
	case QUEST_CLEAR_COND_OPENFATHERDIALOG:
	case QUEST_CLEAR_COND_ORDERTOMARKET:
	case QUEST_CLEAR_COND_OPENPOSTOFFICEDIALOG:
	case QUEST_CLEAR_COND_MAKELEVELITEM:
	case QUEST_CLEAR_COND_TAKEEQUIPLEVELWEAPON:
	case QUEST_CLEAR_COND_GETELEMENT_RESTORE:
	case QUEST_CLEAR_COND_GETPOWDER_RESTORE:
	case QUEST_CLEAR_COND_ENCHANTEQUIPLEVELWEAPON:
	case QUEST_CLEAR_COND_TAKEWEAPONHAVESLOT:
	case QUEST_CLEAR_COND_ARTIFACTMIXWEAPON:
	case QUEST_CLEAR_COND_TEACHERSET:
	case QUEST_CLEAR_COND_SUMMONBONUSZERO:
	case QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM:
	case QUEST_CLEAR_COND_SWORDSKILLCLICK:
	case QUEST_CLEAR_COND_LEARNWEAPONSKILL:
	case QUEST_CLEAR_COND_PVPWIN:
	case QUEST_CLEAR_COND_PVPLEAGUERESERVE:
	case QUEST_CLEAR_COND_PVPLEAGUEWIN:
	case QUEST_CLEAR_COND_ARTIFACTMIX:
	case QUEST_CLEAR_COND_SETMYKEY:
	case QUEST_CLEAR_COND_SUMMONHOUSE_INPUT:
	case QUEST_CLEAR_COND_SUMMONHOUSE_OUTPUT:
	case QUEST_CLEAR_COND_ENCHANTEQUIPPOS:
	case QUEST_CLEAR_COND_BUFFCLICK:
	case QUEST_CLEAR_COND_MAKESKILLEXPUP:
	case QUEST_CLEAR_COND_SEARCH_TOTALMAP:
	case QUEST_CLEAR_COND_USE_SKILLBOOK:
		{
			SI32 para1, para2;

			_fgetts(buffer, sizeof(buffer), fp);
			_stscanf(buffer, TEXT("%d %d"), &para1, &para2);

			clClearCond.siPara1	= para1;
			clClearCond.siPara2	= para2;
		}
		break;

	case QUEST_CLEAR_COND_CHECKSTATUS:
		{
			SI32	siPara1 = 0, siPara2 = 0;
			TCHAR	tszPara1[20] = { 0, };


			_fgetts(buffer, sizeof(buffer), fp);
			_stscanf(buffer, TEXT("%s %d"), tszPara1, &siPara2);

			if ( _tcscmp( tszPara1, TEXT("str") ) == 0 )			siPara1 = 1; // �ٷ�
			else if ( _tcscmp( tszPara1, TEXT("agi") ) == 0 )		siPara1 = 2; // ���߷�
			else if ( _tcscmp( tszPara1, TEXT("wis") ) == 0 )		siPara1 = 3; // ����
			else if ( _tcscmp( tszPara1, TEXT("vit") ) == 0 )		siPara1 = 4; // ������
			else if ( _tcscmp( tszPara1, TEXT("dex") ) == 0 )		siPara1 = 5; // ������
			else if ( _tcscmp( tszPara1, TEXT("luck") ) == 0 )		siPara1 = 6; // ���
			else if ( _tcscmp( tszPara1, TEXT("speed") ) == 0 )		siPara1 = 7; // �̵��ӵ�
			else if ( _tcscmp( tszPara1, TEXT("attack") ) == 0 )	siPara1 = 8; // ���ݷ�
			else if ( _tcscmp( tszPara1, TEXT("defense") ) == 0 )	siPara1 = 9; // ����
			else													siPara1 = 0; // �߸��Ǿ���

			clClearCond.siPara1	= siPara1;
			clClearCond.siPara2	= siPara2;
		}
		break;

	case QUEST_CLEAR_COND_MAKECRYSTAL:
		{
			SI32 para1, para2;

			_fgetts(buffer, sizeof(buffer), fp);
			_stscanf(buffer, TEXT("%d %d"), &para1, &para2);

#ifdef _DEBUG
			if ( (CRYSTAL_GRADE_NONE >= para1) || (CRYSTAL_GRADE_MAX <= para1) )
			{
				MessageBox( NULL, TEXT("QUEST_CLEAR_COND_MAKECRYSTAL ����"), TEXT("Para1�� ��޼����� �߸��Ǿ����ϴ�"), MB_OK );
			}
#endif

			clClearCond.siPara1	= para1;
			clClearCond.siPara2	= para2;
		}
		break;

	case QUEST_CLEAR_COND_MARKETCONDITION:
		{
			SI32	siPara1		= 0;
			SI32	siPara2		= 0;
			TCHAR	szPara1[64]	= TEXT("");
			
			_fgetts(buffer, sizeof(buffer), fp);
			_stscanf(buffer, TEXT("%s %d"), szPara1, &siPara2);

			if ( _tcscmp( szPara1, TEXT("ITEMTYPE_SWORD") ) == 0 )						siPara1 = ITEMTYPE_SWORD;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_SPEAR") ) == 0 )					siPara1 = ITEMTYPE_SPEAR;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_AXE") ) == 0 )					siPara1 = ITEMTYPE_AXE;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_BOW") ) == 0 )					siPara1 = ITEMTYPE_BOW;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_GUN") ) == 0 )					siPara1 = ITEMTYPE_GUN;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_STAFF") ) == 0 )					siPara1 = ITEMTYPE_STAFF;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_HELMET") ) == 0 )				siPara1 = ITEMTYPE_HELMET;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_ARMOUR") ) == 0 )				siPara1 = ITEMTYPE_ARMOUR;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_BELT") ) == 0 )					siPara1 = ITEMTYPE_BELT;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_SHOES") ) == 0 )					siPara1 = ITEMTYPE_SHOES;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_NECK") ) == 0 )					siPara1 = ITEMTYPE_NECK;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_RING") ) == 0 )					siPara1 = ITEMTYPE_RING;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_BAG") ) == 0 )					siPara1 = ITEMTYPE_BAG;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_HAT") ) == 0 )					siPara1 = ITEMTYPE_HAT;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_DRESS") ) == 0 )					siPara1 = ITEMTYPE_DRESS;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_MANTLE") ) == 0 )				siPara1 = ITEMTYPE_MANTLE;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_FOOD") ) == 0 )					siPara1 = ITEMTYPE_FOOD;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_HEAL") ) == 0 )					siPara1 = ITEMTYPE_HEAL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_BOOK") ) == 0 )					siPara1 = ITEMTYPE_BOOK;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_BOX") ) == 0 )					siPara1 = ITEMTYPE_BOX;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_MINERAL") ) == 0 )				siPara1 = ITEMTYPE_MINERAL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_TREE") ) == 0 )					siPara1 = ITEMTYPE_TREE;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_CLOTH") ) == 0 )					siPara1 = ITEMTYPE_CLOTH;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_SEAFOOD") ) == 0 )				siPara1 = ITEMTYPE_SEAFOOD;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_CRYSTAL") ) == 0 )				siPara1 = ITEMTYPE_CRYSTAL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_COMPOSITIONMATERIAL") ) == 0 )	siPara1 = ITEMTYPE_COMPOSITIONMATERIAL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_PAPER") ) == 0 )					siPara1 = ITEMTYPE_PAPER;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_IRON") ) == 0 )					siPara1 = ITEMTYPE_IRON;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_DISH") ) == 0 )					siPara1 = ITEMTYPE_DISH;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_FISHINGROD") ) == 0 )			siPara1 = ITEMTYPE_FISHINGROD;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_DOLL") ) == 0 )					siPara1 = ITEMTYPE_DOLL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_ANIMAL") ) == 0 )				siPara1 = ITEMTYPE_ANIMAL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_TICKET") ) == 0 )				siPara1 = ITEMTYPE_TICKET;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_PREMIUM") ) == 0 )				siPara1 = ITEMTYPE_PREMIUM;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_EVENTITEM") ) == 0 )				siPara1 = ITEMTYPE_EVENTITEM;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_ETC") ) == 0 )					siPara1 = ITEMTYPE_ETC;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_SCROLL") ) == 0 )				siPara1 = ITEMTYPE_SCROLL;
			else if ( _tcscmp( szPara1, TEXT("ITEMTYPE_QUEST") ) == 0 )					siPara1 = ITEMTYPE_QUEST;
			else																		siPara1 = 0;

#ifdef _DEBUG
			if ( 0 >= siPara1 )
			{
				MessageBox( NULL, TEXT("QUEST_CLEAR_COND_MARKETCONDITION ����"), TEXT("Para1 ������ Ÿ�Լ��� ����"), MB_OK );
			}
#endif
			
			clClearCond.siPara1	= siPara1;
			clClearCond.siPara2	= siPara2;
			
		}
		break;
	case QUEST_CLEAR_COND_MAPKILLMON:
		{
			SI32	siPara1		= 0;
			SI32	siPara2		= 0;

			_fgetts(buffer, sizeof(buffer), fp);
			_stscanf(buffer, TEXT("%d %d"), &siPara1, &siPara2);

#ifdef _DEBUG
			if ( NULL == pclClient->pclMapManager->GetMapName(siPara1) )
			{
				MessageBox( NULL, TEXT("QUEST_CLEAR_COND_MAPKILLMON ����"), TEXT("Para1 �� �ε��� ���� ����"), MB_OK );
			}
#endif
			clClearCond.siPara1	= siPara1;
			clClearCond.siPara2	= siPara2;
		}
		break;

	default:
		MsgBox(TEXT("ClearCondError"), TEXT("%s %d"), clearcondtext, unique);
		break;
	}


	//------------------------------------------------------------------------------------------------------------------------------
	// ���� ���� �о� ����
	//------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------
	// 3�� ����. 
	//-------------------------------------
	for(i = 0;i < 3;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if( i == 1 )
		{
			if(_tcsncmp(buffer, "Reward_Other", 12) != 0)
			{
				MessageBox(NULL, TEXT("Line : 19"), filename, MB_OK|MB_TOPMOST);
			}
		}
		if( i == 2 )
		{
			if(_tcsncmp(buffer, "Money", 5) != 0)
			{
				MessageBox(NULL, TEXT("Line : 20"), filename, MB_OK|MB_TOPMOST);
				fclose(fp);
			}
		}
#endif
	}

	//-------------------------------------
	// �������� ������ ������� �׸� ����
	//-------------------------------------
	SI32 siMoney		= 0;	// ��
	SI32 siApplyNPCRate	= 0;	// ���� �����Ҷ� �ü��� ������ �޴��� ����
	SI32 siExp			= 0;	// ����ġ
	SI32 siNCoin		= 0;	// ������(���ǰ� ���� ����)
	SI32 siMoveBuf		= 0;	// �̼ӹ������󿩺�
	SI32 siSelectItem	= 0;	// ������ ���� ����(���ǰ� ���� ����)
	SI32 siSkillUnique	= 0;	// ��ų ����ġ�� �ش�(��ų ����ũ)
	SI32 siSkillExp		= 0;	// ��ų ����ġ�� �ش�(��ų ����ġ)
	SI32 siGetMeterial	= 0;	// ������ ������ �ش� �������� ���� �� �ִ� ���� �ִ��� ����


	_fgetts( buffer, sizeof(buffer), fp );
	_stscanf( buffer, TEXT("%d %d %d %d %d %d %d %d %d"), 
		&siMoney,			&siApplyNPCRate,		&siExp,	
		&siNCoin,			&siMoveBuf,				&siSelectItem,
		&siSkillUnique,		&siSkillExp,			&siGetMeterial 
	);


	clReward.siMoney		= siMoney;
	clReward.bApplyNpcRate	= DATA_TO_BOOL(siApplyNPCRate);

	clReward.siExp			= siExp;

	clReward.siNCoin		= siNCoin;

	clReward.bMoveBuf		= DATA_TO_BOOL(siMoveBuf);

	clReward.bSelectItem	= DATA_TO_BOOL(siSelectItem);
    
	clReward.siSkillType	= siSkillUnique;
	clReward.siSkillExp		= siSkillExp;

	clReward.bIsGetMaterial	= DATA_TO_BOOL(siGetMeterial);

	clReward.siFameExp		= 2;		// ��� ����Ʈ�� �⺻������ 2�� ���� ����ġ�� �ο����ش�. 
	if(questtype == QUEST_TYPE_MAKEBEGINNER1)
	{
		clReward.siFameExp	= 0;		// ���� Ʃ�丮���� ���� ����ġ�� ���ش�. �ʹ� ���⶧����.
	}

	clReward.bDeBuff		= false;	// ����� ����Ʈ�� ������ false���·� �ʱ�ȭ �ȴ�. (�Ŀ� �ʿ��ϸ� ���ҽ��� �÷� �߰�)

	//-------------------------------------
	// �������� ������ ������� �׸� �������
	//-------------------------------------

	//-------------------------------------
	// ������ ������� ����
	//-------------------------------------
	//-------------------------------------
	// 3�� ����. 
	//-------------------------------------
	for(i = 0;i < 3;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if( i == 1 )
		{
			if(_tcsncmp(buffer, "Reward_Item", 11) != 0)
			{
				MessageBox(NULL, TEXT("Line : 23"), filename, MB_OK|MB_TOPMOST);
			}
		}
		if( i == 2 )
		{
			if(_tcsncmp(buffer, "ItemUnique1", 11) != 0)
			{
				MessageBox(NULL, TEXT("Line : 24"), filename, MB_OK|MB_TOPMOST);
				fclose(fp);
			}
		}
#endif
	}

	// tys - ������ ���� �迭�� ����......
	SI32 siItemUnique[MAX_QUEST_REWARD_NUMBER]	= {0,};
	SI32 siItemNum[MAX_QUEST_REWARD_NUMBER]		= {0,};
	SI32 siItemUseDate[MAX_QUEST_REWARD_NUMBER]	= {0,};


	_fgetts( buffer, sizeof(buffer), fp );
	_stscanf( buffer, TEXT("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d")
		,&siItemUnique[0],	&siItemNum[0],	&siItemUseDate[0]
		,&siItemUnique[1],	&siItemNum[1],	&siItemUseDate[1]
		,&siItemUnique[2],	&siItemNum[2],	&siItemUseDate[2]
		,&siItemUnique[3],	&siItemNum[3],	&siItemUseDate[3]
		,&siItemUnique[4],	&siItemNum[4],	&siItemUseDate[4]
		,&siItemUnique[5],	&siItemNum[5],	&siItemUseDate[5]
	);

	// ������ ���� ����..
	for ( i=0; i<MAX_QUEST_REWARD_NUMBER; ++i )
	{
		if( siItemUnique[i] > 0 )
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(siItemUnique[i]);
			if ( ref )
			{
				clReward.siItemUnique[i]	= siItemUnique[i];
				clReward.siItemNum[i]		= siItemNum[i];

				//cyj ����Ʈ ������ �Ⱓ ���� ���ҽ� �⺻ �Ⱓ���� ����
				if (siItemUseDate[i] > 0)
				{
					clReward.siItemUseDate[i] = siItemUseDate[i];
				}
				else
				{
					clReward.siItemUseDate[i] = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;
				}
			}
			else
			{
				#ifdef _DEBUG
					MsgBox(filename, TEXT("Invalid ItemUnique - QuestUnique:%d, ItemUnique: %d"),	questunique, siItemUnique[i]);
				#else
					MsgBox(TEXT("fds8vd"), TEXT("fds3:%d %d"),	questunique, siItemUnique[i]);
				#endif
			}
		}
	}

	//-------------------------------------
	// ������ ������� �������
	//-------------------------------------


	SI32 bit = GetBitFromUnique(questtype,questunique);
	SI32 textlength = 0 ;

	//if(questtype == QUEST_TYPE_BEGINNER5 && pclClient->siServiceArea == ConstServiceArea_Korea && bNewQuestDoing == TRUE)
	//{
	//	//-------------------------------------
	//	// 2�� ����. 
	//	//-------------------------------------
	//	_fgetts(buffer, sizeof(buffer), fp);
	//	_fgetts(buffer, sizeof(buffer), fp);

	//	_fgetts(buffer, sizeof(buffer), fp);
	//	_stscanf(buffer, TEXT("%d"), &clQuestTxtInfo[questtype][bit].siStartConversationNum);

	//	TCHAR ConversationHead[50] = TEXT("");
	//	TCHAR Conversationbuffer[1024] = TEXT("");
	//	SI32 textlength = 0 ;
	//	SI32 SprFontIndex = 0;

	//	for(i=0; i<clQuestTxtInfo[questtype][bit].siStartConversationNum; ++i)
	//	{
	//		_fgetts(buffer, sizeof(buffer), fp);
	//		_stscanf(buffer, TEXT("%s %d"), ConversationHead, &SprFontIndex);
	//		MStrCpy(Conversationbuffer,&buffer[12],1024);

	//		textlength = _tcslen(Conversationbuffer) ;

	//		for ( SI32 k = 0 ; k < textlength ; k ++ )
	//		{
	//			if ( (Conversationbuffer[k] == 92 && Conversationbuffer[k+1] == 114) && Conversationbuffer[k+1] != NULL ) // 92���� TEXT("\"), 114���� "r"
	//			{
	//				Conversationbuffer[k] = '\r';
	//				Conversationbuffer[k+1] = '\n';
	//			}
	//		}
	//		MStrCpy(clQuestTxtInfo[questtype][bit].szStartQuestConversation[i],Conversationbuffer,1024);
	//		clQuestTxtInfo[questtype][bit].siStartSprIndex[i] = SprFontIndex;
	//	}

	//	//-------------------------------------
	//	// 1�� ����. 
	//	//-------------------------------------
	//	_fgetts(buffer, sizeof(buffer), fp);
	//	// Middle
	//	TCHAR txtbuffer[2048] = TEXT("") ;
	//	_fgetts(buffer, sizeof(buffer), fp);
	//	MStrCpy(txtbuffer,&buffer[11],1024);
	//	textlength = _tcslen(txtbuffer) ;
	//	for ( SI32 k = 0 ; k < textlength ; k ++ )
	//	{
	//		if ( (txtbuffer[k] == 92 && txtbuffer[k+1] == 114) && txtbuffer[k+1] != NULL ) // 92���� TEXT("\"), 114���� "r"
	//		{
	//			txtbuffer[k] = '\r';
	//			txtbuffer[k+1] = '\n';
	//		}
	//	}
	//	MStrCpy(clQuestTxtInfo[questtype][bit].szQuestMiddle,txtbuffer,1024);

	//	//-------------------------------------
	//	// 2�� ����. 
	//	//-------------------------------------
	//	_fgetts(buffer, sizeof(buffer), fp);
	//	_fgetts(buffer, sizeof(buffer), fp);

	//	_fgetts(buffer, sizeof(buffer), fp);
	//	_stscanf(buffer, TEXT("%d"), &clQuestTxtInfo[questtype][bit].siEndConversationNum);

	//	for(i=0; i<clQuestTxtInfo[questtype][bit].siEndConversationNum; ++i)
	//	{
	//		_fgetts(buffer, sizeof(buffer), fp);
	//		_stscanf(buffer, TEXT("%s %d"), ConversationHead, &SprFontIndex);
	//		MStrCpy(Conversationbuffer,&buffer[12],1024);

	//		textlength = _tcslen(Conversationbuffer) ;

	//		for ( SI32 k = 0 ; k < textlength ; k ++ )
	//		{
	//			if ( (Conversationbuffer[k] == 92 && Conversationbuffer[k+1] == 114) && Conversationbuffer[k+1] != NULL ) // 92���� TEXT("\"), 114���� "r"
	//			{
	//				Conversationbuffer[k] = '\r';
	//				Conversationbuffer[k+1] = '\n';
	//			}
	//		}
	//		MStrCpy(clQuestTxtInfo[questtype][bit].szEndQuestConversation[i],Conversationbuffer,1024);
	//		clQuestTxtInfo[questtype][bit].siEndSprIndex[i] = SprFontIndex;
	//	}
	//}
	//else
	//{
		//-------------------------------------
		// 1�� ����. 
		//-------------------------------------
		_fgetts(buffer, sizeof(buffer), fp);

		// Greeting , Middle , Clear ������
		TCHAR txtbuffer[2048] = TEXT("") ;
		NTCHARString<2048> kQuestText;
		NTCHARString<64>   kKey;
		// Greeting
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if(_tcsncmp(buffer, "GreetingText", 12) != 0)
		{
			MessageBox(NULL, TEXT("Line : 23"), filename, MB_OK|MB_TOPMOST);
		}

		if( m_pQuestText == NULL )		MessageBox(NULL, TEXT("m_pQuestText is NULL"), filename, MB_OK|MB_TOPMOST);
#endif
		/*MStrCpy(txtbuffer,&buffer[13],1024);
		textlength = _tcslen(txtbuffer) ;
		for ( SI32 k = 0 ; k < textlength ; k ++ )
		{
			if ( (txtbuffer[k] == 92 && txtbuffer[k+1] == 114) && txtbuffer[k+1] != NULL ) // 92���� TEXT("\"), 114���� "r"
			{
				txtbuffer[k] = '\r';
				txtbuffer[k+1] = '\n';
			}
		}*/
		kKey  = SI32ToString( questunique );
		kKey += "_Greeting";
		kQuestText = m_pQuestText->GetText( kKey );
		kQuestText.ReplaceAll( "\\r", "\r\n" );
		//MStrCpy(clQuestTxtInfo[questtype][bit].szQuestGreeting, txtbuffer,1024);
		MStrCpy(clQuestTxtInfo[questtype][bit].szQuestGreeting, kQuestText, 1024);
		// Middle
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if(_tcsncmp(buffer, "MiddleText", 10) != 0)
		{
			MessageBox(NULL, TEXT("Line : 24"), filename, MB_OK|MB_TOPMOST);
		}
#endif
		/*MStrCpy(txtbuffer,&buffer[11],1024);
		textlength = _tcslen(txtbuffer) ;
		for ( SI32 k = 0 ; k < textlength ; k ++ )
		{
			if ( (txtbuffer[k] == 92 && txtbuffer[k+1] == 114) && txtbuffer[k+1] != NULL ) // 92���� TEXT("\"), 114���� "r"
			{
				txtbuffer[k] = '\r';
				txtbuffer[k+1] = '\n';
			}
		}
		MStrCpy(clQuestTxtInfo[questtype][bit].szQuestMiddle,txtbuffer,1024);*/
		kKey  = SI32ToString( questunique );
		kKey += "_Middle";
		kQuestText = m_pQuestText->GetText( kKey );
		kQuestText.ReplaceAll( "\\r", "\r\n" );
		MStrCpy(clQuestTxtInfo[questtype][bit].szQuestMiddle, kQuestText, 1024);
		// Clear
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if(_tcsncmp(buffer, "ClearText", 9) != 0)
		{
			MessageBox(NULL, TEXT("Line : 25"), filename, MB_OK|MB_TOPMOST);
		}
#endif
		/*MStrCpy(txtbuffer,&buffer[10],1024);
		textlength = _tcslen(txtbuffer) ;
		for ( SI32 k = 0 ; k < textlength ; k ++ )
		{
			if ( (txtbuffer[k] == 92 && txtbuffer[k+1] == 114) && txtbuffer[k+1] != NULL ) // 92���� TEXT("\"), 114���� "r"
			{
				txtbuffer[k] = '\r';
				txtbuffer[k+1] = '\n';
			}
		}
		MStrCpy(clQuestTxtInfo[questtype][bit].szQuestClear,txtbuffer,1024);*/
		kKey  = SI32ToString( questunique );
		kKey += "_Clear";
		kQuestText = m_pQuestText->GetText( kKey );
		kQuestText.ReplaceAll( "\\r", "\r\n" );
		MStrCpy(clQuestTxtInfo[questtype][bit].szQuestClear, kQuestText, 1024);
	//}

	//-------------------------------------
	// 3�� ����. 
	//-------------------------------------
	for(i = 0;i < 3;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
#ifdef _DEBUG
		if( i == 1 )
		{
			if(_tcsncmp(buffer, "InitCondition", 13) != 0)
			{
				MessageBox(NULL, TEXT("Line : 27"), filename, MB_OK|MB_TOPMOST);
			}
		}
		if( i == 2 )
		{
			if(_tcsncmp(buffer, "Level", 5) != 0)
			{
				MessageBox(NULL, TEXT("Line : 28"), filename, MB_OK|MB_TOPMOST);
			}
		}
#endif
	}

	// ����. 
	SI32 level = 0;
	SI32 skillunique = 0;
	SI32 skilllevel = 0;
	SI32 mustclearquestunique = 0;

	_fgetts(buffer, sizeof(buffer), fp);
	_stscanf(buffer, TEXT("%d %d %d %d"), &level , &skillunique, &skilllevel, &mustclearquestunique );

	if(level < 0)level				= 0;
	if(skillunique < 0)skillunique	= 0;
	if(skilllevel < 0)skilllevel	= 0;
	if(mustclearquestunique < 0) mustclearquestunique = 0;

	clinitcond.siLevel			= level;
	clinitcond.siSkillUnique	= skillunique;
	clinitcond.siSkillLevel		= skilllevel;
	clinitcond.siMustClearQuestUnique = mustclearquestunique;

	//--------------------------------------
	// 3�� ���� 
	//--------------------------------------
	for(i = 0;i < 3;i++)
	{
		_fgetts(buffer, sizeof(buffer), fp);
	}

	TCHAR* pimagefilename = NULL;
	// �̹���. 
	/*	TCHAR imagefileswitch[128] = TEXT("") ;
	TCHAR imagefilename[128] = TEXT("") ;
	_fgetts(buffer, 1024, fp);
	_stscanf(buffer, TEXT("%s"), imagefileswitch);

	if(_tcscmp(imagefileswitch, TEXT("TRUE"))==0)
	{
	sprintf(imagefilename, TEXT("Quest\\%s\\%03d.bmp"), szQuestTypeName[questtype], questunique);
	pimagefilename = imagefilename;
	}
	else
	{
	pimagefilename = NULL;
	}*/

	// SPR �̹���

	SI32 greetingspr = -1 ;
	SI32 middlespr = -1 ;
	SI32 clearspr = -1 ;

	_fgetts(buffer, sizeof(buffer), fp);
	_stscanf(buffer, TEXT("%d %d %d"), &greetingspr , &middlespr, &clearspr);
	clQuestTxtInfo[questtype][bit].siGreetingSpr = greetingspr ;
	clQuestTxtInfo[questtype][bit].siMiddleSpr = middlespr ;
	clQuestTxtInfo[questtype][bit].siClearSpr = clearspr ;
	
	// ���� ����
/*
	SI32 buf = 0;
	
	// ���� ���� ������ �߰��� �������
	if(_fgetts(buffer, 1024, fp) != NULL)
	{
		if(_fgetts(buffer, 1024, fp) != NULL)
		{
			_stscanf(buffer, TEXT("%d"), &buf);

			if( buf == 1)
			{
				clReward.bMoveBuf = TRUE;
			}

		}
	}
*/
	





	fclose(fp);

	Add(questtype, questunique, &clinitcond, &clClearCond, &clReward,
		kTitle,
		pimagefilename);

	return true;
}

// ����Ʈ �ؽ�Ʈ�� �ҷ��´�. 
bool cltQuestManager::LoadQuestText()
{
	SI32 i, j;

	m_pQuestText = new NTextManager;
	m_pQuestText->CreateTextManager();

	/*
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		m_pQuestText->LoadTextFromCryptFile(TEXT("TextMgr\\QuestTextMgr.txt"), pclClient->GetTextColumn());	
	}
	else
	{
		m_pQuestText->LoadTextFromFile(TEXT("TextMgr\\QuestTextMgr.txt"), pclClient->GetTextColumn());
	}
	*/

	TCHAR szFileName[MAX_PATH] = TEXT("");

#ifdef _DEV
	StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_DEV.txt") );
#else
	switch ( pclClient->siServiceArea )
	{
	case ConstServiceArea_Korea:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_KOR.txt") );
		}
		break;
	case ConstServiceArea_Japan:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_JPN.txt") );
		}
		break;
	case ConstServiceArea_China:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_CHI.txt") );
		}
		break;
	case ConstServiceArea_English:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_ENG.txt") );
		}
		break;
	case ConstServiceArea_Taiwan:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_TAI.txt") );
		}
		break;
	case ConstServiceArea_USA:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_ENG.txt") );
		}
		break;
	case ConstServiceArea_NHNChina:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_CHI.txt") );
		}
		break;
	case ConstServiceArea_EUROPE:
		{
			StringCchCopy( szFileName, MAX_PATH, TEXT("TextMgr\\TextMgr_Quest_EUR.txt") );
		}
		break;
	}
#endif

	// Plaintext text is the permanent choice for this deployment (see
	// cltCommonLogic::Create) - the NCR decrypt path is not used.
	m_pQuestText->LoadTextFromFile( szFileName, 1 );


	for(i = 0;i < MAX_QUEST_TYPE;i++)
	{
		for(j = 0;j < MAX_QUEST_PER_TYPE_NUMBER;j++)
		{
			if(siQuestBitInfo[i][j])
			{
				LoadQuestText(i, siQuestBitInfo[i][j]);
			}
		}
	}

	delete m_pQuestText;
	m_pQuestText = NULL;

	return true;
}

// ����Ʈ�� ����ũ�� ��Ʈ�� �����Ѵ�.
bool cltQuestManager::ConnectBitWithUnique()
{
	SI32 i;
	FILE* fp;
	TCHAR buffer[1024];
	SI32 questtype = QUEST_TYPE_BATTLE1;	

	TCHAR QuestFileName[][128] ={
		TEXT("Special"),		// ������ ����.
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
		TEXT("HuntingETC01"),
		TEXT("Naming"),		//������ ����
		TEXT("HappyBean"),	//������ ����
		TEXT("HuntingGround01"),
		TEXT("HuntingGround02"),
		TEXT("MakeHat"),
		TEXT("MakeCake"),
		TEXT("PlayChristmas"),
		TEXT("HuntingGround03"),	// �޸���
		TEXT("HuntingGround04"),	// �����
		TEXT("HuntingGround05"),	// ����Ÿ
		TEXT("HuntingGround06"),	// ���Ͼ
		TEXT("HuntingGround07"),	// �̶�
		TEXT("HuntingGround08"),	// ������
		TEXT("HuntingGround09"),	// ����
		TEXT("HuntingGround10"),	// ���긮��
		TEXT("HuntingGround11"),	// �ϸ��Ƴ�
		TEXT("MakeFood1"),			// ��������
		TEXT("Beginner1"),			// ���̸�
		TEXT("Dailyquest2"),		// ������ ����
		TEXT("Dailyquest3"),		// ������ ����
		TEXT("Makeartifact1"),		// ��Ƽ��Ʈ ����Ʈ(�ŵ�)
		TEXT("MISSION1"),			// ������ ����
		TEXT("HuntingGround12"),	// �����帯
		TEXT("HuntingGround13"),	// ���̵�
		TEXT("FindTreasure"),		// ����ã��
		TEXT("HuntingGround14"),	// �Ҿ���� ����		
		TEXT("AvatarDress"),		// �ƹ�Ÿ �巹��
		TEXT("AvatarHat"),			// �ƹ�Ÿ ����
		TEXT("AvatarMantle"),		// �ƹ�Ÿ ����
		TEXT("AvatarNeck"),			// �Ҿ���� ����
		TEXT("AvatarRing"),			// �ƹ�Ÿ ����
		TEXT("PlayChristmasHidden"),// ũ�������� ����Ʈ4
		TEXT("HuntingGround15"),// ũ�������� ����Ʈ4

		//---------������ �߰� ���ּ���---------------------//
		TEXT("NULL")
	};


	while(_tcscmp(QuestFileName[questtype], TEXT("NULL")) != 0)
	{
		if ( _tcscmp(QuestFileName[questtype], TEXT("Naming"))		== 0 )	questtype++;
		if ( _tcscmp(QuestFileName[questtype], TEXT("HappyBean"))	== 0 )	questtype++;
		if ( _tcscmp(QuestFileName[questtype], TEXT("Dailyquest2")) == 0 )	questtype++;
		if ( _tcscmp(QuestFileName[questtype], TEXT("Dailyquest3")) == 0 )	questtype++;
		if ( _tcscmp(QuestFileName[questtype], TEXT("MISSION1"))	== 0 )	questtype++;


		StringCchPrintf(buffer, 1024, TEXT("Quest\\%s.txt"), QuestFileName[questtype]);

		fp = _tfopen(buffer, TEXT("rt"));

		if(fp == NULL )
		{
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox(NULL, TEXT("QuestInfo Load Error"), buffer, MB_OK|MB_TOPMOST);
			return false;
		}

		// ����Ʈ Ÿ�� �̸��� Ȯ���Ѵ�. 
		StringCchCopy(szQuestTypeName[questtype], 128, QuestFileName[questtype]);

		// ù��° 8�� �����Ѵ�. 
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

			siQuestBitInfo[questtype][bit]	= unique;
		}

		fclose(fp);

		questtype++;

	}

	return true;

}
