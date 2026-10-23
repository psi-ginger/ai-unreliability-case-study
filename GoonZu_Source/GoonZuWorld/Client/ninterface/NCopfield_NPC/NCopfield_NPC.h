/* ==========================================================================
클래스 :		 

작성일 :		 
작성자 :		 

변경사항 :		 

차후작업 :		
========================================================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


class NCopfield_NPC : public CDialogController
{
public:

	NCopfield_NPC();
	~NCopfield_NPC();

	void	Create( void );
	void	Init( void );
	bool	EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
	
	void	SetItemList( void );
	void	DelUseItem( void );
	void	BuyItem( void );
	
	static void CALLBACK StaticNCopfield_NPCProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NCopfield_NPCProc( UINT nEvent, int nControlID, CControllerObj* pControl );


private:
	InterfaceFactory	m_InterfaceFactory;

	cltItem				m_clUseItem;
	SI32				m_siUseItemInvenPos;
	GMONEY				m_siUseMoney;
};
