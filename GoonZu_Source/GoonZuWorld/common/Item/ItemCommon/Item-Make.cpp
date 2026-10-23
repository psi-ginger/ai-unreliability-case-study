//---------------------------------
// 2004/5/19 김태곤
//---------------------------------
#include <stdio.h>
#include "cltItem.h"
#include "CommonLogic.h"

#include "Skill/Skill-Manager.h"
#include "Char/Disease/Disease.h"
#include "Util/Util.h"
#include "Char/CharCommon/Char-Common.h"
#include "../../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../../DBManager/GameDBManager_World/DBMsg-Market.h"

#include "Char\CharManager\CharManager.h"
#include "../../../Server/Server.h"

#include "../../../Server/GachaManager/GachaManager.h"
#include "../../../Server/ArtifactRestoreManager/ArtifactRestoreManager.h"
//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

// 공격력을 부여한다. 
void cltItemManagerCommon::MakeApower(cltItem* pclitem, SI32 qualityrate)
{
	if ( NULL == pclitem )
	{
		return;
	}

	pclitem->clItemWeapon.uiMinDamage	= max(1, pclitem->clItemWeapon.uiMinDamage * qualityrate / 100 );
	pclitem->clItemWeapon.uiMaxDamage	= max(1, pclitem->clItemWeapon.uiMaxDamage * qualityrate / 100 );
}

//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 결정체 타입.
void cltItemManagerCommon::MakeCrystal(cltItem* pclitem, SI32 qualityrate) // qualityrate 값은 제외 .
{
	if ( NULL == pclitem )
	{
		return;
	}

	UI08 temCrystalAbility = 0;
	UI08 temCrystalAbilityNum = 0;
	
	SI32 ref = FindItemRefFromUnique(pclitem->siUnique);

	SI32 randAbility = rand()%100+1; // MAX 확률 수치.
	SI32 bCrystalAbMin = 0;
	SI32 bCrystalAbMax = 0;
	//SI32 rateIndex[MAX_CAN_CRYSTALABILITY_NUM]={0,}; // 각 능력이 선택될 확률 모음.
	//SI32 selectAbilityIndex = 0; // 선택된 능력의 인덱스.

	cltCrystalItemData tempCrystalData[MAX_CAN_CRYSTALABILITY_NUM];
	SI32 tempIndex = 0;

	// 파일에 있는 파라미터 값들을 읽어 온다.
	for(int i = 0; i <MAX_CAN_CRYSTALABILITY_NUM ; i++)
	{
		if((pclItemInfo[ref]->CrystalData[i].siCrystalType) > 0)
		{
			tempCrystalData[tempIndex].Set(&pclItemInfo[ref]->CrystalData[i]); // 리소스의 데이터를 읽어온다. 만약을 대비.
			tempIndex++;
		}		
	}

	SI32 tempRate1 = 0;
	SI32 tempRate2 = 0;

	for(int k = 0; k <tempIndex ; k++)
	{	
		for(int x = 0; x < k+1; x++)
		{	
			tempRate2 = tempRate2 + tempCrystalData[x].siTypeRate;
		}

		if((randAbility > tempRate1) &&(randAbility <= tempRate2))
		{
			temCrystalAbility = tempCrystalData[k].siCrystalType;
			bCrystalAbMin 	= tempCrystalData[k].siMinValue;
			bCrystalAbMax 	= tempCrystalData[k].siMaxValue;
			break;
		}

		tempRate1 = tempRate2;
		tempRate2 =0;
	}

	// 선택된 능력에 대한 수치 결정.
	if(temCrystalAbility !=0)
	{
		// 성능증가는 이 부분에서 추가한다.
		//KHY - 0805 - 아트펙스 수정.
		//if(qualityrate >0)
		//	temCrystalAbilityNum = (UI08)((float)(bCrystalAb*qualityrate/100)+(float)0.5);
		//else

		// 절반에서, 절대값 사이의 값을 가진다. (10 -> 5~10)
		//SI32 randCrystalAb = rand()%((bCrystalAb/2)+1);
		//temCrystalAbilityNum = (bCrystalAb/2)+randCrystalAb ;

		if(IsItemInfoAtb(pclitem->siUnique,ITEMINFOATB_ITEMMALL) == true ) // 결정체 이면서 아이템 몰 상품 이라면.
		{
			// 과금 결정체 ( 아바타 결정체 ) 는 능력이 고정이다.
			temCrystalAbilityNum = bCrystalAbMin; // 최소값을 기준으로 한다.
		}
		else
		{
			// 일반 결정체
			// 명품이 있을 수 있다. - 100~280%
			SI32 qualityrate = max(100, GetAverage(100, true, 0));

			SI32 randAB = rand()%(bCrystalAbMax-bCrystalAbMin+1);

			temCrystalAbilityNum = max(1, (((bCrystalAbMin+randAB)*qualityrate)/100));
		}

		pclitem->clItemCrystal.siMaxDurability = 30000; // 결정체 최대 내구도  - 고정.
		pclitem->clItemCrystal.uiCrystalAbility = temCrystalAbility; 
		pclitem->clItemCrystal.uiCrystalAbilityNum = temCrystalAbilityNum; 
	}
}

// 방어력을 부여한다. 
void cltItemManagerCommon::MakeDpower(cltItem* pclitem, SI32 qualityrate)
{
	if ( NULL == pclitem )
	{
		return;
	}

	SI32 ref = FindItemRefFromUnique(pclitem->siUnique);
	if(ref >  0)
	{
		// 아이템의 Type을 구한다. 
		SI32 type = pclItemInfo[ref]->siType;

		//if (type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT)
		//{
		//	pclitem->clItemStatusArmour.uiAC	= max(1, pclitem->clItemStatusArmour.uiAC * qualityrate / 100 );
		//}
		//else
		//{
			pclitem->clItemArmour.uiAC	= max(1, pclitem->clItemArmour.uiAC * qualityrate / 100 );
		//}
	}

}

// 물품의 내구도를 만든다. 
void cltItemManagerCommon::MakeDur(SI16* pbrareswitch, SI32 orgqualityrate, SI32 addlevel, cltItem* pclitem, bool bforseswitch,
								   SI32 durrate,SI32 monstercorp)
{
	if( pclClient->IsCountrySwitch( Switch_NewDurability ) == false )		return;
	if ( (NULL == pbrareswitch) || (NULL == pclitem) )						return;

	// 성능신기일 경우 내구도를 GV_Dur_HIGHGRADEDURABILITY로 고정하기 위함
	SI16 siRareSwitch = *pbrareswitch;

	SI32 qualityrate = 0;

	// 성능을 지정된 것으로 강제한다. 
	if(bforseswitch)
	{
		qualityrate = orgqualityrate;
	}
	else if ( durrate <= 0 || durrate > 500 )
	{
		qualityrate = GetAverage( max(10, orgqualityrate), bCanMakeRareItemSwitch, addlevel, false );
	}
	else if ( durrate != 0 && durrate > 0 && durrate <= 500 )
	{
		qualityrate = durrate ;
	}

	qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;

	if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
	else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
	else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )
	{
		// 내구도 신기 등급일때는 신기구간의 값들이 안나오게 100~149값들중 랜덤하게 나오게함
		qualityrate = rand() % 50;
		qualityrate += 100;
	}

	if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
	{
		if(orgqualityrate < 100)
		{
			qualityrate = orgqualityrate;
			// 고물품
			*pbrareswitch = -1;
		}
	}

	UI32 uiMaxDurability		= (UI32)pclClient->GetGlobalValue( TEXT("GV_MaxDurability") );
	UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
	UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

	if( uiForeverDurability == pclitem->clItemCommon.clCommonInfo.uiMaxDurability ||
		uiForeverDurability2 == pclitem->clItemCommon.clCommonInfo.uiMaxDurability )
	{
		// FOREVER DURABILITY ------------------------------------------------------------------------------------------
		pclitem->clItemCommon.clCommonInfo.uiMaxDurability = pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
		//--------------------------------------------------------------------------------------------------------------
	}
	else if ( 3 == siRareSwitch )
	{
		UI32 uiHighGradeMaxDur = (UI32)pclClient->GetGlobalValue( "GV_Dur_HIGHGRADEDURABILITY" );

		pclitem->clItemCommon.clCommonInfo.uiMaxDurability = uiHighGradeMaxDur;
	}
	else
	{
		// SET MAX DURABILITY ----------------------------------------------------------------------------------------------
		pclitem->clItemCommon.clCommonInfo.uiMaxDurability = min(uiMaxDurability, max(10, pclitem->clItemCommon.clCommonInfo.uiMaxDurability * qualityrate / 100)  );
		pclitem->clItemCommon.clCommonInfo.uiMaxDurability -= (pclitem->clItemCommon.clCommonInfo.uiMaxDurability%10);	// 1단위는 삭제한다. 
		//--------------------------------------------------------------------------------------------------------------
	}

	// SET DURABILITY ----------------------------------------------------------------------------------------------
	pclitem->clItemCommon.clCommonInfo.uiDurability = pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
	//--------------------------------------------------------------------------------------------------------------
}


// 물품의 기간을 만든다.
void cltItemManagerCommon::MakeDate(SI16* pbrareswitch, SI32 orgqualityrate, SI32 addlevel, cltItem* pclitem, bool bforseswitch,
									SI32 durrate,SI32 monstercorp)
{
	if ( (NULL == pbrareswitch) || (NULL == pclitem) )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	SI32 qualityrate = 0;

	// 성능을 지정된 것으로 강제한다. 
	if(bforseswitch)
	{
		qualityrate = orgqualityrate;
	}
	else if ( durrate <= 0 || durrate > 500 )
	{
		qualityrate = GetAverage( max(10, orgqualityrate), bCanMakeRareItemSwitch, addlevel);
	}
	else if ( durrate != 0 && durrate > 0 && durrate <= 500 )
		qualityrate = durrate ;
	qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;

	if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
	else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산

	pclitem->uiDateUseDay = min(360,max(1,pclitem->uiDateUseDay* qualityrate / 100)) ;
	
	if(pclitem->uiDateUseDay < 1)
	{
		pclitem->uiDateUseDay = 5;
	}

	if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
	{
		// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
		if ( 3 == *pbrareswitch )
		{
			UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
			UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
			UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

			if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
			{
				pclitem->uiDateUseDay = 0;
			}
		}
	}

	//	pclitem->uiDateYear = pclserver->sTime.wYear - 2000 ;
	//	pclitem->uiDateMonth = pclserver->sTime.wMonth ;
	//	pclitem->uiDateDay = pclserver->sTime.wDay ;
}


// 물품의 기간을 만든다.
void cltItemManagerCommon::MakeAvatarDate(SI16* pbrareswitch, SI32 orgqualityrate, SI32 addlevel, cltItem* pclitem, bool bforseswitch,
									SI32 durrate,SI32 monstercorp)
{
	if ( (NULL == pbrareswitch) || (NULL == pclitem) )
	{
		return;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( pclserver == NULL ) return ;

	SI32 qualityrate = 0;

	// 성능을 지정된 것으로 강제한다. 
	if(bforseswitch)
	{
		qualityrate = orgqualityrate;
	}
	// 운영자의 설정이 아니다. - 기본 설정.
	else if ( durrate <= 0 || durrate > 500 )
	{
		qualityrate = GetAverage( max(10, orgqualityrate), bCanMakeRareItemSwitch, addlevel);

		SI32 randaddrate = rand() % 2 ;

		if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
			qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
	}
	// 운영자 설정값.
	else if ( durrate != 0 && durrate > 0 && durrate <= 500 )
		qualityrate = durrate ;


	if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
	else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
	else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산

	// 실제 날짜를 만들어 입력하는 부분.
	pclitem->uiDateUseDay = min(360,max(1,pclitem->uiDateUseDay* qualityrate / 100)) ;
	
	if(pclitem->uiDateUseDay < 1)
	{
		pclitem->uiDateUseDay = 5;
	}
}
// 고급품의 기간을 얻어온다. (랜덤)
SI32 cltItemManagerCommon::GetHighItemDate()
{
	// 중국은 막음
	if ( pclClient->IsWhereServiceArea( ConstServiceArea_China ) )
		return 0;

	//SI32 siUseDate = 7;
	//SI32 siRand = rand()% 100;
	//if(siRand >= 0 && siRand < 10)			// 10% : 7 ~ 13일
	//{
	//	siUseDate = 7;
	//	siUseDate += rand() % 7;
	//}
	//else if(siRand >= 10 && siRand < 80)	// 70% : 14일 ~ 22일
	//{
	//	siUseDate = 18;
	//	siUseDate += (rand() % 9) - 4;
	//}
	//else if(siRand >= 80 && siRand < 100)	// 20% : 23 ~ 28일
	//{
	//	siUseDate = 23;
	//	siUseDate += rand() % 6;
	//}


	// 7 ~ 14일까지 랜덤으로 돌린다.
	SI32 siUseDate = 7;
	SI32 siPlusDate = 0;
    siPlusDate = rand() % 8;

	return siUseDate + siPlusDate;
}


// [추가 : 황진성 2008. 3. 3 => 아이템의 환원시 나오는 아이템.]
bool cltItemManagerCommon::MakeItembyRestore( cltItem* pclEquipItem, cltItem* pclRestoreItem, cltCharCommon* pclchar )
{
	if ( (NULL == pclEquipItem) || (NULL == pclchar) )
	{
		return false;
	}

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( pclEquipItem->siUnique );
	if( ref <= 0 ) 
		return false;

	// 아이템의 장착 레벨.
	SI32 siItemLevel = pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel; 
	if( 0 >= siItemLevel )
		return false;

	SI32 siRand				= rand()%100 + 1;	
	SI32 siRestoreItemCount	= 0;
	SI32 siIndex			= siItemLevel - 1;	// 레벨별로 정보가 있으므로 		
	SI32 siItemUnique		= 0;
	SI16 brareswitch		= 0;

	//___________________________________________________________________________________________________	
	// 원소 아이템을 얻는다.
	//___________________________________________________________________________________________________	
	
	// 원소 배열 포인터 얻기.
	clElementRestore* pclElementRestore = &pclClient->pclArtifactRestoreManager->m_clElementRestore[ siIndex ];

	for( SI32 ItemCount = ITEM_BY_ELEMENT_KIND_NUM - 1; 0 <= ItemCount; --ItemCount )
	{
		if( siRand < pclElementRestore->m_siRate[ ItemCount ] )
		{
			siItemUnique = pclElementRestore->m_siItemUnique[ ItemCount ];	
			brareswitch  = 0;

			if( false ==  MakeRandItemUnique( siItemUnique, pclRestoreItem, 0, 0, &brareswitch ) )
			{
				return false;
			}
			
			SI32 siMinItemNum = pclElementRestore->m_siMinNum[ ItemCount ];
			SI32 siMaxItemNum = pclElementRestore->m_siMaxNum[ ItemCount ];
			
			//// 해당 최대 값으로 아이템개수를 뽑는다.
			//SI32 siItemNum = 1 + rand()%siMaxItemNum;
			//			
			//// 최대값으로 뽑은 개수가 최소값보다 작은경우 최소값을 적용한다.
			//if(  siItemNum < siMinItemNum )
			//	siItemNum = siMinItemNum;

			//아이템개수를 뽑는다.
			SI32 siItemNum = siMinItemNum + ( rand() % ( siMaxItemNum - siMinItemNum +1) );

#ifdef _SAFE_MEMORY
			SI32 SkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0,  &pclchar->pclCI->clCharItem.clItem[0]);
#else
			SI32 SkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0,  pclchar->pclCI->clCharItem.clItem);
#endif

			if(30 <= SkillLevel)
			{
				/*float Percent = 0;
					 if( 45  >  SkillLevel)  Percent = 0.2f;
				else if( 60  >  SkillLevel)  Percent = 0.4f;
				else if( 75  >  SkillLevel)  Percent = 0.6f;
				else if( 100 >  SkillLevel)  Percent = 0.8f;
				else						 Percent = 1.0f;

				siItemNum += siItemNum * Percent;
				*/
				SI32 temMinnum = 0;
				SI32 temnum = 0;
				SI32 AddPer = 0;

				//makeitemnum =((makeitemnum*140)/100);
				if(SkillLevel >= 30 && SkillLevel < 45)
				{
					temMinnum = ((siItemNum*1200)/1000); // 최소값.

					AddPer = (SkillLevel - 30) * 13; 
					temnum = ((siItemNum*(1200+AddPer))/1000); // 기술 레벨에 따른 맥스값.
						
					siItemNum = max(temMinnum ,temnum );
				}
				else if(SkillLevel >= 45 && SkillLevel < 60)
				{
					temMinnum = ((siItemNum*1400)/1000); // 최소값.

					AddPer = (SkillLevel - 45) * 13; 
					temnum = ((siItemNum*(1400+AddPer))/1000); // 기술 레벨에 따른 맥스값.
						
					siItemNum = max(temMinnum ,temnum );			
				}
				else if(SkillLevel >= 60 && SkillLevel < 75)
				{
					temMinnum = ((siItemNum*1600)/1000); // 최소값.

					AddPer = (SkillLevel - 60) * 13; 
					temnum = ((siItemNum*(1600+AddPer))/1000); // 기술 레벨에 따른 맥스값.
						
					siItemNum = max(temMinnum ,temnum );
				}
				else if(SkillLevel >= 75 && SkillLevel < 100)
				{
					temMinnum = ((siItemNum*1800)/1000); // 최소값.

					AddPer = (SkillLevel - 75) * 13; 
					temnum = ((siItemNum*(1800+AddPer))/1000); // 기술 레벨에 따른 맥스값.
						
					siItemNum = max(temMinnum ,temnum );
				}
				else if(SkillLevel ==  100)
				{
					siItemNum =((siItemNum*2000)/1000);
				}
			}
			
			// 아이템 개수.
			pclRestoreItem->siItemNum = siItemNum;
			break;
		}
	}									  
	
	if( 0 >= pclRestoreItem->siUnique )
	{
		siItemUnique = pclElementRestore->m_siItemUnique[0];
		brareswitch  = 0;
		
		if( false ==  MakeRandItemUnique( siItemUnique, pclRestoreItem, 0, 0, &brareswitch ) )
		{
			return false;
		}

		SI32 siMinItemNum = pclElementRestore->m_siMinNum[ 0 ];
		SI32 siMaxItemNum = pclElementRestore->m_siMaxNum[ 0 ];

		//// 해당 최대 값으로 아이템개수를 뽑는다.
		//SI32 siItemNum = 1 + rand()%siMaxItemNum;
		//			
		//// 최대값으로 뽑은 개수가 최소값보다 작은경우 최소값을 적용한다.
		//if(  siItemNum < siMinItemNum )
		//	siItemNum = siMinItemNum;

		//아이템개수를 뽑는다.
		SI32 siItemNum = siMinItemNum + ( rand() % ( siMaxItemNum - siMinItemNum +1 ) );

#ifdef _SAFE_MEMORY
		SI32 SkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0,  &pclchar->pclCI->clCharItem.clItem[0]);
#else
		SI32 SkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0,  pclchar->pclCI->clCharItem.clItem);
#endif
		if(30 <= SkillLevel)
		{
			/*float Percent = 0;
				 if( 45  >  SkillLevel)  Percent = 0.2f;
			else if( 60  >  SkillLevel)  Percent = 0.4f;
			else if( 75  >  SkillLevel)  Percent = 0.6f;
			else if( 100 >  SkillLevel)  Percent = 0.8f;
			else						 Percent = 1.0f;

			siItemNum += siItemNum * Percent;
			*/
			SI32 temMinnum = 0;
			SI32 temnum = 0;
			SI32 AddPer = 0;

			//makeitemnum =((makeitemnum*140)/100);
			if(SkillLevel >= 30 && SkillLevel < 45)
			{
				temMinnum = ((siItemNum*1200)/1000); // 최소값.

				AddPer = (SkillLevel - 30) * 13; 
				temnum = ((siItemNum*(1200+AddPer))/1000); // 기술 레벨에 따른 맥스값.
					
				siItemNum = max(temMinnum ,temnum );
			}
			else if(SkillLevel >= 45 && SkillLevel < 60)
			{
				temMinnum = ((siItemNum*1400)/1000); // 최소값.

				AddPer = (SkillLevel - 45) * 13; 
				temnum = ((siItemNum*(1400+AddPer))/1000); // 기술 레벨에 따른 맥스값.
					
				siItemNum = max(temMinnum ,temnum );			
			}
			else if(SkillLevel >= 60 && SkillLevel < 75)
			{
				temMinnum = ((siItemNum*1600)/1000); // 최소값.

				AddPer = (SkillLevel - 60) * 13; 
				temnum = ((siItemNum*(1600+AddPer))/1000); // 기술 레벨에 따른 맥스값.
					
				siItemNum = max(temMinnum ,temnum );
			}
			else if(SkillLevel >= 75 && SkillLevel < 100)
			{
				temMinnum = ((siItemNum*1800)/1000); // 최소값.

				AddPer = (SkillLevel - 75) * 13; 
				temnum = ((siItemNum*(1800+AddPer))/1000); // 기술 레벨에 따른 맥스값.
					
				siItemNum = max(temMinnum ,temnum );
			}
			else if(SkillLevel ==  100)
			{
				siItemNum =((siItemNum*2000)/1000);
			}
		}

		// 아이템 개수.
		pclRestoreItem->siItemNum = siItemNum;
	}
	//___________________________________________________________________________________________________
	
	//___________________________________________________________________________________________________	
	// 가루 아이템을 얻는다.
	//___________________________________________________________________________________________________	

	// 다음 아이템 배열에 가루를 저장하기 위해 하나 증가.
	++pclRestoreItem;
	
	// 아이템이 가진 인챈트 속성. 
	SI32 siElementType			= pclEquipItem->Element.siType;
	SI32 siEnchantTypeSlotNum	= -1;
	
	// 속성별 슬롯의 정보.
	switch( siElementType )
	{
		case ENCHANT_TYPE_DEFAULT:	siEnchantTypeSlotNum =  0;	break;
		case ENCHANT_TYPE_FIRE_A:	
		case ENCHANT_TYPE_FIRE_D:	siEnchantTypeSlotNum =  1;	break;
		case ENCHANT_TYPE_WATER_A:
		case ENCHANT_TYPE_WATER_D:	siEnchantTypeSlotNum =  2;	break;
		case ENCHANT_TYPE_GROUND_A:
		case ENCHANT_TYPE_GROUND_D:	siEnchantTypeSlotNum =  3;	break;
		case ENCHANT_TYPE_WIND_A:	
		case ENCHANT_TYPE_WIND_D:	siEnchantTypeSlotNum =  4;	break;	
		default:					siEnchantTypeSlotNum = -1;	break;	
	}

	if( -1 == siEnchantTypeSlotNum )
		return false;
	
	// 원소 배열 포인터 얻기.
	clPowderRestore* pclPowderRestore = &pclClient->pclArtifactRestoreManager->m_clPowderRestore[ siIndex ];
	
	if( siRand < pclPowderRestore->m_siRate[ siEnchantTypeSlotNum ] )
	{
		siItemUnique = pclPowderRestore->m_siItemUnique[ siEnchantTypeSlotNum ];	
		brareswitch  = 0;

		if( false ==  MakeRandItemUnique( siItemUnique, pclRestoreItem, 0, 0, &brareswitch ) )
		{
			return false;
		}

		SI32 siMinItemNum = pclPowderRestore->m_siMinNum[ siEnchantTypeSlotNum ];
		SI32 siMaxItemNum = pclPowderRestore->m_siMaxNum[ siEnchantTypeSlotNum ];

		//// 해당 최대 값으로 아이템개수를 뽑는다.
		//SI32 siItemNum = 1 + rand()%siMaxItemNum;
		//			
		//// 최대값으로 뽑은 개수가 최소값보다 작은경우 최소값을 적용한다.
		//if(  siItemNum < siMinItemNum )
		//	siItemNum = siMinItemNum;

		//아이템개수를 뽑는다.
		SI32 siItemNum = siMinItemNum + ( rand() % ( siMaxItemNum - siMinItemNum +1 ) );

#ifdef _SAFE_MEMORY
		SI32 SkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0,  &pclchar->pclCI->clCharItem.clItem[0]);
#else
		SI32 SkillLevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0,  pclchar->pclCI->clCharItem.clItem);
#endif
		/*if(30 <= SkillLevel)
		{
			float Percent = 0;
				 if( 45  >  SkillLevel)  Percent = 0.2f;
			else if( 60  >  SkillLevel)  Percent = 0.4f;
			else if( 75  >  SkillLevel)  Percent = 0.6f;
			else if( 100 >  SkillLevel)  Percent = 0.8f;
			else						 Percent = 1.0f;

			siItemNum += siItemNum * Percent;
		}
		*/
		SI32 temMinnum = 0;
		SI32 temnum = 0;
		SI32 AddPer = 0;

		//makeitemnum =((makeitemnum*140)/100);
		if(SkillLevel >= 30 && SkillLevel < 45)
		{
			temMinnum = ((siItemNum*1200)/1000); // 최소값.

			AddPer = (SkillLevel - 30) * 13; 
			temnum = ((siItemNum*(1200+AddPer))/1000); // 기술 레벨에 따른 맥스값.
				
			siItemNum = max(temMinnum ,temnum );
		}
		else if(SkillLevel >= 45 && SkillLevel < 60)
		{
			temMinnum = ((siItemNum*1400)/1000); // 최소값.

			AddPer = (SkillLevel - 45) * 13; 
			temnum = ((siItemNum*(1400+AddPer))/1000); // 기술 레벨에 따른 맥스값.
				
			siItemNum = max(temMinnum ,temnum );			
		}
		else if(SkillLevel >= 60 && SkillLevel < 75)
		{
			temMinnum = ((siItemNum*1600)/1000); // 최소값.

			AddPer = (SkillLevel - 60) * 13; 
			temnum = ((siItemNum*(1600+AddPer))/1000); // 기술 레벨에 따른 맥스값.
				
			siItemNum = max(temMinnum ,temnum );
		}
		else if(SkillLevel >= 75 && SkillLevel < 100)
		{
			temMinnum = ((siItemNum*1800)/1000); // 최소값.

			AddPer = (SkillLevel - 75) * 13; 
			temnum = ((siItemNum*(1800+AddPer))/1000); // 기술 레벨에 따른 맥스값.
				
			siItemNum = max(temMinnum ,temnum );
		}
		else if(SkillLevel ==  100)
		{
			siItemNum =((siItemNum*2000)/1000);
		}
		
		// 아이템 개수.
		pclRestoreItem->siItemNum = siItemNum;
	}
			
	return true;
}


// 아이템의 Unique정보를 토대로 랜덤하게 아이템을 하나 만든다.
// 아이템의 Unique정보를 토대로 랜덤하게 아이템을 하나 만든다.
//====================================================
// PARAMETER
//====================================================
// siunique			: 생성할 아이템 유니크
// pclitem			: 생성한 아이템
// orgqualityrate	: Quality Rate
// addlevel			: 손재주차이(1000자리), 레벨차이(1자리)
// pbrareswitch		: 명품 또는 호품이 결정된 경우 반환
// attackrate		: 운영자권한 설정값 
// magrate			: 운영자권한 설정값
// skillrate		: 운영자권한 설정값
// durrate			: 운영자권한 설정값
// monstercorp		: 미니게임 값
// usedate			: 사용기간
// enchanttype		: 운영자권한 설정값
// enchantlevel		: 운영자권한 설정값
// slotnum			: 운영자권한 설정값
//====================================================
bool cltItemManagerCommon::MakeRandItemUnique(SI32 siunique, cltItem* pclitem,
											  SI32 orgqualityrate, SI32 addlevel,SI16* pbrareswitch,
											  SI32 attackrate , SI32 magrate ,SI32 skillrate, SI32 durrate,
											  SI32 monstercorp,SI32 usedate, SI32 enchanttype, SI32 enchantlevel, SI32 slotnum,
											  bool bSEQCreate)
{
	if ( (NULL == pclitem) || (NULL == pbrareswitch) )
	{
		return false;
	}

	if(siunique == 0)return false;
	SI32 qualityrate  = 0;

	cltServer* pclserver = (cltServer*)pclClient;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)
	{
		TCHAR buffer[256];
		StringCchPrintf(buffer, 256, TEXT("MakeRandItem() f87s3:%d"), siunique);
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Error"), buffer, MB_OK|MB_TOPMOST);

		return false;
	}

	// 아이템의 Type을 구한다. 
	SI32 type = pclItemInfo[ref]->siType;

	// 아이템 Type의 Atb를 구한다. 
	SI64 atb = pclItemTypeInfo[type]->siAtb;

	// 아이템이 Object이면, 
	if(atb & ITEMATB_OBJECT)
	{
		pclitem->Set(&pclItemInfo[ref]->clItem);
	}
	else
	{
		pclitem->Set(&pclItemInfo[ref]->clItem);
	}

	if ( usedate > 0 && (atb & ITEMATB_OBJECT))
	{
		pclitem->uiDateYear = 0;
		pclitem->uiDateMonth = 0;
		pclitem->uiDateDay = 0;
		pclitem->uiDateUseDay = usedate;
	}
 
	//-----------------------------------------------
	//    인챈트관련 처음에 만들때는 0   by tae-yang
	//-----------------------------------------------
	pclitem->Element.siType = ENCHANT_TYPE_DEFAULT;
	pclitem->Element.siPower = 0;

	// [춘기] /seq명령어로 아이템 생성시에 인챈트 관련 정보를 넣어준다. (09.10.12)
	if(bSEQCreate == true)
	{
		if(enchanttype > 0)		pclitem->Element.siType = enchanttype;
		if(enchantlevel > 0)	pclitem->Element.siPower = enchantlevel;
	}

	// 아이템 몰 상품이며 의복이나 모자, 망토이면 받은 기간을 설정해 준다.
	if ( IsItemInfoAtb(siunique,ITEMINFOATB_ITEMMALL) == true )
	{
		//etc 아이템도 기간설정 [성웅] -> 과금총알, 과금화살이 프리미엄으로 옮겨감
		// 프리미엄 아이템 중에 usedate가 있는 것만 자동으로 시작시간을 설정한다. - by LEEKH 2008-10-07
		if( type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT || type == ITEMTYPE_MANTLE || 
			(type == ITEMTYPE_PREMIUM && usedate > 0) || 
			type == ITEMTYPE_BAG )
		{			
			pclitem->uiDateYear = pclserver->sTime.wYear - 2000 ;
			pclitem->uiDateMonth = pclserver->sTime.wMonth ;
			pclitem->uiDateDay = pclserver->sTime.wDay ;			
		}
	}

	// [지연/ 09-05-06 - 아바타 아이템 제조시 슬롯은 3개 만들어 준다. 목걸이만 1개 만들어줌.
	if(pclClient->IsCountrySwitch(Switch_Avatar_artisan))
	{
		if(IsItemInfoAtb(siunique,ITEMINFOATB_ITEMMALL) != true ) // 아이템 몰 상품은 슬롯을 안만들어준다.
		{
			if(pclClient->pclItemManager->UsedSlotItem(siunique) == SLOTITEMTYPE_AVATAR) // 아바타 아이템으로 슬롯생성이 가능할경우.
			{
				if(type ==ITEMTYPE_AVATAR_NECK )
				{
					pclitem->siUseCrystalNum =1; // 슬롯 1개
				}	
				else
				{
					pclitem->siUseCrystalNum = 3; // 슬롯 3개
				}
			}
		}
	}

	//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 아이템 제조시 슬롯을 만들어 준다.
	if(pclClient->IsCountrySwitch(Switch_Artifact))
	{
		if ( IsItemInfoAtb(siunique,ITEMINFOATB_ITEMMALL) != true ) // 아이템 몰 상품은 슬롯을 안만들어준다.
		{
			if(pclClient->pclItemManager->UsedSlotItem(siunique) == SLOTITEMTYPE_WEAPON) // 아바타 아이템을 제외한 나머지 아이템들.
			{
				cltItemConition  *pclCondition = &pclItemInfo[ref]->clCondition;
				SI08 temCrystalNum = 0;

				SI16 randNum = rand()%100; // 난수 발생.

				if((pclCondition->siLevel >= 1)&&(pclCondition->siLevel <= 29))
				{
					if((randNum>=0)&&(randNum < 15))
					{
						temCrystalNum = 1;
					}
					else if((randNum>=15)&&(randNum < 28))
					{
						temCrystalNum = 2;
					}
				}
				else if((pclCondition->siLevel >= 30)&&(pclCondition->siLevel <= 59))
				{
					if((randNum>=0)&&(randNum < 13))
					{
						temCrystalNum = 1;
					}
					else if((randNum>=13)&&(randNum < 24))
					{
						temCrystalNum = 2;
					}
					else if((randNum>=24)&&(randNum < 30))
					{
						temCrystalNum = 3;
					}
				}
				else if((pclCondition->siLevel >= 60)&&(pclCondition->siLevel <= 89))
				{
					if((randNum>=0)&&(randNum < 12))
					{
						temCrystalNum = 1;
					}
					else if((randNum>=12)&&(randNum < 22))
					{
						temCrystalNum = 2;
					}
					else if((randNum>=22)&&(randNum < 29))
					{
						temCrystalNum = 3;
					}
					else if((randNum>=29)&&(randNum < 33))
					{
						temCrystalNum = 4;
					}
				}
				else if(pclCondition->siLevel >= 90)
				{
					if((randNum>=0)&&(randNum < 10))
					{
						temCrystalNum = 1;
					}
					else if((randNum>=10)&&(randNum < 19))
					{
						temCrystalNum = 2;
					}
					else if((randNum>=19)&&(randNum < 27))
					{
						temCrystalNum = 3;
					}
					else if((randNum>=27)&&(randNum < 33))
					{
						temCrystalNum = 4;
					}
					else if((randNum>=33)&&(randNum < 35))
					{
						temCrystalNum = 5;
					}
				}
				pclitem->siUseCrystalNum = temCrystalNum;

				//if(pclitem->SetUseCrystalNum(temCrystalNum) != true)
				//	return false;
			}
		}
	}

	// [춘기] /SEQ 명령어로 만든 물품이면 슬롯을 넣어준다.
	if(bSEQCreate == true)
	{
		if(slotnum > 0)		pclitem->siUseCrystalNum = slotnum;
	}

	// 기본 물품으로 만들어야 하는 설정이면 리턴. 
	if((orgqualityrate == 0)&&(type != ITEMTYPE_CRYSTAL))
	{
		if (usedate == 0 && type != ITEMTYPE_NECK)
			pclitem->uiDateUseDay = 0;

		SI32 rndStateKind	= 0;
		SI32 rndStateAmount = 0;

		// 아이템 몰 상품이며 의복이나 모자이면 부적을 발라준다.
		if( IsItemInfoAtb(siunique,ITEMINFOATB_ITEMMALL) == true )
		{
			if ( type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT )
			{
				pclitem->clItemStatusArmour.clOptionInfo.uiType1 = SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON ;
				pclitem->clItemStatusArmour.clOptionInfo.uiPara1 = 20 ;

				// 가차 목록에 들어간 아이템이라면 랜덤으로 능력치를 넣어준다.
				if ( pclserver->pclGachaManager->IsGachaRareItem( siunique ) == true )
				{
					pclitem->clItemStatusArmour.clOptionInfo.uiType1 = SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON ;

					SI32 rndWeaponSkillUpLevel = 0;
					SI32 rndMakeSkillUpLevel = 0;

					// PCK : 무기술을 4~8까지 랜덤으로 준다.
					// PCK : 제조술을 4~8까지 랜덤으로 준다.
					if(	  siunique == ITEMUNIQUE(23734) || siunique == ITEMUNIQUE(23735) || siunique == ITEMUNIQUE(23115) || siunique == ITEMUNIQUE(23116)		// 폭주족 의복
					   || siunique == ITEMUNIQUE(23696) || siunique == ITEMUNIQUE(23698) || siunique == ITEMUNIQUE(23117) || siunique == ITEMUNIQUE(23118)		// 결혼 예복
					   || siunique == ITEMUNIQUE(23410) || siunique == ITEMUNIQUE(23411) || siunique == ITEMUNIQUE(23742) || siunique == ITEMUNIQUE(23741)  )	// 악마 의복		
					{
						rndWeaponSkillUpLevel	= rand() % 5 + 4;
						rndMakeSkillUpLevel		= rand() % 5 + 4;
					}
					else
					{
						rndWeaponSkillUpLevel	= rand() % 5 + 3;
						rndMakeSkillUpLevel		= rand() % 5 + 3;
					}
					
					pclitem->clItemStatusArmour.clOptionInfo.uiPara1 = rndWeaponSkillUpLevel * 10 ;
					pclitem->clItemStatusArmour.uiSkillUp			 = rndMakeSkillUpLevel;

					rndStateKind   = rand() % 6;
					rndStateAmount = 0;

					// PCK : 폭주족 의복의 경우 능력치를 20으로 정한다.
					// PCK : 결혼 예복 추가 (09.05.06)
					if(   siunique == ITEMUNIQUE(23734) || siunique == ITEMUNIQUE(23735) || siunique == ITEMUNIQUE(23115) || siunique == ITEMUNIQUE(23116)			// 폭주족 의복
					   || siunique == ITEMUNIQUE(23696) || siunique == ITEMUNIQUE(23698) || siunique == ITEMUNIQUE(23117) || siunique == ITEMUNIQUE(23118)			// 결혼 예복
					   || siunique == ITEMUNIQUE(23410) || siunique == ITEMUNIQUE(23411 )|| siunique == ITEMUNIQUE(23742) || siunique == ITEMUNIQUE(23741) )		// 악마 의복	
					{
						rndStateAmount = 20;
					}
					else
					{
						rndStateAmount = 15;
					}

					switch( rndStateKind )
					{
						case 0 : pclitem->clItemStatusArmour.uiStr = rndStateAmount; break;
						case 1 : pclitem->clItemStatusArmour.uiDex = rndStateAmount; break;
						case 2 : pclitem->clItemStatusArmour.uiVit = rndStateAmount; break;
						case 3 : pclitem->clItemStatusArmour.uiMag = rndStateAmount; break;
						case 4 : pclitem->clItemStatusArmour.uiHnd = rndStateAmount; break;
						case 5 : pclitem->clItemStatusArmour.uiLuk = rndStateAmount; break;
					}
				}
			}
		}
		
		// 드레스와 모자인경우 무기 제조술과 장비 제조술 처리.
		if( type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT )
		{
			// 테이블의 값을 적용하는 아이템이면.
			if( UPGRADESKILLTYPE_USE_TABLE_VALUE & pclItemInfo[ref]->siUpgradeSkillType ) 
			{
				pclitem->clItemStatusArmour.clOptionInfo.uiType1 = SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON ;
				pclitem->clItemStatusArmour.clOptionInfo.uiPara1 = 0;

				// 가차 목록에 들어간 아이템이라면 랜덤으로 능력치를 넣어준다.
				if ( pclserver->pclGachaManager->IsGachaRareItem( siunique ) == true )
				{
					if( UPGRADESKILLTYPE_USEWEAPON & pclItemInfo[ref]->siUpgradeSkillType )
					{
						pclitem->clItemStatusArmour.clOptionInfo.uiPara1 = ((rand() % 5) + pclItemInfo[ref]->clItem.GetAC(this)) * 10 ;
					}
					if( UPGRADESKILLTYPE_MAKE & pclItemInfo[ref]->siUpgradeSkillType )
					{
						pclitem->clItemStatusArmour.uiSkillUp = ((rand() % 5) + pclItemInfo[ref]->clItem.GetAC(this));
					}
				}
				else
				{
					if( UPGRADESKILLTYPE_USEWEAPON & pclItemInfo[ref]->siUpgradeSkillType )
					{
						pclitem->clItemStatusArmour.clOptionInfo.uiPara1 = pclItemInfo[ref]->clItem.GetAC(this) * 10 ;
					}
					if( UPGRADESKILLTYPE_MAKE & pclItemInfo[ref]->siUpgradeSkillType )
					{
						pclitem->clItemStatusArmour.uiSkillUp = pclItemInfo[ref]->clItem.GetAC(this);
					}
				}
			}
			
			//[진성] 스테이터스 20을 적용한다.
			if( UPGRADESKILLTYPE_STATUS_UP_20 & pclItemInfo[ref]->siUpgradeSkillType ) 
			{
				rndStateAmount = 20;
				switch( rndStateKind )
				{
					case 0 : pclitem->clItemStatusArmour.uiStr = rndStateAmount; break;
					case 1 : pclitem->clItemStatusArmour.uiDex = rndStateAmount; break;
					case 2 : pclitem->clItemStatusArmour.uiVit = rndStateAmount; break;
					case 3 : pclitem->clItemStatusArmour.uiMag = rndStateAmount; break;
					case 4 : pclitem->clItemStatusArmour.uiHnd = rndStateAmount; break;
					case 5 : pclitem->clItemStatusArmour.uiLuk = rndStateAmount; break;
				}
			}
		}

		return true;
	}

	// 유저가 직접 제조한 아이템이면서 의복이나 모자라면 부적을 발라준다.
	if ( (type == ITEMTYPE_DRESS || type == ITEMTYPE_HAT) &&(siunique != ITEMUNIQUE(23061)))  // 산타모자만 예외다;;
	{
		pclitem->clItemStatusArmour.clOptionInfo.uiType1 = SCROLL_UNIQUE_FUNCTION_USESKILLUP_WEAPON ;
		pclitem->clItemStatusArmour.clOptionInfo.uiPara1 = 20 ;
	}

	// 품질 확률에 따라 아이템의 능력치를 조정한다.
	switch(type)
	{
	//KHY - 20090526 - 아바타 결정체.
	case ITEMTYPE_CRYSTAL:
		{
			if(IsItemInfoAtb(siunique,ITEMINFOATB_ITEMMALL) == true ) // 결정체 이면서 아이템 몰 상품 이라면.
			{
				// 능력치를 결정해 버린다.
				MakeCrystal(pclitem, qualityrate);
			}
		}
		break;
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
		{
			SI32 randaddrate = rand() % 2 ;	// 미니게임 결과를 적용할지 결정

			// 공격력을 설정한다. 
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;	// 운영자 설정값
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);

			if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
			{
				// 기존 수치의 최대 110%까지만 늘려준다.
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}

			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산

			MakeApower(pclitem, qualityrate);

			// 도력 향상을 설정한다. 
			if(pclitem->clItemWeapon.uiMag)
			{
				if ( magrate != 0 && magrate > 0 && magrate <= 500 )
					qualityrate = magrate ;
				else
					qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);	// -> 지속적으로 감소해야 한다. 

				if ( randaddrate == 1 )	// 미니게임 결과 적용 ->  퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
				{
					randaddrate = rand() % 2 ;	// 다시한번 미니게임 결과를 적용할지 결정한다.
					if ( randaddrate == 0 )
					{
						qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
					}
				}

				// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
				if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
				{
					if(orgqualityrate < 100)
					{
						qualityrate = orgqualityrate;
						// 고물품
						*pbrareswitch = -1;
					}
				}

				SI16 beforerareswitch = *pbrareswitch;

				if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
				else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
				else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산

				UI32 basicMag = pclitem->clItemWeapon.uiMag;

				pclitem->clItemWeapon.uiMag = min(1000, pclitem->clItemWeapon.uiMag * qualityrate / 100);

				if( beforerareswitch <= 0 && *pbrareswitch > 0 && basicMag == pclitem->clItemWeapon.uiMag)
				{
					*pbrareswitch = 0; //일반품으로 
				}
			}

			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}

			// ITEMTYPE_CANNON이 아니고, 고급품인 경우 사용기간을 설정한다. - By LEEKH
			if(type != ITEMTYPE_CANNON && *pbrareswitch == 3)
			{
				pclitem->uiDateUseDay = GetHighItemDate();
			}

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
				if ( 3 == *pbrareswitch )
				{
					UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
					UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
					UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

					if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
					{
						pclitem->uiDateUseDay = 0;
					}
				}
			}

			/*if ( type == ITEMTYPE_TORPEDO )
			{
				if ( randaddrate == 0 )
				{
					// 내구도를 설정한다.
					MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
				}
				else
				{
					// 내구도를 설정한다.
					MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
				}
			}*/ // -- 내구도 코드 삭제 - by LEEKH 2007.07.30
			/*else
			{
			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate,monstercorp);
			}
			}*/
		}
		break;

	case ITEMTYPE_HELMET:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_BELT:
		{
			SI32 randaddrate = rand() % 2 ;
			// 방어력을 설정한다. 
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);

			if ( randaddrate == 0 )
			{
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}

			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산


			MakeDpower(pclitem, qualityrate);

			// 행운을 설정한다. 
			if(pclitem->clItemArmour.uiLuck)
			{
				if ( magrate != 0 && magrate > 0 && magrate <= 500 )
					qualityrate = magrate ;
				else
					qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
				if ( randaddrate == 1 )
				{
					randaddrate = rand() % 2 ;
					if ( randaddrate == 0 )
					{
						qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
					}
				}

				// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
				if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
				{
					if(orgqualityrate < 100)
					{
						qualityrate = orgqualityrate;
						// 고물품
						*pbrareswitch = -1;
					}
				}

				SI16 beforerareswitch = *pbrareswitch;

				if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
				else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
				else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산

				UI32 basicLuck = pclitem->clItemArmour.uiLuck;

				pclitem->clItemArmour.uiLuck	= min(500, pclitem->clItemArmour.uiLuck * qualityrate / 100 );

				if( beforerareswitch <= 0 && *pbrareswitch > 0 && basicLuck == pclitem->clItemArmour.uiLuck)
				{
					*pbrareswitch = 0; //일반품으로 
				}
			}

			// 기술 향상을 설정한다. 
			if(pclitem->clItemArmour.uiSkillUp)
			{
				if ( skillrate != 0 && skillrate > 0 && skillrate <= 500 )
					qualityrate = skillrate ;
				else
					qualityrate = GetAverage( max(10, orgqualityrate) , bCanMakeRareItemSwitch, addlevel);
				if ( randaddrate == 1 )
				{
					randaddrate = rand() % 2 ;
					if ( randaddrate == 0 )
					{
						qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
					}
				}

				// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
				if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
				{
					if(orgqualityrate < 100)
					{
						qualityrate = orgqualityrate;
						// 고물품
						*pbrareswitch = -1;
					}
				}

                SI16 beforerareswitch = *pbrareswitch;

				if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
				else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
				else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산

				UI32 basicSkillUp = pclitem->clItemArmour.uiSkillUp;

				// [기형] 2010.02.02 "무기 사용술 증가" 스킬 타입 일 경우 아이템 퀄리티와는 상관 없이 기본값으로 한다.
				if(!( pclItemInfo[ref]->siUpgradeSkillType & UPGRADESKILLTYPE_USEWEAPON))
				{
					pclitem->clItemArmour.uiSkillUp	= min(60, pclitem->clItemArmour.uiSkillUp * qualityrate / 100 );
				}

				if( beforerareswitch <= 0 && *pbrareswitch > 0 && basicSkillUp == pclitem->clItemArmour.uiSkillUp)
				{
					*pbrareswitch = 0; //일반품으로 
				}
			}

			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}

			// 고급품인 경우 사용기간을 설정한다. - By LEEKH
			if ( *pbrareswitch == 3 )
			{
				pclitem->uiDateUseDay = GetHighItemDate();
			}

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
				if ( 3 == *pbrareswitch )
				{
					UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
					UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
					UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

					if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
					{
						pclitem->uiDateUseDay = 0;
					}
				}
			}

			/*			//MakeDur(pbrareswitch, 100, pclitem, true);
			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}*/
		}
		break;
	case ITEMTYPE_HAT:
	case ITEMTYPE_DRESS:
		{
			SI32 randaddrate = rand() % 2 ;
			// 방어력을 설정한다. 
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);

			if ( randaddrate == 0 )
			{
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}

			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산


			//MakeDpower(pclitem, qualityrate);

			//// 행운을 설정한다. 
			//if(pclitem->clItemArmour.uiLuck)
			//{
			//	if ( magrate != 0 && magrate > 0 && magrate <= 500 )
			//		qualityrate = magrate ;
			//	else
			//		qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			//	if ( randaddrate == 1 )
			//	{
			//		randaddrate = rand() % 2 ;
			//		if ( randaddrate == 0 )
			//		{
			//			qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			//		}
			//	}
			//	if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			//	else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산

			//	pclitem->clItemArmour.uiLuck	= min(500, pclitem->clItemArmour.uiLuck * qualityrate / 100 );
			//}

			//// 기술 향상을 설정한다. 
			//if(pclitem->clItemArmour.uiSkillUp)
			//{
			//	if ( skillrate != 0 && skillrate > 0 && skillrate <= 500 )
			//		qualityrate = skillrate ;
			//	else
			//		qualityrate = GetAverage( max(10, orgqualityrate) , bCanMakeRareItemSwitch, addlevel);
			//	if ( randaddrate == 1 )
			//	{
			//		randaddrate = rand() % 2 ;
			//		if ( randaddrate == 0 )
			//		{
			//			qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			//		}
			//	}
			//	if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			//	else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산

			//	pclitem->clItemArmour.uiSkillUp	= min(60, pclitem->clItemArmour.uiSkillUp * qualityrate / 100 );
			//}

			/*			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate,monstercorp);
			}*/
			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}
		}
		break;
	case ITEMTYPE_BAG:
		// 방어력을 설정한다. 
		qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);

		if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
		else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산


		pclitem->clItemBag.uiAC	= max(1, pclitem->clItemBag.uiAC * qualityrate / 100 );

		// 내구도를 설정한다. - 내구도 삭제 - by LEEKH 2007.04.25
		//MakeDur(pbrareswitch, orgqualityrate,  addlevel, pclitem, false);
		break;

	case ITEMTYPE_MANTLE:
		// 방어력을 설정한다. 
		qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
		if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
		else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산

		pclitem->clItemMantle.uiAC	= max(1, pclitem->clItemMantle.uiAC * qualityrate / 100 );

		// 내구도를 설정한다. - 내구도 삭제 - by LEEKH 2007.04.25
		//MakeDur(pbrareswitch, orgqualityrate,  addlevel, pclitem, false);
		break;

		// 아바타 망토
	case ITEMTYPE_AVATAR_MANTLE:
		{
			// 아이템의 기본적인 qualityrate 를 결정한다.
			//---------------------------------------------------------------------------------------------
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			//---------------------------------------------------------------------------------------------

			// 아이템에 미니게임에 의한 qualityrate 증가를 적용한다.
			//---------------------------------------------------------------------------------------------
			SI32 randaddrate = rand() % 2 ;

			if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
			{
				// 기존 수치의 최대 110%까지만 늘려준다.
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}
			//---------------------------------------------------------------------------------------------

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			//---------------------------------------------------------------------------------------------
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}
			//---------------------------------------------------------------------------------------------
			// 1차 방어력만가지고, 명품을 가린다.
			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산


			// 방어력 설정
			pclitem->clItemAvatarMantle.uiAC	= max(1, pclitem->clItemAvatarMantle.uiAC * qualityrate / 100 );

			//  아바타 망토는 방어력만 있다.

			// 만약, 기타 다른 스택이 추가 되어, 추가 상승을 원할 결우, 이부분에 추가.

			// 사용 가능 날짜를 MakeDate() 함수를 이용해 세팅한다.
			// 호품, 명품, 신기에 따라 사용기간 설정. - 내부에서 한다.
			// bforseswitch = true , 기간에 대한, 중감이 없다. - 명품이 안만들어진다.
			//MakeAvatarDate( pbrareswitch, 100, addlevel, pclitem, true, durrate );

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
				if ( 3 == *pbrareswitch )
				{
					UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
					UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
					UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

					if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
					{
						pclitem->uiDateUseDay = 0;
					}
				}
			}
		}
		break;

	// 아바타 의복
	case ITEMTYPE_AVATAR_DRESS:
		{
			// 아바타 의복은 다른 능력이 없음으로, 무의미 하다.
			/*
			// 아이템의 기본적인 qualityrate 를 결정한다.
			//---------------------------------------------------------------------------------------------
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			//---------------------------------------------------------------------------------------------

			// 아이템에 미니게임에 의한 qualityrate 증가를 적용한다.
			//---------------------------------------------------------------------------------------------
			SI32 randaddrate = rand() % 2 ;

			if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
			{
				// 기존 수치의 최대 110%까지만 늘려준다.
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}
			//---------------------------------------------------------------------------------------------

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			//---------------------------------------------------------------------------------------------
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}
			//---------------------------------------------------------------------------------------------
			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산
			*/
			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			//---------------------------------------------------------------------------------------------
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}

			// 아바타 드레스는 날짜 밖에 없다.


			// 사용 가능 날짜를 MakeDate() 함수를 이용해 세팅한다.
			// 호품, 명품, 신기에 따라 사용기간 설정. - 내부에서 한다.
			// bforseswitch = false , 기간에 대한, 중감이 있다. - 명품이 만들어진다.
			MakeAvatarDate( pbrareswitch, orgqualityrate, addlevel, pclitem, false, durrate );

			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false, durrate);
		}
		break;

	// 아바타 모자
	case ITEMTYPE_AVATAR_HAT:
		{
			// 다른 능력이 없는한, 무의미하다.
			/*
			// 아이템의 기본적인 qualityrate 를 결정한다.
			//---------------------------------------------------------------------------------------------
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			//---------------------------------------------------------------------------------------------

			// 아이템에 미니게임에 의한 qualityrate 증가를 적용한다.
			//---------------------------------------------------------------------------------------------
			SI32 randaddrate = rand() % 2 ;

			if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
			{
				// 기존 수치의 최대 110%까지만 늘려준다.
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}
			//---------------------------------------------------------------------------------------------

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			//---------------------------------------------------------------------------------------------
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}
			//---------------------------------------------------------------------------------------------
			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산
			*/

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			//---------------------------------------------------------------------------------------------
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}
			
			// 사용 가능 날짜를 MakeDate() 함수를 이용해 세팅한다.
			// 호품, 명품, 신기에 따라 사용기간 설정. - 내부에서 한다.
			// bforseswitch = false , 기간에 대한, 중감이 있다. - 명품이 만들어진다.
			MakeAvatarDate( pbrareswitch, orgqualityrate, addlevel, pclitem, false, durrate );

			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
		}
		break;

	// 아바타 목걸이
	case ITEMTYPE_AVATAR_NECK:
		{
			// 아이템의 기본적인 qualityrate 를 결정한다.
			//---------------------------------------------------------------------------------------------
			//if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
			//	qualityrate = attackrate ;
			//else
			//	qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);

			if(pclitem->clItemAvatarNeck.uiSkillValue >0 )
			{
				if ( skillrate != 0 && skillrate > 0 && skillrate <= 500 )
					qualityrate = skillrate ;
				else
					qualityrate = GetAverage( max(10, orgqualityrate) , bCanMakeRareItemSwitch, addlevel);

				//---------------------------------------------------------------------------------------------
				// 아이템에 미니게임에 의한 qualityrate 증가를 적용한다.
				//---------------------------------------------------------------------------------------------
				SI32 randaddrate = rand() % 2 ;

				if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
				{
					// 기존 수치의 최대 110%까지만 늘려준다.
					qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
				}
				//---------------------------------------------------------------------------------------------

				// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
				//---------------------------------------------------------------------------------------------
				if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
				{
					if(orgqualityrate < 100)
					{
						qualityrate = orgqualityrate;
						// 고물품
						*pbrareswitch = -1;
					}
				}
				//---------------------------------------------------------------------------------------------
				if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
				else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
				else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산


				// 최소 최대값이 설정되는, 부분을 먼저 설정한다.
				//SI32 skillup = (rand() % (TABS(pclitem->clItemAvatarNeck.uiMaxSkillPoint - pclitem->clItemAvatarNeck.uiSkillValue) + 1)) + pclitem->clItemAvatarNeck.uiSkillValue;
				//pclitem->clItemAvatarNeck.uiSkillValue	= max(pclitem->clItemAvatarNeck.uiSkillValue, (UI32)skillup);

				//SI32 usedate = (rand() % (TABS(pclitem->clItemAvatarNeck.uiMaxUsedate - pclitem->uiDateUseDay) + 1)) + pclitem->uiDateUseDay;
				//pclitem->uiDateUseDay = max(pclitem->uiDateUseDay, usedate);

				// qualityrate 에 따른 아바타 목걸이 스킬 포인트.
				//SI32 skillup = (float)pclitem->clItemAvatarNeck.uiMaxSkillPoint * (float)(qualityrate/100);
				//skillup = max(1,skillup);

				// 아바타 목걸이의 스킬 설정.
				//---------------------------------------------------------------------------------------------
				// 아이템의 ref을 구한다. 
				SI32 ref = FindItemRefFromUnique(siunique);
				if(ref <= 0)
				{
#ifdef _DEBUG
					_ASSERT( ref > 0 );
#endif
					return false;    
				}
				else
				{
					// 기본 스킬 타입을 가지고 세부 스킬을 선택한다.
					SI32 szIncSkillUnique =  pclitem->SelectUseBaseSkillTypeToAvatar(pclItemInfo[ref]->siUpgradeSkillUnique);

					// 스킬 설정.
					 pclitem->clItemAvatarNeck.uiUpgradeSkillUnique	=  szIncSkillUnique;

					// 지연 : 기존에 레어 등급 따라 아이템의 스킬 증가 부분이 적용되도록 변경					
					pclitem->clItemAvatarNeck.uiSkillValue = max(pclitem->clItemAvatarNeck.uiSkillValue, pclitem->clItemAvatarNeck.uiSkillValue * qualityrate / 100 );
				}
			}

			// 아바타 목걸이는 스킬만 있다.

			// 사용 가능 날짜를 MakeDate() 함수를 이용해 세팅한다.
			// 호품, 명품, 신기에 따라 사용기간 설정. - 내부에서 한다.
			// bforseswitch = true , 기간에 대한, 중감이 없다. - 명품이 안만들어진다.
			//MakeAvatarDate( pbrareswitch, 100, addlevel, pclitem, true, durrate );    << 야바타

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
				if ( 3 == *pbrareswitch )
				{
					UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
					UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
					UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

					if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
					{
						pclitem->uiDateUseDay = 0;
					}
				}
			}
		}
		break;

	// 아바타 반지
	case ITEMTYPE_AVATAR_RING:
		{
			// 아이템의 기본적인 qualityrate 를 결정한다.
			//---------------------------------------------------------------------------------------------
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			//---------------------------------------------------------------------------------------------

			// 아이템에 미니게임에 의한 qualityrate 증가를 적용한다.
			//---------------------------------------------------------------------------------------------
			SI32 randaddrate = rand() % 2 ;

			if ( randaddrate == 0 )	// 미니게임 결과 적용 -> 퀄리티Rate 증가 (공격력과 도력중 한번만 증가한다.)
			{
				// 기존 수치의 최대 110%까지만 늘려준다.
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}
			//---------------------------------------------------------------------------------------------

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			//---------------------------------------------------------------------------------------------
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}
			//---------------------------------------------------------------------------------------------
			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산
			
			// 지연 : 레어 수치에 따라 반지의 스탯의 수치가 달라진다. (좋은건 스탯등이 더 많이 증가)
			pclitem->clItemAvatarRing.SetQualityRate( qualityrate );

			// 아바타 반지는 스택만 있다.

			// 사용 가능 날짜를 MakeDate() 함수를 이용해 세팅한다.
			// 호품, 명품, 신기에 따라 사용기간 설정. - 내부에서 한다.
			// bforseswitch = true , 기간에 대한, 중감이 없다. - 명품이 안만들어진다.
			//MakeAvatarDate( pbrareswitch, 100, addlevel, pclitem, true, durrate );

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
				if ( 3 == *pbrareswitch )
				{
					UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
					UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
					UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

					if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
					{
						pclitem->uiDateUseDay = 0;
					}
				}
			}
		}
		break;


	case ITEMTYPE_SHOES:
		{
			SI32 randaddrate = rand() % 2 ;
			// 방어력을 설정한다. 
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			if ( randaddrate == 0 )
			{
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}

			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산


			pclitem->clItemShoes.uiAC	= max(1, pclitem->clItemShoes.uiAC * qualityrate / 100 );

			// 이동속도를 설정한다. 
			if ( magrate != 0 && magrate > 0 && magrate <= 500 )
				qualityrate = magrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			if ( randaddrate == 1 )
			{
				randaddrate = rand() % 2 ;
				if ( randaddrate == 0 )
				{
					qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
				}
			}

			// 제조실패방지 - 고물품은 능력이 100%보다 낮다. - by LEEKH 2007.07.23
			if( pclClient->IsCountrySwitch( Switch_DoNotFailedMakeItem ) )
			{
				if(orgqualityrate < 100)
				{
					qualityrate = orgqualityrate;
					// 고물품
					*pbrareswitch = -1;
				}
			}

			SI16 beforerareswitch = *pbrareswitch;

			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산
			else if ( qualityrate >= RARE3_ITEM_RATE && *pbrareswitch != 1 && *pbrareswitch != 2 )*pbrareswitch = 3; // 고급품생산

			UI32 basicSpeedUp = pclitem->clItemShoes.uiMoveSpeed;

			pclitem->clItemShoes.uiMoveSpeed	= min(9, max(1, pclitem->clItemShoes.uiMoveSpeed * qualityrate / 100 ));

			if( beforerareswitch <= 0 && *pbrareswitch > 0 && basicSpeedUp == pclitem->clItemShoes.uiMoveSpeed )
				*pbrareswitch = 0; //일반품으로 

			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false, durrate, monstercorp);
			}

			// 고급품인 경우 사용기간을 설정한다. - By LEEKH
			if ( *pbrareswitch == 3 )
			{
				pclitem->uiDateUseDay = GetHighItemDate();
			}

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				// 신기이면서 무한내구도가 아니면 물품 날짜가 없음
				if ( 3 == *pbrareswitch )
				{
					UI32 uiMaxDurability		= (UI32)pclitem->clItemCommon.clCommonInfo.uiMaxDurability;
					UI32 uiForeverDurability	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability") );
					UI32 uiForeverDurability2	= (UI32)pclClient->GetGlobalValue( TEXT("GV_Forever_Durability2") );

					if ( (uiForeverDurability != uiMaxDurability) && (uiForeverDurability2 != uiMaxDurability) )
					{
						pclitem->uiDateUseDay = 0;
					}
				}
			}

			/*			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate,monstercorp);
			}*/
			/*			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}*/
		}
		break;

	case ITEMTYPE_RING:
		{
			SI32 randaddrate = rand() % 2 ;
			if ( attackrate != 0 && attackrate > 0 && attackrate <= 500 )
				qualityrate = attackrate ;
			else
				qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
			if ( randaddrate == 0 )
			{
				qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
			}
			if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
			else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산

			// 근력 
			pclitem->clItemRing.uiStr	= pclitem->clItemRing.uiStr * (qualityrate / 100);
			// 순발력
			pclitem->clItemRing.uiDex	= pclitem->clItemRing.uiDex * (qualityrate / 100) ;
			// 도력
			pclitem->clItemRing.uiMag	= pclitem->clItemRing.uiMag * (qualityrate / 100) ;
			// 생명력 
			pclitem->clItemRing.uiVit	= pclitem->clItemRing.uiVit * (qualityrate / 100) ;
			// 운 
			pclitem->clItemRing.uiLuk	= pclitem->clItemRing.uiLuk * (qualityrate / 100) ;
			// 손재주 
			pclitem->clItemRing.uiHnd	= pclitem->clItemRing.uiHnd * (qualityrate / 100) ;

			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}

			/*			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate,monstercorp);
			}*/
			/*			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate,monstercorp);
			}*/
		}
		break;

	case ITEMTYPE_NECK:
		{
			SI32 randaddrate = rand() % 2 ;
			// 기술 향상을 설정한다. 
			if(pclitem->clItemNeck.uiSkillPointUp)
			{
				if ( skillrate != 0 && skillrate > 0 && skillrate <= 500 )
					qualityrate = skillrate ;
				else
					qualityrate = GetAverage(orgqualityrate, bCanMakeRareItemSwitch, addlevel);
				if ( randaddrate == 0 )
				{
					qualityrate = qualityrate + ( qualityrate * monstercorp / 100000 ) ;
				}
				if(qualityrate >= RARE2_ITEM_RATE)*pbrareswitch = 1;		// 명품 생산
				else if ( qualityrate >= RARE1_ITEM_RATE && *pbrareswitch != 1 )*pbrareswitch = 2; // 호품생산

				pclitem->clItemNeck.uiSkillPointUp	= min(60, pclitem->clItemNeck.uiSkillPointUp * qualityrate / 100 );
			}

			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);
			}

			/*			if ( randaddrate == 0 )
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate);
			}
			else
			{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, pclitem, false,durrate,monstercorp);
			}*/
			if ( randaddrate == 0 )
			{
				// 내구도를 설정한다.
				MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate);
			}
			else
			{
				// 내구도를 설정한다.
				MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate, monstercorp);
			}
		}
		break;

	case ITEMTYPE_IRON:
	case ITEMTYPE_DISH:		
	case ITEMTYPE_PAPER:
	case ITEMTYPE_FISHINGROD:
		{
			// 내구도를 설정한다.
			MakeDur(pbrareswitch, orgqualityrate, addlevel, pclitem, false,durrate,monstercorp);

			if ( pclClient->IsCountrySwitch(Switch_NewDurability) )
			{
				if ( 0 < pclitem->uiDateUseDay )
				{
					MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate,monstercorp);
				}
			}
			else
			{
				MakeDate(pbrareswitch, orgqualityrate, addlevel,  pclitem, false,durrate,monstercorp);
			}
		}
		break;
	}
	// 중국은 고급품 막는다.

	if ( pclClient->IsWhereServiceArea( ConstServiceArea_China) )
	{
		if ( *pbrareswitch == 3 )
			*pbrareswitch = 0;
	}

	//--------------------------------------------------------
	// 명품이 만들어지면 일정 시간 못만들도록 설정한다. 
	//--------------------------------------------------------
	// [춘기] /SEQ 로 만드는 아이템은 시간적용없이 바로 만들수 있게 한다.
	if(bSEQCreate == true)
	{
		//명품
		if(*pbrareswitch == 1)
		{
			pclitem->clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_2;
		}
		// 호품
		else if(*pbrareswitch == 2)
		{
			pclitem->clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_1;
		}
		// 고급품
		else if(*pbrareswitch == 3)
		{
			pclitem->clItemCommon.clCommonInfo.uiRareInfo = RARELEVEL_5;
		}

		return true;
	}

	if( *pbrareswitch > 0 && *pbrareswitch < 3 )
	{
		bCanMakeRareItemSwitch	= false;
		dwLastMakeRareItemClock = pclClient->CurrentClock;
	}
	else
	{
		if(bCanMakeRareItemSwitch == false && TABS(pclClient->CurrentClock - dwLastMakeRareItemClock) >= 600000)
		{
			bCanMakeRareItemSwitch = true;
		}
	}

	return true;
}


bool cltItemManagerCommon::MakeDefaultItem(SI32 siunique, SI32 itemnum, cltItem* pclitem)
{
	if(siunique == 0)		return false;
	if(itemnum == 0)		return false;
	if(pclitem == NULL)		return false;

	cltServer* pclserver = (cltServer*)pclClient;
	if(pclserver == NULL)		return false;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)
	{
#ifdef _DEBUG
		_ASSERT( ref > 0 );
#endif
		return false;
	}

	pclitem->Set(&pclItemInfo[ref]->clItem);

	pclitem->Element.siType = ENCHANT_TYPE_DEFAULT;
	pclitem->Element.siPower = 0;

	return true;
}


// 주어진 재료를 근거로 생산 가능한 아이템의 리스트를 작성해서 리턴한다. 
// 얻어온 리스트는 사용후 삭제해 주어야 한다. 
// pclskill					: 사용자의 기술 정보.(NULL이면 기술은 무시된다.) 
// bonlymaterialcheckswitch : 원료만 가지고 만들수 있는 품목을 체크한다.(도구는 무시됨) 
#define MAX_FINDMAKABLEITEM_NUM		100
bool cltItemManagerCommon::FindMakeableItem(cltPSkillInfo* pclskill, cltPItemInfo* pclcharitem, cltBasicAbility* pclBA, SI32 materialnum, cltItem pclmaterialitem[], SI32* pmakeableitemnum, SI32** ppitemlist, bool bonlymaterialcheckswitch)
{
	SI32	palliteminfo[MAX_FINDMAKABLEITEM_NUM * 2];
	ZeroMemory(palliteminfo, sizeof(SI32) * MAX_FINDMAKABLEITEM_NUM * 2);
	
	SI32 i;
	BOOL bTutorialMake = FALSE;
	SI32 siTutorialItemUnique = 0;
	BOOL bTutorialLevelMake = FALSE;
	SI32 siTutorialItemLevel = 0;

	SI32 producenum;
	SI32 returnval;
	SI32 returnpara1;

	SI16 questtype = 0, questunique = 0, para1 = 0, para2 = 0;
	SI16 questclearcond =  QUEST_CLEAR_COND_TUTORIALMAKEITEM;

	SI16 siAntiPotion	= ITEMUNIQUE(2796);
	SI16 siLostWorldQuestItem1 = ITEMUNIQUE(8115);
	SI16 siLostWorldQuestItem2 = ITEMUNIQUE(8116);
	SI16 siLostWorldQuestItem3 = ITEMUNIQUE(8117);
	SI16 siLostWorldQuestItem4 = ITEMUNIQUE(8118);

	BOOL bAntiPotion	= FALSE;
	BOOL bLostWorldQuestItem1 = FALSE;
	BOOL bLostWorldQuestItem2 = FALSE;
	BOOL bLostWorldQuestItem3 = FALSE;
	BOOL bLostWorldQuestItem4 = FALSE;

	// 퀘스트 확인은 클라이언트만 한다. - by LEEKH 2008-09-09
	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		cltCharClient* pclcharclient = pclClient->pclCM->GetCharClient( 1 );
		if ( pclcharclient )
		{
			if(pclcharclient->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
			{
				if(	( pclcharclient->pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == para1) 
					&& ( pclcharclient->pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 < para2) )
				{
					bTutorialMake = TRUE;
					siTutorialItemUnique = para1;
				}
			}//퀘스트 중인지 확인 한다.

			questclearcond =  QUEST_CLEAR_COND_TUTORIALMAKELEVELITEM;
			if(pclcharclient->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
			{
				if(	( pclcharclient->pclCI->clQuestInfo.clUnit[questtype].siQuestPara1 == para1) 
					&& ( pclcharclient->pclCI->clQuestInfo.clUnit[questtype].siQuestPara2 < para2) )
				{
					bTutorialLevelMake = TRUE;
					siTutorialItemLevel = para1;
				}
			}//퀘스트 중인지 확인 한다.

			// 안티포션(2796)은 해당퀘스트 제조가 걸렸을때만 가능하다
			questclearcond = QUEST_CLEAR_COND_MAKEITEM;
			if ( true == pclcharclient->pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, siAntiPotion) )
			{
				bAntiPotion = TRUE;
			}
			// 안티포션체크 여기까지

			// PCK : 잃어버린 세계 퀘스트 제조 아이템은 해당 퀘스트 아이템 제조가 걸렸을때만 가능하다.(09.05.12)
			if ( true == pclcharclient->pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, siLostWorldQuestItem1))
			{
				bLostWorldQuestItem1 = TRUE;
			}

			if ( true == pclcharclient->pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, siLostWorldQuestItem2))
			{
				bLostWorldQuestItem2 = TRUE;
			}

			if ( true == pclcharclient->pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, siLostWorldQuestItem3))
			{
				bLostWorldQuestItem3 = TRUE;
			}

			if ( true == pclcharclient->pclCI->clQuestInfo.IsSameParaQuestGoing(questclearcond, siLostWorldQuestItem4))
			{
				bLostWorldQuestItem4 = TRUE;
			}
			// 잃어버린 세계 퀘스트 제조 체크 여기까지
		}
	}

	// 초기화 해줘야 한다.
	*pmakeableitemnum	= 0;

	for(i = 0; i<MAX_ITEMINFO_NUMBER;i++)
	{
		if(pclItemInfo[i] == NULL)		continue;

		// 안티포션은 해당아이템 제조 퀘스트때만 만들수 있도록 한다
		if ( (siAntiPotion == pclItemInfo[i]->clItem.siUnique) )
		{
			if ( FALSE == bAntiPotion )	continue;
		}
		
		// PCK : 잃어버린 세계 퀘스트용 아이템은 해당 아이템 제조 퀘스트때만 만들수 있도록 한다. (09.05.12)
		if ( (siLostWorldQuestItem1 == pclItemInfo[i]->clItem.siUnique) )
		{
			if ( FALSE == bLostWorldQuestItem1 )	continue;
		}
		if ( (siLostWorldQuestItem2 == pclItemInfo[i]->clItem.siUnique) )
		{
			if ( FALSE == bLostWorldQuestItem2 )	continue;
		}
		if ( (siLostWorldQuestItem3 == pclItemInfo[i]->clItem.siUnique) )
		{
			if ( FALSE == bLostWorldQuestItem3 )	continue;
		}
		if ( (siLostWorldQuestItem4 == pclItemInfo[i]->clItem.siUnique) )
		{
			if ( FALSE == bLostWorldQuestItem4 )	continue;
		}

		// 튜토리얼 퀘스트 중이라면 스킬레벨은 체크하지 않는다. 퀘스트 재료가 있다면 재료를 재설정해 주기도 한다.
		if( (bTutorialMake && siTutorialItemUnique == pclItemInfo[i]->clItem.siUnique) || 
			(bTutorialLevelMake && siTutorialItemLevel == pclItemInfo[i]->clCondition.siLevel) )
		{
			cltMaterialInfo	TutorialQuestMaterial[MAX_MATERIAL_NUMBER];
			BOOL bUseQuestItem = FALSE;

			if(pclClient->pclQuestManager->GetTutorialMakeQuestItem(questunique,siTutorialItemUnique,TutorialQuestMaterial) == true)
			{
				bUseQuestItem = TRUE;
			}

			producenum = returnval = returnpara1 = 0;
			if(pclItemInfo[i]->CanMake(materialnum,  pclmaterialitem, pclmaterialitem, &producenum, &returnval, &returnpara1, bonlymaterialcheckswitch,TRUE,bUseQuestItem,TutorialQuestMaterial) == true)
			{
				// 유효한 생산 범위 안에서. 
				if((*pmakeableitemnum) < MAX_FINDMAKABLEITEM_NUM)
				{
					palliteminfo[(*pmakeableitemnum)*2]	= pclItemInfo[i]->clItem.siUnique;
					palliteminfo[(*pmakeableitemnum)*2+1] = producenum;

					(*pmakeableitemnum)++;
				}
			}//인벤토리에서 검사.
			continue;
		}
	
		// 기술을 근거로 제품을 만들 수 있는지 확인한다. 
		if(pclskill && pclItemInfo[i]->CanMake_Skill(pclskill, pclcharitem, pclBA) == false)continue;

		producenum = returnval = returnpara1 = 0;
		if(pclItemInfo[i]->CanMake(materialnum,  pclmaterialitem, pclmaterialitem, &producenum, &returnval, &returnpara1, bonlymaterialcheckswitch) == true)
		{
			// 유효한 생산 범위 안에서. 
			if((*pmakeableitemnum) < MAX_FINDMAKABLEITEM_NUM)
			{
				palliteminfo[(*pmakeableitemnum)*2]	= pclItemInfo[i]->clItem.siUnique;
				palliteminfo[(*pmakeableitemnum)*2+1] = producenum;
				
				(*pmakeableitemnum)++;
			}
		}
	}	

	// 얻은 정보를 정렬해서 리턴한다. 
	// 아무것도 생산할 수 없으면 리턴. 
	if((*pmakeableitemnum) == 0)return false;
	
	(*ppitemlist) = new SI32 [(*pmakeableitemnum) * 2];
	
	for(i = 0;i< (*pmakeableitemnum);i++)
	{
		(*ppitemlist)[i*2]		= palliteminfo[i*2];
		(*ppitemlist)[i*2+1]	= palliteminfo[i*2+1];
	}
	
	return true;	
	
}
// 물품을 명품으로 만들어준다. 
bool cltItemManagerCommon::MakeRareItem(cltItem* pclitem,SI32 mode)
{
	if(pclitem == NULL)return false;
	if(pclitem->siUnique <= 0)return false;

	SI32 siunique = pclitem->siUnique;

	// 아이템의 ref을 구한다. 
	SI32 ref = FindItemRefFromUnique(siunique);
	if(ref <= 0)
	{
		TCHAR buffer[256];
		StringCchPrintf(buffer, 256, TEXT("MakeRareItem() f827s3:%d"), siunique);
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Error"), buffer, MB_OK|MB_TOPMOST);
		return false;
	}

	// 아이템의 Type을 구한다. 
	SI32 type = pclItemInfo[ref]->siType;

	// 아이템 Type의 Atb를 구한다. 
	SI64 atb = pclItemTypeInfo[type]->siAtb;

	// 아이템이 Object가 아니면 리턴. 
	if((atb & ITEMATB_OBJECT) == 0)
	{
		return false;
	}

	SI16 brareswitch = 0;

	// 품질 확률에 따라 아이템의 능력치를 조정한다. 
	switch(type)
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_AXE:
	case ITEMTYPE_BOW:
	case ITEMTYPE_GUN:
	case ITEMTYPE_CANNON:
	case ITEMTYPE_TORPEDO:
		// 공격력 명품을 만든다. 
		if( ( rand() % 3 && mode == 0 ) || mode == 1 )
		{
			brareswitch	= true;

			pclitem->clItemWeapon.uiMaxDamage = pclItemInfo[ref]->clItem.clItemWeapon.uiMaxDamage;
			pclitem->clItemWeapon.uiMinDamage = pclItemInfo[ref]->clItem.clItemWeapon.uiMinDamage;

			MakeApower(pclitem, 300);
		}
		break;
	case ITEMTYPE_STAFF:
		// 공격력 명품을 만든다. 
		if( ( rand() % 3 && mode == 0 ) || mode == 1 )
		{
			brareswitch	= true;

			pclitem->clItemWeapon.uiMaxDamage = pclItemInfo[ref]->clItem.clItemWeapon.uiMaxDamage;
			pclitem->clItemWeapon.uiMinDamage = pclItemInfo[ref]->clItem.clItemWeapon.uiMinDamage;

			MakeApower(pclitem, 300);
		}
		// 도력 명품을 만든다.
		if ( !brareswitch )
		{
			if( ( rand() % 4 && mode == 0 ) || mode == 2 )
			{
				brareswitch	= true;

				pclitem->clItemWeapon.uiMag = pclItemInfo[ref]->clItem.clItemWeapon.uiMag;

				pclitem->clItemWeapon.uiMag = min(1000, pclitem->clItemWeapon.uiMag * 3 );
			}
		}
		break;

	case ITEMTYPE_HELMET:
	case ITEMTYPE_BELT:
	case ITEMTYPE_ARMOUR:
		// 방어력 명품을 만든다. 
		if( ( rand() % 3 && mode == 0 ) || mode == 1 )
		{
			brareswitch	= true;

			pclitem->clItemArmour.uiAC = pclItemInfo[ref]->clItem.clItemArmour.uiAC;

			MakeDpower(pclitem, 300);
		}
		break;

	case ITEMTYPE_DRESS:
	case ITEMTYPE_HAT:
		// 방어력 명품을 만든다. 
		//if( ( rand() % 3 && mode == 0 ) || mode == 1 )
		//{
		//	brareswitch	= true;

		//	pclitem->clItemStatusArmour.uiAC = pclItemInfo[ref]->clItem.clItemStatusArmour.uiAC;

		//	MakeDpower(pclitem, 300);
		//}
		break;
	case ITEMTYPE_BAG:
		break;

	case ITEMTYPE_MANTLE:
		break;
		
	case ITEMTYPE_SHOES:
		break;
		
	case ITEMTYPE_RING:
		break;

	case ITEMTYPE_NECK:
		break;

	case ITEMTYPE_IRON:
	case ITEMTYPE_DISH:		
	case ITEMTYPE_PAPER:
	case ITEMTYPE_FISHINGROD:
	//case ITEMTYPE_CRYSTAL: //아티팩트
		break;
	}

	// 명품이 아니면 내구도라도 명품을 만든다. 
	SI32 dur = 0;
	if(brareswitch == false)
	{
		dur = 300;
	}
	else
	{
		dur = 100;
	}

	// 내구도를 설정한다.(성능을 강제한다.) 
	//MakeDur(&brareswitch, dur, pclitem, true);

	//cyj 기간으로 변경
	MakeDate(&brareswitch, dur, 0, pclitem, true);

	return true;

}

// 생산된 물품의 가격을 근거로 경험치를 구한다. 
SI32 cltItemManagerCommon::GetMakeExpFromItemPrice(INT64 totalprice, SI32 level)
{
	// NPC시세율에 따라 보정한다. 
	SI32 npcrate = clItemStatistic.siNPCPriceRate;
	if(npcrate == 0)npcrate = 100;

	return min(level * 20, totalprice / 50 * 100 / npcrate) ;
}


// 생산된 물품의 가격을 근거로 경험치를 구한다. 
SI32 cltItemManagerCommon::GetMakeSkillExpFromItemPrice(INT64 totalprice, SI32 skilllevel)
{
	// NPC시세율에 따라 보정한다. 
	SI32 npcrate = clItemStatistic.siNPCPriceRate;
	if(npcrate == 0)npcrate = 100;

	if (skilllevel < 6)	totalprice = totalprice * 1.7;
	else if (skilllevel < 11)	totalprice = totalprice * 1.6;
	else if (skilllevel < 16)	totalprice = totalprice * 1.5;
	else if (skilllevel < 21)	totalprice = totalprice * 1.4;
	else if (skilllevel < 26)	totalprice = totalprice * 1.3;
	else if (skilllevel < 31)	totalprice = totalprice * 1.2;
	else if (skilllevel < 36)	totalprice = totalprice * 1.1;
	else if (skilllevel >= 36)	totalprice = totalprice * 1.0;

	INT64 price = (totalprice / 4000 ) * 100  / npcrate;

	// 신서버 어드벤티지 
	SI32 adv = max(0, 150 - npcrate);
	price = price + price * adv / 100;

	return  min(max(10, skilllevel * 20), price);
}

// 이 물품의 등급을 정한다.
SI16 cltItemManagerCommon::GetTradeServerObjectRank(cltItem*pclitem)
{
	if ( NULL == pclitem )
	{
		return 0;
	}

	SI32 ref = FindItemRefFromUnique(pclitem->siUnique);
	if(ref <= 0)
	{
		char buffer[256];
		sprintf(buffer, "GetTradeServerObjectRank() a35d6:%d", pclitem->siUnique);
		MessageBox(NULL, "Error", buffer, MB_OK|MB_TOPMOST);

		return 0 ;
	}

	cltItemInfo* pinfo = pclItemInfo[ref];
	if ( pinfo == NULL )
		return 0 ;

	// 아이템의 Type을 구한다. 
	SI32 type = pinfo->siType;
	cltItemTypeInfo* ptypeinfo = pclItemTypeInfo[type];
	if ( ptypeinfo == NULL )
		return 0 ;

	if ( ptypeinfo->IsAtb(ITEMATB_TRADESERVER) == FALSE )
		return 0 ;

	SI32 quality = 0 ;

	// 무기냐?
	if ( ptypeinfo->IsAttach(ITEMATTACH_WEAPON) )
	{
		// 무기 맥스 데미지로 결정한다
		quality = ( pclitem->clItemWeapon.uiMaxDamage * 100 ) / pinfo->clItem.clItemWeapon.uiMaxDamage ;
	}
	// 방어구냐?
	else if ( ptypeinfo->IsAttach(ITEMATTACH_ARMOUR) )
	{
		quality = ( pclitem->clItemArmour.uiAC * 100 ) / pinfo->clItem.clItemArmour.uiAC ;
	}
	else return 0 ;

	SI16 index = 0 ;
	for ( SI16 i = 0 ; i < MAX_TRADESERVER_OBJECT_RANK ; i ++ )
	{
		if ( quality <= siTradeServerObejectRank[i] )
		{
			index = i ;
			break; 
		}
		else if ( siTradeServerObejectRank[MAX_TRADESERVER_OBJECT_RANK-1] <= quality )
		{
			index = MAX_TRADESERVER_OBJECT_RANK - 1;
			break;
		}
	}

	return index ;
}

// 무역을 받은 아이템을 만든다.
BOOL cltItemManagerCommon::MakeTradeServerObjectForRank(cltItem*pclitem,SI16 unique,SI16 num ,SI16 rank,SI08 rare,SI16 type,SI16 power)
{
	if ( NULL == pclitem )
	{
		return FALSE;
	}

	SI32 index = 0 ;

	SI32 ref = FindItemRefFromUnique(unique);
	if(ref <= 0)
	{
		char buffer[256];
		sprintf(buffer, "MakeTradeServerObjectForRank() f3d67:%d", unique);
		MessageBox(NULL, "Error", buffer, MB_OK|MB_TOPMOST);

		return FALSE ;
	}

	cltItemInfo* pinfo = pclItemInfo[ref];
	if ( pinfo == NULL )
		return FALSE ;

	// 아이템의 Type을 구한다. 
	SI32 itemtype = pinfo->siType;
	cltItemTypeInfo* ptypeinfo = pclItemTypeInfo[itemtype];
	if ( ptypeinfo == NULL )
		return FALSE ;

	if ( ptypeinfo->IsAtb(ITEMATB_TRADESERVER) == FALSE )
		return FALSE ;

	if ( rank < 0 || rank >= MAX_TRADESERVER_OBJECT_RANK )
		return FALSE ;

	SI32 quality =  siTradeServerObejectRank[rank] ;
	cltItem clitem;
	clitem.Init();
	clitem.siUnique = unique ;
	clitem.siItemNum = num ;
	SI16 brareswitch = false ;

	if( MakeRandItemUnique(clitem.siUnique, &clitem , quality, 0,&brareswitch,quality) )
	{
		clitem.Element.siPower = power ;
		clitem.Element.siType = type ;
		clitem.clItemCommon.clCommonInfo.uiRareInfo = rare ;

		pclitem->Set(&clitem);
	}
	else
	{
		return FALSE ;
	}

	return TRUE ;
}
