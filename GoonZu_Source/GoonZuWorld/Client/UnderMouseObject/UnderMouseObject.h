#ifndef _UNDERMOUSEOBJECT_H
#define _UNDERMOUSEOBJECT_H

#include <Directives.h>

class cltCharManager;
class cltIDManager;
class cltMapClient;
class cltClient;

class CDrawSequence;

//-------------------------------------------
// 마우스로 선택한 객체의 정보.
//-------------------------------------------
class cltMouseObject{
public:
	int X, Y;
	SI32 ID;

	cltMouseObject()
	{
		Init();
	}

	void Init()
	{
		X	= 0;
		Y	= 0;
		ID	= 0;
	}

	void Set(SI32 x, SI32 y, SI32 id)
	{
		X	= x;
		Y	= y;
		ID	= id;
	}
};

//------------------------------------------
// 마우스 아래 객체를 처리하는 클래스. 
//------------------------------------------
class cltUnderMouseObject{
public:
	cltClient*		pclGame;
	cltCharManager* pclCM;

	// 캐릭터인 경우, 
	SI32 ID;					// 마우스 포인터 아래에 있는 캐릭터. 
	SI32 siUnderMouseLockChar;	// 마우스 포인터 락 캐릭터. 

	SI32 siUnderUserCnt;

	// 건물인 경우, 
	SI32 siStructureUnique;		// 건물의 종류. 
	// NPC인 경우, 
	SI32 siNPCUnique;			// NPC인 경우, 
	// Gate 인 경우, 
	SI32 siGateUnique;
	// 마을인 경우, 
	SI32 siVillageUnique;
	// 책상인 경우, 
	SI32 siDeskUnique;

	// 마우스 밑의 타일 상황. 
	SI32 siTileAtb;		

	// 아이템인 경우, 
	SI32 siItemUnique;
	SI32 siItemIndex;
	SI32 siItemX, siItemY;


	SI32 siRX, siRY;			// 도트 단위의 화면 출력 변위.

	DWORD NewIDSelectedClock;	// 새로운 캐릭터를 선택한 시간. 

	CDrawSequence* pclDrawSequence;	// 마우스 밑의 캐릭터를 찾을때 사용하는 순서 저장

public:
	SI32 MapX, MapY;                    // 마우스가 위치한 Tile 좌표 

	cltUnderMouseObject(cltClient* pclgame, cltCharManager* pclcm)
	{
		pclGame	= pclgame;
		pclCM	=  pclcm;
		

		Init();
	}

	~cltUnderMouseObject();

	void Init();
	 
	// 지상에서 캐릭터를 찾는다. 
	BOOL FindLandChar(cltMapClient* pclMap, OUT SI32 * newid, OUT SI32 * underUserCnt );
	// 건축물을 찾는다. 
	SI32 FindStructure(cltMapClient* pclMap);
	// Gate를 찾는다. 
	SI32 FindGate(cltMapClient* pclMap);
	// 마을을  찾는다. 
	SI32 FindVillage(cltMapClient* pclMap);
	
	// FieldObject를 찾는다. 찾으면 그 FOUnique를 리턴한다. 
	SI32 FindFieldObject(cltMapClient* pclMap);


	// 아이템을 찾는다. 찾으면 true 리턴. 
	bool FindItem(cltMapClient* pclMap, SI32* pitemunique, SI32* pitemindex);


	// 마우스 아래 객체를 파악한다. 
	void GetUnderMouseObject(cltMapClient* pclMap, SI32 screenmousex, SI32 screenmousey);

	SI32 GetUnderMouseID()const{return ID;}
	void SetUnderMouseID(SI32 id){ID = id;}
   
	SI32 GetUnderUserCnt()const{return siUnderUserCnt;}
	void SetUnderUserCnt(SI32 cnt){ siUnderUserCnt  = cnt;}

	SI32 GetStructureUnique()const{return siStructureUnique;}
	SI32 GetNPCUnique()const{return siNPCUnique;}

	SI32 GetGateUnique()const{return siGateUnique;}

	SI32 GetVillageUnique()const{return siVillageUnique;}
	
	SI32 GetItemUnique()const{return siItemUnique;}
	SI32 GetItemIndex()const{ return siItemIndex;}
	SI32 GetItemX()const{return siItemX;}
	SI32 GetItemY()const{return siItemY;}
	
	void SetRX(SI32 rx){siRX = rx;}
	SI32 GetRX()const{return siRX;}

	void SetRY(SI32 ry){siRY = ry;}
	SI32 GetRY()const{return siRY;}
	
};



#endif