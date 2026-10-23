#ifndef _FourleafMANAGER_H_
#define _FourleafMANAGER_H_

#include <Directives.h>
#include "CommonLogic.h"

#define Fourleaf_MAX_LIST	10
#define MAX_UNIQUENUM	20	//m_100healmanager.txt 에 들어갈 최대 유니크 갯수

struct stFourleafProduct
{
	SI32 ItemUnique;
	SI32 ItemNum;

	stFourleafProduct()
	{
		Init();
	}

	void Init()
	{
		memset(this, 0, sizeof(stFourleafProduct));
	}

	void Set( stFourleafProduct * src )
	{
		memcpy(this,src, sizeof(stFourleafProduct));
	}
	bool IsSame( stFourleafProduct * src )
	{
		if( ItemUnique == src->ItemUnique && ItemNum == src->ItemNum )
		{
			return true;
		}

		return false;
	}

};

class CFourLeafManager
{
public:

	CFourLeafManager();
	~CFourLeafManager();

	void Init();

	//네잎클로버의 2배 아이템수량적용이 안되야 하는 제외 아이템 목록을 얻어 온다..
	bool LoadFourleafProductDataFromTxt();

#ifdef _SAFE_MEMORY
	NSafeTArray<stFourleafProduct, MAX_UNIQUENUM>	m_FourleafProduct;
	NSafeTArray<SI32, MAX_UNIQUENUM>				m_100healmanager;
#else
	stFourleafProduct	m_FourleafProduct[MAX_UNIQUENUM];
	SI32 m_100healmanager[MAX_UNIQUENUM];
#endif

	// 가차 시스템에서 얻을수있었거나 얻을수있는 아이템들의 목록을 읽어온다.
	// 등록된 아이템들은 아이템을 만들때, 분해할때 특별한 동작을 취한다.
	
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, Fourleaf_MAX_LIST>		m_FourleafRareList;
#else
	SI32 m_FourleafRareList[Fourleaf_MAX_LIST];
#endif

	bool IsFourleafRareItem(SI32 ItemUnique );


private:	

};

#endif
