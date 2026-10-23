#pragma once

// Space and Tab String
#define WHITESPACE_N_TAB _T("\t ")
#define NULL_STRING ""

//enum CONTROL_TYPE
//{
//	BUTTON_TYPE = 0,
//    EDIT_TYPE = 1,
//	STATIC_TYPE = 2,
//	CHECKBOX_TYPE = 3,
//	RADIOBUTTON_TYPE = 4,
//	COMBOBOX_TYPE = 5,
//	LISTBOX_TYPE = 6,
//	GROUPBOX_TYPE = 7
//};

// error
#define	ERROR_CODE -1

// ddf file 
#define DLG_PAPER_STR						"dialog"
#define DLG_REPLACE_PAPER_STR				"dialog"
#define DLG_HEADER_SPLITTER_STR				">"
#define DLG_REPLACE_HEADER_SPLITTER_STR		"\\shiftright"
#define DLG_ITEM_SPLITTER_STR				","
#define DLG_ITEM_SPLITTER_CHAR				','
#define	DLG_CHILD_FILENAME_SPLITTER_CHAR	','
#define	DLG_CHILD_FILENAME_SPLITTER_STR		","
#define DLG_REPLACE_ITEM_SPLITTER_STR		"\\comma"
#define DLG_ENDLINE_STR						";"
#define DLG_ENDLINE_CHAR					';'
#define DLG_REPLACE_ENDLINE_STR				"\\semicolon"
#define DLG_NEWLINE_STR						"\r\n"
#define DLG_REPLACE_NEWLINE					"\\newline"
#define DLG_FILE_COMMENT_SIGN				"#"				// 파일에서 컴멘트 시작 문자
#define DLG_FILE_TRIM_LETTER				"\t "			// 공백과 탭을 잘라냄
#define FILE_PATH_SPLITTER_BACKSLASH		'\\'
#define FILE_PATH_SPLITTER_SLASH			'/'

// string
#define STR_EMPTY							""				// NULL 스트링
#define STR_ZERO							"0"				// 0 스트링
#define STR_ONE								"1"				// 0 스트링
#define STR_TRIM_LETTER						"\t "			// 공백과 탭을 잘라냄

// filename meaning / tile font
#define WHOLE_IMAGE_STYLE_FONTNUM			1				// 통 이미지 방식 폰트 수
#define TILEINFO_COUNT_OF_FILENAME			3				// 파일이름에서 타일 정보가 차지하는 뒤에서 부터의 개수
#define TILEIMAGENUM_START_POS				1				// 파일명의 타일 정보 부분에서 타일 이미지 개수가 시작되는 위치
#define TILEIMAGENUM_CHAR_LENGTH			2				// 파일명의 타일 정보 부분에서 타일 이미지 개수 문자 수
#define TILEMODE_POS						0				// 파일명의 타일 정보 부분에서 타일 방식 모드의 시작 위치
#define TILEMODE_CHAR_LENGTH				1				// 파일명의 타일 정보 부분에서 타일 방식 모드의 문자 수
#define VALID_TILEIMAGE_NUM0				0				// 타당한 타일 이미지 개수
#define VALID_TILEIMAGE_NUM1				1	
#define VALID_TILEIMAGE_NUM3				3
#define VALID_TILEIMAGE_NUM9				9

#define BASIC_ID_FORMAT						"%s_NONAME%d"		// 컨트롤 및 다이얼로그 ID 저장시 기본 포맷
#define EX_ID_FORMAT						"%s_%s_NONAME%d"	// 컨트롤 및 다이얼로그 ID 저장시 확장 포맷
#define DIALOG_DESIGN_FORMAT_EXT			"ddf"				// 파일 확장자
#define SAVE_FILE_PREFIX					"DLG_제목없음"		// 디폴트 파일명

// Interface
#define MARKERSIZE_X						4				// 컨트롤 객체 선택시 생기는 사각형 X값
#define MARKERSIZE_Y						4				// 컨트롤 객체 선택시 생기는 사각형 Y값
#define DLG_MARGIN_MIN_LENGTH				0				// 최소 마진 간격 값 - x,y 공통임
#define DLG_GRID_MIN_LENGTH					8				// 최소 그리드 간격 값 - x,y 공통임

// letter font 
#define DEFAULT_FONT_COLOR					0
#define DEFAULT_FONT_STYLE					"" //"굴림"
#define DEFAULT_FONT_SIZE					12
#define DEFAULT_FONT_WEIGHT					400				// 얇게 : 100, 보통 400, 굵게 700

// dialog property
//#define DLG_PROPERTY_COUNT 19
#define DLG_SIZE_X							400				// Dialog Default Width
#define DLG_SIZE_Y							600				// Dialog Default Height
#define DLG_DEFAULT_TITLE					""
#define DLG_CHILD_FILENUM					1				// 자식창 없음 => 차후 10개까지 늘어날 수 있음
#define DLG_FILE_PROPERTY_COUNT				(DLG_PROPERTY_COUNT+DLG_CHILD_FILENUM - 1)
#define DLG_DEFAULT_CHILD_FILENAMES			",,,,,,,,,"

enum DIALOG_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	DLG_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	DLG_RESOURCE_ID,		// ID : 리소스 ID							
	DLG_X_COORD,			// X좌표						
	DLG_Y_COORD,			// Y좌표						
	DLG_WIDTH,				// 가로폭					
	DLG_HEIGHT,				// 세로폭					
	DLG_BG_IMAGE_FULLPATH,	// 백그라운드 이미지 								
	DLG_BG_WIDTH,			// 컨트롤 혹은 다이얼로그의 최대 가로폭						
	DLG_BG_HEIGHT,			// 컨트롤 혹은 다이얼로그의 최대 세로폭						
	DLG_BG_TILE,			// 백그라운드 이미지가 타일 인지 통맵인지 여부						
								// true : 타일방식, false : 통맵 방식		
	DLG_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	DLG_BG_START_FONT,		// 파일에서 백그라운드 이미지 시작 위치							
	DLG_TEXT_COLOR,		// 텍스트 컬러							
	DLG_TEXT_STYLE,		// 텍스트 글자체							
	DLG_TEXT_SIZE,			// 텍스트 크기						
	/// ####################################################									
										
    //DLG_CALLBACK_POINTER,// 다이얼로그 콜백 함수 포인터명										
	DLG_TITLE_STR,			// 다이얼로그 타이틀 명						
	DLG_TITLE_VISIBLE,		// 다이얼로그 타이틀이 보이는지 안보이는지 여부							
	DLG_MOVABLE,			// 다이얼로그가 이동 가능한지 여부						
	DLG_CLOSABLE,			// 다이얼로그 닫기 버튼 존재 여부						
										
	//DLG_CHILD_FILENAMES = 20	// 자식 다이얼로그 ID								
	DLG_PROPERTY_COUNT,		// 속성 끝
};										
										
// 사용하지 않는다... 제거됨 2005-03-03										
enum DIALOG_PROPERTY_EXCEPTION										
{										
	DLG_CALLBACK_POINTER,// 다이얼로그 콜백 함수 포인터명									
	DLG_CHILD_FILENAMES = 101	// 자식 다이얼로그 ID								
};										
										
// button property										
//#define BTN_PROPERTY_COUNT 21										
#define BTN_DEFAULT_TITLE ""										
#define BTN_SIZE_X 100										
#define BTN_SIZE_Y 100										
										
enum BUTTON_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	BTN_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	BTN_RESOURCE_ID,		// ID : 리소스 ID							
	BTN_X_COORD,			// X좌표						
	BTN_Y_COORD,			// Y좌표						
	BTN_WIDTH,				// 가로폭					
	BTN_HEIGHT,				// 세로폭					
	BTN_BG_IMAGE_FULLPATH,	// 백그라운드 이미지 								
	BTN_BG_WIDTH,			// 컨트롤 혹은 다이얼로그의 최대 가로폭						
	BTN_BG_HEIGHT,			// 컨트롤 혹은 다이얼로그의 최대 세로폭						
	BTN_BG_TILE,			// 백그라운드 이미지가 타일 인지 통맵인지 여부						
								// true : 타일방식, false : 통맵 방식		
	BTN_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	BTN_BG_START_FONT,		// 파일에서 백그라운드 이미지 시작 위치							
	BTN_TEXT_COLOR,		// 텍스트 컬러							
	BTN_TEXT_STYLE,		// 텍스트 글자체							
	BTN_TEXT_SIZE,			// 텍스트 크기						
	BTN_GROUP_NUM,			// control group 번호						
	/// ####################################################									
										
	BTN_TEXT_STR,			// 버튼에 출력할 TEXT						
										
	BTN_ANIMATION,	// 버튼 위로 마우스 위치시 애니메이션 할지 여부								
	BTN_TEXT_COLOR_R,		// R									
	BTN_TEXT_COLOR_G,		// G						
	BTN_TEXT_COLOR_B,		// B							

	BTN_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	BTN_PROPERTY_COUNT,
};										
										
// ComboBox Property										
//#define CBX_PROPERTY_COUNT 21										
#define CBX_DEFAULT_TITLE ""										
#define CBX_SIZE_X 100										
#define CBX_SIZE_Y 100										
#define CBX_ROW_HT 17	// 콤보박스 한 줄 높이									
#define CBX_POPUP_HT CBX_ROW_HT*15	// 콤보박스 팝업창만 높이 : 기본값 : 한줄높이 * 20 배									
#define CBX_LINE_HT  17										
										
enum COMBOBOX_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	CBX_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	CBX_RESOURCE_ID,	// ID : 리소스 ID								
	CBX_X_COORD,		// X좌표							
	CBX_Y_COORD,		// Y좌표							
	CBX_WIDTH,			// 가로폭						
	CBX_HEIGHT,			// 세로폭						
	CBX_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	CBX_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	CBX_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	CBX_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	CBX_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	CBX_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	CBX_TEXT_COLOR,	// 텍스트 컬러								
	CBX_TEXT_STYLE,	// 텍스트 글자체								
	CBX_TEXT_SIZE,		// 텍스트 크기							
	CBX_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	CBX_POPUP_HEIGHT,	// 콤보 박스가 펼쳐질 때의 POP-UP창 높이								
	CBX_LINE_HEIGHT,	// 콤보 박스의 한줄의 높이								

	CBX_TEXT_COLOR_R,		// R									
	CBX_TEXT_COLOR_G,		// G						
	CBX_TEXT_COLOR_B,		// B							

	CBX_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	CBX_PROPERTY_COUNT, // 끝
						
};										
										
/**										
leon add										
* EditBox Property Order										
*/										
//#define ETB_PROPERTY_COUNT 19										
										
#define ETB_DEFAULT_TITLE ""										
#define ETB_SIZE_X 100										
#define ETB_SIZE_Y 100										
										
enum EDITBOX_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	ETB_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	ETB_RESOURCE_ID,	// ID : 리소스 ID								
	ETB_X_COORD,		// X좌표							
	ETB_Y_COORD,		// Y좌표							
	ETB_WIDTH,			// 가로폭						
	ETB_HEIGHT,			// 세로폭						
	ETB_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	ETB_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	ETB_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	ETB_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	ETB_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	ETB_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	ETB_TEXT_COLOR,	// 텍스트 컬러								
	ETB_TEXT_STYLE,	// 텍스트 글자체								
	ETB_TEXT_SIZE,		// 텍스트 크기							
	ETB_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	ETB_TEXT_STR,		// 처음에 출력할 TEXT							
	ETB_MULTI_LINE,	// true : Multi-line, false : single line								
	ETB_READONLY,		// true : ReadOnly,	  false : 편집 가능						

	ETB_TEXT_COLOR_R,		// R									
	ETB_TEXT_COLOR_G,		// G						
	ETB_TEXT_COLOR_B,		// B							

	ETB_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	ETB_PROPERTY_COUNT
};										
										
/**										
leon add										
* STATIC TEXT Property Order										
*/										
//#define STATT_PROPERTY_COUNT 17										
										
#define STATT_DEFAULT_TITLE ""										
#define STATT_SIZE_X 100										
#define STATT_SIZE_Y 100										
										
enum STATIC_TEXT_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	STATT_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	STATT_RESOURCE_ID,	// ID : 리소스 ID								
	STATT_X_COORD,		// X좌표							
	STATT_Y_COORD,		// Y좌표							
	STATT_WIDTH,			// 가로폭						
	STATT_HEIGHT,			// 세로폭						
	STATT_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	STATT_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	STATT_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	STATT_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	STATT_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	STATT_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	STATT_TEXT_COLOR,	// 텍스트 컬러								
	STATT_TEXT_STYLE,	// 텍스트 글자체								
	STATT_TEXT_SIZE,		// 텍스트 크기							
	STATT_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	STATT_TEXT_STR,	// 처음에 출력할 TEXT							

	STATT_TEXT_COLOR_R,		// R									
	STATT_TEXT_COLOR_G,		// G						
	STATT_TEXT_COLOR_B,		// B							

	STATT_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	STATT_PROPERTY_COUNT,
};										
										
// IMAGE STATIC Property										
//#define IMGSTT_PROPERTY_COUNT 16										
#define IMGSTT_SIZE_X 100										
#define IMGSTT_SIZE_Y 100										
										
enum IMAGE_STATIC_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	IMGSTT_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	IMGSTT_RESOURCE_ID,	// ID : 리소스 ID								
	IMGSTT_X_COORD,		// X좌표							
	IMGSTT_Y_COORD,		// Y좌표							
	IMGSTT_WIDTH,			// 가로폭						
	IMGSTT_HEIGHT,			// 세로폭						
	IMGSTT_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	IMGSTT_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	IMGSTT_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	IMGSTT_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	IMGSTT_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	IMGSTT_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	IMGSTT_TEXT_COLOR,	// 텍스트 컬러								
	IMGSTT_TEXT_STYLE,	// 텍스트 글자체								
	IMGSTT_TEXT_SIZE,		// 텍스트 크기							
	IMGSTT_GROUP_NUM,		// control group 번호							
	/// ####################################################									
	IMGSTT_TEXT_COLOR_R,		// R									
	IMGSTT_TEXT_COLOR_G,		// G						
	IMGSTT_TEXT_COLOR_B,		// B							

	IMGSTT_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	IMGSTT_PROPERTY_COUNT,
};										
										
// LISTBOX Property Order										
//#define LSTB_PROPERTY_COUNT 20										
#define LSTB_DEFAULT_TITLE ""										
#define LSTB_SIZE_X 100										
#define LSTB_SIZE_Y 100										
#define LSTB_LINE_HT 14										
										
enum LISTBOX_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	LSTB_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	LSTB_RESOURCE_ID,	// ID : 리소스 ID								
	LSTB_X_COORD,		// X좌표							
	LSTB_Y_COORD,		// Y좌표							
	LSTB_WIDTH,			// 가로폭						
	LSTB_HEIGHT,			// 세로폭						
	LSTB_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	LSTB_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	LSTB_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	LSTB_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	LSTB_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	LSTB_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	LSTB_TEXT_COLOR,	// 텍스트 컬러								
	LSTB_TEXT_STYLE,	// 텍스트 글자체								
	LSTB_TEXT_SIZE,		// 텍스트 크기							
	LSTB_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	LSTB_MULTI_COLUMN,	// true : Multi-Column, false : single Column								
	LSTB_MULTI_SELECTION, // true : Multi-Selection, false : 불가능									
	LSTB_LINE_HEIGHT, // 한 줄의 높이									
	LSTB_FIXED_ROW_VISIBLE, // 그리드 형태의 리스트 박스에서 fixed Row를 보여줄지 여부									

	LSTB_TEXT_COLOR_R,		// R									
	LSTB_TEXT_COLOR_G,		// G						
	LSTB_TEXT_COLOR_B,		// B							

	LSTB_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	LSTB_PROPERTY_COUNT,
};										
										
// LISTVIEW Property Order										
//#define LSTV_PROPERTY_COUNT 27										
#define LSTV_SIZE_X 100										
#define LSTV_SIZE_Y 100										
#define LSTV_TOOLTIP_SIZE_WIDTH		150		// 툴팁 surface Max x						
#define LSTV_TOOLTIP_SIZE_HEIGHT	300		// 툴팁 surface Max y							
#define LSTV_ITEM_START_POS_X 0										
#define LSTV_ITEM_START_POS_Y 0										
#define LSTV_ITEM_OFFSET_POS_X 0										
#define LSTV_ITEM_OFFSET_POS_Y 0										
#define	LSTV_ITEM_DEFAULT_SIZE_X 40									
#define	LSTV_ITEM_DEFAULT_SIZE_Y 40									
										
enum LISTVIEW_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	LSTV_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	LSTV_RESOURCE_ID,	// ID : 리소스 ID								
	LSTV_X_COORD,		// X좌표							
	LSTV_Y_COORD,		// Y좌표							
	LSTV_WIDTH,			// 가로폭						
	LSTV_HEIGHT,			// 세로폭						
	LSTV_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	LSTV_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	LSTV_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	LSTV_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
										
	LSTV_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	LSTV_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	LSTV_TEXT_COLOR,	// 텍스트 컬러								
	LSTV_TEXT_STYLE,	// 텍스트 글자체								
	LSTV_TEXT_SIZE,		// 텍스트 크기							
	LSTV_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	LSTV_ITEM_EMPTY,		// 인벤토리에서 빈 아이템 표시 여부							
	LSTV_ITEM_COUNT_VISIBLE, // 인벤토리에서 아이템 개수 표시 여부									
	LSTV_ITEM_TOOLTIP_USABLE,			// 인벤토리에서 아이템 툴팁 여부						
										
	LSTV_ITEM_TOOLTIP_WIDTH,	// 툴팁 폭								
	LSTV_ITEM_TOOLTIP_HEIGHT,	// 툴팁 높이								
										
	LSTV_ITEM_START_X,	// 아이템 시작 위치 X								
	LSTV_ITEM_START_Y,	// 아이템 시작 위치 Y								
										
	LSTV_ITEM_OFFSET_X,			// 아이템 오프셋 X값						
	LSTV_ITEM_OFFSET_Y,			// 아이템 오프셋 Y값						
										
	LSTV_ITEM_SIZE_X,	// 아이템 사이즈 X								
	LSTV_ITEM_SIZE_Y,	// 아이템 사이즈 Y								

	LSTV_TEXT_COLOR_R,		// R									
	LSTV_TEXT_COLOR_G,		// G						
	LSTV_TEXT_COLOR_B,		// B							

	LSTV_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	LSTV_PROPERTY_COUNT,
										
};										
										
// CheckBox Property - 2005-03-07										
//#define CHKB_PROPERTY_COUNT 17										
#define CHKB_SIZE_X 100										
#define CHKB_SIZE_Y 100										
#define CHKB_DEFAULT_TITLE	""									
										
enum CHECKBOX_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	CHKB_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	CHKB_RESOURCE_ID,		// ID : 리소스 ID							
	CHKB_X_COORD,			// X좌표						
	CHKB_Y_COORD,			// Y좌표						
	CHKB_WIDTH,				// 가로폭					
	CHKB_HEIGHT,			// 세로폭						
	CHKB_BG_IMAGE_FULLPATH,	// 백그라운드 이미지 								
	CHKB_BG_WIDTH,			// 컨트롤 혹은 다이얼로그의 최대 가로폭						
	CHKB_BG_HEIGHT,			// 컨트롤 혹은 다이얼로그의 최대 세로폭						
	CHKB_BG_TILE,			// 백그라운드 이미지가 타일 인지 통맵인지 여부						
								// true : 타일방식, false : 통맵 방식		
	CHKB_BG_TILE_IMAGE_NUM,// 타일 이미지 개수 1, 3, 9									
	CHKB_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	CHKB_TEXT_COLOR,		// 텍스트 컬러							
	CHKB_TEXT_STYLE,		// 텍스트 글자체							
	CHKB_TEXT_SIZE,		// 텍스트 크기							
	CHKB_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	CHKB_TEXT_STR,			// 버튼에 출력할 TEXT						

	CHKB_TEXT_COLOR_R,		// R									
	CHKB_TEXT_COLOR_G,		// G						
	CHKB_TEXT_COLOR_B,		// B							

	CHKB_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	CHKB_PROPERTY_COUNT,
};										
										
// Radio Property - 2005-03-07										
//#define RADB_PROPERTY_COUNT 18										
#define RADB_SIZE_X 100										
#define RADB_SIZE_Y 100										
#define RADB_DEFAULT_TITLE	""									
										
enum RADIOBUTTON_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	RADB_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	RADB_RESOURCE_ID,		// ID : 리소스 ID							
	RADB_X_COORD,			// X좌표						
	RADB_Y_COORD,			// Y좌표						
	RADB_WIDTH,				// 가로폭					
	RADB_HEIGHT,			// 세로폭						
	RADB_BG_IMAGE_FULLPATH,	// 백그라운드 이미지 								
	RADB_BG_WIDTH,			// 컨트롤 혹은 다이얼로그의 최대 가로폭						
	RADB_BG_HEIGHT,			// 컨트롤 혹은 다이얼로그의 최대 세로폭						
	RADB_BG_TILE,			// 백그라운드 이미지가 타일 인지 통맵인지 여부						
								// true : 타일방식, false : 통맵 방식		
	RADB_BG_TILE_IMAGE_NUM,// 타일 이미지 개수 1, 3, 9									
	RADB_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	RADB_TEXT_COLOR,		// 텍스트 컬러							
	RADB_TEXT_STYLE,		// 텍스트 글자체							
	RADB_TEXT_SIZE,		// 텍스트 크기							
	RADB_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	RADB_TEXT_STR,			// 버튼에 출력할 TEXT						
	RADB_GROUP_ID,			// 라디오 버튼 그룹 ID						

	RADB_TEXT_COLOR_R,		// R									
	RADB_TEXT_COLOR_G,		// G						
	RADB_TEXT_COLOR_B,		// B							

	RADB_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	RADB_PROPERTY_COUNT,
};										
										
/**										
leon add										
* CHAT EDITBOX Property Order										
*/										
//#define CHTETB_PROPERTY_COUNT 21										
										
#define CHTETB_DEFAULT_TITLE ""										
#define CHTETB_SIZE_X 100										
#define CHTETB_SIZE_Y 100										
#define CHTETB_LINE_HT 14										
#define CHTETB_AUTO_SCROLL_VALUE 1										
#define CHTETB_OFFSET_X_VALUE 2										
#define CHTETB_OFFSET_Y_VALUE 2										
										
enum CHATEDIT_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	CHTETB_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	CHTETB_RESOURCE_ID,	// ID : 리소스 ID								
	CHTETB_X_COORD,		// X좌표							
	CHTETB_Y_COORD,		// Y좌표							
	CHTETB_WIDTH,			// 가로폭						
	CHTETB_HEIGHT,			// 세로폭						
	CHTETB_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	CHTETB_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	CHTETB_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	CHTETB_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	CHTETB_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	CHTETB_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	CHTETB_TEXT_COLOR,	// 텍스트 컬러								
	CHTETB_TEXT_STYLE,	// 텍스트 글자체								
	CHTETB_TEXT_SIZE,		// 텍스트 크기							
	CHTETB_GROUP_NUM,		// control group 번호							
	/// ####################################################									
	CHTETB_TEXT_STR,			// 출력할 TEXT						
	CHTETB_LINE_HEIGHT,	// 한줄 높이								
	CHTETB_AUTOSCROLL,		// true : 자동 스크롤, false : 스크롤 안함							
	CHTETB_OFFSET_X,		// offset x값							
	CHTETB_OFFSET_Y,		// offset y값							

	CHTETB_TEXT_COLOR_R,		// R									
	CHTETB_TEXT_COLOR_G,		// G						
	CHTETB_TEXT_COLOR_B,		// B							

	CHTETB_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	CHTETB_PROPERTY_COUNT,
};										
										
/**										
leon add										
* OUTLINE Property Order										
*/										
//#define OUTLNE_PROPERTY_COUNT 17										
										
#define OUTLNE_DEFAULT_TITLE ""										
#define OUTLNE_SIZE_X 100										
#define OUTLNE_SIZE_Y 100										
										
enum OUTLINE_PROPERTY										
{										
	// 공통 PROPERTY#######################################									
	OUTLNE_KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류						
	OUTLNE_RESOURCE_ID,	// ID : 리소스 ID								
	OUTLNE_X_COORD,		// X좌표							
	OUTLNE_Y_COORD,		// Y좌표							
	OUTLNE_WIDTH,			// 가로폭						
	OUTLNE_HEIGHT,			// 세로폭						
	OUTLNE_BG_IMAGE_FULLPATH,// 백그라운드 이미지 									
	OUTLNE_BG_WIDTH,		// 컨트롤 혹은 다이얼로그의 최대 가로폭							
	OUTLNE_BG_HEIGHT,		// 컨트롤 혹은 다이얼로그의 최대 세로폭							
	OUTLNE_BG_TILE,	// 백그라운드 이미지가 타일 인지 통맵인지 여부								
						// true : 타일방식, false : 통맵 방식				
	OUTLNE_BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9								
	OUTLNE_BG_START_FONT,	// 파일에서 백그라운드 이미지 시작 위치								
	OUTLNE_TEXT_COLOR,	// 텍스트 컬러								
	OUTLNE_TEXT_STYLE,	// 텍스트 글자체								
	OUTLNE_TEXT_SIZE,		// 텍스트 크기							
	OUTLNE_GROUP_NUM,		// control group 번호							
	/// ####################################################									
										
	OUTLNE_TEXT_STR,		// 처음에 출력할 TEXT							

	OUTLNE_TEXT_COLOR_R,		// R									
	OUTLNE_TEXT_COLOR_G,		// G						
	OUTLNE_TEXT_COLOR_B,		// B							

	OUTLNE_WEIGHT,				// 폰트 굵기 - 얇게 : 400, 보통 : 700, 굵게 : 800

	OUTLNE_PROPERTY_COUNT,
};										
//##########################################################										
// 각 컨트롤 별 공통 프로퍼티의 배열 번호 지정										
#define	CTRL_COUNT			11						
										
#define BUTTON_POS			0							
#define COMBOBOX_POS		1								
#define EDITBOX_POS			2							
#define STATIC_TEXT_POS		3								
#define IMAGE_STATIC_POS	4									
#define LISTBOX_POS			5							
#define LISTVIEW_POS		6								
#define CHECKBOX_POS		7								
#define	RADIOBUTTON_POS		8							
#define	CHATEDIT_POS		9							
#define	OUTLINE_POS			10						
										
#define BTN_STR				"button"						
#define CBX_STR				"combobox"						
#define ETB_STR				"editbox"						
#define STATT_STR			"statictext"							
#define IMGSTT_STR			"imagestatic"							
#define LSTB_STR			"listbox"							
#define LSTV_STR			"listview"							
#define CHKB_STR			"checkbox"							
#define RADB_STR			"radiobutton"							
#define CHTETB_STR			"chatedit"							
#define OUTLNE_STR			"outline"							
										
#define COMMON_PROPERTY_COUNT 16										
//KIND_NAME,			// 다이얼로그 혹은 컨트롤 종류							
//RESOURCE_ID,			// ID : 리소스 ID							
//X_COORD,				// X좌표						
//Y_COORD,				// Y좌표						
//WIDTH,				// 가로폭						
//HEIGHT,				// 세로폭						
//BG_IMAGE_FULLPATH,	// 백그라운드 이미지 									
//BG_WIDTH,				// 컨트롤 혹은 다이얼로그의 최대 가로폭						
//BG_HEIGHT,			// 컨트롤 혹은 다이얼로그의 최대 세로폭							
//BG_TILE,				// 백그라운드 이미지가 타일 인지 통맵인지 여부						
//							// true : 타일방식, false : 통맵 방식			
//BG_TILE_IMAGE_NUM,	// 타일 이미지 개수 1, 3, 9									
//BG_START_FONT,		// 파일에서 백그라운드 이미지 시작 위치								
//TEXT_COLOR,			// 텍스트 컬러							
//TEXT_STYLE,			// 텍스트 글자체							
//TEXT_SIZE,			// 텍스트 크기							
//GROUP_NUM,			// control group 번호							
										
										
// 각 컨트롤별 속성의 배열 번호										
// 값이  -1일 경우 == 그런 속성 없음										
struct CTRL_PROPERTY_COUNT_INFO										
{										
	char* type;									
	int	  num;								
};										
										
// 컨트롤 속성 타입 별 저장 순서 => 2군데 있음 ( DiagramEditor.cpp의 AddObject와 DLGEditorSDoc.cpp의 Serialize!!)										
struct CTRL_SAVING_ORDER_INFO										
{										
	char* type;									
	int pos;									
};										

enum {

/*#define*/ KIND_NAME = 0, //=	0		, // 다이얼로그 혹은 컨트롤 종류, //	
/*#define*/ RESOURCE_ID, //	1		, // ID : 리소스 ID, //	
/*#define*/ X_COORD, //		2		, // X좌표, //
/*#define*/ Y_COORD, //		3		, // Y좌표, //
/*#define*/ WIDTH, //		4		, // 가로폭, //
/*#define*/ HEIGHT, //		5		, // 세로폭, //
/*#define*/ BG_IMAGE_FULLPATH,//		6		, // 백그라운드 이미지 , //, //
/*#define*/ BG_WIDTH, //	7		, // 컨트롤 혹은 다이얼로그의 최대 가로폭, //	
/*#define*/ BG_HEIGHT, //	8		, // 컨트롤 혹은 다이얼로그의 최대 세로폭, //	
/*#define*/ BG_TILE, //		9		, // 백그라운드 이미지가 타일 인지 통맵인지 여부, //
//, //, //, //	, // true : 타일방식 false : 통맵 방식
/*#define*/ BG_TILE_IMAGE_NUM, //	10		, // 타일 이미지 개수 1, 3, 9, //, //
/*#define*/ BG_START_FONT, //11		, // 파일에서 백그라운드 이미지 시작 위치, //		
/*#define*/ TEXT_COLOR, //	12		, // 텍스트 컬러, //	
/*#define*/ TEXT_STYLE, //	13		, // 텍스트 글자체, //	
/*#define*/ TEXT_SIZE, //	14		, // 텍스트 크기, //	
/*#define*/ GROUP_NUM, //	15		, // control group 번호		
};
//, //, //, //	
										
#define ID_LENGTH	31									
										
// 각 컨트롤 저장 순서 지정!! => 2군데 있음 ( DiagramEditor.cpp의 AddObject와 DLGEditorSDoc.cpp의 Serialize!!)										
static CTRL_SAVING_ORDER_INFO ctrl_saving_order[CTRL_COUNT] = 										
{										
	IMGSTT_STR, 0, 									
	STATT_STR, 1,									
	CHKB_STR, 2,									
	RADB_STR, 3,									
	BTN_STR, 4,									
	ETB_STR, 5,									
	CHTETB_STR, 6,									
	LSTB_STR, 7, 									
	LSTV_STR, 8,									
	CBX_STR, 9,									
	OUTLNE_STR, 10,									
};										

