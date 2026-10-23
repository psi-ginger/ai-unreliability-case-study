#include "CommonHeader.h"
#include "../Common/Char/Health/Health.h"
#include "Guild/Guild.h"


//====================================
// cltMoney
//====================================
void cltMoney::Init()
{
	itMoney = 0;
}

void cltMoney::Set(cltMoney* pclmoney) 
{
	if(pclmoney == NULL)return;

	Set(pclmoney->itMoney);

}  
void cltMoney::Set(GMONEY money)
{
	itMoney = money;
}

cltMoney cltMoney::Minus(cltMoney* pclmoney)
{
	cltMoney clmoney;
	clmoney.Set(this);
	clmoney.Decrease(pclmoney);

	return clmoney;
}

cltMoney cltMoney::Plus(cltMoney* pclmoney)
{
	cltMoney clmoney;
	clmoney.Set(this);
	clmoney.Increase(pclmoney);

	return clmoney;
}

void cltMoney::Increase(cltMoney* pclmoney)
{
	if(pclmoney == NULL)return ;

	itMoney += pclmoney->itMoney;
	if(itMoney > 20000000000)itMoney = 20000000000;
}

void cltMoney::Decrease(cltMoney* pclmoney)
{
	if(pclmoney == NULL)return ;

	if(itMoney >=  pclmoney->itMoney)
	{
		itMoney -= pclmoney->itMoney;
	}
	else
	{
		itMoney = 0;
	}
}

// itMoney에서 pclmoney가 차지하는 비율. 
SI32 cltMoney::GetPercent(cltMoney* pclmoney)
{
	if(itMoney)
	{
		SI64 percent = (pclmoney->itMoney * 100) / itMoney;
		return (SI32)percent;
	}

	return 0;
}

void cltMoney::Clear()
{
	itMoney = 0;
}

bool cltMoney::Empty()
{
	if(itMoney == 0)return true;

	return false;
}

bool cltMoney::Large(cltMoney* pclmoney)
{
	if(itMoney > pclmoney->itMoney)return true;
	return false;
}

bool cltMoney::Small(cltMoney* pclmoney)
{
	if(itMoney < pclmoney->itMoney)return true;
	return false;
}

//====================================
// cltPos
//====================================
cltPos::cltPos()
{
	siX = 0; siY = 0;
}
cltPos::cltPos(SI32 x, SI32 y)
{
	siX = x; siY = y;
}

void cltPos::Init()
{
	siX = 0;
	siY = 0;
}

void cltPos::Set(const cltPos* pclpos)
{
	siX = pclpos->siX;
	siY	= pclpos->siY;
}

void cltPos::Set(SI32 six, SI32 siy)
{
	siX	= six;
	siY	= siy;
}

//====================================
// cltWarpInfo
//====================================
cltWarpInfo::cltWarpInfo()
{
	Init();
}

cltWarpInfo::cltWarpInfo(UI32 warpswitch, UI32 mapindex, UI32 forceswitch, UI32 ticketuse, UI32 x, UI32 y, SI16 gatex, SI16 gatey)
{
	uiWarpSwitch		= warpswitch;
	uiMapIndex			= mapindex;
	uiForceSwitch		= forceswitch;
	uiTicketUseSwitch	=ticketuse;
	uiX					= x;
	uiY					= y;
	siGateX				= gatex;
	siGateY				= gatey;
}

void cltWarpInfo::Set(const cltWarpInfo* pclinfo)
{
	uiWarpSwitch		= pclinfo->uiWarpSwitch;
	uiMapIndex			= pclinfo->uiMapIndex;
	uiForceSwitch		= pclinfo->uiForceSwitch;
	uiTicketUseSwitch	= pclinfo->uiTicketUseSwitch;
	uiX					= pclinfo->uiX;
	uiY					= pclinfo->uiY;
	siGateX				= pclinfo->siGateX;
	siGateY				= pclinfo->siGateY;
}

void cltWarpInfo::Init()
{
	uiWarpSwitch		= 0;
	uiMapIndex			= 0;
	uiX					= 0;
	uiY					= 0;
	uiForceSwitch		= 0;
	uiTicketUseSwitch	= 0;
	siGateX				= -1;
	siGateY				= -1;
}

UI32 cltWarpInfo::GetWarpSwitch() const
{
	return uiWarpSwitch;
}

void cltWarpInfo::SetWarpSwitch(UI32 mode)
{
	uiWarpSwitch = mode;
}

UI32 cltWarpInfo::GetMapIndex() const
{
	return uiMapIndex;
}

UI32 cltWarpInfo::GetX() const
{
	return uiX;
}
UI32 cltWarpInfo::GetY() const
{
	return uiY;
}

SI16 cltWarpInfo::GetGateX() const
{
	return siGateX;
}

SI16 cltWarpInfo::GetGateY() const
{
	return siGateY;
}

//====================================
// cltSimplePerson
//====================================
cltSimplePerson::cltSimplePerson()
{
	Init();
}

cltSimplePerson::cltSimplePerson(SI32 personid, const TCHAR* pname)
{
	Set(personid, pname);
}

void cltSimplePerson::Set(const cltSimplePerson* pinfo)
{
	SI32 siGetPersonID = pinfo->GetPersonID();

	siPersonID	= siGetPersonID;

	MStrCpy(szName, pinfo->GetName(), MAX_PLAYER_NAME );
}

void cltSimplePerson::Set(SI32 personid, const TCHAR* pname)
{
	siPersonID = personid;
	MStrCpy(szName, pname, MAX_PLAYER_NAME );
}

void cltSimplePerson::Init()
{
	siPersonID = 0;
	MStrCpy(szName, TEXT("NOPERSONNAME"), MAX_PLAYER_NAME );
}

SI32 cltSimplePerson::GetPersonID()const
{
	return siPersonID;
}

void cltSimplePerson::SetPersonID(SI32 id)
{ 
	siPersonID = id;
}

const TCHAR* cltSimplePerson::GetName() const
{
	return szName;
}

void cltSimplePerson::SetName(const TCHAR* pname)
{
	MStrCpy(szName, pname, MAX_PLAYER_NAME );
}

//====================================
// cltCharPos
//====================================
cltCharPos::cltCharPos()
{
	Init();
}

cltCharPos::cltCharPos(SI32 mapindex, SI32 x, SI32 y)
{
	siMapIndex	= mapindex;
	siX			= x;
	siY			= y;
}

void cltCharPos::Set(const cltCharPos* pclpos)
{
	memcpy(this, pclpos, sizeof(cltCharPos));				//ok
}

void cltCharPos::Init()
{
	siMapIndex	= 0;
	siX			= 0;
	siY			= 0;
}

void cltCharPos::SetMapIndex(SI32 index)
{
	siMapIndex = index;
}

SI32 cltCharPos::GetMapIndex() const
{
	return siMapIndex;
}

void cltCharPos::SetX(SI32 x)
{
	siX = x;
}

SI32 cltCharPos::GetX() const
{
	return siX;
}

void cltCharPos::SetY(SI32 y)
{
	siY = y;
}

SI32 cltCharPos::GetY() const
{
	return siY;
}

//====================================
// cltSimpleRank
//====================================
cltSimpleRank::cltSimpleRank()
{
	Init();
}

cltSimpleRank::cltSimpleRank(SI32 ranktype, SI32 villageunique)
{
	siType			= ranktype;
	siVillageUnique = villageunique;
}
cltSimpleRank::cltSimpleRank(cltSimpleRank* pclrank)
{
	memcpy(this, pclrank, sizeof(cltSimpleRank));				//ok
}

void cltSimpleRank::Init()
{
	siType			= 0;
	siVillageUnique = 0;
}

void cltSimpleRank::Set(SI08 ranktype, SI08 villageunique)
{
	siType			= ranktype;
	siVillageUnique = villageunique;
}

void cltSimpleRank::Set(const cltSimpleRank* pinfo)
{
	memcpy(this, pinfo, sizeof(cltSimpleRank));				//ok
}

void cltSimpleRank::SetRankType(SI08 type)
{
	siType = type;
}

SI32 cltSimpleRank::GetRankType() const
{
	return siType;
}

void cltSimpleRank::SetVillageUnique(SI08 villageunique)
{
	siVillageUnique = villageunique;
}

SI32 cltSimpleRank::GetVillageUnique() const
{
	return siVillageUnique;
}

BOOL cltSimpleRank::IsSame(cltSimpleRank* pclrank) const
{
	if(pclrank == NULL)return FALSE;

	if(siType == pclrank->GetRankType() && siVillageUnique == pclrank->GetVillageUnique())
		return TRUE;

	return FALSE;
}

//====================================
// cltSimpleGuildRank
//====================================
cltSimpleGuildRank::cltSimpleGuildRank()
{
	Init();
}

cltSimpleGuildRank::cltSimpleGuildRank( SI08 ranktype, SI32 villageunique, SI32 guildunitindex,TCHAR*name,UI08 confirm,SI32 mark, SI32 enmeyguildunique, SI32 EmpireUnique)
{
	siType			 = ranktype;
	siVillageUnique  = villageunique;
	siGuildUnitIndex = guildunitindex;
	MStrCpy(szGuildName,name,13);
	uiConfirm = confirm;
	siMarkIndex = mark ;
	siEnemyGuildUnique = enmeyguildunique;
	siEmpireUnique = EmpireUnique;
}

cltSimpleGuildRank::cltSimpleGuildRank( cltSimpleGuildRank* pclrank )
{
	siType		  	 = pclrank->siType;
	siVillageUnique  = pclrank->siVillageUnique;
	siGuildUnitIndex = pclrank->siGuildUnitIndex;
	MStrCpy(szGuildName,pclrank->szGuildName,13);
	uiConfirm = pclrank->uiConfirm ;
	siMarkIndex = pclrank->siMarkIndex ;
	siEnemyGuildUnique = pclrank->siEnemyGuildUnique;
	siEmpireUnique = pclrank->siEmpireUnique;
}

void cltSimpleGuildRank::Init()
{
	siType			= 0;
	siVillageUnique = 0;
	siGuildUnitIndex = 0;
	ZeroMemory(szGuildName,sizeof(TCHAR)*13);
	uiConfirm = GUILDRANKTYPE_STANDING ;
	siMarkIndex = 0 ;
	siEnemyGuildUnique = 0;
	siEmpireUnique = 0;
}

void cltSimpleGuildRank::Set( SI08 ranktype, SI32 villageunique, SI32 guildunitindex,TCHAR*name,UI08 confirm,SI32 mark, SI32 enemyguildunique, SI32 EmpireUnique )
{
	siType			 = ranktype;
	siVillageUnique  = villageunique;
	siGuildUnitIndex = guildunitindex; 
	MStrCpy(szGuildName,name,13);
	uiConfirm = confirm;
	siMarkIndex = mark;
	siEnemyGuildUnique = 0;
	EmpireUnique = 0;
}

void cltSimpleGuildRank::Set(const cltSimpleGuildRank* pclrank)
{
	siType		  	 = pclrank->siType;
	siVillageUnique  = pclrank->siVillageUnique;
	siGuildUnitIndex = pclrank->siGuildUnitIndex;
	MStrCpy(szGuildName,pclrank->szGuildName,13);
	uiConfirm = pclrank->uiConfirm;
	siMarkIndex = pclrank->siMarkIndex ;
	siEnemyGuildUnique = pclrank->siEnemyGuildUnique;
	siEmpireUnique = pclrank->siEmpireUnique;
}

void cltSimpleGuildRank::SetRankType(SI08 type)
{
	siType = type;
}

SI32 cltSimpleGuildRank::GetRankType() const
{
	return siType;
}

void cltSimpleGuildRank::SetVillageUnique(SI32 villageunique)
{
	siVillageUnique = villageunique;
}

SI32 cltSimpleGuildRank::GetVillageUnique() const
{
	return siVillageUnique;
}

void cltSimpleGuildRank::SetGuildUnitIndex(SI32 guildunitindex)
{
	siGuildUnitIndex = guildunitindex;
}

SI32 cltSimpleGuildRank::GetGuildUnitIndex() const
{
	return siGuildUnitIndex;
}

void cltSimpleGuildRank::SetEnemyGuild( SI32 enemyguildunique )
{
	siEnemyGuildUnique = enemyguildunique;
}

SI32 cltSimpleGuildRank::GetEnemyGuild() const
{
	return siEnemyGuildUnique;
}

void cltSimpleGuildRank::SetEmpireUnique( SI32 EmpireUnique )
{
	siEmpireUnique = EmpireUnique;
}

SI32 cltSimpleGuildRank::GetEmpireUnique() const
{
	return siEmpireUnique;
}

BOOL cltSimpleGuildRank::IsSame(cltSimpleGuildRank* pclrank) const
{
	if(pclrank == NULL)return FALSE;

	if(siType == pclrank->siType && 
		siVillageUnique == pclrank->siVillageUnique &&
		siGuildUnitIndex == pclrank->siGuildUnitIndex &&
		!strcmp(szGuildName,pclrank->szGuildName) &&
		uiConfirm == pclrank->uiConfirm &&
		siMarkIndex == pclrank->siMarkIndex &&
		siEmpireUnique == pclrank->siEmpireUnique )
		return TRUE;

	return FALSE;
}

bool cltSimpleGuildRank::IsGuildMember()
{
	if ( siType == GUILDRANKTYPE_STANDING )
		return false ;

	if ( siGuildUnitIndex <= 0 )
		return false ;

	if ( siVillageUnique <= 0 )
		return false ;

	if ( uiConfirm == GUILDSTATUS_STANDING )
		return false ;

	return true ;
}

//cltPersonNaming
cltPersonNaming::cltPersonNaming()
{
	Init();
}

cltPersonNaming::~cltPersonNaming()
{

}

void cltPersonNaming::Init()
{
	for ( SI32 i = 0; i < MAX_NAMINGQUEST_INDEX ; ++i )
	{
		m_siLevel[i] = 0;
	}
	m_siSelectedIndex = 0;
	m_siSelectedLevel = 0;
}

void cltPersonNaming::Set( cltPersonNaming* pclPersonNaming )
{
	for ( SI32 i = 0; i < MAX_NAMINGQUEST_INDEX ; ++i )
	{
		m_siLevel[i] = pclPersonNaming->m_siLevel[i];
	}
	m_siSelectedIndex = pclPersonNaming->m_siSelectedIndex;
	m_siSelectedLevel = pclPersonNaming->m_siSelectedLevel;
}

//====================================
// cltAccountNum
//====================================
cltAccountNum::cltAccountNum()
{
	Init();
}

cltAccountNum::cltAccountNum(SI32 villageunique, SI32 accountnum)
{
	siVillageUnique = villageunique;
	siAccountNum	= accountnum;

}


void cltAccountNum::Init()
{
	siVillageUnique		= 0;
	siAccountNum		= 0;	
}

void cltAccountNum::Set(const cltAccountNum* pclaccountnum)
{
	memcpy(this, pclaccountnum, sizeof(cltAccountNum));				//ok
}

// 이 통장 계좌가 유효한가?
BOOL cltAccountNum::IsValid()
{
	if(siAccountNum)return TRUE;
	return FALSE;
}

bool cltAccountNum::IsSame(cltAccountNum* pclnum)
{
	if(siVillageUnique != pclnum->siVillageUnique)return false;

	if(siAccountNum != pclnum->siAccountNum)return false;

	return true;
}

void cltAccountNum::SetVillageUnique(SI32 villageunique)
{
	siVillageUnique = villageunique; 
}

SI32 cltAccountNum::GetVillageUnique() const
{
	return siVillageUnique;
}

void cltAccountNum::SetAccountNum(SI32 num)
{
	siAccountNum = num;
}

SI32 cltAccountNum::GetAccountNum() const
{
	return siAccountNum;
}

//====================================
// cltRankPerson
//====================================
cltRankPerson::cltRankPerson()
{
	siVillageUnique = 0;
};

cltRankPerson::cltRankPerson(SI32 sivillageunique, const cltSimplePerson* pinfo)
{
	siVillageUnique = sivillageunique;
	clPerson.Set(pinfo);
}

void cltRankPerson::Set(const cltRankPerson* pclinfo)
{
	memcpy(this, pclinfo, sizeof(cltRankPerson));				//ok
}
void cltRankPerson::Set(SI32 sivillageunique, const cltSimplePerson* pinfo)
{
	siVillageUnique = sivillageunique;
	clPerson.Set(pinfo);
}

void cltRankPerson::Init(SI32 villageunique)
{
	siVillageUnique = villageunique;
	clPerson.Init();

}

SI32 cltRankPerson::GetPersonID() const 
{
	return clPerson.GetPersonID();
}

SI32 cltRankPerson::GetVillageUnique() const
{
	return siVillageUnique;
}

//====================================
// cltServerResponseValue
//====================================
cltServerResponseValue::cltServerResponseValue()
{
	siReceiver		= 0;
	siReturnValue	= 0;
	siPara1			= 0;
	siPara2			= 0;
}

cltServerResponseValue::cltServerResponseValue(SI16 receiver, SI16 value, SI16 sipara1, SI16 sipara2)
{
	siReceiver		= receiver;
	siReturnValue	= value;
	siPara1			= sipara1;
	siPara2			= sipara2;
}

//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
//====================================
// cltServerResponseCharValue
//====================================
cltServerResponseCharValue::cltServerResponseCharValue()
{
	siReceiver		= 0;
	siReturnValue	= 0;

	ZeroMemory(siPara1, sizeof(siPara1));
	ZeroMemory(siPara2, sizeof(siPara2));
}

cltServerResponseCharValue::cltServerResponseCharValue(SI16 receiver, SI16 value, TCHAR* sipara1, TCHAR* sipara2)
{
	siReceiver		= receiver;
	siReturnValue	= value;

	MStrCpy(siPara1, sipara1, sizeof(siPara1));
	MStrCpy(siPara2, sipara2, sizeof(siPara2));
}
//성웅 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.2->3개로 늘림
//====================================
// cltServerResponseCharValue
//====================================
cltServerResponseCharValue2::cltServerResponseCharValue2()
{
	siReceiver		= 0;
	siReturnValue	= 0;

	ZeroMemory(siPara1, sizeof(siPara1));
	ZeroMemory(siPara2, sizeof(siPara2));
	ZeroMemory(siPara3, sizeof(siPara3));
}

cltServerResponseCharValue2::cltServerResponseCharValue2(SI16 receiver, SI16 value, TCHAR* sipara1, TCHAR* sipara2, TCHAR* sipara3)
{
	siReceiver		= receiver;
	siReturnValue	= value;

	MStrCpy(siPara1, sipara1, sizeof(siPara1));
	MStrCpy(siPara2, sipara2, sizeof(siPara2));
	MStrCpy(siPara3, sipara3, sizeof(siPara3));
}


//====================================
// cltFee
//====================================
void cltFee::Set(cltFee* pclfee,SI32 value)
{
	memcpy(this, pclfee, sizeof(cltFee));				//ok

//	if ( value == 1000 )
//	{
//		if( siFeeRate < 10 )
//			siFeeRate = 10;
//	}
//	else if ( value == 100 )
//	{
		if( siFeeRate < 1 )
			siFeeRate = 1;
//	}
}

// 거래 금액을 근거로 수수료를 계산한다. 
GMONEY cltFee::CalcFee(GMONEY money,SI32 value)
{
	//return max(siMinFee, min(siMaxFee, money * siFeeRate / 1000)) ;

	if ( value == 1000 )
	{
		if( siFeeRate < 10 ) siFeeRate = 10;

		GMONEY ret = (GMONEY)( money * ( siFeeRate / 1000. ) );

		if( ret < 0 ) return 0;

		return ret;
	}
	else if ( value == 100 )
	{
		if( siFeeRate < 1 ) siFeeRate = 1;

		GMONEY ret = (GMONEY)( money * ( siFeeRate / 100. ) );

		if( ret < 0 ) return 0;

		return ret;
	}

	return 0 ;
}

BOOL cltFee::SetFeeRate(SI32 rate,SI32 value)
{
	if( rate <  10 ) { siFeeRate =  10; return FALSE; }
	if( rate > 100 ) { siFeeRate = 100; return FALSE; }
	siFeeRate = rate;
	return TRUE;
}

BOOL cltFee::SetFeeRateNew(SI32 rate, SI32 value)
{
	if( rate <  50 ) { siFeeRate =  50; return FALSE; }
	if( rate > 150 ) { siFeeRate = 150; return FALSE; }
	
	siFeeRate = rate;
	
	return TRUE;
}
//SI32 cltFee::GetFeeRate()const{return max(10, siFeeRate);}
SI32 cltFee::GetFeeRate()const{return siFeeRate;}

BOOL cltFee::SetMinFee(GMONEY fee)
{
	if(fee < 0 || fee > MAX_FEE)return FALSE;

	siMinFee = fee;
	return TRUE;
}
BOOL cltFee::SetMinFeeNew(GMONEY fee)
{
	if(fee < 500 || fee > 150000) return FALSE;

	siMinFee = fee;
	return TRUE;
}
GMONEY cltFee::GetMinFee() const
{
	return siMinFee;
}

BOOL cltFee::SetMaxFee(GMONEY fee)
{
	if(fee < 0 || fee > MAX_FEE || fee < siMinFee)return FALSE;

	siMaxFee = fee;
	return TRUE;
}
BOOL cltFee::SetMaxFeeNew(GMONEY fee)
{
	if(fee < 500 || fee > 150000 || fee < siMinFee) return FALSE;
	
	siMaxFee = fee;
	return TRUE;
}
GMONEY cltFee::GetMaxFee() const
{
	return siMaxFee;
}

// 현재 수수료가 적절한지 확인한다. 
BOOL cltFee::IsValid(SI32 value)
{
	if(siFeeRate < 1 || siFeeRate > MAX_FEE_RATE)return FALSE;

	return TRUE;
}

BOOL cltFee::IsValid()
{
	if(siFeeRate < 1 || siFeeRate > MAX_FEE_RATE)	return FALSE;
	
	return TRUE;
}

BOOL cltFee::IsValidNew()
{
	if( siFeeRate < 50 || siFeeRate > 150 )	return FALSE;

	return TRUE;
}

//====================================
// cltLogInInfo
//====================================
cltLogInInfo::cltLogInInfo(SI32	authkey,  TCHAR* id,  TCHAR* password, UI08 gamemode)
{
	siAuthKey	= authkey;
	MStrCpy( szID,	id,	MAX_PLAYER_NAME );
	MStrCpy( szPassword, password,	MAX_PLAYER_NAME );
	uiGameMode = gamemode;
}

cltLogIn_HanAuthInfo::cltLogIn_HanAuthInfo(SI32 authkey, TCHAR* authstring, TCHAR* id, UI08 gamemode)
{
	siAuthKey	= authkey;
	MStrCpy(szAuthString, authstring,	2048 );
	MStrCpy(szID,	id,	MAX_PLAYER_NAME );
	uiGameMode = gamemode;
}

cltLogIn_TaiwanInfo::cltLogIn_TaiwanInfo(SI32 authkey, TCHAR* gameaccount, TCHAR* onetimepassword, UI08 gamemode)
{
	siAuthKey	= authkey;
	MStrCpy(szGameAccount,		gameaccount,		sizeof(szGameAccount) );
	MStrCpy(szOneTimePassword,	onetimepassword,	sizeof(szOneTimePassword) );
	uiGameMode = gamemode;
}

//====================================
// cltOrderDataInfo
//====================================
cltOrderDataInfo::cltOrderDataInfo()
{
	Init();
}

cltOrderDataInfo::cltOrderDataInfo(SI08 order, SI08 hostmode, SI32 para1, SI16 para2, SI16 para3, SI16 para4)
{
	Make(order, hostmode, para1, para2, para3, para4);
}

cltOrderDataInfo::cltOrderDataInfo(cltOrderDataInfo* pclinfo)
{
	Set(pclinfo);
}

void cltOrderDataInfo::Init()
{
	siOrder		= ORDER_WAIT;
	siHostMode	= 0;
	siPara1		= 0;
	siPara2		= 0;
	siPara3		= 0;
	siPara4		= 0;

	dwPushedTime = 0;
}

void cltOrderDataInfo::Make(SI08 order, SI08 hostmode, SI32 para1, SI16 para2, SI16 para3, SI16 para4)
{
	siOrder		= order;
	siHostMode	= hostmode;

	siPara1		= para1;
	siPara2		= para2;
	siPara3		= para3;
	siPara4		= para4;

	dwPushedTime = 0;
}

void cltOrderDataInfo::Set(cltOrderDataInfo* pclorder)
{
	siOrder		=	pclorder->siOrder;
	siHostMode	=	pclorder->siHostMode;
	siPara1		=	pclorder->siPara1;
	siPara2		=	pclorder->siPara2;
	siPara3		=	pclorder->siPara3;
	siPara4		=	pclorder->siPara4;

	dwPushedTime = pclorder->dwPushedTime;
}

//====================================
// cltOrderData
//====================================
cltOrderData::cltOrderData()
{
	ZeroMemory(this, sizeof(cltOrderData));
}

void cltOrderData::Make(SI32 order, SI32 charunique, SI32 para1, SI16 para2, SI16 para3, SI16 para4, SI08 hostmode)
{
	ODI.Make(order, hostmode, para1, para2, para3, para4);

	CharUnique		 = charunique;
}
void cltOrderData::Make(SI32 charunique, cltOrderDataInfo* pclinfo)
{
	CharUnique = charunique;
	ODI.Set(pclinfo);
}

//====================================
// cltShapeEquipInfoUnit
//====================================
cltShapeEquipInfoUnit::cltShapeEquipInfoUnit()
{
	Init();
}
cltShapeEquipInfoUnit::cltShapeEquipInfoUnit(UI32 unique, UI32 rare , UI32 enchanttype , UI32 enchantpower )
{
	uiUnique	= unique;
	uiRare		= rare;
	uiEnchantType = enchanttype;
	uiEnchantPower = enchantpower;
	uiReserved	= 0;

}
void cltShapeEquipInfoUnit::Init()
{
	uiUnique	= 0;
	uiRare		= 0;
	uiEnchantType = 0;
	uiEnchantPower = 0;
	uiReserved	= 0;
}
void cltShapeEquipInfoUnit::Set(cltShapeEquipInfoUnit* pclinfo)
{
	uiUnique	= pclinfo->uiUnique;
	uiRare		= pclinfo->uiRare;
	uiEnchantType = pclinfo->uiEnchantType;
	uiEnchantPower = pclinfo->uiEnchantPower;
	uiReserved	= pclinfo->uiReserved;
}

//====================================
// cltShapeEquipInfo
//====================================
cltShapeEquipInfo::cltShapeEquipInfo()
{
	Init();
}

cltShapeEquipInfo::cltShapeEquipInfo(cltShapeEquipInfoUnit* pclArmour, cltShapeEquipInfoUnit* pclHelmet, cltShapeEquipInfoUnit* pclMantle, cltShapeEquipInfoUnit* pclWeapon,
									 bool bsword, bool bspear, bool baxe, bool bgun, bool bbow, bool bstaff, bool bcannon,
									 SI08 SwordScrollNum, SI08 SpearScrollNum, SI08 AxeScrollNum,
									 SI08 GunScrollNum, SI08 BowScrollNum, SI08 StaffScrollNum,
									 SI16 CompletedSetItemIndex,
									 SI16 CompletedSetItemIndexStep,
									 SI16 CompletedSetItemIndex2,
									 SI16 CompletedSetItemIndexStep2,
									 SI16 CompletedSetItemIndex3,
									 SI16 CompletedSetItemIndexStep3,
									 SI16 CompletedSetItemIndex4,
									 SI16 CompletedSetItemIndexStep4,
									 SI16 CompletedSetItemIndex5,
									 SI16 CompletedSetItemIndexStep5)
{
	clArmour.Set(pclArmour);
	clHelmet.Set(pclHelmet);
	clMantle.Set(pclMantle);
	clWeapon.Set(pclWeapon);

	uiSwordScroll	= bsword;
	uiSpearScroll	= bspear;
	uiAxeScroll		= baxe;	
	uiGunScroll		= bgun;		
	uiBowScroll		= bbow;		
	uiStaffScroll	= bstaff;
	uiCannonScroll	= bcannon;

	siSwordScrollNum = SwordScrollNum;
	siSpearScrollNum = SpearScrollNum;
	siAxeScrollNum = AxeScrollNum;
	siGunScrollNum = GunScrollNum;
	siBowScrollNum = BowScrollNum;
	siStaffScrollNum = StaffScrollNum;

	siCompletedSetItemIndex		= CompletedSetItemIndex;
	siCompletedSetItemIndexStep = CompletedSetItemIndexStep;

	siCompletedSetItemIndex2	 = CompletedSetItemIndex2;
	siCompletedSetItemIndexStep2 = CompletedSetItemIndexStep2;

	siCompletedSetItemIndex3	 = CompletedSetItemIndex3;
	siCompletedSetItemIndexStep3 = CompletedSetItemIndexStep3;

	siCompletedSetItemIndex4	 = CompletedSetItemIndex4;
	siCompletedSetItemIndexStep4 = CompletedSetItemIndexStep4;

	siCompletedSetItemIndex5	 = CompletedSetItemIndex5;
	siCompletedSetItemIndexStep5 = CompletedSetItemIndexStep5;
}

void cltShapeEquipInfo::Init()
{
	clArmour.Init();
	clHelmet.Init();
	clMantle.Init();
	clWeapon.Init();

	uiSwordScroll	= 0;
	uiSpearScroll	= 0;
	uiAxeScroll		= 0;	
	uiGunScroll		= 0;		
	uiBowScroll		= 0;		
	uiStaffScroll	= 0;
	uiCannonScroll	= 0;

	siSwordScrollNum = 0;
	siSpearScrollNum = 0;
	siAxeScrollNum = 0;
	siGunScrollNum = 0;
	siBowScrollNum = 0;
	siStaffScrollNum = 0;

	siCompletedSetItemIndex		= 0;
	siCompletedSetItemIndexStep = 0;

	siCompletedSetItemIndex2	 = 0;
	siCompletedSetItemIndexStep2 = 0;

	siCompletedSetItemIndex3	 = 0;
	siCompletedSetItemIndexStep3 = 0;
}


void cltShapeEquipInfo::Set(cltShapeEquipInfo* pclinfo)
{
	clArmour.Set(&pclinfo->clArmour);
	clHelmet.Set(&pclinfo->clHelmet);
	clMantle.Set(&pclinfo->clMantle);
	clWeapon.Set(&pclinfo->clWeapon);

	uiSwordScroll	= pclinfo->uiSwordScroll;
	uiSpearScroll	= pclinfo->uiSpearScroll;
	uiAxeScroll		= pclinfo->uiAxeScroll;	
	uiGunScroll		= pclinfo->uiGunScroll;		
	uiBowScroll		= pclinfo->uiBowScroll;		
	uiStaffScroll	= pclinfo->uiStaffScroll;
	uiCannonScroll	= pclinfo->uiCannonScroll;

	siSwordScrollNum  = pclinfo->siSwordScrollNum;
	siSpearScrollNum  = pclinfo->siSpearScrollNum;
	siAxeScrollNum	  = pclinfo->siAxeScrollNum;
	siGunScrollNum    = pclinfo->siGunScrollNum;
	siBowScrollNum    = pclinfo->siBowScrollNum;
	siStaffScrollNum  = pclinfo->siStaffScrollNum;

	siCompletedSetItemIndex		= pclinfo->siCompletedSetItemIndex;
	siCompletedSetItemIndexStep = pclinfo->siCompletedSetItemIndexStep;

	siCompletedSetItemIndex2	 = pclinfo->siCompletedSetItemIndex2;
	siCompletedSetItemIndexStep2 = pclinfo->siCompletedSetItemIndexStep2;

	siCompletedSetItemIndex3	 = pclinfo->siCompletedSetItemIndex3;
	siCompletedSetItemIndexStep3 = pclinfo->siCompletedSetItemIndexStep3;
}

//====================================
// cltCharChangeStatusInfo
//====================================
cltCharChangeStatusInfo::cltCharChangeStatusInfo ()
{
	siChangeType	= 0;
	bChangeSwitch	= false;
	siChageAmount1	= 0;
	siChageAmount2	= 0;
	siChageAmount3	= 0;
}

cltCharChangeStatusInfo::cltCharChangeStatusInfo (SI32 changetype, bool bswitch, SI32 amount1, SI32 amount2, SI32 amount3 )
{
	siChangeType	= changetype;
	bChangeSwitch	= bswitch;
	siChageAmount1	= amount1;
	siChageAmount2  = amount2;
	siChageAmount3  = amount3;
}

void cltCharChangeStatusInfo::Set(cltCharChangeStatusInfo *pclInfo)
{
	siChangeType	= pclInfo->siChangeType;
	bChangeSwitch	= pclInfo->bChangeSwitch;
	siChageAmount1	= pclInfo->siChageAmount1;
	siChageAmount2  = pclInfo->siChageAmount2;
	siChageAmount3  = pclInfo->siChageAmount3;

}

//====================================
// cltCharAddStatusInfo
//====================================
cltCharAddStatusInfo::cltCharAddStatusInfo ()
{
	//siIdentity			= 0;
	//siHome				= 0;
	//siCapa				= 0;
	//siChildCharUnique	= 0;
	//siParentCharUnique	= 0;
	//siParentPersonID	= 0;
	//siSummonIndex		= 0;
};

//cltCharAddStatusInfo::cltCharAddStatusInfo (SI08 identity, SI16 home, SI16 capa, SI32 childunique, SI32 parentunique, SI32 parentperson, SI16 summonindex)
//{
//siIdentity			= identity;
//siHome				= home;
//siCapa				= capa;
//siChildCharUnique	= childunique;
//siParentCharUnique	= parentunique;
//siParentPersonID	= parentperson;
//siSummonIndex		= summonindex;
//};


void cltCharAddStatusInfo::Init()
{
#ifdef _SAFE_MEMORY
	siReserved.ZeroMem();
#endif
	//siIdentity			= 0;
	//siHome				= 0;
	//siCapa				= 0;
	//siChildCharUnique	= 0;
	//siParentCharUnique	= 0;
	//siParentPersonID	= 0;
	//siSummonIndex		= 0;
};

void cltCharAddStatusInfo::Set(cltCharAddStatusInfo *pclInfo)
{
	//siIdentity			= pclInfo->siIdentity;
	//siHome				= pclInfo->siHome;
	//siCapa				= pclInfo->siCapa;
	//siChildCharUnique	= pclInfo->siChildCharUnique;	
	//siParentCharUnique	= pclInfo->siParentCharUnique;
	//siParentPersonID	= pclInfo->siParentPersonID;	
	//siSummonIndex		= pclInfo->siSummonIndex;

}

//====================================
// cltEventCharInfo
//====================================
cltEventCharInfo::cltEventCharInfo()
{
	Init();
}

void cltEventCharInfo::Init()
{
	bEvent1Switch	= false;
	bEvent2Switch	= false;
	bEvent3Switch	= false;
	bEvent4Switch	= false;
	bEvent5Switch	= false;
	bEvent6Switch	= false;
	bEvent7Switch	= false;
}

void cltEventCharInfo::Set(cltEventCharInfo* pclinfo)
{
	bEvent1Switch	= pclinfo->bEvent1Switch;
	bEvent2Switch	= pclinfo->bEvent2Switch;
	bEvent3Switch	= pclinfo->bEvent3Switch;
	bEvent4Switch	= pclinfo->bEvent4Switch;
	bEvent5Switch	= pclinfo->bEvent5Switch;
	bEvent6Switch	= pclinfo->bEvent6Switch;
	bEvent7Switch	= pclinfo->bEvent7Switch;
}

//====================================
// cltEnemyTypeInfo
//====================================
cltEnemyTypeInfo::cltEnemyTypeInfo()
{
	Init();
}
void cltEnemyTypeInfo::Init()
{
	siEnemyType = 0;
	siPara1		= 0;
}
void cltEnemyTypeInfo::Set(cltEnemyTypeInfo* pclinfo)
{
	siEnemyType = pclinfo->siEnemyType;
	siPara1		= pclinfo->siPara1;
}
void cltEnemyTypeInfo::Set(SI16 type, SI16 para1)
{
	siEnemyType = type;
	siPara1		= para1;
}

bool cltEnemyTypeInfo::IsType(SI32 type)
{
	if(siEnemyType & type)return true;

	return false;
}

//====================================
// cltInnerStatusInfo
//====================================
cltInnerStatusInfo::cltInnerStatusInfo()
{
	ZeroMemory(this, sizeof(cltInnerStatusInfo));
};

cltInnerStatusInfo::cltInnerStatusInfo(SI32 status,	cltEnemyTypeInfo* pclenemytype, 
									   cltEventCharInfo* pclevent,
									   SI16 topskillunique)
{
	siStatus		= status;
	clEnemyType.Set(pclenemytype);
	clEventInfo.Set(pclevent);

	siTopSkillUnique	= topskillunique;
}

void cltInnerStatusInfo::Set(cltInnerStatusInfo* pclinfo)
{
	siStatus		= pclinfo->siStatus;
	clEnemyType.Set(&pclinfo->clEnemyType);
	clEventInfo.Set(&pclinfo->clEventInfo);

	siTopSkillUnique		= pclinfo->siTopSkillUnique;
}

void cltInnerStatusInfo::Init()
{
	siStatus				= 0;	
	clEnemyType.Init();	
	clEventInfo.Init();	

	siTopSkillUnique		= 0;
}

bool cltInnerStatusInfo::IsStatus(SI32 status)
{
	if(siStatus & status)return true;

	return false;
}

void cltInnerStatusInfo::SetStatus(SI32 status)
{
	siStatus |= status;
}

void cltInnerStatusInfo::DelStatus(SI32 status)
{
	if(siStatus & status)
	{
		siStatus -= status;
	}
}

//====================================
// cltIdentity
//====================================
cltIdentity::cltIdentity()
{
	Init();
}
cltIdentity::cltIdentity(SI08 identity, SI16 para1, SI16 para2, SI32 para3)
{
	siIdentity	= identity;
	clCommonInfo.siPara1		= para1;
	clCommonInfo.siPara2		= para2;
	clCommonInfo.siPara3		= para3;
}
void cltIdentity::Init()
{
	siIdentity	= 0;
	clCommonInfo.siPara1		= 0;
	clCommonInfo.siPara2		= 0;
	clCommonInfo.siPara3		= 0;
}

void cltIdentity::Set(cltIdentity* pclinfo)
{
	siIdentity	= pclinfo->siIdentity;
	clCommonInfo.siPara1		= pclinfo->clCommonInfo.siPara1;
	clCommonInfo.siPara2		= pclinfo->clCommonInfo.siPara2;
	clCommonInfo.siPara3		= pclinfo->clCommonInfo.siPara3;
}

// 소환수를 설정한다. 
void cltIdentity::MakeSummon(SI32 iddentity, SI32 index, SI32 parentcharid, SI32 parentpersonid)
{
	siIdentity			= iddentity;

	clSummonInfo.siMySummonIndex		= index;
	clSummonInfo.siParentCharUnique	= parentcharid;
	clSummonInfo.siParentPersonID	= parentpersonid;
}

// 성벽, 성황신, 성환신소환수를 설정한다.
void cltIdentity::MakeGod(SI32 iddentity, SI32 capa)
{
	siIdentity			= iddentity;
	clGodInfo.siCapa	= capa;
}

// 불가사리를 설정한다.
void cltIdentity::MakeHunt(SI32 iddentity, SI32 villageunique)
{
	siIdentity			= iddentity;
	clHuntInfo.siHome	= villageunique;
}

// 흑의군단 설정한다.
void cltIdentity::MakeBlackWar(SI32 iddentity)
{
	siIdentity			= iddentity;
}

// 불가사리를 설정한다.
void cltIdentity::MakeBlackArmy(SI32 iddentity, SI16 WarMapIndex, SI16 MapIndex)
{
	siIdentity			= iddentity;
	clBlackArmyInfo.siWarMapIndex = WarMapIndex;
	clBlackArmyInfo.siMapIndex = MapIndex;
}

void cltIdentity::MakeKingMonument(SI32	iddentity, SI32 villageunique) // 마을 수호상을 설정한다.
{
	siIdentity = iddentity;
	clKingMonument.siVillageUnique = villageunique;
}


// 모후모후 이벤트 몬스터
void cltIdentity::MakeMofuMofuEvent(SI32 iddentity, SI16 field, SI16 index, SI32 siCanAttack)
{
	siIdentity					= iddentity;
	clMofoMofuEvent.siField		= field;
	clMofoMofuEvent.siIndex		= index;
	clMofoMofuEvent.siCanAttack = siCanAttack;
}

void cltIdentity::MakeInstanceMap(SI32 iddentity, SI32 siScore)
{
	siIdentity					= iddentity;
	clInstanceMap.siScore		= siScore;
}
void cltIdentity::MakeMonsterAttackEvent( SI32 iddentity )
{
	siIdentity = iddentity;
}
void cltIdentity::MakeTreasureEvent( SI32 iddentity )
{
	siIdentity = iddentity;
}
void cltIdentity::MakeRaccoonDodgeEvent( SI32 iddentity )
{
	siIdentity = iddentity;
}

void cltIdentity::MakeBBBEventMonster( SI32 identity )
{
	siIdentity = identity;
}
void cltIdentity::MakeSoulGuard_Summon( SI32 iddentity )
{
	siIdentity = iddentity;
}
void cltIdentity::MakeBattleRoyalMob( SI32 iddentity)
{
	siIdentity = iddentity;
}
void cltIdentity::MakeGMCreateMonster( SI32 iddentity)
{
	siIdentity = iddentity;
}
void cltIdentity::MakeQuest( SI16 identity, SI32 siEnemyPersonID, SI32 siEnemyCharID )
{
	siIdentity						= identity;
	clQuestInfo.m_siEnemyPersonID	= siEnemyPersonID;
	clQuestInfo.m_siEnemyCharID		= siEnemyCharID;
}

void cltIdentity::MakeSoulBound( SI16 identity, SI32 siPersonID )
{
	siIdentity					= identity;
	clSoulBound.m_siPersonID	= siPersonID;
}

void cltIdentity::MakeSoulGuard( SI08 identity, SI16 siParentCharUnique, SI32 siParentPersonID )
{
	siIdentity							= identity;

	clSoulGuard.m_siParentCharUnique	= siParentCharUnique;
	clSoulGuard.m_siParentPersonID		= siParentPersonID;
}

void cltIdentity::MakeGiantRaccoonResurrectEvent( void )
{
	siIdentity = IDENTITY_GIANTRACCOONRESURRECTEVENT;
}

//====================================
// cltMakeCharInfoCommon
//====================================
cltMakeCharInfoCommon::cltMakeCharInfoCommon()
{
	Init();
}

cltMakeCharInfoCommon::cltMakeCharInfoCommon(UI32 makekind, UI32 makecharunique, SI32 home, UI32 level, 
											 UI32 life, UI32 x, UI32 y, UI32 mapindex, UI32 speed, SI16 jimcharunique, 
											 cltOrderDataInfo* pclorder,
											 cltIdentity* pclidentity, 
											 cltInnerStatusInfo* pclinnerstatus)
{
	if(IsPC(makecharunique))
	{
		uiType				= MAKECHARTYPE_PC;
	}
	else
	{
		uiType				= MAKECHARTYPE_NPC;
	}

	uiMakeCharKind		= makekind;
	uiMakeCharUnique	= makecharunique;
	siHomeVillageUnique	= home;
	uiLevel				= level;
	uiLife				= life;
	uiX					= x;
	uiY					= y;
	uiMapIndex			= mapindex;
	uiTotalMoveSpeed	= speed;
	siJimCharUnique		= jimcharunique;
	if(pclorder)
	{
		clOrder.Set(pclorder);
	}

	if(pclidentity)
	{
		clIdentity.Set(pclidentity);
	}

	if(pclinnerstatus)
	{
		clInnerStatus.Set(pclinnerstatus);
	}
	/*if(pclPVP_LeagueInfo)
	{
		clPVP_LeagueInfo.Set(pclPVP_LeagueInfo);
	}*/
}

void cltMakeCharInfoCommon::Init()
{
	uiType				= 0;
	uiMakeCharKind		= 0;
	uiMakeCharUnique	= 0;
	siHomeVillageUnique	= 0;
	uiLevel				= 0;
	uiLife				= 0;
	uiX					= 0;
	uiY					= 0;
	uiMapIndex			= 0;
	uiTotalMoveSpeed	= 0;
	siJimCharUnique		= 0;

	clOrder.Init();

	clIdentity.Init();

	clInnerStatus.Init();
	//clPVP_LeagueInfo.Init();
}

void cltMakeCharInfoCommon::Set(cltMakeCharInfoCommon* pclinfo)
{
	uiType				= pclinfo->uiType;
	uiMakeCharKind		= pclinfo->uiMakeCharKind;
	uiMakeCharUnique	= pclinfo->uiMakeCharUnique;
	siHomeVillageUnique	= pclinfo->siHomeVillageUnique;
	uiLevel				= pclinfo->uiLevel;
	uiLife				= pclinfo->uiLife;
	uiX					= pclinfo->uiX;
	uiY					= pclinfo->uiY;
	uiMapIndex			= pclinfo->uiMapIndex;
	uiTotalMoveSpeed	= pclinfo->uiTotalMoveSpeed;
	siJimCharUnique		= pclinfo->siJimCharUnique;
	clOrder.Set(&pclinfo->clOrder);
	clIdentity.Set(&pclinfo->clIdentity);
	clInnerStatus.Set(&pclinfo->clInnerStatus);
	/*clPVP_LeagueInfo.Set( &pclinfo->clPVP_LeagueInfo);*/
}

//====================================
// cltMakeCharInfo
//====================================
cltMakeCharInfo::cltMakeCharInfo() : cltMakeCharInfoCommon()
{
	Init();
};


cltMakeCharInfo::cltMakeCharInfo(UI32 					uikind, 
								 UI32 					makecharunique, 
								 UI32 					level, 
								 UI32 					life, 
								 UI32 					x, 
								 UI32 					y, 
								 UI32 					mapindex, 
								 UI32 					childcharunique, 
								 UI32 					summonindex, 
								 UI32 					bheadchatswitch, 
								 UI32 					bpersonalshopswitch, 
								 UI32 					bnofirstattackswitch,
								 UI32 					bnoiceattackswitch,
								 UI32 					bhearteffectswitch,
								 UI32 					bfourleafswitch,
								 UI32 					bfarmminingswitch,
								 UI32 					bcushionswitch,
								 UI32					bsex,
								 cltShapeEquipInfo*		pclequipinfo, 
								 SI08 					CurrentShowHorse, 
								 UI32 					totalmovespeed, 
								 SI16 					jimcharunique, 
								 SI08 					gamemasterlevel, 
								 SI16 					mapunique, 
								 UI32 					horseunique1, 
								 UI32 					homevillageunique, 
								 UI32 					schoolunique,
								 cltSimpleRank*			pclrank,
								 SI32 					addstatusdate,
								 SI32 					ticketnum, 
								 cltOrderDataInfo*		pclorder, 
								 cltIdentity*			pclidentity,
								 cltInnerStatusInfo*	pclinnerstatus, 
								 UI32 					requestaddinfo,
								 UI32 					ticketwarp,
								 UI08 					gmmode,
								 bool 					pcroom,
								 SI08 					makeanitype,
								 cltSimpleGuildRank*	pclguildrank,
								 cltPersonNaming*		pclpersonnaming,
								 bool 					CurrentHorseRIDEHORSE,
								 UI16 					NowTransFormKind,
								 bool 					TransFormMode,
								 UI32 					BestGuild,					// [추가 : 황진성 2008. 1. 14 => 자신의 길드가 최강인가.]
								 bool 					bIsElectionStump,
								 bool 					TransForm100Effect,
								 SI16 					siBufTransform,
								 SI16 					EnemyUnique,
								 cltValentineWarInfo*	pclValentineWarInfo,
								 bool					bElixir,
								 cltPVP_LeagueInfo* 	pclPVP_Leagueinfo,
								 SI32 					PremiumPartsdate_Slot1,
								 SI32 					PremiumPartsUnique_Slot1,
								 SI32 					PremiumPartsdate_Slot2,
								 SI32 					PremiumPartsUnique_Slot2,
								 SI32 					PremiumPartsdate_Slot3,
								 SI32 					PremiumPartsUnique_Slot3,
								 SI32 					PremiumPartsNum,
								 UI08 					weaponSkillMasterEmblem,
								 SI16 					displayLoveLevel,
								 SI16 					soulGuardUnique,
								 bool 					bRepairShopSW,
								 cltBufInfo*			_pclBufInfo	 ) : cltMakeCharInfoCommon (uikind, 
																								makecharunique, 
																								homevillageunique, 
																								level, 
																								life, 
																								x, 
																								y, 
																								mapindex, 
																								totalmovespeed, 
																								jimcharunique, 
																								pclorder,
																								pclidentity, 
																								pclinnerstatus
																								//,pclPVP_Leagueinfo
								 )
{

	uiChildCharUnique		= childcharunique;
	uiSummonIndex			= summonindex;

	uiHeadChatSwitch		= bheadchatswitch;
	uiPersonalShopSwitch	= bpersonalshopswitch;

	uiNoFirstAttackSwitch	= bnofirstattackswitch;
	uiNoIceAttackSwitch		= bnoiceattackswitch;
	uiHeartEffectSwitch		= bhearteffectswitch;
	uiFourLeafSwitch		= bfourleafswitch;
	uiCusionSwitch			= bcushionswitch;
	uiSex					= bsex;

	uiHorseUnique1		= horseunique1;
	uiSchoolUnique		= schoolunique;

	siCurrentShowHorse		= CurrentShowHorse;
	uiRequestAddinfo    = requestaddinfo;
	uiTicketWarpSwitch	= ticketwarp;


	siGameMasterLevel	= gamemasterlevel;
	siMapUnique			= mapunique;

	if(pclequipinfo)
	{
		clShapeEquip.Set(pclequipinfo);
	}

	if(pclrank)
	{
		clRank.Set(pclrank);
	}

	siAddStatusDate = addstatusdate ;
	siStatusTicketNum = ticketnum;

	uiGMMode = gmmode;
	bPCRoom = pcroom;
	siMakeAniType = makeanitype;

	if ( pclguildrank )
	{
		clGuildRank.Set(pclguildrank);
	}
	siCurrentHorseRIDEHORSE = CurrentHorseRIDEHORSE;
	if ( pclpersonnaming )
	{
		clPersonNaming.Set(pclpersonnaming);
	}
	uiNowTransFormKind	= NowTransFormKind;
	bTransFormMode		= TransFormMode;

	uiBestGuild			= BestGuild;					// [추가 : 황진성 2008. 1. 14 => 자신의 길드가 최강인가.]
	uiElectionStump		= bIsElectionStump;
	uiElixir			= bElixir;		
	uiRepairShopSwitch	= bRepairShopSW;

	bTransForm100Effect = TransForm100Effect;

    siBufTransformKind	= siBufTransform;
	siEnemyUnique		= EnemyUnique;	// 개인간 PVP
	if(pclValentineWarInfo)
	{
		clValentineWarInfo.Set( pclValentineWarInfo);
	}
	siPremiumPartsDate_Slot1 = PremiumPartsdate_Slot1 ;
	siPremiumPartsUnique_Slot1 = PremiumPartsUnique_Slot1;
	siPremiumPartsDate_Slot2 = PremiumPartsdate_Slot2;
	siPremiumPartsUnique_Slot2 = PremiumPartsUnique_Slot2;
	siPremiumPartsDate_Slot3 = PremiumPartsdate_Slot3 ;
	siPremiumPartsUnique_Slot3 = PremiumPartsUnique_Slot3;
	siPremiumPartsNum = PremiumPartsNum;

	uiWeaponSkillMasterEmblem	= weaponSkillMasterEmblem;

	siDisplayLoveLevel			= displayLoveLevel;

	siSoulGuardUnique			= soulGuardUnique;

	m_clBufInfo.SetBuf( _pclBufInfo );
}

void cltMakeCharInfo::Init()
{
	cltMakeCharInfoCommon::Init();

	uiChildCharUnique	= 0;
	uiSummonIndex	= 0;
	uiHeadChatSwitch	= 0;
	uiPersonalShopSwitch	= 0;
	uiNoFirstAttackSwitch	= 0;
	uiNoIceAttackSwitch	= 0;
	uiHeartEffectSwitch = 0;
	uiFourLeafSwitch	= 0;
	uiCusionSwitch		= 0;
	uiSex	= 0;


	uiHorseUnique1	= 0;
	uiSchoolUnique	= 0;
	uiRequestAddinfo	= 0;
	uiTicketWarpSwitch	= 0;

	siCurrentShowHorse	= 0;
	siGameMasterLevel	= 0;
	siMapUnique		= 0;

	clShapeEquip.Init();

	clRank.Init();

	siAddStatusDate = 0 ;
	siStatusTicketNum = 0 ;

	uiGMMode = 0;
	bPCRoom = false;
	siMakeAniType = 0;

	clGuildRank.Init();
	clPersonNaming.Init();
	siCurrentHorseRIDEHORSE = false;

	uiNowTransFormKind	= 0;
	bTransFormMode		= false;
	uiBestGuild			= false;
	
	uiElectionStump		= false;
	uiElixir			= false;
	bTransForm100Effect = false;
	uiRepairShopSwitch	= false;

	siBufTransformKind	= 0;
	siEnemyUnique		= 0;
	clValentineWarInfo.Init();
	
	siPremiumPartsDate_Slot1 = 0;
	siPremiumPartsUnique_Slot1= 0;
	siPremiumPartsDate_Slot2= 0;
	siPremiumPartsUnique_Slot2= 0;
	siPremiumPartsDate_Slot3= 0;
	siPremiumPartsUnique_Slot3= 0;
	siPremiumPartsNum= 0;

	uiWeaponSkillMasterEmblem	= 0;

	siSoulGuardUnique			= 0;

	uiRepairShopSwitch			= false;
}


void cltMakeCharInfo::Set(cltMakeCharInfoCommon* pclinfo)
{
	Init();
	cltMakeCharInfoCommon::Set(pclinfo);
}

void cltMakeCharInfo::Set(cltMakeCharInfo* pclinfo)
{
	cltMakeCharInfoCommon::Set(pclinfo);

	uiChildCharUnique			= pclinfo->uiChildCharUnique;
	uiSummonIndex				= pclinfo->uiSummonIndex;

	uiHeadChatSwitch			= pclinfo->uiHeadChatSwitch;
	uiPersonalShopSwitch		= pclinfo->uiPersonalShopSwitch;

	uiNoFirstAttackSwitch		= pclinfo->uiNoFirstAttackSwitch;
	uiNoIceAttackSwitch			= pclinfo->uiNoIceAttackSwitch;
	uiHeartEffectSwitch			= pclinfo->uiHeartEffectSwitch;
	uiFourLeafSwitch			= pclinfo->uiFourLeafSwitch;
	uiCusionSwitch				= pclinfo->uiCusionSwitch;
	uiSex						= pclinfo->uiSex;	

	uiHorseUnique1				= pclinfo->uiHorseUnique1;
	uiSchoolUnique				= pclinfo->uiSchoolUnique;

	siCurrentShowHorse			= pclinfo->siCurrentShowHorse;
	uiRequestAddinfo			= pclinfo->uiRequestAddinfo;
	uiTicketWarpSwitch			= pclinfo->uiTicketWarpSwitch;

	siGameMasterLevel			= pclinfo->siGameMasterLevel;
	siMapUnique					= pclinfo->siMapUnique;

	clShapeEquip.Set(&pclinfo->clShapeEquip);

	clRank.Set(&pclinfo->clRank);

	siAddStatusDate				= pclinfo->siAddStatusDate ;
	siStatusTicketNum			= pclinfo->siStatusTicketNum ;

	uiGMMode					= pclinfo->uiGMMode;
	bPCRoom						= pclinfo->bPCRoom;
	siMakeAniType				= pclinfo->siMakeAniType;

	clGuildRank.Set(&pclinfo->clGuildRank);
	clPersonNaming.Set(&pclinfo->clPersonNaming);
	siCurrentHorseRIDEHORSE		= pclinfo->siCurrentHorseRIDEHORSE;

	uiNowTransFormKind			= pclinfo->uiNowTransFormKind;
	bTransFormMode				= pclinfo->bTransFormMode;
	uiBestGuild					= pclinfo->uiBestGuild;

	uiElectionStump				= pclinfo->uiElectionStump;	
	uiElixir					= pclinfo->uiElixir;
	uiRepairShopSwitch			= pclinfo->uiRepairShopSwitch;
	uiRepairShopSwitch			= pclinfo->uiRepairShopSwitch;
	
	bTransForm100Effect			= pclinfo->bTransForm100Effect;

	siBufTransformKind			= pclinfo->siBufTransformKind;

	siEnemyUnique				= pclinfo->siEnemyUnique;
	clValentineWarInfo.Set( &pclinfo->clValentineWarInfo ) ;
	
	siPremiumPartsDate_Slot1	=	pclinfo->siPremiumPartsDate_Slot1;
	siPremiumPartsUnique_Slot1	=	pclinfo->siPremiumPartsUnique_Slot1;
	siPremiumPartsDate_Slot2	=	pclinfo->siPremiumPartsDate_Slot2;
	siPremiumPartsUnique_Slot2	=	pclinfo->siPremiumPartsUnique_Slot2;
	siPremiumPartsDate_Slot3	=	pclinfo->siPremiumPartsDate_Slot3;
	siPremiumPartsUnique_Slot3	=	pclinfo->siPremiumPartsUnique_Slot3;
	siPremiumPartsNum			=	pclinfo->siPremiumPartsNum	;

	uiWeaponSkillMasterEmblem	= pclinfo->uiWeaponSkillMasterEmblem;

	siDisplayLoveLevel			= pclinfo->siDisplayLoveLevel;

	siSoulGuardUnique			= pclinfo->siSoulGuardUnique;

	m_clBufInfo.SetBuf( &pclinfo->m_clBufInfo);
}

//====================================
// cltPFatherInfo
//====================================
cltPFatherInfo::cltPFatherInfo()
{
	Init();
}

void cltPFatherInfo::Init()
{
	clFather.Init();
	szAccountID[ 0 ] = NULL;
	siMyLevelForFather		= 0;
	siFatherChangeNumber	= 0;
	siTeacherPoint = 0;
	bOverTake = false;
}

void cltPFatherInfo::Set(cltPFatherInfo* pclinfo)
{
	clFather.Set(&pclinfo->clFather);
	MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );
	siMyLevelForFather		= pclinfo->siMyLevelForFather;
	siFatherChangeNumber	= pclinfo->siFatherChangeNumber;
	siTeacherPoint = pclinfo->siTeacherPoint;
	bOverTake = pclinfo->bOverTake;
}

//====================================
// cltBufInfo
//====================================
cltBufInfo::cltBufInfo()
{
	Init();
}

void cltBufInfo::Init()
{
	for ( SI16 i=0 ; i < MAX_BUF_COUNT ; i++ )
	{
		m_stBuf[i].siOverTime = 0;
		m_stBuf[i].siBufType = 0;
		m_stBuf[i].siPercent = 0;
		m_stBuf[i].siGetType = 0;
	}
	m_bNoFirstAttackPreminum = false;
	m_bitBufUse.ClearAll();
}

void cltBufInfo::SetBuf( cltBufInfo* pclinfo )
{
	for ( SI16 i=0; i < MAX_BUF_COUNT ; i++ )
	{
		m_stBuf[i].siBufType	= pclinfo->m_stBuf[i].siBufType;
		// 버프 사용 표시
		if( m_stBuf[i].siBufType > 0 && m_stBuf[i].siBufType < BUF_END )
		{
			m_bitBufUse.SetBit(m_stBuf[i].siBufType);
		}
		m_stBuf[i].siPercent	= pclinfo->m_stBuf[i].siPercent;
		m_stBuf[i].siOverTime	= pclinfo->m_stBuf[i].siOverTime;

		m_stBuf[i].siGetType	= pclinfo->m_stBuf[i].siGetType;
	}
	m_bNoFirstAttackPreminum = pclinfo->m_bNoFirstAttackPreminum;
}

SI16 cltBufInfo::FindEmptySlot( SI16 type, bool overlap )
{
	if( overlap == false)
	{	
		// 같은 버프는 다시 걸수 없다.
		if( IsHaveBuf(type) )			return -1;
	}
	// 빈 버프 슬롯을 찾는다.
	// 중복될수 없거나 빈슬롯이 없는경우 -1을 리턴
	for ( SI16 index=0 ; index < MAX_BUF_COUNT ; index ++ )
	{
		if ( m_stBuf[index].siBufType == 0 )
		{
			return index;
		}
	}
	return -1;
}

void cltBufInfo::AddBuf( SI16 index, stBuf buf )
{
	if( index < 0 || index > MAX_BUF_COUNT )		return;

	// 해당 인덱스에 버프 추가
	m_stBuf[index].siBufType = buf.siBufType;
	m_stBuf[index].siPercent = buf.siPercent;
	m_stBuf[index].siOverTime = buf.siOverTime;
	m_stBuf[index].siGetType = buf.siGetType;

	// 버프 사용 표시 - 체크
	m_bitBufUse.SetBit(buf.siBufType);
}

void cltBufInfo::DeleteBuf( SI16 index )
{
	if( index < 0 || index > MAX_BUF_COUNT )		return;

	// 버프 사용 표시 - 삭제
	m_bitBufUse.ClearBit(m_stBuf[index].siBufType);

	m_stBuf[index].siOverTime = 0;
	m_stBuf[index].siBufType = 0;
	m_stBuf[index].siPercent = 0;
	m_stBuf[index].siGetType = 0;
}

bool cltBufInfo::CanUseBuf( SI16 type )
{
	if ( (type < 0) || (type > MAX_BUF) ) return false;

	// 각 버프별로 중복 할 수 없는 버프들을 설정한다.
	switch(type)
	{
	case BUF_ATTACKUP:
		{
		}
		break;
	case BUF_DEFENSEUP:
		{
		}
		break;
	case BUF_SPEEDUP:
		{
		}
		break;
	case BUF_MAKEEXPUP:
		{
			if ( IsHaveBuf( BUF_FRIENDSHIP) )	return false ; 
			if ( IsHaveBuf( BUF_LOVEPOWER) )	return false ;
			if ( IsHaveBuf( BUF_HINAMOCHI) )	return false ;
		}
		break;
	case BUF_NOFIRSTATTACK:
		{
		}
		break;
	case BUF_SUMMONATACUP:
		{
		}
		break;
	case BUF_LOVEPOWER:
		{
		}
		break;
	case BUF_FRIENDSHIP:
		{
		}
		break;
	case BUF_HINAMOCHI:
		{
		}
		break;
	case BUF_SUCCESS_MANUFACTURE:
		{
		}
		break;
	case BUF_MOFUMOFU_PERFUME:
		{
		}
		break;
	case BUF_FOURLEAF:
		{
		}
		break;
	case BUF_PREMIUMEXPUP:
		{
		}
		break;
	case BUF_HAPPYBEAN:
		{
		}
		break;
	case BUF_ATTACKUP30:
		{
		}
		break;
	case BUF_DEFENSEUP30:
		{
		}
		break;
	case BUF_SUPEREXPUP:
		{
		}
		break;
	case BUF_NOTBONUSTIME_MAKEEXPUP:
		{
		}
		break;
	case BUF_THANKSGIVING_DAY:		
		{
		}
		break;
	case BUF_FARMINGNMININGFOREVER:	
		{
		}
		break;
	case BUF_STATUE_UP_STR:	
		{
		}
		break;
	case BUF_STATUE_UP_DEX:	
		{
		}
		break;
	case BUF_STATUE_UP_MAG:	
		{
		}
		break;
	case BUF_STATUE_UP_VIT:	
		{
		}
		break;
	case BUF_STATUE_UP_HAND:	
		{
		}
		break;
	case BUF_EXP_UP:
		{
		}
		break;
	case BUF_OLYMPICMEDAL:
		{
			if ( IsHaveBuf( BUF_STATUE_UP_STR) )	return false ; 
			if ( IsHaveBuf( BUF_STATUE_UP_DEX) )	return false ;
			if ( IsHaveBuf( BUF_STATUE_UP_VIT) )	return false ;
			if ( IsHaveBuf( BUF_STATUE_UP_HAND) )	return false ; 
			if ( IsHaveBuf( BUF_OLYMPICMEDAL) )		return false ;
		}
		break;
	case BUF_QUEST_ANTIPOTION:
		{
		}
		break;
	case BUF_EVENT_FINDTREASURE:
		{
		}
		break;
	case BUF_QUEST_ATTACK:
		{
		}
		break;
	case BUF_WATERMELON:
		{
		}
		break;
	case BUF_TRANSFORM_HORSE:
		{
		}
		break;
	case BUF_RESIDENTS_EXPUP_10:
		{
		}
		break;
	case BUF_RESIDENTS_STRONG:
		{
		}
		break;

	case BUF_VALENTINEDAY:
		{

		}
		break;
	}

	return true;
}

SI32 cltBufInfo::CalcTotalBufValue( SI16 type, SI32 currentValue )
{
	SI32 calcValue = currentValue;

	for ( SI16 i=0; i<MAX_BUF_COUNT; i++ )
	{
		if( m_stBuf[i].siBufType == type )
		{
			if ( m_stBuf[i].siPercent )
			{
				calcValue += calcValue * m_stBuf[i].siPercent  / 100;
			}
		}
	}

	return calcValue;
}

bool cltBufInfo::IsHaveBuf(SI16 type)
{
	if ( (type < 0) || (type > MAX_BUF) ) return 0;

	// 버프 사용 여부
	return m_bitBufUse.GetBit(type);
}

bool cltBufInfo::IsHaveBufWithGetType( SI16 siFindBufType, SI16 siFindGetType )
{
	if ( (0 > siFindBufType) || (MAX_BUF < siFindBufType) )
	{
		return false;
	}

	// 빠른 검색을 위해 먼저 해당 버프가 있는지 부터 검사한다
	if ( false == m_bitBufUse.GetBit(siFindBufType) )
	{
		return false;
	}

	for ( SI32 siBufIndex=0; siBufIndex<MAX_BUF_COUNT; siBufIndex++ )
	{
		// 버프 타입도 같고 겟타입도 같아야 한다
		if ( siFindBufType == m_stBuf[siBufIndex].siBufType )
		{
			if ( siFindGetType == m_stBuf[siBufIndex].siGetType )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

bool cltBufInfo::IsHaveBufExecptGetType( SI16 siFindBufType, SI16 siFindGetType )
{
	if ( (0 > siFindBufType) || (MAX_BUF < siFindBufType) )
	{
		return false;
	}

	// 빠른 검색을 위해 먼저 해당 버프가 있는지 부터 검사한다
	if ( false == m_bitBufUse.GetBit(siFindBufType) )
	{
		return false;
	}

	for ( SI32 siBufIndex=0; siBufIndex<MAX_BUF_COUNT; siBufIndex++ )
	{
		// 버프 타입은 같지만 겟타입이 같은건 제외한다(즉 겟타입이 달라야 한다)
		if ( siFindBufType == m_stBuf[siBufIndex].siBufType )
		{
			if ( siFindGetType != m_stBuf[siBufIndex].siGetType )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

SI32 cltBufInfo::GetBufPercent(SI16 type)
{
	if ( (type < 0) || (type > MAX_BUF) ) return 0;

	for( SI16 i = 0 ; i < MAX_BUF_COUNT; i++)
	{
		if( m_stBuf[i].siBufType == type )
		{
			return m_stBuf[i].siPercent;
		}
	}

	return 0;
}

stBuf* cltBufInfo::GetBuf(SI16 type)
{
	if ( (type < 0) || (type > MAX_BUF) ) return NULL;

	for( SI16 i = 0 ; i < MAX_BUF_COUNT; i++)
	{
		if( m_stBuf[i].siBufType == type )
		{
			return &m_stBuf[i];
		}
	}

	return NULL;
}

SI32 cltBufInfo::GetBufOvertime(SI16 type)
{
	if ( (type < 0) || (type > MAX_BUF) ) return 0;

	for( SI16 i = 0 ; i < MAX_BUF_COUNT; i++)
	{
		if( m_stBuf[i].siBufType == type )
		{
			return m_stBuf[i].siOverTime;
		}
	}

	return 0;
}

SI16 cltBufInfo::FindBufSlot( SI16 siType )
{
	if ( (siType < 0) || (siType > MAX_BUF) ) return -1;

	for ( SI16 siCount=0; siCount<MAX_BUF_COUNT; siCount++ )
	{
		if ( m_stBuf[siCount].siBufType == siType )
		{
			return siCount;
		}
	}

	return -1;
}

SI16 cltBufInfo::FindBufGetTypeSlot( SI16 GetType )
{
	if ( (GetType < 0) || (GetType > MAX_BUF) ) return -1;

	for ( SI16 siCount=0; siCount<MAX_BUF_COUNT; siCount++ )
	{
		if ( m_stBuf[siCount].siGetType == GetType )
		{
			return siCount;
		}
	}

	return -1;
}

void cltBufInfo::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	pkGMSectionData->BeginData( "BufInfo" );
	{
		for ( SI16 siCount=0; siCount<MAX_BUF_COUNT; siCount++ )
		{
			pkGMSectionData->BeginData("buf");
			if ( m_stBuf[siCount].siBufType > 0 )
			{
				pkGMSectionData->AddDataInt( "type",		m_stBuf[siCount].siGetType );
				pkGMSectionData->AddDataInt( "overtime",	m_stBuf[siCount].siOverTime);	
			}
			pkGMSectionData->EndData("buf");
		}
	}
	pkGMSectionData->EndData( "BufInfo" );
}

//KHY - 1220 - 변신시스템.
//====================================
// cltBufInfo
//====================================
cltTimeMeterRateInfo::cltTimeMeterRateInfo()
{
	m_siUseTimePerSec = 1;		// 기본적으로 1초씩 차감된다.

	Init();
}

void cltTimeMeterRateInfo::Init()
{
	for ( SI16 i=0 ; i < MAX_TIMEMETERRATE ; i++ )
	{
		m_clTimeMeterRate[i].Init();	
		m_clTimeMeterRate[i].siTimeMeterRateType= i;
	}
}

void cltTimeMeterRateInfo::SetTimeMeterRate( cltTimeMeterRateInfo* pclinfo )
{
	for ( SI16 i=0; i < MAX_TIMEMETERRATE ; i++ )
	{
		m_clTimeMeterRate[i].Set(&pclinfo->m_clTimeMeterRate[i]);
	}
}

void cltTimeMeterRateInfo::AddLeftTime( SI16 siTimeMeterType, SI32 siAddTimeSec )
{
	cltTimeMeterRate* pclTimeMeterRate = GetTimeMeterRate( siTimeMeterType );

	if(pclTimeMeterRate != NULL)
	{
		pclTimeMeterRate->AddLeftTime(siAddTimeSec);
	}
}

void cltTimeMeterRateInfo::UseTime( SI16 siTimeMeterType, SI32 siUseTimeSec )
{
	cltTimeMeterRate* pclTimeMeterRate = GetTimeMeterRate( siTimeMeterType );

	if(pclTimeMeterRate != NULL)
	{
		pclTimeMeterRate->UseTime(siUseTimeSec);
	}
}

SI32 cltTimeMeterRateInfo::GetLeftTime( SI16 siTimeMeterType )
{
	cltTimeMeterRate* pclTimeMeterRate = GetTimeMeterRate( siTimeMeterType );

	if(pclTimeMeterRate != NULL)
	{
		return pclTimeMeterRate->siLeftTimeSec;
	}

	return 0;
}

bool cltTimeMeterRateInfo::UseStart( SI16 siTimeMeterType )
{
	cltTimeMeterRate* pclTimeMeterRate = GetTimeMeterRate( siTimeMeterType );

	if(pclTimeMeterRate != NULL)
	{
		if( pclTimeMeterRate->IsUse() )				return false;
		if( pclTimeMeterRate->siLeftTimeSec <= 0)	return false;

		pclTimeMeterRate->dwStartedTime = GetTickCount();

		return true;
	}

	return false;
}

bool cltTimeMeterRateInfo::UseEnd( SI16 siTimeMeterType )
{
	cltTimeMeterRate* pclTimeMeterRate = GetTimeMeterRate( siTimeMeterType );

	if(pclTimeMeterRate != NULL)
	{
		if( pclTimeMeterRate->IsUse() == false)		return false;

		pclTimeMeterRate->siUseTimeSec = 0;
		pclTimeMeterRate->dwStartedTime = 0;

		return true;
	}

	return false;
}

cltTimeMeterRate* cltTimeMeterRateInfo::GetTimeMeterRate( SI16 siTimeMeterRateType )
{
	if( siTimeMeterRateType < 1 || siTimeMeterRateType >= MAX_TIMEMETERRATE)
		return NULL;

	cltTimeMeterRate* pclTimeMeterRate = &m_clTimeMeterRate[siTimeMeterRateType];
	if( pclTimeMeterRate != NULL)
	{
		if( pclTimeMeterRate->siTimeMeterRateType != siTimeMeterRateType )
			return NULL;
	}
	return pclTimeMeterRate;
}

//====================================
// cltPBasicInfo
//====================================
cltPBasicInfo::cltPBasicInfo()
{		
	siIDNum				= 0;
	uiIndex				= 0;
	uiSex				= 0;
	uiKind				= 0;
	siHomeVillage		= 0;
	uiGRank				= 0;
	uiAngelSwitch		= 0;
	uiPoliceSwitch		= 0;
	siPlaySecond		= 0;
	siPlayEventSecond	= 0;
	clLastMainMapPos.Init();
	siSchoolUnique		= 0;
	clPerson.Init();
	clFatherInfo.Init();
	clRecommenderInfo.Init();
	siVoteKingSelectionIndex = 0;

	siWealthRank		= 0;
	clTotalWealthMoney.Clear();
	siWealthCalcDay		= 0;

	szAccountID[ 0 ] = NULL;

	bWennyOldManUpgrade = false ;

	bNewAccount = false ;
	bGameEvent = false ;
	bWinPrizeEvent = false ;

	bUseNoLimitTicketWarp = false ;
	siNoLimitTicketWarpDateVary = 0 ;

	uiGMMode = 0;
	bPCRoom = false;
	sPCroomGrade = 0;
	StringCchCopy(szCRMcode, 16, TEXT(""));
	bQuestEvolve = false ;
	StringCchCopy(szGreeting, 128, TEXT(""));

	siCountryID = -1;
	bShowCountry = false;
	bShowPuchikomi = false;

	//_LEON_GUILD_CHAT		
	//#if defined(_DEBUG)
	clGuildRank.Init();
	//#endif

	bPremiumService = false ;
	siPremiumServiceDate = 0 ;
	bCombatService = false ;
	siCombatServiceDate = 0 ;
	bMakeItemService = false;
	siMakeItemServiceDate = 0;
	bBulletService = false;
	siBulletServiceDate = 0;
	bSummonStaminaService = false;
	siSummonStaminaServiceDate = 0;

	clVillageScoreInfo.Init();

	bReceiveBeginnerWeapon = false;

	siPVPWinNumber = 0;
	siPVPLoseNumber = 0;
	MStrCpy ( szPVPWinHighLevelUserName, "", MAX_PLAYER_NAME );

	bMarKetPremiumService			= false;
	siMarKetPremiumServiceDate		= 0;
	
	bNewMarketPremiumService		= false;
	siNewMarketPremiumServiceDate	= 0;

	bSpaceBoxPremiumService			= false;	//시공간 상자
	siSpaceBoxPremiumServiceDate	= 0;		//시공간 상자;

#ifdef _SAFE_MEMORY
	bAttendToday.ZeroMem();
#else
	ZeroMemory( bAttendToday, sizeof( bAttendToday ) );
#endif
	bIsAttendTime		= false;
	bIsCanAttend		= false;
	bIsCanReward		= false;
	siAttendanceDays	= 0;

	ZeroMemory(&siFirstCreatTimeDateVary , sizeof(_SYSTEMTIME) );	 //KHY - 0906 - 최초 계정 생성 시간.

	bSummonExtraSlot		= false ;
	siSummonExtraSlotDate	=	0	;				// 소환수 확장 티켓 사용날자

}

cltPBasicInfo::cltPBasicInfo(TCHAR* paccountid, SI32 idnum, UI08 index, 
							 UI08 sex, cltSimplePerson* pclperson, UI08 kind, 
							 SI08 homevillage, cltCharPos* pclpos, 
							 SI32 playsecond, SI32 playeventsecond, 
							 SI16 schoolunique, cltPos* pclmainmappos, 
                             cltPFatherInfo* pclfather, 
							 cltSimplePerson* pclRecommenderInfo, 
							 SI16 VoteKingSelectionIndex, SI32 wealthrank, 
							 cltMoney totalmoney, SI32 wealthday,
							 bool wennyoldmanupgrade, bool newaccount,
							 bool gameevent , bool winprizeevent,
							 bool nolimitticketwarp , SI32 warpticketdatevary,
							 UI08 gmmode , bool pcroom, TCHAR pcroomgrade , 
							 TCHAR* crmcode, bool bquestevolve, TCHAR* greeting,
							 cltSimpleGuildRank* pclguildrank ,bool premium,
							 SI32 premiumdate,bool combat,SI32 combatdate, 
							 bool MakeItem, SI32 MakeItemdate, 
							 bool BulletService, SI32 BulletServiceDate,
							 bool SummonStaminaService, 
							 SI32 SummonStaminaServiceDate, 
							 cltVillageScoreInfo* pclVillageScoreInfo,
							 cltBufInfo* pclBufInfo, bool ReceiveBeginnerWeapon,
							 SI32 WinNumber, SI32 LoseNumber, 
							 TCHAR * HighLevelUserName, 
							 bool MarKetPremiumService, 
							 SI32 MarKetPremiumServiceDate, bool* AttendToday, CEmpireInfo* EmpireInfo ,_SYSTEMTIME firstcreatTime,
							 bool SpaceBoxPremiumService,
                             SI32 SpaceBoxPremiumServiceDate,
							 bool	SummonExtraSlot,
							 SI32	SummonExtraSlotDate,
							 bool	NewMarketPremiumService,
							 SI32	NewMarketPremiumServiceDate,
							 UI08 uiRealAge
							 )
{
	MStrCpy(szAccountID, paccountid, MAX_PLAYER_NAME );

	siIDNum			= idnum;
	uiIndex			= index;
	uiSex			= sex;

	uiKind			= kind;
	siHomeVillage	= homevillage;
	uiGRank			= 0;
	uiAngelSwitch	= 0;
	uiPoliceSwitch	= 0;
	clPos.Set(pclpos);
	siPlaySecond	= playsecond;
	siPlayEventSecond	= playeventsecond;
	siSchoolUnique	= schoolunique;
	clLastMainMapPos.Set(pclmainmappos);

	clPerson.Set(pclperson);

	clFatherInfo.Set(pclfather);
	clRecommenderInfo.Set(pclRecommenderInfo);
	siVoteKingSelectionIndex = VoteKingSelectionIndex;

	siWealthRank		= wealthrank; 
	clTotalWealthMoney.Set(&totalmoney);
	siWealthCalcDay		= wealthday;

	bWennyOldManUpgrade = wennyoldmanupgrade ;

	bNewAccount = newaccount ;
	bGameEvent = gameevent ;
	bWinPrizeEvent = winprizeevent ;

	bUseNoLimitTicketWarp = nolimitticketwarp ;
	siNoLimitTicketWarpDateVary = warpticketdatevary ;

	bSpaceBoxPremiumService = SpaceBoxPremiumService; //공간상자
	siSpaceBoxPremiumServiceDate = SpaceBoxPremiumServiceDate; //공간상자

	uiGMMode = gmmode;
	bPCRoom = pcroom;
	sPCroomGrade = pcroomgrade;
	MStrCpy(szCRMcode, crmcode,16);
	bQuestEvolve = bquestevolve ;
	MStrCpy(szGreeting, greeting, 128);


	clGuildRank.Set( pclguildrank );

	bPremiumService = premium ;
	siPremiumServiceDate = premiumdate;
	bCombatService = combat ;
	siCombatServiceDate = combatdate;
	bMakeItemService = MakeItem;
	siMakeItemServiceDate = MakeItemdate;
	bBulletService = BulletService;
	siBulletServiceDate = BulletServiceDate;
	bSummonStaminaService = SummonStaminaService;
	siSummonStaminaServiceDate = SummonStaminaServiceDate;

	clVillageScoreInfo.Set(pclVillageScoreInfo);
	clBufInfo.SetBuf( pclBufInfo );

	bReceiveBeginnerWeapon = ReceiveBeginnerWeapon;

	siPVPWinNumber = WinNumber;
	siPVPLoseNumber = LoseNumber;
	if( NULL == HighLevelUserName )
	{
		MStrCpy(szPVPWinHighLevelUserName, "", MAX_PLAYER_NAME);
	}
	else
	{
		MStrCpy(szPVPWinHighLevelUserName, HighLevelUserName, MAX_PLAYER_NAME);
	}

	bMarKetPremiumService			= MarKetPremiumService;
	siMarKetPremiumServiceDate		= MarKetPremiumServiceDate;

	bNewMarketPremiumService		= NewMarketPremiumService;		
	siNewMarketPremiumServiceDate	= NewMarketPremiumServiceDate;

	bSpaceBoxPremiumService			= SpaceBoxPremiumService;
	siSpaceBoxPremiumServiceDate	= SpaceBoxPremiumServiceDate;
	
#ifdef _SAFE_MEMORY
	memcpy( &bAttendToday[0], AttendToday, sizeof( bAttendToday ) );
#else
	memcpy( bAttendToday, AttendToday, sizeof( bAttendToday ) );
#endif
	memcpy( &siFirstCreatTimeDateVary , &firstcreatTime , sizeof(_SYSTEMTIME) );	 //KHY - 0906 - 최초 계정 생성 시간.
	clEmpireInfo.SetEmpireInfo( EmpireInfo );
	bSummonExtraSlot		=	SummonExtraSlot			;
	siSummonExtraSlotDate	=	SummonExtraSlotDate	;

	m_uiRealAge = uiRealAge;
}

cltPBasicInfo::~cltPBasicInfo()
{};

void cltPBasicInfo::Set(cltPBasicInfo* pclinfo)
{
	MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );

	siIDNum						= pclinfo->siIDNum;
	uiIndex						= pclinfo->GetIndex();
	uiSex						= pclinfo->uiSex;
	uiKind						= pclinfo->GetKind();
	siHomeVillage				= pclinfo->siHomeVillage;
	uiGRank						= pclinfo->uiGRank;
	uiAngelSwitch				= pclinfo->uiAngelSwitch;
	uiPoliceSwitch				= pclinfo->uiPoliceSwitch;
	clPos.Set(pclinfo->GetPos());
	siPlaySecond				= pclinfo->siPlaySecond;
	siPlayEventSecond			= pclinfo->siPlayEventSecond;
	siSchoolUnique				= pclinfo->siSchoolUnique;
	clLastMainMapPos.Set(&pclinfo->clLastMainMapPos);

	clPerson.Set(&pclinfo->clPerson);
	clFatherInfo.Set(&pclinfo->clFatherInfo);
	clRecommenderInfo.Set(&pclinfo->clRecommenderInfo);
	siVoteKingSelectionIndex	= pclinfo->siVoteKingSelectionIndex;

	siWealthRank				= pclinfo->siWealthRank; 
	clTotalWealthMoney.Set(&pclinfo->clTotalWealthMoney);
	siWealthCalcDay				= pclinfo->siWealthCalcDay;

	bWennyOldManUpgrade			= pclinfo->bWennyOldManUpgrade ;

	bNewAccount					= pclinfo->bNewAccount ;
	bGameEvent					= pclinfo->bGameEvent ;
	bWinPrizeEvent				= pclinfo->bWinPrizeEvent ;

	bSpaceBoxPremiumService 	= pclinfo->bSpaceBoxPremiumService;	//공간상자
	siSpaceBoxPremiumServiceDate= pclinfo->siSpaceBoxPremiumServiceDate; //공간상자

	bUseNoLimitTicketWarp		= pclinfo->bUseNoLimitTicketWarp ;
	siNoLimitTicketWarpDateVary = pclinfo->siNoLimitTicketWarpDateVary;

	uiGMMode					= pclinfo->uiGMMode;
	bPCRoom						= pclinfo->bPCRoom;
	sPCroomGrade				= pclinfo->sPCroomGrade;
	MStrCpy(szCRMcode, pclinfo->szCRMcode ,16);
	bQuestEvolve				= pclinfo->bQuestEvolve ;
	MStrCpy(szGreeting, pclinfo->szGreeting, 128);

	siCountryID					= pclinfo->siCountryID;
	bShowCountry				= pclinfo->bShowCountry;
	
	bShowPuchikomi				= pclinfo->bShowPuchikomi;

	clGuildRank.Set(&pclinfo->clGuildRank);

	bPremiumService				= pclinfo->bPremiumService;
	siPremiumServiceDate		= pclinfo->siPremiumServiceDate;
	bCombatService				= pclinfo->bCombatService;
	siCombatServiceDate			= pclinfo->siCombatServiceDate;
	bMakeItemService			= pclinfo->bMakeItemService;
	siMakeItemServiceDate		= pclinfo->siMakeItemServiceDate;
	bBulletService				= pclinfo->bBulletService;
	siBulletServiceDate			= pclinfo->siBulletServiceDate;
	bSummonStaminaService		= pclinfo->bSummonStaminaService;
	siSummonStaminaServiceDate	= pclinfo->siSummonStaminaServiceDate;
	
	clVillageScoreInfo.Set(&pclinfo->clVillageScoreInfo);
	clBufInfo.SetBuf(&pclinfo->clBufInfo);
	clTimeMeterRateInfo.SetTimeMeterRate(&pclinfo->clTimeMeterRateInfo);

	bReceiveBeginnerWeapon = pclinfo->bReceiveBeginnerWeapon;

	siPVPWinNumber  = pclinfo->siPVPWinNumber;
	siPVPLoseNumber = pclinfo->siPVPLoseNumber;
	if( NULL == pclinfo->szPVPWinHighLevelUserName )
	{
		MStrCpy(szPVPWinHighLevelUserName, "", MAX_PLAYER_NAME);
	}
	else
	{
		MStrCpy(szPVPWinHighLevelUserName, pclinfo->szPVPWinHighLevelUserName, MAX_PLAYER_NAME);
	}

	bMarKetPremiumService			= pclinfo->bMarKetPremiumService;
	siMarKetPremiumServiceDate		= pclinfo->siMarKetPremiumServiceDate;

	bNewMarketPremiumService		= pclinfo->bNewMarketPremiumService;
	siNewMarketPremiumServiceDate	= pclinfo->siNewMarketPremiumServiceDate;
		
	bSummonExtraSlot				= pclinfo->bSummonExtraSlot;
	siSummonExtraSlotDate			= pclinfo->siSummonExtraSlotDate;
	
#ifdef _SAFE_MEMORY
	memcpy( &bAttendToday[0], &pclinfo->bAttendToday[0], sizeof( bAttendToday ) );
#else
	memcpy( bAttendToday, pclinfo->bAttendToday, sizeof( bAttendToday ) );
#endif
	memcpy( &siFirstCreatTimeDateVary, &(pclinfo->siFirstCreatTimeDateVary), sizeof(_SYSTEMTIME) );	 //KHY - 0906 - 최초 계정 생성 시간.
	clEmpireInfo.SetEmpireInfo( &pclinfo->clEmpireInfo );

	m_uiRealAge = pclinfo->m_uiRealAge;
}

void cltPBasicInfo::Init()
{
	szAccountID[ 0 ]	= NULL;
	siIDNum				= 0;
	uiIndex				= 0;
	uiSex				= 0;
	uiKind				= 0;
	siHomeVillage		= 0;
	siPlaySecond		= 0;
	siPlayEventSecond	= 0;
	siSchoolUnique		= 0;
	uiGRank				= 0;
	uiAngelSwitch		= 0;
	uiPoliceSwitch  	= 0;

	clPos.Init();
	clLastMainMapPos.Init();

	clPerson.Init();
	clFatherInfo.Init();
	clRecommenderInfo.Init();
	siVoteKingSelectionIndex = 0;

	siWealthRank			= 0;
	clTotalWealthMoney.Clear();
	siWealthCalcDay			= 0;

	//bTodayFirstAttendanceSwitch	= false;
	//siAttendanceDays			= 0;

	bWennyOldManUpgrade 	= false ;

	bNewAccount				= false ;
	bGameEvent				= false ;
	bWinPrizeEvent			= false ;

	bUseNoLimitTicketWarp	= false ;
	siNoLimitTicketWarpDateVary = 0 ;

	uiGMMode = 0;
	bPCRoom = false;
	sPCroomGrade = 0;
	MStrCpy(szCRMcode, TEXT(""),16);
	bQuestEvolve = false ;
	MStrCpy(szGreeting, TEXT(""),128);

	clGuildRank.Init();
	bPremiumService				= false;
	siPremiumServiceDate		= 0;
	bCombatService				= false;
	siCombatServiceDate			= 0;
	bMakeItemService			= false;
	siMakeItemServiceDate		= 0;
	bBulletService				= false;
	siBulletServiceDate			= 0;
	bSummonStaminaService		= false;
	siSummonStaminaServiceDate	= 0;

	clVillageScoreInfo.Init();

	bReceiveBeginnerWeapon		= 0;
	
	siPVPWinNumber				= 0;
	siPVPLoseNumber				= 0;
	MStrCpy(szPVPWinHighLevelUserName, "", MAX_PLAYER_NAME);

	bMarKetPremiumService			= false;
	siMarKetPremiumServiceDate		= 0;

	bNewMarketPremiumService		= false;
	siNewMarketPremiumServiceDate	= 0;

	bSpaceBoxPremiumService			= false;
	siSpaceBoxPremiumServiceDate	= 0;

#ifdef _SAFE_MEMORY
	bAttendToday.ZeroMem();
#else
	ZeroMemory( bAttendToday, sizeof( bAttendToday ) );
#endif
	bIsAttendTime			= false;
	bIsCanAttend			= false;
	siAttendanceDays		= 0;
	bIsCanReward			= 0;
	ZeroMemory(&siFirstCreatTimeDateVary , sizeof(_SYSTEMTIME) );	 //KHY - 0906 - 최초 계정 생성 시간. 
	clEmpireInfo.SetEmpireInfo( 0, 0 );
	bSummonExtraSlot		= false;
	siSummonExtraSlotDate	=	0	;				// 소환수 확장 티켓 사용날자

	m_uiRealAge				= 0;

}

void cltPBasicInfo::SetIndex(SI32 index)
{
	uiIndex = index;
}

SI32 cltPBasicInfo::GetIndex() const
{
	return uiIndex;
}

void cltPBasicInfo::SetKind(SI32 kind)
{
	uiKind = kind;
}

SI32 cltPBasicInfo::GetKind() const
{
	return uiKind;
}

const cltCharPos* cltPBasicInfo::GetPos() const
{
	return &clPos;
}

void cltPBasicInfo::SetMapIndex(SI32 index)
{
	clPos.SetMapIndex(index);
}

SI32 cltPBasicInfo::GetMapIndex() const
{
	return clPos.GetMapIndex();
}

void cltPBasicInfo::SetX(SI32 x)
{
	clPos.SetX(x);
}

SI32 cltPBasicInfo::GetX() const
{
	return clPos.GetX();
}

void cltPBasicInfo::SetY(SI32 y)
{
	clPos.SetY(y);
}

SI32 cltPBasicInfo::GetY() const
{
	return clPos.GetY();
}

void cltPBasicInfo::SetName(const TCHAR* pname)
{
	MStrCpy( clPerson.szName, pname, MAX_PLAYER_NAME );
}
const TCHAR* cltPBasicInfo::GetName() const
{
	return clPerson.szName;
}

bool cltPBasicInfo::IsQuestEvolve() 
{
	return bQuestEvolve ; 
}

void cltPBasicInfo::SetQuestEvolve(bool questevolve)
{
	bQuestEvolve = questevolve ;
}

void cltPBasicInfo::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	pkGMSectionData->BeginData( "BasicInfo" );
	{
		pkGMSectionData->AddDataInt( "idnum",		siIDNum);
		pkGMSectionData->AddDataStr( "accountid",	szAccountID);
		pkGMSectionData->AddDataInt( "kind",		uiKind);
		pkGMSectionData->AddDataInt( "home",		siHomeVillage);
		pkGMSectionData->AddDataInt( "personid",	clPerson.GetPersonID());
		pkGMSectionData->AddDataStr( "name",		(char*)clPerson.GetName());
		pkGMSectionData->AddDataStr( "guildname",	clGuildRank.szGuildName);
		pkGMSectionData->AddDataInt( "guildvillage",clGuildRank.siVillageUnique);		
	}
	pkGMSectionData->EndData( "BasicInfo" );
}

//====================================
// cltBasicAbility
//====================================
/*cltBasicAbility::cltBasicAbility()
{
	Init();		
}

cltBasicAbility::~cltBasicAbility(){};

void cltBasicAbility::Init()
{
	ZeroMemory(this, sizeof(cltBasicAbility));
}
*/
void cltBasicAbility::Set(SI32 str, SI32 dex, SI32 vit, SI32 mag, SI32 hnd, SI32 luk , SI32 Wis)
{
	siStr	= str;
	siDex	= dex;
	siVit	= vit;
	siMag	= mag;
	siHnd	= hnd;
	siLuk	= luk;
	siWis	= Wis;
}
void cltBasicAbility::Set(cltBasicAbility* pclinfo)
{
	Set(pclinfo->siStr, pclinfo->siDex, pclinfo->siVit, pclinfo->siMag, pclinfo->siHnd, pclinfo->siLuk , pclinfo->siWis);
}

SI32 cltBasicAbility::GetWis() const
{
	return siWis;
}

void cltBasicAbility::SetWis(SI32 dat)
{
	siWis = dat;
}

SI32 cltBasicAbility::IncreaseWis(SI32 amount)
{
	siWis += amount; return siWis;
}

SI32 cltBasicAbility::DecreaseWis(SI32 amount)
{
	siWis -= amount; return siWis;
}

SI32 cltBasicAbility::GetStr() const
{
	return siStr;
}

void cltBasicAbility::SetStr(SI32 dat)
{
	siStr = dat;
}

SI32 cltBasicAbility::IncreaseStr(SI32 amount)
{
	siStr += amount; return siStr;
}

SI32 cltBasicAbility::DecreaseStr(SI32 amount)
{
	siStr -= amount; return siStr;
}

SI32 cltBasicAbility::GetStrDamage( bool Person ) 
{
	 // pvp-
	if( Person)
	{
		return siStr * 2 / 5;
	}
	else
	{
		return siStr / 5;
	}
}

SI32 cltBasicAbility::GetDex() const
{
	return siDex;
}

void cltBasicAbility::SetDex(SI32 dat)
{
	siDex = dat;
}

SI32 cltBasicAbility::IncreaseDex(SI32 amount)
{
	siDex += amount; 
	return siDex;
}
SI32 cltBasicAbility::DecreaseDex(SI32 amount)
{
	siDex -= amount; return siDex;
}

SI32 cltBasicAbility::GetVit() const
{
	return siVit;	
}

void cltBasicAbility::SetVit(SI32 dat)
{
	siVit = dat;
}

SI32 cltBasicAbility::IncreaseVit(SI32 amount)
{
	siVit += amount; 
	return siVit;
}

SI32 cltBasicAbility::DecreaseVit(SI32 amount)
{
	siVit -= amount; 
	return siVit;
}

SI32 cltBasicAbility::GetMag() const
{
	return siMag;	
}

void cltBasicAbility::SetMag(SI32 dat)
{
	siMag = dat;
}

SI32 cltBasicAbility::IncreaseMag(SI32 amount)
{
	siMag += amount; 
	return siMag;
}

SI32 cltBasicAbility::DecreaseMag(SI32 amount)
{
	siMag -= amount; 
	return siMag;
}

SI32 cltBasicAbility::GetHnd() const
{
	return siHnd;	
}

void cltBasicAbility::SetHnd(SI32 dat)
{
	siHnd = dat;
}

SI32 cltBasicAbility::IncreaseHnd(SI32 amount)
{
	siHnd += amount; 
	return siHnd;
}
SI32 cltBasicAbility::DecreaseHnd(SI32 amount)
{
	siHnd -= amount; 
	return siHnd;
}

SI32 cltBasicAbility::GetLuk() const
{
	return siLuk;	
}

void cltBasicAbility::SetLuk(SI32 dat)
{
	siLuk = dat;
}

SI32 cltBasicAbility::IncreaseLuk(SI32 amount)
{
	siLuk += amount; 
	return siLuk;
}
SI32 cltBasicAbility::DecreaseLuk(SI32 amount)
{
	siLuk -= amount; 
	return siLuk;
}

void cltBasicAbility::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	pkGMSectionData->BeginData( "BasicAbility" );
	{
		pkGMSectionData->AddDataInt( "str", siStr );
		pkGMSectionData->AddDataInt( "dex", siDex );
		pkGMSectionData->AddDataInt( "vit", siVit );
		pkGMSectionData->AddDataInt( "mag", siMag );
		pkGMSectionData->AddDataInt( "hnd", siHnd );
		pkGMSectionData->AddDataInt( "luk", siLuk );
		pkGMSectionData->AddDataInt( "wis", siWis );
	}
	pkGMSectionData->EndData( "BasicAbility" );
}


//====================================
// cltInvestInfo
//====================================
cltInvestInfo::cltInvestInfo()
{
	Init();
}

void cltInvestInfo::Set(cltInvestInfo* pinfo)
{
	clPerson.Set(&pinfo->clPerson);
	siStockNum	= pinfo->siStockNum;
}

void cltInvestInfo::Init()
{
	clPerson.Init();
	siStockNum	= 0;
}

SI32 cltInvestInfo::GetStockNum() const
{
	return siStockNum;
}

void cltInvestInfo::SetStockNum(SI32 num)
{
	siStockNum = num;
}

//====================================
// cltVillageInvestInfo
//====================================
cltVillageInvestInfo::cltVillageInvestInfo()
{
	siVillageUnique = 0;
}

void cltVillageInvestInfo::Set(cltVillageInvestInfo* pclMayorInfo)
{
	memcpy(this, pclMayorInfo, sizeof(cltVillageInvestInfo));				//ok
}

//====================================
// cltHealthInfo
//====================================
cltHealthInfo::cltHealthInfo()
{
	Init();
}

void cltHealthInfo::Init()
{
	//------------------------
	// 기초 상태 
	//------------------------
	siHungry		= 0;			// 배고픔.		

	//-------------------------
	// 기관 상태 
	//-------------------------
	siInnerOrg1		= 0;		// 소화계 
	siInnerOrg2		= 0;		// 순환계 
	siInnerOrg3		= 0;		// 호흡계 
	siInnerOrg4		= 0;		// 신경계 

	//------------------------
	// 질병 상태 
	//------------------------
	uiOrg1Disease	= 0;		// 소화계 질병.			
	uiOrg2Disease	= 0;		// 순환계 질병. 
	uiOrg3Disease	= 0;		// 호흡계 질병. 
	uiOrg4Disease	= 0;		// 신경계 질병. 

	//-----------------------
	// 종합 건강 지수.
	//-----------------------
	uiHealthPoint	= 100;
}

void cltHealthInfo::Set(cltHealthInfo* pclinfo)
{
	memcpy(this, pclinfo, sizeof(cltHealthInfo));						//ok
}

void cltHealthInfo::IncreaseInnerOrg1(SI32 amount)
{
	SI32 sum = siInnerOrg1 + amount;

	siInnerOrg1 = max(-100, min(100, sum));
}
void cltHealthInfo::IncreaseInnerOrg2(SI32 amount)
{
	SI32 sum = siInnerOrg2 + amount;

	siInnerOrg2 = max(-100, min(100, sum));
}
void cltHealthInfo::IncreaseInnerOrg3(SI32 amount)
{
	SI32 sum = siInnerOrg3 + amount;

	siInnerOrg3 = max(-100, min(100, sum));
}
void cltHealthInfo::IncreaseInnerOrg4(SI32 amount)
{
	SI32 sum = siInnerOrg4 + amount;

	siInnerOrg4 = max(-100, min(100, sum));
}

// 내장 기관을 손상시킨다. 
bool cltHealthInfo::DecreaseOrg(SI32 type, SI32 amount, SI32 level, bool forceDec)
{

	SI32 max = MAXORG(level);
	bool bfailswitch = false;

	if(rand() % 2)bfailswitch = true;

	//cyj 채광, 농경시에는 무조건 손상시키도록 수정
	if (forceDec)
		bfailswitch = false;

	SI32 olddata;

	switch(type)
	{
	case HEALTH_ORGAN_TYPE_ORG1: 
		if(siInnerOrg1 < 0 && bfailswitch == true)return false;

		olddata = siInnerOrg1;

		IncreaseInnerOrg1(-amount);	
		if(siInnerOrg1 > max)siInnerOrg1	= max;
		if(siInnerOrg1 < -max)siInnerOrg1	= -max;

		if(olddata != siInnerOrg1)return true;
		break;
	case HEALTH_ORGAN_TYPE_ORG2: 
		if(siInnerOrg2 < 0 && bfailswitch == true)return false;

		olddata = siInnerOrg2;

		IncreaseInnerOrg2(-amount);	
		if(siInnerOrg2 > max)siInnerOrg2	= max;
		if(siInnerOrg2 < -max)siInnerOrg2	= -max;

		if(olddata != siInnerOrg2)return true;
		break;	

	case HEALTH_ORGAN_TYPE_ORG3: 
		if(siInnerOrg3 < 0 && bfailswitch == true)return false;

		olddata = siInnerOrg3;

		IncreaseInnerOrg3(-amount);	
		if(siInnerOrg3 > max)siInnerOrg3	= max;
		if(siInnerOrg3 < -max)siInnerOrg3	= -max;

		if(olddata != siInnerOrg3)return true;
		break;

	case HEALTH_ORGAN_TYPE_ORG4:
		if(siInnerOrg4 < 0 && bfailswitch == true)return false;

		olddata = siInnerOrg4;

		IncreaseInnerOrg4(-amount);	
		if(siInnerOrg4 > max)siInnerOrg4	= max;
		if(siInnerOrg4 < -max)siInnerOrg4	= -max;

		if(olddata != siInnerOrg4)return true;
		break;
	}

	return false;
}

// 내장 기관의 상태를 얻어온다. 
SI32 cltHealthInfo::GetOrg(SI32 type)
{
	switch(type)
	{
	case HEALTH_ORGAN_TYPE_ORG1: return siInnerOrg1;
	case HEALTH_ORGAN_TYPE_ORG2: return siInnerOrg2;
	case HEALTH_ORGAN_TYPE_ORG3: return siInnerOrg3;
	case HEALTH_ORGAN_TYPE_ORG4: return siInnerOrg4;
	}

	return 0;
}

void cltHealthInfo::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	pkGMSectionData->BeginData( "HealthInfo" );
	{
		pkGMSectionData->AddDataInt("hungry",			siHungry);
		pkGMSectionData->AddDataInt("innerorg1",		siInnerOrg1);
		pkGMSectionData->AddDataInt("innerorg2",		siInnerOrg2);
		pkGMSectionData->AddDataInt("innerorg3",		siInnerOrg3);
		pkGMSectionData->AddDataInt("innerorg4",		siInnerOrg4);
		pkGMSectionData->AddDataInt("org1disease",		uiOrg1Disease);
		pkGMSectionData->AddDataInt("org2disease",		uiOrg2Disease);
		pkGMSectionData->AddDataInt("org3disease",		uiOrg3Disease);
		pkGMSectionData->AddDataInt("org4disease",		uiOrg4Disease);
		pkGMSectionData->AddDataInt("healthpoint",		uiHealthPoint);
	}
	pkGMSectionData->EndData( "HealthInfo" );
}

//====================================
// cltRentContract
//====================================
void cltRentContract::Set( GMONEY fee, SI32 period)
{
	siRentFee		= fee;
	siRentPeriod	= period;		
}

void cltRentContract::Set( GMONEY Fee, SI32 Period, cltDate *pclDate )
{
	siRentFee = Fee;
	siRentPeriod = Period;
	clDate.Set( pclDate );
}

void cltRentContract::Set(const cltRentContract* pclcontract)
{
	siRentFee		= pclcontract->siRentFee;
	siRentPeriod	= pclcontract->siRentPeriod;
	clDate.Set(&pclcontract->clDate);
}

BOOL cltRentContract::IsValid()
{
	if(siRentFee < 0		|| siRentFee > MAX_RENT_FEE)return FALSE;
	if(siRentPeriod <= 0	|| siRentPeriod > MAX_RENT_PERIOD)return FALSE;

	return TRUE;
}

BOOL cltRentContract::IsSame(const cltRentContract* pclcontract)
{
	if(siRentFee != pclcontract->siRentFee)return FALSE;
	if(siRentPeriod != pclcontract->siRentPeriod)return FALSE;

	return TRUE;
}
// 계약이 종료되었는가?
bool cltRentContract::IsEnd(cltDate* pcldate)
{
	if(clDate.uiYear + siRentPeriod < pcldate->uiYear)return true;
	else if(clDate.uiYear + siRentPeriod == pcldate->uiYear)
	{
		if(clDate.uiMonth	< pcldate->uiMonth)return true;
		else if(clDate.uiMonth	== pcldate->uiMonth)
		{
			if( clDate.uiDay <= pcldate->uiDay)return true;
		}
	}

	return false;
}

void cltRentContract::Init()
{
	siRentFee		= 0;		// 월 임대료. 
	siRentPeriod	= 0;		// 임대기간.
	clDate.Set(0, 0, 0, 0, 0);
}

void cltRentContract::GetRentInfoText( TCHAR *RentInfoText, SI16 txtSize )
{
	if ( RentInfoText == NULL )
	{
		return;
	}

	//TCHAR temp[ 1024 ];
	NTCHARString1024	temp;
	//TCHAR temp1[ 1024 ];
	NTCHARString1024	temp1;
	//TCHAR* pText = GetTxtFromMgr(3510);
	NTCHARString128		Text(GetTxtFromMgr(3510));

	//StringCchPrintf( temp, 1024, pText, siRentFee, siRentPeriod );
	temp.FormatString(Text, siRentFee, siRentPeriod);
	StringCchCat( RentInfoText, txtSize, temp );
	
	//pText = GetTxtFromMgr(3511);
	Text = GetTxtFromMgr(3511);
	if ( clDate.GetDateText( temp ) == TRUE )
	{
		//StringCchPrintf( temp1, 1024,  pText, temp );
		temp1.FormatString(Text, temp);
		StringCchCat( RentInfoText, txtSize, temp1 );
	}
}

//====================================
// cltHorse
//====================================
cltHorse::cltHorse()
{
	Init();
}

void cltHorse::Init()
{
	ZeroMemory(this, sizeof(cltHorse));
}

cltHorse::cltHorse(SI16 unique,  TCHAR* pname, cltDate* pcldate, SI08 sex, SI16 life, SI08 siPregnentPeriod,
				   SI16 str, SI16 dex, SI16 mag, SI16 movepseed, SI32 curhourvary,SI32 bonus,
				   cltDate* pclstatusdate , SI32 ticketnum
				   ,cltDate* pclPremiumPartsdate_Slot1,SI32 PremiumUnique_Slot1
				   ,cltDate* pclPremiumPartsdate_Slot2,SI32 PremiumUnique_Slot2
				   ,cltDate* pclPremiumPartsdate_Slot3,SI32 PremiumUnique_Slot3
				   ,SI32 PremiumPartsNum)
{
	siHorseUnique	= unique;
	MStrCpy( szName, pname, MAX_HORSE_NAME );
	clBirthDate.Set(pcldate);

	siSex				= sex;
	siPregnentPeriod	= siPregnentPeriod;
	siLife				= life;

	siStr				= str;
	siDex				= dex;
	siMag				= mag;
	siMoveSpeed			= movepseed;

	siStatusBonus		= bonus ;

	siLastUpdateSecond	= 0;
	siLastNeedTime		= curhourvary;	
	siDueTime			= 0;
	siNeed				= 0;
	siPara1				= 0;
	siPara2				= 0;
	siSupply			= 0;
	siRate				= 0;

	siRewardType		= 0;
	siRewardAmount		= 0;

	clAddStatusDate.Set(pclstatusdate);
	siStatusTicketNum	= ticketnum;

	clPremiumPartsDate_Slot1.Set(pclPremiumPartsdate_Slot1)	;
	SetPremiumPartsUnique_Slot1(PremiumUnique_Slot1);
	clPremiumPartsDate_Slot2.Set(pclPremiumPartsdate_Slot2)	;
	SetPremiumPartsUnique_Slot2(PremiumUnique_Slot2);
	clPremiumPartsDate_Slot3.Set(pclPremiumPartsdate_Slot3)	;
	SetPremiumPartsUnique_Slot3(PremiumUnique_Slot3);
	siPremiumPartsNum = PremiumPartsNum	;
}

void cltHorse::Set(cltHorse* pclhorse)
{
	if(pclhorse == NULL)		return;
	memcpy(this, pclhorse, sizeof(cltHorse));						//ok
}

// 게임에 적용될 실제 이동 속도를 구한다.
SI16 cltHorse::GetRealMoveSpeed()
{
	return siMoveSpeed/100;
}

// 나이를 구한다. 
SI32 cltHorse::GetAge(SI32 curday)
{
	SI32 birthday	= clBirthDate.GetDateVary();

	if(curday >= birthday)
	{
		SI32 age = (curday - birthday) / 360 + 1;
		return age;
	}
	return 0;
}

// 같은 말인지 여부를 판단한다. 
bool cltHorse::IsSame(cltHorse* pclhorse)
{
	if(pclhorse == NULL)return false;
	if(siHorseUnique	!= pclhorse->siHorseUnique)return false;
	if(siSex			!= pclhorse->siSex)return false;
	if(siLife			!= pclhorse->siLife)return false;
	if(siStr			!= pclhorse->siStr)return false;
	if(siDex			!= pclhorse->siDex)return false;
	if(siMag			!= pclhorse->siMag)return false;
	if(siMoveSpeed		!= pclhorse->siMoveSpeed)return false;
	if(siStatusBonus	!= pclhorse->siStatusBonus)return false;
	if(_tcscmp(szName, pclhorse->szName) != 0)return false;

	return true;
}
//// 프리미엄 파츠를 셋팅
//
SI32 cltHorse::GetPremiumUnique_Slot1(void)
{
	return siPremiumUnique_Slot1;
}
SI32 cltHorse::GetPremiumUnique_Slot2(void)
{
	return siPremiumUnique_Slot2;
}
SI32 cltHorse::GetPremiumUnique_Slot3(void)
{
	return siPremiumUnique_Slot3;
}
cltDate cltHorse::GetPremiumPartsDate_Slot1(void)
{
	return clPremiumPartsDate_Slot1;
}
cltDate cltHorse::GetPremiumPartsDate_Slot2(void)
{
	return clPremiumPartsDate_Slot2;
}
cltDate cltHorse::GetPremiumPartsDate_Slot3(void)
{
	return clPremiumPartsDate_Slot3;
}

void cltHorse::SetPremiumPartsUnique_Slot1( SI32 siUnique )
{
	siPremiumUnique_Slot1 = siUnique	;
}
void cltHorse::SetPremiumPartsUnique_Slot2( SI32 siUnique )
{
	siPremiumUnique_Slot2 = siUnique	;
}
void cltHorse::SetPremiumPartsUnique_Slot3( SI32 siUnique )
{
	siPremiumUnique_Slot3 = siUnique	;
}
//
bool cltHorse::HaveSamePremiumParts( SI32 siItemUnique )
{
	SI32 siSlot1 = GetPremiumUnique_Slot1();
	SI32 siSlot2 = GetPremiumUnique_Slot2();
	SI32 siSlot3 = GetPremiumUnique_Slot3();
	if ( siItemUnique == siSlot1 || 
		siItemUnique == siSlot2 ||
		siItemUnique == siSlot3)	// 3개의 슬롯중에 같은 유니크를 가지고 있는 파츠가 있다면 true
	{
		return true	;
	}
	return false ;
}
void cltHorse::SetPremiumParts(cltDate* pclDate,SI32 siUnique )
{
	if ( siPremiumPartsNum == 3 )	return	;
	switch ( siPremiumPartsNum )
	{
	case 0 :
		{	clPremiumPartsDate_Slot1.Set(pclDate)	;
			SetPremiumPartsUnique_Slot1( siUnique ) ;
			AddPremiumPartsNum(1)	;
		}
		break;
	case 1 :
		{
			clPremiumPartsDate_Slot2.Set(pclDate)	;
			SetPremiumPartsUnique_Slot2( siUnique ) ;
			AddPremiumPartsNum(1)	;
		}
		break;
	case 2 :
		{
			clPremiumPartsDate_Slot3.Set(pclDate)	;
			SetPremiumPartsUnique_Slot3( siUnique ) ;
			AddPremiumPartsNum(1)	;
		}
		break;
	}	
}
SI32 cltHorse::GetPremiumPartsNum(void)
{
	SI32 PremiumPartsNum	= 0	;
	SI32 siSlot1 = GetPremiumUnique_Slot1() ;
	SI32 siSlot2 = GetPremiumUnique_Slot2() ;
	SI32 siSlot3 = GetPremiumUnique_Slot3() ;
	if ( siSlot1 > 0 )	PremiumPartsNum += 1	;
	if ( siSlot2 > 0 )	PremiumPartsNum += 1	;
	if ( siSlot3 > 0 )	PremiumPartsNum += 1	;
	return  PremiumPartsNum ;
}
SI32 cltHorse::GetPremiumPartsNum_Slot1(void)
{
	SI32 itemnum = 0	;
	if ( GetPremiumUnique_Slot1() > 0 )
	{
		itemnum = 1	;
	}
	return itemnum ;
}
SI32 cltHorse::GetPremiumPartsNum_Slot2(void)
{
	SI32 itemnum = 0	;
	if ( GetPremiumUnique_Slot2() > 0 )
	{
		itemnum = 1	;
	}
	return itemnum ;
}
SI32 cltHorse::GetPremiumPartsNum_Slot3(void)
{
	SI32 itemnum = 0	;
	if ( GetPremiumUnique_Slot2() > 0 )
	{
		itemnum = 1	;
	}
	return itemnum ;
}
void cltHorse::AddPremiumPartsNum( SI32 siNum)
{
	siPremiumPartsNum = siPremiumPartsNum + siNum	;
}
//====================================
// cltAttackTarget
//====================================
cltAttackTarget::cltAttackTarget()
{
	siAttackID	= 0;
	clPos.Init();
	siHitRate	= 0;
	bPVPHit = false;
}

cltAttackTarget::cltAttackTarget( SI32 attackid, const cltCharPos* pclpos, SI32 hitrate,SI32 DodgeRate, bool PVPHit )
{
	siAttackID	= attackid;
	clPos.Set(pclpos);

	siHitRate	= hitrate;
	siDodgeRate	= DodgeRate;

	bPVPHit = PVPHit;
}

void cltAttackTarget::Set( const cltAttackTarget* pcltarget )
{
	siAttackID	= pcltarget->siAttackID;
	clPos.Set(&pcltarget->clPos);

	siHitRate	= pcltarget->siHitRate;
	siDodgeRate		 = pcltarget->siDodgeRate;

	bPVPHit = pcltarget->bPVPHit;
}

void cltAttackTarget::Set( SI32 attackid, const cltCharPos* pclpos, SI32 hitrate, SI32 DodgeRate, bool PVPHit )
{
	siAttackID	= attackid;
	clPos.Set(pclpos);

	siHitRate	= hitrate;
	siDodgeRate	= DodgeRate;

	bPVPHit = PVPHit;
}

void cltAttackTarget::Init()
{
	siAttackID	= 0;
	clPos.Init();
	siHitRate	= 0;
	siDodgeRate = 0;
	bPVPHit = false;
}

void cltAttackTarget::SetAttackID(SI32 id)
{
	siAttackID	= id;
}

SI32 cltAttackTarget::GetAttackID() const
{
	return siAttackID;
}

//====================================
// cltGuiltyInfo
//====================================
cltGuiltyInfo::cltGuiltyInfo()
{
	Init();
}
void cltGuiltyInfo::Init()
{
	siTotalPrisonSecond = 0;
	siLeftPrisonSecond	= 0;
	szPrisonReason[ 0 ] = NULL;
	siTotalPrisonNum = 0 ;
}
void cltGuiltyInfo::Set(cltGuiltyInfo* pclinfo)
{
	siTotalPrisonSecond = pclinfo->siTotalPrisonSecond;
	siLeftPrisonSecond	= pclinfo->siLeftPrisonSecond;

	MStrCpy( szPrisonReason, pclinfo->szPrisonReason, MAX_PRISON_REASON_SIZE );

	siTotalPrisonNum = pclinfo->siTotalPrisonNum ;
}
void cltGuiltyInfo::SetTotalPrisonSecond(SI32 second)
{
	siTotalPrisonSecond = second ;
}
void cltGuiltyInfo::SetLeftPrisonSecond(SI32 second)
{
	siLeftPrisonSecond = second ;
}
void cltGuiltyInfo::SetPrisonReason( TCHAR * reason)
{
	MStrCpy(szPrisonReason, reason, MAX_PRISON_REASON_SIZE );
}
void cltGuiltyInfo::SetPrisonNumber(SI32 num)
{
	siTotalPrisonNum = num ;
}

//====================================
// cltHuntStg
//====================================
void cltHuntStg::Set(cltHuntStg* pclstg)
{
	siItemUnique	= pclstg->siItemUnique;
	siItemNum		= pclstg->siItemNum;
}

//====================================
// cltHuntLineUp
//====================================
void cltHuntLineUp::Set(cltHuntLineUp* pclinfo)
{
	siLineUpKind		= pclinfo->siLineUpKind;
	siLineUpKindNum		= pclinfo->siLineUpKindNum;
	siLineUpKindLevel	= pclinfo->siLineUpKindLevel;
}

//====================================
// cltFeastStg
//====================================
void cltFeastStg::Set( cltFeastStg* pclstg )
{
	siItemUnique	= pclstg->siItemUnique;
	siItemNum		= pclstg->siItemNum;
}

//====================================
// cltFatherApplyerInfo
//====================================
cltFatherApplyerInfo::cltFatherApplyerInfo()
{
	Init();
}

cltFatherApplyerInfo::cltFatherApplyerInfo(SI16 charunique, SI32 personid,  TCHAR* pname,  TCHAR* paccountid, SI16 level, SI16 famelevel, bool bangel)
{
	siCharUnique	= charunique;
	siPersonID		= personid;
	MStrCpy(szName,		pname, MAX_PLAYER_NAME);
	MStrCpy(szAccountID, paccountid, MAX_PLAYER_NAME);

	siLevel			= level;
	siFameLevel		= famelevel;
	bAngelSwitch	= bangel;
}

void cltFatherApplyerInfo::Init()
{
	siCharUnique	= 0;
	siPersonID		= 0;
	szName[ 0 ] = NULL;
	szAccountID[ 0 ] = NULL;

	siLevel			= 0;
	siFameLevel		= 0;
	bAngelSwitch	= false;
}

void cltFatherApplyerInfo::Set(cltFatherApplyerInfo* pclinfo)
{
	siCharUnique	= pclinfo->siCharUnique;
	siPersonID		= pclinfo->siPersonID;
	MStrCpy(szName, pclinfo->szName, MAX_PLAYER_NAME);

	MStrCpy(szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME);

	siLevel			= pclinfo->siLevel;
	siFameLevel		= pclinfo->siFameLevel;
	bAngelSwitch	= pclinfo->bAngelSwitch;
}

//====================================
// cltChildInfo
//====================================
cltChildInfo::cltChildInfo()
{
	siLevel = 0;
	szAccountID[ 0 ] = NULL;
}
cltChildInfo::cltChildInfo(cltSimplePerson* pclperson, SI16 level, TCHAR * accountID)
{
	clPerson.Set(pclperson);
	siLevel	= level;
	MStrCpy(szAccountID,accountID,MAX_PLAYER_NAME);
}
void cltChildInfo::Init()
{
	clPerson.Init();
	siLevel = 0;
	szAccountID[ 0 ] = NULL;
}
void cltChildInfo::Set(cltChildInfo* pclchild)
{
	clPerson.Set(&pclchild->clPerson);
	siLevel	= pclchild->siLevel;
	MStrCpy(szAccountID,pclchild->szAccountID,MAX_PLAYER_NAME);
}

//====================================
// cltPFriendInfo
//====================================
void cltPFriendInfo::Set(cltPFriendInfo* pclinfo)
{
	SI32 i;

	for(i = 0;i < MAX_FRIEND_GROUP;i++)
	{
		MStrCpy(szGroupName[i],pclinfo->szGroupName[i],MAX_FRIEND_GROUPNAME);
	}
	for(i = 0;i < MAX_FRIEND_NUMBER;i++)
	{
		clFriend[i].Set(&pclinfo->clFriend[i]);
		MStrCpy(szAccountID[i],pclinfo->szAccountID[i],MAX_PLAYER_NAME);
	}
}

//====================================
// cltFriendInfo
//====================================
void cltFriendInfo::Init()
{
//	for( SI16 i = 0; i < MAX_FRIEND_GROUP; ++i )
//	{
//		szGroupName[i][0] = NULL;
//	}
	for( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		siGroupID[ i ] = 0;
		memset( &clFriend[ i ], 0, sizeof( cltSimplePerson ) );
		siCharID[ i ] = 0;
		bConnectStatus[ i ] = false;
		szAccountID[ i ][ 0 ] = NULL;
	}
}

void cltFriendInfo::Init( SI16 Index )
{
	if(Index < 0 || Index >= MAX_FRIEND_NUMBER)		return;

	siGroupID[ Index ] = 0;
	memset( &clFriend[ Index ], 0, sizeof( cltSimplePerson ) );
	siCharID[ Index ] = 0;
	bConnectStatus[ Index ] = false;
	szAccountID[ Index ][ 0 ] = NULL;
}

void cltFriendInfo::Set( SI16 Index, SI08 GroupID, SI32 PersonID, SI32 CharID,  TCHAR *CharName, bool ConnectStatus , TCHAR *AccountID)
{
	if(Index < 0 || Index >= MAX_FRIEND_NUMBER)		return;

	siCharID[ Index ] = CharID;
	bConnectStatus[ Index ] = ConnectStatus;
	siGroupID[ Index ] = GroupID;
	clFriend[ Index ].Set( PersonID, CharName );
	MStrCpy(szAccountID[Index],AccountID,MAX_PLAYER_NAME);
}

void cltFriendInfo::Set( SI16 Index, SI08 GroupID, SI32 CharID, bool ConnectStatus , TCHAR * AccountID)
{
	if(Index < 0 || Index >= MAX_FRIEND_NUMBER)		return;

	siGroupID[ Index ] = GroupID;
	siCharID[ Index ] = CharID;
	bConnectStatus[ Index ] = ConnectStatus;
	MStrCpy(szAccountID[Index],AccountID, MAX_PLAYER_NAME);
}

void cltFriendInfo::Set( cltPFriendInfo *pclInfo )
{
	SI32 i;

	for(i = 0;i < MAX_FRIEND_GROUP; i++)
		MStrCpy(szGroupName[i], pclInfo->szGroupName[i], MAX_FRIEND_GROUPNAME);

	for(i = 0;i < MAX_FRIEND_NUMBER;i++)
	{
		siGroupID[i] = pclInfo->siGroupID[i];
		clFriend[i].Set(&pclInfo->clFriend[i]);
	}
}

void cltFriendInfo::Set( cltFriendInfo *pclInfo )
{
	memcpy( this, pclInfo, sizeof( cltFriendInfo) );					//ok
}

SI16 cltFriendInfo::GetFriendNum()
{
	SI16 Counter = 0;
	for( SI32 i = 0; i < MAX_FRIEND_NUMBER; ++i )
	{
		if ( clFriend[ i ].GetPersonID() > 0 )
		{
			++Counter;
		}
	}

	return Counter;
}
//====================================
// cltPCharOptionInfo
//====================================

cltPCharOptionInfo::cltPCharOptionInfo()
{
	Init();
}
void cltPCharOptionInfo::Init()
{
	memset( this, 0, sizeof( cltPCharOptionInfo ) );
}
// 차후 값이 추가 될경우 변수만 추가 해주는 방식으로 한다.
// 현제 채팅창의 해당 메세지를 보여주지 않는 것을 선택하는 방식이라 기본은 false
void cltPCharOptionInfo:: Set( bool ChatSetNormal, bool ChatSetVillage, bool ChatSetGuild, bool ChatSetParty, bool ChatSetSystem, bool pvpmode )
{
	bChatSetNormal	=	ChatSetNormal ;

	bChatSetVillage	=	ChatSetVillage ;
	bChatSetGuild	=	ChatSetGuild ;
	bChatSetParty	=	ChatSetParty ;
	bChatSetSystem	=	ChatSetSystem ;
	bPvpMode		=	pvpmode	;		// pvp를 거부할건지를 셋팅 한다.
}
void cltPCharOptionInfo::Set( cltPCharOptionInfo *pclInfo )
{
	memcpy( this, pclInfo, sizeof( cltPCharOptionInfo) );
}
void cltPCharOptionInfo:: SetChatSetNormal( bool SetNormalChat )
{
	bChatSetNormal	=	SetNormalChat ;
}
void cltPCharOptionInfo:: SetChatSetVillage( bool SetVillageChat )
{
	bChatSetVillage	=	SetVillageChat ;
}
void cltPCharOptionInfo:: SetChatSetGuild( bool SetGuildChat )
{
	bChatSetGuild	=	SetGuildChat ;
}
void cltPCharOptionInfo:: SetChatSetParty( bool SetPartyChat )
{
	bChatSetParty	=	SetPartyChat ;
}
void cltPCharOptionInfo:: SetChatSetSystem( bool SetSystemChat )
{
	bChatSetSystem	=	SetSystemChat ;
}
void cltPCharOptionInfo:: SetPVPMode( bool pvpmode )
{
	bPvpMode	=	pvpmode ;
}
bool cltPCharOptionInfo:: GetPVPMode()
{
	return bPvpMode	;
}


//====================================
// cltPDailyQuestInfo
//====================================
cltPDailyQuestInfo::cltPDailyQuestInfo()
{
	Init();
}

void cltPDailyQuestInfo::Init()
{
	memset( this, 0, sizeof( cltPDailyQuestInfo ) );
}

void cltPDailyQuestInfo::Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 QuestTime, SI32 LeftTime, SI16 KillMonsterNum, BOOL MoneySelected )
{
	siMonsterKind = MonsterKind;
	siMonsterNum = MonsterNum;

	siRewardItemUnique = RewardItemUnique;
	siRewardItemNum = RewardItemNum;
	siRewardMoney = RewardMoney;
	siRewardExp = RewardExp;

	siFee = Fee;

	siQuestTime = QuestTime;
	siLeftTime = LeftTime;

	siKillMonsterNum = KillMonsterNum;

	bMoneySelected = MoneySelected;
}

void cltPDailyQuestInfo::Set( cltPDailyQuestInfo *pclInfo )
{
	memcpy( this, pclInfo, sizeof( cltPDailyQuestInfo) );					//ok
}

BOOL cltPDailyQuestInfo::IsSame(cltPDailyQuestInfo *pclInfo )
{
	if(siMonsterKind != pclInfo->siMonsterKind  ||
		siMonsterNum != pclInfo->siMonsterNum  ||
		siRewardItemUnique != pclInfo->siRewardItemUnique  ||
		siRewardItemNum != pclInfo->siRewardItemNum  ||
		siRewardExp != pclInfo->siRewardExp  ||
		siFee != pclInfo->siFee  ||
		siQuestTime != pclInfo->siQuestTime ||
		siLeftTime != pclInfo->siLeftTime  ||
		siKillMonsterNum != pclInfo->siKillMonsterNum )
	{
		return FALSE;
	}
	return TRUE;
}

//====================================
// cltPDailyQuest2Info 데일리퀘스트2
//====================================
cltPDailyQuest2Info::cltPDailyQuest2Info()
{
	Init();
}

void cltPDailyQuest2Info::Init()
{
	memset( this, 0, sizeof( cltPDailyQuest2Info ) );
}

void cltPDailyQuest2Info::Set( SI16 MonsterKind, SI16 MonsterNum, SI16 RewardItemUnique, SI16 RewardItemNum, GMONEY RewardMoney, SI32 RewardExp, GMONEY Fee, SI32 QuestTime, SI32 LeftTime, SI16 KillMonsterNum, BOOL MoneySelected, bool HiddenQuest )
{
	siMonsterKind = MonsterKind;
	siMonsterNum = MonsterNum;

	siRewardItemUnique = RewardItemUnique;
	siRewardItemNum = RewardItemNum;
	siRewardMoney = RewardMoney;
	siRewardExp = RewardExp;

	siFee = Fee;

	siQuestTime = QuestTime;
	siLeftTime = LeftTime;

	siKillMonsterNum = KillMonsterNum;

	bMoneySelected = MoneySelected;
	bHiddenQuest = HiddenQuest;


}

void cltPDailyQuest2Info::Set( cltPDailyQuest2Info *pclInfo )
{
	memcpy( this, pclInfo, sizeof( cltPDailyQuest2Info) );					//ok
}

BOOL cltPDailyQuest2Info::IsSame(cltPDailyQuest2Info *pclInfo )
{
	if(siMonsterKind != pclInfo->siMonsterKind  ||
		siMonsterNum != pclInfo->siMonsterNum  ||
		siRewardItemUnique != pclInfo->siRewardItemUnique  ||
		siRewardItemNum != pclInfo->siRewardItemNum  ||
		siRewardExp != pclInfo->siRewardExp  ||
		siFee != pclInfo->siFee  ||
		siQuestTime != pclInfo->siQuestTime ||
		siLeftTime != pclInfo->siLeftTime  ||
		siKillMonsterNum != pclInfo->siKillMonsterNum )
	{
		return FALSE;
	}
	return TRUE;
}

//====================================
// cltPDailyQuest3Info 데일리퀘스트3
//====================================
cltPDailyQuest3Info::cltPDailyQuest3Info()
{
	Init();
}

void cltPDailyQuest3Info::Init()
{
	memset( this, 0, sizeof( cltPDailyQuest3Info ) );
}

void cltPDailyQuest3Info::Set( SI32 ItemUnique, SI32 ItemNum, SI32 RewardItemUnique, SI32 RewardItemNum, SI32 RewardExp, GMONEY Fee, SI32 QuestTime, SI32 LeftTime, SI16 MadeItemNum, BOOL MoneySelected, bool HiddenQuest )
{
	siItemUnique = ItemUnique;
	siItemNum = ItemNum;

	siRewardItemUnique = RewardItemUnique;
	siRewardItemNum = RewardItemNum;
	siRewardExp = RewardExp;

	siFee = Fee;

	siQuestTime = QuestTime;
	siLeftTime = LeftTime;

	siMadeItemNum = MadeItemNum;

	bMoneySelected = MoneySelected;
	bHiddenQuest = HiddenQuest;


}

void cltPDailyQuest3Info::Set( cltPDailyQuest3Info *pclInfo )
{
	memcpy( this, pclInfo, sizeof( cltPDailyQuest3Info) );					//ok
}

BOOL cltPDailyQuest3Info::IsSame(cltPDailyQuest3Info *pclInfo )
{
	if(siItemUnique != pclInfo->siItemUnique  ||
		siItemNum != pclInfo->siItemNum  ||
		siRewardItemUnique != pclInfo->siRewardItemUnique  ||
		siRewardItemNum != pclInfo->siRewardItemNum  ||
		siRewardExp != pclInfo->siRewardExp  ||
		siFee != pclInfo->siFee  ||
		siQuestTime != pclInfo->siQuestTime ||
		siLeftTime != pclInfo->siLeftTime  ||
		siMadeItemNum != pclInfo->siMadeItemNum )
	{
		return FALSE;
	}
	return TRUE;
}
//====================================
// cltMaterialInfo
//====================================
cltMaterialInfo::cltMaterialInfo()
{
	siUnique	= 0;
	siNeedNum	= 0;
}

cltMaterialInfo::cltMaterialInfo(SI32 unique, SI32 itemnum)
{
	siUnique	= unique;
	siNeedNum	= itemnum;
}

void cltMaterialInfo::Set(cltMaterialInfo* pclmaterial)
{
	siUnique	= pclmaterial->siUnique;
	siNeedNum	= pclmaterial->siNeedNum;
}
void cltMaterialInfo::Set(SI32 unique, SI32 itemnum)
{
	siUnique	= unique;
	siNeedNum	= itemnum;
}

//====================================
// cltToolInfo
//====================================
cltToolInfo::cltToolInfo()
{
	siUnique	= 0;
}

cltToolInfo::cltToolInfo(SI32 unique)
{
	siUnique	= unique;
}

void cltToolInfo::Set(cltToolInfo* pcltool)
{
	siUnique	= pcltool->siUnique;
}
void cltToolInfo::Set(SI32 unique)
{
	siUnique	= unique;
}

//KHY - 1220 - 변신시스템.
//==================================================
// cltTransFormInfo
//==================================================
cltTransFormInfo::cltTransFormInfo()
{
	Init();
}

void cltTransFormInfo::Init()
{
	ZeroMemory(this, sizeof(cltTransFormInfo));
}


// 변신 캐릭터를 셋팅한다.
bool cltTransFormInfo::SetTransFormChar(UI16 uTransFormChar)
{
	if(uTransFormChar <= 0)
		return false;

	for(SI32 i=0;i<MAX_TRANSFORM_CHAR;i++)
	{	
		if(clTransForm[i].uiCanTransFormKind == uTransFormChar) //이미 등록된 캐릭터다.
			return false;

		if(clTransForm[i].uiCanTransFormKind <= 0)
		{
			clTransForm[i].uiCanTransFormKind = uTransFormChar;
			return true;
		}
	}

	return false;
}
bool cltTransFormInfo::SearchTransFormChar(UI16 uTransFormChar)
{
	if(uTransFormChar <= 0)
		return false;

	for(SI32 i=0;i<MAX_TRANSFORM_CHAR;i++)
	{	
		if(clTransForm[i].uiCanTransFormKind == uTransFormChar) //이미 등록된 캐릭터다.
			return true;
	}

	return false; //등록된 캐릭터가 없다.
}

SI32 cltTransFormInfo::AddTransFormTime(UI16 uiTransformChar, SI32 sAddSecond)
{
	for(SI32 i=0;i<MAX_TRANSFORM_CHAR;i++)
	{	
		if(clTransForm[i].uiCanTransFormKind == uiTransformChar)
		{
			clTransForm[i].siUseTotalSecond += sAddSecond;
			return clTransForm[i].siUseTotalSecond;
		}
	}
	return 0;
}

// 변신 가능여부를 확인한다.
bool cltTransFormInfo::CanTransForm(UI16 uTransFormChar)
{
	if(uTransFormChar <= 0)
		return 0;

	// 변신 캐릭터가 추가되어있는가?
	for(SI32 i=0;i<MAX_TRANSFORM_CHAR;i++)
	{	
		if(clTransForm[i].uiCanTransFormKind == uTransFormChar) // 캐릭터 확인.
		{
			return true;
		}
	}

	return false;
}
#define MAX_SYNCHRO_TIME 2851200

// 해당 타입의 캐릭터와의 싱크로율을 계산한다.
REAL32 cltTransFormInfo::GetSynchroTransForm(UI16 uTransFormChar)			
{
	REAL32 SynchroPercent = 0;

	if(uTransFormChar <= 0)
		return 0;

	for(SI32 i=0;i<MAX_TRANSFORM_CHAR;i++)
	{	
		if(clTransForm[i].uiCanTransFormKind == uTransFormChar) // 캐릭터 확인.
		{
			SI32	sUsedTime = clTransForm[i].siUseTotalSecond;
			SI32 tempTime = min(MAX_SYNCHRO_TIME, sUsedTime);	
			SynchroPercent = (REAL32)tempTime/ (REAL32)MAX_SYNCHRO_TIME * 100.0f;

			return min(100.0f, SynchroPercent);
		}
	}
	
	return SynchroPercent;
}


