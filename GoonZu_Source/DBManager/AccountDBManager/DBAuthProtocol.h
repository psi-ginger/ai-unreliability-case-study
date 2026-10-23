#ifndef _DBAUTHPROTOCOL_H
#define _DBAUTHPROTOCOL_H

#include "AccountDB.h"
#include "ITZNetworkThread.h"

class DBAuthProtocol : public ITZNetworkThread
{
public:
	DBAuthProtocol();
	~DBAuthProtocol();

public:
	void			Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW );

	virtual void	Update();
	virtual void	SessionProc( ITZSession *pSession );

private:
	ITZSessionFactory< cAuth >	m_SF;
	
//#ifdef USE_DBGW
//	DBGW						m_odbc;
//#else
	RichODBC					m_odbc;
//#endif
	char						m_szSPName[ 512 ];

	void Convert_SYSTEMTIME_from_TIMESTAMP(_SYSTEMTIME *stST, TIMESTAMP_STRUCT *stTS);
	void Convert_TIMESTAMP_from_SYSTEMTIME(TIMESTAMP_STRUCT *stTS, _SYSTEMTIME *stST);

public:
	// 지표 생성 함수
	void Statistics_UserCount(SYSTEMTIME stTime);			// 1. 동접보고
	void Statistics_UserInfo(SYSTEMTIME stTime);			// 2. 이용자 지수
	void Statistics_TotalUserLevel(SYSTEMTIME stTime);		// 3. 전체 이용자 레벨별 분포
	void Statistics_WeekUserLevel(SYSTEMTIME stTime);		// 4. 주간 이용자 레벨별 분포
	void Statistics_NewUserLevel(SYSTEMTIME stTime);		// 5. 신규고객 레벨별 분포
	void Statistics_ActiveUserLevel(SYSTEMTIME stTime);		// 6. 액티브이용자 레벨별 분포
	void Statistics_TotalUserAgeGender(SYSTEMTIME stTime);	// 7. 전체 이용자 성별 및 연령 구분
	void Statistics_WeekUserAgeGender(SYSTEMTIME stTime);	// 8. 주간 이용자 성별 및 연령 구분
	void Statistics_NewUserAgeGender(SYSTEMTIME stTime);	// 9. 신규 고객 성별 및 연령 구분
	void Statistics_ActiveUserAgeGender(SYSTEMTIME stTime);	// 10. 액티브 이용자 성별 및 연령 구분
};


#endif
