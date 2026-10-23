#include "../Client/NInterface/NLowLevelDlg/NLowLevelDlg.h"


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

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../Common/HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"
#include "../../lib/WebHTML/WebHTML.h"

#include "..\..\Resource.h"
#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../Client/Music/Music.h"


extern cltCommonLogic* pclClient;

CNLowLevelDlg::CNLowLevelDlg() : CDialogController()
{
	m_siSelectedIndex = -1;
	m_bUpdateLowLevelListSwitch = false;
	SI32 i;
	for(i=0; i < 3; ++i)
	{
		m_pButton[i] = NULL;	
	}
	m_pList = NULL;
}

CNLowLevelDlg::~CNLowLevelDlg()
{
	SetTimerOnOff(FALSE);

	for(int i=0; i < 3; ++i)
	{
		if(m_pButton[i])
		{
			delete m_pButton[i];
			m_pButton[i] = NULL;
		}
	}

	if(m_pList)
	{
		delete m_pList;
		m_pList = NULL;
	}
}

void CNLowLevelDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NLowLevelDlg/DLG_NLowLevel.ddf");
		file.CreatePopupDialog( this, NLOWLEVEL_DLG, "dialog_LowLevel", StaticCallBackDialogNLowLevelDlg );

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);

		NEWCLIST(m_pList);

		file.CreateControl( m_pButton[0], NLOWLEVEL_BUTTON_CLOSE, "button_LowLevel_Close" );
		file.CreateControl( m_pButton[1], NLOWLEVEL_BUTTON_TALK, "button_Talk" );
		file.CreateControl( m_pButton[2], NLOWLEVEL_BUTTON_UPDATE, "button_Update" );


		char* pText;


		file.CreateControl( m_pList, NLOWLEVEL_LIST, "listbox_LowLevel_List" );

		pText= GetTxtFromMgr(3299);
		m_pList->SetColumn(0,  188, pText);
		pText= GetTxtFromMgr(3300);
		m_pList->SetColumn(1,  88, pText);
		m_pList->SetBorder(TRUE);
		m_pList->SetBKColorUse(TRUE);
		m_pList->Refresh();


		SetTimerOnOff(TRUE);

		// 첫 페이지의 제자목록을 요청한다.
		DelLowLevelList();

		CDialogController::MovePos( GetX()+360, GetY() );

		// 20미만 레벨 유저 요청
		cltGameMsgRequest_LowLevelList clinfo(pclClient->pclCM->CR[1]->GetCharUnique());
		cltMsg clMsg(GAMEMSG_REQUEST_LOWLEVELLIST, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNLowLevelDlg::StaticCallBackDialogNLowLevelDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNLowLevelDlg *pThis = (CNLowLevelDlg *)pControllerMgr;
	pThis->CallBackDialogNLowLevelDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNLowLevelDlg::CallBackDialogNLowLevelDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	SI32 i = 0 ;
	SI32 id = 1;

	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NLOWLEVEL_BUTTON_CLOSE:
		{
			DeleteDialog();
		}
		break;
	case NLOWLEVEL_BUTTON_TALK: // 귓말보내기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 현재 유저 선정한다. 
					if ( m_siSelectedIndex < 0)break;

					SI32 LowLevelpersonid = clLowLevelList[m_siLowLevelListInx[m_siSelectedIndex]].clPerson.GetPersonID();

					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(LowLevelpersonid);
					pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CHATTOBEGGINER );

				}
				break;
			}
		}
		break;
	case NLOWLEVEL_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 tempSelectIndex = m_pList->GetSelectedIndex();

					if(tempSelectIndex < 0)
						return;

					m_siSelectedIndex = tempSelectIndex;
				}
				break;
			}
		}
		break;
	case NLOWLEVEL_BUTTON_UPDATE:  
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DelLowLevelList();

					// 20미만 레벨 유저 요청
					cltGameMsgRequest_LowLevelList clinfo(pclClient->pclCM->CR[1]->GetCharUnique());
					cltMsg clMsg(GAMEMSG_REQUEST_LOWLEVELLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				}
				break;
			}
		}
		break;
	}
}

void CNLowLevelDlg::Action()
{
	if(m_bUpdateLowLevelListSwitch)
	{
		m_bUpdateLowLevelListSwitch = false;

		// 리스트를 표시한다. 
		SI32 count = 0 ;
		char buffer[256] = "" ;

		SI32 sidata[MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER];

		int i;

		for( i = 0;i < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;i++)
		{
			sidata[i] = -1 ;
			if(clLowLevelList[i].clPerson.GetPersonID() <= 0)continue;
			sidata[i] = clLowLevelList[i].siLevel;
		}
		LowLevelSort(sidata);

		m_pList->Clear();
		stListBoxItemData Data;
		for( i = 0;i < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;i++)
		{
			if (m_siLowLevelListInx[i] < 0 ) continue ;
			if(clLowLevelList[i].clPerson.GetPersonID() <= 0)continue;

			sprintf(buffer, "%s", clLowLevelList[m_siLowLevelListInx[i]].clPerson.GetName()) ;
			Data.Set(0, buffer);

			sprintf( buffer, "%d", clLowLevelList[m_siLowLevelListInx[i]].siLevel);
			Data.Set(1, buffer);

			m_pList->AddItem(&Data);

			count ++ ;
		}
		m_pList->Refresh();
	}

}

void CNLowLevelDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{

}

void CNLowLevelDlg::DelLowLevelList()
{
	SI32 i;

	for(i = 0;i < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;i++)
	{
		clLowLevelList[i].Init();
	}
}

char * CNLowLevelDlg::GetLowLevelName(SI32 personid)
{
	SI32 i;

	if(personid <= 0)return NULL;


	for(i = 0;i < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clLowLevelList[i].clPerson.GetPersonID() == personid)
		{
			return clLowLevelList[i].clPerson.szName;
		}
	}

	return NULL;
}

void CNLowLevelDlg::LowLevelSort(SI32 sisortdata[])
{
	SI32 i;
	SI32 index = 0;

	bool bLowLevelInfo[MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER];

	for(i = 0;i < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;i++)
	{
		m_siLowLevelListInx[i] = -1 ;
		bLowLevelInfo[i] = false;
	}

	for(SI32 vill = 0;vill < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;vill++)
	{
		SI32 maxnum = -1;
		SI32 maxindex = -1;

		for(i = 0;i < MAX_LOWLEVEL_LIST_IN_CLIENT_NUMBER;i++)
		{
			if(bLowLevelInfo[i] == false && sisortdata[i] > maxnum)
			{
				maxnum		= sisortdata[i];
				maxindex	= i;
			}
		}

		if(maxindex >= 0)
		{
			bLowLevelInfo[maxindex] = true;
			m_siLowLevelListInx[index] = maxindex;
			index++;
		}
	}
}


void CNLowLevelDlg::EnableButton(CButton* pButton, char* pText, BOOL Flag /* = false */)
{
	//if(Flag)
	//{
	//	pButton->SetFontColor( BUTTON_NORMALFONT_COLOR );
	//	pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	pButton->Enable(true);
	//}
	//else
	//{
	//	pButton->SetFontColor( BUTTON_DISABLEFONT_COLOR );
	//	pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	pButton->Enable(false);
	//}
}
