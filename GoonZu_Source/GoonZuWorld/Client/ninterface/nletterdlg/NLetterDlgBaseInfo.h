


#ifdef __SUN_NLETTER__


#include <windows.h>
#include <Directives.h>

#include "..\..\..\Resource.h"

#define		MAX_LETTERMSG_NUM		10				// 쪽지를 받을 수 있는 최대 개수
#define		MAX_LETTERMSG_SIZE		1024			// 쪽지에 날아오는 데이터 용량


#define		LETTER_INSERT_MSG		29000
#define		LETTER_DELETE_MSG		29001

struct	stNLetterHeader
{
	UI16 uiMsg;
	UI16 uiMsgSize;
};

struct stNLetterInsert
{
	stNLetterHeader sHeader;	
	char strLetterMsg[ MAX_LETTERMSG_SIZE - 100 ];

	stNLetterInsert()
	{
		sHeader.uiMsg = LETTER_INSERT_MSG;
		sHeader.uiMsgSize = sizeof( stNLetterInsert );
	}
};


class CNLetterDlg;
extern	CNLetterDlg	g_NLetterDlg;

#endif