#ifndef _AUTHFRONTPROTOCOL_H
#define _AUTHFRONTPROTOCOL_H

#include "AuthServer.h"
#include "ITZNetworkThread.h"
//KHY - 0218 - 중국 NHN 피로도 모듈 적용
#ifdef USE_HANANTITOXIC
	#define MAX_SID_BUF 64
#endif

class AuthFrontProtocol : public ITZNetworkThread
{
public:
	AuthFrontProtocol();
	~AuthFrontProtocol();

public:
	void			Init(UI16 usFrontServerListenPort);

	virtual void	Update();
	virtual void	LogOut( ITZSession *pSession );
	virtual void	SessionProc( ITZSession *pSession );

public:
	ITZSessionFactory< cFront >	m_SF;
};


//---------------------------------------------------------
//KHY - 0617 - 일본 채널링 추가. - 감마니아용 인증 함수.
/*int HttpsPostAuth(TCHAR *pszID, TCHAR *pszPW);	// For 감마니아

int SoapAuth(TCHAR *pszID, TCHAR *pszPW ,  TCHAR *puserIP);	// For GamePot
int GamePotAuth(char* pszID, char* pszPW, char* pszIP);*/

void DeltPublisherID(TCHAR * Input_userid ,TCHAR * Output_userid);

bool SSNCreate(SI08 siAge, SI08 siSex, TCHAR* pszRet, UI32 uiRetSize);		// 주민등록 생성함수

// 일본 넥슨 용 파싱함수.
void NexonjpIDDataDivision(sRequest_NEXONJP_FrontAuth *FfrontAuth , sDBRequest_IDNumGet *FsendMsg);

#endif
