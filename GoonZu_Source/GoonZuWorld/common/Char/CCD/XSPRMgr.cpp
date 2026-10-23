#include "XSPRMgr.h"
#include "XSPRObj.h"
#include "..\..\SMLib\LibList\LibList.h"

#include "..\..\Client\Client.h"
extern cltCommonLogic* pclClient;

#define	MAX_LOADED_FILE_NUM		1000		// 중복 이미지 생각해도 한 화면에 유저가 300명이면 800 개 정도 잡아야 됨
											// 우선은, 게임 초반이라 사람이 적으므로...

/*
CXSPRMgr	g_XSPRMgr;

CXSPRMgr::CXSPRMgr() : CLibListMgr()
{
	CreateEmptyObj( MAX_LOADED_FILE_NUM );
	Initialize();
}

CXSPRMgr::~CXSPRMgr()
{
	Destroy();
}

void CXSPRMgr::Initialize()
{
	return;
}

void CXSPRMgr::Destroy()
{
	return;
}

void CXSPRMgr::CreateEmptyObj( SI32 siMaxEmptyObj )
{
	CXSPRObj *pTemp = NULL;

	for ( int i = 0; i < siMaxEmptyObj; ++i )
	{
		pTemp = new CXSPRObj();		
		PushBackToEmptyList( pTemp );
	}

	return;
}

char CXSPRMgr::LoadXSPR( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex )
{

	CXSPRObj* pTemp = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

	if ( pTemp == NULL )
	{
		pTemp = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTemp )
		{
			if ( ! pTemp->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return LOAD_FAIL;
			}
		}
		else
		{
			return LOAD_FAIL;
		}

		return SUCCESS;
	}

	return FILE_EXIST;
}

void CXSPRMgr::LoadSimilarXSPR( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex )
{
	if ( ssActionIndex > SPCU )			// 말 탄 경우
	{
		return;
		
		//if ( strFileName[14]== 'H' && strFileName[15]== 'O' )
		//{
			//strncpy( &strFileName[ 14 ], TEXT("HOR01"), 5 );
		//}
		//else if ( strFileName[14]== 'M' && strFileName[15]== 'T' )
		//{
			//strncpy( &strFileName[ 14 ], TEXT("MTL01"), 5 );
		//}
		//else if ( strFileName[14]== 'E' && strFileName[15]== 'Q' )
		//{
			//strncpy( &strFileName[ 14 ], TEXT("EQU01"), 5 );
		//}
		//else
		//{
			// 한손 공격( 1HSR ) 은 맨손으로 가능함으로 무기 코드 없어도 됨.
		//	if ( ssActionIndex > HSR1 && ssActionIndex < HITR )
		//	{
				// 무기 코드가 있어야 됨. 맨손에 공격 동작은 없음.
		//	}
		//	else
		//	{
				//strncpy( &strFileName[ 14 ], TEXT("KOR01"), 5 );
		//	}
		//}
	}
	else								// 말 타지 않은 경우
	{		
		if ( ssActionIndex < HITU )
		{
			// 1HSU 와 2HSU 는 1HSU 동작을 같이 쓴다.
			strncpy( &strFileName[ 25 ], TEXT("1HSU"), 4 );
		}

		return;
		
		//if ( strFileName[14]== 'M' && strFileName[15]== 'T' )
		//{
			//strncpy( &strFileName[ 14 ], TEXT("MTL01"), 5 );
		//}
		//else if ( strFileName[14]== 'E' && strFileName[15]== 'Q' )
		//{
			//strncpy( &strFileName[ 14 ], TEXT("EQU01"), 5 );
		//}
		//else
		//{
			// 한손 공격( 1HSU ) 은 맨손으로 가능함으로 무기 코드 없어도 됨.
		//	if ( ssActionIndex > HSU1 && ssActionIndex < HITU )
		//	{				
		//	}
		//	else
		//	{
				//strncpy( &strFileName[ 14 ], TEXT("KOR01"), 5 );
		//	}
		//}
	}

	return;
}


CXSPRObj* CXSPRMgr::IsSameFileExist( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex )
{

	CXSPRObj *pTemp = (CXSPRObj *)GetExistList()->GetFirst();

	while( pTemp )
	{
		if ( pTemp->IsSameFile( strFileName, CharacterKind, cPart, ssActionIndex ) )
		{
			return pTemp;
		}

		pTemp = (CXSPRObj *)pTemp->GetNext();
	}

	return NULL;
}

cltTSpr* CXSPRMgr::PutSprT( SI16 CharacterKind, char cPart, short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}
		}
		else
		{
			LoadSimilarXSPR( strFileName, CharacterKind, cPart, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );
			
			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );
				
				if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}
			}			
		}
	}
	
	return pTempExistObj->PutSprT( siPosX, siPosY, siWidth, siHeight, siStartPos );
	

	// 현재 사용중인 파일들의 메모리를 확인하기 위해서...
	//UI32 temp1;
	//GetLoadedImageDataSize( &temp1 );
	

	//return SUCCESS;
}

cltTSpr* CXSPRMgr::PutSprRT( SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}
		}
		else
		{
			LoadSimilarXSPR( strFileName, CharacterKind, cPart, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );
			
			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );
				
				if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}
			}
		}
	}

	return pTempExistObj->PutSprRT( siPosX, siPosY, siWidth, siHeight, siStartPos );	
}

cltTSpr* CXSPRMgr::PutSprScreenT( SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}
		}
		else
		{
			LoadSimilarXSPR( strFileName, CharacterKind, cPart, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );

				if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}
			}			
		}
	}

	return pTempExistObj->PutSprScreenT( siPosX, siPosY, siWidth, siHeight, siStartPos );


	// 현재 사용중인 파일들의 메모리를 확인하기 위해서...
	//UI32 temp1;
	//GetLoadedImageDataSize( &temp1 );


	//return SUCCESS;
}

cltTSpr* CXSPRMgr::PutSprScreenRT( SI16 CharacterKind, char cPart, short ssActionIndex, char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}
		}
		else
		{
			LoadSimilarXSPR( strFileName, CharacterKind, cPart, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName, CharacterKind, cPart, ssActionIndex );

			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );

				if ( ! pTempExistObj->LoadXSPR( strFileName, CharacterKind, cPart, ssActionIndex ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}
			}
		}
	}

	return pTempExistObj->PutSprScreenRT( siPosX, siPosY, siWidth, siHeight, siStartPos );	
}

int CXSPRMgr::GetLoadedImageDataSize( unsigned int *pFileSize )
{
	int siCount = 0;
	*pFileSize = 0;

	CXSPRObj *pTemp = (CXSPRObj *)GetExistList()->GetFirst();

	while( pTemp )
	{
		*pFileSize += pTemp->GetImageFileSize();
		pTemp = (CXSPRObj *)pTemp->GetNext();
		++siCount;
	}

	return siCount;
}

int CXSPRMgr::GetLoadedImageNum()
{
	return GetExistList()->GetObjNum();
}

void CXSPRMgr::Action()
{
	UI32 CurrentFrame = pclClient->GetFrame();

	CXSPRObj *pTemp = (CXSPRObj *)GetExistList()->GetFirst();
	CXSPRObj *pTemp1 = NULL;

	while( pTemp )
	{	
		if ( pTemp->Action( CurrentFrame ) )
		{
			pTemp1 = (CXSPRObj *)pTemp->GetNext();

			GetExistList()->Pop( pTemp );
			GetEmptyList()->PushBack( pTemp );
			pTemp->Initialize();
			
			pTemp = pTemp1;
		}
		else
		{
			pTemp = (CXSPRObj *)pTemp->GetNext();
		}
	}

	return;
}
*/


CXSPRMgr	g_XSPRMgr;

typedef pair< tstring, CXSPRObj* > XSPRPair;


CXSPRMgr::CXSPRMgr()
{	
	CreateEmptyObj( MAX_LOADED_FILE_NUM );
	Initialize();
}

CXSPRMgr::~CXSPRMgr()
{	
	m_XSPRMap.clear();
	Destroy();
}

void CXSPRMgr::Initialize()
{
	return;
}

void CXSPRMgr::Destroy()
{
	return;
}

void CXSPRMgr::CreateEmptyObj( SI32 siMaxEmptyObj )
{
	CXSPRObj *pTemp = NULL;

	for ( int i = 0; i < siMaxEmptyObj; ++i )
	{
		pTemp = new CXSPRObj();		
		PushBackToEmptyList( pTemp );
	}

	return;
}

char CXSPRMgr::LoadXSPR( TCHAR *strFileName, SI16 CharacterKind, char cPart, short ssActionIndex )
{
	CXSPRObj* pTemp = IsSameFileExist( strFileName );

	if ( pTemp == NULL )
	{
		pTemp = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTemp )
		{
			if ( ! pTemp->LoadXSPR( strFileName ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return LOAD_FAIL;
			}
		}
		else
		{
			return LOAD_FAIL;
		}

		m_XSPRMap.insert( XSPRPair( strFileName, pTemp ) );

		return SUCCESS;
	}

	return FILE_EXIST;
}

void CXSPRMgr::LoadSimilarXSPR( TCHAR *strFileName, short ssActionIndex )
{
	if ( ssActionIndex > SPCU )			// 말 탄 경우
	{
		return;
	}
	else								// 말 타지 않은 경우
	{		
		if ( ssActionIndex < HITU )
		{
			// 1HSU 와 2HSU 는 1HSU 동작을 같이 쓴다.
			_tcsncpy( &strFileName[ 25 ], TEXT("1HSU"), 4 );
		}

		return;
	}

	return;
}


CXSPRObj* CXSPRMgr::IsSameFileExist( const TCHAR *strFileName )
{
	if  ( strFileName == NULL || _tcslen( strFileName ) < 1 )
	{
		return NULL;
	}

	map< tstring, CXSPRObj * >::iterator itr;
	itr = m_XSPRMap.find( strFileName );

	if ( itr != m_XSPRMap.end() )
	{
		CXSPRObj *pXSPRObj = itr->second;
		return pXSPRObj;
	}

	return NULL;
}


TSpr* CXSPRMgr::PutSprT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}

			m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
		}
		else
		{
			LoadSimilarXSPR( strFileName, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName );

			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );

				if ( ! pTempExistObj->LoadXSPR( strFileName ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}

				m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
			}			
		}		
	}

	return pTempExistObj->PutSprT( siPosX, siPosY, siWidth, siHeight, siStartPos );
}


TSpr* CXSPRMgr::PutSprRT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}

			m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
		}
		else
		{
			LoadSimilarXSPR( strFileName, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName );

			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );

				if ( ! pTempExistObj->LoadXSPR( strFileName ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}

				m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
			}
		}
	}

	return pTempExistObj->PutSprRT( siPosX, siPosY, siWidth, siHeight, siStartPos );	
}

TSpr* CXSPRMgr::PutSprScreenT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}

			m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
		}
		else
		{
			LoadSimilarXSPR( strFileName, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName );

			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );

				if ( ! pTempExistObj->LoadXSPR( strFileName ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}

				m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
			}			
		}
	}

	return pTempExistObj->PutSprScreenT( siPosX, siPosY, siWidth, siHeight, siStartPos );
}

TSpr* CXSPRMgr::PutSprScreenRT( short ssActionIndex, TCHAR *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = ( CXSPRObj * )PopFrontToEmptyListAndPushFrontToExistList();

		if ( pTempExistObj )
		{
			if ( ! pTempExistObj->LoadXSPR( strFileName ) )
			{
				PopFrontToExistListAndPushBackToEmptyList();
				return NULL;
			}

			m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
		}
		else
		{
			LoadSimilarXSPR( strFileName, ssActionIndex );

			pTempExistObj = IsSameFileExist( strFileName );

			if ( pTempExistObj == NULL )
			{
				pTempExistObj = (CXSPRObj *)GetExistList()->PopBack();
				GetExistList()->PushFront( pTempExistObj );

				if ( ! pTempExistObj->LoadXSPR( strFileName ) )
				{
					PopFrontToExistListAndPushBackToEmptyList();
					return NULL;
				}

				m_XSPRMap.insert( XSPRPair( strFileName, pTempExistObj ) );
			}
		}
	}

	return pTempExistObj->PutSprScreenRT( siPosX, siPosY, siWidth, siHeight, siStartPos );	
}

int CXSPRMgr::GetLoadedImageDataSize( unsigned int *pFileSize )
{
	int siCount = 0;
	*pFileSize = 0;

	CXSPRObj *pTemp = (CXSPRObj *)GetExistList()->GetFirst();

	while( pTemp )
	{
		*pFileSize += pTemp->GetImageFileSize();
		pTemp = (CXSPRObj *)pTemp->GetNext();
		++siCount;
	}

	return siCount;
}

int CXSPRMgr::GetLoadedImageNum()
{
	return GetExistList()->GetObjNum();
}

void CXSPRMgr::Action()
{
	UI32 CurrentFrame = pclClient->GetFrame();
	
	CXSPRObj *pTemp = (CXSPRObj *)GetExistList()->GetFirst();
	CXSPRObj *pTemp1 = NULL;

	map< tstring, CXSPRObj *>::iterator itr;

	while( pTemp )
	{	
		if ( pTemp->Action( CurrentFrame ) )
		{
			pTemp1 = (CXSPRObj *)pTemp->GetNext();
			
			itr = m_XSPRMap.find( pTemp->GetFileName() );
			if ( itr != m_XSPRMap.end() )
			{
				GetExistList()->Pop( pTemp );
				GetEmptyList()->PushBack( pTemp );
				pTemp->Initialize();
				
				m_XSPRMap.erase( itr );
			}
#ifdef _DEBUG
			else
			{
				MsgBox( pTemp->GetFileName(), TEXT("CCD No File Error") );
			}
#endif

			pTemp = pTemp1;
		}
		else
		{
			pTemp = (CXSPRObj *)pTemp->GetNext();
		}
	}

	return;
}