#include "NMykey.h"
#include "NMykeyMini.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char\CharManager\CharManager.h"
#include "../../ninterface/NNewinventory/NNewInventory.h"
#include "../../ninterface/ninfodlg/NInfoDlg.h"
#include "../../ninterface/nresidentsdlg/NResidentsDlg.h"
#include "../../CommonLogic/Msg/MsgType-PersonalShop.h"

CNMykeyDlg::CNMykeyDlg()
{
	ZeroMemory( m_siDialogKind,			sizeof(m_siDialogKind)		 );
	ZeroMemory( m_clControlMapping,		sizeof(m_clControlMapping)	 );

	memset( m_siDialogKind,		  -1,	sizeof(m_siDialogKind)	     );

	m_bCreateComplete = false;

	CheckControlMapping();
}

CNMykeyDlg::~CNMykeyDlg()
{
	Destroy();
}

void CNMykeyDlg::Init()
{
	
}

void CNMykeyDlg::Destroy()
{


}

void CNMykeyDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;
	
	file.LoadFile("Ninterface/Data/NMyKey/NMyKey.ddf");
	file.CreatePopupDialog( this , NMYKEY, "dialog_mykey" , StaticNMykeyDlgProc );

	//if( g_bFullScreen)	MovePos( 80, 40 );
	//else		 			MovePos( 45, 20 );

	if( g_bFullScreen)	MovePos( 459, 478 );
	else		 		MovePos( 424, 458 );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  NMYKEY_STATIC_EXPLAIN,	this), NMYKEY_STATIC_EXPLAIN,	"statictext_explain" );
	
	m_InterfaceFactory.GetStatic( NMYKEY_STATIC_EXPLAIN )->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	m_InterfaceFactory.GetStatic( NMYKEY_STATIC_EXPLAIN )->Refresh();

	TCHAR szControlName[128] = "";
	for(SI32 Count = 0; Count < CHECKBOX_NUM; ++Count)
	{
		StringCchPrintf(szControlName, sizeof(szControlName),  "checkbox_NONAME%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(CHECKBOX, NMYKEY_CHECKBOX_NO0 + Count,	this), NMYKEY_CHECKBOX_NO0 + Count, szControlName );

		if( 0 >= m_clControlMapping[Count].GetTextNum() ) continue;
		
		m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO0 + Count )->SetText( GetTxtFromMgr( m_clControlMapping[Count].GetTextNum() ) ); 
	}

	for(SI32 Count = 0; Count < MYKEY_NUM; ++Count)
	{
		StringCchPrintf(szControlName, sizeof(szControlName),  "statictext_NONAME%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, NMYKEY_STATIC_ALT_NO0 + Count,	this), NMYKEY_STATIC_ALT_NO0 + Count, szControlName );

		StringCchPrintf(szControlName, sizeof(szControlName),  "editbox_NONAME%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NMYKEY_EDIT_ALT_NO0 + Count,		this), NMYKEY_EDIT_ALT_NO0   + Count, szControlName );
	}

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NMYKEY_OUTLINE_NO0,	this), NMYKEY_OUTLINE_NO0,		"outline_NONAME0"	 );

	
	if ( pclClient->IsCountrySwitch(Switch_PVP_League) )
	{
		m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO24 )->Show( true );
	}
	else
	{
		m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO24 )->Show( false );
	}

	if ( pclClient->IsCountrySwitch(Switch_WeddingSystem) )
	{
		m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO25 )->Show( true );
	}
	else
	{
		m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO25 )->Show( false );
	}

	

	LoadFile();
	LoadKey();

	m_bCreateComplete = true;

	Show( false );
}

void CALLBACK CNMykeyDlg::StaticNMykeyDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMykeyDlg *pThis = (CNMykeyDlg *)pControllerMgr;
	pThis->NMykeyDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK CNMykeyDlg::NMykeyDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				if( NULL == pclclient->m_pDialog[NMYKEYMINI] )
					pclclient->CreateInterface( NMYKEYMINI );
				else
				{
					((CNMykeyMiniDlg*)pclclient->m_pDialog[NMYKEYMINI])->Show( true );
					((CNMykeyMiniDlg*)pclclient->m_pDialog[NMYKEYMINI])->LoadFile();
				}

				Show( false );
			}
			break;
		case NMYKEY_CHECKBOX_NO0:
		case NMYKEY_CHECKBOX_NO1:
		case NMYKEY_CHECKBOX_NO2:
		case NMYKEY_CHECKBOX_NO3:
		case NMYKEY_CHECKBOX_NO4:
		case NMYKEY_CHECKBOX_NO5:
		case NMYKEY_CHECKBOX_NO6:
		case NMYKEY_CHECKBOX_NO7:
		case NMYKEY_CHECKBOX_NO8:
		case NMYKEY_CHECKBOX_NO9:
		case NMYKEY_CHECKBOX_NO10:
		case NMYKEY_CHECKBOX_NO11:
		case NMYKEY_CHECKBOX_NO12:
		case NMYKEY_CHECKBOX_NO13:
		case NMYKEY_CHECKBOX_NO14:
		case NMYKEY_CHECKBOX_NO15:
		case NMYKEY_CHECKBOX_NO16:
		case NMYKEY_CHECKBOX_NO17:
		case NMYKEY_CHECKBOX_NO18:
		case NMYKEY_CHECKBOX_NO19:
		case NMYKEY_CHECKBOX_NO20:
		case NMYKEY_CHECKBOX_NO21:
		case NMYKEY_CHECKBOX_NO22:
		case NMYKEY_CHECKBOX_NO23:
		case NMYKEY_CHECKBOX_NO24:
		case NMYKEY_CHECKBOX_NO25: 
			{
				switch( nEvent )
				{
					case EVENT_CHECKBOX_CHANGED:
						{
							if( true == m_InterfaceFactory.GetCheckBox(nControlID)->IsCheck() )
							{
								if( true == IsFullSetkey( nControlID ) )
								{
									m_InterfaceFactory.GetCheckBox(nControlID)->SetCheck( false, CONTROLLER_KIND_CHECKBOX, false );
									return;
								}
							}

							
							//SetKey();
							SetKey( nControlID );
							SaveFile();	// 설정이 바뀌어 저장.
							LoadFile();	// 설정이 바뀌어 저장.
							LoadKey();
						}
						break;
				}
			}
			break;

		default:
			{
			}
			break;
	}
}



void CNMykeyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}


void CNMykeyDlg::SetDialogKind(SI32* _psiDialogKind)
{


}

void CNMykeyDlg::KeyAction( SI32 _KeyKind )
{
	cltClient * pclclient = (cltClient*) pclClient;

	SI32	KeyIndex = -1;
	switch(_KeyKind)
	{
		case ID_SHORT_ALT_1:	KeyIndex = 0;	break;
		case ID_SHORT_ALT_2:	KeyIndex = 1;	break;
		case ID_SHORT_ALT_3:	KeyIndex = 2;	break;
		case ID_SHORT_ALT_4:	KeyIndex = 3;	break;
		case ID_SHORT_ALT_5:	KeyIndex = 4;	break;
	}

	if(-1 == KeyIndex )							 return;
	if( DIALOG_END <= m_siDialogKind[KeyIndex] ) return;
	if( 0		   >  m_siDialogKind[KeyIndex] ) return;

	if( IsDestroyWindow( m_siDialogKind[KeyIndex] ) )
	{
		if(m_siDialogKind[KeyIndex] == NPERSONALSHOP_DLG)
		{
			if( NULL == pclclient->m_pDialog[m_siDialogKind[KeyIndex]] )
			{
				cltGameMsgRequest_PersonalShopData clinfo( pclClient->pclCM->CR[1]->GetCharUnique(), FALSE,FALSE ); //만드는거 아니면 FALSE 
				cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);		
				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CLICKPERSONALSHOP );
			}
			else 
			{
				pclclient->DestroyInterface( pclclient->m_pDialog[m_siDialogKind[KeyIndex]] );
			}
		}
		else if(m_siDialogKind[KeyIndex] == NMANUFACTURE_DLG)
		{
			if( NULL == pclclient->m_pDialog[m_siDialogKind[KeyIndex]] )
			{
				cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),TRUE, FALSE ); // 만드는 창이지만 기본은 FALSE 
				cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);		
			}
			else 
			{
				pclclient->DestroyInterface( pclclient->m_pDialog[m_siDialogKind[KeyIndex]] );
			}
		}
		else if(m_siDialogKind[KeyIndex] == CLTNCHARSTATUS_DLG)
		{
			pclclient->CreateCharStatusDlg();
		}
		else if(m_siDialogKind[KeyIndex] == NHOMEPAGE_DLG)
		{
			HomePage();
		}
		else if(m_siDialogKind[KeyIndex] == NFATHER_DLG)
		{
			if ( ConstServiceArea_Japan != pclClient->siServiceArea )
			{
				// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
				if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < pclClient->siMinimumTeacherLevel )
				{
					pclclient->CreateInterface(NCHILD_DLG);
				}
				else
				{	
					pclclient->CreateInterface(NFATHER_DLG);
					pclclient->CreateInterface(NLOWLEVEL_DLG);
				}
			}
			else if ( ConstServiceArea_Japan == pclClient->siServiceArea )
			{
				// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
				if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL)
				{
					pclclient->CreateInterface(NCHILD_DLG);
					pclclient->CreateInterface(NLOWLEVEL_DLG);
				}
				else
				{	
					pclclient->CreateInterface(NFATHER_DLG);
					pclclient->CreateInterface(NLOWLEVEL_DLG);
				}
			}
		}
		else
		{
			if( NULL == pclclient->m_pDialog[m_siDialogKind[KeyIndex]] )
				pclclient->CreateInterface( m_siDialogKind[KeyIndex] );
			else
				pclclient->DestroyInterface( pclclient->m_pDialog[m_siDialogKind[KeyIndex]] );
		}
	}
	else
	{
		if(NNEWINVENTORY_DLG == m_siDialogKind[KeyIndex])
		{
			if (pclclient->m_pDialog[ NNEWINVENTORY_DLG ] )
			{
				if ( pclclient->m_pDialog[ NNEWINVENTORY_DLG ]->IsShow() == false )
				{
					((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->Show();
				}
				else
				{
					((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->Hide();
				}
			}
		}
		else if( NINFO_DLG == m_siDialogKind[KeyIndex])
		{
			if (pclclient->m_pDialog[ NINFO_DLG ] )
			{
				if ( pclclient->m_pDialog[ NINFO_DLG ]->IsShow() == false )
				{
					((cltNInfoDlg*)pclclient->m_pDialog[ NINFO_DLG ])->Show();
				}
				else
				{
					((cltNInfoDlg*)pclclient->m_pDialog[ NINFO_DLG ])->Close();
				}
			}
		}
		else if( NRESIDENTS_DLG == m_siDialogKind[KeyIndex])
		{
			if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
			{
				( ( CNResidentsDlg * )pclclient->m_pDialog[ NRESIDENTS_DLG ] )->Show();
			}
		}
		else
		{
			if( NULL ==  pclclient->m_pDialog[m_siDialogKind[KeyIndex]] ) return;

			bool bShow = pclclient->m_pDialog[m_siDialogKind[KeyIndex]]->IsShow();

			pclclient->m_pDialog[m_siDialogKind[KeyIndex]]->Show( !bShow );

		}
	}
}

void CNMykeyDlg::SaveFile()
{
	FILE* fp = NULL;
	fp = _tfopen(TEXT("Config/Mykey.dat"), TEXT("wb"));
		
	fwrite( m_siDialogKind, sizeof(m_siDialogKind), 1, fp );

	fclose( fp );
}

void CNMykeyDlg::LoadFile()
{
	FILE* fp = NULL;
	fp = _tfopen(TEXT("Config/Mykey.dat"), TEXT("rb"));
	
	if( NULL == fp )	// 파일이 없으면 생성.
	{
		SaveFile();
	}
	else				// 있으면 읽자.
	{
		fread( m_siDialogKind, sizeof(m_siDialogKind), 1, fp );
		fclose( fp );
	}

	// 파일에서 얻어온 창의 정보가 이상한게 있다면 현재 저장했던 모든 키들을 삭제한다.
	for( SI32 Count = 0; Count < MYKEY_NUM; ++Count ) 
	{
		if(0 < m_siDialogKind[Count]) 
		{
			if( 0 > FindTextNum(m_siDialogKind[Count]) )
			{
				memset( m_siDialogKind,	-1,	sizeof(m_siDialogKind) );
				SaveFile();
				break;
			}
		}
	}
	
	// 설정한 컨트롤 인덱스를 이용해 텍스트번호를 얻어 설정한 키 정보로 표시. 
	SI32 TextNum = 0;
	for( SI32 Count = 0; Count < MYKEY_NUM; ++Count ) 
	{
		TextNum = FindTextNum( m_siDialogKind[Count] );
		
		if( 0 >= TextNum ) 
			m_InterfaceFactory.GetEdit(NMYKEY_EDIT_ALT_NO0 + Count)->SetText("");
		else
			m_InterfaceFactory.GetEdit(NMYKEY_EDIT_ALT_NO0 + Count)->SetText( GetTxtFromMgr( TextNum ) );
	}
}

void CNMykeyDlg::CheckControlMapping()
{
	CControlMapping	Mapping[] = 
	{
		CControlMapping( CLTNCHARSTATUS_DLG,		8331	),
		CControlMapping( NNEWEQUIPWINDOW_DLG,		8332	),
		CControlMapping( NNEWINVENTORY_DLG,			8333	),
		CControlMapping( NINFO_DLG,					8334	),
		CControlMapping( NITEMMALL_DLG,				8807	),
		CControlMapping( NSKILL_DLG,				8313	),
		CControlMapping( true == pclClient->IsCountrySwitch(Switch_SkillBook)			 ? NNEWMAGICINFO_DLG    : NMAGICINFO_DLG,	8314	),
		CControlMapping( true == pclClient->IsCountrySwitch(Switch_QuestInfoDlg_Renewal) ? NQUESTINFODLG_PARENT : NQUESTINFO_DLG,	8315	),
		CControlMapping( NRESIDENTS_DLG,			8319	),
		CControlMapping( NFATHER_DLG,				8320	),
		CControlMapping( NGUILDMEMBER_DLG,			8321	),
		CControlMapping( NNEWFRIEND_DLG,			8322	),
		CControlMapping( NPERSONALSHOP_DLG,			8323	),
		CControlMapping( NPERSONALSHOPSEARCH_DLG,	8806	),
		CControlMapping( NMANUFACTURE_DLG,			2265	),
		CControlMapping( NMASTERINFO_DLG,			3340	),
		CControlMapping( NHOMEPAGE_DLG,				8317	),
		CControlMapping( NMINIMAP_DLG,				8326	),
		CControlMapping( NSOUNDONOFF_DLG,			8327	),
		CControlMapping( NHINT_DLG,					8329	),
		CControlMapping( NEXIT_DLG,					8330	),
		CControlMapping( CHARTRANSFORMINFO_DLG,		7782	),
		CControlMapping( NSPACE_BOX,				7921	),
		CControlMapping( NNEWITEMMALL_DLG,			8335	),
		CControlMapping( PVPLEAGUE_DLG,				9581	),
		CControlMapping( NFAMILYINFO_DLG,			30100	),
	};

	SI32 ControlNum = sizeof(Mapping)/ sizeof(Mapping[0]);
	for(SI32 Count = 0; Count < ControlNum; ++Count)
	{
		m_clControlMapping[Count].Set( Mapping[Count].GetDialogIndex(), Mapping[Count].GetTextNum() );
	}
}

// 창번호로 텍스트 번호 얻기. 
SI32 CNMykeyDlg::FindTextNum( SI32 _DialogIndex )
{	
	for( SI32 Count = 0; Count < CHECKBOX_NUM; ++Count )
	{
		if( _DialogIndex == m_clControlMapping[Count].GetDialogIndex() )
			return 	m_clControlMapping[Count].GetTextNum();
	}
	
	return -1;
}

// 창번호로 컨트롤 번호 얻기.
SI32 CNMykeyDlg::FindCotrolIndex( SI32 _DialogIndex )
{	
	for( SI32 Count = 0; Count < CHECKBOX_NUM; ++Count )
	{
		if( _DialogIndex == m_clControlMapping[Count].GetDialogIndex() )
			return 	Count;
	}

	return -1;
}

void CNMykeyDlg::SetKey( void )
{
	memset( m_siDialogKind,	-1,	sizeof(m_siDialogKind) );

	SI32 DialogCount = 0;
	for( SI32 siCount = 0; siCount < CHECKBOX_NUM; ++siCount )
	{
		if( true == m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO0 + siCount )->IsCheck() )
		{
			m_siDialogKind[DialogCount++] = m_clControlMapping[siCount].GetDialogIndex();
			
			if( MYKEY_NUM <= DialogCount )	break;
		}
	}
}

void CNMykeyDlg::SetKey( SI32 _ControllID )
{
	SI32 ControlMapingIndex = _ControllID - NMYKEY_CHECKBOX_NO0;

	if( true == m_InterfaceFactory.GetCheckBox( _ControllID )->IsCheck() )
	{
		for(SI32 Count = 0; Count < MYKEY_NUM; ++Count)
		{
			if( 0 > m_siDialogKind[Count] )
			{
				m_siDialogKind[Count] = m_clControlMapping[ControlMapingIndex].GetDialogIndex();
				return;
			}
		}		
	}
	else
	{
		for(SI32 Count = 0; Count < MYKEY_NUM; ++Count)
		{
			if( m_siDialogKind[Count] == m_clControlMapping[ControlMapingIndex].GetDialogIndex() )
			{
				m_siDialogKind[Count] = -1;
				return;
			}
		}		
	}
}

void CNMykeyDlg::LoadKey( void )
{
	for( SI32 siCount = 0; siCount < CHECKBOX_NUM; ++siCount )
	{
		m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO0 + siCount )->SetCheck( false, CONTROLLER_KIND_CHECKBOX, false );
	}

	for( SI32 siCount = 0; siCount < MYKEY_NUM; ++siCount )
	{
		if(-1 != m_siDialogKind[siCount])
		{
			m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO0 + FindCotrolIndex( m_siDialogKind[siCount] ) )->SetCheck( true, CONTROLLER_KIND_CHECKBOX, false );
		}
	}
}

//-- 다이얼로그중에서 이미 자기자신이 생성되어 있다면 없어지는 창은 이곳에 추가한다 show 로 제어하지 않는 창들.
bool CNMykeyDlg::IsDestroyWindow( SI32 _DialogIndex )
{
	bool Result = false;
	switch( _DialogIndex )
	{
		case CHARTRANSFORMINFO_DLG:
		case CLTNCHARSTATUS_DLG:
		case NSKILL_DLG:
		case NFATHER_DLG:
		case NITEMMALL_DLG:
		case NNEWITEMMALL_DLG:
		case NMASTERINFO_DLG:
		case NEXIT_DLG: 
		case NPERSONALSHOP_DLG: 
		case NGUILDMEMBER_DLG:
		case NPERSONALSHOPSEARCH_DLG: 
		case NMANUFACTURE_DLG:  
		case NHINT_DLG:		
		case NHOMEPAGE_DLG:
		case NQUESTINFODLG_PARENT:
		case NSPACE_BOX:	
		case PVPLEAGUE_DLG:
		case NFAMILYINFO_DLG:
			{
				Result = true; 
			}
			break;
	}

	return Result;
}

bool CNMykeyDlg::IsFullSetkey( int _nControlID )
{
	SI32 SetKeyNum = 0;
	for( SI32 siCount = 0; siCount < CHECKBOX_NUM; ++siCount )
	{
		if( true == m_InterfaceFactory.GetCheckBox( NMYKEY_CHECKBOX_NO0 + siCount )->IsCheck() )
		{
			if( _nControlID != NMYKEY_CHECKBOX_NO0 + siCount )
				++SetKeyNum;
		}
	}
	
	bool bResult = ( MYKEY_NUM <= SetKeyNum ) ? true : false;
	if( bResult )
	{
		cltClient * pclclient = (cltClient*) pclClient;
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8826) );
	}

	return bResult;
}

void CNMykeyDlg::HomePage( void )
{
	cltClient * pclclient = (cltClient*) pclClient;
	if( pclclient->siServiceArea == ConstServiceArea_English )
	{
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.playluminary.com", NULL, SW_SHOWNORMAL );								
	}
	//KHY - 0617 - 일본 채널링 추가.
	else if( pclclient->siServiceArea == ConstServiceArea_Japan )
	{
		TCHAR* AccountID = pclclient->pclCM->CR[1]->pclCI->clBI.szAccountID;

		// 퍼블리셔 확인을 위한 파싱 부분.
		SI16 idSize = strlen(AccountID);

		TCHAR temPublisher[20] = {'\0',};
		bool	    tempFlage =false;
		SI16    tempNum = 0;

		for(SI16 i =0;i < idSize; i++)
		{
			// "@" 문자 이전의 문자만을 리턴한다.
			if((tempFlage == false) &&( AccountID[i] == '@'))
			{	
				tempFlage = true;
				continue;
			}

			if(tempFlage == true)
			{
				temPublisher[tempNum] = AccountID[i];
				tempNum++;
			}
		}

		if(strcmp(temPublisher, "HG") == 0) // 한게임.
		{
			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.hangame.co.jp", NULL, SW_SHOWNORMAL );
		}
		else if(strcmp(temPublisher, "GP") == 0) // 게임팟.
		{
			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.postar.jp", NULL, SW_SHOWNORMAL );
		}
		else if(strcmp(temPublisher, "GA") == 0) // 감마니아.
		{
			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.ndoors.jp/gamania", NULL, SW_SHOWNORMAL );							
		}
		else if(strcmp(temPublisher, "ND") == 0) // NDE
		{
			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.ndoors.jp", NULL, SW_SHOWNORMAL );
		}
		else
		{
#ifdef _DEBUG
			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://kunshu.ndoors.jp/gamania", NULL, SW_SHOWNORMAL );							
#else
			return;	
#endif
		}

	}
	else if( pclclient->siServiceArea == ConstServiceArea_China )
	{
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.hljz.net/Default_Z1.aspx", NULL, SW_SHOWNORMAL );				
	}
	//KHY -0610 - S 추가.
	else if( pclclient->siServiceArea == ConstServiceArea_Korea )
	{
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://goonzus.ndoors.com/", NULL, SW_SHOWNORMAL );
	}
	else if( pclclient->siServiceArea == ConstServiceArea_Taiwan )
	{
		//ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://global.goonzu.com", NULL, SW_SHOWNORMAL );
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.omg.com.tw/", NULL, SW_SHOWNORMAL );
	}
	else if( pclclient->siServiceArea == ConstServiceArea_USA )
	{
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );								
	}
	else if( pclclient->siServiceArea == ConstServiceArea_EUROPE )
	{
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.aeriagames.com/ ", NULL, SW_SHOWNORMAL );								
	}
	else if( pclclient->siServiceArea == ConstServiceArea_NHNChina )// 임시 NHN중국 
	{
		ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "www.newhljz.cn", NULL, SW_SHOWNORMAL );								
	}
}
