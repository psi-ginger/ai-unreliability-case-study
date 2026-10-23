#include "CCDSurfaceMgr.h"
#include "CCDSurfaceObj.h"
#include "..\..\SMLib\LibList\LibList.h"

#include "CommonLogic.h"
extern cltCommonLogic* pclClient;

CCCDSurfaceMgr::CCCDSurfaceMgr() : CLibListMgr()
{	
	m_siCurrentIndex = -1;
	CreateEmptyObj( MIN_SURFACE_NUM );

	Initialize();
}

CCCDSurfaceMgr::~CCCDSurfaceMgr()
{
	Destroy();
}

void CCCDSurfaceMgr::Initialize()
{
	return;
}

void CCCDSurfaceMgr::Destroy()
{
	return;
}

void CCCDSurfaceMgr::CreateEmptyObj( SI32 siMinEmptyObj )
{
	CCCDSurfaceObj *pTemp = NULL;

	for ( int i = 0; i < siMinEmptyObj; ++i )
	{
		pTemp = new CCCDSurfaceObj();		
		PushBackToEmptyList( pTemp );
	}

	return;
}

SI32 CCCDSurfaceMgr::GetSurfaceIndex()
{	
	CCCDSurfaceObj *pTemp = (CCCDSurfaceObj *)PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp == NULL )
	{
		pTemp = new CCCDSurfaceObj();
		pTemp->Set( ++m_siCurrentIndex );
		PushBackToExistList( pTemp );
	}
	else
	{
		pTemp->Set( ++m_siCurrentIndex );
	}

	return m_siCurrentIndex;
}

LPDIRECTDRAWSURFACE7 CCCDSurfaceMgr::GetSurface( SI32 siIndex )
{
	CCCDSurfaceObj *pTemp = (CCCDSurfaceObj *)GetExistList()->GetFirst();
	CCCDSurfaceObj *pTemp1 = NULL;

	UI32 uiCurrentFrame = pclClient->GetFrame();
	UI32 uiFrame;

	while ( pTemp )
	{
		if ( pTemp->IsExist( siIndex, &uiFrame ) )
		{
			return pTemp->GetSurface( uiCurrentFrame );
		}
		else
		{
			pTemp1 = pTemp;
			pTemp = (CCCDSurfaceObj *)pTemp->GetNext();

			if ( uiCurrentFrame - uiFrame > SURFACE_OBJ_SURVIVAL_FRAME )
			{
				if ( GetExistList()->GetObjNum() > MIN_SURFACE_NUM )
				{
					GetExistList()->Pop( pTemp1 );
					delete pTemp1;
					pTemp1 = NULL;
				}
				else
				{
					GetExistList()->Pop( pTemp1 );
					PushBackToEmptyList( pTemp1 );
				}
			}
		}
	}

	return NULL;
}