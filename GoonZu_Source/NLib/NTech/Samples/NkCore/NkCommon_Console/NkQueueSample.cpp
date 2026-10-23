#include <NkDeque.h>

#include "display_helper.h"

#define EXCUTE_QUEUE(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_queue(&kQueue); _getch();

void NkQueueSample()
{
	EXCUTE_QUEUE( NkQueue<int> kQueue );

	EXCUTE_QUEUE( kQueue.Push(10) );
	EXCUTE_QUEUE( kQueue.Push(20) );
	EXCUTE_QUEUE( kQueue.Push(30) );
	EXCUTE_QUEUE( kQueue.Push(40) );

	int iValue;
	EXCUTE_QUEUE( kQueue.Pop(iValue) );
	EXCUTE_QUEUE( kQueue.Pop(iValue) );

	EXCUTE_QUEUE( kQueue.Push(50) );
	EXCUTE_QUEUE( kQueue.Push(60) );
	EXCUTE_QUEUE( kQueue.Push(70) );
	EXCUTE_QUEUE( kQueue.Push(80) );
	EXCUTE_QUEUE( kQueue.Push(90) );

	EXCUTE_QUEUE( kQueue.Pop(iValue) );
	EXCUTE_QUEUE( kQueue.Pop(iValue) );
	EXCUTE_QUEUE( kQueue.Pop(iValue) );
	EXCUTE_QUEUE( kQueue.Pop(iValue) );
	EXCUTE_QUEUE( kQueue.Pop(iValue) );

	EXCUTE_QUEUE( kQueue.Pop(iValue) );
	EXCUTE_QUEUE( kQueue.Pop(iValue) );

}