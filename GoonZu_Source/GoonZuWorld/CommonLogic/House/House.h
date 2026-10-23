//---------------------------------
// 2003/6/26 김태곤
//---------------------------------
#ifndef _HOUSE_H
#define _HOUSE_H

#include <Directives.h>

#include "..\StructureClass\StructureClass.h"
#include "..\..\Server\Time\MyTime.h"

#include "..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "../../common/Horse/Horse.h"

#include <CommonHeader.h>
#include <PersonInfo.h>

#define MAX_HOUSE_TAB_NUMBER							5

// 마을에 창고(시전) 갯수를 우선 한국만 150개로 늘리고 나머지 국가는 차후 사양변경한다
#ifdef _KOREA
	#define MAX_HOUSEUNIT_NUMBER							150		// 각 마을별 시전의 개수
#else
	#define MAX_HOUSEUNIT_NUMBER							100		// 각 마을별 시전의 개수
#endif

#define MAX_HOUSEUNIT_HORSENUMBER						5		// 각 시전에 말을 넣어둘수 있는 개수
#define MAX_HOUSEUNIT_SUMMONNUMBER						5		// 각 시전에 소환수를 넣어둘수 있는 개수
#define MAX_APPLY_PER_HOUSE								50		// 시전 임차 신청인의 수. (한 마을당. ) 
#define MAX_RENT_PERIOD_YEAR							2		// 최대 임대 가능 년수.

#define LEVELLIMIT_USE_HOUSE							20		// 시전 사용을 가능하게 하는 최소 레벨.

#define MIN_HOUSE_RENT_FEE								10000	// 시전 임대료 최저가격. 	
#define MAX_HOUSE_RENT_FEE								200000	// 시전 임대료 최고 가격. 

#define HOUSEUNIT_CREATESTABLE_USEITEMNUM				1		// 마구간 짓는데, 사용되는 이용권 개수
#define HOUSEUNIT_CREATESTABLE_NEEDMONEY				1000	// 마구간 짓는데, 필요한 돈 ( * siNPCPriceRate )
#define HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM	2		// 고물상을 짓는데, 사용되는 이용권 개수

#define MAX_RENT_COUNT									3		// 임대할 수 있는 최대 수

class cltTaxPaymentListDlg;

//----------------------------------------
// 임대요청 클래스 
//----------------------------------------
class cltRequestRent{
public:
	SI32 siPersonID;		// 임대 희망자의 PersonID
	SI32 siVillageUnique;	// 시전이 위치한 마을의 VillageUnique
	SI32 siSlot;			// 임대할 빈 시전의 슬롯. 

	cltRequestRent(SI32 personid, SI32 villageunique, SI32 slot)
	{
		siPersonID		= personid;
		siVillageUnique	= villageunique;
		siSlot			= slot;
	}

};

//----------------------------------------
// 자신의 창고 정보
//----------------------------------------
class cltMyHouseInfo{
public:
	SI32  m_siIndex;						// 창고 인덱스
	TCHAR m_szName[MAX_PLAYER_NAME];		// 창고 이름
	cltRentContract m_clRentContract;		// 계약 만료일

	cltMyHouseInfo()
	{
		ZeroMemory(this, sizeof(cltMyHouseInfo));
	}

	cltMyHouseInfo(SI32 siIndex , TCHAR* pszName,cltRentContract* pclRentContract)
	{
		m_siIndex = siIndex;
		_tcscpy(m_szName, pszName);
		m_clRentContract.Set(pclRentContract);
	}

	void Set(const cltMyHouseInfo* pclMyHouseInfo)
	{
		m_siIndex = pclMyHouseInfo->m_siIndex;
		_tcscpy(m_szName, pclMyHouseInfo->m_szName);
		m_clRentContract.Set(&pclMyHouseInfo->m_clRentContract);
	}
};

//------------------------------------
// 임차희망자 정보. 
//------------------------------------
class cltRentApplyer{
public:
	cltSimplePerson	clPerson;
	cltRentContract clContract;

	cltRentApplyer(){}
	
	cltRentApplyer(const cltSimplePerson* pclperson, const cltRentContract* pclcontract)
	{
		clPerson.Set(pclperson);
		clContract.Set(pclcontract);
	}

	void Set(const cltRentApplyer* pclapplyer)
	{
		clPerson.Set(&pclapplyer->clPerson);
		clContract.Set(&pclapplyer->clContract);
	}

	void Set(const cltSimplePerson* pclperson, const cltRentContract* pclcontract)
	{
		clPerson.Set(pclperson);
		clContract.Set(pclcontract);
	}

	void Init()
	{
		clPerson.Init();
		clContract.Init();

	}

};


//-------------------------------------
// 임대 계약 내용을 전송하기 위한 패킷. 
//--------------------------------------
class cltRentContractPacket{
public:
	SI32 siVillageUnique;
	cltRentContract clRentContract;

	cltRentContractPacket(SI32 villageunique, const cltRentContract* pclcontract)
	{
		siVillageUnique = villageunique;
		clRentContract.Set(pclcontract);
	}
};


//-----------------------------------------
// 임자 희망자 정보를 보내기 위한 패킷. 
//-----------------------------------------
class cltHouseApplyerPacket{
public:
	SI32			siIndex;
	cltRentApplyer clHouseApplyer;

	cltHouseApplyerPacket(SI32 index, const cltRentApplyer* pclapplyer)
	{
		siIndex = index;
		clHouseApplyer.Set(pclapplyer);
	}
};

//--------------------------------------
// 단위시전의 창고 정보. 
//--------------------------------------
#define MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER	30
class cltHouseUnitStorageInfo{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltItem, MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER>		clItem;
	NSafeTArray<cltHorse, MAX_HOUSEUNIT_HORSENUMBER>			clHorse;
	NSafeTArray<bool, MAX_HOUSEUNIT_HORSENUMBER>				bCreatedHouseUnitStable;

//	NSafeTArray<cltSummon, MAX_HOUSEUNIT_SUMMONNUMBER>			clSummon;
//	NSafeTArray<SI16, MAX_HOUSEUNIT_SUMMONNUMBER>				siHouseUnitSummonBuildingLevel;
#else
	cltItem				clItem[MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER];				// 창고에 저장될 아이템 저장 공간. 
	cltHorse			clHorse[ MAX_HOUSEUNIT_HORSENUMBER ];					// 창고에 들어가 있는 말에 대한 정보
	bool				bCreatedHouseUnitStable[ MAX_HOUSEUNIT_HORSENUMBER ];	// 창고에 마구간이 만들어져 있는지에 대한 정보 ( 마구간이 만들어져 있어야, 말을 넣을 수 있다. )
	
//	cltSummon			clSummon[ MAX_HOUSEUNIT_SUMMONNUMBER ];							// 창고에 들어가 있는 소환수에 대한 정보
//	SI16				siHouseUnitSummonBuildingLevel[ MAX_HOUSEUNIT_SUMMONNUMBER ];	// 소환수 목장 레벨( 초기값 0 )
#endif
	
	bool				bCreatedHouseUnitUsedGoodsBuilding;						// 창고에 고물상이 만들어져 있는지에 대한 정보
	
	
	
	cltHouseUnitStorageInfo(){};
#ifdef _SAFE_MEMORY
	void Set(cltHouseUnitStorageInfo* pclinfo)
#else
	void Set(const cltHouseUnitStorageInfo* pclinfo)
#endif
	{
		SI32 i;

		for( i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
		{
			clItem[i].Set( &pclinfo->clItem[i] );
		}

		for ( i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
		{
			clHorse[i].Set( (cltHorse*)&pclinfo->clHorse[i] );
			bCreatedHouseUnitStable[i] = pclinfo->bCreatedHouseUnitStable[i];
		}

		/*for ( i = 0; i < MAX_HOUSEUNIT_SUMMONNUMBER; ++i )
		{
			clSummon[i].Set( (cltSummon *)&pclinfo->clSummon[i] );
			siHouseUnitSummonBuildingLevel[i] = pclinfo->siHouseUnitSummonBuildingLevel[i];
		}*/
		

		bCreatedHouseUnitUsedGoodsBuilding = pclinfo->bCreatedHouseUnitUsedGoodsBuilding;
	}

	void Init()
	{
		SI32 i;

		for(i = 0;i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER;i++)
		{
			clItem[i].Init();
		}

		for ( i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
		{
			clHorse[ i ].Init();
			bCreatedHouseUnitStable[ i ] = false;
		}

		/*for ( i = 0; i < MAX_HOUSEUNIT_SUMMONNUMBER; ++i )
		{
			clSummon[ i ].Init();			
			siHouseUnitSummonBuildingLevel[ i ] = 0;
		}*/

		bCreatedHouseUnitUsedGoodsBuilding = false;
	}

	BOOL IsItemRemaind() // 시전안에 아이템이 남아있나 체크
	{
		SI32 i;
		for(i = 0; i< MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; i++ )
		{
			if(clItem[i].siUnique > 0)
				return TRUE;
		}
		return FALSE;		
	}
	BOOL IsHorseRemaind()
	{
		SI32 i;
		for ( i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
		{
			if(clHorse[ i ].siHorseUnique > 0)
				return TRUE;
		}
		return FALSE;
	}

	BOOL IsSummonRemaind()
	{
		/*SI32 i;
		for ( i = 0; i < MAX_HOUSEUNIT_SUMMONNUMBER; ++i )
		{
			if(clSummon[ i ].siUnique > 0)
				return TRUE;
		}*/
		return FALSE;
	}
	// 창고에  아이템을 추가할 수 있는지 확인한다. 
	bool CanAddStg(cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos);

	// 아이템을 창고에 넣는다.
	void SetItemStg(SI32 stgpos, cltItem* pclitem)
	{
		clItem[stgpos].Set(pclitem);
	}

	// 창고 아이템의 포인터를 구한다. 
	cltItem* GetStgItemPointer(SI32 stgpos)
	{
		return &clItem[stgpos];
	}
	
	// 창고에 있는 아이템의 위치를 구한다.
	SI16 GetStgExistItemPos( SI16 ItemUnique, SI16 ItemNum );

	// 말을 추가할 수 있는지 확인한다.
	bool CanAddHorse( SI16 *pHorsePos );
	
	// 말이 그 위치에 있는지 체크한다.
	bool IsHorseExist( SI16 HorsePos );

	// 말을 창고에 넣는다.
	bool SetHorseStg( SI16 HorsePos, cltHorse *pclHorse )
	{
		if ( bCreatedHouseUnitStable[ HorsePos ] )
		{
			clHorse[ HorsePos ].Set( pclHorse );
			return true;
		}

		return false;
	}

	// 창고 말의 포인터를 구한다. 
	cltHorse* GetStgHorsePointer( SI32 HorsePos )
	{
		return &clHorse[ HorsePos ];
	}
	
};

// 단위 시전의 한개 아이템 정보. 
#define STGITEMINFO_OPTION_NONE		0	// 아무런 옵션이 없다. 
#define STGITEMINFO_OPTION_SHOW		1	// 화면에 정보를 표시해준다.

class cltHouseUnitStgItemInfo{
public:
	SI16 siVillageUnique;
	SI16 siHouseUnitSlot;
	SI08 siIndex;

	SI16 siOption;
	SI16 siChangedNum;

	SI32 siExp;					// 물품 생산에 따른 경험치.

	cltItem clItem;

	cltHouseUnitStgItemInfo(SI16 villageunique, SI16 houseunitslot, SI08 index, SI16 option, SI16 changednum, SI32 exp, cltItem* pclitem)
	{
		siVillageUnique	= villageunique;
		siHouseUnitSlot	= houseunitslot;
		siIndex			= index;

		siOption		= option;
		siChangedNum	= changednum;

		siExp			= exp;
		clItem.Set(pclitem);

	}
};

//--------------------------------------
// 시전유니트 
//--------------------------------------
class cltHouseUnit {
public:
	TCHAR			m_szName[MAX_PLAYER_NAME];					// 창고 이름
	cltSimplePerson clPerson;										// 소유주. 
	cltRentContract	clContract;										// 체결되어 있는 임대계약의 내용.
	bool	bSell;													// [지연-09.05.25] 판매중인지 체크.

	cltHouseUnitStorageInfo clStorageInfo;							// 창고 정보.	

	cltHouseUnit(){}
	~cltHouseUnit(){}

#ifdef _SAFE_MEMORY
	void Set(cltHouseUnit* punit)
#else
	void Set(const cltHouseUnit* punit)
#endif
	{
		if(punit == NULL)return ;

		clPerson.Set(&punit->clPerson);
		clContract.Set(&punit->clContract);
		clStorageInfo.Set(&punit->clStorageInfo);
		bSell = punit->bSell;
		StringCchCopy(m_szName , MAX_PLAYER_NAME, punit->m_szName);
	}

	void Init()
	{
		clPerson.Init();
		clContract.Init();
		clStorageInfo.Init();
		bSell = false;
	}

	SI32 GetItemNum()
	{
		int ret = 0;

		for( int i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; i++ )
		{
			if( clStorageInfo.clItem[ i ].siUnique != 0 ) ret++;
		}

		return ret;
	}

	SI32 GetStableNum()
	{

		int ret = 0;

		for( int i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
		{
			if( clStorageInfo.bCreatedHouseUnitStable[ i ] == TRUE ) ret++;
		}

		return ret;
	}

	SI32 GetHorseNum()
	{
		int ret = 0;
		for( int i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
		{
			if( clStorageInfo.clHorse[ i ].siHorseUnique ) ret++;
		}

		return ret;
	}

};

//--------------------------------------
// 각 마을별로 존재하는 시전객체 
//--------------------------------------
class cltHouse : public cltStructureBasic{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltHouseUnit, MAX_HOUSEUNIT_NUMBER>		clUnit;
	NSafeTArray<cltRentApplyer, MAX_APPLY_PER_HOUSE>	clApply;
#else
	cltHouseUnit	clUnit[MAX_HOUSEUNIT_NUMBER];

	cltRentApplyer	clApply[MAX_APPLY_PER_HOUSE];
#endif

	cltHouse(SI32 sivillageunique);
	~cltHouse();

	// 시전의 계약조건을 변경한다. 
	BOOL SetRentContract(const cltRentContract* pclcontract )
	{
		clStrInfo.clHouseStrInfo.clRentContract.Set(pclcontract);
		return TRUE;
	}

	void SetHouseUnit(SI32 index, cltHouseUnit* pclunit)
	{
		if(index < 0 || index >= MAX_HOUSEUNIT_NUMBER)
		{
			SI32 a = 0;
			return ;
		}

		clUnit[index].Set(pclunit);
	}

	cltHouseUnit* GetHouseUnit(SI32 index)
	{
		return &clUnit[index];
	}

	void SetHouseApplyer(SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract)
	{
		clApply[index].Set(pclapplyer, pclcontract);
	}
	cltRentApplyer* GetHouseApplyer(SI32 index)
	{
		return &clApply[index];
	}

	// 모든 임차 희망자를 삭제한다.
	void InitHouseApplyer()
	{
		for(SI32 i = 0;i< MAX_APPLY_PER_HOUSE;i++)
		{
			clApply[i].Init();
		}

	}

	//PersonID를 가지고 희망자의 정보를 구한다. 
	cltRentApplyer* GetHouseApplyerFromPersonID(SI32 personid);

	// 같은 계약조건인지 확인한다. 
	BOOL IsSameContract(const cltRentContract* pclcontract);

	// 임차 후보자로 신청한다. 
	BOOL AddApplyer(const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract );

	// 임차 신청자를 삭제한다. 
	BOOL DeleteApplyer(SI32 personid);

	// 빈집인지 여부를 확인한다. 
	BOOL IsEmpty(SI32 slot);

	// 임차인을 시전에 등록한다.
	BOOL SetRent(SI32 slot, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract);

	// 특정 Person이 단위 시전의 주인인지 확인한다. 
	bool IsHouseHost(SI32 personid, SI32 houseunitslot);

	// 창고에  아이템을 추가할 수 있는지 확인한다. 
	bool CanAddStg(SI32 houseunitslot, cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos);

	// 아이템을 창고에 넣는다.
	void SetItemStg(SI32 houseunitslot, SI32 stgpos, cltItem* pclitem);

	// 창고 아이템의 포인터를 구한다. 
	cltItem* GetStgItemPointer(SI32 houseunitslot, SI32 stgpos);

	// 창고의 포인터를 구한다. 
	cltHouseUnitStorageInfo* GetStgPointer(SI32 houseunitslot)
	{
		return &clUnit[houseunitslot].clStorageInfo;
	}

	// 각 시전의 임대료를 징수한다.
	void GetRentFee();

	
};



//--------------------------------------
// 시전 관리자. 
//--------------------------------------
class cltHouseManager : public cltStructureMngBasic{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltHouseUnit, MAX_HOUSEUNIT_NUMBER>		clClientUnit;
	NSafeTArray<cltRentApplyer, MAX_APPLY_PER_HOUSE>	clClientApplyer;
#else
	cltHouseUnit	clClientUnit[MAX_HOUSEUNIT_NUMBER];
	cltRentApplyer	clClientApplyer[MAX_APPLY_PER_HOUSE];
#endif

	BOOL	UpdateRentContractShowSwitch;				// 계약 조건을 화면에 업데이트 하도록 한다. 
	bool	bUpdateHouseUnitListShowChiefSwitch;		// 시전유니트를 화면에 업데이트 하도록 한다. (행수용.) 
	bool	bUpdateHouseUnitListShowSwitch;				// 시전유니트를 화면에 업데이트 하도록 한다. 
	BOOL	UpdateHouseVarySwitch;						// 시전 변수가 업데이트 되도록 한다. 
	BOOL	UpdateHouseApplyerListShowSwitch;			// 임차 희망자 리스트를 화면에 보여줄 수 있도록 한다. 


	SI32 CurrentSelectedHouseUnitSlot;
	SI32 CurrentSelectedApplyerSlot;

	SI32 siSelectedHouseUnitNumber;						// 선택된 단위시전의번호. 

	HIMAGELIST	hImageList;


	cltHouseManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltHouseManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	// 시전을 신설한다.
	SI32 CreateHouse(SI32 siunique);

	// 특정 시전의 계약조건을 변경한다. 
	BOOL SetRentContract(SI32 villageunique, const cltRentContract* pclcontract);
	
	// 특정 마을시전의 특정 시전 슬롯을 얻어온다.
	cltHouseUnit* GetHouseUnit(SI32 villageunique, SI32 houseunitslot );

	// 특정 마을의 시전의 계약조건이 주어진 계약조건과 동일한지 확이한다.
	BOOL IsSameContract(SI32 sivillageunique, const cltRentContract*pclcontract);

	// 임차신청자를 등록한다. 
	BOOL AddApplyer(SI32 sivillageunique, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract );

	// 임차 신청자를 삭제한다. 
	BOOL DeleteApplyer(SI32 sivillageunique, SI32 personid);

	// 선택된 시전 유니트의 정보를 보여준다. 
	BOOL ShowSelectedHouseUnitExplain(HWND hDlg, SI32 index);

	// 빈집인지 여부를 확인한다. 
	BOOL IsEmpty(SI32 sivillageunique, SI32 slot);

	//PersonID를 가지고 희망자의 정보를 구한다. 
	cltRentApplyer* GetHouseApplyerFromPersonID(SI32 sivillageunique, SI32 personid);

	// 임차인을 시전에 등록한다.
	BOOL SetRent(SI32 sivillageunique, SI32 slot, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract, const TCHAR* pszHouseName = NULL);
	
	// 시전을 회수한다. 
	void RecallHouse(SI32 villageunique, SI32 houseindex);
	
	// 특정 Person이 단위 시전의 주인인지 확인한다. 
	bool IsHouseHost(SI32 personid, SI32 villageunique, SI32 houseunitslot);

	// 창고에  아이템을 추가할 수 있는지 확인한다. 
	bool CanAddStg(SI32 villageunique, SI32 houseunitslot, cltItemManagerCommon* pclItemManager, cltItem* pclitem, SI32* pitempos);

	// 아이템을 창고에 넣는다.
	void SetItemStg(SI32 villageunique, SI32 houseunitslot, SI32 stgpos, cltItem* pclitem);
	
	// 창고 아이템의 포인터를 구한다. 
	cltItem* GetStgItemPointer(SI32 villageunique, SI32 houseunitslot, SI32 stgpos);

	// 창고의 포인터를 구한다. 
	cltHouseUnitStorageInfo* GetStgPointer(SI32 villageunique, SI32 houseunitslot);
	
	// 임대료를 처리하는등의 액션을 취한다. 
	void Action(cltTime* pcltime, clock_t CurrentClock);
	
	// 게임서버로 단위시전의 리스트를 요청한다. 
	void RequestHouseUnitList();
	// 게임서버로 단위시전의 상세 정보 리스트를 요청한다. 
	void RequestHouseUnitListDetail();
	
	// 게임서버로 임차희망자 리스트를 요청한다. 
	void RequestHouseRentApplyerList();

	// 서버 리턴값 처리 함수. 
	void ServerRtnValOperate();


	// 계약 조건을 보여준다. 
	BOOL ShowRentContract(HWND hDlg);
	// 시전 유니트 정보를 보여준다. 
	BOOL ShowHouseUnitListInChief(HWND hDlg);
	// 시전 임차 희망자의 리스트를 보여준다. 
	BOOL ShowHouseApplyerListInChief(HWND hDlg);

	//계약 조건을 보여준다. 
	BOOL ShowRentContractInUser(HWND hDlg);

	// 시전리스트를 만든다.
	BOOL MakeHouseUnitList(HWND hDlg);

	// 시전리스트를 보여준다. 
	BOOL ShowHouseUnitList(HWND hDlg);

};

#endif