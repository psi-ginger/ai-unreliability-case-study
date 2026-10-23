#include "NkFileManager.h"

NkFileManager* NkFileManager::ms_pkFileManager = 0;

bool NkFileManager::IsInitialized()
{
	return ms_pkFileManager != 0;
}

void NkFileManager::Init()
{
	NkAssert(!ms_pkFileManager);
	ms_pkFileManager = NkNew NkFileManager();
}

void NkFileManager::Shutdown()
{
	NkDelete ms_pkFileManager;
	ms_pkFileManager = NULL;
}