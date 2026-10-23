//[성웅] 라쿤이벤트 군주 이벤트로 변경
#include "../Client/NInterface/NGoonzuDlg/NGoonzuRaccoonEventDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../common/Char/CharCommon/Char-Common.h"
#include "../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNGoonzuRaccoonEventDlg::CNGoonzuRaccoonEventDlg() : CDialogController()
{
	init();
}

CNGoonzuRaccoonEventDlg::~CNGoonzuRaccoonEventDlg()
{
}
void CNGoonzuRaccoonEventDlg::init()
{
	ZeroMemory(GoonzuRaccoonEventData,sizeof(GoonzuRaccoonEventData))	;
}

void CNGoonzuRaccoonEventDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NGoonzuDlg/DLG_GoonzuRaccoonEvent.ddf");
		file.CreatePopupDialog( this, NRACCOONEVENT_CHANGE_GOONZUEVENT_DLG, "dialog_NONAME3", StaticCallBackDialogNGoonzuRaccoonEventDlg );

		// 첫번째 라쿤 셋팅
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC,	STATIC_RACCOONEVENT_TITLE_1ST	,		this ),		STATIC_RACCOONEVENT_TITLE_1ST,		"statictext_NONAME4");	// 첫번째 자이언트 라쿤	
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC,	STATIC_RACCOONEVENT_TIME_1ST	,		this ),		STATIC_RACCOONEVENT_TIME_1ST,		"statictext_NONAME5");	// 발동시간
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_TIME_1ST	,		this ),		COMBOBOX_RACCOONEVENT_TIME_1ST,		"combobox_NONAME1");
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_HOUR_1ST		,		this ),		STATIC_RACCOONEVENT_HOUR_1ST,		"statictext_NONAME8");	// 시
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_MONNUM_1ST	,		this ),		STATIC_RACCOONEVENT_MONNUM_1ST,		"statictext_NONAME7");	// 출현 수	
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_MONNUM_1ST	,		this ),	COMBOBOX_RACCOONEVENT_MONNUM_1ST,	"combobox_NONAME2");
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_NUM_1ST		,		this ),		STATIC_RACCOONEVENT_NUM_1ST,		"statictext_NONAME11");	// 마리
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_FIELD_1ST		,		this ),		STATIC_RACCOONEVENT_FIELD_1ST,		"statictext_NONAME9");	// 출현장소
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD_1ST ,		this ),		COMBOBOX_RACCOONEVENT_FIELD_1ST,	"combobox_NONAME3");	// 출현장소1
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD1_1ST,		this 	),	COMBOBOX_RACCOONEVENT_FIELD1_1ST,	"combobox_NONAME4");	// 출현장소2
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD2_1ST	,		this ),	COMBOBOX_RACCOONEVENT_FIELD2_1ST,	"combobox_NONAME5");	// 출현장소3		
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, OUTLINE_RACCOONEVENT_FIELD_1ST	,		this ),		OUTLINE_RACCOONEVENT_FIELD_1ST,		"outline_NONAME1"); 

		// 두번째 라쿤 셋팅
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_TITLE_2ND ,		this ),		STATIC_RACCOONEVENT_TITLE_2ND,		"statictext_NONAME1");	// 첫번째 자이언트 라쿤	
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_TIME_2ND,		this  ),		STATIC_RACCOONEVENT_TIME_2ND,		"statictext_NONAME2");	// 발동시간
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_TIME_2ND ,		this ),	COMBOBOX_RACCOONEVENT_TIME_2ND,		"combobox_NONAME6");
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_HOUR_2ND ,		this ),		STATIC_RACCOONEVENT_HOUR_2ND,		"statictext_NONAME6");	// 시
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_MONNUM_2ND ,		this ),		STATIC_RACCOONEVENT_MONNUM_2ND,		"statictext_NONAME3");	// 출현 수	
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_MONNUM_2ND ,		this ), COMBOBOX_RACCOONEVENT_MONNUM_2ND,	"combobox_NONAME7");
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_NUM_2ND ,		this ),		STATIC_RACCOONEVENT_NUM_2ND,		"statictext_NONAME10");	// 마리
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_FIELD_2ND ,		this ),		STATIC_RACCOONEVENT_FIELD_2ND,		"statictext_NONAME12");	// 출현장소
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD_2ND ,		this ),	COMBOBOX_RACCOONEVENT_FIELD_2ND,	"combobox_NONAME8");	// 출현장소1
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD1_2ND ,		this ), COMBOBOX_RACCOONEVENT_FIELD1_2ND,	"combobox_NONAME9");	// 출현장소2
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD2_2ND ,		this ), COMBOBOX_RACCOONEVENT_FIELD2_2ND,	"combobox_NONAME10");	// 출현장소3		
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, OUTLINE_RACCOONEVENT_FIELD_2ND ,		this ),	OUTLINE_RACCOONEVENT_FIELD_2ND,		"outline_NONAME2"); 

		// 세번째 라쿤 셋팅
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_TITLE_3RD ,		this ),		STATIC_RACCOONEVENT_TITLE_3RD,		"statictext_NONAME13");	// 첫번째 자이언트 라쿤	
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_TIME_3RD ,		this ),		STATIC_RACCOONEVENT_TIME_3RD,		"statictext_NONAME14");	// 발동시간
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_TIME_3RD ,		this ),	COMBOBOX_RACCOONEVENT_TIME_3RD,		"combobox_NONAME11");
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_HOUR_3RD ,		this ),		STATIC_RACCOONEVENT_HOUR_3RD,		"statictext_NONAME17");	// 시
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_MONNUM_3RD ,		this ),		STATIC_RACCOONEVENT_MONNUM_3RD,		"statictext_NONAME15");	// 출현 수	
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_MONNUM_3RD ,		this ), COMBOBOX_RACCOONEVENT_MONNUM_3RD,	"combobox_NONAME12");
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_NUM_3RD ,		this ),		STATIC_RACCOONEVENT_NUM_3RD,		"statictext_NONAME18");	// 마리
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( STATIC, STATIC_RACCOONEVENT_FIELD_3RD ,		this ),		STATIC_RACCOONEVENT_FIELD_3RD,		"statictext_NONAME16");	// 출현장소
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD_3RD ,		this ),	COMBOBOX_RACCOONEVENT_FIELD_3RD,	"combobox_NONAME13");	// 출현장소1
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD1_3RD ,		this ), COMBOBOX_RACCOONEVENT_FIELD1_3RD,	"combobox_NONAME14");	// 출현장소2
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( COMBOBOX, COMBOBOX_RACCOONEVENT_FIELD2_3RD,		this  ), COMBOBOX_RACCOONEVENT_FIELD2_3RD,	"combobox_NONAME15");	// 출현장소3		
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( OUTLINE, OUTLINE_RACCOONEVENT_FIELD_3RD ,		this ), OUTLINE_RACCOONEVENT_FIELD_3RD, "outline_NONAME3"); 

		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, BUTTON_RACCOONEVENT_ACCEPT ,		this ), BUTTON_RACCOONEVENT_ACCEPT, "button_NONAME4"); 
		file.CreateControl( m_InterfaceFactory_.CreateUserInterface( BUTTON, BUTTON_RACCOONEVENT_CANCEL ,		this ), BUTTON_RACCOONEVENT_CANCEL, "button_NONAME3"); 

		TCHAR buf[32];
		buf[0] = 0;

		// 시간
		CComboBox* pComboBox_Time_1st	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_TIME_1ST )	;
		CComboBox* pComboBox_Time_2nd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_TIME_2ND )	;
		CComboBox* pComboBox_Time_3rd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_TIME_3RD )	;

		// 
		CComboBox* pComboBox_MonsterNum_1st	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_1ST )	;
		CComboBox* pComboBox_MonsterNum_2nd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_2ND )	;
		CComboBox* pComboBox_MonsterNum_3rd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_3RD )	;

		for (SI16 i = 0; i < MAX_DATA_TIME_NUM; i++)
		{
			stComboBoxItemData tmpComboBoxItemData;

			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );

			pComboBox_Time_1st->AddItem( &tmpComboBoxItemData );
			pComboBox_Time_2nd->AddItem( &tmpComboBoxItemData );
			pComboBox_Time_3rd->AddItem( &tmpComboBoxItemData );
		}
		for (SI16 i = 0; i < MAX_MONSTER_NUM_GOONZURACCOONEVENT; i++)	// 몬스터 생성 갯수
		{
			stComboBoxItemData tmpComboBoxItemData;

			StringCchPrintf(buf, 32, "%d", i+1);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );

			pComboBox_MonsterNum_1st->AddItem( &tmpComboBoxItemData );
			pComboBox_MonsterNum_2nd->AddItem( &tmpComboBoxItemData );
			pComboBox_MonsterNum_3rd->AddItem( &tmpComboBoxItemData );
		}
		for (SI16 i = 0; i < MAX_MAP_NUMBER; i++)	// 맵이름
		{
			if(i >= MAX_MAP_NUMBER )	break	;
			TCHAR* pMapname = pclClient->pclMapManager->GetMapName(i)	;
			if(pMapname != NULL )	continue	;

			stComboBoxItemData tmpComboBoxItemData;

			StringCchPrintf(buf, 32, "%d", i);

			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( 0, buf );

			pComboBox_MonsterNum_1st->AddItem( &tmpComboBoxItemData );
			pComboBox_MonsterNum_2nd->AddItem( &tmpComboBoxItemData );
			pComboBox_MonsterNum_3rd->AddItem( &tmpComboBoxItemData );
		}

		AddHuntmaps();

		SetTimerOnOff( true );
		SetActionTimer( 100 );

		cltCharClient* pChar = pclClient->pclCM->GetCharClient(1)	;
		if(pChar != NULL )
		{
			if(pChar->pclCI->clBI.uiGRank	!=	GRANK_KING)
			{
				CButton* pAcceptButton	=	m_InterfaceFactory_.GetButton(BUTTON_RACCOONEVENT_ACCEPT)	;
				pAcceptButton->Enable(false	)	;
			}
		}

		cltGameMsgRequest_GoonzuRaccoonEventInfo cMsgGoonzuRaccoonEventgetEvent(pclClient->pclCM->CR[1]->pclCI->GetPersonID());
		cltMsg clMsg(GAMEMSG_REQUEST_GOONZURACCOONEVENT_SETINFO, sizeof(cltGameMsgRequest_GoonzuRaccoonEventInfo), (BYTE*)&cMsgGoonzuRaccoonEventgetEvent);
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNGoonzuRaccoonEventDlg::StaticCallBackDialogNGoonzuRaccoonEventDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNGoonzuRaccoonEventDlg *pThis = (CNGoonzuRaccoonEventDlg *)pControllerMgr;
	pThis->CallBackDialogNGoonzuRaccoonEventDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNGoonzuRaccoonEventDlg::CallBackDialogNGoonzuRaccoonEventDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case BUTTON_RACCOONEVENT_CANCEL:
		{
			DeleteDialog();
		}
		break;
	// 발동시간
	case COMBOBOX_RACCOONEVENT_TIME_1ST:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_TimeComboBox(COMBOBOX_RACCOONEVENT_TIME_1ST,0);
				}
				break;
			}
		}
		break;
	case COMBOBOX_RACCOONEVENT_TIME_2ND:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_TimeComboBox(COMBOBOX_RACCOONEVENT_TIME_2ND,1);
				}
				break;
			}
		}
		break;
	case COMBOBOX_RACCOONEVENT_TIME_3RD:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_TimeComboBox(COMBOBOX_RACCOONEVENT_TIME_3RD,2);
				}
				break;
			}
		}
		break;
		// 몬스터 출현 수
	case COMBOBOX_RACCOONEVENT_MONNUM_1ST:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_MonsterNumComboBox(COMBOBOX_RACCOONEVENT_MONNUM_1ST,0);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_MONNUM_2ND:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_MonsterNumComboBox(COMBOBOX_RACCOONEVENT_MONNUM_2ND,1);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_MONNUM_3RD:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_MonsterNumComboBox(COMBOBOX_RACCOONEVENT_MONNUM_3RD,2);
				}
				break;
			}
		}break;
		// 맵 콤보박스
	case COMBOBOX_RACCOONEVENT_FIELD_1ST:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD_1ST,FirstRaccoonTime,FirstMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD1_1ST:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD1_1ST,FirstRaccoonTime,SecondMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD2_1ST:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD2_1ST,FirstRaccoonTime,ThirdMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD_2ND:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD_2ND,SecondRaccoonTime,FirstMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD1_2ND:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD1_2ND,SecondRaccoonTime,SecondMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD2_2ND:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD2_2ND,SecondRaccoonTime,ThirdMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD_3RD:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD_3RD,ThirdRaccoonTime,FirstMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD1_3RD:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD1_3RD,ThirdRaccoonTime,SecondMapIndex);
				}
				break;
			}
		}break;
	case COMBOBOX_RACCOONEVENT_FIELD2_3RD:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					Select_FiledComboBox(COMBOBOX_RACCOONEVENT_FIELD2_3RD,ThirdRaccoonTime,ThirdMapIndex);
				}break;
			}
		}break;
	case BUTTON_RACCOONEVENT_ACCEPT:// 설정하기
		{
			switch( nEvent )
			{
				case EVENT_BUTTON_CLICKED:
					{
						cltCharClient* pChar = pclClient->pclCM->GetCharClient(1)	;
						if(pChar	!= NULL )
						{
							if(pChar->pclCI->clBI.uiGRank	==	GRANK_KING)
							{
								cltGameMsgRequest_SetRaccoonEventToGoonzu clinfo(  GoonzuRaccoonEventData );
								cltMsg clMsg(GAMEMSG_REQUEST_RACCOONEVENT_TOGOONZU, sizeof(clinfo), (BYTE*)&clinfo);
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
								DeleteDialog();
							}
						}
					}break	;
			}
		}break;
	}
}


void CNGoonzuRaccoonEventDlg::Action()
{
	cltClient *pclclient = (cltClient*)pclClient;
	cltCharClient* pChar = pclClient->pclCM->GetCharClient(1)	;
	if(pChar	== NULL )	return	;

	if(pChar->pclCI->clBI.uiGRank	!=	GRANK_KING)
	{
		for ( SI32 ComboIndex = 0 ; ComboIndex < MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT; ComboIndex++ ) 
		{
			CComboBox* pComboBox_Time	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_TIME_1ST+ComboIndex)	;
			if(pComboBox_Time == NULL ) continue ;
			pComboBox_Time->Enable(false)	;
			CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_1ST+ComboIndex)	;
			if(pComboBox_MonsterNum_1 == NULL ) continue ;			
			pComboBox_MonsterNum_1->Enable(false)	;
		}
		CComboBox* pComboBox_Map_1	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD_1ST)	;			if(pComboBox_Map_1 == NULL ) return ;
		CComboBox* pComboBox_Map_2	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD1_1ST)	;			if(pComboBox_Map_2 == NULL ) return ;
		CComboBox* pComboBox_Map_3	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD2_1ST)	;			if(pComboBox_Map_3 == NULL ) return ;
		CComboBox* pComboBox_Map_4	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD_2ND)	;			if(pComboBox_Map_4 == NULL ) return ;
		CComboBox* pComboBox_Map_5	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD1_2ND)	;			if(pComboBox_Map_5 == NULL ) return ;
		CComboBox* pComboBox_Map_6	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD2_2ND)	;			if(pComboBox_Map_6 == NULL ) return ;
		CComboBox* pComboBox_Map_7	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD_3RD)	;			if(pComboBox_Map_7 == NULL ) return ;
		CComboBox* pComboBox_Map_8	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD1_3RD)	;			if(pComboBox_Map_8 == NULL ) return ;
		CComboBox* pComboBox_Map_9	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD2_3RD)	;			if(pComboBox_Map_9 == NULL ) return ;

		pComboBox_Map_1->Enable(false)	;
		pComboBox_Map_2->Enable(false)	;
		pComboBox_Map_3->Enable(false)	;
		pComboBox_Map_4->Enable(false)	;
		pComboBox_Map_5->Enable(false)	;
		pComboBox_Map_6->Enable(false)	;
		pComboBox_Map_7->Enable(false)	;
		pComboBox_Map_8->Enable(false)	;
		pComboBox_Map_9->Enable(false)	;

	}
	else
	{
		for ( SI32 ComboIndex = 0 ; ComboIndex < MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT; ComboIndex++ ) 
		{
			CComboBox* pComboBox_Time	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_TIME_1ST+ComboIndex)	;
			if(pComboBox_Time == NULL ) continue ;
			CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_1ST+ComboIndex)	;
			if(pComboBox_MonsterNum_1 == NULL ) continue ;
			SI32 siSelectIndex_Time = pComboBox_Time->GetSelectedIndex()	;
			TCHAR* pszMonsterNum_Time = pComboBox_Time->GetText( siSelectIndex_Time)	;
			if (pszMonsterNum_Time == NULL ) 
			{
				CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_1ST+ComboIndex)	;
				if(pComboBox_MonsterNum_1)
				{
					pComboBox_MonsterNum_1->Enable(false)	;
				}
			}
			else
			{
				CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_MONNUM_1ST+ComboIndex)	;
				if(pComboBox_MonsterNum_1)
				{
					pComboBox_MonsterNum_1->Enable(true)	;
				}
			}

			SI32 siSelectIndex = pComboBox_MonsterNum_1->GetSelectedIndex()	;
			TCHAR* pszMonsterNum = pComboBox_MonsterNum_1->GetText( siSelectIndex)	;
			if(pszMonsterNum!= NULL)
			{
				SI32 siMonsterNum = atoi(pszMonsterNum)	;
				switch(ComboIndex) {
			case 0:
				{
					EnableComboBox_Map( COMBOBOX_RACCOONEVENT_FIELD_1ST ,siMonsterNum)	;
				}
				break;
			case 1:
				{
					EnableComboBox_Map( COMBOBOX_RACCOONEVENT_FIELD_2ND ,siMonsterNum)	;
				}
				break;
			case 2:
				{
					EnableComboBox_Map( COMBOBOX_RACCOONEVENT_FIELD_3RD,siMonsterNum)	;
				}
				break;
				}
			}
			else
			{
				switch(ComboIndex) 
				{
				case 0:
					{
						EnableComboBox_Map( COMBOBOX_RACCOONEVENT_FIELD_1ST ,0)	;
					}
					break;
				case 1:
					{
						EnableComboBox_Map( COMBOBOX_RACCOONEVENT_FIELD_2ND ,0)	;
					}
					break;
				case 2:
					{
						EnableComboBox_Map( COMBOBOX_RACCOONEVENT_FIELD_3RD,0)	;
					}
					break;
				}

			}
		}

	}
}
void CNGoonzuRaccoonEventDlg::AddHuntmaps()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/KingGuriInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/KingGuriInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile KingGuriInfo.txt"),TEXT("Error"));
		return;
	}

	SI32 TextNum	=	0	;	// 텍스트 매니저 번호
	SI32 RocX	=	0	;
	SI32 RocY	=	0	;
	SI32 siCount	=	0	;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&TextNum,	4,
			NDATA_INT32,	&RocX,	4,
			NDATA_INT32,	&RocY,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData() ) 
		{
			if(siCount >= MAX_DATA_MAP_NUM)	break	;
			if(TextNum < 0 ||TextNum == 0 )	continue	;
			GoonzuRaccoonLoadData[siCount].SetRoc(TextNum,RocX,RocY)	;
			TCHAR* pMapName = GetTxtFromMgr( TextNum )	;
			AddCombo(pMapName);
			siCount++;
		}
	}

}
void CNGoonzuRaccoonEventDlg::AddCombo(TCHAR * maptypename)
{
	TCHAR buf[32];
	buf[0] = 0;
	// 맵이름
	CComboBox* pComboBox_MapName_1st	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD_1ST )	;
	CComboBox* pComboBox_MapName1_1st	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD1_1ST )	;
	CComboBox* pComboBox_MapName2_1st	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD2_1ST )	;

	CComboBox* pComboBox_MapName_2nd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD_2ND )	;
	CComboBox* pComboBox_MapName1_2nd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD1_2ND )	;
	CComboBox* pComboBox_MapName2_2nd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD2_2ND )	;

	CComboBox* pComboBox_MapName_3rd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD_3RD )	;
	CComboBox* pComboBox_MapName1_3rd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD1_3RD )	;
	CComboBox* pComboBox_MapName2_3rd	=	m_InterfaceFactory_.GetComboBox( COMBOBOX_RACCOONEVENT_FIELD2_3RD )	;

	stComboBoxItemData ComboBoxItemData;
	ComboBoxItemData.Init();
	ComboBoxItemData.Set( maptypename );

	pComboBox_MapName_1st->AddItem( &ComboBoxItemData );
	pComboBox_MapName1_1st->AddItem( &ComboBoxItemData );
	pComboBox_MapName2_1st->AddItem( &ComboBoxItemData );

	pComboBox_MapName_2nd->AddItem( &ComboBoxItemData );
	pComboBox_MapName1_2nd->AddItem( &ComboBoxItemData );
	pComboBox_MapName2_2nd->AddItem( &ComboBoxItemData );

	pComboBox_MapName_3rd->AddItem( &ComboBoxItemData );
	pComboBox_MapName1_3rd->AddItem( &ComboBoxItemData );
	pComboBox_MapName2_3rd->AddItem( &ComboBoxItemData );
}
void CNGoonzuRaccoonEventDlg::Select_FiledComboBox(SI32 siComboBoxID, SI32 siRaccoonTimeIndex, SI32 siMapIndexArray)
{
	CComboBox* pComboBox	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )	;
	SI32 siSelectIndex = pComboBox->GetSelectedIndex()	;

	// 리소스 배열에 저장된 정보와 콤보 박스는 순서가 같으니까
	if ( siSelectIndex >= MAX_DATA_MAP_NUM) return	;
	SI32 siMapindex = GoonzuRaccoonLoadData[siSelectIndex].GetMapIndex()	;
	SI32 siMapX = GoonzuRaccoonLoadData[siSelectIndex].GetX();
	SI32 siMapY = GoonzuRaccoonLoadData[siSelectIndex].GetY();

	//if ( pMapname != NULL && siMapX > 0 && siMapY> 0 ) 
	if ( siMapindex > 0 && siMapX > 0 && siMapY> 0 ) 
	{
		//GoonzuRaccoonEventData[siRaccoonTimeIndex].SetMapName(siMapIndexArray ,pMapname);	// 첫번째 두번쨰 3번째 인지/ 첫번째 맵인덱스인지 두번째 맵인덱스인지 세번쨰 인지 
		GoonzuRaccoonEventData[siRaccoonTimeIndex].SetMapIndex(siMapIndexArray ,siMapindex);	// 첫번째 두번쨰 3번째 인지/ 첫번째 맵인덱스인지 두번째 맵인덱스인지 세번쨰 인지 
		GoonzuRaccoonEventData[siRaccoonTimeIndex].SetMapRocX(siMapIndexArray ,siMapX);	// 첫번째 두번쨰 3번째 인지/ 첫번째 맵인덱스인지 두번째 맵인덱스인지 세번쨰 인지 
		GoonzuRaccoonEventData[siRaccoonTimeIndex].SetMapRocY(siMapIndexArray ,siMapY);	// 첫번째 두번쨰 3번째 인지/ 첫번째 맵인덱스인지 두번째 맵인덱스인지 세번쨰 인지 
	}

}
void CNGoonzuRaccoonEventDlg::Select_TimeComboBox(SI32 siComboBoxID, SI32 siRaccoonTimeIndex)
{
	CComboBox* pComboBox	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )	;
	SI32 siSelectIndex = pComboBox->GetSelectedIndex()	;
	TCHAR* pTimeText = pComboBox->GetText( siSelectIndex );	
	if(pTimeText != NULL )
	{
		GoonzuRaccoonEventData[siRaccoonTimeIndex].SetTime(atoi(pTimeText))	;
	}
}
void CNGoonzuRaccoonEventDlg::Select_MonsterNumComboBox(SI32 siComboBoxID, SI32 siRaccoonTimeIndex)
{
	CComboBox* pComboBox	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )	;
	SI32 siSelectIndex = pComboBox->GetSelectedIndex()	;
	TCHAR* pMonsterNumText = pComboBox->GetText( siSelectIndex );
	if(pMonsterNumText != NULL )
	{
		GoonzuRaccoonEventData[siRaccoonTimeIndex].SetMakeMonsterNum(atoi(pMonsterNumText))	;
	}	
}
void CNGoonzuRaccoonEventDlg::SetComboX(cGoonzuRaccoonEventData *pData , SI32 siRaccoonTime)
{
	SI32 siTime = pData->GetTime()		;
	if (siTime < 0 || siTime > MAX_HOUR_GOONZURACCOONEVENT )	return	;
	SI32 siMonsterNumber = pData->GetMonsterNum()	;
	if (siMonsterNumber <= 0 || siMonsterNumber > MAX_MONSTER_NUM_GOONZURACCOONEVENT )	return	;

	switch(siRaccoonTime) {
	case 0:
		{
			
			SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_TIME_1ST,siTime)	;
			SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_MONNUM_1ST,siMonsterNumber)	;
			for ( SI32 ComboIndex_Map = 0 ; ComboIndex_Map < MAX_MAP_NUM; ComboIndex_Map++ )	// 9번의 선택할수 있는 최대 콤보 박스 
			{
				SI32 siMapindex = pData->GetRocIndex(ComboIndex_Map)	;
				if ( CheckSetMapData(siMapindex) == false ) 
				{
					siMapindex	=	0	;
				}
				if ( siMapindex <= 0 )	continue	;
				TCHAR* pMapName	=	GetTxtFromMgr( siMapindex)	;
				if(pMapName == NULL)	break	;
				SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_FIELD_1ST+ComboIndex_Map,pMapName)	;
			}
		}
		break;
	case 1:
		{
			SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_TIME_2ND,siTime)	;
			SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_MONNUM_2ND,siMonsterNumber)	;
			for ( SI32 ComboIndex_Map = 0 ; ComboIndex_Map < MAX_MAP_NUM; ComboIndex_Map++ )	// 9번의 선택할수 있는 최대 콤보 박스 
			{
				SI32 siMapindex = pData->GetRocIndex(ComboIndex_Map)	;
				if ( CheckSetMapData(siMapindex) == false ) 
				{
					siMapindex	=	0	;
				}
				if ( siMapindex <= 0 )	continue	;
				TCHAR* pMapName	=	GetTxtFromMgr( siMapindex)	;
				if(pMapName == NULL)	break	;
				SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_FIELD_2ND+ComboIndex_Map,pMapName)	;
			}
		}
		break;
	case 2:
		{
			SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_TIME_3RD,siTime)	;
			SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_MONNUM_3RD,siMonsterNumber)	;
			for ( SI32 ComboIndex_Map = 0 ; ComboIndex_Map < MAX_MAP_NUM; ComboIndex_Map++ )	// 9번의 선택할수 있는 최대 콤보 박스 
			{
				SI32 siMapindex = pData->GetRocIndex(ComboIndex_Map)	;
				if ( CheckSetMapData(siMapindex) == false ) 
				{
					siMapindex	=	0	;
				}
				if ( siMapindex <= 0 )	continue	;
				TCHAR* pMapName	=	GetTxtFromMgr( siMapindex)	;
				if(pMapName == NULL)	break	;
				SetComboBox_Cursel(COMBOBOX_RACCOONEVENT_FIELD_3RD+ComboIndex_Map,pMapName)	;
			}
		}
		break;
	}
}
void CNGoonzuRaccoonEventDlg::SetComboBox_Cursel( SI32 siComboBoxID , TCHAR* szText)
{

	if ( 0 < strlen(szText)) 
	{
		CComboBox* pComboBox	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )	;
		if(pComboBox  != NULL )
		{
			// 해당 콤보 박스 찾아서 셋팅 
			SI32 siFindIndex = pComboBox->FindIndexFromText( szText )	;
			pComboBox->SetCursel( siFindIndex, true )	;
		}
	}

}
void CNGoonzuRaccoonEventDlg::SetComboBox_Cursel( SI32 siComboBoxID , SI32 siNumber )
{
	CComboBox* pComboBox	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )	;
	if(pComboBox  != NULL )
	{
		TCHAR szText[10] = { '\0', };
		itoa(siNumber,szText,10)	;

		// 해당 콤보 박스 찾아서 셋팅 
		SI32 siFindIndex = pComboBox->FindIndexFromText( szText );
		pComboBox->SetCursel( siFindIndex, true )	;

	}
}
void CNGoonzuRaccoonEventDlg::EnableComboBox_Map(SI32 siComboBoxID,SI32 siMonsterNum)	
{
	switch(siMonsterNum)
	{
	case 1:
		{
			CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( siComboBoxID)		;	if( pComboBox_MonsterNum_1 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_2	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+1)	;	if( pComboBox_MonsterNum_2 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_3	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+2 )	;	if( pComboBox_MonsterNum_3 == NULL)		return	;
			pComboBox_MonsterNum_1->Enable(true)	;
			pComboBox_MonsterNum_2->Enable(false)	;
			pComboBox_MonsterNum_3->Enable(false)	;
		}
		break;
	case 2:
		{
			CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )		;	if( pComboBox_MonsterNum_1 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_2	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+1)	;	if( pComboBox_MonsterNum_2 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_3	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+2)	;	if( pComboBox_MonsterNum_3 == NULL)		return	;
			pComboBox_MonsterNum_1->Enable(true)	;
			pComboBox_MonsterNum_2->Enable(true)	;
			pComboBox_MonsterNum_3->Enable(false)	;
		}
		break;
	case 3:
		{
			CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( siComboBoxID)		;	if( pComboBox_MonsterNum_1 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_2	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+1 )	;	if( pComboBox_MonsterNum_2 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_3	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+2 )	;	if( pComboBox_MonsterNum_3 == NULL)		return	;
			pComboBox_MonsterNum_1->Enable(true)	;
			pComboBox_MonsterNum_2->Enable(true)	;
			pComboBox_MonsterNum_3->Enable(true)	;
		}
		break;
	default:
		{
			CComboBox* pComboBox_MonsterNum_1	=	m_InterfaceFactory_.GetComboBox( siComboBoxID )		;	if( pComboBox_MonsterNum_1 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_2	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+1 )	;	if( pComboBox_MonsterNum_2 == NULL)		return	;
			CComboBox* pComboBox_MonsterNum_3	=	m_InterfaceFactory_.GetComboBox( siComboBoxID+2 )	;	if( pComboBox_MonsterNum_3 == NULL)		return	;
			pComboBox_MonsterNum_1->Enable(false)	;
			pComboBox_MonsterNum_2->Enable(false)	;
			pComboBox_MonsterNum_3->Enable(false)	;
		}
	}
}
void CNGoonzuRaccoonEventDlg::BeforeCheckData()
{	
}
bool CNGoonzuRaccoonEventDlg::CheckSetMapData( SI32 siMapname )
{
	if( siMapname <= 0 ) return false ;
	for( SI32 i = 0 ; i < MAX_DATA_MAP_NUM ; i++ )
	{
		if(siMapname == GoonzuRaccoonLoadData[i].GetMapIndex() )	return true ;
	}

	return false ;
}