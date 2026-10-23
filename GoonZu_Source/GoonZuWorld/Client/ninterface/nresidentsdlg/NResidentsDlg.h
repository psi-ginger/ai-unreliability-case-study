#ifndef _NRESIDENTSDLG_H_
#define _NRESIDENTSDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class cltVillageResidentsList;

class CButton;
class CComboBox;
class CStatic;
class CEdit;
class CChatEdit;
class CList;

struct stChatEditData;

class CNResidentsDlg : public CDialogController
{
public:
	CNResidentsDlg();
	~CNResidentsDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNResidents( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNResidents( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void RecvResidentsList( cltVillageResidentsList *pclPersonList, UI32 ListCount );
	
	bool CheckData( TCHAR *PlayerName, TCHAR *ChatText );
	void ResidentsChatRecv( TCHAR *PlayerName, TCHAR *ChatText );

	void InsertChatEditData( stChatEditData *pChatEditData );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	BOOL IHaveEventNpcForPersonID( SI32 kind , SI32 personId);

private:
	cltVillageResidentsList *m_pVillageResidentsList;

	CChatEdit	*m_pChatEdit;
	CEdit		*m_pInputEdit;
	CComboBox	*m_pComboBox;
	CList		*m_pList;
	CButton		*m_pOKButton;
	CButton		*m_pRefreshButton;
	CButton		*m_pMoveToVillageButton;
	CButton		*m_pDetailInfoButton;
	CButton		*m_pSendWhisperButton;
	CStatic		*m_pNameStatic;
};

#endif
