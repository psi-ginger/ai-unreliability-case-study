#pragma once

#define SERVERIP "222.122.40.126"
#define NETMARBLESERVERIP "220.95.226.81"
//#define SERVERIP "192.168.1.49"
#define MAX_LOGTEXT_LEN 512
#define MAX_SEARCH_LEN 40

class CLogUnit
{
public:
	char	szNumber[10];
	char	szDate[25];
	char	szCommand[50];
	char	szRetPersonName[21];
	char	szDstPersonName[21];
	char	szItemUnique[64];
	char	szLogTxt[MAX_LOGTEXT_LEN];

	void	Init()
	{
		ZeroMemory(szNumber, sizeof(szNumber));
		ZeroMemory(szDate, sizeof(szDate));
		ZeroMemory(szCommand, sizeof(szCommand));
		ZeroMemory(szRetPersonName, sizeof(szRetPersonName));
		ZeroMemory(szDstPersonName, sizeof(szDstPersonName));
		ZeroMemory(szItemUnique, sizeof(szItemUnique));
		ZeroMemory(szLogTxt, sizeof(szLogTxt));
	}

	CLogUnit(void)
	{
		Init();
	}
	CLogUnit(char *date, char *command, char *retpersonname, char *dstpersonname, char *itemunique, char *logtxt)
	{
		Init();
		strncpy(szDate, date, sizeof(szDate));
		strncpy(szCommand, command, sizeof(szCommand));
		strncpy(szRetPersonName, retpersonname, sizeof(szRetPersonName));
		strncpy(szDstPersonName, dstpersonname, sizeof(szDstPersonName));
		strncpy(szItemUnique, itemunique, sizeof(szItemUnique));
		strncpy(szLogTxt, logtxt, sizeof(szLogTxt));
	}
	~CLogUnit(void)
	{
	}
};
