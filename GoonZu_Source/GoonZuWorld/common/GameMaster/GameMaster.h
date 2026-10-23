//---------------------------------
// 2004/5/18 김태곤
//---------------------------------
#ifndef _GAMEMASTER_H
#define _GAMEMASTER_H

#include <Directives.h>
#include "../Item/ItemCommon/ItemUnit.h"
#include "../NLib/NUpdateTimer.h"

#define MAX_GMINFO_NUMBER			200
#define MAX_GM_ORDER_NAME_LENGTH	256
#define MAX_GM_EXPLAIN_LENGTH		128

#define GM_ATB_NONE							0x00000000
#define GM_ATB_CHARUNIQUE					0x00000001		
#define GM_ATB_MONSTER						0x00000002		
#define GM_ATB_ITEM							0x00000004		
#define GM_ATB_SKILL						0x00000010  	
#define GM_ATB_VILLAGE						0x00000020  	
#define GM_ATB_HUNTMAP						0x00000040		
#define GM_ATB_ETC							0x00000080

class cltGameMasterInfo{
public:
	
	SI32 siUnique;
	SI32 siNeedGMLevel;
	TCHAR szOrder[MAX_GM_ORDER_NAME_LENGTH];
	TCHAR* pExplain;
	TCHAR* pSampleUse;

	SI32 mGMatb;

	cltGameMasterInfo(SI32 unique, SI32 gmlevel, TCHAR* porder, TCHAR* pexplain, TCHAR* psampleuse, SI32 aAtb);
	~cltGameMasterInfo();

};

class cltGameMasterManager{

private :
	SI32	m_siGMInfoCount;			// 운영자 명령 갯수

	cltItem	m_clSupplyItem;		// 매 시간마다 유저에게 배부하는 아이템 유니크
	TCHAR	m_szSupplyMsg[256];				// 매 시간마다 유저에게 아이템 배부하며 전해지는 메시지

	//KHY -0702- 접속중인 모든 유저들에게, 예약한 시간에 아이템을 제공한다. 
	SI32 time_hour;
	SI32 time_min;
	SI32 itemunique;
	SI32 num;
	UI16 usedate;

	NUpdateTimer kUpdateTimer;
	
public:
	bool timeSwitch;
	///////////////////////////////////////////

#ifdef _SAFE_MEMORY
	NSafeTArray<cltGameMasterInfo*, MAX_GMINFO_NUMBER>		pclGMInfo;
#else
	cltGameMasterInfo* pclGMInfo[MAX_GMINFO_NUMBER];
#endif

	cltGameMasterManager();
	~cltGameMasterManager();

	// 운영자 권한을 추가한다. 
	void Add(SI32 gmlevel, TCHAR* porder, TCHAR* pexplain, TCHAR* psampleuse, SI32 aAtb);

	// 명령을 근거로 유니크를 찾는다. 
	SI32 FindUniqueFromOrder(TCHAR* porder);

	// 운영자 명령에 대한 전제 설명을 얻어온다 (2K의 텍스트 사이즈를 확보해서 넘겨야 한다) 
	bool GetText(TCHAR* porder, TCHAR* ptext, SI16 txtSize);

	bool SetSupplyItem(IN const cltItem * pclitem);
	bool GetSupplyItem(OUT cltItem * pclitem);
	void SetSupplyMsg(const TCHAR * msg);
	TCHAR * GetSupplyMsg();

	//KHY -0702- 접속중인 모든 유저들에게, 예약한 시간에 아이템을 제공한다. 
	void giveItemTime(UI32 time_hour,UI32 time_min,SI32 itemunique,SI32 num,UI16 usedate );
	void Action( );
	
};
#endif


