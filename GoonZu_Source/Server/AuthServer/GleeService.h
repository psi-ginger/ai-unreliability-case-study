//
// sproxy.exe generated file
// do not modify this file
//
// Created: 05/29/2007@18:53:34
//

#pragma once

#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace GameActivService
{

template <typename TClient = CSoapSocketClientT<> >
class CGameActivServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CGameActivServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://webservice.hljz.net/GameActivService.asmx"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CGameActivServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT GleeActivHLJZGame(
		BSTR DBIP, 
		int port, 
		BSTR userName, 
		BSTR passWord, 
		int* GleeActivHLJZGameResult, 
		BSTR* message
	);
};

typedef CGameActivServiceT<> CGameActivService;

struct __CGameActivService_GleeActivHLJZGame_struct
{
	BSTR DBIP;
	int port;
	BSTR userName;
	BSTR passWord;
	int GleeActivHLJZGameResult;
	BSTR message;
};

extern __declspec(selectany) const _soapmapentry __CGameActivService_GleeActivHLJZGame_entries[] =
{

	{
		0x00266C3F, 
		"DBIP", 
		L"DBIP", 
		sizeof("DBIP")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CGameActivService_GleeActivHLJZGame_struct, DBIP),
		NULL,
		NULL,
		-1,
	},
	{
		0x003F51C5, 
		"port", 
		L"port", 
		sizeof("port")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CGameActivService_GleeActivHLJZGame_struct, port),
		NULL,
		NULL,
		-1,
	},
	{
		0x8253F720, 
		"userName", 
		L"userName", 
		sizeof("userName")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CGameActivService_GleeActivHLJZGame_struct, userName),
		NULL,
		NULL,
		-1,
	},
	{
		0x9C320613, 
		"passWord", 
		L"passWord", 
		sizeof("passWord")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CGameActivService_GleeActivHLJZGame_struct, passWord),
		NULL,
		NULL,
		-1,
	},
	{
		0x795380A5, 
		"GleeActivHLJZGameResult", 
		L"GleeActivHLJZGameResult", 
		sizeof("GleeActivHLJZGameResult")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CGameActivService_GleeActivHLJZGame_struct, GleeActivHLJZGameResult),
		NULL,
		NULL,
		-1,
	},
	{
		0xBA86A185, 
		"message", 
		L"message", 
		sizeof("message")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CGameActivService_GleeActivHLJZGame_struct, message),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CGameActivService_GleeActivHLJZGame_map =
{
	0x1BDD5E35,
	"GleeActivHLJZGame",
	L"GleeActivHLJZGameResponse",
	sizeof("GleeActivHLJZGame")-1,
	sizeof("GleeActivHLJZGameResponse")-1,
	SOAPMAP_FUNC,
	__CGameActivService_GleeActivHLJZGame_entries,
	sizeof(__CGameActivService_GleeActivHLJZGame_struct),
	2,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xF5B7D6CC,
	"http://www.youxi.net.cn/",
	L"http://www.youxi.net.cn/",
	sizeof("http://www.youxi.net.cn/")-1
};

extern __declspec(selectany) const _soapmap * __CGameActivService_funcs[] =
{
	&__CGameActivService_GleeActivHLJZGame_map,
	NULL
};

template <typename TClient>
inline HRESULT CGameActivServiceT<TClient>::GleeActivHLJZGame(
		BSTR DBIP, 
		int port, 
		BSTR userName, 
		BSTR passWord, 
		int* GleeActivHLJZGameResult, 
		BSTR* message
	)
{
    if ( GleeActivHLJZGameResult == NULL )
		return E_POINTER;
    if ( message == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CGameActivService_GleeActivHLJZGame_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.DBIP = DBIP;
	__params.port = port;
	__params.userName = userName;
	__params.passWord = passWord;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://www.youxi.net.cn/GleeActivHLJZGame\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GleeActivHLJZGameResult = __params.GleeActivHLJZGameResult;
	*message = __params.message;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CGameActivServiceT<TClient>::GetFunctionMap()
{
	return __CGameActivService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CGameActivServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CGameActivService_GleeActivHLJZGame_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CGameActivService_GleeActivHLJZGame_atlsoapheader_map = 
	{
		0x1BDD5E35,
		"GleeActivHLJZGame",
		L"GleeActivHLJZGameResponse",
		sizeof("GleeActivHLJZGame")-1,
		sizeof("GleeActivHLJZGameResponse")-1,
		SOAPMAP_HEADER,
		__CGameActivService_GleeActivHLJZGame_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xF5B7D6CC,
		"http://www.youxi.net.cn/",
		L"http://www.youxi.net.cn/",
		sizeof("http://www.youxi.net.cn/")-1
	};


	static const _soapmap * __CGameActivService_headers[] =
	{
		&__CGameActivService_GleeActivHLJZGame_atlsoapheader_map,
		NULL
	};
	
	return __CGameActivService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CGameActivServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CGameActivServiceT<TClient>::GetNamespaceUri()
{
	return L"http://www.youxi.net.cn/";
}

template <typename TClient>
ATL_NOINLINE inline const char * CGameActivServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CGameActivServiceT<TClient>::GetNamespaceUriA()
{
	return "http://www.youxi.net.cn/";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CGameActivServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CGameActivServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace GameActivService
