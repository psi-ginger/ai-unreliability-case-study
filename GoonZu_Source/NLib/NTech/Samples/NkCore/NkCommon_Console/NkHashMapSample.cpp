#include <NkHashMap.h>

#include "display_helper.h"

#define EXCUTE_HASHMAP(a) a; CDispHelper::Get().PrintSource(#a); disp_hashmap(&kHashMap); _getch();

//-------------------------------------------------------------------------------------------------
// Functors (Hash Functor, Equals Functor)
//     NkHashMap 사용시 
//     해쉬값을 구하기 위한 HashFunctor와 키값 비교를 위한 EqualsFunctor를 직접 구현해주어야함
//-------------------------------------------------------------------------------------------------
class MyHashFunctor
{
public:
	static unsigned int KeyToHashIndex(char key, unsigned int uiHashSize)
	{
		return key % uiHashSize;
	}
};

class MyHashEqualsFunctor
{
public:
	static bool IsKeysEqual(char key1, char key2)
	{
		return key1 == key2;
	}
};

//-------------------------------------------------------------------------------------------------

void disp_hashmap(NkHashMap<char, int, MyHashFunctor, MyHashEqualsFunctor>* pkHashMap);

void NkHashMapSample()
{
	NkHashMap<char, int, MyHashFunctor, MyHashEqualsFunctor> kHashMap(5);

	CDispHelper::Get().PrintSource( "SOURCE CODE:\n NkHashMap<char, int, MyHashFunctor, MyHashEqualsFunctor> kHashMap(5)\n" );
	disp_hashmap(&kHashMap);
	_getch();

	EXCUTE_HASHMAP( kHashMap.SetAt('a', 10) );
	EXCUTE_HASHMAP( kHashMap.SetAt('b', 20) );
	EXCUTE_HASHMAP( kHashMap.SetAt('c', 30) );
	EXCUTE_HASHMAP( kHashMap.SetAt('d', 40) );
	EXCUTE_HASHMAP( kHashMap.SetAt('e', 50) );
	EXCUTE_HASHMAP( kHashMap.SetAt('f', 60) );
	EXCUTE_HASHMAP( kHashMap.SetAt('g', 70) );

}




//-------------------------------------------------------------------------------------------------
// 해쉬맵 상태 디스플레이를 위한 함수
//-------------------------------------------------------------------------------------------------
void disp_hashmap(NkHashMap<char, int, MyHashFunctor, MyHashEqualsFunctor>* pkHashMap)
{
	CDispHelper::Get().PutStr("buckets  nodes→\n");


	unsigned int uiHashSize = pkHashMap->GetHashSize();

	NkCharsA< 16 > kValue;
	unsigned int i, j, uiNodeCount;

	NkHashMapPos kHashPos;
	pkHashMap->MoveBegin(kHashPos);

	for( i = 0; i < uiHashSize; ++i ) 
	{
		uiNodeCount = pkHashMap->GetNodeCount(i);

		// 
		CDispHelper::Get().PutStr("┌─┐");

		for( j = 0; j < uiNodeCount; ++j ) CDispHelper::Get().PutStr("┌KEY ┬DATA┐");
		CDispHelper::Get().PutStr("\n");

		//
		kValue.Format("%d", i);
		CDispHelper::Get().Printf("│%2s│", kValue.GetString());

		for( j = 0; j < uiNodeCount; ++j ) 
		{
			char cKey = pkHashMap->GetKey(kHashPos);
			int iValue = pkHashMap->GetData(kHashPos);

			kValue.Format("%c", cKey);

			if( uiNodeCount == 1 ) CDispHelper::Get().Printf("┤%4s│%4d│", kValue.GetString(), iValue);
			//else if( j == 0 ) printf("┤%4s│%4d├", kValue.GetString(), iValue);
			else if( j == uiNodeCount - 1 ) CDispHelper::Get().Printf("┤%4s│%4d│", kValue.GetString(), iValue);
			else CDispHelper::Get().Printf("┤%4s│%4d├", kValue.GetString(), iValue);
			

			pkHashMap->MoveNext(kHashPos);
		}
		CDispHelper::Get().PutStr("\n");

		//
		CDispHelper::Get().PutStr(DISP_NODE_BOTTOM);

		for( j = 0; j < uiNodeCount; ++j ) CDispHelper::Get().PutStr("└──┴──┘");
		CDispHelper::Get().PutStr("\n");

	}

	CDispHelper::Get().PutStr("-------------------------------------------------------------------------------\n");
}