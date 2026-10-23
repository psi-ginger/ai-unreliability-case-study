//---------------------------------
// 2004/2/5 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		CNVillageDetailInfo

	작성일 :		05/03/02
	작성자 :		정용래

	목  적 :		CNVillageDetailInfo 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 마을 상세 정보 다이얼로그를 생성한다.

	변경사항 :		없음
	
	차후작업 :		cpp 작업 완료 요망

   ========================================================================*/

//#if (defined(__LEON_START_ALL__) || defined(__LEON_NVILLAGEDETAILINFO__)) && defined(_DEBUG)

#pragma once

#include <Directives.h>
#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Item/ItemCommon/ItemUnit.h"

#define NVILLAGEDETAILINFOTYPE_VILLAGELIST		1		// 마을명단 
#define NVILLAGEDETAILINFOTYPE_SCORE			2		// 마을점수
#define NVILLAGEDETAILINFOTYPE_VISITORNUM		3		// 마을방문자수 조회
#define NVILLAGEDETAILINFOTYPE_VILLAGEMONEY		4		// 마을이 가진 자금. 
#define NVILLAGEDETAILINFOTYPE_VILLAGEPROFIT	5		// 마을이 가진 자금(이익금) 
#define NVILLAGEDETAILINFOTYPE_VILLAGESTOCK		6		// 마을 총 주식수.
#define NVILLAGEDETAILINFOTYPE_GENERALMEETING	7		// 주주총회진행마을 
#define NVILLAGEDETAILINFOTYPE_WAR				8		// 전쟁 결과 표시. 
#define NVILLAGEDETAILINFOTYPE_WARPERRESIDENT	9		// 전쟁 결과 표시. (주민 100명당)
#define NVILLAGEDETAILINFOTYPE_PEOPLE			10		// 주민수  표시. 
#define NVILLAGEDETAILINFOTYPE_USER				11		// 마을 사용자수 
#define NVILLAGEDETAILINFOTYPE_PREVYEARDBID		12		// 전년도 배당액. 
#define NVILLAGEDETAILINFOTYPE_HUNT				13		// 수렵장
#define NVILLAGEDETAILINFOTYPE_DECLAREWAR		14		// 마을간 적대 관계. 
#define NVILLAGEDETAILINFOTYPE_HORSEMARKET		15		// 사복시 
#define NVILLAGEDETAILINFOTYPE_STOCK			16		// 여각 
#define NVILLAGEDETAILINFOTYPE_REALESTATEMARKET	17		// 거간
#define NVILLAGEDETAILINFOTYPE_SUMMONMARKET		18		// 장예원
#define NVILLAGEDETAILINFOTYPE_SUMMONHEROMARKET	19		// 비변사
#define NVILLAGEDETAILINFOTYPE_FARM				20		// 농장
#define NVILLAGEDETAILINFOTYPE_FEAST			21		// 내자시
#define NVILLAGEDETAILINFOTYPE_MINE				22		// 광산
#define NVILLAGEDETAILINFOTYPE_GUILD			23		// 상단(길드)
#define NVILLAGEDETAILINFOTYPE_CITYHALLSTATE	24		// 관청상태정보( 정상기, 파산기, 안정기, 초기화기 등..)
#define NVILLAGEDETAILINFOTYPE_VILLAGEWARRECORD	25		// 마을레벨 조회
#define NVILLAGEDETAILINFOTYPE_NEWMARKET		26		// [진성] 통합마켓.

#define MAX_VILLAGEDETIALINFOTYPE	30
#define LEAST_COMMON_MULTILPLE	30


class CStatic;
class CComboBox;
class CList;
class CCheckBox;
class CButton;
class CEdit;

class cltGameMsgResponse_VillageDetailInfo;

class cltNVillageDetailData{
public:
	SI32	siType;
	TCHAR	szTypeText[64];
	TCHAR	szExplainText[256];
};

class CNVillageDetailInfo : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 100,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 100,
	};


	cltNVillageDetailData m_clvillagedata[MAX_VILLAGEDETIALINFOTYPE];

	RECT DlgRect;
	bool bDlgRectInitSwitch;
	bool bCheckSortVillage;			// 마을 정렬 체크 버튼의 상태 01/18 추가

	SI32 siCurrentType;
	SI32 siOldCurrentType;
	SI32 siOldViliage;

	bool bMasterView;				// [추가 : 황진성 2008. 2. 22 => 관리자 모드이면 주주총회 가능 마을이 보이게 리스트 정렬.]

	CNVillageDetailInfo();
	~CNVillageDetailInfo();
	

	void Create(CControllerMgr * pParent);

	static void CALLBACK StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	// 콤보에 데이터 타입을 넣는다. 
	void SetDataTypeInCombo();

	// 타입을 근거로 정보 포인터를 구한다. 
	cltNVillageDetailData* GetNVillageDetailData(SI32 type);

	// 텍스트를 근거로 마을 상세 정보 코드를 구한다. 
	SI32 GetVillageDetailType(TCHAR* ptext);

	// 서버로 부터 받은 정보를 표시한다. 
	void ShowInfo(cltGameMsgResponse_VillageDetailInfo* pclinfo);
	
	// 건물 상세 정보를 표시한다. 
	void ShowStrInfo(TCHAR*  ptext);


	// 내림차순으로 정렬한다. 
	SI32 Sort(SI32 sisortdata[], SI32* pvillageunique);
	SI32 Sort(SI64 sisortdata[], SI32* pvillageunique);
	SI32 Sort(TCHAR* pSortname[], SI32* pvillageunique);

	//bool IsShow();
	void Show();
	void	 Hide();

	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void GeneralMeetingSuggestInfo(void);

    void FindVillage(TCHAR * szVillagename);
	// 검색 기능을 위해
	void ItemFind(TCHAR* pStrname);	// 선택된 아이템 정보를 표시
	void Find(TCHAR* pItemname);
	void AddListItem(TCHAR* findname);
	void Update();

	bool		m_bShow;
	SI32		m_timerCount;

	CStatic		*m_pNVillageDetailStatic;
	CComboBox	*m_pNVillageDetailComboBox;
	CList		*m_pNVillageDetailList;
	CButton		*m_pWarpToNVillageButton;
	CEdit		*m_pNVillageDetailEdit;
	CButton		*m_pCloseButton;
	CCheckBox	*m_pSortVillageCheckBox;
	 
    SI32		m_siListdata[MAX_VILLAGE_NUMBER];

	//[추가 : 황진성 2008. 2. 22 => 마을에 주주총회 설정 버튼 추가.]
	CButton*	pSetGeneralmeetingButton;
	// 검색기능
	CList			*m_pListBox_AutoMake;
	CEdit			*m_pEdit_Name;
	CButton			*m_pButton_Accept;
	CStatic			*m_pStatic_Back;//백그라운드

private:
	NTCHARString64		m_LastInput;
};

//#endif //#if (defined(__LEON_START_ALL__) || defined(__LEON_NVILLAGEDETAILINFO__)) && defined(_DEBUG)