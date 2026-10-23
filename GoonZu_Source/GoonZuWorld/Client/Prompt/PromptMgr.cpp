#include "PromptMgr.h"
#include "Prompt.h"
#include "EtcTool\EtcTool.h"

#include "..\FontManager\FontManager.h"

#include "../InterfaceMgr/InterfaceMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "..\Client.h"
extern cltCommonLogic* pclClient;
extern SI32		gsiResolution;

CPromptMgr	g_PromptMgr;

CPromptMgr::CPromptMgr()
{
	m_siPromptNum = 0;

	m_bSetUnderLineText		= false;
	m_bHaveUrl				= FALSE;

	m_hNormalFont			= NULL;
	m_hUnderlineFont		= NULL;

	m_ImgEncBuffer			= NULL;

	Initialize();
}

CPromptMgr::~CPromptMgr()
{
	Destroy();

	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}

	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}
}

void CPromptMgr::Initialize()
{
	m_lpSurface = NULL;

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siPromptNum = 0;

	m_bEncodeBuffer = false;

	memset( m_szCurrentURL, 0, MAX_PATH );
	//StringCchCopy( m_szCurrentURL, MAX_PATH, TEXT("") );
	m_bHaveUrl = false;

	return;
}

void CPromptMgr::Destroy()
{
	CPrompt *pTemp = m_pFirst;

	while( pTemp )
	{
		m_pFirst = m_pFirst->GetNext();
		delete pTemp;
		pTemp = m_pFirst;
	}

	if(m_hUnderlineFont != NULL)
	{
		DeleteObject(m_hUnderlineFont);
		m_hUnderlineFont = NULL;
	}

	if(m_hNormalFont != NULL)
	{
		DeleteObject(m_hNormalFont);
		m_hNormalFont = NULL;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	return;
}

void CPromptMgr::Create()
{
	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}

	if ( g_graphics.CreateSurface16( &m_lpSurface, PROMPT_DRAW_WIDTH, PROMPT_DRAW_HEIGHT, TRUE, RGB16_TRANS ) == FALSE )
	{
		MsgBox(TEXT("CreateOffScreen Error, CPromptMgr "), TEXT("fsdfs"));
	}

	m_bEncodeBuffer = false;
	m_ImgEncBuffer = new RGB16[ PROMPT_DRAW_WIDTH * PROMPT_DRAW_HEIGHT * 2 ];
	if(m_ImgEncBuffer != NULL)
	{
		memset( m_ImgEncBuffer,0, sizeof( RGB16 ) * PROMPT_DRAW_WIDTH * PROMPT_DRAW_HEIGHT * 2 );
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// 밑줄 폰트 생성
	if ( pclclient->siServiceArea == ConstServiceArea_Japan )
	{
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			m_hUnderlineFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, TRUE, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림체"));
		}
		else {
			m_hUnderlineFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, TRUE, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림체"));
		}		
	}
	else
	{
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			m_hUnderlineFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, TRUE, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("Tahoma"));
		}
		else {
			m_hUnderlineFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, TRUE, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("Tahoma"));
		}
		
	}

	if(m_hUnderlineFont == NULL)
	{
		MsgBox(TEXT("OutlineFont Error, CPromptMgr "), TEXT("Font Create"));
	}

	// 일반 폰트 생성
	if ( pclclient->siServiceArea == ConstServiceArea_Japan )
	{
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			m_hNormalFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림체"));
		}
		else {
			m_hNormalFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림체"));
		}
	}
	else
	{
		if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
			m_hNormalFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("Tahoma"));
		}
		else {
			m_hNormalFont = CreateFont( 13, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("Tahoma"));
		}		
	}

	if(m_hNormalFont == NULL)
	{
		MsgBox(TEXT("NormalFont Error, CPromptMgr "), TEXT("Font Create"));
	}

	return;
}

bool CPromptMgr::Set( TCHAR* strPrompt, TCHAR* strURL, SI32 si32RepetitionNum, COLORREF TextColor )
{
	if ( m_siPromptNum > MAX_PROMT_NUM )
	{
		return false;
	}

	CPrompt *pTemp = m_pFirst;

	while ( pTemp )
	{
		if ( pTemp->IsSame( strPrompt ) )
		{
			return false;
		}

		pTemp = pTemp->GetNext();
	}	

	CPrompt *pPrompt = new CPrompt();
	if(pPrompt == NULL)			return false;

	pPrompt->SetData( strPrompt, strURL, si32RepetitionNum, TextColor );
	PushPrompt( pPrompt );

	++m_siPromptNum;

	return true;
}

void CPromptMgr::PushPrompt( CPrompt *pPrompt )
{
	if ( m_pFirst )
	{
		m_pLast->SetNext( pPrompt );
		pPrompt->SetPre( m_pLast );
		m_pLast = pPrompt;
	}
	else
	{
		m_pFirst = pPrompt;
		m_pLast = pPrompt;
	}

	return;
}

void CPromptMgr::MoveToBack( CPrompt *pPrompt )
{
	if ( pPrompt == m_pFirst )
	{
		if ( m_pFirst->GetNext() )
		{
			m_pFirst->GetNext()->SetPre( NULL );
			m_pFirst = m_pFirst->GetNext();
			pPrompt->SetNext( NULL );
			pPrompt->SetPre( NULL );
			PushPrompt( pPrompt );
		}
		else
		{
			// 가만히 두면 됨. 노드가 1개인데 어떻게 하라고...
		}
	}
	else if ( pPrompt == m_pLast )
	{
		// 가만히 두면 됨.. 쩰 끝에 있는 놈 더 어떻게 하라고...
	}
	else
	{
		pPrompt->GetNext()->SetPre( pPrompt->GetPre() );
		pPrompt->GetPre()->SetNext( pPrompt->GetNext() );
		pPrompt->SetNext( NULL );
		pPrompt->SetPre( NULL );
		PushPrompt( pPrompt );
	}

	return;
}

void CPromptMgr::DelAllPrompt()
{
	CPrompt *pTemp = m_pFirst;

	while( pTemp )
	{
		m_pFirst = m_pFirst->GetNext();
		delete pTemp;
		pTemp = m_pFirst;

		--m_siPromptNum;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siPromptNum=0;

	return;
}

void CPromptMgr::DelPrompt( CPrompt *pPrompt )
{
	if ( pPrompt == m_pFirst )
	{
		if ( m_pFirst->GetNext() )
		{
			m_pFirst->GetNext()->SetPre( NULL );
			m_pFirst = m_pFirst->GetNext();
			delete pPrompt;
			pPrompt = NULL;
		}
		else
		{
			delete pPrompt;
			pPrompt = NULL;
			m_pFirst = NULL;
			m_pLast = NULL;
		}
	}
	else if ( pPrompt == m_pLast )
	{
		m_pLast->GetPre()->SetNext( NULL );
		m_pLast = m_pLast->GetPre();
		delete pPrompt;
		pPrompt = NULL;
	}
	else
	{
		pPrompt->GetNext()->SetPre( pPrompt->GetPre() );
		pPrompt->GetPre()->SetNext( pPrompt->GetNext() );
		delete pPrompt;
		pPrompt = NULL;
	}

	--m_siPromptNum;

	return;
}

void CPromptMgr::Action()
{

	if( m_lpSurface == NULL )			return;

	if ( m_siPromptNum == 0 )
	{
		return;
	}

	CPrompt *pTemp = m_pFirst;
	CPrompt *pTemp1 = NULL;

	while( pTemp )
	{
		pTemp1 = pTemp->ShiftLeft();	// 왼쪽으로 -2 만큼 이동, 정상이동 되면 NULL이 반환됨
		// 이동이 완료되었다면 완료된 포인터가 반환됨
		if ( pTemp1 )
		{
			pTemp = pTemp->GetNext();

			if ( pTemp1->IsSameRepetitionCount() )
			{
				DelPrompt( pTemp1 );
				pTemp1 = NULL;
			}
			else
			{
				MoveToBack( pTemp1 );
				pTemp1 = NULL;
			}
		}
		else
		{
			pTemp = pTemp->GetNext();
		}
	}

	if ( m_siPromptNum == 0 )
	{
		return;
	}


	g_graphics.ClearSurface( m_lpSurface, RGB16_TRANS );

	//------------------------------
	// 텍스트를 출력한다. 
	//-----------------------------
	HDC hdc;
	SIZE size;

	SI32 AvalibleDrawPos = -20000;
	SI32 siInterval = 20;

	cltClient *pclclient = (cltClient *)pclClient;
	if( m_lpSurface->GetDC(&hdc) == DD_OK)
	{
		HFONT hOldFont = (HFONT)SelectObject( hdc, m_hNormalFont );
		SetBkMode( hdc, TRANSPARENT );

		// URL링크 버퍼 초기화
		memset( m_szCurrentURL, 0, MAX_PATH );
		m_bHaveUrl = false;

		CPrompt *pTemp = m_pFirst;
		while( pTemp )
		{
			if ( pTemp->GetCurrentDrawPos() > AvalibleDrawPos && pTemp->GetCurrentDrawPos() <= PROMPT_DRAW_WIDTH )
			{
				GetTextExtentPoint32( hdc, pTemp->GetPrompt(), _tcslen( pTemp->GetPrompt() ), &size);
				if ( size.cx > 0 && pTemp->GetCurrentDrawPos() + size.cx > 0 )
				{
					RECT rect;

					// 그림자 글씨를 위한 위치
					SetRect( &rect, pTemp->GetCurrentDrawPos() + 1, 1, pTemp->GetCurrentDrawPos() + size.cx + 1, size.cy + 1 );

					SI32 left = rect.left;
					SI32 right = rect.right;

					if(left  < 0)		left  = 0;
					if(right < 0)		right = 0;

					// 텍스트의 화면상의 위치
					m_Rect.left  = pclclient->GetScreenRX() + left + 155 + 40;
					m_Rect.right = pclclient->GetScreenRX() + right + 155 + 40;
					m_Rect.top = pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet - 20 + pclclient->GetScreenRY() + rect.top;
					m_Rect.bottom = pclclient->siScreenMode800X600YSize + pclclient->siScreenModeYOffSet - 20 + pclclient->GetScreenRY() + rect.bottom;

					// 마우스가 텍스트 위에 있다면 텍스트에 밑줄을 그린다. 그리고 URL을 기억한다.
					if( pclclient->GameMouseX > m_Rect.left && pclclient->GameMouseX < m_Rect.right 
						&& pclclient->GameMouseY > m_Rect.top && pclclient->GameMouseY < m_Rect.bottom
						&& pTemp->GetURL() != NULL && _tcsncmp( pTemp->GetURL() , TEXT("") , MAX_PATH ) != 0 )
					{
						SelectObject( hdc, m_hUnderlineFont );

						StringCchCopy( m_szCurrentURL, MAX_PATH, pTemp->GetURL() );
						m_bHaveUrl = true;
					}
					else
					{
						SelectObject( hdc, m_hNormalFont );
					}

					// 텍스트 크기 얻음
					GetTextExtentPoint32( hdc, pTemp->GetPrompt(), _tcslen( pTemp->GetPrompt() ), &size);

					// 그림자 글씨를 위한 위치
					SetRect( &rect, pTemp->GetCurrentDrawPos() + 1, 1, pTemp->GetCurrentDrawPos() + size.cx + 1, size.cy + 1 );
					SetTextColor( hdc, RGB( 10, 10, 10 ) );
					DrawText( hdc, pTemp->GetPrompt(), pTemp->GetPromptLength(), &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

					// 일반 글자를 위한 위치
					SetRect( &rect, pTemp->GetCurrentDrawPos(), 0, pTemp->GetCurrentDrawPos() + size.cx, size.cy );
					SetTextColor( hdc, pTemp->m_TextColor );
					DrawText( hdc, pTemp->GetPrompt(), pTemp->GetPromptLength(), &rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

					AvalibleDrawPos = pTemp->GetCurrentDrawPos() + size.cx + siInterval;
					pTemp->SetMoveStatus( true );
				}
			}

			pTemp = pTemp->GetNext();
		}

		SelectObject( hdc, hOldFont );
		m_lpSurface->ReleaseDC(hdc);
	}

	if ( GP.LockSurface( m_lpSurface ) )
	{
		int size = 0;
		if(m_ImgEncBuffer)
		{
			g_graphics.EncodeXpr( m_ImgEncBuffer, PROMPT_DRAW_WIDTH, PROMPT_DRAW_HEIGHT, RGB16_TRANS, &size );
			m_bEncodeBuffer = true;
		}

#ifdef _DEBUG
		if( size > PROMPT_DRAW_WIDTH * PROMPT_DRAW_HEIGHT * 4)
		{
			MsgBox(TEXT("Xpr Size is Small"), TEXT("XPR"));
		}
#endif

		GP.UnlockSurface( m_lpSurface );

	}

	return;
}

void CPromptMgr::Draw( SI32 si32DrawXPos, SI32 si32DrawYPos )
{

	if ( m_siPromptNum == 0 )
	{
		return;
	}

	/*
	#ifdef GRA_MODE_16
	g_graphics.ClearSurface( m_lpSurface, RGB16_TRANS );
	#endif

	if ( GP.LockSurface( m_lpSurface ) == TRUE )
	{
	#ifdef GRA_MODE_16
	#else
	GP.FillScreen( COLOR_TRANS );
	#endif
	GP.UnlockSurface( m_lpSurface );
	}
	//------------------------------
	// 텍스트를 출력한다. 
	//-----------------------------
	HDC hdc;

	SIZE size;

	SI32 AvalibleDrawPos = 0;

	if( m_lpSurface->GetDC(&hdc) == DD_OK)
	{		
	HFONT hOldFont = (HFONT)SelectObject( hdc, pclClient->pclMessage->hFont );


	SetBkMode( hdc, TRANSPARENT );

	CPrompt *pTemp = m_pFirst;

	SI32 siInterval = 20;
	SI32 siPreNodeWidthSize = 0;

	while( pTemp )
	{
	if ( pTemp->GetCurrentDrawPos() > AvalibleDrawPos )
	{
	// 해야 할 것 : 한 줄이 출력되고 있으면, 그거 끝날때까지 기다렸다가 출력 처음부터 시작하기...

	//if ( pTemp != m_pFirst && pTemp->GetCurrentDrawPos() - AvalibleDrawPos < siInterval )
	//{
	//	break;
	//}

	GetTextExtentPoint32( hdc, pTemp->GetPrompt(), _tcslen( pTemp->GetPrompt() ), &size);

	SetTextColor( hdc, RGB( 10, 10, 10 ) );
	TextOut( hdc, pTemp->GetCurrentDrawPos() + 1, 0 + 1, pTemp->GetPrompt(), pTemp->GetPromptLength() );

	SetTextColor( hdc, pTemp->m_TextColor );
	TextOut( hdc, pTemp->GetCurrentDrawPos(), 0, pTemp->GetPrompt(), pTemp->GetPromptLength() );

	AvalibleDrawPos = pTemp->GetCurrentDrawPos() + size.cx + siInterval;
	pTemp->SetMoveStatus( true );
	}

	pTemp = pTemp->GetNext();
	}

	SelectObject( hdc, hOldFont );
	m_lpSurface->ReleaseDC(hdc);
	}

	RECT rect;
	SetRect( &rect, PROMPT_DRAW_WIDTH / 5, 0, PROMPT_DRAW_WIDTH - PROMPT_DRAW_WIDTH / 5, PROMPT_DRAW_HEIGHT );

	// 실제 화면 출력 부분 아래 주석 해제하면 화면 아래 부분에 출력됨.
	lpsurface->BltFast( si32DrawXPos, si32DrawYPos, m_lpSurface, &rect, DDBLTFAST_SRCCOLORKEY);
	*/
	if ( m_ImgEncBuffer && m_bEncodeBuffer )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		GP.PutXpr16( si32DrawXPos, si32DrawYPos, 
			PROMPT_DRAW_WIDTH, PROMPT_DRAW_HEIGHT,
			m_ImgEncBuffer,
			RGB16_TRANS );

		/*
		int x1, y1, x2, y2;
		int clip_x1, clip_y1, clip_x2, clip_y2;

		GP.GetClipArea( x1, y1, x2, y2 );

		clip_x1 = max( si32DrawXPos, x1 );
		clip_y1 = max( si32DrawYPos, y1 );
		clip_x2 = min( si32DrawXPos + PROMPT_DRAW_WIDTH, x2 );
		clip_y2 = min( si32DrawYPos + PROMPT_DRAW_HEIGHT, y2 );

		GP.SetClipArea( clip_x1, clip_y1, clip_x2, clip_y2 );
		*/

		// 음수는 처리하지 않는다. - by LEEKH 2007.04.09 
		/*	if(si32DrawXPos > 0 || si32DrawYPos > 0)
		{
		// 클리핑 코드 추가 - by LEEKH 2007.04.04
		int x1, x2, y1, y2;
		GP.GetClipArea(x1, y1, x2, y2);

		if ( gsiResolution == 2 )  //  창모드
		GP.SetClipArea( 41 + si32DrawXPos, si32DrawYPos, 41 + si32DrawXPos + 855, si32DrawYPos + 17 );
		else
		GP.SetClipArea( 41 + si32DrawXPos, si32DrawYPos, 41 + si32DrawXPos + 910, si32DrawYPos + 17 );

		GP.PutXpr16( si32DrawXPos, si32DrawYPos, 
		PROMPT_DRAW_WIDTH, PROMPT_DRAW_HEIGHT,
		m_ImgEncBuffer,
		RGB16_TRANS );

		GP.SetClipArea(x1, y1, x2, y2);
		}
		*/
		/*
		GP.SetClipArea( x1, y1, x2, y2 );
		*/
	}

	return;
}
void CPromptMgr::SetUnderLineText(bool bSet )
{
	m_bSetUnderLineText = bSet;
}