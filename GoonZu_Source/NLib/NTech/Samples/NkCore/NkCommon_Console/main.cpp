//-------------------------------------------------------------------------------------------------
// main.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include <NkCore.h>

#include <stdio.h>
#include <conio.h>

#include "display_helper.h"

struct SampleFunction
{
	char m_chKey;
	char* m_pcFunctionName;
	void (*pFunc)();
};


//-------------------------------------------------------------------------------------------------
// 여기에 함수 선언 추가 및 등록만 하면 됨.
//-------------------------------------------------------------------------------------------------

void NkCharsSample();
void NkStringSample();
void NkVectorSample();
void NkListSample();
void NkStackSample();
void NkQueueSample();
void NkDequeSample();
void NkHashMapSample();
void NkMsgQueueSample();
void NkMsgQueueThreadTest();
void NkRbTreeMapSample();

void MemoryLeak();

SampleFunction sample[] =
{
	{ '1', "벡터(NkVector) 샘플", NkVectorSample },
	{ '2', "리스트(NkList) 샘플", NkListSample },
	{ '3', "스택(NkStack) 샘플", NkStackSample },
	{ '4', "큐(NkQueue) 샘플", NkQueueSample },
	{ '5', "데크(NkDeque) 샘플", NkDequeSample },
	{ '6', "해쉬맵(NkHashMap) 샘플", NkHashMapSample },
	{ '7', "메시지큐(NkMsgQueue) 스레드 구동 샘플", NkMsgQueueThreadTest },
	{ '8', "고정버퍼 지역 문자열(NkChars) 샘플", NkCharsSample },
	{ '9', "힙 문자열(NkStrings) 샘플", NkStringSample },
	{ 'a', "레드블랙트리(NkRbTreeMap) 샘플", NkRbTreeMapSample },
	{ '!', "메모리 릭 발생", MemoryLeak },

	{ 0, "", 0 }

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void disp_menu()
{
	CDispHelper::Get().Printf("-------------------------------------------------------------------------------\n");
	CDispHelper::Get().Printf("NkCommon Samples\n");
	CDispHelper::Get().Printf("-------------------------------------------------------------------------------\n");

	for( int i = 0; sample[i].m_chKey != 0; ++i )
	{
		if( 'a' <= sample[i].m_chKey && sample[i].m_chKey <= 'z' )
			sample[i].m_chKey -= ('a'-'A');

		CDispHelper::Get().Printf("[%c] %s\n", sample[i].m_chKey, sample[i].m_pcFunctionName);
	}

	CDispHelper::Get().Printf("\n[ESC] Exit\n");
}

void main()
{
	//-------------------------------------------------------------------------
	// NkCore 스타트
	// NkCommon 라이브러리를 이용하기 위해서는 
	// 어플리케이션 시작시 NkCore::Startup() 구동 필수 
	//-------------------------------------------------------------------------
	NkCore::Startup();

	CDispHelper::Init();

	CDispHelper::Get().CreateConsole("NkCommon 샘플", 1024);

	//-------------------------------------------------------------------------
	// 메뉴 출력
	//-------------------------------------------------------------------------
	disp_menu();

	//-------------------------------------------------------------------------
	// 입력 처리
	//-------------------------------------------------------------------------
	char key;
	while( (key = (char)_getch()) != 27 )
	{
		if( 'a' <= key && key <= 'z' )
			key -= ('a'-'A');

		for( int i = 0; sample[i].m_chKey != 0; ++i )
		{
			if( key == sample[i].m_chKey )
			{
				CDispHelper::Get().Printf("\n\n");
				CDispHelper::Get().Printf("-------------------------------------------------------------------------------\n");
				CDispHelper::Get().Printf("%s\n", sample[i].m_pcFunctionName);
				CDispHelper::Get().Printf("-------------------------------------------------------------------------------\n");

				sample[i].pFunc();
				CDispHelper::Get().Printf("\n\n");
				disp_menu();
			}
		}
	}
	//-------------------------------------------------------------------------

	CDispHelper::Get().CloseConsole();

	CDispHelper::Shutdown();

	//-------------------------------------------------------------------------
	// NkCore 해제. 어플리케이션 종료시 NkCore::Shutdown()으로 반드시 종료
	//-------------------------------------------------------------------------
	NkCore::Shutdown();
}
