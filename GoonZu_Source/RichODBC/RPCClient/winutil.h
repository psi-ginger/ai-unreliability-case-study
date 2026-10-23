/**
	@file 
	@brief 윈도우 유틸리티들
	@author rapool
	@version $Revision: 1.0 $
	@date $Date: 2006/03/20 14:00:00 $

	$Id: filemng.h,v 1.0 2006/03/20 14:00:00 rapool Exp $
 */

#ifndef _NANO_WINUTILITIES_H_
#define _NANO_WINUTILITIES_H_

#include "common.h"
#include <float.h>
#include <limits.h>
#include <time.h>

namespace nano {
/**
	@addtogroup unixutilities
	@{
 */


#ifndef __DBL_EPSILON__
#define __DBL_EPSILON__ DBL_EPSILON
#endif
/**
 * @brief 윈도와의 호환성을 위하여 timeval 구조체를 정의함
 * @remark 리눅스에서 자주 쓰이는 timeval 구조체를 윈도에서 지원하지 않으므로 따로 정의함
 */
struct timeval
{
	long tv_sec; // second
	long tv_usec; // microsecond
};
/* end of struct timeval */


/**
 * @brief linux::gettimeofday()
 * @param tv 리턴될 시간값을 저장하는 변수
 * @param tzp 사용안함 (NULL로 넘기면 됨)
 * @remark timeval::tv_usec는 milli-second까지 정확함 (윈도에서 micro-second를 구하는 API를 모르겠음)
 */
int gettimeofday(struct timeval *tv, void *tzp);

/**
 * @brief 대소문자 구별 없이 문자열이 같은지 비교
 * @param a 문자열
 * @param b 문자열
 * @return 같으면 0, a > b 이면 +, a < b 이면 -
 */
int strcasecmp(const char *a, const char *b);

/**
 * @brief 대소문자 구별 없이 문자열이 같은지 비교
 * @param a 문자열
 * @param b 문자열
 * @param n 비교하고자 하는 바이트수
 * @return 같으면 0, a > b 이면 +, a < b 이면 -
 */
int strncasecmp(const char *a, const char *b, size_t n);

/**
 * @brief POSIX::strtoll()의 윈도우 버전
 * @param __int64로 변환하고자 하는 string
 * @param nptr에서 invalid한 문자열의 처음 위치
 * @param base 진수를 표시
 */
__int64 strtoll(const char *nptr, char **endptr, int base);

/**
 * @brief 절대값을 리턴
 * @param value 값
 */
__int64 llabs(__int64 value);

/** @} */


} // end of namespcae nano

#endif
