#ifndef _MINE_H
#define _MINE_H

#include <Directives.h>

#include "..\..\Common\CommonHeader.h"
#include "..\StructureClass\StructureClass.h"
#include "..\..\Server\Time\MyTime.h"

#include "..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "../../commonlogic/house/house.h"

#define MAX_MINE_TAB_NUMBER								5
#define MAX_MINEUNIT_NUMBER								50		// 각 마을별 광산의 개수
#define MAX_APPLY_PER_MINE								50		// 광산 임차 신청인의 수. (한 마을당. ) 
#define MAX_RENT_PERIOD_YEAR							2		// 최대 임대 가능 년수.

#define LEVELLIMIT_USE_MINE								30		// 광산 사용을 가능하게 하는 최소 레벨.

#define MIN_MINE_RENT_FEE								10000	// 광산 임대료 최저가격. 	
#define MAX_MINE_RENT_FEE								200000	// 광산 임대료 최고 가격. 

class cltTaxPaymentListDlg;

//--------------------------------------
// 광산 유니트 
//--------------------------------------
class cltMineUnit {
public:
	cltSimplePerson clPerson;									// 소유주. 
	cltRentContract	clContract;									// 체결되어 있는 임대계약의 내용. 

	cltMineUnit() { Init(); }
	~cltMineUnit() { }

	void Set(const cltMineUnit* punit)
	{
		if( punit == NULL )return ;

		clPerson.Set(&punit->clPerson);
		clContract.Set(&punit->clContract);
	}

	void Init()
	{
		clPerson.Init();
		clContract.Init();
	}

};

//--------------------------------------
// 각 마을별로 존재하는 광산 객체 
//--------------------------------------
class cltMine : public cltStructureBasic{
public:
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltMineUnit, MAX_MINEUNIT_NUMBER>		clUnit;
	NSafeTArray<cltRentApplyer, MAX_APPLY_PER_MINE>		clApplyer;
#else
	cltMineUnit		clUnit[ MAX_MINEUNIT_NUMBER ];
	cltRentApplyer	clApplyer[ MAX_APPLY_PER_MINE ];
#endif

	cltMine(SI32 sivillageunique);
	~cltMine();


	// 광산의 계약조건을 변경한다. 
	BOOL SetRentContract(const cltRentContract* pclcontract )
	{
		clStrInfo.clMineStrInfo.clRentContract.Set( pclcontract );
		return TRUE;
	}

	// 광산 유닛을 설정한다 
	void SetMineUnit( SI32 index, cltMineUnit* pclunit )
	{
		clUnit[ index ].Set(pclunit);
	}

	// 광산 유닛을 얻어온다 
	cltMineUnit* GetMineUnit( SI32 index )
	{
		return &clUnit[ index ];
	}

	// 임차 신청자를 설정한다 
	void SetRentApplyer( SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract )
	{
		clApplyer[ index ].Set( pclapplyer, pclcontract );
	}

	// 임차 신청자를 얻어온다 
	cltRentApplyer* GetRentApplyer(SI32 index)
	{
		return &clApplyer[ index ];
	}

	// 모든 임차 신청자를 삭제한다.
	void InitRentApplyer()
	{
		for( SI32 i = 0; i < MAX_APPLY_PER_MINE; i++ )
		{
			clApplyer[i].Init();
		}
	}

	// 모든 광산정보를 초기화한다.
	void InitMineUnits()
	{
		for( int i = 0; i < MAX_MINEUNIT_NUMBER; ++i ) {
			clUnit[ i ].Init();
		}
	}

	//PersonID를 가지고 희망자의 정보를 구한다. 
	cltRentApplyer* GetMineApplyerFromPersonID(SI32 personid);

	// 같은 계약조건인지 확인한다. 
	BOOL IsSameContract(const cltRentContract* pclcontract);

	// 임차 신청한다. 
	BOOL AddApplyer(const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract );

	// 임차 신청자를 삭제한다. 
	BOOL DeleteApplyer(SI32 personid);

	// 빈집인지 여부를 확인한다. 
	BOOL IsEmpty(SI32 index);

	// 임차인을 광산에 등록한다.
	BOOL SetRent(SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract);

	// 특정 Person이 단위 광산의 주인인지 확인한다. 
	bool IsMineHost(SI32 personid, SI32 Mineunitslot);

	// 각 광산의 임대료를 징수한다.
	void GetRentFee();

};



//--------------------------------------
// 광산 관리자. 
//--------------------------------------
class cltMineManager : public cltStructureMngBasic{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltMineUnit, MAX_MINEUNIT_NUMBER>	clClientUnit;
	NSafeTArray<cltRentApplyer, MAX_APPLY_PER_MINE>	clClientApplyer;
#else
	cltMineUnit		clClientUnit[ MAX_MINEUNIT_NUMBER ];
	cltRentApplyer	clClientApplyer[ MAX_APPLY_PER_MINE ];
#endif

	BOOL	UpdateRentContractShowSwitch;				// 계약 조건을 화면에 업데이트 하도록 한다. 
	bool	bUpdateMineUnitListShowChiefSwitch;			// 유니트를 화면에 업데이트 하도록 한다. (행수용.) 
	bool	bUpdateMineUnitListShowSwitch;				// 광산유니트를 화면에 업데이트 하도록 한다. 
	BOOL	UpdateMineApplyerListShowSwitch;			// 임차 희망자 리스트를 화면에 보여줄 수 있도록 한다. 
	BOOL	UpdateMineVarySwitch;						// 광산 변수가 업데이트 되도록 한다. 


	SI32 CurrentSelectedMineUnitSlot;
	SI32 CurrentSelectedApplyerSlot;
	SI32 siSelectedMineUnitNumber;						// 선택된 단위광산의번호. 

	HIMAGELIST	hImageList;


	cltMineManager( cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltMineManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	void Action( cltTime* pcltime, clock_t CurrentClock );

	// 광산을 신설한다.
	SI32 CreateMine(SI32 siunique);


	//계약 조건을 보여준다. 
	BOOL ShowRentContractInUser(HWND hDlg);

	// 광산리스트를 만든다.
	BOOL MakeMineUnitList(HWND hDlg);

	// 광산리스트를 보여준다. 
	BOOL ShowMineUnitList(HWND hDlg);



	// 게임서버로 단위광산의 리스트를 요청한다. 
	void RequestMineUnitList();
	// 게임서버로 단위광산의 상세 정보 리스트를 요청한다. 
	void RequestMineUnitListDetail();	
	// 게임서버로 임차희망자 리스트를 요청한다. 
	void RequestMineRentApplyerList();

	// 계약 조건을 보여준다. 
	BOOL ShowRentContract(HWND hDlg);
	// 광산 유니트 정보를 보여준다. 
	BOOL ShowMineUnitListInChief(HWND hDlg);
	// 광산 임차 희망자의 리스트를 보여준다. 
	BOOL ShowMineApplyerListInChief(HWND hDlg);
	// 선택된 시전 유니트의 정보를 보여준다. 
	BOOL ShowSelectedMineUnitExplain(HWND hDlg, SI32 index);

	// 특정 사용자가 마을에 광산을 가지고 있는지 여부르 알려준다 
	BOOL IsMiner( SI32 villageunique, SI32 personid );


};

#endif