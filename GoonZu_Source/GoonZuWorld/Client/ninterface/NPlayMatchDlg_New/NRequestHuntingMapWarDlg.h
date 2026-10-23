#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-Guild.h"

enum
{
	Round_1 = 0	,
	Round_2,
	Round_3	,

};

class CNRequestHuntingMapWarDlgDlg : public CDialogController
{
public:

	CNRequestHuntingMapWarDlgDlg();
	~CNRequestHuntingMapWarDlgDlg();

	void	Init();
	void	Destroy();

	void	Create();
	void	Action();
	void	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );


	static void CALLBACK StaticRequestHuntingMapWarDlgDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NRequestHuntingMapWarDlgDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory m_InterfaceFactory;

	void	SetComboBox();
	void	RefreshApplyList();
	void	SetRankList( cltGameMsgResponse_GuilHuntMapdWar_GetTopTen* pclInfo );

	void	ApplyGuildDungeon();

	void	SetTime(SYSTEMTIME stTIme );

	void	SetButton(SI32 siMapindex );

	clock_t OpenTime	;
	clock_t ChangeTime	;
	SYSTEMTIME stOpenTiem;	

private:

	SI32				m_siSelectedMapIndex;
	CStatic     		*m_pNNPCMenuStatic1;	//¾ó±¼
	CStatic     		*m_pNNPCMenuStatic2;	//´ë»ç
};


