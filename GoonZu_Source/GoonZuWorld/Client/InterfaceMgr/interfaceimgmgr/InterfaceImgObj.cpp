#include "InterfaceImgObj.h"

#include <Graphic.h>
#include <Graphic_Capsule.h>

CInterfaceImgObj::CInterfaceImgObj()
{
	m_strFileName = NULL;
	m_pImgSpr = NULL;
}

CInterfaceImgObj::~CInterfaceImgObj()
{	
	Destroy();
}

void CInterfaceImgObj::Destroy()
{
	if ( m_pImgSpr )
	{
		if ( m_pImgSpr->pImage )
		{
			GP.FreeSpr( *m_pImgSpr );
		}

		SAFE_DELETE( m_pImgSpr );
	}

	SAFE_DELETE_A( m_strFileName );

	m_ImgLoadClock = 0;

	return;
}

TSpr* CInterfaceImgObj::LoadInterfaceImg( TCHAR *FileName, clock_t Clock )
{	
	Destroy();

	m_strFileName = new TCHAR[ MAX_PATH ];
	_tcsncpy( m_strFileName, FileName, MAX_PATH - 1 );
	m_strFileName[ MAX_PATH - 1 ] = NULL;

	m_pImgSpr = new TSpr();	
	if ( GP.LoadSprFromFile( FileName, m_pImgSpr ) == FALSE )
	{
		Destroy();
		return NULL;
	}
	
	m_ImgLoadClock = Clock;

	return m_pImgSpr;
}

bool CInterfaceImgObj::IsSameFile( TCHAR *FileName )
{
	if ( m_strFileName == NULL )
	{
		return false;
	}

	if ( _tcsncmp( m_strFileName, FileName, MAX_PATH -1 ) == 0 )
	{
		return true;
	}

	return false;
}

bool CInterfaceImgObj::Action( clock_t clock )
{
	if ( ( ( clock - m_ImgLoadClock ) / CLOCKS_PER_SEC ) > MAX_INTERFACEIMGOBJ_AVAILABLE_SECOND )
	{
		Destroy();

		return true;
	}
	
	return false;
}