//---------------------------------
// 2003/6/21 ���°�
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

//-------------------------------------
// CommonLogic
//-------------------------------------
#include "msgType-Person.h"
#include "MsgType-System.h"

#include "../../Server/AuthServer/AuthMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"

#include "JwLib\IsCorrectID\IsCorrectID.h"

#include "MsgRval-Define.h"
#include "TraceBoot.h"

//�ɸ��� �������������� �ý��� �ð����� �߰�.
#include "../NLib/NUtil.h"


void cltServer::DoMsg_GAMEMSG_REQUEST_LOGIN(cltMsg* pclMsg, SI32 id)
{
	if( ConstServiceArea_China == siServiceArea )
	{
		if( false == pclCM->IsValidID(id) )
		{
			SendServerResponseMsg(0, SRVAL_LOGINFAIL_LIMITMAXUSER,  0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}

		if( 0 == pclCM->CR[id]->GetGameMasterLevel() )
		{
			if( siCurUserNum >= siLimitMaxUserNumber )
			{
				SendServerResponseMsg(0, SRVAL_LOGINFAIL_LIMITMAXUSER,  0, 0, pclCM->CR[id]->GetCharUnique());
				return;
			}
		}
	}
	//-----------------------------------------------------
	// ����ڷκ��� ���� ������ ���ͼ� �����Ѵ�. 
	//-----------------------------------------------------
	cltLogInInfo* pcllogininfo = (cltLogInInfo* )pclMsg->cData;

	TraceBoot("LOGIN REQ from client id=%d gameMode=%u authKey=%d user='%s'",
		id, (unsigned)pcllogininfo->uiGameMode, pcllogininfo->siAuthKey, pcllogininfo->szID);

	switch( pcllogininfo->uiGameMode )
	{
	case 0:	
		siWndInterfaceNum++;				// ���� ������ ����� �� ������â + ������ �������̽� ����� ������
		break;
	case 1:
		siWnd_NewInterfaceNum++;			// ���� ������ ����� �� ������â + ���������̽� ����� ������
		break;
	case 2:
		siFullWnd_NewInterfaceNum++;		// ���� ������ ����� �� ��üȭ�� + ���������̽� ����� ������
		break;
	}
	
	// ���������� ������ ��û�Ѵ�. 
	sRequest_GameAuth clMsg;
	clMsg.packetHeader.usSize = sizeof( clMsg );
	clMsg.packetHeader.usCmd = AUTHMSG_REQUEST_GAMEAUTH;
	
	clMsg.usCharID	= id;
	clMsg.siAuthKey	= pcllogininfo->siAuthKey;
	MStrCpy(clMsg.userid,	pcllogininfo->szID,			MAX_PLAYER_NAME );
	MStrCpy(clMsg.userpw,	pcllogininfo->szPassword,	MAX_PLAYER_NAME );
	MStrCpy(clMsg.userServerID,   szServerID, MAX_PLAYER_NAME );
	MStrCpy(clMsg.userIP,   pclCM->CR[id]->GetSession()->GetIP(), MAX_PLAYER_NAME );
	
	pclFrontAuthProcess->SendMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_GAMEMSG_REQUEST_HANAUTH_LOGIN(cltMsg* pclMsg, SI32 id)
{
	//-----------------------------------------------------
	// ����ڷκ��� ���� ������ ���ͼ� �����Ѵ�. 
	//-----------------------------------------------------
	cltLogIn_HanAuthInfo* pcllogininfo = (cltLogIn_HanAuthInfo* )pclMsg->cData;

	switch( pcllogininfo->uiGameMode )
	{
	case 0:	
		siWndInterfaceNum++;				// ���� ������ ����� �� ������â + ������ �������̽� ����� ������
		break;
	case 1:
		siWnd_NewInterfaceNum++;			// ���� ������ ����� �� ������â + ���������̽� ����� ������
		break;
	case 2:
		siFullWnd_NewInterfaceNum++;		// ���� ������ ����� �� ��üȭ�� + ���������̽� ����� ������
		break;
	}

	// ���������� ������ ��û�Ѵ�. 
	sRequest_HanAuth_GameAuth clMsg;
	clMsg.packetHeader.usSize = sizeof( clMsg );
	clMsg.packetHeader.usCmd = AUTHMSG_REQUEST_HANAUTH_GAMEAUTH;
	clMsg.usServerAge = siServiceAge;

	clMsg.usCharID	= id;
	clMsg.siAuthKey	= pcllogininfo->siAuthKey;
	MStrCpy(clMsg.authstring,	pcllogininfo->szAuthString,	2048 );
	MStrCpy(clMsg.userid,		pcllogininfo->szID,				MAX_PLAYER_NAME );

	pclFrontAuthProcess->SendMsg((sPacketHeader*)&clMsg);
}


void cltServer::DoMsg_GAMEMSG_REQUEST_HANAUTH_GAMERUNAUTH(cltMsg* pclMsg, SI32 id)
{
	//-----------------------------------------------------
	// ����ڷκ��� ���� ������ ���ͼ� �����Ѵ�. 
	//-----------------------------------------------------
	cltHanAuth_GameRunAuth* pclgamerunauth = (cltHanAuth_GameRunAuth* )pclMsg->cData;

	// ���������� ������ ��û�Ѵ�. 
/*	sRequest_HanAuth_GameRunAuth clMsg;
	clMsg.packetHeader.usSize = sizeof( clMsg );
	clMsg.packetHeader.usCmd = AUTHMSG_REQUEST_HANAUTH_GAMERUNAUTH;
	clMsg.usServerAge = siServiceAge;

	clMsg.usCharID	= id;
	MStrCpy(clMsg.authstring,	pclgamerunauth->authstring,	2048 );
	MStrCpy(clMsg.userid,		pclgamerunauth->userid,		MAX_PLAYER_NAME );

	pclFrontAuthProcess->SendMsg((sPacketHeader*)&clMsg);
	*/
}

void cltServer::DoMsg_GAMEMSG_REQUEST_TAIWAN_LOGIN(cltMsg* pclMsg, SI32 id)
{
	//-----------------------------------------------------
	// ����ڷκ��� ���� ������ ���ͼ� �����Ѵ�. 
	//-----------------------------------------------------
	cltLogIn_TaiwanInfo* pcllogininfo = (cltLogIn_TaiwanInfo* )pclMsg->cData;

	switch( pcllogininfo->uiGameMode )
	{
	case 0:	
		siWndInterfaceNum++;				// ���� ������ ����� �� ������â + ������ �������̽� ����� ������
		break;
	case 1:
		siWnd_NewInterfaceNum++;			// ���� ������ ����� �� ������â + ���������̽� ����� ������
		break;
	case 2:
		siFullWnd_NewInterfaceNum++;		// ���� ������ ����� �� ��üȭ�� + ���������̽� ����� ������
		break;
	}

	// ���������� ������ ��û�Ѵ�. 
	sRequest_Taiwan_GameAuth clMsg;
	clMsg.packetHeader.usSize = sizeof( clMsg );
	clMsg.packetHeader.usCmd = AUTHMSG_REQUEST_TAIWAN_GAMEAUTH;

	clMsg.usCharID	= id;
	clMsg.siAuthKey	= pcllogininfo->siAuthKey;

	TCHAR* pFind = _tcsstr(pcllogininfo->szGameAccount, "_");
	if(pFind)		{		*pFind = '\0';		}

	MStrCpy(clMsg.gameaccount,		pcllogininfo->szGameAccount,		sizeof(clMsg.gameaccount) );
	MStrCpy(clMsg.onetinepassword,	pcllogininfo->szOneTimePassword,	sizeof(clMsg.onetinepassword) );

	pclFrontAuthProcess->SendMsg((sPacketHeader*)&clMsg);
}



void cltServer::DoMsg_GAMEMSG_REQUEST_SELECTCHAR(cltMsg* pclMsg, SI32 id)
{
	SI32 index	= pclMsg->siData32[0];
	if(index < 0 || index >= 3)return ;
	
	SI32 idnum	= pclCM->CR[id]->pclCI->clBI.siIDNum;
	if(idnum <= 0)return;

	//---------------------------------------------------
	// ������ Person�� ��ü ������ DB�� ��û�Ѵ�. 
	//---------------------------------------------------
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	
    // �̹� ���õ� ĳ�����̰� �ε����� ������ �Ͱ� ���ٸ� �ߺ� �α����� ��û�ϴ� ���̴�. 
	if(pclchar->GetKind() != KIND_SYSTEM &&  pclchar->pclCI->clBI.uiIndex == index)
	{
		//------------------------------------------------------------------------------------	
		// [���� : Ȳ���� 2007. 11. 9 ĳ���͸� �����Ҷ� �ߺ��� ĳ���͸� ���ý� �����۸��� ������� ���� �ذ��� ���� 
		//								�ߺ��Ǵ� ĳ���͵� �ٽ� �ʱ�ȭ ��Ų��.]
		
		//return;

		//------------------------------------------------------------------------------------
	}
	


	// �ǰ� ���ų� �������̸� �ٲ� ������.
	// pvp ���̸� ������ �ٲܼ� ����.
	if ( pclchar->IsAutoHunting()  || pclchar->clInnerStatus.clEnemyType.IsType( ENEMYTYPE_PVP  ) )
	{
		SendServerResponseMsg(0, SRVAL_NO_BATTLEQUIT,  0, 0, pclchar->GetCharUnique());
		return ;
	}
	if ( pclchar->pclCI->clIP.GetLife() <= 0 )
	{
		SendServerResponseMsg(0, SRVAL_NO_DIEQUIT,  0, 0, pclchar->GetCharUnique());
		return ;
	}

	TCHAR* pip = pclchar->GetSession()->GetIP();
	if(pip)
	{
		sDBRequest_PersonTotalInfoGetByIDNum clMsg( id, idnum, index, pclchar->GetSession()->GetIP(),(bool)pclchar->pclCI->clDormancyInfo.GetDormancyState(), pclchar->bTiredSystem, pclchar->dwPlayingTime );
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);
	}
}



void cltServer::DoMsg_GAMEMSG_REQUEST_MAKENEWCHAR(cltMsg* pclMsg, SI32 id)
{
	cltCharListInfo* pclCharlistInfo = (cltCharListInfo*)pclMsg->cData;

	TCHAR buf[128];
	StringCchPrintf(buf, 128, pclCharlistInfo->clBI.GetName() );
#ifdef _IAF_EXPORT
	if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
	{
		TCHAR ResultBuf[1024] = "";
		IAF::Filtering( buf, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
		memcpy( buf, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
	}
#else
    m_pCAbuseFilter->CheckAbuse(buf);
#endif

	TraceBoot("MAKENEWCHAR enter slot=%d name='%s' kind=%u sex=%u idnum_in=%d",
		id, buf, (unsigned)pclCharlistInfo->clBI.uiKind,
		(unsigned)pclCharlistInfo->clBI.uiSex,
		(int)pclCharlistInfo->clBI.siIDNum);

	if ( IsCorrectID( buf ) == FALSE )
	{
		TraceBoot("MAKENEWCHAR REJECT IsCorrectID failed for '%s' - nothing sent to DB", buf);
		return;
	}
	TraceBoot("MAKENEWCHAR IsCorrectID ok, proceeding");

	//----------------------------------------------------------
	// �⺻ �ɷ�ġ�� �����Ѵ�. 
	//----------------------------------------------------------
	SI32 kind = pclCharlistInfo->clBI.uiKind;
	pclCharlistInfo->clIP.Init();

	//KHY - 0331 - �ʱ� ������ �ٲ��. - ü�� ������ �ٲ�� ���⵵ �����ؾ� �Ѵ�.
	if(pclClient->IsCountrySwitch(Switch_HealthAddChange))
	{				
		pclCharlistInfo->clIP.SetHnd(5);
		pclCharlistInfo->clIP.SetWis(5);
		pclCharlistInfo->clIP.SetLuk(5);

		SI16 basicLife = 0;
		SI16 basicMana = 0;
		
		basicLife = (pclCharlistInfo->clIP.GetVit() * (8000 + (pclCharlistInfo->clIP.GetLevel()*5)))/1000 ;	
		basicMana = (pclCharlistInfo->clIP.GetMag() * (4000 + (pclCharlistInfo->clIP.GetLevel() *5)))/1000 ;	

		pclCharlistInfo->clIP.SetLife(basicLife);
		pclCharlistInfo->clIP.SetMana(basicMana);					
	}

	pclCharlistInfo->clBI.uiKind	= kind;

	cltPersonInfo	clPersonInfo;
	clPersonInfo.clBI	= pclCharlistInfo->clBI;
	clPersonInfo.clIP	= pclCharlistInfo->clIP;
	
	//----------------------------------------------------------
	// ĳ���� �̸��� ��Ȯ���Ѵ�.
	//----------------------------------------------------------
	clPersonInfo.clBI.clPerson.szName[MAX_PLAYER_NAME - 1] ='\0';

	// ����, �ѱ�, ���ڸ� �����ϴ�. 
	//cyj ������ ĳ���� üũ �κ� �ӽ÷� �ּ�
	//if(IsCorrectID( clPersonInfo.clBI.clPerson.szName ) == FALSE)return ;


	//----------------------------------------------------------
	//�ӽ÷� ���� ������� ĳ���Ϳ��� �������� �ش�. 
	//----------------------------------------------------------
	SI32 itemunique[] = {
		-1
	};
	
	
	SI32 index = 0;
	while(itemunique[index*2] > 0)
	{
		cltItem clItem;
		SI16 brareswitch = 0;
		if(pclItemManager->MakeRandItemUnique( itemunique[index*2], &clItem, 0, 0, &brareswitch ) == true)
		{
			if(clItem.siUnique)
			{
				clItem.siItemNum	= itemunique[index*2 + 1];
				clPersonInfo.clCharItem.AddInv(pclItemManager, &clItem);
			}
		}
		index++;

	}


	// Ŭ���̾�Ʈ���� ��û�� ���ο� ĳ������ ������ �˻��Ͽ� ������ ������ Ȯ���Ѵ�. 

	// ������ ���ٸ� ���ο� ĳ���͸� �����Ѵ�. 

	SI32 idnum	= pclCM->CR[id]->pclCI->clBI.siIDNum;
	SI16 sex	= pclCM->CR[id]->pclCI->clBI.uiSex;
	TraceBoot("MAKENEWCHAR slotState idnum=%d sex=%d gameMode=%d (GAMEMODE_SERVER=%d)",
		(int)idnum, (int)sex, (int)pclClient->GameMode, (int)GAMEMODE_SERVER);
	if(idnum)
	{
		// Basic���� 
		cltPBasicInfo	clbasicinfo;

		// IDNum�� �־��ش�. Ŭ���̾�Ʈ���� ���� IDNum�� �����ؾ� �Ѵ�. (��ŷ�� ����.)
		clPersonInfo.clBI.siIDNum	= idnum;
		clPersonInfo.clBI.uiSex		= sex;		// ���� �Է�. 
		
		clbasicinfo.Set(&clPersonInfo.clBI);

		//if ( siServiceArea == ConstServiceArea_China )
		//{
		//	// ĳ���Ͱ� �����Ǿ� �� ��ġ�� �����Ѵ�. 
		//	clbasicinfo.clPos.SetMapIndex(MAPINDEX_PALACE);	// �溹��. 
		//	clbasicinfo.clPos.SetX(27);
		//	clbasicinfo.clPos.SetY(87);
		//}
		//else
		//{
		//	if ( pclKindInfoSet->pclKI[kind]->siDefaultCountry == ConstServiceArea_China )
		//	{
		//		// ĳ���Ͱ� �����Ǿ� �� ��ġ�� �����Ѵ�. 
		//		clbasicinfo.clPos.SetMapIndex(MAPINDEX_CHINABORDER);	// ���漱 
		//		clbasicinfo.clPos.SetX(17);
		//		clbasicinfo.clPos.SetY(19);
		//	}
		//	else
		//	{
//				// ĳ���Ͱ� �����Ǿ� �� ��ġ�� �����Ѵ�. 
//				clbasicinfo.clPos.SetMapIndex(MAPINDEX_PALACE);	// �溹��. 
//				clbasicinfo.clPos.SetX(27);
//				clbasicinfo.clPos.SetY(87);
		//	}
		//}


		// ĳ���Ͱ� �����Ǿ� �� ��ġ�� �����Ѵ�. 
		clbasicinfo.clPos.SetMapIndex(MAPINDEX_PALACE);	// �溹��. 

		SI32 RandXPos = rand()%7 + 24;
		SI32 RandYPos = rand()%23 + 68;

		clbasicinfo.clPos.SetX(RandXPos);
		clbasicinfo.clPos.SetY(RandYPos);


		// ImportantParameta���� 
		cltPIPInfo clipinfo;
		clipinfo.Set(&clPersonInfo.clIP);

		// �ź� ����. 
		cltSimpleRank	clrankinfo;
		clrankinfo.Set(&clPersonInfo.clCharRank);

		// ��� ����.
		cltPSkillInfo clskillinfo;
		clskillinfo.Set(&clPersonInfo.clCharSkill);

		// ��� ����. 
		cltPItemInfo clitem;
		clitem.Set(&clPersonInfo.clCharItem);


		// ��������. 
		cltBankAccount clBankInfo;

		// �ֽ� ���� 
		cltPStockInfo clStockInfo;

		// ���� ����. 
		cltPMarketInfo	clMarketInfo;

		// �ǰ� ���� 
		cltHealthInfo clHealthInfo;
		
		// ����Ʈ ����.
		cltPQuestInfo clQuestInfo;
		
		// �� ����. 
		cltPersonHorseInfo clHorseInfo;
		
		// ���� ���. 
		cltGuiltyInfo clGuiltyInfo;
	
		//--------------------------
		// DailyQuest ����. 
		//--------------------------
		cltPDailyQuestInfo clDailyQuestInfo;
		//--------------------------
		// DailyQuest2 ����. 
		//--------------------------
		cltPDailyQuest2Info clDailyQuest2Info;
		//--------------------------
		// DailyQuest3 ����. 
		//--------------------------
		cltPDailyQuest3Info clDailyQuest3Info;
		// ��ȯ�� ���� 
		cltSummonInfo	clSummonInfo;

//		cltPGuildStockInfo	clGuildStockInfo;


		// Person������ �����Ѵ�. 
		sDBPersonTotalInfo clinfo;

		clinfo.siGameMasterLevel = 0;
		clinfo.BasicInfo.Set(&clbasicinfo);
		clinfo.IPInfo.Set(&clipinfo);
		clinfo.RankInfo.Set(&clrankinfo);
		clinfo.SkillInfo.Set(&clskillinfo);
		clinfo.ItemInfo.Set(&clitem);
		clinfo.clBankInfo.Set(&clBankInfo);
		clinfo.clStockInfo.Set(&clStockInfo);
		clinfo.clMarketInfo.Set(&clMarketInfo);
		clinfo.clHealthInfo.Set(&clHealthInfo);
		clinfo.clQuestInfo.Set(&clQuestInfo);
		clinfo.clHorseInfo.Set(&clHorseInfo);
		clinfo.clGuiltyInfo.Set(&clGuiltyInfo);
		clinfo.clDailyQuestInfo.Set(&clDailyQuestInfo);
		clinfo.clDailyQuest2Info.Set(&clDailyQuest2Info);
		clinfo.clDailyQuest3Info.Set(&clDailyQuest3Info);
		clinfo.clSummonInfo.Set(&clSummonInfo);	

		// ���ο� Person�� ���鵵�� �䱸�Ѵ�. 
		sDBRequest_PersonTotalInfoCreate clMsg(id, &clinfo);
		TraceBoot("MAKENEWCHAR SEND -> GameDBManager DBSELECT_LOGIN name='%s' kind=%u idnum=%d size=%d",
			buf, (unsigned)kind, (int)idnum, (int)sizeof(clMsg));
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);

	}
	else
	{
		TraceBoot("MAKENEWCHAR ABORT idnum==0 - character NOT created, no DB request sent");
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("fd927434"), TEXT("f2-994"), MB_OK|MB_TOPMOST);
		return ;
	}
}

void cltServer::DoMsg_GAMEMSG_REQUEST_DELUSERCHAR(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgRequest_DelUserChar* pclDelUserChar = (cltGameMsgRequest_DelUserChar*)pclMsg->cData;

	SI32 slot		= pclDelUserChar->siCharSlot;
	TCHAR idennum[12];
	StringCchCopy( idennum, 12, pclDelUserChar->szIdentification );

	if(slot < 0 || slot >= MAX_TOTAL_CHAR_PER_USER)return ;

	//----------------------------------------------------
	// ĳ���� ������ �����Ѵ�. 
	//----------------------------------------------------
	SI32 idnum	= pclCM->CR[id]->pclCI->clBI.siIDNum;
	if(idnum <=0)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid = 0;
	if(pclchar->pclCharListInfo != NULL)	personid = pclchar->pclCharListInfo[slot].clBI.clPerson.siPersonID;
	//SI32 personid = pclchar->clCharListInfo[slot].clBI.clPerson.siPersonID;
	
	if(personid)
	{
		sDBRequest_PersonTotalInfoDelete clMsg(id, idnum, slot,	personid, idennum, ((ITZPerson *)pclCM->CR[id])->GetSession()->GetIP());
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);
	}

}
void cltServer::DoMsg_GAMEMSG_REQUEST_PREDELUSERCHAR(cltMsg* pclMsg, SI32 id)//�ɸ��� ���������Ⱓ
{
	cltGameMsgRequest_PreDelUserChar* pclPreDelUserChar = (cltGameMsgRequest_PreDelUserChar*)pclMsg->cData;

	SI32 slot		= pclPreDelUserChar->siCharSlot;
	TCHAR idennum[12];
	StringCchCopy( idennum, 12, pclPreDelUserChar->szIdentification );

	if(slot < 0 || slot >= MAX_CHAR_PER_USER)return ;
	cltServer* pclserver = (cltServer*)pclClient;

	SYSTEMTIME stTime;//�� �ý��۽ð�
	//SYSTEMTIME stDelTime;//��������� �ɸ��͸� ����ð�
	//SI32 AddDate = 3;// ��� �ð��� �Ǳ����� ���ý��� �ð��� ������� �ϴ� �ð� or ��¥.
	GetLocalTime(&stTime);//���� ���� �ð� ����.
	//TimeAddDate(&stTime,AddDate,&stDelTime);

	SI32 idnum	= pclCM->CR[id]->pclCI->clBI.siIDNum;
	if(idnum <=0)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 personid = 0;
	if( pclchar->pclCharListInfo )	personid = pclchar->pclCharListInfo[slot].clBI.clPerson.siPersonID;
	SI16 deltype = pclPreDelUserChar->predeltype;

	if (personid) {
		sDBRequest_PersonTotalInfoPreDelete clMsg(id, idnum, slot,	personid, idennum, stTime, ((ITZPerson *)pclCM->CR[id])->GetSession()->GetIP(),deltype);
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);
	}

}

// Ŭ���̾�Ʈ�� �غ�Ǿ���. 
void cltServer::DoMsg_GAMEMSG_REQUEST_READY(cltMsg* pclMsg, SI32 id)
{
	cltCharServer* pChar = (cltCharServer*)pclCM->CR[id];

	// ĳ���Ͱ� �غ�Ǿ����� Ȯ���Ѵ�. 
	pChar->bReadySwitch	= true;
}

// [����] CRC üũ
void cltServer::DoMsg_GAMEMSG_REQUEST_FILECRCCHECK(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgRequest_FileCRCCheck* pclCRC = (cltGameMsgRequest_FileCRCCheck*)pclMsg->cData;
	if ( pclCRC == NULL )							return;

	// �������� üũ�� ����� ������ üũ���� �ʾƵ� �ȴ�.
	if ( pclCRCFileCheckManager->m_siCount <= 0 )	return;

	bool bCheckResult = false;
	

	if ( pclCRC->siCount == pclCRCFileCheckManager->m_siCount )
	{
		for ( SI32 siIndex=0; siIndex<pclCRC->siCount; siIndex++ )
		{
			NCRCFileCheck* pCRCFile = &pclCRC->CheckFileList[siIndex];
			if ( pCRCFile == NULL ) continue;

			bCheckResult = pclCRCFileCheckManager->CheckFile( pCRCFile );

			if ( bCheckResult == false )
			{
				break;
			}
		}
	}

	if ( bCheckResult == false )
	{
		if ( pclCM->IsValidID( id ) == false )		return;

		SendServerResponseMsg( 0, SRVAL_FAIL_FILECRCCHECK, 0, 0, id );
			
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
		if ( pclchar == NULL )						return;

		pclchar->bCloseSocketSwitch = true;
		pclchar->dwCloseSocketTimer = 5;
	}
	
}

void cltServer::DoMsg_GAMEMSG_REQUEST_CHECKPROCESS(cltMsg* pclMsg,SI32 id)
{
	cltGameMsgRequest_CheckProcess* pclInfo = (cltGameMsgRequest_CheckProcess*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharServer* pclchar = pclCM->GetCharServer( id );
	if ( NULL == pclchar )
	{
		return;
	}

	pclCheckProcessManager->AddPersonInfo( id, pclInfo->m_siProcessCount, pclInfo->m_szMacAddress );

}

void cltServer::DoMsg_GAMEMSG_REQUEST_PERSONSLOTNUMCHANGE(cltMsg* pclMsg, SI32 id)
{
	if ( false == IsCountrySwitch(Switch_CharHouse) )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer(id);
	if ( NULL == pclChar )
	{
		return;
	}

	SI32 siFromIndex = pclMsg->siData32[0];
	if ( (MAX_CHAR_PER_USER > siFromIndex) || (MAX_TOTAL_CHAR_PER_USER <= siFromIndex) )
	{
		return;
	}

	SI32 siToIndex	= -1; // �ε����� 0���� �����̹Ƿ�
	for ( SI32 siIndex=0; siIndex<MAX_CHAR_PER_USER; siIndex++ )
	{
		if ( 0 >= pclChar->pclCharListInfo[siIndex].clBI.clPerson.GetPersonID() )
		{
			siToIndex = siIndex;
			break;
		}
	}

	if ( (0 > siToIndex) || (MAX_CHAR_PER_USER <= siToIndex) )
	{
		return;
	}

	SI32 siGetPersonID = pclChar->pclCharListInfo[siFromIndex].clBI.clPerson.GetPersonID();
	if ( 0 >= siGetPersonID )
	{
		return;
	}

	sDBRequest_PersonSlotNumChange clMsg( id, siFromIndex, siToIndex, siGetPersonID );
	SendDBMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg );
	
}
