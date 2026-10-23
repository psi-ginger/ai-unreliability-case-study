#include <NkList.h>

#include "display_helper.h"

#define EXCUTE_LIST(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_list(&kList); _getch();

void NkListSample()
{
	EXCUTE_LIST( NkList<int> kList );
	EXCUTE_LIST( kList.PushFront(1) );
	EXCUTE_LIST( kList.PushFront(2) );
	EXCUTE_LIST( kList.PushBack(3) );

}