//-------------------------------------------------------------------------------------------------
// NkMsgQueueThreadTest.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include <NkMsgQueue.h>
#include <process.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

enum
{
	MSGTYPE_TESTMSG1 = 1,
	MSGTYPE_TESTMSG2
};

class TestMsg1 : public NkMsg
{
public:
	TestMsg1() { m_usMsgType = MSGTYPE_TESTMSG1; }

	int m_iSerial;
	char m_str[ 2048 ];
};

class TestMsg2 : public NkMsg
{
public:
	TestMsg2() { m_usMsgType = MSGTYPE_TESTMSG2; m_usMsgSize = sizeof(TestMsg2); }

	int m_iSerial;
	int m_iLevel;
	int m_iExp;
};

BOOL g_bThreadRun;

unsigned __stdcall NkMsgQueue_Thread(LPVOID param)
{
	NkMsgQueue* pkMsgQueue = (NkMsgQueue*)param;

	int iSerial = 0;

	while( g_bThreadRun )
	{
		// TestMsg1 보내기
		{
			TestMsg1 msg;

			// 가변 길이 임의 문자열 생성 
			int len = (rand() % 11) + 5;
			for( int i = 0; i < len; ++i )
			{
				msg.m_str[ i ] = (char)((rand() % 26) + 'a');
			}
			msg.m_str[ len ] = '\0';
			msg.m_iSerial = iSerial;

			msg.m_usMsgSize = (unsigned short)(sizeof(NkMsg) + len + 5);

			// 큐에 메시지 넣기
			if( pkMsgQueue->PushMsg( &msg ) == true )
				iSerial++;
		}

		// TestMsg2 보내기
		{
			TestMsg2 msg;

			msg.m_iLevel = rand() % 100;
			msg.m_iExp = rand() % 1000;

			msg.m_iSerial = iSerial;

			// 큐에 메시지 넣기
			if( pkMsgQueue->PushMsg( &msg ) == true )
				iSerial++;
		}
	}

	return 0;
}

void NkMsgQueueThreadTest()
{
	NkMsgQueue kMsgQueue( 64 * 1024 );

	g_bThreadRun = TRUE;

	// 메시지 입력 스레드 시작
	unsigned int uiThreadID;
	HANDLE hThread = (HANDLE) _beginthreadex( NULL, 0, NkMsgQueue_Thread, (LPVOID)&kMsgQueue, 0, &uiThreadID );
	CloseHandle( hThread );	

	// 메시지 처리 루프 - 키를 누르면 중지한다
	NkMsg *pkMsg;
	while( !_kbhit() )
	{	
		pkMsg = kMsgQueue.GetFrontMsg();					// 메시지 얻어오기

		if( pkMsg == NULL ) continue;

		switch( pkMsg->GetMsgType() )						// 메시지 타입에 따른 처리
		{                                                   // (메시지 타입 개수가 많을 때는 함수 포인터 사용 권장)
		case MSGTYPE_TESTMSG1:
			{
				TestMsg1 *pkTestMsg1 = (TestMsg1*)pkMsg;

				printf( "Msg size:%3d, type:%3d, serial:%d, str:%s\n", 
					pkTestMsg1->GetMsgSize(),
					pkTestMsg1->GetMsgType(),
					pkTestMsg1->m_iSerial,
					pkTestMsg1->m_str );
			}
			break;
		case MSGTYPE_TESTMSG2:
			{
				TestMsg2 *pkTestMsg2 = (TestMsg2*)pkMsg;

				printf( "Msg size:%3d, type:%3d, serial:%d, level:%d, exp:%d\n",
					pkTestMsg2->GetMsgSize(),
					pkTestMsg2->GetMsgType(),
					pkTestMsg2->m_iSerial,
					pkTestMsg2->m_iLevel,
					pkTestMsg2->m_iExp
					);
			}
			break;
		}

		kMsgQueue.RemoveFrontMsg();							// 메시지 삭제
	}

	InterlockedExchange((LPLONG)&g_bThreadRun, FALSE);

	printf("[중지 시점]   NkMsgQueue: FreeSize=%d, MsgCount=%d\n", kMsgQueue.GetFreeSize(), kMsgQueue.GetMsgCount());


	// 남은 메시지들을 빼내고, 
	// Queue의 남은 사이즈와 메시지 개수를 확인해보자. (초기화 사이즈와 같은가, 남은 메시지 개수가 0인가)          
	while( (pkMsg = kMsgQueue.GetFrontMsg()) != 0 )
	{
		kMsgQueue.RemoveFrontMsg();
	}

	printf("[꺼내기 완료] NkMsgQueue: FreeSize=%d, MsgCount=%d\n", kMsgQueue.GetFreeSize(), kMsgQueue.GetMsgCount());
}