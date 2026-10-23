

#include "NHireSystemNPCDlg.h" 
#include "../../resource.h"

#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "MsgType-Structure.h"

//-------------------------------
// Common
//------------------------------
#include "Quest\Quest.h"
#include "Char\CharManager\CharManager.h"
#include "../../../CommonLogic/HiredNPCMgr/HiredNPCMgr.h"


#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"



#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../Lib/JYLibFunction.h"


//HWND					CNHireSystemNPCDlg::m_hWnd;
cltStructureMngBasic*	CNHireSystemNPCDlg::m_pclStructureManager;
//HBITMAP					CNHireSystemNPCDlg::m_hPortraitBitmap;
SI32					CNHireSystemNPCDlg::m_siCount;
cltHiredNPCPayInfo		CNHireSystemNPCDlg::m_clHiredNPCPayInfo[ MAX_HIREDNPC_KIND_NUM ];
BOOL					CNHireSystemNPCDlg::m_bUpdateNPCPaySwitch;

static int				hiretemp;

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

stListBoxItemData NHireNPCListBoxData;

CNHireSystemNPCDlg::CNHireSystemNPCDlg()
{
	//m_hWnd = NULL;
	//m_hPortraitBitmap = NULL;
	
        m_pNHireNPC_Edit1 = NULL;
        m_pNHireNPC_Edit2 = NULL;
        m_pNHireNPC_Static1 = NULL;
        m_pNHireNPC_Static2 = NULL;
        m_pNHireNPC_Combo = NULL;
        m_pNHireNPC_Button1 = NULL;
        m_pNHireNPC_Button2 = NULL;
        m_pNHireNPC_List = NULL;
		SetTimerOnOff( true );
}

CNHireSystemNPCDlg::~CNHireSystemNPCDlg()
{
	SetTimerOnOff( false );
	SAFE_DELETE(m_pNHireNPC_Edit1);
	SAFE_DELETE(m_pNHireNPC_Edit2);
	SAFE_DELETE(m_pNHireNPC_Static1);
	SAFE_DELETE(m_pNHireNPC_Static2);
	SAFE_DELETE(m_pNHireNPC_Combo);
	SAFE_DELETE(m_pNHireNPC_Button1);
	SAFE_DELETE(m_pNHireNPC_Button2 );
	SAFE_DELETE(m_pNHireNPC_List);


}

void CNHireSystemNPCDlg::Set(cltStructureMngBasic *pclStructureManager)
{

	m_pclStructureManager = pclStructureManager;
	
	SI32 siSystemNPCKind=35;
	TCHAR szexplain[512];
	if(pclClient->pclQuestManager->GetQuestExplain(siSystemNPCKind,szexplain, 512))						
	{							
		TCHAR buffer[ 256 ];

		SI32 pay = (SI32)(pclClient->pclKindInfoSet->GetHireBasicMoney( siSystemNPCKind )
			* ( pclClient->pclItemManager->clItemStatistic.siNPCPriceRate / 100. ));

		SI32 siHirePay = pay;
		SI32 siMonthlyPay = pay;

		SI32 siVillageLevel = pclClient->pclKindInfoSet->GetHireVillageLevel( siSystemNPCKind );
		
//#if defined(_CITYHALL_BANKRUPTCY)	
		{
			UI08 usCurStatus = 0;
			
			if( pclClient == NULL || pclClient->pclCityHallManager == NULL ) return;

			usCurStatus = pclClient->pclCityHallManager->clClientStrInfo.clCityhallStrInfo.usCurStatus;

			memset( buffer, 0, sizeof(buffer));

			switch( usCurStatus )
			{
			case CITYHALL_BEING_STABLE	:
				{
					TCHAR buffer2[512];
					memset( buffer2, 0, sizeof(buffer2));

					TCHAR* pText = GetTxtFromMgr(1646);								
					TCHAR* pText1 = GetTxtFromMgr(5168);
					TCHAR* pText2 = GetTxtFromMgr(5170);

					// �������̹Ƿ� ������� 0���� �׸��� ��� ���� ���� ������ 0 ��
					/*StringCchPrintf( buffer, 256, pText,
							szexplain, TEXT("0"), 
							0 pclClient->pclKindInfoSet->GetHireVillageLevel(siSystemNPCKind)*/

					//KHY - 0816 - �������� ���� (NPC������)
					StringCchPrintf( buffer, 256, pText,szexplain, TEXT("0"));				

					StringCchPrintf( buffer2, 512, TEXT("%s\r\n%s\r\n\r\n%s"), pText1, pText2, buffer );

					m_pNHireNPC_Edit1->SetText(buffer2);
				}
				break;
			default:
				{
//#endif
					SI32 pay = siMonthlyPay * 12;
					TCHAR buf[64];

					g_JYLibFunction.SetNumUnit( pay , buf, 64 );

					TCHAR* pText = GetTxtFromMgr(1646);
					//KHY - 0816 - �������� ���� (NPC������)
					StringCchPrintf( buffer, 256, pText,szexplain, buf);
					
//							pclClient->pclKindInfoSet->GetHireVillageLevel(siSystemNPCKind));
					m_pNHireNPC_Edit1->SetText(buffer);
//#if defined(_CITYHALL_BANKRUPTCY)
				}
			}
		}
//#endif
	}
	

}

void CNHireSystemNPCDlg::Create( /*cltStructureMngBasic *pclStructureManager*/ )
{

	SetActionTimer(200);
	
	


	//m_pclStructureManager = pclStructureManager;
	
	if(!IsCreate())	{

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_NHireNPC/DLG_NHireNPC.ddf"));
		file.CreatePopupDialog( this, NHIRENPC_DLG, TEXT("dialog_NHireNPC"), StatiCNHireSystemNPCDlgProc);


		m_pNHireNPC_Edit1 = new CEdit(this);
        //m_pNHireNPC_Edit2 = new CEdit(this);
        m_pNHireNPC_Static1 = new CStatic(this);
        m_pNHireNPC_Static2 = new CStatic(this);
        m_pNHireNPC_Combo =  new CComboBox(this);
        m_pNHireNPC_Button1 = new CButton(this);
        m_pNHireNPC_Button2 = new CButton(this);
        m_pNHireNPC_List = new CList(this);
	
	
		file.CreateControl(m_pNHireNPC_Edit1, 	NHIRENPC_EDIT1, TEXT("editbox_NHireNPC1"));
		//file.CreateControl(m_pNHireNPC_Edit2,   NHIRENPC_EDIT2,TEXT("editbox_NHireNPC2"));
		file.CreateControl(m_pNHireNPC_Static1, NHIRENPC_STATIC1, TEXT("statictext_NHireNPC1"));	
		file.CreateControl(m_pNHireNPC_Static2, NHIRENPC_STATIC2, TEXT("statictext_NHireNPC2"));
		file.CreateControl(m_pNHireNPC_Combo, NHIRENPC_COMBO,TEXT("combobox_NHireNPC"));	
		file.CreateControl(m_pNHireNPC_Button1, NHIRENPC_BUTTON1,TEXT("button_NHireNPC1"));
		file.CreateControl(m_pNHireNPC_Button2, NHIRENPC_BUTTON2, TEXT("button_NHireNPC2"));
		file.CreateControl(m_pNHireNPC_List, NHIRENPC_LIST, TEXT("listbox_NHireNPC"));
	}

	m_bUpdateNPCPaySwitch = FALSE;
	m_siCount = 0;
	ZeroMemory( m_clHiredNPCPayInfo, sizeof( m_clHiredNPCPayInfo ) );

	// NPC �޿� ���� ��û 
	{
		cltMsg clMsg(GAMEMSG_REQUEST_NPCPAYMENT, 0 );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);				
	}

	TCHAR buffer[256 ];

	// ����Ʈ ��Ʈ�� �ʱ�ȭ 
	

	m_pNHireNPC_List->Clear();
	int i;

	LVCOLUMN lvcol;
	LVITEM lvi;

	int width[] = { 42, 100, 130, 105 };
//	TCHAR *text[] = { TEXT("��ȣ"), TEXT("�̸�"), TEXT("����"), TEXT("�����") };

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	TCHAR* pText[4];
	
	pText[0] = GetTxtFromMgr(1641);
	m_pNHireNPC_List->SetColumn(0,43,pText[0]);
	
	pText[1] = GetTxtFromMgr(1556);
	m_pNHireNPC_List->SetColumn(1,100,pText[1]);
	
	pText[2] = GetTxtFromMgr(1642);
	m_pNHireNPC_List->SetColumn(2,100,pText[2]);
	
	pText[3] = GetTxtFromMgr(1643);	
	m_pNHireNPC_List->SetColumn(3,100,pText[3]);
	m_pNHireNPC_List->Refresh();
	
	m_pNHireNPC_List->SetBorder(true);
	m_pNHireNPC_List->SetBKColorUse(true);


	for( i = 0; i < 4; ++i ) 
	{

		lvcol.cx = width[ i ];
		lvcol.pszText = pText[i]; //text[ i ];
		lvcol.iSubItem = i;

		//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
	}

	lvi.mask = LVIF_TEXT;

	for( i = 0; i < MAX_HIRED_NPC_NUM; ++i ) 
	{

		lvi.iItem = i;
		lvi.iSubItem = 0;

		_itot( i + 1, buffer, 10 );
		lvi.pszText = (TCHAR *)buffer;

		//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_INSERTITEM, 0, (LPARAM)&lvi );

	}


	cltKindInfo* pclki = NULL;

	stComboBoxItemData tmpComboBoxItemData;	
	for ( i = 10; i < MAX_KIND_NUMBER; ++i )
	{
		pclki = pclClient->pclKindInfoSet->pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_HIREABLE ) == false ) continue;
		
		TCHAR* pname = (TCHAR*)pclki->GetName();
		m_pNHireNPC_Combo->GetItemData(NHIRENPC_COMBO );									
		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( pname );
		m_pNHireNPC_Combo->AddItem( &tmpComboBoxItemData );		
	}
	SI32 siSystemNPCKind=34;
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siSystemNPCKind, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pNHireNPC_Static1->SetFileName(szPortrait);
	m_pNHireNPC_Static1->SetFontIndex(siPortraitFont);  //ó��ȭ�� ����̷� ����
	

	



	//SendDlgItemMessage( hDlg, IDC_COMBO_SYSTEMNPC_SELECT, CB_SETCURSEL, 0, 0 );

	//SendMessage( hDlg, WM_COMMAND, MAKEWPARAM( IDC_COMBO_SYSTEMNPC_SELECT, CBN_SELCHANGE ), 0 );

	//SetTimer( hDlg, 1, 600, NULL );


}
void CNHireSystemNPCDlg::Action()
{
	
	/*
    //m_siNPCCharUnique = charunique;
	*/
	cltClient *pclclient = ( cltClient * )pclClient;
	SI32 charunique=m_pclStructureManager->CharUnique;
	SI32 CharID = pclClient->pclCM->GetIDFromCharUnique( charunique );
	
	
	//SI32 m_siNPCKind = pclclient->pclCM->CR[ CharID ]->GetKind();			
	cltHiredNPC *pclHiredNPC = m_pclStructureManager->clClientStrInfo.clCityhallStrInfo.clHiredNPC;

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	TCHAR buf[ 256 ];
	TCHAR buffer[256];
    
	NHireNPCListBoxData.Init();
	GMONEY gmPayment = 0;

	m_pNHireNPC_List->Clear();
	for( int i = 0; i < MAX_HIRED_NPC_NUM; ++i )
	{

		lvi.iItem = i;
		StringCchPrintf(buffer, 256, TEXT("%d"),lvi.iItem+1);
		NHireNPCListBoxData.Set(0,buffer);

		// NPC��
		lvi.iSubItem = 1;

		if( pclHiredNPC[ i ].siNPCKind == 0 ) 
		{
			TCHAR* pText = GetTxtFromMgr(1561);
			lvi.pszText = pText;
			NHireNPCListBoxData.Set(1,pText);

			//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_SETITEM, 0, (LPARAM)&lvi );

			lvi.iSubItem = 2;
			lvi.pszText = TEXT("");
			NHireNPCListBoxData.Set(2,lvi.pszText);
			//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_SETITEM, 0, (LPARAM)&lvi );
			
			lvi.iSubItem = 3;
			lvi.pszText = TEXT("");
			NHireNPCListBoxData.Set(3,lvi.pszText);
			//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_SETITEM, 0, (LPARAM)&lvi );
			m_pNHireNPC_List->AddItem(&NHireNPCListBoxData);
			


		} 
		

		else
		{

			lvi.pszText = (TCHAR *)pclClient->pclKindInfoSet->GetName( pclHiredNPC[ i ].siNPCKind );

			//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_SETITEM, 0, (LPARAM)&lvi );

			NHireNPCListBoxData.Set(1,lvi.pszText);

			// ����
			gmPayment = pclHiredNPC[ i ].siMonthlyPay * 12;

			g_JYLibFunction.SetNumUnit( gmPayment, buf, 256 );
			lvi.iSubItem = 2;
			lvi.pszText = buf;
			NHireNPCListBoxData.Set(2,lvi.pszText);

			//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_SETITEM, 0, (LPARAM)&lvi );

			// �����
			TCHAR* pText = GetTxtFromMgr(1644);
			StringCchPrintf( buf, 256, pText, 
				pclHiredNPC[ i ].clHireDate.uiYear,
				pclHiredNPC[ i ].clHireDate.uiMonth,
				pclHiredNPC[ i ].clHireDate.uiDay );
			lvi.iSubItem = 3;
			lvi.pszText = buf;
			NHireNPCListBoxData.Set(3,lvi.pszText);

			m_pNHireNPC_List->AddItem(&NHireNPCListBoxData);

			//SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_SETITEM, 0, (LPARAM)&lvi );

		}

	}//end for 
	m_pNHireNPC_List->Refresh();
	
	
}

void CALLBACK CNHireSystemNPCDlg::StatiCNHireSystemNPCDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNHireSystemNPCDlg *pThis = (CNHireSystemNPCDlg*) pControllerMgr;
	pThis->NHireSystemNPCDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNHireSystemNPCDlg::NHireSystemNPCDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	
	TCHAR strSystemNPCName[ 32 ];

	cltHiredNPC *pclHiredNPC = m_pclStructureManager->clClientStrInfo.clCityhallStrInfo.clHiredNPC;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:	
			{
				DeleteDialog();
				//Hide();
			}
			break;
	
		case NHIRENPC_COMBO:
			{
				switch(nEvent)	
				{				
				case EVENT_COMBOBOX_SELECTION_CHANGED:
					{							
						
						int index=m_pNHireNPC_Combo->GetSelectedIndex();
						if ( index < 0 )
							break;			
						
						StringCchCopy(strSystemNPCName, 32, m_pNHireNPC_Combo->GetText(index));
						SI32 siSystemNPCKind = pclClient->pclKindInfoSet->FindKindFromName( strSystemNPCName );
						TCHAR szPortrait[256];
						SI32 siPortraitFont;
						pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siSystemNPCKind, szPortrait, sizeof(szPortrait), &siPortraitFont);
						m_pNHireNPC_Static1->SetFileName(szPortrait);
						m_pNHireNPC_Static1->SetFontIndex(siPortraitFont);
					

						TCHAR strFileName[ 128 ] = TEXT("");
						TCHAR szexplain[512];
						if(pclClient->pclQuestManager->GetQuestExplain(siSystemNPCKind,szexplain, 512))
						{							
							TCHAR buffer[ 1024 ];

							GMONEY	gmPay = pclClient->pclKindInfoSet->GetHireBasicMoney( siSystemNPCKind );
							gmPay = gmPay * ( pclClient->pclItemManager->clItemStatistic.siNPCPriceRate / 100. );

							GMONEY gmHirePay	= gmPay;
							GMONEY gmMonthlyPay	= gmPay;

							SI32 siVillageLevel = pclClient->pclKindInfoSet->GetHireVillageLevel( siSystemNPCKind );
//#if defined(_CITYHALL_BANKRUPTCY)
							{
								UI08 usCurStatus = 0;
								
								if( pclClient == NULL || pclClient->pclCityHallManager == NULL ) return;

								usCurStatus = pclClient->pclCityHallManager->clClientStrInfo.clCityhallStrInfo.usCurStatus;

								memset( buffer, 0, sizeof(buffer));

								switch( usCurStatus )
								{
								case CITYHALL_BEING_STABLE	:
									{
										TCHAR buffer2[512];
										memset( buffer2, 0, sizeof(buffer2));

										TCHAR* pText = GetTxtFromMgr(1646);								
										TCHAR* pText1 = GetTxtFromMgr(5168);
										TCHAR* pText2 = GetTxtFromMgr(5170);

										// �������̹Ƿ� ������� 0����
										/*StringCchPrintf( buffer, 1024, pText,
												szexplain, TEXT("0"), 
												0 pclClient->pclKindInfoSet->GetHireVillageLevel(siSystemNPCKind)*/

										//KHY - 0816 - �������� ���� (NPC������)
										StringCchPrintf( buffer, sizeof(buffer), pText,szexplain, TEXT("0"));				

										StringCchPrintf( buffer2, sizeof(buffer2), TEXT("%s\r\n%s\r\n\r\n%s"), pText1, pText2, buffer );

										m_pNHireNPC_Edit1->SetText(buffer2);
									}
									break;
								default:
									{
//#endif							
										GMONEY gmPayment = gmMonthlyPay * 12;

										TCHAR buf[64];

										g_JYLibFunction.SetNumUnit( gmPayment , buf, 64 );

										TCHAR* pText = GetTxtFromMgr(1646);								
										//KHY - 0816 - �������� ���� (NPC������)
										//[���� : Ȳ���� 2008. 1. 18 => ���۸� ����ϴ� ũ�⸦ �۰��ؼ� NPC ������ ©���°�]
										StringCchPrintf( buffer, sizeof(buffer), pText,szexplain, buf);
//												pclClient->pclKindInfoSet->GetHireVillageLevel(siSystemNPCKind));

										m_pNHireNPC_Edit1->SetText(buffer);
//#if defined(_CITYHALL_BANKRUPTCY)
									}
								}
							}
//#endif
						
							//TCHAR* pText = GetTxtFromMgr(1646);								
							//sprintf( buffer, pText,
							//		szexplain, siMonthlyPay * 12, 
							//		pclClient->pclKindInfoSet->GetHireVillageLevel(siSystemNPCKind));
							//m_pNHireNPC_Edit1->SetText(buffer);
							SetTimerOnOff( true );

						}
						

					}//end small case 
					break;

				}
				

			}//end large cases
			break;

		case NHIRENPC_BUTTON1:
			{
				switch(nEvent)	
				{				
				case EVENT_BUTTON_CLICKED:
					{	
						SI32 id = 1;

						int index1, index2;
						index1=m_pNHireNPC_List->GetSelectedIndex();
						index2=m_pNHireNPC_Combo->GetSelectedIndex();


						// ���� ��� ���� ��ȣ�� ���´� 

						//index1 = SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_ALL | LVNI_SELECTED );

						//index2 = (SI16)SendDlgItemMessage( hDlg, IDC_COMBO_SYSTEMNPC_SELECT, CB_GETCURSEL, 0, 0 );
					

						if( index2 < 0 ) break;
							
						// npc ����Ʈ���� ���õ� �̸��� ���´� 

						//SendDlgItemMessage( hDlg, IDC_COMBO_SYSTEMNPC_SELECT, CB_GETLBTEXT, index2, (LPARAM)strSystemNPCName );
						StringCchCopy(strSystemNPCName, 32, m_pNHireNPC_Combo->GetText(index2));

						int kind = pclClient->pclKindInfoSet->FindKindFromName( strSystemNPCName );


						for( int i = 0; i < MAX_HIRED_NPC_NUM; ++i ) {
							
							if( pclHiredNPC[ i ].siNPCKind == kind && kind > 0 ) break;

						}

						// �ߺ� ��� ���� 
						if( i < MAX_HIRED_NPC_NUM ) {
							TCHAR* pText = GetTxtFromMgr(1645);
							m_pNHireNPC_Edit1->SetText(pText);							

							break;
						}

						// NPC ����� ������ ��û�Ѵ� 

						cltGameMsgRequest_HireNPC clHireNPC( m_pclStructureManager->siVillageUnique, index1, kind );

						cltMsg clMsg( GAMEMSG_REQUEST_HIRENPC, sizeof(cltGameMsgRequest_HireNPC), (BYTE*)&clHireNPC );
						
						pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
						SetTimerOnOff( true );
						
					

					}
					break;
				}
				
			}
			break;
		case NHIRENPC_BUTTON2:
			{
				switch(nEvent)	
				{				
				case EVENT_BUTTON_CLICKED:
					{
						
						//SI32 id = cltCharCommon::CharUniqueInfo[ m_pclStructureManager->CharUnique ];
						//id =m_pNHireNPC_List->GetSelectedIndex();
						SI32 id=1;
				
						if(id == 0) break;

						int index1, index2=0;

						// ���� ��� ���� ��ȣ�� ���´� 

						//index1 = SendDlgItemMessage( hDlg, IDC_LIST_HIREDNPC, LVM_GETNEXTITEM, (WPARAM)-1, LVNI_ALL | LVNI_SELECTED );
						index1=m_pNHireNPC_List->GetSelectedIndex();


						// NPC �ذ�� ������ ��û�Ѵ� 

						cltGameMsgRequest_HireNPC clHireNPC( m_pclStructureManager->siVillageUnique, index1, 0 );

						cltMsg clMsg( GAMEMSG_REQUEST_HIRENPC, sizeof(cltGameMsgRequest_HireNPC), (BYTE*)&clHireNPC );
						
						pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
						SetTimerOnOff( true );
						
						
					}
					break;
				}
				
			}
			break;

		case NHIRENPC_LIST:
			{
				SetTimerOnOff( false );
				switch(nEvent)	
				{		
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
                        
					}
					break;
				}

			}
			break;
	}	



	return;
}

