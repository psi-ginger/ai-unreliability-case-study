#ifndef _NBILLMSG_H
#define _NBILLMSG_H


enum {
	BILLMSG_REQUEST_ENCRYPTIDPW = 1,
	BILLMSG_RESPONSE_ENCRYPTIDPW,
	BILLMSG_REPORT_GAMEHEARTBEAT,
	BILLMSG_REQUEST_ENCRYPTIDPW_WEB,
	BILLMSG_RESPONSE_ENCRYPTIDPW_WEB,
	BILLMSG_REQUEST_ENCRYPTIDPW_GACHA,
	BILLMSG_RESPONSE_ENCRYPTIDPW_GACHA,
};

/*
//-----------------------------------------------------------------------------
// 게임서버상태 보고 (게임서버 -> 빌서버)
//-----------------------------------------------------------------------------
struct sReportToBill_GameHeartbeat
{
	sPacketHeader			packetHeader;

	UI16					serverId;
	GAMESERVER_REFRESHINFO	refreshinfo;
};

*/

class NBillRequest_EncryptIDPW
{
public:
	sPacketHeader		packetHeader;
	int			m_charid;
	int			m_personid;
	TCHAR		m_szID[ 20 ];
	TCHAR		m_szPW[ 128 ];

	NBillRequest_EncryptIDPW()
	{
		ZeroMemory(this, sizeof(NBillRequest_EncryptIDPW));
	}

	NBillRequest_EncryptIDPW( int charid, int personid, TCHAR *id, TCHAR *pw )
	{
		packetHeader.usSize = sizeof( NBillRequest_EncryptIDPW );
        packetHeader.usCmd = BILLMSG_REQUEST_ENCRYPTIDPW;
		m_charid = charid;
		m_personid = personid;
		
		memcpy(m_szID, id, 20 );
		m_szID[ 19 ] = NULL;
		memcpy(m_szPW, pw, 128 );
		m_szPW[127] = NULL;

	}

};


class NBillResponse_EncryptIDPW
{
public:
	sPacketHeader		packetHeader;
	int			m_charid;
	int			m_personid;
	TCHAR		m_ret[ 256 ];

	NBillResponse_EncryptIDPW()
	{
		ZeroMemory(this, sizeof(NBillResponse_EncryptIDPW));
	}

	NBillResponse_EncryptIDPW( int charid,int personid, TCHAR *ret )
	{
		packetHeader.usSize = sizeof( NBillResponse_EncryptIDPW);
		packetHeader.usCmd = BILLMSG_RESPONSE_ENCRYPTIDPW;

		m_charid = charid;
		m_personid = personid;
		memcpy(m_ret, ret, 256 );
		m_ret[255] = NULL;
	}
};

class NBillRequest_EncryptIDPW_Web
{
public:
	sPacketHeader		packetHeader;
	int			m_charid;
	int			m_personid;
	TCHAR		m_szID[ 20 ];
	TCHAR		m_szPW[ 128 ];
	TCHAR		m_szUrl[ MAX_PATH ];

	NBillRequest_EncryptIDPW_Web()
	{
		ZeroMemory(this, sizeof(NBillRequest_EncryptIDPW_Web));
	}

	NBillRequest_EncryptIDPW_Web( int charid, int personid, TCHAR *id, TCHAR *pw, TCHAR *url )
	{
		packetHeader.usSize = sizeof( NBillRequest_EncryptIDPW_Web );
		packetHeader.usCmd = BILLMSG_REQUEST_ENCRYPTIDPW_WEB;
		m_charid = charid;
		m_personid = personid;

		memcpy(m_szID, id, 20 );
		m_szID[ 19 ] = NULL;
		memcpy(m_szPW, pw, 128 );
		m_szPW[127] = NULL;
		memcpy(m_szUrl, url, MAX_PATH );
		m_szUrl[MAX_PATH - 1] = NULL;

	}

};


class NBillResponse_EncryptIDPW_Web
{
public:
	sPacketHeader		packetHeader;
	int			m_charid;
	int			m_personid;
	TCHAR		m_ret[ 256 ];
	TCHAR		m_szUrl[ MAX_PATH ];

	NBillResponse_EncryptIDPW_Web()
	{
		ZeroMemory(this, sizeof(NBillResponse_EncryptIDPW_Web));
	}

	NBillResponse_EncryptIDPW_Web( int charid,int personid, TCHAR *url, TCHAR *ret )
	{
		packetHeader.usSize = sizeof( NBillResponse_EncryptIDPW_Web);
		packetHeader.usCmd = BILLMSG_RESPONSE_ENCRYPTIDPW_WEB;

		m_charid = charid;
		m_personid = personid;

		memcpy(m_szUrl, url, MAX_PATH);
		m_szUrl[MAX_PATH - 1] = NULL;

		memcpy(m_ret, ret, 256 );
		m_ret[255] = NULL;
	}
};


class NBillRequest_EncryptIDPW_Gacha
{
public:
	sPacketHeader		packetHeader;
	int			m_charid;
	int			m_personid;
	TCHAR		m_szID[ 20 ];
	TCHAR		m_szPW[ 128 ];

	NBillRequest_EncryptIDPW_Gacha()
	{
		ZeroMemory(this, sizeof(NBillRequest_EncryptIDPW_Gacha));
	}

	NBillRequest_EncryptIDPW_Gacha( int charid, int personid, TCHAR *id, TCHAR *pw )
	{
		packetHeader.usSize = sizeof( NBillRequest_EncryptIDPW_Gacha );
		packetHeader.usCmd = BILLMSG_REQUEST_ENCRYPTIDPW_GACHA;
		m_charid = charid;
		m_personid = personid;

		memcpy(m_szID, id, 20 );
		m_szID[ 19 ] = NULL;
		memcpy(m_szPW, pw, 128 );
		m_szPW[127] = NULL;

	}

};


class NBillResponse_EncryptIDPW_Gacha
{
public:
	sPacketHeader		packetHeader;
	int			m_charid;
	int			m_personid;
	TCHAR		m_ret[ 256 ];

	NBillResponse_EncryptIDPW_Gacha()
	{
		ZeroMemory(this, sizeof(NBillResponse_EncryptIDPW_Gacha));
	}

	NBillResponse_EncryptIDPW_Gacha( int charid,int personid, TCHAR *ret )
	{
		packetHeader.usSize = sizeof( NBillResponse_EncryptIDPW_Gacha);
		packetHeader.usCmd = BILLMSG_RESPONSE_ENCRYPTIDPW_GACHA;

		m_charid = charid;
		m_personid = personid;
		memcpy(m_ret, ret, 256 );
		m_ret[255] = NULL;
	}
};



#endif