#include "CommonHeader.h"

#include "../Client/ninterface/NCharTransformDlg/CharTransformDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"
#include "Char\CharManager\CharManager.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../NLib/NTCHARString.h"
#include "../../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/NInstantDungeonDlg/NInstantDungeonBoardDlg.h"
#include "../Client/InterfaceMgr/Interface/List.h"
#include "../Client/Client.h"
#include "../../CommonLogic/Msg/MsgType-Hunt.h"
#include "../../../../NLib/NUtil.h"
#include "../Client/MarkManager/MarkManager.h"



extern cltCommonLogic* pclClient;

NInstantDungeonBoradDlg :: NInstantDungeonBoradDlg()
{
	Initialize();
}

NInstantDungeonBoradDlg ::~NInstantDungeonBoradDlg(){
	Destroy();
}
void NInstantDungeonBoradDlg::Initialize(){
	m_siRequestPage = 1;
	m_siMaxPage = 50;
	m_MaxDisplatPage = MAX_DISPLAY_PAGE_NUM;
}
void NInstantDungeonBoradDlg :: Destroy(){

}
void NInstantDungeonBoradDlg ::Create(){

	if ( IsCreate() )		return;
	CInterfaceFile file;


	file.LoadFile(TEXT("NInterface/Data/NInstantDungeonDlg/NInstantDungeonBoardDlg.ddf"));
	file.CreatePopupDialog( this, NINSTANTDUNGEONBOARD_DLG , TEXT("dialog_instantdungeonBoard"), StaticNInstantDungeonBoradDlgProc );
	//버튼 생성.

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NINSTANTDUNGEONBOARDDLG_BUTTON_RANKING , this ), NINSTANTDUNGEONBOARDDLG_BUTTON_RANKING,   "button_Ranking");
    file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NINSTANTDUNGEONBOARDDLG_BUTTON_HOF , this ), NINSTANTDUNGEONBOARDDLG_BUTTON_HOF,   "button_HOF");
	//명예의 전당 페이지 이동 버튼 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NINSTANTDUNGEONBOARDDLG_BUTTON_PREV , this ), NINSTANTDUNGEONBOARDDLG_BUTTON_PREV,   "button_PREV");//뒤로
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NINSTANTDUNGEONBOARDDLG_BUTTON_NEXTV , this ), NINSTANTDUNGEONBOARDDLG_BUTTON_NEXTV,   "button_NEXTV");//앞으로
	//랭킹페이지표시 텍스트 박스 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NINSTANTDUNGEONBOARDDLG_EDIT_PAGEVIEW , this ), NINSTANTDUNGEONBOARDDLG_EDIT_PAGEVIEW,   "editbox_PageView");
	//랭킹 페이지 이동 버튼 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NINSTANTDUNGEONBOARDDLG_BUTTON_RANKPREV , this ), NINSTANTDUNGEONBOARDDLG_BUTTON_RANKPREV,   "button_RankPREV");//뒤로
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NINSTANTDUNGEONBOARDDLG_BUTTON_RANKNEXTV , this ), NINSTANTDUNGEONBOARDDLG_BUTTON_RANKNEXTV,   "button_RankNEXTV");//앞으로
	
	//리스트 박스 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST , this ), NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST,   "listbox_RHList");
	//기본 적으로는 랭킹을 보여준다.
	CList* pList = m_InterfaceFactory.GetList(NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST);
	TCHAR * pTextRank = GetTxtFromMgr(7843);
	TCHAR * pTextHOF = GetTxtFromMgr(7842);
	CButton* bRank = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKING);
	CButton* bHOF = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_HOF);
	bRank->SetText(pTextRank);
	bHOF->SetText(pTextHOF);
	SetListBoxRanking();//랭킹리스트 박스로 화면을 보여준다.
	//ChangeListBoxRanking();//내용을 일단 보여준다.
}

void CALLBACK NInstantDungeonBoradDlg :: StaticNInstantDungeonBoradDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl){
	NInstantDungeonBoradDlg *pThis = (NInstantDungeonBoradDlg*)pControllerMgr;
	pThis->NInstantDungeonBoradDlgProc(nEvent, nControlID, pControl);
}
void CALLBACK NInstantDungeonBoradDlg :: NInstantDungeonBoradDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl){

	cltClient *pclclient = (cltClient *)pclClient;
	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON :
		{
			DeleteDialog();
		}break;
		//랭킹을 보자.
	case NINSTANTDUNGEONBOARDDLG_BUTTON_RANKING :
		{
			//패이지 변환버튼은 비활성화 시켜줘야함
			//그리고 리스트 박스를 랭킹으로 셋팅.
			//순위는 10등까지
			SetListBoxRanking();
		}break;
		//명예의 전당을 보자
	case NINSTANTDUNGEONBOARDDLG_BUTTON_HOF :
		{
			SetListBoxHOF();

		}break;
		//랭킹 이전 페이지
	case NINSTANTDUNGEONBOARDDLG_BUTTON_RANKPREV :
		{
			cltClient* pclclient = (cltClient*)pclClient;
			UI08 uiYear = pclclient->clClientDate.uiYear;//군주력 
			--m_siRequestPage;
			if( 0 < m_siRequestPage )
			{
				cltGameMsgRequest_RankingList sendMsg( uiYear, m_siRequestPage );//현제 군주력을 날려준다.
				cltMsg clMsg( GAMEMSG_REQUEST_RAKINGLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			else
			{
				m_siRequestPage = 1;
			}
			
		}break;
		//랭킹 다음 페이지
	case NINSTANTDUNGEONBOARDDLG_BUTTON_RANKNEXTV :
		{
			cltClient* pclclient = (cltClient*)pclClient;
			UI08 uiYear = pclclient->clClientDate.uiYear;//군주력
			++m_siRequestPage;

			if( m_siRequestPage <= m_siMaxPage )
			{
				cltGameMsgRequest_RankingList sendMsg( uiYear, m_siRequestPage );//현제 군주력을 날려준다.
				cltMsg clMsg( GAMEMSG_REQUEST_RAKINGLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			else
			{
				m_siRequestPage = m_siMaxPage;
			}

		}break;
		//이전 패이지를 보다
	case NINSTANTDUNGEONBOARDDLG_BUTTON_PREV :
		{
			--m_siRequestPage;
			

			if( 0 < m_siRequestPage )
			{	
				cltGameMsgRequest_HOFList sendMsg(m_siRequestPage );//원하는 페이지만 날려준다
				cltMsg clMsg( GAMEMSG_REQUEST_HOFLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			else
			{
				m_siRequestPage = 1;
			}
		}break;
		//다음 패이지를 보자
	case NINSTANTDUNGEONBOARDDLG_BUTTON_NEXTV :
		{
			++m_siRequestPage;

			if( m_siRequestPage <= m_siMaxPage )
			{
				cltGameMsgRequest_HOFList sendMsg( m_siRequestPage );//원하는 패이지만 날려준다
				cltMsg clMsg( GAMEMSG_REQUEST_HOFLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			else
			{
				m_siRequestPage = m_siMaxPage;
			}
		}break;
	}

}

//랭킹버튼 눌렀을때 리스트 박스 셋팅.
void NInstantDungeonBoradDlg::SetListBoxRanking()
{
	cltClient* pclclient = (cltClient*)pclClient;
	//칼럼 설정.
	CList* pList = m_InterfaceFactory.GetList(NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST);
	//기본 랭킹 버튼을 눌렀을때는 명예의 전당 페이지 이동 버튼이 보여서는 안된다.
	CButton * pPrev = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_PREV);
	CButton * pNextv = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_NEXTV);
	//텍스트 박스도 보이지 않도록 한다.
	CEdit * pPageview = m_InterfaceFactory.GetEdit(NINSTANTDUNGEONBOARDDLG_EDIT_PAGEVIEW);
	//기본 랭킹 버튼을 눌렀을때는 패이지 이동 버튼이 보여서는 안된다.
	CButton * pRankPrev = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKPREV);
	CButton * pRankNextv = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKNEXTV);
	
	pPrev->Show(false);
	pNextv->Show(false);
	pRankPrev->Show(true);
	pRankNextv->Show(true);

    pPageview->Clear();	
	pList->Clear();//리스트 박스를 지워준다.
    if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 5398 );
		pList->SetColumn(0,50 , pText);
		pText = GetTxtFromMgr( 7818 );
		pList->SetColumn(1,55 , pText);
		pText = GetTxtFromMgr( 6580 );
		pList->SetColumn(2, 100, pText);
		pText = GetTxtFromMgr( 7820 );
		pList->SetColumn(3, 70, pText);
		pText = GetTxtFromMgr( 7821 );
		pList->SetColumn(4, 70, pText);
		pText = GetTxtFromMgr( 7822 );
		pList->SetColumn(5, 120, pText);
		
		m_siRequestPage = 1; //최신 첫페이지를 달라고 한다
		//SI32 datevary = pclclient->clClientDate.uiYear.GetDateVary();//군주력
		//UI08 uiYear  = datevary / (12* 30);//군주 년도를 구한다.

		UI08 uiYear = pclclient->clClientDate.uiYear;//군주력 

		cltGameMsgRequest_RankingList sendMsg( uiYear, m_siRequestPage );//현제 군주력을 날려준다.
		cltMsg clMsg( GAMEMSG_REQUEST_RAKINGLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
		pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
		pList->Refresh();


	}
}
//명예의 전당 셋팅
void NInstantDungeonBoradDlg::SetListBoxHOF(){
	//칼럼 설정.
	CList* pList = m_InterfaceFactory.GetList(NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST);
	//명예의 전당일 경우에는 리스트를 패이지 단위로 옮겨야 하므로 버튼이 보이도록 한다.
	CButton * pPrev = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_PREV);
	CButton * pNextv = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_NEXTV);
	//기본 랭킹 버튼을 눌렀을때는 패이지 이동 버튼이 보여서는 안된다.
	CButton * pRankPrev = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKPREV);
	CButton * pRankNextv = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKNEXTV);
	//텍스트 박스도 보이도록 한다.
	CEdit * pPageview = m_InterfaceFactory.GetEdit(NINSTANTDUNGEONBOARDDLG_EDIT_PAGEVIEW);

	m_siRequestPage = 1; //처음 셋팅 할때는 첫 페이지므로.
	
	pPrev->Show(true);
	pNextv->Show(true);
	pRankPrev->Show(false);
	pRankNextv->Show(false);
	
	pPageview->Clear();
	pList->Clear();//리스트 박스를 지워준다.
	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 7823 );
		pList->SetColumn(0, 50, pText);
		pText = GetTxtFromMgr( 7818 );
		pList->SetColumn(1, 55, pText);
		pText = GetTxtFromMgr( 6580 );
		pList->SetColumn(2, 100, pText);
		pText = GetTxtFromMgr( 7820 );
		pList->SetColumn(3, 70, pText);
		pText = GetTxtFromMgr( 7821 );
		pList->SetColumn(4, 70, pText);
		pText = GetTxtFromMgr( 7822 );
		pList->SetColumn(5, 120, pText);
		pList->Refresh();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

		cltGameMsgRequest_HOFList sendMsg( m_siRequestPage );//원하는 페이지만 날려준다
		cltMsg clMsg( GAMEMSG_REQUEST_HOFLIST, sizeof(sendMsg), (BYTE*)&sendMsg );
		pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
		pList->Refresh();
	}
}


//변환 버튼을 누르면 리스트 박스를 바꿔줄 함수들
void NInstantDungeonBoradDlg::ChangeListBoxRanking(HOFList *hoflist, SI16 requestpage, SI16 Maxpage){
	//랭킹을 보여줄때는 아직 그다지 쓸모가 없을듯. 일단 구색은 마춰야 하니 만들어 둠. 차후 다른 기능을 넣을때 쓰겠지?
	CList* pList = m_InterfaceFactory.GetList(NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST);
	CEdit* pEdit = m_InterfaceFactory.GetEdit(NINSTANTDUNGEONBOARDDLG_EDIT_PAGEVIEW);//페이지를 보여주는 에디트 박스 
	CButton* pRankPrevB = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKPREV);//다음페이지
	CButton* pRankNextv = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_RANKNEXTV);//이전 페이지
	TCHAR buffer[128] = {'\0'};	
	pList->Clear();//리스트 박스를 지워준다.
	pEdit->Clear();

	SI32 point = hoflist->siClearPoint;	
	stListBoxItemData tempItemData;

	if (requestpage== Maxpage) {
		pRankNextv->Enable(false);
	}
	else if (requestpage < Maxpage) {
		pRankNextv->Enable(true);
	}
	if (requestpage<=1) {
		pRankPrevB->Enable(false);
	}	
	else if (requestpage>1) {
		pRankPrevB->Enable(true);
	}
	//에디트 박스에 페이지 보여준다.	
	pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	StringCchPrintf( buffer, 128, "%d/%d", requestpage, Maxpage );
	pEdit->SetText( buffer );
	
	for(int Cnt = 0; Cnt < MAX_HOF_RANKCOUNT; ++Cnt)
	{
		tempItemData.Init();
		TCHAR	szBuffer[32] = "";
		//랭킹은 0부터 나오면안되기 때문에 +1 해준다.
		//2 page부터는 11위 부터 나와야 하니까.
		if (requestpage>=2) {
			itoa((requestpage*10-10)+Cnt+1, szBuffer, 10);
		}
		else
		{
			itoa(Cnt+1, szBuffer, 10);
		}

		TCHAR buffer[ 256 ];

		SI32 year = hoflist[Cnt].ClearDateTime.wYear;
		SI32 month = hoflist[Cnt].ClearDateTime.wMonth;
		SI32 day = hoflist[Cnt].ClearDateTime.wDay;
		
		TCHAR * pText = GetTxtFromMgr(1508);
		sprintf( buffer, pText, 
			hoflist[Cnt].ClearDateTime.wYear, hoflist[Cnt].ClearDateTime.wMonth, hoflist[Cnt].ClearDateTime.wDay );	

		SI32  guildicon  = -1 ;
		TSpr* guildspr	= NULL;

		cltClient* pclclient = (cltClient*)pclClient;
		pclclient->m_pMarkManager->GetMarkSprAndFontIndex( hoflist[Cnt].siGuildMarkindex, &guildspr, &guildicon );
				
		SI16 SprNum = -1;
		switch(hoflist[Cnt].siGuildMarkindex / 256)
		{
			case 0: SprNum = GIMG_VILLAGEMARK01;	break;
			case 1: SprNum = GIMG_VILLAGEMARK02;	break;
			case 2: SprNum = GIMG_VILLAGEMARK03;	break;
		}	

		tempItemData.Set( 0,szBuffer);

		//길드 마크가 있을때만 그려준다 
		if(guildicon >= 0)
		{
			tempItemData.SetSmallIconGImg( 1, SprNum, guildicon);
		}		

		tempItemData.Set( 2, hoflist[Cnt].szHOFGuildName);
		tempItemData.Set( 3, SI32ToString(hoflist[Cnt].siClearPoint));
		TCHAR szClearTime[128];
		TCHAR *pszText = GetTxtFromMgr(1252);
		StringCchPrintf(szClearTime, sizeof(szClearTime), pszText, hoflist[Cnt].ClearTime / (1000 * 60) );
		tempItemData.Set( 4, szClearTime );
		tempItemData.Set( 5, buffer);
		pList->AddItem( &tempItemData );
		pList->Refresh();
	}
}
//
void NInstantDungeonBoradDlg::ChangeListBoxHOF(HOFList *hoflist, SI16 requestpage, SI16 Maxpage){
	CList* pList = m_InterfaceFactory.GetList(NINSTANTDUNGEONBOARDDLG_LISTBOX_RHLIST);
	CEdit* pEdit = m_InterfaceFactory.GetEdit(NINSTANTDUNGEONBOARDDLG_EDIT_PAGEVIEW);//페이지를 보여주는 에디트 박스 
	CButton* pPrevB = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_PREV);//다음페이지
	CButton* pNextv = m_InterfaceFactory.GetButton(NINSTANTDUNGEONBOARDDLG_BUTTON_NEXTV);//이전 페이지
	pList->Clear();//리스트 박스를 지워준다.
	pEdit->Clear();
	TCHAR buffer[128] = {'\0'};
	SI32 point = hoflist->siClearPoint;	
	stListBoxItemData tempItemData;
	
	if (requestpage== Maxpage) {
		pNextv->Enable(false);
	}
	else if (requestpage < Maxpage) {
		pNextv->Enable(true);
	}
	if (requestpage<=1) {
		pPrevB->Enable(false);
	}	
	else if (requestpage>1) {
		pPrevB->Enable(true);
	}
	//에디트 박스에 페이지 보여준다.	
	pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	StringCchPrintf( buffer, 128, "%d/%d", requestpage, Maxpage );
	pEdit->SetText( buffer );
	
	for(int Cnt = 0; Cnt < MAX_HOF_RANKCOUNT; ++Cnt)
	{
		tempItemData.Init();
		//더이상 데이터가 없으면 그려주지 않는다.
		if( hoflist[Cnt].uiYear <= 0 )		continue;

		TCHAR * pText = GetTxtFromMgr(1508);
		sprintf( buffer, pText, 
			hoflist[Cnt].ClearDateTime.wYear, hoflist[Cnt].ClearDateTime.wMonth, hoflist[Cnt].ClearDateTime.wDay );	

		SI32  guildicon  = -1 ;
		TSpr* guildspr	= NULL;

		cltClient* pclclient = (cltClient*)pclClient;
		pclclient->m_pMarkManager->GetMarkSprAndFontIndex( hoflist[Cnt].siGuildMarkindex, &guildspr, &guildicon );


		SI16 SprNum = -1;
		switch(hoflist[Cnt].siGuildMarkindex / 256)
		{
			case 0: SprNum = GIMG_VILLAGEMARK01;	break;
			case 1: SprNum = GIMG_VILLAGEMARK02;	break;
			case 2: SprNum = GIMG_VILLAGEMARK03;	break;
		}	

		//길드 마크가 있을때만 그려준다 
		if(guildicon >= 0)
		{
			tempItemData.SetSmallIconGImg( 1, SprNum, guildicon);
		}

		tempItemData.Set( 0,UI08ToString(hoflist[Cnt].uiYear));
		tempItemData.Set( 2, hoflist[Cnt].szHOFGuildName);
		tempItemData.Set( 3, SI32ToString(hoflist[Cnt].siClearPoint) );
		TCHAR szClearTime[128];
		TCHAR *pszText = GetTxtFromMgr(1252);
		StringCchPrintf(szClearTime, sizeof(szClearTime), pszText, hoflist[Cnt].ClearTime / (1000 * 60) );
		tempItemData.Set( 4, szClearTime );
		tempItemData.Set( 5, buffer);
		pList->AddItem( &tempItemData );
		pList->Refresh();
	}
}
