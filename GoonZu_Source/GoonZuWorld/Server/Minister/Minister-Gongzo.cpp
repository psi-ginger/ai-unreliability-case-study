
#include "..\..\Common\CommonHeader.h"
#include "..\..\CommonLogic\CommonLogic.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "..\Server.h"

#include "MinisterBase.h"
#include "Minister-Gongzo.h"

#include "..\..\CommonLogic\MsgRval-Define.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\CommonLogic\MsgType-System.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CMinisterGongzo::CMinisterGongzo() : CMinisterBase()
{
	m_siPriceTime = 0 ;
	m_siLeftOverValue = 100 ;
	m_siLeftOverValue_Armor = 50 ;
	for ( int i = 0 ; i < GONGZO_SKILL_TOTAL ; i ++ )
	{
		m_stGongzoMember.siPriceValue[i] = 0 ;
		m_stGongzoMember.siSkillValue[i] = 0 ;
	}
	m_stGongzoMember.siCraftFund = ConstCraftFund_Min;
}

CMinisterGongzo::~CMinisterGongzo()
{
}

// DB에 공조의 정보를 요청한다.
void CMinisterGongzo::InitializeDB()
{
	// 공조의 정보를 요청한다.
	sDBRequest_GetGongzoInfo clMsg;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

// DB에서 공조의 정보를 받으면 값을 Class에 설정한다.
void  CMinisterGongzo::DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket)
{

	sDBResponse_GetGongzoInfo* pclMsg = (sDBResponse_GetGongzoInfo *)pPacket;

	if(pclMsg->siResult)
	{
		SI16 siTotal = 0 ;

		StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szAccountID);
		StringCchCopy(m_szPersonName, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szPersonName);
		StringCchCopy(m_szGreeting, 256,	pclMsg->MinisterBase.m_szGreeting);

		m_siPriceTime = pclMsg->siPriceTime ;
		if(m_siPriceTime <= 0 ) m_siPriceTime = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() - 30 ;
		
		m_siLeftOverValue = pclMsg->siLeftOverValue ;
		for ( int i = GONGZO_SKILL_SWORD ; i <= GONGZO_SKILL_GUN ; i ++)
		{
			m_stGongzoMember.siPriceValue[i] = pclMsg->siPriceValue[i];
			if ( m_stGongzoMember.siPriceValue[i] > 50 || m_stGongzoMember.siPriceValue[i] < 0 )
			{
				m_stGongzoMember.siPriceValue[i] = 0 ;
			}

			siTotal += m_stGongzoMember.siPriceValue[i] ;
		}
		siTotal += m_siLeftOverValue ;

		bool bSuccess = DATA_TO_BOOL(siTotal) ;
		if ( !bSuccess )
		{
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_SWORD] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_SPEAR] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_STAFF] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_AXE] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_BOW] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_GUN] = 0 ;

			m_siLeftOverValue = 100 ;
		}
		else if ( siTotal != 100 )
		{
			siTotal -= m_siLeftOverValue ;
			m_siLeftOverValue = 100 - siTotal ;
		}

		siTotal = 0 ;
		m_siLeftOverValue_Armor = pclMsg->siLeftOverValue_Armor ;

		for ( int i = GONGZO_SKILL_HELMET ; i <= GONGZO_SKILL_RESERVED3 ; i ++)
		{
			m_stGongzoMember.siPriceValue[i] = pclMsg->siPriceValue[i];
			if ( m_stGongzoMember.siPriceValue[i] > 50 || m_stGongzoMember.siPriceValue[i] < 0 )
			{
				m_stGongzoMember.siPriceValue[i] = 0 ;
			}

			siTotal += m_stGongzoMember.siPriceValue[i] ;
		}
		siTotal += m_siLeftOverValue_Armor ;

		bSuccess = DATA_TO_BOOL(siTotal) ;
		if ( !bSuccess )
		{
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_HELMET] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_ARMOR] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_BELT] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_SHOES] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_RESERVED1] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_RESERVED2] = 0 ;
			m_stGongzoMember.siPriceValue[GONGZO_SKILL_RESERVED3] = 0 ;

			m_siLeftOverValue_Armor = 50 ;
		}
		else if ( siTotal != 50 )
		{
			siTotal -= m_siLeftOverValue_Armor ;
			m_siLeftOverValue_Armor = 50 - siTotal ;
		}

		m_stGongzoMember.siCraftFund = pclMsg->siCraftFund;
		if ( m_stGongzoMember.siCraftFund < ConstCraftFund_Min || 
			m_stGongzoMember.siCraftFund > ConstCraftFund_Max )
		{
			m_stGongzoMember.siCraftFund = ConstCraftFund_Min;
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetGongzoInfo"));
}

// Client에서 공조의 정보를 요청하면 지금 Class에 있는 정보를 전송한다.
void CMinisterGongzo::DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID)
{
	CMinisterBase *pMinister = (CMinisterBase* )(this);
	//((cltServer *)pclClient)->pclCM->siUserWeaponStatistic[1];

	SI32 TotalValue = 0 ;

	for ( int i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
		TotalValue += ((cltServer *)pclClient)->pclCM->siUserWeaponStatistic[i];

	for ( i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
	{
		REAL32	fSkillValue;
		// 반올림처리
		fSkillValue = ((REAL32)((cltServer *)pclClient)->pclCM->siUserWeaponStatistic[i] / (REAL32)TotalValue * 100 );
		fSkillValue += 0.5f;
		m_stGongzoMember.siSkillValue[i] = (SI16)fSkillValue;
	}

	cltGameMsgResponse_GetGongzoInfo clinfo(pMinister,m_siPriceTime, m_siLeftOverValue,&m_stGongzoMember, m_siLeftOverValue_Armor);

	cltMsg clMsg(GAMEMSG_RESPONSE_GETGONGZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
	((cltServer *)pclClient)->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);
}

// Client에서 공조의 정보 새로 설정하기를 원하면 서버에서는 DB에 정보 설정을 요청한다.
void CMinisterGongzo::DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID)
{
	cltGameMsgRequest_SetGongzoInfo *pclinfo = (cltGameMsgRequest_SetGongzoInfo*)pclMsg->cData;

	// 한달 이내에는 재처리하지 않는다. 
	SI32 curdate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary();
	if(m_siPriceTime + 30 >  curdate)return ;
	if(pclinfo->siLeftOverValue<0) return ;
	bool bSuccess;
	SI16 siTotal = 0 ;

	siTotal += pclinfo->siLeftOverValue;
	for ( int i = GONGZO_SKILL_SWORD ; i <= GONGZO_SKILL_GUN ; i ++ )
	{
		if ( pclinfo->siPriceValue[i] < 0 || pclinfo->siPriceValue[i] > 50 ) return ;
		siTotal += pclinfo->siPriceValue[i] ;
	}

	bSuccess = DATA_TO_BOOL(siTotal) ;
	if ( siTotal != 100 || !bSuccess ) return ;

	siTotal = 0 ;
	siTotal += pclinfo->siLeftOverValue_Armor;
	for ( int i = GONGZO_SKILL_HELMET ; i < GONGZO_SKILL_RESERVED3 ; i ++ )
	{
		if ( pclinfo->siPriceValue[i] < 0 || pclinfo->siPriceValue[i] > 50 ) return ;
		siTotal += pclinfo->siPriceValue[i] ;
	}
	bSuccess = DATA_TO_BOOL(siTotal) ;
	if ( siTotal != 50 || !bSuccess ) return ;

	sDBRequest_SetGongzoInfo clMsg(siID, ((cltServer *)pclClient)->pclCM->CR[siID]->pclCI->GetPersonID(),
		pclinfo->szGreeting, curdate,pclinfo->siLeftOverValue,pclinfo->siPriceValue, pclinfo->siCraftFund,
		pclinfo->siLeftOverValue_Armor);

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

// DB에서 정보의 쓰기가 완료되면 Client에게 다시 전송하여준다.
void CMinisterGongzo::DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket)
{
	sDBResponse_SetGongzoInfo* pclMsg = (sDBResponse_SetGongzoInfo*)pPacket;

	SI32 id = pclMsg->usCharID;

	if(((cltServer *)pclClient)->pclCM->IsAlive(id) == false)return ;

	if(pclMsg->siResult == 1)
	{
		// 동일인이라면 
		if(((cltServer *)pclClient)->pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
		{
			StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szAccountID);
			StringCchCopy(m_szPersonName, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szPersonName);
			StringCchCopy(m_szGreeting,	 256, pclMsg->MinisterBase.m_szGreeting);

			m_siPriceTime = pclMsg->siPriceTime;
			m_siLeftOverValue = pclMsg->siLeftOverValue;
			m_siLeftOverValue_Armor = pclMsg->siLeftOverValue_Armor;
			for ( int i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
				m_stGongzoMember.siPriceValue[i] = pclMsg->siPriceValue[i];

			m_stGongzoMember.siCraftFund = pclMsg->siCraftFund;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 10] // 공조설정.	(수정필요 : 배열정보 추가 못함.)
				// param1 == 시간, param2 == siLeftOverValue, param3 == 지원금, 
				// pszParam1 == 계정, pszParam2 == ???, 검제조, 창제조, 지팡이제조, 도끼제조?, 활제조, 총제조, 
				cltServer * pclserver = (cltServer*)pclClient;
				cltCharServer* pclchar = NULL;
				if(pclClient->pclCM->IsValidID(id))			pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				TCHAR szCharParam1[50] = { '\0', };
				TCHAR szCharParam2[50] = { '\0', };
				StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("ID:%s, Name:%s"), pclMsg->MinisterBase.m_szAccountID, pclMsg->MinisterBase.m_szPersonName);
				StringCchPrintf(szCharParam2, sizeof(szCharParam2), TEXT("%d/%d/%d/%d/%d/%d"),  pclMsg->siPriceValue[0],
					pclMsg->siPriceValue[1],
					pclMsg->siPriceValue[2],
					pclMsg->siPriceValue[3],
					pclMsg->siPriceValue[4],
					pclMsg->siPriceValue[5]);

				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_GONGZO_SET, 
					0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					pclMsg->siPriceTime, pclMsg->siLeftOverValue, pclMsg->siCraftFund, 0, 0, 
					szCharParam1, szCharParam2);
			}
		}
	}
	else
	{
		// 서버 Error
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETGONGZOINFO,  0, 0, id);
	}
	
	// 정보를 요청했을때와 같이 돌려준다.
	DoMsg_GameMsgRequest_GetMinisterInfo(id);

}

SI32 CMinisterGongzo::GetGongzoSkillValue(SI16 gongzomembervalue)
{
	if ( gongzomembervalue < GONGZO_SKILL_START || gongzomembervalue > GONGZO_SKILL_LAST ) return 0 ;
	if ( m_stGongzoMember.siPriceValue[gongzomembervalue] < 0 || m_stGongzoMember.siPriceValue[gongzomembervalue] > 50 ) return 0 ;

	return (SI32)(m_stGongzoMember.siPriceValue[gongzomembervalue]) ;
}