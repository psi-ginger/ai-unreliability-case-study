//==========================================================================
// 제목: 복불복 미니게임
// 스위치: Switch_BokBulBok
// 내용: 
// 작성일: 2009.08.03
// 작성자: 오지연
//==========================================================================
#pragma once

#include "../../Client/Client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-System.h"

enum GameLocation
	{
		TOTAL_GAME_FIELD = 4,
	};

class NBokBulBokEventDlg: public CDialogController
{
public:

	NBokBulBokEventDlg();
	~NBokBulBokEventDlg();

	void init();
	
	void Create();
	void Destroy();
	void Action();
	void Show_FirstMessageDlg(GMONEY siTotalMoney);

	static void CALLBACK StaticNBokBulBok_JoinDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
		   void CALLBACK NBokBulBok_InfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	//void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:	
	void CheckAllMapUse(); // 현재 모든 맵에서 유저가 게임 이용 중인지 체크 한다.
	void ShowWaitingPlayersList(); // 대기자 등록 리스트 팝업.
	
private:
	InterfaceFactory	m_InterfaceFactory;
};




