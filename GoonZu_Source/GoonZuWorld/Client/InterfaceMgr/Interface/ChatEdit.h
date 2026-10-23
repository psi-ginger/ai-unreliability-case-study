#ifndef _CHATEDIT_H_
#define _CHATEDIT_H_

#include "../ControllerObj.h"
#include "Scroll.h"

#include <deque>
#include <vector>
using namespace std;

struct stChatEditData
{
	SI16 siChatEditDataSize;

	SI16 siChatKind;
	
	SI16 siParam;
	
	TCHAR strNameData[ CHATEDIT_NAMEDATA_LENGTH  ];
	TCHAR strChatData[ CHATEDIT_CHATDATA_LENGTH ];

	/*
	채팅 타입에 따라서 Color 가 결정되기 때문에 여기서 Color 값을 클라이언트만 가지고 있으면 됨.
	서버에서는 ChatData에서 대부분 데이터가 잘림. 따라서 뒤에 컬러값이 오지 않는다고 봐야 함.
	*/
	SI16 siNameDataColorIndex;
	SI16 siChatDataColorIndex;

	// LineCount 변수는 ChatEdit 클래스의 Line 계산 연산을 없애기 위해서 둔 것임
	SI16 siLineCount;

	stChatEditData()
	{
		Init();
	}

	stChatEditData( stChatEditData *pChatEditData )
	{
		Set( pChatEditData );
	}

	void Init()
	{
		memset( this, 0, sizeof( stChatEditData ) );
		
		siNameDataColorIndex = CHATEDIT_COLOR_BLUE;
		siChatDataColorIndex = CHATEDIT_COLOR_BLACK;
	}

	void Set( stChatEditData *pChatEditData )
	{
		memcpy( this, pChatEditData, sizeof( stChatEditData ) );
	}
};

struct stEmoticonData
{
	SI16	siXpos;
	SI16	siYpos;
	SI32	siFontIndex;
};

class CChatEdit : public CControllerObj  
{
public:
	CChatEdit( CControllerMgr *pParent );
	~CChatEdit();

	void Create( SI16 ControllerID, TCHAR *Text,
		TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileImgNum, 
		SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 SurfaceWidth, SI16 SurfaceHeight,
		SI16 LineHeight = CHATEDIT_LINEHEIGHT, bool AutoScroll = true,
		SI16 OffsetX = 5, SI16 OffsetY = 5,
		SI16 ScrollWidth = -1, SI16 ScrollHeight = -1,
		UI16 ScrollButtonWidth = 0, UI16 ScrollButtonHeight = 0
		);

	bool HandleMouse( UINT uMsg, POINT pt, WPARAM &wParam, LPARAM &lParam );
	
	void Insert( stChatEditData *pChatEditData );

	void MakeTextImgEncBuffer();

	SI16 DrawNameData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData );
	void DrawOneLineChatData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData, bool* bEmoticon );
	void DrawMultiLineChatData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData, TCHAR *pDrawStartChatData, SI16 DrawChatEditDataLen );
	void DrawOneLineData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData, bool* bEmoticon );
	void DrawMultiLineData( HDC hDC, SI16 DrawXPos, SI16 DrawYPos, stChatEditData *pChatEditData );
    
	void Clear();
	void Refresh();
	void RefreshScroll();

	void Show( bool ShowOption );

	void DrawController( SI16 BaseXPos, SI16 BaseYPos, SI32 LeftTime = 0 );
	
	void SetMaxChatEditDataNum( SI16 MaxChatEditDataNum )
	{
		m_siMaxChatEditDataNum = MaxChatEditDataNum;
	}

	SI16 LineExamine( HDC hDC, TCHAR *Text, int TextLen, TCHAR **pEachLineEndPtr );		// return value : Line Counter
	bool IsMultiLine( HDC hDC, TCHAR *Text, int TextLen );
	bool IsPrevWordDBCS( TCHAR *Text, SI16 nPos );
	SI16 GetCharWidth( HDC hDC, TCHAR *OneChar, bool DBCS );

	void SetScrollBKFile( TCHAR *FileName, SI16 FontIndex )
	{
		m_pScroll->SetScrollBKFile( FileName, FontIndex );
	}

	void SetScrollBKFileFontIndex( SI16 FontIndex )
	{
		m_pScroll->SetScrollBKFileFontIndex( FontIndex );
	}

	void SetScrollThumbFile( TCHAR *FileName, SI16 FontIndex )
	{
		m_pScroll->SetScrollThumbFile( FileName, FontIndex );
	}

	void SetScrollThumbFileFontIndex( SI16 FontIndex )
	{
		m_pScroll->SetScrollThumbFileFontIndex( FontIndex );
	}

	void SetScrollUpButtonFile( TCHAR *FileName, SI16 FontIndex )
	{
		m_pScroll->SetScrollUpButtonFile( FileName, FontIndex );
	}

	void SetScrollUpButtonFileFontIndex( SI16 FontIndex )
	{
		m_pScroll->SetScrollUpButtonFileFontIndex( FontIndex );
	}

	void SetScrollDownButtonFile( TCHAR *FileName, SI16 FontIndex )
	{
		m_pScroll->SetScrollDownButtonFile( FileName, FontIndex );
	}

	void SetScrollDownButtonFileFontIndex( SI16 FontIndex )
	{
		m_pScroll->SetScrollDownButtonFileFontIndex( FontIndex );
	}

	void SetShowChatDataFilter(SI32 siFilter)
	{
		siShowChatDataFilter = siFilter;
	}

private:
	RGB16*	m_ImgEncBuffer;

	UI16 m_siMaxChatEditDataNum;			// 채팅 개수

	SI16 m_siLineHeight;

	deque< stChatEditData > m_ChatEditDeque;

	CScroll *m_pScroll;

	bool m_bAutoScroll;

	SI16 m_siOffsetX;
	SI16 m_siOffsetY;

	SI16 m_siAvailabeUseWidth;

	bool m_bShowScroll;

	vector<stEmoticonData>	m_EmoticonData;
	TSpr*					m_pEmoticonSpr;

	SI32 siShowChatDataFilter;
};

#endif