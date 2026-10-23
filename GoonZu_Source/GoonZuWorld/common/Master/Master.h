//---------------------------------
// 2004/1/31 김태곤
//---------------------------------

#ifndef _MASTER_H
#define _MASTER_H

#include <Directives.h>
#include "Skill\Skill.h"

#define MAX_MASTER_PER_SKILL	20			// 각 스킬당 마스터 수. 


class cltMasterUnitInfo{
public:
	SI16 siCharUnique;
	TCHAR szName[MAX_PLAYER_NAME];
	TCHAR szAccountID[MAX_PLAYER_NAME];
	SI16 siSkillLevel;
	SI16 siHnd;
	SI16 siLuk;
	SI16 siMapIndex;
	SI32 siPersonID;

	cltMasterUnitInfo()
	{
		Init();

	}

	cltMasterUnitInfo(SI16 charunique, TCHAR* pname, TCHAR* accountid, SI16 skilllevel, SI16 hnd, SI16 luk, SI16 mapindex, SI32 personid)
	{
		siCharUnique	= charunique;
		MStrCpy(szName,			pname,	MAX_PLAYER_NAME );
		MStrCpy(szAccountID, accountid, MAX_PLAYER_NAME );
		siSkillLevel	= skilllevel;
		siHnd			= hnd;
		siLuk			= luk;
		siMapIndex		= mapindex;
		siPersonID		= personid;

	}

	void Init()
	{
		siCharUnique	= 0;
		StringCchCopy(szName, MAX_PLAYER_NAME, TEXT(""));
		StringCchCopy(szAccountID, MAX_PLAYER_NAME, TEXT(""));
		siSkillLevel	= 0;
		siHnd			= 0;
		siLuk			= 0;
		siMapIndex		= 0;
		siPersonID		= 0;
	}

	void Set(cltMasterUnitInfo* pclinfo)
	{
		siCharUnique	= pclinfo->siCharUnique;
		MStrCpy( szName, pclinfo->szName, MAX_PLAYER_NAME );
		MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );
		siSkillLevel	= pclinfo->siSkillLevel;
		siHnd			= pclinfo->siHnd;
		siLuk			= pclinfo->siLuk;
		siMapIndex		= pclinfo->siMapIndex;
		siPersonID		= pclinfo->siPersonID;
	}
};


//----------------------------
// 특정 기술에 대한 장인 정보. 
//----------------------------
class cltMasterInfo{
public:
	SI32 siRegistedNumber;
#ifdef _SAFE_MEMORY
	NSafeTArray<cltMasterUnitInfo, MAX_MASTER_PER_SKILL>	clUnit;
#else
	cltMasterUnitInfo clUnit[MAX_MASTER_PER_SKILL];
#endif
	
	cltMasterInfo()
	{
		siRegistedNumber = 0;
	}

	~cltMasterInfo()
	{

	}

	void Init()
	{
		SI32 i;

		siRegistedNumber = 0;

		for(i = 0;i < MAX_MASTER_PER_SKILL;i++)
		{
			clUnit[i].Init();
		}
	}

};



class cltMasterManager{
public:
	RECT DlgRect;
	bool bDlgRectInitSwitch;

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_MASTER_PER_SKILL>		siSkillInfo;
#else
	SI32 siSkillInfo[MAX_MASTER_PER_SKILL];
#endif

	SI32			siCurrentSkillUnique;
	SI32			m_siSelectedIndex;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltMasterInfo, MAX_SKILL_NUMBER>	clMasterInfo;
#else
	cltMasterInfo clMasterInfo[MAX_SKILL_NUMBER];
#endif

	cltMasterManager();
	~cltMasterManager();

	// 내림차순으로 정렬한다. 
	void Sort(SI32 sisortdata[]);
	

	void SetSkillList(HWND hDlg);
	
	// 특정 기술 장인으로 등록되어 있는가.
	bool IsRegisterd(SI32 id, SI32 skillunique);
	// 특정 기술 장인에서 해제한다.
	bool Unregist(SI32 id, SI32 skillunique);
	// 특정 기술 장인으로 등록한다.
	bool Regist(SI32 id, SI32 skillunique, SI32 skilllevel, SI32 personid);
	
	// 특정 사용자가 특정 기술의 최고 장인인가 ?
	bool IsTopSkill(SI32 id, SI32 skillunique);

	// 특정 기술의 두번째 장인을 구한다. 
	SI32 GetSecondSkillID(SI32 skillunique);


	bool Delete(SI32 id);



};
#endif
