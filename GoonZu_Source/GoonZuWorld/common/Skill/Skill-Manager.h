//---------------------------------
// 2003/8/1 김태곤
//---------------------------------

#ifndef _SKILLMANAGER_H
#define _SKILLMANAGER_H

#include <Directives.h>

#include "Skill.h"

class cltSkillManager
{
public:
	HWND m_hDlg;
#ifdef _SAFE_MEMORY
	NSafeTArray<HWND, MAX_SKILL_TYPE_NUMBER>	TabDlg;
#else
	HWND TabDlg[MAX_SKILL_TYPE_NUMBER];
#endif

	BOOL DlgRectInitSwitch;
	RECT DlgRect;

	SI32 CharUnique;					// 대상이 되는 캐릭터. 

	BOOL BonusButtonEnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 

	SI32 CurrentSkillTypeUnique;		// 현재 선택된 스킬 타입. 
	SI32 CurrentSkillUnique;			// 현재 선택된  스킬 유니크

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSkillTypeInfo*, MAX_SKILL_TYPE_NUMBER>	pclSkillTypeInfo;
	NSafeTArray<cltSkillInfo*, MAX_SKILL_NUMBER>			pclSkillInfo;
#else
	cltSkillTypeInfo* pclSkillTypeInfo[MAX_SKILL_TYPE_NUMBER];
	cltSkillInfo* pclSkillInfo[MAX_SKILL_NUMBER];
#endif

	HIMAGELIST hImageList;

	cltWeaponSkillMasterManager m_clMasterManager;	// 무기술의 달인

	cltSkillManager();
	~cltSkillManager();

	// 스킬 타입을 추가한다. 
	SI32 AddSkillType(SI32 skilltype, TCHAR* pskillcode, TCHAR* name,BOOL bshow = TRUE );
	SI32 FindTypeRefFromUnique(SI32 unique);
	// 코드를 근거로 스킬타입을 구한다. 
	SI32 FindSkillTypeFromCode(TCHAR* pcode);


	// 스킬을 추가한다. 
	SI32 AddSkill(SI32 unique, SI32 skilltype, SI32 skillatb, TCHAR *code, TCHAR* name, SI32 maxskilllevel,
				  SI32 parentref, SI32 bitmapindex, SI32 explainunique,SI32 weapontype = 0 );

	bool LoadSkillLimitFile(TCHAR* szFileName);

	SI32 FindUniqueFromSkillCode( TCHAR *strSkillCode );
	SI32 FindUniqueFromSkillName( TCHAR *strSkillName );

	SI32 FindSkillTypeFromUnique( SI32 siUnique );
	SI32 FindSkillUniqueFromWeaponSkillType( SI32 siWeaponSkillType );

	BOOL TreeViewInsert(SI32 skilltypeunique);
	// 트리구조에  추가한다.
	BOOL TreeViewInsert(HWND hwnd, SI32 type, SI32 parentref, HTREEITEM node);

	void MakeSkillTitle(cltPSkillInfo* pclskillmanager, cltPItemInfo* pclcharitem, SI32 skillunique,bool bWeapon = false);
	void MakeWeaponSkillTitle(cltPSkillInfo* pclskillmanager, cltPItemInfo* pclcharitem, SI32 skillunique);

	// 스킬을 최초로 연마한다. 
	BOOL DevelopSkill(SI32 skillunique);

	// skillunique를 근거로 스킬의 이름을 얻어온다.
	bool GetSkillName(SI32 skillunique, TCHAR* buffer);
	bool GetWeaponSkillNeedSkillPoint( IN SI32 siSkillUnique, IN SI32 siSkillLevel, OUT SI32* psiNeedSkillPoint );

	//--------------------------------
	// Skill-Set.cpp
	//--------------------------------
	void SetSkillType();
	void SetSkill();

	void	GetSkillLevel();


	SI32	IsChanged();

	bool	FindTreeData( HWND hDlg, HTREEITEM, TV_ITEM *, SI32 Param );
	bool	FindChildTreeData( HWND hDlg, HTREEITEM, TV_ITEM *, SI32 Param );

	// skillunique를 근거로 최대 스킬 레벨을 구한다. 
	SI32 GetMaxLevel(SI32 skillunique);
	// skillunique를 근거로 최대 경험스킬 레벨을 구한다. 
	SI32 GetMaxExpLevel(SI32 skillunique);

	SI32 GetTotalMaxLevel(SI32 skillunique)
	{
		return GetMaxLevel(skillunique) + GetMaxExpLevel(skillunique);
	}

	// 부모 기술의 레퍼런스를 구한다. 
	SI32 GetParentSkillRef(SI32 skillunique);

	// 스킬이 제조에 관련된 스킬인가?
	bool IsManufactureSkill(SI32 skillunique);

	void SetSkillExplainText( SI16 CurBaseSkillLevel, SI16 MaxBaseSkillLevel, SI16 ExpSkillLevel, SI16 MaxExpSkillLevel, SI32 CurExp, SI32 NextExp );

	// 전투향상술 올릴 수 있나 없나
	bool IsUpgradeWeaponSkill(SI32 unique,SI32 level,SI32 id);

	// 히든 전투 향상술을 올릴 수 있나 없나
	bool IsUpgradeHiddenWeaponSkill( SI32 siSkillUnique, SI32 siLevel, SI32 siCharID );

	// 해당 히든 전투향상술에 필요한 아이템을 갖고 온다
	bool GetHiddenWeaponSkillItem( IN SI32 siSkillUnique, IN SI32 siLevel, OUT cltItem* pclGetItem );

	// siNextLevel로 가기위해 어떠한 조건이 있는지 가져온다
	void GetWeaponSkillExplain( IN SI32 siSkillUnique, IN SI32 siSkillLevel, OUT TCHAR * pszExplain );
	SI32 GetWeaponSkillRate( IN SI32 siSkillUnique, IN SI32 siSkillLevel );

private:	
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_SKILL_NUMBER>		m_si32SkillLevel;
#else
	SI32		m_si32SkillLevel[ MAX_SKILL_NUMBER ];
#endif

	HTREEITEM	m_hNow;
	HBITMAP		m_hDecreaseSkillHitBitmap;
};


#endif