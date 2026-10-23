#pragma once

#define CONTROL_NUM 8
#define MAX_CONTROL_BUFFER_LENGTH 3

#define MAX_DLG_BUFFER_LENGTH 3

// 실제 이미지 저장 버퍼 번호
#define REAL_BACKGROUND_NUM 0
// 마스크 이미지 저장 버퍼 번호
#define MASK_BACKGROUND_NUM 1
// 작업 이미지 저장 버퍼 번호
#define WORK_BACKGROUND_NUM 2

//#define GOONZU_DEFAULT_ROOT_PATH "C:/Program Files/Intizen/GoonZu/"
#define GOONZU_DEFAULT_ROOT_PATH "C:/공유방/betherich/"
#define GOONZU_DEFAULT_TEXTMGR_PATH "C:/공유방/betherich/TextMgr/TextMgr.txt"
#define GOONZU_BETHERICH	"betherich"

#define GRID_DEFAULT_WIDTH 8
#define GRID_DEFAULT_HEIGHT 8

#define MARGIN_DEFAULT_LEFT 0
#define MARGIN_DEFAULT_TOP 0
#define MARGIN_DEFAULT_RIGHT 0
#define MARGIN_DEFAULT_BOTTOM 0

#define PIXEL_OFFSET_X 0
#define PIXEL_OFFSET_Y 0

#define MAX_CONTROL_PROPERTY_NUM 8

// 투명색 선언: 검정색으로 정의
#define TRANSPARENT_COLOR	(DWORD)0x00000000
//#define TRANSPARENT_COLOR	(DWORD)0x00f800f0

#define WHITE_COLOR	(DWORD)0x00ffffff

#define MINI_PICTURE_RATIO	5

// mini Picture Position
#define MINI_PICTURE_LEFT 70
#define MINI_PICTURE_TOP  180
#define MINI_PICTURE_WIDTH 140
#define MINI_PICTURE_HEIGHT 140


#define WIN_STARTX	0
#define WIN_STARTY	0
#define	WIN_WIDTH	700
#define	WIN_HEIGHT	800

#define	WIN_TITLE	_T("NDoors 다이얼로그 에디터")