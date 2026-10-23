#ifndef _MSGTYPEITEM_H
#define _MSGTYPEITEM_H

#define ChangeDexStatus	1
#define ChangeHndStatus	2
#define ChangeLukStatus	3
#define ChangeMagStatus	4
#define ChangeStrStatus	5
#define ChangeVitStatus	6

#include "..\Common\Item\ItemCommon\cltItem.h"

#include "../Client/MiniGame/Roulette/RouletteLogic.h"
#include "../Common/Util/Util.h"


//---------------------------------------------------------------
// NPC수리 요청. 
//--------------------------------------------------------------
class cltGameMsgRequest_NPCRepair
{
public:

	SI32	siCharUnique;
	SI16	siPos;			// 인벤내 아이템 위치. 
	cltItem clItem;			// 수리 대상 아이템 

	cltGameMsgRequest_NPCRepair( SI32 CharUnique, SI16 pos, cltItem* pclitem )
	{
		siCharUnique = CharUnique;
		siPos = pos;
		clItem.Set(pclitem);
	}
};

//---------------------------------------------------------------
// NPC수리 가격 요청. 
//--------------------------------------------------------------
class cltGameMsgRequest_NPCRepairPrice
{
public:	
	cltItem clItem;

	cltGameMsgRequest_NPCRepairPrice(cltItem* pclitem)
	{		
		clItem.Set(pclitem);
	}
};

class cltGameMsgResponse_NPCRepairPrice{
public:
	SI16	siItemUnique;
	GMONEY	siRepairePrice;		// 수리비. 

	cltGameMsgResponse_NPCRepairPrice(SI16 itemunique, GMONEY repairprice)
	{
		siItemUnique	= itemunique;
		siRepairePrice	= repairprice;
	}
};


//---------------------------------------------------------------
// 소지품 내의 아이템을 드랍하도록 한다. 
//--------------------------------------------------------------
class cltGameMsgRequest_InvDrop{
public:
	SI32	siItemPos;
	cltItem clItem;

	cltGameMsgRequest_InvDrop(SI32 itempos, cltItem* pclitem)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
	}
};


//*****************************************************************************************
//[ 추가 : 황진성 2008. 2. 28 => 아티펙트 아이템 조합하기 패킷 구조체. ]
class cltGameMsgRequest_Artifact_Mixture
{
public:
	SI16	m_siEquipItemPos;									// 조합되는 아이템의 위치.(장비류가 되겠지..)
	cltItem m_clEquipItem;										// 조합되는 사용되는 아이템.

	SI16	m_siCrystalItemPos[MAX_ITEM_ARTIFACT_MIXTURE];		// 조합에 사용되는 아이템의 위치.
	cltItem m_clCrystalItem[MAX_ITEM_ARTIFACT_MIXTURE];			// 조합에 사용되는 크리스탈 아이템.
	
	cltGameMsgRequest_Artifact_Mixture( void )
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Artifact_Mixture ) );
	}
};

//[ 추가 : 황진성 2008. 2. 28 => 아티펙트 조합된 아이템을 받을 패킷 구조체. ]
class cltGameMsgResponse_Artifact_Mixture
{
public:
	SI16		m_siEquipItemPos;									// 조합된을 아이템을 넣을 위치.
	cltItem		m_clEquipItem;										// 조합된 결과 아이템.

	SI16		m_siCrystalItemPos[MAX_ITEM_ARTIFACT_MIXTURE];		// 조합에 사용된 결정 아이템의 위치 받아서 인벤에서 제거.
	cltItem		m_clCrystalItem[MAX_ITEM_ARTIFACT_MIXTURE];			// 조합에 사용되는 크리스탈 아이템.
	
	cltGameMsgResponse_Artifact_Mixture( void )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Artifact_Mixture ) );
	}

	cltGameMsgResponse_Artifact_Mixture( SI16		_siEquipItemPos,   cltItem*	_clEquipItem,		
										 SI16*		_siCrystalItemPos, cltItem*	_clCrystalItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Artifact_Mixture ) );

		m_siEquipItemPos = _siEquipItemPos;	
		memcpy(	&m_clEquipItem,		   _clEquipItem,		sizeof( m_clEquipItem )	);
		memcpy(	m_siCrystalItemPos,	   _siCrystalItemPos,	sizeof( m_siCrystalItemPos ) );
		memcpy(	m_clCrystalItem,	   _clCrystalItem,		sizeof( m_clCrystalItem ) );
	}
};

//[ 추가 : 황진성 2008. 2. 28 => 아티펙트 아이템 결정체 제거하기 패킷 구조체. ]
class cltGameMsgRequest_Artifact_Remove
{
public:
	SI16		m_siEquipItemPos;									// 결정체가 제거되는 아이템의 위치.(장비류가 되겠지..)
	cltItem		m_clEquipItem;										// 결정체가 제거되는 아이템.

	SI16		m_siCrystalItemPosList[ MAX_CRYSTAL_NUM ];				// 결정체가 제거에 사용될 아이템의 위치.(장비류가 되겠지..)
	cltItem		m_clCrystalItemList[ MAX_CRYSTAL_NUM ];					// 결정체가 제거에 사용될 아이템.
	
	bool		m_bRemoveSlotIndexList[ MAX_CRYSTAL_NUM ];				// 제거되는 슬롯 true, false.
	
	cltGameMsgRequest_Artifact_Remove( SI16  _siEquipItemPos,		cltItem* _clEquipItem, 
									   SI16* _siCrystalItemPosList, cltItem* _clCrystalItemList,
									   bool* _bRemoveSlotIndexList )
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Artifact_Remove ) );

		m_siEquipItemPos	= _siEquipItemPos;
		memcpy(&m_clEquipItem,  _clEquipItem, sizeof( m_clEquipItem ) );
				
		memcpy(&m_siCrystalItemPosList,  _siCrystalItemPosList, sizeof( m_siCrystalItemPosList ) );
		memcpy(&m_clCrystalItemList,	 _clCrystalItemList,	sizeof( m_clCrystalItemList ) );
		memcpy(m_bRemoveSlotIndexList,	 _bRemoveSlotIndexList, sizeof( m_bRemoveSlotIndexList) );
	}
};

//[ 추가 : 황진성 2008. 2. 28 => 아티펙트 결정체 제거된 아이템을 받을 패킷 구조체. ]
class cltGameMsgResponse_Artifact_Remove
{
public:
	SI16		m_siEquipItemPos;		// 결정체가 제거된 아이템을 넣을 위치.
	cltItem		m_clEquipItem;			// 결정체가 제거된 결과 아이템.

	SI16		m_siCrystalItemPosList[ MAX_CRYSTAL_NUM ];		// 결정체가 제거에 사용된 아이템의 위치.
	cltItem		m_clCrystalItemList[ MAX_CRYSTAL_NUM ];			// 결정체가 제거에 사용된 아이템.

	SI16		m_siCreateCrystalItemPos[ MAX_CRYSTAL_NUM ];	// 결정체가 제거되 생성된 아이템의 위치.()
	cltItem		m_clCreateCrystalItem[ MAX_CRYSTAL_NUM ];		// 결정체가 제거되 생성된 아이템.

	cltGameMsgResponse_Artifact_Remove( void )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Artifact_Remove ) );
	}

	cltGameMsgResponse_Artifact_Remove( SI16  _siEquipItemPos,			cltItem* _clEquipItem, 
										SI16* _siCrystalItemPosList,	cltItem* _clCrystalItemList,
										SI16* _siCreateCrystalItemPos,  cltItem* _clCreateCrystalItem	)
										
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Artifact_Remove ) );

		m_siEquipItemPos	= _siEquipItemPos;
		memcpy(&m_clEquipItem,  _clEquipItem, sizeof( m_clEquipItem ) );

		memcpy( m_siCrystalItemPosList,  _siCrystalItemPosList, sizeof( m_siCrystalItemPosList ) );
		memcpy( m_clCrystalItemList,	 _clCrystalItemList,	sizeof( m_clCrystalItemList ) );

		memcpy( m_siCreateCrystalItemPos, _siCreateCrystalItemPos, sizeof( m_siCreateCrystalItemPos ) );	
		memcpy( m_clCreateCrystalItem,	  _clCreateCrystalItem,	   sizeof( m_clCreateCrystalItem ) );	
	}
};



//[ 추가 : 황진성 2008. 2. 28 => 아티펙트 슬롯 생성 받을 패킷 구조체. ]
class cltGameMsgRequest_Artifact_Slot_Create
{
public:
	SI16		m_siEquipItemPos;					// 슬롯을 생성 할 아이템의 위치.
	cltItem		m_clEquipItem;						// 슬롯을 생성 할 아이템. 

	SI16		m_siCrystalItemPos;					// 슬롯을 생성할 때 사용하는 아이템의 위치.
	cltItem		m_clCrystalItem;					// 슬롯을 생성할 때 사용하는 아이템. 
	
	cltGameMsgRequest_Artifact_Slot_Create(SI16 _siEquipItemPos, cltItem* _clEquipItem, SI16 _siCrystalItemPos, cltItem* _clCrystalItem)
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Artifact_Slot_Create ) );

		m_siEquipItemPos		= _siEquipItemPos;	
		memcpy(&m_clEquipItem,  _clEquipItem, sizeof( m_clEquipItem ) );		
	
		m_siCrystalItemPos		= _siCrystalItemPos;
		memcpy(&m_clCrystalItem,  _clCrystalItem, sizeof( m_clCrystalItem ) );
	}
};

//[ 추가 : 황진성 2008. 2. 28 => 아티펙트 슬롯 생성 받을 패킷 구조체. ]
class cltGameMsgResponse_Artifact_Slot_Create
{
public:
	SI16		m_siEquipItemPos;				// 슬롯을 생성 할 아이템의 위치.
	cltItem		m_clEquipItem;					// 슬롯을 생성 할 아이템. 

	SI16		m_siCrystalItemPos;				// 변경된 슬롯을 생성할 때 사용한 아이템의 위치.
	cltItem		m_clCrystalItem;				// 변경된 슬롯을 생성할 때 사용한 아이템. 
	
	cltGameMsgResponse_Artifact_Slot_Create( void )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Artifact_Slot_Create ) );
	}

	cltGameMsgResponse_Artifact_Slot_Create(SI16 _siEquipItemPos, cltItem* _clEquipItem, SI16 _siCrystalItemPos, cltItem* _clCrystalItem)
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Artifact_Slot_Create ) );

		m_siEquipItemPos		= _siEquipItemPos;	
		memcpy(&m_clEquipItem,  _clEquipItem, sizeof( m_clEquipItem ) );		

		m_siCrystalItemPos		= _siCrystalItemPos;
		memcpy(&m_clCrystalItem,  _clCrystalItem, sizeof( m_clCrystalItem ) );
	}
};



//*****************************************************************************************

// [추가 : 황진성 2008. 2. 28 => 아이템 분해를 하는 NPC의 타입, enum{} ]
class ItemDisAssemble_NPC_KIND
{
public:
	enum{ NPC_TINTIN, NPC_ATIFACT };
};

//---------------------------------------------------------------
// 소지품 내의 아이템을 분해한다.
//--------------------------------------------------------------
class cltGameMsgRequest_ItemDisAssemble
{
public:
	SI32	siItemPos;
	cltItem clItem;

	SI16	m_siNPCKind;		//[추가 : 황진성 2008. 2. 28 => 메시지를 던진 NPC 종류. 위에 enum 정보를 채운다.]
	
	cltGameMsgRequest_ItemDisAssemble(SI32 itempos, cltItem* pclitem, SI16 _siNPCKind = ItemDisAssemble_NPC_KIND::NPC_TINTIN)
	{
		siItemPos = itempos;
		clItem.Set(pclitem);
		
		m_siNPCKind = _siNPCKind;
	}
};

class cltGameMsgResponse_ItemDisAssemble{
public:
	SI32	siResult;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEM_PER_WENNYOLD>		clItem;
#else
	cltItem clItem[MAX_ITEM_PER_WENNYOLD];
#endif
	SI16	siWenneyPercent;
	SI16	siResolutionRate;

	SI16	siDurabilityPenalty;	// 내구도에 의한 패널티

	SI16	m_siNPCKind;			// [추가 : 황진성 2008. 2. 28 => 메시지를 던진 NPC 종류. 위에 enum 정보를 채운다.]
	
	cltGameMsgResponse_ItemDisAssemble(SI32 result, cltItem* pclitem, SI16 wennypercent , SI16 resolutionrate, SI16 durabilityPenalty, SI16 _siNPCKind = ItemDisAssemble_NPC_KIND::NPC_TINTIN)
	{
		siResult			= result;
#ifdef _SAFE_MEMORY
		clItem.MemCpy(pclitem);
#else
		memcpy(clItem, pclitem, sizeof(clItem));
#endif
		siWenneyPercent		= wennypercent;
		siResolutionRate	= resolutionrate;

		siDurabilityPenalty	= durabilityPenalty;

		m_siNPCKind			= _siNPCKind;
	}
};

class cltGameMsgRequest_ItemDisAssemble_List
{
public:
	
	SI16	m_siNPCKind;		// [추가 : 황진성 2008. 2. 28 => 메시지를 던진 NPC 종류. 위에 enum 정보를 채운다.]

	cltGameMsgRequest_ItemDisAssemble_List( SI16 _siNPCKind = ItemDisAssemble_NPC_KIND::NPC_TINTIN )
	{
		m_siNPCKind = _siNPCKind;
	}
};

class cltGameMsgResponse_ItemDisAssemble_List{
public:
	SI32	siResult;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_ITEM_PER_WENNYOLD>		clItem;
#else
	cltItem clItem[MAX_ITEM_PER_WENNYOLD];
#endif

	SI16	m_siNPCKind;		// [추가 : 황진성 2008. 2. 28 => 메시지를 던진 NPC 종류. 위에 enum 정보를 채운다.]

	cltGameMsgResponse_ItemDisAssemble_List(SI32 result, cltItem* pclitem, SI16 _siNPCKind = ItemDisAssemble_NPC_KIND::NPC_TINTIN)
	{
		siResult = result;
#ifdef _SAFE_MEMORY
		clItem.MemCpy(pclitem);
#else
		memcpy(clItem, pclitem, sizeof(clItem));
#endif

		m_siNPCKind = _siNPCKind;
	}
};

class cltGameMsgRequest_ItemDisAssemble_Recieve{
public:
	SI16				siStgPos;			// 혹부리(2호식) 인벤토리내 위치. 

	// what
	cltItem				clItem;

	SI16	m_siNPCKind;		// [추가 : 황진성 2008. 2. 28 => 메시지를 던진 NPC 종류. 위에 enum 정보를 채운다.]

	cltGameMsgRequest_ItemDisAssemble_Recieve(SI16 stgpos, cltItem *pclitem, SI16 _siNPCKind = ItemDisAssemble_NPC_KIND::NPC_TINTIN )
	{
		siStgPos = stgpos;
		clItem.Set(pclitem);

		m_siNPCKind = _siNPCKind;
	}
};

class cltGameMsgResponse_ItemDisAssemble_Recieve{
public:
	SI32				siResult;			// 혹부리(2호식) 인벤토리내 위치. 

	cltGameMsgResponse_ItemDisAssemble_Recieve(SI32 result)
	{
		siResult = result;
	}
};

//---------------------------------------------------------------
// 소지품 내의 부적을 장비에 부착한다. 
//--------------------------------------------------------------
class cltGameMsgRequest_Enchant{
public:
	SI16	siFromPos;
	SI16	siFromItemUnique;

	SI16	siToPos;
	cltItem clToItem;

	cltGameMsgRequest_Enchant(SI16 frompos, SI16 sifromunique, SI16 topos, cltItem* pclitem)
	{
		siFromPos			= frompos;
		siFromItemUnique	= sifromunique;

		siToPos				= topos;

		clToItem.Set(pclitem);
	}
};


//---------------------------------------------------------------
// 땅에 떨어진 아이템을 줍도록 한다. 
//--------------------------------------------------------------
class cltGameMsgRequest_InvPickUp{
public:
	SI16	siItemUnique;
	SI16	siItemIndex;	

	SI16	siMapIndex;
	SI16	siX, siY;

	cltGameMsgRequest_InvPickUp(SI16 itemunique, SI16 itemindex, SI16 mapindex, SI16 x, SI16 y)
	{
		siItemUnique	= itemunique;
		siItemIndex		= itemindex;

		siMapIndex		= mapindex;
		siX				= x;
		siY				= y;
	};
};


//-----------------------------------
// 땅에 떨어진 아이템의 정보를 클라이언트에 보낸다. 
//-----------------------------------
class cltGameMsgResponse_ItemDrop{
public:
	cltCharPos clPos;				// 아이템이 떨어진 위치. 
	UI16		uiItemUnique;		// 아이템의 유니크. 
	UI16		uiItemListIndex;	// 서버상의 아이템 리스트 인덱스. 

	cltGameMsgResponse_ItemDrop(cltCharPos* pclpos, UI16 itemunique, UI16 itemlistindex)
	{
		clPos.Set(pclpos);
		uiItemUnique	= itemunique;
		uiItemListIndex	= itemlistindex;
	}
};

//-------------------------------------------
// 인베토리에서 아이템을 만든다. 
//-------------------------------------------
class cltGameMsgRequest_MakeItemInv{
public:
	SI16 siItemUnique;
	SI16 siItemNum;

	SI16 siMaterialCharUnique;		// 원료 제공자(제조 의뢰자 )
	SI16 siMakeCharUnique;			// 제작자 
	SI32 siShopType;				// 상점 타입

	SI08 siType;

	bool bNPCMake;					// NPC를 통해 제작하는가!
	bool bRequestManufacture	;	// 제조 의뢰인지 아닌지 

	cltGameMsgRequest_MakeItemInv()
	{
		Init();
	}

	cltGameMsgRequest_MakeItemInv(SI16 itemunique, SI16 itemnum, SI16 materialcharunique,SI16 MakeCharUnique, SI32 ShopType, SI08 type, bool npcmake = false ,bool RequestManufacture = false   )
	{
		siItemUnique			= itemunique;
		siItemNum				= itemnum;
		siMaterialCharUnique	= materialcharunique;
		siType					= type;
		bNPCMake				= npcmake;
		bRequestManufacture		= RequestManufacture	;
		siMakeCharUnique		=	MakeCharUnique	;
		siShopType				=	ShopType	;
	}

	void Init()
	{
		siItemUnique			= 0;
		siItemNum				= 0;
		siMaterialCharUnique	= 0;
		siType					= 0;
	}

	void Set(cltGameMsgRequest_MakeItemInv* pclinfo)
	{
		siItemUnique			= pclinfo->siItemUnique;
		siItemNum				= pclinfo->siItemNum;
		siMaterialCharUnique	= pclinfo->siMaterialCharUnique;
		siType					= pclinfo->siType;
		bNPCMake				= pclinfo->bNPCMake;
		bRequestManufacture		= pclinfo->bRequestManufacture	;
		siMakeCharUnique		= pclinfo->siMakeCharUnique	;
		siShopType				= pclinfo->siShopType	;
	}
};
//-------------------------------------------
// 물품 생산에 필요한 지연 시간을 통보한다.
//-------------------------------------------
class cltGameMsgResponse_MakeItemDelay{
public:
	DWORD dwDelayClock;
	cltGameMsgResponse_MakeItemDelay(DWORD delay)
	{
		dwDelayClock = delay;
	}
};

//------------------------------------------
// 기간 연장 아이템을 사용한다.
//------------------------------------------
class cltGameMsgRequest_UseDayPlus{
public:
	SI32 siItemUnique;	
	
	SI32 siOrgInx;
	SI32 siEleInx;

	cltGameMsgRequest_UseDayPlus(SI32 itemunique, SI32 orginx , SI32 eleinx)
	{
		siItemUnique = itemunique;
		siOrgInx = orginx;
		siEleInx = eleinx;
	}
};

//-------------------------------------------
// Active Weapon의 상태를 통보
//-------------------------------------------
class cltGameMsgResponse_ExchageWeapon{
public:
	SI16	siActiveWeapon;

	cltGameMsgResponse_ExchageWeapon(SI16 active)
	{
		siActiveWeapon	= active;
	}
};

//-------------------------------------------
// Active Clothes의 상태를 통보
//-------------------------------------------
class cltGameMsgResponse_ExchageClothes{
public:
	SI16	siActiveClothes;
	SI16    siActiveHat;

	cltGameMsgResponse_ExchageClothes(SI16 active_c, SI16 active_h )
	{
		siActiveClothes	= active_c;
		siActiveHat = active_h;

	}
};


class cltGameMsgRequest_UpgradeItem
{
public:
	SI16 siToPos;				// 업그레이드할 아이템의 인벤토리 위치
	SI32 siVillageUnique;		// 혹부리영감이 있는 마을의 Index

	cltGameMsgRequest_UpgradeItem( SI16 ToPos, SI32 VillageUnique )
	{
		siToPos = ToPos;
		siVillageUnique = VillageUnique;
	}
};

class cltGameMsgResponse_UpgradeItem
{
public:
	SI16 siFromPos;
	cltItem clFromItem;

	SI16 siToPos;
	cltItem clToItem;

	bool bSuccess;

	cltGameMsgResponse_UpgradeItem( bool Success, SI16 FromPos, cltItem *pclFromItem, SI16 ToPos, cltItem *pclToItem )
	{		
		siFromPos = FromPos;
		clFromItem.Set( pclFromItem );

		siToPos = ToPos;
		clToItem.Set( pclToItem );
		
		bSuccess = Success;
	}
};

class cltGameMsgRequest_PerformanceUpgradeItem
{
public:	
	SI32 siVillageUnique;		// 혹부리영감이 있는 마을의 Index

	SI16 siItemPos;				// 업그레이드할 아이템의 인벤토리 위치
	cltItem clItem;

	cltGameMsgRequest_PerformanceUpgradeItem( SI32 VillageUnique,
		SI16 ItemPos, cltItem *pclItem )
	{
		siVillageUnique = VillageUnique;

		siItemPos = ItemPos;
		clItem.Set( pclItem );
	}
};

class cltGameMsgResponse_PerformanceUpgradeItem
{
public:
	SI16 siItemPos;
	cltItem clItem;

	SI16 siScrollItemPos;
	cltItem clScrollItem;
	
	cltGameMsgResponse_PerformanceUpgradeItem( SI16 ItemPos, cltItem *pclItem,
		SI16 ScrollItemPos, cltItem *pclScrollItem
		)
	{
		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siScrollItemPos = ScrollItemPos;
		clScrollItem.Set( pclScrollItem );
	}
};


class cltGameMsgResponse_MakeItemDlg
{
public:
	cltItem clItem;
	SI32 siBaseExp;
	SI32 siSkillUnique;
	SI32 siSkillExp;
	SI32 siMonsterCorp;

	cltGameMsgResponse_MakeItemDlg( cltItem *pclItem, SI32 baseexp, SI32 skillunique,SI32 skillexp,SI32 monstercorp)
	{
		clItem.Set(pclItem);
		siBaseExp = baseexp ;
		siSkillUnique = skillunique ;
		siSkillExp = skillexp ;
		siMonsterCorp = monstercorp ;
	}
};


class cltGameMsgResponse_EnchantItemDlg
{
public:
	cltItem clItem;
	SI32 siSucess;


	cltGameMsgResponse_EnchantItemDlg( cltItem *pclItem, SI32 Sucess)
	{
		clItem.Set(pclItem);	
		siSucess = Sucess;

	}
};

class cltGameMsgResponse_IsSkillItem
{
public:
	SI08		siResult; // 결과값 1 외에는 전부 실패

	cltGameMsgResponse_IsSkillItem(SI32 result)
	{
		siResult = result ;
	}
};

class cltGameMsgRequest_MonsterCorp_Score
{
public:
	SI32		siScore;

	cltGameMsgRequest_MonsterCorp_Score(SI32 para)
	{
		siScore = para ;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_MonsterCorp_ComboInfo
//
// Last Modified 	: 2007 / 04 / 05
// Created		 	: 김광명
//
// Function			: [요청] 미니 게임 점수 계산을 위해서 서버로 콤보정보를 보낸다.
//
//***************************************************************************************************
class cltGameMsgRequest_MonsterCorp_ComboInfo
{
private:
	SI32		siComboInfo;
	SI32		siScore;

public:
	cltGameMsgRequest_MonsterCorp_ComboInfo(SI32 ComboInfo, SI32 Score) : siComboInfo(ComboInfo), siScore(Score)
	{
		siComboInfo = ComboInfo;
		siScore = Score;
	}

	SI32 GetComboInfo()
	{
		return siComboInfo;
	}

	SI32 GetScore()
	{
		return siScore;
	}
};

class cltGameMsgRequest_RouletteGame_Info
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<RouletteTable, 5>, 5>		m_stRoulette;
#else
	RouletteTable m_stRoulette[5][5];
#endif

	cltGameMsgRequest_RouletteGame_Info(RouletteTable table[][5])
	{
		for(SI32 y = 0;y<5;++y)
		{
			for (SI32 x = 0;x<5;++x) 
			{
				m_stRoulette[y][x].Set(table[y][x].m_bUserSelect,
									   table[y][x].m_bCpuSelect,
									   table[y][x].m_bResult,
									   table[y][x].m_siX,
									   table[y][x].m_siY,
									   table[y][x].m_siValue);
			}
		}
	}
};

class cltGameMsgResponse_RouletteGame_Result
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI16, 5>		 m_siRandom;
#else
	SI16 m_siRandom[5];
#endif
	SI16 m_siResult;
	SI16 m_siNumber;
	
	cltGameMsgResponse_RouletteGame_Result(SI16 siRandom[], SI16 siResult, SI16 siNumber)
	{
		for(SI32 i=0;i<5;++i)
			m_siRandom[i] = siRandom[i];
		m_siResult = siResult;
		m_siNumber = siNumber;
	}
};

class cltGameMsgResponse_Premium_Info
{
public:
	SI32		siItemPos;
	cltItem		clItem ;

	bool	bPremium;
	SI32	siPremiumDate;
	SI32	siCombatDate;
	SI32	siMakeItemDate;
	SI32	siBulletDate;
	SI32	siSummonStaminaDate;

	SI08	siType;

	cltGameMsgResponse_Premium_Info(SI32 pos , cltItem* pclitem,bool premium,SI32 premiumdate,SI32 combatdate,SI32 makeitemdate, SI32 bulletdate, SI32 summonstaminadate, SI08 type)
	{
		siItemPos = pos ;
		clItem.Set(pclitem);

		bPremium = premium ;
		siPremiumDate = premiumdate ;
		siCombatDate = combatdate ;
		siMakeItemDate = makeitemdate;
		siBulletDate = bulletdate;
		siSummonStaminaDate = summonstaminadate;

		siType = type ;
	}
};


//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_MakeItemSwitch
//
// Last Modified 	: 2006 / 01 / 06
// Created		 	: 김광명
//
// Function			: 제조시 제조이미지 보여주기 true는 제조시작 false는 제조끝
//
//***************************************************************************************************
class cltGameMsgRequest_MakeItemSwitch
{
public:
	SI08 siType;	// 0: 제조 안함 1:무기 방어구 제조 2:음식,약

	cltGameMsgRequest_MakeItemSwitch( SI08 Type )
	{
		siType = Type;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_MakeItemSwitch
//
// Last Modified 	: 2006 / 01 / 06
// Created		 	: 김광명
//
// Function			: 제조시 제조이미지 보여주기 true는 제조시작 false는 제조끝
//
//***************************************************************************************************
class cltGameMsgResponse_MakeItemSwitch
{
public:
	SI08 siType;

	cltGameMsgResponse_MakeItemSwitch( SI08 Type )
	{
		siType = Type;
	}
};




//태양 제조시.. 제조창에서 사용될 인덱스를 써버로 보내기 위해 ..  
class cltGameMsgRequest_MakeItem_SelectedIndex
{
public:

	SI32			siMoveInvIndex;
	SI32			siInvSelectedValue;

	cltGameMsgRequest_MakeItem_SelectedIndex( SI32 MoveInvIndex,SI32 InvSelectedValue)
	{
		siMoveInvIndex = MoveInvIndex ;
		siInvSelectedValue = InvSelectedValue ;		
	}
};




//태양 인챈트시.. 제조창에서 사용될 인덱스를 써버로 보내기 위해 ..  
class cltGameMsgRequest_EnchantItem_SelectedIndex
{
public:

	//[인첸트 UI 개선 - 2007.12.24 김영훈 수정 : 인첸트 재료 아이템을 받을 수 있는 변수를 배열로 변경]
	SI32			siOrgInvEnchantInx;  
	SI32			siMatInvEnchantInx;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, NOW_ENCHANT_NUMBER>		siElementEnchantInx;
#else
	SI32			siElementEnchantInx[NOW_ENCHANT_NUMBER];
#endif
	SI32			siElementType;


	cltGameMsgRequest_EnchantItem_SelectedIndex( SI32 OrgInvEnchantInx,SI32 MatInvEnchantInx,const SI32* pElementEnchantInx, SI32 ElementType)
	{
		siOrgInvEnchantInx = OrgInvEnchantInx;
		siMatInvEnchantInx = MatInvEnchantInx;
		
		for (SI16 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
		{
			siElementEnchantInx[siIndex] = *(pElementEnchantInx + siIndex);
		}
		
		siElementType		= ElementType;

	}
};



class  cltGameMsgRequest_Enchant_Switch
{
public:
	bool bEnchantSwitch;


	cltGameMsgRequest_Enchant_Switch( bool EnchantSwitch)
	{	
		bEnchantSwitch  = EnchantSwitch;
	}
};

class  cltGameMsgRequest_Enchant_StartOrEnd
{

public:
	bool bEnchantStart;

	cltGameMsgRequest_Enchant_StartOrEnd( bool EnchantStart)
	{	
		bEnchantStart  = EnchantStart;
	}
};

class cltGameMsgResponse_Enchant_StartOrEnd
{
public:
	bool bEnchantStart;

	cltGameMsgResponse_Enchant_StartOrEnd( bool EnchantStart)
	{
		bEnchantStart = EnchantStart;
	}	
};


class cltGameMsgResponse_EnchantOther
{
public:
	//[인첸트 UI 개선 - 2007.12.24 김영훈 수정 : 인첸트 재료 아이템을 받을 수 있는 변수를 배열로 변경]
	cltItem clOrgItem;
	cltItem clMatItem;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, NOW_ENCHANT_NUMBER>		clEleItem;
#else
	cltItem clEleItem[NOW_ENCHANT_NUMBER];
#endif
	SI32	siOrgIndex;
	SI32	siMatIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, NOW_ENCHANT_NUMBER>			siEleIndex;
#else
	SI32	siEleIndex[NOW_ENCHANT_NUMBER];
#endif
	bool	bStartEnchant;
	bool	bUpdateMaterial;

	cltGameMsgResponse_EnchantOther(cltItem* OrgItem,cltItem* MatItem,cltItem* pEleItem,
		SI32 OrgIndex, SI32 MatIndex, SI32* pEleIndex, bool StartEnchant, bool UpdateMaterial )
	{
		clOrgItem.Set(OrgItem);
		clMatItem.Set(MatItem);
		siOrgIndex = OrgIndex;
		siMatIndex = MatIndex;
		bStartEnchant = StartEnchant;
		bUpdateMaterial = UpdateMaterial;

		for (SI16 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
		{
			clEleItem[siIndex].Init();
			clEleItem[siIndex].Set( &pEleItem[siIndex] );
			siEleIndex[siIndex] = pEleIndex[siIndex];
		}
	}	
};

class cltGameMsgRequest_EnchantOther
{
public:
	//[인첸트 UI 개선 - 2007.12.24 김영훈 수정 : 인첸트 재료 아이템을 받을 수 있는 변수를 배열로 변경]
	SI32	siOrgIndex;
	SI32	siMatIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, NOW_ENCHANT_NUMBER>		siEleIndex;
#else
	SI32	siEleIndex[NOW_ENCHANT_NUMBER];
#endif
	SI32	m_siOtherCharUnique;

	cltGameMsgRequest_EnchantOther(	SI32 OrgIndex, SI32 MatIndex, SI32* pEleIndex, SI32 OtherCharUnique )
	{

		siOrgIndex = OrgIndex;
		siMatIndex = MatIndex;
		for (SI16 siIndex=0;siIndex<NOW_ENCHANT_NUMBER; siIndex++)
		{
			siEleIndex[siIndex] = pEleIndex[siIndex];
		}
		m_siOtherCharUnique = OtherCharUnique;
	}	
};

class cltGameMsgResponse_EnchantPrice
{
public:

	SI32 siUnique;
	cltGameMsgResponse_EnchantPrice(SI32 Unique)
	{
		siUnique = Unique ; 		
	}	
};

class cltGameMsgRequest_EnchantPrice
{
public:

	GMONEY	siCurrentPrice;

	cltGameMsgRequest_EnchantPrice(	GMONEY	CurrentPrice )
	{
		siCurrentPrice= CurrentPrice;	
	}	
};

class cltGameMsgResponse_SetItemTypeInfo
{
private:
	SI16	m_siSetItemType;
	SI16	m_siSetItemStep;

	SI16	m_siSetItemType2;
	SI16	m_siSetItemStep2;

	SI16	m_siSetItemType3;
	SI16	m_siSetItemStep3;

	SI16	m_siSetItemType4;
	SI16	m_siSetItemStep4;

	SI16	m_siSetItemType5;
	SI16	m_siSetItemStep5;

public:

	cltGameMsgResponse_SetItemTypeInfo( SI16 SetItemType, SI16 _siSetItemStep, SI16 SetItemType2, SI16 _siSetItemStep2, SI16 SetItemType3, SI16 _siSetItemStep3, SI16 SetItemType4, SI16 _siSetItemStep4, SI16 SetItemType5, SI16 _siSetItemStep5)
	{
		m_siSetItemType = SetItemType; 
		m_siSetItemStep = _siSetItemStep;

		m_siSetItemType2 = SetItemType2; 
		m_siSetItemStep2 = _siSetItemStep2;

		m_siSetItemType3 = SetItemType3; 
		m_siSetItemStep3 = _siSetItemStep3;

		m_siSetItemType4 = SetItemType4; 
		m_siSetItemStep4 = _siSetItemStep4;

		m_siSetItemType5 = SetItemType5; 
		m_siSetItemStep5 = _siSetItemStep5;
	}

	SI16 GetSetItemTypeInfo() 
	{
		return m_siSetItemType;
	}
	SI16 GetSetItemStepInfo() 
	{
		return m_siSetItemStep;
	}
	SI16 GetSetItemTypeInfo2() 
	{
		return m_siSetItemType2;
	}
	SI16 GetSetItemStepInfo2() 
	{
		return m_siSetItemStep2;
	}

	SI16 GetSetItemStepInfo3() 
	{
		return m_siSetItemStep3;
	}

	SI16 GetSetItemStepInfo4() 
	{
		return m_siSetItemStep4;
	}

	SI16 GetSetItemStepInfo5() 
	{
		return m_siSetItemStep5;
	}
};


//[추가 : 황진성 2008. 1. 29 => 공간 상자 아이템 정보 S->C]
class cltGameMsgResponse_Space_Box_List
{
public:
	cltSpaceBox		m_SpaceBoxItem;
	cltTreasureBox	m_TreasureBoxItem;

	cltGameMsgResponse_Space_Box_List()
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Space_Box_List ) );	
	}

	cltGameMsgResponse_Space_Box_List( cltSpaceBox* _SpaceBoxItem, cltTreasureBox* _TreasureBoxItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Space_Box_List ) );	

		memcpy( &m_SpaceBoxItem,	_SpaceBoxItem,    sizeof( m_SpaceBoxItem ) );
		memcpy( &m_TreasureBoxItem, _TreasureBoxItem, sizeof( m_TreasureBoxItem ) );
	}
};

// [영훈] 공간상자 아이템을 사용한 정보
class cltGameMsgResponse_Use_SpaceBox_Item
{
public:
	SI16	m_siItemPos;
	cltItem	m_clItem;

	cltGameMsgResponse_Use_SpaceBox_Item()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Use_SpaceBox_Item) );
	}

	cltGameMsgResponse_Use_SpaceBox_Item( SI16 siItemPos, cltItem* pclItem )
	{	
		m_siItemPos = siItemPos;
		m_clItem.Set( pclItem );
	}

};

class cltGameMsgResponse_Use_Treasure_Item
{
public:
	SI16	m_siItemPos;
	cltItem	m_clItem;

	cltGameMsgResponse_Use_Treasure_Item()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Use_Treasure_Item) );
	}

	cltGameMsgResponse_Use_Treasure_Item( SI16 siItemPos, cltItem* pclItem )
	{	
		m_siItemPos = siItemPos;
		m_clItem.Set( pclItem );
	}

};

////[추가 : 황진성 2008. 1. 29 => 공간 상자(보물) 아이템 정보 S->C]
//class cltGameMsgResponse_Treasure_Box_List
//{
//private:
//	cltTreasureBox m_TreasureBoxItem;
//
//public:
//
//	cltGameMsgResponse_Treasure_Box_List()
//	{
//		ZeroMemory( this, sizeof( cltGameMsgResponse_Treasure_Box_List ) );	
//	}
//};

//[추가 : 황진성 2008. 1. 29 => 공간, 보물 상자 아이템 저장 S->C]
class cltGameMsgRequest_Keep_Box_Move_Item
{
public:
	bool		m_bGacha;					// 보물상자인가 공간상자 인가?
	SI32		m_siPos;					// 아이템의 위치.
	cltItem		m_KeepItem;					// 저장 할 아이템.

	cltGameMsgRequest_Keep_Box_Move_Item( bool _bGacha, SI32 _siPos, cltItem* _KeepItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Keep_Box_Move_Item ) );	

		m_bGacha   = _bGacha;
		m_siPos	   = _siPos;
		m_KeepItem.Set( _KeepItem );
	}
};

//[추가 : 황진성 2008. 1. 29 => 공간, 보물 상자 아이템 저장 S->C]
class cltGameMsgResponse_Keep_Box_Move_Item
{
public:
	bool		m_bGacha;					// 보물상자인가 공간상자 인가?
	
	SI16		m_siInvenPos;				// 인벤토리 아이템의 위치.
	cltItem		m_InvenItem;				// 인벤토리 할 아이템.
	
	SI16		m_siKeepPos;				// 아이템의 위치.
	cltItem		m_KeepItem;					// 저장 한 아이템.

	cltGameMsgResponse_Keep_Box_Move_Item( bool _bGacha, SI16 _siInvenPos, cltItem* _InvenItem, SI16 _siKeepPos, cltItem* _KeepItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Keep_Box_Move_Item ) );	

		m_bGacha   = _bGacha;

		m_siInvenPos = _siInvenPos;	
		m_InvenItem.Set( _InvenItem );

		m_siKeepPos	= _siKeepPos;
		m_KeepItem.Set( _KeepItem );
	}
};


//[추가 : 황진성 2008. 1. 29 => 공간, 보물 상자 아이템 회수 S->C]
class cltGameMsgRequest_Recovery_Box_Move_Item
{
public:
	bool			m_bGacha;				// 보물상자인가 공간상자 인가?
	SI32			m_siPos;				// 아이템의 위치.
	cltItem			m_RecoveryItem;			// 회수 할 아이템.

	cltGameMsgRequest_Recovery_Box_Move_Item( bool _bGacha, SI32 _siPos, cltItem* _RecoveryItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgRequest_Recovery_Box_Move_Item ) );

		m_bGacha	   = _bGacha;		
		m_siPos		   = _siPos;		
		m_RecoveryItem.Set( _RecoveryItem );
	}
};

//[추가 : 황진성 2008. 1. 29 => 공간, 보물 상자 아이템 회수 S->C]
class cltGameMsgResponse_Recovery_Box_Move_Item
{
public:
	bool		m_bGacha;					// 보물상자인가 공간상자 인가?
	
	SI16		m_siInvenPos;				// 인벤토리 아이템의 위치.
	cltItem		m_InvenItem;				// 인벤토리 아이템.

	SI16		m_siRecoveryPos;			// 아이템의 위치.
	cltItem		m_RecoveryItem;				// 회수 한 아이템.

	cltGameMsgResponse_Recovery_Box_Move_Item( bool _bGacha, SI16 _siInvenPos, cltItem* _InvenItem , SI16 _siRecoveryPos, cltItem* _RecoveryItem )
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Recovery_Box_Move_Item ) );	

		m_bGacha   = _bGacha;
		
		m_siInvenPos = _siInvenPos;	
		m_InvenItem.Set( _InvenItem );
		
		m_siRecoveryPos = _siRecoveryPos;
		m_RecoveryItem.Set( _RecoveryItem );
	}
};

// [영훈] 인첸트 속성 변경 요청 - 2008.02.20
class cltGameMsgRequest_ChangeEnchant
{
public:
	SI16	m_siOrgUnique;	// 바꾸고자 하는 아이템의 Unique
	SI16	m_siOrgIndex;	// 바꾸고자 하는 아이템이 들어있는 인벤토리 번호
	SI16	m_siChangeType;	// 바꾸고자 하는 인첸트 속성

	SI16	m_siEleIndex;	// 인첸트 변환 속성 아이템이 들어있는 인벤토리 번호


	cltGameMsgRequest_ChangeEnchant(SI16 siOrgUnique, SI16 siOrgIndex, SI16 siChangeType, SI16 siEleIndex)
	{
		m_siOrgUnique		= siOrgUnique;
		m_siOrgIndex		= siOrgIndex;
		m_siChangeType		= siChangeType;

		m_siEleIndex		= siEleIndex;
	}
};

class cltGameMsgRequest_OpenBigItemBox
{
public:
	SI16		m_siBoxPos;				// 박스 아이템의 위치.
	cltItem		m_clBoxItem;			// 박스 아이템.

	SI16		m_siKeyPos;				// 키 아이템의 위치.
	cltItem		m_clKeyItem;			// 키 아이템.

	cltGameMsgRequest_OpenBigItemBox(SI16 siBoxPos, cltItem* pclBoxItem, SI16 siKeyPos, cltItem* pclKeyItem)
	{
		m_siBoxPos	= siBoxPos;	
		if(pclBoxItem)	m_clBoxItem.Set(pclBoxItem);

		m_siKeyPos	= siKeyPos;	
		if(pclKeyItem)	m_clKeyItem.Set(pclKeyItem);
	}
};

//---------------------------------------
// 사람에서 소환수에게 아이템이 이동되었을때
//---------------------------------------
class cltGameMsgRequest_SummonPushItem
{
public:

	SI08 m_siSummonIndex;	// 소환수 인덱스
	//From
	UI16 m_uiFrom;			// 어디에서 옮기는가.
	//To
	UI16 m_uiTo;			// 어디로 옮기는가 ?


	cltGameMsgRequest_SummonPushItem()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_SummonPushItem));
	}

	cltGameMsgRequest_SummonPushItem( SI08 siSummonIndex, UI16 uiFrom, UI16 uiTo )
	{
		m_siSummonIndex	= siSummonIndex;
		m_uiFrom		= uiFrom;
		m_uiTo			= uiTo;
	}

};


//---------------------------------------
// 소환수에서 사람에게 아이템이 이동되었을때
//---------------------------------------
class cltGameMsgRequest_SummonPopItem
{
public:

	SI08 m_siSummonIndex;	// 소환수 인덱스
	//From
	UI16 m_uiFrom;			// 어디에서 옮기는가.
	//To
	UI16 m_uiTo;			// 어디로 옮기는가 ?


	cltGameMsgRequest_SummonPopItem()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_SummonPopItem));
	}

	cltGameMsgRequest_SummonPopItem( SI08 siSummonIndex, UI16 uiFrom, UI16 uiTo )
	{
		m_siSummonIndex	= siSummonIndex;
		m_uiFrom		= uiFrom;
		m_uiTo			= uiTo;
	}

};

// Push/Pop를 해서 소환수의 아이템이 변경되었다
class cltGameMsgResponse_SummonChangeItem
{
public:

	SI08	m_siSummonIndex;	// 소환수 인덱스
	SI16	m_siItemPos;		// 아이템 위치
	cltItem	m_clItem;			// 아이템


	cltGameMsgResponse_SummonChangeItem()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_SummonChangeItem));
	}

	cltGameMsgResponse_SummonChangeItem( SI08 siSummonIndex, SI16 siItemPos, cltItem* pclItem )
	{
		m_siSummonIndex	= siSummonIndex;
		m_siItemPos		= siItemPos;
		m_clItem.Set( pclItem );
	}

};

//[진성] 호박상자. 박스에서 나올 아이템이 정보. => 2008-9-16
class cltGameMsgResponse_Amber_Box_List
{
public:
	SI32			m_siItemList[AMBER_BOX_LIST_NUM];
	SI32			m_siListNum;
	SI16			m_siUseItemUnique;						// 사용될 아이템의 유니크.

public:
	cltGameMsgResponse_Amber_Box_List()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_Amber_Box_List));
	}

	cltGameMsgResponse_Amber_Box_List( const SI32* _psiItemList, SI32 _siListNum, SI16	_siUseItemUnique )
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_Amber_Box_List));

		memcpy( m_siItemList, _psiItemList, sizeof(SI32)*_siListNum );

		m_siListNum			= _siListNum;
		m_siUseItemUnique	= _siUseItemUnique;
	}
};

//[진성] 호박상자. 클릭하는 세가지 아이템 항목 리스트. => 2008-9-16
class cltGameMsgResponse_Amber_Box_Select_Item_List
{
public:
	AmberboxSelectItemInfo	m_clAmberboxItemList[3];

public:
	cltGameMsgResponse_Amber_Box_Select_Item_List(AmberboxSelectItemInfo* _clAmberboxItemList)
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Amber_Box_Select_Item_List ) );

		memcpy(m_clAmberboxItemList, _clAmberboxItemList, sizeof(m_clAmberboxItemList));
	}
};

// [진성] 호박상자. 클릭하여 렌덤하게 결정된 아이템 추가. => 2008-9-16
class cltGameMsgRequest_Amber_Box_Select_Item_Add	// response : cltGameMsgResponse_Amber_Box_Select_Item_List
{
public:
	SI16 	m_siSlotIndex;
	SI32		m_clUseItemUnique;
public:
	cltGameMsgRequest_Amber_Box_Select_Item_Add( SI16 _siSlotIndex  , SI32 _clUseItemUnique)
	{
		m_siSlotIndex = _siSlotIndex;
		m_clUseItemUnique = _clUseItemUnique;
	}
};


// [진성] 호박상자. 클릭한 아이템 획득. => 2008-9-16
class cltGameMsgRequest_Amber_Box_Select_Item_Get	
{
public:
	SI32					m_clUseItemUnique;		// 사용되는 아이템. 
	
	AmberboxSelectItemInfo	m_clAmberboxItem;		// 선택한 아이템 정보.

public:
	cltGameMsgRequest_Amber_Box_Select_Item_Get( SI32 _clUseItemUnique, AmberboxSelectItemInfo*	_pclAmberboxItem )
	{
		m_clUseItemUnique = _clUseItemUnique;
		
		memcpy( &m_clAmberboxItem, _pclAmberboxItem, sizeof(m_clAmberboxItem) );
	}
};

class cltGameMsgResponse_Amber_Box_Select_Item_Get	
{
public:
	SI16					m_siRemainItemPos;		// 사용하고 남은 아이템의 인벤 위치. 
	cltItem					m_clRemainItem;			// 사용하고 남은 아이템 정보.

	SI16					m_siToItemPos;			// 얻은 아이템의 인벤 위치. 
	cltItem					m_clToItem;				// 얻은 아이템 정보(기존 아이템이 있으면 누적된 정보).


public:
	cltGameMsgResponse_Amber_Box_Select_Item_Get( 	SI16 _siRemainItemPos,   cltItem* _pclRemainItem,
													SI16 _siToItemPos,		 cltItem* _pclToItem		)
	{
		m_siRemainItemPos = _siRemainItemPos;
		m_clRemainItem.Set( _pclRemainItem );

		m_siToItemPos	  = _siToItemPos;
		m_clToItem.Set( _pclToItem );
	}
};

// [영훈] 새로운 선택상자에서 12가지 값중 하나를 선택할때
class cltGameMsgRequest_NewSelectBox_Select
{
public:
	SI16	m_siBoxItemUnique;
	SI16	m_siSelectIndex;

	cltGameMsgRequest_NewSelectBox_Select( SI16 siBoxItemUnique, SI16 siSelectIndex )
	{
		m_siBoxItemUnique	= siBoxItemUnique;
		m_siSelectIndex		= siSelectIndex;
	}
};	// GAMEMSG_REQUEST_NEWSELECTBOX_SELECT

// [영훈]
class cltGameMsgResponse_NewSelectBox_Select
{
public:
	SI16	m_siBoxItemUnique;	// 기존 선택값과 리턴값을 확인하기 위한용
	SI16	m_siSelectIndex;
	// 이 선택 아이템에서 나올 아이템 목록
	SI16	m_siItemList[MAX_NEWSELECTBOX_ITEM_LIST];	
	
	cltGameMsgResponse_NewSelectBox_Select( SI16 siBoxItemUnique, SI16 siSelectIndex, SI16* psiItemList )
	{
		m_siBoxItemUnique	= siBoxItemUnique;
		m_siSelectIndex		= siSelectIndex;

		if ( psiItemList )
		{
			memcpy( m_siItemList, psiItemList, sizeof(m_siItemList) );
		}
	}
};	// GAMEMSG_RESPONSE_NEWSELECTBOX_SELECT

// [영훈] 새로운 선택상자에서 아이템을 받는다
class cltGameMsgRequest_NewSelectBox_GetItem
{
public:
	SI16	m_siBoxItemUnique;
	SI16	m_siSelectIndex;

	cltGameMsgRequest_NewSelectBox_GetItem( SI16 siBoxItemUinque, SI16 siSelectIndex )
	{
		m_siBoxItemUnique	= siBoxItemUinque;
		m_siSelectIndex		= siSelectIndex;
	}
};	// GAMEMSG_REQUEST_NEWSELECTBOX_GETITEM

class cltGameMsgResponse_NewSelectBox_GetItem
{
public:
	cltItem	m_clBoxItem;	// 사용하고 남은 상자 아이템
	SI16	m_siBoxItemPos;	// 

	cltItem	m_clGetItem;	// 받은 아이템
	SI16	m_siGetItemPos;	//

	cltItem m_clToItem;		// 내가 실제로 받은아이템(위의 받은 아이템은 내 인벤에 있는 것과 합쳐진것);

	cltGameMsgResponse_NewSelectBox_GetItem( cltItem* pclBoxItem, SI16 siBoxItemPos, cltItem* pclGetItem, SI16 siGetItemPos, cltItem* pclToItem )
	{
		if ( pclBoxItem )
		{
			m_clBoxItem.Set( pclBoxItem );
		}
		m_siBoxItemPos = siBoxItemPos;

		if ( pclGetItem )
		{
			m_clGetItem.Set( pclGetItem );
		}
		m_siGetItemPos = siGetItemPos;

		if ( pclToItem )
		{
			m_clToItem.Set( pclToItem );
		}
	}

};	// GAMEMSG_RESPONSE_NEWSELECTBOX_GETITEM

//---------------------------------------------------------------
// NPC수리 요청. [강형] 새로운 수리
//--------------------------------------------------------------
class cltGameMsgRequest_NewNPCRepair
{
public:
	SI16	m_siPos;			// 인벤내 아이템 위치. 
	cltItem m_clItem;			// 수리 대상 아이템 

	GMONEY	m_money;

	// 수리 재료 아이템 : 수리석 + 기타재료
	RepairItemExtraMaterial	m_Material;

	cltGameMsgRequest_NewNPCRepair( SI16 pos, cltItem* pclitem, GMONEY money, RepairItemExtraMaterial& ExtraMaterial )
	{
		m_siPos			= pos;
		m_clItem.Set(pclitem);

		m_money			= money;

		for( SI32 i=0; i<m_Material.GetSize(); i++ )
		{
			m_Material[i].Set( &ExtraMaterial[i] );
		}
	}
};

class cltGameMsgResponse_NewNpcRepair
{
public:
	SI32	m_siResult;

};

//---------------------------------------------------------------
// 아이템 수리 & 요청 [강형] 새로운 수리
//--------------------------------------------------------------
class cltGameMsgRequest_NewItemRepair
{
public:
	SI16	m_siPos;			// 인벤내 아이템 위치. 
	cltItem m_clItem;			// 수리 대상 아이템 

	GMONEY	m_money;

	// 수리 재료 아이템 : 수리석 + 기타재료
	RepairItemExtraMaterial	m_Material;

	SI16	m_siType;			// 1:직접수리, 2:수리요청, 3:의뢰수리, 4:수리요청 취소
	SI32	m_siItemPersonID;
    
	cltGameMsgRequest_NewItemRepair( SI16 pos, cltItem* pclitem, GMONEY money, RepairItemExtraMaterial& ExtraMaterial, SI32 siItemPersonID, SI16 siType )
	{
		m_siPos			= pos;
		m_clItem.Set(pclitem);

		m_money			= money;

		for( SI32 i=0; i<m_Material.GetSize(); i++ )
		{
			m_Material[i].Set( &ExtraMaterial[i] );
		}

		m_siItemPersonID= siItemPersonID;
		m_siType		= siType;
	}
};

class cltGameMsgResponse_NewItemRepair
{
public:
	SI32	m_siResult;
	SI32	m_siItemCharUnique;

	cltItem m_clItem;			// 수리 대상 아이템 

	SI16	m_siType;			// 1:직접수리, 2:수리요청, 3:의뢰수리, 4:수리요청 취소

	cltGameMsgResponse_NewItemRepair( SI32 siResult, SI32 siCharUnique, cltItem* pclitem, SI16 siType )
	{
		m_siResult			= siResult;

		m_siItemCharUnique	= siCharUnique;
		m_clItem.Set( pclitem );
		m_siType			= siType;
	}
};

//---------------------------------------------------------------
// 아이템 수리정보 요청
//--------------------------------------------------------------
#define NEWITEMREPAIR_INFO_BANNER		0x0001
#define NEWITEMREPAIR_INFO_ITEM			0x0002

class cltGameMsgRequest_NewItemRepairRequestInfo
{
public:
	SI32	m_siCharUnique;		// 수리를 요청한 캐릭터 유니크

	SI16	m_siType;			// 0x01:수리배너 정보 요청, 0x02:수리상세정보 요청

	cltGameMsgRequest_NewItemRepairRequestInfo(SI32 siCharUnique, SI32 siType)
	{
		m_siCharUnique		= siCharUnique;
		m_siType			= siType;
	}
};

class cltGameMsgResponse_NewItemRepairRequestInfo
{
public:
	SI32	m_siItemPersonID;	// 아이템을 가지고 있는 캐릭터의 PersonID
	SI32	m_siItemCharUnique;	// 아이템을 가지고 있는 캐릭터의 CharUnique

	SI16	m_siPos;			// 인벤내 아이템 위치. 
	cltItem m_clItem;			// 수리 대상 아이템 

	GMONEY	m_money;

	// 수리 재료 아이템 : 수리석 + 기타재료
	RepairItemExtraMaterial	m_Material;

	SI16	m_siType;			// 0x0001:수리배너 정보 요청, 0x0002:수리상세정보 요청

	cltGameMsgResponse_NewItemRepairRequestInfo( SI32 siItemPersonID, SI32 siItemCharUnique, SI16 pos, cltItem* pclitem, GMONEY money, RepairItemExtraMaterial& ExtraMaterial, SI32 siType )
	{
		m_siItemPersonID	= siItemPersonID;
		m_siItemCharUnique	= siItemCharUnique;

		m_siPos			= pos;
		m_clItem.Set(pclitem);

		m_money			= money;

		for( SI32 i=0; i<m_Material.GetSize(); i++ )
		{
			m_Material[i].Set( &ExtraMaterial[i] );
		}

		m_siType			= siType;
	}
};

class cltGameMsgRequest_EquipmentCreate
{
public:
	SI32		m_siWeaponUnique;
	SI32		m_siHelmetUnique;
	SI32		m_siArmorUnique;
	SI32		m_siBeltUnique;
	SI32		m_siShoeUnique;
	SI32		m_siNeckUnique;
	SI32		m_siRingUnique;
	SI32		m_siMantleUnique;

	SI32		m_siWeaponPower1;
	SI32		m_siWeaponPower2;
	SI32		m_siWeaponPower3;
	SI32		m_siWeaponPower4;

	SI32		m_siHelmetPower1;
	SI32		m_siHelmetPower2;
	SI32		m_siHelmetPower3;
	SI32		m_siHelmetPower4;

	SI32		m_siArmorPower1;
	SI32		m_siArmorPower2;
	SI32		m_siArmorPower3;
	SI32		m_siArmorPower4;

	SI32		m_siBeltPower1;
	SI32		m_siBeltPower2;
	SI32		m_siBeltPower3;
	SI32		m_siBeltPower4;

	SI32		m_siShoePower1;
	SI32		m_siShoePower2;
	SI32		m_siShoePower3;
	SI32		m_siShoePower4;

	SI32		m_siMantlePower1;
	SI32		m_siMantlePower2;
	SI32		m_siMantlePower3;
	SI32		m_siMantlePower4;

	SI32		m_siNeckLacePower1;
	SI32		m_siNeckLacePower2;
	SI32		m_siNeckLacePower3;
	SI32		m_siNeckLacePower4;

	SI32		m_siRingPower1;
	SI32		m_siRingPower2;
	SI32		m_siRingPower3;
	SI32		m_siRingPower4;

	SI32		m_siWeaponProPerty;
	SI32		m_siHelmetProPerty;
	SI32		m_siArmorProPerty;
	SI32		m_siBeltProPerty;
	SI32		m_siShoeProPerty;

	SI32		m_siWeaponEnchant;
	SI32		m_siHelmetEhchant;
	SI32		m_siArmorEnchant;
	SI32		m_siBeltEnchat;
	SI32		m_siShoeEnchant;

	SI32		m_siWeaponSlot;
	SI32		m_siHelmetSlot;
	SI32		m_siArmorSlot;
	SI32		m_siBeltSlot;
	SI32		m_siShoeSlot;

	cltGameMsgRequest_EquipmentCreate (SI32 siWeaponUnique, SI32 siHelmetUnique, SI32 siArmorUnique, SI32 siBeltUnique, SI32 siShoeUnique, SI32 siNeckUnique, SI32 siRingUnique, SI32 siMantleUnique,
									   SI32 siWeaponPower1, SI32 siWeaponPower2, SI32 siWeaponPower3, SI32 siWeaponPower4, SI32 siHelmetPower1, SI32 siHelmetPower2, SI32 siHelmetPower3, SI32 siHelmetPower4,
									   SI32 siArmorPower1, SI32 siArmorPower2, SI32 siArmorPower3, SI32 siArmorPower4, SI32 siBeltPower1, SI32 siBeltPower2, SI32 siBeltPower3, SI32 siBeltPower4,
									   SI32 siShoePower1, SI32 siShoePower2, SI32 siShoePower3, SI32 siShoePower4, SI32 siMantlePower1, SI32 siMantlePower2, SI32 siMantlePower3, SI32 siMantlePower4,
									   SI32 siNeckLacePower1, SI32 siNeckLacePower2, SI32 siNeckLacePower3, SI32 siNeckLacePower4, SI32 siRingPower1, SI32 siRingPower2, SI32 siRingPower3, SI32 siRingPower4,
									   SI32 siWeaponProPerty, SI32 siHelmetProPerty, SI32 siArmorProPerty, SI32 siBeltProPerty, SI32 siShoeProPerty,
									   SI32 siWeaponEnchant, SI32 siHelmetEhchant, SI32 siArmorEnchant, SI32 siBeltEnchat, SI32 siShoeEnchant,
									   SI32 siWeaponSlot, SI32 siHelmetSlot, SI32 siArmorSlot, SI32 siBeltSlot, SI32 siShoeSlot)
	{
		m_siWeaponUnique = siWeaponUnique;
		m_siHelmetUnique = siHelmetUnique;
		m_siArmorUnique = siArmorUnique;
		m_siBeltUnique = siBeltUnique;
		m_siShoeUnique = siShoeUnique;
		m_siNeckUnique = siNeckUnique;
		m_siRingUnique = siRingUnique;
		m_siMantleUnique = siMantleUnique;

		m_siWeaponPower1 = siWeaponPower1;
		m_siWeaponPower2 = siWeaponPower2;
		m_siWeaponPower3 = siWeaponPower3;
		m_siWeaponPower4 = siWeaponPower4;

		m_siHelmetPower1 = siHelmetPower1;
		m_siHelmetPower2 = siHelmetPower2;
		m_siHelmetPower3 = siHelmetPower3;
		m_siHelmetPower4 = siHelmetPower4;

		m_siArmorPower1 = siArmorPower1;
		m_siArmorPower2 = siArmorPower2;
		m_siArmorPower3 = siArmorPower3;
		m_siArmorPower4 = siArmorPower4;

		m_siBeltPower1 = siBeltPower1;
		m_siBeltPower2 = siBeltPower2;
		m_siBeltPower3 = siBeltPower3;
		m_siBeltPower4 = siBeltPower4;

		m_siShoePower1 = siShoePower1;
		m_siShoePower2 = siShoePower2;
		m_siShoePower3 = siShoePower3;
		m_siShoePower4 = siShoePower4;

		m_siMantlePower1 = siMantlePower1;
		m_siMantlePower2 = siMantlePower2;
		m_siMantlePower3 = siMantlePower3;
		m_siMantlePower4 = siMantlePower4;

		m_siNeckLacePower1 = siNeckLacePower1;
		m_siNeckLacePower2 = siNeckLacePower2;
		m_siNeckLacePower3 = siNeckLacePower3;
		m_siNeckLacePower4 = siNeckLacePower4;

		m_siRingPower1 = siRingPower1;
		m_siRingPower2 = siRingPower2;
		m_siRingPower3 = siRingPower3;
		m_siRingPower4 = siRingPower4;

		m_siWeaponProPerty = siWeaponProPerty;
		m_siHelmetProPerty = siHelmetProPerty;
		m_siArmorProPerty = siArmorProPerty;
		m_siBeltProPerty = siBeltProPerty;
		m_siShoeProPerty = siShoeProPerty;

		m_siWeaponEnchant = siWeaponEnchant;
		m_siHelmetEhchant = siHelmetEhchant;
		m_siArmorEnchant = siArmorEnchant;
		m_siBeltEnchat = siBeltEnchat;
		m_siShoeEnchant = siShoeEnchant;

		m_siWeaponSlot = siWeaponSlot;
		m_siHelmetSlot = siHelmetSlot;
		m_siArmorSlot = siArmorSlot;
		m_siBeltSlot = siBeltSlot;
		m_siShoeSlot = siShoeSlot;
	}
};

class cltGameMsgResponse_EquipmentCreate
{
public:
	SI32	m_siResult;

};

class cltGameMsgRequest_SkillBook_BuyItem
{
public:	
	SI32		m_siGrade;
	cltItem		m_clBuyItem;
	cltItem		m_clUseItem;
	SI32		m_siInvenPos;
	GMONEY		m_siUseMoney;

	cltGameMsgRequest_SkillBook_BuyItem( SI32 _siGrade, cltItem* _pclBuyItem, cltItem* _pclUseItem, SI32 _siInvenPos, GMONEY	_siUseMoney )
	{
		ZeroMemory( this, sizeof(*this) );

		m_siGrade	 = _siGrade;
		m_clBuyItem.Set( _pclBuyItem );
		m_clUseItem.Set( _pclUseItem );
		m_siInvenPos = _siInvenPos;
		m_siUseMoney = _siUseMoney;
	}
};

class cltGameMsgResponse_SkillBook_BuyItem
{
public:
	cltGameMsgResponse_SkillBook_BuyItem( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}

};

class cltGameMsgResponse_SkillBook_MagicInfo_Get
{
public:
	cltGameMsgResponse_SkillBook_MagicInfo_Get() 
	{
		ZeroMemory( this, sizeof(*this) );
	}
	~cltGameMsgResponse_SkillBook_MagicInfo_Get()
	{

	}

	SI08					m_iIndex[SKILLBOOK_MAGIC_NUM];
	cltSkillBookMagic		m_clAddSkillBookMagic[SKILLBOOK_MAGIC_NUM];
	cltSkillBookSubMagic	m_clAddSkillBookSubMagic[MAX_SKILLBOOK_SUBMAGIC_NUM];
};

class cltGameMsgResponse_SkillBook_Magic_Add
{
public:
	cltGameMsgResponse_SkillBook_Magic_Add( cltSkillBookMagic* _pclAddSkillBookMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		memcpy( &m_clAddSkillBookMagic, _pclAddSkillBookMagic, sizeof(m_clAddSkillBookMagic) );
	}
	
	SI08				m_iIndex;
	cltSkillBookMagic	m_clAddSkillBookMagic;
};

class cltGameMsgRequest_SkillBook_Magic_Del
{
public:
	cltGameMsgRequest_SkillBook_Magic_Del( SI08	_iIndex, cltSkillBookMagic* _pclAddSkillBookMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		m_iIndex = _iIndex;
		m_clAddSkillBookMagic.Set( _pclAddSkillBookMagic );
	}

	SI08				m_iIndex;
	cltSkillBookMagic	m_clAddSkillBookMagic;
};

class cltGameMsgResponse_SkillBook_Magic_Del
{
public:
	cltGameMsgResponse_SkillBook_Magic_Del( SI08 _iIndex )
	{
		ZeroMemory( this, sizeof(*this) );

		m_iIndex = _iIndex;
	}

	SI08				m_iIndex;
};


class cltGameMsgResponse_SkillBook_SubMagic_Add
{
public:
	cltGameMsgResponse_SkillBook_SubMagic_Add( cltSkillBookSubMagic* _pclAddSkillBookSubMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		memcpy( &m_clAddSkillBookSubMagic, _pclAddSkillBookSubMagic, sizeof(m_clAddSkillBookSubMagic) );
	}

	cltSkillBookSubMagic m_clAddSkillBookSubMagic;
};

class cltGameMsgRequest_SkillBook_SubMagic_Del
{
public:
	cltGameMsgRequest_SkillBook_SubMagic_Del( cltSkillBookSubMagic* _pclAddSkillBookSubMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		memcpy( &m_clAddSkillBookSubMagic, _pclAddSkillBookSubMagic, sizeof(m_clAddSkillBookSubMagic) );
	}

	cltSkillBookSubMagic m_clAddSkillBookSubMagic;
};

class cltGameMsgResponse_SkillBook_SubMagic_Del
{
public:
	cltGameMsgResponse_SkillBook_SubMagic_Del( cltSkillBookSubMagic* _pclAddSkillBookSubMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		memcpy( &m_clAddSkillBookSubMagic, _pclAddSkillBookSubMagic, sizeof(m_clAddSkillBookSubMagic) );
	}

	cltSkillBookSubMagic m_clAddSkillBookSubMagic;
};


//////////////////////////////////////////////////////////////////////////
//[지연] : 2010신년이벤트 - 신년이벤트에 쓰일 메세지 클래스 선언
//////////////////////////////////////////////////////////////////////////

// 운세 : 나온 운세의 결과를 클라에 통보하기 위한 클래스
class cltGameMsgResponse_2010NewYear_FortuneResult
{
public:
	SI32 m_siPersonID;
	NTCHARString128 m_strUserName;

	SI16 m_siFortuneQuality;		// 대길/길/흉/팅키 (팅키 받으면 퀄리티에 100을 넣어서 클라에 전달)
	SI32 m_siFortuneTextUnique;		// 대길/길/흉에 따른 운세 메세지의 유니크
	SI32 m_siBuffNameUnique;		// 길 일때 받은 이로운 버프의 이름 유니크

	SI16 m_siQuestClearCondition;	// 흉 일때 받은 퀘스트의 클리어 조건
	SI32 m_siQuestPara1;			// 퀘스트 파라미터 1
	SI32 m_siQuestPara2;			// 퀘스트 파라미터 2

public:
	cltGameMsgResponse_2010NewYear_FortuneResult(SI32 siPersonID, SI16 siFortuneQuality, SI32 siFortuneTextUnique, SI32 siBuffNameUnique, SI16 siQuestClearCondition, SI32 siQuestPara1, SI32 siQuestPara2, TCHAR* pUserName = NULL)
	{
		ZeroMemory( this, sizeof(*this) );

		m_siPersonID = siPersonID;

		m_siFortuneQuality = siFortuneQuality;
		m_siFortuneTextUnique = siFortuneTextUnique;
		m_siBuffNameUnique = siBuffNameUnique;

		m_siQuestClearCondition = siQuestClearCondition;
		m_siQuestPara1 = siQuestPara1;
		m_siQuestPara2 = siQuestPara2;

		m_strUserName = pUserName;
	}
};
// [성웅] 레벨 보상 상자에서 아이템을 준다 
class cltGameMsgRequest_UseLimitLevleBox
{
public:
	SI16 m_siOpenItemBox	;	
	cltGameMsgRequest_UseLimitLevleBox()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_UseLimitLevleBox));
	}

	cltGameMsgRequest_UseLimitLevleBox( SI16 siOpenItemBox )
	{
		m_siOpenItemBox	= siOpenItemBox;
	}

};
//[성웅] // 죽었던 말던 어떤상황이던 아이템을 줄인다 
class cltGameMsgRequest_UseJustUseItem
{
public:
	SI16 m_siitempos	;	// 사용할 아이템의 포지션 
	SI32 m_siitemunique	;	// 사용할 아이템 유니크 	
	SI32 m_siitemnum	;	// 사용할 아이템의 수량 
	cltGameMsgRequest_UseJustUseItem()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_UseJustUseItem));
	}

	cltGameMsgRequest_UseJustUseItem( SI32 siitemunique, SI16 siitempos, SI32 siitemnum  )
	{
		m_siitemunique	= siitemunique;		
		m_siitemnum	= siitemnum;
		m_siitempos	= siitempos;

	}

};
#endif

