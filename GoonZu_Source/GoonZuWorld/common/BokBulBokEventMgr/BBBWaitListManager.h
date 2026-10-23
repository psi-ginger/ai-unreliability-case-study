//===================================================================================================
// 제목	 : 복불복 미니게임
// 스위치: Switch_BokBulBok
// 내용	 : 복불복 예약리스트를 관리할 메니져.
// 작성일: 2009.08.10
// 작성자: 오지연
//===================================================================================================

#ifndef _BBBWAITLIST_MANANGER_H_
#define _BBBWAITLIST_MANANGER_H_


#include "../../Common/CommonHeader.h"
#include <list>





#define MAX_RESERVATION_NUM	30	// 최대 예약가능 수
#define MAX_CHARACTER_NAME_LENGTH	20		// 캐릭터 이름 최대길이



struct _BBBWaitUserData
{
	SI32 siUserID;
	TCHAR szUserName[MAX_CHARACTER_NAME_LENGTH];
	//SI32 siPersionID;

	// 기타 예약리스트에 추가된 시간 등을 저장할수도..

};


class CBBBWaitListManager
{
public:
	CBBBWaitListManager(void);
	~CBBBWaitListManager(void);

	// 대기리스트 관리
protected:
	typedef std::list< _BBBWaitUserData > WAITLIST;

	WAITLIST m_WaitList;										// 대기자리스트
	WAITLIST::iterator m_WLitr;									// 대기리스트에서 쓸 이터레이터

	DWORD m_dwWaitListRefreshTime;								// 다음 대기리스트 갱신 시간

public:
	//SI16 GetWaitLength();										// 현재 대기중인 사람의 수 리턴
	bool IsFull();												// 대기열이 꽉찼는지 체크 (30명 풀인지..)

    bool AddUser(SI32 siUserID, TCHAR* pszUserName = NULL);		// 예약리스트에 추가
	void RemoveUser(SI32 siUserID);								// 예약리스트에서 삭제

	void GetWaitList(_BBBWaitUserData* pWaitList);				// 대기자 리스트를 얻는다.
	void SubAction();											// 매 순환 루프마다 대기매니저가 해야 할일 처리

	bool WaitUserExist();										// 대기리스트에 대기중인 유저가 있는지 확인
	void Send_EnterMapQuestion();								// 1순위 대기자에게 맵에 진입할거냐고 패킷 보냄
	SI32 Get1stUserID();

public:
	bool _AlreadyExist(TCHAR* pszUserName);						// 대기리스트에 이미 존재하는 유저인지 확인
	void _Refresh_UserList();									// 대기자 리스트에 유효하지 않은 유저를 찾아서 제거
};

#endif