//----------------------------------------------------------------------------
//
//	버그가 생기면 수정 할 버그 전용 npc의 다이얼로그
//
//	2008.05.08 손성웅
//
//----------------------------------------------------------------------------
#ifndef _NCHANGE_DLG_
#define _NCHANGE_DLG_

#pragma once

#include "Char\CharManager\CharManager.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


class NChangeDlg : public CDialogController
{
	public:
		NChangeDlg();
		~NChangeDlg();

		void Create( void );

		static void CALLBACK StaticNChangeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		void CALLBACK NChangeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
		void Action(void);
		bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );
		void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
		bool Checkslot();
		bool Checkstatus() ;
		bool Checkgacha( SI32 siItemUnique  );// 가차인지
			
	private:
		InterfaceFactory	m_InterfaceFactory;
		cltItem *Targetitem;
		// 가지고 있는 스탯
		UI16		uiStr ;
		UI16		uiDex ;
		UI16		uiAgi ;		
		UI16		uiMag ;
		UI16		uiVit ;
		UI16		uiHnd ;
		UI16		uiLuk ;
		SI32		ItemPos;
		UI16		Status;
		SI32		ItemUnique;
		cltClickAndDrop_Item* pBeforeItem ;
};

#endif