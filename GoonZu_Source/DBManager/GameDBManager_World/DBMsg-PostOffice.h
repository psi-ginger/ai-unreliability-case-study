#ifndef _DBMSGPOSTOFFICE_H
#define _DBMSGPOSTOFFICE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\PostOffice\PostOffice.h"
#include "..\..\GoonZuWorld\CommonLogic\CityHall\CityHall.h"

class sDBRequest_PostOfficeMasterSendItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siReason;										// 아이템 지급 사유

	SI32				siReceiverPersonID;								// 수신자 person id
	TCHAR				szReceiverPersonName[ MAX_PLAYER_NAME ];		// 수신자 name

	cltItem				clItem;											// item			

	cltDate				clDate;											// 송신일자

	sDBRequest_PostOfficeMasterSendItem( SI32 reason, SI32 personid, TCHAR *receiverPersonName, cltItem *pclItem, cltDate *pclDate )
	{

		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_MASTERSENDITEM;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeMasterSendItem );

		siReason = reason;

		siReceiverPersonID = personid;

		if( receiverPersonName ) {		
			memcpy( szReceiverPersonName, receiverPersonName, MAX_PLAYER_NAME );
			szReceiverPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
		} else {
			szReceiverPersonName[ 0 ] = NULL;
		}

		clItem.Set( pclItem );

		clDate.Set( pclDate );
	}
};

class sDBResponse_PostOfficeMasterSendItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;												// 결과

	SI32				siReason;												// 아이템 지급 사유
	
	SI32				siReceiverPersonID;										// 수신자 personid

	cltItem				clItem;													// item (송신자)

	cltDate				clDate;

	sDBResponse_PostOfficeMasterSendItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeMasterSendItem ) );
	}


};


// 물품 전달을 요청한다
class sDBRequest_PostOfficeSendItem 
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPostType;												// 0:일반유저, 1:운영자

	SI32				siVillageUnique;

	SI32				siSenderPersonID;										// 송신자 personid

	TCHAR				szReceiverPersonName[ MAX_PLAYER_NAME ];				// 수신자 name

	SI16				siItemInventoryIndex;									// item index
	SI32				siItemUnique;											// item unique
	SI16				siItemNum;												// item num

	cltDate				clDate;													// 송신날짜
	GMONEY				siFee;													// 수수료
	//KHY - 0614 - 가차 아이템은 같은 계정간 캐릭터에 한하여 우편으로 주고 받을 수 있다. 
	BOOL				bGacha;                                                                             //가차 아이템 유무.

	sDBRequest_PostOfficeSendItem( UI16 id, SI32 posttype, SI32 villageUnique, SI32 senderpersonid, TCHAR *receiverPersonName,
		SI16 itemInventoryIndex, SI32 itemUnique, SI16 itemNum, cltDate *pclDate, GMONEY fee ,BOOL gacha)
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_SENDITEM;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeSendItem );

		usCharID = id;
		
		siPostType = posttype;
		siVillageUnique = villageUnique;
		siSenderPersonID = senderpersonid;

		memcpy( szReceiverPersonName, receiverPersonName, MAX_PLAYER_NAME );
		szReceiverPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;

		siItemInventoryIndex = itemInventoryIndex;
		siItemUnique = itemUnique;
		siItemNum = itemNum;

		clDate.Set( pclDate );
		siFee = fee;
		bGacha = gacha;
	}

};

class sDBResponse_PostOfficeSendItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;												// 결과

	SI32				siPostType;												// 0:일반유저, 1:운영자

	SI32				siVillageUnique;										// 마을유니크

	SI32				siSenderPersonID;										// 송신자 personid
	SI32				siReceiverPersonID;										// 수신자 personid

	SI16				siItemInventoryIndex;									// item index(송신자)
	SI32				siItemUnique;											// item unique(송신자)
	SI16				siItemNum;												// item num(송신자)

	cltMoney			clPostOfficeMoney;										// 역참 보유자금
	cltMoney			clPersonMoney;											// 송신자 소지금 정보	

	//[추가 : 황진성 2007. 10. 24 로그에 이용, 보내는 아이템 정보]
	SI16				siSendItemInventoryIndex;									// item index(송신자)
	SI32				siSendItemUnique;											// item unique(송신자)
	SI16				siSendItemNum;												// item num(송신자)

	// PCK : 로그용 패킷 추가
	GMONEY				siFee;													// 수수료.
	sDBResponse_PostOfficeSendItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeSendItem ) );
	}

};

// 송금을 요청한다
class sDBRequest_PostOfficeSendMoney
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPostType;												// 0:일반유저, 1:운영자

	SI32				siVillageUnique;

	SI32				siSenderPersonID;										// 송신자 person id
	
	SI32				siReceiverPersonID;										// 수신자 person id (0이면 name으로 검색)	
	TCHAR				szReceiverPersonName[ MAX_PLAYER_NAME ];				// 수신자 name
	
	GMONEY				siMoney;												// 보낼 금액 
	cltDate				clDate;													// 보내는 시간 
	GMONEY				siFee;
	
	sDBRequest_PostOfficeSendMoney( UI16 id, SI32 posttype, SI32 villageUnique, SI32 senderPersonID, SI32 receiverPersonID, TCHAR *receiverPersonName, 
		GMONEY money, cltDate *pclDate, GMONEY fee )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_SENDMONEY;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeSendMoney );

		usCharID = id;

		siPostType = posttype;
		siVillageUnique = villageUnique;
		siSenderPersonID = senderPersonID;

		siReceiverPersonID = receiverPersonID;

		if( receiverPersonName ) {		
			memcpy( szReceiverPersonName, receiverPersonName, MAX_PLAYER_NAME );
			szReceiverPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
		} else {
			szReceiverPersonName[ 0 ] = NULL;
		}

		siMoney = money;		
		clDate.Set( pclDate );
		siFee = fee;
	}
	
};

class sDBResponse_PostOfficeSendMoney
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;												// 결과

	SI32				siPostType;												// 0:일반유저, 1:운영자

	SI32				siVillageUnique;										// 마을유니크
	
	SI32				siSenderPersonID;										// 송신자 personid
	SI32				siReceiverPersonID;										// 수신자 personid

	cltMoney			clPostOfficeMoney;										// 역참 보유자금
	cltMoney			clPersonMoney;											// 송신자 소지금 정보
	
	//[추가 : 황진성 2007. 10. 25 로그에 이용. 송금시 송금액과 수수료 정보.]
	GMONEY				siMoney;												// 보낼 금액 
	GMONEY				siFee;

	sDBResponse_PostOfficeSendMoney()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeSendMoney ) );
	}
};



// 메시지 전달을 요청한다
class sDBRequest_PostOfficeSendMsg
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPostType;												// 0:일반유저, 1:운영자

	SI32				siVillageUnique;

	SI32				siSenderPersonID;										// 송신자 person id

	SI32				siReceiverPersonID;										// 수신자 person id (0이면 name으로 검색)
	TCHAR				szReceiverPersonName[ MAX_PLAYER_NAME ];				// 수신자 name
	TCHAR				szMsg[ MAX_POSTOFFICE_MSG_SIZE ];						// 메시지
	cltDate				clDate;													// 보내는 시간 
	GMONEY				siFee;
	
	sDBRequest_PostOfficeSendMsg( UI16 id, SI32 posttype, SI32 villageUnique, SI32 senderPersonID, 
		SI32 receiverPersonID, TCHAR *receiverPersonName, 
		TCHAR *szmsg, cltDate *pclDate, GMONEY fee )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_SENDMSG;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeSendMsg );

		usCharID = id;

		siPostType = posttype;
		siVillageUnique = villageUnique;
		siSenderPersonID = senderPersonID;

		siReceiverPersonID = receiverPersonID;

		if( receiverPersonName ) {
			memcpy( szReceiverPersonName, receiverPersonName, MAX_PLAYER_NAME );
			szReceiverPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
		} else {
			szReceiverPersonName[ 0 ] = NULL;
		}

		memcpy( szMsg, szmsg, MAX_POSTOFFICE_MSG_SIZE );
		szMsg[ MAX_POSTOFFICE_MSG_SIZE - 1 ] = NULL;
		
		clDate.Set( pclDate );
		siFee = fee;
	}
	
};

class sDBResponse_PostOfficeSendMsg
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;												// 결과

	SI32				siPostType;												// 0:일반유저, 1:운영자

	SI32				siVillageUnique;										// 마을유니크
	
	SI32				siSenderPersonID;										// 송신자 personid
	SI32				siReceiverPersonID;										// 수신자 personid

	cltMoney			clPostOfficeMoney;										// 역참 보유자금
	cltMoney			clPersonMoney;											// 송신자 소지금 정보
	
	// PCK : 로그용 패킷 추가
	GMONEY				siFee;													// 수수료
	TCHAR				szMsg[MAX_POSTOFFICE_MSG_SIZE];							// 보내는 메시지

	sDBResponse_PostOfficeSendMsg()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeSendMsg ) );
	}
};

// 수신 아이템 목록을 요청한다
class sDBRequest_PostOfficeRecvItemList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;

	sDBRequest_PostOfficeRecvItemList( UI16 id, SI32 villageunique, SI32 personid )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_RECVITEMLIST;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeRecvItemList );

		usCharID = id;

		siVillageUnique = villageunique;
		siPersonID = personid;
	}
	
};

class sDBResponse_PostOfficeRecvItemList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siPersonID;

	UI16				usCount;
	cltPostOfficeRecvItemInfo			clRecvItemInfo[ MAX_POSTOFFICE_RECV_NUMBER ];

	sDBResponse_PostOfficeRecvItemList()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeRecvItemList ) );
	}

	SI32 GetTotalSize() {
		return sizeof( packetHeader ) + 24 + (sizeof( cltPostOfficeRecvItemInfo ) * usCount );
	}
	
};

// 수신 현금 목록을 요청한다
class sDBRequest_PostOfficeRecvMoneyList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;

	sDBRequest_PostOfficeRecvMoneyList( UI16 id, SI32 villageunique, SI32 personid )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_RECVMONEYLIST;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeRecvMoneyList );

		usCharID = id;

		siVillageUnique = villageunique;
		siPersonID = personid;
	}
	
};

class sDBResponse_PostOfficeRecvMoneyList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siPersonID;

	UI16				usCount;
	cltPostOfficeRecvMoneyInfo			clRecvMoneyInfo[ MAX_POSTOFFICE_RECV_NUMBER ];

	sDBResponse_PostOfficeRecvMoneyList()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeRecvMoneyList ) );
	}

	SI32 GetTotalSize() {
		return sizeof( packetHeader ) + 24 + (sizeof( cltPostOfficeRecvMoneyInfo ) * usCount );
	}
	
};


// 수신 메시지 목록을 요청한다
class sDBRequest_PostOfficeRecvMsgList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siVillageUnique;
	SI32				siPersonID;

	sDBRequest_PostOfficeRecvMsgList( UI16 id, SI32 villageunique, SI32 personid )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_RECVMSGLIST;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeRecvMsgList );

		usCharID = id;

		siVillageUnique = villageunique;
		siPersonID = personid;
	}

};


class sDBResponse_PostOfficeRecvMsgList
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siVillageUnique;
	SI32				siPersonID;

	UI16				usCount;
	cltPostOfficeRecvMsgInfo			clRecvMsgInfo[ MAX_POSTOFFICE_RECV_NUMBER ];

	sDBResponse_PostOfficeRecvMsgList()
	{

	}

	SI32 GetTotalSize() {
		return sizeof( packetHeader ) + 24 + (sizeof( cltPostOfficeRecvMsgInfo ) * usCount );
	}


};


// 수신 아이템을 내 인벤토리로 넣는다
class sDBRequest_PostOfficeGetItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siMsgID;

	SI16				siInventoryIndex;							// 인벤토리 위치
	BOOL				bPile;										// 중첩될수 있는가



	sDBRequest_PostOfficeGetItem( UI16 id, SI32 personid, SI32 msgid, SI16 inventoryindex, BOOL bpile )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_GETITEM;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeGetItem );

		usCharID = id;

		siPersonID = personid;
		siMsgID = msgid;

		siInventoryIndex = inventoryindex;
		bPile = bpile;

	}
};

class sDBResponse_PostOfficeGetItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	
	SI32				siResult;

	SI32				siPersonID;
	SI32				siMsgID;

	SI16				siItemInventoryIndex;
	cltItem				clItem;

	SI32				siSenderPersonID;
	TCHAR				szRecverName[ MAX_PLAYER_NAME ];

	SI16				siRecvItemNum;

	sDBResponse_PostOfficeGetItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeGetItem ) );
	}

};

class sDBRequest_PostOfficeGetMoney
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siMsgID;

	sDBRequest_PostOfficeGetMoney( UI16 id, SI32 personid, SI32 msgid )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_GETMONEY;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeGetMoney );

		usCharID = id;

		siPersonID = personid;
		siMsgID = msgid;
	}
};

class sDBResponse_PostOfficeGetMoney
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	
	SI32				siResult;

	SI32				siPersonID;
	SI32				siMsgID;

	GMONEY				siPersonMoney;			// 수신자의 소지금

	SI32				siSenderPersonID;
	TCHAR				szRecverName[ MAX_PLAYER_NAME ];

	sDBResponse_PostOfficeGetMoney()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeGetMoney ) );
	}
};

// 읽은 메시지를 지운다
class sDBRequest_PostOfficeDelMsg
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siMsgID;


	sDBRequest_PostOfficeDelMsg( UI16 id, SI32 personid, SI32 msgid )
	{
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_DELMSG;
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeGetItem );

		usCharID = id;

		siPersonID = personid;
		siMsgID = msgid;
	}
};


 
// 역참 수수료 설정
class sDBRequest_PostOfficeSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI32				siVillageUnique;			// 역참이 위치한 곳의 VillageUnique

	cltFee				clMoneyFee;					// 송금 수수료 
	cltFee				clItemFee;					// 물품 전달 수수료 

	GMONEY				siMsgFee;					// 메시지 전송 수수료 (건당)
	GMONEY				siRecvFee;					// 수신함 열람 수수료 (회당)


	sDBRequest_PostOfficeSetFee( SI32 id, SI32 personid, SI32 villageunique, 
		cltFee* pclMoneyFee, cltFee* pclItemFee, GMONEY msgfee, GMONEY recvfee )
	{
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeSetFee );
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_SETFEE;

		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;

		clMoneyFee.Set( pclMoneyFee );
		clItemFee.Set( pclItemFee );

		siMsgFee = msgfee;
		siRecvFee = recvfee;
	}
};


class sDBResponse_PostOfficeSetFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	SI32				siPersonID;
	SI32				siVillageUnique;			// 역참이 위치한 마을의 VillageUnique

	cltFee				clMoneyFee;					// 송금 수수료 
	cltFee				clItemFee;					// 물품 전달 수수료 

	GMONEY				siMsgFee;					// 메시지 전송 수수료 (건당)
	GMONEY				siRecvFee;					// 수신함 열람 수수료 (회당)

	sDBResponse_PostOfficeSetFee()
	{
		ZeroMemory( this, sizeof(sDBResponse_PostOfficeSetFee) );
	}

};

class sDBRequest_PostOfficeGetRecord
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;					// 요청자 personid 

	sDBRequest_PostOfficeGetRecord( SI32 id, SI32 personid )
	{
		packetHeader.usSize = sizeof( sDBRequest_PostOfficeGetRecord );
		packetHeader.usCmd = DBMSG_REQUEST_POSTOFFICE_GETRECORD;

		usCharID		= id;
		siPersonID		= personid;
	}

};

class sDBResponse_PostOfficeGetRecord
{
public:
	sPacketHeader				packetHeader;
	UI16						usCharID;

	SI32						siResult;

	SI32						siPersonID;

	UI16						usCount;
	cltPostOfficeRecordInfo		recordinfo[ MAX_POSTOFFICE_RECORD_NUMBER ];

	sDBResponse_PostOfficeGetRecord()
	{
		ZeroMemory( this, sizeof( sDBResponse_PostOfficeGetRecord ) );
	}

	SI32 GetTotalSize() {
		return sizeof( packetHeader ) + 12 + (sizeof( cltPostOfficeRecordInfo ) * usCount );
	}

};

// 운영자에게 보관될 유저의 물품 정보( 일단은 봉이김선달이 보관한다 )
class sDBRequest_Get_SaveUserItem
{
public:
	sPacketHeader		packetHeader;
	
	SI16				siCharId;
	SI32				siSaveItemPersonID;		// 아이템 보관 받을 person id
	SI16				siIndex;				// 받아올 아이템 인덱스		
	SI16				siItemPos;				// 아이템 위치

	bool				bAddItem;				// 합쳐질 것인가?
		
	SI64				siServiceCharge;		// 수수료

	sDBRequest_Get_SaveUserItem( SI16 charid, SI32 personid, SI16 index, SI16 pos, bool bdd, SI64 servicecharge )
	{

		packetHeader.usCmd = DBMSG_REQUEST_GET_SAVEUSERITEM;
		packetHeader.usSize = sizeof( sDBRequest_Get_SaveUserItem );

		siCharId = charid;
		siSaveItemPersonID = personid;
		siIndex = index;
		siItemPos = pos;

		bAddItem = bdd;
		
		siServiceCharge = servicecharge;

	}
};

class sDBResponse_Get_SaveUserItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI16				siCharId;
	SI32				siSaveItemPersonID;		// 아이템 보관 받을 person id
	SI16				siIndex;				// 아이템 인덱스
	SI16				siItemPos;				// 아이템 위치
	SI64				siUserMoney;
	
	cltItem				clItem;

	sDBResponse_Get_SaveUserItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_Get_SaveUserItem ) );
	}
};

//KHY - 1127 - 클라라 물품 보관 개선
class sDBRequest_Get_SaveUserHorse
{
public:
	sPacketHeader		packetHeader;
	
	SI16				siCharId;
	SI32				siSaveItemPersonID;		// 아이템 보관 받을 person id

	SI16				siIndex;				// 받아올 기승돌물의  인덱스		
	SI32				siEnptyHorseindex;	// 들어갈 위치.		
	
	SI64				siServiceCharge;		// 수수료

	sDBRequest_Get_SaveUserHorse( SI16 charid, SI32 personid, SI16 index, SI32 enptyHorseindex,  SI64 servicecharge )
	{

		packetHeader.usCmd = DBMSG_REQUEST_GET_SAVEUSERHORSE;
		packetHeader.usSize = sizeof( sDBRequest_Get_SaveUserHorse );

		siCharId = charid;
		siSaveItemPersonID = personid;
		siIndex = index;
		siEnptyHorseindex = enptyHorseindex;
		
		siServiceCharge = servicecharge;

	}
};
class sDBResponse_Get_SaveUserHorse
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI16				siCharId;
	SI32				siSaveHorsePersonID;		// 기승동물 보관 받을 person id
	SI16				siIndex;				// 기승동물 인덱스
	SI16				siEnptyHorseindex;				//  기승동물 위치
	SI64				siUserMoney;
	
	cltHorse			clHorse;

	sDBResponse_Get_SaveUserHorse()
	{
		ZeroMemory( this, sizeof( sDBResponse_Get_SaveUserHorse ) );
	}
};

// 운영자에게 보관된 유저의 물품 리스트 얻어오기( 일단은 봉이김선달이 보관한다 )
class sDBRequest_Get_SaveUserItemList 
{
public:
	sPacketHeader		packetHeader;

	SI16				siCharId;
	SI32				siSaveItemPersonID;

	sDBRequest_Get_SaveUserItemList(SI16 charid, SI32 personid)
	{
		packetHeader.usCmd  = DBMSG_REQUEST_GET_SAVEUSERITEMLIST;
		packetHeader.usSize = sizeof( sDBRequest_Get_SaveUserItemList );

		siCharId = charid;
		siSaveItemPersonID = personid;
	}
};

class sDBResponse_Get_SaveUserItemList
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;						// 결과값.

	SI16				siCharId;
	SI32				siSaveItemPersonID;
	
	cltSaveUersItemInfo	clSaveUsersItemInfo[MAX_SAVEUSERITEM_LIST];

	//KHY - 1127 - 클라라 물품 보관 개선
	cltSaveUersHorseInfo	clSaveUsersHorseInfo[MAX_SAVEUSERHORSE_LIST];

	sDBResponse_Get_SaveUserItemList() 
	{
		ZeroMemory(this, sizeof(sDBResponse_Get_SaveUserItemList));
	}
};


enum NAME_CHECK_REASON
{ 
	POSTOFFICE_ADD_RECV_USER,						// 우체국에서 메시지를 전달할 유저 추가를 위해.
};

class sDBRequest_Char_Name_Check
{
public:
    sPacketHeader		packetHeader;

	SI16				siCharId;
	SI16				siReason;								// 이름을 확인하는 이유.

	TCHAR				m_szCheckPersonName[MAX_PLAYER_NAME];

	sDBRequest_Char_Name_Check( SI16 CharId, SI16 Reason, TCHAR* _pCheckPersonName )
	{
		ZeroMemory(this, sizeof(sDBRequest_Char_Name_Check));

		packetHeader.usCmd  = DBMSG_REQUEST_CHAR_NAME_CHECK;
		packetHeader.usSize = sizeof( sDBRequest_Char_Name_Check );

		siCharId = CharId;
		siReason = Reason;
		memcpy( m_szCheckPersonName, _pCheckPersonName, sizeof(m_szCheckPersonName) );
	}
};

class sDBResponse_Char_Name_Check
{
public:
	sPacketHeader		packetHeader;

	SI16				siCharId;
	SI16				siReason;						// 이름을 확인하는 이유.
	SI16				CheckPersonLevel;				// 레벨.
	SI16				CheckPersonVillage;				// 마을 정보.
	cltSimplePerson		CheckPersonInfo;

	SI32				siResult;						// 결과값.
	

	sDBResponse_Char_Name_Check() 
	{
		ZeroMemory(this, sizeof(sDBResponse_Char_Name_Check));
	}
};


//----------------------------------------------------------------------------------------------
// 작성일 : 2008-8-6
//
// 그룹 리스트 요청. (친구, 길드, 주민 등..)	
//
// - 접속안된 관련 유저 정보를 얻기 어려워 DB에 필요한 정보 요청.
//
//----------------------------------------------------------------------------------------------
// 리스트 요청 이유.
enum GROUP_LIST_REQUEST_REASON
{
	POSTOFFICE_GOURP_MESSAGE = 1,			// 우체국의 단체 메시지에 이용.

};

//[진성] 길드, 친구, 주민등의 리스트를 요청할때 얻어올 유저 정보.
class clCharInfo
{
public:
	SI16	m_siLevel;
	SI16	m_siVillageUnique;
	SI32	m_siPersonID;
	TCHAR	m_PersonName[MAX_PLAYER_NAME];

	clCharInfo()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 친구 목록 요청.	=> 2008-8-
class sDBRequest_Friend_Info
{
public:
	sPacketHeader		m_packetHeader;

	SI08				m_siRequestReason;	//요청 이유.

	SI16				m_siCharId;
	SI32				m_siPersonID;

	sDBRequest_Friend_Info(SI08	_siRequestReason, SI16	_siCharId, SI32 _siPersonID)
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd  = DBMSG_REQUEST_FRIEND_INFO;
		m_packetHeader.usSize = sizeof( sDBRequest_Friend_Info );

		m_siRequestReason =	_siRequestReason;
		m_siCharId		  =	_siCharId;
		m_siPersonID	  =	_siPersonID;
	}
};


class sDBResponse_Friend_Info
{
public:
	sPacketHeader		m_packetHeader;

	SI08				m_siRequestReason;	//요청 이유.
	SI16				m_siCharId;
	clCharInfo			m_clCharInfo[MAX_FRIEND_NUMBER];

	sDBResponse_Friend_Info()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 길드 목록 요청.	=> 2008-8-
class sDBRequest_Guild_Info
{
public:
	sPacketHeader		m_packetHeader;

	SI08				m_siRequestReason;	//요청 이유.
	SI16				m_siCharId;
	SI32				m_GuildUnique;

	sDBRequest_Guild_Info(SI08	_siRequestReason, SI16	_siCharId, SI32 _GuildUnique)
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd  = DBMSG_REQUEST_GUILD_INFO;
		m_packetHeader.usSize = sizeof( sDBRequest_Guild_Info );

		m_siRequestReason =	_siRequestReason;
		m_siCharId		  =	_siCharId;
		m_GuildUnique	  =	_GuildUnique;
	}

};


class sDBResponse_Guild_Info
{
public:	
	sPacketHeader		m_packetHeader;

	SI08				m_siRequestReason;	//요청 이유.
	SI16				m_siCharId;
	clCharInfo			m_clCharInfo[MAX_GUILD_MEMBER_NUM];

	sDBResponse_Guild_Info()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 마을 주민 목록 요청.	=> 2008-8-
class sDBRequest_Village_Resident_Info
{
public:
	sPacketHeader		m_packetHeader;

    SI08				m_siRequestReason;	//요청 이유.
	SI16				m_siCharId;
	SI32				m_VillageUnique;

	sDBRequest_Village_Resident_Info(SI08	_siRequestReason, SI16	_siCharId, SI16 _VillageUnique)
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd  = DBMSG_REQUEST_VILLAGE_RESIDENT_INFO;
		m_packetHeader.usSize = sizeof( sDBRequest_Village_Resident_Info );

		m_siRequestReason =	_siRequestReason;
		m_siCharId		  =	_siCharId;
		m_VillageUnique	  =	_VillageUnique;
	}
};


class sDBResponse_Village_Resident_Info
{
public:
	sPacketHeader		m_packetHeader;

	SI08				m_siRequestReason;	//요청 이유.
	SI16				m_siCharId;
	clCharInfo			m_clCharInfo[MAX_RESIDENT_NUMBER];

	sDBResponse_Village_Resident_Info()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};



#endif
