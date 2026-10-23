#ifndef _ITEMEVENTSTRUCTURE_H_
#define _ITEMEVENTSTRUCTURE_H_

#include <Directives.h>
#include "../../common/CommonHeader.h"

#define MAX_EVENT_STRUCTURE_COUNT	15

class cltItemManagerCommon;

class cltEventStructureUnit
{
public:
	SI16				m_siStructureType;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltMaterialInfo, MAX_MATERIAL_NUMBER>		clMaterial;
#else
	cltMaterialInfo		clMaterial[MAX_MATERIAL_NUMBER];	// 건설 재료
#endif
	SI16				m_siRewardItemUnique;
	SI16				m_siRewardItemNum;

	cltEventStructureUnit()
	{
		Init();
	}

	void Init()
	{
		m_siStructureType = 0;

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
			clMaterial[i].Set(0, 0);
	}

	bool AddMaterial(cltMaterialInfo* pclmaterial)
	{
		SI32 i;
		for(i = 0;i < MAX_MATERIAL_NUMBER;i++)
		{
			if(clMaterial[i].siUnique == 0)
			{
				clMaterial[i].Set(pclmaterial);
				return true;
			}
		}

		return false;
	}

	bool AnalyzeMaterial(TCHAR* pszMaterialText, cltItemManagerCommon* pclItemManager);
};

class cltEventStructure
{
public:
	cltEventStructure();
	~cltEventStructure();

	void Init();
	void Destroy();

	void LoadFromFile();

	cltEventStructureUnit* GetEventStructure( SI16 siStructureType );

public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltEventStructureUnit*, MAX_EVENT_STRUCTURE_COUNT>		m_pclUnit;
#else
	cltEventStructureUnit* m_pclUnit[MAX_EVENT_STRUCTURE_COUNT];
#endif
};

#endif