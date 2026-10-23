#include "NPeriodItemListDlg.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../Client/Client.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "NDate.h"


extern cltCommonLogic* pclClient;

CNPeriodItemListDlg::CNPeriodItemListDlg()
{	
	Initialize();
}

CNPeriodItemListDlg::~CNPeriodItemListDlg()
{
	Destroy();
}


void CNPeriodItemListDlg::Initialize()
{
	for(int i = 0; i < PERIOD_TAB_END; i++)
	{
		m_pButton_PeriodItem[i]	=	NULL;

		m_vecPeriodItem[i].clear();
	}

	m_siCurrentPage		=	1;
	m_siMaxPage			=	0;
	m_siCurrentTabIndex	=	PERIOD_TAB_CHARACTER;


	for( SI16 i = 0 ; i < MAX_ITEMMALL_ITEM_PER_PAGE; i++ )
	{
		m_siImageType[i] = -1;
		m_siImageIndex[i] = -1;
	}
}

void CNPeriodItemListDlg::Destroy()
{
	SetTimerOnOff(false);

	for(int i = 0; i < PERIOD_TAB_END; i++)
		SAFE_DELETE(m_pButton_PeriodItem[i]);

	if(m_BuffImg.pImage)
		GP.FreeSpr(m_BuffImg);
}

void CNPeriodItemListDlg::Create()
{
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NPeriodItemListDlg/DLG_NPeriodItemList.ddf"));
		file.CreatePopupDialog( this, NPERIODITEMLIST_DLG , TEXT("dialog_PeriodItemList"), StaticNPeriodItemListDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NPERIODITEMLIST_STATIC_1 , this ), NPERIODITEMLIST_STATIC_1 ,   "statictext_NONAME1"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NPERIODITEMLIST_STATIC_2 , this ), NPERIODITEMLIST_STATIC_2 ,   "statictext_NONAME2"); 		

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NPERIODITEMLIST_IMAGE_1, this ), NPERIODITEMLIST_IMAGE_1, "imagestatic_NONAME1");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NPERIODITEMLIST_IMAGE_2, this ), NPERIODITEMLIST_IMAGE_2, "imagestatic_NONAME2");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NPERIODITEMLIST_IMAGE_3, this ), NPERIODITEMLIST_IMAGE_3, "imagestatic_NONAME3");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NPERIODITEMLIST_IMAGE_4, this ), NPERIODITEMLIST_IMAGE_4, "imagestatic_NONAME4");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NPERIODITEMLIST_IMAGE_5, this ), NPERIODITEMLIST_IMAGE_5, "imagestatic_NONAME5");

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NPERIODITEMLIST_EDIT_1 , this ), NPERIODITEMLIST_EDIT_1,   "editbox_NONAME1");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NPERIODITEMLIST_EDIT_2 , this ), NPERIODITEMLIST_EDIT_2,   "editbox_NONAME2");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NPERIODITEMLIST_EDIT_3 , this ), NPERIODITEMLIST_EDIT_3,   "editbox_NONAME3");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NPERIODITEMLIST_EDIT_4 , this ), NPERIODITEMLIST_EDIT_4,   "editbox_NONAME4");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NPERIODITEMLIST_EDIT_5 , this ), NPERIODITEMLIST_EDIT_5,   "editbox_NONAME5");

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NPERIODITEMLIST_BUTTON_PRE, this ), NPERIODITEMLIST_BUTTON_PRE,   "button_NONAME1"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NPERIODITEMLIST_BUTTON_NEXT, this ), NPERIODITEMLIST_BUTTON_NEXT,   "button_NONAME2"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NPERIODITEMLIST_STATIC_PAGE , this ), NPERIODITEMLIST_STATIC_PAGE ,   "statictext_NONAME3"); 		

		m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_1)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_2)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_3)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_4)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_5)->ShowScrollBar(false);
		
		MovePos(250,105);


		//	090326	[종호]탭 버튼 추가
		for(int i = 0; i < PERIOD_TAB_END; i++)
			NEWCBUTTON(m_pButton_PeriodItem[i]);

		file.CreateControl( m_pButton_PeriodItem[PERIOD_TAB_CHARACTER], NPERIODITEMLIST_BUTTON_CHARACTER, TEXT("button_CHARACTER"));
		file.CreateControl( m_pButton_PeriodItem[PERIOD_TAB_BUFF], NPERIODITEMLIST_BUTTON_HORSE, TEXT("button_BUFF"));
		file.CreateControl( m_pButton_PeriodItem[PERIOD_TAB_HORSE], NPERIODITEMLIST_BUTTON_SUMMON, TEXT("button_HORSE"));

		TCHAR* pText = GetTxtFromMgr(10310);
		m_pButton_PeriodItem[PERIOD_TAB_CHARACTER]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(10311);
		m_pButton_PeriodItem[PERIOD_TAB_BUFF]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pText = GetTxtFromMgr(372);
		m_pButton_PeriodItem[PERIOD_TAB_HORSE]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		// 타이머를 설정한다. 
		SetActionTimer( 1000 );
		SetTimerOnOff( true );  

		//	090326 [종호]	탭 버튼 상태 설정
		SelectedTabBtn();

		GP.LoadSprFromFile(TEXT("GIMG\\BufImage.SPR"),&m_BuffImg);
			
		/*if(m_BuffImg.GetImageNum() > 0)
		{
			for (SI16 i = 0; i < m_BuffImg.GetImageNum(); i++) 
			{
				m_BuffImageStatic[i] = new CImageStatic(this);
				if ( m_BuffImageStatic[i] == NULL)
					continue;

				m_BuffImageStatic[i]->Create(	NBUFFSTATUS_IMAGESTATIC_PREMIUM_0,TEXT("GIMG/BUFIMAGE.SPR"),
												i,
												false,
												0,
												0,
												0,
												m_BuffImg.GetXSize(),
												m_BuffImg.GetYSize(),
												m_BuffImg.GetXSize(),
												m_BuffImg.GetYSize());

				m_BuffImageStatic[i]->Show(false);
			}
		}*/

		SetPeriodItemList();		
		ShowPeriodItemList();		
	}
	else
	{
        DeleteDialog();

	}
}

void CALLBACK CNPeriodItemListDlg::StaticNPeriodItemListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPeriodItemListDlg *pThis = (CNPeriodItemListDlg*) pControllerMgr;
	pThis->NPeriodItemListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPeriodItemListDlg::NPeriodItemListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	stTabTextInfo tmp_stTabTextInfo;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NPERIODITEMLIST_BUTTON_PRE:
		{
			if(nEvent == EVENT_BUTTON_CLICKED)
			{
				m_siCurrentPage--;
				m_siCurrentPage = max(m_siCurrentPage,1);
				ShowPeriodItemList();
			}
		}
		break;

	case NPERIODITEMLIST_BUTTON_NEXT:
		{
			if(nEvent == EVENT_BUTTON_CLICKED)
			{
				m_siCurrentPage++;		
				ShowPeriodItemList();
			}
		}
		break;

	case NPERIODITEMLIST_BUTTON_CHARACTER:
		{
			if(nEvent == EVENT_BUTTON_CLICKED)
			{
				m_siCurrentTabIndex = PERIOD_TAB_CHARACTER;

				SelectedTabBtn();
				ShowPeriodItemList();
			}
		}
		break;

	case NPERIODITEMLIST_BUTTON_HORSE:
		{
			if(nEvent == EVENT_BUTTON_CLICKED)
			{
				pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_BUFFCLICK );
				m_siCurrentTabIndex = PERIOD_TAB_BUFF;
				SelectedTabBtn();
				ShowPeriodItemList();
			}
		}
		break;

	case NPERIODITEMLIST_BUTTON_SUMMON:
		{
			if(nEvent == EVENT_BUTTON_CLICKED)
			{
				m_siCurrentTabIndex = PERIOD_TAB_HORSE;

				SelectedTabBtn();
				ShowPeriodItemList();
			}
		}
		break;
	}

}

void CNPeriodItemListDlg::Action()
{
	SetPeriodItemList();		
	ShowPeriodItemList();		
}


void CNPeriodItemListDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	CImageStatic*	pImageStatic[MAX_ITEMMALL_ITEM_PER_PAGE];

	for(int i = 0; i < MAX_ITEMMALL_ITEM_PER_PAGE; i++)
	{
		pImageStatic[i]	=	m_InterfaceFactory.GetImageStatic( NPERIODITEMLIST_IMAGE_1 + i );
		_ASSERT(pImageStatic[i] != NULL);
	}	


	cltClient *pclclient = ( cltClient * )pclClient;

	for( SI32 i = 0 ; i < MAX_ITEMMALL_ITEM_PER_PAGE; i++ )
	{
		//	아이콘이 아이템 아이콘 인지 버프 아이콘인지 판별해서 따로 그려준다.
		switch(m_siCurrentTabIndex)
		{
		case PERIOD_TAB_CHARACTER:
		case PERIOD_TAB_HORSE:
			{
				if(m_siImageIndex[i] < 0 || m_siImageType[i] < 0)   
					continue;

				TSpr *pSpr =  pclclient->pclGImg->GetGlobalSpr( m_siImageType[i] );

				if ( pSpr )
				{
					GP.PutSpr(	pSpr ,
								ScreenRX +  GetX() + pImageStatic[i]->GetX() + 10,
								ScreenRY +  GetY() + pImageStatic[i]->GetY(),
								m_siImageIndex[i] );
				}
			}
			break;

		case PERIOD_TAB_BUFF:
			{
				SI32 siIndex = ((m_siCurrentPage - 1) * MAX_ITEMMALL_ITEM_PER_PAGE) + i;

				if((SI32)m_vecPeriodItem[PERIOD_TAB_BUFF].size() > siIndex)
				{
					SI32 siItemUnique = m_vecPeriodItem[PERIOD_TAB_BUFF].at(siIndex).siRefIndex;
					GP.PutSpr(	&m_BuffImg,
								ScreenRX +  GetX() + pImageStatic[i]->GetX() + 10, 
								ScreenRY +  GetY() + pImageStatic[i]->GetY(),
								pclclient->pclGImg->GetBufImageFontIndex(siItemUnique));	// SPR 상의 버프 인덱스와 코드상 정의되어있는 버프 인덱스가 일치하지 않으므로  GetBufImageFontIndex 함수를  통해 인덱스를 맞춘다.
					//m_BuffImageStatic[pclclient->pclGImg->GetBufImageFontIndex(siItemUnique)]->DrawController(ScreenRX +  GetX() + pImageStatic[i]->GetX() + 10, ScreenRY +  GetY() + pImageStatic[i]->GetY());
				}
			}
			break;
		
		default:
			_ASSERT(pImageStatic[i] != NULL);
			break;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	090327 [종호]		버프 남은 기간 설정
//	siBuffIndex		:	버프 인덱스	(버프라면 버프의 인덱스가 들어간다. PremiumService등의 버프이외의 효과라면 -1이 들어감)
//	siTimeVary		:	남은 시간d]
//	Return Value	:	남은시간데이터인 cltDate형으로 리턴
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
cltDate CNPeriodItemListDlg::SetPeriodItemTime(bool bIsBuff, SI32 siTimeVary)
{
	cltDate cldate;

	if(bIsBuff == true)								//	버프라면 MakeFullDateFromDateVary 사용
		cldate.MakeFullHourFromHourVary(siTimeVary);
	else												//	아니라면 MakeFullHourFromHourVary 사용
    	cldate.MakeFullDateFromDateVary(siTimeVary);

	return cldate;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	090326[종호]		기간제리스트 슬롯에 들어갈 Info를 설정
//
//	pchDataInfo		:	데이터를 컨테이너(vector)에 넣을때 필요한 data info (남은기간 표시등의 문자열들)
//	siTimeVary		:	남은시간 설정에 필요한 data
//	siTextFromMgr	:	TextMgr Index
//	siBuffPercent	:	Buff중 몇몇 스탯 버프등 몇몇 버프만 쓰이는 data
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNPeriodItemListDlg::SetPeriodItemInfo(cltDate pcldate, TCHAR* pchDataInfo, SI32 siTextFromMgr, SI32 siBuffPercent)
{
	TCHAR	timebuf[512]		= TEXT("");
	
	if(NULL == pchDataInfo)
		return;

	if ( pcldate.GetDateText(timebuf) )
	{
		NTCHARString128 kText( GetTxtFromMgr(siTextFromMgr) );
		
		if(siBuffPercent != 0)	
		{
			kText.Replace(TEXT("#value#"),SI32ToString(siBuffPercent));
			kText.Replace(TEXT("#period#"),timebuf);
			StringCchCopy(pchDataInfo, 2048, kText);
		}
		else
		{
			kText.Replace(TEXT("#period#"),timebuf);
			StringCchCopy(pchDataInfo, 2048, kText);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	090327 [종호]		기간제 List에 Data set
//	DataIndex		:	버프의 인덱스나 아이템Unique의 인덱스
//	siKindOfList	:	들어갈 리스트의 종류
//	pchDataInfo		:	들어갈 info
//	siTime			:	남은시간
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CNPeriodItemListDlg::InputData(SI32 DataIndex, SI32 siKindOfList, TCHAR* pchDataInfo, SI32 siTime)
{
	sPeriodItem sItem;
	ZeroMemory(&sItem, sizeof(sPeriodItem));
	sItem.siRefIndex		=	ITEMUNIQUE(DataIndex);	
	sItem.strPeriodItemInfo	=	pchDataInfo;	
	sItem.siPeriodTime		=	siTime;	

	m_vecPeriodItem[siKindOfList].push_back(sItem);		
}



//	090326 [종호] 기간제리스트 슬롯을 set
void CNPeriodItemListDlg::SetPeriodItemList()
{
	for(int i = 0; i < PERIOD_TAB_END; i++)
		m_vecPeriodItem[i].clear();

	cltDate cldate;							//	버프및서비스 남은시간
	TCHAR	chDataInfo[2048] = TEXT("");	//	버프정보들


	cltClient* pClient = (cltClient*)pclClient;

	//경험치 150%포션 기간표시
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMEXPUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_PREMIUMEXPUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

        cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_PREMIUMEXPUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);																						
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());																		
	}

	//경험치 200%포션 기간표시
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUPEREXPUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_SUPEREXPUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SUPEREXPUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);																						
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}
	
	//경험치 300%포션 기간표시(스페셜 엠플)
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPECIALAMPUL_EXPUP300))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_SPECIALAMPUL_EXPUP300);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SPECIALAMPUL_EXPUP300));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);																						
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//채광 농경용 아이템
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_FARMINGNMININGFOREVER);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_FARMINGNMININGFOREVER));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);																						
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//공간상자.
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bSpaceBoxPremiumService)
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_PremiumService_SpaceBox);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siSpaceBoxPremiumServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//스텟 적용 버프의 남은 시간표시
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_STR))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_STATUE_UP_STR);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_STR));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_STR)->siPercent);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	스텟 DEX		
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_DEX))
	{	
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_STATUE_UP_DEX);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_DEX));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_DEX)->siPercent);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}
	
	//	스텟 MAG
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_MAG))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_STATUE_UP_MAG);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_MAG));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_MAG)->siPercent);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	// 스텟 Wis
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_WIS))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_STATUE_UP_WIS);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_WIS));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_WIS)->siPercent);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	스텟 VIT
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_VIT))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_STATUE_UP_VIT);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_VIT));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_VIT)->siPercent);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	스텟 HAND
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_STATUE_UP_HAND))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_STATUE_UP_HAND);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_STATUE_UP_HAND));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_STATUE_UP_HAND)->siPercent);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}
	
	//	경힘치 UP
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_EXP_UP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_EXP_UP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_EXP_UP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBuf(BUF_EXP_UP)->siPercent+100);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	PremiumService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bPremiumService )
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_PremiumService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siPremiumServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	CombatService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bCombatService )
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_CombatService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siCombatServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	MakeItemService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bMakeItemService )
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_MakeItemService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siMarKetPremiumServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	BulletService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bBulletService )
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BulletService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siBulletServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	SummonStaminaService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bSummonStaminaService )
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_SummonStaminaService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siSummonStaminaServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	MarketPremiumService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bMarKetPremiumService )
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_MarKetPremiumService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siMarKetPremiumServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	//	NewMarketPremiumService
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bNewMarketPremiumService )		//	아직 통합 마켓이 없으므로 그냥 마켓프리미엄 text쓴다.
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_NewMarketPremiumService);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siNewMarketPremiumServiceDate);	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	// PCK : 제조 프리미엄 기간 표시
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_PREMIUMMAKEEXPUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_PREMIUMMAKEEXPUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_PREMIUMMAKEEXPUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
	}
		

	//	BUF탭에 표시될 버프들

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_ATTACKUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_ATTACKUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_ATTACKUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_DEFENSEUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_DEFENSEUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_DEFENSEUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}
		
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SPEEDUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_SPEEDUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SPEEDUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MAKEEXPUP))
	{
		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_MAKEEXPUP));	

		SI32 buffpercent = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent(BUF_MAKEEXPUP);

		if (buffpercent == 20)										//	Buff Percent가 20 이라면 제조경험치가 20%
		{
			sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_MAKEEXPUP_20);

			if(pData == NULL)
				assert(0);

			SI32 siTabIndex = pData->GetTabIndex();
			SI32 siIconIndex = pData->GetIconIndex();
			SI32 siTextIndex = pData->GetTextIndex();

			SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);				
			InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
		}
		else														//	아니라면 제조경험치가 150%
		{
			sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_MAKEEXPUP_150);

			if(pData == NULL)
				assert(0);

			SI32 siTabIndex = pData->GetTabIndex();
			SI32 siIconIndex = pData->GetIconIndex();
			SI32 siTextIndex = pData->GetTextIndex();

			SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
			InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
		}
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_NOFIRSTATTACK);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_NOFIRSTATTACK));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUMMONATACUP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_SUMMONATACUP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SUMMONATACUP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_LOVEPOWER);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_LOVEPOWER));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_FRIENDSHIP);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_FRIENDSHIP));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_HINAMOCHI);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_HINAMOCHI));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_SUCCESS_MANUFACTURE))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_SUCCESS_MANUFACTURE);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_SUCCESS_MANUFACTURE));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_MOFUMOFU_PERFUME))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_MOFUMOFU_PERFUME);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_MOFUMOFU_PERFUME));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_FOURLEAF))
	{	
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_FOURLEAF);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_FOURLEAF));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_HAPPYBEAN))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_HAPPYBEAN);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_HAPPYBEAN));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_ATTACKUP30))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_ATTACKUP30DAY);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_ATTACKUP30));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_DEFENSEUP30))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_DEFENSEUP30DAY);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_DEFENSEUP30));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_THANKSGIVING_DAY))
	{	
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_THANKSGIVING_DAY);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_THANKSGIVING_DAY));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_WATERMELON))
	{	
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_WATERMELON);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_WATERMELON));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_CHRISTMASEVENT_SNOWBLESS))
	{	
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_CHRISTMASEVENT_SNOWBLESS);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_CHRISTMASEVENT_SNOWBLESS));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_CHRISTMASEVENT_DECORATION))
	{	
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_CHRISTMASEVENT_DECORATION);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_CHRISTMASEVENT_DECORATION));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	// 마을 EXP 캡슐
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_RESIDENTS_EXPUP_10))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_RESIDENTS_EXPUP_10);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_RESIDENTS_EXPUP_10));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	// 마을 강화제
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_RESIDENTS_STRONG))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_RESIDENTS_STRONG);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_RESIDENTS_STRONG));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}

	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_VALENTINEDAY))
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_VALENTINE);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();

		cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_VALENTINEDAY));	
		SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);			
		InputData(siIconIndex, siTabIndex, chDataInfo, cldate.GetDateVary());
	}


	//	무한워프인가?
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp)
	{
		sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_UseNoLimitTicketWarp);

		if(pData == NULL)
			assert(0);

		SI32 siTabIndex = pData->GetTabIndex();
		SI32 siIconIndex = pData->GetIconIndex();
		SI32 siTextIndex = pData->GetTextIndex();
		

		//무한 워프 기간 표시.

		cltDate MaxWarpDate;//무한 티켓 날자 최대 셋팅
		MaxWarpDate.uiYear = 255;
		MaxWarpDate.uiMonth = 12;
		MaxWarpDate.uiDay = 30;

		SI32 MAX_GAME_DATE = MaxWarpDate.GetDateVary();
		SI32 warpdate = pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary;//현제 워프가능 기한 

		if (warpdate >=MAX_GAME_DATE) 
		{
			TCHAR* timebuf= GetTxtFromMgr(7866);
			TCHAR* pText = GetTxtFromMgr(siTextIndex);
			StringCchPrintf(chDataInfo, 2048, pText,timebuf);
			InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, MaxWarpDate.GetDateVary());
		}
		else
		{
			TCHAR timebuf[512] = TEXT("") ;
			cltDate cldate ;
			cldate.MakeFullDateFromDateVary(pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary);
			if ( cldate.GetDateText(timebuf) )
			{
				cldate	=	SetPeriodItemTime(false, pclClient->pclCM->CR[1]->pclCI->clBI.siNoLimitTicketWarpDateVary);	
				SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);
				InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
			}
		}
	}

	//	메달 시간표시
	if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL))
	{
		SI16 bufpersent = pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufPercent( BUF_OLYMPICMEDAL);
		if (bufpersent == 20) 
		{
            if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL))		//	금메달
			{
				sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_OLYMPICMEDAL_GOLD);

				if(pData == NULL)
					assert(0);

				SI32 siTabIndex = pData->GetTabIndex();
				SI32 siIconIndex = pData->GetIconIndex();
				SI32 siTextIndex = pData->GetTextIndex();

				cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_OLYMPICMEDAL));	
				SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);
				InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
			}
		}

		else if (bufpersent == 10) 
		{
			if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL))		//	은메달
			{
				sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_OLYMPICMEDAL_SILVER);

				if(pData == NULL)
					assert(0);

				SI32 siTabIndex = pData->GetTabIndex();
				SI32 siIconIndex = pData->GetIconIndex();
				SI32 siTextIndex = pData->GetTextIndex();

				cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_OLYMPICMEDAL));	
				SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);
				InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
			}
		}

		else if (bufpersent == 5) 
		{
			if (pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf(BUF_OLYMPICMEDAL))		//	동메달
			{
				sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BUF_OLYMPICMEDAL_BROLZE);

				if(pData == NULL)
					assert(0);

				SI32 siTabIndex = pData->GetTabIndex();
				SI32 siIconIndex = pData->GetIconIndex();
				SI32 siTextIndex = pData->GetTextIndex();

				cldate	=	SetPeriodItemTime(true, pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.GetBufOvertime(BUF_OLYMPICMEDAL));	
				SetPeriodItemInfo(cldate, chDataInfo, siTextIndex);
				InputData(ITEMUNIQUE(siIconIndex), siTabIndex, chDataInfo, cldate.GetDateVary());
			}
		}
	}

	// 말의 추가능력의 제한날자랑 요구사항을 표시한다.
	TCHAR text[2048]=TEXT("");
	TCHAR ticketAdddate[256] = TEXT("");
	TCHAR needtext[256] = TEXT("");
	TCHAR tempbuffer_slot1[256] = TEXT("");
	TCHAR tempbuffer_slot2[256] = TEXT("");
	TCHAR tempbuffer_slot3[256] = TEXT("");

	cltHorse* pclhorse = pclClient->pclCM->CR[1]->pclCI->clHorseInfo.GetCurrentShowHorse();

	cltClient* pclclient = (cltClient*)pclClient;
	
    sItemPeriodObj* pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_RIDE_ADDSTATUS);
	pclClient->pclHorseManager->GetAddStatusLimitDateText(pclhorse,ticketAdddate, sizeof(ticketAdddate), pData->GetTextIndex());

	//	마법의 탈것 강화제
	if(strcmp(ticketAdddate, TEXT("")) != 0)
	{
		sPeriodItem sItem;
		ZeroMemory(&sItem, sizeof(sPeriodItem));
		sItem.siRefIndex		=	pData->GetIconIndex();			
		sItem.strPeriodItemInfo	=	ticketAdddate;	

		m_vecPeriodItem[PERIOD_TAB_HORSE].push_back(sItem);
	}

	if(pclhorse->siNeed)
		pclClient->pclHorseManager->GetNeedText(pclhorse, needtext, sizeof(needtext));

	
	if( NULL != pclhorse &&	pclhorse->GetPremiumPartsNum() > 0)
	{
		if(pclhorse->GetPremiumUnique_Slot1() > 0)
		{
			sItemPeriodObj* pData = NULL;

			switch(pclhorse->GetPremiumUnique_Slot1())
			{
			case ITEMUNIQUE(13635):	// 순발력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDDEX);
				break;

			case ITEMUNIQUE(13638):	// 순발력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDDEX);
				break;

			case ITEMUNIQUE(13636):	// 근력강화
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDSTR);
				break;

			case ITEMUNIQUE(13639):	// 근력강화
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDSTR);
				break;

			case ITEMUNIQUE(13637):	// 지력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDMAG);
				break;

			case ITEMUNIQUE(13640):	// 지력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDMAG);
				break;

			default:
				assert(0);
				break;
			}

			TCHAR buffer[256] = TEXT("");
			SI32 silimitdate = 0 ;
			
			silimitdate = pclhorse->clPremiumPartsDate_Slot1.GetDateVary() + MAX_HORSE_ADDSTATUS_LIMITDATE;

			if(pclclient->clClientDate.GetDateVary() <= silimitdate) 
			{
				cltDate cldate;
				cldate.MakeFullDateFromDateVary(silimitdate);

				if (cldate.GetDateText( buffer ) == TRUE)
				{
					NTCHARString256 pText = GetTxtFromMgr(pData->GetTextIndex());
					pText.Replace( TEXT("#period#"),buffer );

					sPeriodItem sItem;
					ZeroMemory(&sItem, sizeof(sPeriodItem));
					sItem.siRefIndex		=	ITEMUNIQUE(pData->GetIconIndex());			
					sItem.strPeriodItemInfo	=	pText;	

					m_vecPeriodItem[PERIOD_TAB_HORSE].push_back(sItem);
				}
			}

		}
		if(pclhorse->GetPremiumUnique_Slot2() > 0)
		{
			sItemPeriodObj* pData = NULL;

			switch(pclhorse->GetPremiumUnique_Slot2())
			{
			case ITEMUNIQUE(13635):	// 순발력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDDEX);
				break;

			case ITEMUNIQUE(13638):	// 순발력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDDEX);
				break;

			case ITEMUNIQUE(13636):	// 근력강화
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDSTR);
				break;

			case ITEMUNIQUE(13639):	// 근력강화
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDSTR);
				break;

			case ITEMUNIQUE(13637):	// 지력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDMAG);
				break;

			case ITEMUNIQUE(13640):	// 지력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDMAG);
				break;

			default:
				assert(0);
				break;
			}

			TCHAR buffer[256] = TEXT("") ;
			SI32 silimitdate = 0 ;

			silimitdate = pclhorse->clPremiumPartsDate_Slot2.GetDateVary() + MAX_HORSE_ADDSTATUS_LIMITDATE;

			if(pclclient->clClientDate.GetDateVary() <= silimitdate) 
			{
				cltDate cldate;
				cldate.MakeFullDateFromDateVary(silimitdate);

				if (cldate.GetDateText( buffer ) == TRUE)
				{
					NTCHARString256 pText = GetTxtFromMgr(pData->GetTextIndex());
					pText.Replace( TEXT("#period#"),buffer );

					sPeriodItem sItem;
					ZeroMemory(&sItem, sizeof(sPeriodItem));
					sItem.siRefIndex		=	ITEMUNIQUE(pData->GetIconIndex());			
					sItem.strPeriodItemInfo	=	pText;	

					m_vecPeriodItem[PERIOD_TAB_HORSE].push_back(sItem);
				}
			}
		}
		if(pclhorse->GetPremiumUnique_Slot3() > 0)
		{
			sItemPeriodObj* pData = NULL;

			switch(pclhorse->GetPremiumUnique_Slot3())
			{
			case ITEMUNIQUE(13635):	// 순발력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDDEX);
				break;

			case ITEMUNIQUE(13638):	// 순발력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDDEX);
				break;

			case ITEMUNIQUE(13636):	// 근력강화
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDSTR);
				break;

			case ITEMUNIQUE(13639):	// 근력강화
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDSTR);
				break;

			case ITEMUNIQUE(13637):	// 지력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_BIKE_ADDMAG);
				break;

			case ITEMUNIQUE(13640):	// 지력
				pData = pClient->m_pItemPeriodMgr->GetBuffData(BuffIndex_HORSE_ADDMAG);
				break;

			default:
				assert(0);
				break;
			}

			TCHAR buffer[256] = TEXT("") ;
			SI32 silimitdate = 0 ;

			silimitdate = pclhorse->clPremiumPartsDate_Slot3.GetDateVary() + MAX_HORSE_ADDSTATUS_LIMITDATE;

			if(pclclient->clClientDate.GetDateVary() <= silimitdate) 
			{
				cltDate cldate;
				cldate.MakeFullDateFromDateVary(silimitdate);

				if (cldate.GetDateText( buffer ) == TRUE)
				{
					NTCHARString256 pText = GetTxtFromMgr(pData->GetTextIndex());
					pText.Replace( TEXT("#period#"),buffer );

					sPeriodItem sItem;
					ZeroMemory(&sItem, sizeof(sPeriodItem));
					sItem.siRefIndex		=	ITEMUNIQUE(pData->GetIconIndex());			
					sItem.strPeriodItemInfo	=	pText;	

					m_vecPeriodItem[PERIOD_TAB_HORSE].push_back(sItem);
				}
			}
		}
	}



	//	각 리스트 남은 기간 순으로 Sort
	for(int i = 0; i < PERIOD_TAB_END; i++)
	{
		sort(m_vecPeriodItem[i].begin(), m_vecPeriodItem[i].end(), CompareForPeriodItem);
	}
}


void CNPeriodItemListDlg::ShowPeriodItemList()
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 외부 컨트롤 초기화

	for(int i = 0; i < MAX_ITEMMALL_ITEM_PER_PAGE; i++)
	{
		m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_1 + i)->Clear();

		m_siImageIndex[i] = -1;        
		m_siImageType[i] = -1;
	}

	// 리스트의 크기와 페이지 크기 측정 및 현제 페이지 설정
	if( m_vecPeriodItem[m_siCurrentTabIndex].empty()) 
		return;

	m_siMaxPage = (m_vecPeriodItem[m_siCurrentTabIndex].size() - 1) / MAX_ITEMMALL_ITEM_PER_PAGE + 1;

	m_siCurrentPage = min( m_siCurrentPage , m_siMaxPage );

	TCHAR strBuffer[256] = {'\0',};
	StringCchPrintf(strBuffer,256,TEXT("%d/%d"),m_siCurrentPage,m_siMaxPage);

	m_InterfaceFactory.GetStatic( NPERIODITEMLIST_STATIC_PAGE )->SetText(strBuffer);

	SI32 siItemUnique;
	string strItemInfo;

	for(int nCount = 0; nCount < MAX_ITEMMALL_ITEM_PER_PAGE; nCount++)
	{	
		SI32 siIndex = ((m_siCurrentPage - 1) * MAX_ITEMMALL_ITEM_PER_PAGE) + nCount;

		if((SI32)m_vecPeriodItem[m_siCurrentTabIndex].size() > siIndex)
		{
			siItemUnique = m_vecPeriodItem[m_siCurrentTabIndex].at(siIndex).siRefIndex;
			strItemInfo = m_vecPeriodItem[m_siCurrentTabIndex].at(siIndex).strPeriodItemInfo;

			// 보여줄 버프 정보 설정
			m_InterfaceFactory.GetEdit(NPERIODITEMLIST_EDIT_1 + nCount )->SetText((TCHAR*)strItemInfo.c_str());

			switch(m_siCurrentTabIndex)
			{
			case PERIOD_TAB_CHARACTER:
			case PERIOD_TAB_HORSE:
				{
					if(FALSE == pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique , &m_siImageType[nCount], &m_siImageIndex[nCount] ))
					{
						_ASSERT(0);
						pclClient->pclLog->LogPrint("[nPeriodItemListDlg.cpp]	아이템 이미지를 찾을 수 없습니다.");
					}
				}
				break;

			case PERIOD_TAB_BUFF:
				{
				}
				break;

			default:
				_ASSERT(0);
				pclClient->pclLog->LogPrint("[nPeriodItemListDlg.cpp]	정의 되지 않은 탭이 선택 되었습니다.");
				break;
			}
		}
	}
}



void CNPeriodItemListDlg::SetTabTextInfo(stTabTextInfo* in_SelectedText)
{
	if( in_SelectedText && in_SelectedText->st_pControllerObj )
	{
		in_SelectedText->st_pControllerObj->SetFontColor( in_SelectedText->st_Color );
		in_SelectedText->st_pControllerObj->SetFontWeight( in_SelectedText->st_siWeight );
		in_SelectedText->st_pControllerObj->Refresh();
	}
}



//	090326 [종호] 현재 선택된 탭 버튼의 폰트 상태를 설정한다.
void CNPeriodItemListDlg::SelectedTabBtn()
{
    stTabTextInfo tmp_stTabTextInfo;


	switch(m_siCurrentTabIndex)
	{
	case PERIOD_TAB_CHARACTER:
		{
			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_CHARACTER], CONTROLLER_BUTTONTEXT_COLOR , FW_BOLD );
			SetTabTextInfo(&tmp_stTabTextInfo);

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_BUFF], CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL);
			SetTabTextInfo(&tmp_stTabTextInfo);

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_HORSE], CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL);
			SetTabTextInfo(&tmp_stTabTextInfo);
		}
		break;

	case PERIOD_TAB_BUFF:
		{
			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_CHARACTER], CONTROLLER_BACK_BUTTONTEXT_COLOR , FW_NORMAL );
			SetTabTextInfo(&tmp_stTabTextInfo);

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_BUFF], CONTROLLER_BUTTONTEXT_COLOR, FW_BOLD);
			SetTabTextInfo(&tmp_stTabTextInfo);

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_HORSE], CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL);
			SetTabTextInfo(&tmp_stTabTextInfo);
		}
		break;

	case PERIOD_TAB_HORSE:
		{
			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_CHARACTER], CONTROLLER_BACK_BUTTONTEXT_COLOR , FW_NORMAL );
			SetTabTextInfo(&tmp_stTabTextInfo);

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_BUFF], CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL);
			SetTabTextInfo(&tmp_stTabTextInfo);

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_PeriodItem[PERIOD_TAB_HORSE], CONTROLLER_BUTTONTEXT_COLOR, FW_BOLD);
			SetTabTextInfo(&tmp_stTabTextInfo);
		}
		break;

	default:
		_ASSERT(NULL);
		break;
	}
}

