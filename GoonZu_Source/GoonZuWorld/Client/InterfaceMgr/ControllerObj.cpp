#include "ControllerObj.h"
#include "ControllerMgr.h"
#include "ControllerSurfaceObj.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "InterfaceMgr.h"

#include <Client.h>
#include "../Client/DLGPositionSave/DLGPositionSave.h"

#include "./interfaceimgmgr/InterfaceImgMgr.h"

extern cltCommonLogic *pclClient;

/*
extern CInterfaceMgr *g_pInterfaceMgr;
*/

CControllerObj::CControllerObj( CControllerMgr *pParent ) : CObj()
{
	m_pParent = pParent;
	Init();
}
CControllerObj::CControllerObj( CControllerMgr *pParent,SI32 siTmpBufferSize ) : CObj(siTmpBufferSize)
{
	m_pParent = pParent;
	Init();
}

void CControllerObj::Init()
{
	// Controller 초기화
	m_siControllerID = 0;
	m_siControllerKind = CONTROLLER_KIND_NONE;

	// HotKey
	m_uiHotkey = 0;

	// Controller 는 처음에 활성화와 동시에 보여줘야 함으로... 동시에 포커스는 없어야지..
	m_bEnable = true;
	m_bShow = true;
	m_bHasFocus = false;
	m_bMouseOver = false;

	m_bBKTileImg = true;					// 대부분 배경 이미지를 Tile로 처리할 예정 ( 용량 줄이기 )
	m_siBKTileImgNum = 9;					// 9개 타일로 이루어 진 경우가 많음으로...
	m_siDrawOption = SPR_PUTSPRT;
	m_siAlphaValue = 12;


	// 단, 버튼과 같은 경우 붙은 이미지는 통짜가 편함 ( 솔직히, 코딩하기 귀찮지... )
	// 배경 이미지 경로
	TMemSet( m_strFileName, 0, MAX_PATH );

	// 좌표 초기화
	m_siXPos = 0;
	m_siYPos = 0;
	m_siWidth = 0;
	m_siHeight = 0;

	m_siMaxSurfaceWidth = 0;
	m_siMaxSurfaceHeight = 0;


	// 바뀐게 있는지 없는지 구분 flag
	m_bChanged = false;

	// Surface
	m_pControllerSurfaceObj = NULL;

	StringCchCopy( m_strFontKind, MAX_PATH, DEFAULT_FONT_KIND );
	m_siFontWidth = 0;
	m_siFontHeight = BASE_FONT_HEIGHT;
	m_siFontWeight = FW_NORMAL;		//FW_BOLD;
	m_FontColor = RGB( 0, 0, 0 );

	m_siTextPrintOption = 0;

	m_bBorder = false;
	m_crBorder = CONTROLLER_BORDER_COLOR;

	m_bBKColorUse = false;
	m_crBKColor = CONTROLLER_BACKGROUND_COLOR;

	m_siBackGroundDrawOption = SPR_PUTSPRT;
	m_siParam1 = BASE_ALPHA_VALUE;
	m_siParam2 = 0;

	m_bImageArea = false;
}

CControllerObj::~CControllerObj()
{
	if( m_pControllerSurfaceObj )
	{
		delete m_pControllerSurfaceObj;
		m_pControllerSurfaceObj = NULL;
	}

	m_pParent->Del( this );
}

void CControllerObj::Create( SI16 ControllerID, SI16 ControllerKind,
							TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
							SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
							SI16 MaxSurfaceWidth, SI16 MaxSurfaceHeight )
{
#ifdef _DEBUG
	if ( Width > MaxSurfaceWidth )
	{
		TCHAR Buffer[ 128 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(3905);
		StringCchPrintf( Buffer, 128, pText, ControllerID, Width, MaxSurfaceWidth );
		MessageBox( pclClient->GetHwnd(), Buffer, TEXT("Error"), MB_OK );
	}
	if ( Height > MaxSurfaceHeight )
	{
		TCHAR Buffer[ 128 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(3906);
		StringCchPrintf( Buffer,  128, pText, ControllerID, Height, MaxSurfaceHeight );
		MessageBox( pclClient->GetHwnd(), Buffer, TEXT("Error"), MB_OK );
	}
#endif


	m_siControllerKind = ControllerKind;
	m_siControllerID = ControllerID;

	// NULL 또는 < 1 인 경우는 배경이 RGB 단색으로 되어 있을 수 있음으로...
	if ( FileName == NULL || !FileName[ 0 ] )
	{
		TMemSet( m_strFileName, 0, MAX_PATH );
	}
	else
	{
		StringCchCopy( m_strFileName, MAX_PATH, FileName );
	}

	m_siFontIndex = FontIndex;

	m_bBKTileImg = BKTileImg;
	m_siBKTileImgNum = BKTileImgNum;

	m_siXPos = XPos;
	m_siYPos = YPos;

	m_siWidth = Width;
	m_siHeight = Height;

	m_siMaxSurfaceWidth = MaxSurfaceWidth;
	m_siMaxSurfaceHeight = MaxSurfaceHeight;

	m_bChanged = false;

	if ( m_pControllerSurfaceObj )
	{
		delete m_pControllerSurfaceObj;
		m_pControllerSurfaceObj = NULL;
	}

	m_pControllerSurfaceObj = new CControllerSurfaceObj();
	m_pControllerSurfaceObj->Create( MaxSurfaceWidth, MaxSurfaceHeight );


	if( m_pParent )
	{
		m_pParent->Add( this );
	}
}

void CControllerObj::SetSurfaceObjSize(SI16 SurfaceWidth, SI16 SurfaceHeight)
{
	if ( m_pControllerSurfaceObj )
	{
		delete m_pControllerSurfaceObj;
		m_pControllerSurfaceObj = NULL;
	}

	m_pControllerSurfaceObj = new CControllerSurfaceObj();
	m_pControllerSurfaceObj->Create( SurfaceWidth, SurfaceHeight );

}


void CControllerObj::Draw( SI16 BaseXPos, SI16 BaseYPos )
{	
	if ( ! IsShow() )
	{
		return;
	}

	/*
	// 배경 이미지 로딩
	if ( GetBKImg()->Image == NULL && GetFileName() != NULL )
	{
		if( ! LoadBKImg() )
		{
			// 배경 없으면 안 그려주면 그만임으로, return 코드는 주석처리함 (ex : Static Controller )
			//return;
		}
	}
	*/

	DrawController( BaseXPos, BaseYPos );
}

// 이 좌표가 이 Controller 영역에 있는지 아닌지 확인
bool CControllerObj::IsInArea( SI32 XPos, SI32 YPos )
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( ! IsShow() )
	{
		return false;
	}

	if ( m_siXPos < XPos && m_siYPos < YPos &&
		m_siXPos + m_siWidth > XPos && m_siYPos + m_siHeight > YPos )
	{
		if( IsBKTileImg() == false && m_bImageArea )
		{
			TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
            BYTE btColor = GP.GetSprColor( pBkImg, XPos - m_siXPos, YPos - m_siYPos, GetFontIndex(), false );

			return (btColor != COLOR_TRANS);
		}
		else
			return true;
	}

	return false;
}

// 이 move 의 경우에는 거의 쓰이지 않음. 다이얼로그랑 상관없이 이것만 움직이게 할때 사용됨
void CControllerObj::Move( SI16 OffsetX, SI16 OffsetY )
{
	m_siXPos += OffsetX;
	m_siYPos += OffsetY;

	return;
}

void CControllerObj::Show( bool ShowOption )
{
	m_bShow = ShowOption;

	if ( m_bShow )
	{
		Refresh();
	}

	if ( ShowOption == false &&
		GetParent()->GetMouseOverObj() != NULL && 
		GetParent()->GetMouseOverObj() == this )
	{
		OnMouseLeave();
		GetParent()->SetMouseOverObj( NULL );
		g_DlgPositionSave.SetPosition( GetControllerID(), GetX(), GetY() );
	}

	return;
}

bool CControllerObj::LButtonDown( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind )
{
	if ( ! m_bShow )
	{
		return false;
	}

	if ( IsInArea( XPos, YPos ) )
	{
		return true;
	}

	return false;
}

bool CControllerObj::LButtonUp( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind )
{
	if ( ! m_bShow )
	{
		return false;
	}

	if ( IsInArea( XPos, YPos ) )
	{
		return true;
	}

	return false;
}

bool CControllerObj::RButtonDown( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind )
{
	if ( ! m_bShow )
	{
		return false;
	}

	if ( IsInArea( XPos, YPos ) )
	{
		return true;
	}

	return false;
}


bool CControllerObj::RButtonUp( SI16 XPos, SI16 YPos, SI16 *pControllerID, SI16 *pKind )
{
	if ( ! m_bShow )
	{
		return false;
	}

	if ( IsInArea( XPos, YPos ) )
	{
		return true;
	}

	return false;
}

bool CControllerObj::MouseMove( SI16 XPos, SI16 YPos, SI16 PreXPos, SI16 PreYPos, SI16 *pControllerID, SI16 *pKind )
{
	if ( ! m_bShow )
	{
		return false;
	}

	if ( IsInArea( XPos, YPos ) )
	{
		return true;
	}

	return false;
}

/*
void CControllerObj::SetBKImg( TCHAR *FileName )
{
	// NULL 또는 < 1 인 경우는 배경이 RGB 단색으로 되어 있을 수 있음으로...
	if ( FileName == NULL || _tcslen( FileName ) < 1 )
	{
		TMemSet( m_strFileName, 0, MAX_PATH );
		return;
	}
	else
	{
		_tcscpy( m_strFileName, FileName );
	}

	return;
}
*/

/*
bool CControllerObj::LoadBKImg()
{
	// NULL 또는 < 1 인 경우는 배경이 RGB 단색으로 되어 있을 수 있음으로...
	if ( GetFileName() == NULL || _tcslen( GetFileName() ) < 1 )
	{
		return false;
	}
	else
	{
		// 배경 이미지 로드되어 있다면.. 로드 할 필요 없음으로...
		if ( GetBKImg()->Image == NULL )
		{	
			if ( GP.LoadSpr( GetFileName(), GetBKImg() ) == FALSE )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool CControllerObj::LoadCaretImg( TCHAR *FileName )
{
	// NULL 또는 < 1 인 경우는 배경이 RGB 단색으로 되어 있을 수 있음으로...
	if ( FileName == NULL || _tcslen( FileName ) < 1 )
	{
		return false;
	}
	else
	{
		// 배경 이미지 로드되어 있다면.. 로드 할 필요 없음으로...
		if ( GetCaretImg()->Image == NULL )
		{	
			if ( GP.LoadSpr( FileName, GetCaretImg() ) == FALSE )
			{
				return false;
			}
		}

		return true;
	}

	return false;
}
*/

void CControllerObj::MoveControl( SI16 XPos, SI16 YPos )
{
	SetX( XPos );
	SetY( YPos );

	return;
}

void CControllerObj::DrawBorder( HDC hDC, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height )
{
	DrawRoundRect( hDC, XPos, YPos, Width, Height, IsBorder(), m_bBKColorUse, m_crBorder, m_crBKColor );
	return;
}

void CControllerObj::SetControllerPos( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight )
{
#ifdef _DEBUG
	if ( Width > SurfaceWidth )
	{
		TCHAR Buffer[ 128 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(3907);
		StringCchPrintf( Buffer,128, pText, Width, SurfaceWidth );
		MessageBox( pclClient->GetHwnd(), Buffer, TEXT("Error"), MB_OK );
	}
	if ( Height > SurfaceHeight )
	{
		TCHAR Buffer[ 128 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(3908);
		StringCchPrintf( Buffer, 128, pText, Height, SurfaceHeight );
		MessageBox( pclClient->GetHwnd(), Buffer, TEXT("Error"), MB_OK );

	}
#endif

	m_siXPos = XPos;
	m_siYPos = YPos;

	m_siWidth = Width;
	m_siHeight = Height;

	m_siMaxSurfaceWidth = SurfaceWidth;
	m_siMaxSurfaceHeight = SurfaceHeight;

	if ( m_pControllerSurfaceObj )
	{
		delete m_pControllerSurfaceObj;
		m_pControllerSurfaceObj = NULL;
	}

	m_pControllerSurfaceObj = new CControllerSurfaceObj();
	m_pControllerSurfaceObj->Create( m_siMaxSurfaceWidth, m_siMaxSurfaceHeight );

	Refresh();

	return;
}

void CControllerObj::BackGroundDrawOption( SI16 BackGroundDrawOption, SI16 Param1, SI16 Param2 )
{
	m_siBackGroundDrawOption = BackGroundDrawOption;
	m_siParam1 = Param1;
	m_siParam2 = Param2;

	return;
}

void CControllerObj::SetSurfaceWidth( SI16 siSurfaceWidht )
{
	m_siMaxSurfaceWidth = siSurfaceWidht;

	if ( m_pControllerSurfaceObj )
	{
		delete m_pControllerSurfaceObj;
		m_pControllerSurfaceObj = NULL;
	}

	m_pControllerSurfaceObj = new CControllerSurfaceObj();
	m_pControllerSurfaceObj->Create( m_siMaxSurfaceWidth, m_siMaxSurfaceHeight );

	Refresh();

	return;
}

void CControllerObj::SetSurfaceHeight( SI16 siSurfaceHeight )
{
	m_siMaxSurfaceHeight = siSurfaceHeight;

	if ( m_pControllerSurfaceObj )
	{
		delete m_pControllerSurfaceObj;
		m_pControllerSurfaceObj = NULL;
	}

	m_pControllerSurfaceObj = new CControllerSurfaceObj();
	m_pControllerSurfaceObj->Create( m_siMaxSurfaceWidth, m_siMaxSurfaceHeight );

	Refresh();

	return;
}
