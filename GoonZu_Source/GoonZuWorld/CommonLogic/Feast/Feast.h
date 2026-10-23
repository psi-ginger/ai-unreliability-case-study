#ifndef _FEAST_H
#define _FEAST_H

#include <Directives.h>

#include "..\..\Common\CommonHeader.h"
#include "..\StructureClass\StructureClass.h"
#include "..\..\Server\Time\MyTime.h"

#include "..\..\Common\Item\ItemCommon\ItemUnit.h"

#define MAX_FEAST_TAB_NUMBER								7

class cltTaxPaymentListDlg;

//--------------------------------------
// 각 마을별로 존재하는 내자시 객체 
//--------------------------------------
class cltFeast : public cltStructureBasic{
public:
	cltFeast(SI32 sivillageunique);
	~cltFeast();
};

#define MAX_PARTYHALL_MAP									5		//[진성] 파티홀. 예약 가능한 맵의 개수. => 2008. 3. 10
#define MAX_PARTYHALL_RESERVATION_INFO_LIST					24		//[진성] 파티홀. 맵당 최대 예약 가능한 개수. => 2008. 3. 10
#define	PARTYHALL_START_MAP_INDEX							335		//[진성] 파티홀. 파티장 맵의 시작 인덱스번호. 
//--------------------------------------
// 내자시 관리자. 
//--------------------------------------
class cltFeastManager : public cltStructureMngBasic{
public:
	enum {

		MAX_FEAST_EFFECT_NUM = 11, // 조합 제외
	};



	cltFeastManager( cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName );
	~cltFeastManager();

	// 최초 환영 인사말표시 . 
	void GreetingMsg(HWND hDlg);
	void Do_WM_INITDIALOG(SI32 charunique);

	// 내자시을 신설한다.
	SI32 CreateFeast(SI32 siunique);

	void Action();

	void PartyhallEnd();										// [진성] 파티홀. 파티가 종료되면 리스트에서 삭제하자.
 	
	SI32 GetReveIndex( SI32 _PartyHallIndex, SI32 _PersonID );	// 해당 파티장에 예약된 위치를 얻는다.
	void GiveReward(void);										// 파티장에 장식물이 새워진 곳의 유저에게 조금만 보상.
	void GiveItem(void);										// 아이템 나눠주기.
	bool IsPartyHallMap( SI32 MapIndex );

public:
	BOOL						IsMyInventoryChanged();
	BOOL						IsStgChanged();
	
	HIMAGELIST					m_hImageList;

	cltItem						*m_pclMyItem;				// 내 인벤토리 체크
	cltFeastStg					*m_pclStgItem;
	
	HWND						m_hUserInventory;
	HWND						m_hFeastStg;
	
	SI32						m_siInventorySelectedIndex;
	SI32						m_siStgSelectedIndex;
/*
	void						ShowFeastInfo( HWND hDlg );
	void						ShowPresentInfo( HWND hDlg );
	void						ShowEffectInfo( HWND hDlg );	// 이펙트 리스트에 이펙트 추가

	void						SetDateCombo( HWND hDlg );

	void						ShowPresentList( HWND hDlg );
*/
	SI32						siChiefPresentSelectIndex;
	SI32						siUserPresentSelectIndex;

	SI16						siChiefEffectSelectIndex;	// 연회 중 선택한 이펙트 번호

	BOOL						bUpdateUser;
	BOOL						bUpdateChief2;

#ifdef _SAFE_MEMORY
	NSafeTArray<PartyHall_Decoration, MAX_PARTYHALL_DECORATION_ITEM>		clDecoration;

	NSafeTArray< NSafeTArray<cltPatyHallResvInfo, MAX_PARTYHALL_RESERVATION_INFO_LIST>, MAX_PARTYHALL_MAP>		clPartyHallResvInfo;

	NSafeTArray<SI32, MAX_PARTYHALL_MAP>	m_siGatePositionID;
	NSafeTArray<POINT, MAX_PARTYHALL_MAP>	m_DecorationPos;
	NSafeTArray<POINT, MAX_PARTYHALL_MAP>	m_PartyHallUserPos;
#else
	PartyHall_Decoration		clDecoration[MAX_PARTYHALL_DECORATION_ITEM];	// 장식물.

	//[진성] 파티홀. 예약정보. => 2008. 3. 13
	cltPatyHallResvInfo			clPartyHallResvInfo[MAX_PARTYHALL_MAP][MAX_PARTYHALL_RESERVATION_INFO_LIST];

	//[진성] 파티홀. 장식물의 아이디 저장. 제거와 주변 캐릭에게 영향을 줄 때 참조. => 2008-4-6
	SI32						m_siGatePositionID[MAX_PARTYHALL_MAP];
	POINT						m_DecorationPos[MAX_PARTYHALL_MAP];			// 장식물 출현 위치.
	POINT						m_PartyHallUserPos[MAX_PARTYHALL_MAP];		// 유저 입장 위치.
#endif
};

#endif