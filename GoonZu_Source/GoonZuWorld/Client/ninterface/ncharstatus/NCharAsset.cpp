//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :

	작성일 :		05/03/17
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#include "..\Client\Client.h"
#include "NCharAsset.h"
#include "NCharStatus.h"
#include "../../../Common/Father\Father.h"

#include "..\resource.h"
#include "..\Server\Rank\Rank.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "Msg\MsgType-Horse.h"
#include "Msg\MsgType-Summon.h"
#include "../../../Common/JWLib/IsCorrectID/IsCorrectID.h"
//#include "../../lib/WebHTML/WebHTML.h"
#include "..\..\Client\Music\Music.h"

#include "../../../Client/InterfaceMgr/Interface/Static.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"
#include "../../../Client/InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"

//-----------------------------
// CommonLogic
//-----------------------------
#include "..\CommonLogic\msgType-Person.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"

extern cltCommonLogic* pclClient;

static cltStockAccount clOldStockInfo;
//_LEON_GUILD_STOCK
//static cltGuildStockAccount clOldGuildStockInfo;

cltNAssetStatus::cltNAssetStatus(cltCharManager* pclcm)
{
	m_pStatictext_asset	 = NULL; 
	m_pStatictext_serverrank	 = NULL; 
	m_pEditbox_serverrank	 = NULL; 
	m_pStatictext_totalwealth	 = NULL; 
	m_pEditbox_totalwealth	 = NULL; 
	m_pStatictext_bank	 = NULL; 
	m_pStatictext_bankmoney	 = NULL; 
	m_pEditbox_bankmoney	 = NULL; 
	m_pStatictext_bankvillage	 = NULL; 
	m_pEditbox_bankvillage	 = NULL; 
	m_pStatictext_stock	 = NULL; 
	m_pListbox_stock	 = NULL; 
	m_pStatictext_house	 = NULL; 
	m_pListbox_house	 = NULL;	
	m_pStatictext_wealthupdate	 = NULL; 

	m_pOutline_noname1	 = NULL; 
	m_pOutline_noname2	 = NULL; 
	m_pOutline_noname3	 = NULL; 
	m_pOutline_noname4	 = NULL; 
	
	//_LEON_GUILD
    m_pOutline_noname6	 = NULL; 
	m_pStatic_guildinfo	 = NULL;
	m_pStatic_guildpos	 = NULL;
	m_pStatic_guildname	 = NULL;
	m_pStatic_guildrank	 = NULL;
	m_pEditbox_guildpos = NULL;    
	m_pEditbox_guildname = NULL;   
	m_pEditbox_guildrank = NULL;   
	
	//_LEON_GUILD_STOCK
	// 상단 주식
	m_pOutLine_guildstock = NULL;
	m_pStatic_guildstock = NULL;
	m_pListbox_guildstock = NULL;

	// 결투장 랭킹
	m_pStatic_battlescore = NULL; 
	m_pStatic_battlewin = NULL;   
	m_pStatic_defeatscore = NULL; 
	m_pStatic_battlemaxwin = NULL;
	m_pEdit_battlewin = NULL;     
	m_pEdit_defeatscore = NULL;   
	m_pEdit_battlemaxwin = NULL;  

	m_bShow = false;
	pclCM	= pclcm;
	CharUnique = 0;
}

cltNAssetStatus::~cltNAssetStatus()
{
	if( 	m_pStatictext_asset	 )  	delete 	m_pStatictext_asset	;
	if( 	m_pStatictext_serverrank	 )  	delete 	m_pStatictext_serverrank	;
	if( 	m_pEditbox_serverrank	 )  	delete 	m_pEditbox_serverrank	;
	if( 	m_pStatictext_totalwealth	 )  	delete 	m_pStatictext_totalwealth	;
	if( 	m_pEditbox_totalwealth	 )  	delete 	m_pEditbox_totalwealth	;
	if( 	m_pStatictext_bank	 )  	delete 	m_pStatictext_bank	;
	if( 	m_pStatictext_bankmoney	 )  	delete 	m_pStatictext_bankmoney	;
	if( 	m_pEditbox_bankmoney	 )  	delete 	m_pEditbox_bankmoney	;
	if( 	m_pStatictext_bankvillage	 )  	delete 	m_pStatictext_bankvillage	;
	if( 	m_pEditbox_bankvillage	 )  	delete 	m_pEditbox_bankvillage	;
	if( 	m_pStatictext_stock	 )  	delete 	m_pStatictext_stock	;
	if( 	m_pListbox_stock	 )  	delete 	m_pListbox_stock	;
	if( 	m_pStatictext_house	 )  	delete 	m_pStatictext_house	;
	if( 	m_pListbox_house	 )  	delete 	m_pListbox_house	;
	if( 	m_pStatictext_wealthupdate	 )  	delete 	m_pStatictext_wealthupdate	;

	if( 	m_pOutline_noname1	 )  	delete 	m_pOutline_noname1	;
	if( 	m_pOutline_noname2	 )  	delete 	m_pOutline_noname2	;
	if( 	m_pOutline_noname3	 )  	delete 	m_pOutline_noname3	;
	if( 	m_pOutline_noname4	 )  	delete 	m_pOutline_noname4	;
	
	if(m_pOutline_noname6	 )  	delete 	m_pOutline_noname6	;
	if(m_pStatic_guildinfo	) delete m_pStatic_guildinfo ;
	if(m_pStatic_guildpos	) delete m_pStatic_guildpos ;
	if(m_pStatic_guildname	) delete m_pStatic_guildname ;
	if(m_pStatic_guildrank	) delete m_pStatic_guildrank ;
	if(m_pEditbox_guildpos) delete m_pEditbox_guildpos ;    
	if(m_pEditbox_guildname) delete m_pEditbox_guildname ;   
	if(m_pEditbox_guildrank) delete m_pEditbox_guildrank ;   

	if(m_pOutLine_guildstock ) delete m_pOutLine_guildstock;
	if(m_pStatic_guildstock ) delete m_pStatic_guildstock;
	if(m_pListbox_guildstock ) delete m_pListbox_guildstock;

	if(m_pStatic_battlescore) delete m_pStatic_battlescore; 
	if(m_pStatic_battlewin) delete m_pStatic_battlewin;   
	if(m_pStatic_defeatscore) delete m_pStatic_defeatscore; 
	if(m_pStatic_battlemaxwin) delete m_pStatic_battlemaxwin;
	if(m_pEdit_battlewin) delete m_pEdit_battlewin;     
	if(m_pEdit_defeatscore) delete m_pEdit_defeatscore;   
	if(m_pEdit_battlemaxwin) delete m_pEdit_battlemaxwin;  

    if(m_pOutLine_acu) delete m_pOutLine_acu;
	if(m_pStatic_acu) delete m_pStatic_acu;
	if(m_pEditbox_acu) delete m_pEditbox_acu;

	SetTimerOnOff( false );

}

void cltNAssetStatus::Create( CControllerMgr *pParent )
{
	//if ( ! IsCreate() )
	{
		cltClient *pclclient = (cltClient*)pclClient;
		CharUnique = pclclient->MyCharUnique;

		//CDialogController::Create( CLTNASSETSTATUS_DLG, CONTROLLER_KIND_CHILDDIALOG, pParent, NAssetStatusStaticProc, NULL, NULL, 0, false, 10, 40, 325, 480, false, false, false);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NCharStatusDlg/DLG_NCharStatus_Child_NAsset.ddf"));
		file.CreateChildDialog( this, CLTNASSETSTATUS_DLG, TEXT("dialog_asset"), NAssetStatusStaticProc, pParent );

		// 다이얼로그 위치 변경
		SetDialogPos( ((cltNCharStatus*)pParent)->GetChildDlgXPos(), ((cltNCharStatus*)pParent)->GetChildDlgYPos(), GetWidth(), GetHeight() );

		m_pStatictext_asset			= new 	CStatic	( this );
		m_pStatictext_serverrank	= new 	CStatic	( this );
		m_pEditbox_serverrank		= new 	CEdit	( this );
		m_pStatictext_totalwealth	= new 	CStatic	( this );
		m_pEditbox_totalwealth		= new 	CEdit	( this );
		m_pStatictext_bank			= new 	CStatic	( this );
		m_pStatictext_bankmoney		= new 	CStatic	( this );
		m_pEditbox_bankmoney		= new 	CEdit	( this );
		m_pStatictext_bankvillage	= new 	CStatic	( this );
		m_pEditbox_bankvillage		= new 	CEdit	( this );
		m_pStatictext_stock	 		= new 	CStatic	( this );
		m_pListbox_stock	 		= new 	CList	( this );
		m_pStatictext_house	 		= new 	CStatic	( this );
		m_pListbox_house	 		= new 	CList	( this );
		m_pStatictext_wealthupdate	= new 	CStatic	( this );

		m_pOutline_noname1	 = new 	COutLine( this );
		m_pOutline_noname2	 = new 	COutLine( this );
		m_pOutline_noname3	 = new 	COutLine( this );
		m_pOutline_noname4	 = new 	COutLine( this );
		
		m_pOutline_noname6	 = new 	COutLine	( this )	;
		m_pStatic_guildinfo	 = new CStatic( this ) ;
		m_pStatic_guildpos	 = new CStatic( this ) ;
		m_pStatic_guildname	 = new CStatic( this ) ;
		m_pStatic_guildrank	 = new CStatic( this ) ;
		m_pEditbox_guildpos	 = new CEdit( this );    
		m_pEditbox_guildname = new CEdit( this );   
		m_pEditbox_guildrank = new CEdit( this );   

		m_pOutLine_guildstock = new COutLine(this) ;
		m_pStatic_guildstock  = new CStatic(this) ;
		m_pListbox_guildstock = new CList(this) ;

		// 결투장 랭킹
		m_pStatic_battlescore	= new CStatic( this ); 
		m_pStatic_battlewin		= new CStatic( this );   
		m_pStatic_defeatscore	= new CStatic( this ); 
		m_pStatic_battlemaxwin	= new CStatic( this );
		m_pEdit_battlewin		= new CEdit( this );     
		m_pEdit_defeatscore		= new CEdit( this );   
		m_pEdit_battlemaxwin	= new CEdit( this );  

		//아큐
		m_pOutLine_acu	= new COutLine( this );
		m_pStatic_acu	= new CStatic( this );
		m_pEditbox_acu	= new CEdit( this );
	
		//_LEON_GUILD
		file.CreateControl( 	m_pStatic_guildinfo	, 	CLTNASSETSTATUS_DLG_STATICTEXT_GUILDINFO	, 	TEXT("statictext_guildinfo")	 )	;
		file.CreateControl( 	m_pStatic_guildpos	, 	CLTNASSETSTATUS_DLG_STATICTEXT_GUILDPOS	, 		TEXT("statictext_guildpos")	 )	;
		file.CreateControl( 	m_pStatic_guildname	, 	CLTNASSETSTATUS_DLG_STATICTEXT_GUILDNAME	, 	TEXT("statictext_guildname")	 )	;
		file.CreateControl( 	m_pStatic_guildrank	, 	CLTNASSETSTATUS_DLG_STATICTEXT_GUILDRANK	, 	TEXT("statictext_guildrank")	 )	;

		file.CreateControl( m_pEditbox_guildpos, CLTNASSETSTATUS_DLG_EDITBOX_GUILDPOS, TEXT("editbox_guildpos") );
		file.CreateControl( m_pEditbox_guildname, CLTNASSETSTATUS_DLG_EDITBOX_GUILDNAME, TEXT("editbox_guildname") );
		file.CreateControl( m_pEditbox_guildrank, CLTNASSETSTATUS_DLG_EDITBOX_GUILDRANK, TEXT("editbox_guildrank") );

		file.CreateControl( m_pStatictext_asset, CLTNASSETSTATUS_DLG_STATICTEXT_ASSET, TEXT("statictext_asset") );
		file.CreateControl( m_pStatictext_serverrank, CLTNASSETSTATUS_DLG_STATICTEXT_SERVERRANK, TEXT("statictext_serverrank") );
		file.CreateControl( m_pEditbox_serverrank, CLTNASSETSTATUS_DLG_EDITBOX_SERVERRANK, TEXT("editbox_serverrank") );
		file.CreateControl( m_pStatictext_totalwealth, CLTNASSETSTATUS_DLG_STATICTEXT_TOTALWEALTH, TEXT("statictext_totalwealth") );
		file.CreateControl( m_pEditbox_totalwealth, CLTNASSETSTATUS_DLG_EDITBOX_TOTALWEALTH, TEXT("editbox_totalwealth") );
		file.CreateControl( m_pStatictext_bank, CLTNASSETSTATUS_DLG_STATICTEXT_BANK, TEXT("statictext_bank") );
		file.CreateControl( m_pStatictext_bankmoney, CLTNASSETSTATUS_DLG_STATICTEXT_BANKMONEY, TEXT("statictext_bankmoney") );
		file.CreateControl( m_pEditbox_bankmoney, CLTNASSETSTATUS_DLG_EDITBOX_BANKMONEY, TEXT("editbox_bankmoney") );
		file.CreateControl( m_pStatictext_bankvillage, CLTNASSETSTATUS_DLG_STATICTEXT_BANKVILLAGE, TEXT("statictext_bankvillage") );
		file.CreateControl( m_pEditbox_bankvillage, CLTNASSETSTATUS_DLG_EDITBOX_BANKVILLAGE, TEXT("editbox_bankvillage") );
		file.CreateControl( m_pStatictext_stock, CLTNASSETSTATUS_DLG_STATICTEXT_STOCK, TEXT("statictext_stock") );
		file.CreateControl( m_pListbox_stock, CLTNASSETSTATUS_DLG_LISTBOX_STOCK, TEXT("listbox_stock") );
		file.CreateControl( m_pStatictext_house, CLTNASSETSTATUS_DLG_STATICTEXT_HOUSE, TEXT("statictext_house") );
		file.CreateControl( m_pListbox_house, CLTNASSETSTATUS_DLG_LISTBOX_HOUSE, TEXT("listbox_house") );
		file.CreateControl( m_pStatictext_wealthupdate	, 	CLTNASSETSTATUS_DLG_STATICTEXT_WEALTHUPDATE	, 	TEXT("statictext_wealthupdate")	 )	;

		file.CreateControl( m_pStatic_guildstock	, 	CLTNASSETSTATUS_DLG_STATICTEXT_GUILDSTOCK	, 	TEXT("statictext_guildstock")	 )	;
		file.CreateControl( m_pListbox_guildstock, CLTNASSETSTATUS_DLG_LISTBOX_GUILDSTOCK, TEXT("listbox_guildstock") );

		// 길드 랭킹
		file.CreateControl( m_pStatic_battlescore	, 	CLTNASSETSTATUS_DLG_STATICTEXT_BATTLESCORE	, 	TEXT("statictext_battlescore")	 )	;
		file.CreateControl( m_pStatic_battlewin	, 	CLTNASSETSTATUS_DLG_STATICTEXT_BATTLEWIN	, 	TEXT("statictext_battlewin")	 )	;
		file.CreateControl( m_pStatic_defeatscore	, 	CLTNASSETSTATUS_DLG_STATICTEXT_DEFEATSCORE	, 	TEXT("statictext_defeatscore")	 )	;
		file.CreateControl( m_pStatic_battlemaxwin	, 	CLTNASSETSTATUS_DLG_STATICTEXT_BATTLEMAXWIN	, 	TEXT("statictext_battlemaxwin")	 )	;

		file.CreateControl( m_pEdit_battlewin, CLTNASSETSTATUS_DLG_EDITBOX_BATTLEWIN, TEXT("editbox_battlewin") );
		file.CreateControl( m_pEdit_defeatscore, CLTNASSETSTATUS_DLG_EDITBOX_DEFEATSCORE, TEXT("editbox_defeatscore") );
		file.CreateControl( m_pEdit_battlemaxwin, CLTNASSETSTATUS_DLG_EDITBOX_BATTLEMAXWIN, TEXT("editbox_battlemaxwin") );

		// 아큐
		file.CreateControl( m_pStatic_acu	, 	CLTNASSETSTATUS_DLG_STATICTEXT_ACU	, 	TEXT("statictext_acu")	 )	;
		file.CreateControl( m_pEditbox_acu, CLTNASSETSTATUS_DLG_EDITBOX_ACU, TEXT("editbox_acu") );
		file.CreateControl( m_pOutLine_acu	, 	CLTNASSETSTATUS_DLG_OUTLINE_ACU	, 	TEXT("outline_acu")	 )	;

		file.CreateControl( 	m_pOutline_noname6	, 	CLTNASSETSTATUS_DLG_OUTLINE_NONAME6	, 	TEXT("outline_NONAME6")	 )	;
		file.CreateControl( 	m_pOutLine_guildstock	, 	CLTNASSETSTATUS_DLG_OUTLINE_GUILDSTOCK	, 	TEXT("outline_guildstock")	 )	;
		file.CreateControl( 	m_pOutline_noname1	, 	CLTNASSETSTATUS_DLG_OUTLINE_NONAME1	, 	TEXT("outline_NONAME1")	 )	;
		file.CreateControl( 	m_pOutline_noname2	, 	CLTNASSETSTATUS_DLG_OUTLINE_NONAME2	, 	TEXT("outline_NONAME2")	 )	;
		file.CreateControl( 	m_pOutline_noname3	, 	CLTNASSETSTATUS_DLG_OUTLINE_NONAME3	, 	TEXT("outline_NONAME3")	 )	;
		file.CreateControl( 	m_pOutline_noname4	, 	CLTNASSETSTATUS_DLG_OUTLINE_NONAME4	, 	TEXT("outline_NONAME4")	 )	;


		TCHAR* pText = GetTxtFromMgr(3772);
		m_pStatictext_asset->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(3773);
		m_pStatictext_serverrank->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(3774);
		m_pStatictext_totalwealth->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(3777);
		m_pStatictext_bank->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(3776);
		m_pStatictext_bankvillage->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(3775);
		m_pStatictext_bankmoney->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//주식
		pText = GetTxtFromMgr(368);
		 m_pStatictext_stock->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//부동산
		pText = GetTxtFromMgr(2321);
		m_pStatictext_house->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        

		//pText = GetTxtFromMgr(3902);
		//m_pListbox_stock->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(5428);
	    m_pStatic_guildstock->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(6147);
		m_pStatic_acu->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		

		SetTimerOnOff( true );
		clOldStockInfo.Init();
		//_LEON_GUILD_STOCK
//		clOldGuildStockInfo.Init();

		m_pListbox_stock->SetColumn( 0, 113, NULL );
		m_pListbox_stock->SetColumn( 1, 18, NULL );
		m_pListbox_stock->SetBorder( true );
		m_pListbox_stock->SetBKColorUse( true );
		m_pListbox_stock->Refresh();

		//_LEON_GUILD_STOCK
		m_pListbox_guildstock->SetColumn( 0, 113, NULL );
		m_pListbox_guildstock->SetColumn( 1, 18, NULL );
		m_pListbox_guildstock->SetBorder( true );
		m_pListbox_guildstock->SetBKColorUse( true );
		m_pListbox_guildstock->Refresh();

		m_pListbox_house->SetColumn( 0, 120, NULL );
		m_pListbox_house->SetColumn( 1, 18, NULL );
		m_pListbox_house->SetBorder( true );
		m_pListbox_house->SetBKColorUse( true );
		m_pListbox_house->Refresh();
		

		//// 길드 정보를 요청함
		//cltMsg clMsg(GAMEMSG_REQUEST_MYGUILDINFO, 0, NULL );
		//pclclient->SendMsgToServer((sPacketHeader*)&clMsg );

		Hide();

	}
	//else
	//{
	//	DeleteDialog();
	//}
}


void CALLBACK cltNAssetStatus::NAssetStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNAssetStatus *pThis = (cltNAssetStatus*)pControllerMgr;
	pThis->NAssetStatusProc( nEvent, nControlID, pControl);
}


void CALLBACK cltNAssetStatus::NAssetStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			// 타이머를 해제한다. 
			SetTimerOnOff( false );
			DeleteDialog();
		}
		break;
	}
}

void cltNAssetStatus::Action()
{
	SI32 i;
	TCHAR buffer[256];
	TCHAR string[128];
	stListBoxItemData tmpListBoxItemData;

	cltClient* pclclient = (cltClient*)pclClient;

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

	//----------------------
	// 총재산 정보 표시. 
	//-----------------------

	// 순위. 
	if(pclchar->pclCI->clBI.siWealthRank && pclchar->pclCI->clIP.GetLevel() >= FATHER_LEVEL)
	{
		StringCchPrintf(buffer, 256, TEXT("%d"), pclchar->pclCI->clBI.siWealthRank);
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(610);
		StringCchPrintf(buffer, 256, pText);
	}
	m_pEditbox_serverrank->SetText( buffer );

	// 총재산액. 
	TCHAR* pText = GetTxtFromMgr(611);
	GMONEY siTotalAsset = pclchar->pclCI->clBI.clTotalWealthMoney.itMoney;
	if ( siTotalAsset == 0 )
	{
		StringCchCopy( buffer, 256, TEXT("[-]") );
	}
	else
	{
		g_JYLibFunction.SetNumUnit(siTotalAsset, string, 128, pText);
		StringCchPrintf(buffer, 256, TEXT("%s"), string);
	}
	m_pEditbox_totalwealth->SetText( buffer );

	// 순위 업데이트 시간. 
	cltDate cldate;
	cldate.MakeFullDateFromDateVary(pclchar->pclCI->clBI.siWealthCalcDay);
	if(cldate.GetDateText(string) == TRUE)
	{
		pText = GetTxtFromMgr(612);
		StringCchPrintf(buffer, 256, pText, string);
		m_pStatictext_wealthupdate->SetText( buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	// 전장 잔액 표시. 
	pText = GetTxtFromMgr(611);
	g_JYLibFunction.SetNumUnit(pclchar->pclCI->clBank.GetMoney(), string, 128, pText);
	StringCchPrintf(buffer, 256, TEXT("%s"), string);
	m_pEditbox_bankmoney->SetText( buffer );

	// 아큐 표시
	pText = GetTxtFromMgr(6147);
	g_JYLibFunction.SetNumUnit( pclchar->pclCI->clWorldMoney.siWorldMoney, string, 128, pText );
	m_pEditbox_acu->SetTextPrintOption( DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	m_pEditbox_acu->SetText( string );

	// 전장 마을 표시. 
	TCHAR* pbankname = pclClient->pclVillageManager->GetName(pclchar->pclCI->clBank.clAccountNum.siVillageUnique);
	if(pbankname)
	{
		StringCchPrintf(buffer, 256, TEXT("%s"), pbankname);
	}
	else
	{
		pText = GetTxtFromMgr(613);
		StringCchPrintf(buffer, 256, TEXT("%s"), pText);
	}
	m_pEditbox_bankvillage->SetText( buffer );

	// 주식 
	if(pclchar->pclCI->clStock.clInfo.IsSame(&clOldStockInfo) == false)
	{
		// 정보를 업데이트 한다. 
		clOldStockInfo.Set(&pclchar->pclCI->clStock.clInfo);

		// 기존의 리스트를 삭제한다. 
		m_pListbox_stock->Clear();

		for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			SI32 villageunique  = pclchar->pclCI->clStock.clInfo.clUnit[i].siVillageUnique;
			SI32 num			= pclchar->pclCI->clStock.clInfo.clUnit[i].siAmount;
			
			if(villageunique)
			{
				pText = GetTxtFromMgr(614);
				StringCchPrintf(buffer, 256, pText, pclClient->pclVillageManager->GetName(villageunique), num);

				tmpListBoxItemData.Init();
				tmpListBoxItemData.Set( 0, buffer );
				m_pListbox_stock->AddItem( &tmpListBoxItemData );
			}
		}
		m_pListbox_stock->Refresh();
	}

	//_LEON_GUILD_STOCK
	// 길드 주식
/*	if(pclchar->pclCI->clGuildStock.clInfo.IsSame(&clOldGuildStockInfo) == false)
	{
		// 정보를 업데이트 한다. 
		clOldGuildStockInfo.Set(&pclchar->pclCI->clGuildStock.clInfo);

		// 기존의 리스트를 삭제한다. 
		m_pListbox_guildstock->Clear();

		for(i = 0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
		{
			SI32 villageunique  = pclchar->pclCI->clGuildStock.clInfo.clUnit[i].siVillageUnique;
			UI08 unitslot	    = pclchar->pclCI->clGuildStock.clInfo.clUnit[i].uiUnitSlot;
			SI32 num			= pclchar->pclCI->clGuildStock.clInfo.clUnit[i].siAmount;
			
			if(villageunique > 0 && villageunique < MAX_VILLAGE_NUMBER )
			{
				if(unitslot >=0 && unitslot < MAX_GUILDUNIT_NUMBER )
				{
                    pText = GetTxtFromMgr(5427);
					StringCchPrintf(buffer, 256, pText, pclClient->pclVillageManager->GetName(villageunique), unitslot+1, num );

					tmpListBoxItemData.Init();
					tmpListBoxItemData.Set( 0, buffer );
					m_pListbox_guildstock->AddItem( &tmpListBoxItemData );
				}
			}
		}
		m_pListbox_guildstock->Refresh();

	}*/

	// 시전 
	m_pListbox_house->Clear();
	for(i = 0;i < MAX_REAL_ESTATE_NUMBER_PER_PERSON;i++)
	{
		SI32 villageunique	= pclchar->pclCI->clRealEstate.clRealEstate[i].siVillageUnique;
		SI32 realestatetype	= pclchar->pclCI->clRealEstate.clRealEstate[i].siType;
		SI32 address		= pclchar->pclCI->clRealEstate.clRealEstate[i].siIndex;


		if(realestatetype == REALESTATE_TYPE_HOUSE)
		{
			pText = GetTxtFromMgr(615);
			StringCchPrintf(buffer, 256, pText, pclClient->pclVillageManager->GetName(villageunique), address + 1);
			tmpListBoxItemData.Init();
			tmpListBoxItemData.Set( 0, buffer );
			m_pListbox_house->AddItem( &tmpListBoxItemData );
		}

		if(realestatetype == REALESTATE_TYPE_LAND)
		{
			pText = GetTxtFromMgr(616);
			StringCchPrintf(buffer, 256, pText, pclClient->pclVillageManager->GetName(villageunique), address + 1);
			tmpListBoxItemData.Init();
			tmpListBoxItemData.Set( 0, buffer );
			m_pListbox_house->AddItem( &tmpListBoxItemData );
		}

		if(realestatetype == REALESTATE_TYPE_MINE)
		{
			pText = GetTxtFromMgr(3239);
			StringCchPrintf(buffer, 256, pText, pclClient->pclVillageManager->GetName(villageunique), address + 1);
			tmpListBoxItemData.Init();
			tmpListBoxItemData.Set( 0, buffer );
			m_pListbox_house->AddItem( &tmpListBoxItemData );
		}

		
	}
	m_pListbox_house->Refresh();
//////////////////////////////////////////////////////////////////////////
	//[추가수정 : 황진성 2007. 10. 12 => 결투장 전적에 정보가 안보이는 것.]
	SI32 WinCnt  = pclclient->pclCM->CR[1]->pclCI->clBI.siPVPWinNumber;
	SI32 LoseCnt = pclclient->pclCM->CR[1]->pclCI->clBI.siPVPLoseNumber;
	TCHAR* Rival = pclclient->pclCM->CR[1]->pclCI->clBI.szPVPWinHighLevelUserName;

	itoa(WinCnt, buffer, 10);
	m_pEdit_battlewin->SetText(buffer);

	itoa(LoseCnt, buffer, 10);
	m_pEdit_defeatscore->SetText(buffer);

	m_pEdit_battlemaxwin->SetText(Rival);
//////////////////////////////////////////////////////////////////////////

	// 길드정보 보여주기 : PCK - 07.10.12
	SI32 sivillageunique = pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siVillageUnique;
	TCHAR* pszGuildName = pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.szGuildName;
	SI08 siGuildRank = pclclient->pclCM->CR[1]->pclCI->clBI.clGuildRank.siType;



	if (siGuildRank == GUILDRANKTYPE_MASTER)
	{
		TCHAR* pRank = GetTxtFromMgr(6540);
		ShowMyGuildInfo(sivillageunique, pszGuildName, pRank);
	}
	else if (siGuildRank == GUILDRANKTYPE_MEMBER)
	{
		TCHAR* pRank = GetTxtFromMgr(6542);
		ShowMyGuildInfo(sivillageunique, pszGuildName, pRank);
	}
	else if (siGuildRank == GUILDRANKTYPE_SECONDMASTER)
	{
		TCHAR* pRank = GetTxtFromMgr(6541);
		ShowMyGuildInfo(sivillageunique, pszGuildName, pRank);
	}
	
	TCHAR strBuf[256] = TEXT("") ;
}

void cltNAssetStatus::Hide()
{
	m_bShow = false;
	CControllerMgr::Show( false );
}

void cltNAssetStatus::Show()
{
	m_bShow = true;
	CControllerMgr::Show( true );
}

bool cltNAssetStatus::IsShow()
{
    return m_bShow;
}

// 상태 정보에 길드 정보를 표시한다.
void cltNAssetStatus::ShowMyGuildInfo( SI32 villageunique, TCHAR* guildname, TCHAR* guildrankname )
{
	//if(villageunique <=0 || villageunique >=MAX_VILLAGE_NUMBER ) return;
    if(guildname == NULL ) 
		m_pEditbox_guildname->SetText( TEXT("") );
	else
		m_pEditbox_guildname->SetText( guildname );
	if(guildrankname == NULL )
		m_pEditbox_guildrank->SetText( TEXT("") );
	else
		m_pEditbox_guildrank->SetText( guildrankname );

	TCHAR* pVillName = NULL;
	pVillName = pclClient->pclVillageManager->GetName( villageunique );

	if( pVillName == NULL )
		m_pEditbox_guildpos->SetText( TEXT("") );
	else
		m_pEditbox_guildpos->SetText( pVillName );
}

//#endif