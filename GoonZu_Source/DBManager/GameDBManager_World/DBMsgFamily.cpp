#include "DBGameProtocol.h"
#include "DBMsg-Family.h"
#include "DBMsg.h"

#include "../../common/CommonHeader.h"
#include "../../common/Char/CharCommon/Char-Common.h"
// 가족 시스템

void DBGameProtocol::Add_Family_Children()
{
	// 받은 패킷
	sDBRequest_Family_Add_Children	*pRecvMsg = (sDBRequest_Family_Add_Children*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Family_Add_Children	sendMsg;

	ZeroMemory(&sendMsg,sizeof(sendMsg))	;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_FAMILY_ADD_CHILDREN;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	SI32 siParent_PersonID ;
	TCHAR szParentCharName[MAX_PLAYER_NAME]= TEXT("");

	SI32 siChildren_PersonID ;
	TCHAR szChildCharName[MAX_PLAYER_NAME]= TEXT("");
	SI32 siDateVary	=	0;
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	SI32 siCookieEndTime	=	0	;	// 쿠키 종료 타임
	SI32 siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	SI32 siCookieLevel		=	0	;	// 쿠키 레벨
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.

	TIMESTAMP_STRUCT stSaveTS;	//-> 내가 저장하는놈
	ZeroMemory(&stSaveTS, sizeof(stSaveTS));	
	TIMESTAMP_STRUCT stRevTS;	//-> db로부터 받을 것
	ZeroMemory(&stRevTS, sizeof(stRevTS));

	Convert_TIMESTAMP_from_SYSTEMTIME( &stSaveTS ,&pRecvMsg->m_stCurrentTime ) 	;

	// DB Query
	BEGIN_SP( "rsp_FamilyAddChild" )
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siParentPersonID, sizeof(pRecvMsg->m_siParentPersonID) );		// 추가하는 부모
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siChildrenPersonID, sizeof(pRecvMsg->m_siChildrenPersonID) );	// 가입하는 자식
	m_odbc.SetParam( SQL_TIMESTAMP, &stSaveTS, sizeof(stSaveTS) );				// 추가된 시간.(가족이팩트 종료 datevary)
	END_SP()

	EXEC_SQL()
	BEGIN_FETCHDATA()
	// 부모 셋팅
	m_odbc.GetData( &siParent_PersonID);
	m_odbc.GetData( szParentCharName,sizeof(szParentCharName));
	//------------------------------------------------------------
	sendMsg.m_cFamily.SetParent(siParent_PersonID,szParentCharName)	;
	END_FETCHDATA()
	BEGIN_NEXTRECORDSET()
	// 자식 셋팅 
	BEGIN_FETCHDATA()
	m_odbc.GetData( &siChildren_PersonID);
	m_odbc.GetData( szChildCharName ,sizeof(szChildCharName));
	m_odbc.GetData( &stRevTS );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.-> DB작업을 안해서 주석
	siCookieEndTime	=	0	;	// 쿠키 종료 타임
	siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	siCookieLevel		=	0	;	// 쿠키 레벨
	m_odbc.GetData( &siCookieLevel );
	m_odbc.GetData( &siCookieLimitTime );
	m_odbc.GetData( &siCookieEndTime );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	//------------------------------------------------------------
	SYSTEMTIME stRevTime	;
	Convert_SYSTEMTIME_from_TIMESTAMP(&stRevTime, &stRevTS );
	sendMsg.m_cFamily.SetChildren(siChildren_PersonID, szChildCharName, stRevTime)	;
	sendMsg.m_cFamily.SetCookie(siChildren_PersonID,siCookieLevel,siCookieEndTime,siCookieLimitTime,MYCLASS_IN_FAMILY_CHILDREN)	;//->DB작업을 안해서삭제
	END_FETCHDATA()
	END_NEXTRECORDSET()



	// send
	SendPacket( (BYTE *)&sendMsg );

}
// 자식 삭제
void DBGameProtocol::Del_Family_Children()
{
	// 받은 패킷
	sDBRequest_Family_Del_Children	*pRecvMsg = (sDBRequest_Family_Del_Children*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Family_Del_Children	sendMsg;

	ZeroMemory(&sendMsg,sizeof(sendMsg))	;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_FAMILY_DEL_CHILDREN;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	SI32 siParent_PersonID ;
	TCHAR szParentCharName[MAX_PLAYER_NAME]= TEXT("");

	SI32 siChildren_PersonID ;
	TCHAR szChildCharName[MAX_PLAYER_NAME]= TEXT("");
	SI32 siDateVary	=	0;
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	SI32 siCookieEndTime	=	0	;	// 쿠키 종료 타임
	SI32 siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	SI32 siCookieLevel		=	0	;	// 쿠키 레벨
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.

	SYSTEMTIME stRevTime	;// 변환해서 나갈 시간
	TIMESTAMP_STRUCT stRevTS;	//-> db로부터 받을 것
	ZeroMemory(&stRevTS, sizeof(stRevTS));

	// DB Query
	BEGIN_SP( "rsp_FamilyDelChild" )
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siChildrenPersonID, sizeof(pRecvMsg->m_siChildrenPersonID) );	// 지우는 자식
	
	END_SP()
	EXEC_SQL()
	BEGIN_FETCHDATA()
	// 부모 셋팅
	m_odbc.GetData( &siParent_PersonID);
	m_odbc.GetData( szParentCharName,sizeof(szParentCharName));
	//------------------------------------------------------------
	sendMsg.m_cFamily.SetParent(siParent_PersonID,szParentCharName)	;
	END_FETCHDATA()
	BEGIN_NEXTRECORDSET()
	// 자식 셋팅 
	BEGIN_FETCHDATA()
	m_odbc.GetData( &siChildren_PersonID);
	m_odbc.GetData( szChildCharName ,sizeof(szChildCharName));
	m_odbc.GetData( &stRevTS );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.->DB작업을 안해서 삭제
	siCookieEndTime	=	0	;	// 쿠키 종료 타임
	siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	siCookieLevel		=	0	;	// 쿠키 레벨
	m_odbc.GetData( &siCookieLevel );
	m_odbc.GetData( &siCookieLimitTime );
	m_odbc.GetData( &siCookieEndTime );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	//------------------------------------------------------------
	
	Convert_SYSTEMTIME_from_TIMESTAMP(&stRevTime, &stRevTS );
	sendMsg.m_cFamily.SetChildren(siChildren_PersonID,szChildCharName,stRevTime)	;
	sendMsg.m_cFamily.SetCookie(siChildren_PersonID,siCookieLevel,siCookieEndTime,siCookieLimitTime,MYCLASS_IN_FAMILY_CHILDREN)	;
	END_FETCHDATA()
	END_NEXTRECORDSET()

		// send
	SendPacket( (BYTE *)&sendMsg );

}
void DBGameProtocol::Get_Family_info()
{
	// 받은 패킷
	sDBRequest_Get_FamilyInfo* pRecvMsg = (sDBRequest_Get_FamilyInfo*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Get_FamilyInfo	sendMsg;

	ZeroMemory(&sendMsg,sizeof(sendMsg))	;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_GET_FAMILYINFO;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	SI32 siParent_PersonID = 0 ;
	TCHAR szParentCharName[MAX_PLAYER_NAME]	= TEXT("");

	SI32 siChildren_PersonID = 0 ;
	SI32 siDateVary =	0	;
	TCHAR szChildCharName[MAX_PLAYER_NAME]	= TEXT("");
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	SI32 siCookieEndTime	=	0	;	// 쿠키 종료 타임
	SI32 siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	SI32 siCookieLevel		=	0	;	// 쿠키 레벨
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.

	SYSTEMTIME stRevTime	;// 변환해서 나갈 시간
	TIMESTAMP_STRUCT stRevTS;	//-> db로부터 받을 것
	ZeroMemory(&stRevTS, sizeof(stRevTS));
	
	// DB Query
	BEGIN_SP( "rsp_FamilyInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );// 정보를 요청한 사람.
	END_SP()
	EXEC_SQL()
	BEGIN_FETCHDATA()
	// 부모 셋팅
		m_odbc.GetData( &siParent_PersonID);
		m_odbc.GetData( szParentCharName,sizeof(szParentCharName));
		//------------------------------------------------------------
		sendMsg.m_cFamily.SetParent(siParent_PersonID,szParentCharName)	;
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	// 자식 셋팅 
		BEGIN_FETCHDATA()
		m_odbc.GetData( &siChildren_PersonID);
		m_odbc.GetData( szChildCharName ,sizeof(szChildCharName));
		m_odbc.GetData( &stRevTS );
		//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.->db작업을 안해서 삭제
		siCookieEndTime	=	0	;	// 쿠키 종료 타임
		siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
		siCookieLevel		=	0	;	// 쿠키 레벨
		m_odbc.GetData( &siCookieLevel );
		m_odbc.GetData( &siCookieLimitTime );
		m_odbc.GetData( &siCookieEndTime );
		//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
		//------------------------------------------------------------
		Convert_SYSTEMTIME_from_TIMESTAMP(&stRevTime, &stRevTS );
		sendMsg.m_cFamily.SetChildren(siChildren_PersonID,szChildCharName,stRevTime)	;
		sendMsg.m_cFamily.SetCookie(siChildren_PersonID,siCookieLevel,siCookieEndTime,siCookieLimitTime,MYCLASS_IN_FAMILY_CHILDREN)	;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.m_siPersonID	=	pRecvMsg->m_siPersonID	;
	// send
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::Plus_FamilyEffect_date()
{
	// 받은 패킷
	sDBRequest_Family_Plus_Add_Date* pRecvMsg = (sDBRequest_Family_Plus_Add_Date*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Family_Plus_Add_Date	sendMsg;

	ZeroMemory(&sendMsg,sizeof(sendMsg))	;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_FAMILY_PLUS_ADD_DATE;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	SI32 siParent_PersonID =	0	;
	TCHAR szParentCharName[MAX_PLAYER_NAME]= TEXT("");

	SI32 siChildren_PersonID =	0	;
	TCHAR szChildCharName[MAX_PLAYER_NAME]= TEXT("");
	SI32 siDateVary	=	0	;
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	SI32 siCookieEndTime	=	0	;	// 쿠키 종료 타임
	SI32 siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	SI32 siCookieLevel		=	0	;	// 쿠키 레벨
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.

	TIMESTAMP_STRUCT stSaveTS;	//-> 내가 저장하는놈
	ZeroMemory(&stSaveTS, sizeof(stSaveTS));	
	TIMESTAMP_STRUCT stRevTS;	//-> db로부터 받을 것
	ZeroMemory(&stRevTS, sizeof(stRevTS));
	SYSTEMTIME stRevTime	;// 변환해서 나갈 시간

	Convert_TIMESTAMP_from_SYSTEMTIME( &stSaveTS ,&pRecvMsg->m_siDateVary ) 	;

	// DB Query
	BEGIN_SP( "rsp_FamilyEffectDateSet  " )
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );// 대상
	m_odbc.SetParam( SQL_TIMESTAMP, &stSaveTS, sizeof(stSaveTS) );// 대상
	END_SP()
	EXEC_SQL()
	BEGIN_FETCHDATA()
		// 부모 셋팅
	m_odbc.GetData( &siParent_PersonID);
	m_odbc.GetData( szParentCharName,sizeof(szParentCharName));
	//------------------------------------------------------------
	sendMsg.m_cFamily.SetParent(siParent_PersonID,szParentCharName)	;
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	// 자식 셋팅 
	BEGIN_FETCHDATA()
	m_odbc.GetData( &siChildren_PersonID);
	m_odbc.GetData( szChildCharName ,sizeof(szChildCharName));
	m_odbc.GetData( &stRevTS );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.->DB작업을 안해서삭제
	siCookieEndTime	=	0	;	// 쿠키 종료 타임
	siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	siCookieLevel		=	0	;	// 쿠키 레벨
	m_odbc.GetData( &siCookieLevel );
	m_odbc.GetData( &siCookieLimitTime );
	m_odbc.GetData( &siCookieEndTime );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	//------------------------------------------------------------
	Convert_SYSTEMTIME_from_TIMESTAMP(&stRevTime, &stRevTS );
	sendMsg.m_cFamily.SetChildren(siChildren_PersonID,szChildCharName,stRevTime)	;
	sendMsg.m_cFamily.SetCookie(siChildren_PersonID,siCookieLevel,siCookieEndTime,siCookieLimitTime,MYCLASS_IN_FAMILY_CHILDREN)	;
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::Set_Cookie_Effect()
{
	// 받은 패킷
	sDBRequest_Set_My_CookeEffect* pRecvMsg = (sDBRequest_Set_My_CookeEffect*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Set_My_CookeEffect	sendMsg;

	ZeroMemory(&sendMsg,sizeof(sendMsg))	;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_FAMILY_SET_COOKIE_EFFECT;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	SI32 siParent_PersonID =	0	;
	TCHAR szParentCharName[MAX_PLAYER_NAME]= TEXT("");

	SI32 siChildren_PersonID =	0	;
	TCHAR szChildCharName[MAX_PLAYER_NAME]= TEXT("");
	SI32 siDateVary	=	0	;
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	SI32 siCookieEndTime	=	0	;	// 쿠키 종료 타임
	SI32 siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	SI32 siCookieLevel		=	0	;	// 쿠키 레벨
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	
	TIMESTAMP_STRUCT stRevTS;	//-> db로부터 받을 것
	ZeroMemory(&stRevTS, sizeof(stRevTS));
	SYSTEMTIME stRevTime	;// 변환해서 나갈 시간;

	// DB Query
	BEGIN_SP( "rsp_FamilyCookieEffectDateSet" )
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );// 쿠키 이팩트를 사용한 사람
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siCookieLevel, sizeof(pRecvMsg->m_siCookieLevel) );// 쿠키 이팩트 레벨
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siEndDateVary, sizeof(pRecvMsg->m_siEndDateVary) );// 쿠키 이팩트 종료 시간	
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siUseLimitDateVary, sizeof(pRecvMsg->m_siUseLimitDateVary) );// 쿠키 이팩트 사용 제한 시간
	END_SP()
	EXEC_SQL()
	BEGIN_FETCHDATA()
	// 부모 셋팅
	m_odbc.GetData( &siParent_PersonID);
	m_odbc.GetData( szParentCharName,sizeof(szParentCharName));
	//------------------------------------------------------------
	sendMsg.m_cFamily.SetParent(siParent_PersonID,szParentCharName)	;
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	// 자식 셋팅 
	BEGIN_FETCHDATA()
	m_odbc.GetData( &siChildren_PersonID);
	m_odbc.GetData( szChildCharName ,sizeof(szChildCharName));
	m_odbc.GetData( &stRevTS );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	siCookieEndTime	=	0	;	// 쿠키 종료 타임
	siCookieLimitTime	=	0	;	// 쿠키 사용 제한 타임
	siCookieLevel		=	0	;	// 쿠키 레벨
	m_odbc.GetData( &siCookieLevel );
	m_odbc.GetData( &siCookieLimitTime );
	m_odbc.GetData( &siCookieEndTime );
	//쿠키이팩트 추가-------------정보 가지고올떄 자식들은 싹 추가 해줘야 합니다.
	//------------------------------------------------------------
	Convert_SYSTEMTIME_from_TIMESTAMP(&stRevTime, &stRevTS );
	sendMsg.m_cFamily.SetChildren(siChildren_PersonID,szChildCharName,stRevTime)	;
	sendMsg.m_cFamily.SetCookie(siChildren_PersonID,siCookieLevel,siCookieEndTime,siCookieLimitTime,MYCLASS_IN_FAMILY_CHILDREN)	;
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.m_siPersonID	=	pRecvMsg->m_siPersonID	;
	// send
	SendPacket( (BYTE *)&sendMsg );
}