#ifndef _EDIT_H_
#define _EDIT_H_

#pragma comment(lib, "imm32.lib")			// include ime library

#include "../ControllerObj.h"
#include "Scroll.h"

#include <list>
using namespace std;

#define EDIT_INPUT_MODE_NUMBER								0x0001
#define EDIT_INPUT_MODE_PASSWORD							0x0002
#define EDIT_INPUT_MODE_CHANGETEXTDRAW						0x0004

#define EDIT_CHANGETEXTDRAW_CONDITION_INSTALL_TEXT_LEN		4
#define EDIT_CHANGETEXTDRAW_CONDITION_UNINSTALL_TEXT_LEN	3

struct stCandidateData
{
	TCHAR	strData[ MAX_PATH ];
	DWORD	dwDataLen;

	stCandidateData()
	{
		Init();
	}
	
	void Init()
	{
		memset( this, 0, sizeof ( stCandidateData ) );
	}
	
	void Set( int Index, TCHAR *pstrData )
	{
		if ( pstrData == NULL || Index < 0 || Index >= 9 )
		{
			return;
		}

		//sprintf( strData, TEXT("%d %s"), Index + 1, pstrData );
		//StringCchPrintf( strData, MAX_PATH, TEXT("%s"), pstrData );
		StringCchPrintf( strData, MAX_PATH, TEXT(" %d  %s"), Index + 1, pstrData );

		dwDataLen = _tcslen( strData );
	}

	TCHAR *GetData()
	{
		return strData;
	}
};

struct stLineInfo
{
	SI32 siLineStart;
	SI32 siLineEnd;

	stLineInfo()
	{
		Init();
	}

	void Init()
	{	
		siLineStart = -1;
		siLineEnd = -1;
		return;
	}
};

class CEdit : public CControllerObj
{
public:
	CEdit( CControllerMgr *pParent );
	void Clear();
	void ResetCompositionString();
	void ResetReadingString();
	void ResetCandiateWindow();
	void ClearText();
	~CEdit();

	void Create( SI16 ControllerID, TCHAR *Text, 
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum,
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, 
		SI16 SurfaceWidth, SI16 SurfaceHeight, 
		bool Multiline = false, bool ReadOnly = true, SI16 LineHeight = EDIT_LINEHEIGHT,
		SI32 MaxEditTextLimitLength = BASE_EDIT_CONTROLLER_LIMIT_TEXT_LENGTH,
		SI16 InputMode = 0,
		SI16 OffsetX = 3, SI16 OffsetY = 3,
		bool AutoScroll = false
		);

	void SetImeMode( HWND hWindow, bool AlphaNumeric );
	bool IsAlphaNumericMode( HWND hWindow );

	bool HandleKeyboard( UINT uMsg, WPARAM &wParam, LPARAM &lParam );
	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );	
	bool MsgProc( UINT uMsg, WPARAM &wParam, LPARAM &lParam );

	void SetFocus( bool HasFocus );

	void SetMaxEditTextLength( SI32 MaxEditTextLength )
	{
		if ( MaxEditTextLength < 1 )
		{
			return;
		}

		Clear();

		SAFE_DELETE_A( m_strText );

		m_siMaxEditTextLimitLength = MaxEditTextLength;

		m_strText = new TCHAR[ MaxEditTextLength ];
		memset( m_strText, 0, MaxEditTextLength );
	}

	void SetAutoScroll( bool AutoScroll )
	{
		m_bAutoScroll = AutoScroll;
		return;
	}

	void ShowScrollBar( bool bShow )
	{
        m_bShowScrollbar = bShow;
	}

	void SetPasswordMode( bool PasswordMode )
	{
		if ( PasswordMode )
		{
			m_siInputMode |= EDIT_INPUT_MODE_PASSWORD;
		}
		else
		{
			m_siInputMode -= EDIT_INPUT_MODE_PASSWORD;
		}
		return;
	}

	void SetNumberMode( bool NumberMode )
	{
		if ( NumberMode )
		{
			m_siInputMode |= EDIT_INPUT_MODE_NUMBER;
		}
		else
		{
			m_siInputMode -= EDIT_INPUT_MODE_NUMBER;
		}
		return;
	}

	void SetChangeTextDrawMode( bool ChangeTextDrawMode )
	{
		if ( ChangeTextDrawMode )
		{
			m_siInputMode |= EDIT_INPUT_MODE_CHANGETEXTDRAW;
		}
		else
		{
			m_siInputMode -= EDIT_INPUT_MODE_CHANGETEXTDRAW;
		}
		return;
	}

	void SetReadOnly( bool ReadOnly )
	{
		m_bReadOnly = ReadOnly;
		return;
	}

	void SetText( TCHAR *Text );
	void SetText( TCHAR *Text , COLORREF fontColor);

	TCHAR *GetText()
	{
		if ( m_strText )
		{
			return m_strText;
		}
		return NULL;
	}

	bool Insert( SI32 nPos, TCHAR *str );
	void Delete( SI32 nPos, SI16 nCount );
	bool DeleteSelection();
	void ClearSelection();
	void CloseCandidate();
	bool IsSelectionStatus();

	void SetTabKeyPerssNotifyUse( bool TabKeyPressNotifyUse )
	{
		m_bTabKeyPressNotify = TabKeyPressNotifyUse;
	}

	bool IsAvailableInsertStringToLine( HDC hDC, stLineInfo *pLineInfo, TCHAR *Text, int TextLen );
	void StringExamine( HDC hDC, TCHAR *Text, int TextLen );

	void MakeAllLineFromStartPos( HDC hDC, SI32 &StartPos );
	//SI16 MakeEachLineFromStartPos( HDC hDC, SI32 &StartPos, stLineInfo *pLineInfo, bool PushBackAvailable );
	SI16 MakeEachLineFromStartPos( HDC hDC, SI32 &StartPos );

	SI32 GetPosFromXY( SI16 XPos, SI16 YPos );
	bool GetXYFromPos( SI32 Pos, POINT &pt );
	SI32 GetLineIndexFromXY( SI16 XPos, SI16 YPos );
	SI16 GetLineIndexFromPos( SI16 Pos );
	bool IsMultiLine( HDC hDC, TCHAR *Text, int TextLen );
	SI32 GetPrevPos( SI32 Pos );
	SI16 GetYPos(){
		return m_siCaretYPos;
	}
	bool IsPrevWordDBCS( TCHAR *Text, SI16 nPos );
	SI32 GetNextPos( SI32 Pos );
	SI16 GetCharWidth( HDC hDC, TCHAR* OneChar, bool DBCS );
	SI16 GetLineWidth( HDC hDC, TCHAR *pText, SI16 TextLen );

	void UpdateLineInfo( SI32 StartPos );
	void UpdateCaretPos();

	void MakeTextImgEncBuffer();
	void MakeCompStrImgEncBuffer();
	void MakeReadingStrImgEncBuffer();
	void MakeCandidateWindowImgEncBuffer();
	void Refresh();

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime );

	void SetLineHeight( SI16 LineHeight )
	{
		m_siLineHeight = LineHeight;
	}
	SI16 GetLineHeight()
	{
		return m_siLineHeight;
	}

	SI16 ChangeText( HDC hDC, SI32 nPos );

	bool IsReadOnly()
	{
		return m_bReadOnly;
	}
	
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

	void LoadDLL();
	void CheckInputLocale();
	void CheckToggleState();

	DWORD GetImeID( UINT Index );

	void GetReadingString( LPARAM lParam );
	void GetCandidateList( HWND hWnd, WPARAM wParam, LPARAM lParam);


	void SetUseToolTip( TCHAR *ToolTipMsg );

	void Show( bool bShowOption );

private:
	TCHAR			*m_strText;
	
	TCHAR			m_strCompStr[ MAX_PATH ];
	SI16			m_siCompStrLen;

	RGB16			*m_ImgCompEncBuffer;
	SI16			m_siCompStrWidth;
	SI16			m_siCompStrHeight;

	TCHAR			m_strReadingStr[ MAX_PATH ];
	SI16			m_siReadingStrLen;
	
	bool			m_bReadiingWindowOpen;

	RGB16			*m_ImgReadingEncBuffer;
	SI16			m_siReadingStrWidth;
	SI16			m_siReadingStrHeight;

	LPCANDIDATELIST	m_pCandidateList;
	bool			m_bCandidateOpen;
	stCandidateData	*m_pCandidateDataList;
	DWORD			m_dwCandidateDataListCount;
	SI16			m_siCandidateDataStartIndex;

	TCHAR			m_strCandidateStr[ MAX_PATH ];
	SI16			m_siCandidateStrLen;
	
	bool			m_bVerticalCandidateWindow;

	RGB16*			m_ImgEncBuffer;
	list< stLineInfo > m_LineInfoList;

	bool			m_bMultiline;
	bool			m_bReadOnly;

	SI16			m_siInputMode;
	SI16			m_siPasswordPixelLen;
	SI16			m_siAsterikPixelLen;

	SI16			m_siLineHeight;

	SI32			m_siMaxEditTextLimitLength;
	SI32			m_siCurrentEditTextLength;

	SI16			m_siCaretCounter;
	SI32			m_siCurrentCaretPos;

	bool			m_bTabKeyPressNotify;

	CScroll			*m_pScroll;

	bool			m_bAutoScroll;

	SI16			m_siAvailabeUseWidth;
	SI16			m_siAvailabeUseHeight;

	SI16			m_siOffsetX;
	SI16			m_siOffsetY;

	bool			m_bDBCS;

	SI16			m_siCaretXPos;
	SI16			m_siCaretYPos;
	
	SI16			m_siSelStartIndex;
	SI16			m_siSelEndIndex;
	bool			m_bSelDrag;

	TCHAR			*m_strSelImageFileName;
	SI16			m_siSelImageFontIndex;
	bool			m_bSelImageTileImg;
	SI16			m_siSelImageTileNum;
	SI16			m_siSelImageAlphaValue;

	POINT			m_ptSelStart;
	POINT			m_ptSelEnd;

	SI32			m_siCompAttrStart;
	SI32			m_siCompAttrEnd;
	SI16			m_siAttrLineStartPos; 

	bool			m_bShowScrollbar;
	
	HFONT			m_hFont;
	HFONT			m_hOldFont;

	bool			m_bUseToolTip;
	TCHAR			*m_strToolTipMsg;


};

#endif
