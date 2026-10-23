#ifndef _INTERFACEMGR_H_
#define _INTERFACEMGR_H_

#include "ControllerBaseDefinition.h"
#include "../../common/Item/ItemCommon/cltItem.h"

#pragma comment(lib, "imm32.lib")			// include ime library


class	CControllerMgr;
class	CControllerObj;
class	CInterfaceImgMgr;
class	CToolTip;

struct	stCandidateData;
class	CControllerSurfaceObj;

#define SCREEN_OFFSET_SIZE					80
#define AVAILABLE_SNAP_OFFSET_SIZE			5

enum CLICKANDDROP_TYPE
{
	CLICKANDDROP_TYPE_NONE,
	CLICKANDDROP_TYPE_ITEM,
};

class cltClickAndDrop
{
protected:
	SI32		m_siType;

public:
	SI32		m_siSrcDialogID;
	SI32		m_siDstDialogID;

	SI32		m_siSrcControlID;
	SI32		m_siDstControlID;

	SI32		m_siPara1;
	SI32		m_siPara2;
	SI32		m_siPara3;
	SI32		m_siPara4;

public:
	cltClickAndDrop(SI32 siType)
	{
		m_siType = siType;

		m_siSrcDialogID = 0;
		m_siDstDialogID = 0;

		m_siSrcControlID = 0;
		m_siDstControlID = 0;

		m_siPara1 = 0;
		m_siPara2 = 0;
		m_siPara3 = 0;
		m_siPara4 = 0;
	}
	~cltClickAndDrop()	{}
	bool IsType(SI32 siType)			{		return m_siType == siType;						}
	SI32 GetType()						{		return m_siType;								}
};

class cltClickAndDrop_Item : public cltClickAndDrop
{
protected:
	cltItem		m_clItem;

public:
	cltClickAndDrop_Item(cltItem* pclItem)
		: cltClickAndDrop(CLICKANDDROP_TYPE_ITEM)
	{
		if(pclItem)			m_clItem.Set(pclItem);
	}

	inline cltItem* GetItem()		{		return &m_clItem;		}
};


enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };

struct stMsgBox
{
	CControllerMgr *pControllerMgr;
	TCHAR strCaption[ 128 ];
	TCHAR strText[ 512 ];
	SI16 siMsgBoxType;
	SI16 siMsgIndex;

	stMsgBox()
	{
		Init();
	}

	void Init()
	{	
		memset( this, 0, sizeof( stMsgBox ) );
		pControllerMgr = NULL;
	}

	void Set( CControllerMgr *ControllerMgr, TCHAR *Caption, TCHAR *Text, SI16 MsgBoxType, SI16 MsgIndex )
	{
		pControllerMgr = ControllerMgr;
		if ( Caption )
		{
			_tcsncpy( strCaption, Caption, 127 );
			strCaption[ 127 ] = NULL;
		}

		if ( Text )
		{
			_tcsncpy( strText, Text, 511 );
			strText[ 511 ] = NULL;
		}

		siMsgBoxType = MsgBoxType;
		siMsgIndex = MsgIndex;
	}
};

class CInterfaceMgr  
{
public:
	CInterfaceMgr();
	~CInterfaceMgr();

	CInterfaceImgMgr *GetInterfaceImgrMgr()
	{
		return m_pInterfaceImgMgr;
	}

	CControllerMgr *GetFirst()
	{
		return m_pFirst;
	}

	CControllerMgr *GetLast()
	{
		return m_pLast;
	}

	void FocusStatusInit();
	void MouseStatusInit();
	void CaptureStatusInit();
	void ItemDragStatusInit();

	void ClearFocusMgr();

	CControllerMgr *GetFouceMgr()
	{
		if ( IsExistInterface( m_pFocusMgr ) )
		{
			return m_pFocusMgr;
		}

		return NULL;
	}

	//void PushFront( CControllerMgr *pNode );
	void PushBack( CControllerMgr *pNode );
	CControllerMgr *Pop( SI16 DialogControllerID );	
	CControllerMgr *Pop( CControllerMgr *pNode );
	
	CControllerMgr *GetExistInterface( SI16 DialogControllerID );
	bool IsExistInterface( SI16 DialogControllerID );
	bool IsExistInterface( CControllerMgr * pControllerMgr );
	bool IsExistChildInterface( CControllerMgr * pControllerMgr );

	CControllerMgr *GetInterfaceAtPos( SI16 XPos, SI16 YPos );

	bool Add( SI16 DialogControllerID );
	bool Add( CControllerMgr *pNode );
	void Del( SI16 DialogControllerID );
	void Del( CControllerMgr *pNode );

	//void TopWindow( SI16 DialogControllerID );
	//void TopWindow( CControllerMgr *pNode );
	void MoveToFirst( CControllerMgr *pNode );

	void SetTopMostWindow( CControllerMgr *pNode, bool TopMostWindow );
	void SetModalDialog( CControllerMgr *pNode, bool ModalDialog );

	bool MsgProc( UINT iMsg, WPARAM &wParam, LPARAM &lParam );

	bool OnButtonDown( CControllerMgr *pControllerMgr, WPARAM wParam, LPARAM lParam );
	bool OnButtonUp( CControllerMgr *pControllerMgr, WPARAM wParam, LPARAM lParam );
	bool OnMouseMove( CControllerMgr *pControllerMgr, WPARAM wParam, LPARAM lParam, bool &MouseOverSet );

	bool IsItemDragMoveStatus()
	{
		return m_bItemDragMove;
	}
	void ItemBeginDrag( SI16 ImageFileIndex, SI16 ImageFontIndex );
	void ItemEndDrag();

	// CLICK AND DROP - by LEEKH 2008.02.27
	bool BeginClickAndDrop_Item(cltItem *pclItem, SI32 siDialogID, SI32 siControlID, SI32 siPara1 = 0, SI32 siPara2 = 0, SI32 siPara3 = 0, SI32 siPara4 = 0);
	void EndClickAndDrop();
	cltClickAndDrop* GetClickAndDrop()	{	return m_pclClickAndDrop;	}

	void CaptureStart( SI16 DialogControllerID, SI16 ControllerID );
	void CaptureEnd();

	void Draw( LPDIRECTDRAWSURFACE7 lpBackSurface, SI32 ScreenRX, SI32 ScreenRY );
	
	bool Is_LButtonDown();
	bool Is_RButtonDown();
	
	void UpdateMousePos( SI16 XPos, SI16 YPos )
	{
		m_PreMousePos.x = XPos;
		m_PreMousePos.y = YPos;
	}

	bool IsInInterfaceAraea();

	void Action( clock_t CurrentClock );
	
	CToolTip *GetToolTip()
	{
		return m_pToolTip;
	}

	void SetDrawToolTip( bool DrawToolTip )
	{
		m_bDrawToolTip = DrawToolTip;
		return;
	}

	void SetFontKind( TCHAR *FontKind )
	{	
		if ( FontKind )
		{
			_tcsncpy( m_strFontKind, FontKind, MAX_FONTKIND_NAME_LEN - 1 );
			m_strFontKind[ MAX_FONTKIND_NAME_LEN - 1 ] = NULL;
		}
		else
		{
			memset( m_strFontKind, 0, MAX_FONTKIND_NAME_LEN );
			_tcsncpy( m_strFontKind, DEFAULT_FONT_KIND, MAX_FONTKIND_NAME_LEN - 1 );
			m_strFontKind[ MAX_FONTKIND_NAME_LEN - 1 ] = NULL;
		}
	}

	TCHAR *GetFontKind()
	{
		return m_strFontKind;
	}

	void PushToDeleteDialog( CControllerMgr *pToDeleteDialog );
	bool DeleteDialog();

	void SetFocus( CControllerMgr* pControllerMgr, CControllerObj *pControllerObj );
	
	bool GetControllerAtPoint( CControllerMgr **pDialog, CControllerObj **pController );

	void SetAvailableScreenRect( SI16 XPos1, SI16 YPos1, SI16 XPos2, SI16 YPos2 )
	{
		m_rcAvailableScreen.left	=	XPos1;
		m_rcAvailableScreen.top		=	YPos1;
		
		m_rcAvailableScreen.right	=	XPos2;
		m_rcAvailableScreen.bottom	=	YPos2;

		m_rcAvailableMove.left		=	XPos1 + SCREEN_OFFSET_SIZE;
		m_rcAvailableMove.top		=	YPos1 + SCREEN_OFFSET_SIZE;

		m_rcAvailableMove.right		=	XPos2 - SCREEN_OFFSET_SIZE;
		m_rcAvailableMove.bottom	=	YPos2 - SCREEN_OFFSET_SIZE;
		
	}

	bool IsAvailableMove( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height );
	void SetSnapIfAvailable( SI16 &XPos, SI16 &YPos, SI16 Width, SI16 Height );

	bool IsModalStatus();
	CControllerMgr *GetModalDialog();
	bool IsExistModalDialog();

	DWORD GetTotalWidthCandidateData( HDC hDC, SI16 CandidateDataStartIndex, LPCANDIDATELIST pCandidateList, stCandidateData *pCandidateDataList );
	DWORD GetMaxWidthCandidateData( HDC hDC, SI16 CandidateDataStartIndex, LPCANDIDATELIST pCandidateList, stCandidateData *pCandidateDataList );

	void SetCandidateOpenStatus( bool CandidateOepnStatus )
	{	
		if ( ! m_bCandidateOpenStatus )
		{
			m_bCandidateOpenStatus = CandidateOepnStatus;
		}
	}

	void SetCandidateDrawOption( SI32 XPos, SI32 YPos, SI32 CompStrWidth, SI32 CompStrHeight, SI16 AttrLineStartPos )
	{
		m_siCandidateXPos = XPos;
		m_siCandidateYPos = YPos;
		m_siCompStrWidth = CompStrWidth;
		m_siCompStrHeight = CompStrHeight;
		m_siAttrLineStartPos = AttrLineStartPos;
	}
	
	void SetCandidateWindow( LPCANDIDATELIST pCandidateList, SI16 CandidateStartIndex, bool VerticalCandidateWindow, stCandidateData *pCandidateDataList, 
		SI32 FontHeight, SI32 FontWidth, SI32 FontWeight, TCHAR *FontKind, COLORREF FontColor );

	void SetMsgBox( stMsgBox *pstMsgBox, BYTE *pData, SI16 DataLen );

	RECT* GetAvailableScreenRect()
	{
		return &m_rcAvailableScreen;
	}
	void CloseWindowEceptBaseWindow();
	void CloseAllWindow();
	bool HasFocusInEditBox();
	
	//////////////////////////////////// IME 관련 메소드( edit.h 에서 가져옴) /////////
	WORD GetIMELanguage( HKL KeyboardLayout )
	{
		return LOWORD( KeyboardLayout );
	}
	WORD GetIMEPrimaryLanguage( HKL KeyboardLayout )
	{
		return PRIMARYLANGID( LOWORD( KeyboardLayout ) );
	}
	WORD GetIMESubLanguage( HKL KeyboardLayout )
	{
		return SUBLANGID( LOWORD( KeyboardLayout ) );
	}

	void CheckToggleState();
	IMESTATE GetToggleState(){ return m_dwImeState; };

	void	SetAlreadyOpenToolTip(BOOL bOpen){ m_bAlreadyOpenToolTip = bOpen;  };
	BOOL	IsToolTipAlreadyOpen(){return m_bAlreadyOpenToolTip;  };
	//void DrawImeState();
	/////////////////////////////////////////////////////////////////////////////////

	BOOL m_bAlreadyOpenToolTip;

	bool ClickAndDrop_Start();
	bool ClickAndDrop_End();

private:
	CInterfaceImgMgr *m_pInterfaceImgMgr;

	CControllerMgr *m_pFirst;
	CControllerMgr *m_pLast;
	
	SI16 m_siCurrentFocusDialogControllerID;
	SI16 m_siCurrentFocusControllerID;
	SI16 m_siCurrentFocusKind;
	
	CControllerMgr	*m_pFocusMgr;
	bool m_bMoveableStatus;
	
	bool m_bLButtonDown;
	bool m_bRButtonDown;

	SI16 m_siCurrentMouseDialogControllerID;
	SI16 m_siCurrentMouseControllerID;
	SI16 m_siCurrentMouseKind;
	
	POINT m_PreMousePos;
	
	bool m_bCaptureStatus;
	SI16 m_siCaptureDialogControllerID;
	SI16 m_siCaptureControllerID;

	bool m_bItemDragMove;
	SI16 m_siImageFileIndex;
	SI16 m_siImageFontIndex;
	
	CToolTip *m_pToolTip;					// 툴팁
	bool m_bDrawToolTip;
	
	TCHAR m_strFontKind[ MAX_FONTKIND_NAME_LEN ];

	CControllerMgr *m_pToDeleteDialog[ MAX_DELETE_DIALOG_NUM ];

	RECT m_rcAvailableScreen;
	RECT m_rcAvailableMove;

	bool m_bModalStatus;
	
	bool	m_bCandidateOpenStatus;
	RGB16	*m_ImgCandidateEncBuffer;
	SI32	m_siCandidateStrWidth;
	SI32	m_siCandidateStrHeight;
	SI32	m_siCandidateXPos;
	SI32	m_siCandidateYPos;
	SI32	m_siCompStrWidth;
	SI32	m_siCompStrHeight;
	SI32	m_siAttrLineStartPos;
	
	IMESTATE	m_dwImeState;

	CControllerSurfaceObj	*m_pCandidateSurfaceObj;

	cltClickAndDrop*		m_pclClickAndDrop;
};

#endif