#include "NkCore.h"
#include "NkStdAllocator.h"
#include "NkMemoryTracker.h"
#include "NkGlobalStringTable.h"


#ifdef NKCORE_EXPORT

BOOL APIENTRY DllMain( HANDLE hModule, DWORD uiReason, LPVOID lpReserved )
{
	switch( uiReason )
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif

//////////////////////////////////////////////////////////////////////////

NkCore* NkCore::ms_pkCore = NULL;
NkAllocator* NkCore::ms_pkAllocator = NULL;

//////////////////////////////////////////////////////////////////////////

NkCore::NkCore()
{}

NkCore::~NkCore()
{}

void
NkCore::SetAllocator( NkAllocator* pkAllocator )
{
	//-NkAssert( NULL == ms_pkAllocator );
	ms_pkAllocator = pkAllocator;
}

void
NkCore::Startup()
{
	if( ms_pkCore ) return;

	
	ms_pkCore = NkExtNew NkCore;

	if( ms_pkCore->ms_pkAllocator == NULL )
	{
#if defined(_DEBUG)
		ms_pkCore->ms_pkAllocator = NkExtNew NkMemoryTracker( NkExtNew NkStdAllocator );
#else
		ms_pkCore->ms_pkAllocator = NkExtNew NkStdAllocator;
#endif
	}

	NkMemoryManager::Init();

	NkGlobalStringTable::Startup();
}

void
NkCore::Shutdown()
{
	if( NULL == ms_pkCore ) return;

	NkGlobalStringTable::Shutdown();
	NkMemoryManager::Shutdown();

	if( ms_pkCore->ms_pkAllocator )
	{
		NkExtDelete ms_pkCore->ms_pkAllocator;
		ms_pkCore->ms_pkAllocator = NULL;
	}

	NkExtDelete ms_pkCore;
	ms_pkCore = NULL;
}

inline NkCore& NkCore::Get()
{
	if( NULL==ms_pkCore )
	{
		Startup();
	}
	return *ms_pkCore;
}

//////////////////////////////////////////////////////////////////////////


NkStaticDataInitor::NkStaticDataInitor( NkAllocator *pAlloc )
{
	Initor( pAlloc );
}

NkStaticDataInitor::~NkStaticDataInitor()
{
	NkCore::Shutdown();
}

void
NkStaticDataInitor::Initor( NkAllocator *pAlloc )
{
	NkCore::SetAllocator( pAlloc );
	NkCore::Get().GetAllocator();
}
