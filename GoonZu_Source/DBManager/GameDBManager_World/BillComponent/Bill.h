// Bill.h : Declaration of the CBill

#ifndef __BILL_H_
#define __BILL_H_

#include "SyncSocket.h"

#define	PL_NULL_VALUE			"(null)"
#define	PL_FIELD_DELEMITER		0x03
#define	PL_VALUE_DELEMITER		0x02
#define	PL_CRLF					"\r\n"
#define PL_OK_RESPONSE_CODE		"0000"

#define	PL_FIELDNAME_ERRMSG		"errmsg"

#define	PL_RESCODE_POS			20
#define	PL_RESCODE_LEN			4
#define	PL_ISENCRYPTED_POS		24		// 암호화여부 (Y or N)
#define	PL_ISENCRYPTED_LEN		1
#define	PL_ENCVER_POS			25		// 암호화버전
#define	PL_ENCVER_LEN			3
#define	CONTROL_PART_LEN		28

/////////////////////////////////////////////////////////////////////////////
// CBill
class CBill
{
public:
	CBill();
	~CBill();

private:
	void InitVar();
	void InitVariables();
	void UnInitVariables();
	int Win32WideCharToMultiByte(LPCWSTR wszString, LPSTR szReturnString, int cbMultiByte);
	int TruncateSpace(char* strSrc);
	int PLToLower(char*	szSrc);
	int SendRecvCommand(char* szCmdRequest, char* szCmdResponse);
	int OpenServer();
	int PLGetVal(char*	szFieldName, char* szValue, unsigned int nBufSize);
	int OpenServerSub();
	int	MakeRequestPacket(char*	szCmdRequest);
	int	ParseResponsePacket();

	DWORD		m_dwInternalErrorCode;
	char		m_szErrorMsg[512];

	// Socket 통신용 멤버변수
	CSyncSocket	m_Socket;

	char*		m_pszRequestParam;
	char*		m_pszResultMsg;
	char		m_szCmd[32];

public:
	char* get_ErrMsg();
	long GetVal(char* strFieldName, char* strValue, unsigned int nBufSize);
	long put_ConnectionTimeout(long newVal);
	void PutHOST(char* szHosts);
	int SetField(char* szFieldName, char* szFieldValue);
	long StartAction();
	void PutTxCmd(char* szTxCmd);
	long SetFieldLong(char* szFieldName, long lngFieldValue);

};

#endif //__BILL_H_
