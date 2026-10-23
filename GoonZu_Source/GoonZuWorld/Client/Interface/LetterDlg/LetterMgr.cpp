#include "LetterMgr.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"

CLetterMgr::CLetterMgr() : CLibListMgr( true )
{
	m_siDeleteCount = 0;
	CreateEmptyObj();
}

CLetterMgr::~CLetterMgr()
{
}

void CLetterMgr::Initialize()
{
	return;
}

void CLetterMgr::Destroy()
{
	return;
}

void CLetterMgr::CreateEmptyObj()
{
	CreateMaxEmptyObj( MAX_LETTERMSG_NUM, MAX_LETTERMSG_SIZE );

	return;
}

void CLetterMgr::Clear()
{
	CLibListMgr::Clear();
	/*
	while ( true )
	{
		CObj *pTemp = PopFrontToExistListAndPushBackToEmptyList();

		if ( pTemp == NULL )
		{
			break;
		}
	}
	*/


	return;
}

bool CLetterMgr::Push( TCHAR *strLetterMsg, SI32 titletxtmgr )
{
	if ( strLetterMsg == NULL )
	{
		return false;
	}

	if ( _tcslen( strLetterMsg ) < 1 )
	{
		return false;
	}

	CObj *pTemp = this->PopFrontToEmptyListAndPushBackToExistList();

	stLetterInsert sLetterInsert;	
	StringCchCopy( sLetterInsert.strLetterMsg,	MAX_LETTERMSG_SIZE - 100, strLetterMsg );
	sLetterInsert.siTitleTxtMgr = titletxtmgr;

	pTemp->SetData( (BYTE*)&sLetterInsert, sLetterInsert.sHeader.uiMsgSize );
	return true;			// 쪽지 더 이상 넣을 수가 없는 경우 ( 최대 MAX_LETTERMSG_NUM 로 셑팅되어 있음 )
}

bool CLetterMgr::IsDuplicate( SI32 siLetterIndex )
{
	for ( SI32 i = 0; i < m_siDeleteCount; ++i )
	{
		if ( m_siDeleteSequenceIndex[ i ] == siLetterIndex )
		{
			return true;
		}
	}
	return false;
}

/*
bool CLetterMgr::IsExist( SI32 siLetterIndex )
{
	CObj *pTemp = GetExistList()->GetFirst();

	while( pTemp )
	{
		stLetterInsert *pLetterInsert = NULL;

		UI32 uiSize;

		pTemp->GetDataPtr( (BYTE*)pLetterInsert, &uiSize );

		if  ( pLetterInsert->siLetterIndex == siLetterIndex )
		{
			return true;
		}
		
		pTemp = pTemp->GetNext();
	}

	return false;
}
*/

/*
void CLetterMgr::PushDeleteIndex( SI32 siLetterIndex )
{
	if ( IsDuplicate( siLetterIndex ) )
	{
		return;
	}

	if ( ! IsExist( siLetterIndex ) )			// 없는  걸 삭제하라고 하믄 안되지...
	{
		return;
	}

	m_siDeleteSequenceIndex[ m_siDeleteCount ] = siLetterIndex;
	++m_siDeleteCount;

	CObj *pTemp = GetExistList()->GetFirst();
	
	stLetterInsert *pLetterInsert = NULL;
	
	while( pTemp )
	{
		UI32 uiSize;
		pTemp->GetDataPtr( (BYTE*)pLetterInsert, &uiSize );
		
		if ( pLetterInsert->siLetterIndex == pLetterInsert->siLetterIndex )
		{
			GetExistList()->Pop( pTemp );
			GetEmptyList()->PushBack( pTemp );
		}
	}
	
	
	return;
}

bool CLetterMgr::Pop( SI32 *pLetterIndex )
{
	if ( m_siDeleteCount > 0 )
	{
		*pLetterIndex = m_siDeleteSequenceIndex[ 0 ];
		--m_siDeleteCount;
		
		for ( SI32 i = 0; i < m_siDeleteCount; ++i )
		{
			m_siDeleteSequenceIndex[ i ] = m_siDeleteSequenceIndex[ i + 1 ];
		}

		return true;
	}

	return false;
}
*/