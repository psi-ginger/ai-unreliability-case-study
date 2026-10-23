#pragma once

#include "../../InterfaceMgr/DialogController.h"
//#include "../common/ItemMallManager/ItemMallManager.h"
#include "../common/item/ItemCommon/ItemMallData.h"
#include "../common/Item/ItemCommon/cltItem.h"
#include "../common/Char/CharCommon/Char-Common.h"
#include "../../DBManager/GameDBManager_World/DBMsg-ItemMall.h"

#include <vector>

typedef std::vector<cltItemMallData>  vecSelectItemMall;
typedef vecSelectItemMall::iterator   vecIterSelectItemMall;

class CButton;
class CImageStatic;
class CStatic;
class CList;
class CCharacterDrawing;

#define DELAYTIME	1500
#define SELECT_ITEM_NUM 6
#define SELECT_HAT		0
#define SELECT_DRESS	1
#define SELECT_MANTLE	2
#define SELECT_ANIMAL	3
#define SELECT_SETITEM1	4
#define SELECT_SETITEM2	5

// 새로운 아이템몰창(사기)
class CNNewItemMallDlg : public CDialogController
{
public:

	CNNewItemMallDlg();
	~CNNewItemMallDlg();

	void Create();

	void InitItemMallData();
	void InitSelectItemMallData();
	void InitItemImageStatic();
	void InitEquipItemData();
	void InitSelectEquipItemData( SI16 index );
	void SetImageFontIndex( SI16 selectindex, bool select );
	void LoadItemMallData( SI32 page, SI32 maxpage, cltItemMallData* pclinfo );	// first 상위 메뉴 선택, second 세부메뉴 선택
	void ShowItemMallData();
	void SelectItemList( SI16 SelectIndex );
	void SetCharDraw( SI16 index, SI16 unique );
	void SetCharSetDraw( SI16 index1, SI16 index2, SI16 index3, SI32 unique1, SI32 unique2, SI32 unique3 );
	void SetHaveHorseDraw( SI16 type );
	void SetMyCashInfo(SI32 realcash, SI32 eventcash);
	void ShowBuyWindow(SI16 index);
	void ShowSmallBuyWindow(SI16 index);
	void ShowPresentWindow(SI16 index);
	void DeleteItemList(SI16 index);
	void SelectThreeMenu(TCHAR* code);	// 뉴, 인기, 스페셜 아이템 버튼
	BOOL ConvertExplain(TCHAR* pText , SI32 size);
	void ShowSiverCoinMark( SI32 index, bool Flag );
	
	void NHNChinaInit( void );
	void NHNChinaGoldItemMall( void );
	void NHNChinaEventItemMall( void );
	void NHNChinaShowCategory( bool	_bShow );


	static void CALLBACK StaticCallBackDialogNNewItemMallDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNNewItemMallDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	void DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY );

//	void SetBillPageHandle( HWND* hWnd );
	
	void Action();
 
	BOOL CheckEquipAble(SI32 siItemUnique);

	TCHAR* m_FileName[5];

	//------------------------------
	// 출력을 위한 장비 코드 
	//------------------------------
	TCHAR				szHelmetCode[32];
	TCHAR				szArmourCode[32];
	TCHAR				szWeaponCode[32];
	TCHAR				szMantleCode[32];
	TCHAR				szHorseCode[32];	

	cltItem				clCharItem[MAX_SHAPE_ITEM];
	cltAniInfo			clAniInfo;
	CCharacterDrawing*  m_pCharDraw;

	DWORD				m_dwStratClock;			// 클릭 지연을 위해서
	DWORD				m_dwLastClock;

	TCHAR				m_szType[4];
	SI32				m_siRealCash;
	SI32				m_siEventCash;
	SI16				m_siAniType;				// 말탄거랑 안 탄거구분
	SI16				m_siAniDirection;
	bool				m_bHaveHorse;				// 말소유
	SI16				m_siMaxPageNumber;			// 총아이템 페이지 수
	SI16				m_siCurrentPageNumber;		// 현재의 페이지 수

	SI16				m_siFirstMenuSelect;		// 상위메뉴 선택
	SI16				m_siSecondMenuSelect;		// 하위메뉴 선택
	SI16				m_siTempKey;				// 상위메뉴를 0~4로 변환

	SI16				m_siItemListSelect;			// 6개중에 선택하는거!!
	bool				m_bSelect[MAX_ITEMMALL_LIST_NUM];				// 선택 or 취소
	bool				m_bPreviewSelect[5];		// 0: 모자, 의복, 망토, 말, 세트
	bool				m_bUseMenu;					// 
	SI16				m_siSetItemLoad;
 
	cltItemMallData		m_ItemDataList[MAX_ITEMMALL_LIST_NUM];
	vecSelectItemMall	m_vecSelectItemMall;	// 미리보기창에 선택된 아이템
	SI16				m_siCount;

	CButton*			m_pButtonForward;
//	CButton*			m_pButtonPresent;
	CButton*			m_pButtonExit;

	CImageStatic*		m_pButtonCharacter;			// 캐릭터
	CImageStatic*		m_pButtonPower;				// 파워
	CImageStatic*		m_pButtonSummonHorse;		// 소환/말
	CImageStatic*		m_pButtonMaterialFood;		// 재료/음식
	CImageStatic*		m_pButtonBeginner;			// 초보용	( 중국에서 은전버튼으로 쓴다 )

	CButton*			m_pButtonNewItem;				// 뉴아이템
	CButton*			m_pButtonPopularItem;			// 인기아이템
	CButton*			m_pButtonSpecialItem;			// 스페셜아이템

	CImageStatic*		m_pButtonSubject1;			// 세부 메뉴1
	CImageStatic*		m_pButtonSubject2;			// 세부 메뉴2	
	CImageStatic*		m_pButtonSubject3;			// 세부 메뉴3
	CImageStatic*		m_pButtonSubject4;			// 세부 메뉴4
	CImageStatic*		m_pButtonSubject5;			// 세부 메뉴5

	CButton*			m_pButtonBuy1;					// 구입 버튼1
	CButton*			m_pButtonBuy2;
	CButton*			m_pButtonBuy3;
	CButton*			m_pButtonBuy4;
	CButton*			m_pButtonBuy5;
	CButton*			m_pButtonBuy6;

	CButton*			m_pButtonPresent[6];
//	CButton*			m_pButtonPresent1;				// 선물 버튼1
//	CButton*			m_pButtonPresent2;
//	CButton*			m_pButtonPresent3;
//	CButton*			m_pButtonPresent4;
//	CButton*			m_pButtonPresent5;
//	CButton*			m_pButtonPresent6;

	CButton*			m_pButtonReset;					// 미리 보기창 초기화
	CButton*			m_pButtonNcoin;					// 엔코인 충전하기

	CButton*			m_pButtonPrev;					// 아이템 목록 이전으로
	CButton*			m_pButtonNext;					// 아이템 목록 다음으로

	CButton*			m_pButtonSmallBuy[4];			// 선택된 아이템 사기 버튼
	CButton*			m_pButtonSmallNo[4];			// 선택된 아이템 해재 버튼

	CButton*			m_pButtonLeftTurn;				// 미리 보기에서 방향 전환
	CButton*			m_pButtonRightTurn;

	CButton*			m_pButtonGoldItemMall;			// [진성] 일반 아이템몰
	CButton*			m_pButtonEventItemMall;			// [진성] 이벤트 아이템몰
	CButton*			m_pButtonEventItemMallExplain;	// [진성] 이벤트 아이템몰이 되었을 때 보이는 버튼. (일단 보이기만) 
	CButton*			m_pButtonEventMoneyInfo;		// [진성] 이벤트 돈 조회.

	//CButton*			m_pButtonFreeReplenishment;		// 무료충전소

	CImageStatic*		m_pISItem[6];					// 아이템 배경 이미지 6개 선택시(폰트1번) 선택 안되면(폰트 0번)
	CImageStatic*		m_pISShowItem[6];				// 보여지는 아이템 그림
	CImageStatic*		m_pISItemName[6];				// 아이템의 글씨

	CStatic*			m_pStaticItemPrice[6];			// 아이템의 가격
	CStatic*			m_pStaticItemEventPrice[6];		// [진성] 이벤트 아이템의 가격
	
	CStatic*			m_pStaticCurrentPage;			// 현재 페이지
	CStatic*			m_pStaticMaxPage;				// 총 페이지
	CStatic*			m_pStaticCurrentPage1;			// 현재 페이지 (10자리)
	CStatic*			m_pStaticMaxPage1;				// 총 페이지 (1자리)

	CStatic*			m_pStaticMyNcoin;				// 현재 나의 엔코인 ( 중국은 금전 )
	CStatic*			m_pStaticEvenyMoney;			// 현재 나의 이벤트 머니( 중국은 은전 )

	CList*				m_pList;						// 선택된 아이템 리스트

	CStatic*			m_pStaticSilverCoin[6];			// 은전 그림 6개

	CButton*			m_pButtonGetItem;				// 중국에서 씁니다. 아이템 수령창 띄우기

	TSpr	    		m_SprBack;						// 1024*768 전체 화면에서의 뒤쪽 배경그림

//	HWND				m_BillPageHandle;
	BOOL				m_bBillPageActive;
	bool				m_bEventItemMall;				// 이벤트 아이템.
};