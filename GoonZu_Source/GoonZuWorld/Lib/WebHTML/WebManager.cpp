// [3/3/2006]
//
//	엔도어즈 웹매니져
//

#include "Exdisp.h"
#include "WebManager.h"

CNdoorsWebManager::CNdoorsWebManager()
{

}

CNdoorsWebManager::~CNdoorsWebManager()
{

}

/************************************************************************/
/* 팝업
/************************************************************************/
void CNdoorsWebManager::Navigate_Popup( CString & strLoginURL,  CString & strPost , UINT cx, UINT cy, HWND * hWnd  )
{
	CoInitialize(NULL);

	IWebBrowser2 *pWebBrowser = NULL;

	CString strHeader = "Content-Type: application/x-www-form-urlencoded\r\n"; 
	CString strTarget = "_top"; 

	VARIANT vtURL, vtHeader, vtTarget, vtFlag, vtPost; 

	VariantInit(&vtURL);
	VariantInit(&vtHeader);
	VariantInit(&vtTarget);
	VariantInit(&vtFlag);
	VariantInit(&vtPost);

	vtURL.vt	= VT_BSTR;
	vtHeader.vt = VT_BSTR; 
	vtTarget.vt = VT_BSTR; 
	vtFlag.vt = VT_I4;

	vtURL.bstrVal = strLoginURL.AllocSysString();
	vtHeader.bstrVal = strHeader.AllocSysString(); 
	vtTarget.bstrVal = strTarget.AllocSysString();
	vtFlag.lVal = 0;	//navNoHistory | navAllowAutosearch;

	int ScreenWidth	= GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight	= GetSystemMetrics(SM_CYSCREEN);

	LPSAFEARRAY psa;
	LPSTR pPostData;
	psa = SafeArrayCreateVector(VT_UI1, 0, strPost.GetLength() );
	if(!psa)
	{
		return; 
	}
	SafeArrayAccessData(psa, (LPVOID*)&pPostData);
	memcpy(pPostData, strPost.GetString(), strPost.GetLength() );
	SafeArrayUnaccessData(psa);

	vtPost.vt = VT_ARRAY | VT_UI1;
	vtPost.parray = psa;

	::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser);

 	pWebBrowser->put_Width(cx);
	pWebBrowser->put_Height(cy); // 창의 크기
	pWebBrowser->put_Left( (ScreenWidth - cx)/2 ); // 위치
	pWebBrowser->put_Top( (ScreenHeight - cy)/2 );

	pWebBrowser->put_Resizable(VARIANT_FALSE); // 각 설정
	pWebBrowser->put_StatusBar(VARIANT_FALSE);
	pWebBrowser->put_ToolBar(VARIANT_FALSE);
	pWebBrowser->put_AddressBar(VARIANT_FALSE);
	pWebBrowser->put_MenuBar(VARIANT_FALSE);
	pWebBrowser->put_RegisterAsBrowser(VARIANT_FALSE);
	pWebBrowser->put_Visible(VARIANT_TRUE);
	

	pWebBrowser->Navigate2( &vtURL, &vtFlag, &vtTarget, &vtPost, &vtHeader); 
	
	
	SHANDLE_PTR hWebWnd;
	pWebBrowser->get_HWND( & hWebWnd);

	*hWnd = (HWND) hWebWnd;

	pWebBrowser->Release();
	CoUninitialize();
}

/************************************************************************/
/* 
/************************************************************************/
void CNdoorsWebManager::Navigate_PostType( CString & strLoginURL ,  CString & strPost  )
{
	CoInitialize(NULL);

	IWebBrowser2 *pWebBrowser = NULL;

	CString strHeader = "Content-Type: application/x-www-form-urlencoded\r\n"; 
	CString strTarget = "_top"; 

	VARIANT vtURL, vtHeader, vtTarget, vtFlag, vtPost; 

	VariantInit(&vtURL);
	VariantInit(&vtHeader);
	VariantInit(&vtTarget);
	VariantInit(&vtFlag);
	VariantInit(&vtPost);

	vtURL.vt	= VT_BSTR;
	vtHeader.vt = VT_BSTR; 
	vtTarget.vt = VT_BSTR; 
	vtFlag.vt = VT_I4;

	vtURL.bstrVal = strLoginURL.AllocSysString();
	vtHeader.bstrVal = strHeader.AllocSysString(); 
	vtTarget.bstrVal = strTarget.AllocSysString();
	vtFlag.lVal = 0;	//navNoHistory | navAllowAutosearch;

	LPSAFEARRAY psa;
	LPSTR pPostData;
	psa = SafeArrayCreateVector(VT_UI1, 0, strPost.GetLength() );
	if(!psa)
	{
		return; 
	}
	SafeArrayAccessData(psa, (LPVOID*)&pPostData);
	memcpy(pPostData, strPost.GetString(), strPost.GetLength() );
	SafeArrayUnaccessData(psa);

	vtPost.vt = VT_ARRAY | VT_UI1;
	vtPost.parray = psa;

	::CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pWebBrowser);

	//pWebBrowser->put_Height(720); // 창의 크기
	//pWebBrowser->put_Width(599);
	//pWebBrowser->put_Resizable(VARIANT_FALSE); // 각 설정
	//pWebBrowser->put_StatusBar(VARIANT_FALSE);
	pWebBrowser->put_ToolBar(VARIANT_TRUE);
	pWebBrowser->put_AddressBar(VARIANT_TRUE);
	pWebBrowser->put_MenuBar(VARIANT_TRUE);
	pWebBrowser->put_RegisterAsBrowser(VARIANT_FALSE);
	pWebBrowser->put_Visible(VARIANT_TRUE);

	pWebBrowser->Navigate2( &vtURL, &vtFlag, &vtTarget, &vtPost, &vtHeader); 

	/*//{{ 스크롤 없애기 - 문서 로드 완료 이벤트 후에 들어가야됨
	HRESULT hr = S_OK;
	IDispatch* pDocDisp = NULL;
	hr = pWebBrowser->get_Document(&pDocDisp);
	if (pDocDisp)
	{ 
	VARIANT v;
	VariantInit(&v);
	IHTMLDocument2* pDoc = NULL; 
	hr = pDocDisp->QueryInterface(IID_IHTMLDocument2, reinterpret_cast<void **>(&pDoc));
	IHTMLElement* pElement = NULL; 
	hr = pDoc->get_body(&pElement);
	IHTMLBodyElement* pBodyElement = NULL; 
	hr = pElement->QueryInterface(IID_IHTMLBodyElement, (void**)&pBodyElement); 
	if (pBodyElement) { 
	pBodyElement->put_scroll(::SysAllocString(L"no")); //show : auto 
	pBodyElement->Release();
	}
	pElement->Release(); 
	pDoc->Release(); 
	pDocDisp->Release(); 
	//}}
	}*/
	pWebBrowser->Release();
	CoUninitialize();
}
