#include "NFeastDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Feast.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../FeastEffect/FeastEffectManager.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NFeastChatDlg::NFeastChatDlg()
{

}

NFeastChatDlg::~NFeastChatDlg()
{

}

void NFeastChatDlg::InitDlg()
{

	SetControlMap( BUTTON_INPUT, TEXT("button_input") );
	SetControlMap( EDIT_CHAT, TEXT("editbox_chat") );
	SetControlMap( EDIT_EFFECT, TEXT("editbox_effectname"));
	SetControlMap( LIST_EFFECT, TEXT("listbox_effect"));
	SetControlMap( BUTTON_USEEFFECT, TEXT("button_useeffect"));

	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_EFFECT ];

	pList->SetBKColorUse( true );
	pList->SetBorder( true );

	int width[] = { 40, 66, 216, 18 };

	TCHAR* pText[4];

	pText[0] = GetTxtFromMgr(1641);
	pText[1] = GetTxtFromMgr(1782);
	pText[2] = GetTxtFromMgr(5223);
	pText[3] = TEXT("");

	for(SI16 i=0; i<3; i++ )
	{
		pList->SetColumn( i, width[i], pText[i] );
	}

	pList->Refresh();

	ShowEffectInfo();
}



void NFeastChatDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	TCHAR buffer[ 1024 ];

	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	cltClient* pclclient = (cltClient*)pclClient;

	CList *pList = (CList*)m_pControlMap[ LIST_EFFECT ];

	CASE_CONTROL( BUTTON_INPUT )
	{

		MStrCpy( buffer, GetEditText( EDIT_CHAT ), 128 );

		SI32 k;
		for ( k = 0 ; k < sizeof(buffer) ; k ++ )
		{
			if( buffer[k] == '\r' ||
				buffer[k] == '\n' )
			{
				buffer[k] = ' ';
			}
		}

		cltGameMsgRequest_FeastSendChat sendmsg( buffer );
	
		cltMsg clMsg(GAMEMSG_REQUEST_FEAST_SENDCHAT, sendmsg.siSize, (BYTE*)&sendmsg);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);				

	} else CASE_CONTROL( LIST_EFFECT )
	{
        switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				SI16 siIndex = pList->GetSelectedIndex();

				if(siIndex < 0 ) return;

				stListBoxItemData* pTmpListBoxItemData = NULL;
				pTmpListBoxItemData = pList->GetItemData( siIndex );

				if( pTmpListBoxItemData == NULL ) return;
			
				pFeastManager->siChiefEffectSelectIndex = pTmpListBoxItemData->siParam[0];

				SetEditText( EDIT_EFFECT, pTmpListBoxItemData->strText[2] );
			}
			break;
		}

	} else CASE_CONTROL( BUTTON_USEEFFECT )
	{
//#if defined(_LEON_FEAST_EFFECT)

		if( pFeastManager->siChiefEffectSelectIndex < 0 || pFeastManager->siChiefEffectSelectIndex > cltFeastManager::MAX_FEAST_EFFECT_NUM ) return;

 		pclclient->m_pFeastEffectManager->StopFeastEffect();

		TCHAR* pTitle = GetTxtFromMgr(5227);
		TCHAR* pText = GetTxtFromMgr(5230);

		SI32 siFeastStatus = 0;
		SI32 personid = 0;
		SI32 reserverid = 0;

		// 연회 중인지 체크
		siFeastStatus = pclClient->pclFeastManager->clClientStrInfo.clFeastStrInfo.clFeastInfo.siStatus;
		//KHY - 0710 -연회중 조건 수정 : 0=없음. 1=연회중,  2 = 예약중
		if( siFeastStatus != 1 ) 
		{
			TCHAR* pTempText = GetTxtFromMgr(5243);
			SetMainExplain( pTempText );
			return;
		}
		
		// 연회자 인지 체크
		personid = pclCM->CR[1]->pclCI->GetPersonID();
		reserverid = pclClient->pclFeastManager->clClientStrInfo.clFeastStrInfo.clFeastResvInfo.siPersonID ;

		if( personid != reserverid ) 
		{
			TCHAR* pTempText = GetTxtFromMgr(5243);
			SetMainExplain( pTempText );
			return;
		}

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
		pclclient->SetMsgBox( &MsgBox, NULL, 0 );

//#endif        
	}

}

void NFeastChatDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	SI32 id = 1;

	switch( MsgIndex )
	{
	case 0:
		{
			if(RetValue)
			{
				cltGameMsgRequest_FeastUseEffect sendmsg( &(pFeastManager->siChiefEffectSelectIndex) );
		        
				cltMsg clMsg(GAMEMSG_REQUEST_FEAST_USEEFFECT, sizeof(sendmsg), (BYTE*)&sendmsg );
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			}
		}
		break;

	}
}

// 이펙트 리스트에 이펙트 추가
void NFeastChatDlg::ShowEffectInfo()
{
	cltFeastManager *pFeastManager = ((cltFeastManager *)m_pStrManager);

	CList *pList = (CList*)m_pControlMap[ LIST_EFFECT ];

	/*cyj TextMgr값으로 변환
	TCHAR* FeastEffectName[cltFeastManager::MAX_FEAST_EFFECT_NUM] = {

			// 하늘에서 떨어지는것
			TEXT("꽃님"),
			TEXT("꽃가루"),
			TEXT("종이리본"),

			// 하늘에서 떨어지는것
			TEXT("사랑불꽃"),
			TEXT("손벽"),
			TEXT("불꽃"),
			TEXT("회전불꽃"),
			TEXT("하트불꽃"),

			// 2개가 하나 되는것
			TEXT("박"),
			TEXT("상자"),
			TEXT("폭죽"),
	};

	TCHAR* FeastEffectKindName[cltFeastManager::MAX_FEAST_EFFECT_NUM] = {

			// 하늘에서 떨어지는것
			TEXT("리본류"),
			TEXT("리본류"),
			TEXT("리본류"),

			// 하늘에서 떨어지는것
			TEXT("불꽃류"),
			TEXT("폭죽류"),
			TEXT("불꽃류"),
			TEXT("불꽃류"),
			TEXT("불꽃류"),

			// 2개가 하나 되는것
			TEXT("폭죽류"),
			TEXT("폭죽류"),
			TEXT("폭죽류"),
	};
	*/
	SI32 FeastEffectName[cltFeastManager::MAX_FEAST_EFFECT_NUM] = {

		// 하늘에서 떨어지는것
			5711,
			5712,
			5713,

			// 하늘에서 떨어지는것
			5714,
			5715,
			5716,
			5717,
			5718,

			// 2개가 하나 되는것
			5719,
			5720,
			5721,
	};

	SI32 FeastEffectKindName[cltFeastManager::MAX_FEAST_EFFECT_NUM] = {

			// 하늘에서 떨어지는것
			5708,
			5708,
			5708,

			// 하늘에서 떨어지는것
			5709,
			5710,
			5709,
			5709,
			5709,

			// 2개가 하나 되는것
			5710,
			5710,
			5710,
	};

	TCHAR buffer[10];

	// List의 Item 모두 지우기
	pList->Clear();


	for( SI16 i = 0; i < cltFeastManager::MAX_FEAST_EFFECT_NUM; i++)
	{
		stListBoxItemData tmpListBoxItemData;

		tmpListBoxItemData.Init();

		StringCchPrintf(buffer, 10, TEXT("%d"), i + 1);

		tmpListBoxItemData.Set( 0, buffer );
		tmpListBoxItemData.siParam[0] = i;

		//cyj 텍스트 메니저로 불러옴
		//tmpListBoxItemData.Set( 1, FeastEffectKindName[ i ] );
		//tmpListBoxItemData.Set( 2, FeastEffectName[ i ] );

		tmpListBoxItemData.Set( 1, GetTxtFromMgr(FeastEffectKindName[ i ]) );
		tmpListBoxItemData.Set( 2, GetTxtFromMgr(FeastEffectName[ i ]) );

		pList->AddItem( &tmpListBoxItemData );
	}

	pList->Refresh();
}
