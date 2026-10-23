
#include "NLetterDlg.h"
#include "..\..\interface\LetterDlg\LetterMgr.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../ninterface/nprivatechatdlg/NPrivateChatDlgMgr.h"
#include "../../ninterface/nprivatechatdlg/NPrivateChatDlg.h"

#include "../../ninterface/npartydlg/NPartyDlg.h"
#include "../NHelpButton/NHelpButtonDlg.h"

#include "../NMarriageLoveLevelDlg/NMarriageLoveLevelDlg.h"
#include "../NFamilyCookieDlg/NFamilyCookieDlg.h"
#include "../NFamilyInfoDlg/NFamilyInfoDlg.h"
#include "../NDormancyAccount/NDormancyPointDlg.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;

extern SI32 gsiResolution;
CNLetterDlg::CNLetterDlg()
{
	m_bMini = FALSE;
	m_pMinimizeButton = NULL;
	m_pCurrentLetter =  NULL;
	m_pImageMinimizedButton = NULL;
	//m_pImageMinimized = NULL;
	m_pLetterMgr = new CLetterMgr();
	Popupflag =0;
	m_siOldPosX = 0;
	m_siOldPosY	= 0;
	SetTimerOnOff( true );	
	m_siMinimizedSlot = 0;
}

CNLetterDlg::~CNLetterDlg()
{

	SAFE_DELETE(m_pMinimizeButton);
	SAFE_DELETE(m_pLetterMgr);
	SAFE_DELETE(m_pNLetterStatic);
	SAFE_DELETE(m_pNLetterButton1);
	SAFE_DELETE(m_pNLetterButton2);
	SAFE_DELETE(m_pMinimizeButton);
	SAFE_DELETE(m_pImageMinimizedButton);
	//SAFE_DELETE(m_pImageMinimized);
	Popupflag =0;
	SetTimerOnOff( false );

}


void CNLetterDlg::Minimi(BOOL bMini )
{	
	if(m_bMini == bMini) return;
	
	cltClient *pclclient = (cltClient*)pclClient;
	
	if( bMini )
	{	
		m_siOldPosX = GetX();
		m_siOldPosY = GetY();
		SetMoveable(FALSE);
		
		/*
		SI16 MiniOffsetX = 0;
		if( pclclient->m_pDialog[NPARTY_DLG] && ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->IsMinimi() )
			MiniOffsetX += 35;

		if( pclclient->m_pNPrivateChatDlgMgr->IsMinimi())
			MiniOffsetX += 35;
		
		SetDialogPos( pclclient->GetScreenXSize() - 32 - MiniOffsetX, 210, 32, 34 );
		*/
		SI16 PartyMinimizedSlot = 0, PrivateChatMinimizedSlot = 0, HelpButtonMiniMizedSlot = 0, LoveLevelMiniMizedSlot = 0 , FamilyCookieEffect = 0  , FamilyMini = 0, siDormancyMiniMizedSlot = 0;

		if( pclclient->m_pDialog[NPARTY_DLG])
				PartyMinimizedSlot = ((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->GetMinimizedSlot();

		if( pclclient->m_pNPrivateChatDlgMgr)
			PrivateChatMinimizedSlot = pclclient->m_pNPrivateChatDlgMgr->GetMinimizedSlot();

		if(pclClient->IsCountrySwitch(Switch_FreshManHelpDialog))
		{
			if( pclclient->m_pDialog[NHELPBUTTON_DLG])
				HelpButtonMiniMizedSlot = ((CNHelpButtonDlg*)pclclient->m_pDialog[NHELPBUTTON_DLG])->GetMinimizedSlot();
		}

		if ( pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG] )
			LoveLevelMiniMizedSlot = ((CNMarriageLoveLevelDlg*)pclclient->m_pDialog[NMARRIAGELOVELEVEL_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG] )
			FamilyCookieEffect = ((CNFamilyCookieDlg*)pclclient->m_pDialog[NFAMILYCOOKIELEVEL_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NFAMILYINFO_DLG] )
			FamilyMini = ((CNFamilyInfoDlg*)pclclient->m_pDialog[NFAMILYINFO_DLG])->GetMinimizedSlot();

		if ( pclclient->m_pDialog[NDORMANCYPOINT_DLG] )
			siDormancyMiniMizedSlot = ((CNDormancyPointDlg*)pclclient->m_pDialog[NDORMANCYPOINT_DLG])->GetMinimizedSlot();

		// 빈 공간을 찾는다.
		for(SI16 i = 1; i <= MAX_MINIMIZE_SLOT; i++)
		{
			if( i == PartyMinimizedSlot || i == PrivateChatMinimizedSlot || i == HelpButtonMiniMizedSlot || i == LoveLevelMiniMizedSlot || i == FamilyCookieEffect || i == FamilyMini || i == siDormancyMiniMizedSlot)
				continue;

			m_siMinimizedSlot = i;
			break;
		}

		// 슬롯에 따른 x 좌표로 움직여 준다.
		SetDialogPos( pclclient->GetScreenXSize() - 40 - (m_siMinimizedSlot-1) * 45 , 215, 34, 34 );

		SetFileName(NULL);
		SetTitle(NULL);

		//m_pImageMinimized->Show(FALSE);
		m_pImageMinimizedButton->Show(FALSE);
		m_pNLetterStatic->Show(FALSE);
		m_pNLetterButton1->Show(FALSE);
		m_pNLetterButton2->Show(FALSE);
		m_pImageMinimizedButton->Show(TRUE);
		//m_pImageMinimized->Show(TRUE);
		m_pCloseButton->Show(FALSE);
	}
	else
	{
		SetMoveable(TRUE);
		SetDialogPos( m_siOldPosX, m_siOldPosY, 320, 100 );
		SetFileName("ninterface/interface/interface_20x20_00_109.spr");
		
		//m_pImageMinimized->Show(TRUE);
		m_pImageMinimizedButton->Show(TRUE);
		m_pNLetterStatic->Show(TRUE);
		m_pNLetterButton1->Show(TRUE);
		m_pNLetterButton2->Show(TRUE);
		m_pImageMinimizedButton->Show(FALSE);
		//m_pImageMinimized->Show(FALSE);
		SetTitle(m_szHiddenTitle);
		m_pCloseButton->Show(TRUE);
		m_siMinimizedSlot = 0;
	}
	m_bMini = bMini;

}
void CNLetterDlg::Action()
{
	/*
	if(m_bMini)
	{	
		if(m_pImageMinimized == NULL) return;
		
		// 0 과 1이 반복됨
		SI32 imgfont = m_pImageMinimized->GetFontIndex() + 1;
		if(imgfont >= 4 ) imgfont = 2; 
		m_pImageMinimized->SetFontIndex( imgfont );
	}
*/
	/*
	Popupflag++;
	if(Popupflag>200)
	{
		DeleteDialog();
	}
	*/
}

/*
void CNLetterDlg::Initialize()
{
	m_hAnswerEditBrush = NULL ;

	return;
}
*/



void CNLetterDlg::Create()
{
	
	

	SetActionTimer( POPUP_TIME );		
	//CDialogController::Create(NLETTER_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StatiCNLetterDlgProc, TEXT("받은 쪽지"), TEXT("NInterface/Interface/INTERFACE_20X20_00_109.spr"), 9, true, 100, 100,320,100, true, false, true);

	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/DLG_NLetter.ddf"));
	file.CreatePopupDialog( this, NLETTER_DLG, TEXT("dialog_NLetter"),StatiCNLetterDlgProc);
	
	/*if(gsiResolution==2)
	{
        MovePos( 670, 66 );		
	}	
	else
	{
		MovePos( 514, 66 );
	}*/	
	
	//m_pImageMinimized = new CImageStatic( this );
    m_pNLetterStatic = new CStatic( this );             
	m_pNLetterButton1 = new CButton( this );     
	m_pNLetterButton2 = new CButton( this ); 
	m_pMinimizeButton = new CButton( this );
	m_pImageMinimizedButton = new CButton ( this );

	//file.CreateControl(m_pImageMinimized,NLETTER_DIALOG_IMAGESTATIC , TEXT("imagestatic_Note"));
	file.CreateControl(m_pNLetterStatic, NLETTER_DIALOG_STATIC , TEXT("statictext_NLetter"));
	m_pNLetterStatic->SetBorder(true);
	file.CreateControl(m_pNLetterButton1, NLETTER_DIALOG_BUTTON1 , TEXT("button_NLetter1"));
	file.CreateControl(m_pNLetterButton2, NLETTER_DIALOG_BUTTON2 , TEXT("button_NLetter2"));       
	
	file.CreateControl(m_pMinimizeButton, NLETTER_DIALOG_MINIMIZE , TEXT("button_minimize"));
	file.CreateControl(m_pImageMinimizedButton, NLETTER_DIALOG_BUTTON_IMAGEMINIMIZED , TEXT("button_pimageminimized"));

	m_pMinimizeButton->Show(FALSE);
	//m_pImageMinimized->Show(FALSE);
	m_pImageMinimizedButton->Show(FALSE);

	m_bShow = false;

	return;
}

void CALLBACK CNLetterDlg::StatiCNLetterDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
		
	CNLetterDlg *pThis = (CNLetterDlg*) pControllerMgr;
	pThis->NLetterDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNLetterDlg::NLetterDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch (nControlID  )
	{	
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NLETTER_DIALOG_MINIMIZE:
		{
			switch( nEvent )
			{		
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(true);
				}
			}
		}
        break;
	case NLETTER_DIALOG_BUTTON_IMAGEMINIMIZED:
		{
			switch( nEvent )
			{		
				case EVENT_BUTTON_CLICKED:
					{
						Minimi(FALSE);
					}
					break;
			}
		}
		break;
	case NLETTER_DIALOG_BUTTON1:
	{
		switch( nEvent )
		{		
		case EVENT_BUTTON_CLICKED:
			{
					
				if ( m_pCurrentLetter )
				{
						
					if ( m_pCurrentLetter->GetPre() )
					{
							
						stLetterInsert LetterInsert;
						UI32 uiDataSize = 0;
						TCHAR Buffer[256];
						TCHAR * pText;
						m_pCurrentLetter = m_pCurrentLetter->GetPre();
						m_pCurrentLetter->GetData( (BYTE*)&LetterInsert, &uiDataSize );
						

						if(LetterInsert.siTitleTxtMgr == 0 )
						{
							pText = GetTxtFromMgr(2313);	//[영진] 기본적으로 "받은 쪽지, %d/%d" 을 쓴다.							
						}
						else
						{
							pText = GetTxtFromMgr(LetterInsert.siTitleTxtMgr); // 변경 가능							
						}

						//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), LetterInsert.strLetterMsg );
						m_pNLetterStatic->SetBorder(true);
						m_pNLetterStatic->SetBKColorUse(true);

						m_pNLetterStatic->SetText(LetterInsert.strLetterMsg, DT_WORDBREAK);


						CObj* pTemp = m_pLetterMgr->GetExistList()->GetLast();

						SI32 siTotalCount = m_pLetterMgr->GetExistList()->GetObjNum();
						SI32 siCount = 0;
							
						while ( pTemp )							
						{								
							if ( pTemp == m_pCurrentLetter )
								{
									
									StringCchPrintf( Buffer, 256, pText, siTotalCount - siCount, siTotalCount );
									if(m_bMini == FALSE)
										SetTitle(Buffer);
									
									StringCchCopy(m_szHiddenTitle,256,Buffer);

									//m_pNLetterStatic->SetBorder(true);
									//m_pNLetterStatic->SetText(Buffer, DT_WORDBREAK);
									
									//SetWindowText( m_hDlg,  );

									if(siCount == siTotalCount - 1) // 첫번째 노드라면 <- 버튼을 비활성화 시킨다.
									{
										m_pNLetterButton1->Enable(false);
									}
									else
									{
										m_pNLetterButton1->Enable(true);
									}

									m_pNLetterButton2->Enable(true);


									break;
								}
								pTemp = pTemp->GetPre();
								++siCount;
							
						}//end while

							
						
					}
					
				}//end if
				
			}
			break;	
			
		}//end switch	
	}break;

	
	
	case NLETTER_DIALOG_BUTTON2:
	{
		switch( nEvent )
		{		
		case EVENT_BUTTON_CLICKED:
			{
							
				if ( m_pCurrentLetter )
				{						
					if ( m_pCurrentLetter->GetNext() )					
					{
							
						stLetterInsert LetterInsert;
						UI32 uiDataSize = 0;
						TCHAR Buffer[256];
						TCHAR * pText;

						m_pCurrentLetter = m_pCurrentLetter->GetNext();
						m_pCurrentLetter->GetData( (BYTE*)&LetterInsert, &uiDataSize );	

						if(LetterInsert.siTitleTxtMgr == 0 )
						{
							pText = GetTxtFromMgr(2313);	//[영진] 기본적으로 2313 을 쓴다.							
						}
						else
						{
							pText = GetTxtFromMgr(LetterInsert.siTitleTxtMgr); // 변경 가능							
						}

						m_pNLetterStatic->SetBorder(true);
						m_pNLetterStatic->SetBKColorUse(true);

                        m_pNLetterStatic->SetText(LetterInsert.strLetterMsg,DT_WORDBREAK);
						//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), LetterInsert.strLetterMsg );
						CObj* pTemp = m_pLetterMgr->GetExistList()->GetLast();
						SI32 siTotalCount = m_pLetterMgr->GetExistList()->GetObjNum();
						SI32 siCount = 0;
							
						while ( pTemp )							
						{								
							if ( pTemp == m_pCurrentLetter )								
							{		
								StringCchPrintf( Buffer, 256, pText, siTotalCount - siCount, siTotalCount );
								if(m_bMini == FALSE)
									SetTitle(Buffer);

								StringCchCopy(m_szHiddenTitle,256,Buffer);
								//SetWindowText( m_hDlg, Buffer );
								//m_pNLetterStatic->SetBorder(true);
								//m_pNLetterStatic->SetText(Buffer, DT_WORDBREAK);

								m_pNLetterButton1->Enable(true);
								
								if( siCount == 0 )
								{
									m_pNLetterButton2->Enable(false);	
								}
								else
								{
									m_pNLetterButton2->Enable(true);
								}
                                

								break;								
							}								
							pTemp = pTemp->GetPre();
							++siCount;
							
						}//end while

						
					}
					
				}//end if

			}break;
		}	
	  }break;
	}//end switch
	

	return ;

	
}


/*

void CNLetterDlg::Show()
{

	CDialogController::Show( true);
	if ( m_bShow )
	{
		return;
	}

	//m_bShow = true;	
	//ShowWindow ( m_hDlg, SW_SHOWNORMAL );

	return;
}

void CNLetterDlg::Hide()
{

	CDialogController::Show( false );

	if ( ! m_bShow )
	{
		return;
	}
	m_bShow = false;
	//ShowWindow( m_hDlg, SW_HIDE );
	m_pLetterMgr->Clear();

	return;
}

bool CNLetterDlg::IsShow()
{
	return m_bShow;
}
*/


void CNLetterDlg::ShowMinimizeButton(bool haveMinimizeButton)
{
	if(m_pMinimizeButton == NULL) return;
	
	m_pMinimizeButton->Show(haveMinimizeButton);
}



bool CNLetterDlg::Push( TCHAR *strLetterMsg, SI32 siTitleTxtMgr )
{
	if( m_bMini == TRUE )
	{
		Minimi(FALSE);
	}

	if ( m_pLetterMgr->Push( strLetterMsg,siTitleTxtMgr ) )
	{
		SI32 ExistLetterNum = m_pLetterMgr->GetExistList()->GetObjNum();
	
		TCHAR Buffer[256];
		
		if(siTitleTxtMgr == 0 )
		{
			TCHAR*	 pText = GetTxtFromMgr(2313);	//[영진] 기본적으로 2313 을 쓴다.  받은 쪽지, %d/%d
			StringCchPrintf( Buffer, 256, pText, ExistLetterNum, ExistLetterNum );
		}
		else
		{
			TCHAR*	 pText = GetTxtFromMgr(siTitleTxtMgr); // 변경 가능
			StringCchPrintf( Buffer, 256, pText, ExistLetterNum, ExistLetterNum );
		}
				
		
		m_pCurrentLetter = m_pLetterMgr->GetExistList()->GetLast();
		
		

		if(m_bMini == FALSE)
			SetTitle(Buffer);

		StringCchCopy(m_szHiddenTitle,256,Buffer);
		m_pNLetterStatic->SetBorder(true);
		m_pNLetterStatic->SetBKColorUse(true);

		m_pNLetterStatic->SetText(strLetterMsg ,DT_WORDBREAK);
		
		m_pNLetterButton2->Enable(false);				   // 처음 메시지니까 -> 버튼 비활성화

        if(m_pLetterMgr->GetExistList()->GetObjNum() == 1) // 메시지가 하나밖에 없다면 <- 버튼도 비활성화
			m_pNLetterButton1->Enable(false);
		else
			m_pNLetterButton1->Enable(true);
		
		//SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_LETTER_DATA ), strLetterMsg );
		//m_pNLetterStatic->SetText(Buffer,DT_WORDBREAK);

		
		//Show();
		return true;
	}

	return false;
}


