#include "NMofuMofuList.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../../Common/Char/CharManager/CharManager.h"
#include "InterfaceMgr/InterfaceMgr.h"
#include "../Common/Event/MofuMofu/MofuMofu.h"
#include "../CommonLogic/Msg/MsgType-MofuMofuEvent.h"

#include "../../../NLib/NTCHARString.h"

extern cltCommonLogic* pclClient;

CNMofuMofuListDlg::CNMofuMofuListDlg()
{
	m_siFieldIndex = 0;
	m_pMofuMofuListBox = NULL;
	m_pMofuMofuListbutton = NULL;
	m_pMofuMofuListbutton_close = NULL;

	m_siYear	= 0;
	m_siMonth	= 0;
	m_siDay		= 0;
	m_siHour	= 0;

	for ( SI32 i=0; i< MOFUMOFUEVENT_ROOMNUM ; i++ )
		m_clUnit[i].Init();
}

CNMofuMofuListDlg::~CNMofuMofuListDlg()
{
	SAFE_DELETE(m_pMofuMofuListBox);
	SAFE_DELETE(m_pMofuMofuListbutton);
	SAFE_DELETE(m_pMofuMofuListbutton_close);
}

void CNMofuMofuListDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NMofuMofuList/DLG_NMofuMofuList.ddf"));
	file.CreatePopupDialog( this, NMOFUMOFULIST_DLG, TEXT("dialog_MofuMofu_List"),StaticNMofuMofuListDlgProc );

	m_pMofuMofuListBox = new CList(this);
	m_pMofuMofuListbutton = new CButton(this);
	m_pMofuMofuListbutton_close = new CButton(this);

	// 컨트롤 생성
	file.CreateControl( m_pMofuMofuListBox ,NMOFUMOFULIST_LIST_APPLYLIST, TEXT("listbox_ApplyList"));
	file.CreateControl( m_pMofuMofuListbutton ,NMOFUMOFULIST_BUTTON_CONFIRM, TEXT("button_confirm"));
	file.CreateControl( m_pMofuMofuListbutton_close ,NMOFUMOFULIST_BUTTON_CLOSE, TEXT("button_close"));

	m_ReceivedClock = pclClient->CurrentClock;

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

	m_pMofuMofuListBox->SetBorder(true);
	m_pMofuMofuListBox->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(7071);
	m_pMofuMofuListBox->SetColumn( 0, 140, pText );		// 시간
	pText = GetTxtFromMgr(1770);
	m_pMofuMofuListBox->SetColumn( 1, 140, pText );		// 마을

	m_pMofuMofuListBox->Refresh();

	cltGameMsgRequest_MofuMofuEvent_ReserveList clinfo;
	cltMsg clMsg(GAMEMSG_REQUEST_MOFUMOFUEVENT_RESERVELIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CALLBACK CNMofuMofuListDlg::StaticNMofuMofuListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMofuMofuListDlg *pThis = (CNMofuMofuListDlg*) pControllerMgr;
	pThis->NMofuMofuListDlgProc( nEvent, nControlID, pControl );
}

void CNMofuMofuListDlg::NMofuMofuListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient* pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NMOFUMOFULIST_BUTTON_CONFIRM:
		{
			SI16 siRow = -1, siCol = -1;

			m_pMofuMofuListBox->GetSelectedIndex(&siRow,&siCol);
			if( siRow == -1 || siCol == -1 )
				return;

			if(m_clUnit[siRow].m_siVillageUnique > 0)
			{
				// 이미 예약된 시간입니다. 다른 시간을 선택해 주십시요.
				return;
			}

			TCHAR* pszText = m_pMofuMofuListBox->GetText(siRow, 0);

			if(pszText != NULL)
			{
				SI32 Year, Month, Day, Hour;

				Year	= 0;
				Month	= 0;
				Day		= 0;
				Hour	= 0;

				NTCHARString64	kText(GetTxtFromMgr(3808));

				_stscanf(pszText, kText, &Year, &Month, &Day, &Hour );

				if( pclclient->sTime.wYear	== Year &&
					pclclient->sTime.wMonth == Month &&
					pclclient->sTime.wDay	== Day &&
					pclclient->sTime.wHour	== Hour )
				{
					NTCHARString256	kTitle;
					NTCHARString256	kText;

					kTitle = GetTxtFromMgr(6694);
					kText = GetTxtFromMgr(7228);

					// 예약할 수 없는 시간입니다. 다른 시간을 선택해 주십시요.
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText);

					return;
				}

				if( Year > 1000 &&
					(Month >= 1 && Month <= 12) &&
					(Day >= 1 && Day <= 31) &&
					(Hour >= 0 && Hour <= 23) )
				{
					m_siYear	= Year;
					m_siMonth	= Month;
					m_siDay		= Day;
					m_siHour	= Hour;

					NTCHARString256	kTitle(GetTxtFromMgr(6694));
					NTCHARString256	kText(GetTxtFromMgr(7229));

					// #Year#년 #Month#월 #Day#일 #Hour#시에 라쿤랜드 이벤트를 예약하시겠습니까?
					kText.Replace(TEXT("#Year#"), SI32ToString(Year));
					kText.Replace(TEXT("#Month#"), SI32ToString(Month));
					kText.Replace(TEXT("#Day#"), SI32ToString(Day));
					kText.Replace(TEXT("#Hour#"), SI32ToString(Hour));

					stMsgBox MsgBox;
					MsgBox.Set( this, kTitle, kText, MSGBOX_TYPE_YESNO, 100 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
			}
		}
		break;
	case NMOFUMOFULIST_BUTTON_CLOSE:
		{
            switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	default:
		break;
	}
}

void CNMofuMofuListDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 100:	
		{
			if(RetValue == 1)
			{
				cltGameMsgRequest_MofuMofuEvent_Reserve clinfo(m_siYear, m_siMonth, m_siDay, m_siHour);
				cltMsg clMsg(GAMEMSG_REQUEST_MOFUMOFUEVENT_RESERVE, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
	}
}

void CNMofuMofuListDlg::SetReserveList(SI32 siCount, cltMofuMofuEventReserveUnit* pReserveList)
{
	SI32	i;
	for(i=0; i<MOFUMOFUEVENT_ROOMNUM; i++)
	{
		m_clUnit[i].Init();
	}

	for(i=0; i<siCount; i++)
	{
		m_clUnit[i].Set(&pReserveList[i]);
	}
}

void CNMofuMofuListDlg::RefreshList()
{
	if(m_pMofuMofuListBox == NULL)			return;

	TCHAR buffer[256];
	stListBoxItemData itemdata;

	m_pMofuMofuListBox->Clear();

	for ( SI32 i=0; i<MOFUMOFUEVENT_ROOMNUM; i++ )
	{
		itemdata.Init();

		NTCHARString64	kText(GetTxtFromMgr(3808));
		StringCchPrintf( buffer, 256, kText, 
					m_clUnit[i].m_ReserveHour.wYear, m_clUnit[i].m_ReserveHour.wMonth, m_clUnit[i].m_ReserveHour.wDay, m_clUnit[i].m_ReserveHour.wHour );
		itemdata.Set( 0, buffer );

		TCHAR* pName = NULL;
		if(m_clUnit[i].m_siVillageUnique > 0)
		{
			pName = pclClient->pclVillageManager->GetName(m_clUnit[i].m_siVillageUnique);
		}

		if(pName == NULL)		itemdata.Set( 1, TEXT("") );
		else					itemdata.Set( 1, pName );

		m_pMofuMofuListBox->AddItem( &itemdata );
	}
	m_pMofuMofuListBox->Refresh();
}