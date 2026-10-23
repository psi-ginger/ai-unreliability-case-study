//-------------------------------------------------------------------------------------------------
// display_helper.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#include <NkCore.h>
#include <NkConsoleOutput.h>
#include <NkVector.h>
#include <NkList.h>
#include <NkQueue.h>
#include <NkDeque.h>
#include <NkStack.h>
#include <NkChars.h>
#include <NkString.h>
#include <NkMsgQueue.h>
#include <NkRbTreeMap.h>

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

#define DISP_NODE_TOP		"┌─┐"
#define DISP_NODE_MIDONE	"│%2s│"
#define DISP_NODE_MIDLEFT	"│%2s├"
#define DISP_NODE_MID		"┤%2s├"
#define DISP_NODE_MIDRIGHT	"┤%2s│"
#define DISP_NODE_BOTTOM	"└─┘"


class CDispHelper : public NkConsoleOutput
{
public:
	static void Init();
	static void Shutdown();
	static CDispHelper& Get() { return *ms_pkDispHelper; }

	static void disp_vector(NkVector<int>* pkVector);
	static void disp_list(NkList<int>* pkList);
	static void disp_stack(NkStack<int>* pkStack);
	static void disp_queue(NkQueue<int>* pkQueue);
	static void disp_deque(NkDeque<int>* pkDeque);
	static void disp_string(char* pcStr, size_t stBufferSize = 0);
	static void disp_msgqueue(NkMsgQueue* pkMsgQueue);
	static void disp_rbtreemap(NkRbTreeMap<int,int>* pkRbTreeMap);

	static void PrintSource(const char* pcStr);

private:
	CDispHelper();

	static CDispHelper* ms_pkDispHelper;
};




