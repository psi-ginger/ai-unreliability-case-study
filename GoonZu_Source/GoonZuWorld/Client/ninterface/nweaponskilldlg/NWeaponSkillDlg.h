#ifndef _NWEAPONSKILLDLG_H_
#define _NWEAPONSKILLDLG_H_

#include "../../InterfaceMgr/DialogController.h"

#include "../../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../../Common/Skill/Skill.h"

class CButton;
class CComboBox;
class CStatic;
class CTree;
class CEdit;

#define WEAPONSKILL_TAB_NUM		6

class CNWeaponSkillDlg : public CDialogController
{
public:
	CNWeaponSkillDlg();
	~CNWeaponSkillDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNSkill( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSkill( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SkillTabChange( SI16 TabIndex );

	void SetSkillExplainText( SI16 CurBaseSkillLevel, SI16 MaxBaseSkillLevel, SI16 ExpSkillLevel, SI16 MaxExpSkillLevel, SI32 CurExp, SI32 NextExp );

	void TreeViewInsert( SI32 type, SI32 parentref );

	SI16 IsChanged();
	void GetSkillLevel();

private:
	CTree		*m_pTree;
	CButton		*m_pTabButton[ WEAPONSKILL_TAB_NUM ];
	CButton		*m_pSkillMinusButton;
	CButton		*m_pOKButton;
	CStatic		*m_pStatic[ 3 ];

	CStatic		*m_pSkillLevel;
	CStatic		*m_pLeftSkill;

	CEdit		*m_pSkillExplain;
	CEdit		*m_pSkillStatus;

	SI16		m_siCurrentSkillUnique;
	SI16		m_siCurrentSkillTypeUnique;
	SI16		m_siCurrentTabIndex;

	SI32		m_siSkillLevel[ MAX_SKILL_NUMBER ];
};

#endif
