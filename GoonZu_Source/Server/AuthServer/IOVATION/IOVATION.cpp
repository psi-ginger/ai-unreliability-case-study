// IOVATION.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "IOVATION.h"
#include <string>

#import "msxml4.dll"
using namespace MSXML2;

#import "C:\Program Files\Common Files\MSSoap\Binaries\mssoap30.dll" \
			exclude("IStream", "IErrorInfo", "ISequentialStream", "_LARGE_INTEGER", \
			"_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")
using namespace MSSOAPLib30;

//#include "gSOAP/soapStub.h"

/*------------------------- GLOBAL VALUES  --------------------------*/
std::string g_LastError;
std::string g_SubScriberID;
std::string g_AdminCode;
std::string g_AdminPassword;
std::string g_Type;

std::string g_Reason;
std::string g_Devicealias;
std::string g_TrackingNumber;

IOVATION_API int Init_IOVATION(const char* pszSubScriberID, const char* pszAdminCode, const char* pszAdminPassword, const char* pszType)
{
	if( pszSubScriberID )		g_SubScriberID	= pszSubScriberID;
	if( pszAdminCode )			g_AdminCode		= pszAdminCode;
	if( pszAdminPassword )		g_AdminPassword	= pszAdminPassword;
	if( pszType )				g_Type			= pszType;

	CoInitialize( NULL );

	return 1;
}

IOVATION_API int Shutdown_IOVATION(void)
{
	CoUninitialize();

	return 1;
}

void AddLastError(const char* pszMessage, HRESULT hr)
{
	if( pszMessage == NULL )			return;

	char szBuf[2048];
	_snprintf( szBuf, sizeof(szBuf), "MESSAGE:[%s] HRESULT:[%d]", pszMessage, hr);

	g_LastError += szBuf;
}

IOVATION_API int CheckTransaction(const char* pszURL, const char* pszID, const char* pszBlackBox, const char* pszIP)
{
	int nRet = IOVATION_RESULT_ERROR;

	HRESULT hr;

	ISoapSerializerPtr		Serializer;
	ISoapReaderPtr			Reader;
	ISoapConnectorPtr		Connector;

	try
	{
		hr = Connector.CreateInstance( __uuidof(HttpConnector30) );
		if( FAILED(hr) )		{		AddLastError("Connector.CreateInstance()", hr);		return nRet;	}
		Connector->Property["EndPointURL"] = pszURL;
		hr = Connector->Connect();
		if( FAILED(hr) )		{		AddLastError("Connector->Connect()", hr);			return nRet;	}

		std::string	soapAction;
		soapAction  = pszURL;
		soapAction += "#CheckTransaction";

		Connector->Property["SoapAction"] = soapAction.c_str();
		hr = Connector->BeginMessage();
		if( FAILED(hr) )		{		AddLastError("Connector->BeginMessage()", hr);		return nRet;	}

		hr = Serializer.CreateInstance( __uuidof(SoapSerializer30) );
		if( FAILED(hr) )		{		AddLastError("Serializer.CreateInstance()", hr);	return nRet;	}
		hr = Serializer->Init( _variant_t((IUnknown*)Connector->InputStream) );
		if( FAILED(hr) )		{		AddLastError("Serializer->Init()", hr);				return nRet;	}

		Serializer->StartEnvelope("", "", "");
		{
			Serializer->StartBody("");
			{
				Serializer->StartElement("CheckTransaction", "", "", "");
				{
					Serializer->StartElement("subscriberid", "", "", "" );
					Serializer->WriteString( g_SubScriberID.c_str() );
					Serializer->EndElement();

					Serializer->StartElement("subscriberaccount", "", "", "" );
					Serializer->WriteString( g_AdminCode.c_str() );
					Serializer->EndElement();

					Serializer->StartElement("subscriberpasscode", "", "", "" );
					Serializer->WriteString( g_AdminPassword.c_str() );
					Serializer->EndElement();

					Serializer->StartElement("enduserip", "", "", "" );
					Serializer->WriteString( pszIP );
					Serializer->EndElement();

					Serializer->StartElement("accountcode", "", "", "" );
					Serializer->WriteString( pszID );
					Serializer->EndElement();

					Serializer->StartElement("beginblackbox", "", "", "" );
					Serializer->WriteString( pszBlackBox );
					Serializer->EndElement();

					Serializer->StartElement("type", "", "", "" );
					Serializer->WriteString( g_Type.c_str() );
					Serializer->EndElement();
				}
				Serializer->EndElement();
			}
			Serializer->EndBody();
		}
		Serializer->EndEnvelope();

		hr = Connector->EndMessage();
		if( FAILED(hr) )		{		AddLastError("Connector->EndMessage()", hr);			return nRet;	}

		hr = Reader.CreateInstance( __uuidof(SoapReader30) );
		if( FAILED(hr) )		{		AddLastError("Reader.CreateInstance()", hr);			return nRet;	}
		hr = Reader->Load( _variant_t((IUnknown*)Connector->OutputStream), "" );

	//	MSXML2::IXMLDOMDocumentPtr pDom = Reader->GetDom();
	//	pDom->save( "SOAPResult.xml" );

		MSXML2::IXMLDOMElementPtr pBody = Reader->GetBody();
		if( pBody )
		{
			for( int i=0; i<pBody->childNodes->length; i++ )
			{
				MSXML2::IXMLDOMNodePtr pNode = pBody->childNodes->item[i];
				_bstr_t bstrNodeName = pNode->GetnodeName();

				if( wcscmp(bstrNodeName, L"namesp1:CheckTransactionResponse") == 0 )
				{
					for( int j=0; j<pNode->childNodes->length; j++ )
					{
						_bstr_t bstrNodeName = pNode->childNodes->item[j]->GetnodeName();
						_bstr_t bstrText = pNode->childNodes->item[j]->Gettext();
						if( wcscmp(bstrNodeName, L"namesp1:result") == 0 )
						{
							if( wcscmp( bstrText, L"A") == 0 )
							{
								nRet = IOVATION_RESULT_ALLOW;
							}
							else if( wcscmp( bstrText, L"D") == 0 )
							{
								nRet = IOVATION_RESULT_DENY;
							}
							else if( wcscmp( bstrText, L"R") == 0 )
							{
								nRet = IOVATION_RESULT_REVIEW;
							}
						}
						else if( wcscmp(bstrNodeName, L"namesp1:reason") == 0 )
						{
							g_Reason = bstrText;
						}
						else if( wcscmp(bstrNodeName, L"namesp1:devicealias") == 0 )
						{
							g_Devicealias = bstrText;
						}
						else if( wcscmp(bstrNodeName, L"namesp1:trackingnumber") == 0 )
						{
							g_TrackingNumber = bstrText;
						}
					}
				}
				else
				{
					// 오류 기록
					MSXML2::IXMLDOMElementPtr pFault = Reader->GetFault();
					if( pFault )
					{
						for( int i=0; i<pFault->childNodes->length; i++ )
						{
							_bstr_t bstrName = pFault->childNodes->item[i]->GetnodeName();
							MSXML2::IXMLDOMNodePtr pNode = pFault->childNodes->item[i];

							if( wcscmp(bstrName, L"faultcode") == 0 || 
								wcscmp(bstrName, L"faultstring") == 0 || 
								wcscmp(bstrName, L"faultactor") == 0 )
							{
								if( g_LastError.length() > 0 )	{		g_LastError += " ";		}
								g_LastError += bstrName;
								g_LastError += ":[";
								g_LastError += pNode->Gettext();
								g_LastError += "]";
							}
						}
					}
					else
					{
						AddLastError("Reader->GetFault() Failed", 0);
					}
				}
			}
		}
		else
		{
			AddLastError("Reader->GetBody() Failed", 0);
		}
	}
	catch( ... )
	{
		AddLastError("Exception Occur", GetLastError());
	}

	return nRet;
}
/*	struct soap *soap = soap_new();
	_ns1__CheckTransaction req;
	_ns1__CheckTransactionResponse resp;

	std::string* pType = new std::string( "default" );

	req.accountcode = new std::string( pszID );
	req.beginblackbox = new std::string( pszBlackBox );
	req.enduserip = new std::string( pszIP );
	req.subscriberid = g_SubScriberID;
	req.subscriberaccount = g_AdminCode;
	req.subscriberpasscode = g_AdminPassword;
	req.type = pType;

	const char **pError = NULL;
	if ( soap_call___ns1__CheckTransaction( soap, pszURL, NULL, &req, &resp ) == SOAP_OK )
	{
		if ( resp.result.length() > 0 )
		{
			if( resp.result.compare( "A" ) == 0 )
			{
				nRet = IOVATION_RESULT_ALLOW;
			}
			else if ( resp.result.compare( "D" ) == 0 )
			{
				nRet = IOVATION_RESULT_DENY;
			}
			else if ( resp.result.compare( "R" ) == 0 )
			{
				nRet = IOVATION_RESULT_REVIEW;
			}
		}
		else
		{
			char szErr[1024];
			sprintf(szErr, "[SoapError:%d]", soap->error);
			g_LastError = szErr;

			pError = soap_faultstring( soap );
			if( *pError != NULL )	{	g_LastError += "[Desc:";	g_LastError += *pError;		g_LastError += "]";		}
		}
	}
	else
	{
		char szErr[1024];
		sprintf(szErr, "[SoapError:%d]", soap->error);
		g_LastError = szErr;

		pError = soap_faultstring( soap );
		if( *pError != NULL )	{	g_LastError += "[Desc:";	g_LastError += *pError;		g_LastError += "]";		}
	}

	if( pType )   delete pType;

	// clean up
	soap_cleanup( soap );
*/

IOVATION_API const char* GetLastErrorString()
{
	return g_LastError.c_str();
}

IOVATION_API const char* GetReasonString()
{
	return g_Reason.c_str();
}

IOVATION_API const char* GetDeviceAliasString()
{
	return g_Devicealias.c_str();
}

IOVATION_API const char* GetTrackingNumberString()
{
	return g_TrackingNumber.c_str();
}