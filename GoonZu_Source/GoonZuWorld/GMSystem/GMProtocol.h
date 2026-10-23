#include "ITZPerson.h"
#include "ITZNetworkThread.h"
#include "GMSectionData.h"
#include "../common/CommonHeader.h"

class CGMProtocol_Updata;



class cGMClient : public ITZPerson
{
protected:
	TCHAR	m_szAccountID[MAX_PLAYER_NAME];
	SI32	m_siMasterLevel;
	
	
public:
	cGMClient();
	~cGMClient() {};

public:
	virtual void Init();
	virtual void Close(); 


	TCHAR	*GetID() { return m_szAccountID; }
	SI32	GetMasterLevel() { return m_siMasterLevel; }

	void	SetAccount( TCHAR* pszAccountID, SI32 siMasterLevel );

	
};

/*
class cGMClient : public ITZPerson
{
	
protected:
	TCHAR	m_szAccountID[MAX_PLAYER_NAME];
	SI32	m_siMasterLevel;


public:
	cGMClient() {};
	~cGMClient() {};

public:
	virtual void Init() { printf( "GMClient 立加\n" ); }
	virtual void Close() { printf( "GMClient 立加\n" ); }//{ SetAccount( TEXT(""), 0 ); }

};
*/


class GMProtocol : public ITZNetworkThread
{
public:
	GMProtocol();
	~GMProtocol();

public:
	void			Init( UI16 usPort );

	void			GMLock();
	void			GMUnlock();

public:
	void			SendPacket( SI32 siCharID, sPacketHeader* pPacket );
	void			SendPacket( ITZSession* pSession, sPacketHeader* pPacket );
	cGMClient		*GetGMClient( int nCharID ) { return m_SF.GetPerson( nCharID ); }

	bool			SendInfofromServer( ITZSession* pSession , const char* pszFormat, ...);

protected:
	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );
	
	void			Do_GMMSG_REQUEST_LOGIN(ITZSession* pSession);
	void			Do_GMMSG_REQUEST_PAGE( ITZSession* pSession);
	void			Do_GMMSG_REQUEST_UPDATEDATA( ITZSession* pSession);

	void			Do_GMMSG_REQUEST_GMCOMMAND(ITZSession* pSession);

	void			Return_UpData_Processed_Message( ITZSession* pSession, SI32 siReturnValue );

	void			WriteGMToolLog( ITZSession* pSession, SI32 siReturnValue );

	
	ITZSessionFactory< cGMClient >	m_SF;

//	ITZSession*		m_pCurrentSession;
	sPacketHeader*	m_pCurrentPacket;

	GMSectionData*	m_pSectionData;

	CRITICAL_SECTION m_GMcs;

	CGMProtocol_Updata* m_pGMProtocol_Updata;
};
