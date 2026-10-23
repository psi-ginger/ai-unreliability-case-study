#include "ITZHttpConnect.h"


// This macro returns the constant name in a string.
#define CASE_OF(constant)   case constant: return (L# constant)

LPCWSTR GetApiErrorString(DWORD dwResult)
{
    // Return the error result as a string so that the
    // name of the function causing the error can be displayed.
    switch(dwResult)
    {
        CASE_OF( API_RECEIVE_RESPONSE );
        CASE_OF( API_QUERY_DATA_AVAILABLE );
        CASE_OF( API_READ_DATA );
        CASE_OF( API_WRITE_DATA );
        CASE_OF( API_SEND_REQUEST );
    }
    return L"Unknown function";

}


void __stdcall AsyncCallback( HINTERNET hInternet, DWORD_PTR pHttpConnect,
                              DWORD dwInternetStatus,
                              LPVOID lpvStatusInformation,
                              DWORD dwStatusInformationLength)
{
	ITZHttpConnect *pClass = (ITZHttpConnect *)pHttpConnect;

	WINHTTP_ASYNC_RESULT *pAR;
	
	WCHAR szBuffer[256];    
    szBuffer[0] = 0;

    // Create a string that reflects the status flag.
    switch( dwInternetStatus )
    {
        case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
			{

//				printf(  TEXT("SENDREQUEST_COMPLETE (%d)\n"), dwStatusInformationLength );
			
				// Prepare the request handle to receive a response.
				if( WinHttpReceiveResponse( pClass->m_hRequest, NULL ) == FALSE )
				{
					pClass->Cleanup();
				}
			}
            break;

        case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
			{
//				printf(  TEXT("HEADERS_AVAILABLE (%d)\n"), dwStatusInformationLength );

				pClass->QueryHeader();            

				// Initialize the buffer sizes.				
				pClass->m_dwTotalSize = 0;

				// Begin downloading the resource.
				if( pClass->QueryData() == FALSE )
				{
					pClass->Cleanup();
				}
			}
            break;

        case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
			{
//				printf(  TEXT("DATA_AVAILABLE (%d)\n"), dwStatusInformationLength );

				DWORD dwSize = *((LPDWORD)lpvStatusInformation);

				// If there is no data, the process is complete.
				if( dwSize == 0 )
				{
					InterlockedExchange( (LPLONG)&pClass->m_bComplete, TRUE );
					/*					
					// All of the data has been read.  Display the data.
					if (cpContext->dwTotalSize)
					{
						// Convert the final context buffer to wide characters,
						// and display.
						LPWSTR lpWideBuffer = new WCHAR[cpContext->dwTotalSize + 1];
						MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, 
											 cpContext->lpBuffer, 
											 cpContext->dwTotalSize, 
											 lpWideBuffer, 
											 cpContext->dwTotalSize);
						lpWideBuffer[cpContext->dwTotalSize] = 0;

						// Delete the remaining data buffers.
						delete [] lpWideBuffer;
						delete [] cpContext->lpBuffer;
						cpContext->lpBuffer = NULL;
					}

					// Close the request and connect handles for this context.
*/
					pClass->m_pResourceBuffer[ pClass->m_dwTotalSize++ ] = NULL;
					pClass->Cleanup();

				}
				else
					// Otherwise, read the next block of data.
					if( pClass->ReadData( dwSize ) == FALSE )
					{
						pClass->Cleanup();
					}
			}
            break;
        case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
			{

//				printf(  "READ_COMPLETE (%d)\n", dwStatusInformationLength );
	
				// Copy the data and delete the buffers.
				if( dwStatusInformationLength != 0 )
				{
					pClass->CompleteRecvData( (LPSTR)lpvStatusInformation, dwStatusInformationLength );

					// Check for more data.
					if( pClass->QueryData() == FALSE)
					{
						pClass->Cleanup();
					}
				}
			}
			break;

		case WINHTTP_CALLBACK_STATUS_REDIRECT:
			{
//				printf(  "REDIRECT (%d)\n", dwStatusInformationLength );
			}
			break;
		case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
			{
				pAR = (WINHTTP_ASYNC_RESULT *)lpvStatusInformation;
	//            swprintf( szBuffer,L"%s: REQUEST_ERROR - error %d, result %s", 
	 //                     cpContext->szMemo, pAR->dwError, 
	  //                    GetApiErrorString(pAR->dwResult));

				pClass->Cleanup();
			}
			break;
		default:
	//        swprintf(szBuffer,L"%s: Unknown/unhandled callback - status %d given",
	  //          cpContext->szMemo, dwInternetStatus);
            break;
    }

    // Add the callback information to the listbox.
    //SendDlgItemMessage( cpContext->hWindow, IDC_CBLIST, LB_ADDSTRING, 0, 
      //                  (LPARAM)szBuffer);

}


ITZHttpConnect::ITZHttpConnect()
{
	m_hSession = NULL;
	m_hConnect = NULL;
	m_hRequest = NULL;

	m_pRecvUnitBuffer = NULL;
	m_pResourceBuffer = NULL;

	m_dwMaxRecvUnitBufferSize = 0;
	m_dwMaxResourceBufferSize = 0;

	m_dwTotalSize = 0;

	m_bComplete = FALSE;
}

ITZHttpConnect::~ITZHttpConnect()
{

	if( m_hSession ) {
		// Close the session handle.
        WinHttpCloseHandle( m_hSession );
	}

	if( m_pRecvUnitBuffer ) delete [] m_pRecvUnitBuffer;
	if( m_pResourceBuffer ) delete [] m_pResourceBuffer;
}

void ITZHttpConnect::Init()
{
	Init( SO_MAX_MSG_SIZE, SO_MAX_MSG_SIZE * 10 );
}

void ITZHttpConnect::Init( SI32 recvUnitBufferSize, SI32 resourceBufferSize )
{
	if( m_pRecvUnitBuffer ) delete [] m_pRecvUnitBuffer;
	if( m_pResourceBuffer ) delete [] m_pResourceBuffer;
	
	m_pRecvUnitBuffer = new char[ recvUnitBufferSize ];
	m_pResourceBuffer = new char[ resourceBufferSize ];

	memset(m_pRecvUnitBuffer, 0, recvUnitBufferSize);
	memset(m_pResourceBuffer, 0, resourceBufferSize);

	m_dwMaxRecvUnitBufferSize = recvUnitBufferSize;
	m_dwMaxResourceBufferSize = resourceBufferSize;

	m_dwTotalSize = 0;
}

void ITZHttpConnect::HttpOpen()
{

	if( m_hSession ) {
		WinHttpCloseHandle( m_hSession );
	}

	m_hSession = WinHttpOpen( L"GoonZu Http", 
                             WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                             WINHTTP_NO_PROXY_NAME,
                             WINHTTP_NO_PROXY_BYPASS,
                             WINHTTP_FLAG_ASYNC );
}

void ITZHttpConnect::Cleanup()
{
    // Set the memo to indicate a closed handle.
//    swprintf(cpContext->szMemo, L"Closed");

    if( m_hRequest )
    {
        WinHttpSetStatusCallback( m_hRequest, 
                NULL, 
                NULL, 
                NULL);

        WinHttpCloseHandle( m_hRequest );
		m_hRequest = NULL;
    }

    if( m_hConnect )
    {
        WinHttpCloseHandle( m_hConnect );
		m_hConnect = NULL;
    }

    // note: this function can be called concurrently by differnet threads, therefore any global data
    // reference needs to be protected
/*
    EnterCriticalSection(&g_CallBackCritSec);

    // If both handles are closed, re-enable the download button.
    if ((wcsncmp( rcContext1.szMemo, L"Closed",6)==0) &&
        (wcsncmp( rcContext2.szMemo, L"Closed",6)==0))
    {
        EnableWindow( GetDlgItem(cpContext->hWindow, IDC_DOWNLOAD),1);
    }

    LeaveCriticalSection(&g_CallBackCritSec);
*/
}

BOOL ITZHttpConnect::SendRequest( LPWSTR szURL )
{
	InterlockedExchange( (LPLONG)&m_bComplete, (LONG)FALSE );

    WCHAR szHost[256];
    DWORD dwOpenRequestFlag = 0;
    URL_COMPONENTS urlComp;
    BOOL fRet = FALSE;
	WINHTTP_STATUS_CALLBACK pCallback;

    // Initialize URL_COMPONENTS structure.
    ZeroMemory(&urlComp, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);

    // Use allocated buffer to store the Host Name.
    urlComp.lpszHostName        = szHost;
    urlComp.dwHostNameLength    = sizeof(szHost) / sizeof(szHost[0]);

    // Set non zero lengths to obtain pointer to the URL Path.
    /* note: if we threat this pointer as a NULL terminated string
            this pointer will contain Extra Info as well. */
    urlComp.dwUrlPathLength = (DWORD)-1;

    // Crack HTTP scheme.
    urlComp.dwSchemeLength = (DWORD)-1;

    // Set the szMemo string.
//	printf( "WinHttpCrackURL\n" );

    // Crack the URL.
    if(!WinHttpCrackUrl( szURL, 0, 0, &urlComp) )
    {
        goto cleanup;
    }

    // Set the szMemo string.
//	printf(  "WinHttpConnect\n" );

    // Open an HTTP session.
    m_hConnect = WinHttpConnect( m_hSession, szHost, urlComp.nPort, 0);
    if( m_hConnect == NULL )
    {
        goto cleanup;
    }
    
    // Prepare OpenRequest flag
    dwOpenRequestFlag = ( INTERNET_SCHEME_HTTPS == urlComp.nScheme ) ?
                            WINHTTP_FLAG_SECURE : 0;

    // Set the szMemo string.
//	printf(  "WinHttpOpenRequest\n" );

    // Open a "GET" request.
    m_hRequest = WinHttpOpenRequest( m_hConnect, 
                                     L"GET", urlComp.lpszUrlPath,
                                     NULL, WINHTTP_NO_REFERER, 
                                     WINHTTP_DEFAULT_ACCEPT_TYPES,
                                     dwOpenRequestFlag );

    if( m_hRequest == NULL )
    {
        goto cleanup;
    }

    // Set the szMemo string.
    //swprintf( cpContext->szMemo, L"WinHttpSetStatusCallback (%d)", cpContext->nURL);

    // Install the status callback function.
    pCallback = WinHttpSetStatusCallback( m_hRequest,
                            (WINHTTP_STATUS_CALLBACK)AsyncCallback,
                            WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS | 
                            WINHTTP_CALLBACK_FLAG_REDIRECT,    
                            NULL);

    // note: On success WinHttpSetStatusCallback returns the previously defined callback function.
    // Here it should be NULL
    if( pCallback != NULL)
    {
        goto cleanup;
    }

    // Set the szMemo string.
    //swprintf( cpContext->szMemo, L"WinHttpSendRequest (%d)", cpContext->nURL);

    // Send the request.
    if (!WinHttpSendRequest( m_hRequest, 
                        WINHTTP_NO_ADDITIONAL_HEADERS, 0, 
                        WINHTTP_NO_REQUEST_DATA, 0, 0, 
                        (DWORD_PTR)this))
    {
        goto cleanup;
    }

    fRet = TRUE;
 
cleanup:

    if (fRet == FALSE)
    {
//       WCHAR szError[256];

        // Set the error message.
//       swprintf(szError, L"%s failed with error %d", cpContext->szMemo, GetLastError());

        // Cleanup handles.
        Cleanup();
                    
        // Display the error message.
		//SetDlgItemText(cpContext->hWindow, cpContext->nResource, szError);

    }
                
    return fRet;
}


BOOL ITZHttpConnect::QueryHeader() 
{
    DWORD dwSize = 0;
    LPVOID lpOutBuffer = NULL;

    // Set the state memo.
    //swprintf(cpContext->szMemo, L"WinHttpQueryHeaders (%d)", cpContext->nURL);

    // Use HttpQueryInfo to obtain the size of the buffer.
    if( !WinHttpQueryHeaders( m_hRequest, 
                              WINHTTP_QUERY_RAW_HEADERS_CRLF,
                              WINHTTP_HEADER_NAME_BY_INDEX, 
							  NULL, 
							  &dwSize, 
							  WINHTTP_NO_HEADER_INDEX ) )
    {
        // An ERROR_INSUFFICIENT_BUFFER is expected because you
        // are looking for the size of the headers.  If any other
        // error is encountered, display error information.
        DWORD dwErr = GetLastError();
        if (dwErr != ERROR_INSUFFICIENT_BUFFER)
        {
//            WCHAR  szError[256];
//            swprintf( szError, L"%s: Error %d encountered.",
//                      cpContext->szMemo, dwErr);
//            SetDlgItemText( cpContext->hWindow, cpContext->nResource, szError);
            return FALSE;
        }
    }
    
    // Allocate memory for the buffer.
    lpOutBuffer = new WCHAR[dwSize];

    // Use HttpQueryInfo to obtain the header buffer.
    if( WinHttpQueryHeaders( m_hRequest, 
                             WINHTTP_QUERY_RAW_HEADERS_CRLF,
                             WINHTTP_HEADER_NAME_BY_INDEX, 
							 lpOutBuffer, 
							 &dwSize, 
							 WINHTTP_NO_HEADER_INDEX))
//        SetDlgItemText( cpContext->hWindow, cpContext->nHeader, 
//                       (LPWSTR)lpOutBuffer);

    // Free the allocated memory.
    delete [] lpOutBuffer;
	lpOutBuffer = NULL;

    return TRUE;
}



BOOL ITZHttpConnect::QueryData()
{
    // Set the state memo.
//	printf(  "WinHttpQueryDataAvailable\n" );

    // Chech for available data.
    if( WinHttpQueryDataAvailable( m_hRequest, NULL) == FALSE )
    {
        // If a synchronous error occured, display the error.  Otherwise
        // the query is successful or asynchronous.
        DWORD dwErr = GetLastError();
//        WCHAR szError[256];
//        swprintf( szError, L"%s: Error %d encountered.",
//                  cpContext->szMemo, dwErr);
//        SetDlgItemText( cpContext->hWindow, cpContext->nResource, szError);
        return FALSE;
    }
    return TRUE;
}


void ITZHttpConnect::CompleteRecvData( LPSTR lpReadBuffer, DWORD dwBytesRead )
{

	if( m_dwMaxResourceBufferSize - 1 < m_dwTotalSize + dwBytesRead ) {

		int siBytesRead = m_dwMaxResourceBufferSize - m_dwTotalSize - 1;

		if( siBytesRead > 0 ) {
		
			dwBytesRead = (DWORD)siBytesRead;

		} else {

			return;
		}

	}
		
	memcpy( &m_pResourceBuffer[ m_dwTotalSize ], lpReadBuffer, dwBytesRead );

	m_dwTotalSize += dwBytesRead;
}


BOOL ITZHttpConnect::ReadData( DWORD dwSize )
{
	if( dwSize > m_dwMaxRecvUnitBufferSize ) dwSize = m_dwMaxRecvUnitBufferSize;

    // Set the state memo.
//	printf( "WinHttpReadData\n" );

    // Read the available data.
    if( WinHttpReadData( m_hRequest, (LPVOID)m_pRecvUnitBuffer, 
                          dwSize, NULL) == FALSE)
    {
        // If a synchronous error occurred, display the error.  Otherwise
        // the read is successful or asynchronous.
        DWORD dwErr = GetLastError();
//        WCHAR  szError[256];
//        swprintf( szError, L"%s: Error %d encountered.",
//                  cpContext->szMemo, dwErr);
//      SetDlgItemText( cpContext->hWindow, cpContext->nResource, szError);
        return FALSE;
    }

    return TRUE;
}

