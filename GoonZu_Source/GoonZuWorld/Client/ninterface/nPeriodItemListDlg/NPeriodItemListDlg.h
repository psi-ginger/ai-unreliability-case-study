//////////////////////////////////////////////////////////////////////////
//	090326	[조종호]	남은 기간 관련된 BUFF & Service들의 리스트 Dlg
//////////////////////////////////////////////////////////////////////////


#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../ninterface/NBuffStatus/NBuffstatus.h"

#define MAX_ITEMMALL_ITEM_PER_PAGE	5

class CButton;
class CImageStatic;

class CNPeriodItemListDlg : public CDialogController
{
public:
	//	Resource상에서 효과별 TextIndex 와 해당 Icon을 지정하기 위해 기간제아이템 다이얼로그 창에서만 쓰이는 buffEffect 효과들인덱스
	//	!Warning : 해동 버프 인덱스는 리소스연동을 위해 만든 버프 인덱스 입니다. 기존의 코드에 있는 버프 인덱스와 혼동 금지!
	enum BuffIndex_UseOnlyThis	
	{
		BuffIndex_BUF_PREMIUMEXPUP,
		BuffIndex_BUF_SUPEREXPUP,
		BuffIndex_BUF_SPECIALAMPUL_EXPUP300,
		BuffIndex_BUF_FARMINGNMININGFOREVER,
		BuffIndex_PremiumService_SpaceBox,
		BuffIndex_BUF_STATUE_UP_STR,
		BuffIndex_BUF_STATUE_UP_DEX,
		BuffIndex_BUF_STATUE_UP_MAG,
		BuffIndex_BUF_STATUE_UP_VIT,
		BuffIndex_BUF_STATUE_UP_HAND,
		BuffIndex_BUF_EXP_UP,
		BuffIndex_BUF_PREMIUMMAKEEXPUP,
		BuffIndex_PremiumService,
		BuffIndex_CombatService,
		BuffIndex_MakeItemService,
		BuffIndex_BulletService,
		BuffIndex_SummonStaminaService,
		BuffIndex_MarKetPremiumService,
		BuffIndex_NewMarketPremiumService,
		BuffIndex_BUF_ATTACKUP,
		BuffIndex_BUF_DEFENSEUP,
		BuffIndex_BUF_SPEEDUP,
		BuffIndex_BUF_MAKEEXPUP_20,
		BuffIndex_BUF_MAKEEXPUP_150,
		BuffIndex_BUF_NOFIRSTATTACK,
		BuffIndex_BUF_SUMMONATACUP,
		BuffIndex_BUF_LOVEPOWER,
		BuffIndex_BUF_FRIENDSHIP,
		BuffIndex_BUF_HINAMOCHI,
		BuffIndex_BUF_SUCCESS_MANUFACTURE,
		BuffIndex_BUF_MOFUMOFU_PERFUME,
		BuffIndex_BUF_FOURLEAF,
		BuffIndex_BUF_HAPPYBEAN,
		BuffIndex_BUF_ATTACKUP30DAY,
		BuffIndex_BUF_DEFENSEUP30DAY,
		BuffIndex_BUF_THANKSGIVING_DAY,
		BuffIndex_BUF_WATERMELON,
		BuffIndex_UseNoLimitTicketWarp,
		BuffIndex_BUF_OLYMPICMEDAL_GOLD,
		BuffIndex_BUF_OLYMPICMEDAL_SILVER,
		BuffIndex_BUF_OLYMPICMEDAL_BROLZE,
		BuffIndex_RIDE_ADDSTATUS,
		BuffIndex_BIKE_ADDDEX,
		BuffIndex_HORSE_ADDDEX,
		BuffIndex_BIKE_ADDSTR,
		BuffIndex_HORSE_ADDSTR,
		BuffIndex_BIKE_ADDMAG,
		BuffIndex_HORSE_ADDMAG,
		BuffIndex_BUF_STATUE_UP_WIS,
		BuffIndex_BUF_CHRISTMASEVENT_SNOWBLESS,
		BuffIndex_BUF_CHRISTMASEVENT_DECORATION,
		BuffIndex_BUF_RESIDENTS_EXPUP_10,
		BuffIndex_BUF_RESIDENTS_STRONG,
		BuffIndex_BUF_VALENTINE,

		BuffIndex_End,
	};


	enum PeriodTabNumber			//	090324[종호]	Tab창별 enum형 
	{
		PERIOD_TAB_CHARACTER,
		PERIOD_TAB_BUFF,
		PERIOD_TAB_HORSE,
		PERIOD_TAB_END,
	};

	struct sPeriodItem											//	090320[종호]	기간제 아이템 구조체
	{
		SI32					siRefIndex;						
		string					strPeriodItemInfo;	
		SI32					siPeriodTime;						//	남은 기간순으로 정렬 하기 위한 data
	};

	static bool CompareForPeriodItem(const sPeriodItem& elem1, const sPeriodItem& elem2 )
	{
		if(elem1.siPeriodTime < elem2.siPeriodTime)
			return true;
		else
			return false;
	}

	InterfaceFactory			m_InterfaceFactory;

	SI32 						m_siCurrentPage;
	SI32 						m_siMaxPage;	
	SI32						m_siCurrentTabIndex;							//	090324[종호]	tab인덱스
	
	SI32 						m_siImageType[MAX_ITEMMALL_ITEM_PER_PAGE];
	SI32 						m_siImageIndex[MAX_ITEMMALL_ITEM_PER_PAGE];

	CButton*					m_pButton_PeriodItem[PERIOD_TAB_END];			//	090324[종호]	tab버튼

	TSpr						m_BuffImg;										//	090331[종호]	버프 이미지 SPR

	vector<sPeriodItem>			m_vecPeriodItem[PERIOD_TAB_END];				//	090324[종호]	기간제 아이템 리스트
	
	struct stTabTextInfo
	{
		CControllerObj	*st_pControllerObj;
		COLORREF	st_Color;
		SI16		st_siWeight;

		stTabTextInfo()
		{
			Init();
		}

		void Init()
		{
			memset( this, 0, sizeof( stTabTextInfo ));
		}

		void Set( CControllerObj* in_pControllerObj, COLORREF in_Color, SI16 in_siWeight )
		{
			st_pControllerObj = in_pControllerObj;
			st_Color = in_Color;
			st_siWeight = in_siWeight;
		}
	};

public:

	CNPeriodItemListDlg();
	~CNPeriodItemListDlg();

	void	Initialize();
	void	Destroy();

	void	Create();

	static	void CALLBACK StaticNPeriodItemListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NPeriodItemListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void	Action();	
	void	DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	void	SelectedTabBtn();		//	090323[종호]	탭 폰트의 상태를 설정
	void	SetTabTextInfo(stTabTextInfo* in_SelectedText);

	void	SetPeriodItemList();	//	090323[종호]	기간아이템 리스트를 set
	void	ShowPeriodItemList();	//	090323[종호]	기간아이템 리스트를 Show

	
	cltDate		SetPeriodItemTime(bool bIsBuff, SI32 siTimeVary);
	void		SetPeriodItemInfo(cltDate pcldate, TCHAR* pchDataInfo, SI32 siTextFromMgr, SI32 siBuffPercent = 0);
	void		InputData(SI32 DataIndex, SI32 siKindOfList, TCHAR* pchDataInfo, SI32 siTime);
	
};
