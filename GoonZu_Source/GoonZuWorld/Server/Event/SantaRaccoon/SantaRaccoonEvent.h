#ifndef _SANTARACCOON_H_
#define _SANTARACCOON_H_

#include "../../../common/Event/event.h"

const SI32 MAX_SANTARACCOON_NUM				= 2;
const SI32 MAX_SANTARACCOON_POSITION_NUM	= 11;
const SI32 MAX_SANTARACCOON_MESSAGE_SIZE	= 256;

class CSantaRaccoon : public cltEvent
{
public:

	enum
	{
		START = 0,
		ACTION,
		DIE,
		ITEM,				//[추가 : 황진성 2007. 11. 29 산타라쿤 죽이고 아이템 획득.]
		END,
	};

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_SANTARACCOON_NUM>		m_bKilledSantaRaccoon;
	NSafeTArray<SI32, MAX_SANTARACCOON_NUM>		m_siSantaRaccoonID;
#else
	/// 산타라쿤이 죽었나?
	bool m_bKilledSantaRaccoon[MAX_SANTARACCOON_NUM];
	/// 2마리의 대왕구리댁ID
	SI32  m_siSantaRaccoonID[MAX_SANTARACCOON_NUM];
#endif

	/// 나올 지역이름
	TCHAR m_szAreaName[MAX_SANTARACCOON_POSITION_NUM][32];
	/// 선택된 죄표이름
	TCHAR m_szSelectedAreaName[MAX_SANTARACCOON_NUM][32];

#ifdef _SAFE_MEMORY
	NSafeTArray<POINT, MAX_SANTARACCOON_POSITION_NUM>	m_ptPosition;
	NSafeTArray<SI32, MAX_SANTARACCOON_NUM>				m_siAreaIndex;
#else
	/// 초기화 좌표
	POINT m_ptPosition[MAX_SANTARACCOON_POSITION_NUM];

	//[추가 : 황진성 2007. 11. 29 산타라쿤이 나온 지역의 인덱스번호]
	SI32 m_siAreaIndex[MAX_SANTARACCOON_NUM];
#endif


	CSantaRaccoon();
	~CSantaRaccoon();

	void Init();
	void Start();
	void Action();
	void End();
	void SendInfo( SI32 Type, DWORD LeftTime, TCHAR* UserNmae = NULL );
	void SetDieSantaRaccoon( bool Flag, SI32 KingGuriID, SI32 UserID );

	void MakeSantaRaccoon();

	//[추가 : 황진성 2007. 11. 29 산타라쿤 몬스터가 다 죽었는가?]
	bool MonsterAllDie(void);
};

#endif
