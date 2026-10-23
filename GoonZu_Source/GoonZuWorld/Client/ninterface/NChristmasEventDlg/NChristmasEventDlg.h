	/*====================================

클래스 : NChristmasEventDlg
작성일 : 07 .11 . 20
작성자: 손성웅
변경사항:
차후작업:
=====================================*/

#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Commonlogic/StructureClass/StructureClass.h"

#include "../CommonLogic/CommonLogic.h"
#include "../common/CommonHeader.h"
#include"../../common/Char/CharManager/CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../common/Item/ItemCommon/Item-EventStructure.h"
#include "../Client/client.h"
#include "../Common/Util/Util.h"

#define MATERIAL_ITEM_MAX_NUM	6	// 아이템 재료 최대 개수


class NChristmasEventDlg : public CDialogController
{
public:
	NChristmasEventDlg();
	~NChristmasEventDlg();

	void init();
	void Destroy();

	void Create();

	static void CALLBACK StaticSelectChristmasEventProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSelectChristmasEventProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void changeEditbox(SI16 *itemUnique, SI16* sitemNum, SI16 m_siConstructType);
	void AcceptItem();

	InterfaceFactory	m_InterfaceFactory;
	/*cltStructureMngBasic*	m_pStrManager;*/
	SI16 matitemunique[MAX_MATERIAL_NUMBER];	//필요한 재료 아이템유니크
	SI16 matitemnum[MAX_MATERIAL_NUMBER];		//필요한 재료아이템 갯수
	SI16 matitemEndunique[MAX_MATERIAL_NUMBER];	//가지고 있는 재료아이템 유니크
	SI16 matitemEndnum[MAX_MATERIAL_NUMBER];	//가지고 있는 재료아이템 갯수

	SI16 EventTreeType;//트리 타입.
};