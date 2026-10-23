#include "ImageStatic.h"
#include "ToolTip.h "
#include "../InterfaceMgr.h"
#include "../ControllerMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"


#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <zmouse.h>

#include "../ControllerSurfaceObj.h"



#include <Client.h>
extern cltCommonLogic *pclClient;

CImageStatic::CImageStatic( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_bLMPressed = false;
	m_bRMPressed = false;
	m_ptImageStaticPos.x = 0;
	m_ptImageStaticPos.y = 0;
	m_bUseToolTip =	false;
	m_bUseToolTipColor = true;
	m_siZDelta = 0; 

	m_strToolTipMsg = NULL;
}

CImageStatic::~CImageStatic()
{
	SAFE_DELETE_A(m_strToolTipMsg);

}

void CImageStatic::Create( SI16 ControllerID, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight )
{
	CControllerObj::Create( ControllerID, CONTROLLER_KIND_IMAGESTATIC, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	return;
}

bool CImageStatic::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				SetCapture( pclClient->GetHwnd() );

				// Pressed while inside the control
				m_bLMPressed = true;
				SetImageStaticPos( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_IMAGESTATIC_LBUTTONDOWN, true, this );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				return true;
			}
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				SetCapture( pclClient->GetHwnd() );

				// Pressed while inside the control
				m_bLMPressed = true;
				SetImageStaticPos( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_IMAGESTATIC_DBCLICKED, true, this );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				return true;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			if( m_bLMPressed )
			{
				ReleaseCapture();

				m_bLMPressed = false;

				if( IsInArea( pt.x, pt.y ) )
				{	
					SetImageStaticPos( pt.x, pt.y );
					GetParent()->SendEvent( EVENT_IMAGESTATIC_LBUTTONUP, true, this );
				}

				return true;
			}
		}
		break;

	case WM_RBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				SetCapture( pclClient->GetHwnd() );

				// Pressed while inside the control
				m_bRMPressed = true;
				SetImageStaticPos( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_IMAGESTATIC_RBUTTONDOWN, true, this );

				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				return true;
			}
		}
		break;

	case WM_RBUTTONUP:
		{
			if( m_bRMPressed )
			{
				ReleaseCapture();

				m_bRMPressed = false;

				if( IsInArea( pt.x, pt.y ) )
				{	
					SetImageStaticPos( pt.x, pt.y );
					GetParent()->SendEvent( EVENT_IMAGESTATIC_RBUTTONUP, true, this );
				}

				return true;
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			if ( IsInArea( pt.x, pt.y ) )
			{
				if ( m_bUseToolTip )
				{
					if ( m_strToolTipMsg )
					{
						cltClient *pclclient = (cltClient*)pclClient;
						pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTip( m_strToolTipMsg, m_bUseToolTipColor  );
						//pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTipMove(true);
					}
				}
			}

			if ( m_bLMPressed )
			{
				SetImageStaticPos( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_IMAGESTATIC_LBUTTONDRAG, true, this );

				return true;
			}

			if ( IsInArea( pt.x, pt.y ) )
			{
				SetImageStaticPos( pt.x, pt.y );
				GetParent()->SendEvent( EVENT_IMAGESTATIC_MOUSEMOVE, true, this );
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if ( IsInArea( pt.x, pt.y ) )
			{
				m_siZDelta = (short) HIWORD(wParam) / WHEEL_DELTA;
				GetParent()->SendEvent( EVENT_IMAGESTATIC_MOUSEWHEEL, true, this );
				return true;
			}
		}
		break;
	}

	return false;
}

void CImageStatic::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient*)pclClient;

	if(IsMouseOver())
	{
		if ( m_bUseToolTip )
		{
			if ( m_strToolTipMsg )
			{
				pclclient->m_pInterfaceMgr->SetDrawToolTip( true );
			}
		}
	}

	TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );

	if ( pBkImg )
	{
		if ( IsBKTileImg() )
		{
			DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex(), GetAlphaValue(), BaseXPos + GetX(), BaseYPos + GetY(), GetSurfaceWidth(), GetSurfaceHeight(), GetBKTileImgNum() );
		}
		else
		{
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetFontIndex() );							break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetAlphaValue(), GetFontIndex() );		break;
			}
		}
	}
}

void CImageStatic::ClearImageStaticPos()
{
	m_ptImageStaticPos.x = 0;
	m_ptImageStaticPos.y = 0;
}

void CImageStatic::SetImageStaticPos( int XPos, int YPos )
{
	m_ptImageStaticPos.x = XPos - GetX();
	m_ptImageStaticPos.y = YPos - GetY();

	return;
}

void CImageStatic::GetImageStaticPos( POINT &Point )
{
	Point = m_ptImageStaticPos;
	return;
}

void CImageStatic::SetUseToolTip( TCHAR *ToolTipMsg, bool bColor)
{
	if ( ToolTipMsg == NULL )
	{
		m_bUseToolTip = false;
		return;
	}
	else
	{
		m_bUseToolTipColor = bColor;
		m_bUseToolTip = true;
		if ( m_strToolTipMsg == NULL )
		{
			m_strToolTipMsg = new char[ MAX_PATH ];
		}

		memset( m_strToolTipMsg, 0, MAX_PATH );
		strncpy( m_strToolTipMsg, ToolTipMsg, MAX_PATH );
		m_strToolTipMsg[ MAX_PATH - 1 ] = NULL;
	}

	return;
}