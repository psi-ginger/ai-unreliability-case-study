#include "../../../common/Char/CharCommon/Char-Common.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../CommonLogic/MsgType-System.h"
#include "DrawChat.h"
#include "DrawChatSurface.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "../../Client/client.h"

extern cltCommonLogic* pclClient;

CDrawChat::CDrawChat()
{

	m_pImageBuffer = NULL;
	m_pTextBuffer = NULL;
	
	m_pTitleBackBuffer = NULL;
	m_pTitleBuffer = NULL;
	
	m_bDrawTitle = false;

	m_siCharUnique = 0;
	Initialize( m_siCharUnique ); 

}

CDrawChat::~CDrawChat()
{
	Destroy();
}

void CDrawChat::Initialize(SI32 siCharUnique)
{
	
	if( m_pImageBuffer ) {
		delete [] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}

	if( m_pTextBuffer ) {
		delete [] m_pTextBuffer;
		m_pTextBuffer = NULL;
	}

	if( m_pTitleBackBuffer ) {
		delete [] m_pTitleBackBuffer;
		m_pTitleBackBuffer = NULL;
	}

	if( m_pTitleBuffer ) {
		delete [] m_pTitleBuffer;
		m_pTitleBuffer = NULL;
	}


	m_si32CurrentFrame	= 0;
	m_bContinueDraw		= false;
	m_bDrawStatus		= false;
	m_siDrawChatKind	= 0;
	
	m_siChatMode		= 0;


	m_siFadeMode		= 0;
	m_siFadeStep		= 1;
	m_bDrawTitle  = false;

	m_siCharUnique	=	siCharUnique	;

	return;
}

void CDrawChat::Destroy()
{
	
	if( m_pImageBuffer ) {
		delete [] m_pImageBuffer;
		m_pImageBuffer = NULL;
	}

	if( m_pTextBuffer ) {
		delete [] m_pTextBuffer;
		m_pTextBuffer = NULL;
	}	

	if( m_pTitleBackBuffer ) {
		delete [] m_pTitleBackBuffer;
		m_pTitleBackBuffer = NULL;
	}

	if( m_pTitleBuffer ) {
		delete [] m_pTitleBuffer;
		m_pTitleBuffer = NULL;
	}


	return;
}

void CDrawChat::Set( CDrawChatSurface *pDrawChatSurface, TCHAR *text, bool bContinueDraw, SI32 siSelectChatKind, SI32 siShopMode ,TCHAR *shopTitle)
{
	m_bDrawTitle = false;

	m_siFadeMode = 1;
	m_siFadeStep = 0;

	if( m_pImageBuffer == NULL ) m_pImageBuffer = new RGB16[ ( MAX_CHAT_SURFACE_WIDTH + 10 ) * ( MAX_CHAT_SURFACE_HEIGHT + 10 ) ];
	if( m_pTextBuffer == NULL )  m_pTextBuffer = new RGB16[ ( MAX_CHAT_SURFACE_WIDTH + 10 ) * ( MAX_CHAT_SURFACE_HEIGHT + 10 ) ];

	LPDIRECTDRAWSURFACE7 lpSurface = pDrawChatSurface->GetChatSurface();

	// 메모리 보안 코드 - by LEEKH 
	if( lpSurface == NULL )				return;

	HFONT hFont, hOldFont;
	if ( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
		hFont = g_FontManager.SetFont( CHAT_FONT_HEIGHT_SIZE );
	}
	else
	{
		hFont = g_FontManager.SetFont( CHAT_FONT_HEIGHT_SIZE , TEXT("Tahoma") );
	}

	//=================================================================================
	// 채팅메시지 줄바꿈 기능 - " " 를 중간에 넣어준다.
	//=================================================================================
	TCHAR	szConvertText[1024] = { '\0' };
	if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
	{
		HDC hdc;
		if( lpSurface->GetDC(&hdc) == DD_OK)
		{
			SI32	siLineLength = 0;
			SIZE sz;

			hOldFont = (HFONT)SelectObject( hdc, hFont );

			GetTextExtentPoint32(hdc, text, _tcslen(text), &sz);
			if(sz.cx > MAX_CHAT_SURFACE_WIDTH - 1 - 4)
			{
				SI32 siPtr = 0;
				while(text[siPtr] != NULL)
				{
					if(IsDBCSLeadByte(text[siPtr]))
					{
						GetTextExtentPoint32(hdc, &text[siPtr], 2, &sz);
						siLineLength += sz.cx == 0 ? 16 : sz.cx;
						if(siLineLength > MAX_CHAT_SURFACE_WIDTH - 1 - 4)
						{
							_tcsncat(szConvertText, " ", 1);
							siLineLength = 0;
						}
						_tcsncat(szConvertText, &text[siPtr], 2);
						siPtr += 2;
					}
					else
					{
						GetTextExtentPoint32(hdc, &text[siPtr], 1, &sz);
						siLineLength += sz.cx == 0 ? 8 : sz.cx;
						if(siLineLength > MAX_CHAT_SURFACE_WIDTH - 1 - 4)
						{
							//_tcsncat(szConvertText, " ", 1);
							//siLineLength = 0;
						}
						_tcsncat(szConvertText, &text[siPtr], 1);
						siPtr++;
					}
				}
			}
			else
			{
				MStrCpy(szConvertText, text, sizeof(szConvertText));
			}

			SelectObject( hdc, hOldFont );
			lpSurface->ReleaseDC(hdc);
		}
	}
	//=================================================================================

	HDC hdc;
	RECT rect;
	rect.left = 4;		// 왼쪽에 테두리 때문
	rect.right = MAX_CHAT_SURFACE_WIDTH - 1 - 4;			// 테두리( 좌(4) )
	rect.top = 4;		// 위에 테두리 때문
	rect.bottom = MAX_CHAT_SURFACE_HEIGHT - 1 - 4 - 10;		// 테두리( 위(4), 꼬리(10)

	m_si32Width = MAX_CHAT_SURFACE_WIDTH - 1;

	// Draw 상태를 설정한다. (JAYCEE)
	m_siDrawChatKind = siSelectChatKind ;
	switch ( siSelectChatKind )
	{
	case USER_CHATTING:			// 일반 채팅
		{
			m_siChatMode = CHATMODE_NORMAL;
	
			m_bContinueDraw = bContinueDraw;
			m_si32CurrentFrame = 0;
			m_bDrawStatus = true;

			TSpr *DrawChatSpr = pDrawChatSurface->GetDrawChatImage(); 
			TSpr *TailSpr = pDrawChatSurface->GetDrawChatTailImage();
			
			// 서페이스에 글자 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			vector<stEmoticonDataDraw>	vEmoticonData;
			stEmoticonDataDraw			EmoticonData;
			bool bFoundEmoticon = false;
			SI32 lineHeight = CHAT_FONT_HEIGHT_SIZE;
			TSpr* pEmoticonSpr = pclClient->GetGlobalSpr(GIMG_EMOTICON_SMALL);
			bool bMultiLine	= false;

			if( lpSurface->GetDC(&hdc) == DD_OK)
			{				
				hOldFont = (HFONT)SelectObject( hdc, hFont );

				SetBkMode( hdc, TRANSPARENT );

				SetTextColor(hdc, RGB( 0, 0, 0));

				LONG curX = rect.left;
				LONG curY = rect.top;
				LONG maxX = rect.right;

				// 이모티콘 출력을 위한 루틴 NOTJAPAN NOTGLOBAL[예정없음]
				if ( ConstServiceArea_China == pclClient->siServiceArea )
				{
					size_t textSize = _tcslen( text );

					int byteCnt = 0;
					int LineCount = 0;

					for ( UI32 i=0; i<textSize ; ++i )
					{
						if( text[i] == '~' )
						{
							if( *(text+i+1) >= '0' && *(text+i+1) <= '9' )
							{
								for ( SI16 j=0 ; j < MAX_EMOTICON_NUMBER; ++j )


								{
									if ( _tcsncmp( text+i+1,
										((cltClient*)pclClient)->clEmoticonInfo.GetCode(j), 
										3) == 0 )
									{
										bFoundEmoticon = true;

										if ( curX + pEmoticonSpr->GetXSize() > maxX )
										{
											curX = 4;
											curY += lineHeight;
										}

										EmoticonData.Set( curX, curY-(pEmoticonSpr->GetYSize()- CHAT_FONT_HEIGHT_SIZE)/2, j );
										vEmoticonData.push_back( EmoticonData );
										i += 3;
										curX += pEmoticonSpr->GetXSize();
										lineHeight = pEmoticonSpr->GetYSize();

										break;
									}
								}
							}
						}

						if ( bFoundEmoticon )
						{
							bFoundEmoticon = false;
							continue;
						}

						byteCnt = IsDBCSLeadByte(text[i]) ? 2 : 1;

						SIZE sz;
						GetTextExtentPoint32( hdc, text+i, byteCnt, &sz );
						if ( curX + sz.cx > maxX )
						{
							curX = 4;
							curY += lineHeight;

							lineHeight = CHAT_FONT_HEIGHT_SIZE;

							bMultiLine = true;
						}

						TextOut( hdc, curX, curY, text+i, byteCnt );
						curX += sz.cx;

						i += byteCnt - 1;
					}
					m_si32Height = curY + CHAT_FONT_HEIGHT_SIZE;
				}
				else
				{
					if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
					{
						m_si32Height = DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );
						DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /* | DT_CALCRECT */ );
					}
					else
					{
						m_si32Height = DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /*| DT_CALCRECT */ );
						DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT  );
					}
				}


				m_si32Height += 8 + 14;		// 위(4), 아래(4) 테두리, 꼬리(14)

				if ( CHAT_FONT_HEIGHT_SIZE > m_si32Height )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else if ( m_si32Height > MAX_CHAT_SURFACE_HEIGHT )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else
				{
					// 이모티콘 출력을 위한 루틴 NOTJAPAN NOTGLOBAL[예정없음]
					if ( ConstServiceArea_China == pclClient->siServiceArea)
					{
						if ( bMultiLine )
						{
							m_si32Width = MAX_CHAT_SURFACE_WIDTH;
						}
						else
						{
							m_si32Width = curX + 4;
						}
					}
					else
					{
						m_si32Width = rect.right - rect.left + 1 + 8;
					}

					if ( m_si32Width > MAX_CHAT_SURFACE_WIDTH )
					{
						m_si32Width = MAX_CHAT_SURFACE_WIDTH;
					}

					SI32 Module = m_si32Width % 4;
					if ( Module !=  0 )
					{
						m_si32Width += 4 - m_si32Width % 4;
					}
				}
				
				SelectObject( hdc, hOldFont );
				lpSurface->ReleaseDC(hdc);
			}

			if( m_si32Height > MAX_CHAT_SURFACE_HEIGHT || m_si32Width > MAX_CHAT_SURFACE_WIDTH )
			{
				m_bDrawStatus = false;
				return;
			}

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					vector< stEmoticonDataDraw >::iterator itr;

					itr = vEmoticonData.begin();

					while ( itr != vEmoticonData.end() )
					{
						// 이모티콘을 차례로 출력한다.
						GP.PutSpr( pEmoticonSpr, (*itr).siXpos, (*itr).siYpos, (*itr).siFontIndex );
						++itr;
					}
					vEmoticonData.clear();

					if(m_pTextBuffer)
					{
						g_graphics.EncodeXpr( m_pTextBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}

			SI32 siYNum = 0;
	
			

			//--------------------------------------------------------------------------
			//	서페이스에 테두리 이미지 그리는 부분


			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI32 siXNum = 0;
			
			if ( m_si32Width % DrawChatSpr->GetXSize() )
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize();
			}
			else
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
			}
			//if ( siXNum < 12 )	siXNum=12;			// 너무 작으면 꼬리가 어긋나므로
			
			if ( ( m_si32Height - TailSpr->GetYSize() ) % DrawChatSpr->GetYSize() == 0 )
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize();
			}
			else
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize() + 1;
			}
			
			
			for ( int v = 0; v < siYNum; ++v )
			{
				for ( int h = 0; h < siXNum; ++h )
				{
					if ( h == 0 || v == 0 || h == siXNum -1 || v == siYNum-1 )
					{
						if ( h == 0 && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, 0, 0, LEFTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == 0 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1  && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTCENTER_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 )
						{
  							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTCENTER_IMAGE_SPR, 0 );
						}
  						else if ( v == 0 && h != siXNum - 2 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERTOP_IMAGE_SPR, 0 );
						}
						else if ( v == siYNum - 1 && h != siXNum - 2 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERBOTTOM_IMAGE_SPR, 0 );
						}
					}
					else			// 배경 하얀색 이미지... ( 가운데 이미지이지... )
					{
						GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTER_IMAGE_SPR, 0 );
					}			
				}
			}	
			
			// 꼬리를 먼저 찍어줌. 그 위에 채팅 나머지 부분이 덮어 써버리면 좀 더 쉬우므로...
			//TailSpr->PutSprT( m_si32Width/2 - TailSpr->clHeader.siXsize/2, siYNum * 4 - 1, 0, 0 );
			// 꼬리 위치 수정 - TYS
 			GP.PutSpr(TailSpr, m_si32Width*2/3-10, siYNum * 4 - 3, 0, 0 );

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pImageBuffer)
					{
						g_graphics.EncodeXpr( m_pImageBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}			

			//--------------------------------------------------------------------------
		}
		break;
		
	case ADVERTISE_CHATTING:			// 광고
		{
			m_siChatMode = CHATMODE_HEAD;

			m_bContinueDraw = bContinueDraw;
			m_si32CurrentFrame = 0;
			m_bDrawStatus = true;

			TSpr *DrawChatSpr = pDrawChatSurface->GetDrawChatImage1(); 
			TSpr *TailSpr = pDrawChatSurface->GetDrawChatTailImage1();
			
			// 서페이스에 글자 그리는 부분


			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			vector<stEmoticonDataDraw>	vEmoticonData;
			stEmoticonDataDraw			EmoticonData;
			bool bFoundEmoticon = false;
			SI32 lineHeight = CHAT_FONT_HEIGHT_SIZE;
			TSpr* pEmoticonSpr = pclClient->GetGlobalSpr(GIMG_EMOTICON_SMALL);
			bool bMultiLine	= false;


			if( lpSurface->GetDC(&hdc) == DD_OK)
			{				
				hOldFont = (HFONT)SelectObject( hdc, hFont );

				SetBkMode( hdc, TRANSPARENT );


				SetTextColor(hdc, RGB( 131, 0, 0));	

				LONG curX = rect.left;
				LONG curY = rect.top;
				LONG maxX = rect.right;


				// 이모티콘 출력을 위한 루틴 NOTJAPAN NOTGLOBAL[예정없음]
				if ( ConstServiceArea_China == pclClient->siServiceArea )
				{
					size_t textSize = _tcslen( text );

					int byteCnt = 0;
					int LineCount = 0;

					for ( UI32 i=0; i<textSize ; ++i )
					{
						if( text[i] == '~' )
						{
							if( *(text+i+1) >= '0' && *(text+i+1) <= '9' )
							{
								for ( SI16 j=0 ; j < MAX_EMOTICON_NUMBER; ++j )


								{
									if ( _tcsncmp( text+i+1,
										((cltClient*)pclClient)->clEmoticonInfo.GetCode(j), 
										3) == 0 )
									{
										bFoundEmoticon = true;

										if ( curX + pEmoticonSpr->GetXSize() > maxX )
										{
											curX = 4;
											curY += lineHeight;
										}

										EmoticonData.Set( curX, curY-(pEmoticonSpr->GetYSize()- CHAT_FONT_HEIGHT_SIZE)/2, j );
										vEmoticonData.push_back( EmoticonData );
										i += 3;
										curX += pEmoticonSpr->GetXSize();
										lineHeight = pEmoticonSpr->GetYSize();

										break;
									}
								}
							}
						}

						if ( bFoundEmoticon )
						{
							bFoundEmoticon = false;
							continue;
						}

						byteCnt = IsDBCSLeadByte(text[i]) ? 2 : 1;

						SIZE sz;
						GetTextExtentPoint32( hdc, text+i, byteCnt, &sz );
						if ( curX + sz.cx > maxX )
						{
							curX = 4;
							curY += lineHeight;

							lineHeight = CHAT_FONT_HEIGHT_SIZE;

							bMultiLine = true;
						}

						TextOut( hdc, curX, curY, text+i, byteCnt );
						curX += sz.cx;

						i += byteCnt - 1;
					}
					m_si32Height = curY + CHAT_FONT_HEIGHT_SIZE;
				}
				else
				{
					if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
					{
						m_si32Height = DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );
						DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /* | DT_CALCRECT */ );
					}
					else
					{
						m_si32Height = DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /*| DT_CALCRECT */ );
						DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT  );
					}
				}


				m_si32Height += 8 + 14;		// 위(4), 아래(4) 테두리, 꼬리(14)

				if ( CHAT_FONT_HEIGHT_SIZE > m_si32Height )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else if ( m_si32Height > MAX_CHAT_SURFACE_HEIGHT )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else
				{
					// 이모티콘 출력을 위한 루틴 NOTJAPAN NOTGLOBAL[예정없음]
					if ( ConstServiceArea_China == pclClient->siServiceArea)
					{
						if ( bMultiLine )
						{
							m_si32Width = MAX_CHAT_SURFACE_WIDTH;
						}
						else
						{
							m_si32Width = curX + 4;
						}
					}
					else
					{
						m_si32Width = rect.right - rect.left + 1 + 8;
					}

					if ( m_si32Width > MAX_CHAT_SURFACE_WIDTH )
					{
						m_si32Width = MAX_CHAT_SURFACE_WIDTH;
					}

					SI32 Module = m_si32Width % 4;
					if ( Module !=  0 )
					{
						m_si32Width += 4 - m_si32Width % 4;
					}
				}
				
				SelectObject( hdc, hOldFont );
				lpSurface->ReleaseDC(hdc);
			}

			if( m_si32Height > MAX_CHAT_SURFACE_HEIGHT || m_si32Width > MAX_CHAT_SURFACE_WIDTH )
			{
				m_bDrawStatus = false;
				return;
			}

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					vector< stEmoticonDataDraw >::iterator itr;

					itr = vEmoticonData.begin();

					while ( itr != vEmoticonData.end() )
					{
						// 이모티콘을 차례로 출력한다.
						GP.PutSpr( pEmoticonSpr, (*itr).siXpos, (*itr).siYpos, (*itr).siFontIndex );
						++itr;
					}
					vEmoticonData.clear();

					if(m_pTextBuffer)
					{
						g_graphics.EncodeXpr( m_pTextBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}

			SI32 siYNum = 0;
			
			//--------------------------------------------------------------------------
			//	서페이스에 테두리 이미지 그리는 부분


			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI32 siXNum = 0;
			
			if ( m_si32Width % DrawChatSpr->GetXSize() )
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize();
			}
			else
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
			}	
			
			if ( ( m_si32Height - TailSpr->GetYSize() ) % DrawChatSpr->GetYSize() == 0 )
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize();
			}
			else
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize() + 1;
			}
			
			
			for ( int v = 0; v < siYNum; ++v )
			{
				for ( int h = 0; h < siXNum; ++h )
				{
					if ( h == 0 || v == 0 || h == siXNum -1 || v == siYNum-1 )
					{
						if ( h == 0 && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, 0, 0, LEFTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == 0 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1  && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTCENTER_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTCENTER_IMAGE_SPR, 0 );
						}
						else if ( v == 0 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERTOP_IMAGE_SPR, 0 );
						}
						else if ( v == siYNum - 1 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERBOTTOM_IMAGE_SPR, 0 );
						}
					}
					else			// 배경 하얀색 이미지... ( 가운데 이미지이지... )
					{
						GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTER_IMAGE_SPR, 0 );
					}			
				}
			}	
			// 꼬리를 먼저 찍어줌. 그 위에 채팅 나머지 부분이 덮어 써버리면 좀 더 쉬우므로...
			//TailSpr->PutSprT( m_si32Width/2 - TailSpr->GetXSize()/2, siYNum * 4 - 1, 0, 0 );
			GP.PutSpr(TailSpr, m_si32Width*2/3, siYNum * 4 - 3, 0, 0 );

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pImageBuffer)
					{
						g_graphics.EncodeXpr( m_pImageBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}			

		}
		break;
	case PRODUCTION_CHATTING:			// 유저가 하는 말이 아닌, 유저가 하는 말( 제조, 실패 등등의 메시지를 보여 줄때 )
		{
			m_siChatMode = CHATMODE_SYSTEM;

			m_bContinueDraw = bContinueDraw;
			m_si32CurrentFrame = 0;
			m_bDrawStatus = true;

			TSpr *DrawChatSpr = pDrawChatSurface->GetDrawChatImage2(); 
			TSpr *TailSpr = pDrawChatSurface->GetDrawChatTailImage2();
	

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if( lpSurface->GetDC(&hdc) == DD_OK)
			{				
				hOldFont = (HFONT)SelectObject( hdc, hFont );

				SetBkMode( hdc, TRANSPARENT );

				SetTextColor(hdc, RGB( 0, 59, 169));	

				if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
				{
					m_si32Height = DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );
					DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /* | DT_CALCRECT */ );
				}
				else
				{
					m_si32Height = DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /*| DT_CALCRECT */ );
					DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT  );
				}

				m_si32Height += 8 + 14;		// 위(4), 아래(4) 테두리, 꼬리(14)

				if ( CHAT_FONT_HEIGHT_SIZE > m_si32Height )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else if ( m_si32Height > MAX_CHAT_SURFACE_HEIGHT )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else
				{
					m_si32Width = rect.right - rect.left + 1 + 8;

					if ( m_si32Width > MAX_CHAT_SURFACE_WIDTH )
					{
						m_si32Width = MAX_CHAT_SURFACE_WIDTH;
					}

					SI32 Module = m_si32Width % 4;
					if ( Module !=  0 )
					{
						m_si32Width += 4 - m_si32Width % 4;
					}
				}
				
				SelectObject( hdc, hOldFont );
				lpSurface->ReleaseDC(hdc);
			}

			if( m_si32Height > MAX_CHAT_SURFACE_HEIGHT || m_si32Width > MAX_CHAT_SURFACE_WIDTH )
			{
				m_bDrawStatus = false;
				return;
			}

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pTextBuffer)
					{
						g_graphics.EncodeXpr( m_pTextBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}



			SI32 siYNum = 0;
			
			//--------------------------------------------------------------------------
			//	서페이스에 테두리 이미지 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI32 siXNum = 0;
			
			if ( m_si32Width % DrawChatSpr->GetXSize() )
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize();
			}
			else
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
			}	
			
			if ( ( m_si32Height - TailSpr->GetYSize() ) % DrawChatSpr->GetYSize() == 0 )
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize();
			}
			else
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize() + 1;
			}
			
			
			for ( int v = 0; v < siYNum; ++v )
			{
				for ( int h = 0; h < siXNum; ++h )
				{
					if ( h == 0 || v == 0 || h == siXNum -1 || v == siYNum-1 )
					{
						if ( h == 0 && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, 0, 0, LEFTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == 0 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1  && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTCENTER_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTCENTER_IMAGE_SPR, 0 );
						}
						else if ( v == 0 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERTOP_IMAGE_SPR, 0 );
						}
						else if ( v == siYNum - 1 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERBOTTOM_IMAGE_SPR, 0 );
						}
					}
					else			// 배경 하얀색 이미지... ( 가운데 이미지이지... )
					{
						GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTER_IMAGE_SPR, 0 );
					}			
				}
			}	
			// 꼬리를 먼저 찍어줌. 그 위에 채팅 나머지 부분이 덮어 써버리면 좀 더 쉬우므로...
			//TailSpr->PutSprT( m_si32Width/2 - TailSpr->clHeader.siXsize/2, siYNum * 4 - 1, 0, 0 );
			GP.PutSpr(TailSpr, m_si32Width*2/3, siYNum * 4 - 3, 0, 0 );

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pImageBuffer)
					{
						g_graphics.EncodeXpr( m_pImageBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}			

		}
		break;
	case NPC_CHATTING:			// NPC가 하는 말
		{
			m_siChatMode = CHATMODE_SYSTEM;

			m_bContinueDraw = bContinueDraw;
			m_si32CurrentFrame = 0;
			m_bDrawStatus = true;

			TSpr *DrawChatSpr = pDrawChatSurface->GetDrawChatImage3(); 
			TSpr *TailSpr = pDrawChatSurface->GetDrawChatTailImage3();
			
			// 서페이스에 글자 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if( lpSurface->GetDC(&hdc) == DD_OK)
			{				
				hOldFont = (HFONT)SelectObject( hdc, hFont );

				SetBkMode( hdc, TRANSPARENT );


				SetTextColor(hdc, RGB( 124, 52, 31));	

				if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
				{
					m_si32Height = DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );
					DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /* | DT_CALCRECT */ );
				}
				else
				{
					m_si32Height = DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /*| DT_CALCRECT */ );
					DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT  );
				}

				m_si32Height += 8 + 14;		// 위(4), 아래(4) 테두리, 꼬리(14)

				if ( CHAT_FONT_HEIGHT_SIZE > m_si32Height )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else if ( m_si32Height > MAX_CHAT_SURFACE_HEIGHT )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else
				{

					m_si32Width = rect.right - rect.left + 1 + 8;

					if ( m_si32Width > MAX_CHAT_SURFACE_WIDTH )
					{
						m_si32Width = MAX_CHAT_SURFACE_WIDTH;
					}

					SI32 Module = m_si32Width % 4;
					if ( Module !=  0 )
					{
						m_si32Width += 4 - m_si32Width % 4;
					}
				}
				
				SelectObject( hdc, hOldFont );
				lpSurface->ReleaseDC(hdc);
			}

			if( m_si32Height > MAX_CHAT_SURFACE_HEIGHT || m_si32Width > MAX_CHAT_SURFACE_WIDTH )
			{
				m_bDrawStatus = false;
				return;
			}

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pTextBuffer)
					{
						g_graphics.EncodeXpr( m_pTextBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}

			SI32 siYNum = 0;
			
			//--------------------------------------------------------------------------
			//	서페이스에 테두리 이미지 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI32 siXNum = 0;
			
			if ( m_si32Width % DrawChatSpr->GetXSize() )
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize();
			}
			else
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
			}	
			
			if ( ( m_si32Height - TailSpr->GetYSize() ) % DrawChatSpr->GetYSize() == 0 )
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize();
			}
			else
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize() + 1;
			}
			
			
			for ( int v = 0; v < siYNum; ++v )
			{
				for ( int h = 0; h < siXNum; ++h )
				{
					if ( h == 0 || v == 0 || h == siXNum -1 || v == siYNum-1 )
					{
						if ( h == 0 && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, 0, 0, LEFTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == 0 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1  && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTCENTER_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTCENTER_IMAGE_SPR, 0 );
						}
						else if ( v == 0 && h != siXNum - 2 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERTOP_IMAGE_SPR, 0 );
						}
						else if ( v == siYNum - 1 && h != siXNum - 2 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERBOTTOM_IMAGE_SPR, 0 );
						}
					}
					else			// 배경 하얀색 이미지... ( 가운데 이미지이지... )
					{
						GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTER_IMAGE_SPR, 0 );
					}			
				}
			}	
			// 꼬리를 먼저 찍어줌. 그 위에 채팅 나머지 부분이 덮어 써버리면 좀 더 쉬우므로...
			//TailSpr->PutSprT( m_si32Width/2 - TailSpr->clHeader.siXsize/2, siYNum * 4 - 1, 0, 0 );
			GP.PutSpr(TailSpr, m_si32Width*2/3, siYNum * 4 - 3, 0, 0 );

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pImageBuffer)
					{
						g_graphics.EncodeXpr( m_pImageBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}			

		}
		break;
	case PERSONALSHOP_BANNER:			// 광고
		{
			m_siChatMode = CHATMODE_SYSTEM;
			
			m_bContinueDraw = bContinueDraw;
			m_si32CurrentFrame = 0;
			m_bDrawStatus = true;

			TSpr *DrawChatSpr = pDrawChatSurface->GetDrawChatImage4(); 
			TSpr *TailSpr = pDrawChatSurface->GetDrawChatTailImage4();
			
			// 서페이스에 글자 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if( lpSurface->GetDC(&hdc) == DD_OK)
			{				
				hOldFont = (HFONT)SelectObject( hdc, hFont );

				SetBkMode( hdc, TRANSPARENT );

				if(siShopMode == PSHOP_MODE_BUY)
					SetTextColor(hdc, RGB( 239, 220, 138));	
				else if(siShopMode == PSHOP_MODE_SELL)
					SetTextColor(hdc, RGB( 244, 244, 244));
				else if(siShopMode == PSHOP_MODE_MAKEITEM)
					SetTextColor(hdc, RGB( 180, 244, 210));	
				else if(siShopMode == PSHOP_MODE_MAKEITEM_WANT)
					SetTextColor(hdc, RGB( 255, 210, 0));	// 색깔 수정 

				if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
				{
					m_si32Height = DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );
					DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /* | DT_CALCRECT */ );
				}
				else
				{
					m_si32Height = DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /*| DT_CALCRECT */ );
					DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT  );
				}


				m_si32Height += 8 + 14;		// 위(4), 아래(4) 테두리, 꼬리(14)

				if ( CHAT_FONT_HEIGHT_SIZE > m_si32Height )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else if ( m_si32Height > MAX_CHAT_SURFACE_HEIGHT )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else
				{
					m_si32Width = rect.right - rect.left + 1 + 8;

					if ( m_si32Width > MAX_CHAT_SURFACE_WIDTH )
					{
						m_si32Width = MAX_CHAT_SURFACE_WIDTH;
					}

					SI32 Module = m_si32Width % 4;
					if ( Module !=  0 )
					{
						m_si32Width += 4 - m_si32Width % 4;
					}
				}
				
				SelectObject( hdc, hOldFont );
				lpSurface->ReleaseDC(hdc);
			}

			if( m_si32Height > MAX_CHAT_SURFACE_HEIGHT || m_si32Width > MAX_CHAT_SURFACE_WIDTH )
			{
				m_bDrawStatus = false;
				return;
			}

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pTextBuffer)
					{
						g_graphics.EncodeXpr( m_pTextBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}

			SI32 siYNum = 0;
			
			//--------------------------------------------------------------------------
			//	서페이스에 테두리 이미지 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI32 siXNum = 0;
			
			if ( m_si32Width % DrawChatSpr->GetXSize() )
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize();
			}
			else
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
			}	
			
			if ( ( m_si32Height - TailSpr->GetYSize() ) % DrawChatSpr->GetYSize() == 0 )
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize();
			}
			else
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize() + 1;
			}
			
			
			for ( int v = 0; v < siYNum; ++v )
			{
				for ( int h = 0; h < siXNum; ++h )
				{
					if ( h == 0 || v == 0 || h == siXNum -1 || v == siYNum-1 )
					{
						if ( h == 0 && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, 0, 0, LEFTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == 0 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1  && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTCENTER_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTCENTER_IMAGE_SPR, 0 );
						}
						else if ( v == 0 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERTOP_IMAGE_SPR, 0 );
						}
						else if ( v == siYNum - 1 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERBOTTOM_IMAGE_SPR, 0 );
						}
					}
					else			// 배경 하얀색 이미지... ( 가운데 이미지이지... )
					{
						GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTER_IMAGE_SPR, 0 );
					}			
				}
			}	
			// 꼬리를 먼저 찍어줌. 그 위에 채팅 나머지 부분이 덮어 써버리면 좀 더 쉬우므로...
			GP.PutSpr(TailSpr, m_si32Width*2/3, siYNum * 4 - 3, 0, 0 );

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pImageBuffer)
					{
						g_graphics.EncodeXpr( m_pImageBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}			

		}
		break;
	case PERSONALSHOP_BANNER_PREMIUM:
	case ELECTIONSTUMP:
		{
			m_siChatMode = CHATMODE_SYSTEM;

			m_bContinueDraw = bContinueDraw;
			m_si32CurrentFrame = 0;
			m_bDrawStatus = true;

			TSpr *DrawChatSpr = pDrawChatSurface->GetDrawChatImage5(); 
			TSpr *TailSpr = pDrawChatSurface->GetDrawChatTailImage5();

			// 서페이스에 글자 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			if( lpSurface->GetDC(&hdc) == DD_OK)
			{				
				hOldFont = (HFONT)SelectObject( hdc, hFont );

				SetBkMode( hdc, TRANSPARENT );

				if(siShopMode == PSHOP_MODE_BUY)
					SetTextColor(hdc, RGB( 239, 220, 138));	
				else if(siShopMode == PSHOP_MODE_SELL)
					SetTextColor(hdc, RGB( 244, 244, 244));
				else if(siShopMode == PSHOP_MODE_MAKEITEM)
					SetTextColor(hdc, RGB( 180, 244, 210));	
				else if(siShopMode == PSHOP_MODE_MAKEITEM)
					SetTextColor(hdc, RGB( 255, 110, 0));

				if( pclClient->IsCountrySwitch( Switch_ChatBalloonLine ))
				{
					m_si32Height = DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT );
					DrawText( hdc, szConvertText, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /* | DT_CALCRECT */ );
				}
				else
				{
					m_si32Height = DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK /*| DT_CALCRECT */ );
					DrawText( hdc, text, -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT  );
				}


				m_si32Height += 8 + 14;		// 위(4), 아래(4) 테두리, 꼬리(14)

				if ( CHAT_FONT_HEIGHT_SIZE > m_si32Height )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else if ( m_si32Height > MAX_CHAT_SURFACE_HEIGHT )
				{
					SelectObject( hdc, hOldFont );
					lpSurface->ReleaseDC(hdc);
					m_bDrawStatus = false;

					return;
				}
				else
				{
					m_si32Width = rect.right - rect.left + 1 + 8;


					// 개인 상점 타이틀이 걸려있다면 좀더 큰쪽으로 크기를 잡는다.

					if( shopTitle )
					{
						DrawText( hdc, shopTitle, -1, &rect, DT_LEFT | DT_TOP | DT_CALCRECT );

						if( m_si32Width < rect.right - rect.left + 1 + 8 )
						{
							m_si32Width = rect.right - rect.left + 1 + 8;
						}
					}


					if ( m_si32Width > MAX_CHAT_SURFACE_WIDTH )
					{
						m_si32Width = MAX_CHAT_SURFACE_WIDTH;
					}

					SI32 Module = m_si32Width % 4;
					if ( Module !=  0 )
					{
						m_si32Width += 4 - m_si32Width % 4;
					}
				}

				SelectObject( hdc, hOldFont );
				lpSurface->ReleaseDC(hdc);
			}

			if( m_si32Height > MAX_CHAT_SURFACE_HEIGHT || m_si32Width > MAX_CHAT_SURFACE_WIDTH )
			{
				m_bDrawStatus = false;
				return;
			}

			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pTextBuffer)
					{
						g_graphics.EncodeXpr( m_pTextBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}

			SI32 siYNum = 0;

			//--------------------------------------------------------------------------
			//	서페이스에 테두리 이미지 그리는 부분

			g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

			SI32 siXNum = 0;

			if ( m_si32Width % DrawChatSpr->GetXSize() )
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize();
			}
			else
			{
				siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
			}	

			if ( ( m_si32Height - TailSpr->GetYSize() ) % DrawChatSpr->GetYSize() == 0 )
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize();
			}
			else
			{
				siYNum = ( m_si32Height - TailSpr->GetYSize() ) / DrawChatSpr->GetYSize() + 1;
			}


			for ( int v = 0; v < siYNum; ++v )
			{
				for ( int h = 0; h < siXNum; ++h )
				{
					if ( h == 0 || v == 0 || h == siXNum -1 || v == siYNum-1 )
					{
						if ( h == 0 && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, 0, 0, LEFTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == 0 && v == siYNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTBOTTOM_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1  && v == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTTOP_IMAGE_SPR, 0 );
						}
						else if ( h == 0 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, LEFTCENTER_IMAGE_SPR, 0 );
						}
						else if ( h == siXNum - 1 )
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h - 4, DrawChatSpr->GetYSize() * v, RIGHTCENTER_IMAGE_SPR, 0 );
						}
						else if ( v == 0 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERTOP_IMAGE_SPR, 0 );
						}
						else if ( v == siYNum - 1 && h != siXNum - 2)
						{
							GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTERBOTTOM_IMAGE_SPR, 0 );
						}
					}
					else			// 배경 하얀색 이미지... ( 가운데 이미지이지... )
					{
						GP.PutSpr(DrawChatSpr, DrawChatSpr->GetXSize() * h, DrawChatSpr->GetYSize() * v, CENTER_IMAGE_SPR, 0 );
					}			
				}
			}	
			// 꼬리를 먼저 찍어줌. 그 위에 채팅 나머지 부분이 덮어 써버리면 좀 더 쉬우므로...
			GP.PutSpr(TailSpr, m_si32Width*2/3, siYNum * 4 - 3, 0, 0 );

			


			{
				int size;
				if ( GP.LockSurface( lpSurface ) == TRUE )
				{
					if(m_pImageBuffer)
					{
						g_graphics.EncodeXpr( m_pImageBuffer, 
							MAX_CHAT_SURFACE_WIDTH + 10, 
							MAX_CHAT_SURFACE_HEIGHT + 10,
							RGB16_TRANS,
							&size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}

			////////////////////////////////////////////////////
			//	상점 타이틀을 체크해서 있다면 그려준다.
			////////////////////////////////////////////////////


			if(shopTitle != NULL)
			{
 				if( m_pTitleBackBuffer == NULL ) m_pTitleBackBuffer = new RGB16[ ( MAX_SHOPBANNER_TITLE_WIDTH + 10 ) * ( MAX_SHOPBANNER_TITLE_HEIGHT + 10 ) ];
				if( m_pTitleBuffer == NULL ) m_pTitleBuffer = new RGB16[ ( MAX_SHOPBANNER_TITLE_WIDTH + 10 ) * ( MAX_SHOPBANNER_TITLE_HEIGHT + 10 ) ];

				TSpr * pShopTitleBackSpr = pDrawChatSurface->GetDrawShopTitleImage();
				g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

				if( lpSurface->GetDC(&hdc) == DD_OK)
				{	
					   
  					hOldFont = (HFONT)SelectObject( hdc, hFont );

					SetBkMode( hdc, TRANSPARENT );

 					SetTextColor(hdc, RGB( 0, 0, 0));						

					
  					m_siTitleHeight = DrawText( hdc, shopTitle, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_CALCRECT);
					
					rect.top -=2;
					rect.bottom -=2;
					
  					if( rect.top < 0) rect.top = 0;
 					if(rect.bottom > m_siTitleHeight+2)rect.bottom = m_siTitleHeight+2;
					
					rect.left = 0;
					rect.right = m_si32Width;
					
					DrawText( hdc, shopTitle, -1, &rect, DT_CENTER | DT_VCENTER  | DT_SINGLELINE);				
											

					if ( CHAT_FONT_HEIGHT_SIZE > m_siTitleHeight )
					{
						SelectObject( hdc, hOldFont );
						lpSurface->ReleaseDC(hdc);
						m_bDrawStatus = false;

						return;
					}
					else if ( m_siTitleHeight > MAX_SHOPBANNER_TITLE_HEIGHT )
					{
						SelectObject( hdc, hOldFont );
						lpSurface->ReleaseDC(hdc);
						m_bDrawStatus = false;

						return;
					}
					/*

					else
					{
						m_si32Width = rect.right - rect.left + 1 + 8;

						if ( m_si32Width > MAX_SHOPBANNER_TITLE_WIDTH )
						{
							m_si32Width = MAX_SHOPBANNER_TITLE_WIDTH;
						}

						SI32 Module = m_si32Width % 4;
						if ( Module !=  0 )
						{
							m_siTitleWidth += 4 - m_siTitleWidth % 4;
						}
					}*/

					SelectObject( hdc, hOldFont );

					lpSurface->ReleaseDC(hdc);
				}
				

				if( m_siTitleHeight > MAX_SHOPBANNER_TITLE_HEIGHT || m_si32Width > MAX_SHOPBANNER_TITLE_WIDTH )
				{
					m_bDrawStatus = false;
					return;
				}

				{
					int size;
					if ( GP.LockSurface( lpSurface ) == TRUE )
					{
						if(m_pTitleBuffer)
						{
							g_graphics.EncodeXpr( m_pTitleBuffer, 
								MAX_SHOPBANNER_TITLE_WIDTH + 10, 
								MAX_SHOPBANNER_TITLE_HEIGHT + 10,
								RGB16_TRANS,
								&size );
						}
						GP.UnlockSurface( lpSurface );
					}
				}

				//--------------------------------------------------------------------------
				//	서페이스에 테두리 이미지 그리는 부분

				g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

				SI32 siXNum = 0;

				if ( m_si32Width % DrawChatSpr->GetXSize() )
				{
					siXNum = m_si32Width / DrawChatSpr->GetXSize();
				}
				else
				{
					siXNum = m_si32Width / DrawChatSpr->GetXSize() + 1;
				}	
				

				for(SI16	i = 0; i < siXNum-1; i++ )
				{
                    if( i == 0 )
					{
						GP.PutSpr(pShopTitleBackSpr, 0, 0, 0, 0 );
					}
					else if( i == siXNum - 2)
					{
						GP.PutSpr(pShopTitleBackSpr, DrawChatSpr->GetXSize() * i, 0, 2, 0 );
					}
					else
					{
						GP.PutSpr(pShopTitleBackSpr, DrawChatSpr->GetXSize() * i, 0, 1, 0 );
					}

				}


				{
					int size;
					if ( GP.LockSurface( lpSurface ) == TRUE )
					{
						if(m_pTitleBackBuffer)
						{
							g_graphics.EncodeXpr( m_pTitleBackBuffer, 
								MAX_SHOPBANNER_TITLE_WIDTH + 10, 
								MAX_SHOPBANNER_TITLE_HEIGHT + 10,
								RGB16_TRANS,
								&size );
						}
						GP.UnlockSurface( lpSurface );
					}
				}

				m_bDrawTitle = true;

			}

		}
		break;

	}
}

void CDrawChat::Action()
{
	if ( ! m_bDrawStatus )
	{
		return;
	}

	if ( m_bContinueDraw )
	{
		return;
	}

	++m_si32CurrentFrame;

	if ( m_si32CurrentFrame > CHAT_DELAY_TIME )
	{
		if( m_siFadeMode != 2 ) {
			m_siFadeMode = 2;
			m_siFadeStep = 0;
		}
	}

	return;
}

void CDrawChat::Draw( SI32 si32CenterXPos, SI32 si32CenterYPos, RECT *pScreenRect )
{
	if ( ! m_bDrawStatus )
	{
		return;
	}

	int alpha = 12;

	// 대화창 처음 열릴 때
	if( m_siFadeMode == 1 ) {
		m_siFadeStep++;

		alpha = (int)(m_siFadeStep * 1.8);

		if( alpha > 12 ) {
			alpha = 12;
			m_siFadeMode = 0;
		}


	} 
	// 대화창 닫힐 때 
	else if ( m_siFadeMode == 2 ) {
		m_siFadeStep++;

		alpha = (int)(12 - ( m_siFadeStep * 0.9 ));

		if( alpha < 0 ) {
			alpha = 0;
			m_siFadeMode = 0;
			StopDraw();
		}
	}


	if ( (m_siDrawChatKind == PERSONALSHOP_BANNER_PREMIUM)
		|| (m_siDrawChatKind == ELECTIONSTUMP)
		) 
	{
		
	
		// 타이틀 배경 찍어준다. 타이틀이 적절하다면.

		if(m_pTitleBackBuffer && m_bDrawTitle)
		{
			/*
			GP.PutXpr16A(si32CenterXPos - m_si32Width/2, 
				si32CenterYPos - m_si32Height - m_siTitleHeight, 
				MAX_CHAT_SURFACE_WIDTH + 10,
				MAX_CHAT_SURFACE_HEIGHT + 10, 
				m_pTitleBackBuffer,
				alpha,
				RGB16_TRANS
				);*/

			GP.PutXpr16(si32CenterXPos - m_si32Width/2, 
				si32CenterYPos - m_si32Height - m_siTitleHeight - 5, 
				MAX_SHOPBANNER_TITLE_WIDTH + 10,
				MAX_SHOPBANNER_TITLE_HEIGHT + 10, 
				m_pTitleBackBuffer,				
				RGB16_TRANS
				);
		}
		
		GP.PutXpr16A(si32CenterXPos - m_si32Width/2, 
		             si32CenterYPos - m_si32Height, 
					 MAX_CHAT_SURFACE_WIDTH + 10,
					 MAX_CHAT_SURFACE_HEIGHT + 10, 
					 m_pImageBuffer,
					 alpha,
					 RGB16_TRANS
					 );
	} else {
		GP.PutXpr16A(si32CenterXPos - m_si32Width/2, 
			         si32CenterYPos - m_si32Height, 
					 MAX_CHAT_SURFACE_WIDTH + 10, 
					 MAX_CHAT_SURFACE_HEIGHT + 10, 
					 m_pImageBuffer,
					 alpha,
					 RGB16_TRANS
					 );
	}

	if( alpha > 6 ) {

		GP.PutXpr16(si32CenterXPos - m_si32Width/2, 
					si32CenterYPos - m_si32Height, 
					MAX_CHAT_SURFACE_WIDTH + 10, 
					MAX_CHAT_SURFACE_HEIGHT + 10, 
					m_pTextBuffer,
					RGB16_TRANS
					);
		
		if(m_pTitleBuffer && m_bDrawTitle)
		{
			GP.PutXpr16(si32CenterXPos - m_si32Width/2, 
				si32CenterYPos - m_si32Height - m_siTitleHeight  - 5, 
				MAX_SHOPBANNER_TITLE_WIDTH + 10,
				MAX_SHOPBANNER_TITLE_HEIGHT + 10, 
				m_pTitleBuffer,				
				RGB16_TRANS
				);
		}



		// 타이틀 찍어준다. 타이틀이 적절하다면.


	}

	return;
}

void CDrawChat::DivideChatData()
{
	return;
}

void CDrawChat::StopDraw()
{
	m_si32CurrentFrame = 0;
	m_bDrawStatus = false;
	m_bContinueDraw = false;

	m_siChatMode = 0;

	if( IsPC( m_siCharUnique ) )
	{
		//cyj 채팅창 없어질때 머리말광고 있는지 요청
		cltGameMsgRequest_HeadChat clinfo(pclClient->pclCM->CR[1]->GetCharUnique());
		cltMsg clMsg(GAMEMSG_REQUEST_HEADCHAT, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	}

	return;
}

SI16 CDrawChat::GetChatMode()
{
	return m_siChatMode;
}
