//---------------------------------
// 2004/2/5 김태곤
//---------------------------------

#ifndef _VILLAGEDETAILINFO_H
#define _VILLAGEDETAILINFO_H

#include <Windows.h>
#include <Directives.h>

#define VILLAGEDETAILINFOTYPE_VILLAGELIST		1		// 마을명단 
#define VILLAGEDETAILINFOTYPE_VILLAGELEVEL		2		// 마을레벨 조회 
#define VILLAGEDETAILINFOTYPE_VISITORNUM		3		// 마을방문자수 조회
#define VILLAGEDETAILINFOTYPE_VILLAGEMONEY		4		// 마을이 가진 자금. 
#define VILLAGEDETAILINFOTYPE_VILLAGEPROFIT		5		// 마을이 가진 자금(이익금) 
#define VILLAGEDETAILINFOTYPE_VILLAGESTOCK		6		// 마을 총 주식수.
#define VILLAGEDETAILINFOTYPE_GENERALMEETING	7		// 주주총회진행마을 
#define VILLAGEDETAILINFOTYPE_WAR				8		// 전쟁 결과 표시. 
#define VILLAGEDETAILINFOTYPE_WARPERRESIDENT	9		// 전쟁 결과 표시. (주민 100명당)
#define VILLAGEDETAILINFOTYPE_PEOPLE			10		// 주민수  표시. 
#define VILLAGEDETAILINFOTYPE_USER				11		// 마을 사용자수 
#define VILLAGEDETAILINFOTYPE_PREVYEARDBID		12		// 전년도 배당액. 
#define VILLAGEDETAILINFOTYPE_HUNT				13		// 수렵장
#define VILLAGEDETAILINFOTYPE_DECLAREWAR		14		// 마을간 적대 관계. 
#define VILLAGEDETAILINFOTYPE_HORSEMARKET		15		// 사복시 
#define VILLAGEDETAILINFOTYPE_STOCK				16		// 여각 
#define VILLAGEDETAILINFOTYPE_REALESTATEMARKET	17		// 거간
#define VILLAGEDETAILINFOTYPE_SUMMONMARKET		18		// 장예원
#define VILLAGEDETAILINFOTYPE_SUMMONHEROMARKET	19		// 비변사
#define VILLAGEDETAILINFOTYPE_PORT				20		// 포구
#define VILLAGEDETAILINFOTYPE_SHIPMARKET		21		// 전함사
#define VILLAGEDETAILINFOTYPE_FARM				22		// 농장
#define VILLAGEDETAILINFOTYPE_FEAST				23		// 내자시
#define VILLAGEDETAILINFOTYPE_MINE				24		// 광산
#define VILLAGEDETAILINFOTYPE_GUILD				25		// 상단(길드)
#define VILLAGEDETAILINFOTYPE_CITYHALLSTATE		26		// 관청상태정보( 정상기, 파산기, 안정기, 초기화기 등..)



class cltGameMsgResponse_VillageDetailInfo;

class cltVillageDetailData{
public:
	SI32	siType;
	char	szTypeText[64];
	char	szExplainText[256];
};

class cltVillageDetailInfoManager{
public:
	HWND m_hDlg;					// 아이템 정보. 

	RECT DlgRect;
	bool bDlgRectInitSwitch;
	bool bCheckSortVillage;			// 마을 정렬 체크 버튼의 상태 01/18 추가

	SI32 siCurrentType;
	SI32 siOldCurrentType;
	SI32 siOldViliage;
	
	HBRUSH	m_hEditBrush;
	HBITMAP m_hWarpBitmap;


	cltVillageDetailInfoManager();
	~cltVillageDetailInfoManager();
	

	BOOL CreateDlg(HINSTANCE hinst, HWND hwnd);
	static BOOL CALLBACK StaticDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// 콤보에 데이터 타입을 넣는다. 
	void SetDataTypeInCombo(HWND hDlg, SI32 dlgitem);

	// 타입을 근거로 정보 포인터를 구한다. 
	cltVillageDetailData* GetVillageDetailData(SI32 type);

	// 텍스트를 근거로 마을 상세 정보 코드를 구한다. 
	SI32 GetVillageDetailType(char* ptext);

	// 서버로 부터 받은 정보를 표시한다. 
	void ShowInfo(cltGameMsgResponse_VillageDetailInfo* pclinfo);
	
	// 건물 상세 정보를 표시한다. 
	void ShowStrInfo(char*  ptext);


	// 내림차순으로 정렬한다. 
	SI32 Sort(SI32 sisortdata[], SI32* pvillageunique);
	SI32 Sort(char* pSortname[], SI32* pvillageunique);
	
};

#endif
