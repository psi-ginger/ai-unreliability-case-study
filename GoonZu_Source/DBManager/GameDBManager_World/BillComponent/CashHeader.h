#ifndef __PLPAY_HEADER_H_
#define __PLPAY_HEADER_H_

// Socket 관련 에러
#define		PLPAY_SOCKERR_200		-200
#define		PLPAY_SOCKERR_200_MSG	"Host Address가 이상합니다.(HOST=%s, PORT=%d)"
#define		PLPAY_SOCKERR_201		-201
#define		PLPAY_SOCKERR_201_MSG	"Socket Open 에러(HOST=%s, PORT=%d)"
#define		PLPAY_SOCKERR_210		-210
#define		PLPAY_SOCKERR_210_MSG	"send 에러(send()=0)"
#define		PLPAY_SOCKERR_220		-220
#define		PLPAY_SOCKERR_220_MSG	"send 에러(보내고자하는 길이보다 적게 전송)"
#define		PLPAY_SOCKERR_230		-230
#define		PLPAY_SOCKERR_230_MSG	"수신 버퍼 Space 부족"
#define		PLPAY_SOCKERR_240		-240
#define		PLPAY_SOCKERR_240_MSG	"Select 에러"
#define		PLPAY_SOCKERR_250		-250
#define		PLPAY_SOCKERR_250_MSG	"전문 수신 실패"
#define		PLPAY_SOCKERR_260		-260
#define		PLPAY_SOCKERR_260_MSG	"Welcome recv 에러"
#define		PLPAY_SOCKERR_270		-270
#define		PLPAY_SOCKERR_270_MSG	"Socket recv timeout 에러"
#define		PLPAY_SOCKERR_280		-280
#define		PLPAY_SOCKERR_280_MSG	"클라이언트에서 Exception 발생(File=%s, Line=%d, Method=%s)"

// 일반 에러
#define		PLPAY_PARAMERR_100			100
#define		PLPAY_PARAMERR_100_MSG		"Parameter값이 설정되지 않았습니다.(%s)"
#define		PLPAY_PARAMERR_110			110
#define		PLPAY_PARAMERR_110_MSG		"PG가 명시되지 않았습니다(%s)."
#define		PLPAY_PARAMERR_120			120
#define		PLPAY_PARAMERR_120_MSG		"WideCharToMultiByte Conversion Err Buffer shortage(%s)."
#define		PLPAY_PARAMERR_130			130
#define		PLPAY_PARAMERR_130_MSG		"정의되지 않은 명령어(%s)."
#define		PLPAY_PARAMERR_140			140
#define		PLPAY_PARAMERR_140_MSG		"아직 구현되지 않은 기능입니다."
#define		PLPAY_PARAMERR_199			199
#define		PLPAY_PARAMERR_199_MSG		"ServerMode를 반드시 지정하셔야 합니다.(TEST 또는 REAL)"

#endif //__PLPAY_HEADER_H_
