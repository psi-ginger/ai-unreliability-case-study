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

#include "../Lib/JYLibFunction.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallChangeMapDlg::NCityhallChangeMapDlg()
{
}

NCityhallChangeMapDlg::~NCityhallChangeMapDlg()
{
}

void NCityhallChangeMapDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	if( pCityhallManager == NULL ) return;

	SetControlMap( IMAGESTATIC_MAP, TEXT("imagestatic_map") );
	SetControlMap( BUTTON_PREV, TEXT("button_prev") );
	SetControlMap( BUTTON_SETUP, TEXT("button_setup") );
	SetControlMap( BUTTON_NEXT, TEXT("button_next") );
	SetControlMap( EDITBOX_COST, TEXT("editbox_cost") );
	SetControlMap( EDITBOX_VILLAGETYPE, TEXT("editbox_villagetype") );
	SetControlMap( EDITBOX_NEEDLEVEL, TEXT("editbox_needlevel") );

#ifdef _SAFE_MEMORY
	pCityhallManager->m_siVillageMapUnique.ZeroMem();
#else
	ZeroMemory( pCityhallManager->m_siVillageMapUnique, sizeof( pCityhallManager->m_siVillageMapUnique ) );
#endif

	pCityhallManager->m_siVillageMapIndex = 0;

	pCityhallManager->m_siVillageMapNumber = 0;			

	cltMapTypeInfo *pMapTypeInfo;

	SI32 villagetype = pclClient->pclVillageManager->GetVillateType( pCityhallManager->siVillageUnique );

	for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i ) {

		pMapTypeInfo = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

		if( pMapTypeInfo == NULL ) continue;

		if( villagetype != 0 ) {
			if( villagetype % 2 == 1 ) {

				if( pMapTypeInfo->siAdaptVillageType == VILLAGETYPE_SPRING_NORMAL ||
					pMapTypeInfo->siAdaptVillageType == VILLAGETYPE_AUTUMN_NORMAL ||
					pMapTypeInfo->siAdaptVillageType == VILLAGETYPE_WINTER_NORMAL ) {

						pCityhallManager->m_siVillageMapUnique[ pCityhallManager->m_siVillageMapNumber ] = pMapTypeInfo->siMapUnique;
						pCityhallManager->m_siVillageMapNumber++;
					}

			}
		}

	}

	ShowVillageMap();
}

void NCityhallChangeMapDlg::Action()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
}

void NCityhallChangeMapDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	cltClient *pclclient = (cltClient*)pclClient;

	CASE_CONTROL( BUTTON_PREV )
	{
		if( pCityhallManager->m_siVillageMapIndex > 0 ) pCityhallManager->m_siVillageMapIndex--;

		ShowVillageMap();
	}
	else CASE_CONTROL( BUTTON_NEXT )
	{
		if( pCityhallManager->m_siVillageMapIndex < pCityhallManager->m_siVillageMapNumber - 1 ) pCityhallManager->m_siVillageMapIndex++;

		ShowVillageMap();

	}
	else CASE_CONTROL( BUTTON_SETUP )
	{

		SI32 mapunique = pCityhallManager->m_siVillageMapUnique[ pCityhallManager->m_siVillageMapIndex ];

		if( mapunique == 0 ) return;				


		if( pclClient->pclMapManager->pclMapArray[ pCityhallManager->siVillageUnique ] == NULL ) return;

		if( pclClient->pclMapManager->pclMapArray[ pCityhallManager->siVillageUnique ]->siMapUnique == mapunique ) {
			TCHAR* pText = GetTxtFromMgr(1534);
			SetMainExplain(pText);
			return;
		}

		TCHAR buffer[256];
		TCHAR* pText = GetTxtFromMgr(1535);
		StringCchPrintf(buffer, 256, pText );

		pText = GetTxtFromMgr(1536);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pText, buffer, MSGBOX_TYPE_YESNO, 26 );
		BYTE TempBuffer[128];
		memset( TempBuffer, 0, sizeof(TempBuffer));
		memcpy( TempBuffer, &mapunique, sizeof(mapunique));
		SI16 Size = sizeof(mapunique);
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size);

		//if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) 
		//{
		//	break;
		//}

		//cltGameMsgRequest_SetVillageMap sendMsg( mapunique );

		//cltMsg clMsg( GAMEMSG_REQUEST_SETVILLAGEMAP, sizeof( sendMsg ), (BYTE*)&sendMsg );
		//pclClient->pclCM->CR[1]->SendNetMsg( (TCHAR*)&clMsg );
	}

}

void NCityhallChangeMapDlg::ShowVillageMap()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	CImageStatic *pImageMap = (CImageStatic *)m_pControlMap[ IMAGESTATIC_MAP ];

	if( pCityhallManager->m_siVillageMapNumber == 0 ) return;

	SI32 mapunique = pCityhallManager->m_siVillageMapUnique[ pCityhallManager->m_siVillageMapIndex ];
	if( mapunique == 0 ) return;

	TCHAR *pmaptype = pclClient->pclMapManager->pclMapTypeManager->GetMapTypeNameFromUnique( mapunique );
	if( pmaptype == NULL )return;

	//if( pCityhallManager->m_hVillageMapBitmap )
	//{
	//	DeleteObject( pCityhallManager->m_hVillageMapBitmap );
	//	pCityhallManager->m_hVillageMapBitmap = NULL;
	//}

	TCHAR strFileName[ 128 ] = TEXT("");
	StringCchPrintf( strFileName, 128, TEXT("map\\%s\\%s.spr"), pmaptype, pmaptype );
	pImageMap->SetFileName( strFileName );

	//if ( pCityhallManager->m_hVillageMapBitmap != NULL )
	//{
	//	SendDlgItemMessage( hDlg, IDC_STATIC_VILLAGEMAP, STpCityhallManager->m_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)pCityhallManager->m_hVillageMapBitmap );
	//}

	TCHAR buffer[256 ];

	cltMapTypeInfo *pMapTypeInfo;

	for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i ) {

		pMapTypeInfo = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[ i ];

		if( pMapTypeInfo == NULL ) continue;

		if( pMapTypeInfo->siMapUnique == mapunique ) {

			TCHAR* pText = GetTxtFromMgr(611);

			g_JYLibFunction.SetNumUnit( pMapTypeInfo->siAdaptNeedVillageMoney, buffer, 256, pText);

			//SetDlgItemText( hDlg, IDC_EDIT_MONEY, buffer );
			SetEditText( EDITBOX_COST, buffer );

			_itot( pMapTypeInfo->siAdaptNeedVillageLevel, buffer, 10 );

			//SetDlgItemText( hDlg, IDC_EDIT_VILLAGELEVEL, buffer );
			SetEditText( EDITBOX_NEEDLEVEL, buffer );
			break;
		}
	}

	switch( pclClient->pclVillageManager->GetVillateType( pCityhallManager->siVillageUnique ) )
	{
	case VILLAGETYPE_SPRING_NORMAL:
		{
			TCHAR* pText = GetTxtFromMgr(1537);
			StringCchCopy( buffer, 256, pText );
		}
		break;

	case VILLAGETYPE_AUTUMN_NORMAL:
		{
			TCHAR* pText = GetTxtFromMgr(1539);
			StringCchCopy( buffer, 256, pText );
		}
		break;


	case VILLAGETYPE_WINTER_NORMAL:
		{
			TCHAR* pText = GetTxtFromMgr(1541);
			StringCchCopy( buffer, 256, pText);
		}
		break;
	}

	//SetDlgItemText( hDlg, IDC_EDIT_VILLAGETYPE, buffer );
	SetEditText( EDITBOX_VILLAGETYPE, buffer );

}

void NCityhallChangeMapDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	switch( MsgIndex )
	{
	case 26:	// 마을 지도 변경
		{
			if( pData == NULL || DataLen < 1 ) return;

			if ( RetValue )
			{
				SI32 mapunique = -1;
				mapunique = *((SI32*)pData);

				cltGameMsgRequest_SetVillageMap sendMsg( mapunique, cltGameMsgRequest_SetVillageMap::MAPTYPE_CITYHALL );

				cltMsg clMsg( GAMEMSG_REQUEST_SETVILLAGEMAP, sizeof( sendMsg ), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg );
			}
		}
		break;
	}		
}
