/**
* @brief RPC 인터페이스 정의
* @author rapool <chankyulee@nhncorp.com>
* @date $Date: 2006-05-10$
* @version $Revision: $
*/


#ifndef MEGA_RPC_INTERFACE_H
#define MEGA_RPC_INTERFACE_H


#include "common.h"

#ifdef __NANO_OS_WIN32__
#include "winutil.h"
#else
#include "unixutil.h"
#endif


namespace mega
{
/**
 * @addtogroup nbfpInterface
 * @{
 */


extern int objectCount;

class IList;
class IMap;
class ITable;
class IVariant;
class VariantBoxing;


/**
 * @brief nano::Variant 인터페이스 클래스
 */
class IVariant
{
	public:
		/**
		 * @brief IVariant에서 사용하는 타입값 지정
		 */
		enum Type {
			TYPE_UNKNOWN = 0, 		/**< UNKNOWN */
			/* basic */
			TYPE_NULL = 1, 			/**< NULL */
			TYPE_BOOL, 				/**< BOOL : boolean */
			TYPE_CHAR, 				/**< CHAR : 한 글자. 1 byte integer가 아님 */
			TYPE_INT, 				/**< INT : 4 byte signed integer */
			TYPE_LONG, 				/**< LONG : 8 byte signed integer */
			TYPE_DOUBLE, 			/**< DOUBLE : IEEE 754, 8 byte floating point number */
			TYPE_DATE, 				/**< DATE : miliseconds since epoch. 8 byte signed integer */
			TYPE_STRING, 			/**< STRING */
			TYPE_BINARY, 			/**< BINARY */
			TYPE_BIGINT, 			/**< BIGINT */
			/* derived from basic */
			TYPE_XML = 50, 			/**< 사용되지 않음 */
			TYPE_REMOTE, 			/**< 사용되지 않음 */
			TYPE_REF, 				/**< 사용되지 않음 */
			/* container */
			TYPE_LIST = 100, 		/**< LIST */
			TYPE_MAP, 				/**< MAP */
			TYPE_TABLE, 			/**< TABLE */
			/* type only */
			TYPE_NUMBER = 150, 		/**< NUMBER : TABLE의 COLUMN TYPE으로만 쓰이며, 실제 FIELD가 INT/LONG/DOUBLE이 가능함을 나타낸다 */
			TYPE_TABLE_COLUMN, 		/**< TABLE_COLUMN : TABLE의 COLUMN을 나타낸다. 실제 object는 존재하지 않는다 */
			TYPE_TABLE_ROW, 		/**< TABLE_ROW : TABLE의 ROW을 나타낸다. 실제 object는 존재하지 않는다 */
			/* other */
			TYPE_EOO = 255, 		/**< EOO : end ob object. STREAM에서만 사용된다. */
		};

		enum ConvertOption
		{
			CONVERT_NONE = 0, // conversion을 수행하지 않는다. type이 정확히 맞는 경우에만 값을 얻거나 conversion이 동작한다.  
			CONVERT_CAST = 1, // conversion이 가능한 경우 conversion을 수행한다. (range가 벗어나거나, 값이 손상될 수 있다)  
			CONVERT_RANGE_SAFE = 2, // CONVERT_CAST와 같으나, range를 벗어나는 경우 conversion 실패로 간주한다. (예: LONG -> INT)  
			CONVERT_VALUE_SAFE = 3, // CONVERT_RANGE_SAFE와 같으나, 값이 손상되는 경우 conversion 실패로 간주한다. (예: 소수점 이하가 있는 DOUBLE -> LONG) 
		};

	public:
		/**
		 * @brief 저장된 값의 타입을 리턴
		 * @return IVariant::Type형식의 타입값
		 */
		virtual IVariant::Type type() const = 0;

		/**
		 * @brief 저장된 값의 바이트수를 리턴
		 * @return 바이트수
		 */
		virtual size_t size() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 */
		virtual bool boolVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 */
		virtual int intVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 */
		virtual double doubleVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 */
		virtual int64_t longVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 */
		virtual const char *stringVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 */
#ifdef __NANO_OS_WIN32__
		virtual struct nano::timeval timeVal() const = 0;
#else
		virtual struct timeval timeVal() const = 0;
#endif

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 * @remark !!! 사용 후 반드시 IList::relase()를 호출해야 함 !!!
		 */
		virtual IList* listVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 * @remark !!! 사용 후 반드시 ITable::relase()를 호출해야 함 !!!
		 */
		virtual ITable* tableVal() const = 0;

		/**
		 * @brief 저장된 값을 가져옴
		 * @return 저장된 값
		 * @remark !!! 사용 후 반드시 IMap::relase()를 호출해야 함 !!!
		 */
		virtual IMap* mapVal() const = 0;

		/**
		 * @brief 저장된 값의 포인터를 가져옴
		 * @return 포인터
		 */
		virtual const void* ptr() const = 0;

public:
		/**
		 * @brief bool 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(bool *val, ConvertOption opt = CONVERT_CAST) const = 0;

		/**
		 * @brief int 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(int *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief float 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(float *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief double 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(double *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief int8_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(int8_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief uint8_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(uint8_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief int16_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(int16_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief uint16_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(uint16_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief uint32_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(uint32_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief int64_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(int64_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief uint64_t 값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(uint64_t *val, ConvertOption opt = CONVERT_RANGE_SAFE) const = 0;

		/**
		 * @brief string 값 얻기
		 * @param ptr 값을 얻을 pointer
		 * @param size 크기를 얻을 pointer
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(const char **ptr, size_t *size = NULL, ConvertOption opt = CONVERT_CAST) const = 0;

		/**
		 * @brief binary 값 얻기
		 * @param ptr 값을 얻을 pointer
		 * @param size 크기를 얻을 pointer
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
		virtual bool get(const void **ptr, size_t *size = NULL, ConvertOption opt = CONVERT_CAST) const = 0;

		/**
		 * @brief date값 얻기
		 * @param val 값을 얻을 변수
		 * @param opt type 변환시 변환 방식
		 * @return 변환에 성공하고, val에 valid한 값을 넣었을 경우 true, 아닌 경우 false
		 * @remark Type이 다른 경우, 값은 변환하나 자체의 type은 변하지 않는다.
		 */
#ifdef __NANO_OS_WIN32__
		virtual bool get(struct nano::timeval* val, ConvertOption opt = CONVERT_CAST) const = 0;
#else
		virtual bool get(struct timeval* val, ConvertOption opt = CONVERT_CAST) const = 0;
#endif

	public:
		/**
		 * @brief 레퍼런스 카운트를 증가시킴
		 */
		virtual void addRef() const = 0;

		/**
		 * @brief 레퍼런스 카운트를 감소시킴
		 * @remark 레퍼런스 카운트가 0이 되는 순간 자동 소멸됨
		 */
		virtual void release() const = 0;

	public:
		/**
		 * @brief 해당 값을 XML 형태로 stdout에 출력함
		 */
		virtual void printAsXML() const = 0;

		/**
		 * @brief 해당 값을 XML 형태의 스트링으로 리턴함, 버퍼의 크기보다 스트링이 길 경우 (size-1)만큼만 쓰여짐
		 * @param buf 스트링을 받을 버퍼
		 * @param size 스트링을 받을 버퍼의 크기
		 * @return XML 형태의 null-terminated string
		 */
		virtual const char* toXML(char *buf, unsigned int size) const = 0;
};


class ITable
{
	public:
		/**
		 * @brief 저장된 Row의 수를 가져옴
		 * @return 저장된 Row의 수
		 * @remark rowSize() 와 같음
		 */
		virtual size_t size() const = 0;

		/**
		 * @brief 저장된 Row의 수를 가져옴
		 * @return 저장된 Row의 수
		 */
		virtual size_t rowSize() const = 0;

		/**
		 * @brief 저장된 Column의 수를 가져옴
		 * @return 저장된 Column의 수
		 */
		virtual size_t columnSize() const = 0;

		/**
		 * @brief 테이블에 저장된 데이터를 가져옴
		 * @return 저장된 IVariant 포인터
		 * @remark rowIndex나 columnIndex가 잘못된 경우 IVariant::TYPE_NULL을 가진 IVariant가 리턴됨
		 * @remark 리턴받은 IVariant에 대하여 release() 호출할 필요 없음
		 */
		virtual const IVariant* getValue(size_t rowIndex, size_t columnIndex) const = 0;

		/**
		 * @brief 테이블에 저장된 데이터를 가져옴
		 * @return columnName이 잘못된 경우 NULL을 리턴함
		 * @remark rowIndex나 columnIndex가 잘못된 경우 IVariant::TYPE_NULL을 가진 IVariant가 리턴됨
		 * @remark 리턴받은 IVariant에 대하여 release() 호출할 필요 없음
		 * @remark ***** getValue(0, 0) 호출시에 위의 함수와 ambigious가 발생해서 일단 막음 ******
		 */
		//virtual IVariant* getValue(size_t rowIndex, const char *columnName) = 0;

		/**
		 * @brief 컬럼의 타입을 가져옴
		 * @param columnIndex 컬럼 인덱스 (zero-based)
		 */
		virtual IVariant::Type columnType(size_t columnIndex) const = 0;

		/**
		 * @brief 컬럼이름을 가져옴
		 * @param columnIndex 컬럼 인덱스 (zero-based)
		 */
		virtual const char *columnName(size_t columnIndex) const = 0;

	public:
		/**
		 * @brief 레퍼런스 카운트를 증가시킴
		 */
		virtual void addRef() const = 0;

		/**
		 * @brief 레퍼런스 카운트를 감소시킴
		 * @remark 레퍼런스 카운트가 0이 되는 순간 자동 소멸됨
		 */
		virtual void release() const = 0;

	public:
		/**
		 * @brief 해당 값을 XML 형태로 stdout에 출력함
		 */
		virtual void printAsXML() const = 0;

		/**
		 * @brief 해당 값을 XML 형태의 스트링으로 리턴함, 버퍼의 크기보다 스트링이 길 경우 (size-1)만큼만 쓰여짐
		 * @param buf 스트링을 받을 버퍼
		 * @param size 스트링을 받을 버퍼의 크기
		 * @return XML 형태의 null-terminated string
		 */
		virtual const char* toXML(char *buf, unsigned int size) const = 0;
};


/**
 * @brief RPC Variant List 인터페이스
 */
class IList
{
	public:
		/**
		 * @brief 리스트의 원소를 가져옴
		 * @param index 리스트 인덱스 (zero-based)
		 * @return 원소의 포인터
		 * @remark 리턴된 IVariant에 대해서 release()를 호출할 필요 없음
		 */
		virtual const IVariant* at(size_t index) const = 0;

		/**
		 * @brief 리스트의 원소를 가져옴
		 * @param index 리스트 인덱스 (zero-based)
		 * @return 원소의 포인터
		 * @remark 리턴된 IVariant에 대해서 release()를 호출할 필요 없음
		 */
		virtual const IVariant* operator[] (size_t index) const = 0;

		/**
		 * @brief 리스트에 원소를 추가
		 * @param iv 추가하고자 하는 값
		 */
		virtual void add(const VariantBoxing &iv) = 0;

		/**
		 * @brief 리스트에 binary를 추가
		 * @param ptr 추가하고자 하는 binary 값의 포인터
		 * @param size 추가하고자 하는 binary 값의 사이즈
		 */
		virtual void add(const void *ptr, size_t size) = 0;

		/**
		 * @brief 리스트 원소의 갯수를 가져옴
		 * @return 리스트 원소의 갯수
		 */
		virtual size_t size() const = 0;

	public:
		/**
		 * @brief 레퍼런스 카운트를 증가시킴
		 */
		virtual void addRef() const = 0;

		/**
		 * @brief 레퍼런스 카운트를 감소시킴
		 * @remark 레퍼런스 카운트가 0이 되는 순간 자동 소멸됨
		 */
		virtual void release() const = 0;

	public:
		/**
		 * @brief 해당 값을 XML 형태로 stdout에 출력함
		 */
		virtual void printAsXML() const = 0;

		/**
		 * @brief 해당 값을 XML 형태의 스트링으로 리턴함, 버퍼의 크기보다 스트링이 길 경우 (size-1)만큼만 쓰여짐
		 * @param buf 스트링을 받을 버퍼
		 * @param size 스트링을 받을 버퍼의 크기
		 * @return XML 형태의 null-terminated string
		 */
		virtual const char* toXML(char *buf, unsigned int size) const = 0;
};


/** 
 * @brief RPC Variant Hashmap 인터페이스
 */
class IMap
{
	public:
		/**
		 * @brief 해쉬맵에서 key에 해당하는 값을가져옴
		 * @param key 키
		 * @param value 값을 저장하기 위한 IVariant의 포인터
		 * @return 해당 키의 아이템이 없는 경우 false 리턴
		 * @remark 리턴받은 value에 대하여 release()를 호출할 필요 없음
		 */
		virtual bool get(const VariantBoxing &key, IVariant **value) const = 0;

		/**
		 * @brief 해쉬맵에 아이템을 삽입함
		 * @param key 아이템의 키
		 * @param value 아이템
		 * @return 같은 키가 존재할 경우 false 리턴
		 */
		virtual bool put(const VariantBoxing &key, const VariantBoxing &value) = 0;

		/**
		 * @brief 해쉬맵에 저장된 노드의 수를 가져옴
		 * @return 노드의 수
		 */
		virtual size_t size() const = 0;

	public:
		/**
		 * @brief 레퍼런스 카운트를 증가시킴
		 */
		virtual void addRef() const = 0;

		/**
		 * @brief 레퍼런스 카운트를 감소시킴
		 * @remark 레퍼런스 카운트가 0이 되는 순간 자동 소멸됨
		 */
		virtual void release() const = 0;

	public:
		/**
		 * @brief 해당 값을 XML 형태로 stdout에 출력함
		 */
		virtual void printAsXML() const = 0;

		/**
		 * @brief 해당 값을 XML 형태의 스트링으로 리턴함, 버퍼의 크기보다 스트링이 길 경우 (size-1)만큼만 쓰여짐
		 * @param buf 스트링을 받을 버퍼
		 * @param size 스트링을 받을 버퍼의 크기
		 * @return XML 형태의 null-terminated string
		 */
		virtual const char* toXML(char *buf, unsigned int size) const = 0;
};


/** @}*/
} /*end of namespace mega*/


#endif
