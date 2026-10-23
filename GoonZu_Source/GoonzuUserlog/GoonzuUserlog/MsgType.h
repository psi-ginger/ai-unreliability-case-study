/*
#include "ITZNetLib/itzbase.h"
#include "item/ItemCommon/commonutil.h"
#define MAX_NAME_NUM 21
#define MAX_PASSWORD_NUM 21
#define MAX_SERVER_NAME 20
#define MAX_PERSON_NAME 20
#define MAX_TIME_LENGTH 17
#define MAX_LOGDATA_LENGTH 25
#define MAX_COMMAND_LENGTH 50
#define MAX_LOGTEXT_LENGTH 512

//서버와 클라이언트간의 메세지
enum UserLogNetworkMsg{
	REQUEST_LOGIN,         //사용자 인증 요청
	RESPONSE_LOGIN,        //사용자 인증 응답 

	REQUEST_INIT_DATA,     //초기화 데이터 요청
	RESPONSE_INIT_DATA,    //초기화 데이터 응답

	REQUEST_LOGDATA,       //로그 데이터 조회 요청
	RESPONSE_LOGDATA,      //로그 데이터 조회 응답

	REQUEST_HEARTBEAT,

	REQUEST_LOGOUT,       //접속해제 요청
	RESPONSE_LOGOUT,      //접속해제 응답
};



class cltMsgResponse_Login{
public:
	char szUserID[20+1];
	char szPassword[20+1];

    cltMsgResponse_Login(char* UserID, char* PassWord)
	{
	
		 MStrCpy(szUserID,UserID, MAX_NAME_NUM);
		 MStrCpy(szPassword,PassWord, MAX_PASSWORD_NUM);
			
	}
};

class cltMsgRequest_Login{
public:
	SI32 siResult;

	cltMsgRequest_Login(SI32 Result)
	{
		siResult = Result;
	}
};

class cltMsgRequest_Init_Data{   //초기화 데이터 요청
public:
	cltMsgRequest_Init_Data()
	{
	}
};

class cltMsgResponse_Init_Data{
public:	
	char szServerName[20];

	cltMsgResponse_Init_Data(char* ServerName)
	{
		 MStrCpy(szServerName,ServerName, MAX_SERVER_NAME);
	}
};

class cltMsgRequest_Logdata{
public:
	char szServerName[20];
	char szPersonName[20];
	SI16 siltemUnique;
	SI08 siLogType;
	char szStartTime[17];
	char szEndTime[17];
	cltMsgRequest_Logdata(char* ServerName, char* PersonName, SI16 itemUnique, SI08 LogType,char* StartTime, char* EndTime)
	{
		MStrCpy(szServerName, ServerName, MAX_SERVER_NAME);
		MStrCpy(szPersonName, PersonName, MAX_PERSON_NAME);
		siltemUnique = itemUnique;
		siLogType = LogType;
		MStrCpy(szStartTime, StartTime, MAX_TIME_LENGTH);
		MStrCpy(szEndTime, EndTime,MAX_TIME_LENGTH);

	}

};

class cltMsgResponse_Logdata{
public:
	char szLogData[25];
	char szCommand[50];
	char szPersonName[20];
	char szDstPersonName[20];
	SI16 siItemUnique;
	char szLogText[512];
	cltMsgResponse_Logdata(char* LogData,char* Command, char* PersonName, char* DstPersonName, SI16 ItemUnique, char *LogText )
	{
		MStrCpy(szLogData,LogData,MAX_LOGDATA_LENGTH);
		MStrCpy(szCommand,Command,MAX_COMMAND_LENGTH);
		MStrCpy(szPersonName,PersonName,MAX_PERSON_NAME);
		siItemUnique = ItemUnique;
		MStrCpy(szDstPersonName,DstPersonName,MAX_PERSON_NAME);
		MStrCpy(szLogText,LogText,MAX_LOGTEXT_LENGTH);

	}

};


//-------------------------------------------------------------------
//클라이언트에서 서버로 보내는 일반적인 게임 메시지를 담을 공간. 
//-------------------------------------------------------------------

#define MAX_CLTMSG_DATA_SIZE   10000 //4의 배수여야 한다. 

class cltMsg{
public:
	sPacketHeader clHead;	

	union{
		char    cData[MAX_CLTMSG_DATA_SIZE];				//실제 데이터가 들어있는 공간. 
		SI32	siData32[MAX_CLTMSG_DATA_SIZE/4];
	};


	cltMsg(){};

	cltMsg(UI16 type, SI32 datasize, const char* pdata)
	{
		clHead.usCmd	=	type;

		if(datasize >= MAX_CLTMSG_DATA_SIZE)
		{
			AfxMessageBox("cltMsg1234");
			//MsgBox("cltMsg", "type:%d datasize:%d", type, datasize);
		}
		else
		{
			clHead.usSize	=	sizeof(sPacketHeader) + datasize;

			if(pdata && datasize)
			{
				memcpy(cData, pdata, datasize);
			}
		}
	}

	// 32bit 변수 설정시 생성자. 
	cltMsg(UI16 type, SI32 sidata)
	{
		clHead.usCmd	=	type;
		clHead.usSize	=	sizeof(sPacketHeader) + sizeof(SI32);

		siData32[0] = sidata;
	}

	SI32 GetTotalSize(){return clHead.usSize;}
	SI32 GetDataSize(){return clHead.usSize - sizeof(sPacketHeader);}
};



*/