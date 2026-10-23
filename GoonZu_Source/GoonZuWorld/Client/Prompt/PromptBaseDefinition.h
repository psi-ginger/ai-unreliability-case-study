#ifndef _PROMPTBASEDEFINITION_H_
#define _PROMPTBASEDEFINITION_H_

#include <Directives.h>
//#include <Graphic.h>

#define MAX_PROMT_NUM				40								// Prompt 최대 개수
#define	MAX_PROMT_STRING_LENGTH		128								// 최대 한글 64자 넣을 수 있음.

#define	PROMPT_DRAW_WIDTH			750	 	
#define	PROMPT_DRAW_HEIGHT			25

class	CPromptMgr;
extern	CPromptMgr	g_PromptMgr;

#endif