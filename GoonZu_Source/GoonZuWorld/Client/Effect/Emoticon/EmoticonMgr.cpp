#include "EmoticonMgr.h"
#include "CommonHeader.h"
#include "NDataLoader.h"
#include "CommonLogic.h"
#include "Graphic_Capsule.h"
#include "Client.h"

extern cltCommonLogic* pclClient;

CEmoticonInfo::CEmoticonInfo()
{
	ZeroMemory( m_siStartFrame, sizeof(SI16) * MAX_EMOTICON_NUMBER );
	ZeroMemory( m_siAniStepNumber, sizeof(SI16) * MAX_EMOTICON_NUMBER );
	ZeroMemory( m_siLastAniDelay, sizeof(SI16) * MAX_EMOTICON_NUMBER );
	ZeroMemory( m_szCode, 4 * MAX_EMOTICON_NUMBER );
	ZeroMemory( m_szInfo, 15 * MAX_EMOTICON_NUMBER );
}

CEmoticonInfo::~CEmoticonInfo()
{
	Destroy();
}

void CEmoticonInfo::Initialize()
{
	LoadEmoticonInfo();
}

void CEmoticonInfo::Destroy()
{
}

inline SI16 CEmoticonInfo::GetAniStepNumber(const SI16 index ) const 
{
	return m_siAniStepNumber[index];
}

inline SI16 CEmoticonInfo::GetLastAniDelay(const SI16 index) const 
{
    return m_siLastAniDelay[index];    
}

inline SI16 CEmoticonInfo::GetStartFrame(const SI16 index ) const 
{
	return m_siStartFrame[index];
}
TCHAR* CEmoticonInfo::GetCode(const SI16 index )
{
	return m_szCode[index];
}
TCHAR* CEmoticonInfo::GetInfo(const SI16 index )
{
	return m_szInfo[index];
}

SI16 CEmoticonInfo::GetEmoticonCount()
{
	for ( SI16 i=0; i < MAX_EMOTICON_NUMBER; i++ )
	{
		if ( m_siAniStepNumber[i] <= 0 )
		{
			return i;
		}
	}
	return MAX_EMOTICON_NUMBER;
}

void CEmoticonInfo::LoadEmoticonInfo()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/EmoticonInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/EmoticonInfo.txt") );
	}

	if (!bLoadFile)
	{
		return;
	}

	SI32 siIndex = 0;
	TCHAR szInfo[MAX_PATH] = TEXT("");
	TCHAR szCode[4] = TEXT("");
	SI32 siStartFrame = 0;
	SI32 siAniNum = 0;
	SI32 siLastDelay = 0;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&siIndex,	4,
		NDATA_MBSTRING,	szInfo,		MAX_PATH,
		NDATA_MBSTRING, szCode,		4,
		NDATA_INT32,	&siStartFrame,	4,
		NDATA_INT32,	&siAniNum,	4,
		NDATA_INT32,	&siLastDelay, 4,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if ( index >= MAX_EMOTICON_NUMBER ) break;

		if (dataloader.ReadData())	
		{
			m_siStartFrame[index] = (SI16)siStartFrame;
			m_siAniStepNumber[index] = (SI16)siAniNum;
			m_siLastAniDelay[index] = (SI16)siLastDelay;
			StringCchCopy( m_szCode[index], 4, szCode );
			StringCchCopy( m_szInfo[index], 15, szInfo );
			++index;
		}
	}
}


CEmoticonEffect::CEmoticonEffect( TSpr *pEmoticonSpr)
{
	if( pEmoticonSpr == NULL) return;

	m_pEmoticonSPR = pEmoticonSpr;

	Initialize();
}

CEmoticonEffect::~CEmoticonEffect()
{

}

void CEmoticonEffect::Initialize()
{
	m_EmoticonClock = pclClient->CurrentClock;
	m_siCurrentDrawFrame = 0;
	m_siDelayCount = 0;

	m_bActive = false;
}

void CEmoticonEffect::Destroy()
{
}

void CEmoticonEffect::Set(SI16 emoticonUnique )
{
	cltClient* pclclient = (cltClient*)pclClient;

	m_siCurrentDrawFrame = pclclient->clEmoticonInfo.GetStartFrame( emoticonUnique );
	m_siLastFrame = m_siCurrentDrawFrame + pclclient->clEmoticonInfo.GetAniStepNumber( emoticonUnique )-1; 
	m_siDelayCount = pclclient->clEmoticonInfo.GetLastAniDelay( emoticonUnique );

	m_bActive = true;
}


void CEmoticonEffect::Action()
{
}

void CEmoticonEffect::Draw( SI32 siCenterXPos, SI32 siCenterYPos)
{
	if ( m_bActive == false )
		return;

	if(TABS(pclClient->CurrentClock - m_EmoticonClock) > 200)
	{
		if( m_siCurrentDrawFrame > m_siLastFrame )
		{
			if ( m_siDelayCount <= 0 )
			{
				Initialize();
				return;
			}
			else
			{
				--m_siDelayCount;
			}
		}
		else
		{
			++m_siCurrentDrawFrame;
		}

		m_EmoticonClock = pclClient->CurrentClock;
	}


	SI16 drawFrame = min( m_siLastFrame, m_siCurrentDrawFrame );

	GP.PutSpr(m_pEmoticonSPR, siCenterXPos - 50, 
			siCenterYPos - 10,
			drawFrame , false );

	return;
}
