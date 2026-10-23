#include "NBokBulBokEventDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

//#include "../../CommonLogic/Msg/MsgType-Quest.h"

#include "../../CommonLogic/Msg/MsgType-BokBulBok.h"

NBokBulBokEventDlg::NBokBulBokEventDlg()
{
}

NBokBulBokEventDlg::~NBokBulBokEventDlg()
{
	Destroy();
}

void NBokBulBokEventDlg::init()
{
	TCHAR szPortrait[256];
	SI32  siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_BOKBULBOKNPC")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	//m_InterfaceFactory.GetStatic( NBOKBULBOKEVENT_NPC_STATIC_PORTRAIT_IMAGE )->SetFileName(szPortrait);
	//m_InterfaceFactory.GetStatic( NBOKBULBOKEVENT_NPC_STATIC_PORTRAIT_IMAGE )->SetFontIndex( siPortraitFont );
	//m_InterfaceFactory.GetEdit( NBOKBULBOKEVENT_DLG_EDIT_JOIN_ASKING )->SetText( GetTxtFromMgr(40030) );
}
 
void NBokBulBokEventDlg::Create()
{
	// 이미 생성되었다면 리턴
	if( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;

	file.LoadFile( TEXT ("Ninterface/Data/NBokBulBokDlg/DLG_NBokBulBok_Join_Asking.ddf") );
	file.CreatePopupDialog( this, NBOKBULBOK_EVENT_DLG,	TEXT( "dialog_bokbulbok_Join" ), StaticNBokBulBok_JoinDlgProc);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NBOKBULBOKEVENT_DLG_EDIT_JOIN_ASKING,	this), NBOKBULBOKEVENT_DLG_EDIT_JOIN_ASKING, "editbox_NPC_Asking" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NBOKBULBOKEVENT_DLG_BUTTON_APPLY,	this), NBOKBULBOKEVENT_DLG_BUTTON_APPLY, "button_Apply"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NBOKBULBOKEVENT_DLG_BUTTON_CANCEL,	this), NBOKBULBOKEVENT_DLG_BUTTON_CANCEL, "button_Cancel" );

	// 퀘스트 관련
	//SI32 charkind = pclClient->GetUniqueFromHash(TEXT("KIND_BOKBULBOKNPC"));
}

void NBokBulBokEventDlg::Show_FirstMessageDlg(GMONEY siTotalMoney)
{
	CEdit* pclExplain = m_InterfaceFactory.GetEdit( NBOKBULBOKEVENT_DLG_EDIT_JOIN_ASKING );
	if( pclExplain )
	{
		SI32 siNPCRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

		GMONEY	siEntranceFee = siNPCRate * 10;	// npcrate(시세) * 10

		pclExplain->SetText( GetTxtFromMgr(40070) ); //40066 에서 바뀐고..
		pclExplain->Enable( true );

		NTCHARString256 pBody(GetTxtFromMgr(40070));
		
		NTCHARString256 strMoney;	// 입장료
		g_JYLibFunction.SetNumUnit(siEntranceFee, strMoney, 256);
		pBody.Replace( "#money#", strMoney );

		NTCHARString256 strReward;					// 현재 누적 보상 금액
		g_JYLibFunction.SetNumUnit(siTotalMoney, strReward, 256);
		pBody.Replace( "#summoney#", strReward );

		pclExplain->SetText( pBody );
		pclExplain->Enable( true );
	}
}

void NBokBulBokEventDlg::Destroy()
{

}

void NBokBulBokEventDlg::Action()
{

}

void CALLBACK NBokBulBokEventDlg::StaticNBokBulBok_JoinDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NBokBulBokEventDlg *pThis = ( NBokBulBokEventDlg * )pControllerMgr;
	pThis->NBokBulBok_InfoDlgProc( nEvent, nControlID, pControl);
}
void CALLBACK NBokBulBokEventDlg::NBokBulBok_InfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NBOKBULBOKEVENT_DLG_BUTTON_APPLY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 
					cltCharClient* pclCharClient = pclClient->pclCM->GetCharClient(1)	;
					if( pclCharClient != NULL  )
					{
						// 이벤트 참여가능 여부 확인 패킷
						cltGameRequest_BBBCheckCondition clinfo( pclCharClient->pclCI->GetPersonID() );
						cltMsg clMsg(GAMEMSG_REQUEST_BBB_CHECK_PlAY_CONDITION, sizeof(clinfo), (BYTE*)&clinfo);
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);							
					}

					DeleteDialog();
				}
			}
		}
		break;

	case NBOKBULBOKEVENT_DLG_BUTTON_CANCEL:
		{
			DeleteDialog(); //Hide();
		}
		break;
	default:
		break;
	}

}

void NBokBulBokEventDlg::CheckAllMapUse() // 현재 모든 맵에서 유저가 게임 이용 중인지 체크 한다.
{
	// 모든유저가 이용중일때 메세지 박스


	// 이용가능할때 대기자 등록 리스트 팝업.
	ShowWaitingPlayersList();

}
void NBokBulBokEventDlg::ShowWaitingPlayersList() // 대기자 등록 리스트 팝업.
{

}