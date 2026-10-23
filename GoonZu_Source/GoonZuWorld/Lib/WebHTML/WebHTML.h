/*
 * This include file is meant to be included with any C source you
 * write which uses the cwebpage DLL.
 */

#ifndef __WEBHTML_H_INCLUDED
#define __WEBHTML_H_INCLUDED

#include <windows.h>



#ifdef __cplusplus
extern "C" 
{
#endif

HWND CreateBrowserWindow();

DWORD InitializeWebEvents( HWND hWnd );
BOOL DestroyWebEvents(HWND hwnd, DWORD dwAdviseCookie);

long WINAPI EmbedBrowserObject(HWND);
#define EMBEDBROWSEROBJECT EmbedBrowserObject
typedef long WINAPI EmbedBrowserObjectPtr(HWND);

void WINAPI UnEmbedBrowserObject(HWND);
#define UNEMBEDBROWSEROBJECT UnEmbedBrowserObject
typedef long WINAPI UnEmbedBrowserObjectPtr(HWND);

long WINAPI DisplayHTMLPage(HWND, LPCTSTR);
#define DISPLAYHTMLPAGE DisplayHTMLPage
typedef long WINAPI DisplayHTMLPagePtr(HWND, LPCTSTR);

long WINAPI DisplayHTMLStr(HWND, LPCTSTR);
#define DISPLAYHTMLSTR DisplayHTMLStr
typedef long WINAPI DisplayHTMLStrPtr(HWND, LPCTSTR);

void WINAPI ResizeBrowser(HWND, DWORD, DWORD);
#define RESIZEBROWSER ResizeBrowser
typedef void WINAPI ResizeBrowserPtr(HWND, DWORD, DWORD);

#define WEBPAGE_GOBACK		0
#define WEBPAGE_GOFORWARD	1
#define WEBPAGE_GOHOME		2
#define WEBPAGE_SEARCH		3
#define WEBPAGE_REFRESH		4
#define WEBPAGE_STOP		5

void WINAPI DoPageAction(HWND, DWORD);
#define DOPAGEACTION DoPageAction
typedef void WINAPI DoPageActionPtr(HWND, DWORD);




#ifdef __cplusplus
}
#endif

#endif /* __WEBHTML_H_INCLUDED */
