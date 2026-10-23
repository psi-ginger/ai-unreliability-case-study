#ifndef _NAMBERBOXDLG_H_
#define _NAMBERBOXDLG_H_

#include "../Client/client.h"
#include "../Common/Util/Util.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../ninterface/NAmberBoxDlg/NAmberBoxDlg.h"

//#define BOX_ITEM_NUM			48	// 박스에서 나올수 있는 아이템의 개수.
//#define SELECT_ITEM_LIST_NUM	3	// 선택되는 아이템의 개수.

class CNAmberBoxDlg : public CDialogController
{
public:
	CNAmberBoxDlg();
	~CNAmberBoxDlg();

	void Create();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	
	static void CALLBACK StaticNAmberBoxDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
		   void CALLBACK NAmberBoxDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void SetItemList( SI32* _psiItemList, SI32 _siItemNum, SI16 _siUseItemUnique );	// 상자를 열면 나오는 아이템의 종류를 리스트에 보여준다.
	void SelectItem( void );																	// 호박상자에서 나오는 아이템 세팅.
	void SetSelectItemList( AmberboxSelectItemInfo* pclAmberboxSelectItemInfo );				// 클릭 후 선택 할 아이템 세팅.
	void SetSelectItemListClear( );																// 선택할 아이템을 모두 삭제.
	bool IsSelectItem( void );																	// 

	void SendMsgSelectItemAdd(SI32 _siSlotIndex);												// 서버에 몇번 인덱스에 아이템을 클릭했는지 알림.
	void SendMsgSelectItemGet(SI32 _siSlotIndex);												// 서버에 몇번 인덱스에 아이템을 획득 할건지 알림.
	void Mix();																					// 사각형이 움직이는 것을 섞는다.
	void ClickSlot(SI32 _siSlotIndex);

private:
	bool					m_bInit;															// DB에서 선택 가능한 아이템의 리스트를 받았는가?
	
	InterfaceFactory		m_InterfaceFactory;

	AmberboxSelectItemInfo	m_clAmberboxSelectItemInfo[AMBER_BOX_SELECT_ITEM_LIST_NUM];

	SI32					m_bClickSlot;														// 클릭버튼 누르기.
	SI32					m_siMix[AMBER_BOX_LIST_NUM];										// 렌덤하게 사각형이 움직이게 순서를 담아둠.

	SI32					m_siRandItemNum;													// 랜덤하게 현재 사각형이 그려지는 리스트의 아이템 번호. 
	DWORD					m_dwPrevTickCount;

	TSpr					m_Spr;																// 렌덤하게 선택되는 모냥새를 보여줌.
	TSpr					m_SprListViewBack;																// 렌덤하게 선택되는 모냥새를 보여줌.
	SI32					m_siRandItemListNum;												// 나오는 아이템의 개수.

	SI32					m_siSelectItemIndex[AMBER_BOX_SELECT_ITEM_LIST_NUM];				// 랜덤하게 선택된 아이템의 리스트뷰의 인덱스 번호.

public:
	SI16					m_siUseItemUnique;													// 사용하는 아이템의 유니크.
};
#endif
