/**
* @file rpcNbfpclient.h
* @brief 
* @author rapool <chankyylee@nhncorp.com>
* @date $Date: 2006-05-10$
* @version $Revision: $
* $Id: $
*/


#ifndef __MEGA_RPC_INBFPCLIENT_H_3ED5B414_2D1A_40D8_AA8A_EF5223D32A95__
#define __MEGA_RPC_INBFPCLIENT_H_3ED5B414_2D1A_40D8_AA8A_EF5223D32A95__

#include "common.h"
#include "rpcInterface.h"


namespace mega {
/**
 * @addtogroup nbfpInterface
 * @{
 */

#ifndef NULL
#define NULL 0
#endif

#ifdef _AFXEXT
#define EXPORT_TAG __declspec(dllexport)
#else
#define EXPORT_TAG
#endif


class INBFPClient;


/**
 * @brief INBFPClient 인스턴스 객체를 생성
 * @return 생성된 INBFPClient 객체의 포인터
 * @remark 자동으로 addRef()가 호출되므로 따로 호출할 필요가 없음
 * @remark !!! 다 쓴 후에 반드시 INBFPClient::release()를 호출해야 함 !!!
 */
EXPORT_TAG INBFPClient* CreateNBFPClientInstance();


/**
 * @brief IVariant객체를 생성
 * @return 생성된 IVariant 객체의 포인터
 * @remark 자동으로 addRef()가 호출되므로 따로 호출할 필요가 없음
 * @remark !!! 사용 후에 반드시 IVariant::release()를 호출해야 함 !!!
 */
EXPORT_TAG IVariant* CreateVariantInstance();

/**
 * @brief IList객체를 생성
 * @return 생성된 IList 객체의 포인터
 * @remark 자동으로 addRef()가 호출되므로 따로 호출할 필요가 없음
 * @remark !!! 사용 후에 반드시 IList::release()를 호출해야 함 !!!
 */
EXPORT_TAG IList* CreateListInstance();

/**
 * @brief IMap객체를 생성
 * @return 생성된 IMap 객체의 포인터
 * @remark 자동으로 addRef()가 호출되므로 따로 호출할 필요가 없음
 * @remark !!! 사용 후에 반드시 IMap::release()를 호출해야 함 !!!
 */
EXPORT_TAG IMap* CreateMapInstance();


/**
 * @brief 기본타입과 IList, IMap을 IVariant로 자동 변환해주는 클래스
 * @remark IVariant::setValue()에서 파라메터 자동 형변환 용도로만 쓰임
 */
class EXPORT_TAG VariantBoxing
{
	public:
		VariantBoxing(bool value);
		VariantBoxing(int value);
		VariantBoxing(int64_t value);
		VariantBoxing(double value);
		VariantBoxing(nano::timeval &value);
		VariantBoxing(const char *value);
		VariantBoxing(const IVariant *value);
		VariantBoxing(const IList *value);
		VariantBoxing(const IMap *value);
		~VariantBoxing();

	public:
		IVariant* getIVariant() const;

		int addRef();
		int release();

	private:
		VariantBoxing(const void *value) {}

		IVariant *_iv;
		int _refcnt;
};


/**
 * @brief NBFPResult 인터페이스 클래스
 */
class INBFPResult
{
	public:
		/** 
		 * @brief 요청에 대한 처리의 성공 여부
		 * @return true or false
		 */
		virtual bool isSuccess() const = 0;

		/**
		 * @brief 에러코드를 가져옴
		 * @return 에러코드값
		 */
		virtual int code() const = 0;

		/**
		 * @brief 에러 메시지를 가져옴
		 * @return 문자열 에러 메시지
		 */
		virtual const char* message() const = 0;

		/**
		 * @brief 에러의 상세 정보를 가져옴
		 * @return 에러정보
		 * @remark !!!리턴받은 IVariant*는 다 쓴 후 반드시 release()를 호출해야 함!!!
		 */
		virtual IVariant* detail() const = 0;

		/**
		 * @brief 리턴 object를 가져옴
		 * @return 리턴 object
		 * @remark !!!리턴받은 IList*는 다 쓴 후 반드시 release()를 호출해야 함!!!
		 */
		virtual IList* objects() const = 0;

	public:
		/**
		 * @brief 레퍼런스 카운터를 증가시킴
		 * @return 증가된 레퍼런스 카운터
		 */
		virtual unsigned short addRef() = 0;

		/**
		 * @brief 레퍼런스 카운터를 감소시킴
		 * @return 감소된 레퍼런스 카운터
		 * @remark 페퍼런스 카운터가 0이 되면 자동으로 객체는 소멸됨
		 */
		virtual unsigned short release() = 0;
};


/**
 * @brief NBFPClient 인터페이스 클래스
 */
class INBFPClient
{
public:
	/**
	 * @copydoc nano::NBFPClient::TransportType
	 */
	enum TransportType {
		TP_TCP = 0,
		TP_UDP = 1
	};


public:
	/**
	 * @brief NBFP 클라이언트 세션을 시작
	 * @param host 서버 주소
	 * @param port 서버 포트
	 * @param timeout Timeout (ms)
	 * @param tp TransportType
	 * @return true or false
	 */
	virtual bool open(const char *host, int port, int timeout = -1, INBFPClient::TransportType tp = TP_TCP) = 0;

	/**
	 * @brief NBFP 클라이언트 세션을 종료
	 */
	virtual void close() = 0;

	/**
	 * @brief timeout을 설정
	 * @param timeout millseconds
	 */
	virtual void setTimeout(int timeout = -1) = 0;

	/**
	 * @brief Namespace를 설정
	 * @param ns Namespace
	 * @remark 한번 설정된 Namespace는 세션이 끝날때까지 유지됨
	 */
	virtual void setNamespace(const char *ns) = 0;

	/**
	 * @brief Character set을 설정
	 * @param charscter set code (Microsoft code page 번호) 
	 * @remark 한번 설정된 Charset은 세션이 끝날때까지 유지됨
	 */
	virtual void setCharset(uint16_t charset) = 0;

	/**
	 * @brief Request ID 설정
	 * @param reqID Request ID
	 * @remark 한번 설정된 reqID는 세션이 끝날때까지 유지됨
	 */
	virtual void setReqID(int32_t reqID) = 0;

	/**
	 * @brief Result 얻기
	 * @return INBFPResult 값
	 * @remark exec()에서 리턴되는 값과 동일
	 * @remark release() 해 줄 필요 없음
	 */
	virtual INBFPResult* getLastResult() = 0;

	/**
	 * @brief 결과를 가져온다
	 * @return 요청에 대한 처리 결과
	 */
	virtual const INBFPResult* retval() = 0;

public:
		/**
		 * @brief 레퍼런스 카운트를 증가시킴
		 * @return 증가된 레퍼런스 카운트
		 */
		virtual size_t addRef() = 0;

		/**
		 * @brief 레퍼런스 카운트를 감소시킴
		 * @return 감소된 레퍼런스 카운트
		 * @remark 레퍼런스 카운트가 0이 되는 순간 자동 소멸됨
		 */
		virtual size_t release() = 0;

public:
	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 0개 전용)
	 * @param method Method 이름
	 * @param params Parameter List
	 * @param size params number
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* exec(const char *method, IList &params, size_t size) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 0개 전용)
	 * @param method Method 이름
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* exec(const char* method) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 1개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 2개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 3개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 4개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 5개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 6개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 7개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 8개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 9개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 10개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 11개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 12개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 13개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 14개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 15개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 16개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 17개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 18개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @param param18 Paramter #18
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17,
		const VariantBoxing& param18) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 19개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @param param18 Paramter #18
	 * @param param19 Paramter #19
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17,
		const VariantBoxing& param18,
		const VariantBoxing& param19) = 0;

	/**
	 * @brief Method 호출 + Reply 읽기 (Parameter 20개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @param param18 Paramter #18
	 * @param param19 Paramter #19
	 * @param param20 Paramter #20
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* INBFPClient::exec(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17,
		const VariantBoxing& param18,
		const VariantBoxing& param19,
		const VariantBoxing& param20) = 0;

	/**
	 * @brief Method 호출 (Parameter 0개 전용)
	 * @param method Method 이름
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(const char* method) = 0;

	/**
	 * @brief Method 호출 (Parameter 1개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1) = 0;

	/**
	 * @brief Method 호출 (Parameter 2개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2) = 0;

	/**
	 * @brief Method 호출 (Parameter 3개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3) = 0;

	/**
	 * @brief Method 호출 (Parameter 4개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4) = 0;

	/**
	 * @brief Method 호출 (Parameter 5개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5) = 0;

	/**
	 * @brief Method 호출 (Parameter 6개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6) = 0;

	/**
	 * @brief Method 호출 (Parameter 7개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7) = 0;

	/**
	 * @brief Method 호출 (Parameter 8개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8) = 0;

	/**
	 * @brief Method 호출 (Parameter 9개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9) = 0;

	/**
	 * @brief Method 호출 (Parameter 10개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10) = 0;

	/**
	 * @brief Method 호출 (Parameter 11개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11) = 0;

	/**
	 * @brief Method 호출 (Parameter 12개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12) = 0;

	/**
	 * @brief Method 호출 (Parameter 13개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13) = 0;

	/**
	 * @brief Method 호출 (Parameter 14개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14) = 0;

	/**
	 * @brief Method 호출 (Parameter 15개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15) = 0;

	/**
	 * @brief Method 호출 (Parameter 16개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16) = 0;

	/**
	 * @brief Method 호출 (Parameter 17개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17) = 0;

	/**
	 * @brief Method 호출 (Parameter 18개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @param param18 Paramter #18
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17,
		const VariantBoxing& param18) = 0;

	/**
	 * @brief Method 호출 (Parameter 19개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @param param18 Paramter #18
	 * @param param19 Paramter #19
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17,
		const VariantBoxing& param18,
		const VariantBoxing& param19) = 0;

	/**
	 * @brief Method 호출 (Parameter 20개 전용)
	 * @param method Method 이름
	 * @param param1 Paramter #1
	 * @param param2 Paramter #2
	 * @param param3 Paramter #3
	 * @param param4 Paramter #4
	 * @param param5 Paramter #5
	 * @param param6 Paramter #6
	 * @param param7 Paramter #7
	 * @param param8 Paramter #8
	 * @param param9 Paramter #9
	 * @param param10 Paramter #10
	 * @param param11 Paramter #11
	 * @param param12 Paramter #12
	 * @param param13 Paramter #13
	 * @param param14 Paramter #14
	 * @param param15 Paramter #15
	 * @param param16 Paramter #16
	 * @param param17 Paramter #17
	 * @param param18 Paramter #18
	 * @param param19 Paramter #19
	 * @param param20 Paramter #20
	 * @return 수행 결과에 대한 reference
	 */	 
	virtual const INBFPResult* call(
		const char* method,
		const VariantBoxing& param1,
		const VariantBoxing& param2,
		const VariantBoxing& param3,
		const VariantBoxing& param4,
		const VariantBoxing& param5,
		const VariantBoxing& param6,
		const VariantBoxing& param7,
		const VariantBoxing& param8,
		const VariantBoxing& param9,
		const VariantBoxing& param10,
		const VariantBoxing& param11,
		const VariantBoxing& param12,
		const VariantBoxing& param13,
		const VariantBoxing& param14,
		const VariantBoxing& param15,
		const VariantBoxing& param16,
		const VariantBoxing& param17,
		const VariantBoxing& param18,
		const VariantBoxing& param19,
		const VariantBoxing& param20) = 0;
};


/** @} */
} /*end of namespace mega*/


#endif
