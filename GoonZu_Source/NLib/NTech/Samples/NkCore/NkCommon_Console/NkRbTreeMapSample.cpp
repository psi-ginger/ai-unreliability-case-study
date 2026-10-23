#include "display_helper.h"

#include <NkRbTreeMap.h>

#define EXCUTE_RBTREE(a) a; CDispHelper::Get().PrintSource(#a); CDispHelper::disp_rbtreemap(&kRbTreeMap); _getch();

void NkRbTreeMapSample()
{
	/*
	NkRbTreeMap<int,int> kRbTreeMap2;

	for( int i = 0; i < 6; ++i )
	{		
		kRbTreeMap2.Insert(i, 1);
		CDispHelper::disp_rbtreemap(&kRbTreeMap2);
	}
	
	CDispHelper::Get().Printf("인서트 완료\n");

	_getch();
	for( int i = 0; i < 16; ++i )
	{
		CDispHelper::disp_rbtreemap(&kRbTreeMap2);
		CDispHelper::Get().Printf("%d\n", i );
		kRbTreeMap2.Remove(rand()%6);
	}

	CDispHelper::Get().Printf("리무브 완료\n");
	_getch();
	*/

	NkRbTreeMap<int,int> kRbTreeMap;

	EXCUTE_RBTREE( kRbTreeMap.Insert(90,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(80,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(70,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(60,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(50,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(40,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(30,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(20,1) );
	EXCUTE_RBTREE( kRbTreeMap.Insert(10,1) );

	NkRbTreeMapPos kPos = kRbTreeMap.GetBeginPos();

	for( int i = 1; i <= 9; ++i )
	{
		CDispHelper::Get().Printf("%d ", kRbTreeMap.GetKey(kPos));
		kPos = kRbTreeMap.GetNextPos(kPos);
	}

	CDispHelper::Get().Printf("\n");

	EXCUTE_RBTREE( kRbTreeMap.Remove(10) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(20) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(30) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(40) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(50) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(60) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(70) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(80) );
	EXCUTE_RBTREE( kRbTreeMap.Remove(90) );


}