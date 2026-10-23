#ifndef _NNEWNPCREPAIR_H_
#define _NNEWNPCREPAIR_H_

#include <Directives.h>

#include "../../../Common/Item/ItemCommon/cltItem.h"

#include "../../InterfaceMgr/DialogController.h"


class NNewNPCRepairDlg : public CDialogController
{
public:
	NNewNPCRepairDlg();
	~NNewNPCRepairDlg();

	void Init();

	void Create( );

	static	void CALLBACK StaticNNewNPCRepairDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NNewNPCRepairDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show( bool bShow );
	void Action();

	void RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	virtual bool EndClickAndDrop( cltClickAndDrop* pclClickAndDrop );

	SI32 GetRepairItemPos()					{		return m_siRepairItemPos;						}
	SI32 GetMaterialItemCount()				{		return m_ExtraMaterial.GetSize();				}
	SI32 GetMaterialItemPos(SI32 siIndex)	{		return m_ExtraMaterial[siIndex].siStgPos;		}

	bool SendRepairMsg();

	void SetTheDropItemMessage( void );

	void SetNPCText( TCHAR* pszText );
	void InitDialogExceptNPCText( void );

private:
	InterfaceFactory		m_InterfaceFactory;

	SI16					m_siRepairItemPos;

	RepairItemExtraMaterial	m_ExtraMaterial;

};

#endif