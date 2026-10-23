//---------------------------------
// 2004/3/8 김태곤
//---------------------------------
#include <Stdio.h>
#include "cltItem.h"
#include "CommonLogic.h"

#include "Skill\Skill-Manager.h"
#include "Char\Disease\Disease.h"
#include "Util\Util.h"
#include "Char\CharCommon\Char-Common.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Item.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

//-----------------------------------------
// 아이템 장착, 사용 조건. 
//-----------------------------------------

// 조건에 부합하는지 확인한다. 
bool cltItemConition::Check(SI32 itemtype, SI32 kind, SI32 level, SI32 str, SI32 dex, SI32 mag, SI32 vit, SI32 hnd, SI32 skillpoint, SI32 famelevel, SI32* returnval)
{
	// 사용 및 장착에 기술 조건이 필요하다면, 
	if(siSkillKind)
	{
		if ( skillpoint < siSkillPoint )
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFSKILL;
			return false;
		}
	}
	
	if(siLevel)
	{
		if(level < siLevel)
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFLEVEL;
			return false;
		}
	}
	if(siStr)
	{
		if(str < siStr)
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFSTR;
			return false;
		}
	}
	if(siDex)
	{
		if(dex < siDex)
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFDEX;
			return false;
		}
	}
	if(siMag)
	{
		if(mag < siMag)
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFMAG;
			return false;
		}
	}
	if(siVit)
	{
		if(vit < siVit )
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFVIT;
			return false;
		}
	}
	if(siHnd)
	{
		if(hnd < siHnd )
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFHND;
			return false;
		}
	}
	
	if(kind == pclClient->GetUniqueFromHash( TEXT("KIND_HERO1") ) || kind == KIND_HERO2 || kind == KIND_HERO3 || kind == KIND_HERO4 )
	{
		if(bForMan == false)
		{
			*returnval = ITEMEQUIP_FAIL_FORWOMAN;
			return false;
		}
	}


	if(kind == KIND_HEROINE1 || kind == KIND_HEROINE2 || kind == KIND_HEROINE3 || kind == KIND_HEROINE4)
	{
		if(bForWoman == false)
		{
			*returnval = ITEMEQUIP_FAIL_FORMAN;
			return false;
		}
	}
	

	// 이순신이나 장보고라면, 활만 장착 가능. 
	//if( kind == pclClient->GetUniqueFromHash(TEXT("KIND_YISOONSIN")) ||
	//	kind == pclClient->GetUniqueFromHash(TEXT("KIND_JANGBOGO")) )
	//{
	//	if(itemtype == ITEMTYPE_IRON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}

	//	if(itemtype == ITEMTYPE_FISHINGROD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}


	//	if(pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE
	//	&& itemtype != ITEMTYPE_BOW)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}
	//}

	// 권율이면 칼만 장착 가능. 
	//if(kind == pclClient->GetUniqueFromHash(TEXT("KIND_GWONYOOL")))
	//{
	//	if(itemtype == ITEMTYPE_IRON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}

	//	if(itemtype == ITEMTYPE_FISHINGROD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}


	//	if(pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE
	//	&& itemtype != ITEMTYPE_SWORD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}
	//}

	// 최무선이면 대포만 장착 가능. 
	//if(kind == pclClient->GetUniqueFromHash(TEXT("KIND_CHOIMOOSUN")) )
	//{
	//	if(itemtype == ITEMTYPE_IRON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}

	//	if(itemtype == ITEMTYPE_FISHINGROD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFFORHERO;
	//		return false;
	//	}


	//	if(pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE
	//		&& itemtype != ITEMTYPE_CANNON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}
	//}

	// 사명대사면 지팡이만 장착 가능
	//if(kind == pclClient->GetUniqueFromHash( TEXT("KIND_SAMYOUNGSAINT")) )
	//{
	//	if(itemtype == ITEMTYPE_IRON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}

	//	if(itemtype == ITEMTYPE_FISHINGROD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}


	//	if(pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE
	//		&& itemtype != ITEMTYPE_STAFF)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}
	//}

	
	// 다모라면, 총만 장착 가능. 
	//if( kind == pclClient->GetUniqueFromHash(TEXT("KIND_DAMO")) )
	//{
	//	if(itemtype == ITEMTYPE_IRON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}

	//	if(itemtype == ITEMTYPE_FISHINGROD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}


	//	if(pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE
	//		&& itemtype != ITEMTYPE_GUN )
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}
	//}

	// 김시민이나 관우라면, 창만 장착 가능. 
	//if( kind == pclClient->GetUniqueFromHash(TEXT("KIND_KIMSIMIN")) || kind == pclClient->GetUniqueFromHash(TEXT("KIND_GWANWOO")) )
	//{
	//	if(itemtype == ITEMTYPE_IRON)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}

	//	if(itemtype == ITEMTYPE_FISHINGROD)
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}


	//	if(pclClient->pclItemManager->pclItemTypeInfo[itemtype]->IsAtb(ITEMATB_WEAPON) == TRUE
	//		&& itemtype != ITEMTYPE_SPEAR )
	//	{
	//		*returnval = ITEMEQUIP_FAIL_NOEQUIPFORHERO;
	//		return false;
	//	}
	//}

	// 명성등급 확인. 
	if(siFameLevel)
	{
		if(famelevel < siFameLevel)
		{
			*returnval = ITEMEQUIP_FAIL_OUTOFFAME;
			return false;
		}
	}
	
	return true;
	
}

// 조건의 문자열을 얻어온다. 
bool cltItemConition::GetText( TCHAR* text, SI16 txtSize, TCHAR *SkillName, SI32 SkillKind )
{
	TCHAR	temp[ 256 ] = TEXT("");
	bool rtnval	= false;
	
	if(siLevel)
	{
		TCHAR* pText = GetTxtFromMgr(3514);
		StringCchPrintf(temp, 256, pText, siLevel);
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siStr)
	{
		TCHAR* pText = GetTxtFromMgr(3515);
		StringCchPrintf(temp, 256, pText, siStr);
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siDex)
	{
		TCHAR* pText = GetTxtFromMgr(3516);
		StringCchPrintf(temp, 256, pText, siDex);
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siMag)
	{
		TCHAR* pText = GetTxtFromMgr(3517);
		StringCchPrintf(temp, 256, pText, siMag);
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siVit)
	{
		TCHAR* pText = GetTxtFromMgr(3518);
		StringCchPrintf(temp, 256, pText, siVit);
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siHnd)
	{
		TCHAR* pText = NULL;
		if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
		{
			pText = GetTxtFromMgr(9862);
		}
		else
		{
			pText = GetTxtFromMgr(3519);
		}

		StringCchPrintf(temp, 256, pText, siHnd);
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	
	// 스킬이 존재하고, 같은 스킬이며, 스킬명이 존재한다면... 
	if( siSkillKind && siSkillKind == SkillKind && siSkillPoint > 0 && SkillName != NULL )
	{
		TCHAR* pText = GetTxtFromMgr(3520);
		StringCchPrintf(temp, 256, pText, SkillName, siSkillPoint );			
		StringCchCat(text, 256, temp);
		rtnval = true;
	}

	// 명성등급 제한. 
	if(siFameLevel)
	{
		TCHAR* pText = GetTxtFromMgr(3521);
		StringCchPrintf(temp, 256, pText, siFameLevel);			
		StringCchCat(text, 256, temp);
		rtnval = true;

	}
	
	// 성별 제한이 있따면, 
	if(bForMan == true && bForWoman == false)
	{
		TCHAR* pText = GetTxtFromMgr(3522);
		StringCchPrintf(temp, 256, pText);			
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(bForMan == false && bForWoman == true)
	{
		TCHAR* pText = GetTxtFromMgr(3523);
		StringCchPrintf(temp, 256, pText);			
		StringCchCat(text, 256, temp);
		rtnval = true;
	}

	// [지연] 아바타 목걸이 : 베이스 스킬 타입이 있다면 해당 텍스트 읽어옴
	if ( 0 < siBaseSkillType && 0 == SkillKind)
	{
		NTCHARString256	kBuf;

		switch ( siBaseSkillType )
		{
		case SKILLTYPE_R_USEWEAPON:
			kBuf = (GetTxtFromMgr(30296)); // 무기술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_MAKEWEAPON:
			kBuf = (GetTxtFromMgr(30297)); // 무기제작술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_ARMOR:
			kBuf = (GetTxtFromMgr(30298)); // 방어구제작술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_MAKEHARVEST:
			kBuf = (GetTxtFromMgr(30299)); // 생산제작술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_HARVEST:
			kBuf = (GetTxtFromMgr(30300)); // 생산기술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_MAKECLOTH:
			kBuf = (GetTxtFromMgr(30301)); //재단술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_MAKEACCESSARY:
			kBuf = (GetTxtFromMgr(30302)); // 액세서리 제작술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		case SKILLTYPE_R_ETC:
			kBuf = (GetTxtFromMgr(30303)); // 기타기술 중 랜덤하게\r 한가지 스킬의 능력치 #SkillPoint#상승
			break;
		}

		kBuf.Replace("#SkillPoint#", SI32ToString(siSkillPoint));	
		StringCchCat(text, 256, kBuf);
		rtnval = true;
	}
	return rtnval;
}

bool cltItemConition::GetCheckedText(  TCHAR* text, SI16 txtSize, TCHAR *SkillName, SI32 SkillKind, cltCharCommon * pclchar  )
{
	if( pclchar == NULL) return FALSE;

	TCHAR	temp[ 256 ] = TEXT("");
	TCHAR	temp2[ 256 ] = TEXT("");

	bool rtnval	= false;
	

	if(siLevel)
	{
		TCHAR* pText = GetTxtFromMgr(3514);
		
		if(  siLevel >  pclchar->pclCI->clIP.GetLevel() )
		{
			StringCchPrintf(temp2, 256, pText, siLevel);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siLevel);
		}
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siStr)
	{	
		TCHAR* pText = GetTxtFromMgr(3515);

		if( siStr > pclchar->clPB.clTotalBA.GetStr() )
		{
			StringCchPrintf(temp2, 256, pText, siStr);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siStr);
		}		
		
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siDex)
	{
		TCHAR* pText = GetTxtFromMgr(3516);
		
		if( siDex > pclchar->clPB.clTotalBA.GetDex() )
		{
			StringCchPrintf(temp2, 256, pText, siDex);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siDex);
		}	

		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siMag)
	{
		TCHAR* pText = GetTxtFromMgr(3517);
		
		if( siMag > pclchar->clPB.clTotalBA.GetMag() )
		{
			StringCchPrintf(temp2, 256, pText, siMag);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siMag);
		}	
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siVit)
	{
		TCHAR* pText = GetTxtFromMgr(3518);

		if( siVit > pclchar->clPB.clTotalBA.GetVit() )
		{
			StringCchPrintf(temp2, 256, pText, siVit);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siVit);
		}
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(siHnd)
	{
		TCHAR*	pText		= NULL;
		SI32	siHndOrLuk	= 0;

		// [영훈] 손재주대신에 행운이 쓰인다
		if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
		{
			pText		= GetTxtFromMgr(9862);
			siHndOrLuk	= pclchar->clPB.clTotalBA.GetLuk();
		}
		else
		{
			pText		= GetTxtFromMgr(3519);
			siHndOrLuk	= pclchar->clPB.clTotalBA.GetHnd();
		}

		if( siHnd > siHndOrLuk )
		{
			StringCchPrintf(temp2, 256, pText, siHnd);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siHnd);
		}

		StringCchCat(text, 256, temp);
		rtnval = true;
	}

	// 스킬이 존재하고, 같은 스킬이며, 스킬명이 존재한다면... 
	if( siSkillKind && siSkillKind == SkillKind && siSkillPoint > 0 && SkillName != NULL )
	{
		TCHAR* pText = GetTxtFromMgr(3520);
				
		SI32 totalskilllevel = pclchar->pclCI->clCharSkill.GetTotalSkillLevel(siSkillKind, 0, 0, &pclchar->pclCI->clCharItem.clItem[0], pclchar->GetID());
		if( siSkillPoint > totalskilllevel )
		{
			StringCchPrintf(temp2, 256, pText, SkillName, siSkillPoint );
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, SkillName, siSkillPoint );	
		}

		StringCchCat(text, 256, temp);
		rtnval = true;

	}

	// 명성등급 제한. 
	if(siFameLevel)
	{
		TCHAR* pText = GetTxtFromMgr(3521);

		if( siFameLevel > pclchar->pclCI->clIP.siFameLevel )
		{
			StringCchPrintf(temp2, 256, pText, siFameLevel);
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText, siFameLevel);			
		}

		StringCchCat(text, 256, temp);
		rtnval = true;

	}

	// 성별 제한이 있따면, 
	if(bForMan == true && bForWoman == false)
	{
		TCHAR* pText = GetTxtFromMgr(3522); 
		
		SI32 kind = pclchar->GetKind();
		if(kind == KIND_HEROINE1 || kind == KIND_HEROINE2 || kind == KIND_HEROINE3 || kind == KIND_HEROINE4)
		{
			StringCchPrintf(temp2, 256, pText);	
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText);	
		}		
		StringCchCat(text, 256, temp);
		rtnval = true;
	}
	if(bForMan == false && bForWoman == true)
	{
		TCHAR* pText = GetTxtFromMgr(3523);

		SI32 kind = pclchar->GetKind();
		if(kind == pclClient->GetUniqueFromHash( TEXT("KIND_HERO1") ) || kind == KIND_HERO2 || kind == KIND_HERO3 || kind == KIND_HERO4)
		{
			StringCchPrintf(temp2, 256, pText);	
			StringCchPrintf(temp, 256, TEXT("%s%s%s"),TEXT("<C08>"),temp2,TEXT("</C>"));
		}
		else
		{
			StringCchPrintf(temp, 256, pText);
		}			
		StringCchCat(text, 256, temp);
		rtnval = true;
	}

	return rtnval;


}


