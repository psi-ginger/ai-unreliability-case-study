#include "NGoonzuSiege.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../nvillagedetailinfo/NVillageDetailInfo.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"


extern cltCommonLogic* pclClient;

CNGoonzuSiegeListDlg::CNGoonzuSiegeListDlg() : CDialogController()
{
	m_pListSiegeList = NULL;
	m_pStaticSiegeTime = NULL;
}

CNGoonzuSiegeListDlg::~CNGoonzuSiegeListDlg()
{

	SAFE_DELETE( m_pListSiegeList );
	SAFE_DELETE( m_pStaticSiegeTime );
}

void CNGoonzuSiegeListDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile	file;

	NEWCLIST( m_pListSiegeList );
	NEWCSTATIC( m_pStaticSiegeTime );

	file.LoadFile("NInterface/Data/NGoonzuDlg/DLG_GoonzuSiegeList.ddf");
	file.CreateChildDialog( this, NGOONZUSIEGE_DLG, TEXT("dialog_goonzusiegelist"), StaticCallBackDialogNGoonzuSiegeListDlg, pParent);


	file.CreateControl( m_pListSiegeList, NGOONZUSIEGELIST_LIST_SIEGE, "listbox_siegelist"); 
	file.CreateControl( m_pStaticSiegeTime, NGOONZUSIEGELIST_STATIC_TIME, "statictext_NONAME1");

	// 기존의 리스트를 삭제한다. 
	TCHAR* pText = NULL;

	pText = GetTxtFromMgr( 6476 );
	m_pListSiegeList->SetColumn( 0, 80, pText );
	pText = GetTxtFromMgr( 6477 );
	m_pListSiegeList->SetColumn( 1, 80, pText );
	pText = GetTxtFromMgr( 6478 );
	m_pListSiegeList->SetColumn( 2, 110, pText );
	m_pListSiegeList->SetBorder( true );
	m_pListSiegeList->SetBKColorUse( true );
	m_pListSiegeList->SetDrawLine( true );
	m_pListSiegeList->SetTextPrintOption( DT_CENTER | DT_VCENTER );
	m_pListSiegeList->Refresh();
	m_pListSiegeList->Clear();

	ShowList();
}

void CNGoonzuSiegeListDlg::Action()
{
}

void CALLBACK CNGoonzuSiegeListDlg::StaticCallBackDialogNGoonzuSiegeListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNGoonzuSiegeListDlg *pThis = (CNGoonzuSiegeListDlg *)pControllerMgr;
	pThis->CallBackDialogNGoonzuSiegeListDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNGoonzuSiegeListDlg::CallBackDialogNGoonzuSiegeListDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	default:
		break;
	}
}
void CNGoonzuSiegeListDlg::ShowList()
{
	cltClient* pclclient = (cltClient*)pclClient;
	TCHAR buffer[256] = TEXT("");

	stListBoxItemData tmpListBoxItemData;

	// 기존의 리스트를 삭제한다. 
	m_pListSiegeList->Clear();

	SI64 sidata[MAX_VILLAGE_NUMBER];
	SI32 siVillageInfo[MAX_VILLAGE_NUMBER];

	NDate currentDate;
	currentDate.m_uiYear = pclclient->sTime.wYear;
	currentDate.m_uiMonth = pclclient->sTime.wMonth;
	currentDate.m_uiDay = pclclient->sTime.wDay;


	//--------------------------------
	// 내림차순으로 정렬한다. 
	//--------------------------------
	SI32 i;
	for(i = 0;i < MAX_VILLAGE_NUMBER;i++)
	{
		sidata[i] = pclClient->pclVillageManager->clDeclareWarDate[i].GetDateVary();
	}
	SI32 index = Sort(sidata, siVillageInfo);

	NDate warDate;					// 실제 전쟁이 시작하는 시간

	UI32 warStartHour;				// 전쟁이 시작하는 시간
	if( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_EUROPE) )
	{
		warStartHour = 1 ;
	}
	else if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
	{
		warStartHour = 18 ;
	}
	else
	{
		warStartHour = 21 ;
	}

	for(i = 0;i < index;i++)
	{

		SI32 villageunique = siVillageInfo[i];

		warDate = pclClient->pclVillageManager->clDeclareWarDate[villageunique];

		if ( warDate.m_uiYear == 0 )
			continue;

		warDate.AddDays( 2 );

		if(pclClient->pclVillageManager->pclVillageInfo[villageunique] == NULL)
			continue;

		// 현재시간이 전쟁이 시작하는 시간보다 지났으면 출력치 않음
		if( warDate.GetDateVary() <= currentDate.GetDateVary() && pclclient->sTime.wHour > warStartHour )
			continue;

		//전쟁하려는 상대 국가가 없으면 출력치 않음
		if( pclclient->pclVillageManager->siDeclareWarVillage[villageunique] == 0 )
			continue;

		if( pclclient->siServiceArea == ConstServiceArea_English
			|| pclclient->siServiceArea == ConstServiceArea_USA
			|| pclclient->siServiceArea == ConstServiceArea_EUROPE
		)
		{
			// 미국식 날짜 표기
			StringCchPrintf(buffer, 256, TEXT("%02d-%02d-%04d"), 
				 warDate.m_uiMonth, warDate.m_uiDay, warDate.m_uiYear );
		}
		else
		{
			// 동양권 날짜 표기
			StringCchPrintf(buffer, 256, TEXT("%04d-%02d-%02d"), 
				warDate.m_uiYear, warDate.m_uiMonth, warDate.m_uiDay );
		}

		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pclClient->pclVillageManager->GetName(villageunique) );
		tmpListBoxItemData.Set( 1, pclClient->pclVillageManager->GetName(pclClient->pclVillageManager->siDeclareWarVillage[villageunique]) );
		tmpListBoxItemData.Set( 2, buffer );
		tmpListBoxItemData.siParam[ 0 ] = villageunique;

		// 리스트 항목에 데이터를 연결한다. 
		SI32 index = m_pListSiegeList->AddItem( &tmpListBoxItemData );
		m_siListdata[index] = villageunique;
	}

	m_pListSiegeList->Refresh();
}
// 내림차순으로 정렬한다. 
SI32 CNGoonzuSiegeListDlg::Sort(SI64 sisortdata[], SI32* pvillageunique)
{
	SI32 i;
	SI32 index = 0;

	SI32 tempbuf[ MAX_VILLAGE_NUMBER ];

	for( i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		tempbuf[ i ] = i;
	}

	for(SI32 vill = 0; vill < MAX_VILLAGE_NUMBER - 1; vill++)
	{
		if(pclClient->pclVillageManager->pclVillageInfo[vill] == NULL)continue;

		for(i = vill + 1; i < MAX_VILLAGE_NUMBER; i++)
		{
			if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;

			int cmpindex1 = tempbuf[ vill ];
			int cmpindex2 = tempbuf[ i ];

			if(sisortdata[cmpindex1] < sisortdata[cmpindex2])
			{
				int temp = tempbuf[ vill ];
				tempbuf[ vill ] = tempbuf[ i ];
				tempbuf[ i ] = temp;
			}
		}
	}

	for(i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		if( pclClient->pclVillageManager->pclVillageInfo[ tempbuf[i] ]== NULL)continue;
		pvillageunique[ index ] = tempbuf[ i ];
		index++;
	}

	return index;
}
