#include <NkDeque.h>

#include "display_helper.h"

#define EXCUTE_DEQUE(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_deque(&kDeque); _getch();


void NkDequeSample()
{
	EXCUTE_DEQUE( NkDeque<int> kDeque );

	EXCUTE_DEQUE( kDeque.PushBack(10) );
	EXCUTE_DEQUE( kDeque.PushBack(20) );
	EXCUTE_DEQUE( kDeque.PushBack(30) );
	EXCUTE_DEQUE( kDeque.PushBack(40) );

	int iValue;
	EXCUTE_DEQUE( kDeque.PopFront(iValue) );
	EXCUTE_DEQUE( kDeque.PopBack(iValue) );

	EXCUTE_DEQUE( kDeque.PushBack(50) );
	EXCUTE_DEQUE( kDeque.PushBack(60) );
	EXCUTE_DEQUE( kDeque.PushBack(70) );
	EXCUTE_DEQUE( kDeque.PushBack(80) );
	EXCUTE_DEQUE( kDeque.PushBack(90) );

	EXCUTE_DEQUE( kDeque.PopFront(iValue) );
	EXCUTE_DEQUE( kDeque.PopFront(iValue) );
	EXCUTE_DEQUE( kDeque.PopFront(iValue) );
	EXCUTE_DEQUE( kDeque.PopFront(iValue) );
	EXCUTE_DEQUE( kDeque.PopFront(iValue) );

	EXCUTE_DEQUE( kDeque.PopBack(iValue) );
	EXCUTE_DEQUE( kDeque.PopBack(iValue) );

}