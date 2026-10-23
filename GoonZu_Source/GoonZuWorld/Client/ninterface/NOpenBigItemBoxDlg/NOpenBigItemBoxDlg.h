
#pragma once

#include "../../InterfaceMgr/DialogController.h"

#include "../../common/Item/ItemCommon/ItemUnit.h"
#include "../../../NLib/NUpdateTimer.h"

class CNOpenBigItemBoxDlg : public CDialogController
{
public:

	CNOpenBigItemBoxDlg();
	~CNOpenBigItemBoxDlg();

	void	Init();
	void	Destroy();

	void	Create();
	void	Action();

	bool	SetBigItemBox( SI32 siPos, cltItem* pclItem );
	bool	SetKeyItem( SI32 siPos, cltItem* pclItem );
	bool	SetResult( SI32 Unique, SI32 Number );

	virtual bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );

	static void CALLBACK StaticOpenBigItemBoxDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NOpenBigItemBoxDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory m_InterfaceFactory;

private:
	SI16	m_siBoxItemPos;
	SI16	m_siKeyItemPos;

	NUpdateTimer	m_UpdateTimer_SendMsg;
	NUpdateTimer	m_UpdateTimer_Twinkle;
};

