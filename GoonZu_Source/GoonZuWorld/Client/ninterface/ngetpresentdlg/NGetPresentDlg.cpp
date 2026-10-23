#include "../../ninterface/ngetpresentdlg/NGetPresentDlg.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "NThanksPresentDlg.h"

#include "Msg/MsgType-ItemMall.h"

#include "../../Client.h"

extern cltCommonLogic* pclClient;

CNGetPresentDlg::CNGetPresentDlg()
{
	m_siItemId = -1;
	m_siLetterNum = 0;
	m_siCurLetter = 0;
	m_siCurLetterServerUnique = 0;
	m_szCurLetterPlayerAccount[0] = '\0';
	m_szCurLetterPlayerName[0] = '\0';
}

CNGetPresentDlg::~CNGetPresentDlg()
{
}

void CNGetPresentDlg::Create()
{
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NGetPresentDlg/DLG_NGetPresent.ddf"));
		file.CreatePopupDialog( this, NGETPRESENT_DLG, TEXT("dialog_NGetPresent"), StatiCNGetPresentDlg);


		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGETPRESENT_STATIC1 , this ), NGETPRESENT_STATIC1,   "statictext_NONAME1"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGETPRESENT_STATIC2 , this ), NGETPRESENT_STATIC2,   "statictext_NONAME2"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NGETPRESENT_PAGE , this ), NGETPRESENT_PAGE,   "statictext_Page"); 	
		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NGETPRESENT_BUTTON_GET_NOW, this ), NGETPRESENT_BUTTON_GET_NOW,   "button_NONAME1"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NGETPRESENT_BUTTON_GET_LATER, this ), NGETPRESENT_BUTTON_GET_LATER,   "button_NONAME2"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NGETPRESENT_BUTTON_ANSWER, this ), NGETPRESENT_BUTTON_ANSWER,   "button_NONAME3"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NGETPRESENT_BUTTON_PRE, this ), NGETPRESENT_BUTTON_PRE,   "button_Pre"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NGETPRESENT_BUTTON_NEXT, this ), NGETPRESENT_BUTTON_NEXT,   "button_Next"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NGETPRESENT_IMAGESTATIC1, this ), NGETPRESENT_IMAGESTATIC1,   "imagestatic_NONAME1"); 


		m_PresentListItr = m_PresentList.end();

	}
}


void CALLBACK CNGetPresentDlg::StatiCNGetPresentDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGetPresentDlg *pThis = (CNGetPresentDlg*) pControllerMgr;
	pThis->NGetPresentDlgdProc( nEvent, nControlID, pControl );
}



void CALLBACK CNGetPresentDlg::NGetPresentDlgdProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID)
	{
	
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	
	case NGETPRESENT_BUTTON_GET_NOW:
		{				
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					// 아이템몰 아이템 받기 창을 보여준다.
                    
  					if(pclclient->m_pDialog[NITEMMALL_DLG] == NULL)
					{
						pclclient->CreateInterface( NITEMMALL_DLG );             
					}

					cltGameMsgRequest_ItemMallItemList clinfo(1);
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				//	DeleteDialog();

				}
				break;
			}
		}
		break;

	case NGETPRESENT_BUTTON_GET_LATER:
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

	case NGETPRESENT_BUTTON_ANSWER:
		{
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					// 답장 창 생성후 데이터 설정
					if( m_szCurLetterPlayerAccount[0] == '\0' 
						|| m_szCurLetterPlayerName[0] == '\0' 
						|| m_siCurLetterServerUnique <= 0 
						|| m_siCurLetterServerUnique > MAX_SERVER_NUMBER
						)
					{
						break;
					}						

 					if( pclclient->m_pDialog[NTHANKSPRESENT_ANSWER_DLG] == NULL )
					{
                        pclclient->CreateInterface(NTHANKSPRESENT_ANSWER_DLG);
					}					
					if( pclclient->m_pDialog[NTHANKSPRESENT_ANSWER_DLG] != NULL )
					{
						((CNThanksPresentDlg *)pclclient->m_pDialog[NTHANKSPRESENT_ANSWER_DLG])->SetSendLetterData( m_siCurLetterServerUnique,m_szCurLetterPlayerName,m_szCurLetterPlayerAccount );
					}
				}
				break;
			}
		}
		break;

	case NGETPRESENT_BUTTON_PRE:
		{
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					PreButtonClicked();					
				}
				break;
			}
		}
		break;
	case NGETPRESENT_BUTTON_NEXT:
		{
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					NextButtonClicked();
				}
				break;
			}
		}
		break;
	}
	return ;
}

void CNGetPresentDlg::PreButtonClicked()
{
	if( m_siCurLetter == 1 ) return;


	m_siCurLetter--;

	if(m_siCurLetter <= 0)
		m_siCurLetter = 1;

	SetCurPresentData();
}

void CNGetPresentDlg::NextButtonClicked()
{
	list<stPresentData>::size_type	PresentDataNum;
	PresentDataNum = m_PresentList.size();

	if( (SI32)PresentDataNum == m_siCurLetter) return;

	m_siCurLetter++;

	if(m_siCurLetter > (SI32)PresentDataNum)
		m_siCurLetter = PresentDataNum;

	SetCurPresentData();

}

void CNGetPresentDlg::SetCurPresentData()
{
	cltClient * pclclient = (cltClient*) pclClient;

	list<stPresentData>::size_type	PresentDataNum;
	PresentDataNum = m_PresentList.size();
	
	if( PresentDataNum <= 0 ) return;

	if( m_siCurLetter <= 0 || m_siCurLetter > (SI32)PresentDataNum  )
	{
		m_siCurLetter = 1;
	}

	m_PresentListItr = m_PresentList.begin();

	for( SI32 i = 0; i < m_siCurLetter-1; i++ )
	{
		if( m_PresentListItr == m_PresentList.end() )
		{
			break;
		}	

		m_PresentListItr++;        
	}

	stPresentData * pPresentData;
		
	pPresentData = &(*m_PresentListItr);	// 값을 가져온다.
	
	// 이미지 인덱스 파일이 하나 더 늘었기때문에, 검사를 해주어야 한다.
	TCHAR szImageFilename[256];
	SI32 siImageFontindex;

	pclClient->pclItemManager->GetItemImageItemmallFileName( pPresentData->siItemImageFont, szImageFilename,sizeof(szImageFilename), &siImageFontindex);

	m_InterfaceFactory.GetImageStatic(NGETPRESENT_IMAGESTATIC1)->SetFileName(szImageFilename);
	m_InterfaceFactory.GetImageStatic(NGETPRESENT_IMAGESTATIC1)->SetFontIndex(siImageFontindex);
	//------------------------------------------------------------------------------------------------------------------------------------------------------

	TCHAR szBuffer[256] = {'\0',};
	TCHAR* pServerName = pclclient->GetServerNameFromUnique(pPresentData->siFromServerUnique );

	StringCchPrintf( szBuffer,256,TEXT("%d/%d"), m_siCurLetter, PresentDataNum );
	
    m_InterfaceFactory.GetStatic(NGETPRESENT_PAGE)->SetText(szBuffer,DT_CENTER);

	//[진성] 선물하기. 메시지 분류. => 2008-7-25
	//[%s]서버의 [%s]님이 \r귀하에게 [%s] [%d]개를 선물하였습니다.
	TCHAR * pText = NULL;
			
	if(0 < strlen(pPresentData->szItemName))
	{		
		pText = GetTxtFromMgr( 6098 );
		StringCchPrintf( szBuffer, 256, pText, pServerName  , pPresentData->szFromPlayerName, pPresentData->szItemName, pPresentData->siItemNum  )	;
	}
	else if(0 < strlen(pPresentData->szFromPlayerName))
	{
		pText = GetTxtFromMgr( 8652 );
		StringCchPrintf( szBuffer, 256, pText, pPresentData->szFromPlayerName );
	}
	
    
    m_InterfaceFactory.GetStatic(NGETPRESENT_STATIC2)->SetText(szBuffer);
	m_InterfaceFactory.GetStatic(NGETPRESENT_STATIC1)->SetText(pPresentData->szComment);
	
	m_siCurLetterServerUnique = pPresentData->siFromServerUnique;
	StringCchCopy(m_szCurLetterPlayerAccount,MAX_PLAYER_NAME,pPresentData->szFromPlayerAccount);
	StringCchCopy(m_szCurLetterPlayerName,MAX_PLAYER_NAME,pPresentData->szFromPlayerName);

}

void CNGetPresentDlg::SetPresentData( SI32 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment, TCHAR * ItemName
					, SI32 	ItemNum, SI32 ItemImageFont, TCHAR * FromPlayerAccount) 					
{
	stPresentData PresentData(FromServerUnique,FromPlayerName,Comment,ItemName,ItemNum,ItemImageFont,FromPlayerAccount);
    
	m_PresentList.push_back( PresentData );
	
	SetCurPresentData();

	
}


