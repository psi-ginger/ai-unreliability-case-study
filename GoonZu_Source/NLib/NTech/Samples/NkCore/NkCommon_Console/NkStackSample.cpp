#include <NkStack.h>

#include "display_helper.h"

#define EXCUTE_STACK(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_stack(&kStack); _getch();

void NkStackSample()
{
	EXCUTE_STACK( NkStack<int> kStack );

	EXCUTE_STACK( kStack.Push(10) );
	EXCUTE_STACK( kStack.Push(20) );
	EXCUTE_STACK( kStack.Push(30) );
	EXCUTE_STACK( kStack.Push(40) );
	EXCUTE_STACK( kStack.Push(50) );

	int iValue;

	EXCUTE_STACK( kStack.Pop(iValue) );
	EXCUTE_STACK( kStack.Pop(iValue) );
	EXCUTE_STACK( kStack.Pop(iValue) );
}