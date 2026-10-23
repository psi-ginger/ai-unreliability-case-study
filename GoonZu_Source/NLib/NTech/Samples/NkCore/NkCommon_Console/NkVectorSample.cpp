#include <NkVector.h>

#include "display_helper.h"

#define EXCUTE_VECTOR(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_vector(&kVector); _getch();


void NkVectorSample()
{
	EXCUTE_VECTOR( NkVector<int> kVector(3) );
	EXCUTE_VECTOR( kVector[0] = 10 );
	EXCUTE_VECTOR( kVector[1] = 20 );
	EXCUTE_VECTOR( kVector[2] = 30 );
	EXCUTE_VECTOR( kVector.Insert(2, 25) );
	EXCUTE_VECTOR( kVector.PushBack(40) );
	EXCUTE_VECTOR( kVector.PopFront() );
	
	NkVector<int> kTemp(2);
	kTemp[0] = 90;
	kTemp[1] = 80;

	EXCUTE_VECTOR( kVector.Assign(kTemp) );
}