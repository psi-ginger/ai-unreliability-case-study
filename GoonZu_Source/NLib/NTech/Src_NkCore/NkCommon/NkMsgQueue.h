//-------------------------------------------------------------------------------------------------
// NkMsgQueue.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include "NkCore.h"
#include "NkMsg.h"

class NKCORE_ENTRY NkMsgQueue
{
public:
	NkMsgQueue(unsigned int uiSize);
	~NkMsgQueue();

	inline unsigned int GetMsgCount() const { return m_uiMsgCount; }
	inline unsigned int GetFreeSize() const { return m_uiFreeSize; }
	inline unsigned int GetAllocedBytes() const { return m_uiBufferSize; }

	bool PushMsg(NkMsg* pkMsg);
	
	NkMsg* GetFrontMsg();
	void RemoveFrontMsg();

protected:
	unsigned int m_uiMsgCount;
	unsigned int m_uiBufferSize;
	unsigned int m_uiFreeSize;

	unsigned int m_uiFrontPos;
	unsigned int m_uiBackPos;

	char* m_pcBuffer;

	CRITICAL_SECTION m_cs;
};
