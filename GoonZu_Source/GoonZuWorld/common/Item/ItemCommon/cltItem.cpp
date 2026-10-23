//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#include "cltItem.h"

#include "..\..\Skill\Skill.h"
#include "../../CommonHeader.h"
#include "../../../common/Char/CharCommon/Char-Common.h"
#include "../../../CommonLogic/MagicMgr/MagicMgr.h"
#include "../../../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"


//---------------------------------------
// 아이템 타입 정보. 
//---------------------------------------	
cltItemTypeInfo::cltItemTypeInfo(SI32 siunique, SI64 siatb, SI64 siattach, const TCHAR* typecode, const TCHAR* pname)
{
	siUnique	= siunique;
	siAtb		= siatb;
	siAttach	= siattach;
	MStrCpy(szName,		pname,		MAX_ITEMTYPE_NAME_LENGTH );
	MStrCpy(szTypeCode, typecode,	MAX_ITEMTYPE_NAME_LENGTH );
}

cltItemTypeInfo::~cltItemTypeInfo()
{

}

BOOL cltItemTypeInfo::IsAtb(SI64 atb)const
{
	if(siAtb & atb)return TRUE;

	return FALSE;
}
BOOL cltItemTypeInfo::IsAttach(SI64 atc)const
{
	if(siAttach& atc)return TRUE;

	return FALSE;
}



//---------------------------------------
// 아이템 Unique별 정보. 
//---------------------------------------
cltItemInfo::cltItemInfo(SI32 imageindex, SI32 smallimageindex, TCHAR* name, SI32 sitype, SI32 weight,
						 cltItemSkillInfo* pclskillinfo, cltItemConition* pclcondition, TCHAR* armcode,
						 TCHAR* material, TCHAR* tool, SI32 productunit, SI32 soundtype, cltItem* pclItem,
						 SI32 Grade, SI32 TradeServerMinNum, cltCrystalItemData *CrystalItemData, bool bNewMarket, bool  ImmutableUseDate )
{

	ZeroMemory(this, sizeof(cltItemInfo));

	siType				= sitype;
	siImageIndex		= imageindex;
	siSmallImageIndex	= smallimageindex;
	siWeight			= weight;
	siSoundType			= soundtype;
	siItemGrade			= Grade;

	siItemInfoAtb		= 0;

	clSkillInfo.Set(pclskillinfo);
	clCondition.Set(pclcondition);

	if(armcode == NULL)
	{
		StringCchCopy(szArmCode, 24, TEXT(""));
	}
	else
	{
		StringCchCopy(szArmCode, 24, armcode);
	}

	if(material)
	{
		pszMaterialText = new TCHAR [ _tcslen(material) + 1];
		StringCchCopy(pszMaterialText, _tcslen(material) + 1, material);
	}
	else
	{
		pszMaterialText = NULL;
	}

	if(tool)
	{
		pszToolText = new TCHAR [ _tcslen(tool) + 1];
		StringCchCopy(pszToolText, _tcslen(tool) + 1, tool);
	}
	else
	{
		pszToolText = NULL;
	}

	// 생산의 단위를 설정한다. 
	clProductionInfo.siProductUnit = productunit;

	clItem.Set(pclItem);

	if(name)
	{
		MStrCpy(szName, name, MAX_ITEM_NAME_LENGTH );
	}
	else
	{
		StringCchCopy(szName, MAX_ITEM_NAME_LENGTH, TEXT("NONAMEITEM"));
	}

	siNeedRank		= 0;
	siDecDur		= 0;
	siExplainUnique	= 0;
	
	siAdapt			= ADAPT_MAN;
	bCanEquipSwitch	= true;

	// 무기류인 경우 
	siDefaultHitRate = 0;

	// 음식인 경우, 
	siHungry		= 0;

	siHorseFoodSwitch	= 0;

	bFishingBaitSwitch	= false;
	siOrg1Penalty		= 0;
	siOrg2Penalty		= 0;
	siOrg3Penalty		= 0;
	siOrg4Penalty		= 0;
	
	// 목걸이인 경우
	siUpgradeSkillUnique	= 0;
	siUpgradeSkillType		= 0;

	// 약품인 경우, 
	siDiseaseUnique		= 0;		// 질병 코드. 
	siHealEft			= 0;
	StringCchCopy( szDiseaseName, 128, TEXT("") );

	// 책인 경우, 
	siSkillUnique		= 0;
	siSkillLevelLimit	= 0;

	//---------------------
	// 부적인 경우 
	//---------------------
	siScrollAdaptAttach	= 0;
	siScrollFunction	= 0;		// 부적의 기능. 
	siScrollAbility		= 0;		// 주문의 기능정도. 
	siScrollDecDuration	= 0;		// 주문의 소모 내구도. 
	siScrollMax			= 0;		// 능력의 최대 범위. 

	//--------------------
	// 소환 인형인 경우, 
	//--------------------
	siMakeKind			= 0;			// 이 소환ㅇ 인형을 사용하면 어떤 몬이 소환되는가. 

	//-------------------
	// 가방인 경우 
	//-------------------
	BagUseItemType		= 0;		// 가방에 들어가는 아이템의 종류. 

	//-------------------
	// 어구인 경우, 
	//-------------------
	siFishClassification	= 0;
    
	//--------------------
	// 세트 아이템 처리
	//-----------------------
	siSetItemIndex = 0;

	//-------------------------------------------------------------------
	// 무역 서버에 올라가냐 안 올라가냐,올라간다면 최소 단위수는 뭐냐
	//-------------------------------------------------------------------
	siTradeServerMinNum = TradeServerMinNum ;

	siTradeServerMinNum = TradeServerMinNum ;

	m_bNewMarket = bNewMarket;

	if(CrystalItemData != NULL)
	{
		memcpy(&CrystalData,CrystalItemData, sizeof(CrystalData));
	}
}

cltItemInfo::~cltItemInfo()
{
	
	if(pszMaterialText)
	{
		delete[] pszMaterialText;
		pszMaterialText = NULL;
	}

	if(pszToolText)
	{
		delete[] pszToolText;
		pszToolText = NULL;
	}

}



// 재료와 도구를 분석한다. 
bool cltItemInfo::AnalyzeMaterial(cltItemManagerCommon* pclItemManager)
{
	TCHAR c;

	// 분석할 원료가 없다. 
	if(pszMaterialText == NULL)return false;

	SI32 itemunique = 0 ;
	TCHAR szitemname[128] =  TEXT("") ;
	SI32 itemnameindex	= 0 ;
	bool itemnameswith = false ;

	TCHAR szitemnum[128] = TEXT("") ;
	SI32 itemnumindex	= 0 ;
	bool itemnumswith = false ;

	SI32 textindex	= 0 ;

    bool unlimitloop = true;
	while( unlimitloop )
	{
		c = pszMaterialText[textindex];
		if(c == '\0')break;

		textindex++;

		// Space는 무시한다. 
		if( c == ' ')continue;
		if( c == '+')
		{
			itemnameswith = false;
			continue;
		}
		if( c == '(')
		{
			szitemname[itemnameindex] = '\0';	// 이름을 마무리한다. 
			itemnameindex	= 0;

			itemunique = _tstoi(szitemname);

			itemnameswith = true;	// 이름은 얻어왔다. 다음은 아이템개수 순서다. 
			continue;
		}
		if( c == ')')
		{
			szitemnum[itemnumindex] = '\0';		// 아이템 개수를 마무리한다. 
			itemnumindex	= 0;

			//-------------------------------------------
			// 이름과 개수를 모두 얻어왔으므로 분석한다. 
			//-------------------------------------------
			
			// NULL이면 리턴한다. 
			if(_tcscmp(szitemname, TEXT("NULL")) == 0)return false;
			SI32 ref = pclItemManager->FindItemRefFromUnique(itemunique);
			//SI32 unique = pclItemManager->FindUniqueFromName(szitemname);
			if(ref)
			{
				SI32 itemnum	= _tstoi(szitemnum);
				if(itemnum > 0 && itemnum  < MAX_ITEM_PILE_NUMBER)
				{
					// 재료를 설정한다. 
					cltMaterialInfo clmaterial(itemunique, itemnum);
					clProductionInfo.AddMaterial(&clmaterial);
				}

			}
			continue;
		}

		// 아이템의 이름이 아직 결정되지 않았으면 이름을 입력받는다. 
		if(itemnameswith == false)
		{
			szitemname[itemnameindex] = c;
			itemnameindex++;
		}
		else
		{
			szitemnum[itemnumindex] = c;
			itemnumindex++;
		}

	}

	return true;
}


// 도구를 분석한다. 
bool cltItemInfo::AnalyzeTool(cltItemManagerCommon* pclItemManager)
{
	TCHAR c;

	// 분석할 도구가 없다. 
	if(pszToolText == NULL)return false;


	SI32 itemunique = 0 ;
	TCHAR szitemname[128];
	SI32 itemnameindex	= 0;
	bool itemnameswith = false;

	TCHAR szitemnum[128];
	SI32 itemnumindex	= 0;
	bool itemnumswith = false;

	SI32 textindex	= 0;


	while((c = pszToolText[textindex]) != '\0')
	{
		textindex++;

		// Space는 무시한다. 
		if( c == ' ')continue;
		if( c == '+')
		{
			itemnameswith = false;
			continue;
		}
		if( c == '(')
		{
			szitemname[itemnameindex] = '\0';	// 이름을 마무리한다. 
			itemnameindex	= 0;

			itemunique = _tstoi(szitemname);

			itemnameswith = true;	// 이름은 얻어왔다. 다음은 아이템개수 순서다. 
			continue;
		}
		if( c == ')')
		{
			szitemnum[itemnumindex] = '\0';		// 아이템 개수를 마무리한다. 
			itemnumindex	= 0;

			//-------------------------------------------
			// 이름과 개수를 모두 얻어왔으므로 분석한다. 
			//-------------------------------------------
			
			// NULL이면 리턴한다. 
			if(_tcscmp(szitemname, TEXT("NULL")) == 0)return false;

			SI32 ref = pclItemManager->FindItemRefFromUnique(itemunique);

			//SI32 unique = pclItemManager->FindUniqueFromName(szitemname);
			if(ref)
			{
				SI32 itemnum	= _tstoi(szitemnum);
				if(itemnum > 0 && itemnum  < MAX_ITEM_PILE_NUMBER)
				{
					// 도구를 설정한다. 
					cltToolInfo cltool(itemunique);
					clProductionInfo.AddTool(&cltool);
				}

			}
			continue;
		}

		// 아이템의 이름이 아직 결정되지 않았으면 이름을 입력받는다. 
		if(itemnameswith == false)
		{
			szitemname[itemnameindex] = c;
			itemnameindex++;
		}
		else
		{
			szitemnum[itemnumindex] = c;
			itemnumindex++;
		}

	}

	
	return true;

}

// 기술을 근거로 물품을 만들수 있는가 ?
bool cltItemInfo::CanMake_Skill(cltPSkillInfo* pclskill, cltPItemInfo* pclcharitem, cltBasicAbility* pclBA)
{
	if(pclskill == NULL)return false;

	// 필요한 스킬등을 확보한다. 
	SI32 skill = clSkillInfo.siSkill;

	// 도력이 부족하여 생산 불가. 
	if(clSkillInfo.siMag > pclBA->GetMag())
	{
		return false;
	}
	
	// [영훈] 행운이 제조시에 영향이 미치면
	if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
	{
		// 행운이 부족하여 생산 불가
		if (clSkillInfo.siHnd > pclBA->GetLuk() )
		{
			return false;
		}
	}
	else
	{	
		// 손재주가 부족하여 생산 불가. 
		if (clSkillInfo.siHnd > pclBA->GetHnd() )
		{
			return false;
		}
	}

	// 필요한 스킬이 없으므로 생산가능. 
	if(skill <= 0)return true;

	// 필요한 스킬 레벨 확보. 
	SI32 skilllevel = clSkillInfo.siSkillLevel;
	// 충분한 레벨을 확보했다면 
	if( skilllevel <= pclskill->GetTotalSkillLevel(skill,0, 0, &pclcharitem->clItem[0]) )
	{
		return true;
	}

	return false;
}

bool cltItemInfo::IsSkill_Maked()
{
	SI32 skill = clSkillInfo.siSkill;
	if ( skill <= 0 ) return false ; // 스킬이 없다.

	SI32 skilllevel = clSkillInfo.siSkillLevel;
	if ( skilllevel <= 0 ) return false ; // 필요 스킬 레벨이 0인 것들도 안된다.

	return true ;
}

// 주어진  재료를 근거로 물품을 생산할 수 있는가?
bool cltItemInfo::CanMake(SI32 itemnum,  cltItem* pclmaterialitem, cltItem* pclitem, SI32* producnum, SI32* preturnval, SI32* preturnpara1, bool bonlymaterialcheckswitch, BOOL bTutorialMake,BOOL bUseQuestItem,cltMaterialInfo* clTutorialMaterial)
{
	SI32 i, j;
	SI32 makerate = MAX_ITEM_PILE_NUMBER;
	SI32 needmaterialnum = 0;

	if(itemnum <= 0)return false;

	//----------------------------------------------------------
	// 필요한 도구가 있는지 확인한다. 
	//----------------------------------------------------------

	// 원료만 가지고 검사하는 것이이 아니면 도구도 검사한다. 
	if(bonlymaterialcheckswitch == false)
	{
		
		for(i = 0;i < MAX_TOOL_NUMBER;i++)
		{
			SI32 unique = clProductionInfo.clTool[i].siUnique;
			if(unique <= 0)continue;
			
			// 주어진 리스트에 필요한 아이템이 있는지 확인한다. 
			bool findswitch = false;
			for(j = 0;j < itemnum;j++)
			{
				// 필요한 아이템을 찾았다면, 그 개수도 확인한다. 
				if(pclitem[j].siUnique == unique)
				{
					findswitch = true;
				}
			}
			
			// 필요한 아이템을 찾지 못했다면 실패. 
			if(findswitch == false)
			{
				*preturnval		= CANMAKE_ERROR_NOTOOL;
				*preturnpara1	= unique;
				//return false; - 뒤에 검사.
			}
			
		}

		//KHY - 1211 - 과금상품 - 만능 도구
		SI32 itemunique = ITEMUNIQUE(8109) ; // 만능도구

		for(j = 0;j < itemnum;j++)
		{
			// 필요한 아이템을 찾았다면, 그 개수도 확인한다. 
			if(pclitem[j].siUnique == itemunique)
			{
				*preturnval		= CANMAKE_USE_ALLPURPOSETOOL;
				*preturnpara1	= 0;
			}
		}		

		if(*preturnval	== CANMAKE_ERROR_NOTOOL)
			return false; 
	}

	
	cltMaterialInfo	clMaterial[MAX_MATERIAL_NUMBER];
	
	if( bTutorialMake == FALSE || bUseQuestItem == FALSE ||  clTutorialMaterial == NULL)
	{
#ifdef _SAFE_MEMORY
		memcpy(clMaterial, &clProductionInfo.clMaterial[0], sizeof(cltMaterialInfo) * MAX_MATERIAL_NUMBER);
#else
		memcpy(clMaterial, clProductionInfo.clMaterial, sizeof(cltMaterialInfo) * MAX_MATERIAL_NUMBER);
#endif
	}
	else	// 튜토리얼제조 퀘스트 중이라면 이곳에 특정 퀘스트 재료를 대체시킨다.
	{
		memcpy(clMaterial, clTutorialMaterial, sizeof(cltMaterialInfo) * MAX_MATERIAL_NUMBER);
	}

	//-------------------------------------------------------------------------
	// 필요한 원료가 존재하는지 확인한다. (몇 배수로 존재하는지도 확인한다. )
	//-------------------------------------------------------------------------
	for(i = 0; i < MAX_MATERIAL_NUMBER;i++)
	{
		SI32 unique = clMaterial[i].siUnique;
		SI32 num	= clMaterial[i].siNeedNum;

		if(unique <= 0)
			continue;
		if(num <= 0)
			continue;

		// 필요한 재료의 수를 파악한다. 
		needmaterialnum ++;

		// 주어진 리스트에 필요한 아이템이 있는지 확인한다. 
		bool findswitch = false;

		SI32 siMaxMakeAbleNum = 0;
 		for(j = 0;j < itemnum ;j++)
		{
			// 필요한 아이템을 찾았다면, 그 개수도 확인한다. 
			if(pclmaterialitem[j].siUnique == unique)
			{
				SI32 makeAbleNum = 0;
				makeAbleNum = pclmaterialitem[j].GetItemNum() / num;

				if(makeAbleNum > 0)
				{
					if(siMaxMakeAbleNum < makeAbleNum)
					{
						siMaxMakeAbleNum = makeAbleNum;
						
						findswitch = true;
					}					
				}				
			}
		}
		
		if( findswitch == true  )
		{
			makerate =  min(makerate, siMaxMakeAbleNum);
		}

		// 필요한 아이템을 찾지 못했다면 실패. 
		if(findswitch == false || !(makerate > 0) )
		{
			*preturnval		= CANMAKE_ERROR_NOMATERIAL;
			*preturnpara1	= 	unique;
			return false;
		}

	}

	if(needmaterialnum )
	{
		// 만들 수 있는 개수를 파악한다. 
		*producnum = makerate * clProductionInfo.siProductUnit;
		return true;
	}
	// 하나의 재료도 없는 아이템은 생산 불가 아이템인것이다. 
	else
	{
		return false;
	}
}



// 주어진  재료를 근거로 물품을 생산할 수 있는가?
bool cltItemInfo::CanMakeForServer(SI32 itemnum,  cltItem* pclmaterialitem, cltItem* pclitem, SI32* producnum, SI32* preturnval, SI32* preturnpara1, bool bonlymaterialcheckswitch, SI32* materialindex , cltItem* pPersonShopItem, bool Check, BOOL bTutorialMake,BOOL bUseQuestItem,cltMaterialInfo* clTutorialMaterial)
{
	SI32 i = 0, j = 0, k = 0;
	SI32 makerate = MAX_ITEM_PILE_NUMBER;
	SI32 needmaterialnum = 0;

	if(itemnum <= 0)return false;

	//----------------------------------------------------------
	// 필요한 도구가 있는지 확인한다. 
	//----------------------------------------------------------

	// 원료만 가지고 검사하는 것이이 아니면 도구도 검사한다. 
	if(bonlymaterialcheckswitch == false)
	{
		for(i = 0;i < MAX_TOOL_NUMBER;i++)
		{
			SI32 unique = clProductionInfo.clTool[i].siUnique;
			if(unique <= 0)continue;

			// 주어진 리스트에 필요한 아이템이 있는지 확인한다. 
			bool findswitch = false;
			for(j = 0;j < itemnum;j++)
			{
				// 필요한 아이템을 찾았다면, 그 개수도 확인한다. 
				if(pclitem[j].siUnique == unique)
				{
					findswitch = true;
				}
			}

			// 필요한 아이템을 찾지 못했다면 실패. 
			if(findswitch == false)
			{
				*preturnval		= CANMAKE_ERROR_NOTOOL;
				*preturnpara1	= unique;
				//return false; - 뒤에 검사.
			}

		}

		//KHY - 1211 - 과금상품 - 만능 도구
		SI32 itemunique = ITEMUNIQUE(8109) ; // 만능도구

		for(j = 0;j < itemnum;j++)
		{
			// 필요한 아이템을 찾았다면, 그 개수도 확인한다. 
			if(pclitem[j].siUnique == itemunique)
			{
				*preturnval		= CANMAKE_USE_ALLPURPOSETOOL;
				*preturnpara1	= 0;
			}
		}		

		if(*preturnval	== CANMAKE_ERROR_NOTOOL)
			return false; 
	}


	cltMaterialInfo	clMaterial[MAX_MATERIAL_NUMBER];

	if( bTutorialMake == FALSE || bUseQuestItem == FALSE ||  clTutorialMaterial == NULL)
	{
#ifdef _SAFE_MEMORY
		memcpy(clMaterial, &clProductionInfo.clMaterial[0], sizeof(cltMaterialInfo) * MAX_MATERIAL_NUMBER);
#else
		memcpy(clMaterial, clProductionInfo.clMaterial, sizeof(cltMaterialInfo) * MAX_MATERIAL_NUMBER);
#endif
	}
	else	// 튜토리얼제조 퀘스트 중이라면 이곳에 특정 퀘스트 재료를 대체시킨다.
	{
		memcpy(clMaterial, clTutorialMaterial, sizeof(cltMaterialInfo) * MAX_MATERIAL_NUMBER);
	}

	//-------------------------------------------------------------------------
	// 필요한 원료가 존재하는지 확인한다. (몇 배수로 존재하는지도 확인한다. )
	//-------------------------------------------------------------------------
	for(i = 0;i < MAX_MATERIAL_NUMBER;i++)
	{
		SI32 materialunique = clMaterial[i].siUnique;
		if(materialunique <= 0)
			continue;
		SI32 materialnum	= clMaterial[i].siNeedNum;
		if(materialnum <= 0)
			continue;

		if( true == Check )
		{
			for( j = 0 ; j < MAX_SHOP_ITEM_NUMBER; ++j )
			{
				if( pPersonShopItem[j].siUnique == materialunique  )
				{
					*preturnval		= CANMAKE_ERROR_USE_PERSONSHOP;
					*preturnpara1	= materialunique;
					return false;
				}
			}
		}


		// 필요한 재료의 수를 파악한다. 
		needmaterialnum ++;

		bool bmaterialfinddswitch = false;

		for( k =0 ; k < 28 ; k++) //자기 자신일때
		{

			if(bmaterialfinddswitch == true)break;

			SI32 siitemindex = materialindex[k] - PERSONITEM_INV0;
			if(siitemindex >= 0 )
			{
				if(pclmaterialitem[siitemindex].siUnique == materialunique)
				{

					SI32 makeAbleNum = 0;
					makeAbleNum = pclmaterialitem[siitemindex].GetItemNum() / materialnum;

					if(makeAbleNum > 0)
					{
						makerate =  min(makerate, makeAbleNum);
						bmaterialfinddswitch = true;
						break;
					}				
					
				}
			}

		}
		// 필요한 아이템을 찾지 못했다면 실패. 
		if(bmaterialfinddswitch == false || !(makerate > 0) )
		{
			*preturnval		= CANMAKE_ERROR_NOMATERIAL;
			*preturnpara1	= 	materialunique;
			return false;
		}
	}


	if(needmaterialnum )
	{
		// 만들 수 있는 개수를 파악한다. 
		*producnum = makerate * clProductionInfo.siProductUnit;
		return true;
	}
	// 하나의 재료도 없는 아이템은 생산 불가 아이템인것이다. 
	else
	{
		return false;
	}
}



TCHAR* cltItemInfo::GetArmCode()
{
	if(_tcscmp(szArmCode, TEXT("")) == 0)
	{
		return NULL;
	}
	else
	{
		return szArmCode;
	}
}

// 최종 생산 확률을 구한다. 
float cltItemInfo::GetTotalMakePbt(SI32 skilllevel)
{
	//필요한 스킬 레벨보다 현재 스킬이 더 낮다면 확률은 0. 
	if(clSkillInfo.siSkillLevel > skilllevel)return 0.0;

	SI32 basicpbt = clSkillInfo.siMakePbt;
	
	SI32 addskill = skilllevel - clSkillInfo.siSkillLevel;

	SI32 addpbt = 0;

	if(addskill > 35)
	{
		addpbt		= addpbt + (addskill - 35) * (clSkillInfo.siIncPbt / 4);
		addskill	= 35;
	}

	if(addskill > 30)
	{
		addpbt		= addpbt + (addskill - 30) * (clSkillInfo.siIncPbt / 3);
		addskill	= 30;
	}

	if(addskill > 25)
	{
		addpbt = addpbt + (addskill - 25) * (clSkillInfo.siIncPbt / 2);
		addskill	= 25;
	}

	addpbt = addpbt + addskill * clSkillInfo.siIncPbt;

	
	SI32 sum	=	basicpbt +  addpbt;

	return (float) sum / 10;
}


cltSkillBookManager::cltSkillBookManager()
{

}
cltSkillBookManager::~cltSkillBookManager()
{

}

void cltSkillBookManager::Init( void )
{
	FileLoad();
}

void cltSkillBookManager::FileLoad( void )
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	TCHAR datfilename[MAX_PATH] = {0,};
	StringCchPrintf(datfilename,  MAX_PATH, TEXT("Item/Data/SkillBookInfo.txt") );


	//KHY - 0809 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );		
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( datfilename );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( datfilename );
		}
	}


	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadSetItemInfo()"), TEXT("LoadError:[%s]"), datfilename);
		return;
	}

	TCHAR* szAttributeList[ATTRIBUTE_NUM] =
	{
		"ATTRIBUTE_NONE",
		"ATTRIBUTE_FIRE", 
		"ATTRIBUTE_WATER", 
		"ATTRIBUTE_EARTH", 
		"ATTRIBUTE_WIND",
	};
		

	TCHAR* szMagicNameList[MAGIC_NAME_NUM] =
	{
		"NONE",
		"BUFTIMEINCREASE1",		// 버프마법 유지시간 10%증가
		"BUFTIMEINCREASE2",		// 버프마법 유지시간 20%증가
		"HITCOUNTINCREASE",		// 마법 유지회수 증가
		"FREEZINGTIMEINCREASE",	// 빙결마법 빙결유지시간 증가
		"ADDMAGIC",				// 특정 마법부여
		"ATTINCREASE",			// 마법공격력 증가
		"ATTINCREASE_CANE",		// 마법공격력 증가(지팡이)
		"RANGEINCREASE",		// 마법사거리 증가
		"COOLTIMEDECREASE",		// 마법 재사용시간 감소
		"MPDECREASE",			// 소모되는 마나 감소
		"ORGDECREASE",			// 소모되는 기관수치 감소
	};

	TCHAR* szBookGradeList[BOOK_GRADE_NUM] =
	{
		"NONE",
		"BGN",		
		"INT",		
		"ADV",		
	};

	SI32  	siItemUnique			= 0;
	TCHAR 	szItemName[128]			= "";	
	TCHAR 	szMagicKind[128]		= "";	
	TCHAR 	szMagicName[128]		= "";	
	TCHAR 	szParentMagicKind[128]	= "";	
	SI32  	siImageNum				= 0;	
	SI32  	siTextNum				= 0;	
	TCHAR 	szAttribute[128]		= "";	
	TCHAR 	siBookGrade[128]		= "";	
	SI32  	siBookNum				= 0;
	GMONEY  siCost					= 0;
	


	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siItemUnique,		4,		
		NDATA_MBSTRING,	 szItemName,		128,
		NDATA_MBSTRING,	 szMagicKind,		128,
		NDATA_MBSTRING,	 szMagicName,		128,
		NDATA_MBSTRING,	 szParentMagicKind,	128,
		NDATA_INT32,	&siImageNum,		4,	
		NDATA_INT32,	&siTextNum,			4,	
		NDATA_MBSTRING,	 szAttribute,		128,
		NDATA_MBSTRING,	&siBookGrade,		128,	
		NDATA_INT32,	&siBookNum,			4,	
		NDATA_INT64,	&siCost,			8,
		0,0,0
	};

	CMagicAttribute* pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
	m_siSkillBookInfoNum = 0;

	dataloader.RegDataType( pDataTypeInfo );
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if( SKILLBOOK_INFO_NUM <= m_siSkillBookInfoNum )
			{
				MessageBox( NULL, "SkillBook Num Over", "Error", 0 );
				return;
			}

			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siItemUnique = siItemUnique;
			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siMagicKind  = pMagicAttributePtr->FindRefFromKind( szMagicKind );

			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siMagicName = NONE;
			for( SI32 iCount = BUFTIMEINCREASE1; iCount < MAGIC_NAME_NUM; ++iCount )
			{
				if( 0 == _tcscmp( szMagicName, szMagicNameList[iCount] ) )
				{
					m_clSkillBookInfo[m_siSkillBookInfoNum].m_siMagicName = iCount;
					break;
				}
			}
			
			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siParentMagicKind = 0 > pMagicAttributePtr->FindRefFromKind( szParentMagicKind ) ? 0 : pMagicAttributePtr->FindRefFromKind( szParentMagicKind );
			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siImageNum		= siImageNum;
			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siTextNum			= siTextNum;

			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siAttribute = ATTRIBUTE_NONE;
			for( SI32 iCount = ATTRIBUTE_FIRE; iCount < ATTRIBUTE_NUM; ++iCount )
			{
				if( 0 == _tcscmp( szAttribute, szAttributeList[iCount] ) )
				{
					m_clSkillBookInfo[m_siSkillBookInfoNum].m_siAttribute = iCount;
					break;
				}
			}

			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siBookGrade = BOOK_GRADE_NONE;
			for( SI32 iCount = BOOK_GRADE_BGN; iCount < BOOK_GRADE_NUM; ++iCount )
			{
				if( 0 == _tcscmp( siBookGrade, szBookGradeList[iCount] ) )
				{
					m_clSkillBookInfo[m_siSkillBookInfoNum].m_siBookGrade = iCount;
					break;
				}
			}

			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siBookNum = siBookNum;
			m_clSkillBookInfo[m_siSkillBookInfoNum].m_siCost	= siCost;
			
			++m_siSkillBookInfoNum;
		}
	}
}

SI32 cltSkillBookManager::GetSkillBookInfoNum( void )
{
	return m_siSkillBookInfoNum;
}

bool cltSkillBookManager::IsSubMagicFromParentMagicKind( IN SI32 _siParentMagicKind )
{
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siParentMagicKind == _siParentMagicKind )
		{
			return true;
		}
	}

	return false;
}

bool cltSkillBookManager::GetSkillBookUniqueFromMagicKind( IN SI32 _siParentMagicKind, IN SI32 _siMagicKind, OUT SI32& _siItemUnique, OUT SI32& _siImageNum, OUT SI32& _siTextNum )
{
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if(		m_clSkillBookInfo[siCount].m_siParentMagicKind	== _siParentMagicKind 
			&&	m_clSkillBookInfo[siCount].m_siMagicKind		== _siMagicKind	)
		{
			_siItemUnique   = m_clSkillBookInfo[siCount].m_siItemUnique;
			_siImageNum		= m_clSkillBookInfo[siCount].m_siImageNum;	
			_siTextNum		= m_clSkillBookInfo[siCount].m_siTextNum;

			return true;
		}
	}

	return false;
}

bool cltSkillBookManager::GetSkillBookUniqueFromParentMagicKind( IN SI32 _siParentMagicKind, OUT SI32& _siItemUnique, OUT SI32& _siImageNum, OUT SI32& _siTextNum )
{
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siParentMagicKind == _siParentMagicKind )
		{
			_siItemUnique   = m_clSkillBookInfo[siCount].m_siItemUnique;
			_siImageNum		= m_clSkillBookInfo[siCount].m_siImageNum;	
			_siTextNum		= m_clSkillBookInfo[siCount].m_siTextNum;
			
			return true;
		}
	}

	return false;
}

bool cltSkillBookManager::GetSkillBookItemUniqueFromAttribute( IN SI32 _siAttribute, IN SI32 _siOutItemNum, OUT SI32* _siItemUnique )
{
	ZeroMemory( _siItemUnique, sizeof(SI32)*_siOutItemNum );

	SI32 siOutItemCount = 0;
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siAttribute == _siAttribute )
		{
			for( int siCount2 = 0; siCount2 < m_siSkillBookInfoNum; ++siCount2 )
			{
				if( _siItemUnique[siCount2] == m_clSkillBookInfo[siCount].m_siItemUnique )
					continue;
			}
			
			_siItemUnique[siOutItemCount++] = m_clSkillBookInfo[siCount].m_siItemUnique;
			
			if( siOutItemCount >= _siOutItemNum ) return true;
		}
	}	

	return true;
}

SI32 cltSkillBookManager::GetSkillBookMagicFromItemUnique( IN SI32 _siItemUnique )
{
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siItemUnique == _siItemUnique )
		{
			return m_clSkillBookInfo[siCount].m_siMagicKind;
		}
	}	

	return 0;
}

SI32 cltSkillBookManager::GetSkillBookParentMagicFromItemUnique( IN SI32 _siItemUnique )
{
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siItemUnique == _siItemUnique )
		{
			return m_clSkillBookInfo[siCount].m_siParentMagicKind;
		}
	}	

	return 0;
}

bool cltSkillBookManager::GetSkillBookInfoFromAttribute( IN SI32 _siAttribute, IN SI32 _siOutNum, OUT cltSkillBookInfo* _pclSkillBookInfo )
{
	ZeroMemory( _pclSkillBookInfo, sizeof(cltSkillBookInfo)*_siOutNum );

	SI32 siOutCount = 0;
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siAttribute == _siAttribute )
		{
			// 이미 같은게 있는지 찾는다.
			for( int siCount2 = 0; siCount2 < m_siSkillBookInfoNum; ++siCount2 )
			{
				if( _pclSkillBookInfo[siCount].m_siItemUnique == m_clSkillBookInfo[siCount].m_siItemUnique )
					continue;
			}
			
			_pclSkillBookInfo[siOutCount++] = m_clSkillBookInfo[siCount];

			if( siOutCount >= _siOutNum ) return true;
		}
	}	

	return true;
}


TCHAR* cltSkillBookManager::GetGradeText( SI32 _siGrade )
{
	switch( _siGrade )
	{
		case BOOK_GRADE_BGN: return GetTxtFromMgr(30527); 
		case BOOK_GRADE_INT: return GetTxtFromMgr(30528);
		case BOOK_GRADE_ADV: return GetTxtFromMgr(30529);
		default:			 return NULL;
	}

	return NULL;
}

SI32 cltSkillBookManager::GetSkillBookGradeFromItemUnique( IN SI32 _siItemUnique )
{
	static const SI32 BGN_BOOK_UNIQUE[] = 
	{
		4000, 4010, 4020, 4030, 4040, 4050, 4060, 4070, 4080, 4100, 4210, 4220, 4320, 4330, 4340, 4350, 4360, 4370, 
		4380, 4390, 4400, 4430, 4440, 4450, 4460, 4480, 4485, 4491, 4496, 4501, 4506, 4511	
	};
	static const SI32 siBgnBookNum = sizeof(BGN_BOOK_UNIQUE)/sizeof(BGN_BOOK_UNIQUE[0]);
	for( SI32 Count = 0; Count < siBgnBookNum; ++Count )
	{
		if( _siItemUnique == BGN_BOOK_UNIQUE[Count] ) return BOOK_GRADE_BGN;
	}

	static const SI32 INT_BOOK_UNIQUE[] = 
	{
		4003, 4006, 4013, 4023, 4033, 4043,	4053, 4063,	4073, 4083,	4130, 4213, 4230, 4323, 4333, 4343, 4353, 
		4363, 4373,	4383, 4393,	4403, 4433, 4444, 4454, 4463, 4483, 4487, 4493, 4498, 4503, 4508, 4513
	};
	
	static const SI32 siIntBookNum = sizeof(INT_BOOK_UNIQUE)/sizeof(INT_BOOK_UNIQUE[0]);
	for( SI32 Count = 0; Count < siIntBookNum; ++Count )
	{
		if( _siItemUnique == INT_BOOK_UNIQUE[Count] ) return BOOK_GRADE_INT;
	}

	static const SI32 ADV_BOOK_UNIQUE[] = 
	{
		4006, 4016, 4026, 4036, 4046, 4056, 4066, 4076, 4086, 4160, 4216, 4240, 4326, 4336, 4346, 4356, 4366, 4376,
		4386, 4396, 4406, 4436, 4448, 4458, 4466, 4490, 4495, 4500, 4505, 4510, 4515
	};
	static const SI32 siAdvBookNum = sizeof(ADV_BOOK_UNIQUE)/sizeof(ADV_BOOK_UNIQUE[0]);
	for( SI32 Count = 0; Count < siAdvBookNum; ++Count )
	{
		if( _siItemUnique == ADV_BOOK_UNIQUE[Count] ) return BOOK_GRADE_ADV;
	}

	return BOOK_GRADE_NONE;
}

bool cltSkillBookManager::IsSkillBookItemGrade( IN SI32 _siGrade, IN SI32 _siItemUnique )
{
	static const SI32 BGN_BOOK_UNIQUE[] = 
	{
		4000, 4010, 4020, 4030, 4040, 4050, 4060, 4070, 4080, 4100, 4210, 4220, 4320, 4330, 4340, 4350, 4360, 4370, 
		4380, 4390, 4400, 4430, 4440, 4450, 4460, 4480, 4485, 4491, 4496, 4501, 4506, 4511	
	};

	static const SI32 INT_BOOK_UNIQUE[] = 
	{
		4003, 4006, 4013, 4023, 4033, 4043,	4053, 4063,	4073, 4083,	4130, 4213, 4230, 4323, 4333, 4343, 4353, 
		4363, 4373,	4383, 4393,	4403, 4433, 4444, 4454, 4463, 4483, 4487, 4493, 4498, 4503, 4508, 4513
	};

	static const SI32 ADV_BOOK_UNIQUE[] = 
	{
		4006, 4016, 4026, 4036, 4046, 4056, 4066, 4076, 4086, 4160, 4216, 4240, 4326, 4336, 4346, 4356, 4366, 4376,
		4386, 4396, 4406, 4436, 4448, 4458, 4466, 4490, 4495, 4500, 4505, 4510, 4515
	};

	switch( _siGrade )
	{
		case BOOK_GRADE_BGN:
			{
				static const SI32 siBgnBookNum = sizeof(BGN_BOOK_UNIQUE)/sizeof(BGN_BOOK_UNIQUE[0]);
				for( SI32 Count = 0; Count < siBgnBookNum; ++Count )
				{
					if( _siItemUnique == BGN_BOOK_UNIQUE[Count] ) return true;
				}
			}
			break;
		case BOOK_GRADE_INT:
			{
				static const SI32 siIntBookNum = sizeof(INT_BOOK_UNIQUE)/sizeof(INT_BOOK_UNIQUE[0]);
				for( SI32 Count = 0; Count < siIntBookNum; ++Count )
				{
					if( _siItemUnique == INT_BOOK_UNIQUE[Count] ) return true;
				}
			}
			break;
		case BOOK_GRADE_ADV:
			{
				static const SI32 siAdvBookNum = sizeof(ADV_BOOK_UNIQUE)/sizeof(ADV_BOOK_UNIQUE[0]);
				for( SI32 Count = 0; Count < siAdvBookNum; ++Count )
				{
					if( _siItemUnique == ADV_BOOK_UNIQUE[Count] ) return true;
				}
			}
			break;
		default:	
			return false;
	}

	return false;
}

bool cltSkillBookManager::IsSkillBookItem( IN SI32 _siItemUnique )
{
	for( int siCount = 0; siCount < m_siSkillBookInfoNum; ++siCount )
	{
		if( m_clSkillBookInfo[siCount].m_siItemUnique == _siItemUnique )
		{
			return true;
		}
	}

	return false;
}
// [성웅] 레벨 보상 상자에서 아이템을 준다 
//레벨 보상 아이템 - 서버 클라도 함께 써야 해서 여기에 만듭니다.- 중국만 사용중
cltRewardItemForLimitLevel::cltRewardItemForLimitLevel()
{
	init()	;
}
cltRewardItemForLimitLevel::~cltRewardItemForLimitLevel()
{
}
void cltRewardItemForLimitLevel::init()
{
	// 초기화 한뒤 
	for ( SI32 i = 0 ; i< MAX_REWARDITEMBOX_NUM ; i++) 
	{
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].gRewardMoney			=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siBoxiOpenLevel		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siBoxitemunique		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemNum_1		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemUnique_1	=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemNum_2		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemUnique_2	=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemNum_3		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemUnique_3	=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemNum_4		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemUnique_4	=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemNum_5		=	0;
		m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardItemUnique_5	=	0;
	}
	// 데이터를 읽는다 .
	LoadBoxInfo()	;

}
void cltRewardItemForLimitLevel::LoadBoxInfo()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/RewrdBoxInfoLevelLimit.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/RewrdBoxInfoLevelLimit.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile RewrdBoxInfoLevelLimit.txt"),TEXT("Error"));
		return;
	}

	SI32 boxunique, openLevel,RewardTotalNum,  Rewarditemunique_1, RewarditemNum_1 , Rewarditemunique_2, RewarditemNum_2 , Rewarditemunique_3, RewarditemNum_3 , Rewarditemunique_4, RewarditemNum_4 , Rewarditemunique_5, RewarditemNum_5;
	GMONEY RewardMoney	;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&boxunique,	4,
			NDATA_INT32,	&openLevel,	4,
			NDATA_INT64,	&RewardMoney,	4,
			NDATA_INT32,	&RewardTotalNum,	4,
			NDATA_INT32,	&Rewarditemunique_1,	4,
			NDATA_INT32,	&RewarditemNum_1,	4,
			NDATA_INT32,	&Rewarditemunique_2,	4,
			NDATA_INT32,	&RewarditemNum_2,	4,
			NDATA_INT32,	&Rewarditemunique_3,	4,
			NDATA_INT32,	&RewarditemNum_3,	4,
			NDATA_INT32,	&Rewarditemunique_4,	4,
			NDATA_INT32,	&RewarditemNum_4,	4,
			NDATA_INT32,	&Rewarditemunique_5,	4,
			NDATA_INT32,	&RewarditemNum_5,	4,
			
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	SI16 index = 0 ;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData( true ))	
		{
			if ( index >= MAX_REWARDITEMBOX_NUM ) break	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siBoxitemunique	=	boxunique	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siBoxiOpenLevel	=	openLevel	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].gRewardMoney		=	RewardMoney	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardTotalNum		=	RewardTotalNum	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemUnique_1		=	Rewarditemunique_1	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemNum_1		=	RewarditemNum_1	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemUnique_2		=	Rewarditemunique_2	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemNum_2		=	RewarditemNum_2	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemUnique_3		=	Rewarditemunique_3	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemNum_3		=	RewarditemNum_3	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemUnique_4		=	Rewarditemunique_4	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemNum_4		=	RewarditemNum_4	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemUnique_5		=	Rewarditemunique_5	;
			m_RewardITemBoxLinmitLevel_ForNHNChina[index].siRewardItemNum_5		=	RewarditemNum_5	;
			index++;
		}
	}

}
bool cltRewardItemForLimitLevel::IsRewardBox_Unique( SI32 siItemUnique )	
{
	for ( SI32 i = 0 ; i< MAX_REWARDITEMBOX_NUM ; i++) 
	{
		if( m_RewardITemBoxLinmitLevel_ForNHNChina[i].siBoxitemunique == siItemUnique )
		{
			return true	;
		}
	}
	return	false ;
}
SI32 cltRewardItemForLimitLevel::GetOpenLevel( SI32 siItemUnique )
{
	for ( SI32 i = 0 ; i< MAX_REWARDITEMBOX_NUM ; i++) 
	{
		if( m_RewardITemBoxLinmitLevel_ForNHNChina[i].siBoxitemunique == siItemUnique )
		{
			return m_RewardITemBoxLinmitLevel_ForNHNChina[i].siBoxiOpenLevel	;
		}
	}
	return	-1;

}
SI32 cltRewardItemForLimitLevel::GetTotalItemNum( SI32 siItemUnique )
{
	for ( SI32 i = 0 ; i< MAX_REWARDITEMBOX_NUM ; i++) 
	{
		if( m_RewardITemBoxLinmitLevel_ForNHNChina[i].siBoxitemunique == siItemUnique )
		{
			return m_RewardITemBoxLinmitLevel_ForNHNChina[i].siRewardTotalNum	;
		}
	}
	return	-1;
}
SI32 cltRewardItemForLimitLevel::GetBoxUnique( SI32 siArray_Index )
{
	if ( siArray_Index >= MAX_REWARDITEMBOX_NUM )	return	0	;
	return m_RewardITemBoxLinmitLevel_ForNHNChina[siArray_Index].siBoxitemunique	;
}