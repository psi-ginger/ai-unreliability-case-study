#include "../Client/NStructureDlg/House/UserInfo/NHouseUserHorseDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"
#include "../Client/InterfaceMgr/Interface/OutLine.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../Client/Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"

#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"

//#include "../../FontManager/FontManager.h"
//#include "../../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType-House.h"
#include "../../CommonLogic/Village/Village.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"


extern cltCommonLogic* pclClient;

CNHouseUserHorseDlg::CNHouseUserHorseDlg() : CDialogController()
{
	m_pclHouseUnit = NULL;

	m_pclHouseUnitHorseList = new cltHorse[ MAX_HOUSEUNIT_HORSENUMBER ];
	memset ( m_pclHouseUnitHorseList, 0, sizeof( cltHorse ) * MAX_HOUSEUNIT_HORSENUMBER );

	for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
	{
		m_bStableCreated[ i ] = false;
	}

	m_siCurrentVillageIndex = 0;
	m_siCurrentHouseIndex = 0;
	m_siSelectHorseIndex = -1;

	m_pOutLine = NULL;
	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pList = NULL;
	m_pEdit = NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
}

CNHouseUserHorseDlg::~CNHouseUserHorseDlg()
{
	if ( m_pclHouseUnitHorseList )
	{
		delete m_pclHouseUnitHorseList;
		m_pclHouseUnitHorseList = NULL;
	}

	SAFE_DELETE(m_pOutLine);
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pList);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
}

void CNHouseUserHorseDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserHorse.ddf"));
	file.CreateChildDialog( this, NHOUSEUSERHORSE_DLG, TEXT("dialog_HouseUserHorse"), StaticCallBackDialogNHouseUserHorseDlg, pParent );

	m_pOutLine = new COutLine( this );

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);
	NEWCLIST(m_pList);
	NEWCEDIT(m_pEdit);
	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	NEWCBUTTON(m_pButton[2]);

	
	file.CreateControl( m_pStatic[0], NHOUSEUSERHORSE_DIALOG_STATIC, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOUSEUSERHORSE_DIALOG_STATIC, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHOUSEUSERHORSE_DIALOG_STATIC, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pList, NHOUSEUSERHORSE_DIALOG_LIST, TEXT("listbox_NONAME1") );
	file.CreateControl( m_pEdit, NHOUSEUSERHORSE_DIALOG_STATIC, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pButton[0], NHOUSEUSERHORSE_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHOUSEUSERHORSE_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	file.CreateControl( m_pButton[2], NHOUSEUSERHORSE_DIALOG_BUTTON3, TEXT("button_NONAME3") );

	m_pList->SetColumn(0, 152, TEXT(""));
	m_pList->SetBorder(TRUE);
	m_pList->SetBKColorUse(TRUE);
	m_pList->Refresh();

	file.CreateControl( m_pOutLine, NHOUSEUSERHORSE_DIALOG_STATIC, TEXT("outline_NONAME1") );
	
	EnableButton(m_pButton[0], FALSE);
	EnableButton(m_pButton[1], FALSE);	// 말 넣기
	EnableButton(m_pButton[2], FALSE);	// 말 꺼내기

	Hide();
}

void CNHouseUserHorseDlg::Hide()
{
	m_siSelectHorseIndex = -1;
	CControllerMgr::Show( false );
}

void CNHouseUserHorseDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNHouseUserHorseDlg::Action()
{
/*	if(m_pList->GetSelectedIndex() < 0)
	{
		EnableButton(m_pButton[1], FALSE);	// 말 넣기
		EnableButton(m_pButton[2], FALSE);	// 말 꺼내기
	}
	else
	{
		EnableButton(m_pButton[1], TRUE);	// 말 넣기
		EnableButton(m_pButton[2], TRUE);	// 말 꺼내기
	}*/
	// 시전 소유주가 아닌경우 타이머 돌 필요 없으므로...
	if( m_pclHouseUnit != NULL &&
		m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
	{
		return;
	}

	if ( ! IsHorseListChanged() )
	{
		return;
	}

	LoadHorseList();	
}

void CALLBACK CNHouseUserHorseDlg::StaticCallBackDialogNHouseUserHorseDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserHorseDlg *pThis = (CNHouseUserHorseDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserHorseDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserHorseDlg::CallBackDialogNHouseUserHorseDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	TCHAR Nbuffer[512]=TEXT(" ");

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
		}
		break;

	case NHOUSEUSERHORSE_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 SelectedListIndex = m_pList->GetSelectedIndex();
					
					if ( SelectedListIndex < 0 )
					{
						return;
					}

				//	EnableButton(m_pButton[1], TRUE);	// 말 넣기
				//	EnableButton(m_pButton[2], TRUE);	// 말 꺼내기

					m_siSelectHorseIndex = SelectedListIndex;

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					// 말이 없으므로, 설명할 게 없음.
					if ( pStgInfo->clHorse[ m_siSelectHorseIndex ].siHorseUnique < 1 )
					{
						// 마구간도 안 지어진 경우
						if ( pStgInfo->bCreatedHouseUnitStable[ m_siSelectHorseIndex ] == false )
						{
						//	SetWindowText( GetDlgItem ( hDlg, IDC_EDIT_HOUSEUNIT_HORSEEXPLAIN ), TEXT("말을 넣을 수 있도록 마구간을 지어주세요.") );
							TCHAR* pText = GetTxtFromMgr(3898);
							m_pEdit->SetText(pText);
							m_pStatic[2]->SetFontIndex(0);
							EnableButton(m_pButton[0], TRUE);
							EnableButton(m_pButton[1], FALSE);
							EnableButton(m_pButton[2], FALSE);

						/*	TCHAR strImagePath[ _MAX_PATH ] = TEXT("");
							GetCurrentDirectory( _MAX_PATH, strImagePath );

							int BufLen = _tcslen( strImagePath );
							if( strImagePath [ BufLen - 1 ] == '\\' )
							{
								strcat( strImagePath, TEXT("Interface\\Stable\\BeforeStable.html") );
							}
							else
							{				
								strcat( strImagePath, TEXT("\\Interface\\Stable\\BeforeStable.html") );
							}							

							//==================================
							// 화면에 보여준다.
							HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_HOUSEUNIT_HORSEIMAGE);
							if(hwnd)
							{
								DisplayHTMLPage(hwnd, strImagePath);
							}
							//==================================*/
						}
						else		// 마구간 지어 놓고 비어 있는 경우
						{
						//	SetWindowText( GetDlgItem ( hDlg, IDC_EDIT_HOUSEUNIT_HORSEEXPLAIN ), TEXT("말을 넣어주세요. 마구간에 말을 넣어두면 사용자가 접속해 있는 동안 필요한 먹이를 시전에서 자동으로 먹습니다.") );
							TCHAR* pText = GetTxtFromMgr(3899);
							m_pEdit->SetText(pText);
							m_pStatic[2]->SetFontIndex(1);
							EnableButton(m_pButton[0], FALSE);
							EnableButton(m_pButton[1], TRUE);	// 말 넣기
							EnableButton(m_pButton[2], FALSE);
													
						/*	TCHAR strImagePath[ _MAX_PATH ] = TEXT("");
							GetCurrentDirectory( _MAX_PATH, strImagePath );

							int BufLen = _tcslen( strImagePath );

							if( strImagePath [ BufLen - 1 ] == '\\' )
							{
								strcat( strImagePath, TEXT("Interface\\Stable\\Stable.html") );
							}
							else
							{
								strcat( strImagePath, TEXT("\\Interface\\Stable\\Stable.html") );
							}

							//==================================
							// 화면에 보여준다.
							HWND hwnd = GetDlgItem(m_hCurrentTabDlg, IDC_STATIC_HOUSEUNIT_HORSEIMAGE);
							if(hwnd)
							{
								DisplayHTMLPage(hwnd, strImagePath);
							}
							//==================================*/
						}
						return;
					}
					else
					{
						if ( pStgInfo->bCreatedHouseUnitStable[ m_siSelectHorseIndex ] == TRUE )
						{
							EnableButton(m_pButton[0], FALSE);
							EnableButton(m_pButton[1], FALSE);
							EnableButton(m_pButton[2], TRUE);
						}
					}

					/*
					SI16 Count = 0;
					for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
					{
					if ( pStgInfo->clHorse[ i ].siHorseUnique > 0 )
					{
					if ( SelectedListIndex == Count )
					{
					break;
					}

					++Count;
					}
					}

					// 못 찾았음.
					if ( i == MAX_HOUSEUNIT_HORSENUMBER )
					{
					return FALSE;
					}
					*/

					TCHAR Buffer[ 256 ] = TEXT("");
					TCHAR NeedText[ 256 ] = TEXT("");

					if ( pclClient->pclHorseManager->GetText( &pStgInfo->clHorse[ m_siSelectHorseIndex ], Buffer, 256 ) )
					{
						if (pclClient->pclHorseManager->GetNeedText( &pStgInfo->clHorse[ m_siSelectHorseIndex ], NeedText , 256) == true )
						{
							StringCchCat( Buffer, 256, NeedText );
						}
					}

				//	SetWindowText( GetDlgItem ( hDlg, IDC_EDIT_HOUSEUNIT_HORSEEXPLAIN ), Buffer );
					m_pEdit->SetText(Buffer);
					
					switch ( pStgInfo->clHorse[ m_siSelectHorseIndex ].siHorseUnique )
					{
					case 1:
					case 2:
					case 3:
						{
							m_pStatic[2]->SetFontIndex(pStgInfo->clHorse[ m_siSelectHorseIndex ].siHorseUnique + 1 ); // 그림 인덱스 맞추기
						}
						break;
					default:
						{
							m_pStatic[2]->SetFontIndex(2);
						}
                        break;
					}

				/*	TCHAR strImagePath[ _MAX_PATH ] = TEXT("");
					GetCurrentDirectory( _MAX_PATH, strImagePath );	

					int BufLen = _tcslen( strImagePath );

					if( strImagePath [ BufLen - 1 ] == '\\' )
					{

						//sprintf( Buffer, TEXT("Interface\\Stable\\%d.html"), pStgInfo->clPersonHorse[ SelectedListIndex ].clHorse.siHorseUnique );
						switch ( pStgInfo->clHorse[ SelectedListIndex ].siHorseUnique )
						{
						case 1:
						case 2:
						case 3:
							sprintf( Buffer, TEXT("Interface\\Stable\\%d.html"), pStgInfo->clHorse[ SelectedListIndex ].siHorseUnique );
							break;
						default:
							strcpy( Buffer, TEXT("Interface\\Stable\\1.html") );
							break;
						}
						strcat( strImagePath, Buffer );
					}
					else
					{
						//sprintf( Buffer, TEXT("Interface\\Stable\\%d.html"), pStgInfo->clPersonHorse[ SelectedListIndex ].clHorse.siHorseUnique );
						switch ( pStgInfo->clHorse[ SelectedListIndex ].siHorseUnique )
						{
						case 1:
						case 2:
						case 3:
							sprintf( Buffer, TEXT("\\Interface\\Stable\\%d.html"), pStgInfo->clHorse[ SelectedListIndex ].siHorseUnique );
							break;
						default:
							strcpy( Buffer, TEXT("\\Interface\\Stable\\1.html") );
							break;
						}
						strcat( strImagePath, Buffer );
					}



					//==================================
					// 화면에 보여준다.
					HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_HOUSEUNIT_HORSEIMAGE);
					if(hwnd)
					{
						DisplayHTMLPage(hwnd, strImagePath);
					}
					//==================================*/
				}
				break;
			}
		}
		break;

	case NHOUSEUSERHORSE_DIALOG_BUTTON1:	// 마구간 짓기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
	//				EnableButton(m_pButton[0], FALSE);	// 마굿간 짓기
					
					// 소유주가 틀린경우 그냥 return 함
					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						TCHAR* pTitle = GetTxtFromMgr(370);
						TCHAR* pText = GetTxtFromMgr(1196);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					SI16 i = 0;
					for ( ; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
					{
						if ( pStgInfo->bCreatedHouseUnitStable[ i ] == false )
						{
							break;
						}
					}

					if ( i == MAX_HOUSEUNIT_HORSENUMBER )
					{
						TCHAR* pTitle = GetTxtFromMgr(2125);
						TCHAR* pText = GetTxtFromMgr(2126);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, MAX_HOUSEUNIT_HORSENUMBER );
						return;
					}

					//SI32 ItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( ITEMUNIQUE( 24025 ) );

					//if ( ItemPos < 0 || pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemPos ].siItemNum < HOUSEUNIT_CREATESTABLE_USEITEMNUM )

					SI32 siNeedMoney = HOUSEUNIT_CREATESTABLE_NEEDMONEY * pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;				//시세에 따른 마굿간 건설비용 계산

					if ( siNeedMoney == 0 )	return;			//아직 시세가 결정되지 않았다면

					if (  pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < siNeedMoney )
					{
						TCHAR* pTitle = GetTxtFromMgr(2125);
						TCHAR* pText = GetTxtFromMgr(6132);
						StringCchPrintf( Nbuffer, 128, pText ,siNeedMoney );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);

						//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclClient->pclItemManager->GetName( 24025 ), HOUSEUNIT_CREATESTABLE_USEITEMNUM);
						return;
					}
					

					TCHAR FeeExplain[128] = TEXT("");
					TCHAR* pText = GetTxtFromMgr(6133);
					StringCchPrintf(FeeExplain , 128, pText, siNeedMoney );

					pText = GetTxtFromMgr(825);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pText, FeeExplain, MSGBOX_TYPE_YESNO, 4 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( i );
					memcpy( TempBuffer, &i, sizeof( i ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					//if( MessageBox(NULL, FeeExplain, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

					//cltGameMsgRequest_HouseUnitStableCreate clGameMsgRequest_HouseUnitStableCreate( m_siCurrentVillageIndex, m_siCurrentHouseIndex, i );
					//cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITSTABLECREATE, sizeof(clGameMsgRequest_HouseUnitStableCreate), (BYTE*)&clGameMsgRequest_HouseUnitStableCreate );
					//pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHOUSEUSERHORSE_DIALOG_BUTTON2:	// 말 넣기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
				//	EnableButton(m_pButton[0], FALSE);	// 마굿간 짓기
				//	EnableButton(m_pButton[1], FALSE);	

					// 소유주가 틀린경우 그냥 return 함
					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						TCHAR* pTitle = GetTxtFromMgr(370);
						TCHAR* pText = GetTxtFromMgr(1196);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					//pclClient->pclCM->CR[1]->pclCI->clHorseInfo.clHorse[0].

					// 마구간이 만들어져 있고, 거기에 말이 없는 것이 존재하는지 체크
					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					SI32 TempIndex = m_pList->GetSelectedIndex();

					if ( TempIndex < 0 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2129);
						TCHAR* pText = GetTxtFromMgr(2130);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return;
					}

					if(pStgInfo->bCreatedHouseUnitStable[ TempIndex ] == false)
					{
						TCHAR* pTitle = GetTxtFromMgr(2129);
						TCHAR* pText = GetTxtFromMgr(2130);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

						return;
					}

					m_siSelectHorseIndex = TempIndex;

					cltHorse* pclHorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

					// 말을 타고 있는 상태에서, 말을 넣어 버리면, 캐릭터 조합쪽에 에러 날수 있으므로...
					if ( pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
					{
						TCHAR* pTitle = GetTxtFromMgr(372);
						TCHAR* pText = GetTxtFromMgr(2131);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return;
					}

					if ( pclHorse != NULL && pclHorse->siHorseUnique > 0 )
					{						
						// 말 인벤토리에 아이템이 들어 있는지 체크						
						if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.IsHorseHaveItem() == true )
						{
							TCHAR* pTitle = GetTxtFromMgr(2132);
							TCHAR* pText = GetTxtFromMgr(2133);

							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
							return;
						}

						cltGameMsgRequest_HouseUnitHorseIn clGameMsgRequest_HouseUnitHorseIn( m_siCurrentVillageIndex, m_siCurrentHouseIndex,  pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentShowHorse, pclHorse->siHorseUnique, m_siSelectHorseIndex );
						cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITHORSEIN, sizeof(clGameMsgRequest_HouseUnitHorseIn), (BYTE*)&clGameMsgRequest_HouseUnitHorseIn );
						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else
					{
						TCHAR* pTitle = GetTxtFromMgr(372);
						TCHAR* pText = GetTxtFromMgr(3273);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return;
					}
				}
				break;
			}
		}
		break;

	case NHOUSEUSERHORSE_DIALOG_BUTTON3:	// 말 꺼내기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
		//			EnableButton(m_pButton[2], FALSE);	

					// 소유주가 틀린경우 그냥 return 함
					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						TCHAR* pTitle = GetTxtFromMgr(370);
						TCHAR* pText = GetTxtFromMgr(1196);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					SI32 TempIndex = m_pList->GetSelectedIndex();

					if ( TempIndex < 0 )
					{
						TCHAR* pTitle = GetTxtFromMgr(2129);
						TCHAR* pText = GetTxtFromMgr(2134);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					m_siSelectHorseIndex = TempIndex;

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					if ( pStgInfo->clHorse[ m_siSelectHorseIndex ].siHorseUnique < 1 )
					{
						return;
					}

					// 말을 타고 있는 상태에서, 말을 넣어 버리면, 캐릭터 조합쪽에 에러 날수 있으므로...
					if ( pclClient->pclCM->CR[1]->pclCI->clHorseInfo.siCurrentHorseRIDEHORSE == true )
					{
						TCHAR* pTitle = GetTxtFromMgr(372);
						TCHAR* pText = GetTxtFromMgr(2135);

						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					cltGameMsgRequest_HouseUnitHorseOut clGameMsgRequest_HouseUnitHorseOut( m_siCurrentVillageIndex, m_siCurrentHouseIndex, m_siSelectHorseIndex, pStgInfo->clHorse[ m_siSelectHorseIndex ].siHorseUnique );
					cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITHORSEOUT, sizeof(clGameMsgRequest_HouseUnitHorseOut), (BYTE*)&clGameMsgRequest_HouseUnitHorseOut );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	}

}

void CNHouseUserHorseDlg::EnableButton(CButton* pButton, BOOL Flag)
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

void CNHouseUserHorseDlg::SetHouseUserItem(cltHouseUnit *pclHouseUnit)
{
	m_pclHouseUnit	  = pclHouseUnit;
}

void CNHouseUserHorseDlg::SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex)
{
	m_siCurrentVillageIndex = siCurrentVillageIndex;
	m_siCurrentHouseIndex = siCurrentHouseIndex;
}

bool CNHouseUserHorseDlg::IsHorseListChanged()
{
	if ( m_pclHouseUnit == NULL )
	{
		return false;
	}

	cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
	if ( pStgInfo == NULL )
	{
		return false;
	}

	for ( int i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
	{
		if ( m_bStableCreated[ i ] != m_pclHouseUnit->clStorageInfo.bCreatedHouseUnitStable[ i ] )
		{
			return true;
		}

		SI32 Size = sizeof( cltHorse );

		BYTE *src = (BYTE *)&m_pclHouseUnit->clStorageInfo.clHorse[ i ];
		BYTE *dest = (BYTE *)&m_pclHouseUnitHorseList[ i ];

		for( int j = 0; j < Size; ++j )
		{
			if( src[j] != dest[j] )
			{
				return true;
			}
		}
	}

	return false;
}

void CNHouseUserHorseDlg::LoadHorseList()
{
	if ( m_pclHouseUnit == NULL )
	{
		return;
	}
//	SetWindowText( GetDlgItem ( hDlg, IDC_EDIT_HOUSEUNIT_HORSEEXPLAIN ), TEXT("") );
//	HWND hHorseListBox = GetDlgItem ( hDlg, IDC_LIST_HOUSEUNIT_HORSE );
//	if ( hHorseListBox == NULL )
//	{
//		return;
//	}
//	SendMessage( hHorseListBox, LB_RESETCONTENT, 0, 0 );

	if(m_siSelectHorseIndex < 0)
	{
		m_pButton[0]->Enable(false);
		m_pButton[1]->Enable(false);
		m_pButton[2]->Enable(false);
	}
	m_pEdit->SetText(TEXT(""));
	m_pList->Clear();

	// 소유주가 틀린경우 그냥 return 함
	if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
	{
		return;
	}

	for ( int k = 0; k < MAX_HOUSEUNIT_HORSENUMBER; ++k )
	{
		m_bStableCreated[ k ] = m_pclHouseUnit->clStorageInfo.bCreatedHouseUnitStable[ k ];
		m_pclHouseUnitHorseList[ k ] = m_pclHouseUnit->clStorageInfo.clHorse[ k ];
	}	

	cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
	if ( pStgInfo == NULL )
	{
		return;
	}

	stListBoxItemData Data;

	for ( SI16 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i )
	{
		if ( pStgInfo->clHorse[ i ].siHorseUnique > 0 )
		{
		//	SendMessage( hHorseListBox, LB_ADDSTRING, 0, (LPARAM)pStgInfo->clHorse[ i ].szName );
			Data.Set(0, pStgInfo->clHorse[ i ].szName);
			m_pList->AddItem(&Data);

			if ( i == 0 )
			{
				switch ( pStgInfo->clHorse[ i ].siHorseUnique )
				{
				case 1:
				case 2:
				case 3:
					m_pStatic[2]->SetFontIndex(pStgInfo->clHorse[ i ].siHorseUnique + 1);
					break;
				default:
					m_pStatic[2]->SetFontIndex(2);
					break;
				}
			}
		}
		else if ( pStgInfo->bCreatedHouseUnitStable[ i ] == false )
		{
			TCHAR* pText2 = GetTxtFromMgr(1192);

		//	SendMessage( hHorseListBox, LB_ADDSTRING, 0, (LPARAM)pText2 );
			Data.Set(0, pText2);
			m_pList->AddItem(&Data);

			if ( i == 0 )
			{
				m_pStatic[2]->SetFontIndex(0);
			}

		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(2149);
		//	SendMessage( hHorseListBox, LB_ADDSTRING, 0, (LPARAM)pText );
			Data.Set(0, pText);
			m_pList->AddItem(&Data);

			if ( i == 0 )
			{
				m_pStatic[2]->SetFontIndex(1);
			
			
			}
		}
	}
	m_pList->Refresh();
	return;
}

