#ifndef _REMEMBERWINDOWPOSBASEINFO_H_
#define _REMEMBERWINDOWPOSBASEINFO_H_

#include <Directives.h>

#define	MAX_REMEMBERWINDOWPOS_OBJ_NUM		100			// 최대로 위치를 기억하는 개수

#define SCRIPT_VERSION						15001		// 스크립트 파일의 버전이 틀릴 경우에는 읽지 않는다.
														// 15001 이라고 준 이유는 윈도우 좌표가 1800 정도 까지는 존재하리가 생각되기에...

//------------------------------------------------------------------------
//-- 각 창(윈도우)들에 대한 정보
#define	MAINWINDOW				1			// 메인창

#define LOGINDLG				3			// 로그인창

#define SELECTWORLDLIST			5			// 월드 선택창

#define SELECTCHARACTERDLG		7			// 캐릭터 선택창
#define MAKECHARACTERDLG		8			// 캐릭터 생성창

#define INVENTORYDLG			10			// 인벤토리창

#define NOTIFY_GETITEM			15			// 아이템 획득 창

#define ERRORMSGDLG				20			// 오류 알림창

#define CHATDLG					25			// 채팅창 위치

#define PRIVATE_CHATDLG0		30			// 귓속말 채팅 0
#define PRIVATE_CHATDLG1		31			// 귓속말 채팅 1
#define PRIVATE_CHATDLG2		32			// 귓속말 채팅 2
#define PRIVATE_CHATDLG3		33			// 귓속말 채팅 3
#define PRIVATE_CHATDLG4		34			// 귓속말 채팅 4
#define PRIVATE_CHATDLG5		35			// 귓속말 채팅 5
#define PRIVATE_CHATDLG6		36			// 귓속말 채팅 6
#define PRIVATE_CHATDLG7		37			// 귓속말 채팅 7
#define PRIVATE_CHATDLG8		38			// 귓속말 채팅 8
#define PRIVATE_CHATDLG9		39			// 귓속말 채팅 9

#define NPC1TRADEDLG			50			// 경복궁 무수리 다이얼로그

#define BANKDLG					100			// 전장 인터페이스
#define CITYHALLDLG				150			// 시청 인터페이스
#define STOCKDLG				200			// 여각 인터페이스

#define HOUSEDLG				250			// 시전 인터페이스
#define HOUSEUNITDLG			251			// 개인 시전( 시전 인터페이스 내에 속함 )

#define MARKETDLG				300			// 객주 인터페이스
//------------------------------------------------------------------------

#endif