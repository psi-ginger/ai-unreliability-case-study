
#include "Resource.h"
#include "../../InterfaceMgr/DialogController.h"


#include "CommonHeader.h"
#include "StructureClass/StructureClass.h"
#include "../../../CommonLogic/HiredNPCMgr/HiredNPCMgr.h"

class cltStructureMngBasic;
class CEdit;
class CStatic;
class CList;
class CButton;
class CComboBox; 

class CNHireSystemNPCDlg : public CDialogController
{
public:
	CNHireSystemNPCDlg();
	~CNHireSystemNPCDlg();
	
	
    static void CALLBACK  StatiCNHireSystemNPCDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NHireSystemNPCDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	void Action();

	void							Create();
	

	static cltStructureMngBasic    *m_pclStructureManager;	
	static SI32						m_siCount;
	static cltHiredNPCPayInfo		m_clHiredNPCPayInfo[ MAX_HIREDNPC_KIND_NUM ];
	static BOOL						m_bUpdateNPCPaySwitch;
	void Set(cltStructureMngBasic *pclStructureManager);

private:
    
    CEdit                   *m_pNHireNPC_Edit1;
    CEdit                   *m_pNHireNPC_Edit2;
    CStatic                 *m_pNHireNPC_Static1;
    CStatic                 *m_pNHireNPC_Static2;
    CComboBox               *m_pNHireNPC_Combo;
    CButton                 *m_pNHireNPC_Button1;
    CButton                 *m_pNHireNPC_Button2;
    CList                   *m_pNHireNPC_List;
	//SI32                    m_siNPCCharUnique;


};
