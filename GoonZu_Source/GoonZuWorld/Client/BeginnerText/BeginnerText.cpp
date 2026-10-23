#include "BeginnerText.h"
#include "../../../NLib/NDataLoader.h"
#include "../../../NLib/NFile.h"
#include "../../../NLib/NUtil.h"
#include "..\client.h"

extern cltCommonLogic* pclClient;

CBeginnerText::CBeginnerText()
{
	m_siIndex = 0;
	m_dwLastShowClock = 0;
	m_szHelpText = NULL;
}

CBeginnerText::~CBeginnerText()
{
	for( SI32 i = 0; i < m_siIndex; ++i )
	{
		NDelete( m_szHelpText[i] );
	}

	NDelete_Array( m_szHelpText );
}

void CBeginnerText::LoadBeginnerText( TCHAR* BeginnerText )
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0809 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( BeginnerText );		
	}
	else
	{
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile( BeginnerText );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile( BeginnerText );
		}
	}

	if (!bLoadFile)
	{
	//	MsgBox(TEXT("LoadBeginnerText()"), TEXT("LoadError:[%s]"), BeginnerText);
		return;
	}

	SI32 index = 0;
	TCHAR korHelpText[128] = { '\0' };
	TCHAR chinaHelpText[128] = { '\0' };

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&index,			4,		
		NDATA_MBSTRING,	korHelpText,	128,
		NDATA_MBSTRING,	chinaHelpText,	128,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 count = 0;
	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData() )
		{
			if( 0 == count )
			{
				m_siIndex = index;
				m_szHelpText = new TCHAR*[ m_siIndex ];
			}
			else
			{
				SI32 size = _tcslen( chinaHelpText );

				if( index - 1 < m_siIndex )
				{
					m_szHelpText[index - 1] = new TCHAR[ size + 1 ];
					StringCchPrintf( m_szHelpText[index - 1], size + 1, chinaHelpText );	
				}
				else
				{
#ifdef _DEBUG
				MsgBox(TEXT("LoadBeginnerText()"), TEXT("LoadError:[%s]"), BeginnerText);
#endif
				}
			}
			count++;
		}
	}
}

void CBeginnerText::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( TABS( pclclient->CurrentClock - m_dwLastShowClock ) > 1000 * 60 )
	{
		SI32 index = rand() % m_siIndex;
		pclclient->pclMessage->SetMsg( m_szHelpText[index] , 0, RGB(255, 255, 0),RGB(0,0,0));
		m_dwLastShowClock = pclclient->CurrentClock;
	}
}
/*
TCHAR* CBeginnerText::GetHelpText( SI32 Index )
{
}
*/
