#include "../Client/NInterface/NPVPMapDlg/NPVPMapDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"

#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../Client.h"

#include "..\..\Resource.h"

#include "CommonLogic.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../common/NPCManager/NPC.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"


#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
//
#include "JwLib/IsCorrectID/IsCorrectID.h"
#include "NDataLoader.h"
extern CSoundOnOffDlg g_SoundOnOffDlg;
extern cltCommonLogic* pclClient;
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNPVPMapDlg::CNPVPMapDlg() : CDialogController()
{
	m_SprMap	=	NULL	;	// 보여줄 맵 그림.

	m_siSprMode				=	0	;		// 0 : 게이트 1: 맵
	m_siSelectGateIndex		=	0	;		// 선택된 게이트 
	m_siSelectMapIndex		=	0	;		// 선택된 맵
	m_siSelectMonsterIndex	=	0	;
	m_siMapIndex			=	0	;

	ZeroMemory( m_ListData_MapIndex, sizeof(m_ListData_MapIndex) )	;
}
CNPVPMapDlg::~CNPVPMapDlg()
{
	GP.FreeSpr(m_SprMap);
}

void CNPVPMapDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( IsCreate() )
	{
		DeleteDialog();
	}
	else
	{
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPVPMapDlg/DLG_NPVPMapDlg.ddf"));
		file.CreatePopupDialog( this, NPVPMAPGODLG,TEXT("dialog_PVPMAP"),StaticCallBackDialogCNPVPMapDlg );	
		// npc 얼굴
		// 에디트 박스 -> pvp 맵의 설명
		// 리스트 ->이름을 보여준다 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NPVPMapDlg_IMAGSTATIC_NPCFACE , this ), NPVPMapDlg_IMAGSTATIC_NPCFACE,   "imagestatic_portrait"); 		//엔피씨 얼굴
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NPVPMapDlg_EDIT_EXPLAIN , this ), NPVPMapDlg_EDIT_EXPLAIN,   "editbox_NONAME1"); 	// 설명 에디트 박스
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NPVPMapDlg_LIST_MAPLIST , this ), NPVPMapDlg_LIST_MAPLIST,   "listbox_NONAME1"); 	// 리스트
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NPVPMapDlg_BUTTON_MOVE , this ), NPVPMapDlg_BUTTON_MOVE,   "button_NONAME1"); 	// 이동 버튼

		// 얼굴 셋팅
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_PKMAPARBEIT")), szPortrait, sizeof(szPortrait), &siPortraitFont);

		m_InterfaceFactory.GetImageStatic(NPVPMapDlg_IMAGSTATIC_NPCFACE)->SetFileName(szPortrait);
		m_InterfaceFactory.GetImageStatic(NPVPMapDlg_IMAGSTATIC_NPCFACE)->SetFontIndex(siPortraitFont);

		// 텍스트 셋팅
		TCHAR* pExplain	=	GetTxtFromMgr( 20096 )	;
		m_InterfaceFactory.GetEdit(NPVPMapDlg_EDIT_EXPLAIN)->SetText( pExplain );


		// 데이터를읽는다.
		LoadMapData()	;
		// 리스트를 셋팅 한다. 
		SetList();

		// 다이얼로그를 생성 하면서 맵 그림을 읽어 둔다 .
		GP.LoadSprFromFile(TEXT("HuntMapInfo/huntmapinfo.SPR"), &m_SprMap);
	}

}
void CALLBACK CNPVPMapDlg::StaticCallBackDialogCNPVPMapDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPVPMapDlg *pThis = (CNPVPMapDlg *)pControllerMgr;
	pThis->CallBackDialogCNPVPMapDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNPVPMapDlg::CallBackDialogCNPVPMapDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NPVPMapDlg_LIST_MAPLIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					TCHAR strdir[128] = {'\0'};
					TCHAR buffer[256] = {'\0'};

					CList* pList = m_InterfaceFactory.GetList(NPVPMapDlg_LIST_MAPLIST);
					if(pList == NULL )	return	;
					SI32 Index = pList->GetSelectedIndex();
					if(Index < 0  )	return	;
					TCHAR* pMapName = pList->GetText(Index,0)	;
					if(pMapName == NULL )	return	;
					SI32 siMapindex = pclClient->pclMapManager->GetMapIndexFromName( pMapName )	;
					if(siMapindex <= 0 ) return	;
					SI32 MapUnique = pclClient->pclMapManager->GetMapUnique( siMapindex )	;
					if(MapUnique <= 0 ) return	;
					//------------------------
					//	리스트를 셀렉트 하면 해당 맵 정보를 에디트 박스에 표기 한다 
					//------------------------
					CEdit* pExplainEdit = m_InterfaceFactory.GetEdit(NPVPMapDlg_EDIT_EXPLAIN);

					SI32 parentmap = pclclient->pclMapManager->GetParentMap(siMapindex);
					SI32 siX = 0;
					SI32 siY = 0;

					pclclient->pclMapManager->pclGateManager->FindMainFieldPos( parentmap, &siX, &siY );
					pclclient->GetNearMapInfo(0, siX, siY ,strdir, 128);

					TCHAR* pText= GetTxtFromMgr(3334);
					StringCchPrintf(buffer, 1024, pText, strdir, pclclient->pclMapManager->GetLevelLimit(siMapindex));
					pExplainEdit->SetText(buffer);

					//------------------------
					//	리스트를 셀렉트 하면 맵 지도를 나타내기 위해 유니크를 셋팅 한다.
					//------------------------
					m_siMapIndex = MapUnique - 1 ;
				}
				break;
			}
		}
		break;
	case NPVPMapDlg_BUTTON_MOVE:
		{
			CList* pList = m_InterfaceFactory.GetList(NPVPMapDlg_LIST_MAPLIST);
			if(pList == NULL )	return	;
			SI32 Index = pList->GetSelectedIndex();
			if(Index < 0  )	return	;
			TCHAR* pMapName = pList->GetText(Index,0)	;
			if(pMapName == NULL )	return	;
			SI32 siMapindex = pclClient->pclMapManager->GetMapIndexFromName( pMapName )	;
			if(siMapindex <= 0 ) return	;

			// 워프 할 수 없는 상황이다.
			if( pclclient->pclCM->CR[1]->CanWarpFrom() == false )	return;

			TCHAR buffer[256] = TEXT("") ;
			bool bInMap = true;

			if ( siMapindex <= 0 || siMapindex > MAX_MAP_NUMBER ) return;
			TCHAR* pText= GetTxtFromMgr(3326);

			TCHAR* pTitle = NULL ;
			TCHAR* PText = NULL;

			// 무료이동 체크
			if( pclClient->pclCM->CR[1]->CanFreeWarp( siMapindex ) == false )
			{
				SI32 itemunique = ITEMUNIQUE(24000);  // 순간이동이용권   14005 -> 24000
				if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
				{
					pText= GetTxtFromMgr(3756);
					StringCchPrintf(buffer, 256, pText);
					pText= GetTxtFromMgr(3326);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer );
					return;
				}

				pTitle = GetTxtFromMgr(3326);
				pText  = GetTxtFromMgr(3332);
				StringCchPrintf(buffer,256, pText,pMapName);
			}
			else
			{
				// 무료이동
				pTitle = GetTxtFromMgr(3189);
				pText = GetTxtFromMgr(3760);

				StringCchPrintf(buffer, 256, pText, pMapName);
			}

			if ( pclClient->IsWhereServiceArea( ConstServiceArea_Japan )) 
			{
				pTitle = GetTxtFromMgr(836);
				TCHAR* pWarnningText = GetTxtFromMgr( 20100 )	;	// 경고 메세지 

				TCHAR buffer2[512] = TEXT("") ;

				StringCchPrintf(buffer2, 512, TEXT("%s \r\n %s"), buffer , pWarnningText );

				stMsgBox MsgBox;
				MsgBox.Set( this ,pTitle, buffer2, MSGBOX_TYPE_YESNO, 0 );
				BYTE TempBuffer[ 128 ];
				SI16 Size = 0;
				memcpy( &TempBuffer[Size], &siMapindex, sizeof(siMapindex) );		Size += sizeof(siMapindex);
				memcpy( &TempBuffer[Size], &bInMap, sizeof(bInMap) );			Size += sizeof(bInMap);

				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

			}
			else
			{
				stMsgBox MsgBox;
				MsgBox.Set( this ,pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
				BYTE TempBuffer[ 128 ];
				SI16 Size = 0;
				memcpy( &TempBuffer[Size], &siMapindex, sizeof(siMapindex) );		Size += sizeof(siMapindex);
				memcpy( &TempBuffer[Size], &bInMap, sizeof(bInMap) );			Size += sizeof(bInMap);

				pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
			}


		}break	;
	}
}
void CNPVPMapDlg::Action()
{

}
void CNPVPMapDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	// 맵인덱스가 선택 되면 지도를 보여준다 .
	if(m_siMapIndex <= 0)	return;
	GP.PutSpr(&m_SprMap, ScreenRX + GetX() + 10 , ScreenRY + GetY() + 220, m_siMapIndex);
}
void CNPVPMapDlg::SetList()
{
	CList* pList = m_InterfaceFactory.GetList( NPVPMapDlg_LIST_MAPLIST );
	if(pList == NULL )	return	;
	pList->SetBorder(true);
	pList->SetBKColorUse(true);
	TCHAR* pText = GetTxtFromMgr(1831);
	pList->SetColumn( 0,  360,  pText );
	pList->Clear();


	stListBoxItemData data	;
	data.Init()	;
	for ( SI32 i = 0 ; i < MAX_PVP_MAPINDEXNUM; i++ ) 
	{
		if(m_ListData_MapIndex[i] <= 0)	continue	;
		TCHAR * pTextMapName = pclClient->pclMapManager->GetMapName( m_ListData_MapIndex[i] )	;
		if(pTextMapName == NULL )	continue	;
		data.Set( 0,pTextMapName );
		pList->AddItem( &data );
	}	
	pList->Refresh();
}

void CNPVPMapDlg::LoadMapData()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/PKHuntingMapInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/PKHuntingMapInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile HuntmapInfo"),TEXT("Error"));
		return;
	}

	TCHAR maptypename[256] = TEXT(""), mapunique[256] = TEXT("");
	SI32	mapinx = 0  , DataIndex = 0 ;

	stListBoxItemData data	;
	data.Init();

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	mapunique,	256,
			NDATA_INT32,	&mapinx,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() ) 
		{
			if( mapinx <= 0 )	continue	;
			if( DataIndex >= MAX_PVP_MAPINDEXNUM )	break	;
			m_ListData_MapIndex[DataIndex]	=	mapinx	;
			DataIndex++;
		}
	}	
}
void CNPVPMapDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				//KHY - 0703 - 게임 머니로 귀환하기.클레스 수정.
				SI32 siDataPtr = 0;
				SI32 siMapIndex = *(SI32*)&pData[siDataPtr];		siDataPtr+= sizeof(siMapIndex);
				bool bInMap		= *(bool*)&pData[siDataPtr];		siDataPtr+= sizeof(bInMap);

				cltGameMsgRequest_HuntMapWarp pclInfo(siMapIndex,0, bInMap);
				cltMsg clMsg( GAMEMSG_REQUEST_PVPMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}