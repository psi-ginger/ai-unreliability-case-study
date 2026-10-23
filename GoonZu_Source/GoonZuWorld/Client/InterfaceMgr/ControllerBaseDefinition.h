#ifndef _CONTROLLERBASEDEFINITION_H_
#define _CONTROLLERBASEDEFINITION_H_



// ::new 라는게 사용되는데, 이것을 사용하기 위해서는 iostream 이 필요하다.
// ::new 예제는 MSDN에 'operator new[]'로 검색하면 나온다.
// 설명 및 예제가 잘 되어 있다.
// ::new 라고 안 쓰고 그냥 new 라고 써도 무방하다.
// DX 예제에서 그렇게 썼기에, 더 안전할듯 싶어 따라쓴다.
#include <iostream>
#include <directives.h>
#include <Graphic.h>
#include <Graphic_Capsule.h>
#include <tchar.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)		{ if(p) { delete   (p);     (p)=NULL; } }
#define NK_SAFE_DELETE(p)	{ if(p) { NkDelete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_A
#define SAFE_DELETE_A(p)	{ if(p) { delete[] (p);     (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

//[진성] 배열에 사용할 인덱스 검사.(배열에서 인덱스의 범위를 확인. 배열을 가리키는 포인터는 사용불가능.)
#define SAFE_ARRAY_CHECK(Index, Array)		((0 <= Index) && (sizeof(Array) / sizeof(Array[0])) > Index) ? true : false

//[진성] 배열에 사용할 인덱스 검사.(최대 배열수로 인덱스의 범위를 확인. 배열을 가리키는 포인터는 이걸루 사용.)
#define SAFE_PARRAY_CHECK(Index, Num)		((0 <= Index) && (Num > Index)) ? true : false;

//-----------------------------------------------------------------------------------------------------------------------
// Defines and macros	-	군주
//-----------------------------------------------------------------------------------------------------------------------
#ifdef GOONZU

#define MAX_INTERFACEIMGOBJ_NUM						256
#define MAX_INTERFACEIMGOBJ_AVAILABLE_SECOND		120			// 120초 = 2분간 사용하지 않으면 지워버림

// Dialog 에서 TitleBar 이미지의 높이는 20 pixel 로 고정
#define DIALOG_TITLEBAR_SURFACE_HEIGHT				20
#define DIALOG_FONT_COLOR							RGB(0, 0, 0)
#define DIALOG_TITLEFONT_COLOR						RGB(50, 100, 135)
#define BUTTON_SELECTFONT_COLOR						RGB(50, 60, 85)	// 선택 됐을때
#define BUTTON_NORMALFONT_COLOR						RGB(140, 150, 180) // 일반
#define BUTTON_DISABLEFONT_COLOR					RGB(155, 155, 155) // 비활성

#define ITEM_IMAGE_SIZE_WIDTH						40
#define ITEM_IMAGE_SIZE_HEIGHT						40

#define MAX_FONTKIND_NAME_LEN						32

#if defined(_ENGLISH)
	#define DEFAULT_FONT_KIND							TEXT("Tahoma")
#elif defined(_USA)
	#define DEFAULT_FONT_KIND							TEXT("Tahoma")
#elif defined(_EUROPE)
	#define DEFAULT_FONT_KIND							TEXT("Tahoma")
#else
	#define DEFAULT_FONT_KIND							TEXT("MS UI Gothic")
#endif

// Dialog 개수
#define MAX_CONTROLLER_MGR_NUM						128

#define MAX_DELETE_DIALOG_NUM						128

// 하나의 Dialog 내에 들어가는 Controller 개수
#define MAX_CONTROLLER_OBJ_NUM						64

#define BASE_EDIT_CONTROLLER_LIMIT_TEXT_LENGTH		256

#define MAX_COMBOBOXITEM_TEXT_LEN					256

#if defined (_ENGLISH)
	#define BASE_FONT_HEIGHT							13
#elif defined (_USA)
	#define BASE_FONT_HEIGHT							13
#elif defined (_EUROPE)
	#define BASE_FONT_HEIGHT							13
#else
	#define BASE_FONT_HEIGHT							12
#endif


#define BASE_BORDER_WIDTH							5

#define BASE_SPACING								4

#define BASE_ALPHA_VALUE							12

#define MAX_TOOLTIP_XSIZE							260
#define MAX_TOOLTIP_YSIZE							500
#define MAX_TOOLTIP_MSG_SIZE						512				// 툴팁 길이 제한


//------------------------------------------------------------
// 현재 ime 입력 모드를 나타내는 그림 파일
#define INTERFACE_MGR_IMESTATE_FILENAME				TEXT("NInterface/Common/Common_18x18_00_000.Spr")



//------------------------------------------------------------
// Button
#define BUTTON_TILE_IMAGE_NUM						3
#define BUTTON_ANIMATION_NUM						3
//------------------------------------------------------------

//------------------------------------------------------------
// Scroll
#define SCROLL_WIDTH								18
#define SCROLL_HEIGHT								18
#define SCROLL_BUTTON_WIDTH							16
#define SCROLL_BUTTON_HEIGHT						15

#define MIN_SCROLL_THUMB_SIZE						12
//------------------------------------------------------------

//------------------------------------------------------------
// ListBox
#define MAX_LISTBOX_ITEM_NUM						512

#define LISTBOX_ITEM_XSIZE							12
#define LISTBOX_ITEM_YSIZE							12
#define LISTBOX_LINE_HEIGHT							14

#define LISTBOX_BOLD_COLOR							COLOR_BLACK
//------------------------------------------------------------

//------------------------------------------------------------
// ComboBox
#define MAX_COMBOBOX_ITEM_NUM						255

#define COMBOBOX_LINE_HEIGHT						14

#define COMBOBOX_FOCUSIMAGE_FILENAME				TEXT("NInterface/Common/Common_4X14_00_103.Spr")
#define COMBOBOX_FOCUSIMAGE_ALPHA_VALUE				8

//------------------------------------------------------------


//------------------------------------------------------------
// CheckBox, RadioButton
#define CHECKBOX_SIZE								12
#define RADIOBUTTON_SIZE							12
//------------------------------------------------------------


//------------------------------------------------------------
// EditBox
#define EDIT_LINEHEIGHT								14

#define EDIT_SELIMAGE_FILENAME						TEXT("NInterface/Common/Common_4X14_00_103.Spr")
#define EDIT_SELIMAGE_ALPHA_VALUE					8

#define EDIT_COMPIMAGE_FILENAME						TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define EDIT_COMPIMAGE_FONTINDEX					0

#define EDIT_COMPATTRIMAGE_FILENAME					TEXT("NInterface/Common/Common_2X14_00_000.Spr")
#define EDIT_COMPATTRIMAGE_FONTINDEX				1


#define EDIT_READINGWINDOWIMAGE_FILENAME			TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define EDIT_READINGWINDOWIMAGE_FONTINDEX			18


/*
#define EDIT_CANDIDATEWINDOWIMAGE_FILENAME			TEXT("NInterface/Common/Common_16X15_00_000.Spr")
#define EDIT_CANDIDATEWINDOWIMAGE_FONTINDEX			0

#define EDIT_CANDIDATESELECTIMAGE_FILENAME			TEXT("NInterface/Common/Common_4X14_00_000.SPR")
#define EDIT_CANDIDATESELECTIMAGE_FONTINDEX			1
*/ // 이미지 파일 그리는 대신 API 로 직접 그려줌

//------------------------------------------------------------

//-------------------------------------------------------------------------------------
// CandidateWindow  ( ime 후보단어 선택창 ex) 카타가나-> 한자 변환, 한글 -> 한자 변환 )
#define EDIT_CANDIDATE_FONTCOLOR RGB(0,0,0)
//#define EDIT_CANDIDATE_NUMBER_BKCOLOR RGB(50,50,50)
#define EDIT_CANDIDATE_SELECTED_FONTCOLOR RGB(240,240,240)
#define EDIT_CANDIDATE_SELECTED_BKCOLOR RGB(0,0,0)
#define EDIT_CANDIDATE_RIGHT_SPACE 15 // 오른쪽 여백의 크기
#define EDIT_CANDIDATE_TOP_SPACE 3 // 윗쪽 여백의 크기
#define EDIT_CANDIDATE_PAGENUM_DRAW_SPACE	30
//-------------------------------------------------------------------------------------


//------------------------------------------------------------
// Icon Image File ( Small Icon : 12X12 )
#define SMALL_ICON_SIZE								12
#define SMALL_ICON_IMG_FILE_0						TEXT("NInterface/Image/Image_12X12_00_000.Spr")
#define SMALL_ICON_IMG_FILE_1						TEXT("NInterface/Image/Image_12X12_01_000.Spr")
#define SMALL_ICON_IMG_FILE_2						TEXT("NInterface/Image/Image_16X16_00_000.SPR")
#define SMALL_ICON_IMG_FILE_3						TEXT("NInterface/Button/Button_20x20_00_000.SPR")
#define SMALL_ICON_IMG_FILE_4						TEXT("NInterface/Image/Image_20X22_00_000.SPR")

//------------------------------------------------------------


// SPI_GETWHEELSCROLLLINES 이것은 winuser.h 에 있는 것인데, 이상하게 include 가 안되서
// 그냥 이렇게 사용했음. MSDN 예제에서도 이렇게 사용했길래, 따라한 것임.. 
// 운영체제가 Win98 이상이라면 문제는 없음.
#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES						0x0068
#endif

#define COMMON_DIALOG_CLOSEBUTTON_FILENAME			TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define COMMON_DIALOG_CLOSEBUTTON_FONTINDEX			56
#define COMMON_CLOSE_BUTTON_OFFSETX					20
#define COMMON_CLOSE_BUTTON_OFFSETY					2
#define COMMON_CLOSE_BUTTON_WIDTH					16
#define COMMON_CLOSE_BUTTON_HEIGHT					15

/*
// 최소화 버튼 정의 [영진]
#define COMMON_DIALOG_MINIMIZEBUTTON_FILENAME			TEXT("Ninterface/Button/Button_16X15_00_000.Spr")
#define COMMON_DIALOG_MINIMIZEBUTTON_FONTINDEX		52
#define COMMON_MINIMIZE_BUTTON_OFFSETX				39
#define COMMON_MINIMIZE_BUTTON_OFFSETY				2
#define COMMON_MINIMIZE_BUTTON_WIDTH					16
#define COMMON_MINIMIZE_BUTTON_HEIGHT					15
*/





#define CARET_IMAGE_FILENAME						TEXT("NInterface/Common/Common_4X14_00_000.SPR")
#define CARET_IMAGE_FONTINDEX						0


#define COMBOBOX_OPEN_BACKGROUND_FILENAME			TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define COMBOBOX_OPEN_BACKGROUND_FONTINDEX			9

#define TOOLTIP_BACKGROUND_FILENAME					TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define TOOLTIP_BACKGROUND_FONTINDEX				0

#define COMBOBOX_DROPDOWN_BUTTON_FILENAME			TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define COMBOBOX_DROPDOWN_BUTTON_FONTINDEX			4

#define LISTBOX_FOCUSIMAGE_FILENAME					TEXT("NInterface/Common/Common_4X14_00_103.Spr")
#define LISTBOX_FOCUSIMAGE_FONTINDEX				3

#define LISTVIEW_ITEMNUMBER_FILENAME				TEXT("NInterface/Common/Common_8X12_00_000.Spr")
#define LISTVIEW_ITEMNUMBER_FONTINDEX				0

#define LISTVIEW_ACCELERATOR_FILENAME				TEXT("NInterface/Common/Common_10X7_00_000.Spr")
#define LISTVIEW_ACCELERATOR_FONTINDEX				0


#define SCROLL_BACKGROUND_FILENAME					TEXT("NInterface/Scroll/Scroll_18X18_00_203.Spr")
#define SCROLL_BACKGROUND_FONTINDEX					0
#define SCROLL_UPBUTTON_FILENAME					TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define SCROLL_UPBUTTON_FONTINDEX					0
#define SCROLL_DOWNBUTTON_FILENAME					TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define SCROLL_DOWNBUTTON_FONTINDEX					4
#define SCROLL_THUMB_FILENAME						TEXT("NInterface/Scroll/Scroll_16X4_00_203.Spr")
#define SCROLL_THUMB_FONTINDEX						0

#define HSCROLL_BACKGROUND_FILENAME					TEXT("NInterface/Scroll/Scroll_18X18_00_203.Spr")
#define HSCROLL_BACKGROUND_FONTINDEX				6
#define HSCROLL_LEFTBUTTON_FILENAME					TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define HSCROLL_LEFTBUTTON_FONTINDEX				36
#define HSCROLL_RIGHTBUTTON_FILENAME				TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define HSCROLL_RIGHTBUTTON_FONTINDEX				40
#define HSCROLL_THUMB_FILENAME						TEXT("NInterface/Scroll/Scroll_6x16_00_203.SPR")
#define HSCROLL_THUMB_FONTINDEX						0


#define CHECKBOX_BUTTON_FILENAME					TEXT("NInterface/Common/Common_12X12_00_000.Spr")
#define CHECKBOX_BUTTON_FONTINDEX					7


#define RADIOBUTTON_BUTTON_FILENAME					TEXT("NInterface/Common/Common_12X12_00_000.Spr")
#define RADIOBUTTON_BUTTON_FONTINDEX				9

#define MAX_CHATEDIT_DATA_NUM						512
#define CHATEDIT_NAMEDATA_LENGTH					30			// 이름앞에 [마을]또는[상단]또는[파티]가 붙여서
#define CHATEDIT_CHATDATA_LENGTH					256
#define CHATEDIT_LINEHEIGHT							14
#define CHATEDIT_MAX_EACHLINE_LINENUMBER			64			// 채팅 하나에 64줄 이상이 될수는 없음. ( 장문 채팅 또는 한글자치고 엔터치고 한글자치고 엔터 치고 계속 하는 경우가 이에 속함 )


#endif
//-----------------------------------------------------------------------------------------------------------------------
// 군주 정의 끝
//-----------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------
// Defines and macros	-	임진왜란
//-----------------------------------------------------------------------------------------------------------------------
#ifdef THREEKINGDOMS

#define MAX_INTERFACEIMGOBJ_NUM						256
#define MAX_INTERFACEIMGOBJ_AVAILABLE_SECOND		120			// 120초 = 2분간 사용하지 않으면 지워버림

// Dialog 에서 TitleBar 이미지의 높이는 20 pixel 로 고정
#define DIALOG_TITLEBAR_SURFACE_HEIGHT				20

#define ITEM_IMAGE_SIZE_WIDTH						40
#define ITEM_IMAGE_SIZE_HEIGHT						40

#define MAX_FONTKIND_NAME_LEN						32

#if defined(_ENGLISH)
	#define DEFAULT_FONT_KIND							TEXT("Tahoma")
#elif defined(_USA)
	#define DEFAULT_FONT_KIND							TEXT("Tahoma")
#elif defined(_EUROPE)
	#define DEFAULT_FONT_KIND							TEXT("Tahoma")
#else
	#define DEFAULT_FONT_KIND							TEXT("MS UI Gothic")
#endif

// Dialog 개수
#define MAX_CONTROLLER_MGR_NUM						128

#define MAX_DELETE_DIALOG_NUM						128

// 하나의 Dialog 내에 들어가는 Controller 개수
#define MAX_CONTROLLER_OBJ_NUM						64

#define BASE_EDIT_CONTROLLER_LIMIT_TEXT_LENGTH		256

#define MAX_COMBOBOXITEM_TEXT_LEN					256

#if defined (_ENGLISH)
	#define BASE_FONT_HEIGHT							13
#elif defined (_USA)
	#define BASE_FONT_HEIGHT							13
#elif defined (_EUROPE)
	#define BASE_FONT_HEIGHT							13
#else
	#define BASE_FONT_HEIGHT							12
#endif


#define BASE_BORDER_WIDTH							5

#define BASE_SPACING								4

#define BASE_ALPHA_VALUE							12

#define MAX_TOOLTIP_XSIZE							200
#define MAX_TOOLTIP_YSIZE							300
#define MAX_TOOLTIP_MSG_SIZE						384				// 툴팁 길이 제한


//------------------------------------------------------------
// Button
#define BUTTON_TILE_IMAGE_NUM						3
#define BUTTON_ANIMATION_NUM						3
//------------------------------------------------------------

//------------------------------------------------------------
// Scroll
#define SCROLL_WIDTH								18
#define SCROLL_HEIGHT								18
#define SCROLL_BUTTON_WIDTH							16
#define SCROLL_BUTTON_HEIGHT						15

#define MIN_SCROLL_THUMB_SIZE						12
//------------------------------------------------------------

//------------------------------------------------------------
// ListBox
#define MAX_LISTBOX_ITEM_NUM						1200

#define LISTBOX_ITEM_XSIZE							12
#define LISTBOX_ITEM_YSIZE							12
#define LISTBOX_LINE_HEIGHT							14

#define LISTBOX_BOLD_COLOR							COLOR_BLACK
//------------------------------------------------------------

//------------------------------------------------------------
// ComboBox
#define MAX_COMBOBOX_ITEM_NUM						128

#define COMBOBOX_LINE_HEIGHT						14

#define COMBOBOX_FOCUSIMAGE_FILENAME				TEXT("NInterface/Common/Common_4X14_00_103.Spr")
#define COMBOBOX_FOCUSIMAGE_ALPHA_VALUE				8
//------------------------------------------------------------


//------------------------------------------------------------
// CheckBox, RadioButton
#define CHECKBOX_SIZE								12
#define RADIOBUTTON_SIZE							12
//------------------------------------------------------------


//------------------------------------------------------------
// EditBox
#define EDIT_LINEHEIGHT								14

#define EDIT_SELIMAGE_FILENAME						TEXT("NInterface/Common/Common_4X14_00_103.Spr")
#define EDIT_SELIMAGE_ALPHA_VALUE					8

#define EDIT_COMPIMAGE_FILENAME						TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define EDIT_COMPIMAGE_FONTINDEX					0

#define EDIT_COMPATTRIMAGE_FILENAME					TEXT("NInterface/Common/Common_2X14_00_000.Spr")
#define EDIT_COMPATTRIMAGE_FONTINDEX				1

#define EDIT_READINGWINDOWIMAGE_FILENAME			TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define EDIT_READINGWINDOWIMAGE_FONTINDEX			18

#define EDIT_CANDIDATEWINDOWIMAGE_FILENAME			TEXT("NInterface/Common/Common_16X15_00_000.Spr")
#define EDIT_CANDIDATEWINDOWIMAGE_FONTINDEX			0

#define EDIT_CANDIDATESELECTIMAGE_FILENAME			TEXT("NInterface/Common/Common_4X14_00_000.SPR")
#define EDIT_CANDIDATESELECTIMAGE_FONTINDEX			1
//------------------------------------------------------------


/*
//------------------------------------------------------------
// Icon Image File ( Small Icon : 12X12 )
#define SMALL_ICON_SIZE								12
#define SMALL_ICON_IMG_FILE_0						TEXT("NInterface/Image/Image_100X108_00_000.Spr")
#define SMALL_ICON_IMG_FILE_1						TEXT("NInterface/Image/Image_12X12_01_000.Spr")
#define SMALL_ICON_IMG_FILE_2						TEXT("NInterface/Image/Image_16X16_00_000.SPR")
#define SMALL_ICON_IMG_FILE_3						TEXT("NInterface/Button/Button_20x20_00_000.SPR")
//------------------------------------------------------------
*/

// SPI_GETWHEELSCROLLLINES 이것은 winuser.h 에 있는 것인데, 이상하게 include 가 안되서
// 그냥 이렇게 사용했음. MSDN 예제에서도 이렇게 사용했길래, 따라한 것임.. 
// 운영체제가 Win98 이상이라면 문제는 없음.
#ifndef SPI_GETWHEELSCROLLLINES
#define SPI_GETWHEELSCROLLLINES						0x0068
#endif


#define COMMON_DIALOG_CLOSEBUTTON_FILENAME			TEXT("NInterface/Button/Button_28X28_00_000.Spr")
#define COMMON_DIALOG_CLOSEBUTTON_FONTINDEX			0
#define COMMON_CLOSE_BUTTON_OFFSETX					28
#define COMMON_CLOSE_BUTTON_OFFSETY					0
#define COMMON_CLOSE_BUTTON_WIDTH					28
#define COMMON_CLOSE_BUTTON_HEIGHT					28


#define CARET_IMAGE_FILENAME						TEXT("NInterface/Common/Common_4X14_00_000.SPR")
#define CARET_IMAGE_FONTINDEX						0


#define COMBOBOX_OPEN_BACKGROUND_FILENAME			TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define COMBOBOX_OPEN_BACKGROUND_FONTINDEX			9

#define TOOLTIP_BACKGROUND_FILENAME					TEXT("NInterface/Common/Common_16X15_00_109.Spr")
#define TOOLTIP_BACKGROUND_FONTINDEX				0

#define COMBOBOX_DROPDOWN_BUTTON_FILENAME			TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define COMBOBOX_DROPDOWN_BUTTON_FONTINDEX			4

#define LISTBOX_FOCUSIMAGE_FILENAME					TEXT("NInterface/Common/Common_4X14_00_103.Spr")
#define LISTBOX_FOCUSIMAGE_FONTINDEX				3

#define LISTVIEW_ITEMNUMBER_FILENAME				TEXT("NInterface/Common/Common_8X12_00_000.Spr")
#define LISTVIEW_ITEMNUMBER_FONTINDEX				0

#define LISTVIEW_ACCELERATOR_FILENAME				TEXT("NInterface/Common/Common_18X16_00_000.Spr")
#define LISTVIEW_ACCELERATOR_FONTINDEX				0


#define SCROLL_BACKGROUND_FILENAME					TEXT("NInterface/Scroll/Scroll_18X18_00_203.Spr")
#define SCROLL_BACKGROUND_FONTINDEX					0
#define SCROLL_UPBUTTON_FILENAME					TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define SCROLL_UPBUTTON_FONTINDEX					0
#define SCROLL_DOWNBUTTON_FILENAME					TEXT("NInterface/Button/Button_16X15_00_000.Spr")
#define SCROLL_DOWNBUTTON_FONTINDEX					4
#define SCROLL_THUMB_FILENAME						TEXT("NInterface/Scroll/Scroll_16X4_00_203.Spr")
#define SCROLL_THUMB_FONTINDEX						0

#define CHECKBOX_BUTTON_FILENAME					TEXT("NInterface/Common/Common_12X12_00_000.Spr")
#define CHECKBOX_BUTTON_FONTINDEX					7

#define RADIOBUTTON_BUTTON_FILENAME					TEXT("NInterface/Common/Common_12X12_00_000.Spr")
#define RADIOBUTTON_BUTTON_FONTINDEX				9

#define MAX_CHATEDIT_DATA_NUM						512
#define CHATEDIT_NAMEDATA_LENGTH					20
#define CHATEDIT_CHATDATA_LENGTH					128
#define CHATEDIT_LINEHEIGHT							14
#define CHATEDIT_MAX_EACHLINE_LINENUMBER			64			// 채팅 하나에 64줄 이상이 될수는 없음. ( 장문 채팅 또는 한글자치고 엔터치고 한글자치고 엔터 치고 계속 하는 경우가 이에 속함 )


#endif
//-----------------------------------------------------------------------------------------------------------------------
// 임진왜란 정의 끝
//-----------------------------------------------------------------------------------------------------------------------

#define CONTROLLER_BACKGROUND_COLOR					RGB( 250, 250, 255 )
#define CONTROLLER_BORDER_COLOR						RGB( 115, 130, 175 )
#define CONTROLLER_OUTLINE_COLOR					115,130,175
#define CONTROLLER_BUTTONTEXT_COLOR					RGB( 50, 60, 85 )
#define CONTROLLER_BACK_BUTTONTEXT_COLOR			RGB( 140, 150, 180 )
#define CONTROLLER_DISABLE_BUTTONTEXT_COLOR			RGB( 160, 160, 160 )
#define CONTROLLER_TITLE_COLOR						RGB(80,100,135)


enum
{
	MSGBOX_TYPE_OK,
	MSGBOX_TYPE_YESNO,
	MSGBOX_TYPE_OKCANCEL,
};


#define CHATEDIT_COLOR_BLACK						1
#define CHATEDIT_COLOR_WHITE						2
#define CHATEDIT_COLOR_RED							3
#define CHATEDIT_COLOR_GREEN						9
#define CHATEDIT_COLOR_BLUE							5
#define CHATEDIT_COLOR_YELLOW						10
#define CHATEDIT_COLOR_SKY							7
#define CHATEDIT_COLOR_TRUERED						8
#define CHATEDIT_COLOR_TRUEGREEN					4
#define CHATEDIT_COLOR_TRUEYELLOW					6
#define CHATEDIT_COLOR_SKYBLUE						11
#define CHATEDIT_COLOR_PINK							12
#define CHATEDIT_COLOR_VIOLET						13
#define CHATEDIT_COLOR_LIGHTREDYELLOW				14
#define CHATEDIT_COLOR_TRUEBLUE						15
inline COLORREF GetChatEditColor( SI16 ColorType )
{
	switch( ColorType )
	{
	case CHATEDIT_COLOR_BLACK:
		return RGB( 0, 0, 0 );
	case CHATEDIT_COLOR_WHITE:
		return RGB( 255, 255, 255 );
	case CHATEDIT_COLOR_RED:
		return RGB( 250, 70, 70 );
	case CHATEDIT_COLOR_GREEN:
		return RGB( 173, 255, 127 );
	case CHATEDIT_COLOR_BLUE:
		return RGB( 25, 70, 230 );
	case CHATEDIT_COLOR_YELLOW:
		return RGB( 255, 202, 5 );
	case CHATEDIT_COLOR_SKY:
		return RGB( 25, 70, 230 );
	case CHATEDIT_COLOR_TRUERED:
		return RGB( 255, 0, 0 );
	case CHATEDIT_COLOR_TRUEGREEN:
		return RGB( 25, 130, 20 );
	case CHATEDIT_COLOR_TRUEYELLOW:
		return RGB( 240, 110, 0 );
	case CHATEDIT_COLOR_SKYBLUE:
        return RGB( 33 , 204, 255);
	case CHATEDIT_COLOR_PINK:
		return RGB( 245,150,200 );
	case CHATEDIT_COLOR_VIOLET:
		return RGB( 183,39,191 );
	case CHATEDIT_COLOR_LIGHTREDYELLOW:
		return RGB( 255,110,0 );
	case CHATEDIT_COLOR_TRUEBLUE:
		return RGB( 0, 0, 255 );
	default:
		return RGB( 0, 0, 0 );
	}
}

#define MAX_TREE_TEXT_LEN							128
#define TREE_LINE_HEIGHT							16

#define NEWCSTATIC(p)		 { (p) = new CStatic( this); }
#define NEWCEDIT(p)			 { (p) = new CEdit( this); }
#define NEWCIMAGESTATIC(p)	 { (p) = new CImageStatic( this); }
#define NEWCBUTTON(p)		 { (p) = new CButton( this); }
#define NEWCLISTVIEW(p)		 { (p) = new CListView( this); }
#define NEWCLIST(p)			 { (p) = new CList( this); }
#define NEWCCOMBOBOX(p)		 { (p) = new CComboBox( this); }
#define NEWCRADIOBUTTON(p)	 { (p) = new CRadioButton( this ); }
#define NEWCOUTLINE(p)		 { (p) = new COutLine( this ); }
#define NEWCCHECKBOX(p)		 { (p) = new CCheckBox( this ); }
#define NEWCIMAGEBUTTON(p)	 { (p) = new CImageButton( this ); }

enum 
{
	CONTROLLER_KIND_NONE,
	CONTROLLER_KIND_POPUPDIALOG,
	CONTROLLER_KIND_CHILDDIALOG,
	CONTROLLER_KIND_STATIC,
	CONTROLLER_KIND_IMAGESTATIC,
	CONTROLLER_KIND_BUTTON,
	CONTROLLER_KIND_EDIT,
	CONTROLLER_KIND_IMEEDIT,
	CONTROLLER_KIND_LIST,
	CONTROLLER_KIND_LISTVIEW,
	CONTROLLER_KIND_COMBOBOX,
	CONTROLLER_KIND_SCROLL,
	CONTROLLER_KIND_CHECKBOX,
	CONTROLLER_KIND_RADIOBUTTON,
	CONTROLLER_KIND_CHATEDIT,
	CONTROLLER_KIND_TREE,
	CONTROLLER_KIND_OUTLINE,
};

enum
{
	EVENT_BUTTON_CLICKED,
	EVENT_BUTTON_DBCLICKED,
	EVENT_BUTTON_RCLICKED,
	//EVENT_BUTTON_MOUSEOVER,

	EVENT_COMBOBOX_SELECTION_CHANGED,
	EVENT_COMBOBOX_OPEN,
	EVENT_RADIOBUTTON_CHANGED,
	EVENT_CHECKBOX_CHANGED,
	EVENT_SLIDER_VALUE_CHANGED,

	EVENT_IMAGESTATIC_LBUTTONDOWN, 
	EVENT_IMAGESTATIC_DBCLICKED, 
	EVENT_IMAGESTATIC_LBUTTONUP,
	EVENT_IMAGESTATIC_RBUTTONDOWN,
	EVENT_IMAGESTATIC_RBUTTONUP,
	EVENT_IMAGESTATIC_LBUTTONDRAG,
	EVENT_IMAGESTATIC_MOUSEMOVE,
	EVENT_IMAGESTATIC_MOUSEWHEEL,

	EVENT_EDITBOX_STRING,
	EVENT_EDITBOX_CHANGE,
	EVENT_EDITBOX_TAB,
	EVENT_EDITBOX_ARROW_UP,
	EVENT_EDITBOX_ARROW_DOWN,

	EVENT_EDITBOX_PAGEUP,
	EVENT_EDITBOX_PAGEDOWN,
	
	EVENT_LISTBOX_ITEM_DBLCLK,
	EVENT_LISTBOX_SELECTION,
	EVENT_LISTBOX_MOUSEOVER,


	EVENT_LISTVIEW_LBUTTONDOWN,
	EVENT_LISTVIEW_LBUTTONUP,
	EVENT_LISTVIEW_RBUTTONDOWN,
	EVENT_LISTVIEW_RBUTTONUP,

	EVENT_TREE_SELECTION,
	EVENT_LISTVIEW_DBLCLK,

	EVENT_SCROLL_UPBUTTONCLICKED,
	EVENT_SCROLL_DOWNBUTTONCLICKED,
	EVENT_SCROLL_MOUSEMOVE,
};

enum
{
	CONTROLLER_STATE_NORMAL = 0,
	CONTROLLER_STATE_PRESSED,
	CONTROLLER_STATE_MOUSEOVER,
	CONTROLLER_STATE_DISABLED,
	CONTROLLER_STATE_HIDDEN,
	CONTROLLER_STATE_FOCUS,
};

enum 
{
	MOUSE_STATUS_NONE, 
	MOUSE_STATUS_LBUTTONDOWN,
	MOUSE_STATUS_RBUTTONDOWN
};

enum
{
	SPR_PUTSPRT,
	SPR_PUTSPRALPHAT,
	SPR_PUTSPRSCREENT
};

inline int RectWidth( RECT &rc ) { return ( (rc).right - (rc).left ); }
inline int RectHeight( RECT &rc ) { return ( (rc).bottom - (rc).top ); }

inline SI32 GetTextExtent( HDC hDC, TCHAR *Text, int TextLen )
{	
	if ( Text == NULL || TextLen < 1 )
	{	
		return 0;
	}

	SIZE sz;
	GetTextExtentPoint32( hDC, Text, TextLen, &sz );
	return sz.cx;
}

inline BOOL IsDBCS( char ch1, char ch2 )
{
	return ( IsDBCSLeadByte( ch1 ) || ( ch1 =='\r' && ch2 == '\n' ) );
}

inline BOOL IsDBCS( char ch1 )
{
	return ( IsDBCSLeadByte( ch1 ) );
}

// [영진] _ImmGetIMEFileNameA 로 char 형식의 문자열을 반환 받아 비교하는 대상이므로 유니코드로 바꾸지 않는다?
#define CHT_IMEFILENAME1    "TINTLGNT.IME" // New Phonetic
#define CHT_IMEFILENAME2    "CINTLGNT.IME" // New Chang Jie
#define CHT_IMEFILENAME3    "MSTCIPHA.IME" // Phonetic 5.1
#define CHS_IMEFILENAME1    "PINTLGNT.IME" // MSPY1.5/2/3
#define CHS_IMEFILENAME2    "MSSCIPYA.IME" // MSPY3 for OfficeXP

#define GETPROCADDRESS( Module, APIName, Temp ) \
	Temp = GetProcAddress( Module, #APIName ); \
	if( Temp ) \
	*(FARPROC*)&_##APIName = Temp

#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)

#define IMEID_CHT_VER42		( LANG_CHT | MAKEIMEVERSION( 4, 2 ) )   // New(Phonetic/ChanJie)IME98  : 4.2.x.x // Win98
#define IMEID_CHT_VER43		( LANG_CHT | MAKEIMEVERSION( 4, 3 ) )   // New(Phonetic/ChanJie)IME98a : 4.3.x.x // Win2k
#define IMEID_CHT_VER44		( LANG_CHT | MAKEIMEVERSION( 4, 4 ) )   // New ChanJie IME98b          : 4.4.x.x // WinXP
#define IMEID_CHT_VER50		( LANG_CHT | MAKEIMEVERSION( 5, 0 ) )   // New(Phonetic/ChanJie)IME5.0 : 5.0.x.x // WinME
#define IMEID_CHT_VER51		( LANG_CHT | MAKEIMEVERSION( 5, 1 ) )   // New(Phonetic/ChanJie)IME5.1 : 5.1.x.x // IME2002(w/OfficeXP)
#define IMEID_CHT_VER52		( LANG_CHT | MAKEIMEVERSION( 5, 2 ) )   // New(Phonetic/ChanJie)IME5.2 : 5.2.x.x // IME2002a(w/Whistler)
#define IMEID_CHT_VER60		( LANG_CHT | MAKEIMEVERSION( 6, 0 ) )   // New(Phonetic/ChanJie)IME6.0 : 6.0.x.x // IME XP(w/WinXP SP1)
#define IMEID_CHS_VER41		( LANG_CHS | MAKEIMEVERSION( 4, 1 ) )   // MSPY1.5  // SCIME97 or MSPY1.5 (w/Win98, Office97)
#define IMEID_CHS_VER42		( LANG_CHS | MAKEIMEVERSION( 4, 2 ) )   // MSPY2    // Win2k/WinME
#define IMEID_CHS_VER53		( LANG_CHS | MAKEIMEVERSION( 5, 3 ) )   // MSPY3    // WinXP

#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY

#define MAKEIMEVERSION( major, minor )      ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )


inline void DrawRoundRect( HDC hDC, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, bool Border, bool FillRoundRect, COLORREF BorderColor = CONTROLLER_BORDER_COLOR, COLORREF FillRoundRectColor = CONTROLLER_BACKGROUND_COLOR, SI16 RoundX = 1, SI16 RoundY = 1 )
{
	// 그리지 않겠다는 것이지...
	if ( Border == false && FillRoundRect == false )
	{
		return;
	}

	HBRUSH hBrush = NULL, hOldBrush = NULL;
	HPEN hPen = NULL, hOldPen = NULL;

	if ( FillRoundRect )
	{
		hBrush = CreateSolidBrush( FillRoundRectColor );
		hOldBrush = (HBRUSH)SelectObject( hDC, hBrush );
	}
	else
	{
		hBrush = CreateSolidBrush( NULL_BRUSH );
		hOldBrush = (HBRUSH)SelectObject( hDC, hBrush );
	}

	if ( Border )
	{
		hPen = CreatePen( PS_SOLID, 1, BorderColor );
		hOldPen = (HPEN)SelectObject( hDC, hPen );
	}
	else
	{
		hPen = CreatePen( PS_SOLID, 0, BorderColor );
		hOldPen = (HPEN)SelectObject( hDC, hPen );
	}

	RoundRect( hDC, XPos, YPos, XPos + Width, YPos + Height, RoundX, RoundY );

	if ( hOldPen )
	{
		SelectObject( hDC, hOldPen );
		DeleteObject( hPen );
	}

	if ( hOldBrush )
	{
		SelectObject( hDC, hOldBrush );
		DeleteObject( hBrush );
	}

	return;
}

// 1 pixel 테두리를 그려준다.
inline void DrawBox( HDC hDC, RECT *pRect )
{
	MoveToEx( hDC, pRect->left, pRect->top, NULL );
	LineTo( hDC, pRect->left, pRect->bottom + 1 );

	MoveToEx( hDC, pRect->left, pRect->top, NULL );
	LineTo( hDC, pRect->right + 1, pRect->top );

	MoveToEx( hDC, pRect->right, pRect->top, NULL );
	LineTo( hDC, pRect->right, pRect->bottom + 1 );

	MoveToEx( hDC, pRect->left, pRect->bottom, NULL );
	LineTo( hDC, pRect->right + 1, pRect->bottom );

	return;
}

// 타일 이미지로 어떤 컨트롤을 덮어 버린다. 단, 반드시 타일로 완전히 덮을 수 있어야 한다.
// 다이얼로그의 배경을 덮을 때도 유용하게 사용됨
// 1pixel 도 차이가 나지 않게 하기 위한것
// PutSprParam1 은 이미지의 Font Index 를 가리키는 것임
// PutSprParam2 는 PutSprOption이 PutSprAlphaT인 경우에 Alpha 값을 넣어줘야 함으로 그때 사용
// TileImgNum 은 1, 3, 9 셋중에 하나만 가능하다.
// 1개의 이미지로 덮을것이냐, 3개의 이미지로 덮을것이냐, 9개의 이미지로 덮을것이냐 차이지..
// IsRowTile 이것은 세로로만 타일을 덮을때 사용한다. 3개의 이미지로 기본적으로 가로로 덮겠지만,
// 스크롤 같은 경우는 세로로 타일 이미지를 덮음므로...
inline bool DrawTileImg( TSpr *pTileImg, SI16 PutSprOption, SI16 PutSprParam1, SI16 PutSprParam2, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, SI16 TileImgNum, bool IsRowTile = true )
{
	bool LeftTileXSize = false;
	bool LeftTileYSize = false;

	SI16 TileXSize = pTileImg->GetXSize();
	SI16 TileYSize = pTileImg->GetYSize();

	// 타일이 완전히 덮을 수 있는지 체크 ( 덜 덮거나, 초과해 버리면 안됨으로 )
	// 아래 3줄 주석 처리해 버리면, 아래 코드에 해당하는 경우에는 
	// 배경을 찍기는 하겠지만, 끝 부분을 안 찍게 됨.
	/*
	if ( ( Width % TileXSize ) != 0 || ( Height % TileYSize ) != 0 )
	{
	return false;
	}
	*/

	SI16 RowNum = Height / TileYSize;

	if ( Height % TileYSize != 0 )
	{
		LeftTileXSize = true;
	}

	SI16 ColumnNum = Width / TileXSize;
	if ( Width % TileXSize != 0 )
	{
		LeftTileYSize = true;
	}

	/*
	SI16 i = 0;
	SI16 j = 0;
	*/

	SI16 CurrentDrawXPos = XPos;
	SI16 CurrentDrawYPos = YPos;

	// 타일 3개로 이루어진 이미지
	if ( TileImgNum == 3 )
	{	
		if ( IsRowTile )
		{
			// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
			// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
			switch( PutSprOption )
			{
			case SPR_PUTSPRT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize >= XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}
							else if ( CurrentDrawXPos + TileXSize > XPos + Width - TileXSize )
							{
								CurrentDrawXPos = XPos + Width - TileXSize * 2;
							}

							if ( CurrentDrawYPos + TileYSize > YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}

							if ( CurrentDrawXPos < XPos + 1 )
							{
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{	
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2 );
							}
							else
							{	
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1 );
							}							
						}
					}

					/*
					for ( i = 0; i < RowNum; ++i )
					{
					for ( j = 0; j < ColumnNum; ++j )
					{
					if ( j < 1 )
					{
					GP.PutSpr( pTileImg,XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 0 );
					}
					else if ( j >= ColumnNum - 1 )
					{
					GP.PutSpr( pTileImg,XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 2 );
					}
					else
					{
					GP.PutSpr( pTileImg,XPos + j * TileXSize, YPos + i * TileYSize, PutSprParam1 + 1 );
					}
					}
					}
					*/
				}
				break;
			case SPR_PUTSPRALPHAT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize >= XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}
							else if ( CurrentDrawXPos + TileXSize > XPos + Width - TileXSize )
							{
								CurrentDrawXPos = XPos + Width - TileXSize * 2;
							}

							if ( CurrentDrawYPos + TileYSize > YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}

							if ( CurrentDrawXPos < XPos + 1 )
							{	
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 );
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{	
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 2 );
							}
							else
							{	
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 1 );
							}							
						}
					}
				}
				break;
			default:
				return false;
			}
		}
		else
		{
			// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
			// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
			switch( PutSprOption )
			{
			case SPR_PUTSPRT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize > XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}

							if ( CurrentDrawYPos + TileYSize >= YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}
							else if ( CurrentDrawYPos + TileYSize > YPos + Height - TileYSize )
							{
								CurrentDrawYPos = YPos + Height - TileYSize * 2;
							}

							if ( CurrentDrawYPos < YPos + 1 )
							{
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{	
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2 );
							}
							else
							{	
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1 );
							}							
						}
					}
				}
				break;
			case SPR_PUTSPRALPHAT:
				{
					for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
					{
						for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
						{
							if ( CurrentDrawXPos + TileXSize > XPos + Width )
							{
								CurrentDrawXPos = XPos + Width - TileXSize;
							}

							if ( CurrentDrawYPos + TileYSize >= YPos + Height )
							{
								CurrentDrawYPos = YPos + Height - TileYSize;
							}
							else if ( CurrentDrawYPos + TileYSize > YPos + Height - TileYSize )
							{
								CurrentDrawYPos = YPos + Height - TileYSize * 2;
							}

							if ( CurrentDrawYPos < YPos + 1 )
							{
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 );
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{	
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 2 );
							}
							else
							{	
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 1 );
							}							
						}
					}
				}
				break;
			default:
				return false;
			}
		}
	}
	else if ( TileImgNum == 9 )		// 타일 9개로 이루어진 이미지
	{
		// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
		// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
		switch( PutSprOption )
		{
		case SPR_PUTSPRT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

						if ( CurrentDrawXPos > XPos && CurrentDrawYPos > YPos && 
							CurrentDrawXPos < XPos + Width - TileXSize && CurrentDrawYPos < YPos + Height - TileYSize )
						{
							GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 4 );
						}
						else
						{
							if ( CurrentDrawXPos < XPos + 1 )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 6 );
								}
								else
								{
									GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 3 );
								}
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 2 );
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 8 );
								}
								else
								{
									GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 5 );
								}
							}
							else if ( CurrentDrawYPos < YPos + 1 )
							{
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 1 );
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{
								GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 + 7 );
							}
						}
					}
				}
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

						if ( CurrentDrawXPos > XPos && CurrentDrawYPos > YPos && 
							CurrentDrawXPos < XPos + Width - TileXSize && CurrentDrawYPos < YPos + Height - TileYSize )
						{
							GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 4 );
						}
						else
						{
							if ( CurrentDrawXPos < XPos + 1 )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 );
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 6 );
								}
								else
								{
									GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 3 );
								}
							}
							else if ( CurrentDrawXPos >= XPos + Width - TileXSize )
							{
								if ( CurrentDrawYPos < YPos + 1 )
								{
									GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 2 );
								}
								else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
								{
									GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 8 );
								}
								else
								{
									GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 5 );
								}
							}
							else if ( CurrentDrawYPos < YPos + 1 )
							{
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 1 );
							}
							else if ( CurrentDrawYPos >= YPos + Height - TileYSize )
							{
								GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 + 7 );
							}
						}
					}
				}
			}
			break;
		default:
			return false;
		}
	}
	else if ( TileImgNum == 1 )		// 타일 1개로 이루어진 이미지 ( 제일 사용안 될것 같기에, if 문 최하위로 뒀음 )
	{
		// 아래 출력부 하나로 사용가능하지만, 따로 만든 이유는 안에서 switch 문 for 문 여러번 도는 것 보다는,
		// 아래와 같이 한번만 돌게 하는 식으로 성능 향상을 위해서 코드를 늘렸음.
		switch( PutSprOption )
		{
		case SPR_PUTSPRT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

						GP.PutSpr( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam1 );
					}
				}
			}
			break;
		case SPR_PUTSPRALPHAT:
			{
				for ( CurrentDrawXPos = XPos; CurrentDrawXPos < XPos + Width; CurrentDrawXPos+=TileXSize )
				{
					for ( CurrentDrawYPos = YPos; CurrentDrawYPos < YPos + Height; CurrentDrawYPos+=TileYSize )
					{
						if ( CurrentDrawXPos + TileXSize > XPos + Width )
						{
							CurrentDrawXPos = XPos + Width - TileXSize;
						}

						if ( CurrentDrawYPos + TileYSize > YPos + Height )
						{
							CurrentDrawYPos = YPos + Height - TileYSize;
						}

						GP.PutSprAlpha( pTileImg,CurrentDrawXPos, CurrentDrawYPos, PutSprParam2, PutSprParam1 );
					}
				}
			}
			break;
		default:
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


#endif