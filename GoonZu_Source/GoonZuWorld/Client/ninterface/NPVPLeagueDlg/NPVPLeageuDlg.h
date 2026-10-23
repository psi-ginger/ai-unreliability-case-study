
#pragma once

#include "../../InterfaceMgr/DialogController.h"

class cltGameMsgResponse_PVP_League_UI_InfoGet;

class CNPVPLeageuDlg : public CDialogController
{
enum PVPLEAGUE_RETMSG
{
	PVPLEAGUE_RETMSG_APPLY = 0,
	PVPLEAGUE_RETMSG_CANCEL,
};
// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;
protected:

public:


// -- Member Function Area
private:
	void DefaultSetting( void );

	void OnButtonClicked_Apply( void );
	void OnButtonClicked_Cancel( void );

	void ShowRetMsgBox( SI32 siTitleNum, SI32 siTextNum, SI16 siIndex );
	void RequestPVPLeagueUIInfo( void );

	SI32 GetPercentageOfVictories( SI32 siWin, SI32 siLose, SI32 siDraw );
	bool GetResultString( IN SI08 siResult, OUT TCHAR* pszResultString, IN SI32 siSize );
	SI32 GetGrade( SI32 siGradeValue );
		

protected:

public:
	CNPVPLeageuDlg( void );
	~CNPVPLeageuDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNPVPLeageuDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPVPLeageuDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetPVPLeagueInfo( cltGameMsgResponse_PVP_League_UI_InfoGet* pclInfo );

};