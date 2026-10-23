#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"
#include "../GMSystem/GMSectionData.h"
#include "../GMSystem/GMProtocol.h"
#include "..\GMSystem\GMMsg.h"
#include "../Server/Server.h"

#include "../CommonLogic/House/House.h"
#include "../Common/Skill/Skill-Manager.h"
#include "../Common/Char/CharManager/CharManager.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Log.h"

//패킷 보내는 부분.

TCHAR* SkillExplain(cltCharClient* _pclchar, SI32 _SkillUnique);
//--------------------------------------------------------------------------
//
//케릭터 기본 정보
//
//--------------------------------------------------------------------------
void cltServer::DoMsg_DBMSG_RESPONSE_GMTOOL_CHARBASICINFO(sPacketHeader* pPacket)
{
	sDBResponse_BasicInfoByName* pclMsg = (sDBResponse_BasicInfoByName*)pPacket;

	cGMClient* pclGMClient = pclGMProtocol->GetGMClient( pclMsg->siClientID );

	pclGMProtocol->SendInfofromServer( pclGMClient->GetSession(), "CharName : %s", pclMsg->Charname );

/*	// 페이지 시작 알림
	sGMResponse_BeginPage beginmsg( pclMsg->siPAGENum, pclMsg->Charname);
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&beginmsg ); 
	
	GMSectionData*	pSectionData;
	pSectionData = new GMSectionData( 300 * 1024 );

	SI32 siPersonID = pclMsg->BasicInfo.clPerson.siPersonID;

	pSectionData->ClearData();

	////////////////////////////////////////////////////////////////////////////////////////////
	//  실행결과 
	////////////////////////////////////////////////////////////////////////////////////////////
	pSectionData->BeginData( "charname_input" );
	{
		pSectionData->AddDataStr("charname", pclMsg->Charname );
	}
	pSectionData->EndData( "charname_input" );

	pSectionData->BeginData( "show_result" ); // 실행 결과
	if ( siPersonID > 0 )
	{
		pSectionData->AddDataStr( "result", "Succeed");
		pSectionData->AddDataStr( "reason", "The character exists, but not connected.");
	}
	else
	{
		pSectionData->AddDataStr( "result", "Failed" );
		pSectionData->AddDataStr( "reason", "Character does not exist." );
	}
	pSectionData->EndData( "show_result" );

	pclMsg->clIp.GetGMSectionData(pSectionData);

	pclMsg->BasicInfo.GetGMSectionData(pSectionData);
	pclMsg->BasicInfo.clBufInfo.GetGMSectionData(pSectionData);*/


	////////////////////////////////////////////////////////////////////////////////////////////
	//  케릭터 기본 정보
	////////////////////////////////////////////////////////////////////////////////////////////
/*	pSectionData->BeginData( "charbasic" );//기본정보
		pSectionData->AddDataInt( "personid", pclMsg->BasicInfo.clPerson.siPersonID ); // PersonID		
		pSectionData->AddDataStr( "name", pclMsg->Charname);//케릭터명
		pSectionData->AddDataInt( "level", pclMsg->clIp.siLevel );//레벌
		pSectionData->AddDataFormat( "exp", "%d/%d", pclMsg->clIp.siExp, GetRequiredExpForLevel(pclMsg->clIp.siLevel + 1) );//경험치
		pSectionData->AddDataFormat( "hp", "%d/%d", pclMsg->clIp.Life, pclMsg->clIp.siVit*4 );//hp
		pSectionData->AddDataFormat( "mp", "%d/%d", pclMsg->clIp.Mana, pclMsg->clIp.siMag*4);//mp
		pSectionData->AddDataFormat( "fame", "%d/%d",pclMsg->clIp.siFameExp,GetRequiredFameExpForLevel(pclMsg->clIp.siFameLevel + 1) );//명성등급
		pSectionData->AddDataInt( "helth",pclMsg->clHealthInfo.uiHealthPoint );//건강
	pSectionData->EndData( "charbasic" );

	////////////////////////////////////////////////////////////////////////////////////////////
	//  자산 보유현황
	////////////////////////////////////////////////////////////////////////////////////////////
	pSectionData->BeginData( "charasset" );//자산정보
		pSectionData->AddDataInt( "money", pclMsg->clIp.siMoney);//소지금
		pSectionData->AddDataInt( "bank", pclMsg->clBankaccount.GetMoney());//전장
		pSectionData->AddDataInt( "acu", 0 );//아큐
	pSectionData->EndData( "charasset" );

	// 자산 수정에 필요한 PersonID수정
	pSectionData->BeginData( "charasset_modify" );//자산정보 수정
		pSectionData->AddDataInt( "personid", siPersonID ); // PersonID		
	pSectionData->EndData( "charasset_modify" );

	////////////////////////////////////////////////////////////////////////////////////////////
	//  케릭터 스테이터스
	////////////////////////////////////////////////////////////////////////////////////////////
	pSectionData->BeginData( "status" );//스태이터스
		pSectionData->AddDataInt( "str", pclMsg->clIp.siStr);//힘
		pSectionData->AddDataInt( "agi", pclMsg->clIp.siDex);//어질리티
		pSectionData->AddDataInt( "wis", pclMsg->clIp.siMag);//위즈
		pSectionData->AddDataInt( "vit", pclMsg->clIp.siVit);//바이탈
		pSectionData->AddDataInt( "dex", pclMsg->clIp.siHnd);//덱스
		pSectionData->AddDataInt( "luck",pclMsg->clIp.siLuk);//럭
		pSectionData->AddDataInt( "bonus",pclMsg->clIp.siBonus);//스탯보너스
	pSectionData->EndData( "status" );

	// 스테이터스 수정에 필요한 PersonID수정
	pSectionData->BeginData( "status_modify" );
		pSectionData->AddDataInt( "personid", siPersonID ); // PersonID		
	pSectionData->EndData( "status_modify" );

	////////////////////////////////////////////////////////////////////////////////////////////
	//  주식 정보 
	////////////////////////////////////////////////////////////////////////////////////////////
	pSectionData->BeginData( "stockinfo" );//주식정보

		for (SI32 siCount=1; siCount<=42; siCount++)
		{
			pSectionData->BeginData( "village" );//주식정보
				pSectionData->AddDataInt( "villageunique", siCount );	// 마을 유니크
				pSectionData->AddDataStr( "villagename", pclVillageManager->GetName(siCount) );	// 마을이름
				pSectionData->AddDataInt( "stocknum", pclMsg->clStockInfo.clInfo.GetStockAmount(siCount) );//보유주식
			pSectionData->EndData( "village" );//주식정보
		}

	pSectionData->EndData( "stockinfo" );

	// 주식정보 수정에 필요한 PersonID
	pSectionData->BeginData( "stock_modify" );
		pSectionData->AddDataInt( "personid", siPersonID ); // PersonID		
	pSectionData->EndData( "stock_modify" );

	////////////////////////////////////////////////////////////////////////////////////////////
	//  버프 정보 
	////////////////////////////////////////////////////////////////////////////////////////////
	
	char	szBufName[][64]		= { 
		"ATTACKUP",					// 공격력 업
		"DEFENSEUP",				// 방어력 업
		"SPEEDUP",					// 속도 업
		"BUF_MAKEEXPUP",			// 제조 경험치 업
		"BUF_NOFIRSTATTACK",		// 선공 방지
		"SUMMONATACUP",				// 소환수 공격력 업
		"LOVEPOWER",				// 사랑 버프
		"FRIENDSHIP",				// 우정 버프
		"HINAMOCHI",				// 히나모찌
		"SUCCESS_MANUFACTURE",		// 제조 성공
		"MOFUMOFU_PERFUME",			// 모후모후 향수
		"FOURLEAF",					// 네잎클로버
		"PREMIUMEXPUP",				// 프리미엄 경헙치 업
		"HAPPYBEAN",				// 해피 빈 업
		"NOTBONUSTIME_MAKEEXPUP",	// 제조보너스 타임에 중첩되지 않는다.
		"ATTACKUP30",				// 전투력 증가약 30일간
		"DEFENSEUP30",				// 방어력 증가약 30일간
		"SUPEREXPUP",				// 슈퍼 EXP UP
		"THANKSGIVING_DAY"			// 추수감사절
	};

	SI32	siBufNameCount	= sizeof(szBufName)/sizeof(char)/64;
	stBuf*	pstBuf			= NULL;
	TCHAR*	pText			= GetTxtFromMgr(7599);


	pSectionData->BeginData( "BufStatus" );

	for (SI32 siBufType=1; siBufType<siBufNameCount; siBufType++)
	{
		pSectionData->BeginData( "buf" );

		// 버프는 추가했는데 이름을 추가 안했을때를 대비해서 오류 검사
		if ( siBufNameCount > (siBufType-1) )
		{
			pSectionData->AddDataStr( "bufname",  szBufName[siBufType-1]  );
		}
		else
		{
			pSectionData->AddDataStr( "bufname", "Do Not Define Buf Name" );
			pSectionData->EndData( "buf" );

			continue;
		}

		pSectionData->AddDataInt( "bufnumber", siBufType );

		pstBuf = pclMsg->BasicInfo.clBufInfo.GetBuf( siBufType );

		// 해당 버프를 사용 중이라면
		if( pstBuf != NULL )
		{
			TCHAR		tszTimeBuf[128]	= TEXT("");
			cltDate		cldate;
			SI16		siBufSlot		= -1;


			cldate.MakeFullDateFromDateVary( pstBuf->siOverTime );
			cldate.GetDateText( tszTimeBuf );

			// 현재 타입의 버프 슬롯 번호 찾기
			siBufSlot = pclMsg->BasicInfo.clBufInfo.FindBufSlot( siBufType );
			
			pSectionData->AddDataInt( "bufslot", siBufSlot );
			pSectionData->AddDataStr( "bufremain", tszTimeBuf );
		}
		else 
		{
			pSectionData->AddDataStr( "bufslot",  "NULL" );
			pSectionData->AddDataStr( "bufremain", "Don't Used");
		}

		pstBuf = NULL;

		pSectionData->EndData( "buf" );
	}

	pSectionData->EndData( "BufStatus" );

	// 버프 수정 정보
	pSectionData->BeginData( "buf_modify" );
		pSectionData->AddDataInt( "personid", siPersonID );
	pSectionData->EndData( "buf_modify" );

	

	//pSectionData->AddDataInt( "CombatPremiumService",pclMsg->BasicInfo.siCombatServiceDate);//전투 프리미엄 서비스
	//pSectionData->AddDataInt( "Marketpremiumservice",pclMsg->BasicInfo.siMakeItemServiceDate);//제조프리미엄서비스
	//pSectionData->AddDataInt( "BulletpremiumService",pclMsg->BasicInfo.siBulletServiceDate);//탄환프리미엄서비스
	//pSectionData->AddDataInt( "SummomPremiumService",pclMsg->BasicInfo.siSummonStaminaServiceDate);//소환수스테미너프리미엄서비스
	//pSectionData->AddDataInt( "MarketPremiumService",pclMsg->BasicInfo.siMarKetPremiumServiceDate);//객주프리미엄서비스
	
*/
	/*sGMResponse_Section sectionsbasiconfomsg( pclMsg->siPAGENum, pclMsg->Charname, pSectionData );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&sectionsbasiconfomsg ); 

	delete pSectionData; 

	printf( "메시지 보냄 : GMMSG_RESPONSE_SECTION\n" );
	// 페이지 종료 알림
	sGMResponse_EndPage endmsg( pclMsg->siPAGENum );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&endmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_ENDPAGE\n" );*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 탈것
//
////////////////////////////////////////////////////////////////////////////////////////////////
void cltServer::DoMsg_DBMSG_RESPONSE_GMTOOL_ANIMALINFO(sPacketHeader* pPacket){	

	GMSectionData*	pSectionData;
	pSectionData = new GMSectionData( 200 * 1024 );
	sDBResponse_AnimalByName* pclMsg = (sDBResponse_AnimalByName*)pPacket;
	
	sGMResponse_BeginPage beginmsg( pclMsg->siPAGENum, pclMsg->Charname );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&beginmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_BEGINPAGE\n" );

	//cltHorse* pclHorse = pclMsg->clHorseInfo.GetCurrentShowHorse();
	//SI32 horseunique = pclHorse->siHorseUnique;
	//SI32 horsebirthYear = pclHorse->clBirthDate.uiYear;
	//SI32 horsebirthMonth = pclHorse->clBirthDate.uiMonth;
	//SI32 horsebirthDay = pclHorse->clBirthDate.uiDay;
	//SI32 horsebirthHour = pclHorse->clBirthDate.uiHour;
	//SI32 lifeSpan = pclHorse->siLife;
	//SI32 datevary = pclTime->clDate.GetDateVary();
	//TCHAR* horsetype = pclHorseManager->pclHorseTypeInfo[horseunique]->szSpecies;

	/////여기부터 데이터 추가.

	//pSectionData->ClearData();
	//pSectionData->BeginData( "animalbasic" );
	//pSectionData->AddDataStr( "animalname", pclHorse->szName);
	//if (horseunique == 0) {
	//	pSectionData->AddDataStr( "animaltype","No Horse");//에게 되려나 ;
	//}
	//else{
	//	pSectionData->AddDataStr( "animaltype",horsetype);//에게 되려나 ;
	//}
	//pSectionData->AddDataFormat( "animalbirth", "Y:%d M:%d D:%dth H:%d",horsebirthYear, horsebirthMonth,horsebirthDay,horsebirthHour );	
 //   pSectionData->AddDataInt( "anumallifespan",lifeSpan);
	//pSectionData->AddDataInt( "animalage", pclHorse->GetAge(datevary) );	
	//if (pclHorse->siSex == 1) {pSectionData->AddDataStr( "animalgender", "Female" );}
	//else{pSectionData->AddDataStr( "animalgender", "Male" );}	
	//pSectionData->EndData( "animalbasic" );

	//pSectionData->BeginData( "anmalstatusinfo" );
	//pSectionData->AddDatafloat( "animalspeed",pclHorse->siMoveSpeed/100);
	//pSectionData->AddDatafloat( "animalstr",pclHorse->siStr/100 );
	//pSectionData->AddDatafloat( "animalagi",pclHorse->siDex/100 );
	//pSectionData->AddDatafloat( "animalwis",pclHorse->siMag/100 );
	//pSectionData->AddDatafloat( "animalbonus",pclHorse->siStatusBonus );
	//pSectionData->EndData( "anmalstatusinfo" );

	cltHorse* pclHorse1 = pclMsg->clHorseInfo.GetHorse(0);
	cltHorse* pclHorse2 = pclMsg->clHorseInfo.GetHorse(1);
	cltHorse* pclHorse3 = pclMsg->clHorseInfo.GetHorse(2);
	cltHorse* pclHorse4 = pclMsg->clHorseInfo.GetHorse(3);
	cltHorse* pclHorse5 = pclMsg->clHorseInfo.GetHorse(4);
	//슬롯 1.
	if (pclHorse1) {
		SI32 horseunique1 = pclHorse1->siHorseUnique;
		SI32 horsebirthYear1 = pclHorse1->clBirthDate.uiYear;
		SI32 horsebirthMonth1 = pclHorse1->clBirthDate.uiMonth;
		SI32 horsebirthDay1 = pclHorse1->clBirthDate.uiDay;
		SI32 horsebirthHour1 = pclHorse1->clBirthDate.uiHour;
		SI32 lifeSpan1 = pclHorse1->siLife;
		SI32 datevary1 = pclTime->clDate.GetDateVary();
		TCHAR* horsetype1 = pclHorseManager->pclHorseTypeInfo[horseunique1]->szSpecies;

		pSectionData->BeginData( "animalbasic" );
		pSectionData->AddDataStr( "animalname", pclHorse1->szName);
		if (horseunique1 == 0) {
			pSectionData->AddDataStr( "animaltype","No Horse");
		}
		else{
			pSectionData->AddDataStr( "animaltype",horsetype1);
		}
		pSectionData->AddDataFormat( "animalbirth", "Y:%d M:%d D:%dth H:%d",horsebirthYear1, horsebirthMonth1,horsebirthDay1,horsebirthHour1 );	
		pSectionData->AddDataInt( "anumallifespan",lifeSpan1);
		pSectionData->AddDataInt( "animalage", pclHorse1->GetAge(datevary1) );	
		if (pclHorse1->siSex == 1) {pSectionData->AddDataStr( "animalgender", "Female" );}
		else{pSectionData->AddDataStr( "animalgender", "Male" );}	
		pSectionData->EndData( "animalbasic" );

		pSectionData->BeginData( "anmalstatusinfo" );
		pSectionData->AddDatafloat( "animalspeed",pclHorse1->siMoveSpeed/100);
		pSectionData->AddDatafloat( "animalstr",pclHorse1->siStr/100 );
		pSectionData->AddDatafloat( "animalagi",pclHorse1->siDex/100 );
		pSectionData->AddDatafloat( "animalwis",pclHorse1->siMag/100 );
		pSectionData->AddDatafloat( "animalbonus",pclHorse1->siStatusBonus );
		pSectionData->EndData( "anmalstatusinfo" );
	}
	if (pclHorse2) {
		//슬롯 2.		
		SI32 horseunique2 = pclHorse2->siHorseUnique;
		SI32 horsebirthYear2 = pclHorse2->clBirthDate.uiYear;
		SI32 horsebirthMonth2 = pclHorse2->clBirthDate.uiMonth;
		SI32 horsebirthDay2 = pclHorse2->clBirthDate.uiDay;
		SI32 horsebirthHour2 = pclHorse2->clBirthDate.uiHour;
		SI32 lifeSpan2 = pclHorse2->siLife;
		SI32 datevary2 = pclTime->clDate.GetDateVary();
		TCHAR* horsetype2 = pclHorseManager->pclHorseTypeInfo[horseunique2]->szSpecies;

		pSectionData->BeginData( "animalbasic1" );
		pSectionData->AddDataStr( "animalname1", pclHorse2->szName);
		if (horseunique2 == 0) {
			pSectionData->AddDataStr( "animaltype1","No Horse");
		}
		else{
			pSectionData->AddDataStr( "animaltype1",horsetype2);
		}
		pSectionData->AddDataFormat( "animalbirth1", "Y:%d M:%d D:%dth H:%d",horsebirthYear2, horsebirthMonth2,horsebirthDay2,horsebirthHour2 );	
		pSectionData->AddDataInt( "anumallifespan1",lifeSpan2);
		pSectionData->AddDataInt( "animalage1", pclHorse2->GetAge(datevary2) );	
		if (pclHorse2->siSex == 1) {pSectionData->AddDataStr( "animalgender1", "Female" );}
		else{pSectionData->AddDataStr( "animalgender1", "Male" );}	
		pSectionData->EndData( "animalbasic1" );

		pSectionData->BeginData( "anmalstatusinfo1" );
		pSectionData->AddDatafloat( "animalspeed1",pclHorse2->siMoveSpeed/100);
		pSectionData->AddDatafloat( "animalstr1",pclHorse2->siStr/100 );
		pSectionData->AddDatafloat( "animalagi1",pclHorse2->siDex/100 );
		pSectionData->AddDatafloat( "animalwis1",pclHorse2->siMag/100 );
		pSectionData->AddDatafloat( "animalbonus1",pclHorse2->siStatusBonus );
		pSectionData->EndData( "anmalstatusinfo1" );
	}
	
	if (pclHorse3) {
		//슬롯 3.		
		SI32 horseunique = pclHorse3->siHorseUnique;
		SI32 horsebirthYear = pclHorse3->clBirthDate.uiYear;
		SI32 horsebirthMonth = pclHorse3->clBirthDate.uiMonth;
		SI32 horsebirthDay = pclHorse3->clBirthDate.uiDay;
		SI32 horsebirthHour = pclHorse3->clBirthDate.uiHour;
		SI32 lifeSpan = pclHorse3->siLife;
		SI32 datevary = pclTime->clDate.GetDateVary();
		TCHAR* horsetype = pclHorseManager->pclHorseTypeInfo[horseunique]->szSpecies;

		pSectionData->BeginData( "animalbasic2" );
		pSectionData->AddDataStr( "animalname2", pclHorse3->szName);
		if (horseunique == 0) {
			pSectionData->AddDataStr( "animaltype2","No Horse");
		}
		else{
			pSectionData->AddDataStr( "animaltype2",horsetype);
		}
		pSectionData->AddDataFormat( "animalbirth2", "Y:%d M:%d D:%dth H:%d",horsebirthYear, horsebirthMonth,horsebirthDay,horsebirthHour );	
		pSectionData->AddDataInt( "anumallifespan2",lifeSpan);
		pSectionData->AddDataInt( "animalage2", pclHorse3->GetAge(datevary) );	
		if (pclHorse3->siSex == 1) {pSectionData->AddDataStr( "animalgender2", "Female" );}
		else{pSectionData->AddDataStr( "animalgender2", "Male" );}	
		pSectionData->EndData( "animalbasic2" );

		pSectionData->BeginData( "anmalstatusinfo2" );
		pSectionData->AddDatafloat( "animalspeed2",pclHorse3->siMoveSpeed/100);
		pSectionData->AddDatafloat( "animalstr2",pclHorse3->siStr/100 );
		pSectionData->AddDatafloat( "animalagi2",pclHorse3->siDex/100 );
		pSectionData->AddDatafloat( "animalwis2",pclHorse3->siMag/100 );
		pSectionData->AddDatafloat( "animalbonus2",pclHorse3->siStatusBonus );
		pSectionData->EndData( "anmalstatusinfo2" );
	}
	if (pclHorse4) {
		//슬롯 4.
		
		SI32 horseunique = pclHorse4->siHorseUnique;
		SI32 horsebirthYear = pclHorse4->clBirthDate.uiYear;
		SI32 horsebirthMonth = pclHorse4->clBirthDate.uiMonth;
		SI32 horsebirthDay = pclHorse4->clBirthDate.uiDay;
		SI32 horsebirthHour = pclHorse4->clBirthDate.uiHour;
		SI32 lifeSpan = pclHorse4->siLife;
		SI32 datevary = pclTime->clDate.GetDateVary();
		TCHAR* horsetype = pclHorseManager->pclHorseTypeInfo[horseunique]->szSpecies;

		pSectionData->BeginData( "animalbasic3" );
		pSectionData->AddDataStr( "animalname3", pclHorse4->szName);
		if (horseunique == 0) {
			pSectionData->AddDataStr( "animaltype3","No Horse");
		}
		else{
			pSectionData->AddDataStr( "animaltype3",horsetype);
		}
		pSectionData->AddDataFormat( "animalbirth3", "Y:%d M:%d D:%dth H:%d",horsebirthYear, horsebirthMonth,horsebirthDay,horsebirthHour );	
		pSectionData->AddDataInt( "anumallifespan3",lifeSpan);
		pSectionData->AddDataInt( "animalage3", pclHorse4->GetAge(datevary) );	
		if (pclHorse4->siSex == 1) {pSectionData->AddDataStr( "animalgender3", "Female" );}
		else{pSectionData->AddDataStr( "animalgender3", "Male" );}	
		pSectionData->EndData( "animalbasic3" );

		pSectionData->BeginData( "anmalstatusinfo3" );
		pSectionData->AddDatafloat( "animalspeed3",pclHorse4->siMoveSpeed/100);
		pSectionData->AddDatafloat( "animalstr3",pclHorse4->siStr/100 );
		pSectionData->AddDatafloat( "animalagi3",pclHorse4->siDex/100 );
		pSectionData->AddDatafloat( "animalwis3",pclHorse4->siMag/100 );
		pSectionData->AddDatafloat( "animalbonus3",pclHorse4->siStatusBonus );
		pSectionData->EndData( "anmalstatusinfo3" );
	}
	if (pclHorse5) {
		//슬롯 5.

		SI32 horseunique = pclHorse5->siHorseUnique;
		SI32 horsebirthYear = pclHorse5->clBirthDate.uiYear;
		SI32 horsebirthMonth = pclHorse5->clBirthDate.uiMonth;
		SI32 horsebirthDay = pclHorse5->clBirthDate.uiDay;
		SI32 horsebirthHour = pclHorse5->clBirthDate.uiHour;
		SI32 lifeSpan = pclHorse5->siLife;
		SI32 datevary = pclTime->clDate.GetDateVary();
		TCHAR* horsetype = pclHorseManager->pclHorseTypeInfo[horseunique]->szSpecies;

		pSectionData->BeginData( "animalbasic4" );
		pSectionData->AddDataStr( "animalname4", pclHorse5->szName);
		if (horseunique == 0) {
			pSectionData->AddDataStr( "animaltype4","No Horse");
		}
		else{
			pSectionData->AddDataStr( "animaltype4",horsetype);
		}
		pSectionData->AddDataFormat( "animalbirth4", "Y:%d M:%d D:%dth H:%d",horsebirthYear, horsebirthMonth,horsebirthDay,horsebirthHour );	
		pSectionData->AddDataInt( "anumallifespan4",lifeSpan);
		pSectionData->AddDataInt( "animalage4", pclHorse5->GetAge(datevary) );	
		if (pclHorse5->siSex == 1) {pSectionData->AddDataStr( "animalgender4", "Female" );}
		else{pSectionData->AddDataStr( "animalgender4", "Male" );}	
		pSectionData->EndData( "animalbasic4" );

		pSectionData->BeginData( "anmalstatusinfo4" );
		pSectionData->AddDatafloat( "animalspeed4",pclHorse5->siMoveSpeed/100);
		pSectionData->AddDatafloat( "animalstr4",pclHorse5->siStr/100 );
		pSectionData->AddDatafloat( "animalagi4",pclHorse5->siDex/100 );
		pSectionData->AddDatafloat( "animalwis4",pclHorse5->siMag/100 );
		pSectionData->AddDatafloat( "animalbonus4",pclHorse5->siStatusBonus );
		pSectionData->EndData( "anmalstatusinfo4" );
	}

	// 탈것 수정에 필요한 PersonID
	pSectionData->BeginData( "animal_add" );
		pSectionData->AddDataInt( "personid", pclMsg->siPersonID ); // PersonID		
	pSectionData->EndData( "animal_add" );

	
	sGMResponse_Section sectionsbasiconfomsg( GMMSG_PAGEID_ANIMAL, (char*)pclMsg->Charname, pSectionData );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&sectionsbasiconfomsg ); 
	delete pSectionData;

	printf( "메시지 보냄 : GMMSG_RESPONSE_SECTION\n" );
	// 페이지 종료 알림
	sGMResponse_EndPage endmsg( pclMsg->siPAGENum );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&endmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_ENDPAGE\n" );

}
void cltServer ::DoMsg_DBMSG_RESPONSE_GMTOOL_INVENTORYINFO(sPacketHeader* pPacket)
{
	GMSectionData*	pSectionData;
	pSectionData = new GMSectionData( 200 * 1024 );
	sDBResponse_InventoryByName* pclMsg = (sDBResponse_InventoryByName*)pPacket;

	sGMResponse_BeginPage beginmsg( pclMsg->siPAGENum, pclMsg->Charname );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&beginmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_BEGINPAGE\n" );

	//여기 부터 데이터
	cltClient *pclclient = ( cltClient * )pclClient;
	//케릭터 장착
	TCHAR* pItemName1 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[2].siUnique);
	SI16 Itemnum1 = pclMsg->clItemInfo.clItem[2].siItemNum;
	TCHAR* pItemName2 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[8].siUnique);
	SI16 Itemnum2 = pclMsg->clItemInfo.clItem[8].siItemNum;
	TCHAR* pItemName3
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[0].siUnique);
	TCHAR* pItemName4
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[1].siUnique);
	TCHAR* pItemName5 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[3].siUnique);
	TCHAR* pItemName6 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[4].siUnique);
	TCHAR* pItemName7 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[5].siUnique);
	TCHAR* pItemName8
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[6].siUnique);
	TCHAR* pItemName9 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[7].siUnique);
	TCHAR* pItemName10 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[9].siUnique);
	TCHAR* pItemName11 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[11].siUnique);
	TCHAR* pItemName12 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[12].siUnique);
	TCHAR* pItemName13 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[10].siUnique);
	TCHAR* pItemName14 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[13].siUnique);
	TCHAR* pItemName15 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[14].siUnique);
	//케릭터인벤토리
	TCHAR* pInvName1 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[15].siUnique);
	SI16 Itemnum15 = pclMsg->clItemInfo.clItem[15].siItemNum;
	TCHAR* pInvName2 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[16].siUnique);
	SI16 Itemnum16 = pclMsg->clItemInfo.clItem[16].siItemNum;
	TCHAR* pInvName3 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[17].siUnique);
	SI16 Itemnum17 = pclMsg->clItemInfo.clItem[17].siItemNum;
	TCHAR* pInvName4 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[18].siUnique);
	SI16 Itemnum18 = pclMsg->clItemInfo.clItem[18].siItemNum;
	TCHAR* pInvName5 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[19].siUnique);
	SI16 Itemnum19 = pclMsg->clItemInfo.clItem[19].siItemNum;
	TCHAR* pInvName6 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[20].siUnique);
	SI16 Itemnum20 = pclMsg->clItemInfo.clItem[20].siItemNum;
	TCHAR* pInvName7 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[21].siUnique);
	SI16 Itemnum21 = pclMsg->clItemInfo.clItem[21].siItemNum;
	TCHAR* pInvName8 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[22].siUnique);
	SI16 Itemnum22 = pclMsg->clItemInfo.clItem[22].siItemNum;
	TCHAR* pInvName9 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[23].siUnique);
	SI16 Itemnum23 = pclMsg->clItemInfo.clItem[23].siItemNum;
	TCHAR* pInvName10 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[24].siUnique);
	SI16 Itemnum24 = pclMsg->clItemInfo.clItem[24].siItemNum;
	//케릭터 인벤토리 2
	TCHAR* pInvName11 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[25].siUnique);
	SI16 Itemnum25 = pclMsg->clItemInfo.clItem[25].siItemNum;
	TCHAR* pInvName12 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[26].siUnique);
	SI16 Itemnum26 = pclMsg->clItemInfo.clItem[26].siItemNum;
	TCHAR* pInvName13 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[27].siUnique);
	SI16 Itemnum27 = pclMsg->clItemInfo.clItem[27].siItemNum;
	TCHAR* pInvName14 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[28].siUnique);
	SI16 Itemnum28 = pclMsg->clItemInfo.clItem[28].siItemNum;
	TCHAR* pInvName15 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[29].siUnique);
	SI16 Itemnum29 = pclMsg->clItemInfo.clItem[29].siItemNum;
	TCHAR* pInvName16 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[30].siUnique);
	SI16 Itemnum30 = pclMsg->clItemInfo.clItem[30].siItemNum;
	TCHAR* pInvName17 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[31].siUnique);
	SI16 Itemnum31 = pclMsg->clItemInfo.clItem[31].siItemNum;
	TCHAR* pInvName18 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[32].siUnique);
	SI16 Itemnum32 = pclMsg->clItemInfo.clItem[32].siItemNum;
	TCHAR* pInvName19 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[33].siUnique);
	SI16 Itemnum33 = pclMsg->clItemInfo.clItem[33].siItemNum;
	TCHAR* pInvName20 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[34].siUnique);
	SI16 Itemnum34 = pclMsg->clItemInfo.clItem[34].siItemNum;
	//동물인벤토리.
	TCHAR* pInvName21 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[35].siUnique);
	SI16 Itemnum35 = pclMsg->clItemInfo.clItem[35].siItemNum;
	TCHAR* pInvName22
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[36].siUnique);
	SI16 Itemnum36 = pclMsg->clItemInfo.clItem[36].siItemNum;
	TCHAR* pInvName23 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[37].siUnique);
	SI16 Itemnum37 = pclMsg->clItemInfo.clItem[37].siItemNum;
	TCHAR* pInvName24 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[38].siUnique);
	SI16 Itemnum38 = pclMsg->clItemInfo.clItem[38].siItemNum;
	TCHAR* pInvName25 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[39].siUnique);
	SI16 Itemnum39 = pclMsg->clItemInfo.clItem[39].siItemNum;
	TCHAR* pInvName26 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[40].siUnique);
	SI16 Itemnum40 = pclMsg->clItemInfo.clItem[40].siItemNum;
	TCHAR* pInvName27 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[41].siUnique);
	SI16 Itemnum41 = pclMsg->clItemInfo.clItem[41].siItemNum;
	TCHAR* pInvName28 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[42].siUnique);
	SI16 Itemnum42 = pclMsg->clItemInfo.clItem[42].siItemNum;
	TCHAR* pInvName29 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[43].siUnique);
	SI16 Itemnum43 = pclMsg->clItemInfo.clItem[43].siItemNum;
	TCHAR* pInvName30 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[44].siUnique);
	SI16 Itemnum44 = pclMsg->clItemInfo.clItem[44].siItemNum;
	//동물인벤토리2
	TCHAR* pInvName31 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[45].siUnique);
	SI16 Itemnum45 = pclMsg->clItemInfo.clItem[45].siItemNum;
	TCHAR* pInvName32
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[46].siUnique);
	SI16 Itemnum46 = pclMsg->clItemInfo.clItem[46].siItemNum;
	TCHAR* pInvName33 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[47].siUnique);
	SI16 Itemnum47 = pclMsg->clItemInfo.clItem[47].siItemNum;
	TCHAR* pInvName34
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[48].siUnique);
	SI16 Itemnum48 = pclMsg->clItemInfo.clItem[48].siItemNum;
	TCHAR* pInvName35
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[49].siUnique);
	SI16 Itemnum49 = pclMsg->clItemInfo.clItem[49].siItemNum;
	TCHAR* pInvName36 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[50].siUnique);
	SI16 Itemnum50 = pclMsg->clItemInfo.clItem[50].siItemNum;
	TCHAR* pInvName37 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[51].siUnique);
	SI16 Itemnum51 = pclMsg->clItemInfo.clItem[51].siItemNum;
	TCHAR* pInvName38 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[52].siUnique);
	SI16 Itemnum52 = pclMsg->clItemInfo.clItem[52].siItemNum;
	TCHAR* pInvName39 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[53].siUnique);
	SI16 Itemnum53 = pclMsg->clItemInfo.clItem[53].siItemNum;
	TCHAR* pInvName40 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[54].siUnique);
	SI16 Itemnum54 = pclMsg->clItemInfo.clItem[54].siItemNum;
	//가방1
	TCHAR* pInvName41 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[55].siUnique);
	SI16 Itemnum55 = pclMsg->clItemInfo.clItem[55].siItemNum;
	TCHAR* pInvName42 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[56].siUnique);
	SI16 Itemnum56 = pclMsg->clItemInfo.clItem[56].siItemNum;
	TCHAR* pInvName43 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[57].siUnique);
	SI16 Itemnum57 = pclMsg->clItemInfo.clItem[57].siItemNum;
	TCHAR* pInvName44 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[58].siUnique);
	SI16 Itemnum58 = pclMsg->clItemInfo.clItem[58].siItemNum;
	TCHAR* pInvName45 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[59].siUnique);
	SI16 Itemnum59 = pclMsg->clItemInfo.clItem[59].siItemNum;
	TCHAR* pInvName46 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[60].siUnique);
	SI16 Itemnum60 = pclMsg->clItemInfo.clItem[60].siItemNum;
	TCHAR* pInvName47 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[61].siUnique);
	SI16 Itemnum61 = pclMsg->clItemInfo.clItem[61].siItemNum;
	TCHAR* pInvName48 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[62].siUnique);
	SI16 Itemnum62 = pclMsg->clItemInfo.clItem[62].siItemNum;
	TCHAR* pInvName49 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[63].siUnique);
	SI16 Itemnum63 = pclMsg->clItemInfo.clItem[63].siItemNum;
	TCHAR* pInvName50 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[64].siUnique);
	SI16 Itemnum64 = pclMsg->clItemInfo.clItem[64].siItemNum;
	TCHAR* pInvName51 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[65].siUnique);
	SI16 Itemnum65 = pclMsg->clItemInfo.clItem[65].siItemNum;
	TCHAR* pInvName52 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[66].siUnique);
	SI16 Itemnum66 = pclMsg->clItemInfo.clItem[66].siItemNum;
	//가방2
	TCHAR* pInvName53 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[67].siUnique);
	SI16 Itemnum67 = pclMsg->clItemInfo.clItem[67].siItemNum;
	TCHAR* pInvName54 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[68].siUnique);
	SI16 Itemnum68 = pclMsg->clItemInfo.clItem[68].siItemNum;
	TCHAR* pInvName55 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[69].siUnique);
	SI16 Itemnum69 = pclMsg->clItemInfo.clItem[69].siItemNum;
	TCHAR* pInvName56 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[70].siUnique);
	SI16 Itemnum70 = pclMsg->clItemInfo.clItem[70].siItemNum;
	TCHAR* pInvName57 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[71].siUnique);
	SI16 Itemnum71 = pclMsg->clItemInfo.clItem[71].siItemNum;
	TCHAR* pInvName58 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[72].siUnique);
	SI16 Itemnum72 = pclMsg->clItemInfo.clItem[72].siItemNum;
	TCHAR* pInvName59 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[73].siUnique);
	SI16 Itemnum73 = pclMsg->clItemInfo.clItem[73].siItemNum;
	TCHAR* pInvName60 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[74].siUnique);
	SI16 Itemnum74 = pclMsg->clItemInfo.clItem[74].siItemNum;
	TCHAR* pInvName61 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[75].siUnique);
	SI16 Itemnum75 = pclMsg->clItemInfo.clItem[75].siItemNum;
	TCHAR* pInvName62 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[76].siUnique);
	SI16 Itemnum76 = pclMsg->clItemInfo.clItem[76].siItemNum;
	TCHAR* pInvName63 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[77].siUnique);
	SI16 Itemnum77 = pclMsg->clItemInfo.clItem[77].siItemNum;
	TCHAR* pInvName64 
		= (TCHAR*)pclItemManager->GetName(pclMsg->clItemInfo.clItem[78].siUnique);
	SI16 Itemnum78 = pclMsg->clItemInfo.clItem[78].siItemNum;

	//케릭터 장착아이템.
	pSectionData->ClearData();
	////////////////////////////////////////////////////////////////////////////////////////////
	//  실행결과 
	////////////////////////////////////////////////////////////////////////////////////////////
	pSectionData->BeginData( "show_result" ); // 실행 결과
	if ( pclMsg->siPersonID > 0 )
	{
		pSectionData->AddDataStr( "result", "완료");
		pSectionData->AddDataStr( "reason", "캐릭터는 존재하지만 접속해 있지는 않습니다");
	}
	else
	{
		pSectionData->AddDataStr( "result", "오류" );
		pSectionData->AddDataStr( "reason", "존재하지 않는 케릭터 입니다" );
	}
	pSectionData->EndData( "show_result" );

	pSectionData->BeginData( "charequip" );
	pSectionData->AddDataInt( "eWeapon1",pclMsg->clItemInfo.clItem[2].siUnique );
	pSectionData->AddDataStr("pItemName1",pItemName1);
	pSectionData->AddDataInt("Itemnum1",Itemnum1);
	pSectionData->AddDataInt( "eWeapon2", pclMsg->clItemInfo.clItem[8].siUnique );
	pSectionData->AddDataStr("pItemName2",pItemName2);
	pSectionData->AddDataInt("Itemnum2",Itemnum2);
	pSectionData->AddDataInt( "eHelmet", pclMsg->clItemInfo.clItem[0].siUnique );
	pSectionData->AddDataStr("pItemName3",pItemName3);
	pSectionData->AddDataInt( "eArmor", pclMsg->clItemInfo.clItem[1].siUnique );
	pSectionData->AddDataStr("pItemName4",pItemName4);
	pSectionData->AddDataInt( "eNecklace", pclMsg->clItemInfo.clItem[3].siUnique );
	pSectionData->AddDataStr("pItemName5",pItemName5);
	pSectionData->AddDataInt( "eRing1", pclMsg->clItemInfo.clItem[4].siUnique );
	pSectionData->AddDataStr("pItemName6",pItemName6);
	pSectionData->AddDataInt( "eRing2", pclMsg->clItemInfo.clItem[5].siUnique );
	pSectionData->AddDataStr("pItemName7",pItemName7);
	pSectionData->AddDataInt( "eBelt", pclMsg->clItemInfo.clItem[6].siUnique );
	pSectionData->AddDataStr("pItemName8",pItemName8);
	pSectionData->AddDataInt( "eShoes", pclMsg->clItemInfo.clItem[7].siUnique );
	pSectionData->AddDataStr("pItemName9",pItemName9);
	pSectionData->AddDataInt( "eCloak", pclMsg->clItemInfo.clItem[9].siUnique );
	pSectionData->AddDataStr("pItemName10",pItemName10);
	pSectionData->AddDataInt( "eHat", pclMsg->clItemInfo.clItem[11].siUnique );
	pSectionData->AddDataStr("pItemName11",pItemName11);
	pSectionData->AddDataInt( "eDress", pclMsg->clItemInfo.clItem[12].siUnique );
	pSectionData->AddDataStr("pItemName12",pItemName12);
	pSectionData->AddDataInt( "eBag1", pclMsg->clItemInfo.clItem[10].siUnique );
	pSectionData->AddDataStr("pItemName13",pItemName13);
	pSectionData->AddDataInt( "eBag2", pclMsg->clItemInfo.clItem[13].siUnique );
	pSectionData->AddDataStr("pItemName14",pItemName14);
	pSectionData->AddDataInt( "eBag3", pclMsg->clItemInfo.clItem[14].siUnique );
	pSectionData->AddDataStr("pItemName15",pItemName15);
	pSectionData->EndData( "charequip" );

	//인벤토리
	pSectionData->BeginData( "charinventory" );
	pSectionData->AddDataInt( "inv1", pclMsg->clItemInfo.clItem[15].siUnique );
	pSectionData->AddDataStr("pInvName1",pInvName1);
	pSectionData->AddDataInt("Itemnum15",Itemnum15);
	pSectionData->AddDataInt( "inv2", pclMsg->clItemInfo.clItem[16].siUnique );
	pSectionData->AddDataStr("pInvName2",pInvName2);
	pSectionData->AddDataInt("Itemnum16",Itemnum16);
	pSectionData->AddDataInt( "inv3", pclMsg->clItemInfo.clItem[17].siUnique );
	pSectionData->AddDataStr("pInvName3",pInvName3);
	pSectionData->AddDataInt("Itemnum17",Itemnum17);
	pSectionData->AddDataInt( "inv4", pclMsg->clItemInfo.clItem[18].siUnique );					
	pSectionData->AddDataStr("pInvName4",pInvName4);
	pSectionData->AddDataInt("Itemnum18",Itemnum18);
	pSectionData->AddDataInt( "inv5", pclMsg->clItemInfo.clItem[19].siUnique );
	pSectionData->AddDataStr("pInvName5",pInvName5);
	pSectionData->AddDataInt("Itemnum19",Itemnum19);
	pSectionData->AddDataInt( "inv6", pclMsg->clItemInfo.clItem[20].siUnique );
	pSectionData->AddDataStr("pInvName6",pInvName6);
	pSectionData->AddDataInt("Itemnum20",Itemnum20);
	pSectionData->AddDataInt( "inv7", pclMsg->clItemInfo.clItem[21].siUnique );
	pSectionData->AddDataStr("pInvName7",pInvName7);
	pSectionData->AddDataInt("Itemnum21",Itemnum21);
	pSectionData->AddDataInt( "inv8", pclMsg->clItemInfo.clItem[22].siUnique );
	pSectionData->AddDataStr("pInvName8",pInvName8);
	pSectionData->AddDataInt("Itemnum22",Itemnum22);
	pSectionData->AddDataInt( "inv9", pclMsg->clItemInfo.clItem[23].siUnique );
	pSectionData->AddDataStr("pInvName9",pInvName9);
	pSectionData->AddDataInt("Itemnum23",Itemnum23);
	pSectionData->AddDataInt( "inv10", pclMsg->clItemInfo.clItem[24].siUnique );					
	pSectionData->AddDataStr("pInvName10",pInvName10);
	pSectionData->AddDataInt("Itemnum24",Itemnum24);
	pSectionData->EndData( "charinventory" );

	pSectionData->BeginData( "charinventory2" );
	pSectionData->AddDataInt( "inv11", pclMsg->clItemInfo.clItem[25].siUnique );
	pSectionData->AddDataStr("pInvName11",pInvName11);
	pSectionData->AddDataInt("Itemnum25",Itemnum25);
	pSectionData->AddDataInt( "inv12", pclMsg->clItemInfo.clItem[26].siUnique );
	pSectionData->AddDataStr("pInvName12",pInvName12);
	pSectionData->AddDataInt("Itemnum26",Itemnum26);
	pSectionData->AddDataInt( "inv13", pclMsg->clItemInfo.clItem[27].siUnique );
	pSectionData->AddDataStr("pInvName13",pInvName13);
	pSectionData->AddDataInt("Itemnum27",Itemnum27);
	pSectionData->AddDataInt( "inv14", pclMsg->clItemInfo.clItem[28].siUnique );
	pSectionData->AddDataStr("pInvName14",pInvName14);
	pSectionData->AddDataInt("Itemnum28",Itemnum28);
	pSectionData->AddDataInt( "inv15", pclMsg->clItemInfo.clItem[29].siUnique );
	pSectionData->AddDataStr("pInvName15",pInvName15);
	pSectionData->AddDataInt("Itemnum29",Itemnum29);
	pSectionData->AddDataInt( "inv16", pclMsg->clItemInfo.clItem[30].siUnique );
	pSectionData->AddDataStr("pInvName16",pInvName16);
	pSectionData->AddDataInt("Itemnum30",Itemnum30);
	pSectionData->AddDataInt( "inv17", pclMsg->clItemInfo.clItem[31].siUnique );
	pSectionData->AddDataStr("pInvName17",pInvName17);
	pSectionData->AddDataInt("Itemnum31",Itemnum31);
	pSectionData->AddDataInt( "inv18", pclMsg->clItemInfo.clItem[32].siUnique );
	pSectionData->AddDataStr("pInvName18",pInvName18);
	pSectionData->AddDataInt("Itemnum32",Itemnum32);
	pSectionData->AddDataInt( "inv19", pclMsg->clItemInfo.clItem[33].siUnique );
	pSectionData->AddDataStr("pInvName19",pInvName19);
	pSectionData->AddDataInt("Itemnum33",Itemnum33);
	pSectionData->AddDataInt( "inv20", pclMsg->clItemInfo.clItem[34].siUnique );
	pSectionData->AddDataStr("pInvName20",pInvName20);
	pSectionData->AddDataInt("Itemnum34",Itemnum34);
	pSectionData->EndData( "charinventory2" );

	pSectionData->BeginData( "animalinventory" );
	pSectionData->AddDataInt( "inv21", pclMsg->clItemInfo.clItem[35].siUnique );
	pSectionData->AddDataStr("pInvName21",pInvName21);
	pSectionData->AddDataInt("Itemnum35",Itemnum35);
	pSectionData->AddDataInt( "inv22", pclMsg->clItemInfo.clItem[36].siUnique );
	pSectionData->AddDataStr("pInvName22",pInvName22);
	pSectionData->AddDataInt("Itemnum36",Itemnum36);
	pSectionData->AddDataInt( "inv23", pclMsg->clItemInfo.clItem[37].siUnique );
	pSectionData->AddDataStr("pInvName23",pInvName23);
	pSectionData->AddDataInt("Itemnum37",Itemnum37);
	pSectionData->AddDataInt( "inv24", pclMsg->clItemInfo.clItem[38].siUnique );
	pSectionData->AddDataStr("pInvName24",pInvName24);
	pSectionData->AddDataInt("Itemnum38",Itemnum38);
	pSectionData->AddDataInt( "inv25", pclMsg->clItemInfo.clItem[39].siUnique );
	pSectionData->AddDataStr("pInvName25",pInvName25);
	pSectionData->AddDataInt("Itemnum39",Itemnum39);
	pSectionData->AddDataInt( "inv26", pclMsg->clItemInfo.clItem[40].siUnique );
	pSectionData->AddDataStr("pInvName26",pInvName26);
	pSectionData->AddDataInt("Itemnum40",Itemnum40);
	pSectionData->AddDataInt( "inv27", pclMsg->clItemInfo.clItem[41].siUnique );
	pSectionData->AddDataStr("pInvName27",pInvName27);
	pSectionData->AddDataInt("Itemnum41",Itemnum41);
	pSectionData->AddDataInt( "inv28", pclMsg->clItemInfo.clItem[42].siUnique );
	pSectionData->AddDataStr("pInvName28",pInvName28);
	pSectionData->AddDataInt("Itemnum42",Itemnum42);
	pSectionData->AddDataInt( "inv29", pclMsg->clItemInfo.clItem[43].siUnique );
	pSectionData->AddDataStr("pInvName29",pInvName29);
	pSectionData->AddDataInt("Itemnum43",Itemnum43);
	pSectionData->AddDataInt( "inv30", pclMsg->clItemInfo.clItem[44].siUnique );
	pSectionData->AddDataStr("pInvName30",pInvName30);
	pSectionData->AddDataInt("Itemnum44",Itemnum44);
	pSectionData->EndData( "animalinventory" );

	pSectionData->BeginData( "animalinventory2" );
	pSectionData->AddDataInt( "inv31", pclMsg->clItemInfo.clItem[45].siUnique );
	pSectionData->AddDataStr("pInvName31",pInvName31);
	pSectionData->AddDataInt("Itemnum45",Itemnum45);
	pSectionData->AddDataInt( "inv32", pclMsg->clItemInfo.clItem[46].siUnique );
	pSectionData->AddDataStr("pInvName32",pInvName32);
	pSectionData->AddDataInt("Itemnum46",Itemnum46);
	pSectionData->AddDataInt( "inv33", pclMsg->clItemInfo.clItem[47].siUnique );
	pSectionData->AddDataStr("pInvName33",pInvName33);
	pSectionData->AddDataInt("Itemnum47",Itemnum47);
	pSectionData->AddDataInt( "inv34", pclMsg->clItemInfo.clItem[48].siUnique );
	pSectionData->AddDataStr("pInvName34",pInvName34);
	pSectionData->AddDataInt("Itemnum48",Itemnum48);
	pSectionData->AddDataInt( "inv35", pclMsg->clItemInfo.clItem[49].siUnique );
	pSectionData->AddDataStr("pInvName35",pInvName35);
	pSectionData->AddDataInt("Itemnum49",Itemnum49);
	pSectionData->AddDataInt( "inv36", pclMsg->clItemInfo.clItem[50].siUnique );
	pSectionData->AddDataStr("pInvName36",pInvName36);
	pSectionData->AddDataInt("Itemnum50",Itemnum50);
	pSectionData->AddDataInt( "inv37", pclMsg->clItemInfo.clItem[51].siUnique );
	pSectionData->AddDataStr("pInvName37",pInvName37);
	pSectionData->AddDataInt("Itemnum51",Itemnum51);
	pSectionData->AddDataInt( "inv38", pclMsg->clItemInfo.clItem[52].siUnique );
	pSectionData->AddDataStr("pInvName38",pInvName38);
	pSectionData->AddDataInt("Itemnum52",Itemnum52);
	pSectionData->AddDataInt( "inv39", pclMsg->clItemInfo.clItem[53].siUnique );
	pSectionData->AddDataStr("pInvName39",pInvName39);
	pSectionData->AddDataInt("Itemnum53",Itemnum53);
	pSectionData->AddDataInt( "inv40", pclMsg->clItemInfo.clItem[54].siUnique );
	pSectionData->AddDataStr("pInvName40",pInvName40);
	pSectionData->AddDataInt("Itemnum54",Itemnum54);
	pSectionData->EndData( "animalinventory2" );

	pSectionData->BeginData( "bag" );
	pSectionData->AddDataInt( "inv41", pclMsg->clItemInfo.clItem[55].siUnique );
	pSectionData->AddDataStr("pInvName41",pInvName41);
	pSectionData->AddDataInt("Itemnum55",Itemnum55);
	pSectionData->AddDataInt( "inv42", pclMsg->clItemInfo.clItem[56].siUnique );
	pSectionData->AddDataStr("pInvName42",pInvName42);
	pSectionData->AddDataInt("Itemnum56",Itemnum56);
	pSectionData->AddDataInt( "inv43", pclMsg->clItemInfo.clItem[57].siUnique );
	pSectionData->AddDataStr("pInvName43",pInvName43);
	pSectionData->AddDataInt("Itemnum57",Itemnum57);
	pSectionData->AddDataInt( "inv44", pclMsg->clItemInfo.clItem[58].siUnique );
	pSectionData->AddDataStr("pInvName44",pInvName44);
	pSectionData->AddDataInt("Itemnum58",Itemnum58);
	pSectionData->AddDataInt( "inv45", pclMsg->clItemInfo.clItem[59].siUnique );
	pSectionData->AddDataStr("pInvName45",pInvName45);
	pSectionData->AddDataInt("Itemnum59",Itemnum59);
	pSectionData->AddDataInt( "inv46", pclMsg->clItemInfo.clItem[60].siUnique );
	pSectionData->AddDataStr("pInvName46",pInvName46);
	pSectionData->AddDataInt("Itemnum60",Itemnum60);
	pSectionData->AddDataInt( "inv47", pclMsg->clItemInfo.clItem[61].siUnique );
	pSectionData->AddDataStr("pInvName47",pInvName47);
	pSectionData->AddDataInt("Itemnum61",Itemnum61);
	pSectionData->AddDataInt( "inv48", pclMsg->clItemInfo.clItem[62].siUnique );
	pSectionData->AddDataStr("pInvName48",pInvName48);
	pSectionData->AddDataInt("Itemnum62",Itemnum62);
	pSectionData->AddDataInt( "inv49", pclMsg->clItemInfo.clItem[63].siUnique );
	pSectionData->AddDataStr("pInvName49",pInvName49);
	pSectionData->AddDataInt("Itemnum63",Itemnum63);
	pSectionData->AddDataInt( "inv50", pclMsg->clItemInfo.clItem[64].siUnique );
	pSectionData->AddDataStr("pInvName50",pInvName50);
	pSectionData->AddDataInt("Itemnum64",Itemnum64);
	pSectionData->AddDataInt( "inv51", pclMsg->clItemInfo.clItem[65].siUnique );
	pSectionData->AddDataStr("pInvName51",pInvName51);
	pSectionData->AddDataInt("Itemnum65",Itemnum65);
	pSectionData->AddDataInt( "inv52", pclMsg->clItemInfo.clItem[66].siUnique );
	pSectionData->AddDataStr("pInvName52",pInvName52);
	pSectionData->AddDataInt("Itemnum66",Itemnum66);
	pSectionData->EndData( "bag" );

	pSectionData->BeginData( "bag2" );
	pSectionData->AddDataInt( "inv53", pclMsg->clItemInfo.clItem[67].siUnique );
	pSectionData->AddDataStr("pInvName53",pInvName53);
	pSectionData->AddDataInt("Itemnum67",Itemnum67);
	pSectionData->AddDataInt( "inv54", pclMsg->clItemInfo.clItem[68].siUnique );
	pSectionData->AddDataStr("pInvName54",pInvName54);
	pSectionData->AddDataInt("Itemnum68",Itemnum68);
	pSectionData->AddDataInt( "inv55", pclMsg->clItemInfo.clItem[69].siUnique );
	pSectionData->AddDataStr("pInvName55",pInvName55);
	pSectionData->AddDataInt("Itemnum69",Itemnum69);
	pSectionData->AddDataInt( "inv56", pclMsg->clItemInfo.clItem[70].siUnique );
	pSectionData->AddDataStr("pInvName56",pInvName56);
	pSectionData->AddDataInt("Itemnum70",Itemnum70);
	pSectionData->AddDataInt( "inv57", pclMsg->clItemInfo.clItem[71].siUnique );
	pSectionData->AddDataStr("pInvName57",pInvName57);
	pSectionData->AddDataInt("Itemnum71",Itemnum71);
	pSectionData->AddDataInt( "inv58", pclMsg->clItemInfo.clItem[72].siUnique );
	pSectionData->AddDataStr("pInvName58",pInvName58);
	pSectionData->AddDataInt("Itemnum72",Itemnum72);
	pSectionData->AddDataInt( "inv59", pclMsg->clItemInfo.clItem[73].siUnique );
	pSectionData->AddDataStr("pInvName59",pInvName59);
	pSectionData->AddDataInt("Itemnum73",Itemnum73);
	pSectionData->AddDataInt( "inv60", pclMsg->clItemInfo.clItem[74].siUnique );
	pSectionData->AddDataStr("pInvName60",pInvName60);
	pSectionData->AddDataInt("Itemnum74",Itemnum74);
	pSectionData->AddDataInt( "inv61", pclMsg->clItemInfo.clItem[75].siUnique );
	pSectionData->AddDataStr("pInvName61",pInvName61);
	pSectionData->AddDataInt("Itemnum75",Itemnum75);
	pSectionData->AddDataInt( "inv62", pclMsg->clItemInfo.clItem[76].siUnique );
	pSectionData->AddDataStr("pInvName62",pInvName62);
	pSectionData->AddDataInt("Itemnum76",Itemnum76);
	pSectionData->AddDataInt( "inv63", pclMsg->clItemInfo.clItem[77].siUnique );
	pSectionData->AddDataStr("pInvName63",pInvName63);
	pSectionData->AddDataInt("Itemnum77",Itemnum77);
	pSectionData->AddDataInt( "inv64", pclMsg->clItemInfo.clItem[78].siUnique );
	pSectionData->AddDataStr("pInvName64",pInvName64);
	pSectionData->AddDataInt("Itemnum78",Itemnum78);
	pSectionData->EndData( "bag2" );	

	// 아이템 수정에 필요한 PersonID
	pSectionData->BeginData( "item_add" );
		pSectionData->AddDataInt( "personid", pclMsg->siPersonID ); // PersonID		
	pSectionData->EndData( "item_add" );

	sGMResponse_Section sectionsbasiconfomsg( GMMSG_PAGEID_INVENTORY, (char*)pclMsg->Charname, pSectionData );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&sectionsbasiconfomsg ); 
	delete pSectionData;
	// 페이지 종료 알림
	sGMResponse_EndPage endmsg( pclMsg->siPAGENum );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&endmsg ); 
	
}

void cltServer :: DoMsg_DBMSG_RESPONSE_GMTOOL_SUMMONINFO(sPacketHeader* pPacket)
{
	GMSectionData*	pSectionData;
	pSectionData = new GMSectionData( 200 * 1024 );
	sDBResponse_SummonByName* pclMsg = (sDBResponse_SummonByName*)pPacket;
	//메세지 보냄 알림
	sGMResponse_BeginPage beginmsg( pclMsg->siPAGENum, pclMsg->Charname );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&beginmsg ); 
	

	//여기 부터 데이터

	// 첫번째 소환수.
	cltSummon* pSummon = pclMsg->clSummonInfo.GetSummon(0);
	//cltKindInfo* pclki = pclKindInfoSet->pclKI[pSummon->siKind];
	cltKindInfo* pclki = NULL;
	if(pclClient->IsCountrySwitch(Switch_Summon_Separate)) 
	{
		pclki = pclClient->pclKindInfoSet->pclSummonKI[pSummon->siKind];
	}
	else
	{
		pclki = pclClient->pclKindInfoSet->pclKI[pSummon->siKind];
	}

	SI32 summonlevel1 = pSummon->clIP.GetLevel();

	if(pclki == NULL )
	{
		return;
	}

	//GetRequiredExpForLevel(summonlevel1 + 1) 
	pSectionData->ClearData();
	pSectionData->BeginData( "summoninfo1" );
	pSectionData->AddDataStr( "summonname1",		pSummon->szName );
	pSectionData->AddDataStr( "summonkind1",		(char*)pclki->GetName());
	pSectionData->AddDataFormat( "summonlevel1",	"%d / 100( LV %d )",	summonlevel1, pclki->siEvolveLevel);
	pSectionData->AddDataFormat( "summonexp1",	"%d / %d",	pSummon->clIP.GetCurExp(),	GetRequiredExpForLevel(summonlevel1 + 1) );
	pSectionData->AddDataFormat( "summonHP1",		"%d / %s",	pSummon->clIP.GetLife(),"No Data in DataBase");
	pSectionData->AddDataFormat( "summonMP1",		"%d / %d",	pSummon->clIP.GetMana(),"No Data in DataBase");
	pSectionData->EndData( "summoninfo1" );

	pSectionData->BeginData( "summonstatus1" );
	pSectionData->AddDataInt( "summonstr1",   pSummon->clIP.GetStr());
	pSectionData->AddDataInt( "summonagi1",   pSummon->clIP.GetMag());
	pSectionData->AddDataInt( "summonwis1",   pSummon->clIP.GetMag());
	pSectionData->AddDataInt( "summonvit1",   pSummon->clIP.GetVit());
	pSectionData->AddDataInt( "summonbonus1", pSummon->clIP.GetBonus());
	pSectionData->EndData( "summonstatus1" );

	// 공격력과 방어력(DB에는 공격력과 방어력이 저장되지 않습니다,.)
	pSectionData->BeginData( "summonattackinfo1" );
	pSectionData->AddDataStr( "summonattack1","No Data From DataBase");
	pSectionData->AddDataStr( "summondefense1","No Data From DataBase");
	pSectionData->EndData( "summonattackinfo1" );


	// 두번째 소환수.
	pSummon = pclMsg->clSummonInfo.GetSummon(1);
	pclki	= pclKindInfoSet->pclKI[pSummon->siKind];
	SI32 summonlevel2 = pSummon->clIP.GetLevel();
	//GetRequiredExpForLevel(summonlevel1 + 1) 

	if(pclki == NULL )
	{
		return;
	}
	
	pSectionData->BeginData( "summoninfo2" );
	pSectionData->AddDataStr( "summonname2",		pSummon->szName );
	pSectionData->AddDataStr( "summonkind2",		(char*)pclki->GetName() );
	pSectionData->AddDataFormat( "summonlevel2",	"%d / 100( LV %d )",summonlevel1 ,   pclki->siEvolveLevel);
	pSectionData->AddDataFormat( "summonexp2",	"%d / %d",			 pSummon->clIP.GetCurExp(),GetRequiredExpForLevel(summonlevel1 + 1));
	pSectionData->AddDataFormat( "summonHP2",		"%d / %s",			 pSummon->clIP.GetLife(),"No Data in DataBase");
	pSectionData->AddDataFormat( "summonMP2",		"%d / %s",			 pSummon->clIP.GetMana(),"No Data in DataBase");
	pSectionData->EndData( "summoninfo2" );

	pSectionData->BeginData( "summonstatus2" );
	pSectionData->AddDataInt( "summonstr2",	pSummon->clIP.GetStr());
	pSectionData->AddDataInt( "summonagi2",	pSummon->clIP.GetMag());
	pSectionData->AddDataInt( "summonwis2",	pSummon->clIP.GetMana());
	pSectionData->AddDataInt( "summonvit2",	pSummon->clIP.GetVit());
	pSectionData->AddDataInt( "summonbonus2",	pSummon->clIP.GetBonus());
	pSectionData->EndData( "summonstatus2" );

	// 공격력과 방어력(DB에는 공격력과 방어력이 저장되지 않습니다,.)
	pSectionData->BeginData( "summonattackinfo2" );
	pSectionData->AddDataStr( "summonattack2","No Data From DataBase");
	pSectionData->AddDataStr( "summondefense2",	"No Data From DataBase");
	pSectionData->EndData( "summonattackinfo2" );

	// 소환수 수정에 필요한 PersonID
	pSectionData->BeginData( "summon_add" );
		pSectionData->AddDataInt( "personid", pclMsg->siPersonID ); // PersonID		
	pSectionData->EndData( "summon_add" );

	sGMResponse_Section sectionsbasiconfomsg( GMMSG_PAGEID_SUMMON, (char*)pclMsg->Charname, pSectionData );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&sectionsbasiconfomsg ); 
	delete pSectionData;
	// 페이지 종료 알림
	sGMResponse_EndPage endmsg( pclMsg->siPAGENum );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&endmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_ENDPAGE\n" );
}
//스킬/스킬쪽은 아이템 파일리스트 떄문에 내일 한동희 과장님꼐 꼭 말씀 드려야 함
void cltServer :: DoMsg_DBMSG_RESPONSE_GMTOOL_SKILLINFO(sPacketHeader* pPacket){
	GMSectionData*	pSectionData;
	pSectionData = new GMSectionData( 200 * 1024 );

	sDBResponse_SkillByName* pclMsg = (sDBResponse_SkillByName*)pPacket;
	
	//메세지 시작 알림
	sGMResponse_BeginPage beginmsg( pclMsg->siPAGENum, pclMsg->szCharname);
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&beginmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_BEGINPAGE\n" );
	//여기 부터 데이터
	TCHAR* pSkillExplain = NULL;
	pSectionData->ClearData();
	TCHAR* strSkill =  "%d / 100";
	cltItem* pclitemlist = &pclMsg->clItemInfo.clItem[0];

	pSectionData->BeginData( "make_equip" );
	pSectionData->AddDataFormat( "make_armor",strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEARMOUR1, 0, 0, pclitemlist));
	////pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEARMOUR1);				
	pSectionData->AddDataStr( "explain1","No Data from DataBase");
	pSectionData->AddDataFormat( "make_helm",	 strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEHELMET1, 0, 0, pclitemlist));
	////pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEHELMET1);				
	pSectionData->AddDataStr( "explain2","No Data from DataBase");
	pSectionData->EndData( "make_equip" );


	pSectionData->BeginData( "make_weapon");
	pSectionData->AddDataFormat( "make_sword",	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKESWORD1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKESWORD1);				
	pSectionData->AddDataStr( "explain3","No Data from DataBase");
	pSectionData->AddDataFormat( "make_spear",	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKESPEAR1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKESPEAR1);				
	pSectionData->AddDataStr( "explain4","No Data from DataBase");
	pSectionData->AddDataFormat( "make_axe",		strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEAXE1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEAXE1);				
	pSectionData->AddDataStr( "explain5","No Data from DataBase");
	pSectionData->AddDataFormat( "make_cane",	 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKESTAFF1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKESTAFF1);				
	pSectionData->AddDataStr( "explain6","No Data from DataBase");
	pSectionData->AddDataFormat( "make_bow" ,	 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEBOW1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEBOW1);				
	pSectionData->AddDataStr( "explain7","No Data from DataBase");
	pSectionData->AddDataFormat( "make_rifle",	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEGUN1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEGUN1);				
	pSectionData->AddDataStr( "explain8","No Data from DataBase");
	pSectionData->EndData( "make_weapon" );

	pSectionData->BeginData( "make_acc" );
	pSectionData->AddDataFormat( "make_shoes",	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKESHOES1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKESHOES1);				
	pSectionData->AddDataStr( "explain9","No Data from DataBase");
	pSectionData->AddDataFormat( "make_belt",	 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEBELT1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEBELT1);				
	pSectionData->AddDataStr( "explain10","No Data from DataBase");
	pSectionData->EndData( "make_acc" );
	
	pSectionData->BeginData( "make_tool" );
	pSectionData->AddDataFormat( "make_iron",	 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEIRON1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEIRON1);				
	pSectionData->AddDataStr( "explain11","No Data from DataBase");
	pSectionData->AddDataFormat( "make_tool",	 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKETOOL1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKETOOL1);				
	pSectionData->AddDataStr( "explain12","No Data from DataBase");
	pSectionData->EndData( "make_tool" );

	pSectionData->BeginData( "make_basic" );
	pSectionData->AddDataFormat( "make_prescription",	 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_CAPTURE, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_CAPTURE);				
	pSectionData->AddDataStr( "explain13","No Data from DataBase");
	pSectionData->AddDataFormat( "make_cooking",			strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MAKEFOOD, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MAKEFOOD);				
	pSectionData->AddDataStr( "explain14","No Data from DataBase");
	pSectionData->AddDataFormat( "make_farming",			strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_FARM, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_FARM);				
	pSectionData->AddDataStr( "explain15","No Data from DataBase");
	pSectionData->AddDataFormat( "make_mining",			strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_MINE, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_MINE);				
	pSectionData->AddDataStr( "explain16","No Data from DataBase");
	pSectionData->EndData( "make_basic" );

	pSectionData->BeginData( "uesage_weapon" );
	pSectionData->AddDataFormat( "uesage_sword", 	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_USESWORD1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_USESWORD1);				
	pSectionData->AddDataStr( "explain17","No Data from DataBase");
	pSectionData->AddDataFormat( "uesage_spear",  strSkill,	pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_USESPEAR1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_USESPEAR1);				
	pSectionData->AddDataStr( "explain18","No Data from DataBase");
	pSectionData->AddDataFormat( "uesage_axe",	strSkill,	pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_USEAXE1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_USEAXE1);				
	pSectionData->AddDataStr( "explain19","No Data from DataBase");
	pSectionData->AddDataFormat( "uesage_cane",   strSkill,	pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_USESTAFF1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_USESTAFF1);				
	pSectionData->AddDataStr( "explain20","No Data from DataBase");
	pSectionData->AddDataFormat( "uesage_bow",	strSkill,	pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_USEBOW1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_USEBOW1);				
	pSectionData->AddDataStr( "explain21","No Data from DataBase");
	pSectionData->AddDataFormat( "uesage_rifle",  strSkill,	pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_USEGUN1, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_USEGUN1);				
	pSectionData->AddDataStr( "explain22","No Data from DataBase");
	pSectionData->EndData( "uesage_weapon" );

	pSectionData->BeginData( "useage_ocean" );
	pSectionData->AddDataFormat( "useage_fishing",  	strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_FISH, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_FISH);				
	pSectionData->AddDataStr( "explain23","No Data from DataBase");
	pSectionData->EndData( "useage_ocean" );

	pSectionData->BeginData( "useage_other" );
	pSectionData->AddDataFormat( "summon",		strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_SUMMON, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_SUMMON);				
	pSectionData->AddDataStr( "explain24","No Data from DataBase");

	pSectionData->AddDataFormat( "horsetrining",  strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_HORSETRAINING, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_HORSETRAINING);				
	pSectionData->AddDataStr( "explain25","No Data from DataBase");
	pSectionData->AddDataFormat( "capture",		strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_CAPTURE, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_CAPTURE);				
	pSectionData->AddDataStr( "explain26","No Data from DataBase");
	pSectionData->AddDataFormat( "artifact",		strSkill, pclMsg->SkillInfo.GetTotalSkillLevel(SKILL_ARTIFACT, 0, 0, pclitemlist));
	//pSkillExplain = SkillExplain((cltCharClient*)pclClient->pclCM->CR[siCharID], SKILL_CAPTURE);				
	pSectionData->AddDataStr( "explain26","No Data from DataBase");//아티팩트 결정 제조 스킬
	pSectionData->EndData( "useage_other" );

	// 스킬 수정에 필요한 PersonID
	pSectionData->BeginData( "skill_add" );
		pSectionData->AddDataInt( "personid", pclMsg->siPersonID ); // PersonID		
	pSectionData->EndData( "skill_add" );

	sGMResponse_Section sectionsbasiconfomsg( GMMSG_PAGEID_SKILLINFO, (char*)pclMsg->szCharname, pSectionData );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&sectionsbasiconfomsg ); 
	delete pSectionData; 

	// 페이지 종료 알림
	sGMResponse_EndPage endmsg( pclMsg->siPAGENum );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&endmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_ENDPAGE\n" );
}
//부동산.
void cltServer :: DoMsg_DBMSG_RESPONSE_GMTOOL_REALESTATEINFO(sPacketHeader* pPacket){
	GMSectionData*	pSectionData;
	pSectionData = new GMSectionData( 200 * 1024 );
	sDBResponse_RealestateByName* pclMsg = (sDBResponse_RealestateByName*)pPacket;
	
	//메세지 시작 알림
	sGMResponse_BeginPage beginmsg( pclMsg->siPAGENum, pclMsg->Charname );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&beginmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_BEGINPAGE\n" );
	
	//여기 부터 데이터
	SI32 RealEstateCnt = pclMsg->clRealEstateInfo.GetRealEstateNum();//부동산이 하나도 없으면 for문으로 들어가지 않는다; 
	// 창고 구분.
	SI32 warehouseCnt = 1;
	// 마굿간 구분.
	SI32 stableCnt = 0;
	char	TableStr[128];
	ZeroMemory(TableStr, sizeof(TableStr));

	char	TempStr[128];
	ZeroMemory(TempStr, sizeof(TempStr));

	char	szDate[1024];
	ZeroMemory(szDate, sizeof(szDate));

	for(SI32 Cnt = 0; Cnt < RealEstateCnt; ++Cnt)
	{
		// 마을 정보 얻기.
		SI32  VillageNo	  = pclMsg->clRealEstateInfo.clRealEstate[Cnt].siVillageUnique;
		SI32  HouseIndex  = pclMsg->clRealEstateInfo.clRealEstate[Cnt].siIndex;
		char* VillageName = pclVillageManager->pclVillageInfo[VillageNo]->szName;

		//------------------------------------------------------------------------------------------------------------------
		TCHAR temp[1024] = TEXT("");
		TCHAR temp1[1024] = TEXT("");
		TCHAR buffer1[1024] = TEXT("");
		TCHAR buffer2[1024] = TEXT("");

		SI32 id = 1;
		cltRealEstateUnit* pclinfo = pclMsg->clRealEstateInfo.Find(VillageNo, pclMsg->clRealEstateInfo.clRealEstate[Cnt].siType, HouseIndex );

		if(pclinfo)
		{
			TCHAR* pText = GetTxtFromMgr(2136);

			StringCchPrintf( temp, 1024, pText, pclinfo->clContract.siRentFee );
			StringCchCopy( buffer1, 1024, temp );

			pText = GetTxtFromMgr(2137);
			StringCchPrintf( temp, 1024, pText, pclinfo->clContract.siRentPeriod );
			StringCchCat( buffer1, 1024, temp );

			if ( pclinfo->clContract.clDate.GetDateText( temp ) == TRUE )
			{
				pText = GetTxtFromMgr(2138);
				StringCchPrintf( temp1, 1024, pText, temp );
				StringCchCat( buffer1, 1024, temp1 );
			}
		}

		memset( buffer2, 0, 1024 );
		TCHAR* pText = GetTxtFromMgr(2136);
		StringCchPrintf( temp, 1024, pText,pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee);
		StringCchCopy( buffer2, 1024, temp );
		pText = GetTxtFromMgr(2137);
		StringCchPrintf( temp, 1024, pText,pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod );
		StringCchCat( buffer2, 1024, temp );
		//------------------------------------------------------------------------------------------------------------------

		// 창고 아이템.
		cltItem* pItem = &pclHouseManager->clClientUnit[HouseIndex].clStorageInfo.clItem[0];

		// 아이템, 말, 소환수가 시전안에 있나 확인한다.
		cltHouseUnitStorageInfo* pclstg =pclHouseManager->GetStgPointer(VillageNo, HouseIndex);

		switch(pclMsg->clRealEstateInfo.clRealEstate[Cnt].siType)
		{
			// 창고.
		case 1:	
			{
				sprintf(TableStr, "wherehouseinfo%d", warehouseCnt);

				pSectionData->BeginData(TableStr);
				{					
					sprintf(TempStr, "village%d", warehouseCnt);
					pSectionData->AddDataStr( TempStr, VillageName );

					sprintf(TempStr, "rentperiod%d", warehouseCnt);
					pSectionData->AddDataStr( TempStr, buffer1 );

					char str[7]   = {'i', 't', 'e', 'm', NULL, NULL, NULL};
					char str2[64] = {NULL, };
					cltItemInfo* ItemInfo = NULL;

					for(SI32 i = 1; i <= MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i)
					{
						if(pclstg->clItem[i-1].siUnique)
						{
							ItemInfo =pclItemManager->FindItemInfo(pclstg->clItem[i-1].siUnique);
							sprintf(str2, "%s(%d)", (char*)ItemInfo->GetName(), pclstg->clItem[i-1].GetItemNum());

							itoa(i, &str[4], 10);
							pSectionData->AddDataStr(str, str2); 
						}
					}
				}
				++warehouseCnt;
				pSectionData->EndData(TableStr);


				// 마굿간.
				pSectionData->BeginData( "stableinfo" );

				for(SI32 i = 0; i < MAX_HOUSEUNIT_HORSENUMBER; ++i)
				{
					// 마굿간이 있다.
					if(pclstg->bCreatedHouseUnitStable[i])
					{
						sprintf(TableStr, "village%d", stableCnt);
						pSectionData->AddDataStr( TableStr,		VillageName );

						sprintf(TableStr, "rentperiod%d", stableCnt);
						pSectionData->AddDataStr( TableStr,	buffer1 );

						// 말 유니크로 말 이름 얻기.
						char* pHorseName = pclHorseManager->GetHorseName(pclstg->clHorse[stableCnt].siHorseUnique);
						sprintf(TableStr, "horse%d", stableCnt);
						pSectionData->AddDataStr( TableStr, pHorseName );

						++stableCnt;
					}
				}
				pSectionData->EndData( "stableinfo" );		
			}
			break;

			// 농장.
		case 2: 
			{
				pSectionData->BeginData( "farminfo" );
				{
					pSectionData->AddDataStr( "village",		VillageName );
					pSectionData->AddDataStr( "rentperiod",	buffer1 );
				}
				pSectionData->EndData( "farminfo" );	
			}
			break;

			// 광산.
		case 3:	
			{
				pSectionData->BeginData( "mineinfo" );
				{
					pSectionData->AddDataStr( "village",		VillageName );
					pSectionData->AddDataStr( "rentperiod",	buffer1 );
				}
				pSectionData->EndData( "mineinfo" );		
			}
			break;
		}
	}

	// 스킬 수정에 필요한 PersonID
	pSectionData->BeginData( "relestate_add" );
		pSectionData->AddDataInt( "personid", pclMsg->siPersonID ); // PersonID		
	pSectionData->EndData( "relestate_add" );

	sGMResponse_Section sectionsbasiconfomsg( GMMSG_PAGEID_REALESTATE, (char*)pclMsg->Charname, pSectionData );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&sectionsbasiconfomsg ); 
	delete pSectionData; 
	//메세지 종료알림
	sGMResponse_EndPage endmsg( pclMsg->siPAGENum );
	pclGMProtocol->SendPacket( pclMsg->siClientID, (sPacketHeader*)&endmsg ); 
	printf( "메시지 보냄 : GMMSG_RESPONSE_ENDPAGE\n" );
}
//여기도 좀 수정이 필요하삼.
//TCHAR* SkillExplain(cltCharClient* _pclchar, SI32 _SkillUnique)
//{
//	cltCharClient* pclchar = _pclchar;
//
//	static TCHAR Buffer[ 512 ] =  TEXT("");
//
//
//	SI32 skillexp = 0;
//	if(SKILL_USESWORD1 >= 0)
//	{
//		skillexp   = pclchar->pclCI->clCharSkill.clSkill[_SkillUnique].siSkillExp;
//	}
//
//	SI32 explevel	= pclchar->pclCI->clCharSkill.GetSkillExpLevel(_SkillUnique);
//
//	TCHAR* pText = GetTxtFromMgr(675);
//
//	StringCchPrintf( Buffer, 512, pText,
//		pclchar->pclCI->clCharSkill.GetSkillLevel(_SkillUnique),
//		pclClient->pclSkillManager->GetMaxLevel(_SkillUnique),
//		pclClient->pclItemManager->GetUpgradeSkillLevel(&pclchar->pclCI->clCharItem.clItem[0], _SkillUnique,1),
//		explevel,
//		pclClient->pclSkillManager->GetMaxExpLevel(_SkillUnique),
//		skillexp,
//		GetRequiredSkillExpForLevel(explevel + 1));
//
//	return Buffer;
//}
