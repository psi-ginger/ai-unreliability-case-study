#include "ITZMap.h"
#include "ITZWorld.h"
#include "ITZCell.h"
#include "ITZPerson.h"

ITZMap::ITZMap()
{

	m_pWorld = NULL;

	m_pCell = NULL;
	
	m_pItem = NULL;

	m_uiCellCount = 0;

	m_uiRemoveCellCount = 0;
	m_uiNewCellCount = 0;
}

ITZMap::~ITZMap()
{
	delete [] m_pCell;
	delete [] m_pItem;
	
	m_pCell = NULL;
	m_pItem = NULL;
}

void ITZMap::SetWorld( ITZWorld< ITZMap >* pWorld )
{

	m_pWorld = pWorld;
}

void ITZMap::CreateMap( int mapXtile, int mapYtile )
{

	if( mapXtile > mapYtile ) mapYtile = mapXtile;
	if( mapXtile < mapYtile ) mapXtile = mapYtile;

	m_mapXtile = mapXtile;
	m_mapYtile = mapYtile;

	// 맵 가로/세로 섹터 개수 얻기
	m_mapXcell = ( mapXtile + m_pWorld->m_cellXtile - 1 ) / m_pWorld->m_cellXtile;
	m_mapYcell = ( mapYtile + m_pWorld->m_cellYtile - 1 ) / m_pWorld->m_cellYtile;

	// 셀 생성
	m_uiCellCount = m_mapXcell * m_mapYcell;

	if( m_pCell ) delete [] m_pCell;    

	m_pCell = new ITZCell[ m_uiCellCount ];

	int j, k, x, y, t;

	for( UI32 i = 0; i < m_uiCellCount; ++i ) {

		m_pCell[ i ].CreateCell( m_pWorld, this ); 
		
		// 셀의 아이템 슬롯 초기화 
		m_pCell[ i ].m_ppItem = new sItem*[ ( m_pWorld->m_cellXtile + 1 ) * ( m_pWorld->m_cellYtile + 1 ) * 2 ];
		m_pCell[ i ].m_uiItemSlotCount = 0;

		x = i % m_mapXcell;
		y = i / m_mapXcell;

		m_pCell[ i ].m_uiNearCellCount = 0;

		for( j = -1; j < 2; ++j ) {

			for( k = -1; k < 2; ++k ) {

				if( ( x + k ) >= 0 && ( x + k ) < m_mapXcell &&
					( y + j ) >= 0 && ( y + j ) < m_mapYcell ) {

					t = ( y + j ) * m_mapXcell +  x + k;

					m_pCell[ i ].m_pNearCell[  m_pCell[ i ].m_uiNearCellCount  ] = &m_pCell[ t ];

					m_pCell[ i ].m_uiNearCellCount++;
				}

			}

		}

	}

	if( m_pItem ) delete [] m_pItem; 

	// 아이템 슬롯 생성 및 초기화
	m_pItem = new sItem[ m_mapXtile * m_mapYtile ];
	
	ITZCell *pCell;
	sItem item;

	for( j = 0; j < m_mapYtile; ++j ) {
		for( k = 0; k < m_mapXtile; ++k ) {

			item.x = k;
			item.y = j;
			item.itemIndex = 0;

			m_pItem[ j * m_mapXtile + k ] = item;

			pCell = GetCell( k, j );
			
			pCell->m_ppItem[ pCell->m_uiItemSlotCount++ ] = &m_pItem[ j * m_mapXtile + k ];
		}
	}

}

int ITZMap::GetCellIndex( int x, int y )
{
	int sx = ( m_mapXtile - y - 1 + x ) / ( m_pWorld->m_cellXtile << 1 );
	int sy = ( x + y ) / ( m_pWorld->m_cellYtile << 1 );

	return m_mapXcell * sy + sx;
}

// 멥에서 x, y 타일 좌표에 해당하는 셀을 얻는다
ITZCell* ITZMap::GetCell( int x, int y )
{
	int sx = ( m_mapXtile - y - 1 + x ) / ( m_pWorld->m_cellXtile << 1 );
	int sy = ( x + y ) / ( m_pWorld->m_cellYtile << 1 );
	int index = m_mapXcell * sy + sx;

	if( index < 0 || index >= m_mapXcell * m_mapYcell ) return NULL;

	return &m_pCell[ index ];
}

void ITZMap::Warp( int x, int y, ITZPerson *pPerson, bool bticketusewarpswitch )
{
	
	ITZCell *pCurrentCell = pPerson->GetCell(); 

	if( pCurrentCell )
	{
		// 삭제 메시지를 보낼 셀들을 얻는다 
		SetCellList( pCurrentCell, NULL );

		// 삭제 메시지를 보낸다
		SendPersonLeaveMsgToOthers( pPerson);

		// 셀에서 캐릭터를 삭제한다 
		pCurrentCell->DelPerson( pPerson );
	}

	MoveChar( x, y, pPerson, bticketusewarpswitch  );
}

void ITZMap::MoveChar( int x, int y, ITZPerson *pPerson, bool bticketusewarpswitch  )
{

	// 이동 할 지점의 x, y 좌표에 해당하는 셀을 구한다 
	ITZCell *pOldCell = pPerson->GetCell();
	ITZCell *pCurrentCell = GetCell( x, y );

	if( pCurrentCell == NULL ) return;

	if( pOldCell == NULL ) {
		
		// 기존에 소속된 셀이 없는 경우 - 이동이 아니라 갑자기 나타나는 경우..(로그인했을경우등)
		// 새로 추가된 셀 리스트를 얻는다( 처음 셀에 등록 되므로 이전 셀의 인자는 NULL이다 )
		SetCellList( NULL, pCurrentCell );

		// 새로 추가된 셀에 있는 캐릭터와 아이템 정보를 얻는다 
		SendNewOthersInfoToPerson( pPerson );
		SendNewItemsInfoToPerson( pPerson );

		// 자신의 정보를 다른 캐릭터들에게 알린다 
		SendPersonEnterMsgToOthers( pPerson, bticketusewarpswitch );

		// 현재 셀에 Person을 설정한다 
		pCurrentCell->SetPerson( pPerson );


	} else {

		if( pOldCell != pCurrentCell ) {
			// 셀이 바뀐 경우

			// 이전에 캐릭터가 위치하고 있던 셀과 현재 셀을 비교하여
			// 삭제된 셀, 추가된 셀을 구한다 ( 결과 값은 멤버 변수에 저장.. 다음 호출 시까지 유효)
			SetCellList( pOldCell, pCurrentCell );

			// 기존 셀에서 Person을 삭제한다
			pOldCell->DelPerson( pPerson );

			// 삭제된 셀에 있는 캐릭터들에게 캐릭터의 삭제(퇴장)를 알린다
			SendPersonLeaveMsgToOthers( pPerson );

			// 새로 추가된 셀에 있는 캐릭터와 아이템 정보를 얻는다 
			SendNewOthersInfoToPerson( pPerson );
			SendNewItemsInfoToPerson( pPerson );
			
			// 새로 추가된 셀에 있는 캐릭터들에게 캐릭터의 추가(등장)를 알린다 
			SendPersonEnterMsgToOthers( pPerson, bticketusewarpswitch );

			// 현재 셀에 Person을 설정한다 
			pCurrentCell->SetPerson( pPerson );
		}

		// 캐릭터 이동 정보를 가시영역에 있는 캐릭터들에게 보낸다 
		SendPersonMoveMsgToOthers( pPerson );
	}	

}

void ITZMap::SetCellList( ITZCell *pOldCell, ITZCell *pCurrentCell )
{	

	UI32 i, j;
	
	m_uiRemoveCellCount = 0;
	m_uiNewCellCount = 0;
	
	if( pOldCell == NULL ) {
		// pOldCell이 NULL이면 새로운 셀 리스트에 pCurrentCell의 주변 셀들을 모두 넣는다
		
		for( i = 0; i < pCurrentCell->m_uiNearCellCount; ++i ) {
			
			m_pNewCell[ m_uiNewCellCount++ ] = pCurrentCell->m_pNearCell[ i ];
		}
		
	} else if( pCurrentCell == NULL ) {
		// pNewCell이 NULL이면 삭제할 셀 리스트에 pOldCell의 주변 셀들을 모두 넣는다 
		
		for( i = 0; i < pOldCell->m_uiNearCellCount; ++i ) {
			
			m_pRemoveCell[ m_uiRemoveCellCount++ ] = pOldCell->m_pNearCell[ i ];
			
		}
		
	} else {
		// 삭제할 셀 리스트와 새로운 셀 리스트를 구한다           

	  	for( i = 0; i < 9; ++i ) {
			
			if( pOldCell->m_pNearCell[ i ] ) {
				// remove cell 얻기 
				for( j = 0; j < pCurrentCell->m_uiNearCellCount; ++j ) {
					
					if( pOldCell->m_pNearCell[ i ] == pCurrentCell->m_pNearCell[ j ] ) break;
				}
				
				if( j == pCurrentCell->m_uiNearCellCount ) {
					// remove cell에 추가 
					
					m_pRemoveCell[ m_uiRemoveCellCount++ ] = pOldCell->m_pNearCell[ i ];
				}
			}

			
			if( pCurrentCell->m_pNearCell[ i ] ) {
				
				// new cell 얻기 	
				for( j = 0; j < pOldCell->m_uiNearCellCount; ++j ) { 

					if( pCurrentCell->m_pNearCell[ i ] == pOldCell->m_pNearCell[ j ] ) break;

				}

				if( j == pOldCell->m_uiNearCellCount ) {
					// new cell에 추가 

					m_pNewCell[ m_uiNewCellCount++ ] = pCurrentCell->m_pNearCell[ i ];
				}

			}

		}

	}

}


BOOL ITZMap::AddItem( int x, int y, UI16 itemIndex )
{

	sItem *pItem = &m_pItem[ y * m_mapXtile + x ];

	if( pItem->itemIndex != 0 ) return FALSE;
	
	pItem->itemIndex = itemIndex;

	SendAddItemMsgToOthers( pItem );

	return TRUE;
}


BOOL ITZMap::DelItem( int x, int y, UI16 itemIndex )
{

	sItem *pItem = &m_pItem[ y * m_mapXtile + x ];

	if( pItem->itemIndex == 0 ) return FALSE;
	
	SendDelItemMsgToOthers( pItem );

	pItem->itemIndex = 0;

	return TRUE;
}

