//---------------------------------
// 2003/8/5 김태곤
//---------------------------------

#include "FieldObject.h"
//Common
#include "CommonHeader.h"

#include "..\CommonLogic\CommonLogic.h"

extern cltCommonLogic* pclClient;

//------------------------------------------------------------
// cltVillageStrucureInfo
//------------------------------------------------------------
cltVillageStructureInfo::cltVillageStructureInfo(SI32 unique, TCHAR* szname, SI32 ranktype)
{
	siUnique			= unique;
	siRankType			= ranktype;
	siFieldObjectNumber	= 0;
	siUPFieldObjectNumber = 0;
	MStrCpy( szName, szname, MAX_STRUCTURE_NAME_LENGTH );
}

cltVillageStructureInfo::~cltVillageStructureInfo()
{

}


//sicenterx, sicentery를 중심으로 삼고 있는 건물이 건물 세트의 키가 되는 건물인지 확인한다. 
bool cltVillageStructureInfo::IsKeyStructure(SI32 sicenterx, SI32 sicentery)
{
	SI32 i;

	for(i = 0; i< siFieldObjectNumber;i++)
	{
		if(clUnit[i].siX == sicenterx && clUnit[i].siY == sicentery
		&& clUnit[i].bKeySwitch)
		return true;
	}

	for(i = 0; i< siUPFieldObjectNumber;i++)
	{
		if(clUPUnit[i].siX == sicenterx && clUPUnit[i].siY == sicentery
			&& clUPUnit[i].bKeySwitch)
			return true;
	}

	return false;
}

// FieldObject를 StructureInfo에 추가한다. 
BOOL cltVillageStructureInfo::Add(SI32 six, SI32 siy, SI32 sifieldobjectunique, bool bkeyswitch)
{
	if(siFieldObjectNumber >= MAX_FIELD_OBJECT_PER_STRUCTURE_SET) return FALSE;
	
	clUnit[siFieldObjectNumber].siX				= six;
	clUnit[siFieldObjectNumber].siY				= siy;
	clUnit[siFieldObjectNumber].siFieldObjectArray	= sifieldobjectunique;
	clUnit[siFieldObjectNumber].bKeySwitch		= bkeyswitch;

	
	siFieldObjectNumber++;

	return TRUE;
}

BOOL cltVillageStructureInfo::AddUP(SI32 six, SI32 siy, SI32 sifieldobjectunique, bool bkeyswitch )
{
	if(siUPFieldObjectNumber >= MAX_FIELD_OBJECT_PER_STRUCTURE_SET) return FALSE;

	clUPUnit[siUPFieldObjectNumber].siX					= six;
	clUPUnit[siUPFieldObjectNumber].siY					= siy;
	clUPUnit[siUPFieldObjectNumber].siFieldObjectArray	= sifieldobjectunique;
	clUPUnit[siUPFieldObjectNumber].bKeySwitch			= bkeyswitch;


	siUPFieldObjectNumber++;

	return TRUE;
}

void cltVillageStructureInfo::SetConstructionInfo( SI32 needVillageLevelRate, GMONEY needMoney, BOOL destroy, SI32 NeedVillageScore )
{
	siNeedVillageLevelRate = needVillageLevelRate;
	siNeedMoney = needMoney;
	bDestroyable = destroy;
	siNeedVillageScore = NeedVillageScore;
}

// 건설에 필요한 마을 레벨을 구한다. 
SI32 cltVillageStructureInfo::GetNeedVillagelevel()
{
	return (100 - siNeedVillageLevelRate)  * max(10, pclClient->siBestVillageLevel) / 100 ;
}

//-----------------------------------------------------------
// cltVillageStructureSet
//-----------------------------------------------------------
//마을 건물들 기본 설정을 해준다. 파신시 건물 파괴 유무, 지을때 가격들도 설정 한다.
cltVillageStructureSet::cltVillageStructureSet()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		pclVillageStructureInfo[i] = NULL;
	}

	//------------
	// 상단(관청)
	//------------
	TCHAR* pText = GetTxtFromMgr(2830);
	pclVillageStructureInfo[0]	= new cltVillageStructureInfo(CITYHALL_UNIQUE, pText, RANKTYPE_CITYHALL);
	pclVillageStructureInfo[0]->SetConstructionInfo( 100, 50000000, FALSE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_NHNChina:
	case ConstServiceArea_Taiwan:
		pclVillageStructureInfo[0]->Add(5, 6, 7251, true);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[0]->Add(2, 7, 7201, true);
		pclVillageStructureInfo[0]->Add(7, 6, 7202);
		pclVillageStructureInfo[0]->Add(7, 2, 7202);
		pclVillageStructureInfo[0]->Add(6, 4, 7203);
		pclVillageStructureInfo[0]->Add(4, 6, 7204);
		pclVillageStructureInfo[0]->Add(6, 6, 7204);
		pclVillageStructureInfo[0]->Add(4, 1, 7204);
		pclVillageStructureInfo[0]->Add(6, 1, 7204);
		break;
	default:		// 글로벌, 한국
		pclVillageStructureInfo[0]->Add(5, 6, 7285, true);
		pclVillageStructureInfo[0]->Add(7, 1, 7286);
		pclVillageStructureInfo[0]->Add(7, 7, 7286);
		pclVillageStructureInfo[0]->Add(7, 3, 7287);
		pclVillageStructureInfo[0]->Add(7, 5, 7287);
		pclVillageStructureInfo[0]->Add(0, 2, 7288);
		break;
	}

	// 전장(은행) 
	pText = GetTxtFromMgr(1437);
	pclVillageStructureInfo[1]	= new cltVillageStructureInfo(BANK_UNIQUE, pText, RANKTYPE_BANK);	
	if(pclClient->IsCountrySwitch(Switch_BasicStructureSet)){
		pclVillageStructureInfo[1]->SetConstructionInfo( 100, 100000000, FALSE );//마을 파산이 되어도 사라지지 않도록 변경
	}
	else{
		pclVillageStructureInfo[1]->SetConstructionInfo( 100, 100000000, TRUE );//내구도가 떨어지면 사라짐
	}

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[1]->Add(3, 6, 7205, true );
		pclVillageStructureInfo[1]->Add(6, 1, 7206 );
		pclVillageStructureInfo[1]->Add(5, 4, 7207 );
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[1]->Add(3, 6, 7205, true );
		pclVillageStructureInfo[1]->Add(5, 2, 7206 );
		pclVillageStructureInfo[1]->Add(6, 5, 7207 );
		break;
	default:
		pclVillageStructureInfo[1]->Add(3, 5, 7281, true);
		pclVillageStructureInfo[1]->Add(6, 2, 7282);
		pclVillageStructureInfo[1]->Add(6, 6, 7283);
		break;
	}

	

	//-----------
	// 여각(주식거래소)
	//-----------
	pText = GetTxtFromMgr(1469);
	pclVillageStructureInfo[2]	= new cltVillageStructureInfo(STOCK_UNIQUE, pText, RANKTYPE_STOCK);
	pclVillageStructureInfo[2]->SetConstructionInfo( 70, 100000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[2]->Add(5, 7, 7270, true);
		pclVillageStructureInfo[2]->Add(7, 1, 7271);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[2]->Add(4, 6, 7215, true);
		pclVillageStructureInfo[2]->Add(7, 1, 7216);
		pclVillageStructureInfo[2]->Add(3, 1, 7217);
		pclVillageStructureInfo[2]->Add(6, 5, 7218);
		break;
	default:
		pclVillageStructureInfo[2]->Add(6, 7, 7307, true);
		break;
	}
	
	//----------
	// 객주(시장)
	//----------
	pText = GetTxtFromMgr(1664);
	pclVillageStructureInfo[4]	= new cltVillageStructureInfo(MARKET_UNIQUE, pText, RANKTYPE_MARKET);
	pclVillageStructureInfo[4]->SetConstructionInfo( 80, 200000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[4]->Add(5, 5, 7262, true);
		pclVillageStructureInfo[4]->Add(7, 5, 7263);
		pclVillageStructureInfo[4]->Add(7, 1, 7264);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[4]->Add(4, 4, 7210, true);
		pclVillageStructureInfo[4]->Add(2, 7, 7211);
		pclVillageStructureInfo[4]->Add(7, 1, 7212);
		break;
	default:
		pclVillageStructureInfo[4]->Add(6, 6, 7297, true);
		break;
	}
	

	//------------
	// 시전(창고)
	//------------
	pText = GetTxtFromMgr(370);
	pclVillageStructureInfo[5]	= new cltVillageStructureInfo(HOUSE_UNIQUE, pText, RANKTYPE_HOUSE);
	pclVillageStructureInfo[5]->SetConstructionInfo( 100, 20000000, FALSE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[5]->Add(6, 4, 7257, true);
		pclVillageStructureInfo[5]->Add(6, 6, 7258);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[5]->Add(5, 4, 7208, true);
		pclVillageStructureInfo[5]->Add(6, 6, 7209);
		break;
	default:
		pclVillageStructureInfo[5]->Add(7, 6, 7293, true);
		break;
	}
	
	//------------
	// 수렵장(수련장)
	//------------
	pText = GetTxtFromMgr(1704);
	pclVillageStructureInfo[6]	= new cltVillageStructureInfo(HUNT_UNIQUE, pText, RANKTYPE_HUNT);
	pclVillageStructureInfo[6]->SetConstructionInfo( 80, 200000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[6]->Add(5, 3, 7259, true);
		pclVillageStructureInfo[6]->Add(4, 7, 7260);
		pclVillageStructureInfo[6]->Add(6, 4, 7261);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[6]->Add(5, 5, 7226, true);
		break;
	default:
		pclVillageStructureInfo[6]->Add(5, 4, 7294, true);
		pclVillageStructureInfo[6]->Add(2, 7, 7295);
		pclVillageStructureInfo[6]->Add(4, 7, 7296);
		break;
	}
	
	//----------
	// 역참(우체국)
	//----------
	pText = GetTxtFromMgr(1711);
	pclVillageStructureInfo[7]	= new cltVillageStructureInfo(POSTOFFICE_UNIQUE, pText, RANKTYPE_POSTOFFICE);
	pclVillageStructureInfo[7]->SetConstructionInfo( 80, 100000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[7]->Add(6, 3, 7266, true);
		pclVillageStructureInfo[7]->Add(6, 7, 7267);

		pclVillageStructureInfo[7]->AddUP(6, 3, 7266, true);
		pclVillageStructureInfo[7]->AddUP(6, 7, 7267);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[7]->Add(5, 1, 7213, true);
		pclVillageStructureInfo[7]->Add(6, 7, 7214);

		pclVillageStructureInfo[7]->AddUP(5, 1, 7213, true);
		pclVillageStructureInfo[7]->AddUP(6, 7, 7214);
		break;
	default:
		pclVillageStructureInfo[7]->Add(5, 6, 7300, true);
		pclVillageStructureInfo[7]->Add(7, 1, 7301);
		break;
	}

	//------------
	// 사복시(탈것거래소)
	//-----------
	pText = GetTxtFromMgr(1757);
	pclVillageStructureInfo[9]	= new cltVillageStructureInfo(HORSEMARKET_UNIQUE, pText, RANKTYPE_HORSEMARKET);
	pclVillageStructureInfo[9]->SetConstructionInfo( 40, 300000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[9]->Add(7, 7, 7272, true);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[9]->Add(7, 7, 7225, true);
		break;
	default:
		pclVillageStructureInfo[9]->Add(7, 7, 7308, true);
		break;
	}

	//------------
	// 거간(부동산)
	//-----------
	pText = GetTxtFromMgr(1775);
	pclVillageStructureInfo[10]	= new cltVillageStructureInfo(REALESTATEMARKET_UNIQUE, pText, RANKTYPE_REALESTATEMARKET);
	pclVillageStructureInfo[10]->SetConstructionInfo( 50, 100000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[10]->Add(5, 6, 7268, true);
		pclVillageStructureInfo[10]->Add(7, 2, 7269 );

		pclVillageStructureInfo[10]->AddUP(5, 6, 7268, true);
		pclVillageStructureInfo[10]->AddUP(7, 2, 7269 );
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[10]->Add(4, 5, 7227, true);
		pclVillageStructureInfo[10]->Add(7, 1, 7228 );

		pclVillageStructureInfo[10]->AddUP(4, 5, 7227, true);
		pclVillageStructureInfo[10]->AddUP(7, 1, 7228 );
		break;
	default:
		pclVillageStructureInfo[10]->Add(3, 5, 7302, true);
		pclVillageStructureInfo[10]->Add(5, 1, 7303 );
		pclVillageStructureInfo[10]->Add(5, 6, 7304 );
		break;
	}

	//------------
	// 장예원(장예원)
	//-----------
	pText = GetTxtFromMgr(1847);
	pclVillageStructureInfo[11]	= new cltVillageStructureInfo(SUMMONMARKET_UNIQUE, pText, RANKTYPE_SUMMONMARKET);
	
    if(pclClient->IsCountrySwitch(Switch_Summon_Market))
	{
		pclVillageStructureInfo[11]->SetConstructionInfo( 30, 500000000, TRUE );
	}
	else
	{
		pclVillageStructureInfo[11]->SetConstructionInfo( 30, 500000000, FALSE );
	}

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[11]->Add(2, 4, 7145, true);
		pclVillageStructureInfo[11]->Add(5, 1, 7146);
		pclVillageStructureInfo[11]->Add(3, 7, 7147);
		pclVillageStructureInfo[11]->Add(6, 7, 7148);
		pclVillageStructureInfo[11]->Add(3, 0, 7149);	
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[11]->Add(2, 4, 7145, true);
		pclVillageStructureInfo[11]->Add(5, 1, 7146);
		pclVillageStructureInfo[11]->Add(3, 7, 7147);
		pclVillageStructureInfo[11]->Add(6, 7, 7148);
		pclVillageStructureInfo[11]->Add(3, 0, 7149);
		break;
	default:
		pclVillageStructureInfo[11]->Add(2, 4, 7145, true);
		pclVillageStructureInfo[11]->Add(5, 1, 7146);
		pclVillageStructureInfo[11]->Add(3, 7, 7147);
		pclVillageStructureInfo[11]->Add(6, 7, 7148);
		pclVillageStructureInfo[11]->Add(3, 0, 7149);
		break;
	}

	//------------
	// 농장
	//-----------
	pText = GetTxtFromMgr(1826);
	pclVillageStructureInfo[13]	= new cltVillageStructureInfo(LAND_UNIQUE, pText, RANKTYPE_LAND);
	pclVillageStructureInfo[13]->SetConstructionInfo( 100, 100000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[13]->Add(5, 6, 7253, true);
		pclVillageStructureInfo[13]->Add(5, 3, 7253, true);
		pclVillageStructureInfo[13]->Add(7, 1, 7256);
		pclVillageStructureInfo[13]->Add(7, 7, 7255);
		pclVillageStructureInfo[13]->Add(2, 7, 7254);
		pclVillageStructureInfo[13]->Add(5, 7, 7254);
		pclVillageStructureInfo[13]->Add(2, 0, 7254);
		pclVillageStructureInfo[13]->Add(5, 0, 7254);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[13]->Add(5, 6, 7220, true);
		pclVillageStructureInfo[13]->Add(5, 3, 7220, true);
		pclVillageStructureInfo[13]->Add(7, 1, 7223);
		pclVillageStructureInfo[13]->Add(7, 7, 7222);
		pclVillageStructureInfo[13]->Add(2, 7, 7221);
		pclVillageStructureInfo[13]->Add(5, 7, 7221);
		pclVillageStructureInfo[13]->Add(2, 0, 7221);
		pclVillageStructureInfo[13]->Add(5, 0, 7221);
		break;
	default:
		pclVillageStructureInfo[13]->Add(4, 6, 7289, true);
		pclVillageStructureInfo[13]->Add(4, 3, 7289, true);
		pclVillageStructureInfo[13]->Add(7, 2, 7290 );
		pclVillageStructureInfo[13]->Add(6, 6, 7291 );
		pclVillageStructureInfo[13]->Add(2, 7, 7292 );
		pclVillageStructureInfo[13]->Add(5, 7, 7292 );
		pclVillageStructureInfo[13]->Add(2, 0, 7292 );
		pclVillageStructureInfo[13]->Add(5, 0, 7292 );
		break;
	}
	
	//------------
	// 내자시(연회장)
	//-----------
	pText = GetTxtFromMgr(1836);
	pclVillageStructureInfo[14]	= new cltVillageStructureInfo(FEAST_UNIQUE, pText, RANKTYPE_FEAST);
	pclVillageStructureInfo[14]->SetConstructionInfo( 50, 200000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[14]->Add(6, 6, 7252, true);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[14]->Add(4, 4, 7224, true);
		break;
	default:
		pclVillageStructureInfo[14]->Add(6, 6, 7284, true);
		break;
	}

	//------------
	// 비변사
	//-----------
	pText = GetTxtFromMgr(2831);
	pclVillageStructureInfo[15]	= new cltVillageStructureInfo(SUMMONHEROMARKET_UNIQUE, pText, RANKTYPE_SUMMONHEROMARKET);
	pclVillageStructureInfo[15]->SetConstructionInfo( 20, 30000000, FALSE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[15]->Add(3, 5, 7134, true);
		pclVillageStructureInfo[15]->Add(6, 0, 7135);
		pclVillageStructureInfo[15]->Add(7, 7, 7135);

		pclVillageStructureInfo[15]->AddUP(4, 4, 7134, true);
		pclVillageStructureInfo[15]->AddUP(6, 1, 7135);
		pclVillageStructureInfo[15]->AddUP(6, 6, 7135);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[15]->Add(3, 5, 7134, true);
		pclVillageStructureInfo[15]->Add(6, 0, 7135);
		pclVillageStructureInfo[15]->Add(7, 7, 7135);

		pclVillageStructureInfo[15]->AddUP(4, 4, 7134, true);
		pclVillageStructureInfo[15]->AddUP(6, 1, 7135);
		pclVillageStructureInfo[15]->AddUP(6, 6, 7135);
		break;
	default:
		pclVillageStructureInfo[15]->Add(3, 5, 7134, true);
		pclVillageStructureInfo[15]->Add(6, 0, 7135);
		pclVillageStructureInfo[15]->Add(7, 7, 7135);

		pclVillageStructureInfo[15]->AddUP(4, 4, 7134, true);
		pclVillageStructureInfo[15]->AddUP(6, 1, 7135);
		pclVillageStructureInfo[15]->AddUP(6, 6, 7135);
		break;
	}

	//------------
	// 광산-가칭
	//-----------
	pText = GetTxtFromMgr(2832);
	pclVillageStructureInfo[16]	= new cltVillageStructureInfo(MINE_UNIQUE, pText, RANKTYPE_MINE);
	pclVillageStructureInfo[16]->SetConstructionInfo( 100, 100000000, TRUE );

	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[16]->Add(6, 5, 7265,true);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[16]->Add(6, 4, 7219,true);
		pclVillageStructureInfo[16]->Add(7, 2, 3);
		break;
	default:
		pclVillageStructureInfo[16]->Add(7, 4, 7298, true);
		pclVillageStructureInfo[16]->Add(2, 5, 7299);
		pclVillageStructureInfo[16]->Add(6, 5, 7309);
		pclVillageStructureInfo[16]->Add(6, 0, 7310);	
		break;
	}
	//------------
	//  길드 (길드사무소)
	//------------
	pText = GetTxtFromMgr(5360);
	pclVillageStructureInfo[17]	= new cltVillageStructureInfo(GUILD_UNIQUE, pText, RANKTYPE_GUILD);
	pclVillageStructureInfo[17]->SetConstructionInfo( 100, 300000000, TRUE );	// 원본
	
	switch( pclClient->siServiceArea )
	{
	case ConstServiceArea_China:
	case ConstServiceArea_Taiwan:
	case ConstServiceArea_NHNChina:
		pclVillageStructureInfo[17]->Add(6, 4, 8847, true);
		break;
	case ConstServiceArea_Japan:
		pclVillageStructureInfo[17]->Add(5, 5, 7151, true);
		break;
	default:
		pclVillageStructureInfo[17]->Add(6, 5, 7305 ,true);
		pclVillageStructureInfo[17]->Add(0, 7, 7306 );
		break;
	}


	//[진성] 통합마켓 
	pText = GetTxtFromMgr(9832);
	pclVillageStructureInfo[RANKTYPE_NEWMARKET]	= new cltVillageStructureInfo( NEWMARKET_UNIQUE, pText, RANKTYPE_NEWMARKET );
	pclVillageStructureInfo[RANKTYPE_NEWMARKET]->SetConstructionInfo( 1, 500000000, TRUE, 3000 );
	switch( pclClient->siServiceArea )
	{
		case ConstServiceArea_China:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_NHNChina:
			pclVillageStructureInfo[RANKTYPE_NEWMARKET]->Add(5, 5, 9342, true);
			break;
		case ConstServiceArea_Japan:
			pclVillageStructureInfo[RANKTYPE_NEWMARKET]->Add(4, 4, 9340, true);
			break;
		default:
			pclVillageStructureInfo[RANKTYPE_NEWMARKET]->Add(6, 6, 9341, true);
			break;
	}
}


cltVillageStructureSet::~cltVillageStructureSet()
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			delete pclVillageStructureInfo[i];
			pclVillageStructureInfo[i] = NULL;
		}
	}

}

SI32 cltVillageStructureSet::FindRefFromUnique(SI32 unique)
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			if(pclVillageStructureInfo[i]->siUnique == unique)
			{
				return i;
			}
		}
	}

	return -1;
}

//sicenterx, sicentery를 중심으로 삼고 있는 건물이 strunique건물 세트의 키가 되는 건물인지 확인한다. 
bool cltVillageStructureSet::IsKeyStructure(SI32 sicenterx, SI32 sicentery, SI32 strunique)
{
	SI32 ref = FindRefFromUnique(strunique);
	if(ref <0)return false;

	return pclVillageStructureInfo[ref]->IsKeyStructure(sicenterx, sicentery);
}

// ranktype을 근거로 건물 이름을 얻어낸다. 
bool cltVillageStructureSet::GetStructureNameFromRankType(SI32 ranktype, TCHAR* pname, SI16 txtSize)
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			if(pclVillageStructureInfo[i]->siRankType == ranktype)
			{
				StringCchCopy(pname, txtSize, pclVillageStructureInfo[i]->szName);
				return true;
			}
		}
	}

	return false;
}

// 건물 이름을 근거로 ranktype을 얻어앤다.
SI32 cltVillageStructureSet::GetRankTypeFromStructureName(TCHAR* pname)
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			if(_tcscmp(pclVillageStructureInfo[i]->szName, pname) == 0)
			{
				
				return pclVillageStructureInfo[i]->siRankType;
			}
		}
	}

	return 0;
}

SI32 cltVillageStructureSet::GetRankTypeFromStrUnique( SI32 strunique )
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			if(pclVillageStructureInfo[i]->siUnique == strunique)
			{
				return pclVillageStructureInfo[i]->siRankType;
			}
		}
	}

	return 0;
}

// ranktype을 근거로 건물 유니크를 얻어낸다
SI32 cltVillageStructureSet::GetStrUniqueFromRankType( SI32 ranktype )
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			if(pclVillageStructureInfo[i]->siRankType == ranktype)
			{
				return pclVillageStructureInfo[i]->siUnique;
			}
		}
	}

	return 0;
}

// ranktype을 근거로 
SI32 cltVillageStructureSet::FindRefFromRankType( SI32 ranktype )
{
	SI32 i;

	for(i = 0;i < MAX_VILLAGE_STRUCTURE_NUMBER;i++)
	{
		if(pclVillageStructureInfo[i])
		{
			if(pclVillageStructureInfo[i]->siRankType == ranktype)
			{
				return i;
			}
		}
	}

	return -1;
}

