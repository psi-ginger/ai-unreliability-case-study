//-------------------------------------------------------------------------------------------------
// NkCore.h
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#pragma once

#ifdef WIN32_LEAN_AND_MEAN
# include <windows.h>
#else
#define WIN32_LEAN_AND_MEAN
# include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif

#include "NkRTLib.h"
#include "NkDebug.h"
#include "NkMemoryManager.h"
#include "NkPointer.h"
#include "NkRTTI.h"
#include "NkChars.h"
#include "NkString.h"
#include "NkStringRef.h"

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

class NKCORE_ENTRY NkCore
{
public:
	static NkCore& Get();
	NkAllocator* GetAllocator();

private:
	NkCore();
	~NkCore();
	//-NkCore(const NkCore&) {};

public:
	static void SetAllocator( NkAllocator* );
	//-static void	Startup(NkAllocator* pkAllocator = NULL);
	static void	Startup();
	static void Shutdown();

	static NkCore* ms_pkCore;
	static NkAllocator* ms_pkAllocator;
};

//////////////////////////////////////////////////////////////////////////

inline NkAllocator* NkCore::GetAllocator()
{
	return ms_pkAllocator;
}

//////////////////////////////////////////////////////////////////////////
// NDOORS (c) Oct. 5, 2009 by RX(박현우)
//////////////////////////////////////////////////////////////////////////
// 정적 데이터 초기화 순서 관리 클래스
// 필요할 경우 상속 받아 구현 가능
//////////////////////////////////////////////////////////////////////////
// 전역 정적 객체에 NkNew NkDelete 함수 사용시 에러 발생 문제 해결.
// NkCore의 Startup/Shutdown, NkMemory, static object의
// 관리 문제로 초기화 기능 추가함.
// cpp 파일에서 다음 내용을 추가해 줄것.
/*
NkCoreInitImplement( NkStaticDataInitor, 0 ); // NkStdAllocator 사용
//또는
//MyCustumAllocator 사용. NkNew 대신 NeExtNew 사용할 것!
NkCoreInitImplement( NkStaticDataInitor, NkExtNew MyCustumAllocator );
*/
class NkStaticDataInitor
{
public:
	NkStaticDataInitor( NkAllocator* );
	virtual ~NkStaticDataInitor();
	virtual void Initor( NkAllocator* );
};

#define NkCoreInitImplement( ClassInitor, pAlloc ) static ClassInitor gs_initor_(pAlloc)
