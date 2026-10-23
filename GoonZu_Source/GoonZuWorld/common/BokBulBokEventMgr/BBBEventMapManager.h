//===================================================================================================
// 제목	 : 복불복 미니게임
// 스위치: Switch_BokBulBok
// 내용	 : 복불복 맵을 관리할 메니져.
// 작성일: 2009.08.10
// 작성자: 오지연
//===================================================================================================

#ifndef _BBBEVENTMAP_MANAGER_H_
#define _BBBEVENTMAP_MANAGER_H_


#include "../../Common/CommonHeader.h"
#include "BBBEventMap.h"
#include <vector>

#define MAX_MAP_COUNT		4
//#define MAX_MAP_COUNT		1	 //test


class CBBBEventMapManager
{
public:
	CBBBEventMapManager(void);
	~CBBBEventMapManager(void);

	// 외부 인터페이스
public:
    bool IsFull();
	SI16 AddUser(SI32 siPersonID, TCHAR* pszCharName);
	void RemoveUser(SI32 siPersonID);
	void SubAction();

	void StageFail(SI32 siPersonID);
	void StageClear(SI32 siPersonID);

	void SetTemporaryReserve(SI32 siPersonID);
	void ActiveTemporaryReserve(SI32 siPersonID);
	void CancelTemporaryReserve(SI32 siPersonID);

	void SetPlayTime(SI32 siPlayTime);

	// 내부 멤버
protected:
	typedef std::vector<CBBBEventMap*> MAPLIST;

	MAPLIST m_MapList;
	MAPLIST::iterator m_MLitr;
};

#endif