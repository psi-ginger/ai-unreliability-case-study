#include "../Client/NInterface/NSummonUnitBuyNpcDlg/NSummonUnitBuyNpcDlg.h"

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
#include "..\..\Client\Music\Music.h"
#include "Char\KindInfo\KindInfo.h"

#include "../../CommonLogic/Msg/MsgType-TradeMerchant.h"

extern cltCommonLogic* pclClient;

CNSummonUnitBuyNpcDlg::CNSummonUnitBuyNpcDlg() : CDialogController()
{
	m_siSummonUnitIndex = -1;
	m_siUseTicketNumber = 0;	//	진화이용권 사용수
	m_siTicketValue = 0;		//  진화이용권값
	m_fRate = 0.7f;	

	m_pStatic[0] = NULL;	
	m_pStatic[1] = NULL;	

	m_pEdit[0] = NULL;	
	m_pEdit[1] = NULL;	

	m_pBtnSummonUnitSell = NULL;
	m_pBtnClose			 = NULL;

}

CNSummonUnitBuyNpcDlg::~CNSummonUnitBuyNpcDlg()
{
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);

	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);

	SAFE_DELETE(m_pBtnSummonUnitSell);
	SAFE_DELETE(m_pBtnClose);
}

void CNSummonUnitBuyNpcDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTradeMerchantHorseDlg/DLG_TradeMerchantHorse.ddf"));
		file.CreatePopupDialog( this, NSUMMONUNITBUYNPC_DLG, TEXT("dialog_TradeMerchant_Horse"), StaticCallBackDialogNSummonUnitBuyNpcDlg );

		SetTitle(TEXT("소환수 상인"));

		m_pStatic[0]			  = new CStatic( this );
		m_pStatic[1]			  = new CStatic( this );
		m_pEdit[0]				  = new CEdit( this );
		m_pEdit[1]				  = new CEdit( this );

		NEWCBUTTON(m_pBtnSummonUnitSell);
		NEWCBUTTON(m_pBtnClose);
		
		file.CreateControl( m_pStatic[0], NSUMMONUNITBUYNPC_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		m_pStatic[0]->SetFontIndex( pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) - 1);

		file.CreateControl( m_pStatic[1], NSUMMONUNITBUYNPC_DIALOG_STATIC2, TEXT("statictext_NONAME2") );

		file.CreateControl( m_pEdit[0], NSUMMONUNITBUYNPC_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pEdit[1], NSUMMONUNITBUYNPC_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

		file.CreateControl( m_pBtnSummonUnitSell, NSUMMONUNITBUYNPC_DIALOG_BUTTON_SELL,  TEXT("button_NONAME1") );
		file.CreateControl( m_pBtnClose,  NSUMMONUNITBUYNPC_DIALOG_BUTTON_CLOSE,  TEXT("button_NONAME2") );
		
		m_pBtnSummonUnitSell->SetText( TEXT("소환수 팔기"), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		m_pBtnSummonUnitSell->Refresh();

		if ( pclClient->pclCM->CR[1]->pclCI->clSummonInfo.HaveSummon() < 0 )
		{
			TCHAR* pText =  TEXT("소환수가 없습니다");
			m_pStatic[1]->SetFileName( TEXT(""));
			m_pEdit[0]->SetText(pText);
			return;
		}

		SI32 id = 1;
		TCHAR szName[128]= TEXT("");
		SI32 siSummonIndex = -1;
		SI32 i;
		for( i = 0 ; i < MAX_SUMMON_OWN; ++i)
		{
			cltSummon *pclSummon = pclClient->pclCM->CR[id]->pclCI->clSummonInfo.GetSummon(i);
			if(pclSummon == NULL) return;
			//===================================================================
			// 정보 검사 
			//===================================================================
			SI32 siKind = pclSummon->siKind;
			if(siKind <= 0 || siKind >= MAX_KIND_NUMBER)		continue;
			
			cltKindInfo* pclki = pclClient->pclKindInfoSet->pclKI[siKind];
			if(pclki == NULL)									continue;
			//===================================================================
			if ( pclki->bSummonMall == TRUE ) continue ;
			// 살아있는 경우 체크
			if(pclSummon->clIP.GetLife() <= 0)				continue;
			if(pclSummon->siStatus == SUMMON_STATUS_TAKEOUT)
			{
				siSummonIndex = i;
				break;
			}
		}

		if(siSummonIndex >= 0)
			LoadSummonUnit(siSummonIndex);
		else
		{
			TCHAR* pText =  TEXT("소환된 소환수가 없습니다");
			m_pStatic[1]->SetFileName( TEXT(""));
			m_pEdit[0]->SetText(pText);
		}
	}
	else
	{
		DeleteDialog();
	}
}

void CNSummonUnitBuyNpcDlg::Action()
{

}

void CALLBACK CNSummonUnitBuyNpcDlg::StaticCallBackDialogNSummonUnitBuyNpcDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSummonUnitBuyNpcDlg *pThis = (CNSummonUnitBuyNpcDlg *)pControllerMgr;
	pThis->CallBackDialogNSummonUnitBuyNpcDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNSummonUnitBuyNpcDlg::CallBackDialogNSummonUnitBuyNpcDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NSUMMONUNITBUYNPC_DIALOG_BUTTON_SELL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 가격체크
					if(m_siSelectSummonPrice < 1)
						break;
					// 소환수 등급체크
					if(m_siSummonUnitRank < TRADE_SUMMONRANK)
						break;

					// 21억이 넘어가는지 체크
					SI64 BuyerMoney   = (SI64)pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();
					BuyerMoney += m_siSelectSummonPrice;
					if(BuyerMoney > 2100000000)
					{
						break;
					}

					cltClient *pclclient = (cltClient*)pclClient;

					TCHAR buffer[100] = TEXT("");

					StringCchPrintf(buffer, 100,  TEXT("소환수를 %d값에 파시겠습니까?"), m_siSelectSummonPrice);

					stMsgBox MsgBox;
					MsgBox.Set( this,  TEXT("소환수 팔기"), buffer, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;

	case NSUMMONUNITBUYNPC_DIALOG_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

void CNSummonUnitBuyNpcDlg::LoadSummonUnit(SI32 siSummonUnitId)
{
	TCHAR szName[128]= TEXT("");

	cltSummonInfo *pclSummonInfo = &pclClient->pclCM->CR[1]->pclCI->clSummonInfo;
	if(pclSummonInfo == NULL)
		return;

	cltSummon *pclSummon= pclSummonInfo->GetSummon(siSummonUnitId);
	if(pclSummon == NULL) 
		return;

	//============================================
	// KIND 비교 
	//============================================

	if( pclSummon->siStatus == SUMMON_STATUS_TAKEOUT )	
	{
		if( pclSummon->siKind && pclSummon->clIP.GetLife() > 0 )
		{
			// 소환수 얼굴 보여주기
			m_pStatic[1]->SetFontIndex(pclSummon->siKind -1); 

			TCHAR buffer[10] = TEXT("");

			TCHAR* pText = GetTxtFromMgr(5089);
			StringCchPrintf(szName, 128, pText, pclSummon->szName);

			pText = GetTxtFromMgr(5090);
			StringCchCat(szName, 128, pText );			
			StringCchCat(szName, 128, _itot(pclSummon->clIP.siStr, buffer, 10));


			pText = GetTxtFromMgr(5091);
			StringCchCat(szName, 128, pText);	
			StringCchCat(szName, 128, _itot(pclSummon->clIP.siDex, buffer, 10));


			pText = GetTxtFromMgr(5092);
			StringCchCat(szName, 128, pText);
			StringCchCat(szName, 128, _itot(pclSummon->clIP.siMag, buffer, 10));


			pText = GetTxtFromMgr(5093);
			StringCchCat(szName, 128, pText);
			StringCchCat(szName, 128, _itot(pclSummon->clIP.siVit, buffer, 10));


			pText = GetTxtFromMgr(5094);
			StringCchCat(szName, 128, pText);
			StringCchCat(szName, 128, _itot(pclSummon->clIP.siBonus, buffer, 10));
			// 모든조건을 만족하는 소환수를 추가한다.
			m_pEdit[1]->SetText(szName);

			SI32 TempSummonUnitRank = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind]->siSummonRank;
			// 소환수 상인이 하는말
			TCHAR NpcBuffer[256] = TEXT("");
			if( TempSummonUnitRank < TRADE_SUMMONRANK)
			{
				StringCchPrintf(NpcBuffer, 256,  TEXT("6단계 이상 소환수를 가지고 오세요"), pclClient->pclKindInfoSet->pclKI[pclSummon->siKind]->siSummonRank);
				m_pEdit[0]->SetText(NpcBuffer);
				m_pBtnSummonUnitSell->Enable(FALSE);
			}
			else
			{
				m_siSummonUnitIndex = siSummonUnitId;
				m_siSummonUnitRank = pclClient->pclKindInfoSet->pclKI[pclSummon->siKind]->siSummonRank;
			//	CheckTicketNumber(m_siSummonUnitRank);
			//	sprintf(NpcBuffer, TEXT("당신의 소환수를 삽니다~\r\n 진화에 사용된 이용권수는 %d장입니다"), m_siUseTicketNumber);
			//	m_pEdit[0]->SetText(NpcBuffer);
				m_pBtnSummonUnitSell->Enable(TRUE);

				cltGameMsgRequest_TradeMerchantBuySummonInfo clGameMsgRequest_TradeMerchantBuySummonInfo( pclSummon->siSummonID );
				cltMsg clMsg( GAMEMSG_REQUEST_TRADEMERCHANTBUYSUMMONINFO, sizeof(clGameMsgRequest_TradeMerchantBuySummonInfo), (BYTE*)&clGameMsgRequest_TradeMerchantBuySummonInfo );
				pclClient->SendMsgToServer( (sPacketHeader*) &clMsg );
			}
		}
	}
	else
		return;
}

void CNSummonUnitBuyNpcDlg::CheckTicketNumber(SI32 siSummonUnitRank)
{
	if(siSummonUnitRank < 9)	// 진화이용권 1장쓰는 소환수들
	{
		m_siUseTicketNumber = siSummonUnitRank - 1;
	}
	else if(siSummonUnitRank < 17)	// 진화이용권 2장쓰는 소환수들
	{
		m_siUseTicketNumber = (2 * (siSummonUnitRank - 8)) + 7;
	}
	else if(siSummonUnitRank < 25)	// 진화이용권 3장쓰는 소환수들
	{
		m_siUseTicketNumber = (3 * (siSummonUnitRank - 16)) + 23;
	}
	else if(siSummonUnitRank < 40)	// 진화이용권 4장쓰는 소환수들
	{
		m_siUseTicketNumber = (4 * (siSummonUnitRank - 24)) + 47;
	}
	else if(siSummonUnitRank >= 40)	// 진화이용권 5장쓰는 소환수들
	{
		m_siUseTicketNumber = 112;
	}
	m_siSelectSummonPrice = (GMONEY)(m_siUseTicketNumber * 1000 * m_fRate);
}

void CNSummonUnitBuyNpcDlg::SetSummonPrice(SI16 siIndex, SI64 siSummonPrice)
{
//	TCHAR *HorseKindName = pclClient->pclHorseManager->GetHorseName( HorseUnique );
	TCHAR Buffer[256 ] =  TEXT("");
//	TCHAR* pText =GetTxtFromMgr(2116);
	m_siSummonUnitIndex = siIndex;
	m_siSelectSummonPrice = siSummonPrice;
	TCHAR* pText = GetTxtFromMgr(5059);
	StringCchPrintf( Buffer, 256, pText, m_siSelectSummonPrice );
	m_pEdit[0]->SetText(Buffer);
}

void CNSummonUnitBuyNpcDlg::SellToNPCSuccess()
{
	TCHAR* pText = GetTxtFromMgr(5060);
	m_pEdit[0]->SetText(pText);
	m_pEdit[1]->SetText( TEXT(""));
	m_pStatic[1]->SetFileName( TEXT(""));

	m_siSummonUnitIndex = -1;
	m_siSelectSummonPrice = 0;
	m_siSummonUnitRank = 0;
}

void CNSummonUnitBuyNpcDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				cltSummon *pclSummon= pclClient->pclCM->CR[1]->pclCI->clSummonInfo.GetSummonFromSummonID(m_siSummonUnitIndex);
				if(pclSummon == NULL) 
					return;

				cltGameMsgRequest_TradeMerchantBuySummon clGameMsgRequest_TradeMerchantBuySummon(  pclClient->pclCM->CR[1]->GetCharUnique(), pclSummon->siSummonID, m_siSummonUnitRank, m_siSelectSummonPrice );
				cltMsg clMsg( GAMEMSG_REQUEST_SUMMONTRADE, sizeof(clGameMsgRequest_TradeMerchantBuySummon), (BYTE*)&clGameMsgRequest_TradeMerchantBuySummon );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg );

			}
		}
		break;
	}
}
