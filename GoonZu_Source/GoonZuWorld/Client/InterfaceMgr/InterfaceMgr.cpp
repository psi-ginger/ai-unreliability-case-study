#include "InterfaceMgr.h"
#include "ControllerMgr.h"
#include "ControllerObj.h"
#include "InterfaceImgMgr/InterfaceImgMgr.h"
#include "ControllerSurfaceObj.h"

#include "Interface/ToolTip.h"
#include "Interface/Edit.h"
#include "NMsgBoxDlg.h"
#include "DLGPositionSave/DLGPositionSave.h"

#include <Client.h>
extern cltCommonLogic *pclClient;

#include <zmouse.h>
#include <dimm.h>

#include "./ninterface/NNewInventory/NNewInventory.h"

INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC );
BOOL (WINAPI * _ImmUnlockIMC)( HIMC );
LPVOID (WINAPI * _ImmLockIMCC)( HIMCC );
BOOL (WINAPI * _ImmUnlockIMCC)( HIMCC );
HIMC (WINAPI * _ImmGetContext)( HWND );
BOOL (WINAPI * _ImmReleaseContext)( HWND, HIMC );
UINT (WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT );

CInterfaceMgr::CInterfaceMgr()
{
	//int nParam;
	//SystemParametersInfo( SPI_SETFONTSMOOTHING, FALSE, &nParam, FALSE );

	m_pInterfaceImgMgr = new CInterfaceImgMgr();

	m_pFocusMgr = NULL;
	m_bMoveableStatus = false;
	
	FocusStatusInit();
	MouseStatusInit();
	CaptureStatusInit();
	ItemDragStatusInit();

	m_pFirst = NULL;
	m_pLast = NULL;

	m_pToolTip = new CToolTip();
	m_pToolTip->Create( MAX_TOOLTIP_XSIZE, MAX_TOOLTIP_YSIZE );

	m_bDrawToolTip = false;
	

	_tcsncpy( m_strFontKind, DEFAULT_FONT_KIND, MAX_FONTKIND_NAME_LEN - 1 );
	m_strFontKind[ MAX_FONTKIND_NAME_LEN - 1 ] = NULL;

	for ( SI16 i = 0; i < MAX_DELETE_DIALOG_NUM; ++i )
	{
		m_pToDeleteDialog[ i ] = NULL;
	}
	
	m_PreMousePos.x = 0;
	m_PreMousePos.y = 0;

	SetRect( &m_rcAvailableScreen, 0, 0, 0, 0 );
	SetRect( &m_rcAvailableMove, 0, 0, 0, 0 );

	m_bModalStatus = false;

	m_bCandidateOpenStatus = false;
	m_ImgCandidateEncBuffer = NULL;
	m_siCandidateStrWidth = 0;
	m_siCandidateStrHeight = 0;
	m_siAttrLineStartPos = 0;
	m_dwImeState = IMEUI_STATE_OFF;

	m_pCandidateSurfaceObj = new CControllerSurfaceObj();
	m_pCandidateSurfaceObj->Create( 800, 600 );

	m_bAlreadyOpenToolTip = false;

	m_pclClickAndDrop		= NULL;	
}

CInterfaceMgr::~CInterfaceMgr()
{
	SAFE_DELETE_A(m_ImgCandidateEncBuffer);
	SAFE_DELETE( m_pToolTip );
	SAFE_DELETE( m_pCandidateSurfaceObj );
	SAFE_DELETE( m_pInterfaceImgMgr );
	
	CControllerMgr *pTemp = GetFirst();
	CControllerMgr *pTemp1 = NULL;

	while( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();
		
		SAFE_DELETE( pTemp1 );
	}

	m_pFirst = NULL;
	m_pLast = NULL;
}

void CInterfaceMgr::FocusStatusInit()
{
	m_siCurrentFocusDialogControllerID = 0;
	m_siCurrentFocusControllerID = 0;
	m_siCurrentFocusKind = 0;
	
	ClearFocusMgr();

	return;
}

void CInterfaceMgr::MouseStatusInit()
{
	m_bLButtonDown = false;
	m_bRButtonDown = false;
	
	m_siCurrentMouseDialogControllerID = 0;
	m_siCurrentMouseControllerID = 0;
	m_siCurrentMouseKind = 0;

	UpdateMousePos( 0, 0 );

	return;
}

void CInterfaceMgr::CaptureStatusInit()
{
	m_bCaptureStatus = false;
	m_siCaptureDialogControllerID = 0;
	m_siCaptureControllerID = 0;

	return;
}

void CInterfaceMgr::ItemDragStatusInit()
{
	m_bItemDragMove = false;
	m_siImageFileIndex = 0;
	m_siImageFontIndex = 0;
	ReleaseCapture();

	return;
}

void CInterfaceMgr::ClearFocusMgr()
{
	if ( m_pFocusMgr )
	{
		m_pFocusMgr->SetFocus( false );
		m_pFocusMgr = NULL;
	}

	m_bMoveableStatus = false;

	return;
}

/*
void CInterfaceMgr::PushFront( CControllerMgr *pNode )
{
	if ( pNode == NULL )
	{
		return;
	}

	if ( m_pFirst )		// 1개라도 존재하는 경우.
	{
		pNode->SetNext( m_pFirst );
		pNode->SetPre( NULL );
		m_pFirst->SetPre( pNode );
		m_pFirst = pNode;
	}
	else				// 아무것도 없는 경우.
	{
		m_pFirst = pNode;
		m_pLast = pNode;
	}
	
	return;
}
*/

void CInterfaceMgr::PushBack( CControllerMgr *pNode )
{
	if ( pNode == NULL )
	{
		return;
	}

	if ( m_pLast )		// 1개라도 존재하는 경우.
	{
		m_pLast->SetNext( pNode );
		pNode->SetPre( m_pLast );
		pNode->SetNext( NULL );
		m_pLast = pNode;
	}
	else				// 아무것도 없는 경우.
	{
		m_pFirst = pNode;
		m_pLast = pNode;
	}

	return;
}

CControllerMgr *CInterfaceMgr::Pop( SI16 DialogControllerID )
{
	if ( DialogControllerID < 0 )
	{
		return NULL;
	}

	CControllerMgr *pNode = GetExistInterface( DialogControllerID );
	
	if ( pNode == NULL )
	{
		return NULL;
	}

	/*
	if ( m_pFocusMgr == pNode )
	{
		m_pFocusMgr = NULL;
	}
	*/

	if ( pNode->GetPre() )
	{
		pNode->GetPre()->SetNext( pNode->GetNext() );
	}
	else				// pNode 가 m_pFirst 인 경우이지...
	{
		if ( pNode->GetNext() )
		{
			m_pFirst = pNode->GetNext();
			m_pFirst->SetPre( NULL );
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}

	if ( pNode->GetNext() )
	{
		pNode->GetNext()->SetPre( pNode->GetPre() );
	}
	else				// pNode 가 m_pLast 인 경우이지...
	{
		if ( pNode->GetPre() )
		{
			m_pLast = pNode->GetPre();
			m_pLast->SetNext( NULL );
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}

	pNode->SetPre( NULL );
	pNode->SetNext( NULL );

	return pNode;
}

CControllerMgr *CInterfaceMgr::Pop( CControllerMgr *pNode )
{
	if ( pNode == NULL )
	{
		return NULL;
	}

	/*
	if ( m_pFocusMgr == pNode )
	{
		m_pFocusMgr = NULL;
	}
	*/

	if ( pNode->GetPre() )
	{
		pNode->GetPre()->SetNext( pNode->GetNext() );
	}
	else				// pNode 가 m_pFirst 인 경우이지...
	{
		if ( pNode->GetNext() )
		{
			m_pFirst = pNode->GetNext();
			m_pFirst->SetPre( NULL );
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}

	if ( pNode->GetNext() )
	{
		pNode->GetNext()->SetPre( pNode->GetPre() );
	}
	else				// pNode 가 m_pLast 인 경우이지...
	{
		if ( pNode->GetPre() )
		{
			m_pLast = pNode->GetPre();
			m_pLast->SetNext( NULL );
		}
		else
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}

	pNode->SetPre( NULL );
	pNode->SetNext( NULL );

	return pNode;
}

CControllerMgr *CInterfaceMgr::GetExistInterface( SI16 DialogControllerID )
{
	if ( DialogControllerID < 0 )
	{
		return NULL;
	}

	CControllerMgr *pTemp = GetFirst();

	while ( pTemp )
	{
		if ( pTemp->GetDialogControllerID() == DialogControllerID )
		{
			return pTemp;
		}

		pTemp = pTemp->GetNext();
	}

	return NULL;
}

bool CInterfaceMgr::IsExistInterface( SI16 DialogControllerID )
{
	if ( DialogControllerID < 0 )
	{
		return false;
	}

	CControllerMgr *pTemp = GetFirst();
	while ( pTemp )
	{
		if ( pTemp->GetDialogControllerID() == DialogControllerID )
		{
			return true;
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}

bool CInterfaceMgr::IsExistInterface( CControllerMgr * pControllerMgr )
{
	if ( pControllerMgr == NULL )
	{
		return false;
	}

	CControllerMgr *pTemp = GetFirst();
	while ( pTemp )
	{
		if ( pTemp == pControllerMgr )
		{
			return true;
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}

bool CInterfaceMgr::IsExistChildInterface( CControllerMgr * pControllerMgr )
{
	if ( pControllerMgr == NULL )
	{
		return false;
	}

	CControllerMgr *pTemp = GetFirst();
	while ( pTemp )
	{
		// 메인 다이얼로그
		if ( pTemp == pControllerMgr )
		{
			return true;
		}

		// 자식 다이얼로그도 검사한다.
		for (SI32 i = 0; i < MAX_HAVE_CHILD_DIALOG_NUM; ++i )
		{
			CControllerMgr* pChild = pTemp->GetChildDialog( i );
			if ( pChild && pChild == pControllerMgr)
			{
				return true;
			}
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}

CControllerMgr *CInterfaceMgr::GetInterfaceAtPos( SI16 XPos, SI16 YPos )
{
	CControllerMgr *pTemp = GetFirst();
	while ( pTemp )
	{
		if ( pTemp->IsInArea( XPos, YPos ) )
		{
			return pTemp;
		}

		pTemp = pTemp->GetNext();
	}

	return NULL;
}

bool CInterfaceMgr::Add( SI16 DialogControllerID )
{
	if ( DialogControllerID < 0 )
	{
		return false;
	}

	if ( IsExistInterface( DialogControllerID ) )
	{
		return false;
	}

	CControllerMgr *pTemp = new CControllerMgr();
	pTemp->SetDialogControllerID( DialogControllerID );

	PushBack( pTemp );
	MoveToFirst( pTemp );
	

	return true;
}

bool CInterfaceMgr::Add( CControllerMgr *pNode )
{
	if ( pNode == NULL )
	{
		return false;
	}

	PushBack( pNode );
	MoveToFirst( pNode );

	return true;
}

void CInterfaceMgr::Del( SI16 DialogControllerID )
{
	if ( DialogControllerID < 0 )
	{
		return;
	}

	// 리스트에서 빼주면 되지.. 굳이 아래에 주석처리 된 곳처럼 삭제할 필요는 없음... 
	// 삭제는 만든 곳에서 해줘야지... 만든 곳은 여기가 아님으로... 삭제는 거기서 해 줄것.
	Pop( DialogControllerID );
	
	/*
	CControllerMgr *pTemp = Pop( DialogControllerID );
	
	if ( pTemp )
	{
		delete pTemp;
		pTemp = NULL;
	}
	*/
}

void CInterfaceMgr::Del( CControllerMgr *pNode )
{
	Pop( pNode );

	return;
}

/*
void CInterfaceMgr::TopWindow( SI16 DialogControllerID )
{	
	CControllerMgr *pTemp = Pop( DialogControllerID );
	if ( pTemp )
	{	
		PushFront( pTemp );
	}

	return;
}

void CInterfaceMgr::TopWindow( CControllerMgr *pNode )
{
	CControllerMgr *pTemp = Pop( pNode );
	if ( pTemp )
	{
		PushBack( pTemp );
		MoveToFirst( pTemp );
	}
	
	return;
}
*/

void CInterfaceMgr::MoveToFirst( CControllerMgr *pNode )
{
	if ( pNode == NULL )
	{
		return;
	}

	if ( ! IsExistInterface( pNode ) )
	{
		return;
	}

	if ( pNode->IsTopMostWindow() )
	{
		return;
	}

	CControllerMgr *pTempNode = Pop( pNode );
	
	if ( pTempNode )
	{
		CControllerMgr *pTemp = GetFirst();
		
		while ( pTemp )
		{
			if ( ! pTemp->IsTopMostWindow() )
			{
				if ( pTemp == GetFirst() )
				{
					pTempNode->SetNext( pTemp );
					pTemp->SetPre( pTempNode );
					m_pFirst = pTempNode;
				}
				else
				{
					pTempNode->SetPre( pTemp->GetPre() );
					pTempNode->SetNext( pTemp );

					pTemp->GetPre()->SetNext( pTempNode );
					pTemp->SetPre( pTempNode );
				}

				break;
			}

			pTemp = pTemp->GetNext();
		}

		if ( pTemp == NULL )		// m_pFirst 가 NULL 인 경우와, 전부 TopMostWindow 인 경우임
		{
			PushBack( pTempNode );
		}

		if ( pTempNode != m_pFocusMgr )
		{
			m_bMoveableStatus = false;
			m_pFocusMgr = pTempNode;
		}
	}

	return;
}

bool CInterfaceMgr::MsgProc( UINT iMsg, WPARAM &wParam, LPARAM &lParam )
{	
	cltClient *pclclient = (cltClient *)pclClient;

	bool MouseOverSet = false;

	DeleteDialog();

	if ( IsModalStatus() )
	{	
		CControllerMgr *pDialog = GetModalDialog();
		if ( pDialog )
		{
			if ( pDialog->MsgProc( iMsg ,wParam, lParam, MouseOverSet ) )
			{
				if ( iMsg == WM_LBUTTONUP )
				{
					SI32 XPos1 = LOWORD(lParam), YPos1 = HIWORD(lParam);
					((cltClient*)pclclient)->SetMousePos( &XPos1, &YPos1 );
					UpdateMousePos( XPos1, YPos1 );
				}
				return true;
			}
			else
			{
				switch( iMsg )
				{
				case WM_LBUTTONDOWN:
				case WM_LBUTTONUP:
				case WM_MBUTTONDOWN:
				case WM_MBUTTONUP:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONUP:
				case WM_LBUTTONDBLCLK:
				case WM_MBUTTONDBLCLK:
				case WM_RBUTTONDBLCLK:
					{
						SI32 XPos1 = LOWORD(lParam), YPos1 = HIWORD(lParam);
						((cltClient*)pclclient)->SetMousePos( &XPos1, &YPos1 );
						UpdateMousePos( XPos1, YPos1 );
					}
					return true;
				case WM_MOUSEMOVE:
					{
						SI32 XPos1 = LOWORD(lParam), YPos1 = HIWORD(lParam);
						((cltClient*)pclclient)->SetMousePos( &XPos1, &YPos1 );
						UpdateMousePos( XPos1, YPos1 );
					}
					return false;
				}
			}
			
			return false;
		}
		else
		{
			m_bModalStatus = false;
		}
	}

	DeleteDialog();

	CControllerMgr *pTemp = GetFirst();	

	switch( iMsg )
	{	
	case 0x0281:
		{
			lParam = 0;
			return false;
		}
		break;
	case 256:
		{
			if ( wParam == 27 )
			{
				if( m_pFocusMgr && IsExistInterface( m_pFocusMgr ) && m_pFocusMgr->HaveCloseButton() )
				{
					CControllerObj *pControl = m_pFocusMgr->GetControl( COMMON_DIALOG_CLOSEBUTTON );
					if ( pControl )
					{
						m_pFocusMgr->SendEvent( COMMON_DIALOG_CLOSEBUTTON, true, pControl );
					}
					return true;
				}
			}
		}
		break;
	}

	/*
	if ( IsExistInterface( m_pFocusMgr ) )
	{
		if ( m_pFocusMgr->MsgProc( iMsg, wParam, lParam, MouseOverSet ) )
		{
			DeleteDialog();
			
			return true;
		}
	}
	*/

	// 클릭 앤 드랍을 먼저 처리한다.
	CControllerMgr *pDialog = NULL;
	CControllerObj *pControllerObj = NULL;

	if( pclClient->IsCountrySwitch(Switch_ClickAndDrop) && m_pclClickAndDrop )
	{	
		// 오른쪽 버튼은 클릭앤 드랍을 취소 시킨다.
		switch ( iMsg )
		{	
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
			{
				EndClickAndDrop();
				return true;
			}
			break;
		}

		if( pclclient->m_pInterfaceMgr->GetControllerAtPoint( &pDialog, &pControllerObj ) == false )
		{
			// 컨트롤이 없으므로 DROPITEM을 수행한다.
			switch ( iMsg )
			{	
			case WM_LBUTTONUP:
				{
					bool bDropItem = false;
					// DropItem
					switch( m_pclClickAndDrop->m_siSrcDialogID )
					{
					case NNEWINVENTORY_DLG:
					case NNEWINVENTORY_HORSE_DLG:
					case NNEWINVENTORY_BAG0_DLG:
					case NNEWINVENTORY_BAG1_DLG:
						{
							bDropItem = true;
						}
						break;
					}

					if(bDropItem)
					{
						SI32 siPos = m_pclClickAndDrop->m_siPara1;
						if( siPos >= 0 && siPos < MAX_ITEM_PER_PERSON )
						{
							if( m_pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) )
							{
								cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)m_pclClickAndDrop;
								if(pclclient->m_pDialog[NNEWINVENTORY_DLG] != NULL)
									((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG])->DropItem_New(pclClickAndDrop_Item->GetItem(), siPos);
							}
						}
					}

					EndClickAndDrop();
					return true;
				}
				break;
			}
		}
		else
		{
			if( pControllerObj )
			{
				switch ( iMsg )
				{	
				case WM_LBUTTONUP:
				case WM_LBUTTONDOWN:
					{
						switch ( pControllerObj->GetControllerKind() )
						{	
						case CONTROLLER_KIND_LISTVIEW:
							{
								// 클릭 앤 드랍을 허용하는 컨트롤인 경우
							}
							break;
						default:
							{
								// End ClickAndDrop
								EndClickAndDrop();
								return true;
							}
							break;
						}
					}
					break;
				}				
			}
		}
	}

	while ( pTemp )
	{	
		if ( pTemp->MsgProc( iMsg, wParam, lParam, MouseOverSet ) )		// 이 부분의 return 값이 true 라면, 다른 인터페이스에서는 포커스를 없애 줘 버려야 함. 추가 해야 됨.
		{
			if ( DeleteDialog() )
			{
			}
			else
			{
				if ( iMsg != WM_LBUTTONUP && iMsg != WM_RBUTTONUP )
				{
					if ( m_pFocusMgr && m_pFocusMgr != pTemp )
					{
						if ( IsExistInterface( m_pFocusMgr ) )
						{
							m_pFocusMgr->SetFocus( false );
						}
					}
					m_pFocusMgr = pTemp;
					m_pFocusMgr->SetFocus( true );

					m_bMoveableStatus = false;

					MoveToFirst( pTemp );
				}
			}
		//	SetAlreadyOpenToolTip(FALSE);
			return true;
		}
		else
		{
			switch( iMsg )
			{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
				{
					if ( OnButtonDown( pTemp, wParam, lParam ) )
					{
						return true;
					}
				}
				break;
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				{
					if ( OnButtonUp( pTemp, wParam, lParam ) )
					{
						return true;
					}
				}                                                                                 
				break;
			case WM_MOUSEMOVE:
				{
					if ( OnMouseMove( pTemp, wParam, lParam, MouseOverSet ) )
					{
						return true;
					}
				}
				break;
			default:
				{
				}
				break;
			}
		}
		
		pTemp = pTemp->GetNext();
	}


	m_bAlreadyOpenToolTip = false;
	//ClearFocusMgr();

	switch ( iMsg )
	{	
	case WM_MOUSEMOVE:
		{
			SI32 XPos1 = LOWORD(lParam), YPos1 = HIWORD(lParam);
			((cltClient*)pclclient)->SetMousePos( &XPos1, &YPos1 );
			UpdateMousePos( XPos1, YPos1 );
		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		{
			ReleaseCapture();
			ClearFocusMgr();
			m_bMoveableStatus = false;
			SI32 XPos1 = LOWORD(lParam), YPos1 = HIWORD(lParam);
			((cltClient*)pclclient)->SetMousePos( &XPos1, &YPos1 );
			UpdateMousePos( XPos1, YPos1 );
		}
		break;
	}
	
	return false;
}

bool CInterfaceMgr::OnButtonDown( CControllerMgr *pControllerMgr, WPARAM wParam, LPARAM lParam )
{
	if ( pControllerMgr == NULL )
	{
		return false;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	SI16 XPos1 = LOWORD( lParam );
	SI16 YPos1 = HIWORD( lParam );

	pclclient->MouseMoveAction( XPos1, YPos1 );

	SI16 XPos = XPos1 + MAP_TILE_XSIZE;//pclclient->GetScreenMouseX();// + pclclient->GetScreenRX();
	SI16 YPos = YPos1 + MAP_TILE_YSIZE;//pclclient->GetScreenMouseY();// + pclclient->GetScreenRY();

	((cltClient*)pclclient)->SetMousePos( (SI32*)&XPos1, (SI32*)&YPos1 );
	UpdateMousePos( XPos1, YPos1 );

	if ( pControllerMgr->IsInArea( XPos, YPos ) )
	{
		if ( m_pFocusMgr && m_pFocusMgr != pControllerMgr && IsExistInterface( m_pFocusMgr ) )
		{
			m_pFocusMgr->SetFocus( false );
		}

		m_pFocusMgr = pControllerMgr;
		m_pFocusMgr->SetFocus( true );

		m_bMoveableStatus = true;

		// TopWindow( m_pFocusMgr );

		MoveToFirst( pControllerMgr );
		return true;
	}

	return false;
}

bool CInterfaceMgr::OnButtonUp( CControllerMgr *pControllerMgr, WPARAM wParam, LPARAM lParam )
{
	if ( pControllerMgr == NULL )
	{
		return false;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	SI16 XPos1 = LOWORD(lParam);
	SI16 YPos1 = HIWORD(lParam);

	pclclient->MouseMoveAction( XPos1, YPos1 );

	SI16 XPos = XPos1 + MAP_TILE_XSIZE;
	SI16 YPos = YPos1 + MAP_TILE_YSIZE;

	if ( pControllerMgr->IsInArea( XPos, YPos ) )
	{
		m_bMoveableStatus = false;

		if ( m_pFocusMgr && m_pFocusMgr == pControllerMgr )
		{
			ReleaseCapture();
			//ClearFocusMgr();
			return true;
		}

		if ( pControllerMgr->IsInArea( XPos, YPos ) )
		{	
			ReleaseCapture();
			//ClearFocusMgr();
			return true;
		}

		if ( IsItemDragMoveStatus() )
		{
			ItemDragStatusInit();
		}
	}	

	return false;
}

bool CInterfaceMgr::OnMouseMove( CControllerMgr *pControllerMgr, WPARAM wParam, LPARAM lParam, bool &MouseOverSet )
{
	if ( pControllerMgr == NULL )
	{
		return false;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	SI32 XPos = LOWORD(lParam);
	SI32 YPos = HIWORD(lParam);

	pclclient->MouseMoveAction( XPos, YPos );
	((cltClient*)pclclient)->SetMousePos( &XPos, &YPos );

	SI16 DialogMoveXOffset = 0;
	SI16 DialogMoveYOffset = 0;


	if ( m_pFocusMgr && pControllerMgr == m_pFocusMgr )
	{
		// 이동 가능한 윈도우이면 이동 시킴
		if ( m_bMoveableStatus && m_pFocusMgr->IsMoveable() )
		{	
			DialogMoveXOffset = XPos - m_PreMousePos.x;
			DialogMoveYOffset = YPos - m_PreMousePos.y;

			if ( DialogMoveXOffset || DialogMoveYOffset )
			{
 				SetCapture( pclClient->GetHwnd() );
				m_pFocusMgr->MoveDialog( DialogMoveXOffset, DialogMoveYOffset );
				UpdateMousePos( XPos, YPos );
				return true;
			}			
		}		
	}

	XPos += MAP_TILE_XSIZE;
	YPos += MAP_TILE_YSIZE;

	if ( ! MouseOverSet && pControllerMgr->IsInArea( XPos, YPos ) )
	{
		MouseOverSet = true;
	}

	return false;
}

void CInterfaceMgr::ItemBeginDrag( SI16 ImageFileIndex, SI16 ImageFontIndex )
{
	m_bItemDragMove = true;
	m_siImageFileIndex = ImageFileIndex;
	m_siImageFontIndex = ImageFontIndex;

	SetCapture( pclClient->GetHwnd() );

	return;
}

void CInterfaceMgr::ItemEndDrag()
{
	ItemDragStatusInit();
	return;
}

bool CInterfaceMgr::BeginClickAndDrop_Item(cltItem *pclItem, SI32 siDialogID, SI32 siControlID, SI32 siPara1 /* = 0 */, SI32 siPara2 /* = 0 */, SI32 siPara3 /* = 0 */, SI32 siPara4 /* = 0 */)
{
	if(m_pclClickAndDrop != NULL)			return false;
	if(pclItem == NULL)						return false;

	m_pclClickAndDrop = new cltClickAndDrop_Item(pclItem);
	if(m_pclClickAndDrop == NULL)			return false;

	m_pclClickAndDrop->m_siSrcDialogID	= siDialogID;
	m_pclClickAndDrop->m_siSrcControlID = siControlID;

	m_pclClickAndDrop->m_siPara1 = siPara1;
	m_pclClickAndDrop->m_siPara2 = siPara2;
	m_pclClickAndDrop->m_siPara3 = siPara3;
	m_pclClickAndDrop->m_siPara4 = siPara4;

	return true;
}

void CInterfaceMgr::EndClickAndDrop()
{
	SAFE_DELETE(m_pclClickAndDrop);
}

// Mouse Capture
void CInterfaceMgr::CaptureStart( SI16 DialogControllerID, SI16 ControllerID )
{
	if ( DialogControllerID < 0 )
	{
		return;
	}

	if ( m_bCaptureStatus )
	{
		MessageBox( NULL, TEXT("Already Capture"), TEXT("Already Capture"), MB_OK );
		return;
	}

	m_bCaptureStatus = true;

	if ( ControllerID > 0 )
	{
		m_siCaptureDialogControllerID = DialogControllerID;
		m_siCaptureControllerID = ControllerID;
	}
	else
	{
		m_siCaptureDialogControllerID = DialogControllerID;
		m_siCaptureControllerID = 0;
	}

	return;
}

// Mouse Capture Release
void CInterfaceMgr::CaptureEnd()
{
	m_bCaptureStatus = false;
	m_siCaptureDialogControllerID = 0;
	m_siCaptureControllerID = 0;

	return;
}

// 뒤에서부터 그려줌.
void CInterfaceMgr::Draw( LPDIRECTDRAWSURFACE7 lpBackSurface, SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = (cltClient *)pclClient;

	m_bDrawToolTip = false;
	m_bCandidateOpenStatus = false;

	CControllerMgr *pTemp = GetLast();

	while ( pTemp )
	{
		if ( pTemp->IsShow() )
		{
			if ( GP.LockSurface(lpBackSurface)==TRUE)
			{
				if( pTemp->GetFadeInTime() > 0 )
				{
					if( pTemp->GetDialogControllerID() == 230 )
					{
						int a = 0;
					}
					DWORD dwElaspedTime = GetTickCount() - pTemp->GetShowStartTime();
					if( dwElaspedTime <= (DWORD)pTemp->GetFadeInTime() )
					{
						pTemp->DrawTransparent( ScreenRX, ScreenRY, dwElaspedTime * 100 / pTemp->GetFadeInTime() );
					}
					else
					{
						pTemp->Draw( ScreenRX, ScreenRY );
					}
				}
				else
				{
					pTemp->Draw( ScreenRX, ScreenRY );
				}

				GP.UnlockSurface(lpBackSurface);
			}

			pTemp->DirectDrawToBackScreen( lpBackSurface, ScreenRX, ScreenRY );

		}

		pTemp = pTemp->GetPre();
	}

	if ( GP.LockSurface(lpBackSurface)==TRUE)
	{
		if(pclClient->IsCountrySwitch(Switch_ClickAndDrop))
		{
			if( m_pclClickAndDrop )
			{
				POINT mousePoint;
				GetCursorPos( &mousePoint );
				ScreenToClient( pclclient->GetHwnd(), &mousePoint );

				switch(m_pclClickAndDrop->GetType())
				{
				case CLICKANDDROP_TYPE_ITEM:
					{
						SI32 siItemUnique = ((cltClickAndDrop_Item*)m_pclClickAndDrop)->GetItem()->siUnique;
						if( siItemUnique > 0 )
						{
							SI32 GlobalImageIndex;
							SI32 GlobalImageFont;
							if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &GlobalImageIndex, &GlobalImageFont ) )
							{
								TSpr *pDragItemSpr = pclclient->pclGImg->GetGlobalSpr( GlobalImageIndex );
								GP.PutSpr( pDragItemSpr, pclclient->GetDrawStartX() + mousePoint.x - ITEM_IMAGE_SIZE_WIDTH / 2, pclclient->GetDrawStartY() + mousePoint.y - ITEM_IMAGE_SIZE_HEIGHT / 2, GlobalImageFont );
							}
						}
					}
					break;
				}
			}
		}
		else
		{
			if ( IsItemDragMoveStatus() )
			{
				POINT mousePoint;
				GetCursorPos( &mousePoint );
				ScreenToClient( pclclient->GetHwnd(), &mousePoint );

				TSpr *pDragItemSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageFileIndex );
				GP.PutSpr( pDragItemSpr, pclclient->GetDrawStartX() + mousePoint.x - ITEM_IMAGE_SIZE_WIDTH / 2, pclclient->GetDrawStartY() + mousePoint.y - ITEM_IMAGE_SIZE_HEIGHT / 2, m_siImageFontIndex );
			}
		}

		if ( m_bDrawToolTip && ! IsModalStatus() )
		{
			POINT mousePoint;
			GetCursorPos( &mousePoint );
			ScreenToClient( pclclient->GetHwnd(), &mousePoint );

			GetToolTip()->Draw( pclclient->GetDrawStartX() + mousePoint.x, pclclient->GetDrawStartY() + mousePoint.y );
			
		}
		GP.UnlockSurface(lpBackSurface);
	}

	if ( m_bCandidateOpenStatus )
	{
		if ( GP.LockSurface( lpBackSurface ) == TRUE )
		{
			//TSpr *pCandidateWindowSpr = GetInterfaceImgrMgr()->LoadInterfaceImg( EDIT_CANDIDATEWINDOWIMAGE_FILENAME );

   			if ( m_siCandidateYPos + m_siCandidateStrHeight > pclclient->GetScreenYSize() - pclclient->GetScreenStatusBarYSize() )
			{
				m_siCandidateYPos -= (m_siCandidateStrHeight) ;//pclclient->GetScreenYSize() - pclclient->GetScreenStatusBarYSize() - m_siCandidateStrHeight;
			}
			else
			{
                m_siCandidateYPos += m_siCompStrHeight;
			}
			if(m_ImgCandidateEncBuffer != NULL)
			{
				GP.PutXpr16( m_siCandidateXPos + m_siAttrLineStartPos , m_siCandidateYPos, 
					m_siCandidateStrWidth, m_siCandidateStrHeight,
					m_ImgCandidateEncBuffer,
					RGB16_TRANS );
			}


			GP.UnlockSurface(lpBackSurface);
		}
	}


	return;
}

bool CInterfaceMgr::Is_LButtonDown()
{
	return m_bLButtonDown;
}

bool CInterfaceMgr::Is_RButtonDown()
{
	return m_bRButtonDown;
}

bool CInterfaceMgr::IsInInterfaceAraea()
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	POINT mousePoint;
	GetCursorPos( &mousePoint );
	ScreenToClient( pclclient->GetHwnd(), &mousePoint );
	mousePoint.x += MAP_TILE_XSIZE;
	mousePoint.y += MAP_TILE_YSIZE;

	CControllerMgr *pTemp = GetFirst();
	
	while ( pTemp )
	{
		if ( pTemp->IsShow() && pTemp->IsInArea( mousePoint.x, mousePoint.y ) )		// 이 부분의 return 값이 true 라면, 다른 인터페이스에서는 포커스를 없애 줘 버려야 함. 추가 해야 됨.
		{
			return true;
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}


void CInterfaceMgr::Action( clock_t CurrentClock )
{
	CControllerMgr *pTemp = GetFirst();

	while ( pTemp )
	{
		if ( pTemp->IsTimerOn() )		// 이 부분의 return 값이 true 라면, 다른 인터페이스에서는 포커스를 없애 줘 버려야 함. 추가 해야 됨.
		{
			pTemp->ControllerMgrAction( CurrentClock );
		}

		pTemp = pTemp->GetNext();
	}

	m_pInterfaceImgMgr->Action();

	return;
}

void CInterfaceMgr::PushToDeleteDialog( CControllerMgr *pToDeleteDialog )
{
	if ( pToDeleteDialog == NULL )
	{
		return;
	}

	for ( SI16 i = 0; i < MAX_DELETE_DIALOG_NUM; ++i )
	{
		if ( m_pToDeleteDialog [ i ] && m_pToDeleteDialog [ i ] == pToDeleteDialog )
		{
			break;
		}

		if ( m_pToDeleteDialog [ i ] == NULL )
		{
			m_pToDeleteDialog[ i ] = pToDeleteDialog;
			break;
		}
	}
    
	return;
}

bool CInterfaceMgr::DeleteDialog()
{
	cltClient *pclclient = (cltClient *)pclClient;

	bool bDelete = false;
	
	for ( SI16 i = 0; i < MAX_DELETE_DIALOG_NUM; ++i )
	{
		if ( m_pFocusMgr && m_pFocusMgr == m_pToDeleteDialog[ i ] ) 
		{
			m_pFocusMgr = NULL;
		}

		if ( m_pToDeleteDialog [ i ] != NULL )
		{
			if( m_pToDeleteDialog [ i ]->IsModalDialog() )
			{
				m_bModalStatus = false;
			}

			pclclient->DestroyInterface( m_pToDeleteDialog[ i ] );
			m_pToDeleteDialog[ i ] = NULL;

			bDelete = true;
		}
	}
	
	return bDelete;
}

void CInterfaceMgr::SetFocus( CControllerMgr* pControllerMgr, CControllerObj *pControllerObj )
{
	if ( pControllerMgr == NULL || pControllerObj == NULL )
	{
		return;
	}
	
	if( m_pFocusMgr )
	{
		if ( m_pFocusMgr == pControllerMgr )
		{
			m_pFocusMgr->SetFocus( pControllerObj );
		}
		else
		{
			m_pFocusMgr = pControllerMgr;
			m_pFocusMgr->SetFocus( pControllerObj );
		}
	}
	else
	{
		m_pFocusMgr = pControllerMgr;
		m_pFocusMgr->SetFocus( pControllerObj );
	}

	return;
}

bool CInterfaceMgr::GetControllerAtPoint( CControllerMgr **pDialog, CControllerObj **pController )
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( pclclient->GetHwnd(), &pt );
	pt.x += MAP_TILE_XSIZE;
	pt.y += MAP_TILE_YSIZE;

	CControllerMgr *pTemp = GetFirst();
	POINT TempPt;

	while ( pTemp )
	{
		if ( pTemp->IsShow() && pTemp->IsInArea( pt.x, pt.y ) )
		{
			*pDialog = pTemp;

			TempPt.x = pt.x - pTemp->GetX();
			TempPt.y = pt.y - pTemp->GetY();

			CControllerObj *pControllerObj = pTemp->GetControlAtPointFromInterfaceMgr( TempPt );
			if ( pControllerObj )
			{	
				*pController = pControllerObj;
			}

			return true;
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}

void CInterfaceMgr::SetTopMostWindow( CControllerMgr *pNode, bool TopMostWindow )
{
	if ( ! IsExistInterface( pNode ) )
	{
		return;
	}

	if ( TopMostWindow )
	{
		if ( pNode == GetFirst() )
		{
			return;
		}
		else if ( pNode == GetLast() )
		{
			m_pLast = pNode->GetPre();
			m_pLast->SetNext( NULL );

			m_pFirst->SetPre( pNode );
			pNode->SetNext( m_pFirst );
			pNode->SetPre( NULL );
			m_pFirst = pNode;
		}
		else
		{
			pNode->GetPre()->SetNext( pNode->GetNext() );
			pNode->GetNext()->SetPre( pNode->GetPre() );

			m_pFirst->SetPre( pNode );
			pNode->SetNext( m_pFirst );
			pNode->SetPre( NULL );
			m_pFirst = pNode;
		}

		if ( pNode != m_pFocusMgr )
		{
			ClearFocusMgr();
			m_bMoveableStatus = false;
			m_pFocusMgr = pNode;
		}
	}
	else
	{
		MoveToFirst( pNode );
	}

	return;
}

void CInterfaceMgr::SetModalDialog( CControllerMgr *pNode, bool ModalDialog )
{
	if ( ! IsExistInterface( pNode ) )
	{
		return;
	}

	if ( ModalDialog )
	{
		if ( pNode == GetFirst() )
		{
			return;
		}
		else if ( pNode == GetLast() )
		{
			m_pLast = pNode->GetPre();
			m_pLast->SetNext( NULL );

			m_pFirst->SetPre( pNode );
			pNode->SetNext( m_pFirst );
			pNode->SetPre( NULL );
			m_pFirst = pNode;
		}
		else
		{
			pNode->GetPre()->SetNext( pNode->GetNext() );
			pNode->GetNext()->SetPre( pNode->GetPre() );

			m_pFirst->SetPre( pNode );
			pNode->SetNext( m_pFirst );
			pNode->SetPre( NULL );
			m_pFirst = pNode;
		}
	}
	else
	{
		MoveToFirst( pNode );
	}

	m_bModalStatus = true;

	m_pFocusMgr = pNode;
	m_pFocusMgr->SetFocus( true );

	if ( IsItemDragMoveStatus() )
	{
		ItemDragStatusInit();
	}

	return;
}

bool CInterfaceMgr::IsAvailableMove( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height )
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( XPos + Width - 10 < m_rcAvailableMove.left)
	{
		return false;
	}

	if ( XPos + 10 > m_rcAvailableMove.right )
	{
		return false;
	}
	
	// 이미지의 일부가 외부로 삐져나갔을때만 못움직이게 한다.
	if (YPos  < 35)				// 35는 상단 다이얼로그 + 오프스크린 대충 두께
	{
		if( YPos  - 10 < m_rcAvailableMove.top )  
		{
			return false;
		}
	}

	if( YPos + Height > pclclient->GetScreenYSize() - 24) // 24는 하단 다이얼로그 두께
	{
		if ( YPos > m_rcAvailableMove.bottom )
		{
			return false;
		}
	}

	if( Height > m_rcAvailableScreen.bottom - m_rcAvailableMove.bottom )
	{
		if( YPos > m_rcAvailableMove.bottom )
		{
			return false;
		}
	}
	else
	{
		if( YPos + Height > m_rcAvailableScreen.bottom )
		{
			return false;
		}

	}

	return true;
}

void CInterfaceMgr::SetSnapIfAvailable( SI16 &XPos, SI16 &YPos, SI16 Width, SI16 Height )
{

	if ( abs( m_rcAvailableScreen.left - XPos ) < AVAILABLE_SNAP_OFFSET_SIZE )
	{
		XPos = m_rcAvailableScreen.left;	
	}
	else if ( abs( m_rcAvailableScreen.right - XPos - Width ) < AVAILABLE_SNAP_OFFSET_SIZE )
	{
		XPos = m_rcAvailableScreen.right - Width;
	}

	if ( abs( m_rcAvailableScreen.top - YPos ) < AVAILABLE_SNAP_OFFSET_SIZE )
	{	
		YPos = m_rcAvailableScreen.top;
	}
	else if ( abs( m_rcAvailableScreen.bottom - YPos - Height ) < AVAILABLE_SNAP_OFFSET_SIZE )
	{
		YPos = m_rcAvailableScreen.bottom - Height;
	}

	return;
}

bool CInterfaceMgr::IsModalStatus()
{
	return m_bModalStatus;
}

CControllerMgr *CInterfaceMgr::GetModalDialog()
{
	CControllerMgr *pTemp = GetFirst();

	while ( pTemp )
	{
		if ( pTemp->IsModalDialog() )
		{
			return pTemp;
		}

		pTemp = pTemp->GetNext();
	}

	return NULL;
}

bool CInterfaceMgr::IsExistModalDialog()
{
	CControllerMgr *pTemp = GetFirst();

	while ( pTemp )
	{
		if ( pTemp->IsModalDialog() )
		{
			return true;
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}

DWORD CInterfaceMgr::GetTotalWidthCandidateData( HDC hDC, SI16 CandidateDataStartIndex, LPCANDIDATELIST pCandidateList, stCandidateData *pCandidateDataList )
{
	if ( pCandidateList == NULL || pCandidateDataList == NULL )
	{
		return 0;
	}

	TCHAR szTemp[ MAX_PATH * 9 ] = TEXT("");

	for ( SI16 i = CandidateDataStartIndex; i < CandidateDataStartIndex + 9; ++i )
	{
		if( (DWORD)i >= pCandidateList->dwCount )
			break;

		StringCchCat( szTemp, MAX_PATH * 9 , pCandidateDataList[ i ].GetData() );
		StringCchCat( szTemp, MAX_PATH * 9 , TEXT("  ") );
	}

	return GetTextExtent( hDC, szTemp, _tcslen( szTemp ) );
}

DWORD CInterfaceMgr::GetMaxWidthCandidateData( HDC hDC, SI16 CandidateDataStartIndex, LPCANDIDATELIST pCandidateList, stCandidateData *pCandidateDataList )
{
	if ( pCandidateList == NULL || pCandidateDataList == NULL )
	{
		return 0;
	}

	SI32 MaxTextWidth = 0;
	SI32 TextWidth = 0;

	for ( SI16 i = CandidateDataStartIndex; i < CandidateDataStartIndex + 9; ++i )
	{
		if( (DWORD)i >= pCandidateList->dwCount )
			break;

		TextWidth = GetTextExtent( hDC, pCandidateDataList[ i ].GetData(), _tcslen( pCandidateDataList[ i ].GetData() ) );

		if ( MaxTextWidth < TextWidth )
		{
			MaxTextWidth = TextWidth;
		}
	}

	if ( MaxTextWidth == 0 )
	{
		int Temp = 0;
		++Temp;
	}

	if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
	{
		MaxTextWidth = max(MaxTextWidth, 56);
	}

	return MaxTextWidth;
}

void CInterfaceMgr::SetCandidateWindow( LPCANDIDATELIST pCandidateList, SI16 CandidateStartIndex, bool VerticalCandidateWindow, stCandidateData *pCandidateDataList,
									   SI32 FontHeight, SI32 FontWidth, SI32 FontWeight, TCHAR *FontKind, COLORREF FontColor
									   )
{
	if ( pCandidateList == NULL || pCandidateDataList == NULL )
	{
		return;
	}

 	SAFE_DELETE_A( m_ImgCandidateEncBuffer );

	if ( m_pCandidateSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = m_pCandidateSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			cltClient *pclclient = ( cltClient * )pclClient;

			HDC hDC;

  			if ( pCandidateList && pCandidateList->dwCount > 0 )
			{
//				RECT SelectRect;

 				g_graphics.ClearSurface( lpSurface, RGB16_TRANS );
			
				if( lpSurface->GetDC( &hDC ) == DD_OK)
				{
					if ( VerticalCandidateWindow )
					{
						m_siCandidateStrWidth = GetMaxWidthCandidateData( hDC, CandidateStartIndex, pCandidateList, pCandidateDataList ) + EDIT_CANDIDATE_RIGHT_SPACE;
						m_siCandidateStrHeight = EDIT_LINEHEIGHT * 10  + EDIT_CANDIDATE_TOP_SPACE ; // 9개의 후보 단어와 1개의 페이지표시 칸
					}
					else
					{
						m_siCandidateStrWidth = GetTotalWidthCandidateData( hDC, CandidateStartIndex, pCandidateList, pCandidateDataList ) + EDIT_CANDIDATE_PAGENUM_DRAW_SPACE ;
						m_siCandidateStrHeight = EDIT_LINEHEIGHT;
					}
					HFONT hFont;
					m_ImgCandidateEncBuffer = new RGB16[ m_siCandidateStrWidth * m_siCandidateStrHeight * 2 ];

					if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
						hFont = CreateFont( FontHeight, FontWidth, 0, 0, FontWeight, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, FontKind );
					}
					else {
						hFont = CreateFont( FontHeight, FontWidth, 0, 0, FontWeight, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, FontKind );
					}
					
					HFONT hOldFont = NULL;
					HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
					HBRUSH hOldBrush = NULL;
					HPEN   hPen = (HPEN)GetStockObject(BLACK_PEN);
					HPEN   hOldPen = NULL;
					

					hOldFont = (HFONT)SelectObject( hDC, hFont );
					hOldBrush = (HBRUSH)SelectObject( hDC, hBrush );
					hOldPen	 = (HPEN)SelectObject( hDC, hPen );
					
					SetTextColor( hDC, FontColor );
					SetBkMode( hDC, TRANSPARENT );
										

					if ( VerticalCandidateWindow )
					{
 						// 일단 바탕에 이쁘게 테두리를 그리고 숫자 표시부분에 색을 칠해준다.						
						Rectangle(hDC,0,0,m_siCandidateStrWidth,
							m_siCandidateStrHeight);
						Rectangle(hDC,0,m_siCandidateStrHeight - EDIT_LINEHEIGHT,m_siCandidateStrWidth,m_siCandidateStrHeight);

						HBRUSH hCandidateBrush = CreateSolidBrush(RGB( 140,158,168 )); //(HBRUSH)GetStockObject(GRAY_BRUSH);
						HBRUSH hCandidateOldBrush = NULL;
						HPEN   hCandidatePen = (HPEN)GetStockObject(NULL_PEN);
						HPEN   hCandidateOldPen = NULL;

 						hCandidateOldBrush = (HBRUSH)SelectObject(hDC, hCandidateBrush);
						hCandidateOldPen = (HPEN)SelectObject(hDC, hCandidatePen);

						Rectangle(hDC,1,1,15,m_siCandidateStrHeight - EDIT_LINEHEIGHT + 1);	 // 마지막 칸은 페이지 표시이기때문에 EDIT_LINEHEIGHT 하나크기를 뺀다. 선이 없어서 1을 더한다

						SelectObject(hDC, hCandidateOldBrush);
						SelectObject(hDC, hCandidateOldPen);
						DeleteObject( hCandidateBrush );
						DeleteObject( hCandidatePen );

						for ( DWORD i = CandidateStartIndex; i < CandidateStartIndex + pCandidateList->dwPageSize; ++i )
						{
							if( i >= pCandidateList->dwCount )
								break;

							// 선택된 후보단어라면 배경을 불투명하게 하고 폰트 색을 바꾸어준다.
							if ( i == pCandidateList->dwSelection  )
							{
								SetTextColor( hDC, EDIT_CANDIDATE_SELECTED_FONTCOLOR );
								SetBkMode( hDC, OPAQUE );
								SetBkColor(hDC, EDIT_CANDIDATE_SELECTED_BKCOLOR );
								RECT Rect = { 0 , EDIT_CANDIDATE_TOP_SPACE +  ( i - CandidateStartIndex ) * EDIT_LINEHEIGHT , m_siCandidateStrWidth - 1, EDIT_CANDIDATE_TOP_SPACE + ( i + 1 - CandidateStartIndex ) * EDIT_LINEHEIGHT - 1 };
								DrawText( hDC, pCandidateDataList[ i ].GetData(), _tcslen( pCandidateDataList[ i ].GetData() ), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
								SetTextColor( hDC, FontColor );
								SetBkMode( hDC, TRANSPARENT );
							}
							else
							{
								RECT Rect = { 0,EDIT_CANDIDATE_TOP_SPACE + ( i - CandidateStartIndex ) * EDIT_LINEHEIGHT , m_siCandidateStrWidth - 1, EDIT_CANDIDATE_TOP_SPACE + ( i + 1 - CandidateStartIndex ) * EDIT_LINEHEIGHT - 1 };
								DrawText( hDC, pCandidateDataList[ i ].GetData(), _tcslen( pCandidateDataList[ i ].GetData() ), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
							}
						}
						
						// 밑에다 페이지 표시해준다 -> ex) -> 1/3
   						TCHAR szBuffer[256] = TEXT("");
   						StringCchPrintf(szBuffer,256,TEXT("%d/%d "), CandidateStartIndex / 9 + 1  , (pCandidateList->dwCount -1) / 9 + 1 ); 
						RECT Rect = { 0, EDIT_CANDIDATE_TOP_SPACE + 9 * EDIT_LINEHEIGHT , m_siCandidateStrWidth - 1,EDIT_CANDIDATE_TOP_SPACE + 10 * EDIT_LINEHEIGHT -1 };
						DrawText( hDC, szBuffer, _tcslen( szBuffer ), &Rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE );

						//if( pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan) &&
						//	(pCandidateList->dwCount -1) / 9 + 1 > 1)
						//{
						//	StringCchPrintf(szBuffer, 256, TEXT(" Shift+"));							
						//	Rect.top += 1;
						//	DrawText( hDC, szBuffer, _tcslen( szBuffer ), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
						//}							
					}
 					else
					{
						// 일단 바탕에 이쁘게 테두리를 그리고
						Rectangle(hDC,0,0,m_siCandidateStrWidth,m_siCandidateStrHeight);
						Rectangle(hDC,m_siCandidateStrWidth - EDIT_CANDIDATE_PAGENUM_DRAW_SPACE,0,m_siCandidateStrWidth,m_siCandidateStrHeight);
 						TCHAR szTemp[ MAX_PATH * 9 ] = TEXT("");
						int CurrentPos = 0;
						
						// 뒤에다 페이지 표시해준다 -> ex) -> 1/3
						TCHAR szBuffer[256] = TEXT("");
						StringCchPrintf(szBuffer,256,TEXT("%d/%d "), CandidateStartIndex / 9 + 1  , (pCandidateList->dwCount -1) / 9 + 1 ); 
						RECT Rect = { m_siCandidateStrWidth - EDIT_CANDIDATE_PAGENUM_DRAW_SPACE, 0 , m_siCandidateStrWidth, EDIT_LINEHEIGHT };
						DrawText( hDC, szBuffer, _tcslen( szBuffer ), &Rect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE );

						for ( DWORD i = CandidateStartIndex; i < CandidateStartIndex + pCandidateList->dwPageSize; ++i )
						{
							if( i >= pCandidateList->dwCount )
								break;

							StringCchCopy( szTemp, MAX_PATH * 9, pCandidateDataList[ i ].GetData() );
							StringCchCat( szTemp, MAX_PATH * 9 , TEXT("  ") );

							SI32 Width = GetTextExtent( hDC, szTemp, _tcslen( szTemp ) );
							
							/*// 선택된 후보단어라면 폰트 색을 바꾸어준다.
							if ( i == pCandidateList->dwSelection )
							{
								SetTextColor( hDC, RGB(240,240,240) );
								SetBkMode( hDC, OPAQUE );	
								SetBkColor(hDC, EDIT_CANDIDATE_SELECTED_BKCOLOR );
								RECT Rect = { CurrentPos, 0, CurrentPos + Width - 1, m_siCandidateStrHeight - 1 };
								DrawText( hDC, szTemp, _tcslen( szTemp ), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
								SetTextColor( hDC, FontColor );
								SetBkMode( hDC, TRANSPARENT );										
							}						
							else*/
							{	// 현재 항목을 선택하지 않는다
								RECT Rect = { CurrentPos, 0, CurrentPos + Width - 1, m_siCandidateStrHeight - 1 };
								DrawText( hDC, szTemp, _tcslen( szTemp ), &Rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE );
							}

							CurrentPos += Width;
						}
					}

					SelectObject( hDC, hOldFont );
					SelectObject( hDC, hOldPen );
					SelectObject( hDC, hOldBrush );

					DeleteObject( hFont );
					DeleteObject( hPen );
					DeleteObject( hBrush );

					lpSurface->ReleaseDC( hDC );
				}

				if ( GP.LockSurface( lpSurface ) )
				{
					/*
 					if ( pCandidateSelectSpr )
					{
						//DrawTileImg( pCandidateSelectSpr, SPR_PUTSPRALPHAT, EDIT_CANDIDATESELECTIMAGE_FONTINDEX, 8, SelectRect.left, SelectRect.top, m_siCandidateStrWidth - 1, SelectRect.bottom - SelectRect.top - 1, 1, true );
					}*/

					if(m_ImgCandidateEncBuffer)
					{
						int size;
						g_graphics.EncodeXpr( m_ImgCandidateEncBuffer, m_siCandidateStrWidth, m_siCandidateStrHeight, RGB16_TRANS, &size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}
		}
	}

	m_bCandidateOpenStatus = true;

	return;
}

void CInterfaceMgr::SetMsgBox( stMsgBox *pstMsgBox, BYTE *pData, SI16 DataLen )
{
	if ( pstMsgBox == NULL )
	{
		return;
	}

	if ( pstMsgBox->pControllerMgr == NULL )
	{
		return;
	}

	CControllerMgr *pMsgDlg = GetExistInterface( NMSGBOX_DLG );
	if ( pMsgDlg == NULL )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		pclclient->CreateInterface( NMSGBOX_DLG );
	}

	pMsgDlg = GetExistInterface( NMSGBOX_DLG );
	if ( pMsgDlg )
	{
		CNMsgBoxDlg *pMsgBoxDlg = static_cast< CNMsgBoxDlg * >(pMsgDlg);
		pMsgBoxDlg->SetMsgData( (BYTE*)pstMsgBox, pData, DataLen );
	}

	return;
}

void CInterfaceMgr::CloseWindowEceptBaseWindow()
{
	ClearFocusMgr();

	CControllerMgr *pTemp = GetFirst();
	CControllerMgr *pTemp1 = NULL;

	while ( pTemp )
	{
		if( pTemp->GetDialogControllerID() == STATUSBAR_DLG
			|| pTemp->GetDialogControllerID() == NSYSMENUBAR_DLG 
			|| pTemp->GetDialogControllerID() == NCHARMENUBAR_DLG
			|| pTemp->GetDialogControllerID() == NMINIMAP_DLG
			|| pTemp->GetDialogControllerID() == NCHATBAR_DLG
			|| pTemp->GetDialogControllerID() == NPREMIUMSTATUS_DLG
			|| pTemp->GetDialogControllerID() == NBUFFSTATUS_DLG //툴팁추가.[2007.07.12 성웅]
			)
		{
			
		}
		else if ( pTemp->IsShowHideMethod() )
		{
			pTemp->Show( false );

		}
		else
		{		
			PushToDeleteDialog( pTemp );
		}

		pTemp = pTemp->GetNext();
	}	

	return;
}


void CInterfaceMgr::CloseAllWindow()
{
	ClearFocusMgr();

	CControllerMgr *pTemp = GetFirst();
	CControllerMgr *pTemp1 = NULL;

	while ( pTemp )
	{
		
		if ( pTemp->IsShowHideMethod() )
		{
			pTemp->Show( false );
			
		}
		else
		{
			/*
			/// 미니맵 버그 관련 임시처리 [영진]
			if( pTemp->GetDialogControllerID() == NMINIMAP_DLG )
			{
				g_DlgPositionSave.SetPosition(pTemp->GetDialogControllerID(),pTemp->GetX(),pTemp->GetY());
				pTemp->MovePos(250,200); 
			}
			else*/
				PushToDeleteDialog( pTemp );

		}

		pTemp = pTemp->GetNext();
	}	

	return;
}

bool CInterfaceMgr::HasFocusInEditBox()
{
	CControllerMgr *pTemp = GetFirst();
	CControllerObj *pFocusObj = NULL;

	while( pTemp )
	{
		if ( pTemp->IsShow() )
		{
			pFocusObj = pTemp->GetFocusObj();
			if ( pFocusObj && pFocusObj->GetControllerKind() == CONTROLLER_KIND_EDIT )
			{
				return true;
			}
		}

		pTemp = pTemp->GetNext();
	}

	return false;
}

void CInterfaceMgr::CheckToggleState()
{	
	cltClient* pclclient = (cltClient*)pclClient;

	HKL hKeyboardLayout = GetKeyboardLayout( 0 );

	if ( hKeyboardLayout == NULL )
	{
		return;
	}
	
	bool bIme = ImmIsIME( hKeyboardLayout ) != 0;
	bool ChineseIME = ( GetIMEPrimaryLanguage( hKeyboardLayout ) == LANG_CHINESE ) && bIme;

	HIMC hImc = _ImmGetContext( pclClient->GetHwnd() );

	if( NULL != hImc )
	{
		DWORD dwConvMode, dwSentMode ;
//		IMESTATE s_ImeState;
		WORD Data = 0;

		if( g_SoundOnOffDlg.m_bMaxWindowMode == true &&
			pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
		{
			// IME STATUSWINDOW를 게임의 적당한 공간에 위치시킨다.
			POINT pt;
			BOOL bRet;
			pt.x = 1024;		pt.y = 692;
			bRet = ImmSetStatusWindowPos( hImc, &pt );
		}

		if( ChineseIME )
		{
			ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
			//			s_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IME_CMODE_NATIVE : IMEUI_STATE_ENGLISH;

			Data = GetIMESubLanguage( hKeyboardLayout );
			if(Data == SUBLANG_CHINESE_TRADITIONAL)		// For TAIWAN
			{
			}

#ifdef _DEBUG
			TCHAR	szBuf[1024];
			StringCchPrintf(szBuf, 1024, "dwConvMode : %d, dwSentMode : %d\n", dwConvMode, dwSentMode);
			OutputDebugString(szBuf);
#endif
		}
		else
		{
			Data = GetIMESubLanguage( hKeyboardLayout );
			m_dwImeState = ( bIme && ImmGetOpenStatus( hImc ) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;
			
		}
		_ImmReleaseContext( pclClient->GetHwnd(), hImc );
	}
	else
	{
		//s_ImeState = IMEUI_STATE_OFF;
	}
}


bool CInterfaceMgr::ClickAndDrop_Start()
{

	return false;
}

bool CInterfaceMgr::ClickAndDrop_End()
{
	return false;
}



/*			//  Statusbar 에서 그려줌
void CInterfaceMgr::DrawImeState()
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(m_dwImeState)
	{
		case IMEUI_STATE_ON:
			{
				TSpr *pImeSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( INTERFACE_MGR_IMESTATE_FILENAME );
				GP.PutSpr( pImeSpr, 500, 300 , 1 );
		}
			break;
		case IMEUI_STATE_OFF:
			{
				TSpr *pImeSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( INTERFACE_MGR_IMESTATE_FILENAME );
				GP.PutSpr( pImeSpr, 500, 300  , 0 );
			}
			break;
		default:
			break;
	}
}
*/
