#include "ChatEdit.h"

#include "../ControllerMgr.h"
#include "../ControllerSurfaceObj.h"

#include "../InterfaceMgr.h"
#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

#include <zmouse.h>

CChatEdit::CChatEdit( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_ImgEncBuffer = NULL;

	m_siMaxChatEditDataNum = MAX_CHATEDIT_DATA_NUM;

	m_siLineHeight = CHATEDIT_LINEHEIGHT;

	m_bAutoScroll = true;

	m_siAvailabeUseWidth = 0;

	m_bShowScroll = false;

	m_pScroll = new CScroll( pParent );

	m_EmoticonData.reserve( 200 );

	m_pEmoticonSpr = NULL;

	siShowChatDataFilter = 0;
}

CChatEdit::~CChatEdit()
{
	Clear();

	SAFE_DELETE( m_pScroll );
}

void CChatEdit::Create( SI16 ControllerID, TCHAR *Text, 
					   TCHAR *FileName, SI16 FontIndex, 
					   bool BKTileImg, SI16 BKTileImgNum,
					   SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
					   SI16 SurfaceWidth, SI16 SurfaceHeight,
					   SI16 LineHeight, bool AutoScroll,
					   SI16 OffsetX, SI16 OffsetY,
					   SI16 ScrollWidth, SI16 ScrollHeight,
					   UI16 ScrollButtonWidth, UI16 ScrollButtonHeight
					   )
{
	m_siLineHeight = LineHeight;
	m_bAutoScroll = AutoScroll;

	m_siOffsetX = OffsetX;
	m_siOffsetY = OffsetY;

	m_siAvailabeUseWidth = Width - OffsetX - SCROLL_WIDTH ;

	CControllerObj::Create( ControllerID, CONTROLLER_KIND_CHATEDIT, 
							FileName, FontIndex, BKTileImg, BKTileImgNum, 
							XPos, YPos, Width, Height, 
							SurfaceWidth, SurfaceHeight );

	if ( ScrollWidth == -1 && ScrollHeight == -1 )
	{
		m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, 
						XPos + Width - SCROLL_WIDTH - 1, YPos + OffsetY, 
						SCROLL_WIDTH, Height - OffsetY * 2, 
						SCROLL_WIDTH, Height - OffsetY * 2, LineHeight );
	}
	else
	{
		m_pScroll->SetScrollButtonSize( ScrollButtonWidth, ScrollButtonHeight );
		m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, 
						XPos + Width - ScrollWidth - 1, YPos + OffsetY, 
						ScrollWidth, Height - OffsetY * 2, 
						ScrollWidth, Height - OffsetY * 2, LineHeight );
	}

	m_pEmoticonSpr = pclClient->GetGlobalSpr(GIMG_EMOTICON_SMALL);


}

bool CChatEdit::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	/*
	POINT ChatEditScrollPt;
	ChatEditScrollPt.x = pt.x - GetX();
	ChatEditScrollPt.y = pt.y - GetY();

	if( m_pScroll->HandleMouse( uMsg, ChatEditScrollPt, wParam, lParam ) )
		return true;
	*/
	if( m_pScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( ! m_pScroll->IsFocus() )
		{
			GetParent()->RequestFocus( m_pScroll );
		}
		return true;
	}

	switch( uMsg )
	{
	case WM_MOUSEWHEEL:
		{
			if ( IsInArea( pt.x, pt.y ) )
			{
				int zDelta = (short) HIWORD(wParam) / WHEEL_DELTA;
				m_pScroll->Scroll( -zDelta );

				return true;
			}
		}
		break;
	}

	return false;
}


void CChatEdit::Insert( stChatEditData *pChatEditData )
{
	if ( pChatEditData == NULL ||
		pChatEditData->strNameData == NULL ||
		pChatEditData->strChatData == NULL )
	{
		return;
	}

	deque<stChatEditData>::size_type ChatEditDataNum;
	ChatEditDataNum = m_ChatEditDeque.size();

	if ( ChatEditDataNum > m_siMaxChatEditDataNum )
	{
		m_ChatEditDeque.pop_front();
	}

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			HDC hDC;

			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				cltClient *pclclient = ( cltClient * )pclClient;
				HFONT hFont;

				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 
						0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 
						0, 0, NONANTIALIASED_QUALITY, 0, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 
						0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 
						0, 0, 0, 0, GetFontKind() );
				}
				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetBkMode( hDC, TRANSPARENT );

				TCHAR TempBuffer[ CHATEDIT_NAMEDATA_LENGTH + CHATEDIT_CHATDATA_LENGTH + 20 ];	// 20 은 이름과 채팅 사이에 [] : [] 뭐 이런거 때문에 더 줬음... 넉넉히 20

				TCHAR *EachLineEndPtr[ CHATEDIT_MAX_EACHLINE_LINENUMBER ];
				for ( SI16 j = 0; j < CHATEDIT_MAX_EACHLINE_LINENUMBER; ++j )
				{
					EachLineEndPtr[ j ] = NULL;
				}

				StringCchPrintf( TempBuffer, 
					CHATEDIT_NAMEDATA_LENGTH + CHATEDIT_CHATDATA_LENGTH + 20, 
					TEXT("[%s] : %s"), 
					pChatEditData->strNameData, pChatEditData->strChatData );

				pChatEditData->siLineCount = LineExamine( hDC, TempBuffer, _tcslen( TempBuffer ), EachLineEndPtr );

				SelectObject( hDC, hOldFont );
				DeleteObject( hFont );

				lpSurface->ReleaseDC( hDC );
			}
		}
	}

	m_ChatEditDeque.push_back( *pChatEditData );

	deque<stChatEditData>::iterator itr;
	itr = m_ChatEditDeque.begin();
	SI32 TotalLineCount = 0;
	while ( itr != m_ChatEditDeque.end() )
	{
		stChatEditData *pChatEditData = &(*itr);
		if ( pChatEditData && pChatEditData->siLineCount > 0 )
		{
			TotalLineCount += pChatEditData->siLineCount;
		}
		++itr;
	}

	m_pScroll->SetTrackRange( 0, TotalLineCount );

	SI16 ScrollPageSize = m_pScroll->GetPageSize();
	if ( TotalLineCount > ScrollPageSize )		// Line 방식으로 변형
	{
		if(IsShow() == true)		m_pScroll->Show( true );

		if ( IsShow() )
		{
			if( m_bAutoScroll )
			{
				m_pScroll->ScrollMove( TotalLineCount - ScrollPageSize );
			}
			else
			{
				Refresh();
			}
		}
	}
	else if ( IsShow() )
	{
		Refresh();
	}

	return;
}

void CChatEdit::MakeTextImgEncBuffer()
{
	if ( ( ! IsAvailable() ) ||  ( ! IsShow() ) )
	{
		return;
	}

	if ( m_ImgEncBuffer )
	{
		delete [] m_ImgEncBuffer;
		m_ImgEncBuffer = NULL;
	}


	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			cltClient *pclclient = ( cltClient * )pclClient;

			SI16 SurfaceWidth = GetSurfaceWidth();
			SI16 SurfaceHeight = GetSurfaceHeight();

			m_ImgEncBuffer = new RGB16[ SurfaceWidth * SurfaceHeight * 2 ];

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI16 i = 0, j = 0;

			HDC hDC;
			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				HFONT hFont;
				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 
						0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 
						0, 0, NONANTIALIASED_QUALITY, 0, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 
						0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 
						0, 0, 0, 0, GetFontKind() );
				}		

				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetTextColor( hDC, GetFontColor() );

				SetBkMode( hDC, TRANSPARENT );

				DrawBorder( hDC, 0, 0, SurfaceWidth, SurfaceHeight );

			/*	deque<stChatEditData>::iterator itr;*/

				SI16 CurrentDrawXPos = m_siOffsetX;
				SI16 CurrentDrawYPos = m_siOffsetY;


				//itr = m_ChatEditDeque.begin();

				//SI32 CurrentScrollLine = m_pScroll->GetTrackPos();
				//if ( CurrentScrollLine > 0 )
				//{

				//	SI32 LineCount = 0;
				//	while( itr != m_ChatEditDeque.end() )
				//	{
				//		stChatEditData *pChatEditData = &(*itr);
				//		if ( pChatEditData )
				//		{
				//			if ( pChatEditData->siLineCount > 0 )
				//			{
				//				if ( LineCount + pChatEditData->siLineCount > CurrentScrollLine )			// CurrentScrollLine 은 0 부터 시작함으로 
				//				{
				//					CurrentDrawYPos -=  ( ( CurrentScrollLine - LineCount ) * m_siLineHeight );
				//					break;
				//				}
				//				else
				//				{
				//					LineCount += pChatEditData->siLineCount;
				//				}
				//			}
				//		}

				//		++itr;
				//	}
				//}

				// 밑에부터 거꾸로 찍자;

				deque<stChatEditData>::reverse_iterator itr;

				itr = m_ChatEditDeque.rbegin();

				CurrentDrawYPos = CurrentDrawYPos + ( SurfaceHeight - m_siLineHeight * 2);

				bool bEmoticon = false;

				SI32 CurrentScrollLine = ( m_pScroll->GetTrackReversePos() - m_pScroll->GetPageSize() );

				while( itr != m_ChatEditDeque.rend() )
				{					
					stChatEditData *pChatEditData = &(*itr);
					bool bFilter = true;
					if( siShowChatDataFilter > 0 )
					{
						if( pChatEditData->siChatKind != siShowChatDataFilter )
							bFilter = false;
					}

					if ( bFilter == true && pChatEditData && pChatEditData->siLineCount > 0 )
					{
						if ( CurrentScrollLine > 0 )
						{
							--CurrentScrollLine;
							++itr;
							continue;
						}

						//if ( CurrentDrawYPos + m_siLineHeight > SurfaceHeight - m_siOffsetY )
						//{	
						//	break;
						//}
						if ( CurrentDrawYPos < m_siOffsetY )
							break;

						if ( pChatEditData->siLineCount == 1 )
						{
							DrawOneLineData( hDC , CurrentDrawXPos, CurrentDrawYPos, pChatEditData, &bEmoticon );
							if ( bEmoticon )
							{
								CurrentDrawYPos -= m_pEmoticonSpr->GetYSize() - 2;
								bEmoticon = false;
							}
							else
							{
								CurrentDrawYPos -= m_siLineHeight;
							}
						}
						else if ( pChatEditData->siLineCount == -1 )
						{
							OutputDebugString(  TEXT("pChatEditData->siLineCount == -1\r\n") );
						}
						else
						{	
							CurrentDrawYPos -= ( m_siLineHeight * ( pChatEditData->siLineCount - 1) );
							DrawMultiLineData( hDC , CurrentDrawXPos, CurrentDrawYPos, pChatEditData );
							CurrentDrawYPos -= m_siLineHeight;
						}
					}

					++itr;
				}
				
				SelectObject( hDC, hOldFont );
				DeleteObject( hFont );

				lpSurface->ReleaseDC( hDC );
			}
			
			if ( GP.LockSurface( lpSurface ) )
			{
				vector< stEmoticonData >::iterator itr;

				itr = m_EmoticonData.begin();


				while ( itr != m_EmoticonData.end() )
				{
					// 이모티콘을 차례로 출력한다.
					GP.PutSpr( m_pEmoticonSpr, (*itr).siXpos, (*itr).siYpos, (*itr).siFontIndex );
					++itr;
				}
				m_EmoticonData.clear();

				if(m_ImgEncBuffer)
				{
					int size;
					g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight , RGB16_TRANS, &size );
				}
				GP.UnlockSurface( lpSurface );
			}
		}
	}

	return;
}

SI16 CChatEdit::DrawNameData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData )
{
	if ( pChatEditData->strNameData == NULL )
	{
		return 0;
	}

	SI16 NameDataLen = _tcslen( pChatEditData->strNameData );

	if ( NameDataLen < 1 )
	{
		return 0;
	}

	TCHAR NameDataPart[ 128 ];
	StringCchPrintf( NameDataPart, 128, TEXT("[%s] : "), pChatEditData->strNameData );

	SetTextColor( hDC, GetChatEditColor( pChatEditData->siNameDataColorIndex ) );

	NameDataLen = _tcslen( NameDataPart );
	TextOut( hDC, DrawXPos, DrawYPos, NameDataPart, NameDataLen );

	return GetTextExtent( hDC, NameDataPart, NameDataLen );
}

void CChatEdit::DrawOneLineChatData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData, bool *bEmoticon)
{
	if ( pclClient->siServiceArea == ConstServiceArea_China )
	{
		if ( pChatEditData->strChatData == NULL )
		{
			return;
		}

		SI16 ChatDataLen = _tcslen( pChatEditData->strChatData );

		if ( ChatDataLen < 1 )
		{
			return;
		}

		SetTextColor( hDC, GetChatEditColor( pChatEditData->siChatDataColorIndex ) );

		TCHAR buf[MAX_CHAT_DATA_LENGTH] = {0,};
		SI16 bufIndex = 0;
		RECT rect;

		rect.left = DrawXPos;
		rect.top = DrawYPos;
		rect.right = DrawXPos;

		stEmoticonData emoticonData;

		TCHAR emoticonBuf[4] = {0,};

		for ( SI16 i = 0; i < ChatDataLen; i++ )
		{
			buf[bufIndex] = pChatEditData->strChatData[i];

			if ( buf[bufIndex] == '~')
			{ 
				SI16 fontIndex = 0;
				bool bFindEmoticon = false;
				if( *(buf+bufIndex+1) >= '0' && *(buf+bufIndex+1) <= '9' )
				{
					for ( SI16 j=0 ; j < MAX_EMOTICON_NUMBER; ++j )

					{
						if ( _tcsncmp( pChatEditData->strChatData + i + 1 , 
							((cltClient*)pclClient)->clEmoticonInfo.GetCode(j), 3) == 0 )
						{
							fontIndex = j;
							*bEmoticon = true;
							bFindEmoticon = true;
							break;
						}
					}
				}


				if ( bFindEmoticon )
				{
					buf[bufIndex] = '\0';

					DrawText( hDC, buf, _tcslen(buf), &rect, DT_CALCRECT|DT_LEFT );


					emoticonData.siFontIndex = fontIndex;
					emoticonData.siXpos = rect.right;
					if ( m_pEmoticonSpr->GetYSize() > m_siLineHeight )
					{
						emoticonData.siYpos = DrawYPos - ( m_pEmoticonSpr->GetYSize() - m_siLineHeight );
					}
					else
					{
						emoticonData.siYpos = DrawYPos;
					}

					m_EmoticonData.push_back( emoticonData );

					TextOut( hDC, rect.left, rect.top, buf, ChatDataLen );

					rect.left = rect.right + m_pEmoticonSpr->GetXSize();

					bufIndex = -1;
					ZeroMemory( buf, MAX_CHAT_DATA_LENGTH );

					i += 3;
				}
			}
			++bufIndex;
		}
		buf[bufIndex] = '\0';

		TextOut( hDC, rect.left, rect.top, buf, ChatDataLen );

		return;
	}
	else
	{
		if ( pChatEditData->strChatData == NULL )
		{
			return;
		}

		SI16 ChatDataLen = _tcslen( pChatEditData->strChatData );

		if ( ChatDataLen < 1 )
		{
			return;
		}

		SetTextColor( hDC, GetChatEditColor( pChatEditData->siChatDataColorIndex ) );
		TextOut( hDC, DrawXPos, DrawYPos, pChatEditData->strChatData, ChatDataLen );

		return;
	}
}

void CChatEdit::DrawMultiLineChatData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData, TCHAR *pDrawStartChatData, SI16 DrawChatEditDataLen )
{
	if ( pDrawStartChatData == NULL || DrawChatEditDataLen < 1 )
	{
		return;
	}

	SI16 ChatDataLen = _tcslen( pChatEditData->strChatData );

	if ( ChatDataLen < 1 || ChatDataLen < DrawChatEditDataLen )
	{
		return;
	}

	SetTextColor( hDC, GetChatEditColor( pChatEditData->siChatDataColorIndex ) );
	TextOut( hDC, DrawXPos, DrawYPos, pDrawStartChatData, DrawChatEditDataLen );

	return;
}

void CChatEdit::DrawOneLineData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData, bool *bEmoticon )
{
	if ( pChatEditData->strNameData == NULL || pChatEditData->strChatData == NULL )
	{
		return;
	}

	SI16 NameDataDrawSize = 0;
	NameDataDrawSize += DrawNameData( hDC, DrawXPos, DrawYPos, pChatEditData );

	DrawOneLineChatData( hDC, DrawXPos + NameDataDrawSize, DrawYPos, pChatEditData, bEmoticon );


	return;
}

void CChatEdit::DrawMultiLineData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData )
{	
	if ( pChatEditData->strNameData && pChatEditData->strChatData )
	{
		SI16 i = 0;
		SI16 j = 0;
		TCHAR TempBuffer[ CHATEDIT_NAMEDATA_LENGTH + CHATEDIT_CHATDATA_LENGTH + 20 ];

		TCHAR *EachLineEndPtr[ CHATEDIT_MAX_EACHLINE_LINENUMBER ];
		for ( j = 0; j < CHATEDIT_MAX_EACHLINE_LINENUMBER; ++j )
		{
			EachLineEndPtr[ j ] = NULL;
		}

		StringCchPrintf( TempBuffer, CHATEDIT_NAMEDATA_LENGTH + CHATEDIT_CHATDATA_LENGTH + 20, TEXT("[%s] : %s"), pChatEditData->strNameData, pChatEditData->strChatData );

		pChatEditData->siLineCount = LineExamine( hDC, TempBuffer, _tcslen( TempBuffer ), EachLineEndPtr );

		if ( pChatEditData->siLineCount == 1 )
		{
			bool bEmoticon = false;
			DrawOneLineChatData( hDC , DrawXPos, DrawYPos, pChatEditData, &bEmoticon);
		}
		else if ( pChatEditData->siLineCount > 1 && pChatEditData->siLineCount < CHATEDIT_MAX_EACHLINE_LINENUMBER )
		{
			if ( DrawYPos + m_siLineHeight < GetSurfaceHeight() - m_siOffsetY )
			{	
				SI16 NameDataDrawSize = DrawNameData( hDC , DrawXPos, DrawYPos, pChatEditData );

				SI16 NameDataLen = 0;
				if ( pChatEditData->strNameData )
				{
					NameDataLen = _tcslen( pChatEditData->strNameData );
				}

				DrawMultiLineChatData( hDC, DrawXPos + NameDataDrawSize, DrawYPos, pChatEditData, pChatEditData->strChatData, EachLineEndPtr[ 0 ] - &TempBuffer[ NameDataLen + 4 ] );		// +4 는 [] :  스페이스 포함 5자인데, 1개 더 빼줘야 함. 계산하면 나옮
			}
			
			TCHAR *DrawStartCharPos = NULL;
			for ( i = 1; i < pChatEditData->siLineCount; ++i )
			{
				DrawYPos += m_siLineHeight;

				if ( DrawYPos + m_siLineHeight > GetSurfaceHeight() - m_siOffsetY )
				{
					return;
				}

				DrawStartCharPos = EachLineEndPtr[ i - 1 ];
				++DrawStartCharPos;
				DrawMultiLineChatData( hDC, DrawXPos, DrawYPos, pChatEditData, DrawStartCharPos, EachLineEndPtr[ i ] - EachLineEndPtr[ i - 1 ] );
			}
		}
		else
		{
			pChatEditData->siLineCount = -1;
		}
	}

	return;
}

void CChatEdit::Clear()
{
	SAFE_DELETE( m_ImgEncBuffer );

	m_ChatEditDeque.clear();

	m_bShowScroll = false;
	m_pScroll->Show( false );
	m_pScroll->SetTrackRange( 0, 0 );

	return;
}

void CChatEdit::Refresh()
{
	MakeTextImgEncBuffer();



	return;
}

void CChatEdit::RefreshScroll()
{
	deque<stChatEditData>::iterator itr;
	itr = m_ChatEditDeque.begin();
	SI32 TotalLineCount = 0;
	while ( itr != m_ChatEditDeque.end() )
	{
		stChatEditData *pChatEditData = &(*itr);
		if ( pChatEditData && pChatEditData->siLineCount > 0 )
		{
			TotalLineCount += pChatEditData->siLineCount;
		}
		++itr;
	}

	// 사이즈도 조정한다.
	m_pScroll->SetScrollPos(GetX() + GetWidth() - m_pScroll->GetWidth() - 1,
							GetY() + m_siOffsetY, 
							m_pScroll->GetWidth(), GetHeight() - m_siOffsetY * 2);

	m_pScroll->SetTrackRange( 0, TotalLineCount );

	// 일단 끄고,
	m_pScroll->Show( false );

	SI16 ScrollPageSize = m_pScroll->GetPageSize();
	if ( TotalLineCount > ScrollPageSize )		// Line 방식으로 변형
	{
		// 보여질 상황이라면, 다시 켠다.
		m_pScroll->Show( true );

		if ( IsShow() )
		{
			if( m_bAutoScroll )
			{
				m_pScroll->ScrollMove( TotalLineCount - ScrollPageSize );
			}
		}
	}
	return;
}


void CChatEdit::Show( bool ShowOption )
{
	CControllerObj::Show( ShowOption );

	if ( IsShow() )
	{
		Refresh();
	}
	else
	{
		// 스크롤바도 함께 꺼진다.
		m_pScroll->Show(false);
	}

	return;
}

void CChatEdit::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	cltClient *pclclient = (cltClient *)pclClient;

	TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
	
	if ( pBkImg )
	{
		if ( IsBKTileImg() )
		{
			DrawTileImg( pBkImg, GetDrawOption(), GetFontIndex(), GetAlphaValue(), BaseXPos + GetX(), BaseYPos + GetY(), GetSurfaceWidth(), GetSurfaceHeight(), 9 );
		}
		else
		{
			if ( GetDrawOption() == SPR_PUTSPRT )
			{
				GP.PutSpr(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetFontIndex() );
			}
			else
			{
				GP.PutSprAlpha(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetAlphaValue(), GetFontIndex() );
			}
		}
	}

	if ( m_ImgEncBuffer && IsShow() )
	{

		GP.PutXpr16( BaseXPos + GetX(), BaseYPos + GetY(), 
			GetSurfaceWidth(), GetSurfaceHeight(),
			m_ImgEncBuffer,
			RGB16_TRANS );
	}

	if ( m_pScroll->IsShow() )
	{
		m_pScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	return;
}

SI16 CChatEdit::LineExamine( HDC hDC, TCHAR *Text, int TextLen, TCHAR **pEachLineEndPtr )
{
	if ( Text == NULL || TextLen < 1 )
	{
		OutputDebugString(  TEXT("Error 6") );
		return -1;
	}

	if ( IsMultiLine( hDC, Text, TextLen ) )
	{	
		SI16 CurrentXSize = 0;
		SI16 CharSize = 0;
		SI16 LineCounter = 0;

		SI16 i = 0;
		for ( i = 0; i < TextLen; ++i )
		{
			if ( IsDBCS( Text[ i ], Text[ i + 1 ] ) )
			{
				if ( Text[ i ] == '\r')
				{
					pEachLineEndPtr[ LineCounter++ ] = &Text[ i + 1 ];
					CurrentXSize = 0;
				}
				else
				{
					CharSize = GetCharWidth( hDC, &Text[ i ], true );

					if ( CurrentXSize + CharSize >= m_siAvailabeUseWidth )
					{
						pEachLineEndPtr[ LineCounter++ ] = &Text[ i - 1  ];

						CurrentXSize = CharSize;
					}
					else
					{
						CurrentXSize += CharSize;
					}
				}

				++i;
			}
			else
			{
				CharSize = GetCharWidth( hDC, &Text[ i ], false );

				if ( CurrentXSize + CharSize >= m_siAvailabeUseWidth )
				{
					pEachLineEndPtr[ LineCounter++ ] = &Text[ i - 1  ];

					CurrentXSize = CharSize;
				}
				else
				{
					CurrentXSize += CharSize;
				}
			}
		}

		if ( CurrentXSize > 0 )
		{
			pEachLineEndPtr[ LineCounter++ ] = &Text[ i - 1  ];
		}

		return LineCounter;
	}
	else
	{
		return 1;
	}
}


bool CChatEdit::IsMultiLine( HDC hDC, TCHAR *Text, int TextLen )
{
	if ( Text == NULL || TextLen < 1 )
	{
		OutputDebugString(  TEXT("3") );
		return false;
	}

	SIZE sz;
	if ( GetTextExtentPoint32( hDC, Text, TextLen, &sz) == FALSE )
	{
		OutputDebugString(  TEXT("2") );
		return false;
	}
	else
	{
		if ( sz.cx >= m_siAvailabeUseWidth )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CChatEdit::IsPrevWordDBCS( TCHAR *Text, SI16 nPos )
{
	if ( Text == NULL || nPos < 1 )
	{
		OutputDebugString(  TEXT("Error IsPrevDBCS0") );

		return false;
	}

	if ( nPos < 2 )
	{
		return false;
	}
	else
	{
		if ( IsDBCS( Text[ nPos - 2 ], Text[ nPos - 1 ] ) && Text[ nPos - 2 ] != 'r' )
		{
			return true;
		}
	}

	return false;
}

SI16 CChatEdit::GetCharWidth( HDC hDC, TCHAR* OneChar, bool DBCS )
{	
	if ( OneChar == NULL )
	{
		OutputDebugString(  TEXT("Error 4") );
		return 0;
	}

	SIZE sz;
	if ( DBCS )
	{
		if ( GetTextExtentPoint32( hDC, OneChar, 2, &sz ) ) 
		{
			return sz.cx;
		}
		else
		{
			OutputDebugString(  TEXT("0") );
			return 0;
		}
	}
	else
	{
		if ( GetTextExtentPoint32( hDC, OneChar, 1, &sz ) ) 
		{
			return sz.cx;
		}
		else
		{
			OutputDebugString(  TEXT("1") );
			return 0;
		}
	}
}