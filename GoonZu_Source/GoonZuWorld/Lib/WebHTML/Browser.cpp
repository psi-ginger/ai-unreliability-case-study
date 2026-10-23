#include "Browser.h"

#include "../Client/Client.h"
extern cltCommonLogic* pclClient;
/*
BOOL RegisterBrowserWnd( WNDPROC WndProc )
{
	static BOOL bRegistered = FALSE;
	WNDCLASS        wc;
	if(!bRegistered)
	{
		wc.style         = 0;
		wc.lpfnWndProc   = WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = pclClient->GetInstance();
		wc.hIcon         = NULL; 
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_WINDOW);
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = TEXT("Browser_main");
		if (RegisterClass(&wc))
			bRegistered = TRUE;
	}
	return bRegistered;
}

HRESULT CBrowser::CreateBrowserWindow()
{
	RECT rcWorkArea; 
	HRESULT hr = E_FAIL;
	// Ensure that the browser window is registered.
	//if(!RegisterBrowserWnd( s_BrowseWndProc ))
		//goto Exit;
	// Retrieve the system's work area in the RECT structure.
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	// Create the top-level browser window.
	_hWnd = ::CreateWindowEx(0,
		TEXT("Browser_main"),
		TEXT("Sample_Browser"),
		WS_VISIBLE,
		rcWorkArea.left,
		rcWorkArea.top,
		rcWorkArea.right - rcWorkArea.left,
		rcWorkArea.bottom - rcWorkArea.top,
		NULL, NULL, pclClient->GetInstance(), 0);
	if (!_hWnd)
		goto Exit;
	// If the window is created successfully, store this object so the 
	//static wrapper can pass calls to the real BrowseWndProc.
	SetWindowLong(_hWnd, GWL_USERDATA, (DWORD)this);
	hr = S_OK;
Exit:
    return hr;
}


//Create the Web browser object. 
HRESULT CBrowser::CreateBrowserObject()
{
	HRESULT hr;
	IUnknown *pUnk = NULL;
	IOleObject *pObject = NULL;
	RECT rc;  // for activate
	if (!_pWB2)
	{
		// Create an instance of a web browser object (from Shdocvw.dll).
		hr = CoCreateInstance(CLSID_WebBrowser, NULL,
			CLSCTX_INPROC_SERVER | 
			CLSCTX_INPROC_HANDLER,
			IID_IUnknown, (LPVOID *)(&pUnk));
		if (FAILED(hr))
			goto Cleanup;
		// Retrieve an IOleObject interface to set up OLE activation.
		hr = pUnk->QueryInterface(IID_IOleObject, (LPVOID *)(&pObject));
		if (FAILED(hr))
			goto Cleanup;
		// Check if Shdocvw requires a client site to be created first.
		DWORD dwFlags;
		hr = pObject->GetMiscStatus(DVASPECT_CONTENT, &dwFlags);
		if (FAILED(hr))
			goto Cleanup;
		if (dwFlags & OLEMISC_SETCLIENTSITEFIRST)
		{
			// Get an IOleClientSite instance from the browser and pass it to the browser.
			IOleClientSite *pClientSite;
			hr = QueryInterface(IID_IOleClientSite, (LPVOID *)(&pClientSite));
			if (FAILED(hr))
				goto Cleanup;
			hr = pObject->SetClientSite(pClientSite);
			pClientSite->Release();
			if (FAILED(hr))
				goto Cleanup;
		}
		// Activate the object. 
		// Store off the IOleObject reference.
		_pObject = pObject;
		_pObject->AddRef();
		::GetClientRect(_hWnd, &rc);
		hr = _pObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, this, 0, _hWnd, &rc);
		if (FAILED(hr))
			goto Cleanup;
		// Retrieve the IWebBrowser2 interface from the IOleObject.
		hr = _pObject->QueryInterface(IID_IWebBrowser2, (void **)&_pWB2);
		if (FAILED(hr))
			goto Cleanup;
	}
Cleanup:
	if (pUnk)
		pUnk->Release();
	if (pObject)
		pObject->Release();
	return hr;
}


HRESULT CBrowser::RegisterBrowserEventSink()
{
	HRESULT     hr = S_FALSE;
	IConnectionPointContainer  *pCPCont = NULL;
	DWebBrowserEvents2          *pEvents = NULL;
	if (!_pWB2)
		goto Cleanup;
	// Get the connection point container for the browser object.
	hr = _pWB2->QueryInterface(IID_IConnectionPointContainer, (LPVOID *)&pCPCont);
	if (FAILED(hr))
	{
		hr = S_FALSE;
		goto Cleanup;
	}
	// Look for DWebBrowserEvents2 connection point.
	hr = pCPCont->FindConnectionPoint(DIID_DWebBrowserEvents2, &_pCP);
	if (FAILED(hr))
	{
		_pCP = NULL;
		goto Cleanup;
	}
	// Get a DWebBrowserEvents2 pointer from the browser.
	hr = QueryInterface(DIID_DWebBrowserEvents2, (LPVOID *)(&pEvents));
	if (FAILED(hr))
		goto Cleanup;
	// Add your event sink to the connectionpoint.
	hr = _pCP->Advise(pEvents, &(_dwEventCookie));
	if (FAILED(hr))
		goto Cleanup;
Cleanup:
	if (pCPCont)
		pCPCont->Release();
	if (pEvents)
		pEvents->Release();
	return hr;
}


HRESULT CBrowser::NavigateToURL(LPWSTR szURL)
{
	HRESULT hr;
	// Check for the browser.
	if (!_pWB2)
		return E_FAIL; 
	// Package the URL as a BSTR for Navigate.
	VARIANT varURL;
	V_VT(&varURL) = VT_EMPTY;
	if(szURL && szURL[0] != L'\0')
	{
		V_VT(&varURL) = VT_BSTR;
		varURL.bstrVal = SysAllocString(szURL);
		if(varURL.bstrVal)
		{
			// Call IWebBrowser2::Navigate2 with no special options.
			hr = _pWB2->Navigate2(&varURL, NULL, NULL, NULL, NULL);
		}
	}
	else
	{
		// If there is no URL, go to the default homepage. 
		hr = _pWB2->GoHome();
	}
	// Clean up the BSTR if it exists.
	VariantClear(&varURL);
	return hr;
}


HRESULT CBrowser::StartBrowser( LPWSTR szURL )
{
	return NULL;
}

void CBrowser::CloseBrowser()
{
}

*/
HRESULT STDMETHODCALLTYPE CBrowser::Invoke
		(
		DWebBrowserEvents2 * This,
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr
		)
{
	MessageBox( NULL, TEXT("et"), TEXT("SEt"), MB_OK );
	return NULL;
}
