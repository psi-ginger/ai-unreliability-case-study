#ifndef _ITZWORLD_H
#define _ITZWORLD_H

#include "ITZObject.h"
#include "ITZList.h"

struct sItem
{
	UI16	x;
	UI16	y;
	UI16	itemIndex;
};

template< class TMap >
class ITZWorld
{
public:
	ITZWorld();
	~ITZWorld();

	void CreateWorld( int numOfMaps, int sizePersonList,
					  int screenXpx, int screenYpx, int tileXpx, int tileYpx, 
		              int cellXtile = 0, int cellYtile = 0 );

	TMap* GetMap( int mapIndex );

public:
	int					m_screenXpx;								// 단위 : pixel
	int					m_screenYpx;

	int					m_screenXtile;								// 단위 : tile
	int					m_screenYtile;

	int					m_tileXpx;									// 단위 : pixel
	int					m_tileYpx;

	int					m_cellXtile;								// 단위 : tile
	int					m_cellYtile;

	TMap*				m_pMap;

	int					m_numOfMaps;


	ITZList< ITZObjectPtr >		m_personPtrList;					//
	
	int					m_count;

};


template< class TMap >
ITZWorld< TMap >::ITZWorld()
{
	m_pMap = NULL;

	m_count = 0;
}


template< class TMap >
ITZWorld< TMap >::~ITZWorld()
{
	delete [] m_pMap;
	m_pMap = NULL;
}


template< class TMap >
void ITZWorld< TMap >::CreateWorld( int numOfMaps, int sizePersonList, 
						    int screenXpx, int screenYpx, int tileXpx, int tileYpx, 
						    int cellXtile, int cellYtile )
{

	m_numOfMaps = numOfMaps;

	// 맵 생성
	m_pMap = new TMap[ numOfMaps ];

	for( int i = 0; i < numOfMaps; ++i ) { 

		m_pMap[ i ].SetWorld( (ITZWorld< ITZMap >* )this );
	}

	m_tileXpx = tileXpx;
	m_tileYpx = tileYpx;

	m_screenXpx = screenXpx;
	m_screenYpx = screenYpx;

	m_screenXtile = ( screenXpx + tileXpx - 1 ) / tileXpx;
	m_screenYtile = ( screenYpx + tileYpx - 1 ) / tileYpx;

	// 섹터 크기 결정하기
	if( cellXtile == 0 || cellYtile == 0 ) { 

		m_cellXtile = (int)(m_screenXtile * 1.0);
		m_cellYtile = (int)(m_screenYtile * 1.0);

	} else {

		m_cellXtile = cellXtile;
		m_cellYtile = cellYtile;
	}

	m_personPtrList.CreateList( sizePersonList, TRUE );

}

template< class TMap >
TMap* ITZWorld< TMap >::GetMap( int mapIndex )
{
	return &m_pMap[ mapIndex ];
}

#endif
