/* ==========================================================================
	클래스 :		

	작성일 :		05/07/22
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
// 전쟁 안내원 다이얼로그
#include "..\Client\Client.h"
#include "./NPlayMatchDlg.h"
#include "..\resource.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../common/Char/CharManager/CharManager.h"
#include "../CommonLogic/BlackArmy/BlackArmy.h"

#include "MsgType-System.h"

extern cltCommonLogic* pclClient;

NPlayMatchDlg::NPlayMatchDlg()
{
	m_pNNPCMenuStatic1 = NULL;
	m_pNNPCMenuStatic2 = NULL;

    m_pButton_blackarmy_apply	 = NULL; 
	m_pButton_blackarmy_mvp		 = NULL;

	m_pButton_Dungeon = NULL;
	m_pButton_Battle = NULL;

	m_pButton_HuntingMapInfo	=	NULL	;
	m_pButton_BattleGuildSet	=	NULL	;
}

NPlayMatchDlg::~NPlayMatchDlg()
{
	NDelete(m_pNNPCMenuStatic1);
	NDelete(m_pNNPCMenuStatic2);

	NDelete(m_pButton_blackarmy_apply);
	NDelete(m_pButton_blackarmy_mvp);

	NDelete(m_pButton_Dungeon);
	NDelete(m_pButton_Battle);

	NDelete(m_pButton_HuntingMapInfo);
	NDelete(m_pButton_BattleGuildSet);

}

void NPlayMatchDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;
	// 대화상자가 없는 상태라면 만든다. 
	if( ! IsCreate() )
	{
		CInterfaceFile file;

		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 예전 방식 
		{
			file.LoadFile("NInterface/Data/NPlayMatchDlg/DLG_NPlayMatch_New.ddf");
			
		}
		else
		{
			file.LoadFile("NInterface/Data/NPlayMatchDlg/DLG_NPlayMatch.ddf");
		}
		file.CreatePopupDialog( this, NPLAYMATCH_DLG, "dialog_playmatch", NPlayMatchStaticProc );

		m_pNNPCMenuStatic1 = new CStatic( this );
		m_pNNPCMenuStatic2 = new CStatic( this );

		m_pButton_blackarmy_apply = new CButton( this );
		m_pButton_blackarmy_mvp	= new CButton( this );

		// 길드 사냥터 소유 방식 변경 
		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 예전 방식 
		{
			m_pButton_HuntingMapInfo = new CButton( this );
			m_pButton_BattleGuildSet = new CButton( this );
		}
		else 
		{
			m_pButton_Dungeon = new CButton( this );
			m_pButton_Battle = new CButton( this );
		}
		file.CreateControl( m_pNNPCMenuStatic1, NPLAYMATCH_DLG_STATIC_NPCMENU1, "statictext_npcmenu1" );
		file.CreateControl( m_pNNPCMenuStatic2, NPLAYMATCH_DLG_STATIC_NPCMENU2, "statictext_npcmenu2" );
		file.CreateControl( m_pButton_blackarmy_apply, NPLAYMATCH_DLG_BUTTON_BLACKARMY_APPLY, "button_blackarmy_apply" );
		file.CreateControl( m_pButton_blackarmy_mvp, NPLAYMATCH_DLG_BUTTON_BLACKARMY_MVP, "button_blackarmy_mvp" );
		// 길드 사냥터 소유 방식 변경 
		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 예전 방식 
		{
			file.CreateControl( m_pButton_HuntingMapInfo, NPLAYMATCH_DLG_BUTTON_HUNTINGMAPINFO, "button_HuntingMapInfo" );
			file.CreateControl( m_pButton_BattleGuildSet, NPLAYMATCH_DLG_BUTTON_BATTLEGUILDSET, "button_BattleGuildSet" );
		}
		else
		{
			file.CreateControl( m_pButton_Dungeon, NPLAYMATCH_DLG_BUTTON_DUNGEON, "button_dungeon" );
			file.CreateControl( m_pButton_Battle, NPLAYMATCH_DLG_BUTTON_BATTLE, "button_battle" );
			m_pButton_Battle->Enable( false );
		}
		m_pNNPCMenuStatic1->SetBorder(true);
		m_pNNPCMenuStatic1->SetBKColorUse(true);
		m_pNNPCMenuStatic2->SetBorder(true);
		m_pNNPCMenuStatic2->SetBKColorUse(true);

		TCHAR *pText = GetTxtFromMgr(5876);
		TCHAR buf[256];
		StringCchPrintf(buf, 256, pText, BLACKARMY_APPLY_MONEY);
		m_pNNPCMenuStatic2->SetText(buf);

		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_BATTLEZONE")), szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pNNPCMenuStatic1->SetFileName(szPortrait);
		m_pNNPCMenuStatic1->SetFontIndex(siPortraitFont);

	}
	else
	{
		DeleteDialog();
	}
}

void NPlayMatchDlg::Close()
{
	DeleteDialog();
}

void CALLBACK NPlayMatchDlg::NPlayMatchStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	NPlayMatchDlg *pThis = (NPlayMatchDlg*)pControllerMgr;
	pThis->NPlayMatchProc( nEvent, nControlID, pControl);
}

void CALLBACK NPlayMatchDlg::NPlayMatchProc( UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Close();
		}
		break;

	case NPLAYMATCH_DLG_BUTTON_BLACKARMY_APPLY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_BlackArmy_Apply clInfo;
					cltMsg clMsg(GAMEMSG_REQUEST_BLACKARMYAPPLY, sizeof(clInfo), (BYTE*)&clInfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NPLAYMATCH_DLG_BUTTON_BLACKARMY_MVP:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->CreateInterface( NBLACKARMYMVP_DLG );
				}
				break;
			}
		}
		break;
	case NPLAYMATCH_DLG_BUTTON_DUNGEON:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				pclclient->CreateInterface( NGUILDDUNGEON_DLG );
			}
		}break;

	case NPLAYMATCH_DLG_BUTTON_HUNTINGMAPINFO:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->CreateInterface( NGUILDDUNGEON_NEW_DLG );// 일단은 기존 창들을 띄운다
				}
				break;
			}
		}break;
	case NPLAYMATCH_DLG_BUTTON_BATTLEGUILDSET:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->CreateInterface( NREQUESTHUNTMAPWARREQUEST );	// 길드전 신청 
				}
				break;
			}
		}break;
	}
}

void NPlayMatchDlg::SetNPCText(TCHAR *text)
{
	if (text)
	{
		m_pNNPCMenuStatic2->SetText(text);
	}
}

