#include "./CMabriumBattleRoyalStadium.h"
#include "../MsgType.h"
#include "../../Server/Server.h"

CMabriumBattleRoyalStadiumServer::CMabriumBattleRoyalStadiumServer()
{
	m_siMapIndex = MAPINDEX_BATTLEROYAL_MABRIUM;

	m_siKindBlackarmySword = pclClient->GetUniqueFromHash("KIND_BattleRoyalSword");
	m_siKindBlackarmyRifle = pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle");
	m_siKindBlackarmyKatar = pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar");
}
CMabriumBattleRoyalStadiumServer::~CMabriumBattleRoyalStadiumServer()
{
	
}

// 배틀로얄 대기를 알린다.
BOOL	CMabriumBattleRoyalStadiumServer::NoticeWait()
{
	return Notice(GetTxtFromMgr(30400)); 
}

// 경기장이 열림을 알린다.
BOOL	CMabriumBattleRoyalStadiumServer::NoticeOpen()
{
	return Notice(GetTxtFromMgr(30399)); 
}

// 경기장 입구를 연다.
void	CMabriumBattleRoyalStadiumServer::OpenGate()
{
	CBattleRoyalStadium::OpenGate();

	//cltMsg clMassage(GAMEMSG_RESPONSE_BATTLEROYAL_OPENSTADIUM, 0 );
	//m_pclServer->pclCM->SendAllPersonMsg((sPacketHeader*)&clMassage);
}

//======================================================================================================================

CMabriumBattleRoyalStadiumClient::CMabriumBattleRoyalStadiumClient()
{
	m_siMapIndex = MAPINDEX_BATTLEROYAL_MABRIUM;

	m_siKindBlackarmySword = pclClient->GetUniqueFromHash("KIND_BattleRoyalSword");
	m_siKindBlackarmyRifle = pclClient->GetUniqueFromHash("KIND_BattleRoyalRifle");
	m_siKindBlackarmyKatar = pclClient->GetUniqueFromHash("KIND_BattleRoyalKatar");
}
CMabriumBattleRoyalStadiumClient::~CMabriumBattleRoyalStadiumClient()
{

}