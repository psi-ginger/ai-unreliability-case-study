//---------------------------------
// 2004/2/18 김태곤
//---------------------------------

#ifndef _HUNT_H
#define _HUNT_H

#include <Directives.h>

#include <Stdio.h>
#include "..\StructureClass\StructureClass.h"
#include "..\..\Common\Item\ItemCommon\cltitem.h"

#define	MAX_HUNT_TAB_NUMBER			4

class cltTaxPaymentListDlg;

//-----------------------------------------------
// 개별적인 수렵장 
//-----------------------------------------------
class cltHunt : public cltStructureBasic{
public:
	DWORD dwLastGiveExpClock;
	DWORD dwLastCheckClock;	
	SI32  siLeftMonNum;
	SI32  siLeftUserNum;

	clock_t m_PreClock;

	cltHunt(SI32 sivillageunique);
	~cltHunt();
	
	// 사용자를 내 쫓는다. 
	void GoOutUser();

	// 몬스터를 배치한다. 
	bool SetMonster();

	// 수렵장에 남은 몬스터 정보를 확보한다.
	void CheckLeftMonNum(SI16* pdata);
	void AddLeftMon(SI32 kind, SI16* pdata);

	
	void Action();

};

//-----------------------------------------------
// 수렵장  관리자. 
//-----------------------------------------------
class cltHuntManager : public cltStructureMngBasic{
public:

	SI32 siHuntAddRate;			// 일반 던전과의 비교를 통한 수렵장의 어드벤티지 확률. 

	cltHuntManager(cltCharManager* pclCM, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltHuntManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	// 수렵장을 신설한다.
	BOOL CreateHunt(SI32 siunique);
	

	// 수렵장의 몬스터를 모두 제거하고 다시 만든다. 
	bool SetNewMonster(SI32 villageunique);

	void Action();

	bool IsMyInventoryChanged();
	void LoadMyInventory();

	bool IsHuntStorageInventoryChanged();
	void LoadHuntStorageInventory();

	void LoadLineUpInfoList();	
	
	// 수렵장 창고의 재고를 소모시킨다. 
	void WasteStgDoll(SI32 villageunique);

	// 수렵장 이용 성공으로 포상을 지급한다. 
	void Reward(SI32 VillageUnique, SI16 rewardid, GMONEY money);
	
	// 수렵장 User 다이얼로그에서 몬스터 배치 정보를 읽어온다.
	void LoadHuntUserDlgLineUpInfo( HWND hDlg );

	bool IsHuntLineUpMonsterReady( SI32 VillageUnique );
		
	void RecoverDollItem( SI32 VillageUnique, SI16 *pItemDataList );// 수렵장에서 빠져 나갈시에 남은 몬스터를 알 아이템으로 다시 변환해줌

public:
	cltItem			*m_pclMyItem;				// 내 인벤토리 체크
	cltHuntStg		*m_pclHuntStg;

	SI16			m_siInventorySelectedIndex;
	SI16			m_siHuntStorageSelectedIndex;

	BOOL			bUpdateLineUpInfo;

	HWND			m_hHuntUserDlg;

	HWND			m_hHuntChiefDlg;
	HWND			m_hUserInventory;
	HWND			m_hHuntStorage;

	HIMAGELIST		m_hImageList;

	HWND			m_hMonsterComboList;
	HWND			m_hMonsterLevelEdit;
	HWND			m_hMonsterNumEdit;

	HWND			m_hHuntLineUpInfoList;


	TCHAR			m_strListSelectedMonsterName[ 64 ];

	// 수렵장 지도
	SI32			m_siHuntMapIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_HUNT_MAP_NUMBER>		m_siHuntMapUnique;
#else
	SI32			m_siHuntMapUnique[ MAX_HUNT_MAP_NUMBER ];
#endif
	SI32			m_siHuntMapNumber;

};
#endif
