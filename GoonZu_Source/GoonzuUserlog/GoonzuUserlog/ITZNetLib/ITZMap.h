#ifndef _ITZMAP_H
#define _ITZMAP_H

#include "itzbase.h"
#include "ITZWorld.h"

class ITZPerson;
class ITZCell;


class ITZMap
{
public:
	ITZMap();
	~ITZMap();

public:
	void				CreateMap( int mapXtile, int mapYtile );
	void				SetWorld( ITZWorld< ITZMap > *pWorld ); 

	int					GetCellIndex( int x, int y );
	ITZCell*			GetCell( int x, int y );

	void				SetCellList( ITZCell *pOldCell, ITZCell *pCurrentCell );

	void				Warp( int x, int y, ITZPerson *pPerson, bool bticketusewarpswitch );
	void				MoveChar( int x, int y, ITZPerson *pPerson, bool bticketusewarpswitch );

	BOOL				AddItem( int x, int y, UI16 itemIndex );
	BOOL				DelItem( int x, int y, UI16 itemIndex );

public:
	virtual void		SendPersonEnterMsgToOthers( ITZPerson *pPerson, bool bticketusewarpswitch ) {};
	virtual void		SendPersonLeaveMsgToOthers( ITZPerson *pPerson ) {};
	virtual void		SendPersonMoveMsgToOthers( ITZPerson *pPerson ) {};

	virtual void		SendAddItemMsgToOthers( sItem *pItem ) {};
	virtual void		SendDelItemMsgToOthers( sItem *pItem ) {};

	virtual void		SendNewOthersInfoToPerson( ITZPerson *pPerson ) {};
	virtual void		SendNewItemsInfoToPerson( ITZPerson *pPerson ) {};


protected:
	ITZWorld< ITZMap >*	m_pWorld;

	ITZCell*			m_pCell;

	int					m_mapXtile;									// 맵의 가로 크기 (단위:타일)
	int					m_mapYtile;									// 멥의 세로 크기 (단위:타일)

	int					m_mapXcell;									// 맵의 가로 크기 (단위:셀)
	int					m_mapYcell;									// 맵의 세로 크기 (단위:셀)

	UI32				m_uiCellCount;								// 맵의 전체 셀의 개수 

	ITZCell*			m_pRemoveCell[ 9 ];							// 가시영역에서 제외되는 셀
	ITZCell*			m_pNewCell[ 9 ];							// 가시영역에 새로 추가되는 셀

	UI32				m_uiRemoveCellCount;						// 가시영역에서 제외되는 셀 개수
	UI32				m_uiNewCellCount;							// 가시영역에 새로 추가되는 셀 개수

	sItem*				m_pItem;
};


#endif

