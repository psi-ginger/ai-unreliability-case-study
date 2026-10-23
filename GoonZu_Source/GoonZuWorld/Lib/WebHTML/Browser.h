#ifndef _BROWSER_H_
#define _BROWSER_H_

#include <windows.h>
#include <exdisp.h>		/* Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above */
#include <ExDispid.h>	/* DISPID_NEWWINDOW2 */
#include <mshtml.h>		/* Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above */
#include <mshtmhst.h>	// Defines of stuff like IDocHostUIHandler. This is an include file with Visual C 6 and above
#include <crtdbg.h>		/* for _ASSERT() */

class CBrowser : 
public IOleContainer,
public IOleClientSite,
public IOleInPlaceSite,
public IOleControlSite,
public DWebBrowserEvents2
{
public:
	// Declare new and delete clear allocators.
    inline void * __cdecl operator new(size_t cb) { return LocalAlloc(LPTR, cb); } 
    inline void __cdecl operator delete(void * pv) { LocalFree(pv); }
	// Constructor and Destructor
	CBrowser();
	~CBrowser();
	//IUnknown methods
	STDMETHOD(QueryInterface) (REFIID riid, LPVOID * ppv)
	{
		//QueryInterface for IoleContainer.
		if ((riid == IID_IOleContainer) || (riid == IID_IUnknown))
		{
			//RETAILMSG(1, (L"Browser QueryInterface for IOleContainer"));
			*ppv = (IOleContainer *) this;
		}     
		else// if ()//...//Inerface)
		{
			//...Query Interface for other interfaces.
		}
		AddRef();
		return S_OK;
	}
	STDMETHOD_(ULONG, AddRef) (void)
	{
		return S_OK;
		//...
	}
	
	STDMETHOD_(ULONG, Release) (void)
	{
		return S_OK;
		//...
	}
	//...// IOleContainer methods
	//...// IOleClientSite methods
	//...// IOleWindow methods
	//...// IOleInPlaceSite methods
	//...// IOleControlSite methods
	//...// DWebBrowserEvents
	HRESULT STDMETHODCALLTYPE Invoke
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
		);
	//...// IDispatch methods
	// public methods
	
	
	
	HRESULT StartBrowser(LPWSTR szURL);     // Start a browser.
	HRESULT NavigateToURL(LPWSTR szURL);    // Browse to a URL.
	void CloseBrowser(); // Close the browser.
	
	//static WndProc wrapper and actual WndProc
	//static LRESULT CALLBACK s_BrowseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	//LRESULT CALLBACK BrowseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	
private:	
	HRESULT CreateBrowserWindow();    // Create the main window.
	HRESULT CreateBrowserObject();    // Create a web browser object.
	HRESULT RegisterBrowserEventSink(); // Connect up the event sink
	// private members
	HWND _hWnd;   //The main window
	IOleObject *_pObject; // The browser object
	IConnectionPoint *_pCP;  // Pointer to the browser event connection-point (for DWebBrowserEvents2)
	IWebBrowser2 *_pWB2; // Pointer to the Web browser object
	BOOL _bInPlaceActive;  // Determines if the document is active
	DWORD _dwEventCookie; // Stores cookies that will be received when the 
	// browser registers to receive DWebBrowserEvents2
	ULONG _ulRefs; //reference count for the interfaces supported by the 
	//container.
};


#endif