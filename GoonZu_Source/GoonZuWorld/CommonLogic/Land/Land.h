#ifndef _LAND_H
#define _LAND_H

#include <Directives.h>

#include "..\..\Common\CommonHeader.h"
#include "..\StructureClass\StructureClass.h"
#include "..\..\Server\Time\MyTime.h"

#include "..\..\Common\Item\ItemCommon\ItemUnit.h"
#include "../../commonlogic/house/house.h"

#define MAX_LAND_TAB_NUMBER								5
#define MAX_LANDUNIT_NUMBER								50		// 각 마을별 농장의 개수
#define MAX_APPLY_PER_LAND								50		// 농장 임차 신청인의 수. (한 마을당. ) 
#define MAX_RENT_PERIOD_YEAR							2		// 최대 임대 가능 년수.

#define LEVELLIMIT_USE_LAND								30		// 농장 사용을 가능하게 하는 최소 레벨.

#define MIN_LAND_RENT_FEE								5000	// 농장 임대료 최저가격. 	
#define MAX_LAND_RENT_FEE								200000	// 농장 임대료 최고 가격. 

class cltTaxPaymentListDlg;

//--------------------------------------
// 농장 유니트 
//--------------------------------------
class cltLandUnit {
public:
	cltSimplePerson clPerson;										// 소유주. 
	cltRentContract	clContract;										// 체결되어 있는 임대계약의 내용. 

	cltLandUnit() { Init(); }
	~cltLandUnit() { }

	void Set(const cltLandUnit* punit)
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
// 각 마을별로 존재하는 농장 객체 
//--------------------------------------
class cltLand : public cltStructureBasic{
public:
	
#ifdef _SAFE_MEMORY
	NSafeTArray<cltLandUnit, MAX_LANDUNIT_NUMBER>	clUnit;
	NSafeTArray<cltRentApplyer, MAX_APPLY_PER_LAND>	clApplyer;
#else
	cltLandUnit		clUnit[ MAX_LANDUNIT_NUMBER ];
	cltRentApplyer	clApplyer[ MAX_APPLY_PER_LAND ];
#endif

	cltLand(SI32 sivillageunique);
	~cltLand();


	// 농장의 계약조건을 변경한다. 
	BOOL SetRentContract(const cltRentContract* pclcontract )
	{
		clStrInfo.clLandStrInfo.clRentContract.Set( pclcontract );
		return TRUE;
	}

	// 농장 유닛을 설정한다 
	void SetLandUnit( SI32 index, cltLandUnit* pclunit )
	{
		clUnit[ index ].Set(pclunit);
	}

	// 농장 유닛을 얻어온다 
	cltLandUnit* GetLandUnit( SI32 index )
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
		for( SI32 i = 0; i < MAX_APPLY_PER_LAND; i++ )
		{
			clApplyer[i].Init();
		}

	}

	// 모든 농장 유닛을 초기화한다.
	void InitLandUnits()
	{
		for( SI32 i = 0; i < MAX_LANDUNIT_NUMBER; i++ )
		{
			clUnit[i].Init();
		}

	}

	//PersonID를 가지고 희망자의 정보를 구한다. 
	cltRentApplyer* GetLandApplyerFromPersonID(SI32 personid);

	// 같은 계약조건인지 확인한다. 
	BOOL IsSameContract(const cltRentContract* pclcontract);

	// 임차 신청한다. 
	BOOL AddApplyer(const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract );

	// 임차 신청자를 삭제한다. 
	BOOL DeleteApplyer(SI32 personid);

	// 빈집인지 여부를 확인한다. 
	BOOL IsEmpty(SI32 index);

	// 임차인을 농장에 등록한다.
	BOOL SetRent(SI32 index, const cltSimplePerson* pclapplyer, const cltRentContract* pclcontract);

	// 특정 Person이 단위 농장의 주인인지 확인한다. 
	bool IsLandHost(SI32 personid, SI32 landunitslot);

	// 각 농장의 임대료를 징수한다.
	void GetRentFee();

};



//--------------------------------------
// 농장 관리자. 
//--------------------------------------
class cltLandManager : public cltStructureMngBasic{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltLandUnit, MAX_LANDUNIT_NUMBER>		clClientUnit;
	NSafeTArray<cltRentApplyer, MAX_APPLY_PER_LAND>		clClientApplyer;
#else
	cltLandUnit		clClientUnit[ MAX_LANDUNIT_NUMBER ];
	cltRentApplyer	clClientApplyer[ MAX_APPLY_PER_LAND ];
#endif

	BOOL	UpdateRentContractShowSwitch;				// 계약 조건을 화면에 업데이트 하도록 한다. 
	bool	bUpdateLandUnitListShowChiefSwitch;			// 유니트를 화면에 업데이트 하도록 한다. (행수용.) 
	bool	bUpdateLandUnitListShowSwitch;				// 농장유니트를 화면에 업데이트 하도록 한다. 
	BOOL	UpdateLandApplyerListShowSwitch;			// 임차 희망자 리스트를 화면에 보여줄 수 있도록 한다. 
	BOOL	UpdateLandVarySwitch;						// 농장 변수가 업데이트 되도록 한다. 


	SI32 CurrentSelectedLandUnitSlot;
	SI32 CurrentSelectedApplyerSlot;
	SI32 siSelectedLandUnitNumber;						// 선택된 단위농장의번호. 

	HIMAGELIST	hImageList;


	cltLandManager( cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltLandManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	void Action( cltTime* pcltime, clock_t CurrentClock );

	// 농장을 신설한다.
	SI32 CreateLand(SI32 siunique);


	//계약 조건을 보여준다. 
	BOOL ShowRentContractInUser(HWND hDlg);

	// 농장리스트를 만든다.
	BOOL MakeLandUnitList(HWND hDlg);

	// 농장리스트를 보여준다. 
	BOOL ShowLandUnitList(HWND hDlg);



	// 게임서버로 단위농장의 리스트를 요청한다. 
	void RequestLandUnitList();
	// 게임서버로 단위농장의 상세 정보 리스트를 요청한다. 
	void RequestLandUnitListDetail();	
	// 게임서버로 임차희망자 리스트를 요청한다. 
	void RequestLandRentApplyerList();

	// 계약 조건을 보여준다. 
	BOOL ShowRentContract(HWND hDlg);
	// 농장 유니트 정보를 보여준다. 
	BOOL ShowLandUnitListInChief(HWND hDlg);
	// 농장 임차 희망자의 리스트를 보여준다. 
	BOOL ShowLandApplyerListInChief(HWND hDlg);
	// 선택된 시전 유니트의 정보를 보여준다. 
	BOOL ShowSelectedLandUnitExplain(HWND hDlg, SI32 index);

	// 특정 사용자가 마을에 농장을 가지고 있는지 여부르 알려준다 
	BOOL IsFarmer( SI32 villageunique, SI32 personid );


};

#endif