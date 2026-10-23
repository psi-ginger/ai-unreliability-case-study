#include "Edit.h"
#include "ToolTip.h"


#include "../InterfaceMgr.h"

#include "../ControllerMgr.h"
#include "../ControllerSurfaceObj.h"

#include "../InterfaceImgMgr/InterfaceImgMgr.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

extern HINSTANCE g_hIMEDll;
extern HINSTANCE g_hVersionIMEDll;


#include <zmouse.h>

#include <Client.h>
extern cltCommonLogic *pclClient;

extern bool g_bCreate;


#include <dimm.h>

extern	INPUTCONTEXT* (WINAPI * _ImmLockIMC)( HIMC );
extern  BOOL (WINAPI * _ImmUnlockIMC)( HIMC );
extern  LPVOID (WINAPI * _ImmLockIMCC)( HIMCC );
extern  BOOL (WINAPI * _ImmUnlockIMCC)( HIMCC );
extern  HIMC (WINAPI * _ImmGetContext)( HWND );
extern  BOOL (WINAPI * _ImmReleaseContext)( HWND, HIMC );
extern  UINT (WINAPI * _ImmGetIMEFileNameA)( HKL, LPSTR, UINT );

BOOL (APIENTRY * _VerQueryValueA)( const LPVOID, LPSTR, LPVOID *, PUINT );
BOOL (APIENTRY * _GetFileVersionInfoA)( LPSTR, DWORD, DWORD, LPVOID );
DWORD (APIENTRY * _GetFileVersionInfoSizeA)( LPSTR, LPDWORD );

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)

bool IsDelimiter( TCHAR SrcChar )
{
	static TCHAR deli[] = TEXT(" \t\r\n\"\'\\.,<>:;/()[]{}~!@#$%^&*-+?=");
	return ( _tcschr(deli,SrcChar) || SrcChar==NULL );
}

CEdit::CEdit( CControllerMgr *pParent ) : CControllerObj( pParent )
{
	m_strText = NULL;

	m_ImgEncBuffer = NULL;
	m_ImgCompEncBuffer = NULL;
	m_ImgReadingEncBuffer = NULL;

	m_pCandidateList = NULL;
	m_bCandidateOpen = false;
	m_pCandidateDataList = NULL;
	m_dwCandidateDataListCount = 0;
	m_siCandidateDataStartIndex = 0;

	ResetCompositionString();
	ResetReadingString();
	ResetCandiateWindow();

	m_bMultiline = false;
	m_bReadOnly = true;

	m_siLineHeight = EDIT_LINEHEIGHT;

	m_siMaxEditTextLimitLength = 0;

	m_siCaretCounter = 0;

	m_bTabKeyPressNotify = false;

	m_bAutoScroll = false;
	m_bAutoScroll = false;

	m_siOffsetX = 0;
	m_siOffsetY = 0;

	m_siCurrentEditTextLength = 0;

	m_siAvailabeUseWidth = 0;
	m_siAvailabeUseHeight = 0;

	m_siCurrentCaretPos = 0;

	m_siInputMode = 0;
	m_siPasswordPixelLen = 0;
	m_siAsterikPixelLen = 0;

	m_bDBCS = false;

	m_siCaretXPos = 0;
	m_siCaretYPos = 0;

	m_siSelStartIndex = -1;
	m_siSelEndIndex = -1;
	m_bSelDrag = false;

	m_ptSelStart.x = 0;
	m_ptSelStart.y = 0;

	m_ptSelEnd.x = 0;
	m_ptSelEnd.y = 0;

	SetTextPrintOption( DT_LEFT | DT_TOP | DT_SINGLELINE );

	m_pScroll = new CScroll( pParent );

	m_strSelImageFileName = new TCHAR[ MAX_PATH ];
	StringCchCopy( m_strSelImageFileName, MAX_PATH, EDIT_SELIMAGE_FILENAME );
	m_siSelImageFontIndex = 3;
	m_bSelImageTileImg = true;
	m_siSelImageTileNum = 3;
	m_siSelImageAlphaValue = EDIT_SELIMAGE_ALPHA_VALUE;

	if ( ! g_bCreate )
	{
		g_bCreate = true;

		LoadDLL();
	}

	m_siCompAttrStart = 0;
	m_siCompAttrEnd = 0;
	m_siAttrLineStartPos = 0;

	m_hFont	= NULL;
	m_hOldFont = NULL;
	m_bShowScrollbar = TRUE;

	m_bUseToolTip = false;
	m_strToolTipMsg = NULL;
}

void CEdit::Clear()
{
	if( m_strText )
	{
		memset( m_strText, 0, m_siMaxEditTextLimitLength );
	}

	ResetCompositionString();
	ResetReadingString();
	ResetCandiateWindow();

	m_LineInfoList.clear();

	m_siPasswordPixelLen = 0;
	m_siAsterikPixelLen = 0;

	m_siCaretCounter = 0;

	m_siCurrentEditTextLength = 0;

	m_siCurrentCaretPos = 0;

	m_bDBCS = false;

	m_siCaretXPos = 0;
	m_siCaretYPos = 0;

	m_siSelStartIndex = -1;
	m_siSelEndIndex = -1;
	m_bSelDrag = false;

	m_pScroll->SetTrackRange( 0, 1 );
	m_pScroll->Show( false );

	SAFE_DELETE_A( m_ImgEncBuffer );

	return;
}

void CEdit::ResetCompositionString()
{
	memset( m_strCompStr, 0, sizeof( m_strCompStr ) );
	m_siCompStrLen = 0;

	SAFE_DELETE_A( m_ImgCompEncBuffer );

	m_siCompStrWidth = 0;
	m_siCompStrHeight = 0;

	m_siCompAttrStart = 0;
	m_siCompAttrEnd = 0;

	m_siAttrLineStartPos = 0;

	return;
}

void CEdit::ResetReadingString()
{
	m_bReadiingWindowOpen = false;

	memset( m_strReadingStr, 0, sizeof( m_strReadingStr ) );
	m_siReadingStrLen = 0;

	SAFE_DELETE_A( m_ImgReadingEncBuffer );

	m_siReadingStrWidth = 0;
	m_siReadingStrHeight = 0;
}

void CEdit::ResetCandiateWindow()
{
	SAFE_DELETE_A( m_pCandidateList );
	m_bCandidateOpen = false;
	SAFE_DELETE_A( m_pCandidateDataList );	
	m_dwCandidateDataListCount = 0;
	m_siCandidateDataStartIndex = 0;

	memset( m_strCandidateStr, 0, sizeof( m_strCandidateStr ) );
	m_siCandidateStrLen = 0;
}

void CEdit::ClearText()
{
	Clear();
	return;
}

CEdit::~CEdit()
{
	m_LineInfoList.clear();

	SAFE_DELETE_A( m_strText );
	SAFE_DELETE_A( m_ImgEncBuffer );
	SAFE_DELETE_A( m_ImgCompEncBuffer );
	SAFE_DELETE_A( m_ImgReadingEncBuffer );

	SAFE_DELETE( m_pScroll );

	SAFE_DELETE_A( m_pCandidateList );
	SAFE_DELETE_A( m_pCandidateDataList );

	//cyj m_strSelImageFileName 메모리 해제
	SAFE_DELETE_A( m_strSelImageFileName );

	if ( m_strToolTipMsg )
	{
		delete [] m_strToolTipMsg;
		m_strToolTipMsg = NULL;
	}

}

void CEdit::Create( SI16 ControllerID, TCHAR *Text,
				   TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
				   SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
				   SI16 SurfaceWidth, SI16 SurfaceHeight, 
				   bool Multiline, bool ReadOnly,
				   SI16 LineHeight,
				   SI32 MaxEditTextLimitLength,
				   SI16 InputMode,
				   SI16 OffsetX, SI16 OffsetY,
				   bool AutoScroll
				   )
{
	cltClient *pclclient = (cltClient *)pclClient;

	m_bMultiline = Multiline;	
	m_bReadOnly = ReadOnly;

	m_siMaxEditTextLimitLength = MaxEditTextLimitLength;
	m_strText = new TCHAR[ MaxEditTextLimitLength ];
	memset( m_strText, 0, MaxEditTextLimitLength );

	m_siLineHeight = LineHeight;


	m_bAutoScroll = AutoScroll;

	m_siOffsetX = OffsetX;
	m_siOffsetY = OffsetY;

	m_siInputMode = InputMode;

	if ( m_bMultiline )
	{
		m_siAvailabeUseWidth = Width - OffsetX - SCROLL_WIDTH;
	}
	else
	{
		m_siAvailabeUseWidth = Width - OffsetX;
	}

	m_siAvailabeUseHeight = Height - OffsetY;

	m_siCaretXPos = 0;
	m_siCaretYPos = 0;

	CControllerObj::Create( ControllerID, CONTROLLER_KIND_EDIT, FileName, FontIndex, BKTileImg, BKTileImgNum, XPos, YPos, Width, Height, SurfaceWidth, SurfaceHeight );

	m_pScroll->Create( COMMON_SCROLL, this, NULL, 0, XPos + Width - SCROLL_WIDTH - 1, YPos, SCROLL_WIDTH, Height, SCROLL_WIDTH, Height, LineHeight );
	m_pScroll->Show( false );

	if ( Text )
	{
		SetText( m_strText );
	}

	return;
}

void CEdit::SetImeMode( HWND hWindow, bool AlphaNumeric )
{
	HIMC hImc;
	hImc = ImmGetContext( hWindow );

	DWORD dwConv, dwSent, dwTemp;
	if ( hImc && ImmGetConversionStatus( hImc, &dwConv, &dwSent ) )
	{
		if ( AlphaNumeric )
		{
			if ( ! ( dwConv & IME_CMODE_NATIVE ) )
				return;

			dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
			dwConv = dwTemp;

			ImmSetConversionStatus( hImc, dwConv, dwSent );
		}
		else
		{
			if ( (dwConv & IME_CMODE_NATIVE) )
				return;

			dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
			//dwTemp |= IME_CMODE_NATIVE;
			dwTemp = dwTemp | IME_CMODE_NATIVE | IME_CMODE_SYMBOL;
			dwConv = dwTemp;

			ImmSetConversionStatus( hImc, dwConv, dwSent );
		}
	}

	if ( hImc )
	{
		ImmReleaseContext( hWindow,hImc );
	}
}

bool CEdit::IsAlphaNumericMode( HWND hWindow )
{
	bool bRet = false;

	HIMC hIMC = NULL;  

	if (hIMC = ImmGetContext( hWindow ) )
	{
		DWORD fdwConversion, fdwSentence;  

		if( ImmGetConversionStatus(hIMC, &fdwConversion, &fdwSentence) )
		{
			bRet = ( fdwConversion & IME_CMODE_NATIVE ? false : true );
			ImmReleaseContext( hWindow, hIMC );
		}
	}  

	return bRet;
}

bool CEdit::HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
	{
		return false;
	}

	bool bShift, bControl;

	bShift = ((GetKeyState(VK_SHIFT) & 0x8000) != 0); 
	bControl = ((GetKeyState(VK_CONTROL) & 0x8000) != 0);


	switch( uMsg )
	{
	case WM_KEYUP:
		{
			switch( wParam )
			{
			case VK_RETURN:
				return true;
			}
		}
		break;
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_LEFT:
				{
					if ( m_siCurrentCaretPos > 0)
					{
						SI32 oldCaretPos = m_siCurrentCaretPos;

						m_siCurrentCaretPos = GetPrevPos( m_siCurrentCaretPos );

						//UpdateLqineInfo( m_siCurrentCaretPos );

						if(GetKeyState(VK_SHIFT) & 0x8000 )
						{
							// 앞에 선택된 영역이 존재할때 선택영역 줄이기
							if( m_siSelStartIndex > 0 && m_siSelStartIndex < m_siSelEndIndex
								&& m_siSelStartIndex < oldCaretPos )
							{
								m_siSelEndIndex = m_siCurrentCaretPos;
							}	// 선택영역 늘리기						
							else
							{
								m_siSelStartIndex = m_siCurrentCaretPos;

								if(m_siSelEndIndex > 0)  // 이미 선택영역의 끝은 설정되어 있다.
								{

								}
								else
								{
									m_siSelEndIndex = oldCaretPos;
								}
							}

							if ( ! GetXYFromPos( m_siSelStartIndex, m_ptSelStart ) )
							{
								OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )\r\n"));
							}

							if ( ! GetXYFromPos( m_siSelEndIndex, m_ptSelEnd ) )
							{
								OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )\r\n") );
							}
							UpdateCaretPos();
						}
						else
						{
							UpdateLineInfo( m_siCurrentCaretPos );
						}
					}				
				}
				return true;
			case VK_RIGHT:
				{
					if ( m_siCurrentCaretPos < m_siCurrentEditTextLength )
					{	
						SI32 oldCaretPos = m_siCurrentCaretPos;
						m_siCurrentCaretPos = GetNextPos( m_siCurrentCaretPos );

						if( GetKeyState(VK_SHIFT) & 0x8000)
						{
							// 오른쪽에 선택된 영역이 존재 한다면 선택영역 줄이기						
							if( m_siSelStartIndex >= 0 && m_siSelStartIndex < m_siSelEndIndex
								&& m_siSelStartIndex >= oldCaretPos )
							{
								m_siSelStartIndex = m_siCurrentCaretPos;
							}							
							else // 선택 영역 늘리기
							{
								m_siSelEndIndex = m_siCurrentCaretPos;

								if( m_siSelStartIndex >= 0)  // 이미 선택영역의 시작은 설정되어 있다.
								{

								}
								else
								{
									m_siSelStartIndex = oldCaretPos;
								}
							}

							if ( ! GetXYFromPos( m_siSelStartIndex, m_ptSelStart ) )
							{
								OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )\r\n"));
							}

							if ( ! GetXYFromPos( m_siSelEndIndex, m_ptSelEnd ) )
							{
								OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )\r\n") );
							}
							UpdateCaretPos();

						}
						else
						{
							UpdateLineInfo( m_siCurrentCaretPos );
						}
					}


					/*
					if ( m_siCurrentCaretPos < m_siCurrentEditTextLength )
					{	
					list< stLineInfo >::size_type LineInfoListNum;
					LineInfoListNum = m_LineInfoList.size();

					if ( LineInfoListNum < 1 )
					{
					return true;
					}

					SI16 LineIndex = GetLineIndexFromPos( m_siCurrentCaretPos );

					list< stLineInfo >::iterator itr;

					itr = m_LineInfoList.begin();

					SI32 Counter = 0;
					while( Counter < LineIndex )
					{
					if ( itr == m_LineInfoList.end() )
					{
					OutputDebugString(  TEXT("if ( itr == m_LineInfoList.end() )1\r\n") );
					return true;
					}

					++Counter;
					++itr;
					}

					stLineInfo *pLineInfo = &(*itr);
					if ( pLineInfo )
					{
					if ( pLineInfo->siLineStart != -1 )
					{
					if ( m_siCurrentCaretPos >= pLineInfo->siLineStart  )
					{
					if ( m_siCurrentCaretPos <= pLineInfo->siLineEnd )
					{
					if ( m_siCurrentCaretPos == pLineInfo->siLineEnd )
					{
					++m_siCurrentCaretPos;
					}
					else
					{
					if ( IsDelimiter( m_strText[ m_siCurrentCaretPos ] ) )
					{
					++m_siCurrentCaretPos;
					}
					else if ( IsDBCS( m_strText[ m_siCurrentCaretPos ], m_strText[ m_siCurrentCaretPos + 1 ] ) )
					{
					m_siCurrentCaretPos += 2;
					}
					else
					{
					++m_siCurrentCaretPos;
					}
					}

					UpdateLineInfo( m_siCurrentCaretPos );
					}
					else
					{
					if ( m_siCurrentCaretPos != pLineInfo->siLineEnd + 1 )
					{
					m_siCurrentCaretPos = pLineInfo->siLineEnd + 1;
					UpdateLineInfo( m_siCurrentCaretPos );
					}
					}
					}
					}
					}
					}
					else
					{
					m_siCurrentCaretPos = m_siCurrentEditTextLength;
					}
					*/
				}
				return true;
			case VK_HOME:
				{
					if(GetKeyState(VK_SHIFT) & 0x8000 )
					{
						// 오른쪽에 선택된 영역이 존재 한다면 선택영역 늘리기					
						if( m_siSelStartIndex >= 0 && m_siSelStartIndex < m_siSelEndIndex
							&& m_siSelStartIndex >= m_siCurrentCaretPos )
						{
							m_siSelStartIndex = 0;
							// m_siSelEndIndex 그대로 유지
						}
						// 왼쪽에 선택된 영역이 존재 한다면 선택영역 재설정
						else if( m_siSelStartIndex >= 0 && m_siSelStartIndex < m_siSelEndIndex
							&& m_siSelEndIndex <= m_siCurrentCaretPos  )
						{
							m_siSelEndIndex = m_siSelStartIndex;
							m_siSelStartIndex = 0;

						}
						else
						{
							m_siSelStartIndex = 0;
							m_siSelEndIndex = m_siCurrentCaretPos;
						}



						if ( ! GetXYFromPos( m_siSelStartIndex, m_ptSelStart ) )
						{
							OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )\r\n"));
						}

						if ( ! GetXYFromPos( m_siSelEndIndex, m_ptSelEnd ) )
						{
							OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )\r\n") );
						}

					}
					else
					{
						ClearSelection();
					}

					m_siCurrentCaretPos = 0;
					UpdateCaretPos();					
				}
				return true;
			case VK_END:
				{
					if(GetKeyState(VK_SHIFT) & 0x8000 )
					{
						// 오른쪽에 선택된 영역이 존재 한다면 선택영역 재설정					
						if( m_siSelStartIndex >= 0 && m_siSelStartIndex < m_siSelEndIndex
							&& m_siSelStartIndex >= m_siCurrentCaretPos )
						{
							m_siSelStartIndex = m_siSelEndIndex;
							m_siSelEndIndex  = m_siCurrentEditTextLength;
						}
						// 왼쪽에 선택된 영역이 존재 한다면 선택영역 재설정
						else if( m_siSelStartIndex >= 0 && m_siSelStartIndex < m_siSelEndIndex
							&& m_siSelEndIndex <= m_siCurrentCaretPos  )
						{
							m_siSelEndIndex = m_siCurrentEditTextLength;
							//m_siSelStartIndex  그대로 유지

						}
						else
						{
							m_siSelStartIndex = m_siCurrentCaretPos;
							m_siSelEndIndex = m_siCurrentEditTextLength;
						}



						if ( ! GetXYFromPos( m_siSelStartIndex, m_ptSelStart ) )
						{
							OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )\r\n"));
						}

						if ( ! GetXYFromPos( m_siSelEndIndex, m_ptSelEnd ) )
						{
							OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )\r\n") );
						}

					}
					else
					{
						ClearSelection();
					}

					m_siCurrentCaretPos = m_siCurrentEditTextLength;
					UpdateCaretPos();
				}
				return true;


			case VK_DELETE:
				{	
					if ( DeleteSelection() )
					{
						return true;
					}
					if ( m_siCurrentCaretPos < m_siCurrentEditTextLength )
					{						
						if ( IsDBCS( m_strText[ m_siCurrentCaretPos ], m_strText[ m_siCurrentCaretPos + 1 ] ) )
						{
							Delete( m_siCurrentCaretPos, 2 );
						}
						else
						{
							Delete( m_siCurrentCaretPos, 1 );
						}
					}
				}
				return true;
			case VK_RETURN:
				{
					if ( ! m_bMultiline )
					{						
						GetParent()->SendEvent( EVENT_EDITBOX_STRING, true, this );
						ClearText();
						return true;
					}
				}			
				break;
			case VK_TAB:
				{
					if ( m_bTabKeyPressNotify )
					{
						GetParent()->SendEvent( EVENT_EDITBOX_TAB, true, this );
						return true;
					}
				}
				break;
			case VK_UP:
				{
					GetParent()->SendEvent( EVENT_EDITBOX_ARROW_UP, true, this );
					return true;
				}
			case VK_DOWN:
				{
					GetParent()->SendEvent( EVENT_EDITBOX_ARROW_DOWN, true, this );
					return true;
				}
			case VK_PRIOR:
				{
					GetParent()->SendEvent( EVENT_EDITBOX_PAGEUP, true, this );
					return true;
				}
			case VK_NEXT:
				{
					GetParent()->SendEvent( EVENT_EDITBOX_PAGEDOWN, true, this );
					return true;
				}
			}
		}
		break;
	}

	return false;
}

bool CEdit::HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
		return false;

	if( m_pScroll->HandleMouse( uMsg, pt, wParam, lParam ) )
	{
		if( ! m_pScroll->IsFocus() )
		{
			GetParent()->RequestFocus( m_pScroll );
		}
		return true;
	}

	cltClient* pclclient = (cltClient*)pclClient;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				if( ! IsFocus() )
				{
					GetParent()->RequestFocus( this );
				}

				ClearSelection();

				if ( !m_bReadOnly && ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD ) == 0 )
				{
					SI16 XPos = pt.x - GetX();
					SI16 YPos = pt.y - GetY();

					SI32 Pos = GetPosFromXY( XPos, YPos );
					if ( Pos >= 0 )
					{
						m_siSelStartIndex = Pos;
						m_siSelEndIndex = Pos;
						m_siCurrentCaretPos = Pos;
						//UpdateLineInfo( m_siCurrentCaretPos );
						UpdateCaretPos();

						m_bSelDrag= true;
						SetCapture( pclClient->GetHwnd() );
					}
				}
				return true;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				ClearSelection();

				return true;
			}
		}
		break;

	case WM_LBUTTONUP:
		{	
			if ( m_bSelDrag )
			{
				m_bSelDrag = false;
				ReleaseCapture();

				SI16 XPos = pt.x - GetX();
				SI16 YPos = pt.y - GetY();

				SI32 Pos = GetPosFromXY( XPos, YPos );
				if ( Pos >= 0 )
				{
					m_siSelEndIndex = Pos;

					m_siCurrentCaretPos = Pos;
					//UpdateLineInfo( m_siCurrentCaretPos );
					UpdateCaretPos();

					SI32 SelFirst, SelSecond;
					SelFirst = min( m_siSelStartIndex, m_siSelEndIndex );
					SelSecond = max( m_siSelStartIndex, m_siSelEndIndex );

					if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )
					{
						OutputDebugString(TEXT("2-if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )\r\n"));
						ClearSelection();
					}

					if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )
					{
						OutputDebugString(TEXT("2-if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )\r\n") );
						ClearSelection();
					}
				}
				else
				{
					//ClearSelection();
				}
			}

			if( IsInArea( pt.x, pt.y ) )
			{
				return true;
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			if( IsInArea( pt.x, pt.y ) )
			{
				if ( m_bUseToolTip )
				{
					if ( m_strToolTipMsg )
					{
						cltClient *pclclient = (cltClient*)pclClient;
						pclclient->m_pInterfaceMgr->GetToolTip()->SetToolTip( m_strToolTipMsg );
					}
				}
			}

			if ( ! IsReadOnly() && m_bSelDrag )
			{	
				SI16 XPos = pt.x - GetX();
				SI16 YPos = pt.y - GetY();

				XPos = max( XPos, 0 );
				XPos = min( XPos, m_siAvailabeUseWidth );

				YPos = max( YPos, 0 );
				YPos = min( YPos, m_siAvailabeUseHeight );

				SI32 Pos = GetPosFromXY( XPos, YPos );
				if ( Pos >= 0 )
				{
					m_siSelEndIndex = Pos;
					m_siCurrentCaretPos = Pos;
					//UpdateLineInfo( m_siCurrentCaretPos );
					UpdateCaretPos();

					SI32 SelFirst, SelSecond;
					SelFirst = min( m_siSelStartIndex, m_siSelEndIndex );
					SelSecond = max( m_siSelStartIndex, m_siSelEndIndex );

					if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )
					{
						OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelFirst, m_ptSelStart ) )\r\n"));
					}

					if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )
					{
						OutputDebugString(TEXT("1-if ( ! GetXYFromPos( SelSecond, m_ptSelEnd ) )\r\n") );
					}
					return true;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if ( m_bMultiline && IsInArea( pt.x, pt.y ) )
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

bool CEdit::MsgProc( UINT uMsg, WPARAM &wParam, LPARAM &lParam )
{
	if ( ! IsAvailable() )
	{
		return false;
	}

	bool bShift, bControl;

	bShift = ((GetKeyState(VK_SHIFT) & 0x8000) != 0); 
	bControl = ((GetKeyState(VK_CONTROL) & 0x8000) != 0);

	switch( uMsg )
	{	
	case WM_CHAR:
		{
			switch( wParam )
			{
			case 1:
			case 26:  
			case 2:   
			case 14:  
			case 19:  
			case 4:   
			case 6:   
			case 7:   
				//case 10:  
			case 11:  
			case 12:  
			case 17:  
			case 23:  
			case 5:   
			case 18:  
			case 20:  
			case 25:  
			case 21:  
			case 9:   
			case 15:  
			case 16:  
			case 27:  
			case 29:  
			case 28:
				break;
			case 3:
			case 24:
				{
					if ( m_siSelStartIndex > -1 && m_siSelEndIndex > -1 )
					{	
						if ( m_siSelStartIndex != m_siSelEndIndex && OpenClipboard( NULL ) )
						{
							EmptyClipboard();

							SI32 SelFirst = m_siSelStartIndex;
							SI32 SelSecond = m_siSelEndIndex;

							SelFirst = min( m_siSelStartIndex, m_siSelEndIndex );
							SelSecond = max( m_siSelStartIndex, m_siSelEndIndex );

							HGLOBAL hBlock = GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, SelSecond - SelFirst + 1 );
							if( hBlock )
							{
								TCHAR *pszText = (TCHAR*)GlobalLock( hBlock );
								if( pszText )
								{
									CopyMemory( pszText, m_strText+SelFirst, SelSecond - SelFirst + 1 );
									pszText[ SelSecond - SelFirst ] = NULL;
									GlobalUnlock( hBlock );
								}
								SetClipboardData( CF_TEXT, hBlock );
							}
							CloseClipboard();

							if( hBlock )
							{
								GlobalFree( hBlock );
							}

							if ( wParam == 24 )
							{
								if ( DeleteSelection() )
								{
									return true;
								}
							}
						}
					}
				}
				break;
			case 22:
				{
					if( (!m_bReadOnly) && IsClipboardFormatAvailable(CF_TEXT) && OpenClipboard( NULL ) )
					{
						HANDLE handle = GetClipboardData( CF_TEXT );
						if( handle )
						{	
							TCHAR *ClipboardData = (TCHAR*)GlobalLock( handle );
							if( ClipboardData )
							{	
								DeleteSelection();
								Insert( m_siCurrentCaretPos, ClipboardData );
								GlobalUnlock( ClipboardData );
							}
						}
						CloseClipboard();
					}
				}
				break;
			case VK_BACK:
				{
					if ( m_siCurrentCaretPos > 0 && m_siCurrentCaretPos <= m_siCurrentEditTextLength )
					{
						if( DeleteSelection() )
						{
							return true;
						}

						if ( m_siCurrentCaretPos == 1 )
						{
							Delete( 0, 1 );
						}
						else
						{
							SI32 Pos = GetPrevPos( m_siCurrentCaretPos );

							if ( IsDBCS( m_strText[ Pos ], m_strText[ Pos + 1 ] ) )
							{
								Delete( Pos, 2 );
							}
							else
							{
								Delete( Pos, 1 );
							}
						}
					}
					GetParent()->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					return true;
				}
				break;
			case VK_RETURN:
				{
					if ( ! m_bMultiline )
					{
						return true;
					}
				}
			default:
				{
					if ( m_bReadOnly )
						return true;


					TCHAR ch = wParam;

					if ( m_siInputMode & EDIT_INPUT_MODE_NUMBER )
					{
						if ( isdigit( ch ) == 0 )
						{
							return true;
						}
					}

					TCHAR szChar[ 3 ] = TEXT("");

					if ( ch == '\r' )
					{
						szChar[ 0 ] = '\r';
						szChar[ 1 ] = '\n';
						szChar[ 2 ] = NULL;
					}
					else
					{
						szChar[0] = ch;
						szChar[1] = NULL;
					}

					int RepeatCounter = lParam;

					if ( RepeatCounter < 16 )
					{
						TCHAR TempBuffer[ 64 ] = TEXT("");
						if ( IsDBCS( szChar[ 0 ] ) )
						{
							for ( SI16 i = 0; i < RepeatCounter; i+=2 )
							{
								TempBuffer[ i ] = szChar[ 0 ];
							}
						}
						DeleteSelection();
						Insert( m_siCurrentCaretPos, TempBuffer );
					}
					else
					{
						DeleteSelection();
						Insert( m_siCurrentCaretPos, szChar );
					}


					m_bDBCS = false;

					GetParent()->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
				}
				break;
			}
			return true;
		}
		break;

	case WM_INPUTLANGCHANGE:
		{
			// 전체화면이고, 윈도우 기본 IME를 사용한다면,
			if( g_SoundOnOffDlg.m_bMaxWindowMode == true &&
				pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
			{
				HIMC hImc = ImmGetContext( pclClient->GetHwnd() );
				if( hImc )
				{
					// IME STATUSWINDOW를 게임의 적당한 공간에 위치시킨다.
					POINT pt;
					BOOL bRet;
					pt.x = 1024;		pt.y = 692;
					bRet = ImmSetStatusWindowPos( hImc, &pt );

					ImmReleaseContext( pclClient->GetHwnd(), hImc );
				}
				OutputDebugString("WM_INPUTLANGCHANGE\n");
			}
		}
		break;

	case WM_IME_COMPOSITION:
		{
			if ( m_bReadOnly )
			{
				return true;
			}

			HIMC hImc = ImmGetContext( pclClient->GetHwnd() );

			if ( hImc == NULL )
			{
				return true;
			}


			if (lParam & GCS_COMPSTR)
			{
				ResetCompositionString();

				int len = ImmGetCompositionString( hImc, GCS_COMPSTR, m_strCompStr, sizeof( m_strCompStr ) );

				if ( len > 0 )
				{
					m_strCompStr[ len ] = NULL;
					m_siCompStrLen = len;
					MakeCompStrImgEncBuffer();
				}
			}

			if ( lParam & GCS_RESULTSTR )
			{
				DeleteSelection();

				int len = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 );

				if ( len > 0 )
				{
					TCHAR *szChar = new TCHAR[ len + 1 ];
					ImmGetCompositionString( hImc, GCS_RESULTSTR, szChar, len );
					szChar[ len ] = NULL;

					Insert( m_siCurrentCaretPos, szChar );

					SAFE_DELETE_A( szChar );
				}
			}

			if ( lParam & GCS_COMPATTR )
			{
				int Len = ImmGetCompositionString( hImc, GCS_COMPATTR, NULL, 0 );

				if ( Len > 0 )
				{
					TCHAR *szChar = new TCHAR[ Len + 1 ];

					ImmGetCompositionString( hImc, GCS_COMPATTR, szChar, Len );
					szChar[ Len ] = NULL;

					if ( _tcslen( szChar ) > 0 )
					{
						int temp1 = 0;
						++temp1;
					}

					SI32 StartPos = 0, EndPos = 0;
					for ( ; StartPos < Len; ++StartPos )
					{
						if ( szChar[ StartPos ] == ATTR_TARGET_CONVERTED || szChar[ StartPos ] == ATTR_TARGET_NOTCONVERTED )
						{
							break;
						}
					}

					for ( EndPos = StartPos; EndPos < Len; ++EndPos )
					{
						if ( szChar[ StartPos ] != szChar[ EndPos ] )
						{
							break;
						}
					}

					m_siCompAttrStart = StartPos;
					m_siCompAttrEnd = EndPos;

					if ( m_siCompAttrEnd != m_siCompAttrStart )
					{
						MakeCompStrImgEncBuffer();
					}					

					SAFE_DELETE_A( szChar );
				}
			}

			/*
			if ( lParam & GCS_COMPREADSTR )
			{	
			m_bReadiingWindowOpen = true;

			ResetReadingString();

			int len = ImmGetCompositionString( hImc, GCS_COMPREADSTR, m_strReadingStr, sizeof( m_strReadingStr ) );

			if ( len > 0 )
			{
			m_strReadingStr[ len ] = NULL;
			m_siReadingStrLen = len;

			MakeReadingStrImgEncBuffer();
			}

			//OutputDebugString( m_strReadingStr );
			//OutputDebugString(TEXT("\r\n"));
			}
			*/

			if ( lParam & GCS_RESULTREADSTR )
			{
				ResetReadingString();
			}


			ImmReleaseContext( pclClient->GetHwnd(), hImc );

			return true;
		}
		break;
	case WM_IME_STARTCOMPOSITION:
		{
			if( pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
			{
				HIMC hIMC = ImmGetContext( pclClient->GetHwnd() );
				if ( hIMC )
				{
					COMPOSITIONFORM	CompForm;
					SI32 siParentX = 0;
					SI32 siParentY = 0;
					ZeroMemory(&CompForm, sizeof(CompForm));

					CompForm.dwStyle = CFS_POINT;
					CControllerMgr* pclParent = GetParent();
					if( pclParent != NULL && pclParent->GetParentDialog() != NULL )
					{
						siParentX = pclParent->GetParentDialog()->GetX();
						siParentY = pclParent->GetParentDialog()->GetY();
					}
					if( pclParent )
					{
						siParentX += pclParent->GetX();
						siParentY += pclParent->GetY();
					}
					CompForm.ptCurrentPos.x = pclClient->GetScreenRX() + siParentX + GetX() + m_siCaretXPos + m_siOffsetX - MAP_TILE_XSIZE_HALF;
					CompForm.ptCurrentPos.y = pclClient->GetScreenRY() + siParentY + GetY() - MAP_TILE_YSIZE;

					ImmSetCompositionWindow(hIMC, &CompForm);

					ImmReleaseContext( pclClient->GetHwnd(), hIMC );
				}
			}
		}
		return true;
	case WM_IME_ENDCOMPOSITION:
		{
			ResetCompositionString();
			ResetReadingString();
			ResetCandiateWindow();
		}
		return true;
	case WM_IME_CHAR:
		return true;
	case WM_IME_NOTIFY:
		{
			switch( wParam )
			{
			case IMN_SETCONVERSIONMODE:
			case IMN_SETOPENSTATUS:
				{
					//CheckToggleState();

					CheckInputLocale();

					/*
					if ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD )
					{
					if ( ! IsAlphaNumericMode( pclClient->GetHwnd() ) )
					{
					SetImeMode( pclClient->GetHwnd(), true );
					return true;
					}
					}
					*/
				}
				break;
			case IMN_OPENCANDIDATE:
				{
					if( pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
					{
						m_bCandidateOpen = true;

						HIMC hIMC = ImmGetContext( pclClient->GetHwnd() );
						if ( hIMC )
						{
							SI32 siParentX = 0;
							SI32 siParentY = 0;

							CANDIDATEFORM Candidateform;
							ZeroMemory(&Candidateform, sizeof(Candidateform));
							Candidateform.dwIndex = 0;
							Candidateform.dwStyle = CFS_CANDIDATEPOS ;		
							
							CControllerMgr* pclParent = GetParent();
							if( pclParent != NULL && pclParent->GetParentDialog() != NULL )
							{
								siParentX = pclParent->GetParentDialog()->GetX();
								siParentY = pclParent->GetParentDialog()->GetY();
							}
							if( pclParent )
							{
								siParentX += pclParent->GetX();
								siParentY += pclParent->GetY();
							}
							Candidateform.ptCurrentPos.x = siParentX + GetX() + m_siCaretXPos + m_siOffsetX - MAP_TILE_XSIZE_HALF;
							Candidateform.ptCurrentPos.y = siParentY + GetY() - MAP_TILE_YSIZE;

							BOOL bRval = ImmSetCandidateWindow( hIMC, &Candidateform );

							ImmReleaseContext( pclClient->GetHwnd(), hIMC );
						}
					}
					else
					{
						OutputDebugString("IMN_OPENCANDIDATE\n");
						GetCandidateList( pclClient->GetHwnd(), wParam, lParam );
						return true;
					}
				}
				break;
			case IMN_CHANGECANDIDATE:
				{
					if( pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
					{
						HIMC hIMC = ImmGetContext( pclClient->GetHwnd() );
						if ( hIMC )
						{
							SI32 siParentX = 0;
							SI32 siParentY = 0;

							CANDIDATEFORM Candidateform;
							ZeroMemory(&Candidateform, sizeof(Candidateform));
							Candidateform.dwIndex = 0;
							Candidateform.dwStyle = CFS_CANDIDATEPOS ;							
							CControllerMgr* pclParent = GetParent();
							if( pclParent != NULL && pclParent->GetParentDialog() != NULL )
							{
								siParentX = pclParent->GetParentDialog()->GetX();
								siParentY = pclParent->GetParentDialog()->GetY();
							}
							if( pclParent )
							{
								siParentX += GetParent()->GetX();
								siParentY += GetParent()->GetY();
							}
							Candidateform.ptCurrentPos.x = siParentX + GetX() + m_siCaretXPos + m_siOffsetX - MAP_TILE_XSIZE_HALF;
							Candidateform.ptCurrentPos.y = siParentY + GetY() - MAP_TILE_YSIZE;

							BOOL bRval = ImmSetCandidateWindow( hIMC, &Candidateform );

							ImmReleaseContext( pclClient->GetHwnd(), hIMC );
						}
					}
					else
					{
						OutputDebugString("IMN_CHANGECANDIDATE\n");
						GetCandidateList( pclClient->GetHwnd(), wParam, lParam );
						return false;
					}
				}				
				break;
			case IMN_CLOSECANDIDATE:
				{
					if( pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
					{
						m_bCandidateOpen = false;
					}
					else
					{
						OutputDebugString("IMN_CLOSECANDIDATE\n");
						ResetCandiateWindow();

						m_bCandidateOpen = false; 
						return true;
					}
				}
				break;
			case IMN_PRIVATE:
				{
					if( ! m_bCandidateOpen )
					{
						GetReadingString( lParam );

						// Trap some messages to hide reading window
						DWORD IMEID = GetImeID( 0 );
						switch( IMEID )
						{
						case IMEID_CHT_VER42:
						case IMEID_CHT_VER43:
						case IMEID_CHT_VER44:
						case IMEID_CHS_VER41:
						case IMEID_CHS_VER42:
							if( ( lParam == 1 ) || ( lParam == 2 ) )
							{
								return true;
							}
							break;

						case IMEID_CHT_VER50:
						case IMEID_CHT_VER51:
						case IMEID_CHT_VER52:
						case IMEID_CHT_VER60:
						case IMEID_CHS_VER53:
							if( (lParam == 16) || (lParam == 17) || (lParam == 26) || (lParam == 27) || (lParam == 28) )
							{
								return true;
							}
							break;
						}
					}
				}
				break;
			}
		}
	}

	return false;
}

void CEdit::SetFocus( bool HasFocus )
{
	CControllerObj::SetFocus( HasFocus );

	if ( HasFocus )
	{
		/*
		if ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD )
		{
		if ( ! IsAlphaNumericMode( pclClient->GetHwnd() ) )
		{
		SetImeMode( pclClient->GetHwnd(), true );
		}
		}
		*/
	}
	else
	{
		ClearSelection();
	}
}

void CEdit::SetText( TCHAR *Text)
{	
	if ( Text == NULL || m_strText == NULL )
	{
		return;
	}

	if ( _tcscmp( m_strText, Text ) == 0 )
	{
		return;
	}

	Clear();

	SI32 TextLen = _tcslen( Text );
	if ( TextLen < 1 )
	{
		return;
	}

	if ( TextLen + 1 >= m_siMaxEditTextLimitLength )
	{
		if ( m_strText )
		{
			SAFE_DELETE_A( m_strText );
		}

		m_strText = new TCHAR[ TextLen + 50 ];
		memset( m_strText, 0, TextLen + 50 );

		m_siMaxEditTextLimitLength = TextLen + 49;
	}

	DeleteSelection();
	Insert( 0, Text );

	return;
}
void CEdit::SetText( TCHAR *Text , COLORREF fontColor )
{
	SetFontColor( fontColor );
	SetText( Text );
}

bool CEdit::Insert( SI32 Pos, TCHAR *str )
{
	if ( Pos < 0 || str == NULL ) 
	{
		return false;
	}

	SI32 InsertLen = _tcslen( str );

	if ( InsertLen < 1 )
	{
		return false;
	}

	if( m_siCurrentEditTextLength + InsertLen >= m_siMaxEditTextLimitLength )
	{
		m_bDBCS = false;
		/*
		HIMC hImc;
		hImc = ImmGetContext( pclClient->GetHwnd() );

		if ( hImc )
		{
		ImmSetOpenStatus( hImc, FALSE );
		}

		if ( hImc )
		{
		ImmReleaseContext( pclClient->GetHwnd(), hImc );
		}
		*/

		OutputDebugString(  TEXT("길이 초과\r\n") );
		return false;
	}

	/*
	if ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD )
	{
	for ( SI16 i = 0; i < InsertLen; ++i )
	{
	if( str[ i ] < 0 )
	{
	return false;
	}

	if ( ! isalnum(  str[ i ] ) )
	{
	SetImeMode( pclClient->GetHwnd(), true );
	return false;
	}
	}
	}
	*/

	if ( m_siInputMode & EDIT_INPUT_MODE_NUMBER )
	{
		for ( SI16 i = 0; i < InsertLen; ++i )
		{
			if( str[ i ] < 0 )	
			{
				return false;
			}

			if ( isdigit(  str[ i ] ) == 0 )
			{
				return false;
			}
		}
	}

	memmove( m_strText + Pos + InsertLen, m_strText + Pos, m_siCurrentEditTextLength - Pos + 1 );
	memcpy( &m_strText[ Pos ], str, InsertLen );
	StringCchCat( m_strText, m_siMaxEditTextLimitLength, TEXT("") );

	m_siCurrentEditTextLength += InsertLen;
	m_siCurrentCaretPos += InsertLen;

	for ( SI16 i = 0; i < InsertLen; ++i )
	{
		if ( m_strText[ Pos + i ] == '\t' )
		{
			m_strText[ Pos + i ] = ' ';
		}
	}

	UpdateLineInfo( Pos );

	return true;
}

void CEdit::Delete( SI32 Pos, SI16 DelNum )
{
	if ( m_bReadOnly || Pos < 0 || DelNum < 1 )
	{
		return;
	}

	if ( Pos >= m_siCurrentEditTextLength )
	{
		return;
	}

	for ( SI16 i = 0; i < DelNum; ++i )
	{
		m_strText[ Pos + i ] = NULL;
	}

	memmove( m_strText + Pos, m_strText + Pos + DelNum, m_siCurrentEditTextLength - Pos - DelNum );

	m_siCurrentEditTextLength -= DelNum;
	m_siCurrentCaretPos = Pos;
	m_strText[ m_siCurrentEditTextLength ] = NULL;
	//strcat( m_strText, TEXT("") );
	/*
	if ( m_siCurrentCaretPos >= Pos )
	{
	m_siCurrentCaretPos -= DelNum;

	if ( m_siCurrentCaretPos < 0 )
	{
	m_siCurrentCaretPos = 0;
	}
	}
	*/

	UpdateLineInfo( Pos );
	return;
}

bool CEdit::DeleteSelection()
{
	if (  IsReadOnly() || ( ! IsSelectionStatus() ) )
	{
		return false;
	}

	int SelFirst, SelSecond;

	if ( m_siSelStartIndex != m_siSelEndIndex )
	{
		SelFirst = min( m_siSelStartIndex, m_siSelEndIndex );
		SelSecond = max( m_siSelStartIndex, m_siSelEndIndex );

		m_siSelStartIndex = m_siSelEndIndex = -1;

		m_siCurrentCaretPos = SelFirst;
		Delete( m_siCurrentCaretPos, SelSecond-SelFirst );

		ClearSelection();

		return true;
	}

	return false;
}

void CEdit::ClearSelection()
{
	m_siSelStartIndex = -1;
	m_siSelEndIndex = -1;
	m_bSelDrag = false;

	m_ptSelStart.x = 0;
	m_ptSelStart.y = 0;

	m_ptSelEnd.x = 0;
	m_ptSelEnd.y = 0;

	ResetCompositionString();
	ResetReadingString();
	ResetCandiateWindow();

	return;
}

void CEdit::CloseCandidate()
{
	SAFE_DELETE_A( m_pCandidateList );

	return;
}

bool CEdit::IsSelectionStatus()
{
	if ( m_bReadOnly )
	{
		return false;
	}

	if ( m_siSelStartIndex > -1 )
	{
		return true;
	}

	return false;	
}

bool CEdit::IsAvailableInsertStringToLine( HDC hDC, stLineInfo *pLineInfo, TCHAR *Text, int TextLen )
{
	if ( pLineInfo == NULL || Text == NULL || TextLen < 0 )
	{
		return false;
	}

	if ( pLineInfo->siLineStart < 0 || pLineInfo->siLineEnd < 0 )
	{
		return true;
	}

	if ( ! m_bMultiline )
	{
		return true;
	}

	if ( pLineInfo->siLineEnd > 0 )
	{
		if ( m_strText[ pLineInfo->siLineEnd - 1 ] == '\r' && m_strText[ pLineInfo->siLineEnd ] == '\n' )
		{
			return false;
		}
		else if ( m_strText[ pLineInfo->siLineEnd - 1 ] == '\r' )
		{
			OutputDebugString(  TEXT("else if ( m_strText[ pLineInfo->siLineEnd - 1 ] == '\r' )") );
		}
	}

	SI16 InsertStringFirstCharSize = 0;

	if ( TextLen < 2 )
	{
		InsertStringFirstCharSize = GetCharWidth( hDC, Text, true );
	}
	else
	{	
		if ( IsDBCS( Text[ 0 ], Text[ 1 ] ) )
		{
			InsertStringFirstCharSize = GetCharWidth( hDC, Text, true );
		}
		else
		{
			InsertStringFirstCharSize = GetCharWidth( hDC, Text, false );
		}
	}

	SI16 LineCharLen = pLineInfo->siLineEnd - pLineInfo->siLineStart + 1;
	SI32 LineTextWidth = GetTextExtent( hDC, &m_strText[ pLineInfo->siLineStart ], LineCharLen );

	if ( LineTextWidth + InsertStringFirstCharSize >= m_siAvailabeUseWidth )
	{
		return false;
	}

	return true;
}

void CEdit::StringExamine( HDC hDC, TCHAR *Text, int TextLen )
{
	return;
}

void CEdit::MakeAllLineFromStartPos( HDC hDC, SI32 &StartPos )
{
	if ( StartPos < 0 )
	{
		OutputDebugString(  TEXT("if ( StartPos < 1\r\n") );
		return;
	}

	list< stLineInfo >::size_type LineInfoListNum;
	LineInfoListNum = m_LineInfoList.size();

	SI16 LineIndex = GetLineIndexFromPos( StartPos );
	--LineIndex;
	if ( LineIndex < 0 )
	{
		LineIndex = 0;
	}

	if ( LineInfoListNum > 0 && (SI16)LineInfoListNum <= LineIndex )
	{
		OutputDebugString(  TEXT("if ( LineIndex > 0 && LineInfoListNum <= LineIndex )\r\n") );
		return;
	}

	list< stLineInfo >::iterator itr;

	itr = m_LineInfoList.begin();

	SI32 Counter = 0;
	while( Counter < LineIndex )
	{
		if ( itr == m_LineInfoList.end() )
		{
			OutputDebugString(  TEXT("if ( itr == m_LineInfoList.end() )7\r\n") );
			return;
		}

		++Counter;
		++itr;
	}

	if ( LineInfoListNum > 0 )
	{
		stLineInfo *pLineInfo = &(*itr);
		if ( pLineInfo )
		{
			if ( pLineInfo->siLineStart != -1 )
			{
				StartPos = pLineInfo->siLineStart;
			}
		}
	}

	while ( itr != m_LineInfoList.end() )
	{
		m_LineInfoList.erase( itr++ );
	}

	if ( m_siCurrentEditTextLength > 0 )
	{
		MakeEachLineFromStartPos( hDC, StartPos );
	}

	return;
}

SI16 CEdit::MakeEachLineFromStartPos( HDC hDC, SI32 &StartPos )
{
	if ( StartPos < 0 || m_strText == NULL )
	{
		return -1;
	}

	stLineInfo sLineInfo;

	if ( StartPos == m_siCurrentEditTextLength )
	{
		sLineInfo.siLineStart = StartPos;
		sLineInfo.siLineEnd = StartPos;

		m_LineInfoList.push_back( sLineInfo );
		return 0;
	}

	SI16 CurrentXSize = 0;
	SI16 CharSize = 0;
	SI16 LineCounter = 0;
	SI32 i = StartPos;

	bool ConditionFind = false;

	for (; i < m_siCurrentEditTextLength; ++i )
	{
		if ( m_siInputMode & EDIT_INPUT_MODE_CHANGETEXTDRAW )
		{
			if ( m_strText[ i ] == '<' )
			{
				ConditionFind = true;
				continue;
			}

			if ( m_strText[ i ] == '>' )
			{
				ConditionFind = false;
				continue;
			}
		}

		if ( ! ConditionFind )
		{
			if ( i + 1 < m_siCurrentEditTextLength &&
				IsDBCS( m_strText[ i ], m_strText[ i + 1 ] ) == TRUE )
			{	
				if ( m_strText[ i ] == '\r')
				{
					sLineInfo.siLineStart = StartPos;
					if ( i - 1 < StartPos )
					{
						sLineInfo.siLineEnd = StartPos;
					}
					else
					{
						sLineInfo.siLineEnd = i - 1;
					}

					StartPos = i + 2;
					m_LineInfoList.push_back( sLineInfo );

					CurrentXSize = 0;
				}
				else
				{
					CharSize = GetCharWidth( hDC, &m_strText[ i ], true );

					if ( CurrentXSize + CharSize >= m_siAvailabeUseWidth )
					{
						if( pclClient->IsCountrySwitch(Switch_EditWordBreak) )
						{
							//---------------------------------------------------------------
							SI32 siFindWhiteSpace = i;
							// wordbreak - 빈칸을 찾을때까지 앞으로 다시 간다
							while( siFindWhiteSpace > StartPos )
							{
								siFindWhiteSpace--;
								if( _istspace(m_strText[siFindWhiteSpace]) )		break;
							}
							if( siFindWhiteSpace > StartPos )		i = siFindWhiteSpace;
							//---------------------------------------------------------------
						}
						sLineInfo.siLineStart = StartPos;
						if ( i - 1 < StartPos )
						{
							sLineInfo.siLineEnd = StartPos;
						}
						else
						{
							sLineInfo.siLineEnd = i - 1;
						}					

						StartPos = i;					
						m_LineInfoList.push_back( sLineInfo );

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
				CharSize = GetCharWidth( hDC, &m_strText[ i ], false );

				if ( CurrentXSize + CharSize >= m_siAvailabeUseWidth )
				{
					if( pclClient->IsCountrySwitch(Switch_EditWordBreak) )
					{
						//---------------------------------------------------------------
						SI32 siFindWhiteSpace = i;
						// wordbreak - 빈칸을 찾을때까지 앞으로 다시 간다
						while( siFindWhiteSpace > StartPos )
						{
							siFindWhiteSpace--;
							if( _istspace(m_strText[siFindWhiteSpace]) )		break;
						}
						if( siFindWhiteSpace > StartPos )		i = siFindWhiteSpace;
						//---------------------------------------------------------------
					}
					sLineInfo.siLineStart = StartPos;
					if ( i - 1 < StartPos )
					{
						sLineInfo.siLineEnd = StartPos;
					}
					else
					{
						sLineInfo.siLineEnd = i - 1;
					}

					StartPos = i;				
					m_LineInfoList.push_back( sLineInfo );

					CurrentXSize = CharSize;
				}
				else
				{
					CurrentXSize += CharSize;
				}
			}
		}
	}

	sLineInfo.siLineStart = StartPos;
	if ( i - 1 < StartPos )
	{
		sLineInfo.siLineEnd = StartPos;
	}
	else
	{
		sLineInfo.siLineEnd = i - 1;
	}

	StartPos = i;
	m_LineInfoList.push_back( sLineInfo );

	return 1;
}

SI32 CEdit::GetPosFromXY( SI16 XPos, SI16 YPos )
{	
	if ( IsReadOnly() )
	{
		return -1;
	}

	SI32 LineIndex = GetLineIndexFromXY( XPos, YPos );
	if ( LineIndex < 0 )
	{
		return -1;
	}

	if ( m_siCurrentEditTextLength < 1 )
	{
		return -1;
	}

	list< stLineInfo >::iterator itr;
	itr = m_LineInfoList.begin();

	SI32 Counter = 0;
	while( Counter < LineIndex )
	{
		if ( itr == m_LineInfoList.end() )
		{
			OutputDebugString(  TEXT("if ( itr == m_LineInfoList.end() )3\r\n") );
			return -1;
		}

		++Counter;
		++itr;
	}

	SI32 i = 0;

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();
	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();
		if ( lpSurface )
		{
			HDC hDC;
			HFONT hFont;
			if( lpSurface->GetDC( &hDC ) == DD_OK)
			{
				cltClient *pclclient = ( cltClient * )pclClient;

				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}
				
				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetBkMode( hDC, TRANSPARENT );

				SI16 Size = m_siOffsetX;

				stLineInfo *pLineInfo = &(*itr);
				if ( pLineInfo )
				{					
					for ( i = pLineInfo->siLineStart; i < pLineInfo->siLineEnd + 1; ++i )
					{
						if ( XPos < Size )
						{
							break;
						}

						if ( IsDBCS( m_strText[ i ] ) )
						{
							Size += GetCharWidth( hDC, &m_strText[ i ], true );
							++i;
						}
						else
						{
							Size += GetCharWidth( hDC, &m_strText[ i ], false );
						}
					}
				}

				SelectObject( hDC, hOldFont );
				DeleteObject( hFont );
				lpSurface->ReleaseDC( hDC );
			}
		}
	}

	return i;
}

bool CEdit::GetXYFromPos( SI32 Pos, POINT &pt )
{	
	if ( IsReadOnly() )
	{
		return false;
	}

	if ( Pos < 0 || Pos > m_siCurrentEditTextLength )
	{
		return false;
	}

	SI32 LineIndex = GetLineIndexFromPos( Pos );

	if ( LineIndex < 0 )
	{
		return false;
	}

	list< stLineInfo >::size_type listsize;
	listsize = m_LineInfoList.size();

	if(listsize <1 ) return false;

	list< stLineInfo >::iterator itr;
	itr = m_LineInfoList.begin();



	SI32 Counter = 0;
	while( Counter < LineIndex )
	{
		if ( itr == m_LineInfoList.end() )
		{
			OutputDebugString(  TEXT("if ( itr == m_LineInfoList.end() )3\r\n") );
			return false;
		}

		++Counter;
		++itr;
	}

	SI32 i = 0;

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
					 hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}		

				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetBkMode( hDC, TRANSPARENT );

				SI16 Size = m_siOffsetX;

				stLineInfo *pLineInfo = &(*itr);
				if ( pLineInfo )
				{					
					for ( i = pLineInfo->siLineStart; i < pLineInfo->siLineEnd + 1; ++i )
					{
						if ( i >= Pos )
						{
							break;
						}

						if ( IsDBCS( m_strText[ i ] ) )
						{
							Size += GetCharWidth( hDC, &m_strText[ i ], true );
							++i;
						}
						else
						{
							Size += GetCharWidth( hDC, &m_strText[ i ], false );
						}
					}
				}

				SelectObject( hDC, hOldFont );
				DeleteObject( hFont );
				lpSurface->ReleaseDC( hDC );

				pt.y = ( LineIndex - m_pScroll->GetTrackPos() )  * m_siLineHeight + m_siOffsetY;
				pt.x = Size;
			}
		}
	}

	return true;
}

SI32 CEdit::GetLineIndexFromXY( SI16 XPos, SI16 YPos )
{
	//XPos -= m_siOffsetX;
	YPos -= m_siOffsetY;

	// 아래에 = 이 조건은 나중에 안정되었다 싶을 때 없애면 됨.
	if ( XPos < 0 || XPos >= m_siAvailabeUseWidth ||
		YPos < 0 || YPos >= m_siAvailabeUseHeight
		)
	{
		return -1;
	}

	list< stLineInfo >::size_type LineInfoListNum;
	LineInfoListNum = m_LineInfoList.size();

	if ( LineInfoListNum < 1 )
	{
		return -1;
	}

	SI32 StartLineIndex = m_pScroll->GetTrackPos();
	if ( StartLineIndex < 0 || (SI16)LineInfoListNum <= StartLineIndex )
	{
		return -1;
	}


	SI16 CurrentAvailableLineNum = LineInfoListNum - StartLineIndex;

	SI16 CurLineIndex = YPos / m_siLineHeight + ( YPos%m_siLineHeight ? 0 : -1 );

	return CurLineIndex < CurrentAvailableLineNum ? ( StartLineIndex + CurLineIndex ) : -1;
}

SI16 CEdit::GetLineIndexFromPos( SI16 Pos )
{
	SI16 LineIndex = 0;

	list< stLineInfo >::iterator itr;
	itr = m_LineInfoList.begin();

	while ( itr != m_LineInfoList.end() )
	{
		stLineInfo *pLineInfo = &(*itr);

		if ( pLineInfo )
		{
			if ( Pos <= pLineInfo->siLineEnd )
			{
				break;
			}
			else
			{	
				++itr;

				if ( itr == m_LineInfoList.end() )
				{
					break;
				}

				++LineIndex;
			}
		}
	}

	return LineIndex;
}


bool CEdit::IsMultiLine( HDC hDC, TCHAR *Text, int TextLen )
{
	if ( Text == NULL || TextLen < 1 )
	{
		OutputDebugString(  TEXT("3\r\n") );
		return false;
	}

	SIZE sz;
	if ( GetTextExtentPoint32( hDC, Text, TextLen, &sz) == FALSE )
	{
		OutputDebugString(  TEXT("2\r\n") );
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

SI32 CEdit::GetPrevPos( SI32 Pos )
{
	int n, size;

	if ( Pos == 0 )
	{
		return 0;
	}

	for ( n = Pos - 1; n != 0; --n )
	{
		if ( m_strText[ n ]=='\r' && m_strText[ n + 1 ]=='\n' )
		{
			break;
		}
	}

	for (;;)
	{
		if ( IsDBCS( m_strText[ n ], m_strText[ n + 1 ] ) )
		{
			size=2;
		}
		else
		{
			size=1;
		}
		n += size;

		if (n >= Pos )
			break;
	}

	return n-size;
}

bool CEdit::IsPrevWordDBCS( TCHAR *Text, SI16 nPos )
{
	if ( Text == NULL || nPos < 1 )
	{
		OutputDebugString(  TEXT("Error IsPrevDBCS0\r\n") );

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

SI32 CEdit::GetNextPos( SI32 Pos )
{
	if ( Pos >= m_siCurrentEditTextLength )
	{
		return m_siCurrentEditTextLength;
	}

	if ( IsDBCS( m_strText[ Pos ], m_strText[ Pos + 1 ] ) )
	{
		return Pos + 2;
	}
	else
	{
		return Pos + 1;
	}
}

SI16 CEdit::GetCharWidth( HDC hDC, TCHAR* OneChar, bool DBCS )
{	
	if ( OneChar == NULL )
	{
		OutputDebugString(  TEXT("Error 4\r\n") );
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
			OutputDebugString(  TEXT("0\r\n") );
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
			OutputDebugString(  TEXT("1\r\n") );
			return 0;
		}
	}
}

SI16 CEdit::GetLineWidth( HDC hDC, TCHAR *pText, SI16 TextLen )
{
	if ( pText == NULL )
	{
		return 0;
	}

	SI16 LineWidth = 0;

	if ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD )
	{
		LineWidth = m_siAsterikPixelLen * m_siCurrentCaretPos;
		return LineWidth;
	}
	else if ( m_siInputMode & EDIT_INPUT_MODE_CHANGETEXTDRAW )
	{
		SI32 StartPos = 0;
		SI16 Counter = 0;
		bool ConditionFind = false;

		for ( SI16 i = 0; i < TextLen; ++i )
		{
			if ( m_strText[ i ] == '<' )
			{
				if ( Counter > 0 )
				{	
					LineWidth += GetTextExtent( hDC, &pText[ StartPos ], i - StartPos - 1 );
				}

				int ConditionTextLen = ChangeText( hDC, i );

				switch ( ConditionTextLen )
				{
				case EDIT_CHANGETEXTDRAW_CONDITION_INSTALL_TEXT_LEN:
					{
						ConditionFind = true;
					}
					break;
				case EDIT_CHANGETEXTDRAW_CONDITION_UNINSTALL_TEXT_LEN:
					{
						ConditionFind = false;
					}
					break;
				default:
					OutputDebugString(  TEXT("요기에 올수 없는뎅.") );
					break;
				}

				i += ConditionTextLen;
				StartPos = i + 1;

				Counter = 0;
			}
			else if ( ! ConditionFind )
			{
				++Counter;
			}
			/*
			// 요기에 올건 <C01> 이라면, C01 이 오겠징...
			else
			{	
			}
			*/
		}

		if ( Counter > 0 &&  ( ! ConditionFind ) )
		{
			LineWidth += GetTextExtent( hDC, &pText[ StartPos ], i - StartPos );
		}
	}
	else
	{
		return GetTextExtent( hDC, &pText[ 0 ], TextLen );
	}

	return 0;
}

void CEdit::UpdateLineInfo( SI32 StartPos )
{
	if ( StartPos < 0 )
	{
		OutputDebugString(  TEXT("if ( StartPos < 0 )\r\n") );
		return;
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
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}

				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetBkMode( hDC, TRANSPARENT );

				MakeAllLineFromStartPos( hDC, StartPos );

				SelectObject( hDC, hOldFont );

				DeleteObject( hFont );

				lpSurface->ReleaseDC( hDC );
			}
		}
	}

	list< stLineInfo >::size_type LineInfoListNum;
	LineInfoListNum = m_LineInfoList.size();

	if ( LineInfoListNum > 0 )
	{
		m_pScroll->SetTrackRange( 0, LineInfoListNum );
	}
	else
	{
		m_pScroll->SetTrackRange( 0, 1 );
	}

	SI32 ScrollPageSize = m_pScroll->GetPageSize();
	if ( (SI32)LineInfoListNum > ScrollPageSize )
	{
		if ( m_bMultiline )
		{
			m_pScroll->Show( true );
		}

		if ( IsShow() )
		{
			if( m_bAutoScroll )
			{
				m_pScroll->ScrollMove( LineInfoListNum - ScrollPageSize );
			}
			else
			{
				Refresh();
			}
		}
	}
	else if ( IsShow() )
	{
		m_pScroll->Show( false );

		Refresh();
	}

	UpdateCaretPos();

	ResetCompositionString();
	ResetReadingString();
	ResetCandiateWindow();

	return;
}

void CEdit::UpdateCaretPos()
{
	list< stLineInfo >::size_type LineInfoListNum;
	LineInfoListNum = m_LineInfoList.size();

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		if ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD )
		{
			LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();
			if ( lpSurface )
			{
				HDC hDC;

				if( lpSurface->GetDC( &hDC ) == DD_OK)
				{
					SI16 LineWidth =  GetLineWidth( hDC, m_strText, m_siCurrentEditTextLength );

					if ( LineWidth >= m_siAvailabeUseWidth )
					{
						m_siCaretXPos = -1;
						m_siCaretYPos = -1;
					}
					else
					{
						m_siCaretXPos = LineWidth;//m_siPasswordPixelLen;
						m_siCaretYPos = 0;
					}
				}

				lpSurface->ReleaseDC( hDC );
			}
		}
		else if ( !m_bReadOnly )
		{
			m_siCaretXPos = 0;
			m_siCaretYPos = 0;

			LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();
			if ( lpSurface )
			{
				HDC hDC;
				if( lpSurface->GetDC( &hDC ) == DD_OK)
				{
					cltClient *pclclient = ( cltClient * )pclClient;
					HFONT hFont;

					if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
						hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
					}
					else {
						hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
					}

					HFONT hOldFont = NULL;

					hOldFont = (HFONT)SelectObject( hDC, hFont );

					SetBkMode( hDC, TRANSPARENT );

					if ( LineInfoListNum > 1 )
					{
						SI16 CurrentCaretLineIndex = 0;

						if ( m_bMultiline )
						{
							CurrentCaretLineIndex = GetLineIndexFromPos( m_siCurrentCaretPos );
						}
						else
						{
							if ( m_bAutoScroll )
							{
								CurrentCaretLineIndex = m_pScroll->GetTrackPos();
							}
						}

						list< stLineInfo >::iterator itr;
						itr = m_LineInfoList.begin();

						SI32 Counter = 0;
						while( Counter < CurrentCaretLineIndex )
						{
							if ( itr == m_LineInfoList.end() )
							{
								OutputDebugString(  TEXT("if ( itr == m_LineInfoList.end() )3\r\n") );
								return;
							}

							++Counter;
							++itr;
						}

						stLineInfo *pLineInfo = &(*itr);
						if ( pLineInfo )
						{
							if ( pLineInfo->siLineStart != -1 && pLineInfo->siLineStart <= pLineInfo->siLineEnd)
							{
								SI32 Width = GetLineWidth( hDC, &m_strText[ pLineInfo->siLineStart ], m_siCurrentCaretPos - pLineInfo->siLineStart );
								if ( Width > m_siAvailabeUseWidth )
								{
									m_siCaretXPos = -1;
									m_siCaretYPos = -1;
								}
								else
								{
									m_siCaretXPos = Width;
									if ( m_bMultiline )
									{
										if ( m_pScroll->IsShow() )
										{
											m_siCaretYPos = m_siLineHeight * ( CurrentCaretLineIndex - m_pScroll->GetTrackPos() );
										}
										else
										{
											m_siCaretYPos = m_siLineHeight * CurrentCaretLineIndex;
										}
									}
									else
									{
										m_siCaretYPos = 0;
									}
								}
							}
						}
					}
					else if ( LineInfoListNum == 1 )
					{
						list< stLineInfo >::iterator itr;
						itr = m_LineInfoList.begin();
						stLineInfo *pLineInfo = &(*itr);
						if ( pLineInfo )
						{
							if ( pLineInfo->siLineStart != -1 && pLineInfo->siLineStart <= pLineInfo->siLineEnd)
							{
								SI32 Width = GetLineWidth( hDC, &m_strText[ pLineInfo->siLineStart ], m_siCurrentCaretPos - pLineInfo->siLineStart );
								if ( Width > m_siAvailabeUseWidth )
								{
									m_siCaretXPos = -1;
									m_siCaretYPos = -1;
								}
								else
								{
									m_siCaretXPos = Width;
									m_siCaretYPos = 0;
								}
							}
						}
					}

					SelectObject( hDC, hOldFont );
					DeleteObject( hFont );
					lpSurface->ReleaseDC( hDC );
				}			
			}
		}
	}

	return;
}

void CEdit::MakeTextImgEncBuffer()
{
	if ( ( ! IsAvailable() ) ||  ( ! IsShow() ) )
	{
		return;
	}

	SAFE_DELETE_A( m_ImgEncBuffer );

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
				SI16 CurrentDrawXPos = m_siOffsetX;
				SI16 CurrentDrawYPos = m_siOffsetY;
				HFONT hFont;

				if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}
				else {
					hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
				}
				
				HFONT hOldFont = NULL;

				hOldFont = (HFONT)SelectObject( hDC, hFont );

				SetTextColor( hDC, GetFontColor() );

				SetBkMode( hDC, TRANSPARENT );

				if ( IsBorder() && IsBKColorUse() )
				{
					DrawRoundRect( hDC, 0, 0, SurfaceWidth - 1, SurfaceHeight - 1, IsBorder(), IsBKColorUse(), GetBorderColor(), GetBKColor() );
				}
				else if ( IsBorder() )
				{
					RECT rect = { 0, 0, SurfaceWidth - 1, SurfaceHeight - 1 };
					DrawBox( hDC, &rect );
				}

				list< stLineInfo >::iterator itr;
				itr = m_LineInfoList.begin();

				SI32 CurrentScrollLine = m_pScroll->GetTrackPos();
				if ( CurrentScrollLine > 0 )
				{
					SI32 LineCount = 0;
					while( LineCount < CurrentScrollLine )
					{	
						++LineCount;
						++itr;
					}
				}

				while ( itr != m_LineInfoList.end() )
				{
					stLineInfo *pLineInfo = &(*itr);
					if ( pLineInfo )
					{	
						if ( m_siInputMode & EDIT_INPUT_MODE_PASSWORD )
						{
							SI16 PasswordLen = _tcslen( m_strText );
							TCHAR szAsterik[] = TEXT("*");
							m_siAsterikPixelLen = GetTextExtent( hDC, szAsterik, 1 );
							TCHAR *szPassword = new TCHAR[ PasswordLen + 1 ];
							SI16 t = 0;
							SI16 DrawWidth = 0;
							for ( ; t < PasswordLen; ++t )
							{
								DrawWidth += m_siAsterikPixelLen;
								if ( DrawWidth < m_siAvailabeUseWidth )
								{
									szPassword[ t ] = '*';
								}
								else
								{
									break;
								}
							}
							szPassword[ t ] = NULL;

							PasswordLen = _tcslen( szPassword );
							m_siPasswordPixelLen = GetTextExtent( hDC, szPassword, PasswordLen );

							TextOut( hDC, CurrentDrawXPos, CurrentDrawYPos, szPassword, PasswordLen );

							SAFE_DELETE_A( szPassword );

							break;
						}
						else if ( m_siInputMode & EDIT_INPUT_MODE_CHANGETEXTDRAW )
						{
							SI32 StartPos = pLineInfo->siLineStart;
							SI16 Counter = 0;
							SI16 DrawXPos = CurrentDrawXPos;

							for ( i = pLineInfo->siLineStart; i < pLineInfo->siLineEnd; ++i )
							{
								if ( m_strText[ i ] == '<' )
								{
									if ( Counter > 0 )
									{	
										if ( m_bMultiline )
										{
											TextOut( hDC, DrawXPos, CurrentDrawYPos, &m_strText[ StartPos ], i - StartPos );
										}
										else
										{
											RECT Rect = { DrawXPos, CurrentDrawYPos, m_siAvailabeUseWidth - 1, CurrentDrawYPos + m_siLineHeight - 1 };
											DrawText( hDC, &m_strText[ StartPos ], i - StartPos, &Rect, GetTextPrintOption() );
										}

										DrawXPos += GetTextExtent( hDC, &m_strText[ StartPos ], i - StartPos );
									}

									int ConditionTextLen = ChangeText( hDC, i );

									i += ConditionTextLen;
									StartPos = i + 1;
									Counter = 0;
								}
								else
								{
									++Counter;
								}
							}

							if ( Counter > 0 )
							{	
								if ( m_bMultiline )
								{
									TextOut( hDC, DrawXPos, CurrentDrawYPos, &m_strText[ StartPos ], i - StartPos + 1  );
								}
								else
								{
									RECT Rect = { DrawXPos, CurrentDrawYPos, m_siAvailabeUseWidth - 1, CurrentDrawYPos + m_siLineHeight - 1 };
									DrawText( hDC, &m_strText[ StartPos ], i - StartPos + 1, &Rect, GetTextPrintOption() );
								}
							}
							CurrentDrawYPos += m_siLineHeight;
						}
						else
						{
							if ( m_bMultiline )
							{
								//if ( pLineInfo->siLineEnd != pLineInfo->siLineStart )
								//{
								if ( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )
								{
									if (m_strText[ pLineInfo->siLineStart ] != '\r')
									{
										TextOut( hDC, CurrentDrawXPos, CurrentDrawYPos, &m_strText[ pLineInfo->siLineStart ], pLineInfo->siLineEnd - pLineInfo->siLineStart + 1  );
									}
								}
								else
								{
									TextOut( hDC, CurrentDrawXPos, CurrentDrawYPos, &m_strText[ pLineInfo->siLineStart ], pLineInfo->siLineEnd - pLineInfo->siLineStart + 1  );
								}
								
								//}
							}
							else
							{
								//if ( pLineInfo->siLineEnd != pLineInfo->siLineStart )
								if ( pLineInfo->siLineStart >= 0 && ( m_strText[ pLineInfo->siLineStart ] != '\r' || m_strText[ pLineInfo->siLineStart ] != '\n' ) )
								{
									RECT Rect = { CurrentDrawXPos, CurrentDrawYPos, m_siAvailabeUseWidth - 1, CurrentDrawYPos + m_siLineHeight - 1 };
									DrawText( hDC, &m_strText[ pLineInfo->siLineStart ], pLineInfo->siLineEnd - pLineInfo->siLineStart + 1, &Rect, GetTextPrintOption() );
								}
							}
							CurrentDrawYPos += m_siLineHeight;
						}
					}

					if ( CurrentDrawYPos > SurfaceHeight - m_siOffsetY )
					{	
						break;
					}

					++itr;
				}
				if(m_siCurrentCaretPos 	> 0 ) // 스크롤이 왓다 갔다 하는 경우가 있어서..텍스트를 다 그리고 나서 라인업데이트를 한번 해줍니다 
				{
					UpdateLineInfo( m_siCurrentCaretPos );
				}
				SelectObject( hDC, hOldFont );
				DeleteObject( hFont );

				lpSurface->ReleaseDC( hDC );
			}

			if ( GP.LockSurface( lpSurface ) )
			{
				if(m_ImgEncBuffer)
				{
					int size;
					g_graphics.EncodeXpr( m_ImgEncBuffer, SurfaceWidth, SurfaceHeight, RGB16_TRANS, &size );
				}
				GP.UnlockSurface( lpSurface );
			}
		}
	}

	return;
}

void CEdit::MakeCompStrImgEncBuffer()
{
	if ( ( ! IsAvailable() ) ||  ( ! IsShow() ) )
	{
		return;
	}

	SAFE_DELETE_A( m_ImgCompEncBuffer );

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			cltClient *pclclient = ( cltClient * )pclClient;

			HDC hDC;

			if ( m_siCompStrLen > 0 )
			{
				g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

				m_siCompStrWidth = 0;
				m_siCompStrHeight = EDIT_LINEHEIGHT;

				int AttrLineWidth = 0;
				m_siAttrLineStartPos = 0;

				if( lpSurface->GetDC( &hDC ) == DD_OK)
				{
					HFONT hFont;
					if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
						hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
					}
					else {
						hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
					}
					HFONT hOldFont = NULL;

					hOldFont = (HFONT)SelectObject( hDC, hFont );

					SetTextColor( hDC, RGB(0,0,0) );

					SetBkMode( hDC, TRANSPARENT );

					m_siCompStrWidth = GetTextExtent( hDC, m_strCompStr, m_siCompStrLen );

					if ( m_siCompAttrStart != m_siCompAttrEnd )
					{
						m_siAttrLineStartPos = GetTextExtent( hDC, m_strCompStr, m_siCompAttrStart );
						AttrLineWidth = GetTextExtent( hDC, &m_strCompStr[ m_siCompAttrStart ], m_siCompAttrEnd - m_siCompAttrStart );
					}

					m_siCompStrWidth += 5;			// 여분

					m_ImgCompEncBuffer = new RGB16[ m_siCompStrWidth * m_siCompStrHeight * 2 ];

					RECT Rect = { 0, 0, m_siCompStrWidth - 1, m_siCompStrHeight - 1 };

					DrawText( hDC, m_strCompStr, m_siCompStrLen, &Rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

					SelectObject( hDC, hOldFont );

					DeleteObject( hFont );

					lpSurface->ReleaseDC( hDC );
				}

				if ( GP.LockSurface( lpSurface ) )
				{
					if ( AttrLineWidth > 0 && m_siCompAttrStart != m_siCompAttrEnd )
					{
						TSpr *pImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( EDIT_COMPATTRIMAGE_FILENAME );
						if ( pImg )
						{
							DrawTileImg( pImg, SPR_PUTSPRT, EDIT_COMPATTRIMAGE_FONTINDEX, 0, m_siAttrLineStartPos, 0, AttrLineWidth, m_siLineHeight, 1 );	
						}
					}

					if(m_ImgCompEncBuffer)
					{
						int size;
						g_graphics.EncodeXpr( m_ImgCompEncBuffer, m_siCompStrWidth, m_siCompStrHeight, RGB16_TRANS, &size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}
		}
	}

	return;
}

void CEdit::MakeReadingStrImgEncBuffer()
{
	if ( ( ! IsAvailable() ) ||  ( ! IsShow() ) )
	{
		return;
	}

	SAFE_DELETE_A( m_ImgReadingEncBuffer );

	CControllerSurfaceObj *pControllerSurfaceObj =  GetControllerSurfaceObj();

	if ( pControllerSurfaceObj )
	{
		LPDIRECTDRAWSURFACE7 lpSurface = pControllerSurfaceObj->GetSurface();

		if ( lpSurface )
		{
			cltClient *pclclient = ( cltClient * )pclClient;

			HDC hDC;

			if ( m_siReadingStrLen > 0 )
			{
				g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

				m_siReadingStrWidth = 0;
				m_siReadingStrHeight = EDIT_LINEHEIGHT;

				if( lpSurface->GetDC( &hDC ) == DD_OK)
				{
					HFONT hFont;
					if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
						hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, FIXED_PITCH|FF_MODERN, GetFontKind() );
					}
					else {
						hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, FIXED_PITCH|FF_MODERN, GetFontKind() );
					}
					

					HFONT hOldFont = NULL;

					hOldFont = (HFONT)SelectObject( hDC, hFont );

					SetTextColor( hDC, GetFontColor() );

					SetBkMode( hDC, TRANSPARENT );

					m_siReadingStrWidth = GetTextExtent( hDC, m_strReadingStr, m_siReadingStrLen );

					m_siReadingStrWidth += 5;			// 여분

					m_ImgReadingEncBuffer = new RGB16[ m_siReadingStrWidth * m_siReadingStrHeight * 2 ];

					RECT Rect = { 0, 0, m_siReadingStrWidth - 1, m_siReadingStrHeight - 1 };

					DrawText( hDC, m_strReadingStr, m_siReadingStrLen, &Rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

					SelectObject( hDC, hOldFont );

					DeleteObject( hFont );

					lpSurface->ReleaseDC( hDC );
				}

				if ( GP.LockSurface( lpSurface ) )
				{
					if(m_ImgReadingEncBuffer)
					{
						int size;
						g_graphics.EncodeXpr( m_ImgReadingEncBuffer, m_siReadingStrWidth, m_siReadingStrHeight, RGB16_TRANS, &size );
					}
					GP.UnlockSurface( lpSurface );
				}
			}
		}
	}

	return;
}

void CEdit::MakeCandidateWindowImgEncBuffer()
{
	if ( ( ! IsAvailable() ) ||  ( ! IsShow() ) )
	{
		return;
	}

	CheckInputLocale();

#ifdef _DEBUG
	TCHAR	szBuf[1024];
	wsprintf(szBuf, "dwSelection:%d, dwPageStart:%d, dwPageSize:%d\n", 
		m_pCandidateList->dwSelection,
		m_pCandidateList->dwPageStart,
		m_pCandidateList->dwPageSize );
	OutputDebugString(szBuf);
#endif


#ifdef _DEBUG
	wsprintf(szBuf, "m_siCandidateDataStartIndex:%d\n", 
		m_siCandidateDataStartIndex);
	OutputDebugString(szBuf);
#endif
	cltClient *pclclient = (cltClient *)pclClient;
	pclclient->m_pInterfaceMgr->SetCandidateWindow( m_pCandidateList, m_siCandidateDataStartIndex, m_bVerticalCandidateWindow, m_pCandidateDataList, GetFontHeight(), GetFontWidth(), GetFontWeight(), GetFontKind(), RGB( 0, 0, 0) );

	return;
}

void CEdit::Refresh()
{
	ClearSelection();

	MakeTextImgEncBuffer();
	return;
}

void CEdit::DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime )
{
	if ( ! IsShow() )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	if ( IsBKTileImg() )
	{
		TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
		if ( pBkImg )
		{
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:
				{	DrawTileImg( pBkImg, SPR_PUTSPRT, GetFontIndex(), 0, BaseXPos + GetX(), BaseYPos + GetY(), GetWidth(), GetHeight(), GetBKTileImgNum() );					}	 break;
			case SPR_PUTSPRALPHAT:
				{	DrawTileImg( pBkImg, SPR_PUTSPRALPHAT, GetFontIndex(), GetAlphaValue(), BaseXPos + GetX(), BaseYPos + GetY(), GetWidth(), GetHeight(), GetBKTileImgNum() );	}	 break;
			}
		}
	}
	else
	{
		TSpr *pBkImg = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( GetFileName() );
		if ( pBkImg )
		{
			switch( GetDrawOption() )
			{
			case SPR_PUTSPRT:		GP.PutSpr(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), 0 );							break;
			case SPR_PUTSPRALPHAT:	GP.PutSprAlpha(pBkImg, BaseXPos + GetX(), BaseYPos + GetY(), GetAlphaValue(), 0 );		break;
			}
		}
	}

	if ( m_ImgEncBuffer && IsShow() )
	{
		switch( GetDrawOption() )
		{
		case SPR_PUTSPRT:
			{
				GP.PutXpr16( BaseXPos + GetX(), BaseYPos + GetY(), 
							 GetSurfaceWidth(), GetSurfaceHeight(),
							 m_ImgEncBuffer,
							 RGB16_TRANS );
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				GP.PutXpr16A( BaseXPos + GetX(), BaseYPos + GetY(), 
							  GetSurfaceWidth(), GetSurfaceHeight(),
							  m_ImgEncBuffer,
							  GetAlphaValue(),
							  RGB16_TRANS );
			}
			break;
		}
	}

	if ( m_siSelStartIndex > -1 && m_siSelEndIndex > -1 )
	{	
		if ( m_siSelStartIndex != m_siSelEndIndex )
		{
			TSpr *pSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( m_strSelImageFileName );
			if ( pSpr )
			{
				if ( m_ptSelStart.y == m_ptSelEnd.y || m_bMultiline == false )			// 한줄 짜리
				{
					DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siSelImageFontIndex, m_siSelImageAlphaValue, BaseXPos + GetX() + m_ptSelStart.x, BaseYPos + GetY() , m_ptSelEnd.x - m_ptSelStart.x - ( ( m_ptSelEnd.x - m_ptSelStart.x ) % pSpr->GetXSize()  )  , m_siLineHeight, m_siSelImageTileNum );
				}
				else											// 멀티라인
				{
					DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siSelImageFontIndex, m_siSelImageAlphaValue, BaseXPos + GetX() + m_ptSelStart.x, BaseYPos + GetY() + m_ptSelStart.y, m_siAvailabeUseWidth - m_ptSelStart.x - ( ( m_siAvailabeUseWidth - m_ptSelStart.x ) % pSpr->GetXSize()  )  , m_siLineHeight, m_siSelImageTileNum );
					DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siSelImageFontIndex, m_siSelImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + m_ptSelStart.y + m_siLineHeight, m_siAvailabeUseWidth - ( m_siAvailabeUseWidth % pSpr->GetXSize() ), m_ptSelEnd.y - ( m_ptSelStart.y + m_siLineHeight ), m_siSelImageTileNum );
					DrawTileImg( pSpr, SPR_PUTSPRALPHAT, m_siSelImageFontIndex, m_siSelImageAlphaValue, BaseXPos + GetX(), BaseYPos + GetY() + m_ptSelEnd.y, m_ptSelEnd.x - ( m_ptSelEnd.x % pSpr->GetXSize()  )  , m_siLineHeight, m_siSelImageTileNum );
				}
			}
		}
	}

	if ( (! m_bReadOnly) && IsFocus() )
	{		
		//if ( ++m_siCaretCounter % 20 < 2 )
		//{	
		//	if ( m_siCaretCounter > 30000 )
		//	{
		//		m_siCaretCounter = 1;
		//	}

		TSpr *pCaretSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( CARET_IMAGE_FILENAME );

		if ( pCaretSpr &&
			m_siCaretXPos >= 0 && m_siCaretXPos < m_siAvailabeUseWidth &&
			m_siCaretYPos >= 0 && m_siCaretYPos < GetSurfaceHeight()
			)
		{
			GP.PutSpr(pCaretSpr, BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX, BaseYPos + GetY() + m_siCaretYPos, CARET_IMAGE_FONTINDEX );
		}
		//}
	}

	if ( m_pScroll->IsShow() && m_bShowScrollbar )
	{
		m_pScroll->DrawController( BaseXPos, BaseYPos, LeftTime );
	}

	if ( m_siCompStrLen > 0 && m_ImgCompEncBuffer && IsShow() )
	{
		TSpr *pCompSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( EDIT_COMPIMAGE_FILENAME );
		if ( pCompSpr )
		{
			DrawTileImg( pCompSpr, SPR_PUTSPRT, 
				EDIT_COMPIMAGE_FONTINDEX, 0,
				BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX,
				BaseYPos + GetY() + m_siCaretYPos,
				m_siCompStrWidth + ( pCompSpr->GetXSize() - ( m_siCompStrWidth % pCompSpr->GetXSize() ) ), m_siCompStrHeight,
				3, true );
		}

		GP.PutXpr16( BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX, BaseYPos + GetY() + m_siCaretYPos, 
			m_siCompStrWidth, m_siCompStrHeight,
			m_ImgCompEncBuffer,
			RGB16_TRANS );
	}

	if ( m_bReadiingWindowOpen && m_siReadingStrLen > 0 && m_ImgReadingEncBuffer && IsShow() )
	{
		TSpr *pReadingSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( EDIT_READINGWINDOWIMAGE_FILENAME );
		if ( pReadingSpr )
		{
			DrawTileImg( pReadingSpr, SPR_PUTSPRT, 
				EDIT_READINGWINDOWIMAGE_FONTINDEX, 0,
				BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX + m_siCompStrWidth,
				BaseYPos + GetY() + m_siCaretYPos,
				m_siReadingStrWidth + ( pReadingSpr->GetXSize() - ( m_siReadingStrWidth % pReadingSpr->GetXSize() ) ), m_siReadingStrHeight,
				3, true );
		}

		GP.PutXpr16( BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX + m_siCompStrWidth, BaseYPos + GetY() + m_siCaretYPos, 
			m_siReadingStrWidth, m_siReadingStrHeight,
			m_ImgReadingEncBuffer,
			RGB16_TRANS );
	}

	if ( IsShow() && m_bCandidateOpen )
	{
		pclclient->m_pInterfaceMgr->SetCandidateOpenStatus( true );
		if( pclClient->IsCountrySwitch(Switch_UseDefaultIME) )
		{
		}
		else
		{
			pclclient->m_pInterfaceMgr->SetCandidateDrawOption
				(
				BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX // + 10
				,BaseYPos + GetY()
				,m_siCompStrWidth
				,m_siCompStrHeight
				,m_siAttrLineStartPos
				); 
		}
	}
	else
	{
		pclclient->m_pInterfaceMgr->SetCandidateOpenStatus( false );
	}

	/*
	if ( IsShow() && m_bCandidateOpen && m_ImgCandidateEncBuffer )
	{
	cltTSpr *pCandidateWindowSpr = pclclient->m_pInterfaceMgr->GetInterfaceImgrMgr()->LoadInterfaceImg( EDIT_CANDIDATEWINDOWIMAGE_FILENAME );

	if ( pCandidateWindowSpr )
	{
	DrawTileImg( pCandidateWindowSpr, SPR_PUTSPRT, 
	EDIT_CANDIDATEWINDOWIMAGE_FONTINDEX, 0,
	BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX + m_siCompStrWidth + 10,
	BaseYPos + GetY() + m_siCaretYPos,
	m_siCandidateStrWidth + ( pCandidateWindowSpr->GetXSize() - ( m_siCandidateStrWidth % pCandidateWindowSpr->GetXSize() ) ), m_siCandidateStrHeight,
	3, true );
	}

	GP.PutXpr16( BaseXPos + GetX() + m_siCaretXPos + m_siOffsetX + m_siCompStrWidth + 10, BaseYPos + GetY() + m_siCaretYPos, 
	m_siCandidateStrWidth, m_siCandidateStrHeight,
	m_ImgCandidateEncBuffer,
	RGB16_TRANS );
	}
	*/
	if ( IsMouseOver() )
	{
		if ( m_bUseToolTip )
		{
			if ( m_strToolTipMsg )
			{
				pclclient->m_pInterfaceMgr->SetDrawToolTip( true );
			}
		}
	}

	return;
}

SI16 CEdit::ChangeText( HDC hDC, SI32 nPos )
{
	switch ( m_strText[ nPos + 1 ] )
	{
	case 'B':
		{
			switch( m_strText[ nPos + 2 ] )
			{
			case '0':
				SetFontWeight( FW_NORMAL );
				break;
			case '1':
				SetFontWeight( FW_SEMIBOLD );
				break;
			case '2':
				SetFontWeight( FW_BOLD );
				break;
			case '3':
				SetFontWeight( FW_EXTRABOLD );
				break;
			case '4':
				SetFontWeight( FW_HEAVY );
				break;
			default:
				OutputDebugString(  TEXT("Edit : No Parameter Error1\r\n") );
				break;
			}

			if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) ){
				m_hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, ((cltClient*)pclClient)->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, GetFontKind() );			
			}
			else {
				m_hFont = CreateFont( GetFontHeight(), GetFontWidth(), 0, 0, GetFontWeight(), 0, 0, 0, ((cltClient*)pclClient)->GetCharSet(), 0, 0, 0, 0, GetFontKind() );			
			}
			
			m_hOldFont = (HFONT)SelectObject( hDC, m_hFont );

			return EDIT_CHANGETEXTDRAW_CONDITION_UNINSTALL_TEXT_LEN;	
		}
		break;
	case 'C':
		{
			TCHAR strNumber[ 3 ] = TEXT("");
			strNumber[ 0 ] = m_strText[ nPos + 2 ];
			strNumber[ 1 ] = m_strText[ nPos + 3 ];
			strNumber[ 2 ] = NULL;

			int ColorIndex = _tstoi( strNumber );
			SetFontColor( GetChatEditColor( ColorIndex ) );
			SetTextColor( hDC, GetFontColor() );
		}
		break;
	case '/':
		{
			switch( m_strText[ nPos + 2 ] )
			{
			case 'B':
				SetFontWeight( FW_NORMAL );
				SelectObject( hDC, m_hOldFont );
				DeleteObject( m_hFont );
				break;
			case 'C':
				SetFontColor( GetChatEditColor( CHATEDIT_COLOR_BLACK ) );
				SetTextColor( hDC, GetFontColor() );
				break;
			default:
				OutputDebugString(  TEXT("Edit : No Command Error2\r\n") );
				break;
			}

			return EDIT_CHANGETEXTDRAW_CONDITION_UNINSTALL_TEXT_LEN;
		}
		break;
	default:
		{
			OutputDebugString(  TEXT("Edit : No Command Error1\r\n") );

			return 0;
		}
		break;
	}

	return EDIT_CHANGETEXTDRAW_CONDITION_INSTALL_TEXT_LEN;
}

void CEdit::LoadDLL()
{
	FARPROC Temp;

	if ( g_hIMEDll )
	{
		GETPROCADDRESS( g_hIMEDll, ImmLockIMC, Temp );
		GETPROCADDRESS( g_hIMEDll, ImmUnlockIMC, Temp );
		GETPROCADDRESS( g_hIMEDll, ImmLockIMCC, Temp );
		GETPROCADDRESS( g_hIMEDll, ImmUnlockIMCC, Temp );
		GETPROCADDRESS( g_hIMEDll, ImmGetContext, Temp );
		GETPROCADDRESS( g_hIMEDll, ImmReleaseContext, Temp );
		GETPROCADDRESS( g_hIMEDll, ImmGetIMEFileNameA, Temp );
	}

	if( g_hVersionIMEDll )
	{
		GETPROCADDRESS( g_hVersionIMEDll, VerQueryValueA, Temp );
		GETPROCADDRESS( g_hVersionIMEDll, GetFileVersionInfoA, Temp );
		GETPROCADDRESS( g_hVersionIMEDll, GetFileVersionInfoSizeA, Temp );
	}

	return;
}

void CEdit::CheckInputLocale()
{
	static HKL hklPrev = NULL;

	HKL hKeyboardLayout = GetKeyboardLayout( 0 );

	if ( hKeyboardLayout == NULL )
	{
		return;
	}

	if ( hklPrev == hKeyboardLayout )
	{
		return;
	}

	hklPrev = hKeyboardLayout;

	switch ( GetIMEPrimaryLanguage( hKeyboardLayout ) )
	{
		// Simplified Chinese
	case LANG_CHINESE:
		m_bVerticalCandidateWindow = true;
		switch ( GetIMESubLanguage( hKeyboardLayout ) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED:
			m_bVerticalCandidateWindow = GetImeID( 0 ) == 0 ? true : false;
			break;
		}
		break;
		// Korean
	case LANG_KOREAN:
		m_bVerticalCandidateWindow =  false;
		break;
		// Japanese
	case LANG_JAPANESE:
		m_bVerticalCandidateWindow = true;
		break;
	default:
		break;
	}

	return;
}

void CEdit::CheckToggleState()
{
	cltClient* pclclient = (cltClient*)pclClient;

	HKL hKeyboardLayout = GetKeyboardLayout( 0 );

	if ( hKeyboardLayout == NULL )
	{
		return;
	}

	CheckInputLocale();
	bool bIme = ImmIsIME( hKeyboardLayout ) != 0;
	bool ChineseIME = ( GetIMEPrimaryLanguage( hKeyboardLayout ) == LANG_CHINESE ) && bIme;

	HIMC hImc = _ImmGetContext( pclClient->GetHwnd() );

	if( NULL != hImc )
	{
		DWORD dwConvMode, dwSentMode ;
		//	IMESTATE s_ImeState;
		WORD Data = 0;


		if( ChineseIME )
		{
			ImmGetConversionStatus( hImc, &dwConvMode, &dwSentMode );
			//			s_ImeState = ( dwConvMode & IME_CMODE_NATIVE ) ? IME_CMODE_NATIVE : IMEUI_STATE_ENGLISH;
		}
		else
		{
			Data = GetIMESubLanguage( hKeyboardLayout );
			//s_ImeState = ( bIme && ImmGetOpenStatus( hImc ) != 0 ) ? IMEUI_STATE_ON : IMEUI_STATE_OFF;

			//pclclient->m_pInterfaceMgr->ImeToggleStateChanged(s_ImeState);
		}
		_ImmReleaseContext( pclClient->GetHwnd(), hImc );


	}
	else
	{
		//s_ImeState = IMEUI_STATE_OFF;
	}
}

DWORD CEdit::GetImeID( UINT Index )
{	// 
	DWORD   dwVerSize;
	DWORD   dwVerHandle;
	LPVOID  lpVerBuffer;
	LPVOID  lpVerData;
	UINT    cbVerData;
	char    szTmp[ 1024 ] = "";

	HKL hKeyboardLayout = GetKeyboardLayout( 0 );
	if ( hKeyboardLayout == NULL )
	{
		return 0;
	}

	// Check if we are using an older Chinese IME
	if( !( ( hKeyboardLayout == _CHT_HKL ) || ( hKeyboardLayout == _CHT_HKL2 ) || ( hKeyboardLayout == _CHS_HKL ) ) )
	{	
		return 0;
	}

	// Obtain the IME file name

	//CHAR* szDest;

	if ( ! _ImmGetIMEFileNameA( hKeyboardLayout, szTmp, sizeof(szTmp)-1 ) )
	{
		return 0;
	}

	char *szUpperTmp = _strupr( szTmp );
	//Unicode 를 MultyByte로 바꿔줌

	// [영진] _ImmGetIMEFileNameA 호출로 char 형식의 문자열을 반환 받으므로 궂이 유니코드로 바꾸지 않는다?


	if(	(strcmp( szUpperTmp, CHT_IMEFILENAME1 ) != 0) &&
		(strcmp( szUpperTmp, CHT_IMEFILENAME2 ) != 0) &&
		(strcmp( szUpperTmp, CHT_IMEFILENAME3 ) != 0) &&
		(strcmp( szUpperTmp, CHS_IMEFILENAME1 ) != 0) &&
		(strcmp( szUpperTmp, CHS_IMEFILENAME2 ) != 0) )
	{
		return 0;
	}

	DWORD	IMEID		= 0;
	DWORD	IMEVersionLS= 0;

	dwVerSize = _GetFileVersionInfoSizeA( szTmp, &dwVerHandle );
	if( dwVerSize )
	{
		lpVerBuffer = HeapAlloc( GetProcessHeap(), 0, dwVerSize );
		if( lpVerBuffer )
		{
			if( _GetFileVersionInfoA( szTmp, dwVerHandle, dwVerSize, lpVerBuffer ) )
			{
				if( _VerQueryValueA( lpVerBuffer, "\\", &lpVerData, &cbVerData ) )
				{
					DWORD dwVer = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionMS;
					dwVer = ( dwVer & 0x00ff0000 ) << 8 | ( dwVer & 0x000000ff ) << 16;
					if( ( GetIMELanguage(hKeyboardLayout) == LANG_CHT &&
						( dwVer == MAKEIMEVERSION(4, 2) || 
						dwVer == MAKEIMEVERSION(4, 3) || 
						dwVer == MAKEIMEVERSION(4, 4) || 
						dwVer == MAKEIMEVERSION(5, 0) ||
						dwVer == MAKEIMEVERSION(5, 1) ||
						dwVer == MAKEIMEVERSION(5, 2) ||
						dwVer == MAKEIMEVERSION(6, 0) ) )
						||
						( GetIMELanguage(hKeyboardLayout) == LANG_CHS &&
						( dwVer == MAKEIMEVERSION(4, 1) ||
						dwVer == MAKEIMEVERSION(4, 2) ||
						dwVer == MAKEIMEVERSION(5, 3) ) )
						)
					{
						IMEID = dwVer | GetIMELanguage(hKeyboardLayout);
						IMEVersionLS = ( (VS_FIXEDFILEINFO*)lpVerData )->dwFileVersionLS;
					}
				}
			}
			HeapFree( GetProcessHeap(), 0, lpVerBuffer );
		}
	}

	return IMEID;
}

void CEdit::GetReadingString( LPARAM lParam )
{	
	DWORD IMEID = GetImeID( 0 );
	if( IMEID == 0 )
	{
		m_bReadiingWindowOpen = false;
		return;
	}

	HIMC hImc;
	hImc = _ImmGetContext( pclClient->GetHwnd() );

	if( hImc == NULL )
	{
		m_bReadiingWindowOpen = false;
		return;
	}

	DWORD dwReadingStrLen = 0;
	DWORD dwErr = 0;
	WCHAR *TempStr = NULL;
	bool bUnicodeIme = false;  // Whether the IME context component is Unicode.

	INPUTCONTEXT *lpIC = _ImmLockIMC( hImc );

	if ( lpIC == NULL )
	{
		m_bReadiingWindowOpen = false;
		return;
	}

	HKL hKeyboardLayout = GetKeyboardLayout( 0 );
	if ( hKeyboardLayout == NULL )
	{
		m_bReadiingWindowOpen = false;
		return;
	}

	LPBYTE p = 0;

	switch( IMEID )
	{
	case IMEID_CHT_VER42: // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
	case IMEID_CHT_VER43: // New(Phonetic/ChanJie)IME98a : 4.3.x.x // WinMe, Win2k
	case IMEID_CHT_VER44: // New ChanJie IME98b          : 4.4.x.x // WinXP
		{
			p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 24 );
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)( p + 7 * 4 + 32 * 4 );
			dwErr = *(DWORD *)( p + 8 * 4 + 32 * 4 );
			TempStr = (WCHAR *)( p + 56 );
			bUnicodeIme = true;
		}
		break;

	case IMEID_CHT_VER50: // 5.0.x.x // WinME
		{
			p = *(LPBYTE *)( (LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 3 * 4 );
			if( !p ) break;
			p = *(LPBYTE *)( (LPBYTE)p + 1*4 + 5*4 + 4*2 );
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16);
			dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 + 1*4);
			TempStr = (WCHAR *)(p + 1*4 + (16*2+2*4) + 5*4);
			bUnicodeIme = false;
		}
		break;

	case IMEID_CHT_VER51: // 5.1.x.x // IME2002(w/OfficeXP)
	case IMEID_CHT_VER52: // 5.2.x.x // (w/whistler)
	case IMEID_CHS_VER53: // 5.3.x.x // SCIME2k or MSPY3 (w/OfficeXP and Whistler)
		{
			p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 4);
			if( !p ) break;
			p = *(LPBYTE *)((LPBYTE)p + 1*4 + 5*4);
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2);
			dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * 2 + 1*4);
			TempStr = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
			bUnicodeIme = true;
		}
		break;
		// the code tested only with Win 98 SE (MSPY 1.5/ ver 4.1.0.21)
	case IMEID_CHS_VER41:
		{
			int nOffset;
			nOffset = ( GetImeID( 1 ) >= 0x00000002 ) ? 8 : 7;

			p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + nOffset * 4);
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)(p + 7*4 + 16*2*4);
			dwErr = *(DWORD *)(p + 8*4 + 16*2*4);
			dwErr = __min( dwErr, dwReadingStrLen );
			TempStr = (WCHAR *)(p + 6*4 + 16*2*1);
			bUnicodeIme = true;
		}
		break;

	case IMEID_CHS_VER42: // 4.2.x.x // SCIME98 or MSPY2 (w/Office2k, Win2k, WinME, etc)
		{
			OSVERSIONINFOW osi;
			osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
			GetVersionExW( &osi );

			int nTcharSize = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? sizeof(WCHAR) : sizeof(TCHAR);
			p = *(LPBYTE *)((LPBYTE)_ImmLockIMCC( lpIC->hPrivate ) + 1*4 + 1*4 + 6*4);
			if( !p ) break;
			dwReadingStrLen = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize);
			dwErr = *(DWORD *)(p + 1*4 + (16*2+2*4) + 5*4 + 16 * nTcharSize + 1*4);
			TempStr = (WCHAR *) (p + 1*4 + (16*2+2*4) + 5*4);
			bUnicodeIme = ( osi.dwPlatformId == VER_PLATFORM_WIN32_NT ) ? true : false;
		}
		break;
	}

	if ( dwReadingStrLen == 0 )
	{
		ResetReadingString();
	}
	else
	{
		if( bUnicodeIme )
		{
			UINT i = 0;
			for( i = 0; i < dwReadingStrLen; ++i ) // dwlen > 0, if known IME
			{
				m_strReadingStr[ i ] = (TCHAR)TempStr[ i ];
				m_strReadingStr[ i + 1 ] = NULL;
			}
		}
		else
		{
			TCHAR *p = (TCHAR*)TempStr;
			DWORD i, j;
			WCHAR wstrReadingStr[ MAX_PATH ];

			for( i = 0, j = 0; i < dwReadingStrLen; ++i, ++j ) // dwlen > 0, if known IME
			{	
				TCHAR strCodePage[ MAX_PATH ];
				UINT uCodePage = CP_ACP;  // Default code page
				if( GetLocaleInfo( MAKELCID( GetIMELanguage( hKeyboardLayout ), SORT_DEFAULT ),
					LOCALE_IDEFAULTANSICODEPAGE,
					strCodePage,
					MAX_PATH ) )
				{
					uCodePage = _tcstoul( strCodePage, NULL, 0 );
				}

#ifndef _UNICODE
				MultiByteToWideChar( uCodePage, 0, p + i, IsDBCSLeadByteEx( uCodePage, p[i] ) ? 2 : 1,
					wstrReadingStr, 1 );
#else
				StringCchCopy(wstrReadingStr, 1, p + i);
#endif

				if( IsDBCSLeadByteEx( uCodePage, p[i] ) )
					++i;
			}

			wstrReadingStr[ j ] = NULL;
		}
	}

	_ImmUnlockIMCC( lpIC->hPrivate );
	_ImmUnlockIMC( hImc );

	if ( hImc )
	{
		_ImmReleaseContext( pclClient->GetHwnd(), hImc );
	}

	if ( dwReadingStrLen > 0 )
	{
		m_bReadiingWindowOpen = true;

		m_siReadingStrLen = dwReadingStrLen;
		MakeReadingStrImgEncBuffer();
	}

	return;
}

void CEdit::GetCandidateList( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	switch( wParam )
	{
	case IMN_OPENCANDIDATE:
	case IMN_CHANGECANDIDATE:
		{
			HIMC hIMC = ImmGetContext( hWnd );
			if ( hIMC )
			{
				CANDIDATEFORM Candidateform;
				Candidateform.dwIndex = 0;
				Candidateform.dwStyle = CFS_FORCE_POSITION;
				Candidateform.ptCurrentPos.x = -10000;
				Candidateform.ptCurrentPos.y = -10000;

				int ScreenWidth = GetSystemMetrics(SM_CXSIZEFRAME);
				int ScreenHeight = GetSystemMetrics(SM_CYSIZEFRAME);

				Candidateform.rcArea.left = -10000;
				Candidateform.rcArea.top = -10000;
				Candidateform.rcArea.right = ScreenWidth + 10000;
				Candidateform.rcArea.bottom = ScreenHeight + 10000;

				BOOL bRval = ImmSetCandidateWindow( hIMC, &Candidateform );


				DWORD dwBufLen =  ImmGetCandidateList( hIMC, 0, NULL, 0 );
				if ( dwBufLen == 0 )
				{
					break;
				}

				SAFE_DELETE_A( m_pCandidateList );



				m_pCandidateList = (LPCANDIDATELIST)new BYTE[ dwBufLen ];

				ImmGetCandidateList( hIMC, 0, m_pCandidateList, dwBufLen );

				m_dwCandidateDataListCount = m_pCandidateList->dwCount;

				if( ConstServiceArea_China == pclClient->siServiceArea ||
					ConstServiceArea_Taiwan == pclClient->siServiceArea ||
					ConstServiceArea_NHNChina == pclClient->siServiceArea )
				{
					if( 0 == m_pCandidateList->dwPageSize )
					{
						break;
					}

					SI16 CurrentPage =  m_pCandidateList->dwSelection / m_pCandidateList->dwPageSize;

					m_siCandidateDataStartIndex = CurrentPage * m_pCandidateList->dwPageSize;

					if( m_pCandidateList->dwPageStart != (DWORD)m_siCandidateDataStartIndex )
					{
						// 이녀석이 IME 관련해서는 꽤 만능의 일을 하더군요^^; 
						ImmNotifyIME( hIMC, NI_SETCANDIDATE_PAGESTART, 0, m_siCandidateDataStartIndex ); 
					}
				}
				else
				{
					SI16 CurrentPage =  m_pCandidateList->dwSelection / m_pCandidateList->dwPageSize;

					m_siCandidateDataStartIndex = CurrentPage * m_pCandidateList->dwPageSize;
				}

				if ( m_pCandidateList->dwCount > 0 )
				{
					SAFE_DELETE_A( m_pCandidateDataList );

					m_pCandidateDataList = new stCandidateData[ m_pCandidateList->dwCount ];


					for ( DWORD i = m_siCandidateDataStartIndex; i < m_siCandidateDataStartIndex + m_pCandidateList->dwPageSize; ++i )
					{
						if( i >= m_pCandidateList->dwCount )
							break;

						m_pCandidateDataList[ i ].Set( i - m_siCandidateDataStartIndex, (TCHAR*)m_pCandidateList + m_pCandidateList->dwOffset[ i ] );
					}
				}

				m_bCandidateOpen = true;

				MakeCandidateWindowImgEncBuffer();

				ImmReleaseContext( hWnd, hIMC );
			}
		}
		break;
	case IMN_CLOSECANDIDATE:
		{
			ResetCandiateWindow();
		}
		break;
	}
}

void CEdit::SetUseToolTip( TCHAR *ToolTipMsg )
{
	if ( ToolTipMsg == NULL )
	{
		m_bUseToolTip = false;
		return;
	}
	else
	{
		m_bUseToolTip = true;
		if ( m_strToolTipMsg == NULL )
		{
			m_strToolTipMsg = new TCHAR[ MAX_PATH ];
		}
		memset( m_strToolTipMsg, 0, MAX_PATH );
		_tcsncpy( m_strToolTipMsg, ToolTipMsg, MAX_PATH );
		m_strToolTipMsg[ MAX_PATH - 1 ] = NULL;
	}

	return;
}

void CEdit::Show( bool bShowOption )
{
	CControllerObj::Show( bShowOption );

	if ( IsShow() )
	{
		Refresh();
	}
	else
	{
		m_pScroll->Show( false );
	}

	return;
}