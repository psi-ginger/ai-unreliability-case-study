#include "../Client/NInterface/NSelectWorldListDlg/SelectWorldListDlg.h"

#include "../../../../Server/FrontServer/FrontMsg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ImageButton.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../NStatusBarDlg/NStatusBarDlg.h"


extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

#define TESTSERVER_ID		11

CNSelectWorldListDlg::CNSelectWorldListDlg() : CDialogController()
{
	m_siSelectListIndex		= 0;
	m_siHeartBeatCount		= 0;

	pWorldInfo				= NULL;

	m_pStatic				= NULL;

	m_pEdit[0]				= NULL;
	m_pEdit[1]				= NULL;

	m_pButton				= NULL;

	for (int i = 0; i < MAX_WORLD_LIST; i++)
	{
		m_pImageButton[i]	= NULL;
	}

	m_pImage_PCRoomLeft		= NULL;
	m_pImage_PCRoomRight	= NULL;
	
	m_bPromterInfoShow		= true;
}

CNSelectWorldListDlg::~CNSelectWorldListDlg()
{
	NDelete(m_pStatic);
	NDelete(m_pEdit[0]);
	NDelete(m_pEdit[1]);
	NDelete(m_pButton);

	NDelete_Array(pWorldInfo);

	for (int i = 0; i < MAX_WORLD_LIST; i++)
	{
		NDelete(m_pImageButton[i]);
	}

	NDelete( m_pImage_PCRoomLeft );
	NDelete( m_pImage_PCRoomRight );
}

void CNSelectWorldListDlg::Create()
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/DLG_NSelectWorldList.ddf"));
	file.CreatePopupDialog( this, NSELECTWORLDLIST_DLG, TEXT("dialog_SELECTWORLDLIST"), StaticCallBackDialogNSelectWorldListDlg,false );

	if( pclClient->siServiceArea == ConstServiceArea_Japan		||
		pclClient->siServiceArea == ConstServiceArea_China		||
		pclClient->siServiceArea == ConstServiceArea_Korea		|| 
		pclClient->siServiceArea == ConstServiceArea_Taiwan		||
		pclClient->siServiceArea == ConstServiceArea_USA		|| 
		pclClient->siServiceArea == ConstServiceArea_English	||
		pclClient->siServiceArea == ConstServiceArea_NHNChina	||
		pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		if (!g_SoundOnOffDlg.m_bMaxWindowMode)		MovePos( 50 + 48, -8 );
		else										MovePos( 50 + 80, 0 + 40 );
	}
	else
	{
		if (!g_SoundOnOffDlg.m_bMaxWindowMode)		MovePos( 200, 145 );
		else										MovePos( 245, 185 );
	}

	m_pStatic	= new CStatic( this );
	m_pEdit[0]	= new CEdit( this );
	m_pEdit[1]	= new CEdit( this );
	m_pButton	= new CButton( this );

	if(	   pclClient->siServiceArea != ConstServiceArea_Japan 
		&& pclClient->siServiceArea != ConstServiceArea_China 
		&& pclClient->siServiceArea != ConstServiceArea_Korea 
		&& pclClient->siServiceArea != ConstServiceArea_Taiwan 
		&& pclClient->siServiceArea != ConstServiceArea_USA
		&& pclClient->siServiceArea != ConstServiceArea_English
		&& pclClient->siServiceArea != ConstServiceArea_NHNChina
		&& pclClient->siServiceArea != ConstServiceArea_EUROPE
	)
	{
		TCHAR* pText = GetTxtFromMgr(5729);
		m_pButton->SetFontColor( RGB( 255, 255, 255 ) );
		m_pButton->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		for (int i = 0; i < MAX_WORLD_LIST; i++)
		{
			SI16 siXpos = 53 + ( i / 12 * 212 );
			SI16 siYpos	= 68 + ( i % 12 * 33 );
			m_pImageButton[i] = new CImageButton( this );
			m_pImageButton[i]->Create(	NSELECTWORLDLIST_DIALOG_SERVERLIST_BUTTON, 
										NULL, 
										TEXT("NInterface/BUTTON/BUTTON_212X33_00_000.SPR"), 
										0, 
										false, 
										0, 
										siXpos, 
										siYpos, 
										212, 
										33, 
										212, 
										33	);
			m_pImageButton[i]->SetInImageFileName(TEXT("ninterface/button/BUTTON_63X12_00_000.SPR"));
			m_pImageButton[i]->SetInImageFontIndex( 0 );
			m_pImageButton[i]->SetInImagePos( 135, 11 );
			m_pImageButton[i]->SetInTextPos( 20, 0 );

			m_pImageButton[i]->SetValue( i );
			m_pImageButton[i]->Show( FALSE );
		}
	}
	else
	{
		for (int i = 0; i < MAX_WORLD_LIST; i++)
		{
			SI16 siXpos = 126 + ( i / 12 * 220 );
			SI16 siYpos	= 138 + ( i % 12 * 30 );
			m_pImageButton[i] = new CImageButton( this );
			m_pImageButton[i]->Create(	NSELECTWORLDLIST_DIALOG_SERVERLIST_BUTTON, NULL, 
										TEXT("NInterface/BUTTON/BUTTON_212X23_00_000.SPR"), 
										0, 
										false, 
										0, 
										siXpos, 
										siYpos, 
										212, 
										23, 
										212, 
										23	);
			m_pImageButton[i]->SetInImageFileName(TEXT("ninterface/image/IMAGE_82X21_00_000.SPR"));
			m_pImageButton[i]->SetInImageFontIndex( 0 );
			m_pImageButton[i]->SetInImagePos( 0, 1 );
			m_pImageButton[i]->SetInTextPos( 87, 1 );

			m_pImageButton[i]->SetValue( i );
			m_pImageButton[i]->Show( FALSE );
		}

#ifdef USE_PROMOTOR_CLIENT
		
			// ���θ��� ����.
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		 NSELECTWORLDLIST_DIALOG_ENTER_SERVER_BUTTON,	this ), NSELECTWORLDLIST_DIALOG_ENTER_SERVER_BUTTON,	"button_enter_server"	);
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		 NSELECTWORLDLIST_DIALOG_CLOSE_BUTTON,			this ), NSELECTWORLDLIST_DIALOG_CLOSE_BUTTON,			"button_close"			); 
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		 NSELECTWORLDLIST_DIALOG_EXPLAIN_STATIC,		this ), NSELECTWORLDLIST_DIALOG_EXPLAIN_STATIC,			"statictext_explain"	);
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NSELECTWORLDLIST_DIALOG_BACK_STATIC,			this ), NSELECTWORLDLIST_DIALOG_BACK_STATIC,			"imagestatic_back"		);
			PromoterHide();
#endif

	}

	if ( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
	{
		NEWCIMAGESTATIC( m_pImage_PCRoomLeft );
		m_pImage_PCRoomLeft->Create( NSELECTWORLDLIST_DIALOG_PCROOM_IMAGESTATIC, TEXT("gimg/Special_No_Pcroom_Large_Mark.SPR"), 0, NULL, 0, 115, 544, 228, 172, 228, 172 );

		NEWCIMAGESTATIC( m_pImage_PCRoomRight );
		m_pImage_PCRoomRight->Create( NSELECTWORLDLIST_DIALOG_PCROOM_IMAGESTATIC, TEXT("gimg/Special_Pcroom_Large_Mark.SPR"), 0, NULL, 0, 581, 544, 228, 172, 228, 172 );
	}


	file.CreateControl( m_pStatic,	NSELECTWORLDLIST_DIALOG_SERVERIMAGE_STATIC, TEXT("statictext_SEVERIMAGE"));  
	file.CreateControl( m_pEdit[0], NSELECTWORLDLIST_DIALOG_WORLDEXPLAIN_EDIT,	TEXT("editbox_WORLD_EXPLAIN"));  
	file.CreateControl( m_pEdit[1], NSELECTWORLDLIST_DIALOG_SERVERINFO_EDIT,	TEXT("editbox_SEVERINFO"));  
	file.CreateControl( m_pButton,	NSELECTWORLDLIST_DIALOG_ENTER_BUTTON,		TEXT("button_enter"));  

	Hide();
}


void CALLBACK CNSelectWorldListDlg::StaticCallBackDialogNSelectWorldListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSelectWorldListDlg *pThis = (CNSelectWorldListDlg *)pControllerMgr;
	pThis->CallBackDialogNSelectWorldListDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNSelectWorldListDlg::CallBackDialogNSelectWorldListDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
		case NSELECTWORLDLIST_DIALOG_SERVERLIST_BUTTON :
			{
				CImageButton* pButton = (CImageButton*)pControl;
				m_siSelectListIndex = pButton->GetValue();

				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED : 
					{
						if(pclClient->IsCountrySwitch(Switch_Sound))
						{
							pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_CLICK_BUTTON")), 0, 0);
						}

						TCHAR BufferExplain[ 1024 ] = TEXT("");

						if (pWorldInfo[ m_siSelectListIndex ].refreshinfo.worldId > 0)
						{
							m_pStatic->SetFontIndex(pWorldInfo[ m_siSelectListIndex ].refreshinfo.worldId - 1);
							m_pStatic->Refresh();
						}

						if (pWorldInfo[ m_siSelectListIndex ].gameserverinfo.goonzuname[0] != NULL)
						{
							StringCchCopy( BufferExplain, 1024, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.goonzutext );
							m_pEdit[0]->SetText(BufferExplain);

							ShowServerInfo( pWorldInfo[ m_siSelectListIndex ].gameserverinfo.goonzuname, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.currentPrice, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.npcrate, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.population, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.fixationmoney );
						}
						else
						{
							m_pEdit[0]->Clear();
							m_pEdit[0]->Refresh();

							m_pEdit[1]->Clear();
							m_pEdit[1]->Refresh();
						}

						// ���ǥ�� �׸��� - �ѱ��� �׸���.
						if( pclClient->IsWhereServiceArea(ConstServiceArea_Korea) )
						{
							// [��ȣ_NHN->NDOORS] �������� ������ �����ϴ� ��ƾ�� ���ش�.
							//if( strstr(pWorldInfo[ m_siSelectListIndex ].baseinfo.worldname, "(18��)") != NULL )
							//{
							//	pclClient->siServiceAge = 18;
							//}
							//else
							//{
							//	pclClient->siServiceAge = 1;
							//}

							pclclient->DrawGovernMentRankMark( 0 );
						}
					}
					break;
				case EVENT_BUTTON_DBCLICKED:
					{
						if( m_siSelectListIndex >= 0)
						{
							if ( pWorldInfo [ m_siSelectListIndex ].refreshinfo.state == 0 || pclclient->bLoginWhileServerDownSwitch )
							{
								pclclient->clSelectedWorldInfo = pWorldInfo [ m_siSelectListIndex ];
								pclclient->SetTitleText();
								Hide();
							}
						}
					}
					break;
				}
			}
			break;
		case NSELECTWORLDLIST_DIALOG_ENTER_BUTTON :
			{
				switch (nEvent)
				{
				case EVENT_BUTTON_CLICKED :
					{
						if(pclClient->IsCountrySwitch(Switch_Sound))
						{
							pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GAME_START")), 0, 0);
						}

						if( m_siSelectListIndex >= 0 && m_siSelectListIndex < MAX_WORLD_LIST )
						{
							if ( pWorldInfo [ m_siSelectListIndex ].refreshinfo.state == 0 || pclclient->bLoginWhileServerDownSwitch )
							{
								pclclient->clSelectedWorldInfo = pWorldInfo [ m_siSelectListIndex ];
								pclclient->SetTitleText();
								Hide();
							}
						}
					}
					break;
				}
			}
			break;

		case NSELECTWORLDLIST_DIALOG_PCROOM_IMAGESTATIC:
			{
				if ( EVENT_IMAGESTATIC_LBUTTONUP == nEvent )
				{
					if ( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
					{
						ShellExecute( NULL, NULL, TEXT("http://gbar.nhncorp.cn/"), NULL, NULL, SW_SHOWNORMAL );
					}
				}
			}
			break;
		case NSELECTWORLDLIST_DIALOG_ENTER_SERVER_BUTTON:
			{
				WORLDINFO WorldTemp;
				for( SI32 i = 0; i < pclclient->clWorldList.GetCount(); ++i )
				{
					pclclient->clWorldList.GetWorldInfo( i + 1, &WorldTemp );
					if( 0 == WorldTemp.refreshinfo.state && m_PromoterServerID == WorldTemp.baseinfo.siServerID )
					{
						pclclient->clSelectedWorldInfo = WorldTemp;

						TCHAR buffer[256];
						StringCchPrintf(buffer, 256, TEXT("[%s_v.%d]"), pclclient->clSelectedWorldInfo.baseinfo.worldname, pclClient->GetVersion());
						if( pclclient->m_pDialog[ STATUSBAR_DLG] )
						{
							((CNStatusBarDlg*)pclclient->m_pDialog[STATUSBAR_DLG])->SetVersionAndServerName(buffer);
						}		

						// �� â�� �ݱ�� ������ ������ �϶�� ���η�ƾ�� Ÿ�� �ȴ�. ���º������� ���־..
						if( pclclient->m_pDialog[ NSELECTWORLDLIST_DLG] ) 
							((CNSelectWorldListDlg*)pclclient->m_pDialog[NSELECTWORLDLIST_DLG])->Hide( );

						break;
					}
				}
			}
			break;
			
		case NSELECTWORLDLIST_DIALOG_CLOSE_BUTTON:
			{
				PromoterHide();
			}
			break;
	}
}

void CNSelectWorldListDlg::Hide()
{
	if (pWorldInfo != NULL )
	{
		delete [] pWorldInfo;
		pWorldInfo = NULL;
	}

	SetTimerOnOff(false);

	CControllerMgr::Show( false ); 

	return;
}

void CNSelectWorldListDlg::Show()
{
	SetTimerOnOff(true);

	CControllerMgr::Show( true );

	cltClient *pclclient = (cltClient *)pclClient;

	if (pWorldInfo == NULL)
		pWorldInfo = new WORLDINFO[ pclclient->clWorldList.GetCount() ];

	int i;
	int j;
	int k;
	WORLDINFO WorldTemp;

	for ( i = 0; i < pclclient->clWorldList.GetCount(); ++i )
	{
		if ( pclclient->clWorldList.m_worldlist[i].baseinfo.worldId < TESTSERVER_ID )
			pclclient->clWorldList.GetWorldInfo( i + 1, &pWorldInfo[i] );
		else if ( pclclient->clWorldList.m_worldlist[i].baseinfo.worldId == TESTSERVER_ID ) // �߱��׽�Ʈ ����
			pclclient->clWorldList.GetWorldInfo( i + 1 , &pWorldInfo[pclclient->clWorldList.GetCount() - 1 ]);
		else
			pclclient->clWorldList.GetWorldInfo( i + 1 , &pWorldInfo[i-1]);
	}

	// �߱��� �븸�� ��������Ʈ�� ���������ʰ� �׳� ������. (PCK - 2007.09.11)
	if( pclClient->IsCountrySwitch(Switch_ServerListArray) )
	{
		// ���� ���� ���� ����
		for ( j = 0; j < pclclient->clWorldList.GetCount() - 1; j++ )
		{
			for ( k = j + 1; k < pclclient->clWorldList.GetCount(); k++ )
			{
				if (   pWorldInfo[j].refreshinfo.currentConnections > pWorldInfo[k].refreshinfo.currentConnections
					&& pWorldInfo[k].refreshinfo.currentConnections > 0
					&& pWorldInfo[k].baseinfo.worldId != TESTSERVER_ID )
				{
					WorldTemp		= pWorldInfo[ k ];
					pWorldInfo[k]	= pWorldInfo[ j ];
					pWorldInfo[ j ] = WorldTemp;
				}
				else if ( pWorldInfo[j].refreshinfo.currentConnections == 0
					&& pWorldInfo[k].refreshinfo.currentConnections > 0
					&& pWorldInfo[k].baseinfo.worldId != TESTSERVER_ID )
				{
					WorldTemp		= pWorldInfo[ k ];
					pWorldInfo[k]	= pWorldInfo[ j ];
					pWorldInfo[ j ] = WorldTemp;
				}
			}
		}
		SI32 baseworldid = 0 ;
		if( pclclient->szLastConnectServerName[0] != NULL )
		{
			for ( j = 0; j < pclclient->clWorldList.GetCount() - 1; j++ )
			{
				// �⺻ �����ΰ�?
				if ( !_tcscmp(pWorldInfo[j].baseinfo.worldname,pclclient->szLastConnectServerName))
				{
					baseworldid = j + 1 ;
					break;
				}
			}

			// �⺻ ������ �ִٸ�
			if ( baseworldid > 0 )
			{
				WorldTemp = pWorldInfo[baseworldid-1] ;
				for ( k = baseworldid-1 ; k > 0 ; k -- )
				{
					pWorldInfo[k] = pWorldInfo[k-1];
				}

				pWorldInfo[0] = WorldTemp ;
			}
		}
	}

	TCHAR BufferExplain[ 1024 ] = TEXT("");
	if (pWorldInfo[ m_siSelectListIndex ].gameserverinfo.goonzuname[0] != NULL)
	{
		StringCchCopy( BufferExplain, 1024, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.goonzutext );
		m_pEdit[0]->SetText(BufferExplain);

		ShowServerInfo( pWorldInfo[ m_siSelectListIndex ].gameserverinfo.goonzuname,  pWorldInfo[ m_siSelectListIndex ].gameserverinfo.currentPrice, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.npcrate, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.population, pWorldInfo[ m_siSelectListIndex ].gameserverinfo.fixationmoney );
	}
	Action();

	return;
}

void CNSelectWorldListDlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if (m_siHeartBeatCount >= 30)
	{
		m_siHeartBeatCount = 0;

		sResponse_HeartBeat sendmsg;

		sendmsg.packetHeader.usSize	= sizeof(sendmsg);
		sendmsg.packetHeader.usCmd	= FRONTMSG_HEARTBEAT;

		// ����Ʈ ������ �޽����� ������. 
		pclclient->SendFrontServerMsg((sPacketHeader*)&sendmsg);
	}

	m_siHeartBeatCount++;

	SI32 i;
	WORLDINFO WorldInfoTemp;

	for ( i = 0; i < pclclient->clWorldList.GetCount(); ++i )
	{
		pclclient->clWorldList.GetWorldInfo( i + 1, &WorldInfoTemp );
		for ( int l = 0; l < pclclient->clWorldList.GetCount(); ++l )
		{				
			if ( WorldInfoTemp.baseinfo.siServerID == pWorldInfo[l].baseinfo.siServerID )
			{
				pWorldInfo[l] = WorldInfoTemp;
				break;
			}
		}
	}			

	for ( i = 0; i < pclclient->clWorldList.GetCount(); ++i )
	{			
		switch( pWorldInfo[ i ].refreshinfo.state )
		{
		case 0:
			{
				if( ConstServiceArea_China == pclClient->siServiceArea )
				{
					if ( pWorldInfo[i].refreshinfo.currentConnections >= 1200 )
					{
						TCHAR* pText = GetTxtFromMgr(1134);

						m_pImageButton[i]->SetInImageFontIndex( 2 );
					}
					else if ( pWorldInfo[i].refreshinfo.currentConnections >= 800 )
					{
						TCHAR* pText = GetTxtFromMgr(1135);

						m_pImageButton[i]->SetInImageFontIndex( 1 );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(1136);

						m_pImageButton[i]->SetInImageFontIndex( 0 );
					}
				}
				else
				{
					if ( pWorldInfo[i].refreshinfo.currentConnections >= 2500 )
					{
						TCHAR* pText = GetTxtFromMgr(1134);

						m_pImageButton[i]->SetInImageFontIndex( 2 );
					}
					else if ( pWorldInfo[i].refreshinfo.currentConnections >= 1500 )
					{
						TCHAR* pText = GetTxtFromMgr(1135);

						m_pImageButton[i]->SetInImageFontIndex( 1 );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(1136);

						m_pImageButton[i]->SetInImageFontIndex( 0 );
					}
				}
			}
			break;
		case 1:
			{
				TCHAR* pText = GetTxtFromMgr(1137);

				m_pImageButton[i]->SetInImageFontIndex( 0 );
			}
			break;
		case 2:
			{
				TCHAR* pText = GetTxtFromMgr(1138);

				m_pImageButton[i]->SetInImageFontIndex( 3 );
			}
			break;
		case 3:
			{
				TCHAR* pText = GetTxtFromMgr(1139);

				m_pImageButton[i]->SetInImageFontIndex( 0 );
			}
			break;
		}

		TCHAR pTitle[ 256 ] = TEXT("");
		StringCchPrintf( pTitle, 256, TEXT("[ %s ]"), pWorldInfo[ i ].baseinfo.worldname);

		m_pImageButton[i]->SetText(pTitle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		m_pImageButton[i]->Show( TRUE );
	}
}

void CNSelectWorldListDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	/*
	if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		TSpr * pPCroomSpr	= pclClient->GetGlobalSpr(GIMG_SPECIAL_PCROOM_LARGE_MARK);
		TSpr * pNoPCroomSpr = pclClient->GetGlobalSpr(GIMG_SPECIAL_NO_PCROOM_LARGE_MARK);

		GP.PutSpr( pPCroomSpr,	 ScreenRX + GetX() + 115, ScreenRY + GetY() + 544 );
		GP.PutSpr( pNoPCroomSpr, ScreenRX + GetX() + 581, ScreenRY + GetY() + 544 );
	}
	*/
}

void CNSelectWorldListDlg::ShowServerInfo(TCHAR* GoonzuName, GMONEY CurrentPrice, SI32 NPCRate, SI32 PeopleNum, GMONEY BonusMoney )
{
	TCHAR Buffer[ 1024 ] = TEXT("");

	if ( CurrentPrice > 5000000 )
	{
		TCHAR* pText = GetTxtFromMgr(1126);


		StringCchPrintf( Buffer, 1024, pText, GoonzuName, PeopleNum, BonusMoney );
	}
	else if ( CurrentPrice > 2000000 )
	{
		TCHAR* pText = GetTxtFromMgr(1127);


		StringCchPrintf( Buffer, 1024, pText, GoonzuName, PeopleNum, BonusMoney );
	}
	else if ( CurrentPrice > 1000000 )
	{
		TCHAR* pText = GetTxtFromMgr(1128);

		StringCchPrintf( Buffer, 1024, pText, GoonzuName, PeopleNum, BonusMoney );
	}
	else if ( CurrentPrice > 500000 )
	{
		TCHAR* pText = GetTxtFromMgr(1129);

		StringCchPrintf( Buffer, 1024, pText, GoonzuName, PeopleNum, BonusMoney );
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1130);

		StringCchPrintf( Buffer, 1024, pText, GoonzuName, PeopleNum, BonusMoney );
	}

	m_pEdit[1]->SetText(Buffer);
}
void CNSelectWorldListDlg::PromoterShow( void )
{
	m_InterfaceFactory.GetStatic( NSELECTWORLDLIST_DIALOG_EXPLAIN_STATIC		)->Show( true );
	m_InterfaceFactory.GetButton( NSELECTWORLDLIST_DIALOG_ENTER_SERVER_BUTTON	)->Show( true );
	m_InterfaceFactory.GetButton( NSELECTWORLDLIST_DIALOG_CLOSE_BUTTON			)->Show( true );
	m_InterfaceFactory.GetImageStatic( NSELECTWORLDLIST_DIALOG_BACK_STATIC		)->Show( true );
}

void CNSelectWorldListDlg::PromoterHide( void )
{
	m_InterfaceFactory.GetStatic( NSELECTWORLDLIST_DIALOG_EXPLAIN_STATIC		)->Show( false );
	m_InterfaceFactory.GetButton( NSELECTWORLDLIST_DIALOG_ENTER_SERVER_BUTTON	)->Show( false );
	m_InterfaceFactory.GetButton( NSELECTWORLDLIST_DIALOG_CLOSE_BUTTON			)->Show( false );
	m_InterfaceFactory.GetImageStatic( NSELECTWORLDLIST_DIALOG_BACK_STATIC		)->Show( false );
}

void CNSelectWorldListDlg::SetPromoterText( TCHAR* _pPromoterName, SI32 _siServerUnique )
{
	if( 0 >= _tcslen(_pPromoterName) )
		PromoterHide();

	PromoterShow();

	cltClient * pclclient = (cltClient*) pclClient;

	CStatic* pExplain = m_InterfaceFactory.GetStatic( NSELECTWORLDLIST_DIALOG_EXPLAIN_STATIC );

	NTCHARString512 szExplain( GetTxtFromMgr(30293) );	
	szExplain.Replace(TEXT("#promotername#"),	_pPromoterName );

	pExplain->SetText( szExplain );

	m_PromoterServerID = _siServerUnique;
}

