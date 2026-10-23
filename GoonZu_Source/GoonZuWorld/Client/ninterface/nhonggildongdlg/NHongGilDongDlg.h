#ifndef _NHONGGILDONGDLG_H_
#define _NHONGGILDONGDLG_H_

#include "../../InterfaceMgr/DialogController.h"

#include "../../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../../Common/Skill/Skill.h"


struct stHongGilDongMsgData
{
	SI32 siSkillUnique;
	SI32 siSkillLevel;
};

struct stHongGilDongData
{
	SI32	SkillUnique;
	SI32	SkillLevel;
	GMONEY	CostMoney;
	SI32	Result;
	SI32	Type;
};

enum HONGGILDONG_SETDATATYPE
{
	HONGGILDONG_SETDATATYPE_GETCOSTINFO = 0,	//	스킬 지불금 정보를 가져옴
	HONGGILDONG_SETDATATYPE_SUCCESSSKILLUP,		//	스킬을 성공적으로 올림
	HONGGILDONG_SETDATATYPE_NOTENOUGHTMONEY,	//	돈이 부족해 스킬을 올릴 수 없음
};


class CNHongGilDongDlg : public CDialogController
{
public:
	CNHongGilDongDlg();
	~CNHongGilDongDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNSkill( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSkill( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetBasicInfo( void );	// 시작시 기본적으로 셋팅되어야할 정보들

	void Action();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SkillTabChange( SI16 siControllerID );
	void SetAllTabButtonFontStatus( SI16 siControllerID );

	void SetSkillExplainText( void );

	void TreeViewInsert( SI32 siWeaponType, SI32 siParentRef, SI16 siDeeps=1 );
	void UpdataTreeViewItem( SI32 siChangeItemIndex );

	SI16 IsChanged();
	void GetSkillLevel();
	
	void SetData(void *pBuffer);

	SI16 GetWeaponSkillTypeFromControllerID( SI16 siControllerID );
	
	void OnButtonSkillPlus( void );
	void OnTreeSelection( void );
	void OnComboBoxSelection( SI16 siComboBoxID );

	void ShowRegMsgBox( SI32 siUpgradeSkillLevel );
	void ShowHiddenSkill( void );

	void GetHiddenWeaponSkillFromWeaponSkillType( IN SI16 siWeaponSkillType, OUT SI16* psiHiddenGrade1, OUT SI16* psiHiddenGrade2 );
	// 목록으로 들어온 히든스킬 중에 배운것이 있다면 그 스킬 유니크를 리턴한다.
	SI16 IsLearnedHiddenWeaponSkill( IN SI16 *psiHiddenSkillList );

	// 목록으로 들어온 히든스킬을 콤보 박스에 넣는다
	void AddHiddenSkillToComboBoxItem( IN SI16 siComboBoxControllerID, IN SI16 siWeaponSkillType, IN SI16 *psiHiddenSkillList );

	// 같은 트리 노드의 빠른 클릭 체크
	BOOL IsRapidlyClicked(SI32 siSelectedIndex);
private:
	void SendPacketGetWeaponSkillCost( void );

private:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_siCurrentSkillUnique;
	SI16				m_siCurrentSkillTypeUnique;

	SI16				m_siCurrentTabControllerID;

	SI32				m_siSkillLevel[ MAX_SKILL_NUMBER ];

	GMONEY				m_siCostMoney;

	SI32				m_siSelectedTreeIndex;		// 선택 되었던 트리 인덱스

	SI32				m_siTreeClickTime;		// 트리 선택 시간(계속된 빠른 클릭을 방지 하기 위해) 

	SI32				m_siRequestCostTime;		// 비용 요청 시간( 표시되는 금액이 0인 경우를 방지하기 위해)

	bool				m_bSelectHiddenWeaponSkill; // 히든(4,5단계) 스킬 선택 여부
};

#endif
