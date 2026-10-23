#include "gametip.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"

CGameTip::CGameTip(void)
{
	ZeroMemory( this, sizeof(*this) );
	
	LoadGameTipFile();
}

CGameTip::~CGameTip(void)
{

}

void CGameTip::LoadGameTipFile(void)
{
	NDataLoader		dataloader;
	BOOL			bLoadFile = false;
	NTCHARString64	datafilename("TextMgr/GameTip.txt");

	if( pclClient->siServiceArea & ConstSwitch_TextEncryption )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( datafilename);
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile(datafilename);
	}

	if (!bLoadFile)
	{
		MsgBox(TEXT("LoadGameTipFile()"), TEXT("LoadError:[%s]"), (TCHAR*)datafilename);
		return;
	}	

	SI16 Count		= 0;
	SI32 TextNum	= 0;
	SI32 Use		= 0;
	NDataTypeInfo pDataTypeInfo[] = 
	{
		NDATA_INT32,	&TextNum,		sizeof(TextNum),
		NDATA_INT32,	&Use,			sizeof(Use),
		0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo);

	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData())
		{
			if(Use)
			{
				m_siTipText[Count] = TextNum;
				++Count;
			}
		}
	}

	m_siTipTextNum = Count;
}


TCHAR* CGameTip::RandTipText(void)
{
	cltClient * pclclient = (cltClient*) pclClient;
	if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bGameTip == true )
		return NULL;
	SI32 siIndex = rand() % m_siTipTextNum;

	return m_siTipText[siIndex] ? GetTxtFromMgr( m_siTipText[siIndex] ) : NULL; 
}