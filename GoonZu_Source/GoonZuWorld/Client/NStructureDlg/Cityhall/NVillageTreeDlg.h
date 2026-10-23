#ifndef _NVILLAGETREEDLG_H
#define _NVILLAGETREEDLG_H

#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

enum {
	TREE_VILLAGE = 1000,

	EDITBOX_VILLAGE,

	BUTTON_DECLAREWAR,
	BUTTON_CANCEL
};

class cltVillageInfo;
class cltStructureMngBasic;

class CStatic;
class CEdit;
class CButton;
class CTree;

class NVillageTreeDlg : public CDialogController
{
public:
	NVillageTreeDlg();
	~NVillageTreeDlg();

	void Create();
	void Init();

	static void CALLBACK StaticCallBackVillageTreeDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackVillageTreeDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();

	void InsertTreeItem( SI32 parentref, cltVillageInfo *pParentVill );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	CTree		*m_pTree;

	CButton		*m_pDeclareWarButton;
	CButton		*m_pCancelButton;

	CEdit		*m_pEdit;

	cltStructureMngBasic*	m_pclStructureManager;

	SI32		m_selectedVillage;
};

#endif
