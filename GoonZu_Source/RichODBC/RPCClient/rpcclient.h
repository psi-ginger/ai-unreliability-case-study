/**
 * @file rpcclient.h
 * @brief
 * @author rapool <chankyulee@nhncorp.com>
 * @date $Date: 2007-01-02$
 * @version $Revision: $
 * $Id: $
 */
#ifndef __RPCCLIENT_H_4FE157CD_5035_43D1_830E_A65D4E326825__
#define __RPCCLIENT_H_4FE157CD_5035_43D1_830E_A65D4E326825__

#include "common.h"
#include "rpcNbfpClient.h"

namespace mega {
/**
 * @addtogroup megarpc
 * @{
 */


class IRPCClient;
class IRPCConnector;
class IVariant;
class IList;
class ITransport;

/**
 * @brief IRPCClient의 객체를 만들어서 리턴한다
 * @return 만들어진 IRPCClient의 object pointer
 * @remark 사용이 끝난 IRPCClient는 delete를 이용하여 해제해 주어야 한다
 */
IRPCClient* CreateRPCClient();

/**
 * @brief IRPCConnector객체를 만들어서 리턴한다
 * @return 만들어진 IRPCConnector object pointer
 * @remark 사용이 끝난 IRPCConnector는 delete를 이용하여 해제해 주어야 한다
 */
IRPCConnector* CreateRPCConnector();


/**
 * @brief IRPCConnector
 */
class IRPCConnector
{
public:
	enum TransportType
	{
		TP_TCP = 0,
		TP_UDP
	};

public:
	virtual ~IRPCConnector() {}

	/**
	 * @brief RPC 클라이언트 세션을 시작한다.
	 *
	 * @param host 서버 host
	 * @param port 서버 Port
	 * @param timeout Timeout (ms)
	 * @param tp Transport Type( IRPCConnector::TP_TCP, IRPCConnector::TP_UDP )
	 * @return 성공시 true, 실패시 false
	 */
	virtual bool open(IRPCClient *client, const char *host, int timeout = -1, TransportType tp = TP_TCP) = 0;

	/*!
	 * @brief 디렉토리 서비스를 제공하는 서버의 정보를 입력한다.
	 * 
	 * @param addr 디렉토리 서비스 서버 주소
	 * @param port 디렉토리 서비스 서버 포트
	 * @param isPrimary primary이면 true, secondary이면 false
	 * @param timeout Timeout(ms)
	 * @param tp Transport Type
	 */
	virtual void setDirectoryServerInformation(const char* addr, int port, bool isPrimary, int timeout = -1, TransportType tp = TP_TCP) = 0;

	/**
	 * @brief 에러코드를 가져온다
	 * @return 발생한 에러코드
	 */
	virtual int errorCode() = 0;

	/**
	 * @brief 에러메세지를 가져온다
	 * @return 에러메세지
	 */
	virtual const char* errorMessage() = 0;
};
/* end of class IRPCConnector */


/**
 * @brief IRPCResult 정보를 가지고 있는 클래스 인터페이스
 * @remark IRPCClient를 통하여 결과를 리턴받을 때 사용한다
 */
class IRPCResult
{
public:
	/**
	 * @brief 에러코드를 얻는다
	 * @return 에러코드
	 */
	virtual int errorCode() const = 0;

	/**
	 * @brief 에러메세지를 얻는다
	 * @return 에러메세지
	 */
	virtual const char* errorMessage() const = 0;

	/**
	 * @brief 상세 에러 정보를 얻는다
	 * @return 상세 에러 정보
	 * @remark !!! 사용 후 반드시 IVariant::relase()를 호출해야 함 !!!
	 */
	virtual IVariant* errorDetail() = 0;

	/**
	 * @brief Return objects를 얻는다
	 * @return Return objects list
	 * @remark !!! 사용 후 반드시 IList::relase()를 호출해야 함 !!!
	 */
	virtual const IList* objects() const = 0;

	/**
	 * @brief reply로 온 transport를 얻는다.
	 * @return Transport object
	 */
	virtual ITransport* getTransport() = 0;

	/**
	 * @brief Call 성공 여부
	 * @return 성공인 경우 true, 실패인 경우 false
	 * @remark Stream에도 에러가 없고, application 에서도 SUCCESS를 리턴한 경우에만 성공으로 간주된다
	 */
	virtual bool isSuccess() const = 0;
};
/* end of class IRPCResult */


/**
 * @brief Transport class
 */
class ITransport
{
public:
	/**
	 * @brief Option의 타입
	 */
	enum OptionType
	{
		OPT_PAYLOAD_SIZE            = 0,    /**< Payload: 길이 */
		OPT_PAYLOAD_TYPE            = 1,    /**< Payload: 타입 */
		OPT_PAYLOAD_ENCRYPTION      = 2,    /**< Payload: Encryption scheme */
		OPT_PAYLOAD_FRAGMENTATION   = 3,    /**< Payload: Fragmentation 정보 */
		OPT_PAYLOAD_CHECKSUM        = 4,    /**< Payload: Checksum */
		OPT_PAYLOAD_LOCALE          = 5,    /**< Payload: Locale */
		OPT_SRC_ADDR                = 6,    /**< Routing: Source Address */
		OPT_DEST_ADDR               = 7,    /**< Routing: Destination Address */
		OPT_TTL                     = 8,    /**< Routing: TTL */
		OPT_TRANSPORT_CONTROL       = 9,    /**< Transrpot Control */
		OPT_TRANSACTION_ID          = 10,   /**< Transaction ID */
		OPT_PEER_ID                 = 11,   /**< Peer ID */
		OPT_PEER_NAME               = 12,   /**< Peer Name */
		OPT_NAMESPACE               = 13,   /**< Namespace */
		OPT_CONTINUED               = 31,   /**< (not used) */
		OPT_USER_DEFINED            = -1,   /**< User-defined Option */
	};

	/**
	 * @brief Protocol Version
	 */
	struct Version
	{
		uint8_t major;      /**< Major version */
		uint8_t minor;      /**< Minor version */

		/**
		 * @brief Set version via uint_t
		 * @param version version
		 */
		void set(uint8_t version)
		{
			major = ( version >> 4 ) & 0x0F;
			minor = version & 0x0F;
		}

		/**
		 * @brief Get version via uint8_t
		 * @return version
		 */
		uint8_t get() const
		{
			return ( ( major << 4 ) + minor );
		}
	};

	virtual ~ITransport() {}

	/**
	 * @brief 내부 상태, option 설정 등을 모두 초기화
	 */
	virtual void reset() = 0;

	/**
	 * @brief 버전을 얻는다.
	 * @return version or NULL(ERROR)
	 */
	virtual const Version* version() const = 0;

	/**
	 * @brief Transport의 크기르 얻는다.
	 * @return Transport의 크기
	 * @remark read() 한 경우에는 network로 읽어들인 크기를, write()할 경우에는 write될 크기를 return한다.
	 */
	virtual uint16_t size() const = 0;

	/**
	 * @brief 특정 option이 존재하는지 알려준다.
	 * @param ot Option Type
	 * @param user_idx ot = OPT_USER_DEFINED인 경우 인덱스 (그 외의 경우에는 무시된다)
	 * @return 해당 option이 존재하는 경우 true, 존재하지 않는 경우 false
	 */
	virtual bool hasOption(OptionType ot, size_t user_idx = 0) const = 0;

	/**
	 * @brief 특정 option의 byte 길이를 얻는다.
	 * @param ot Option Type (그 외의 경우에는 무시된다)
	 * @param user_idx ot = OPT_USER_DEFINED인 경우 인덱스
	 * @return 해당 option이 존재하는 경우 true, 존재하지 않는 경우 false
	 */
	virtual size_t getOptionSize(OptionType ot, size_t user_idx = 0) const = 0;

	/**
	 * @brief namespace 사용여부
	 */
	virtual bool hasNamespace() const = 0;

	/**
	 * @brief Payload locale 사용여부
	 */
	virtual bool hasPayloadLocale() const = 0;

	/**
	 * @brief Transaction ID 사용여부
	 */
	virtual bool hasTransactionID() const = 0;

	/**
	 * @brief PayloadSize option을 얻는다.
	 * @return Payload size
	 */
	virtual uint32_t getPayloadSize() const = 0;

	/**
	 * @brief PayloadType option을 얻는다.
	 * @param size 문자열의 길이를 받을 포인터. 받지 않으려면 NULL을 지정하거나 지정하지 않으면 된다.
	 * @return Payload type
	 */
	virtual const char* getPayloadType( size_t* size = NULL ) const = 0;

	/**
	 * @brief PayloadLocale option을 얻는다.
	 * @return Payload locale
	 */
	virtual uint16_t getPayloadLocale() const = 0;

	/**
	 * @brief SourceAddress option을 얻는다.
	 * @param size 문자열의 길이를 받을 포인터. 받지 않으려면 NULL을 지정하거나 지정하지 않으면 된다.
	 * @return Source address
	 */
	virtual const char* getSrcAddr( size_t* size = NULL ) const = 0;

	/**
	 * @brief DestinationAddress option을 얻는다.
	 * @param size 문자열의 길이를 받을 포인터. 받지 않으려면 NULL을 지정하거나 지정하지 않으면 된다.
	 * @return Destination address
	 */
	virtual const char* getDestAddr( size_t* size = NULL ) const = 0;

	/**
	 * @brief TransactionID option을 얻는다.
	 * @return Transaction ID
	 */
	virtual uint32_t getTransactionID() const = 0;

	/**
	 * @brief PeerID option을 얻는다.
	 * @return Peer ID
	 */
	virtual uint32_t getPeerID() const = 0;

	/**
	 * @brief PeerName option을 얻는다.
	 * @param size 문자열의 길이를 받을 포인터. 받지 않으려면 NULL을 지정하거나 지정하지 않으면 된다.
	 * @return Peer name
	 */
	virtual const char* getPeerName( size_t* size = NULL ) const = 0;

	/**
	 * @brief Namespace option을 얻는다.
	 * @param size 문자열의 길이를 받을 포인터. 받지 않으려면 NULL을 지정하거나 지정하지 않으면 된다.
	 * @return Namespace
	 */
	virtual const char* getNamespace( size_t* size = NULL ) const = 0;

public:
	/**
	 * @brief PayloadSize를 설정한다.
	 * @param opt Payload size
	 */
	virtual void setPayloadSize( uint32_t opt ) = 0;

	/**
	 * @brief PayloadType을 설정한다.
	 * @param opt Payload type
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setPayloadType( const char* opt, size_t size = (size_t)-1 ) = 0;

	/**
	 * @brief PayloadLocale을 설정한다.
	 * @param opt Payload locale
	 */
	virtual void setPayloadLocale( uint16_t opt ) = 0;

	/**
	 * @brief SourceAddress를 설정한다.
	 * @param opt Source address
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setSrcAddr( const char* opt, size_t size = (size_t)-1 ) = 0;

	/**
	 * @brief DestinationAddress를 설정한다.
	 * @param opt Destination address
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setDestAddr( const char* opt, size_t size = (size_t)-1 ) = 0;

	/**
	 * @brief TransactionID를 설정한다.
	 * @param opt Transaction ID
	 */
	virtual void setTransactionID( uint32_t opt ) = 0;

	/**
	 * @brief PeerID를 설정한다.
	 * @param opt Peer ID
	 */
	virtual void setPeerID( uint32_t opt ) = 0;

	/**
	 * @brief PeerName을 설정한다.
	 * @param opt Peer name
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setPeerName( const char* opt, size_t size = (size_t)-1 ) = 0;

	/**
	 * @brief Namespace를 설정한다.
	 * @param opt Namespace
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setNamespace( const char* opt, size_t size = (size_t)-1 ) = 0;

public:
	/**
	 * @brief 문자열로 option 얻기
	 * @param ot Option type
	 * @param size 문자열의 길이를 저장할 포인터. 받지 않으려면, 지정하지 않거나 NULL을 지정하면 된다.
	 * @return 해당 option의 const char* 포인터. option이 존재하지 않으면 NULL
	 */
	virtual const char* getOption( OptionType ot, size_t* size = NULL ) const = 0;

public:
	/**
	 * @brief 문자열로 option 지정
	 * @param ot Option type
	 * @param opt Option
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setOption( OptionType ot, const char* opt, size_t size = (size_t)-1 ) = 0;

public:
	/**
	 * @brief 문자열로 user-defined option 얻기
	 * @param idx Option index
	 * @param size 문자열의 길이를 저장할 포인터. 받지 않으려면, 지정하지 않거나 NULL을 지정하면 된다.
	 * @return 해당 option의 const char* 포인터. option이 존재하지 않으면 NULL
	 */
	virtual const char* getUserOption( size_t idx, size_t* size = NULL ) const = 0;

public:
	/**
	 * @brief 문자열로 user-defined option 지정
	 * @param idx Option idx
	 * @param opt Option
	 * @param size 문자열의 길이. 지정하지 않으면 내부에서 strlen(3)으로 계산한다.
	 */
	virtual void setUserOption( size_t idx, const char* opt, size_t size = (size_t)-1 ) = 0;

public:
	/**
	 * @brief 특정 option 설정을 해제한다.
	 * @param ot Option type
	 * @param user_idx ot = OPT_USER_DEFINED인 경우 index. 그 외에는 무시된다.
	 */
	virtual void clearOption( OptionType ot, size_t user_idx = 0 ) = 0;

};
/* end of class ITransport */


/**
 * @brief IRPCClient
 */
class IRPCClient
{
public:
	virtual ~IRPCClient() {}

	/**
	 * @brief Transport를 가져온다
	 * @return Transport 인터페이스
	 */
	virtual ITransport* getTransport() = 0;

	/**
	 * @brief RPC 클라이언트 세션을 시작한다.
	 * @param host 서버 host
	 * @param port 서버 Port
	 * @param timeout Timeout (ms)
	 * @param tp Transport Type( IRPCConnector::TP_TCP, IRPCConnector::TP_UDP )
	 * @return 성공시 true, 실패시 false
	 */
	virtual bool open( const char* host, int port, int timeout = -1, IRPCConnector::TransportType tp = IRPCConnector::TP_TCP ) = 0;

public:
	/**
	 * @brief IRPCResult를 얻는다
	 * @return IRPCResult
	 */
	virtual const IRPCResult* getLastResult() = 0;

	/**
	 * @brief reply 읽기
	 * @return 수행 결과에 대한 reference
	 */
	virtual bool reply() = 0;

public:
	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param params 전달하고자 하는 파라메터의 리스트
	 * @param size params 갯수
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, IList *params, size_t size) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @param param1 Paramter #18
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17,
		const VariantBoxing &param18) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @param param1 Paramter #18
	 * @param param1 Paramter #19
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17,
		const VariantBoxing &param18,
		const VariantBoxing &param19) = 0;

	/**
	 * @brief 서버에 Request를 보낸다
	 * @param Method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @param param1 Paramter #18
	 * @param param1 Paramter #19
	 * @param param1 Paramter #20
	 * @return 성공여부
	 * @remark 성공한 결과는 reply()를 이용해서 받을 수 있다
	 */
	virtual bool call(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17,
		const VariantBoxing &param18,
		const VariantBoxing &param19,
		const VariantBoxing &param20) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 1개 전용)
	 * @param method Method 이름
	 * @param params 전달하고자 하는 파라메터의 리스트
	 * @param size params 갯수
	 * @return 수행 결과에 대한 result 
	 */
	virtual const IRPCResult* exec(const char* method, IList *params, size_t size) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 0개 전용)
	 * @param method Method 이름
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char* method) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 1개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 2개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 3개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 4개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 5개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 6개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 7개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 8개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 9개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 10개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 11개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 12개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 13개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 14개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 15개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 16개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 17개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 18개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @param param1 Paramter #18
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17,
		const VariantBoxing &param18) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 19개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @param param1 Paramter #18
	 * @param param1 Paramter #19
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17,
		const VariantBoxing &param18,
		const VariantBoxing &param19) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 20개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param1 Paramter #2
	 * @param param1 Paramter #3
	 * @param param1 Paramter #4
	 * @param param1 Paramter #5
	 * @param param1 Paramter #6
	 * @param param1 Paramter #7
	 * @param param1 Paramter #8
	 * @param param1 Paramter #9
	 * @param param1 Paramter #10
	 * @param param1 Paramter #11
	 * @param param1 Paramter #12
	 * @param param1 Paramter #13
	 * @param param1 Paramter #14
	 * @param param1 Paramter #15
	 * @param param1 Paramter #16
	 * @param param1 Paramter #17
	 * @param param1 Paramter #18
	 * @param param1 Paramter #19
	 * @param param1 Paramter #20
	 * @return 수행 결과에 대한 result
	 */
	virtual const IRPCResult* exec(const char *method, 
		const VariantBoxing &param1,
		const VariantBoxing &param2,
		const VariantBoxing &param3,
		const VariantBoxing &param4,
		const VariantBoxing &param5,
		const VariantBoxing &param6,
		const VariantBoxing &param7,
		const VariantBoxing &param8,
		const VariantBoxing &param9,
		const VariantBoxing &param10,
		const VariantBoxing &param11,
		const VariantBoxing &param12,
		const VariantBoxing &param13,
		const VariantBoxing &param14,
		const VariantBoxing &param15,
		const VariantBoxing &param16,
		const VariantBoxing &param17,
		const VariantBoxing &param18,
		const VariantBoxing &param19,
		const VariantBoxing &param20) = 0;
};
/* end of class IRPCClient */

/** @} */
} /* end of namespace mega */ 
#endif /*__RPCCLIENT_H_4FE157CD_5035_43D1_830E_A65D4E326825__*/
