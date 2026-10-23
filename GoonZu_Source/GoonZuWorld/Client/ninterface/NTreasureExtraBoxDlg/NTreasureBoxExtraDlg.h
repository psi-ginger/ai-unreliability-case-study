
#pragma once

#include "../../InterfaceMgr/DialogController.h"

#include "../../common/Item/ItemCommon/ItemUnit.h"
#include "../../Inventory/ItemInventoryBuffer.h"

class CNTreasureBoxExtraDlg : public CDialogController
{
public:

	enum TB_VARIABLE
	{
		TB_VARIABLE_MAXFRAMENUM = 13
	};

	CNTreasureBoxExtraDlg();
	~CNTreasureBoxExtraDlg();

	void	Init();
	void	Destroy();

	void	Create();
	void	Action();

	void	DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StaticTreasureBoxExtraDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NTreasureBoxExtraDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory m_InterfaceFactory;

	void	SetTreasureBox( SI32 BoxPos, cltItem* Item, CItemInventoryBuffer* Buffer );
	void	ShowTreasureItem( SI32 Unique, SI32 Number);
	void	SetErrorText( TCHAR* Text);
	void	SetTitle( TCHAR* Text);

	bool	CanDraw();
	void	StopDraw();
	void	DrawQuestion(SI32 ScreenRX, SI32 ScreenRY );


private:
	clock_t					m_pushTime;
	CItemInventoryBuffer*	m_pBuffer;
	cltItem					m_clItem;
	SI32					m_siBoxPos;					// 포지션
	bool					m_bIsDrawing;
	bool					m_bSend;

	SI32					m_siFontIndex;

	TSpr					m_sprQuestion;				// 물음표
};

