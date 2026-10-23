#ifndef _NNPartyInviteDlg_H_
#define _NNPartyInviteDlg_H_

#include <Directives.h>
#include "..\..\..\Resource.h"

#include "..\..\..\Common\PartyMgr\PartyBaseInfo.h"

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNPartyInviteDlg : public CDialogController
{
public:
	CNPartyInviteDlg();
	~CNPartyInviteDlg();

	void Create();
	void Show();
	void Hide();

	static void CALLBACK StaticCallBackDialogPartyInviteDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void	CALLBACK CallBackDialogPartyInviteDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void PartyCreateInvite( SI32 LeaderPersonID, TCHAR *strLeaderCharName );
	void PartyInvite( TCHAR *CharName, SI32 PartyLeaderPersonID, SI32 PartyIndex );

private:

	SI32		m_siPartyIndex;
	SI32		m_siPartyLeaderPeronID;

	CEdit*		m_pEdit;
	CButton*	m_pButton[2]; // 0: 수락 1: 거절
};

#endif