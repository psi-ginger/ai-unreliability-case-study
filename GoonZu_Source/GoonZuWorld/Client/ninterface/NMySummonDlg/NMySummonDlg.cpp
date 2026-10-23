#include "../Client/NInterface/NMySummonDlg/NMySummonDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Summon.h"
#include "..\..\Client\Client.h"
#include "Char\KindInfo\KindInfo.h"

extern cltCommonLogic* pclClient	;
stListBoxItemData HaveSummonData	;

CNMySummonDlg::CNMySummonDlg() : CDialogController()
{
	m_plSummonList	=	NULL	;
	m_pBtnOutput	=	NULL	;	
	m_pBtnInput		=	NULL	;		

	m_pBtSlot1		=	NULL	;
	m_pBtSlot2		=	NULL	;
	m_pBtSlot3		=	NULL	;

	m_stSummonSlot1	=	NULL	;
	m_stSummonSlot2	=	NULL	;
	m_stSummonSlot3	=	NULL	;
    
	m_OutlineSlot1	=	NULL	;// 백패널로 쓸 아웃라인
	m_OutlineSlot2	=	NULL	;// 백패널로 쓸 아웃라인
	m_OutlineSlot3	=	NULL	;// 백패널로 쓸 아웃라인

	m_pStaticNoName01	=	NULL	;
	m_pStaticNoName02	=	NULL	;
	m_pBtUseTicket		=	NULL	;
	m_pOutLine02		=	NULL	;
	m_pOutLine01		=	NULL	;

	siSlotSelectIndex	=	-1	;
	siListSelectIndex	=	-1	;
}

CNMySummonDlg::~CNMySummonDlg()
{
	SAFE_DELETE( m_plSummonList )	;
	SAFE_DELETE( m_pBtnOutput )		;
	SAFE_DELETE( m_pBtnInput )		;

	SAFE_DELETE( m_pBtSlot1 )		;
	SAFE_DELETE( m_pBtSlot2 )		;
	SAFE_DELETE( m_pBtSlot3 )		;
	
	
	SAFE_DELETE( m_stSummonSlot1 )	;
	SAFE_DELETE( m_stSummonSlot2 )	;
	SAFE_DELETE( m_stSummonSlot3 )	;
	
	SAFE_DELETE( m_OutlineSlot1 )	;
	SAFE_DELETE( m_OutlineSlot2 )	;
	SAFE_DELETE( m_OutlineSlot3 )	;

	SAFE_DELETE( m_pStaticNoName01 )	;
	SAFE_DELETE( m_pStaticNoName02 )	;
	SAFE_DELETE( m_pBtUseTicket )	;
	SAFE_DELETE( m_pOutLine02 )	;
	SAFE_DELETE( m_pOutLine01 )	;
	
	
}

void CNMySummonDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;
		m_plSummonList		=	new CList( this )	;
		m_pBtnOutput		=	new CButton( this )	;
		m_pBtnInput			=	new CButton( this )	;

		m_pBtSlot1			=	new CButton( this )	;
		m_pBtSlot2			=	new CButton( this )	;
		m_pBtSlot3			=	new CButton( this )	;
		
		m_stSummonSlot1		=	new CImageStatic( this )	;
		m_stSummonSlot2		=	new CImageStatic( this )	;
		m_stSummonSlot3		=	new CImageStatic( this )	;
		
		m_OutlineSlot1		=	new COutLine( this )	;
		m_OutlineSlot2		=	new COutLine( this )	;
		m_OutlineSlot3		=	new COutLine( this )	;

		m_pStaticNoName01	=	new CStatic( this )		;
		m_pStaticNoName02	=	new CStatic( this )		;
		m_pBtUseTicket		=	new CButton( this )		;
		m_pOutLine02		=	new COutLine( this )	;
		m_pOutLine01		=	new COutLine( this )	;

		file.LoadFile(TEXT("NInterface/Data/NMySummonDlg/DLG_NMySummon.ddf"))	;
		file.CreatePopupDialog( this, NMYSUMMON_DLG, TEXT("dialog_MySummonDlg"), StaticCallBackDialogCNMySummonDlg )	;
		file.CreateControl( m_plSummonList, NMYSUMMON_LIST_SUMMONLIST, TEXT("listbox_mysummonlist") )	;
		file.CreateControl( m_pStaticNoName01, NMYSUMMON_STATIC_NONAME01, TEXT("statictext_NONAME1") )	;
		file.CreateControl( m_pStaticNoName02, NMYSUMMON_STATIC_NONAME02, TEXT("statictext_NONAME2") )	;
		//티켓사용 버튼 삭제.
		//file.CreateControl( m_pBtUseTicket, NMYSUMMON_BUTTON_USETICKET, TEXT("button_useticket") )	;
		file.CreateControl( m_pBtnOutput, NMYSUMMON_BUTTON_INPUTSLOT, TEXT("button_input") )	;
		file.CreateControl( m_pBtnInput, NMYSUMMON_BUTTON_OUTPUTSLOT, TEXT("button_output") )	;
		file.CreateControl( m_pBtSlot1, NMYSUMMON_BUTTON_SLOT1, TEXT("button_slot1") )	;
		file.CreateControl( m_pBtSlot2, NMYSUMMON_BUTTON_SLOT2, TEXT("button_slot2") )	;
		file.CreateControl( m_pBtSlot3, NMYSUMMON_BUTTON_SLOT3, TEXT("button_slot3") )	;

		file.CreateControl( m_stSummonSlot1, NMYSUMMON_IMAGESTATIC_SLOT1,  TEXT("imagestatic_slot1") )	;
		file.CreateControl( m_stSummonSlot2, NMYSUMMON_IMAGESTATIC_SLOT2,  TEXT("imagestatic_slot2") )	;
		file.CreateControl( m_stSummonSlot3, NMYSUMMON_IMAGESTATIC_SLOT3,  TEXT("imagestatic_slot3") )	;

		file.CreateControl( m_pOutLine01, NMYSUMMON_OUTLINE_NONAME01, TEXT("outline_NONAME1") )	;
		file.CreateControl( m_pOutLine02, NMYSUMMON_OUTLINE_NONAME02, TEXT("outline_NONAME2") )	;
		file.CreateControl( m_OutlineSlot1,  NMYSUMMON_OITLINE_SLOT1,  TEXT("outline_slot1") )	;
		file.CreateControl( m_OutlineSlot2,  NMYSUMMON_OITLINE_SLOT2,  TEXT("outline_slot2") )	;
		file.CreateControl( m_OutlineSlot3,  NMYSUMMON_OITLINE_SLOT3,  TEXT("outline_slot3") )	;
		
		m_stSummonSlot1->Show(false);
		m_stSummonSlot2->Show(false);
		m_stSummonSlot3->Show(false);

		SI16 siNumWidth		= 25;
		SI16 siKindWidth	= 120;
		SI16 siNameWidth	= 90;
		SI16 siLevelWidth	= 30;
		SI16 siPeriodWidth	= m_plSummonList->GetWidth() - siNumWidth - siKindWidth - siNameWidth - siLevelWidth;

		TCHAR* pText = GetTxtFromMgr(8759);
		m_plSummonList->SetColumn( 0,	siNumWidth,		pText );
		pText = GetTxtFromMgr(8760);
		m_plSummonList->SetColumn( 1,	siKindWidth,	pText );
		pText = GetTxtFromMgr(8761);
		m_plSummonList->SetColumn( 2,	siNameWidth,	pText );
		pText = GetTxtFromMgr(8762);
		m_plSummonList->SetColumn( 3,	siLevelWidth,	pText );
		pText = GetTxtFromMgr(8763);
		m_plSummonList->SetColumn( 4,	siPeriodWidth,	pText );

		m_plSummonList->SetBorder(true);
		m_plSummonList->SetBKColorUse(true);
		m_plSummonList->Refresh();

		LodaMyhaveSummonList();	
		LodaMyOutSummon();
		SetActionTimer(500);
		SetTimerOnOff( true );

		m_OutlineSlot1->SetFontIndex(1);
		m_OutlineSlot2->SetFontIndex(1);
		m_OutlineSlot3->SetFontIndex(1);
	}
	else
	{
		DeleteDialog();
	}
}

void CNMySummonDlg::Action()
{
	
}

void CALLBACK CNMySummonDlg::StaticCallBackDialogCNMySummonDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMySummonDlg *pThis = (CNMySummonDlg *)pControllerMgr;
	pThis->CallBackDialogCNMySummonDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNMySummonDlg::CallBackDialogCNMySummonDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NMYSUMMON_LIST_SUMMONLIST :
		{
			switch( nEvent ) {
			case EVENT_LISTBOX_SELECTION:
				SetListSelectItem( m_plSummonList->GetSelectedIndex() );
				break;
			}
		}
	case NMYSUMMON_BUTTON_OUTPUTSLOT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//siSlotSelectIndex ;											// 소환수를 넣을 슬롯.
					SI16 siListIndex = m_plSummonList->GetSelectedIndex();
					TCHAR* szname = m_plSummonList->GetText( siListIndex, 2 );
					TCHAR* szUseDate = m_plSummonList->GetText( siListIndex, 4 );

					cltKindInfo* pclKI		=	NULL		;
					cltSummonInfo *pclSummonInfo = &pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
					// 빼야할 슬롯의 소환수 정보를 받아 와서.
					cltSimpleSummon *pclSummonBase = pclSummonInfo->GetSummonBase(siListIndex);
					if( pclSummonBase == NULL ) return;

					// 현제 슬롯이 사용 가능한 슬롯인지 확인한다.// 현제 슬롯은 사용 이 종료되었습니다.
					if(siListIndex > 2 && 
						pclClient->pclCM->CR[1]->pclCI->clBI.bSummonExtraSlot == false &&
						pclClient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate < pclclient->clClientDate.GetDateVary() )
					{
						TCHAR * pTitle = GetTxtFromMgr(449) ;
						TCHAR * pText = GetTxtFromMgr( 8773);
						if ( !pTitle || !pText ) break ;
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);

						// 활성화 안됐다면 메세지를 출력하고 리턴시킽다.
						return;
					}


					// 활성화 될 수 있는 슬롯을 고른다.
					bool	indexSelect = true;
					SI32 SlotSelectIndex = GetSelectSlot();
					
					// 선택된 것이 없으면 빈공간을 찾는다.
					if( SlotSelectIndex < 0 || SlotSelectIndex >= MAX_SUMMON_OWN)
					{
						pclSummonInfo->GetEmptySummonIndex(&SlotSelectIndex);
					}

					// 더 이상 빈공간이 없다.
					if( SlotSelectIndex < 0 || SlotSelectIndex >= MAX_SUMMON_OWN)
						return;

					if( pclSummonBase )
					{
						SI32 siKind				=	pclSummonBase->siKind	;
						// 해당 소환수의 카인드 값이 정상 적인지 체크.
						if( pclClient->pclKindInfoSet->IsValidKind( siKind ) == false )	return	;
						if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
						{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[ siKind ];	}
						else
						{	pclKI = pclClient->pclKindInfoSet->pclKI[ siKind ];			}
						if ( pclKI	==	NULL )	return	;

						// 해당 슬롯에 넣어 달라고 서버로 보낸다
						cltGameMsgRequest_SlotIn clinfo( pclSummonBase->siSummonID, SlotSelectIndex );
						cltMsg clMsg(GAMEMSG_REQUEST_SUMMONSLOTIN, sizeof(clinfo), (BYTE*)&clinfo);
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					}

					SetSelectSlot(-1);
					SetListSelectItem(-1);

					m_OutlineSlot1->SetFontIndex(1);
					m_OutlineSlot2->SetFontIndex(1);
					m_OutlineSlot3->SetFontIndex(1);
				}
				break;
			}
		}
		break;

	case NMYSUMMON_BUTTON_INPUTSLOT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltKindInfo* pclKI		=	NULL		;
					cltSummonInfo *pclSummonInfo = &pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
					// 빼야할 슬롯의 소환수 정보를 받아 와서.
					SI32 SlotSelectIndex = GetSelectSlot();

					cltSummon *pclSummon = pclSummonInfo->GetSummon( SlotSelectIndex );
					if(pclSummon == NULL) return;
					if(pclSummon)
					{
						SI32 siKind				=	pclSummon->siKind	;
						// 해당 소환수의 카인드 값이 정상 적인지 체크.
						if( pclClient->pclKindInfoSet->IsValidKind( siKind ) == false )	return	;
						if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
						{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[ siKind ];	}
						else
						{	pclKI = pclClient->pclKindInfoSet->pclKI[ siKind ];			}
						if ( pclKI	==	NULL )	return	;
						// 해당 소환수를 빼는 메세지를 날린다.
						cltGameMsgRequest_SlotOut clinfo( pclSummon->siSummonID, SlotSelectIndex );
						cltMsg clMsg(GAMEMSG_REQUEST_SUMMONSLOTOUT, sizeof(clinfo), (BYTE*)&clinfo);
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					SetSelectSlot(-1);
					SetListSelectItem(-1);

					m_OutlineSlot1->SetFontIndex(1);
					m_OutlineSlot2->SetFontIndex(1);
					m_OutlineSlot3->SetFontIndex(1);
				}
				break;
			}
		}
		break;
	case NMYSUMMON_BUTTON_SLOT1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetSelectSlot( 0 )	;// 선택한 슬롯의 인덱스

					m_OutlineSlot1->SetFontIndex(0);
					m_OutlineSlot2->SetFontIndex(1);
					m_OutlineSlot3->SetFontIndex(1);
				}								  
				break;							  
			}									  
		}
		break; 
	case NMYSUMMON_BUTTON_SLOT2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetSelectSlot( 1 )	;// 선택한 슬롯의 인덱스
					m_OutlineSlot1->SetFontIndex(1);
					m_OutlineSlot2->SetFontIndex(0);
					m_OutlineSlot3->SetFontIndex(1);
				}
				break;
			}
		}
		break;
	case NMYSUMMON_BUTTON_SLOT3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: 
				{
					SetSelectSlot( 2 )	;// 선택한 슬롯의 인덱스
					m_OutlineSlot1->SetFontIndex(1);
					m_OutlineSlot2->SetFontIndex(1);
					m_OutlineSlot3->SetFontIndex(0);
				}
				break;
			}  
		}
		break;
	}
}
void CNMySummonDlg::LodaMyOutSummon(){
	SI32 siPortraitFont		=	0	;
	SI32 siPortraitIndex	=	0	;
	SI32 siSummonPortrait	=	0	;
	TCHAR szPortrait[256]	=	TEXT("")	;
	cltKindInfo* pclKI		=	NULL		;
	m_stSummonSlot1->Show( false ) ;
	m_stSummonSlot2->Show( false ) ;
	m_stSummonSlot3->Show( false ) ;

	cltSummonInfo *pclSummonInfo = &pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL) return;
	for ( SI16 siSummonIndex = 0 ; siSummonIndex < MAX_SUMMON_OWN; siSummonIndex++)
	{
		cltSummon *pclSummon = pclSummonInfo->GetSummon(siSummonIndex);
		if(pclSummon)
		{
			SI32 siKind				=	pclSummon->siKind	;
			// 해당 소환수의 카인드 값이 정상 적인지 체크.
			if( pclClient->pclKindInfoSet->IsValidKind( siKind ) == false )	continue;						

			if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
			{	pclKI = pclClient->pclKindInfoSet->pclSummonKI[ siKind ];	}
			else
			{	pclKI = pclClient->pclKindInfoSet->pclKI[ siKind ];
			}
			if ( pclKI	==	NULL )	continue	;

			siSummonPortrait	=	pclKI->siSummonPortrait	;
			siPortraitFont		=	( siSummonPortrait *2 )	;
			pclClient->pclKindInfoSet->GetNPCImageFileAndFontsmall(siKind, szPortrait, sizeof(szPortrait), &siSummonPortrait);
			switch(siSummonIndex) {
			case 0:
				// 소환수사진을 보여준다.
				m_stSummonSlot1->Show( true ) ;
				m_stSummonSlot1->SetFontIndex( siPortraitFont );
				break;
			case 1:
				// 소환수사진을 보여준다.
				m_stSummonSlot2->Show( true ) ;
				m_stSummonSlot2->SetFontIndex( siPortraitFont );
				break;
			case 2:
				// 소환수사진을 보여준다.
				m_stSummonSlot3->Show( true ) ;
				m_stSummonSlot3->SetFontIndex( siPortraitFont );
				break;
			}
		}		
	}
	
}
void CNMySummonDlg::LodaMyhaveSummonList(){
	SI32			siListIndex		=	0		;
	cltKindInfo*	pclKI			=	NULL	;
	TCHAR			SummonName[256]	=	TEXT("");
	cltDate			cldate						;
	SI32			siDate			= 0			;
	TCHAR			timebuf[64]		= TEXT("")	;

	cltSummonInfo *pclSummonInfo = &pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL) return;

	m_plSummonList->Clear();
	
	COLORREF color = 0x00000000;	// 글자색.
	
	for ( SI16 siSummonIndex = 0 ; siSummonIndex < MAX_HAVE_SUMMON; siSummonIndex++)
	{
		cltSimpleSummon *pclSummonBase = pclSummonInfo->GetSummonBase(siSummonIndex);
		if(pclSummonBase)
		{
			SI32 siKind	= pclSummonBase->siKind;
			HaveSummonData.Init();
			// 해당 소환수의 카인드 값이 정상 적인지 체크.
			if( pclClient->pclKindInfoSet->IsValidKind( siKind ) == true )
			{
				if(pclClient->IsCountrySwitch(Switch_Summon_Separate))
				{	
					pclKI = pclClient->pclKindInfoSet->pclSummonKI[ siKind ];	
				}
				else
				{
					pclKI = pclClient->pclKindInfoSet->pclKI[ siKind ];
				}
				if( pclKI == NULL )	continue;
								
				for(SI32 Index = 0; Index < MAX_SUMMON_OWN; ++Index )
				{
					if( pclSummonInfo->GetSummon(Index)->siSummonID == pclSummonBase->siSummonID ){
						color = 0x00ff0000;
						break;
					}
					else
					{
						color = 0x00000000;
					}
						
				}
				StringCchPrintf(SummonName, 256, TEXT("%s"), pclKI->GetName());
				HaveSummonData.Set( 0, SI32ToString( siSummonIndex+1 ), color  );
				HaveSummonData.Set( 1, SummonName, color )	;							// 종류
				HaveSummonData.Set( 2, pclSummonBase->szName, color );					// 이름
				HaveSummonData.Set( 3, SI16ToString( pclSummonBase->siLevel), color  );	// 레벨
			}
		}
		if ( siSummonIndex >2) {
			switch( siSummonIndex ) {
				// 해당 인덱스에 따라서 날짜를 만들어 낸다.
				case 3:
					{
						siDate = pclClient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate;// 각슬롯의 날짜
					}
					break;
				case 4:
					{
						siDate = pclClient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate;
					}
					break;
				case 5:
					{
						siDate = pclClient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate;
					}
					break;
				case 6:
					{
						siDate = pclClient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate;
					}
					break;
				case 7:
					{
						siDate = pclClient->pclCM->CR[1]->pclCI->clBI.siSummonExtraSlotDate;
					}
					break;
			}
			if ( siDate > 0 ){
				cldate.MakeFullDateFromDateVary( siDate )	;
				cldate.GetDateText(timebuf)					;
			}
			else {
				TCHAR* pText = GetTxtFromMgr( 8775 )	;
				strncpy(timebuf, pText, sizeof(timebuf));
			}
		}
		else{
			if ( siSummonIndex < 3 ){
				TCHAR* pText = GetTxtFromMgr( 8774 )	;
				strncpy(timebuf, pText, sizeof(timebuf));
			}
			
		}
		HaveSummonData.Set( 4, timebuf );// 사용기간
		m_plSummonList->AddItem( &HaveSummonData );	
	}
	m_plSummonList->Refresh();
}
SI16 CNMySummonDlg::GetListSelectItem(){
		return siListSelectIndex;
}
SI16 CNMySummonDlg::GetSelectSlot(){
		return siSlotSelectIndex ;
}
void CNMySummonDlg::SetListSelectItem( SI16 siListSelectItem ){
	siListSelectIndex = siListSelectItem ;
}

void CNMySummonDlg::SetSelectSlot( SI16 siSelectSlot ){
	siSlotSelectIndex = siSelectSlot ;
}