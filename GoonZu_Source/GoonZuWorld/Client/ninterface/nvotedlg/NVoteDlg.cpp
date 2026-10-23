#include "NVoteDlg.h"

#include "../../Client/client.h"
//#include "../../CommonLogic/MsgType-Person.h"

#include "../../CommonLogic/Msg/MsgType-Vote.h"

#include "../../Common/VoteMgr/VoteBaseInfo.h"
#include "../../Common/VoteMgr/VoteMgr.h"

#include "../../interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../lib/WebHTML/WebHTML.h"
#include "Resource.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;
stListBoxItemData voteListBoxItemData;

CNVoteDlg::CNVoteDlg()
{
	m_bShow = false;	
	m_pstCandidatureList = new stCandidature[ MAX_CANDIDATURE_NUM ];
	memset( m_pstCandidatureList, 0, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );


	m_pVoteKingList    = NULL;
	m_pVoteKingButton1 = NULL;
	m_pVoteKingButton2 = NULL;  
	
}

CNVoteDlg::~CNVoteDlg()
{
	SAFE_DELETE(m_pVoteKingList);
	SAFE_DELETE(m_pVoteKingButton1);
	SAFE_DELETE(m_pVoteKingButton2);
}

void CNVoteDlg::Initialize()
{
	memset( m_pstCandidatureList, 0, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );
	
	return;
}

void CNVoteDlg::Create( )
{
	if(!IsCreate())
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NVoteKing/DLG_NVoteKing.ddf"));
		file.CreatePopupDialog( this, NVOTEKING_DLG, TEXT("dialog_VoteKing"), StaticNVoteDlgProc);


		m_pVoteKingList    = new CList( this ); 
		m_pVoteKingButton1 = new CButton( this );
		m_pVoteKingButton2 = new CButton( this );	
	 

		file.CreateControl(m_pVoteKingList,		NVOTEKING_LIST,		TEXT("listbox_VoteKing"));
		file.CreateControl(m_pVoteKingButton1,	NVOTEKING_BUTTON1,	TEXT("button_VoteKing1"));	
		file.CreateControl(m_pVoteKingButton2,	NVOTEKING_BUTTON2,	TEXT("button_VoteKing2"));

		//m_pVoteKingList->Clear();
		TCHAR* pText = GetTxtFromMgr(2469);
		m_pVoteKingList->SetColumn( 0, 20, TEXT("") );
		m_pVoteKingList->SetColumn( 1, 212, TEXT("") );
		m_pVoteKingList->SetBorder(true);
		m_pVoteKingList->SetBKColorUse(true);
		LoadCandidatureList();
		m_pVoteKingList->Refresh();

		//대만은 유세보기 버튼 비활성화 (PCK - 08.01.08)
		if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
		{
			m_pVoteKingButton2->Enable(FALSE);
		}
	}
	

	return;
}

void CALLBACK CNVoteDlg::StaticNVoteDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNVoteDlg *pThis = (CNVoteDlg*) pControllerMgr;
	pThis->NVoteDlgProc( nEvent, nControlID, pControl );
	
}

void CALLBACK CNVoteDlg::NVoteDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	InitCommonControls();


	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:	
			{
				DeleteDialog();
			}
			break;

		case NVOTEKING_BUTTON1:
			{
				switch(nEvent)
				{				

				case EVENT_BUTTON_CLICKED:
					{
						SI16 index = m_pVoteKingList->GetSelectedIndex();
						if ( index < 0 ) break ;
						
						stListBoxItemData* pvoteListBoxItemData=NULL;
						pvoteListBoxItemData = m_pVoteKingList->GetItemData( index, 0 );
			
						m_iSelectVoteInx=index;
				
						TCHAR NameBuffer[ MAX_PLAYER_NAME ] = TEXT("");
						
						StringCchCopy(NameBuffer, MAX_PLAYER_NAME, m_pVoteKingList->GetText(m_iSelectVoteInx,1));					

						if ( NameBuffer == NULL || _tcslen ( NameBuffer ) < 1 )
						{
							break;
							
						}

						
						SI16 i = 0;
						for ( ; i < MAX_CANDIDATURE_NUM; ++i )
						{
							if ( m_pstCandidatureList[ i ].siListIndex > 0 )
							{
								if ( _tcscmp( m_pstCandidatureList[ i ].strChatName, NameBuffer ) == 0 )
								{
									break;
								}
							}
						}

						if ( i == MAX_CANDIDATURE_NUM )
						{
							break;
							
						}		

						NTCHARString64	kTitle( GetTxtFromMgr(8295) );
						NTCHARString256 kText( GetTxtFromMgr(8294) );
						stMsgBox		MsgBox;
												

						kText.Replace( TEXT("#name#"), m_pVoteKingList->GetText(m_iSelectVoteInx, 1) );
		
						MsgBox.Set( this, kTitle, kText, MSGBOX_TYPE_YESNO, 0 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					}
					break;
				}
			}
			break;


		case NVOTEKING_BUTTON2:
			{
				
				switch(nEvent)
				{				

				case EVENT_BUTTON_CLICKED:
					{
						
						SI16 index = m_pVoteKingList->GetSelectedIndex();
						if( index < 0 ) break;


						m_iSelectVoteInx=index;
						TCHAR NameBuffer[ MAX_PLAYER_NAME ] = TEXT("");
						TCHAR Url[256]= TEXT("");
						StringCchCopy(NameBuffer, MAX_PLAYER_NAME, m_pVoteKingList->GetText(m_iSelectVoteInx,1));

						if ( NameBuffer == NULL || _tcslen ( NameBuffer ) < 1 )
						{
							break;
							
						}

						SI16 i = 0;
						for (i=0 ; i < MAX_CANDIDATURE_NUM; ++i )
						{
							if ( m_pstCandidatureList[ i ].siListIndex > 0 )
							{
								if ( _tcscmp( m_pstCandidatureList[ i ].strChatName, NameBuffer ) == 0 )
								{
									break;
								}
							}
						}

						if ( i == MAX_CANDIDATURE_NUM )
						{
							break;
							
						}

						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), m_pstCandidatureList[ i ].strWebPageURL , NULL, SW_SHOWNORMAL );
											
					}						
					break;
				}

			}
			break;
	
	}
	
	return;
}




void CNVoteDlg::LoadCandidatureList()
{
	NTCHARString64	kName( TEXT("") );
	SI32			siListIndex = 0;

	m_pVoteKingList->Clear();


	for ( SI32 i = 0; i < MAX_CANDIDATURE_NUM; ++i )
	{
	
		if ( m_pstCandidatureList->siListIndex > 0 )
		{
			kName		= TEXT("");
			kName		= m_pstCandidatureList[i].strChatName;

			siListIndex = m_pstCandidatureList[i].siListIndex;

			// 혹시 이름이 없을지도 모르니
			if ( kName.Compare( TEXT("") ) == 0 )
			{
				continue;
			}

			voteListBoxItemData.Init();
			voteListBoxItemData.Set( 0, SI32ToString(siListIndex) );
			voteListBoxItemData.Set( 1, kName );
			
			voteListBoxItemData.siParam[0] = i;

			m_pVoteKingList->AddItem( &voteListBoxItemData );	
		}

	}
	m_pVoteKingList->Refresh();

	return;
}

void CNVoteDlg::Set( TCHAR *VoteExplain, stCandidature *pCandidature )
{
	if(!IsCreate())
	{
		Create();
	}
	
	memcpy( m_pstCandidatureList, pCandidature, sizeof( stCandidature ) * MAX_CANDIDATURE_NUM );
	LoadCandidatureList();
	m_pVoteKingList->Refresh();
	
	return;
}

void CNVoteDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch ( MsgIndex ) 
	{
		case 0:
			{
				if ( RetValue )	
				{

					// 실제 투표에 참여
					cltGameMsgRequest_VoteKingSelection clVoteKingSelection( m_pstCandidatureList[ m_iSelectVoteInx ].siListIndex );
					cltMsg clMsg( GAMEMSG_REQUEST_VOTEKINGSELECTION, sizeof(clVoteKingSelection), (BYTE*)&clVoteKingSelection );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					DeleteDialog();

				}
			}
			break;
	
	}
}