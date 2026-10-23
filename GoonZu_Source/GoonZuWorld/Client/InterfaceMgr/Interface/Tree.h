#ifndef _TREE_H_
#define _TREE_H_

#include "../ControllerObj.h"

#include <vector>
using namespace std;

class CControllerMgr;
class CScroll;

#define MAX_TREEITEM_HAVE_CHILD_NUM					10
#define TREEITEM_ICON_SIZE							12

struct stTreeItemDrawInfo
{	
	SI16 siDrawXPos;
	SI16 siDrawYPos;

	stTreeItemDrawInfo()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( stTreeItemDrawInfo ) );
	}
};

struct stTreeItem
{
	SI16 siIndex;
	SI16 siParentIndex;
	SI16 siFileIndex;
	SI16 siImageIndex;
	SI16 siParam;
	TCHAR strText[ MAX_TREE_TEXT_LEN ];
	bool bOpen;

	SI16 siTextPixelLen;
	stTreeItemDrawInfo sTreeItemDrawInfo;

	stTreeItem()
	{
		Init();
	}
	void Init()
	{	
		siIndex = 0;
		siParentIndex = 0;

		siFileIndex = -1;
		siImageIndex = -1;

		bOpen = true;

		siTextPixelLen = 0;

		sTreeItemDrawInfo.Init();
	}

	void Set( stTreeItem *pTreeItem )
	{
		if ( pTreeItem )
		{
			memcpy( this, pTreeItem, sizeof( stTreeItem ) );
		}
	}
};

class CTree : public CControllerObj
{
public:
	CTree( CControllerMgr *pParent );
	~CTree();

	void Create( SI16 ControllerID, 
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
		SI16 SurfaceWidth, SI16 SurfaceHeight,				   
		SI16 LineHeight = TREE_LINE_HEIGHT, SI16 OffsetX = 10, SI16 OffsetY = 10
		);

	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );

	void Clear();
	void InsertItem( stTreeItem *pTreeItem );
	void UpdateItem( stTreeItem *pTreeItem );

	void MakeTreeImgEncBuffer();

	void Refresh();

	SI16 GetSelectedItemIndex()
	{
		return m_siSelectedIndex;
	}

	void SetSelectedItemIndex( SI16 siSelectedIndex )
	{
		m_siSelectedIndex = siSelectedIndex;
	}

	SI16 GetItemIndexFromPosition( SI16 XPos, SI16 YPos );
	stTreeItem *GetTreeItemFromIndex( SI16 Index );
	stTreeItem *GetParentTreeItemFromIndex( SI16 ParentTreeItemIndex );
	stTreeItem *GetPreTreeItemFromIndex( SI16 ParentIndex, SI16 MyIndex );
	
	bool IsHaveParentItem( stTreeItem *pTreeItem );

	void DrawInfoInit();
	bool DrawTree( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 &DrawYPos );
	bool DrawTreeItem( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos );
	void DrawLinkLine( HDC hDC, stTreeItem *pPreTreeItem, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos );
	void DrawConnectLine( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos );
	void DrawTreeItemText( HDC hDC, stTreeItem *pTreeItem, SI16 DrawXPos, SI16 DrawYPos );
	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );
private:
	SI16 m_siOffsetX;
	SI16 m_siOffsetY;

	SI16 m_siLineHeight;
	SI16 m_siLineSize;

	SI16 m_siSelectedIndex;

	vector< stTreeItem > m_TreeItemVector;

	RGB16*	m_ImgEncBuffer;

	CScroll	*m_pVScroll;
	CScroll	*m_pHScroll;

	SI16 m_siMaxWidth;

	SI16 m_siAvailabeUseWidth;
	SI16 m_siAvailabeUseHeight;
};

#endif