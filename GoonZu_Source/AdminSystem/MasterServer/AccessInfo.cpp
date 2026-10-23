#include ".\accessinfo.h"
#include "../../Server/FrontServer/Log.h"

CAccessInfo::CAccessInfo(void)
{
	ZeroMemory(this,sizeof(CAccessInfo));
}

CAccessInfo::~CAccessInfo(void)
{
}

BOOL	CAccessInfo::ReadFromFile()
{
	m_siMCIPCount = 0;

	DWORD dwBytesRead;
	
	ifstream in("accessip.txt");//파일을 읽기 모드로
	
	const SI32 ConstBufferSize = 64;
	TCHAR	szBuffer[ ConstBufferSize ];
	SI32	siSection = 0;

	while( TRUE ) {

		if(in.getline(szBuffer,ConstBufferSize) == NULL)
			break;
	
		if(!_tcscmp(szBuffer,TEXT("[MoniterClient]")))
		{
			siSection = ACCESS_IP_MC;
		}
		else
		{
			switch(siSection)
			{
			case ACCESS_IP_MC:
				{
					SetMCIP(szBuffer);
					break;
				}
				
			}
		}
	}

	in.close();

	//CloseHandle( hFile );

	return TRUE;
}
BOOL	CAccessInfo::SetMCIP(TCHAR* pszIP)
{
	if(pszIP == NULL)
		return FALSE;

	StringCchCopy(m_szMCIPList[m_siMCIPCount] ,ConstIPSize, pszIP);
	m_siMCIPCount++;
	return TRUE;
}

BOOL	CAccessInfo::ConfirmAccessMC(TCHAR* pszIP)
{
	if(pszIP == NULL)
		return FALSE;

	for(SI32 i = 0 ; i < m_siMCIPCount ;i++)
	{
		if( !_tcscmp(m_szMCIPList[i], pszIP))
			return TRUE;
	}

	return FALSE;
}
