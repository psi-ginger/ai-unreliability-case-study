// gb.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{


	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}


	return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif

	void __stdcall EncryptBill(int keyType , char *id, char *pw, char *ret )
	{

		String * skey = NULL;

		switch(keyType)
		{
		case 0:
			{
				skey = new String( "srA*15" );
			}
			break;
		case 1:
			{
				skey = new String( "0mm-M8*L3" );
			}
			break;
		case 2:
			{
				skey = new String( "0mm-M8*L3" );
			}
			break;
		default:
			{
				return;
			}
			break;
		}



		String *sid = new String( id );
		String *spw = new String( pw );

		gBillWeb::LoginCrypt *g = new gBillWeb::LoginCrypt( skey );

		String *r = g->Encrypt( sid, spw );

		__wchar_t t __gc[] = r->ToCharArray();

		int c = 0;
		char temp;

		for( int i = 0; i < r->get_Length(); ++i )
		{
			temp = (char)t[ i ];

			if( temp == '+' )
			{
				ret[ c++ ] = '%';
				ret[ c++ ] = '2';
				ret[ c++ ] = 'b';

			} else {
				ret[ c++ ] = temp;
			}
		}

		ret[ c ] = NULL;

		return;
	}


#ifdef __cplusplus
}
#endif