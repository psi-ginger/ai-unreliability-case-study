//-----------------------------------------------------------------------------
// 2005/09/26 ������
//-----------------------------------------------------------------------------

#include "MapManager.h"
#include "MapTypeManager.h"
#include "NDataLoader.h"
#include "NHashTableString.h"

#include "..\..\..\Lib\EtcTool\EtcTool.h"

#include "CommonLogic.h"
#include "..\..\..\Server\TraceBoot.h"

extern cltCommonLogic* pclClient;

cltMapTypeInfo::cltMapTypeInfo()
{
	ZeroMemory( this, sizeof( cltMapTypeInfo ) );
}

cltMapTypeInfo::~cltMapTypeInfo()
{

}

void cltMapTypeInfo::SetMapType(SI32 mapunique, TCHAR* pname, SI32 centerx, SI32 centery, SI32 gatex, SI32 gatey, POINT* ppoint,
						   SI32 atb, SI32 adapt, SI32 level, GMONEY money,SI32 huntadapt)
{
	SI32 i;

	siMapUnique	= mapunique;

	if( pname ) {
		StringCchCopy( szMapTypeName, 64, pname );
	}

	siCenterX	= centerx;
	siCenterY	= centery;

	siGateX		= gatex;
	siGateY		= gatey;

	if( ppoint )
	{
		for( i = 0;i < MAX_CONSTRUCTION_NUM; ++i ) 
		{
			StrLocation[i] = ppoint[i]; 
		}

	} else {

		for( i = 0;i < MAX_CONSTRUCTION_NUM; ++i ) 
		{
			StrLocation[i].x = 0;
			StrLocation[i].y = 0;
		}
	}

	siAdaptVillageType		= adapt;
	siAdaptHuntType			= huntadapt;
	siAdaptNeedVillageLevel	= level;
	siAdaptNeedVillageMoney	= money;
	siAtb					= atb;
}

cltMapTypeManager::cltMapTypeManager()
{

	POINT point[ 10 ];

	for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i ) 
	{
		pclMapTypeInfo[i] = NULL;
	}

	LoadMapTypeInfo();		
	//	LoadMapTypeInfo_Village();
	//	LoadMapTypeInfo_Hunt();


	SetMapType(13,	TEXT("Hunt01"),			15, 76, 15, 76, NULL, 0,	0, 2, 10000000, HUNTTYPE_ALL); // �߿ܼ�����

	/*
	point[0].x = 7;		point[0].y = 7; // ����
	point[1].x = 19;	point[1].y = 7; // ����
	point[2].x = 31;	point[2].y = 7; // ������
	point[3].x = 7;		point[3].y = 19; // ����
	point[4].x = 19;	point[4].y = 21; // ���
	point[5].x = 31;	point[5].y = 19; // ����
	point[6].x = 9;		point[6].y = 33; // NPC
	point[7].x = 21;	point[7].y = 33; // ����
	point[8].x = 33;	point[8].y = 33; // ����
	*/ 


	point[0].x = MAPTYPE_STRUCTURE_POINT_X_SLOT0;	point[0].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT0; // ����
	point[1].x = MAPTYPE_STRUCTURE_POINT_X_SLOT1;	point[1].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT1; // ����
	point[2].x = MAPTYPE_STRUCTURE_POINT_X_SLOT2;	point[2].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT2; // ����
	point[3].x = MAPTYPE_STRUCTURE_POINT_X_SLOT3;	point[3].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT3; // ����
	point[4].x = MAPTYPE_STRUCTURE_POINT_X_SLOT4;	point[4].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT4; // ���
	point[5].x = MAPTYPE_STRUCTURE_POINT_X_SLOT5;	point[5].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT5; // ����
	point[6].x = MAPTYPE_STRUCTURE_POINT_X_SLOT6;	point[6].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT6; // NPC
	if ( pclClient->siServiceArea == ConstServiceArea_China )
	{
		point[7].x = MAPTYPE_STRUCTURE_POINT_X_SLOT7 - 1;	point[7].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT7; // ����
	}
	else
	{
        point[7].x = MAPTYPE_STRUCTURE_POINT_X_SLOT7;	point[7].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT7; // ����
	}
	point[8].x = MAPTYPE_STRUCTURE_POINT_X_SLOT8;	point[8].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT8; // ����
	point[9].x = MAPTYPE_STRUCTURE_POINT_X_SLOT9;	point[9].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT9; 


	switch( pclClient->siServiceArea )
	{

	case ConstServiceArea_Japan:
	case ConstServiceArea_China:
		SetMapType(24,	TEXT("Village01"),		26, 13, 26, 8, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000);
		break;
	default:
		SetMapType(24,	TEXT("Village01"),		25, 13, 25, 7, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000);
		break;
	}

	SetMapType(25,	TEXT("Village02"),		24, 22, 23, 18, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000);
	SetMapType(26,	TEXT("Village03"),		24, 22, 23, 18, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000);
	SetMapType(27,	TEXT("Village04"),		24, 22, 23, 18, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000);

	point[0].x = MAPTYPE_STRUCTURE_POINT_X_SLOT0;	point[0].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT0; // ����
	point[1].x = MAPTYPE_STRUCTURE_POINT_X_SLOT1;	point[1].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT1; // ����
	point[2].x = MAPTYPE_STRUCTURE_POINT_X_SLOT2;	point[2].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT2; // ����
	point[3].x = MAPTYPE_STRUCTURE_POINT_X_SLOT3;	point[3].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT3; // ����
	point[4].x = MAPTYPE_STRUCTURE_POINT_X_SLOT4;	point[4].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT4; // ���
	point[5].x = MAPTYPE_STRUCTURE_POINT_X_SLOT5;	point[5].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT5; // ����
	point[6].x = MAPTYPE_STRUCTURE_POINT_X_SLOT6;	point[6].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT6; // NPC
	if ( pclClient->siServiceArea == ConstServiceArea_China )
	{
		point[7].x = MAPTYPE_STRUCTURE_POINT_X_SLOT7 - 1;	point[7].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT7; // ����
	}
	else
	{
		point[7].x = MAPTYPE_STRUCTURE_POINT_X_SLOT7;	point[7].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT7; // ����
	}
	point[8].x = MAPTYPE_STRUCTURE_POINT_X_SLOT8;	point[8].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT8; // ����
	point[9].x = MAPTYPE_STRUCTURE_POINT_X_SLOT9;	point[9].y = MAPTYPE_STRUCTURE_POINT_Y_SLOT9;

	SetMapType(31,	TEXT("Village05"),			16, 12, 16, 8, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000); // �� ����
	SetMapType(33,	TEXT("Village06"),			16, 12, 16, 8, point, 0, VILLAGETYPE_AUTUMN_NORMAL, 2, 10000000); // ���� ����
	SetMapType(34,	TEXT("Village07"),			16, 12, 16, 8, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000); // �� �ױ�����
	SetMapType(35,	TEXT("Village08"),			16, 12, 16, 8, point, 0, VILLAGETYPE_SPRING_NORMAL, 2, 10000000); // ���� �ױ� ����
	SetMapType(13,	TEXT("Hunt01"),	6, 14, 4, 15, NULL, 0,	0, 2, 10000000, HUNTTYPE_ALL); // ����Ǯ��
	SetMapType(36,	TEXT("Hunt02"),	6, 43, 4, 45, NULL, 0,	0, 2, 10000000, HUNTTYPE_ALL); // ����ź��
	SetMapType(46,	TEXT("Hunt03"),	9, 22, 7, 23, NULL, 0,	0, 2, 10000000, HUNTTYPE_ALL); // ������
	SetMapType(51,	TEXT("Hunt04"),	22, 11, 22, 8, NULL, 0,	0, 2, 10000000, HUNTTYPE_ALL); // ��������

}

cltMapTypeManager::~cltMapTypeManager()
{

	for( int i = 0; i < MAX_MAPTYPE_NUMBER; ++i )
	{
		NDelete( pclMapTypeInfo[ i ] );
	}
}

void cltMapTypeManager::LoadMapTypeInfo()
{

	int	iMapUnique;
	TCHAR szMapTypeString[ 64 ];
	TCHAR szMapTypeName[ 64 ];

	NDataLoader		dataloader;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	bool bLoadResult = false;
	bool bOpened = false;
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		BOOL bNCR1 = FALSE;
		FILE* fp1 = _tfopen( TEXT("Data/MapTypeInfo.txt"), TEXT("rb") );
		if( fp1 ) {
			BYTE m[3] = {0};
			fread( m, 1, 3, fp1 );
			fclose( fp1 );
			bOpened = true;
			bNCR1 = (m[0] == 'N' && m[1] == 'C' && m[2] == 'R');
		}
		TraceBoot("MapTypeInfo: file_opened=%d isNCR=%d", bOpened?1:0, bNCR1?1:0);
		if( bNCR1 )
			bLoadResult = dataloader.LoadDataFromCryptFile( TEXT("Data/MapTypeInfo.txt") );
		else
			bLoadResult = dataloader.LoadDataFromFile( TEXT("Data/MapTypeInfo.txt") );
	}
	else
	{
		bLoadResult = dataloader.LoadDataFromFile( TEXT("Data/MapTypeInfo.txt") );
		TraceBoot("MapTypeInfo: non-encrypted service area path");
	}
	TraceBoot("MapTypeInfo: loader_result=%d", bLoadResult?1:0);

	NDataTypeInfo	datatypeinfo[] = 
	{ 
		NDATA_INT32,	&iMapUnique,		4,				// MapTypeUnique
		NDATA_MBSTRING,	szMapTypeString,	64,				// MapTypeString
		NDATA_MBSTRING,	szMapTypeName,		64,				// MTName
		0,0,0
	};							

	dataloader.RegDataType( datatypeinfo );

	SI32 siTypeCount = 0;
	while( !dataloader.IsEndOfData() )
	{

		if( dataloader.ReadData()) {

			if( iMapUnique == 0 || szMapTypeString[ 0 ] == NULL ) {
				TraceBoot("MapTypeInfo: break at iMapUnique=%d str='%s'", iMapUnique, szMapTypeString);
				break;
			}
			
#ifdef _DEBUG
			if( iMapUnique >= MAX_MAPTYPE_NUMBER )
				MessageBox(NULL, TEXT("OVERFLOW MAPINDEX"), TEXT("ERROR"), MB_OK);
#endif
			// MapType�� �߰��Ѵ�
			if( AddMapType( iMapUnique, szMapTypeName ) ) {
				siTypeCount++;

				// MapUniqueString�� �ؽ����̺��� �߰��Ѵ�
				GetGlobalHashTableString()->Insert( szMapTypeString, iMapUnique );
			}

		} else break;
	}

	{
		TCHAR* pType14 = GetMapTypeNameFromUnique( 14 );
		TraceBoot("MapTypeInfo: types_loaded=%d type14=%s", siTypeCount, pType14 ? pType14 : "NULL");
	}

}

void cltMapTypeManager::LoadMapTypeInfo_Village()
{

	NDataAtbParser atbparser;

	atbparser.AddAtbInfo( TEXT("MAPTYPE_ATB_VILLAGE_SPRING"), MAPTYPE_ATB_VILLAGE_SPRING );
	atbparser.AddAtbInfo( TEXT("MAPTYPE_ATB_VILLAGE_SUMMER"), MAPTYPE_ATB_VILLAGE_SUMMER );
	atbparser.AddAtbInfo( TEXT("MAPTYPE_ATB_VILLAGE_AUTUMN"), MAPTYPE_ATB_VILLAGE_AUTUMN );
	atbparser.AddAtbInfo( TEXT("MAPTYPE_ATB_VILLAGE_WINTER"), MAPTYPE_ATB_VILLAGE_WINTER );
	atbparser.AddAtbInfo( TEXT("MAPTYPE_ATB_HUNT"), MAPTYPE_ATB_HUNT );


	int iMapUnique;
	TCHAR szMapTypeString[ 64 ];
	TCHAR szMapTypeAttb[ 128 ];
	TCHAR szSlot[ 9 ][ 32 ];

	int	iCenterX;
	int iCenterY;
	int iGateX;
	int iGateY;
	int iLevel;
	int iCost;
	
	DWORD dwMapTypeAtb;

	NDataLoader		dataloader;

	//KHY - 0810 - �ؽ�Ʈ ���ҽ� ��ȣȭ.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		BOOL bNCR2 = FALSE;
		FILE* fp2 = _tfopen( TEXT("Data/MapTypeInfo_Village.txt"), TEXT("rb") );
		if( fp2 ) {
			BYTE m[3] = {0};
			fread( m, 1, 3, fp2 );
			fclose( fp2 );
			bNCR2 = (m[0] == 'N' && m[1] == 'C' && m[2] == 'R');
		}
		if( bNCR2 )
			dataloader.LoadDataFromCryptFile( TEXT("Data/MapTypeInfo_Village.txt") );
		else
			dataloader.LoadDataFromFile( TEXT("Data/MapTypeInfo_Village.txt") );
	}
	else
	{
		dataloader.LoadDataFromFile( TEXT("Data/MapTypeInfo_Village.txt") );
	}

	NDataTypeInfo	datatypeinfo[] = 
	{ 
		NDATA_MBSTRING,	szMapTypeString,	64,		// MapTypeString
		NDATA_MBSTRING,	szMapTypeAttb,		128,	// MapTypeAttb
		NDATA_INT32,	&iCenterX,	4,		// CenterX
		NDATA_INT32,	&iCenterY,	4,		// CenterY
		NDATA_INT32,	&iGateX,	4,		// GateX
		NDATA_INT32,	&iGateY,	4,		// GateY
		NDATA_INT32,	&iCost,		4,		// Cost
		NDATA_INT32,	&iLevel,	4,		// Level
		NDATA_MBSTRING,	szSlot[0],	32,		// Slot0
		NDATA_MBSTRING,	szSlot[1],	32,		// Slot1
		NDATA_MBSTRING,	szSlot[2],	32,		// Slot2
		NDATA_MBSTRING,	szSlot[3],	32,		//Slot3
		NDATA_MBSTRING,	szSlot[4],	32,		// Slot4
		NDATA_MBSTRING,	szSlot[5],	32,		// Slot5
		NDATA_MBSTRING,	szSlot[6],	32,		// Slot6
		NDATA_MBSTRING,	szSlot[7],	32,		// Slot7
		NDATA_MBSTRING,	szSlot[8],  32,		// Slot8
		0,0,0
	};							

	dataloader.RegDataType( datatypeinfo );

	while( !dataloader.IsEndOfData() )
	{

		if( dataloader.ReadData() ) 
		{


			dwMapTypeAtb = atbparser.GetValueByString( szMapTypeAttb );

			// �ؽ����̺����� MayTypeString�� �ش��ϴ� MapUnique�� ã�´�
			if( GetGlobalHashTableString()->Find( szMapTypeString, iMapUnique ) ) {

				int hunt = ( dwMapTypeAtb & MAPTYPE_ATB_HUNT ) ? 1 : 0;
				int atb = 0;
				
				if( dwMapTypeAtb & MAPTYPE_ATB_VILLAGE_SPRING )
				{
					atb = VILLAGETYPE_SPRING_NORMAL;

				} else if( dwMapTypeAtb & MAPTYPE_ATB_VILLAGE_AUTUMN )
				{
					atb = VILLAGETYPE_AUTUMN_NORMAL;

				} else if( dwMapTypeAtb & MAPTYPE_ATB_VILLAGE_WINTER )
				{
					atb = VILLAGETYPE_WINTER_NORMAL;

				}
									
				// MapType�� �߰��Ѵ�
				if( SetMapType( iMapUnique, NULL, iCenterX, iCenterY, iGateX, iGateY, 0,
								 atb, 0, iLevel, iCost, hunt ) ) {
				}

			} else {

				// ��� �ȵ� ���̴�..
			}

		}

	}

}

void cltMapTypeManager::LoadMapTypeInfo_Hunt()
{

}

bool cltMapTypeManager::AddMapType(SI32 mapunique, TCHAR* maptypename, SI32 centerx, SI32 centery, SI32 gatex, SI32 gatey,
								   POINT* ppoint, SI32 atb, SI32 adapt, SI32 level, GMONEY money,SI32 huntadapt)
{
	if( mapunique <= 0 || mapunique >= MAX_MAPTYPE_NUMBER ) return false;

	if( pclMapTypeInfo[ mapunique ] ) return false;

	pclMapTypeInfo[ mapunique ] = new cltMapTypeInfo;
	
	pclMapTypeInfo[ mapunique ]->SetMapType( 
		mapunique, 
		maptypename, 
		centerx, 
		centery, 
		gatex, 
		gatey, 
		ppoint,
		atb, 
		adapt, 
		level, 
		money,
		huntadapt );

	return true;
}

bool cltMapTypeManager::SetMapType( SI32 mapunique, TCHAR* maptypename, SI32 centerx, SI32 centery, SI32 gatex, SI32 gatey,
								    POINT* ppoint, SI32 atb, SI32 adapt, SI32 level, GMONEY money,SI32 huntadapt)
{
	if( mapunique <= 0 || mapunique >= MAX_MAPTYPE_NUMBER ) return false;

	if( pclMapTypeInfo[ mapunique ] == NULL ) return false;

	pclMapTypeInfo[ mapunique ]->SetMapType( 
		mapunique, 
		maptypename, 
		centerx, 
		centery, 
		gatex, 
		gatey, 
		ppoint,
		atb, 
		adapt, 
		level, 
		money,
		huntadapt );


	return true;
}

TCHAR* cltMapTypeManager::GetMapTypeNameFromUnique( SI32 mapunique )
{
	if( mapunique <= 0 || mapunique >= MAX_MAPTYPE_NUMBER ) return NULL;

#ifdef _DEBUG
	if( NULL == pclMapTypeInfo[ mapunique ] ) 
	{	
		TCHAR ErrorBuffer[128] = "";
		StringCchPrintf( ErrorBuffer, sizeof(ErrorBuffer), "[ %d ]�� ����ũ ������ ����!!", mapunique );
		MessageBox( NULL, ErrorBuffer, "����!", 0 );
		return NULL;
	}
#else
	if( NULL == pclMapTypeInfo[ mapunique ] ) return NULL;
#endif

	return pclMapTypeInfo[ mapunique ]->szMapTypeName;
}


