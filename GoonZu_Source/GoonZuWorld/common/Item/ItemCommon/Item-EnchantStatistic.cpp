//---------------------------------
// 2004/5/19 김태곤
//---------------------------------
#include "cltItem.h"
#include "CommonLogic.h"
#include "../DBManager/GameDBManager_World/dbmsg.h"


#ifdef MAKE_BY_MUNIN //2007-05-16

void cltItemManagerCommon::WriteEnchantStatisticAction(bool backUpEnchantDataLoaded , _SYSTEMTIME sTime )
{

	TCHAR fileNameBuffer[256];
	fileNameBuffer[0] = 0;

	// 서버가 켜진후 기존 백업된 데이터를 읽어 온다
	static SI32 lastEnchantStatisticWrittenDay = 0;

	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\Enchant_backup.bak") , sTime.wYear , sTime.wMonth , sTime.wDay );

	if( backUpEnchantDataLoaded == false )
	{
		ReadEnchantStatisticBackUp( fileNameBuffer ,sTime);
		lastEnchantStatisticWrittenDay	= sTime.wDay; // 초기화된 날짜를 마지막 업데이트 
	}

	// 날짜가 바뀌면 일단위 통계 데이터를 초기화 해준다.
	if( lastEnchantStatisticWrittenDay != sTime.wDay )
	{
		clItemEnchantStatistic.InitEnchantStatistic();
		lastEnchantStatisticWrittenDay = sTime.wDay;
	}

	// 통계 데이터를 쓴다
	WriteEnchantStatistic(sTime );
    
	// 백업 통계 데이터를 쓴다
	WriteEnchantStatisticBackUp( fileNameBuffer , sTime  );

	return;
}

void cltItemManagerCommon::WriteEnchantStatistic(_SYSTEMTIME sTime )
{
	SI32 i;
	TCHAR fileNameBuffer[256];


	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\Enchant_Result_SFDB_%04d%02d%02d.log") , sTime.wYear , sTime.wMonth , sTime.wDay );
	FILE *fp_SFDB = fopen( fileNameBuffer, "wt" );

	StringCchPrintf( fileNameBuffer , 256 , TEXT("Config\\Enchant_Result_Material_%04d%02d%02d.log") , sTime.wYear , sTime.wMonth , sTime.wDay );
	FILE *fp_Material = fopen( fileNameBuffer, "wt" );	

//if ( ConstServiceArea_China == pclClient->siServiceArea )

//	FILE *fp_SFDB = fopen( fileNameBuffer, "wt" );
//	FILE *fp_Material = fopen( "Config/Enchant_Result_Material.txt", "wt" );

	if (( fp_SFDB == NULL )||( fp_Material == NULL ))
	{
		return;
	}


	// 맨 첫줄. 
	//S = Sucess, F = Fail,  D = DownGrade, B= Broken

	fprintf( fp_SFDB, TEXT("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\n"), 
		TEXT("Name"),TEXT("Unique"), 
		TEXT("1_Lv_S"), TEXT("1_Lv_F"),TEXT("1_Lv_D"), TEXT("1_Lv_B"), 
		TEXT("2_Lv_S"), TEXT("2_Lv_F"),TEXT("2_Lv_D"), TEXT("2_Lv_B"), 
		TEXT("3_Lv_S"), TEXT("3_Lv_F"),TEXT("3_Lv_D"), TEXT("3_Lv_B"),
		TEXT("4_Lv_S"), TEXT("4_Lv_F"),TEXT("4_Lv_D"), TEXT("4_Lv_B"),
		TEXT("5_Lv_S"), TEXT("5_Lv_F"),TEXT("5_Lv_D"), TEXT("5_Lv_B"),
		TEXT("6_Lv_S"), TEXT("6_Lv_F"),TEXT("6_Lv_D"), TEXT("6_Lv_B"),
		TEXT("7_Lv_S"), TEXT("7_Lv_F"),TEXT("7_Lv_D"), TEXT("7_Lv_B"),
		TEXT("8_Lv_S"), TEXT("8_Lv_F"),TEXT("8_Lv_D"), TEXT("8_Lv_B"),
		TEXT("9_Lv_S"), TEXT("9_Lv_F"),TEXT("9_Lv_D"), TEXT("9_Lv_B"),
		TEXT("10_Lv_S"), TEXT("10_Lv_F"),TEXT("10_Lv_D"), TEXT("10_Lv_B"));

	fprintf( fp_Material, TEXT("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\n"),
		TEXT("Name"),TEXT("Unique"), 
		TEXT("1_EM_F"), TEXT("1_EM_WT"),TEXT("1_EM_WD"), TEXT("1_EM_L"),TEXT("1_BS"), 
		TEXT("2_EM_F"), TEXT("2_EM_WT"),TEXT("2_EM_WD"), TEXT("2_EM_L"),TEXT("2_BS"),
		TEXT("3_EM_F"), TEXT("3_EM_WT"),TEXT("3_EM_WD"), TEXT("3_EM_L"),TEXT("3_BS"),
		TEXT("4_EM_F"), TEXT("4_EM_WT"),TEXT("4_EM_WD"), TEXT("4_EM_L"),TEXT("4_BS"),
		TEXT("5_EM_F"), TEXT("5_EM_WT"),TEXT("5_EM_WD"), TEXT("5_EM_L"),TEXT("5_BS"),
		TEXT("6_EM_F"), TEXT("6_EM_WT"),TEXT("6_EM_WD"), TEXT("6_EM_L"),TEXT("6_BS"),
              TEXT("7_EM_F"), TEXT("7_EM_WT"),TEXT("7_EM_WD"), TEXT("7_EM_L"),TEXT("7_BS"),
              TEXT("8_EM_F"), TEXT("8_EM_WT"),TEXT("8_EM_WD"), TEXT("8_EM_L"),TEXT("8_BS"),
              TEXT("9_EM_F"), TEXT("9_EM_WT"),TEXT("9_EM_WD"), TEXT("9_EM_L"),TEXT("9_BS"),
              TEXT("10_EM_F"), TEXT("10_EM_WT"),TEXT("10_EM_WD"), TEXT("10_EM_L"),TEXT("10_BS"));
 
/*
		TEXT("2_Lv_F"), TEXT("3_Lv_S"), TEXT("3_Lv_F"), TEXT("4_Lv_S"), TEXT("4_Lv_F"), TEXT("5_Lv_S"),TEXT("5_Lv_F"),TEXT("6_Lv_S"),TEXT("6_Lv_F"),TEXT("7_Lv_S"),TEXT("7_Lv_F")
		,TEXT("Fire Stone"),TEXT("Water Stone"),TEXT("Wind Stone"),TEXT("Land Stone"),TEXT("Blessing Stone"));
*/
	for (i = 0; i < MAX_ITEMINFO_NUMBER; ++i )
	{
		if(pclItemInfo[i] == NULL)continue;

		SI32 ItemType = pclItemInfo[i]->siType;
		bool flag = true;
		switch(ItemType) 
		{
		case ITEMTYPE_SWORD:
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_STAFF:
		case ITEMTYPE_AXE:	
		case ITEMTYPE_BOW:	
		case ITEMTYPE_GUN:
		case ITEMTYPE_ARMOUR:
		case ITEMTYPE_DRESS:
		case ITEMTYPE_BELT:
		case ITEMTYPE_SHOES:
		case ITEMTYPE_HELMET:
		case ITEMTYPE_HAT:
		case ITEMTYPE_RING:
		case ITEMTYPE_CRYSTAL:
		case ITEMTYPE_MANTLE:
			break;

		default:
			flag = false;
			break;
		}
		if(flag == false)continue;		

		fprintf( fp_SFDB, "%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n", 
			pclItemInfo[i]->GetName(), 
			pclItemInfo[i]->clItem.siUnique,
		 	
			clItemEnchantStatistic.siGradeSucess[0][i], 
			clItemEnchantStatistic.siGradeFail[0][i], 
			clItemEnchantStatistic.siGradeDownGrade[0][i], 
			clItemEnchantStatistic.siGradeBroken[0][i], 						

			clItemEnchantStatistic.siGradeSucess[1][i], 
			clItemEnchantStatistic.siGradeFail[1][i], 
			clItemEnchantStatistic.siGradeDownGrade[1][i], 
			clItemEnchantStatistic.siGradeBroken[1][i], 

			clItemEnchantStatistic.siGradeSucess[2][i], 
			clItemEnchantStatistic.siGradeFail[2][i], 
			clItemEnchantStatistic.siGradeDownGrade[2][i], 
			clItemEnchantStatistic.siGradeBroken[2][i], 

			clItemEnchantStatistic.siGradeSucess[3][i], 
			clItemEnchantStatistic.siGradeFail[3][i], 
			clItemEnchantStatistic.siGradeDownGrade[3][i], 
			clItemEnchantStatistic.siGradeBroken[3][i], 

			clItemEnchantStatistic.siGradeSucess[4][i], 
			clItemEnchantStatistic.siGradeFail[4][i], 
			clItemEnchantStatistic.siGradeDownGrade[4][i], 
			clItemEnchantStatistic.siGradeBroken[4][i], 

			clItemEnchantStatistic.siGradeSucess[5][i], 
			clItemEnchantStatistic.siGradeFail[5][i], 
			clItemEnchantStatistic.siGradeDownGrade[5][i], 
			clItemEnchantStatistic.siGradeBroken[5][i], 

			clItemEnchantStatistic.siGradeSucess[6][i], 
			clItemEnchantStatistic.siGradeFail[6][i], 
			clItemEnchantStatistic.siGradeDownGrade[6][i], 
			clItemEnchantStatistic.siGradeBroken[6][i], 

			clItemEnchantStatistic.siGradeSucess[7][i], 
			clItemEnchantStatistic.siGradeFail[7][i], 
			clItemEnchantStatistic.siGradeDownGrade[7][i], 
			clItemEnchantStatistic.siGradeBroken[7][i], 

			clItemEnchantStatistic.siGradeSucess[8][i], 
			clItemEnchantStatistic.siGradeFail[8][i], 
			clItemEnchantStatistic.siGradeDownGrade[8][i], 
			clItemEnchantStatistic.siGradeBroken[8][i], 		

			clItemEnchantStatistic.siGradeSucess[9][i], 
			clItemEnchantStatistic.siGradeFail[9][i], 
			clItemEnchantStatistic.siGradeDownGrade[9][i], 
			clItemEnchantStatistic.siGradeBroken[9][i] 
			);

		fprintf( fp_Material, "%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n", 
			pclItemInfo[i]->GetName(), 
			pclItemInfo[i]->clItem.siUnique,

			clItemEnchantStatistic.siEnchantMaterial_EM_F[0][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[0][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[0][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[0][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[0][i], 
			
			clItemEnchantStatistic.siEnchantMaterial_EM_F[1][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[1][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[1][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[1][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[1][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[2][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[2][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[2][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[2][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[2][i], 	

			clItemEnchantStatistic.siEnchantMaterial_EM_F[3][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[3][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[3][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[3][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[3][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[4][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[4][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[4][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[4][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[4][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[5][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[5][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[5][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[5][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[5][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[6][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[6][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[6][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[6][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[6][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[7][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[7][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[7][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[7][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[7][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[8][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[8][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[8][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[8][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[8][i], 

			clItemEnchantStatistic.siEnchantMaterial_EM_F[9][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WT[9][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_WD[9][i], 
			clItemEnchantStatistic.siEnchantMaterial_EM_L[9][i], 
			clItemEnchantStatistic.siEnchantMaterial_BS[9][i] 
			);


	}
	fclose( fp_SFDB);
	fclose( fp_Material);

	return;

}

// 아이템 거래 통계를 낸다. 
void cltItemManagerCommon::AddEnchantNum(SI32 itemunique, SI32 ItemGrade, DBMaterialInfo* pclinfo, SI32 sucess)
{

	if(itemunique <= 0)return ;
	SI32 ItemType = GetItemType(itemunique);
	switch(ItemType) 
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:	
	case ITEMTYPE_BOW:	
	case ITEMTYPE_GUN:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:
	case ITEMTYPE_RING:
	case ITEMTYPE_CRYSTAL:
	case ITEMTYPE_AVATAR_MANTLE:	// 아바타 망토
	case ITEMTYPE_AVATAR_DRESS:		// 아바타 의복
	case ITEMTYPE_AVATAR_HAT:		// 아바타 모자
	case ITEMTYPE_AVATAR_NECK:		// 아바타 목걸이
	case ITEMTYPE_AVATAR_RING:		// 아바타 반지
		break;

	default:
		return;
	}

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	//ItemGrade = ItemGrade - 1;
/*
	if(ItemGrade > 7 || ItemGrade < 0) return;

	if(sucess ==true)
	{
		clItemEnchantStatistic.siGradeSucess[ItemGrade][ref] += 1;
	}
	else
	{
		clItemEnchantStatistic.siGradeFail[ItemGrade][ref] += 1;
	}
*/
	if(ItemGrade > MAX_ITEMGRADE || ItemGrade < 0) return;

	switch(sucess) //인첸트 결과 타입을 구분한다.
	{
		case  ENCHANT_SUCCESS :
			clItemEnchantStatistic.siGradeSucess[ItemGrade][ref] += 1;
			break;
		
		case  ENCHANT_FAIL :
			clItemEnchantStatistic.siGradeFail[ItemGrade][ref] += 1;
			break;

		case ENCHANT_DOWNGRADE :
			clItemEnchantStatistic.siGradeDownGrade[ItemGrade][ref] += 1;
			break;

		case ENCHANT_BROKEN :
			clItemEnchantStatistic.siGradeBroken[ItemGrade][ref] += 1;
			break;
	}


	for( SI32 i = 0; i < 4; i++)
	{
		if(pclinfo[i].siItemUnique > 0 )
		{
			switch(pclinfo[i].siItemUnique)
			{
				case ITEMUNIQUE(5350) : //Fire Stone
					clItemEnchantStatistic.siEnchantMaterial_EM_F[ItemGrade][ref] += pclinfo[i].siUseItemNum ;
					break;

				case ITEMUNIQUE(5355) : //Water Stone
					clItemEnchantStatistic.siEnchantMaterial_EM_WT[ItemGrade][ref] += pclinfo[i].siUseItemNum ;
					break;	

				case ITEMUNIQUE(5360) : //Wind Stone
					clItemEnchantStatistic.siEnchantMaterial_EM_WD[ItemGrade][ref] += pclinfo[i].siUseItemNum ;
					break;

				case ITEMUNIQUE(5365) : //Land Stone
					clItemEnchantStatistic.siEnchantMaterial_EM_L[ItemGrade][ref] += pclinfo[i].siUseItemNum ;
					break;

				case ITEMUNIQUE(5370) : //Blessing Stone
					clItemEnchantStatistic.siEnchantMaterial_BS[ItemGrade][ref] += pclinfo[i].siUseItemNum ;
					break;
			}		

		}		
	}

}
// 통계 데이터를 파일로 백업시킨다 ( 서버를 다시 켜도 유지되게끔 )
void cltItemManagerCommon::WriteEnchantStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime )
{
	FILE *fp = _tfopen( filename , TEXT("wb") );

	if ( fp == NULL )
	{
		return;
	}

	stServerBackUpFileHeader clBackUpFileHeader;
	StringCchPrintf(clBackUpFileHeader.szDesc, 32 , TEXT("Statistics%04d%02d%02d"), sTime.wYear , sTime.wMonth , sTime.wDay );
	clBackUpFileHeader.siVersion = SERVER_BACKUPFILE_HEADER_VERSION;

	fwrite(&clBackUpFileHeader , sizeof(stServerBackUpFileHeader) , 1 , fp);
	fwrite(&clItemEnchantStatistic , sizeof(clItemEnchantStatistic) , 1 , fp);

	fclose( fp );
	
}

void cltItemManagerCommon::ReadEnchantStatisticBackUp( TCHAR * filename, _SYSTEMTIME sTime )
{

	TCHAR buffer[256];
	buffer[0] = 0;

	FILE *fp = _tfopen( filename , TEXT("rb") );

	if ( fp == NULL )
	{
		return;
	}
    
	stServerBackUpFileHeader clBackUpFileHeader;
	memset( &clBackUpFileHeader , 0 , sizeof(stServerBackUpFileHeader) );

	fread( &clBackUpFileHeader , sizeof(stServerBackUpFileHeader) , 1 , fp );
	clBackUpFileHeader.szDesc[31] = 0; // Insert NULL

	// 오늘 날짜를 문자열로 만들어 헤더 정보와 비교

	StringCchPrintf( buffer, 256 , TEXT("Statistics%04d%02d%02d"), sTime.wYear , sTime.wMonth , sTime.wDay );


	// 파일의 헤더가 적절하지 않다면 읽지 않는다.
	if( _tcscmp( clBackUpFileHeader.szDesc , buffer ) != 0 ) return;


	// 파일 버젼이 다르면 읽지 않는다
	if( clBackUpFileHeader.siVersion != SERVER_BACKUPFILE_HEADER_VERSION ) return;

	fread( &clItemEnchantStatistic , sizeof( clItemEnchantStatistic ) , 1 , fp );
	

	fclose( fp );

}



#else
void cltItemManagerCommon::WriteEnchantStatistic()
{
	SI32 i;

	FILE *fp = fopen( "Config/EnchantSF.log", "wt" );

	if ( fp == NULL )
	{
		return;
	}

	// 맨 첫줄. 
	fprintf( fp, TEXT("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n"), TEXT("Name"), TEXT("1_Lv_S"), TEXT("1_Lv_F"), TEXT("2_Lv_S"),
		TEXT("2_Lv_F"), TEXT("3_Lv_S"), TEXT("3_Lv_F"), TEXT("4_Lv_S"), TEXT("4_Lv_F"), TEXT("5_Lv_S"),TEXT("5_Lv_F"),TEXT("6_Lv_S"),TEXT("6_Lv_F"),TEXT("7_Lv_S"),TEXT("7_Lv_F")
		,TEXT("Fire Stone"),TEXT("Water Stone"),TEXT("Wind Stone"),TEXT("Land Stone"),TEXT("Blessing Stone"));

	for (i = 0; i < MAX_ITEMINFO_NUMBER; ++i )
	{
		if(pclItemInfo[i] == NULL)continue;

		SI32 ItemType = pclItemInfo[i]->siType;
		bool flag = true;
		switch(ItemType) 
		{
		case ITEMTYPE_SWORD:
		case ITEMTYPE_SPEAR:
		case ITEMTYPE_STAFF:
		case ITEMTYPE_AXE:	
		case ITEMTYPE_BOW:	
		case ITEMTYPE_GUN:
		case ITEMTYPE_ARMOUR:
		case ITEMTYPE_DRESS:
		case ITEMTYPE_BELT:
		case ITEMTYPE_SHOES:
		case ITEMTYPE_HELMET:
		case ITEMTYPE_HAT:
		case ITEMTYPE_RING:
		case ITEMTYPE_CRYSTAL:
		case ITEMTYPE_MANTLE:
			break;

		default:
			flag = false;
			break;
		}
		if(flag == false)continue;		



		fprintf( fp, "%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
			pclItemInfo[i]->GetName(), 
			clItemEnchantStatistic.siGradeSucess[0][i], 
			clItemEnchantStatistic.siGradeFail[0][i], 
			clItemEnchantStatistic.siGradeSucess[1][i], 
			clItemEnchantStatistic.siGradeFail[1][i], 
			clItemEnchantStatistic.siGradeSucess[2][i], 
			clItemEnchantStatistic.siGradeFail[2][i], 
			clItemEnchantStatistic.siGradeSucess[3][i], 
			clItemEnchantStatistic.siGradeFail[3][i], 
			clItemEnchantStatistic.siGradeSucess[4][i], 
			clItemEnchantStatistic.siGradeFail[4][i], 
			clItemEnchantStatistic.siGradeSucess[5][i], 
			clItemEnchantStatistic.siGradeFail[5][i], 
			clItemEnchantStatistic.siGradeSucess[6][i], 
			clItemEnchantStatistic.siGradeFail[6][i] 
			,clItemEnchantStatistic.siEnchantMaterial[0][i] 
			,clItemEnchantStatistic.siEnchantMaterial[1][i]
			,clItemEnchantStatistic.siEnchantMaterial[2][i]
			,clItemEnchantStatistic.siEnchantMaterial[3][i]
			,clItemEnchantStatistic.siEnchantMaterial[4][i]
			);

	}

	fclose( fp );

	return;
}

// 아이템 거래 통계를 낸다. 
void cltItemManagerCommon::AddEnchantNum(SI32 itemunique, SI32 ItemGrade, DBMaterialInfo* pclinfo, bool sucess)
{

	if(itemunique <= 0)return ;
	SI32 ItemType = GetItemType(itemunique);
	switch(ItemType) 
	{
	case ITEMTYPE_SWORD:
	case ITEMTYPE_SPEAR:
	case ITEMTYPE_STAFF:
	case ITEMTYPE_AXE:	
	case ITEMTYPE_BOW:	
	case ITEMTYPE_GUN:
	case ITEMTYPE_ARMOUR:
	case ITEMTYPE_DRESS:
	case ITEMTYPE_BELT:
	case ITEMTYPE_SHOES:
	case ITEMTYPE_HELMET:
	case ITEMTYPE_HAT:
	case ITEMTYPE_RING:
	case ITEMTYPE_CRYSTAL:
	case ITEMTYPE_AVATAR_MANTLE:	// 아바타 망토
	case ITEMTYPE_AVATAR_DRESS:		// 아바타 의복
	case ITEMTYPE_AVATAR_HAT:		// 아바타 모자
	case ITEMTYPE_AVATAR_RING:		// 아바타 반지
	case ITEMTYPE_AVATAR_NECK:		// 아바타 목걸이
		break;

	default:
		return;
	}

	SI32 ref = FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	//ItemGrade = ItemGrade - 1;
	if(ItemGrade > 7 || ItemGrade < 0) return;

	if(sucess ==true)
	{
		clItemEnchantStatistic.siGradeSucess[ItemGrade][ref] += 1;
	}
	else
	{
		clItemEnchantStatistic.siGradeFail[ItemGrade][ref] += 1;
	}


	for( SI32 i = 0; i < 4; i++)
	{
		if(pclinfo[i].siItemUnique > 0 )
		{
			if( pclinfo[i].siItemUnique == ITEMUNIQUE(5350) )
			{
				clItemEnchantStatistic.siEnchantMaterial[0][ref] += pclinfo[i].siUseItemNum ;
			}
			else if( pclinfo[i].siItemUnique == ITEMUNIQUE(5355) )
			{
				clItemEnchantStatistic.siEnchantMaterial[1][ref] += pclinfo[i].siUseItemNum;
			}
			else if( pclinfo[i].siItemUnique == ITEMUNIQUE(5360) )
			{
				clItemEnchantStatistic.siEnchantMaterial[2][ref] += pclinfo[i].siUseItemNum;
			}
			else if( pclinfo[i].siItemUnique == ITEMUNIQUE(5365) )
			{
				clItemEnchantStatistic.siEnchantMaterial[3][ref] += pclinfo[i].siUseItemNum;
			}
			else if( pclinfo[i].siItemUnique == ITEMUNIQUE(5370) )
			{
				clItemEnchantStatistic.siEnchantMaterial[4][ref] += pclinfo[i].siUseItemNum;
			}

		}		
	}

}

#endif

