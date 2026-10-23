//------------------------------------------------------------------------------
//
//	아티팩트 창.
//
//	2008. 2. 26
//	황진성
//
//------------------------------------------------------------------------------
#ifndef _NARTIFACT_BASE_DLG_
#define _NARTIFACT_BASE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../Client/ninterface/NArtifactDlg/NArtifactMixtureDlg.h"
#include "../Client/ninterface/NArtifactDlg/NArtifactSlotCreateDlg.h"
#include "../Client/ninterface/NArtifactDlg/NArtifactRestoreDlg.h"
#include "../Client/ninterface/NArtifactDlg/NArtifactRemoveDlg.h"

class NArtifactBaseDlg : public CDialogController
{
public: 
	enum{ MIX, CREATE, RESTORE, REMOVE };

public:
	NArtifactBaseDlg();
	~NArtifactBaseDlg();

	void Create( void );
	
	static void CALLBACK StaticNArtifactBaseDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NArtifactBaseDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);


    void ButtonSetting( SI32 _ButtonKind );		// 누른 버튼에 해당하는것만 버튼색이 밝게하기.
	void EquipmentListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent );	
	void SetPotrait( void );															// 초상화 그리기
	void SetExplain(SI32 _siType);
	bool IsGage(void);																	// 현재 게이지 이미지로 활성화 되있는 창이 있다면.
	
	void Clear( void );

	void Action(void);
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	NArtifactMixtureDlg		m_NArtifactMixtureDlg;
	NArtifactSlotCreateDlg	m_NArtifactSlotCreateDlg;
	NArtifactRestoreDlg		m_NArtifactRestoreDlg;
	NArtifactRemoveDlg		m_NArtifactRemoveDlg;

private:
	InterfaceFactory	m_InterfaceFactory;
};

#endif