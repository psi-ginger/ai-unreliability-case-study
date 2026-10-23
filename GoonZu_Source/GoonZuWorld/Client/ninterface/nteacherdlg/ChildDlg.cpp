#include "../Client/NInterface/NTeacherDlg/ChildDlg.h"


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

#include "JwLib\IsCorrectID\IsCorrectID.h"
#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"



extern cltCommonLogic* pclClient;

CNChildDlg::CNChildDlg() : CDialogController()
{
	m_siSelectedIndex = -1;

	m_bUpdateFatherSwitch = false;
	m_bUpdateFatherApplyerListSwitch = false;

	SI32 i;

	m_pEdit = NULL;

	for( i=0; i < 6; ++i)
		m_pButton[i] = NULL;

	for( i=0; i < 4; ++i)
		m_pStatic[i] = NULL;

	m_pList = NULL;
}

CNChildDlg::~CNChildDlg()
{
	SetTimerOnOff(FALSE);

	SI32 i;
	if(m_pEdit)
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}
	for(i=0; i < 6; ++i)
	{
		if(m_pButton[i])
		{
			delete m_pButton[i];
			m_pButton[i] = NULL;
		}
	}
	for(i=0; i < 4; ++i)
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
}

void CNChildDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NTeacherDlg/DLG_Child.ddf"));
		file.CreatePopupDialog( this, NCHILD_DLG, TEXT("dialog_Child"), StaticCallBackDialogNChildDlg );

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		NEWCBUTTON(m_pButton[2]);
		NEWCBUTTON(m_pButton[3]);
		NEWCBUTTON(m_pButton[4]);
		NEWCBUTTON(m_pButton[5]);

		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);
		NEWCSTATIC(m_pStatic[2]);
		NEWCSTATIC(m_pStatic[3])
		
		NEWCLIST(m_pList);
		NEWCEDIT(m_pEdit);

		file.CreateControl( m_pButton[0], NCHILD_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButton[1], NCHILD_DIALOG_BUTTON2, TEXT("button_NONAME2") );
		file.CreateControl( m_pButton[2], NCHILD_DIALOG_BUTTON3, TEXT("button_NONAME4") );
		file.CreateControl( m_pButton[3], NCHILD_DIALOG_BUTTON4, TEXT("button_NONAME5") );
		file.CreateControl( m_pButton[4], NCHILD_DIALOG_BUTTON5, TEXT("button_NONAME6") );
//KHY - 0610 - 신청자 	꼼지가기 삭제
//		file.CreateControl( m_pButton[5], NCHILD_DIALOG_BUTTON6, TEXT("button_NONAME7") );

		file.CreateControl( m_pStatic[0], NCHILD_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pStatic[1], NCHILD_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pStatic[2], NCHILD_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
		file.CreateControl( m_pStatic[3], NCHILD_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
		
		SI32 i;
		for(i=0;i<4;++i)
		{
			m_pStatic[i]->SetBKColorUse( true );
			m_pStatic[i]->SetBorder( true );
			m_pStatic[i]->Refresh();
		}

		file.CreateControl( m_pEdit, NNCHILD_DIALOG_DIALOG_EDIT, TEXT("editbox_NONAME1") );

		file.CreateControl( m_pList, NCHILD_DIALOG_LIST, TEXT("listbox_NONAME1") );

		TCHAR* pText= GetTxtFromMgr(3299);
		m_pList->SetColumn(0,  130, pText);
		pText= GetTxtFromMgr(3300);
		m_pList->SetColumn(1,  50, pText);
		pText= GetTxtFromMgr(3138);
		m_pList->SetColumn(2,  50, pText);
		pText= GetTxtFromMgr(3139);
		m_pList->SetColumn(3,  80, pText);
		m_pList->SetColumn(4,  26, TEXT(""));
		m_pList->SetBorder(TRUE);
		m_pList->SetBKColorUse(TRUE);
		m_pList->Refresh();

		cltGameMsgRequest_FatherApplyerList clinfo(pclClient->pclCM->CR[1]->GetCharUnique());
		cltMsg clMsg(GAMEMSG_REQUEST_FATHERAPPLYERLIST, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);


		TCHAR buffer[1024];
		const TCHAR* pname = pclClient->pclCM->CR[1]->GetName();

		pText = GetTxtFromMgr(3137);
		TCHAR level[128]= TEXT(" ") ;	

		//KHY - 0610 - 스승제자 S 추가수정.
		if ( ConstServiceArea_Japan != pclClient->siServiceArea )
		{
			StringCchPrintf(level, 128, TEXT("%d"), NEWFATHER_LEVEL);
		}
		else
		{
			StringCchPrintf(level, 128, TEXT("%d"), FATHER_LEVEL);
		}

		TCHAR num[128]= TEXT(" ") ;
		StringCchPrintf(num, 128, TEXT("%d"), pclClient->siMinimumTeacherLevel);
		Nsprintf(buffer,pText,TEXT("user"),  pname  ,TEXT("level"),  level  , TEXT("teacherlevel"), num, NULL);

		m_pEdit->SetFontSize( 12 );
		m_pEdit->SetFontWeight( 0 );
		m_pEdit->SetText(buffer);

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기 
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi))
		{
			m_pButton[2]->Show(false);
		}
		if ( ConstServiceArea_Korea == pclClient->siServiceArea )
		{
			// 한국은 스승님이란 버튼 안보이기
			m_pButton[0]->Show( false );
		}

		SetTimerOnOff(TRUE);

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_OPENFATHERDIALOG );
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNChildDlg::StaticCallBackDialogNChildDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNChildDlg *pThis = (CNChildDlg *)pControllerMgr;
	pThis->CallBackDialogNChildDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNChildDlg::CallBackDialogNChildDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NCHILD_DIALOG_BUTTON1: // 스승님이란?
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->ShowHelpWindow(TEXT("fatherhint"));
					//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
					//{
					//	pclclient->CreateInterface( NHINT_DLG );
					//}
					//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("fatherhint"));
				}
				break;
			}
		}
		break;
	case NCHILD_DIALOG_BUTTON2: // 스승님에게 귓말보내기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
					if(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID <= 0)
					{
						TCHAR* pTitle = GetTxtFromMgr(3140);
						TCHAR* pText = GetTxtFromMgr(3141);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID);
				}
				break;
			}
		}
		break;
	case NCHILD_DIALOG_BUTTON3: // 스승님 꼼지가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
					if(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID <= 0)
					{
						TCHAR* pTitle = GetTxtFromMgr(3140);
						TCHAR* pText = GetTxtFromMgr(3142);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( pclchar->pclCI->clBI.clFatherInfo.clFather.szName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}
				break;
			}
		}
		break;
	case NCHILD_DIALOG_BUTTON4: // 스승님설정
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

					if( 0 >= m_pList->GetListNum() )
					{
						//[진성] 스승, 제자. 스승 설정 버튼 클릭하면 퀘스트 클리어 메시지 보냄. => 2008-10-8
						pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_TEACHERSET );
					}
					
					// 현재 선택된 스승를  확보한다. 
					if(m_siSelectedIndex < 0)break;

					SI32 personid = clApplyerList[m_siSelectedIndex].siPersonID;
					if(personid <= 0)break;

					// 이미 스승이 있는 상태. 
					if(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID)
					{
						// 기존의 스승인 이름과 같은지 확인한다. 
						if(pclchar->pclCI->clBI.clFatherInfo.clFather.siPersonID !=  personid)
						{
							TCHAR* pTitle = GetTxtFromMgr(3144);//텍스트타이틀 교체[2007.07.11 성웅]
							TCHAR* pText = GetTxtFromMgr(3143);

							stMsgBox MsgBox;
							MsgBox.Set( this, pText, pTitle, MSGBOX_TYPE_YESNO, 0 );
							BYTE TempBuffer[ 128 ];
							SI16 Size = sizeof( personid );
							memcpy( TempBuffer, &personid, Size );
							pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
						}
						else
						{
							TCHAR* pTitle = GetTxtFromMgr(3145);
							TCHAR* pText = GetTxtFromMgr(3146);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							break;
						}
					}
					else
					{
						TCHAR* pTitle = GetTxtFromMgr(3147);
						TCHAR* pText = GetTxtFromMgr(3148);

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( personid );
						memcpy( TempBuffer, &personid, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
					}
				}
				break;
			}
		}
		break;
	case NCHILD_DIALOG_BUTTON5: // 신청자에게 귓말보내기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(m_siSelectedIndex < 0)break;

					SI32 personid = clApplyerList[m_siSelectedIndex].siPersonID;
					if(personid <= 0)
					{
						TCHAR* pTitle = GetTxtFromMgr(3149);
						TCHAR* pText = GetTxtFromMgr(3150);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					// 귓말 요청. 
					((cltClient*)pclClient)->RequestSecretChatByPersonID(personid);
					m_siSelectedIndex = -1;
				}
				break;
			}
		}
		break;
	case NCHILD_DIALOG_BUTTON6: // 신청자 꼼지가기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(m_siSelectedIndex < 0)break;

					SI32 personid = clApplyerList[m_siSelectedIndex].siPersonID;
					if(personid <= 0)
					{
						TCHAR* pTitle = GetTxtFromMgr(3149);
						TCHAR* pText = GetTxtFromMgr(3151);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						break;
					}

					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( clApplyerList[m_siSelectedIndex].szName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

					/*
					cltGameMsgRequest_Userid clUserid( clApplyerList[m_siSelectedIndex].szName );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/

					m_siSelectedIndex = -1;
				}
				break;
			}
		}
		break;
	case NCHILD_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					m_siSelectedIndex = m_pList->GetSelectedIndex();
				}
				break;
			}
		}
		break;
	}
}
/*
void CNChildDlg::Hide()
{
	SetTimerOnOff( false );

	CControllerMgr::Show( false );

	return;
}

void CNChildDlg::Show()
{
	SetTimerOnOff( true );

	cltGameMsgRequest_FatherApplyerList clinfo(pclClient->pclCM->CR[1]->GetCharUnique());
	cltMsg clMsg(GAMEMSG_REQUEST_FATHERAPPLYERLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);


	TCHAR buffer[1024];
	const TCHAR* pname = pclClient->pclCM->CR[1]->GetName();

	TCHAR* pText = GetTxtFromMgr(3137);
	sprintf(buffer, pText, pname, FATHER_LEVEL);

	m_pEdit->SetFontSize( 12 );
	m_pEdit->SetFontWeight( 0 );
	m_pEdit->SetText(buffer);

	CControllerMgr::Show( true );

	return;
}
*/
void CNChildDlg::Action()
{
	// 스승  이름 표기. 
	ShowFatherName();

	if ( m_bUpdateFatherSwitch )
	{
	//	HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_FATHERPROFILE);
	//	if ( hwnd )
		{
			/*					if ( _tcslen(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.szAccountID) > 0 &&
			pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.szAccountID != NULL &&
			pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.siPersonID != 0 )
			{
			TCHAR ProfileURL[MAX_PATH]; // = TEXT("") ;
			pclClient->GetCZProfileUrl( ProfileURL, pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.szAccountID );


			DisplayHTMLPage(hwnd, ProfileURL);
			}*/
		}

	/*	hwnd = GetDlgItem(hDlg, IDC_STATIC_FATHERINTRO);
		if ( hwnd )
		{
			if ( _tcslen(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.szAccountID) > 0 &&
				pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.szAccountID != NULL &&
				pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.siPersonID != 0 )
			{
				TCHAR ProfileURL[MAX_PATH]; // = TEXT("") ;
				pclClient->GetCZIntroUrl( ProfileURL, pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.szAccountID );


				DisplayHTMLPage(hwnd, ProfileURL);
			}
		}
	*/
		m_bUpdateFatherSwitch = false ;
	}
	if(m_bUpdateFatherApplyerListSwitch)
	{
		ShowFatherList();

		m_bUpdateFatherApplyerListSwitch = false;
	}
}

void CNChildDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{

}

void CNChildDlg::ShowFatherName()
{
	if(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.siPersonID == 0 ||
		_tcslen(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.szName)<= 0 ||
		!_tcscmp(pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.szName, TEXT("")) )
	{
		TCHAR* pText = GetTxtFromMgr(3152);
		m_pStatic[1]->SetText( pText, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
		EnableButton(m_pButton[1], FALSE);
		EnableButton(m_pButton[2], FALSE);
	}
	else
	{
		TCHAR pfathername[128] = TEXT("") ;
		TCHAR* pText = GetTxtFromMgr(3153);
		StringCchPrintf(pfathername, 128, pText,pclClient->pclCM->CR[1]->pclCI->clBI.clFatherInfo.clFather.szName);
		m_pStatic[1]->SetText( pfathername, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
	}
}

void CNChildDlg::ShowFatherList()
{
	m_pList->Clear();

	SI32 count = 0 ;
	TCHAR buffer[256] = TEXT("") ;
	cltFatherApplyerInfo clapplyerlist[MAX_FATHER_APPLYER_LIST_NUMBER];

	for(SI32 i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
	{
		m_siFatherApplyInx[i] = -1 ;
		clapplyerlist[i].Init();
		clapplyerlist[i].Set(&clApplyerList[i]);
		clApplyerList[i].Init();
	}

	for ( i = 0 ; i < MAX_FATHER_APPLYER_LIST_NUMBER ; i ++)
	{
		SI32 index = 0 ;
		bool bSet = false ;
		for ( SI32 j = 0 ; j < MAX_FATHER_APPLYER_LIST_NUMBER ; j ++ )
		{
			if(clapplyerlist[j].siCharUnique == 0) continue;
			if ( !bSet )
			{
				index = j ;
				bSet = true ;
			}
			if ( clapplyerlist[j].bAngelSwitch )
			{
				clApplyerList[i].Set(&clapplyerlist[j]);
				clapplyerlist[j].Init();
				bSet = false ;
				break;
			}
		}

		if ( bSet )
		{
			clApplyerList[i].Set(&clapplyerlist[index]);
			clapplyerlist[index].Init();
		}
	}

	for(i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
	{
		stListBoxItemData Data;
		Data.Init();

		if(clApplyerList[i].siCharUnique == 0) continue;

		StringCchPrintf(buffer, 256, TEXT("%s"), clApplyerList[i].szName) ;
		Data.Set(0, buffer);
	//	ListViewInsertItem(m_hFatherSetDlg, -1, count, 0,buffer, -1);

		StringCchPrintf( buffer, 256, TEXT("%d"), clApplyerList[i].siLevel);
		Data.Set(1, buffer);
	//	ListViewInsertItem(m_hFatherSetDlg, -1, count, 1, buffer, -1);

		StringCchPrintf(buffer, 256, TEXT("%d"),clApplyerList[i].siFameLevel);
		Data.Set(2, buffer);
	//	ListViewInsertItem(m_hFatherSetDlg, -1, count, 2, buffer, -1);

		if ( clApplyerList[i].bAngelSwitch )
		{
			TCHAR* pText = GetTxtFromMgr(3139);
			StringCchPrintf(buffer, 256, TEXT("%s"), pText);
			Data.Set(3, buffer);
		//	ListViewInsertItem(m_hFatherSetDlg, -1, count, 3, buffer, -1);
		}

		m_pList->AddItem(&Data);
		

		m_siFatherApplyInx[count] = i ;
		count ++ ;
	}
	m_pList->Refresh();
}

// 스승  등록 신청자를 추가한다. 
bool CNChildDlg::AddApplyer(cltFatherApplyerInfo* pclinfo, SI32* preturnval)
{
	SI32 i;
	SI32 minlevel	= 1000;
	SI32 minindex	= -1;

	if(pclinfo->siPersonID <= 0)return false;
	SI32 id = pclinfo->siCharUnique;
	if(pclClient->pclCM->IsValidID(id) == false)return false;
	SI32 level = pclClient->pclCM->CR[id]->pclCI->clIP.GetLevel();

	// 이미 있는 것은 아닌지 확인한다. 
	for(i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
	{
		if(clApplyerList[i].siPersonID == pclinfo->siPersonID)
		{
			*preturnval = ADD_FATHERSUPPLYER_ALREADY;
			return false;
		}
	}


	// 빈자리를 찾아 추가한다. 
	for(i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
	{
		if(clApplyerList[i].siCharUnique == 0)
		{
			clApplyerList[i].Set(pclinfo);
			return true;
		}
		else
		{
			if(pclClient->pclCM->IsValidID(clApplyerList[i].siCharUnique))
			{
				SI32 templevel = pclClient->pclCM->CR[ clApplyerList[i].siCharUnique ]->pclCI->clIP.GetLevel();
				if(minlevel > templevel)
				{
					minlevel = templevel;
					minindex = i;
				}
			}
		}
	}

	// 최저 레벨 보다 높다면 기 사용자를 탈락시키고 새 사용자를 등록한다. 
	if(minindex >= 0)
	{
		if(level >minlevel)
		{
			clApplyerList[minindex].Set(pclinfo);
			return true;
		}
	}

	*preturnval = ADD_FATHERSUPPLYER_FULL;
	return false;
}

// 특정 사용자가 리스트에 존재하는가. 
bool CNChildDlg::IsExist(SI32 personid)
{
	SI32 i;

	if(personid <= 0)return false;

	for(i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
	{
		if(clApplyerList[i].siPersonID == personid)
		{
			return true;
		}
	}

	return false;
}

// 스승 등록 신청자를 삭제 한다. 
bool CNChildDlg::DeleteApplyer(SI32 personid)
{
	SI32 i;

	if(personid <= 0)return false;

	// 이미 있는 것은 아닌지 확인한다. 
	for(i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
	{
		if(clApplyerList[i].siPersonID == personid)
		{
			clApplyerList[i].Init();
		}
	}

	return true;
}

void CNChildDlg::EnableButton(CButton* pButton, BOOL Flag /* = false */)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
		pButton->Enable(false);
	}
}

void CNChildDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if(RetValue)
			{
				//KHY - 0610 - 스승제자 S 추가수정.
				if ( ConstServiceArea_Japan != pclClient->siServiceArea )
				{
					cltGameMsgRequest_SetFather clinfo(*((SI32*)pData), 1, 0);
					cltMsg clMsg(GAMEMSG_REQUEST_SETFATHER, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				else
				{
					cltGameMsgRequest_SetFather clinfo(*((SI32*)pData));
					cltMsg clMsg(GAMEMSG_REQUEST_SETFATHER, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				//[진성] 스승, 제자. 스승 설정 버튼 클릭하면 퀘스트 클리어 메시지 보냄. => 2008-10-8
				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_TEACHERSET );
			}
			else
				break;
		}
		break;
	}
}
