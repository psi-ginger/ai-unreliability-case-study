#ifndef _GOONZUKINGGURI_H_
#define _GOONZUKINGGURI_H_

#include "../../../common/Event/event.h"
#include "../../../Client/ninterface/ngoonzudlg/NGoonzuRaccoonEventDlg.h"

const SI32 MAX_GOONZU_KINGGURI_NUM = 3;
const SI32 MAX_GOONZU_POSITION_NUM = 11;
const SI32 MAX_GOONZU_MESSAGE_SIZE = 256;

//#define CanSelect_EventNum	3		// 하루가능한 자이언트 라쿤의 횟수 3회 
//#define MAX_MONSTER_NUM_GOONZURACCOONEVENT	3	// 최대 생성 몬스터 3마리
//#define MAX_SELECTRFIELD_NUM	3	//한 라쿤이벤트에서 선택할수 있는 최대한의 필드콤보박스 인덱스 갯수
class CGoonzuKingGuriEvent : public cltEvent
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
	enum
	{
		GOONZU_KINGGURI_FIRSTMAP = 0,
		GOONZU_KINGGURI_SECONDMAP,
		GOONZU_KINGGURI_THIRDTMAP,
	};
	enum
	{
		GOONZU_KINGGURI_FIRSTTIME = 0,
		GOONZU_KINGGURI_SECONDTIME,
		GOONZU_KINGGURI_THIRDTTIME,
	};
	enum
	{
		GOONZU_KINGGURI_SAMETIME = 0,		// 3타임중 같은 타임이 있다
		GOONZU_KINGGURI_NOTOVERTIME,		// 24시간이 지나기 전에 셋팅 했다
		GOONZU_KINGGURI_NOTENOUGHDATA,		// 3타임이 모두 설정 되지 않았다.
		GOONZU_KINGGURI_CORRECTDATA,		// 정상 데이터 
	};

#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_GOONZU_KINGGURI_NUM>		m_bKilledKingGuri;
	NSafeTArray<SI32, MAX_GOONZU_KINGGURI_NUM>		m_siKingGuriID;
#else
	// 해당 구리덱이 죽었는지 
	bool m_bKilledKingGuri[MAX_GOONZU_KINGGURI_NUM];
	// 만든 구리덱 아이디
	SI32  m_siKingGuriID[MAX_GOONZU_KINGGURI_NUM];
#endif

	

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GOONZU_KINGGURI_NUM>		m_siAreaIndex;
#else
	// 구리덱이 나온 지역 인덱스
	SI32 m_siAreaIndex[MAX_GOONZU_KINGGURI_NUM];
#endif
	/// 선택된 지역이름
	TCHAR m_szSelectedAreaName[MAX_GOONZU_KINGGURI_NUM][32];
	/// 선택된 죄표
	POINT m_ptPosition[MAX_GOONZU_POSITION_NUM];


	SYSTEMTIME LsatsetTime	;	// 마지막으로 셋팅 한 시간

	RocData GoonzuRaccoonRocData[MAX_DATA_MAP_NUM]	;	// 지도 좌표를 가지고 있는다.


	//필요한 정보
	/*
	1. 구리덱이 나올 지도와 좌표
	2. 생성된 구리덱의 리스트 
	*/
	// 정보를 저장해둘 배열

	cGoonzuRaccoonEventData GoonzuRaccoonEventData[MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT]	; // 서버에서 가지고 있어야할 라쿤이벤트 발생 데이터 (시간 , 라쿤 마리수,나올 맵인덱스 , 나올 X y 좌표 )

	CGoonzuKingGuriEvent();
	~CGoonzuKingGuriEvent();

	void Init();
	void Start();
	void Action();
	void End();
	void SendInfo( SI32 Type, DWORD LeftTime, TCHAR* UserNmae = NULL );
	void SetDieKingGuri( bool Flag, SI32 KingGuriID, SI32 UserID );

	void MakeKingGuri();	
	bool MonsterAllDie(void);

	// DATACHECK
	SI32 ChekData(cGoonzuRaccoonEventData* GoonzuRaccoonEventData)	; // 셋팅 하기에 적합한 데이터 인지 체크 한다 
	void LoadData(void);	// 리소스에서 좌표를 읽어 온다 .

	bool IsCorrectRoc(SI32 siMapname , SI32 X , SI32 y); // 클라이언트 에서 넘어온 좌표가 정상인지 확인 한다 비정상이면 무조건 셋팅 불가능 

};

#endif
