//
// sproxy.exe generated file
// do not modify this file
//
// Created: 05/29/2007@19:14:55
//

#pragma once


#ifndef _WIN32_WINDOWS
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace AEService
{

template <typename TClient = CSoapSocketClientT<> >
class CAEServiceT : 
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

	CAEServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://192.168.1.12:88/AEService.asmx"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CAEServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT HLJZLogOut(
		BSTR serverID, 
		BSTR AccountID, 
		BSTR roleName, 
		int grade, 
		int* HLJZLogOutResult, 
		BSTR* message
	);

	HRESULT HLJZLogIn(
		BSTR serverID, 
		BSTR AccountID, 
		BSTR ip, 
		int* HLJZLogInResult, 
		BSTR* message
	);
};

typedef CAEServiceT<> CAEService;

struct __CAEService_HLJZLogOut_struct
{
	BSTR serverID;
	BSTR AccountID;
	BSTR roleName;
	int grade;
	int HLJZLogOutResult;
	BSTR message;
};

extern __declspec(selectany) const _soapmapentry __CAEService_HLJZLogOut_entries[] =
{

	{
		0x92C94D44, 
		"serverID", 
		L"serverID", 
		sizeof("serverID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CAEService_HLJZLogOut_struct, serverID),
		NULL,
		NULL,
		-1,
	},
	{
		0xC1439C7A, 
		"AccountID", 
		L"AccountID", 
		sizeof("AccountID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CAEService_HLJZLogOut_struct, AccountID),
		NULL,
		NULL,
		-1,
	},
	{
		0x990E6353, 
		"roleName", 
		L"roleName", 
		sizeof("roleName")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CAEService_HLJZLogOut_struct, roleName),
		NULL,
		NULL,
		-1,
	},
	{
		0x07880903, 
		"grade", 
		L"grade", 
		sizeof("grade")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CAEService_HLJZLogOut_struct, grade),
		NULL,
		NULL,
		-1,
	},
	{
		0x80606071, 
		"HLJZLogOutResult", 
		L"HLJZLogOutResult", 
		sizeof("HLJZLogOutResult")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CAEService_HLJZLogOut_struct, HLJZLogOutResult),
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
		offsetof(__CAEService_HLJZLogOut_struct, message),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CAEService_HLJZLogOut_map =
{
	0x19A16101,
	"HLJZLogOut",
	L"HLJZLogOutResponse",
	sizeof("HLJZLogOut")-1,
	sizeof("HLJZLogOutResponse")-1,
	SOAPMAP_FUNC,
	__CAEService_HLJZLogOut_entries,
	sizeof(__CAEService_HLJZLogOut_struct),
	2,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xBE9F8D84,
	"http://AvertEnthrallment.hlsk.com/",
	L"http://AvertEnthrallment.hlsk.com/",
	sizeof("http://AvertEnthrallment.hlsk.com/")-1
};


struct __CAEService_HLJZLogIn_struct
{
	BSTR serverID;
	BSTR AccountID;
	BSTR ip;
	int HLJZLogInResult;
	BSTR message;
};

extern __declspec(selectany) const _soapmapentry __CAEService_HLJZLogIn_entries[] =
{

	{
		0x92C94D44, 
		"serverID", 
		L"serverID", 
		sizeof("serverID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CAEService_HLJZLogIn_struct, serverID),
		NULL,
		NULL,
		-1,
	},
	{
		0xC1439C7A, 
		"AccountID", 
		L"AccountID", 
		sizeof("AccountID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CAEService_HLJZLogIn_struct, AccountID),
		NULL,
		NULL,
		-1,
	},
	{
		0x00000DF9, 
		"ip", 
		L"ip", 
		sizeof("ip")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CAEService_HLJZLogIn_struct, ip),
		NULL,
		NULL,
		-1,
	},
	{
		0x19534EB0, 
		"HLJZLogInResult", 
		L"HLJZLogInResult", 
		sizeof("HLJZLogInResult")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CAEService_HLJZLogIn_struct, HLJZLogInResult),
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
		offsetof(__CAEService_HLJZLogIn_struct, message),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CAEService_HLJZLogIn_map =
{
	0xBB08DB00,
	"HLJZLogIn",
	L"HLJZLogInResponse",
	sizeof("HLJZLogIn")-1,
	sizeof("HLJZLogInResponse")-1,
	SOAPMAP_FUNC,
	__CAEService_HLJZLogIn_entries,
	sizeof(__CAEService_HLJZLogIn_struct),
	2,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xBE9F8D84,
	"http://AvertEnthrallment.hlsk.com/",
	L"http://AvertEnthrallment.hlsk.com/",
	sizeof("http://AvertEnthrallment.hlsk.com/")-1
};

extern __declspec(selectany) const _soapmap * __CAEService_funcs[] =
{
	&__CAEService_HLJZLogOut_map,
	&__CAEService_HLJZLogIn_map,
	NULL
};

template <typename TClient>
inline HRESULT CAEServiceT<TClient>::HLJZLogOut(
		BSTR serverID, 
		BSTR AccountID, 
		BSTR roleName, 
		int grade, 
		int* HLJZLogOutResult, 
		BSTR* message
	)
{
    if ( HLJZLogOutResult == NULL )
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
	__CAEService_HLJZLogOut_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.serverID = serverID;
	__params.AccountID = AccountID;
	__params.roleName = roleName;
	__params.grade = grade;

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
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://AvertEnthrallment.hlsk.com/HLJZLogOut\"\r\n"));
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

	*HLJZLogOutResult = __params.HLJZLogOutResult;
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
inline HRESULT CAEServiceT<TClient>::HLJZLogIn(
		BSTR serverID, 
		BSTR AccountID, 
		BSTR ip, 
		int* HLJZLogInResult, 
		BSTR* message
	)
{
    if ( HLJZLogInResult == NULL )
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
	__CAEService_HLJZLogIn_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.serverID = serverID;
	__params.AccountID = AccountID;
	__params.ip = ip;

	__atlsoap_hr = SetClientStruct(&__params, 1);
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
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://AvertEnthrallment.hlsk.com/HLJZLogIn\"\r\n"));
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

	*HLJZLogInResult = __params.HLJZLogInResult;
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
ATL_NOINLINE inline const _soapmap ** CAEServiceT<TClient>::GetFunctionMap()
{
	return __CAEService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CAEServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CAEService_HLJZLogOut_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CAEService_HLJZLogOut_atlsoapheader_map = 
	{
		0x19A16101,
		"HLJZLogOut",
		L"HLJZLogOutResponse",
		sizeof("HLJZLogOut")-1,
		sizeof("HLJZLogOutResponse")-1,
		SOAPMAP_HEADER,
		__CAEService_HLJZLogOut_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xBE9F8D84,
		"http://AvertEnthrallment.hlsk.com/",
		L"http://AvertEnthrallment.hlsk.com/",
		sizeof("http://AvertEnthrallment.hlsk.com/")-1
	};

	static const _soapmapentry __CAEService_HLJZLogIn_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CAEService_HLJZLogIn_atlsoapheader_map = 
	{
		0xBB08DB00,
		"HLJZLogIn",
		L"HLJZLogInResponse",
		sizeof("HLJZLogIn")-1,
		sizeof("HLJZLogInResponse")-1,
		SOAPMAP_HEADER,
		__CAEService_HLJZLogIn_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xBE9F8D84,
		"http://AvertEnthrallment.hlsk.com/",
		L"http://AvertEnthrallment.hlsk.com/",
		sizeof("http://AvertEnthrallment.hlsk.com/")-1
	};


	static const _soapmap * __CAEService_headers[] =
	{
		&__CAEService_HLJZLogOut_atlsoapheader_map,
		&__CAEService_HLJZLogIn_atlsoapheader_map,
		NULL
	};
	
	return __CAEService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CAEServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CAEServiceT<TClient>::GetNamespaceUri()
{
	return L"http://AvertEnthrallment.hlsk.com/";
}

template <typename TClient>
ATL_NOINLINE inline const char * CAEServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CAEServiceT<TClient>::GetNamespaceUriA()
{
	return "http://AvertEnthrallment.hlsk.com/";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CAEServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CAEServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
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

} // namespace AEService
