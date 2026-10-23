#ifndef _DRAWCHATBASEINFO_H_
#define _DRAWCHATBASEINFO_H_

//#include <Graphic.h>

#include "..\..\StatusBar\ChatStatusBar\ChatStatusBarBaseInfo.h"
#include "..\..\FontManager\FontManager.h"

#define		MAX_CHAT_SURFACE_WIDTH			172				// 채팅 가로 크기
#define		MAX_CHAT_SURFACE_HEIGHT			200				// 채팅 세로 크기

#define		MAX_SHOPBANNER_TITLE_WIDTH		172				// 상점 타이틀 배너 가로 크기
#define		MAX_SHOPBANNER_TITLE_HEIGHT		35				// 상점 타이틀 배너 가로 크기

#define		CHAT_FONT_HEIGHT_SIZE			13				// 채팅 폰트 세로 크기 

#define		DRAW_CHAT_IMAGE_NUM				10				// 채팅 배경에 사용되는 이미지의 종류 갯수

#define		AVERAGE_LINE_CHAR_NUM			14				// 한줄에 표시하는 평균 글자수 ( byte 단위임 ) 오차 값 : +-5

#define		CHAT_DELAY_TIME					150				// 채팅이 화면에 나타나는 프레임

enum { 
	LEFTTOP_IMAGE_SPR, 
	CENTERTOP_IMAGE_SPR, 
	RIGHTTOP_IMAGE_SPR, 
	LEFTCENTER_IMAGE_SPR, 
	CENTER_IMAGE_SPR, 
	RIGHTCENTER_IMAGE_SPR, 
	LEFTBOTTOM_IMAGE_SPR, 
	CENTERBOTTOM_IMAGE_SPR, 
	RIGHTBOTTOM_IMAGE_SPR, 
	TAIL_IMAGE_SPR 
};

enum { 
	USER_CHATTING, 
	ADVERTISE_CHATTING, 
	PRODUCTION_CHATTING, 
	NPC_CHATTING, 
	PERSONALSHOP_BANNER, 
	PERSONALSHOP_BANNER_PREMIUM,
	ELECTIONSTUMP
};


#endif
