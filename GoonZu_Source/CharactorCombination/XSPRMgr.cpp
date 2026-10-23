#include "stdafx.h"
#include "XSPRMgr.h"
#include "XSPRObj.h"

CXSPRMgr::CXSPRMgr()
{
	m_usImageNum = 0;
	Initialize();
}

CXSPRMgr::~CXSPRMgr()
{
	Destroy();
}

void CXSPRMgr::Initialize()
{
	m_usImageNum = 0;

	return;
}

void CXSPRMgr::Destroy()
{
	CXSPRObj *pTemp = GetFirst();
	CXSPRObj *pTemp1 = NULL;

	while( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();

		delete pTemp1;
		pTemp1 = NULL;
	}

	m_usImageNum = 0;

	return;
}

CXSPRObj *CXSPRMgr::GetFirst()
{
	return m_pFirst;
}

void CXSPRMgr::SetFirst( CXSPRObj *pNode )
{
	m_pFirst = pNode;
}

CXSPRObj *CXSPRMgr::GetLast()
{
	return m_pLast;
}

void CXSPRMgr::SetLast( CXSPRObj *pNode )
{
	m_pLast = pNode;
	return;
}

char CXSPRMgr::LoadXSPR( char *strFileName )
{
	CXSPRObj* pTemp = IsSameFileExist( strFileName );

	if ( pTemp == NULL )
	{
		CXSPRObj *pTemp = new CXSPRObj();

		if ( ! pTemp->LoadXSPR( strFileName ) )
		{
			return LOAD_FAIL;
		}

		if ( m_usImageNum > MAX_LOADED_FILE_NUM )
		{
			PopFront();
		}

		PushBack( pTemp );

		return SUCCESS;
	}

	MoveToBack( pTemp );

	return FILE_EXIST;
}

void CXSPRMgr::PushBack( CXSPRObj *pNode )
{
	if ( m_pFirst )
	{
		m_pLast->SetNext( pNode );
		pNode->SetPre( m_pFirst );
		m_pLast = pNode;
	}
	else
	{
		m_pFirst = pNode;
		m_pLast = pNode;
	}

	++m_usImageNum;

	return;
}

void CXSPRMgr::PopFront()
{
	CXSPRObj *pTemp = m_pFirst->GetNext();

	if ( pTemp == NULL )
	{
		delete m_pFirst;
		m_pFirst = NULL;
	}
	else
	{
		delete m_pFirst;
		m_pFirst = pTemp;
	}

	--m_usImageNum;
}

void CXSPRMgr::MoveToBack( CXSPRObj *pNode )
{
	if ( pNode == m_pLast )
	{
		return;
	}
	else if ( pNode == m_pFirst )
	{
		m_pFirst = m_pFirst->GetNext();

		m_pLast->SetNext( pNode );
		pNode->SetPre( m_pLast );
		pNode->SetNext( NULL );
		m_pLast = pNode;
		
		return;
	}
	else
	{
		m_pLast->SetNext( pNode );
		pNode->SetPre( m_pLast );
		pNode->SetNext( NULL );
		m_pLast = pNode;
	}

	return;
}

CXSPRObj* CXSPRMgr::IsSameFileExist( char *strFileName )
{
	CXSPRObj *pTemp = GetFirst();

	while( pTemp )
	{
		if ( pTemp->IsSameFile( strFileName ) )
		{
			return pTemp;
		}

		pTemp = pTemp->GetNext();
	}

	return NULL;
}

char CXSPRMgr::PutSprT( char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	if ( strFileName == NULL || strFileName == "")
	{
		MessageBox( NULL, "strFileName == NULL", "Error", MB_OK );
		return 0;
	}

	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = new CXSPRObj();

		if ( ! pTempExistObj->LoadXSPR( strFileName ) )
		{
			return LOAD_FAIL;
		}

		if ( m_usImageNum > MAX_LOADED_FILE_NUM )
		{
			PopFront();
		}

		PushBack( pTempExistObj );
	}
	else
	{
		MoveToBack( pTempExistObj );
	}

	pTempExistObj->PutSprT( siPosX, siPosY, siWidth, siHeight, siStartPos );
	
	return SUCCESS;

}

char CXSPRMgr::PutSprRT( char *strFileName, int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	if ( pTempExistObj == NULL )
	{
		pTempExistObj = new CXSPRObj();

		if ( ! pTempExistObj->LoadXSPR( strFileName ) )
		{
			return LOAD_FAIL;
		}

		if ( m_usImageNum > MAX_LOADED_FILE_NUM )
		{
			PopFront();
		}

		PushBack( pTempExistObj );
	}
	else
	{
		MoveToBack( pTempExistObj );
	}

	pTempExistObj->PutSprRT( siPosX, siPosY, siWidth, siHeight, siStartPos );
	
	return SUCCESS;

}

short CXSPRMgr::GetDirection( char *strFileName, char cDirection, short *pssAnimationNum )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	return pTempExistObj->GetDirectionStartIndexAndDirectionAnimationNum( cDirection, pssAnimationNum );
}

short CXSPRMgr::GetTotalTileNum( char *strFileName )
{
	CXSPRObj *pTempExistObj = IsSameFileExist( strFileName );

	return pTempExistObj->GetTotalTileNum();
}

