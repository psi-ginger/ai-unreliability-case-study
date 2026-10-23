#ifndef _WHISPERCHATMGR_H_
#define _WHISPERCHATMGR_H_

#include "../SMLib/LibList/LibListMgr.h"

#include "WhisperChatBaseDefine.h"

class CWhisperChatObj;

class CWhisperChatMgr : public CLibListMgr
{
public:
	CWhisperChatMgr();
	~CWhisperChatMgr();

	void Init();

	void CreateMaxEmptyObj();
	
	CWhisperChatObj *GetWhisperChatObj( SI32 Index );
	
	SI32 Create( SI32 CharID1, SI32 PersonID1, TCHAR *CharName1, SI32 CharID2, SI32 PersonID2, TCHAR *CharName2 );

private:
};

#endif