// 다음 ifdef 블록은 DLL에서 내보내기하는 작업을 쉽게 해 주는 매크로를 만드는 
// 표준 방식입니다. 이 DLL에 들어 있는 파일은 모두 명령줄에 정의된 NHNCOINDLL_EXPORTS 기호로
// 컴파일되며, 동일한 DLL을 사용하는 다른 프로젝트에서는 이 기호를 정의할 수 없습니다.
// 이렇게 하면 소스 파일에 이 파일이 들어 있는 다른 모든 프로젝트에서는 
// NHNCOINDLL_API 함수를 DLL에서 가져오는 것으로 보고,
// 이 DLL은 해당 매크로로 정의된 기호가 내보내지는 것으로 봅니다.
#ifdef NHNCOINDLL_EXPORTS
#define NHNCOINDLL_API __declspec(dllexport)
#else
#define NHNCOINDLL_API __declspec(dllimport)
#endif


class NHNCoin;


extern "C" NHNCOINDLL_API NHNCoin* __cdecl CreateNHNCoinInstance(const char *serverIP, unsigned short serverPort, unsigned int timeout = -1);


// 이 클래스는 NHNCoinDll.dll에서 내보낸 것입니다.
class NHNCoin 
{
public:
	enum RET_CODE
	{
		RET_OK,					// 성공
		RET_ERR_INVALID_PARAM,	// 파라메터 에러
		RET_ERR_NETWORK,		// 네트워크 장애
		RET_ERR_SERVER,			// 서버 로직 장애
		RET_ERR_NO_DATA,		// QueryCoin()에서 해당 사용자 정보가 없음
	};

	/**
	 * @brief constructor
	 * @param serverIP SQLMap서버 IP주소
	 * @param serverPort SQLMap서버 포트
	 * @param timeout SQLMap서버의 실행 대기 시간
	 */
	NHNCoin(const char *serverIP, unsigned short serverPort, unsigned int timeout = -1);

	/**
	 * @brief destructor
	 */
	virtual ~NHNCoin();

	/**
	 * @brief 대상의 포인트를 조회
	 * @param id [IN] 사용자 아이디
	 * @param idno [IN] 사용자 번호
	 * @param rtn [OUT] 처리 결과 코드 (0: 성공, 그 외는 에러)
	 * @param msg [OUT] 에러 메시지 (Max 1024bytes)
	 * @param totbal [OUT] 총잔액 ( chrgbal+inevtbal+outevtbal+giftbal)
	 * @param chrgbal [OUT] 유료 충전잔액
	 * @param inevtbal [OUT] 내부이벤트 잔액
	 * @param outevtbal [OUT] 외부 이벤트 잔액
	 * @param giftbal [OUT] 선물받은 잔액
	 */
	virtual RET_CODE queryCoin(const char *id, const char *idno, int *rtn, char *msg, int *totbal, int *chrgbal, int *inevtbal, int *outevtbal, int *giftbal);

	/**
	 * @brief 대상의 포인트 차감
	 * @param id [IN] 사용자 아이디
	 * @param idno [IN] 사용자 번호
	 * @param prodkind [IN] 상품 분류 코드
	 * @param prodcode [IN] 상품 코드
	 * @param snm [IN] 상품 이름
	 * @param samt [IN] 상품 가격
	 * @param refkey [IN] 주문번호 (Max 20byte)
	 * @param ip [IN] 사용자 아이피 주소
	 * @param sid [OUT] 빌링주문코드 (Max 20byte)
	 * @param rtn [OUT] 처리 결과 코드 (0: 성공, 그 외는 에러)
	 * @param bal [OUT] 코인 잔액
	 * @param msg [OUT] 에러 메시지
	 */
	virtual RET_CODE useCoin(const char *id, const char *idno, const char *prodkind, const char *prodcode, const char *snm, int samt, const char *refkey, const char *ip, char *sid, int *rtn, int *bal, char *msg);

	/**
	 * @brief 선물 사용
	 * @param id [IN] 사용자 아이디
	 * @param idno [IN] 사용자 번호
	 * @param prodkind [IN] 상품 분류 코드
	 * @param prodcode [IN] 상품 코드
	 * @param snm [IN] 상품 이름
	 * @param samt [IN] 상품 가격
	 * @param refkey [IN] 주문번호 (Max 20byte)
	 * @param ip [IN] 사용자 아이피 주소
	 * @param sid [OUT] 빌링주문코드 (Max 20byte)
	 * @param rtn [OUT] 처리 결과 코드 (0: 성공, 그 외는 에러)
	 * @param bal [OUT] 코인 잔액
	 * @param msg [OUT] 에러 메시지
	 */
	virtual RET_CODE useCoinGift(const char *id, const char *idno, const char *prodkind, const char *prodcode, const char *snm, int samt, const char *refkey, const char *ip, char *sid, int *rtn, int *bal, char *msg);

	/**
	 * @brief 대상의 포인트 차감을 취소
	 * @param id [IN] 사용자 아이디
	 * @param idno [IN] 사용자 번호
	 * @param prodkind [IN] 상품 분류 코드
	 * @param prodcode [IN] 상품 코드
	 * @param snm [IN] 상품 이름
	 * @param samt [IN] 상품 가격
	 * @param refkey [IN] 주문번호 (Max 20byte)
	 * @param ip [IN] 사용자 아이피 주소
	 * @param sid [OUT] 빌링주문코드 (Max 20bytes)
	 * @param rtn [OUT] 처리 결과 코드 (0: 성공, 그 외는 에러)
	 * @param bal [OUT] 코인 잔액
	 * @param msg [OUT] 에러 메시지
	 */
	virtual RET_CODE cancelUse(const char *id, const char *idno, const char *prodkind, const char *prodcode, const char *snm, int samt, const char *refkey, const char *ip, char *sid, int *rtn, int *bal, char *msg);

private:
	RET_CODE _executeProcedure(const char *procedureName, const char *tcode, const char *id, const char *idno, const char *prodkind, const char *prodcode, const char *snm, int samt, const char *refkey, const char *ip, char *sid, int *rtn, int *bal, char *msg);

	char _ip[32];
	unsigned short _port;
	unsigned int _timeout;
};