// NdoorsStarter.cpp : DLL 내보내기의 구현입니다.


#include "stdafx.h"
#include "resource.h"
#include "NdoorsStarter.h"


class CNdoorsStarterModule : public CAtlDllModuleT< CNdoorsStarterModule >
{
public :
	DECLARE_LIBID(LIBID_NdoorsStarterLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_NDOORSSTARTER, "{9273E927-187B-422E-8989-E4A8048B9D38}")
};

CNdoorsStarterModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL 진입점입니다.
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// DLL이 OLE에 의해 언로드될 수 있는지 결정하는 데 사용됩니다.
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// 클래스 팩터리를 반환하여 요청된 형식의 개체를 만듭니다.
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - 시스템 레지스트리에 항목을 추가합니다.
STDAPI DllRegisterServer(void)
{
    // 개체, 형식 라이브러리 및 형식 라이브러리에 들어 있는 모든 인터페이스를 등록합니다.
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - 시스템 레지스트리에서 항목을 제거합니다.
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

