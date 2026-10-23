#include "../Client/NInterface/NTeacherDlg/FatherDlg.h"


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
#include "Msg/MsgType-Web.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"



extern cltCommonLogic* pclClient;

CNFatherDlg::CNFatherDlg() : CDialogController()
{
	m_siSelectedIndex = -1;
	m_siSelectedChildID = 0;
	m_bUpdateChildListSwitch = false;

	SI32 i;
	for(i=0; i < 9; ++i)
		m_pButton[i] = NULL;
	for(i=0; i < 3; ++i)
		m_pStatic[i] = NULL;
	m_pList = NULL;

	m_pResidentInviteButton = NULL;
}

CNFatherDlg::~CNFatherDlg()
{
	SetTimerOnOff(FALSE);

	for(int i=0; i < 9; ++i)
	{
		if(m_pButton[i])
		{
			delete m_pButton[i];
			m_pButton[i] = NULL;
		}
	}

	for(int i=0; i < 3; ++i)
	{
		if(m_pStatic[i])
		{
			delete m_pStatic[i];
			m_pStatic[i] = NULL;
		}
	}

	if(m_pList)
	{
		delete m_pList;
		m_pList = NULL;
	}

	SAFE_DELETE( m_pResidentInviteButton );
}

void CNFatherDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTeacherDlg/DLG_Father.ddf"));
		file.CreatePopupDialog( this, NCHILD_DLG, TEXT("dialog_Father"), StaticCallBackDialogNFatherDlg );

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		NEWCBUTTON(m_pButton[4]);
		NEWCBUTTON(m_pButton[5]);
		NEWCBUTTON(m_pButton[6]);
		NEWCBUTTON(m_pButton[7]);
		NEWCBUTTON(m_pButton[8]);
		NEWCBUTTON(m_pResidentInviteButton);

		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);
		NEWCSTATIC(m_pStatic[2]);

		NEWCLIST(m_pList);
	
		file.CreateControl( m_pButton[0], NFATHER_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NFATHER_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NFATHER_DIALOG_BUTTON3, TEXT("button_NONAME3") );
		file.CreateControl( m_pButton[3], NFATHER_DIALOG_BUTTON4, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[4], NFATHER_DIALOG_BUTTON5, TEXT("button_NONAME5") );
		file.CreateControl( m_pButton[5], NFATHER_DIALOG_BUTTON6, TEXT("button_NONAME6") );
		file.CreateControl( m_pButton[6], NFATHER_DIALOG_BUTTON7, TEXT("button_NONAME7") );
		file.CreateControl( m_pButton[7], NFATHER_DIALOG_BUTTON8, TEXT("button_NONAME8") );
		file.CreateControl( m_pButton[8], NFATHER_DIALOG_BUTTON9, TEXT("button_NONAME9") );

		file.CreateControl( m_pResidentInviteButton, NFATHER_DIALOG_RESIDENT_INVITE, TEXT("button_resident_invite") );

		file.CreateControl( m_pStatic[0], NFATHER_DIALOG_BUTTON1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pStatic[1], NFATHER_DIALOG_BUTTON2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pStatic[2], NFATHER_DIALOG_BUTTON3, TEXT("statictext_NONAME3") );

		TCHAR* pText= GetTxtFromMgr(3347);
		m_pStatic[1]->SetText( pText, DT_LEFT|DT_VCENTER|DT_WORDBREAK );

		SI32 i;
		for(i=0;i<3;++i)
		{
			m_pStatic[i]->SetBKColorUse( true );
			m_pStatic[i]->SetBorder( true );
			m_pStatic[i]->Refresh();
		}

		file.CreateControl( m_pList, NFATHER_DIALOG_LIST, TEXT("listbox_NONAME1") );

		//KHY - 0610 - 스승제자 S 추가수정.
		if ( ConstServiceArea_Japan != pclClient->siServiceArea )
		{
			pText= GetTxtFromMgr(3299);
			m_pList->SetColumn(0,  200, pText);
			pText= GetTxtFromMgr(3300);
			m_pList->SetColumn(1,  80, pText );
			pText= GetTxtFromMgr(6876);
			m_pList->SetColumn(2,  30, pText );
			m_pList->SetColumn(3,  26, TEXT(""));
			m_pList->SetBorder(TRUE);
			m_pList->SetBKColorUse(TRUE);
			m_pList->Refresh();
		}
		else
		{
			pText= GetTxtFromMgr(3299);
			m_pList->SetColumn(0,  230, pText);
			pText= GetTxtFromMgr(3300);
			m_pList->SetColumn(1,  80, pText);
			m_pList->SetColumn(2,  26, TEXT(""));
			m_pList->SetBorder(TRUE);
			m_pList->SetBKColorUse(TRUE);
			m_pList->Refresh();
		}

		if(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.siPersonID == 0 ||
			_tcslen(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.szName)<= 0 ||
			!_tcscmp(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.szName,TEXT("")) )
		{
			pText= GetTxtFromMgr(3348);
			m_pStatic[0]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pText= GetTxtFromMgr(3349);
			EnableButton(m_pButton[0], pText); // false가 디폴트
			pText= GetTxtFromMgr(3350);

			if( pclClient->siServiceArea != ConstServiceArea_Korea )
			{
				EnableButton(m_pButton[1], pText);
			}			
		}
		else
		{
			TCHAR pfathername[128] = TEXT("") ;
			pText= GetTxtFromMgr(3351);
			StringCchPrintf(pfathername, 128, pText,pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.szName);
			m_pStatic[0]->SetText(pfathername, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}

		// 생성 즉시 제자 목록을 받아온다 - TYS
		SI32 charunique = pclClient->pclCM->CR[1]->GetCharUnique();

		cltGameMsgRequest_UpdateChildList clinfo(charunique);
		cltMsg clMsg(GAMEMSG_REQUEST_UPDATECHILDLIST, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		//KHY - 0610 - 스승제자 S 추가수정.
		 if((pclClient->siServiceArea == ConstServiceArea_Korea)	||
		    (pclClient->siServiceArea == ConstServiceArea_China)	||
		    (pclClient->siServiceArea == ConstServiceArea_English)	||
		    (pclClient->siServiceArea == ConstServiceArea_Taiwan)	||
			(pclClient->siServiceArea == ConstServiceArea_USA )		||
			(pclClient->siServiceArea == ConstServiceArea_NHNChina)	||
			(pclClient->siServiceArea == ConstServiceArea_EUROPE)
		)
		{
			// 중국이랑 글로벌은 제자탈퇴시키기
			pText= GetTxtFromMgr(6862);
			m_pButton[8]->SetText( pText );
		}

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pButton[1]->Show(false);
			m_pButton[7]->Show(false);
		}

		if(pclClient->IsCountrySwitch(Switch_Teacher_Reform))
		{
			m_pResidentInviteButton->Enable( 0 < pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage ? true : false ); 			
		}
		else
		{
			m_pResidentInviteButton->Show(false);
		}

		if ( ConstServiceArea_Korea == pclClient->siServiceArea )
		{
			// 한국은 스승님이란 버튼 안보이기
			m_pButton[4]->Show( false );
		}


		SetTimerOnOff(TRUE);

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_OPENFATHERDIALOG );

		if( pclClient->IsWhereServiceArea(ConstServiceArea_Japan) )
		{
			if( 20 <= pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() )
			{
				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_TEACHERSET );
			}
		}
		else
		{
			//[진성] 스승, 제자. 스승 설정 버튼 클릭하면 퀘스트 클리어 메시지 보냄. => 2008-10-8
			if( 50 <= pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() )
			{
				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_TEACHERSET );
			}
		}
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNFatherDlg::StaticCallBackDialogNFatherDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNFatherDlg *pThis = (CNFatherDlg *)pControllerMgr;
	pThis->CallBackDialogNFatherDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNFatherDlg::CallBackDialogNFatherDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
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

	case NFATHER_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: // 스승님에게 귓말보내기
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
					if(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID <= 0)
					{
						TCHAR* pText1= GetTxtFromMgr(3352);
						TCHAR* pText2= GetTxtFromMgr(3353);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText1,pText2);
						break;
					}

					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID);
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: // 스승님 꼼지가기
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
					if(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID <= 0)
					{
						TCHAR* pText1= GetTxtFromMgr(3352);
						TCHAR* pText2= GetTxtFromMgr(3353);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText1,pText2);
						break;
					}

					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( pclchar->pclCI->clBI.clFatherInfo.clFather.szName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

					/*
					cltGameMsgRequest_Userid clUserid( pclchar->pclCI->clBI.clFatherInfo.clFather.szName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON3: // 스승님신청하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 charunique = pclClient->pclCM->CR[1]->GetCharUnique();

					cltGameMsgRequest_ApplyFather clinfo(charunique);
					cltMsg clMsg(GAMEMSG_REQUEST_APPLYFATHER, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON4: // 스승님등록취소
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_ApplyFather clinfo(0);
					cltMsg clMsg(GAMEMSG_REQUEST_APPLYFATHER, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	// 스승님이란?
				{
					
					//if ( g_SoundOnOffDlg.m_bFullScreenMode )
					//{
					pclclient->ShowHelpWindow(TEXT("fatherhint"));
						//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
						//{
						//	pclclient->CreateInterface( NHINT_DLG );
						//}
						//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("fatherhint"));

				//	}
				//	else 
				//pclclient->m_pHintDlg->Show(TEXT("fatherhint"));
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON6:	// 명단 업데이트
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//KHY - 0610 - 받은 다음에 지운다. - 0629 -임시로 되돌림.
					// 리스트를 모두 삭제한다. 
					DelChildList();

					SI32 charunique = pclClient->pclCM->CR[1]->GetCharUnique();

					cltGameMsgRequest_UpdateChildList clinfo(charunique);
					cltMsg clMsg(GAMEMSG_REQUEST_UPDATECHILDLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON7: // 귓말보내기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 현재 선택된 제자를 선정한다. 
					//SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_CHILDLIST);
					if ( m_siSelectedIndex < 0)break;

					SI32 childpersonid = clChildList[m_siChildListInx[m_siSelectedIndex]].clPerson.GetPersonID();
				
					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(childpersonid);
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON8: // 꼼지가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 현재 선택된 제자을 선정한다. 
					//SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_CHILDLIST);
					if(m_siSelectedIndex < 0)break;

					SI32 childpersonid = clChildList[m_siChildListInx[m_siSelectedIndex]].clPerson.GetPersonID();

					if(childpersonid <= 0)break;
					TCHAR* pname = GetChildName(childpersonid);
					if(pname)
					{
						cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( pname );
						cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
						pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

						/*
						cltGameMsgRequest_Userid clUserid( pname );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						*/
					}
				}
				break;
			}
		}
		break;
	case NFATHER_DIALOG_BUTTON9: // 닫기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//KHY - 0610 - 스승제자 S 추가수정.
					if ( ConstServiceArea_Japan != pclClient->siServiceArea )
					{
						if(m_siSelectedIndex < 0)
							break;

						SI32 childpersonid = clChildList[m_siChildListInx[m_siSelectedIndex]].clPerson.GetPersonID();

						if( childpersonid > 0 )
						{
							m_siSelectedChildID = childpersonid;
							TCHAR* pTitle = GetTxtFromMgr(6862);
							TCHAR* pText = GetTxtFromMgr(6863);
							TCHAR buffer[256] = {'\0' };

							StringCchPrintf( buffer, 256, pText, clChildList[m_siChildListInx[m_siSelectedIndex]].clPerson.GetName() );

							stMsgBox MsgBox;
							MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
							pclclient->SetMsgBox( &MsgBox, NULL, 0 );
						}
					}
					else
					{
						DeleteDialog();
					}
				}
				break;
			}
		}
        break;
	case NFATHER_DIALOG_RESIDENT_INVITE:	//[진성] 스승, 제자. => 2008-10-8
		{
			if( 0 >= pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage )	return;
			
			SI16 siRow = -1, siCol = -1;
			m_pList->GetSelectedIndex( &siRow, &siCol );

			stListBoxItemData* ListBoxItemData;
			ListBoxItemData = m_pList->GetItemData( siRow );
			//if( 20 > _ttoi(ListBoxItemData->strText[1]) ) return;

			TCHAR* pName = ListBoxItemData->strText[0];
			if( NULL == pName )	return;

			TCHAR*			pTitle = GetTxtFromMgr(5614);
			NTCHARString512 kbuffer(GetTxtFromMgr(8916));
			kbuffer.Replace( "#name#",			pName );
			kbuffer.Replace( "#villagename#",	pclClient->pclVillageManager->GetName(pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage) );

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, kbuffer, MSGBOX_TYPE_YESNO, 100 );
			pclclient->SetMsgBox( &MsgBox, NULL, 100 );
		}
		break;
	case NFATHER_DIALOG_LIST:
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
	}
}

void CNFatherDlg::Action()
{
	
	if(m_bUpdateChildListSwitch)
	{
		m_bUpdateChildListSwitch = false;

		// 제자 리스트를 표시한다. 

		// 기존의 리스트를 삭제한다. 
		m_pList->Clear();
		TCHAR buffer[256] = {'\0'};
		SI32 sidata[MAX_CHILD_LIST_IN_CLIENT_NUMBER];

		int i;

		for( i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
		{
			sidata[i] = -1 ;
			if(clChildList[i].clPerson.GetPersonID() <= 0)continue;
			sidata[i] = clChildList[i].siLevel;
		}

		ChildSort(sidata);

		stListBoxItemData Data;

		//KHY - 0610 - 스승제자 S 추가수정.
		if ( ConstServiceArea_Japan != pclClient->siServiceArea )
		{
			for( i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
			{
				if (m_siChildListInx[i] < 0 ) continue ;
				if(clChildList[i].clPerson.GetPersonID() <= 0)continue;

				StringCchPrintf(buffer, 256, TEXT("%s"), clChildList[m_siChildListInx[i]].clPerson.GetName()) ;
				Data.Set(0, buffer);


				StringCchPrintf( buffer, 256, TEXT("%d"), clChildList[m_siChildListInx[i]].siLevel);
				Data.Set(1, buffer);

				if( NEWFATHER_LEVEL <= clChildList[m_siChildListInx[i]].siLevel )
				{
					Data.Set( 2, TEXT("O") );
				}
				else
				{
					Data.Set(2, TEXT("X") );
				}

				m_pList->AddItem(&Data);
			}
		}
		else
		{
			for( i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
			{
				if (m_siChildListInx[i] < 0 ) continue ;
				if(clChildList[i].clPerson.GetPersonID() <= 0)continue;

				StringCchPrintf(buffer, 256, TEXT("%s"), clChildList[m_siChildListInx[i]].clPerson.GetName()) ;
				Data.Set(0, buffer);


				StringCchPrintf( buffer, 256, TEXT("%d"), clChildList[m_siChildListInx[i]].siLevel);
				Data.Set(1, buffer);

				m_pList->AddItem(&Data);
			}
		}
		m_pList->Refresh();
	}
}

void CNFatherDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{

}

void CNFatherDlg::DelChildList()
{
	SI32 i;

	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		clChildList[i].Init();
	}
}

TCHAR * CNFatherDlg::GetChildName(SI32 personid)
{
	SI32 i;

	if(personid <= 0)return NULL;


	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clChildList[i].clPerson.GetPersonID() == personid)
		{
			return clChildList[i].clPerson.szName;
		}
	}

	return NULL;
}

void CNFatherDlg::ChildSort(SI32 sisortdata[])
{
	SI32 i;
	SI32 index = 0;

	bool bChildInfo[MAX_CHILD_LIST_IN_CLIENT_NUMBER];

	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		m_siChildListInx[i] = -1 ;
		bChildInfo[i] = false;
	}

	for(SI32 vill = 0;vill < MAX_CHILD_LIST_IN_CLIENT_NUMBER;vill++)
	{
		SI32 maxnum = -1;
		SI32 maxindex = -1;

		for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
		{
			if(bChildInfo[i] == false && sisortdata[i] > maxnum)
			{
				maxnum		= sisortdata[i];
				maxindex	= i;
			}
		}

		if(maxindex >= 0)
		{
			bChildInfo[maxindex] = true;
			m_siChildListInx[index] = maxindex;
			index++;
		}
	}
}

bool CNFatherDlg::AddChildList(cltChildInfo* pclinfo)
{
	SI32 i;

	if(pclinfo->clPerson.siPersonID <= 0)return false;

	// 이미 리스트에 있는지 확인한다. 
	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clChildList[i].clPerson.siPersonID == pclinfo->clPerson.siPersonID)return false;
	}

	for(i = 0;i < MAX_CHILD_LIST_IN_CLIENT_NUMBER;i++)
	{
		if(clChildList[i].clPerson.siPersonID == 0)
		{
			clChildList[i].Set(pclinfo);
			return true;
		}
	}

	return false;
}

void CNFatherDlg::EnableButton(CButton* pButton, TCHAR* pText, BOOL Flag /* = false */)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(255, 255, 255));
		pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pButton->Enable(false);
	}
}

void CNFatherDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				if( 0 < m_siSelectedChildID )
				{
					cltGameMsgRequest_RenouncePupil clRenouncePupil( m_siSelectedChildID );
					cltMsg clMsg( GAMEMSG_REQUEST_RENOUNCEPUPIL , sizeof( clRenouncePupil ), (BYTE*)&clRenouncePupil );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

					m_siSelectedChildID = 0;
				}
			}
			else
				return;
		}
		break;
	case 100:
		{
			if ( RetValue )
			{
				SI16 siRow = -1, siCol = -1;
				m_pList->GetSelectedIndex( &siRow, &siCol );

				stListBoxItemData* ListBoxItemData;
				ListBoxItemData = m_pList->GetItemData( siRow );
				//if( 20 > _ttoi(ListBoxItemData->strText[1]) ) return;

				TCHAR* pName = ListBoxItemData->strText[0];
				if( NULL == pName )	return;

				cltGameMsgRequest_Village_Resident_Invite	clVillageResidentInvite( pName );
				cltMsg clMsg( GAMEMSG_REQUEST_VILLAGE_RESIDENT_INVITE , sizeof( clVillageResidentInvite ), (BYTE*)&clVillageResidentInvite );
				pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
		}
		break;
	}
}
