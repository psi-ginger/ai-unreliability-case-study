#ifndef _KINGGURI_H_
#define _KINGGURI_H_

#include "../../../common/Event/event.h"

const SI32 MAX_KINGGURI_NUM = 3;
const SI32 MAX_POSITION_NUM = 11;
const SI32 MAX_MESSAGE_SIZE = 256;

const WORD KingGuriEventStartTime[] =
{
		12, 0,
		13, 0,
		18, 40,
		19, 40,
		20, 40,
		21, 40
};

class CKingGuriEvent : public cltEvent
{
public:

	enum
	{
		START = 0,
		ACTION,
		DIE_KINGGURI,
		ITEM,				//[추가 : 황진성 2007. 11. 14 대왕 구리댁 죽이고 아이템 획득.]
		END,
	};

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_KINGGURI_NUM>		m_bKilledKingGuri;
	NSafeTArray<SI32, MAX_KINGGURI_NUM>		m_siKingGuriID;
#else
	/// 대왕구리댁이 죽었나?
	bool m_bKilledKingGuri[MAX_KINGGURI_NUM];
	/// 2마리의 대왕구리댁ID
	SI32  m_siKingGuriID[MAX_KINGGURI_NUM];
#endif
	/// 나올 지역이름
	TCHAR m_szAreaName[MAX_POSITION_NUM][32];
	/// 선택된 죄표이름
	TCHAR m_szSelectedAreaName[MAX_KINGGURI_NUM][32];

#ifdef _SAFE_MEMORY
	NSafeTArray<POINT, MAX_POSITION_NUM>	m_ptPosition;
	NSafeTArray<SI32, MAX_KINGGURI_NUM>		m_siAreaIndex;
#else
	/// 초기화 좌표
	POINT m_ptPosition[MAX_POSITION_NUM];

	//[추가 : 황진성 2007. 11. 13 대왕 구리댁이 나온 지역의 인덱스번호]
	SI32 m_siAreaIndex[MAX_KINGGURI_NUM];
#endif

	CKingGuriEvent();
	~CKingGuriEvent();

	void Init();
	void Start();
	void Action();
	void End();
	void SendInfo( SI32 Type, DWORD LeftTime, TCHAR* UserNmae = NULL );
	void SetDieKingGuri( bool Flag, SI32 KingGuriID, SI32 UserID );

	void MakeKingGuri();
	
	//[추가 : 황진성 2007. 11. 14 대왕 구리댁 몬스터가 다 죽었는가?]
	bool MonsterAllDie(void);
};

#endif
