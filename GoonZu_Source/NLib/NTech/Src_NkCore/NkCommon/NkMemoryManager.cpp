#include "NkCore.h"
#include "NkMemoryManager.h"

NkMemoryManager* NkMemoryManager::ms_pkMemManager = 0;

bool NkMemoryManager::IsInitialized()
{
	return ms_pkMemManager != 0;
}

void NkMemoryManager::Init()
{
	NkAssert(!ms_pkMemManager);
	ms_pkMemManager = NkExtNew NkMemoryManager();
	ms_pkMemManager->m_pkAllocator = NkCore::Get().GetAllocator();

	ms_pkMemManager->m_pkAllocator->Init();
}

void NkMemoryManager::Shutdown()
{
	NkAssert(ms_pkMemManager->m_pkAllocator);

	ms_pkMemManager->m_pkAllocator->Shutdown();
	
	NkExtDelete ms_pkMemManager;
	ms_pkMemManager = NULL;
}