#pragma once

#include "../../InterfaceMgr/DialogController.h"

// 군주 정책창
class CNGoonZuPolicyDlg : public CDialogController
{
public:

	InterfaceFactory	m_InterfaceFactory_;

	CNGoonZuPolicyDlg();
	~CNGoonZuPolicyDlg();

	void Create();
	CControllerObj* AddInterface( SI16 Type, SI32 TypeID );

	static void CALLBACK StaticCallBackDialogNGoonZuPolicyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNGoonZuPolicyDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetGoonzuBonus( SI16 BonusTime , SI16 MakeBonusTime , bool ChangeAble );
	void SetLiquidationVillageNumber( SI16 LiquidationVillageNumber );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );


	// 결정할 마을의 개수
	SI32 m_siSelectNumber;

	// 결정한 뽀너스 타임
	SI32 m_siSelectedBonusTime;
	// 결정한 제조 뽀너스 타임
	SI32 m_siSelectedMakeBonusTime;

};