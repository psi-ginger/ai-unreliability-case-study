//---------------------------------
// 2004/11/8 김태곤
//---------------------------------

//--------------------------------------
// Common
//--------------------------------------
#include "Char/CharServer/Char-Server.h"
#include "Char/CharManager/CharManager.h"
#include "PersonInfo.h"
#include "Order/Order.h"

//--------------------------------------
// CommonLogic
//---------------------------------------
#include "MagicMgr/MagicMgr.h"
#include "MagicMgr/MagicAttribute/MagicAttribute.h"

//--------------------------------------
// Server
//--------------------------------------
#include "../../Server/Server.h"


extern cltCommonLogic* pclClient;

// 현재 설정된 마법이 적절한지 점검한다. 
void cltCharServer::CheckCurrentMagic( SI32 _siPrevActiveWeapon )
{
	if(GetCurrentMagic())
	{		
		//[영진] 활성화된 무기만 체크하면 된다.
		SI32 siWeaponType	= pclCI->clCharItem.clItem[ActiveWeapon].GetType(pclClient->pclItemManager);
		SI32 siCurWeapon	= pclCI->clCharItem.clItem[ActiveWeapon].siUnique;
			

		CMagicAttribute *pMagicAttributePtr = pclClient->pclMagicManager->GetMagicAttributePtr();
		SI32 magictype = pMagicAttributePtr->GetMagicType(GetCurrentMagic());
		
		// 기술에 해당하는 무기타입을 알아낸다. 
		bool bfailswitch = false;
		switch(magictype)
		{
		case MAGIC_TYPE_SWORD:
			if(siWeaponType != ITEMTYPE_SWORD )bfailswitch = true;
			break;
			
		case MAGIC_TYPE_SPEAR:
			if(siWeaponType != ITEMTYPE_SPEAR )bfailswitch = true;
			break;
			
		case MAGIC_TYPE_AXE:
			if(siWeaponType != ITEMTYPE_AXE)bfailswitch = true;
			break;
			
		case MAGIC_TYPE_STAFF:	
			if(siWeaponType != ITEMTYPE_STAFF )bfailswitch = true;
			break;
			
		case MAGIC_TYPE_BOW:
			if(siWeaponType != ITEMTYPE_BOW )bfailswitch = true;
			break;
			
		case MAGIC_TYPE_GUN:	
			if(siWeaponType != ITEMTYPE_GUN )bfailswitch = true;
			break;

		case MAGIC_TYPE_MON:		bfailswitch = true;	break;
		case MAGIC_TYPE_SOULGUARD:	bfailswitch = true; break;
		case MAGIC_TYPE_SKILL_BOOK:	
			{
				switch( siWeaponType )
				{
					case ITEMTYPE_SWORD:
					case ITEMTYPE_SPEAR:
					case ITEMTYPE_AXE:
					case ITEMTYPE_STAFF:
					case ITEMTYPE_BOW:
					case ITEMTYPE_GUN:	
						{
							_siPrevActiveWeapon == siCurWeapon ? bfailswitch = false 
															   : bfailswitch = true; 
							break;
						}
						
	
					default:			
						bfailswitch = true; break;
				}
			}
			break;
		default:	bfailswitch = true;	break;
		}		
		
		// 현재의 무기가 적절하지 않다면 설정된 Magic을 초기화 한다.
		if(bfailswitch)
		{
			SetCurrentMagic(0, 0);
			m_clMagicBufMgr.Init();
		}		
	}
	// Client에게 알려준다.
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMAGIC, GetCurrentMagic());
	SendNetMsg((sPacketHeader*)&clMsg);
}


// 전투중 마법 구현에 대한 행위. 
bool cltCharServer::MagicInAttackAction()
{
	if(IsNPC(GetCharUnique()) == false && IsInstanceNPC(GetCharUnique()) == false )return false;
	if(IsNPC(GetCharUnique()) == false && IsInstanceNPC(GetCharUnique()) == false )return false;

	// 소환수인경우에는 자주 사용하지 않는다. 
	if(GetSummonParentID())
	{
		if(rand() % 5)return false;
	}
	else
	{
		if(rand() % 2)return false;
	}

	SI32 magickind = pclKI->siMagic;
	switch(magickind)
	{
	case 0:
		{
			// 소환수인경우에 수동마법은 사용하지 않는다.
			SI32 parentid = GetSummonParentID() ;
			if ( pclCM->IsValidID(parentid) )
				return false ;
		}
		break;
		// 치료하기 기능이 있다면, 
		// 주변에서 치료할 캐릭터를 찾는다. 
	case MAGIC_MON_HEAL:
		{
			if(HealAI() == true)
			{
				return true;
			}
		}
		break;
	default:
		SI32 returnval = 0;

		// 능동마법인 경우에만 사용한다.
		if ( MAGIC_CASTTYPE_ACTIVE == GetCurrentMagicCastType() )
		{
			if(CanMagic(clAttackTarget.GetAttackID(), magickind, &returnval) == true)
			{
				bBusySwitch = false;
				
				pclClient->pclOrderManager->OrderMagic(GetID(), clAttackTarget.GetAttackID(), magickind, BY_COMPUTER);
				
				return true;		
			}
		}
	}

	return false;
}

void cltCharServer::SetMagicDelay(SI32 delay)
{
	siMagicDelay = delay;
}

SI32 cltCharServer::GetMagicDelay()
{
	return siMagicDelay ;
}

