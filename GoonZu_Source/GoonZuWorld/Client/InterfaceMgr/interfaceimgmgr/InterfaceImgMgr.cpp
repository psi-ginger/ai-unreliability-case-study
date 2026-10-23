#include "InterfaceImgMgr.h"
#include "InterfaceImgObj.h"

#include "../ControllerBaseDefinition.h"
#include "../../Common/SMLib/LibList/LibList.h"



CInterfaceImgMgr::CInterfaceImgMgr() : CLibListMgr()
{
	CreateMaxEmptyObj();
}

CInterfaceImgMgr::~CInterfaceImgMgr()
{

}

void CInterfaceImgMgr::CreateMaxEmptyObj()
{
	CInterfaceImgObj *pTemp = NULL;

	for( SI16 i = 0; i < MAX_INTERFACEIMGOBJ_NUM; ++i )
	{
		pTemp = new CInterfaceImgObj();
		PushBackToEmptyList( pTemp );
	}

	return;
}

TSpr* CInterfaceImgMgr::LoadInterfaceImg( TCHAR *FileName )
{
	if ( FileName == NULL || _tcslen( FileName ) < 1 )
	{
		return NULL;
	}

	CInterfaceImgObj *pTemp = (CInterfaceImgObj *)GetExistList()->GetFirst();

	clock_t Clock = clock();

	while( pTemp )
	{
		if ( pTemp->IsSameFile( FileName ) )
		{
			return pTemp->GetImgSpr( Clock );
		}

		pTemp = (CInterfaceImgObj *)pTemp->GetNext();
	}

	pTemp = (CInterfaceImgObj *)PopFrontToEmptyListAndPushBackToExistList();
	if ( pTemp )
	{	
		TSpr* RetVal = pTemp->LoadInterfaceImg( FileName, Clock );
		
		if ( RetVal == NULL )
		{
			GetExistList()->Pop( pTemp );
			GetEmptyList()->PushBack( pTemp );
			pTemp->Initialize();
		}

		return RetVal;
	}
	else
	{
		return NULL;
	}

//	return NULL;
}

void CInterfaceImgMgr::Action()
{
	int ImgCounter = 0;
	clock_t Clock = clock();

	CInterfaceImgObj *pTemp = (CInterfaceImgObj *)GetExistList()->GetFirst();
	CInterfaceImgObj *pTemp1 = NULL;

	while( pTemp )
	{
		++ImgCounter;

		if ( pTemp->Action( Clock ) )
		{	
			pTemp1 = (CInterfaceImgObj* )pTemp->GetNext();

			GetExistList()->Pop( pTemp );
			GetEmptyList()->PushBack( pTemp );
			pTemp->Initialize();

			pTemp = pTemp1;
		}
		else
		{
			pTemp = (CInterfaceImgObj *)pTemp->GetNext();
		}
	}

	/*
	char TempStr[ MAX_PATH ];
	sprintf( TempStr, TEXT("%d\r\n"), ImgCounter );
	OutputDebugString( TempStr );
	*/

	return;
}