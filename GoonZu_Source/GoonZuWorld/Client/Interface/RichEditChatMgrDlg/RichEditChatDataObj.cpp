#include "RichEditChatDataObj.h"
#include "RichEditChatMgrDlgBaseInfo.h"

CRichEditChatDataObj::CRichEditChatDataObj() : CObj( MAX_RICHEDIT_OBJ_SIZE )
{
	Initialize();
}

CRichEditChatDataObj::~CRichEditChatDataObj()
{
}

void CRichEditChatDataObj::Initialize()
{
	CObj::Initialize();		// 데이터 초기화

	return;
}

void CRichEditChatDataObj::Destroy()
{
	return;
}