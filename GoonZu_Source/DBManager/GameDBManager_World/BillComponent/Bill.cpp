// Bill.cpp : Implementation of CBill
#include <stdio.h>
#include "Bill.h"
#include "limits.h"

/////////////////////////////////////////////////////////////////////////////
// CBill
CBill::CBill()
{	
	(void)InitVariables();
}

CBill::~CBill()
{
	(void)UnInitVariables();
}

int CBill::SetField(char* szFieldName, char* szFieldValue)
{
	char	szFdel[2];
	char	szVdel[2];

	try
	{
		szFdel[0] = PL_FIELD_DELEMITER;
		szVdel[0] = PL_VALUE_DELEMITER;
		szFdel[1] = NULL;
		szVdel[1] = NULL;

		strcat(m_pszRequestParam, szFdel);
		
		(void)PLToLower(szFieldName);
		strcat(m_pszRequestParam, szFieldName);
		
		strcat(m_pszRequestParam, szVdel);

		strcat(m_pszRequestParam, szFieldValue);
	}
	catch(...) {}

	return 0;
}


void CBill::InitVariables()
{
	m_pszRequestParam		= new char[MAX_PACKETSIZE];
	m_pszResultMsg			= new char[MAX_PACKETSIZE];

	(void)InitVar();
}

// added from soohyunk at 2002-09-23
void CBill::InitVar()
{
	m_Socket.Init(INVALID_SOCKET);

	m_dwInternalErrorCode	= 0;
	m_pszRequestParam[0]	= NULL;
	m_pszResultMsg[0]		= NULL;
	m_szErrorMsg[0]			= NULL;
	m_szCmd[0]				= NULL;
}

void CBill::UnInitVariables()
{
	m_Socket.Close();

	if (m_pszRequestParam != NULL)
	{
		delete [] m_pszRequestParam;
		m_pszRequestParam = NULL;
	}
	if (m_pszResultMsg != NULL)
	{
		delete [] m_pszResultMsg;
		m_pszResultMsg = NULL;
	}
}

// Truncate Left & Right Spaces
int CBill::TruncateSpace(char* strSrc)
{
	try
	{
		int len = strlen(strSrc);
		char*	pszTmp = new char[len+1];

		// RTrim
		char*	pstr = strSrc + len - 1;
		while (*pstr == ' ')
		{
			*pstr = NULL;
			if (pstr == strSrc)
				break;
			pstr--;
		}

		// LTrim
		pstr = strSrc;
		while (*pstr == ' ')
		{
			*pstr = NULL;
			pstr++;
		}
		strcpy(pszTmp, pstr);
		strcpy(strSrc, pszTmp);

		delete [] pszTmp;
	}
	catch(...) {}

	return (strlen(strSrc));
}

int CBill::PLToLower(char*	szSrc)
{
	char*	p = szSrc;
	for(p = szSrc; p < szSrc + strlen(szSrc);p++)
	{
		if (isupper(*p))
			*p = tolower(*p);
	}
	return (0);
}

// field0x02value0x03field0x02value
int CBill::PLGetVal(char* strCharSet1, char* szValue, unsigned int nBufSize)
{
	char	*p1 = NULL;
	char	*p2 = NULL;
	char	strCharSet[132];
	char	szFdel[2];

	try
	{
		(void)PLToLower(strCharSet1);

		szFdel[0] = PL_FIELD_DELEMITER;
		szFdel[1] = NULL;

		sprintf(strCharSet, "%c%s%c", PL_FIELD_DELEMITER, strCharSet1, PL_VALUE_DELEMITER);
		int		len	= strlen(strCharSet);

		szValue[0] = NULL;

		p1 = strstr(m_pszResultMsg+CONTROL_PART_LEN, strCharSet);
		if (p1 == NULL) // Not found
			return 0;

		p2 = strstr(p1+1, szFdel); // 0x03
		if (p2 == NULL)
		{
			if (nBufSize < strlen(p1 + len)+1)
				return 0;
			strcpy(szValue, p1 + len);
		}
		else
		{
			if (nBufSize < (unsigned int)(p2 - p1 - len + 1))
				return 0;
			strncpy(szValue, p1 + len, p2 - p1 - len);
			szValue[p2 - p1 - len] = NULL;
		}
	}
	catch(...) {}

	return 0;
}

int	CBill::ParseResponsePacket()
{
	char	szResCode[PL_RESCODE_LEN+1];

	try
	{
		(void)strncpy(szResCode, m_pszResultMsg+PL_RESCODE_POS, PL_RESCODE_LEN);
		szResCode[PL_RESCODE_LEN] = NULL;

		m_dwInternalErrorCode = atoi(szResCode);
		// 에러난 경우 에러메세지를 담는다.
		if (m_dwInternalErrorCode != 0)
		{
			(void)PLGetVal(PL_FIELDNAME_ERRMSG, m_szErrorMsg, sizeof(m_szErrorMsg));
		}
	}
	catch(...) {}

	return (0);
}

int	CBill::MakeRequestPacket(char*	szCmdRequest)
{
	if (szCmdRequest == NULL)
	{
		return (-1);
	}

	// TXCMD가 설정이 되어 있지 않다.
	if (strlen(m_szCmd) < 1)
	{
		m_dwInternalErrorCode = PLPAY_PARAMERR_100;
		sprintf(m_szErrorMsg, PLPAY_PARAMERR_100_MSG, "TXCMD");
		return (-1);
	}
	try
	{
		(void)sprintf(szCmdRequest, "%04.04d%-16.16s%-4.4s%c%03.03d%s%s",
							0,
							m_szCmd,
							PL_OK_RESPONSE_CODE,
							'N',
							0,
							m_pszRequestParam,
							PL_CRLF);

		// 요구전문의 맨 앞에 전문길이(4바이트) 정보를 추가 한다.
		char szTmp[10];
		(void)sprintf(szTmp, "%04d", strlen(szCmdRequest));
		(void)strncpy(szCmdRequest, szTmp, 4);
	}
	catch(...) {}

	return (0);
}

int CBill::SendRecvCommand(char* szCmdRequest, char* szCmdResponse)
{
	int nSentByte = 0;
	int nRecvByte = 0;

	try
	{
		m_pszResultMsg[0] = NULL;

		nSentByte = m_Socket.Write(szCmdRequest, strlen(szCmdRequest));
		// send 에러
		if (nSentByte < 0)
		{
			strcpy(m_szErrorMsg, PLPAY_SOCKERR_210_MSG);
			return nSentByte;
		}
		// 보내고자하는 길이보다 적게 보낸 경우
		if (nSentByte != (int)strlen(szCmdRequest))
		{
			strcpy(m_szErrorMsg, PLPAY_SOCKERR_220_MSG);
			return PLPAY_SOCKERR_220;
		}

		szCmdResponse[0] = NULL;
		nRecvByte = m_Socket.GetLine(szCmdResponse);
		// revc 에러
		if (nRecvByte < 0)
		{
			strcpy(m_szErrorMsg, PLPAY_SOCKERR_250_MSG);
			return nRecvByte;
		}
	}
	catch(...) {}

	return 0;
}

int CBill::OpenServer()
{
	int nResult = 0;

	(void)strcpy(m_Socket.m_sHost, m_Socket.m_sMainHost);
	m_Socket.m_nPort = m_Socket.m_nMainPort;

	if (strlen(m_Socket.m_sHost) < 1 )
	{
		return(-231);
	}
	if (m_Socket.m_nPort < 0 )
	{
		return(-232);
	}

	nResult = OpenServerSub();
	if (nResult != 0)
	{
		if (strcmp(m_Socket.m_sHost, m_Socket.m_sMainHost) == 0)
		{
			(void)strcpy(m_Socket.m_sHost, m_Socket.m_sBackupHost);
			m_Socket.m_nPort = m_Socket.m_nBackupPort;
		}
		else
		{
			(void)strcpy(m_Socket.m_sHost, m_Socket.m_sMainHost);
			m_Socket.m_nPort = m_Socket.m_nMainPort;
		}

		m_Socket.Close();
		nResult = OpenServerSub();
	}

	if (nResult != 0)
	{
		m_Socket.Close();

		(void)sprintf(m_szErrorMsg, PLPAY_SOCKERR_201_MSG, m_Socket.m_sHost, m_Socket.m_nPort);
		m_dwInternalErrorCode = nResult;
	}

	return (nResult);
}

int CBill::OpenServerSub()
{
	int		nResult = 0;
	int		nRecvByte = 0;
	int		nConcurrentConnection = 0;
	char	szSockMsg[256];

	try
	{
		nResult = m_Socket.Open();
		if (nResult != 0)
		{
			m_dwInternalErrorCode = (DWORD)nResult;
			if (m_dwInternalErrorCode == PLPAY_SOCKERR_200)
			{
				(void)sprintf(m_szErrorMsg, PLPAY_SOCKERR_200_MSG, m_Socket.m_sHost, m_Socket.m_nPort);
			}
			else
			{
				(void)sprintf(m_szErrorMsg, PLPAY_SOCKERR_201_MSG, m_Socket.m_sHost, m_Socket.m_nPort);
			}
			return nResult;
		}
		szSockMsg[0] = NULL;
		nRecvByte = m_Socket.GetLine(szSockMsg);
		// revc 에러
		if (nRecvByte < 0)
		{
			m_Socket.Close();
			m_dwInternalErrorCode = PLPAY_SOCKERR_260;
			strcpy(m_szErrorMsg, PLPAY_SOCKERR_260_MSG);
			return nRecvByte;
		}
		// Socket 연결이 성공하면 데몬으로부터 동시사용자수를 받는다.
		if (strncmp(szSockMsg, PL_OK_RESPONSE_CODE, 4) == 0)
		{
			nConcurrentConnection = atoi(szSockMsg+4);
		}
		// '000'이 아닌 경우 현재 데몬 서버가 '서비스일시중지상태'이거나
		// '연결이 허용되지 않는 경우'임
		else
		{
			char szTmp[5];

			strncpy(szTmp, szSockMsg, 4);
			szTmp[4] = NULL;
			m_dwInternalErrorCode = (DWORD)atoi(szTmp);
			strcpy(m_szErrorMsg, szSockMsg+4);

			return m_dwInternalErrorCode;
		}
	}
	catch(...) { return (-1); }

	return 0;
}

char* CBill::get_ErrMsg()
{
	(void)TruncateSpace(m_szErrorMsg);

	return m_szErrorMsg;
}

long CBill::GetVal(char* szFieldName, char* szValue, unsigned int nBufSize)
{
	if (nBufSize < 1 || szFieldName == NULL || szValue == NULL)
		return (-1);

	int nResult = 0;

	try
	{
		szValue[0] = NULL;

		nResult = PLGetVal(szFieldName, szValue, nBufSize);
		(void)TruncateSpace(szValue);
	}
	catch(...)
	{
		nResult = 0;
	}

	return nResult;
}



long CBill::put_ConnectionTimeout(long newVal)
{
	if (newVal < 0 || LONG_MAX < newVal) 
		return (-1);

	m_Socket.m_Contimeout.tv_sec  = (int)newVal;
	m_Socket.m_Contimeout.tv_usec = 0;

	return 0;
}

void CBill::PutHOST(char* szHosts)
{
	char*	p0 = NULL;
	char*	p1 = NULL;

	m_Socket.m_sHost[0] = NULL;
	m_Socket.m_sMainHost[0] = NULL;
	m_Socket.m_sBackupHost[0] = NULL;

	try
	{
		// obj.HOST = "pg1.payletter.com:7009;pg2.payletter.com:7009"
		p0 = szHosts;
		p1 = strchr(szHosts, ':');
		if (p1 == NULL)
		{
			return ;
		}
		*p1++ = NULL;
		(void)strcpy(m_Socket.m_sMainHost, p0);

		p0 = p1;
		p1 = strchr(p0, ';');
		if (p1 == NULL)
		{
			(void)TruncateSpace(p0);
			m_Socket.m_nMainPort = atoi(p0);
			
			// 한개 IP만 넘어온 경우 Backup 및 Main이 동일하도록 설정
			(void)strcpy(m_Socket.m_sBackupHost, m_Socket.m_sMainHost);
			m_Socket.m_nBackupPort = m_Socket.m_nMainPort;

			return ;
		}
		*p1++ = NULL;
		m_Socket.m_nMainPort = atoi(p0);

		p0 = p1;
		p1 = strchr(p0, ':');
		if (p1 == NULL)
		{
			// 두번째 서버 설정 값을 제대로 찾지 못하면 backup서버 정보를 main서버 정보와 동일하게 처리.
			(void)strcpy(m_Socket.m_sBackupHost, m_Socket.m_sMainHost);
			m_Socket.m_nBackupPort = m_Socket.m_nMainPort;
			return ;
		}

		*p1++ = NULL;
		(void)strcpy(m_Socket.m_sBackupHost, p0);
		(void)TruncateSpace(p1);
		m_Socket.m_nBackupPort = atoi(p1);

		(void)TruncateSpace(m_Socket.m_sMainHost);
		(void)TruncateSpace(m_Socket.m_sBackupHost);
	}
	catch(...) {}

	return ;
}

void CBill::PutTxCmd(char* szTxCmd)
{
	if (szTxCmd == NULL || strlen(szTxCmd) > 16)
		return;

	strcpy(m_szCmd, szTxCmd);

	return ;
}

long CBill::StartAction()
{
	int		nResult = 0;
	char	szCmdRequest[MAX_PACKETSIZE+CONTROL_PART_LEN];

	// 보낼 전문 포멧팅
	nResult = MakeRequestPacket(szCmdRequest);
	if (nResult != 0)
	{
		return m_dwInternalErrorCode;
	}

	nResult = OpenServer();
	if (nResult != 0)
	{
		return nResult;
	}

	// 데몬 서버로 명령을 보내고 결과를 받는다.
	nResult = SendRecvCommand(szCmdRequest, m_pszResultMsg);
	m_Socket.Close();
	if (nResult == 0)
	{
		(void)ParseResponsePacket();
	}
	else
	{
		m_dwInternalErrorCode = nResult;
	}
	// Request 버퍼 Clearing
	m_pszRequestParam[0] = NULL;

	return m_dwInternalErrorCode;
}

long CBill::SetFieldLong(char* szFieldName, long lngFieldValue)
{
	char	szFieldValue[32];
	
	sprintf(szFieldValue, "%d", lngFieldValue);
	(void)SetField(szFieldName, szFieldValue);

	return 0;
}
