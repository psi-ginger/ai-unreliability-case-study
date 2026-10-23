#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include "../ControllerObj.h"
#include "Scroll.h"


#define MAX_LISTVIEW_DUPLICATE_ITEM_NUM		30000			// 중첩 아이템 개수 제한 ( 떡국 30000 개 있을 수 없을거라는 가정함 )

#define MAX_LISTVIEW_ITEM_NUM				128				// 아이템 개수
#define MAX_LISTVIEW_ITEM_NAME_LENGTH		64				// 아이템 이름 길이 제한

class CToolTip;

#define LISTVIEW_ITEMMASK_SEALED		0x00000001
#define LISTVIEW_ITEMMASK_EXPIRED		0x00000002
#define LISTVIEW_ITEMMASK_CANNOTEQIP	0x00000004

struct stListViewItemData
{
	SI16 ImageFileIndex;									// 몇번째 이미지 파일에 있는지...
	SI16 ImageFontIndex;									// 이미지 파일 몇번째 Font
	SI16 ItemListViewPos;									// 리스트 뷰 상의 위치	
	SI16 ItemNum;
	TCHAR ItemName[ MAX_LISTVIEW_ITEM_NAME_LENGTH ];			// 말이 아이템 이름이지 실제로는 "도끼 3개" 이런식으로 갯수 문자열도 포함됨
	TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ];				// 툴팁 Msg
	SI16 bDrawAlphaOption;		// 이미지를 그려줄때의 옵션( 반투명 등)
	bool bItemSealed;			// 봉인되었는지 여부
	SI32 siItemMask;			// by LEEKH 2007.10.11 - 아이템 표시 마스크

	stListViewItemData()
	{
		memset( this, 0, sizeof( stListViewItemData ) );
	}

	stListViewItemData( stListViewItemData *pItemData )
	{
		memcpy( this, pItemData, sizeof( stListViewItemData ) );
	}

	void Init()
	{
		ZeroMemory(this, sizeof(*this));
	}

	void Set( stListViewItemData *pListViewItemData )
	{
		memcpy( this, pListViewItemData, sizeof( stListViewItemData ) );
	}

	void Set( SI16 FileIndex, SI16 FontIndex, SI16 ListViewPos, SI16 Num, TCHAR *Name, TCHAR *TipMsg,BOOL DrawAlphaOption = false, bool ItemSealed = false)
	{
		ImageFileIndex = FileIndex;
		ImageFontIndex = FontIndex;
		ItemListViewPos = ListViewPos;		
		ItemNum = Num;
		
		if ( Name )
		{
			_tcsncpy( ItemName, Name, MAX_LISTVIEW_ITEM_NAME_LENGTH - 1 );
			ItemName[ MAX_LISTVIEW_ITEM_NAME_LENGTH - 1 ] = NULL;
		}
		if ( TipMsg )
		{
			_tcsncpy( ToolTipMsg, TipMsg, MAX_TOOLTIP_MSG_SIZE - 1 );
			ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE - 1 ] = NULL;
		}

		bDrawAlphaOption = DrawAlphaOption;
		bItemSealed = ItemSealed;

		if(ItemSealed)	siItemMask = siItemMask | LISTVIEW_ITEMMASK_SEALED;
	}

	void SetSealed(bool bSealed)
	{
		bItemSealed = bSealed;
		if(bSealed)
			siItemMask = siItemMask | LISTVIEW_ITEMMASK_SEALED;
		else
			siItemMask = siItemMask - LISTVIEW_ITEMMASK_SEALED;
	}

	void SetExpired(bool bExpired)
	{
		if(bExpired)
			siItemMask = siItemMask | LISTVIEW_ITEMMASK_EXPIRED;
		else
			siItemMask = siItemMask - LISTVIEW_ITEMMASK_EXPIRED;
	}

	void SetCanNotEquip(bool bCanNotEquip)
	{
		if(bCanNotEquip)
			siItemMask = siItemMask | LISTVIEW_ITEMMASK_CANNOTEQIP;
		else
			siItemMask = siItemMask - LISTVIEW_ITEMMASK_CANNOTEQIP;
	}

};

class CListView : public CControllerObj
{
public:
	CListView( CControllerMgr *pParent );
	CListView( CControllerMgr *pParent,SI32 siTmpBufferSize );

	~CListView();

	void Init();

	void Create( SI16 ControllerID,
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
		SI16 SurfaceWidth, SI16 SurfaceHeight, 
		SI16 ListViewOffsetX, SI16 ListViewOffsetY,
		SI16 ListViewItemOffsetX, SI16 ListViewItemOffsetY,
		SI16 ListViewItemSizeWidth, SI16 ListViewItemSizeHeight,
		bool AllowEmptyItemPos, bool ItemNumShow,
		bool UseToolTip, SI16 ToolTipWidth, SI16 ToolTipHeight );
	
	bool InsertItem( stListViewItemData *pItemData );
	bool UpdateItem( stListViewItemData *pItemData );
	bool DeleteItem( SI16 ItemPos );
	void DeleteAllItems();
	bool ChangeItemNum( SI16 ItemPos, SI16 ItemNum );
	bool ChangeItemPos( SI16 PreItemPos, SI16 NextItemPos );

	void SetFocus( bool HasFocus );
	void ClearSelectedItem();

	SI16 GetMouseSelectedItemIndex( LONG XPos, LONG YPos );
	SI16 GetItemIndexFromPoint( POINT pt, bool AbsolutePoint = false );			// 좌표로 아이템 Index 얻어오기

	SI16 GetSelectedItemIndex();
	SI16 GetLastSelectedItemIndex();	

	// 키보드로는 아이템 선택 변경 및 엔터키 입력시 확인 기능을 할 수 있음.
	//bool HandleKeyboard( UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );
	
	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );
	void DrawItemNumber( SI16 DrawXPos, SI16 DrawYPos, SI16 ItemNum );

	void SetStartItemPos( SI16 StartItemPos )
	{
		m_siListViewStartItemPos = StartItemPos;
	}

	void UpdateScrollPos();	
	void Refresh();
	
	stListViewItemData *GetListViewItemData( SI16 Index );

	void SetBorder( bool Border );
	void SetBKColorUse( bool BKColorUse );
	void SetItemNumShow( bool ItemNumShow );
	
	void SetItemDrawAlphaOption( SI16 ItemPos,  bool DrawAlphaOption = true );//[영진] 아이템의 반투명 출력을 임의로 설정하기 위해서

	void SetListViewBorderAndBKColorUse( bool Border, bool BKColorUse );

	SI32 GetScrollTrackPos()
	{
		return m_pScroll->GetTrackPos();
	}

	void SetScrollTrackPos( SI32 siPosition )
	{
		m_pScroll->SetTrackPos( siPosition );
	}

	void SetUseToolTip( bool bUseToolTip )
	{
		m_bUseToolTip = bUseToolTip;
	}

	// 리스트에 등록된 전체 개수.
	SI16 GetListViewItemCounter( void )
	{
		return m_siListViewItemCounter;
	}
	
private:
	CScroll *m_pScroll;				// 스크롤 부분

	bool m_bLMPressed;				// 왼쪽 마우스 버튼
	bool m_bRMPressed;				// 오른쪽 마우스 버튼
	

	// 초기 무 선택 상태( index = -1 )
	// 선택 상태 ( 아이템이 약간 흐려져 보이거나 그래야지 )
	SI16 m_siSelectedIndex;			// 선택된 아이템의 Index, 선택되지 않았다면 -1 리턴
	SI16 m_siLastSelectedIndex;		// 선택한 아이템이 있다면 그 Index, 선택이 없고, 공백일 경우 공백 Index
									// m_siLastSelectedIndex 가 좀 더 세밀한 정보를 주지만, 아이템 없을 경우에 대한 체크가 필요하겠지.. 그래서 특별한 경우가 아니라면 m_siSelectedIndex 이것을 사용하기 바람
	SI16 m_siMouseOverItemIndex;	// Mouse Over 시에 툴팁 보여줘야 함으로.
									// bAllowEmpty true 상태라면 이값(m_siMouseOverItemIndex)은 false 상태랑 틀림...( 공백이 있다면... )

	SI16 m_siMaxHorizontalItemNum;	// 한줄에 몇개의 아이템이 들어가는지... 줄을 맞춰서 넣어야 함으로...

	bool m_bItemExplainShow;		// 아이템 설명( ex : 죽도 2개 ) 아이템 아래에 글자를 보여 줄지에 대한 flag
									// 안 보여 줘도 될듯. 툴팁으로 보여주는 걸로 충분
									// 중첩된 아이템인 경우 개수를 작게 이미지위에 글자로 덮어서 모서리쪽에 보여 주면 됨으로.

	bool m_bItemNumShow;			// 아이템 개수를 모서리 부분에 보여줄려고 하는데, 보여 줄 필요가 없을 경우도 있음으로...
									// 심지어 어떤 경우는 이미지만 들어가는 경우도 있음으로...

	bool		m_bUseToolTip;		// 툴팁

	SI16 m_siListViewOffsetX;		// ListView 이미지의 OffSet X
	SI16 m_siListViewOffsetY;		// ListView 이미지의 OffSet Y

	SI16 m_siListViewItemOffsetX;	// ListView 아이템의 OffSet X
	SI16 m_siListViewItemOffsetY;	// ListView 아이템의 OffSet Y

	SI16 m_siListViewItemSizeWidth;	// ListView 아이템 하나가 차지하는 공간(Width)
	SI16 m_siListViewItemSizeHeight;// ListView 아이템 하나가 차지하는 공간(Height)

	bool m_bAllowEmptyItemPos;		// 리스트뷰는 2가지( 어떤 자리[(ex)3번째 자리]가 비었다면, 4번째 자리에 있는 아이템이 보여주는 것만 3번째 자리에 채워져서 보여지는 방법과, 그 칸을 비워두고 보여주는 방법 2가지가 있지 )

	SI16 m_siListViewItemCounter;

	stListViewItemData	*m_pListViewItemData[ MAX_LISTVIEW_ITEM_NUM ];

	SI16 m_siListViewStartItemPos;	// 리스트뷰가 아이템 리스트의 중간에것을 사용할 수 있기 때문.

	RGB16*	m_ImgEncBuffer;

	SI32 m_siPreScrollPos;			// Update 방식이 구현 됐었다면, 이거 필요없는 변수임
};

#endif
