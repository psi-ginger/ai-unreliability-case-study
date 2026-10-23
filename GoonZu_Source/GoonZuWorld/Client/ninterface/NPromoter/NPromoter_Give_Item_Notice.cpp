#include "NPromoter_Give_Item_Notice.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"

NPromoter_Give_Item_Notice::NPromoter_Give_Item_Notice()
{
	m_siButtonType = NONE;
}

NPromoter_Give_Item_Notice::~NPromoter_Give_Item_Notice()
{
	
}

void NPromoter_Give_Item_Notice::Create()
{
	if( IsCreate() ) return;

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NPromoterDlg/NPromoter_Give_Item_Notice.ddf"));
	file.CreatePopupDialog( this, NPROMOTER_GIVE_ITEM_NOTICE, TEXT("NPromoter_Give_Item_Notice"), StatiNPromoter_Give_Item_NoticeProc, false );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			NPROMOTER_GIVE_ITEM_NOTICE_WHISPER,		this ), NPROMOTER_GIVE_ITEM_NOTICE_WHISPER,		"button_whisper"			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,			NPROMOTER_GIVE_ITEM_NOTICE_NOTICE,		this ),	NPROMOTER_GIVE_ITEM_NOTICE,				"editbox_notice"			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	NPROMOTER_GIVE_ITEM_NOTICE_TREASURE_BOX,this ),	NPROMOTER_GIVE_ITEM_NOTICE_TREASURE_BOX,"imagestatic_treasure_box"	);

	SetRecommendeeText(1, "test", "Yfire", "33");
}

void NPromoter_Give_Item_Notice::Action()
{

}

void NPromoter_Give_Item_Notice::SetRecommendText( SI32 _bGiveItemSuccess, const TCHAR* _pTempName, const TCHAR* _pItemName, const TCHAR* _pItemNum  )
{
	m_siButtonType = PERSONAL_INFO_CHANGE;

	if( _bGiveItemSuccess )
	{
		m_InterfaceFactory.GetImageStatic(NPROMOTER_GIVE_ITEM_NOTICE_TREASURE_BOX)->SetFontIndex( GIVE_ITEM_SUCCESS );					// 성공 이미지.
		
		NTCHARString256	 kText;
			 if( 'Y' == _pItemName[0] ) kText = GetTxtFromMgr(30278);
		else if( 'N' == _pItemName[0] ) kText = GetTxtFromMgr(40174);

		
		kText.Replace( "#ItemName#", &_pItemName[1] );
		kText.Replace( "#ItemNum#" , _pItemNum );

		m_InterfaceFactory.GetEdit(NPROMOTER_GIVE_ITEM_NOTICE_NOTICE)->SetText( kText );
		m_InterfaceFactory.GetButton( NPROMOTER_GIVE_ITEM_NOTICE_WHISPER )->Show( false );								// 추천한놈이 성공했으면 버튼이 보이지 말자. 필요없다 ㅋ
	}
	else
	{
		m_InterfaceFactory.GetImageStatic(NPROMOTER_GIVE_ITEM_NOTICE_TREASURE_BOX)->SetFontIndex( GIVE_ITEM_FAIL );
		
		m_InterfaceFactory.GetEdit(NPROMOTER_GIVE_ITEM_NOTICE_NOTICE)->SetText( GetTxtFromMgr(30279) );
		m_InterfaceFactory.GetButton( NPROMOTER_GIVE_ITEM_NOTICE_WHISPER )->SetText( GetTxtFromMgr(40005) );
	}
}

void NPromoter_Give_Item_Notice::SetRecommendeeText( SI32 _bGiveItemSuccess, const TCHAR* _pRecommendName, const TCHAR* _pItemName, const TCHAR* _pItemNum )
{
	m_siButtonType = WHIPERER;

	StringCchCopy( m_NewbieID, sizeof(m_NewbieID), _pRecommendName );

	if( _bGiveItemSuccess )
	{
		m_InterfaceFactory.GetImageStatic( NPROMOTER_GIVE_ITEM_NOTICE_TREASURE_BOX )->SetFontIndex( GIVE_ITEM_SUCCESS );
	
		NTCHARString256	 kText;
			 if( 'Y' == _pItemName[0] ) kText = "#ItemName# #ItemNum#";//GetTxtFromMgr(30280);
		else if( 'N' == _pItemName[0] ) kText = GetTxtFromMgr(40175);
		
		kText.Replace( "#RecommendName#",	_pRecommendName );
		kText.Replace( "#ItemName#",		&_pItemName[1] );
		kText.Replace( "#ItemNum#" ,		_pItemNum );

		m_InterfaceFactory.GetEdit( NPROMOTER_GIVE_ITEM_NOTICE_NOTICE )->SetText( kText );
		m_InterfaceFactory.GetButton( NPROMOTER_GIVE_ITEM_NOTICE_WHISPER )->SetText( GetTxtFromMgr(40006) );
	}
	else
	{
        m_InterfaceFactory.GetImageStatic(NPROMOTER_GIVE_ITEM_NOTICE_TREASURE_BOX)->SetFontIndex( GIVE_ITEM_FAIL );
		
		m_InterfaceFactory.GetEdit( NPROMOTER_GIVE_ITEM_NOTICE_NOTICE )->SetText( GetTxtFromMgr(30281)  );
		m_InterfaceFactory.GetButton( NPROMOTER_GIVE_ITEM_NOTICE_WHISPER )->SetText( GetTxtFromMgr(40007) );
	}
}

void CALLBACK NPromoter_Give_Item_Notice::StatiNPromoter_Give_Item_NoticeProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPromoter_Give_Item_Notice *pThis = (NPromoter_Give_Item_Notice *)pControllerMgr;
	pThis->NPromoter_Give_Item_NoticeProc( nEvent, nControlID, pControl );
}
 
void CALLBACK NPromoter_Give_Item_Notice::NPromoter_Give_Item_NoticeProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case NPROMOTER_GIVE_ITEM_NOTICE_WHISPER:
			{
				switch( m_siButtonType )
				{
					case PERSONAL_INFO_CHANGE:
						{		
							ShellExecute( NULL, NULL, TEXT("http://www.newhljz.cn/"), NULL, NULL, SW_SHOWNORMAL ); 
						}
						break;
					case WHIPERER:
						{
							if( _tcslen(m_NewbieID) )
								pclclient->RequestSecretChatByCharName(m_NewbieID);
						}
						break;
				}
			}
			break;
	}
}


