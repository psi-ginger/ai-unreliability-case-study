#include "NCharSummonReset.h"
#include "NCharSummon.h"

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"
#include "Char\CharManager\CharManager.h"

#include "..\Client\Client.h"
#include "./NCharGreetingChange.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"

#include "../CommonLogic/MsgType-Person.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/InterfaceMgr/Interface/Button.h"
#include "../../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../../Client/InterfaceMgr/Interface/Static.h"
#include "../../Client/InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "Msg\MsgType-Horse.h"
#include "Msg\MsgType-Summon.h"
#include "NCharStatus.h"

#include "../common/Event/event.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNCharSummonReset::CNCharSummonReset()
{
	m_pEditbox_Info	= NULL;
	m_pStatictext_1	= NULL;
	m_pButton_ReSet	= NULL;
}

CNCharSummonReset::~CNCharSummonReset()
{
	SAFE_DELETE( m_pEditbox_Info );
	SAFE_DELETE( m_pStatictext_1 );
	SAFE_DELETE( m_pButton_ReSet );
}

void CNCharSummonReset::Create()
{
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NChangeGreetingDlg/DLG_NCharStatus_NSummonReset.ddf"));
	file.CreatePopupDialog( this, NCHARSUMMONRESET_DLG, TEXT("dialog_summonreset"), NCharSummonResetStaticProc );

	m_pEditbox_Info	= new CEdit( this );
	m_pStatictext_1	= new CStatic( this );
	m_pButton_ReSet	= new CButton( this );

	file.CreateControl( m_pEditbox_Info,	NCHARSUMMONRESET_EDITBOX,	TEXT("editbox_summoninfo") );
	file.CreateControl( m_pStatictext_1,	NCHARSUMMONRESET_STATIC,	TEXT("statictext_NONAME1") );
	file.CreateControl( m_pButton_ReSet,	NCHARSUMMONRESET_BUTTON,	TEXT("button_summonreset") );
}

void CALLBACK CNCharSummonReset::NCharSummonResetStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNCharSummonReset* pThis = (CNCharSummonReset*)pControllerMgr;
	pThis->NCharSummonResetProc( nEvent, nControlID, pControl );
}

void CALLBACK CNCharSummonReset::NCharSummonResetProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NCHARSUMMONRESET_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendSummonStatusInitMessageToServer();
				}
				break;
			}
		}
		break;
	} // switch( nControlID ) 종료
}

void CNCharSummonReset::SendSummonStatusInitMessageToServer()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( pclclient == NULL )
	{
		return;
	}

	/*if ( pclclient->m_pDialog[CLTNSUMMONSTATUS_DLG] == NULL )
	{
		return; 릴리즈시 이부분에서 리턴 되어 소환수 초기화 이용권이 사용되지 않는다.
	}*/

	SI32 id = 1;
	if(pclclient->pclCM->IsAlive(id) == false)
	{
		return ;
	}

	

	cltSummonInfo* pclSummonInfo = &pclclient->pclCM->CR[id]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL)
	{
		return;
	}

	//=======================================
	// ComboList의 소환수를 얻어온다.
	//=======================================
	TCHAR	szName[ MAX_SUMMON_NAME ] = TEXT("");
	SI32	siSummonIndex = 0;
	
	if ( FALSE == ((cltNSummonStatus*)((cltNCharStatus*)pclclient->m_pDialog[CLTNCHARSTATUS_DLG])->m_pTabDlg2)->GetComboSummonNameIndex(szName, MAX_SUMMON_NAME, &siSummonIndex ) )
	{
		return;
	}

	cltSummon *pclSummon = pclSummonInfo->GetSummon(siSummonIndex);
	if ( pclSummon==NULL )
	{
		return;
	}

	//[진성] 캐릭터 상태 초기화 일정기간동안 마음것 사용. 
	if(pclClient->pclEventTimeManager->InEventTime("char_ability_init", &pclclient->sTime) )
	{
		BYTE TempBuffer[ 128 ];
		SI16 Size = sizeof( id ) + sizeof( SI32 );
		memcpy( TempBuffer, &id, sizeof( id ) );
		memcpy( &TempBuffer[ sizeof( id ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );

		SI32 id = *((SI32*)TempBuffer);
		SI32 siSummonID = *((SI32*)&TempBuffer[ 4 ] );

		cltGameMsgRequest_Summon_StatusInit clinfo( pclclient->pclCM->CR[id]->GetCharUnique(), siSummonID);
		cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_STATUSINIT, sizeof(clinfo), (BYTE*)&clinfo);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
		return;
	}

	TCHAR buffer[256] = TEXT("") ;

	//cyj 상태초기화이용권 유니크변경 14090->24030
	//SI32 itemunique = 14090 ;  // 상태초기화이용권
	SI32 itemunique			= ITEMUNIQUE(24030) ;  // 상태초기화이용권
	SI32 siEventItemUnique	= ITEMUNIQUE(24125) ;  // 상태초기화이용권(이벤트)
	if (	pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 &&
			pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(siEventItemUnique) <= 0)
	{
		TCHAR* pText = GetTxtFromMgr(608);
		StringCchPrintf(buffer, 256, pText);

		TCHAR* pTitle = GetTxtFromMgr(605);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,buffer);
		return ;
	}

	TCHAR* pText = GetTxtFromMgr(2840);
	TCHAR* pTitle = GetTxtFromMgr(605);
	StringCchCopy(buffer, 256, pText);

	SI32 siCharUnique = pclclient->pclCM->CR[id]->GetCharUnique();

	stMsgBox MsgBox;
	MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 28 );
	BYTE TempBuffer[ 128 ];
	SI16 Size = sizeof( siCharUnique ) + sizeof( SI32 );
	memcpy( TempBuffer, &siCharUnique, sizeof( siCharUnique ) );
	memcpy( &TempBuffer[ sizeof( siCharUnique ) ], &(pclSummon->siSummonID), sizeof( SI32 ) );
	pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

}

void CNCharSummonReset::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 28 :	// 상태초기화이용권
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 siCharUnique = *((SI32*)pData);
				SI32 siSummonID = *((SI32*)&pData[ 4 ] );

				cltGameMsgRequest_Summon_StatusInit clinfo( siCharUnique, siSummonID);
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_STATUSINIT, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
	return;
}
void CNCharSummonReset:: ViewStoneResetDlg( TCHAR* pText )
{
	m_pEditbox_Info->SetText( pText);


}
