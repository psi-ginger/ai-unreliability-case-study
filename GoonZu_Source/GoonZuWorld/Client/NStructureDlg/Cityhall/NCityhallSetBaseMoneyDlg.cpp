#include "NCityhallDlg.h"

//#include "../../InterfaceMgr/Interface/List.h"
//#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../Client/NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "Msg/MsgType-Village.h"

#include "..\..\Client\Music\Music.h"

#include "../common/Map/FieldObject/FieldObject.h"
#include "../CommonLogic/Village/Village.h"

#include "../CommonLogic/Bank/Bank-Manager.h"
#include "../CommonLogic/Guild/Guild.h"
#include "../CommonLogic/House/House.h"
#include "../CommonLogic/HorseMarket/HorseMarket.h"
#include "../CommonLogic/SummonHeroMarket/SummonHeroMarket.h"
#include "../CommonLogic/SummonMarket/SummonMarket.h"
#include "../CommonLogic/RealEstate/RealEstate.h"
#include "../CommonLogic/Land/Land.h"
#include "../CommonLogic/Mine/Mine.h"
#include "../CommonLogic/PostOffice/Postoffice.h"
#include "../CommonLogic/Feast/Feast.h"

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallSetBaseMoneyDlg::NCityhallSetBaseMoneyDlg()
{
	m_bUpdateSwitch = false;
	m_siSelectRankType = 0;
}

NCityhallSetBaseMoneyDlg::~NCityhallSetBaseMoneyDlg()
{
}

void NCityhallSetBaseMoneyDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	if( pCityhallManager == NULL ) return;

	SetControlMap( LIST_STRUCTURE, TEXT("listbox_structure") );
	SetControlMap( BUTTON_SETMONEY, TEXT("button_setmoney") );
	SetControlMap( EDIT_SETMONEY, TEXT("editbox_setmoney") );

	CList *pList = (CList *)m_pControlMap[ LIST_STRUCTURE ];

	TCHAR* pText[3];

	pText[0] = GetTxtFromMgr(1611);
	pText[1] = GetTxtFromMgr(1612);
	pText[2] = GetTxtFromMgr(5990);

	int width[] = { 36, 300 };
	for( SI16 i = 0; i < 2; ++i ) 
	{
		pList->SetColumn( i, width[ i ], pText[i] ); 
	}

	pList->SetBorder( true );
	pList->SetBKColorUse( true );
	pList->Refresh();

	m_bUpdateSwitch = true;
}

void NCityhallSetBaseMoneyDlg::Action()
{
	if( m_bUpdateSwitch == true )
	{
		m_bUpdateSwitch = false;
		cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

		CList *pList = (CList *)m_pControlMap[ LIST_STRUCTURE ];

		SI32 siVillageUnique = pCityhallManager->siVillageUnique;
		cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ siVillageUnique ];
		TCHAR buf[ 256 ] = {'\0'};

		if( pVillageInfo )
		{
			// 泅犁 付阑 格废
			SI32 i;
			for( i = 0; i < MAX_CONSTRUCTION_NUM; ++i ) 
			{
				SI32 siStructureUnique = pVillageInfo->clVillageVary.clStructureInfo.Get( i );

				if( siStructureUnique )
				{
					SI32 ref = pclClient->pclVillageStructureSet->FindRefFromUnique( siStructureUnique );

					if( ref != -1 )
					{
						cltVillageStructureInfo *pStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[ ref ];

						if( pStructureInfo )
						{
							stListBoxItemData tmpListBoxItemData;
							tmpListBoxItemData.Init();
							tmpListBoxItemData.siParam[0] = pStructureInfo->siUnique;
							tmpListBoxItemData.siParam[1] = pStructureInfo->siRankType;

							_itot( i + 1, buf, 10 );
							tmpListBoxItemData.Set( 0, buf );
							tmpListBoxItemData.Set( 1, pStructureInfo->szName );

						/*	SI64 tempBaseMoney = 0;
							if( RANKTYPE_CITYHALL == pStructureInfo->siRankType)
							{
								tempBaseMoney = m_pStrManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney);
							}
							if( RANKTYPE_BANK ==  pStructureInfo->siRankType)
							{
								cltStrInfo* pStrInfo = pclClient->pclBankManager->GetStrInfo( siVillageUnique );
								if( pStrInfo )
								{
									tempBaseMoney = pStrInfo->clCommonInfo.clBaseMoney.itMoney;
								}
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_STOCK == pStructureInfo->siRankType )
							{
								cltStrInfo* pStrInfo = pclClient->pclStockManager->GetStrInfo( siVillageUnique );
								if( pStrInfo )
								{
									tempBaseMoney = pStrInfo->clCommonInfo.clBaseMoney.itMoney;
								}
								sprintf( buf, "%I64d成", tempBaseMoney);
							}
							else if( RANKTYPE_MARKET == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclMarketManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_HOUSE == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclHouseManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_POSTOFFICE == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclPostOfficeManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_PORT == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclPortManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_HORSEMARKET == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclHouseManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_REALESTATEMARKET == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclRealEstateMarketManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_SUMMONMARKET == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclSummonMarketManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_SHIPMARKET == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclShipMarketManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_LAND == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclLandManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_FEAST == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclFeastManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_SUMMONHEROMARKET == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclSummonHeroMarketManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_MINE == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclMineManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_GUILD == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclGuildStructManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
							else if( RANKTYPE_DEPOSITORY == pStructureInfo->siRankType )
							{
								cltStructureMngBasic *pTempManager = pclClient->pclDepositoryManager;
								tempBaseMoney = pTempManager->clClientStrInfo.clCommonInfo.clBaseMoney.itMoney;
								sprintf( buf, "%I64d成", tempBaseMoney );
							}
						//	else
                        //     sprintf( buf, "0成" );

							tmpListBoxItemData.Set( 2, buf);*/

							pList->AddItem( &tmpListBoxItemData );

						}

					}
				}
				else
				{
					stListBoxItemData tmpListBoxItemData;
					tmpListBoxItemData.Init();
					tmpListBoxItemData.siParam[0] = 0;

					itoa( i + 1, buf, 10 );
					tmpListBoxItemData.Set( 0, buf );

					TCHAR* pText = GetTxtFromMgr(1619);
					tmpListBoxItemData.Set( 1, pText );

				/*	if( i == 6 )
					{
						pText = GetTxtFromMgr(1620);
						tmpListBoxItemData.Set( 2, pText );
					}
					else
					{
						tmpListBoxItemData.Set( 2, "--" );
					}*/

					pList->AddItem( &tmpListBoxItemData );
				}
			}

			pList->Refresh();
		}
	}
}

void NCityhallSetBaseMoneyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	cltClient *pclclient = (cltClient*)pclClient;

	CASE_CONTROL( LIST_STRUCTURE )
	{
		CList *pList = (CList *)m_pControlMap[ LIST_STRUCTURE ];
		SI16 tempSelectIndex = pList->GetSelectedIndex();
		if( tempSelectIndex < 0 )
			return;

		stListBoxItemData* pTempData = pList->GetItemData( tempSelectIndex );
		m_siSelectRankType = pTempData->siParam[1];
	}
	else CASE_CONTROL( BUTTON_SETMONEY )
	{
		cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
		if( pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
		{
			TCHAR* pText;
			if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) pText = GetTxtFromMgr(1547);
			else pText = GetTxtFromMgr(1440);
			SetMainExplain(pText);
			return;
		}
	
		SI64 money = _atoi64( GetEditText( EDIT_SETMONEY ) );
		
		if( money <=  0)
		{
			TCHAR* pText = GetTxtFromMgr(5991);
			SetMainExplain(pText);
		}

		if( m_siSelectRankType <=0 )
		{
			TCHAR* pText = GetTxtFromMgr(5992);
			SetMainExplain(pText);
		}

		cltGameMsgRequest_ChangeStrBaseMoney sendMsg( m_siSelectRankType, money);
		cltMsg clMsg( GAMEMSG_REQUEST_CHANGESTRBASEMONEY, sizeof( cltGameMsgRequest_ChangeStrBaseMoney ), (BYTE *)&sendMsg );
		pclClient->pclCM->CR[ 1 ]->SendNetMsg( (sPacketHeader*)&clMsg );

		CEdit* pEdit = (CEdit *)m_pControlMap[ EDIT_SETMONEY ];
		pEdit->SetText("");
	}
	
}

void NCityhallSetBaseMoneyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	switch( MsgIndex )
	{
	case 0:	// 付阑 瘤档 函版
		{
			if( pData == NULL || DataLen < 1 ) return;

			if ( RetValue )
			{
			}
		}
		break;
	}		
}
