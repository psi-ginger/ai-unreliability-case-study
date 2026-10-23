

#ifdef __SUN_NLETTER__


#include "NLetterMgr.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"

CNLetterMgr::CNLetterMgr() : CLibListMgr( true )
{
	m_siDeleteCount = 0;
	CreateEmptyObj();
}

CNLetterMgr::~CNLetterMgr()
{
}

void CNLetterMgr::Initialize()
{
	return;
}

void CNLetterMgr::Destroy()
{
	return;
}

void CNLetterMgr::CreateEmptyObj()
{
	CreateMaxEmptyObj( MAX_LETTERMSG_NUM, MAX_LETTERMSG_SIZE );

	return;
}

void CNLetterMgr::Clear()
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

bool CNLetterMgr::Push( char *strNLetterMsg )
{
	if ( strNLetterMsg == NULL )
	{
		return false;
	}

	if ( strlen( strNLetterMsg ) < 1 )
	{
		return false;
	}

	CObj *pTemp = this->PopFrontToEmptyListAndPushBackToExistList();

	stNLetterInsert sNLetterInsert;	
	strcpy( sNLetterInsert.strNLetterMsg, strNLetterMsg );

	pTemp->SetData( (char*)&sNLetterInsert, sNLetterInsert.sHeader.uiMsgSize );
	return true;			// 쪽지 더 이상 넣을 수가 없는 경우 ( 최대 MAX_LETTERMSG_NUM 로 셑팅되어 있음 )
}

bool CNLetterMgr::IsDuplicate( SI32 siLetterIndex )
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
bool CNLetterMgr::IsExist( SI32 siLetterIndex )
{
	CObj *pTemp = GetExistList()->GetFirst();

	while( pTemp )
	{
		stNLetterInsert *pLetterInsert = NULL;

		UI32 uiSize;

		pTemp->GetDataPtr( (char*)pLetterInsert, &uiSize );

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
void CNLetterMgr::PushDeleteIndex( SI32 siLetterIndex )
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
	
	stNLetterInsert *pLetterInsert = NULL;
	
	while( pTemp )
	{
		UI32 uiSize;
		pTemp->GetDataPtr( (char*)pLetterInsert, &uiSize );
		
		if ( pLetterInsert->siLetterIndex == pLetterInsert->siLetterIndex )
		{
			GetExistList()->Pop( pTemp );
			GetEmptyList()->PushBack( pTemp );
		}
	}
	
	
	return;
}

bool CNLetterMgr::Pop( SI32 *pLetterIndex )
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
#endif