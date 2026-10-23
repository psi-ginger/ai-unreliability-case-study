#include "../Client/NInterface/NNewItemMalldlg/NNewItemMall.h"
#include "../Client/NInterface/NNewItemMalldlg/NBuyWindowDlg.h"
#include "../Client/NInterface/NNewItemMalldlg/NPresentWindowDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/list.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "Char\CharManager\CharManager.h"

//#include "../common/ItemMallManager/ItemMallManager.h"


#include "../common/Char/ccd/CharacterDrawing.h"
#include "../client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../CommonLogic/CommonLogic.h"

#include "../../ninterface/NNewInventory/NNewInventory.h"
#include "../../ninterface/NCommonMsgDlg/NCommonMsgDlg.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;
extern	bool					bActiveSwitch;		

CNNewItemMallDlg::CNNewItemMallDlg() : CDialogController()
{
	
	// �̸� ���⸦ ���ؼ� �� ������ �� ���� �ʱ�ȭ
	strcpy(szHelmetCode,		"KOR01");
	strcpy(szArmourCode,		"KOR01");
	strcpy(szWeaponCode,		"KOR01");
	strcpy(szMantleCode,		"");
	strcpy(szHorseCode,			"");

	m_pCharDraw = NULL;

	m_dwStratClock = 0;
	m_dwLastClock  = 0;

	strcpy(m_szType, "A");

	m_siRealCash = 0;
	m_siEventCash = 0;

	m_siAniType  = WT2U;
	m_siAniDirection = CCD_SOUTH_EAST;
	m_bHaveHorse = false;

	SI32 i;
	for(i = 0; i < MAX_SHAPE_ITEM; ++i)
	{
		clCharItem[i].Init();
	}

	m_siMaxPageNumber = 1;			// �Ѿ����� ������ ��
	m_siCurrentPageNumber = 1;		// ������ ������ ��

	m_siFirstMenuSelect = 0;
	m_siSecondMenuSelect = 0;
	m_siTempKey = 0;				// ���� �޴� �ε����� ������ �ش�

	m_siItemListSelect = 0;			// 6���߿� �����ϴ°�!!

	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		m_bSelect[i] = false;					// ���� or ���
	}

	for(i = 0; i < SELECT_ITEM_NUM; ++i)
	{
		m_bPreviewSelect[i] = false;			
	}	  

	m_siSetItemLoad = 0;

	// ������ ������ �ʱ�ȭ
	InitItemMallData();

	// �̸� ���⿡ ���� �������� 4��
	m_vecSelectItemMall.reserve(4);
	InitSelectItemMallData();

	m_pButtonForward		= NULL;
//	m_pButtonPresent= NULL;
	m_pButtonExit			= NULL;

	m_pButtonCharacter		= NULL;
	m_pButtonPower			= NULL;			// �Ŀ�
	m_pButtonSummonHorse	= NULL;		// ��ȯ/��
	m_pButtonMaterialFood	= NULL;		// ���/����
	m_pButtonBeginner		= NULL;			// �ʺ���

	m_pButtonNewItem		= NULL;			// ��������
	m_pButtonPopularItem	= NULL;		// �α������
	m_pButtonSpecialItem	= NULL;		// ����Ⱦ�����

	m_pButtonSubject1		= NULL;			// ���� �޴�1
	m_pButtonSubject2		= NULL;			// ���� �޴�2	
	m_pButtonSubject3		= NULL;			// ���� �޴�3
	m_pButtonSubject4		= NULL;			// ���� �޴�4
	m_pButtonSubject5		= NULL;			// ���� �޴�5

	m_pButtonBuy1			= NULL;
	m_pButtonBuy2			= NULL;
	m_pButtonBuy3			= NULL;
	m_pButtonBuy4			= NULL;
	m_pButtonBuy5			= NULL;
	m_pButtonBuy6			= NULL;

//	m_pButtonPresent1		= NULL;
//	m_pButtonPresent2		= NULL;
//	m_pButtonPresent3		= NULL;
//	m_pButtonPresent4		= NULL;
//	m_pButtonPresent5		= NULL;
//	m_pButtonPresent6		= NULL;

	m_pButtonReset			= NULL;
	m_pButtonNcoin			= NULL;

	m_pButtonPrev 			= NULL;
	m_pButtonNext 			= NULL;

	for(i = 0; i < 4; ++i)
	{
		m_pButtonSmallBuy[i]= NULL;
		m_pButtonSmallNo[i]	= NULL;
	}

	m_pButtonLeftTurn	= NULL;
	m_pButtonRightTurn	= NULL;

	m_pButtonGoldItemMall			= NULL;		
	m_pButtonEventItemMall			= NULL;		
	m_pButtonEventItemMallExplain	= NULL;
	m_pButtonEventMoneyInfo			= NULL;		

//	m_pButtonFreeReplenishment = NULL;

	for(i = 0; i < 6; ++i)
	{
		m_pButtonPresent[i]			= NULL;
		m_pISItem[i]				= NULL;
		m_pISShowItem[i]			= NULL;
		m_pISItemName[i]			= NULL;
		m_pStaticItemPrice[i]		= NULL;
		m_pStaticItemEventPrice[i]	= NULL;
		m_pStaticSilverCoin[i]		= NULL;
	}

	m_pStaticCurrentPage	= NULL;
	m_pStaticCurrentPage1	= NULL;
	m_pStaticMaxPage		= NULL;
	m_pStaticMaxPage1		= NULL;

	m_pStaticMyNcoin		= NULL;
	m_pStaticEvenyMoney		= NULL;
    
	m_pList					= NULL;
	m_pButtonGetItem		= NULL;

	m_bBillPageActive		= TRUE;

	m_bEventItemMall		= false;
}

void CNNewItemMallDlg::InitItemMallData()
{
	SI32 i;

	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		m_ItemDataList[i].Init();
	}
}


void CNNewItemMallDlg::InitSelectItemMallData()
{
	m_vecSelectItemMall.clear();
}


void CNNewItemMallDlg::InitItemImageStatic()
{
	SI32 i;
	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		m_bSelect[i] = false;
		m_pISItem[i]->SetFontIndex(0);
		m_pStaticItemPrice[i]->SetText("");
		m_pStaticItemEventPrice[i]->SetText("");
	}
}

void CNNewItemMallDlg::InitEquipItemData()
{
	SI32 i;
	for(i = 0; i < MAX_SHAPE_ITEM; ++i)
	{
		clCharItem[i].Init();
	}
}

void CNNewItemMallDlg::InitSelectEquipItemData( SI16 index )
{
	clCharItem[index].Init();
}

CNNewItemMallDlg::~CNNewItemMallDlg()
{
	//[����] â�� �Ҹ� �� �� �ѹ��� â���� Ȯ���Ͽ� ������������ �ݴ´�. => 2008-9-19 
	cltClient *pclclient = (cltClient *)pclClient;
	if( pclclient->m_pDialog[ NBUYWINDOW_DLG ] )
		pclclient->m_pDialog[ NBUYWINDOW_DLG ]->DeleteDialog();

	if( pclclient->m_pDialog[ NPRESENTWINDOW_DLG ] )
		pclclient->m_pDialog[ NPRESENTWINDOW_DLG ]->DeleteDialog();

	if(pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ])
		pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ]->DeleteDialog();

	if(pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ])
		pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ]->DeleteDialog();

	InitSelectItemMallData();

	if( g_bFullScreen)
	{
		GP.FreeSpr(m_SprBack);
	}

	SAFE_DELETE(m_pCharDraw);

	SAFE_DELETE(m_pButtonGetItem);
	SAFE_DELETE(m_pButtonForward);
//	SAFE_DELETE(m_pButtonPresent);
	SAFE_DELETE(m_pButtonExit);

	SAFE_DELETE(m_pButtonCharacter);
	SAFE_DELETE(m_pButtonPower);		// �Ŀ�
	SAFE_DELETE(m_pButtonSummonHorse);		// ��ȯ/��
	SAFE_DELETE(m_pButtonMaterialFood);		// ���/����
	SAFE_DELETE(m_pButtonBeginner);			// �ʺ���

	SAFE_DELETE(m_pButtonNewItem);		// ��������
	SAFE_DELETE(m_pButtonPopularItem);		// �α������
	SAFE_DELETE(m_pButtonSpecialItem);		// ����Ⱦ�����

	SAFE_DELETE(m_pButtonSubject1);		// ���� �޴�1
	SAFE_DELETE(m_pButtonSubject2);			// ���� �޴�2	
	SAFE_DELETE(m_pButtonSubject3);			// ���� �޴�3
	SAFE_DELETE(m_pButtonSubject4);			// ���� �޴�4
	SAFE_DELETE(m_pButtonSubject5);

	SAFE_DELETE(m_pButtonBuy1);
	SAFE_DELETE(m_pButtonBuy2);
	SAFE_DELETE(m_pButtonBuy3);
	SAFE_DELETE(m_pButtonBuy4);
	SAFE_DELETE(m_pButtonBuy5);
	SAFE_DELETE(m_pButtonBuy6);

//	SAFE_DELETE(m_pButtonPresent1);
//	SAFE_DELETE(m_pButtonPresent2);
//	SAFE_DELETE(m_pButtonPresent3);
//	SAFE_DELETE(m_pButtonPresent4);
//	SAFE_DELETE(m_pButtonPresent5);
//	SAFE_DELETE(m_pButtonPresent6);

	SAFE_DELETE(m_pButtonReset);
	SAFE_DELETE(m_pButtonNcoin);

	SAFE_DELETE(m_pButtonPrev);
	SAFE_DELETE(m_pButtonNext);

	SI32 i;
	for(i = 0; i < 4; ++i)
	{
		SAFE_DELETE(m_pButtonSmallBuy[i]);
		SAFE_DELETE(m_pButtonSmallNo[i]);
	}

	SAFE_DELETE(m_pButtonLeftTurn);
	SAFE_DELETE(m_pButtonRightTurn);

//	SAFE_DELETE(m_pButtonFreeReplenishment);

	SAFE_DELETE(m_pButtonGoldItemMall)
	SAFE_DELETE(m_pButtonEventItemMall)
	SAFE_DELETE(m_pButtonEventItemMallExplain)
	SAFE_DELETE(m_pButtonEventMoneyInfo)

	for(i = 0; i < 6; ++i)
	{
		SAFE_DELETE(m_pButtonPresent[i]);
		SAFE_DELETE(m_pISItem[i]);
		SAFE_DELETE(m_pISShowItem[i]);
		SAFE_DELETE(m_pISItemName[i]);
		SAFE_DELETE(m_pStaticItemPrice[i]);
		SAFE_DELETE(m_pStaticItemEventPrice[i]);
		SAFE_DELETE(m_pStaticSilverCoin[i]);
	}

	SAFE_DELETE(m_pStaticCurrentPage);
	SAFE_DELETE(m_pStaticMaxPage);

	SAFE_DELETE(m_pStaticCurrentPage1);
	SAFE_DELETE(m_pStaticMaxPage1);

	SAFE_DELETE(m_pStaticMyNcoin);
	SAFE_DELETE(m_pStaticEvenyMoney);

	SAFE_DELETE(m_pList);
}

void CNNewItemMallDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclClient->IsWhereServiceArea(ConstServiceArea_English ) )
	{
		Show(false);
	}
	
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NNewItemMallDlg/DLG_NNewItemMall.ddf");
		file.CreatePopupDialog( this, NNEWITEMMALL_DLG, "dialog_NNewItemMall", StaticCallBackDialogNNewItemMallDlg );

		if( g_bFullScreen)
		{
			MovePos( 104, 73 );
			//GP.LoadSprFromFile("NInterface/Interface/Interface_1024x768_00_000.SPR", &m_SprBack);
		}
		else
		{
			MovePos( 80, 40 );
		}

		m_pCharDraw = new CCharacterDrawing(pclClient->GameMode);

		NEWCBUTTON(m_pButtonForward);
//		NEWCBUTTON(m_pButtonPresent);
		NEWCBUTTON(m_pButtonExit);

		NEWCIMAGESTATIC(m_pButtonCharacter);
		NEWCIMAGESTATIC(m_pButtonPower);		// �Ŀ�
		NEWCIMAGESTATIC(m_pButtonSummonHorse);		// ��ȯ/��
		NEWCIMAGESTATIC(m_pButtonMaterialFood);		// ���/����
		NEWCIMAGESTATIC(m_pButtonBeginner);			// �ʺ���

		NEWCBUTTON(m_pButtonNewItem);		// ��������
		NEWCBUTTON(m_pButtonPopularItem);		// �α������
		NEWCBUTTON(m_pButtonSpecialItem);		// ����Ⱦ�����

		NEWCIMAGESTATIC(m_pButtonSubject1);		// ���� �޴�1
		NEWCIMAGESTATIC(m_pButtonSubject2);			// ���� �޴�2	
		NEWCIMAGESTATIC(m_pButtonSubject3);			// ���� �޴�3
		NEWCIMAGESTATIC(m_pButtonSubject4);			// ���� �޴�4
		NEWCIMAGESTATIC(m_pButtonSubject5);			//

		NEWCBUTTON(m_pButtonBuy1);
		NEWCBUTTON(m_pButtonBuy2);
		NEWCBUTTON(m_pButtonBuy3);
		NEWCBUTTON(m_pButtonBuy4);
		NEWCBUTTON(m_pButtonBuy5);
		NEWCBUTTON(m_pButtonBuy6);

	//	NEWCBUTTON(m_pButtonPresent1);
	//	NEWCBUTTON(m_pButtonPresent2);
	//	NEWCBUTTON(m_pButtonPresent3);
	//	NEWCBUTTON(m_pButtonPresent4);
	//	NEWCBUTTON(m_pButtonPresent5);
	//	NEWCBUTTON(m_pButtonPresent6);

		NEWCBUTTON(m_pButtonReset);
		NEWCBUTTON(m_pButtonNcoin);

		NEWCBUTTON(m_pButtonPrev);
		NEWCBUTTON(m_pButtonNext);

		SI32 i;
		for(i = 0; i < 4; ++i)
		{
			NEWCBUTTON(m_pButtonSmallBuy[i]);
			NEWCBUTTON(m_pButtonSmallNo[i]);
		}

		NEWCBUTTON(m_pButtonLeftTurn);
		NEWCBUTTON(m_pButtonRightTurn);

		NEWCBUTTON(m_pButtonGoldItemMall)
		NEWCBUTTON(m_pButtonEventItemMall)
		NEWCBUTTON(m_pButtonEventItemMallExplain)
		NEWCBUTTON(m_pButtonEventMoneyInfo)

//		NEWCBUTTON(m_pButtonFreeReplenishment);

		for(i = 0; i < 6; ++i)
		{
			NEWCBUTTON(m_pButtonPresent[i]);
			NEWCIMAGESTATIC(m_pISItem[i]);
			NEWCIMAGESTATIC(m_pISShowItem[i]);
			NEWCIMAGESTATIC(m_pISItemName[i]);
			NEWCSTATIC(m_pStaticItemPrice[i]);
			NEWCSTATIC(m_pStaticItemEventPrice[i]);
			NEWCSTATIC(m_pStaticSilverCoin[i]);
		};	

		NEWCSTATIC(m_pStaticCurrentPage);
		NEWCSTATIC(m_pStaticMaxPage);

		NEWCSTATIC(m_pStaticCurrentPage1);
		NEWCSTATIC(m_pStaticMaxPage1);

		NEWCSTATIC(m_pStaticMyNcoin);
		NEWCSTATIC(m_pStaticEvenyMoney);

		NEWCLIST(m_pList);

		NEWCBUTTON(m_pButtonGetItem);


		file.CreateControl( m_pButtonForward,				NNEWITEMMALL_DIALOG_BUTTONFORWARD,					"button_forward"); 
//		file.CreateControl( m_pButtonPresent,				NNEWITEMMALL_DIALOG_BUTTONPRESENT,					"button_present"); 
		file.CreateControl( m_pButtonExit,					NNEWITEMMALL_DIALOG_BUTTONEXIT,						"button_exit"); 

		file.CreateControl( m_pButtonCharacter,				NNEWITEMMALL_DIALOG_IMAGESTATICCHARACTER,			"imagestatic_main1"); 
		file.CreateControl( m_pButtonPower,					NNEWITEMMALL_DIALOG_IMAGESTATICPOWER,				"imagestatic_main2"); 
		file.CreateControl( m_pButtonSummonHorse,			NNEWITEMMALL_DIALOG_IMAGESTATICSUMMONHORSE,			"imagestatic_main3"); 
		file.CreateControl( m_pButtonMaterialFood,			NNEWITEMMALL_DIALOG_IMAGESTATICMATERIALFOOD,		"imagestatic_main4"); 
		file.CreateControl( m_pButtonBeginner,				NNEWITEMMALL_DIALOG_IMAGESTATICBEGINNER,			"imagestatic_main5"); 

		file.CreateControl( m_pButtonNewItem,				NNEWITEMMALL_DIALOG_BUTTONNEWITEM,					"button_newitem"); 
		file.CreateControl( m_pButtonPopularItem,			NNEWITEMMALL_DIALOG_BUTTONPOPULARITEM,				"button_popularitem"); 
		file.CreateControl( m_pButtonSpecialItem,			NNEWITEMMALL_DIALOG_BUTTONSPECIALITEM,				"button_specialitem"); 

		file.CreateControl( m_pButtonSubject1,				NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT1, 			"imagestatic_subject1"); 
		file.CreateControl( m_pButtonSubject2,				NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT2, 			"imagestatic_subject2"); 
		file.CreateControl( m_pButtonSubject3,				NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT3, 			"imagestatic_subject3"); 
		file.CreateControl( m_pButtonSubject4,				NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT4, 			"imagestatic_subject4"); 
		file.CreateControl( m_pButtonSubject5,				NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT5, 			"imagestatic_subject5"); 

		file.CreateControl( m_pButtonBuy1,					NNEWITEMMALL_DIALOG_BUTTONBUY1, 					"button_buy1"); 
		file.CreateControl( m_pButtonBuy2,					NNEWITEMMALL_DIALOG_BUTTONBUY2, 					"button_buy2"); 
		file.CreateControl( m_pButtonBuy3,					NNEWITEMMALL_DIALOG_BUTTONBUY3, 					"button_buy3"); 
		file.CreateControl( m_pButtonBuy4,					NNEWITEMMALL_DIALOG_BUTTONBUY4, 					"button_buy4"); 
		file.CreateControl( m_pButtonBuy5,					NNEWITEMMALL_DIALOG_BUTTONBUY5, 					"button_buy5"); 
		file.CreateControl( m_pButtonBuy6,					NNEWITEMMALL_DIALOG_BUTTONBUY6, 					"button_buy6"); 

		file.CreateControl( m_pButtonPresent[0],			NNEWITEMMALL_DIALOG_BUTTONPRESENT1, 				"button_present1"); 
		file.CreateControl( m_pButtonPresent[1],			NNEWITEMMALL_DIALOG_BUTTONPRESENT2, 				"button_present2"); 
		file.CreateControl( m_pButtonPresent[2],			NNEWITEMMALL_DIALOG_BUTTONPRESENT3, 				"button_present3"); 
		file.CreateControl( m_pButtonPresent[3],			NNEWITEMMALL_DIALOG_BUTTONPRESENT4, 				"button_present4"); 
		file.CreateControl( m_pButtonPresent[4],			NNEWITEMMALL_DIALOG_BUTTONPRESENT5, 				"button_present5"); 
		file.CreateControl( m_pButtonPresent[5],			NNEWITEMMALL_DIALOG_BUTTONPRESENT6, 				"button_present6"); 

		file.CreateControl( m_pButtonReset,					NNEWITEMMALL_DIALOG_BUTTONRESET, 					"button_reset"); 
		file.CreateControl( m_pButtonNcoin,					NNEWITEMMALL_DIALOG_BUTTONNCOIN, 					"button_ncoin"); 

		file.CreateControl( m_pButtonPrev,					NNEWITEMMALL_DIALOG_BUTTONPREV, 					"button_prev"); 
		file.CreateControl( m_pButtonNext,					NNEWITEMMALL_DIALOG_BUTTONNEXT, 					"button_next"); 

		file.CreateControl( m_pButtonSmallBuy[0],			NNEWITEMMALL_DIALOG_BUTTONSMALLBUY1, 				"button_NONAME1"); 
		file.CreateControl( m_pButtonSmallBuy[1],			NNEWITEMMALL_DIALOG_BUTTONSMALLBUY2, 				"button_NONAME3"); 
		file.CreateControl( m_pButtonSmallBuy[2],			NNEWITEMMALL_DIALOG_BUTTONSMALLBUY3, 				"button_NONAME5"); 
		file.CreateControl( m_pButtonSmallBuy[3],			NNEWITEMMALL_DIALOG_BUTTONSMALLBUY4, 				"button_NONAME7"); 

		file.CreateControl( m_pButtonSmallNo[0],			NNEWITEMMALL_DIALOG_BUTTONSMALLNO1, 				"button_NONAME2"); 
		file.CreateControl( m_pButtonSmallNo[1],			NNEWITEMMALL_DIALOG_BUTTONSMALLNO2, 				"button_NONAME4"); 
		file.CreateControl( m_pButtonSmallNo[2],			NNEWITEMMALL_DIALOG_BUTTONSMALLNO3, 				"button_NONAME6"); 
		file.CreateControl( m_pButtonSmallNo[3],			NNEWITEMMALL_DIALOG_BUTTONSMALLNO4, 				"button_NONAME8"); 

		file.CreateControl( m_pButtonLeftTurn,				NNEWITEMMALL_DIALOG_BUTTONLEFTTURN,					"button_NONAME9"); 
		file.CreateControl( m_pButtonRightTurn,				NNEWITEMMALL_DIALOG_BUTTONRIGHTTURN,				"button_NONAME10"); 

//		file.CreateControl( m_pButtonFreeReplenishment,		NNEWITEMMALL_DIALOG_BUTTONFREEREPLENISHMENT,		"button_NONAME11"); 

		file.CreateControl( m_pISShowItem[0],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM1, 			"imagestatic_showitem1"); 
		file.CreateControl( m_pISShowItem[1],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM2, 			"imagestatic_showitem2"); 
		file.CreateControl( m_pISShowItem[2],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM3, 			"imagestatic_showitem3"); 
		file.CreateControl( m_pISShowItem[3],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM4, 			"imagestatic_showitem4"); 
		file.CreateControl( m_pISShowItem[4],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM5, 			"imagestatic_showitem5"); 
		file.CreateControl( m_pISShowItem[5],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM6, 			"imagestatic_showitem6"); 

		file.CreateControl( m_pISItemName[0],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWNAME1, 			"imagestatic_name1"); 
		file.CreateControl( m_pISItemName[1],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWNAME2, 			"imagestatic_name2"); 
		file.CreateControl( m_pISItemName[2],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWNAME3, 			"imagestatic_name3"); 
		file.CreateControl( m_pISItemName[3],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWNAME4, 			"imagestatic_name4"); 
		file.CreateControl( m_pISItemName[4],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWNAME5, 			"imagestatic_name5"); 
		file.CreateControl( m_pISItemName[5],				NNEWITEMMALL_DIALOG_IMAGESTATICSHOWNAME6, 			"imagestatic_name6"); 

		file.CreateControl( m_pISItem[0],					NNEWITEMMALL_DIALOG_IMAGESTATIC1, 					"imagestatic_NONAME1"); 
		file.CreateControl( m_pISItem[1],					NNEWITEMMALL_DIALOG_IMAGESTATIC2, 					"imagestatic_NONAME2"); 
		file.CreateControl( m_pISItem[2],					NNEWITEMMALL_DIALOG_IMAGESTATIC3, 					"imagestatic_NONAME3"); 
		file.CreateControl( m_pISItem[3],					NNEWITEMMALL_DIALOG_IMAGESTATIC4, 					"imagestatic_NONAME4"); 
		file.CreateControl( m_pISItem[4],					NNEWITEMMALL_DIALOG_IMAGESTATIC5, 					"imagestatic_NONAME5"); 
		file.CreateControl( m_pISItem[5],					NNEWITEMMALL_DIALOG_IMAGESTATIC6, 					"imagestatic_NONAME6"); 

		file.CreateControl( m_pStaticItemPrice[0],			NNEWITEMMALL_DIALOG_STATICITEMPRICE1, 				"statictext_NONAME1"); 
		file.CreateControl( m_pStaticItemPrice[1],			NNEWITEMMALL_DIALOG_STATICITEMPRICE2, 				"statictext_NONAME2"); 
		file.CreateControl( m_pStaticItemPrice[2],			NNEWITEMMALL_DIALOG_STATICITEMPRICE3, 				"statictext_NONAME3"); 
		file.CreateControl( m_pStaticItemPrice[3],			NNEWITEMMALL_DIALOG_STATICITEMPRICE4, 				"statictext_NONAME4"); 
		file.CreateControl( m_pStaticItemPrice[4],			NNEWITEMMALL_DIALOG_STATICITEMPRICE5, 				"statictext_NONAME5"); 
		file.CreateControl( m_pStaticItemPrice[5],			NNEWITEMMALL_DIALOG_STATICITEMPRICE6, 				"statictext_NONAME6"); 


		if( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
		{
			file.CreateControl( m_pButtonGoldItemMall,			NNEWITEMMALL_DIALOG_BUTTON_GOLD_ITEM_MALL, 			"button_gold_item_mall"); 
			file.CreateControl( m_pButtonEventItemMall,			NNEWITEMMALL_DIALOG_BUTTON_EVENT_ITEM_MALL, 		"button_event_item_mall"); 
			file.CreateControl( m_pButtonEventItemMallExplain,	NNEWITEMMALL_DIALOG_BUTTON_EVENT_ITEM_MALL_EXPLAIN, "button_event_item_mall_explain"); 
			file.CreateControl( m_pButtonEventMoneyInfo,		NNEWITEMMALL_DIALOG_BUTTON_EVENT_MONEY_INFO, 		"button_event_money_info"); 

			file.CreateControl( m_pStaticItemEventPrice[0],		NNEWITEMMALL_DIALOG_STATICITEMEVENTPRICE1, 			"statictext_EventPrice1"); 
			file.CreateControl( m_pStaticItemEventPrice[1],		NNEWITEMMALL_DIALOG_STATICITEMEVENTPRICE2, 			"statictext_EventPrice2"); 
			file.CreateControl( m_pStaticItemEventPrice[2],		NNEWITEMMALL_DIALOG_STATICITEMEVENTPRICE3, 			"statictext_EventPrice3"); 
			file.CreateControl( m_pStaticItemEventPrice[3],		NNEWITEMMALL_DIALOG_STATICITEMEVENTPRICE4, 			"statictext_EventPrice4"); 
			file.CreateControl( m_pStaticItemEventPrice[4],		NNEWITEMMALL_DIALOG_STATICITEMEVENTPRICE5, 			"statictext_EventPrice5"); 
			file.CreateControl( m_pStaticItemEventPrice[5],		NNEWITEMMALL_DIALOG_STATICITEMEVENTPRICE6, 			"statictext_EventPrice6"); 
		}

		file.CreateControl( m_pStaticCurrentPage,			NNEWITEMMALL_DIALOG_STATIC1, 						"statictext_currentpage"); 
		file.CreateControl( m_pStaticMaxPage,				NNEWITEMMALL_DIALOG_STATIC2, 						"statictext_maxpage"); 

		file.CreateControl( m_pStaticCurrentPage1,			NNEWITEMMALL_DIALOG_STATIC1, 						"statictext_currentpage1"); 
		file.CreateControl( m_pStaticMaxPage1,				NNEWITEMMALL_DIALOG_STATIC2, 						"statictext_maxpage1"); 

		file.CreateControl( m_pStaticMyNcoin,				NNEWITEMMALL_DIALOG_STATICNCOIN,					"statictext_ncoin"); 
		file.CreateControl( m_pStaticEvenyMoney,			NNEWITEMMALL_DIALOG_STATICEVENYMONEY,				"statictext_eventmoney");

		file.CreateControl( m_pList,						NNEWITEMMALL_DIALOG_LIST,							"listbox_NONAME1"); 

		file.CreateControl( m_pStaticSilverCoin[0],			NNEWITEMMALL_DIALOG_STATIC_SILVERCOIN1, 			"statictext_SilverCoin1");
		file.CreateControl( m_pStaticSilverCoin[1],			NNEWITEMMALL_DIALOG_STATIC_SILVERCOIN2, 			"statictext_SilverCoin2");
		file.CreateControl( m_pStaticSilverCoin[2],			NNEWITEMMALL_DIALOG_STATIC_SILVERCOIN3, 			"statictext_SilverCoin3");
		file.CreateControl( m_pStaticSilverCoin[3],			NNEWITEMMALL_DIALOG_STATIC_SILVERCOIN4, 			"statictext_SilverCoin4");
		file.CreateControl( m_pStaticSilverCoin[4],			NNEWITEMMALL_DIALOG_STATIC_SILVERCOIN5, 			"statictext_SilverCoin5");
		file.CreateControl( m_pStaticSilverCoin[5],			NNEWITEMMALL_DIALOG_STATIC_SILVERCOIN6, 			"statictext_SilverCoin6");
		
		file.CreateControl( m_pButtonGetItem,				NNEWITEMMALL_DIALOG_BUTTON_GETITEM,					"button_getitem");


		// kkm �߱��� �ƴ� ������ ���� ó��
		if( false == pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina))
		{
			if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea) == false)
			{
				// ���� ��ư �Ű��ش�
				m_pButtonNcoin->MoveControl( 106, 635 );
			}

			// ������ ����â ������.
			m_pButtonGetItem->Show(false);
		}



		m_pButtonForward->Show(false);
	//	m_pButtonPresent->Show(false);

		m_pList->SetColumn(0, 116, "");
		m_pList->SetDrawLine( false );
		m_pList->Refresh();

		m_pList->SetFocusImageFileName("");

		for(i = 0; i < 4; ++i)
		{
			m_pButtonSmallBuy[i]->Show( false );
			m_pButtonSmallNo[i]->Show( false );
		}

		m_pButtonSubject1->Show(false);
		m_pButtonSubject2->Show(false);
		m_pButtonSubject3->Show(false);
 		m_pButtonSubject4->Show(false);
		m_pButtonSubject5->Show(false);


		m_pButtonCharacter->Show(false);
		m_pButtonPower->Show(false);
		m_pButtonSummonHorse->Show(false);
		m_pButtonMaterialFood->Show(false);
   		m_pButtonBeginner->Show(false);
		

		for( SI32 j = 0; j < 6; j++)
		{
			m_pStaticItemPrice[j]->SetTextPrintOption(DT_RIGHT);
			m_pStaticItemPrice[j]->SetFontWeight(FW_BOLD);
			m_pStaticItemPrice[j]->SetFontColor(RGB(60,60,60));

			if( pclclient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
			{
				m_pStaticItemEventPrice[j]->SetTextPrintOption( DT_RIGHT );
				m_pStaticItemEventPrice[j]->SetFontWeight( FW_BOLD );
				m_pStaticItemEventPrice[j]->SetFontColor( RGB(60,60,60) );
			}

			if( pclClient->siServiceArea == ConstServiceArea_Korea )
			{
				m_pStaticItemPrice[j]->Move(15,-3);
			}
		}

		// [��ȣ_NHN->NDOORS] �����۸���� ���� ����s �� �̺�Ʈ�Ӵϸ� ����Ѵ�
		//if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
		//	//if( pclClient->siServiceArea == ConstServiceArea_Korea )
		//{
		//	m_pStaticMyNcoin->Move(0,-3);
		//	//m_pStaticEvenyMoney->Move(0,-3);
		//	m_pStaticEvenyMoney->Show(false);
		//}


		SetTopMostWindow( true );

		SetCharDraw(0 , 0);

		cltClient *pclclient = (cltClient *)pclClient;
		
		m_bBillPageActive = TRUE;

		// �����۸� ó�� ������ �� �������� �ҷ��´�
		m_siFirstMenuSelect = 0;
		m_siSecondMenuSelect = 0;
		strcpy(m_szType,"N");



		cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, m_siCurrentPageNumber);
		cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

		cltMsg clMsg2( GAMEMSG_REQUEST_GET_MYCASH_INFO, 0, NULL );
 		pclclient->SendMsgToServer((sPacketHeader*)&clMsg2);


		SetTimerOnOff(TRUE);
		SetActionTimer(1000);
	}
	else
	{
		if( pclclient->m_pDialog[ NBUYWINDOW_DLG ] )
			pclclient->m_pDialog[ NBUYWINDOW_DLG ]->DeleteDialog();

		if( pclclient->m_pDialog[ NPRESENTWINDOW_DLG ] )
			pclclient->m_pDialog[ NPRESENTWINDOW_DLG ]->DeleteDialog();

		if(pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ])
			pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ]->DeleteDialog();

		if(pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ])
			pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ]->DeleteDialog();

		DeleteDialog();
	}
	//KHY - 1024 - �Ѱ��� VIP �̺�Ʈ- �̺�Ʈ â.
	//by LEEKH 2007.10.30 - �Ⱓ ����
	if (pclClient->IsCountrySwitch(Switch_HanGameVIPEvent) )
	{		
			//cltServer* pclServer = (cltServer*)pclClient;
			cltClient* pclclient = (cltClient*)pclClient;
			_SYSTEMTIME sStartAEventDay , sEndAEventDay;//�̺�Ʈ ���۽ð��� ������ �ð��α��� �ð�.

			sStartAEventDay.wYear = 2006;
			sStartAEventDay.wMonth = 11;
			sStartAEventDay.wDay = 8;
			sStartAEventDay.wHour = 0;
			sStartAEventDay.wDayOfWeek = 0;
			sStartAEventDay.wMinute = 0;
			sStartAEventDay.wSecond = 0;
			sStartAEventDay.wMilliseconds=0;

			sEndAEventDay.wYear = 2006;
			sEndAEventDay.wMonth = 11;
			sEndAEventDay.wDay = 30;
			sEndAEventDay.wHour = 0;
			sEndAEventDay.wDayOfWeek = 0;
			sEndAEventDay.wMinute = 0;
			sEndAEventDay.wSecond = 0;
			sEndAEventDay.wMilliseconds=0;

			SI32 startAEventdatevary = pclClient->GetMinuteFromsTime(sStartAEventDay);
			SI32 endAEventdatevary = pclClient->GetMinuteFromsTime(sEndAEventDay);
			SI32 LoginTime = pclClient->GetMinuteFromsTime(pclclient->sTime);

			if((LoginTime <= endAEventdatevary)
				&&(LoginTime >= startAEventdatevary))
				//�̺�Ʈ �Ⱓ���� �����̸� �����߱�.
			{
				if(pclclient->m_pDialog[ NCOMMONMSG_DLG ]==NULL) // VIP ����Ʈ ���޼���.
				{
					pclclient->CreateInterface( NCOMMONMSG_DLG );
				}
				((CNCommonMsgDlg*)pclclient->m_pDialog[ NCOMMONMSG_DLG ])->Set(COMMONDLG_HANGAME_VIP_EVENT_ITEM);			
			}
	}	

	if( pclClient->IsWhereServiceArea(ConstServiceArea_English ) )
	{
		if( pclClient->IsCountrySwitch(Switch_GlobalItemMall_Limit) == false )
		{
			// ������ �������� ������ ������ �����۸��� ����� �� �ִ�.		
			SYSTEMTIME stStart, stEnd;
			memcpy(&stStart, &pclClient->pclCM->CR[1]->pclCI->clBI.siFirstCreatTimeDateVary, sizeof(SYSTEMTIME));
			TimeAddDate(&stStart, 7, &stEnd);
			if( TimeInBounds(&pclclient->sTime, &stStart, &stEnd) == true )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8386 ) );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			// ���� 30�̻��� �Ǿ�� ������ ���� ����� �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 30 )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8387 ) );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}
		}

		Show(true);
	}

	if( pclClient->IsWhereServiceArea( ConstServiceArea_NHNChina | ConstServiceArea_Korea) )
	{
		m_pStaticEvenyMoney->Show( true );
	}
	
	// [����] �߱� �����۸� ���� �������̽��� �����ش�.
	NHNChinaInit();
	NHNChinaGoldItemMall();
}

void CALLBACK CNNewItemMallDlg::StaticCallBackDialogNNewItemMallDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNNewItemMallDlg *pThis = (CNNewItemMallDlg *)pControllerMgr;
	pThis->CallBackDialogNNewItemMallDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNNewItemMallDlg::CallBackDialogNNewItemMallDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case NNEWITEMMALL_DIALOG_BUTTON_GOLD_ITEM_MALL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
                    InitItemMallData();

					NHNChinaShowCategory( false );

					if( false == m_bEventItemMall )	break; 

					m_bEventItemMall = false;

					// �����۸� ó�� ������ �� �������� �ҷ��´�
					cltGameMsgRequest_Get_Sel_PGList pclinfo( 0, 0, "N", 1 );
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					NHNChinaGoldItemMall();
				}
				break;
			}
		}
		break;
	case NNEWITEMMALL_DIALOG_BUTTON_EVENT_ITEM_MALL:
		{
			switch( nEvent )
			{

			case EVENT_BUTTON_CLICKED:
				{
					InitItemMallData();
					//SelectThreeMenu( "E" );
					m_dwStratClock	= pclClient->CurrentClock;

					m_pButtonSubject1->Show(false);
					m_pButtonSubject2->Show(false);
					m_pButtonSubject3->Show(false);
					m_pButtonSubject4->Show(false);
					m_pButtonSubject5->Show(false);

					m_pButtonCharacter->SetFontIndex(1);
					m_pButtonPower->SetFontIndex(3);
					m_pButtonSummonHorse->SetFontIndex(5);
					m_pButtonMaterialFood->SetFontIndex(7);
					//	m_pButtonBeginner->SetFontIndex(9);

					cltClient *pclclient = (cltClient *)pclClient;
					m_siFirstMenuSelect  = 10;
					m_siSecondMenuSelect = 11;
					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, "E", 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					NHNChinaShowCategory( true );

					if( true == m_bEventItemMall )	break;

					m_bEventItemMall = true;

					NHNChinaEventItemMall();
				}
				break;
			}
		}
		break;
	case NNEWITEMMALL_DIALOG_BUTTON_EVENT_ITEM_MALL_EXPLAIN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{

				}
				break;
			}
		}
		break;
	case NNEWITEMMALL_DIALOG_BUTTON_EVENT_MONEY_INFO:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShellExecute( NULL,NULL,TEXT("http://www.newhljz.cn"),NULL,NULL,SW_SHOWNORMAL );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONFORWARD:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{

				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//	DeleteDialog();
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTON_GETITEM:
		{
			if(!pclclient->m_pDialog[NITEMMALL_DLG])
			{
				pclclient->CreateInterface(NITEMMALL_DLG);

				cltGameMsgRequest_ItemMallItemList clinfo(1);
				cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case NNEWITEMMALL_DIALOG_BUTTONEXIT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltClient *pclclient = (cltClient *)pclClient;
					if( pclclient->m_pDialog[ NBUYWINDOW_DLG ] )
						pclclient->m_pDialog[ NBUYWINDOW_DLG ]->DeleteDialog();

					if( pclclient->m_pDialog[ NPRESENTWINDOW_DLG ] )
						pclclient->m_pDialog[ NPRESENTWINDOW_DLG ]->DeleteDialog();

					if(pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ])
						pclclient->m_pDialog[ NBUYITEMINFOWINDOW_DLG ]->DeleteDialog();

					if(pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ])
						pclclient->m_pDialog[ NPRESENTINFOWINDOW_DLG ]->DeleteDialog();

					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONLEFTTURN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					++m_siAniDirection;
					if(m_siAniDirection > CCD_EAST)
						m_siAniDirection = CCD_SOUTH_EAST;
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONRIGHTTURN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					--m_siAniDirection;
					if(m_siAniDirection < CCD_SOUTH_EAST)
						m_siAniDirection = CCD_EAST;
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONNEWITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pStaticSilverCoin[0]->Show( false );
					m_pStaticSilverCoin[1]->Show( false );
					m_pStaticSilverCoin[2]->Show( false );
					m_pStaticSilverCoin[3]->Show( false );
					m_pStaticSilverCoin[4]->Show( false );
					m_pStaticSilverCoin[5]->Show( false );

					m_pButtonCharacter->Show(false);
					m_pButtonPower->Show(false);
					m_pButtonSummonHorse->Show(false);
					m_pButtonMaterialFood->Show(false);
					m_pButtonBeginner->Show(false);

					// �� ������ �ҷ�����
					SelectThreeMenu("N");
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPOPULARITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_pStaticSilverCoin[0]->Show( false );
					m_pStaticSilverCoin[1]->Show( false );
					m_pStaticSilverCoin[2]->Show( false );
					m_pStaticSilverCoin[3]->Show( false );
					m_pStaticSilverCoin[4]->Show( false );
					m_pStaticSilverCoin[5]->Show( false );

					m_pButtonCharacter->Show(false);
					m_pButtonPower->Show(false);
					m_pButtonSummonHorse->Show(false);
					m_pButtonMaterialFood->Show(false);
					m_pButtonBeginner->Show(false);

					// �Ŀ� ������ �ҷ�����
					SelectThreeMenu("P");
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSPECIALITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{			
					m_pStaticSilverCoin[0]->Show( false );
					m_pStaticSilverCoin[1]->Show( false );
					m_pStaticSilverCoin[2]->Show( false );
					m_pStaticSilverCoin[3]->Show( false );
					m_pStaticSilverCoin[4]->Show( false );
					m_pStaticSilverCoin[5]->Show( false );

					m_pButtonCharacter->Show(true);
					m_pButtonPower->Show(true);
					m_pButtonSummonHorse->Show(true);
					m_pButtonMaterialFood->Show(true);
                    
					// �߱������� �޴����� �����̴�.
					if( ConstServiceArea_China == pclClient->siServiceArea )
					{
						m_pButtonBeginner->SetFontIndex(8);
						m_pButtonBeginner->Show(true);
					}
						
					// ����� ������ �ҷ�����  -> ������ �϶����� ����
					//SelectThreeMenu("S");
 					 SendEvent(EVENT_IMAGESTATIC_LBUTTONDOWN, true, m_pButtonMaterialFood);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICCHARACTER:
 		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{

					// ��ư �����⿡ ����� �ֱ�
					//m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;
					m_siTempKey = 0;

					m_pButtonCharacter->SetFontIndex(0);
					m_pButtonPower->SetFontIndex(3);			// �Ŀ�
					m_pButtonSummonHorse->SetFontIndex(5);		// ��ȯ/��
					m_pButtonMaterialFood->SetFontIndex(7);		// ���/����
					m_pButtonBeginner->SetFontIndex(9);			// �ʺ���

					m_pButtonSubject1->Show(true);
					m_pButtonSubject1->SetFontIndex(0);
					m_pButtonSubject2->Show(true);
					m_pButtonSubject2->SetFontIndex(3);
					m_pButtonSubject3->Show(true);
					m_pButtonSubject3->SetFontIndex(5);
					m_pButtonSubject4->Show(true);
					m_pButtonSubject4->SetFontIndex(7);
					m_pButtonSubject5->Show(true);
					m_pButtonSubject5->SetFontIndex(39);

					InitItemImageStatic();

					m_siFirstMenuSelect  = ITEMALL_CHARACTER_MENU;
					m_siSecondMenuSelect = ITEMALL_CHARACTER_SUBJECT1;
					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");
					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICPOWER:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siTempKey = 1;

					m_pButtonCharacter->SetFontIndex(1);
					m_pButtonPower->SetFontIndex(2);		// �Ŀ�
					m_pButtonSummonHorse->SetFontIndex(5);	// ��ȯ/��
					m_pButtonMaterialFood->SetFontIndex(7);	// ���/����
					m_pButtonBeginner->SetFontIndex(9);			// �ʺ���

					m_pButtonSubject1->Show(true);
					m_pButtonSubject1->SetFontIndex(8);
					m_pButtonSubject2->Show(true);
					m_pButtonSubject2->SetFontIndex(11);
					m_pButtonSubject3->Show(true);
					m_pButtonSubject3->SetFontIndex(13);
					m_pButtonSubject4->Show(true);
					m_pButtonSubject4->SetFontIndex(15);
					m_pButtonSubject5->Show(false);

					InitItemImageStatic();

					m_siFirstMenuSelect = ITEMALL_POWER_MENU;
					m_siSecondMenuSelect = ITEMALL_POWER_SUBJECT1;
					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");
					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICSUMMONHORSE:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siTempKey = 2;

					m_pButtonCharacter->SetFontIndex(1);
					m_pButtonPower->SetFontIndex(3);		// �Ŀ�
					m_pButtonSummonHorse->SetFontIndex(4);	// ��ȯ/��
					m_pButtonMaterialFood->SetFontIndex(7);	// ���/����
					m_pButtonBeginner->SetFontIndex(9);			// �ʺ���

					m_pButtonSubject1->Show(true);
					m_pButtonSubject1->SetFontIndex(20);
					m_pButtonSubject2->Show(true);
					m_pButtonSubject2->SetFontIndex(23);
					
					m_pButtonSubject3->Show(false);
					m_pButtonSubject4->Show(false);
					m_pButtonSubject5->Show(false);
		
					InitItemImageStatic();

					m_siFirstMenuSelect = ITEMALL_SUMMONHORSE_MENU;
					m_siSecondMenuSelect = ITEMALL_SUMMONHORSE_SUBJECT1;
					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");
					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICMATERIALFOOD:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siTempKey = 3;

					m_pButtonCharacter->SetFontIndex(1);
					m_pButtonPower->SetFontIndex(3);		// �Ŀ�
					m_pButtonSummonHorse->SetFontIndex(5);	// ��ȯ/��
					m_pButtonMaterialFood->SetFontIndex(6);	// ���/����
					m_pButtonBeginner->SetFontIndex(9);			// �ʺ���

					m_pButtonSubject1->Show(true);
					m_pButtonSubject1->SetFontIndex(24);
					m_pButtonSubject2->Show(true);
					m_pButtonSubject2->SetFontIndex(27);
					m_pButtonSubject3->Show(true);
					m_pButtonSubject3->SetFontIndex(29);
					m_pButtonSubject4->Show(true);
					m_pButtonSubject4->SetFontIndex(31);
					m_pButtonSubject5->Show(false);
					
					InitItemImageStatic();

					m_siFirstMenuSelect = ITEMALL_MATERIALFOOD_MENU;
					m_siSecondMenuSelect = ITEMALL_MATERIALFOOD_SUBJECT1;
					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");
					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICBEGINNER:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siTempKey = 4;

					m_pButtonCharacter->SetFontIndex(1);
					m_pButtonPower->SetFontIndex(3);		// �Ŀ�
					m_pButtonSummonHorse->SetFontIndex(5);	// ��ȯ/��
					m_pButtonMaterialFood->SetFontIndex(7);	// ���/����
					m_pButtonBeginner->SetFontIndex(8);			// �ʺ���

					if( ConstServiceArea_China == pclClient->siServiceArea )
					{
						m_pButtonSubject1->Show(false);
						m_pButtonSubject2->Show(false);
						m_pButtonSubject3->Show(false);
						m_pButtonSubject4->Show(false);
						m_pButtonSubject5->Show(false);

						InitItemImageStatic();

						m_siFirstMenuSelect = ITEMALL_SILVERCOIN_MENU;
						m_siSecondMenuSelect = ITEMALL_SILVERCOIN_SUBJECT1;
						if( m_bEventItemMall )
							strcpy(m_szType, "E");
						else
							strcpy(m_szType, "A");
						cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
						cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else
					{
						m_pButtonSubject1->Show(true);
						m_pButtonSubject1->SetFontIndex(32);
						m_pButtonSubject2->Show(true);
						m_pButtonSubject2->SetFontIndex(35);
						m_pButtonSubject3->Show(true);
						m_pButtonSubject3->SetFontIndex(37);
						m_pButtonSubject4->Show(false);
						m_pButtonSubject5->Show(false);

						InitItemImageStatic();

						m_siFirstMenuSelect = ITEMALL_BEGINNER_MENU;
						m_siSecondMenuSelect = ITEMALL_BEGINNER_SUBJECT1;
						if( m_bEventItemMall )
							strcpy(m_szType, "E");
						else
							strcpy(m_szType, "A");
						cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
						cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT1:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					//m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < 1000 )
					//	return;
					
					m_siSecondMenuSelect = m_siFirstMenuSelect + 1;
					
					// ITEMALL_SUMMONHORSE_MENU �� �޴� 3,4 �� 1,2 �� �ٲ� ����ġ�� �ʿ�
					if( m_siFirstMenuSelect == ITEMALL_SUMMONHORSE_MENU  )
					{
						m_pButtonSubject1->SetFontIndex( 20 );
						m_pButtonSubject2->SetFontIndex( 23 );

					}
					else
					{
						m_pButtonSubject1->SetFontIndex(m_siTempKey*8);
						m_pButtonSubject2->SetFontIndex((m_siTempKey*8) + 3);
					}
					m_pButtonSubject3->SetFontIndex((m_siTempKey*8) + 5);
					if( m_pButtonSubject4->IsShow() == true )
						m_pButtonSubject4->SetFontIndex((m_siTempKey*8) + 7);

					m_pButtonSubject5->SetFontIndex( 39 );

					InitItemImageStatic();

					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT2:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siSecondMenuSelect = m_siFirstMenuSelect + 2;
					
					// ITEMALL_SUMMONHORSE_MENU �� �޴� 3,4 �� 1,2 �� �ٲ� ����ġ�� �ʿ�
					if( m_siFirstMenuSelect == ITEMALL_SUMMONHORSE_MENU  )
					{
 						m_pButtonSubject1->SetFontIndex( 21 );
						m_pButtonSubject2->SetFontIndex( 22 );

					}
					else
					{
						m_pButtonSubject1->SetFontIndex((m_siTempKey*8) + 1);
						m_pButtonSubject2->SetFontIndex((m_siTempKey*8) + 2);
					}
						m_pButtonSubject3->SetFontIndex((m_siTempKey*8) + 5);

					if( m_pButtonSubject4->IsShow() == true )
						m_pButtonSubject4->SetFontIndex((m_siTempKey*8) + 7);

					m_pButtonSubject5->SetFontIndex( 39 );

					InitItemImageStatic();

					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT3:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siSecondMenuSelect = m_siFirstMenuSelect + 3;

					m_pButtonSubject1->SetFontIndex((m_siTempKey*8) + 1);
					m_pButtonSubject2->SetFontIndex((m_siTempKey*8) + 3);
					m_pButtonSubject3->SetFontIndex((m_siTempKey*8) + 4);
					if( m_pButtonSubject4->IsShow() == true )
						m_pButtonSubject4->SetFontIndex((m_siTempKey*8) + 7);
					m_pButtonSubject5->SetFontIndex( 39 );

					InitItemImageStatic();

					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT4:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siSecondMenuSelect = m_siFirstMenuSelect + 4;

					m_pButtonSubject1->SetFontIndex((m_siTempKey*8) + 1);
					m_pButtonSubject2->SetFontIndex((m_siTempKey*8) + 3);
					m_pButtonSubject3->SetFontIndex((m_siTempKey*8) + 5);
					if( m_pButtonSubject4->IsShow() == true )
						m_pButtonSubject4->SetFontIndex((m_siTempKey*8) + 6);
					m_pButtonSubject5->SetFontIndex( 39 );


					InitItemImageStatic();

					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	
	case NNEWITEMMALL_DIALOG_IMAGESTATICSUBJECT5:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					m_siSecondMenuSelect = m_siFirstMenuSelect + 5;

					m_pButtonSubject1->SetFontIndex((m_siTempKey*8) + 1);
					m_pButtonSubject2->SetFontIndex((m_siTempKey*8) + 3);
					m_pButtonSubject3->SetFontIndex((m_siTempKey*8) + 5);
					m_pButtonSubject4->SetFontIndex((m_siTempKey*8) + 7);
					if( m_pButtonSubject5->IsShow() == true )
						m_pButtonSubject5->SetFontIndex( 38 );

					InitItemImageStatic();

					if( m_bEventItemMall )
						strcpy(m_szType, "E");
					else
						strcpy(m_szType, "A");

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;


	case NNEWITEMMALL_DIALOG_BUTTONPREV:	// ���� �������� ����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					--m_siCurrentPageNumber;
					if(m_siCurrentPageNumber < 1)
					{
						m_siCurrentPageNumber = 1;
					}

					InitItemImageStatic();

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, m_siCurrentPageNumber);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONNEXT:  // ���� �������� ����
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_dwStratClock	= pclClient->CurrentClock;

					//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
					//	return;

					++m_siCurrentPageNumber;

					if(m_siCurrentPageNumber >= m_siMaxPageNumber)
					{
						m_siCurrentPageNumber = m_siMaxPageNumber;
					}

					InitItemImageStatic();

					cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, m_siCurrentPageNumber);
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATIC1:
	case NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM1:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siItemListSelect = 0;			// 6���߿� �����ϴ°�!!
					SelectItemList( m_siItemListSelect );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATIC2:
	case NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM2:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siItemListSelect = 1;			// 6���߿� �����ϴ°�!!
					SelectItemList( m_siItemListSelect );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATIC3:
	case NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM3:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siItemListSelect = 2;			// 6���߿� �����ϴ°�!!
					SelectItemList( m_siItemListSelect );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATIC4:
	case NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM4:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siItemListSelect = 3;			// 6���߿� �����ϴ°�!!
					SelectItemList( m_siItemListSelect );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATIC5:
	case NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM5:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siItemListSelect = 4;			// 6���߿� �����ϴ°�!!
					SelectItemList( m_siItemListSelect );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_IMAGESTATIC6:
	case NNEWITEMMALL_DIALOG_IMAGESTATICSHOWITEM6:
		{
			switch( nEvent )
			{
			case EVENT_IMAGESTATIC_LBUTTONDOWN:
				{
					m_siItemListSelect = 5;			// 6���߿� �����ϴ°�!!
					SelectItemList( m_siItemListSelect );
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLBUY1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowSmallBuyWindow(0);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLBUY2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowSmallBuyWindow(1);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLBUY3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowSmallBuyWindow(2);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLBUY4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowSmallBuyWindow(3);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLNO1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteItemList(0);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLNO2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteItemList(1);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLNO3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteItemList(2);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONSMALLNO4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteItemList(3);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONBUY1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowBuyWindow(0);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONBUY2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowBuyWindow(1);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONBUY3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowBuyWindow(2);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONBUY4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowBuyWindow(3);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONBUY5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowBuyWindow(4);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONBUY6:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowBuyWindow(5);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowPresentWindow(0);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowPresentWindow(1);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowPresentWindow(2);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT4:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowPresentWindow(3);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT5:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowPresentWindow(4);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONPRESENT6:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					ShowPresentWindow(5);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONRESET:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// �̸�����â �ʱ�ȭ
					m_pList->Clear();
					m_pList->Refresh();

					SI32 i;
					for(i = 0; i < 4; ++i)
					{
						m_pButtonSmallBuy[i]->Show(false);
						m_pButtonSmallNo[i]->Show(false);

					}
					for(i = 0; i < SELECT_ITEM_NUM; ++i)
						m_bPreviewSelect[ i ] = false;

					for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
					{
						m_bSelect[i] = false;
						m_pISItem[i]->SetFontIndex(0);
					}
					InitEquipItemData();
					InitSelectItemMallData();

					SetCharDraw(0,0);
					SetHaveHorseDraw(0);
				}
				break;
			}
		}
		break;

	case NNEWITEMMALL_DIALOG_BUTTONNCOIN:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(pclClient->siServiceArea == ConstServiceArea_Japan)
					{
						cltGameMsgRequest_BillPage_URL clinfo( pclclient->szID,pclclient->szPassword);
						cltMsg clMsg(GAMEMSG_REQUEST_BILLPAGE_URL,sizeof(clinfo),(BYTE*)&clinfo);
						pclclient->SendMsgToServer( (sPacketHeader*)&clMsg);
					}
					else if( pclClient->siServiceArea == ConstServiceArea_Korea )
					{
						ShellExecute( NULL, NULL, TEXT("http://cash.ndoors.com/"), NULL, NULL, SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
					{
						ShellExecute( NULL,NULL,TEXT("http://www.youxi.net.cn/AccountManage/AddCostIdx.aspx"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
					{
						ShellExecute( NULL,NULL,TEXT("http://www.newhljz.cn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan) )
					{
						//ShellExecute( NULL,NULL,TEXT("http://bill.goonzu.com"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
					{
						ShellExecute( NULL,NULL,TEXT("https://billing.ijji.com/payment/PaymentProcess.nhn"),NULL,NULL,SW_SHOWNORMAL );
					}
					else if ( pclClient->IsWhereServiceArea( ConstServiceArea_EUROPE ) )
					{
						ShellExecute( NULL,NULL,TEXT("http://billing.aeriagames.com/index.asp"),NULL,NULL,SW_SHOWNORMAL );
					}
					// PCK : �۷ι��� ��� NHNȸ���� NDIȸ���� ���� �������� ��ũ�������� �ٸ���. (09.03.30)
					else if( pclClient->IsWhereServiceArea(ConstServiceArea_English) )
					{
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nh") >= 0 )		// NHN ȸ���̸�
						{
							ShellExecute( NULL,NULL,TEXT("http://luminary.ijji.com/center/billing/transfer_gcoin_01.asp"),NULL,NULL,SW_SHOWNORMAL );
						}
						else							// NDI ȸ���̸�
						{
							ShellExecute( NULL,NULL,TEXT("https://billing.ndoorsgames.com/fillup/payment_option.asp?site_code=luminary"),NULL,NULL,SW_SHOWNORMAL );
						}
					}
				}
				break;
			}
		}
		break;
/*
	case NNEWITEMMALL_DIALOG_BUTTONFREEREPLENISHMENT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( g_bFullScreen)
						ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );

					// goonzu
					ShellExecute( NULL, NULL, "IEXPLORE.EXE", "http://goonzu.ndoors.com/center/mall/freecash.asp?menu=mall", NULL, SW_SHOWNORMAL );
				}
				break;
			}
		}
		break;
		*/
	}

}

void CNNewItemMallDlg::SetImageFontIndex(SI16 selectindex, bool select)
{
	if( select == true )
	{
		for(SI32 i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
		{
			if(i == selectindex)
				m_pISItem[i]->SetFontIndex(1);
			else
				m_pISItem[i]->SetFontIndex(0);
		}
	}
	else
	{
		m_pISItem[selectindex]->SetFontIndex(0);
	}
}

void CNNewItemMallDlg::LoadItemMallData(SI32 page, SI32 maxpage, cltItemMallData* pclinfo)
{
	if(pclinfo == NULL)
		return;

	InitItemMallData();

	m_siMaxPageNumber = maxpage;		// �Ѿ����� ������ ��
	m_siCurrentPageNumber = page;		// ������ ������ ��

	if(m_siMaxPageNumber == 1)
	{
		m_pButtonPrev->Enable(false);
		m_pButtonNext->Enable(false);
	}
	else
	{
		if(m_siMaxPageNumber == m_siCurrentPageNumber)
			m_pButtonNext->Enable(false);
		else
			m_pButtonNext->Enable(true);

		if(m_siCurrentPageNumber == 1)
			m_pButtonPrev->Enable(false);
		else
			m_pButtonPrev->Enable(true);
	}

	SI32 i;

	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		if(pclinfo[i].clItem[0].siUnique > 0)
		{
			bool bColorUsed = 	ConvertExplain(pclinfo[i].szExplain, 512 ) ? true : false;

			m_ItemDataList[i].Set(&pclinfo[i]);

			m_pISItem[i]->SetUseToolTip(pclinfo[i].szExplain,		bColorUsed);
			m_pISShowItem[i]->SetUseToolTip(pclinfo[i].szExplain,	bColorUsed);
		}
		else
		{
			m_pISItem[i]->SetUseToolTip(false);
			m_pISShowItem[i]->SetUseToolTip(false);
		}

	}

	ShowItemMallData();

	//m_dwLastClock = m_dwStratClock;

	/*	if( firstselect == 0)
	{
	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
	if(pclClient->pclItemMallManager->m_CharacterItemData[secondselect][i].siUnique[0] > 0)
	{
	m_ItemDataList[0][i].Set(pclClient->pclItemMallManager->m_CharacterItemData[secondselect][i]);
	m_siMaxPageNumber = 1;
	}
	}

	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
	if(pclClient->pclItemMallManager->m_CharacterItemData[secondselect][i + MAX_ITEMMALL_LIST_NUM].siUnique[0] > 0)
	{
	m_ItemDataList[1][i].Set(pclClient->pclItemMallManager->m_CharacterItemData[secondselect][i + MAX_ITEMMALL_LIST_NUM]);
	m_siMaxPageNumber = 2;
	}
	}

	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
	if(pclClient->pclItemMallManager->m_CharacterItemData[secondselect][i + (MAX_ITEMMALL_LIST_NUM*2)].siUnique[0] > 0)
	{
	m_ItemDataList[2][i].Set(pclClient->pclItemMallManager->m_CharacterItemData[secondselect][i + (MAX_ITEMMALL_LIST_NUM*2)]);
	m_siMaxPageNumber = 3;
	}
	}
	}
	*/
}

void CNNewItemMallDlg::ShowItemMallData()
{
	SI32 i;
	SI32 tempImageType = -1;
	SI32 tempImageIndex = 0;
	TCHAR tempBuffer[50] = { '\0' };

	for(i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		if( m_ItemDataList[i].clItem[0].siUnique > 0 )
		{
			// ������ �̹����� �Ѿ� ���� ���� �о���°� �ٲ��.
			// ���� ������ ��Ʈ �ε����� ���� �о� ���� ������ �ٲ۴�.
			// �׸��� �ٲ� ������ ��쿡 ���Ӱ� ��Ʈ �ε����� �־��ִ� �۾��� ������.
			TCHAR szImageFilename[256];
			SI32 siImageFontindex;
			pclClient->pclItemManager->GetItemImageItemmallFileName( m_ItemDataList[i].siImageFont, szImageFilename,sizeof(szImageFilename), &siImageFontindex);
			m_pISShowItem[i]->SetFileName( szImageFilename );
			m_pISShowItem[i]->SetFontIndex(siImageFontindex);
			TCHAR szNameFilename[256];
			SI32 siNameFontindex;
			pclClient->pclItemManager->GetItemNameItemmallFileName( m_ItemDataList[i].siImageFont, szNameFilename,sizeof(szNameFilename), &siNameFontindex);
			m_pISItemName[i]->SetFileName(szNameFilename);
			m_pISItemName[i]->SetFontIndex(siNameFontindex);

			g_JYLibFunction.SetNumUnit(m_ItemDataList[i].siPrice, tempBuffer, 50, NULL);
			m_pStaticItemPrice[i]->SetText(tempBuffer);

			if( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
			{
				g_JYLibFunction.SetNumUnit( m_ItemDataList[i].siEventPrice, tempBuffer, 50, NULL );
				m_pStaticItemEventPrice[i]->SetText( tempBuffer );
			}
			else
			{
				m_pStaticItemEventPrice[i]->SetText( "0" );
			}

			if( ConstServiceArea_China == pclClient->siServiceArea )
			{
				if( false == m_ItemDataList[i].bOnlyBuySilverCoin )
				{
					ShowSiverCoinMark( i, false );
				}
				else if( true == m_ItemDataList[i].bOnlyBuySilverCoin )
				{
					// ������ ������ �ȵǰ� �Ѵ�.
					ShowSiverCoinMark( i, true );
				}
			}
		}
		else
		{
			TCHAR szImageFilename[256];
			SI32 siImageFontindex;
			pclClient->pclItemManager->GetItemImageItemmallFileName( m_ItemDataList[i].siImageFont, szImageFilename,sizeof(szImageFilename), &siImageFontindex);
			m_pISShowItem[i]->SetFileName( szImageFilename );
			m_pISShowItem[i]->SetFontIndex(0);

			TCHAR szNameFilename[256];
			SI32 siNameFontindex;
			pclClient->pclItemManager->GetItemNameItemmallFileName( m_ItemDataList[i].siImageFont, szNameFilename,sizeof(szNameFilename), &siNameFontindex);
			m_pISItemName[i]->SetFileName(szNameFilename);
			m_pISItemName[i]->SetFontIndex(0);
			m_pStaticItemPrice[i]->SetText("");

			if( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
			{
				m_pStaticItemEventPrice[i]->SetText( "" );
			}
		}
	}

	if( m_pStaticCurrentPage1 != NULL && 
		m_pStaticMaxPage1 != NULL )
	{
		if(m_siMaxPageNumber >= 10)
		{
			m_pStaticMaxPage->SetFontIndex(m_siMaxPageNumber / 10);
			m_pStaticMaxPage1->Show(TRUE);
			m_pStaticMaxPage1->SetFontIndex(m_siMaxPageNumber % 10);
		}
		else
		{
			m_pStaticMaxPage1->Show(FALSE);
			m_pStaticMaxPage->SetFontIndex(m_siMaxPageNumber);
		}

		if(m_siCurrentPageNumber >= 10)
		{
			m_pStaticCurrentPage1->Show(TRUE);
			m_pStaticCurrentPage1->SetFontIndex(m_siCurrentPageNumber / 10);
			m_pStaticCurrentPage->SetFontIndex(m_siCurrentPageNumber % 10);
		}
		else
		{
			m_pStaticCurrentPage1->Show(FALSE);
			m_pStaticCurrentPage->SetFontIndex(m_siCurrentPageNumber);
		}
	}
	else
	{
		m_pStaticCurrentPage->SetFontIndex(m_siCurrentPageNumber);
		m_pStaticMaxPage->SetFontIndex(m_siMaxPageNumber);
	}
}

void CNNewItemMallDlg::SetCharDraw(SI16 index, SI16 unique)
{
	cltClient *pclclient = (cltClient *)pclClient;

	char* armcode = NULL; 

	cltItemManagerCommon* pclItemManager = pclClient->pclItemManager;

	clCharItem[index].siUnique = unique;
	SI32 tempunique = 0;

	// ����, ����
	if( clCharItem[PERSONITEM_HAT].siUnique )
	{
		tempunique = clCharItem[PERSONITEM_HAT].siUnique;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(tempunique);
	if(ref > 0 )
	{
		armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
	}
	else armcode = NULL;

	if(armcode)
	{
		strcpy(szHelmetCode,  armcode );
	}
	else
	{
		strcpy(szHelmetCode,  "KOR01");
	}


	// �Ǻ�, ����
	tempunique = 0;
	if( clCharItem[PERSONITEM_DRESS].siUnique )
	{
		tempunique = clCharItem[PERSONITEM_DRESS].siUnique;
	}

	ref = pclItemManager->FindItemRefFromUnique(tempunique);
	if(ref > 0 )
	{
		armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
	}
	else armcode = NULL;

	if(armcode)
	{
		strcpy(szArmourCode, armcode);
	}
	else
	{
		strcpy(szArmourCode, "KOR01");
	}

	// ����� ����~
	strcpy(szWeaponCode,  "KOR01");

	// ����
	tempunique = 0;
	if( clCharItem[PERSONITEM_MANTLE].siUnique )
	{
		tempunique = clCharItem[PERSONITEM_MANTLE].siUnique;
	}

	ref = pclItemManager->FindItemRefFromUnique(tempunique);
	if(ref > 0 )
	{
		armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
	}
	else armcode = NULL;

	if(armcode)
	{
		strcpy( szMantleCode,  armcode );
	}
	else
	{
		strcpy( szMantleCode,  "" );
	}

	SI32 delay = 0, instant = 0;

	if( m_bHaveHorse == false)
	{
		SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")) )
			tempKind = KIND_HERO1;

		m_pCharDraw->LoadItemCodeData(tempKind, szHelmetCode, szArmourCode,	szWeaponCode, szMantleCode, "", 0, NULL );

		m_siAniType = WT2U;

		delay	= m_pCharDraw->GetDelayTime(m_siAniType, tempKind);
		instant	= m_pCharDraw->GetFrameNum(m_siAniType) - 1 ;

	}
	else
	{
		SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")))
			tempKind = KIND_HERO1;

		m_pCharDraw->LoadItemCodeData( tempKind, szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, szHorseCode, 0, NULL );

		m_siAniType = WT2R;

		delay	= m_pCharDraw->GetDelayTime(m_siAniType, tempKind);
		instant	= m_pCharDraw->GetFrameNum(m_siAniType) - 1 ;
	}

	clAniInfo.SetAniDelay(delay);
	clAniInfo.SetAniInstant( instant );
}


void CNNewItemMallDlg::SetCharSetDraw( SI16 index1, SI16 index2, SI16 index3, SI32 unique1, SI32 unique2, SI32 unique3 )
{
	cltClient *pclclient = (cltClient *)pclClient;

	char* armcode = NULL; 

	cltItemManagerCommon* pclItemManager = pclClient->pclItemManager;
	
	// �׳� �ʱ�ȭ ���ش�.
//	clCharItem[PERSONITEM_HAT].siUnique = 0;
//	clCharItem[PERSONITEM_DRESS].siUnique = 0;
//	clCharItem[PERSONITEM_MANTLE].siUnique = 0;

	clCharItem[index1].siUnique = unique1;
	clCharItem[index2].siUnique = unique2;
	clCharItem[index3].siUnique = unique3;

	SI32 tempunique = 0;

	// ����, ����
	if( clCharItem[PERSONITEM_HAT].siUnique )
	{
		tempunique = clCharItem[PERSONITEM_HAT].siUnique;
	}

	SI32 ref = pclItemManager->FindItemRefFromUnique(tempunique);
	if(ref > 0 )
	{
		armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
	}
	else armcode = NULL;

	if(armcode)
	{
		strcpy(szHelmetCode,  armcode );
	}
	else
	{
		strcpy(szHelmetCode,  "KOR01");
	}


	// �Ǻ�, ����
	tempunique = 0;
	if( clCharItem[PERSONITEM_DRESS].siUnique )
	{
		tempunique = clCharItem[PERSONITEM_DRESS].siUnique;
	}

	ref = pclItemManager->FindItemRefFromUnique(tempunique);
	if(ref > 0 )
	{
		armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
	}
	else armcode = NULL;

	if(armcode)
	{
		strcpy(szArmourCode, armcode);
	}
	else
	{
		strcpy(szArmourCode, "KOR01");
	}

	// ����� ����~
	strcpy(szWeaponCode,  "KOR01");

	// ����
	tempunique = 0;
	if( clCharItem[PERSONITEM_MANTLE].siUnique )
	{
		tempunique = clCharItem[PERSONITEM_MANTLE].siUnique;
	}

	ref = pclItemManager->FindItemRefFromUnique(tempunique);
	if(ref > 0 )
	{
		armcode = pclItemManager->pclItemInfo[ref]->GetArmCode();
	}
	else armcode = NULL;

	if(armcode)
	{
		strcpy( szMantleCode,  armcode );
	}
	else
	{
		strcpy( szMantleCode,  "" );
	}

	SI32 delay = 0, instant = 0;

	if( m_bHaveHorse == false)
	{
		SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")))
			tempKind = KIND_HERO1;

		m_pCharDraw->LoadItemCodeData(tempKind, szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, "", 0, NULL );

		m_siAniType = WT2U;

		delay	= m_pCharDraw->GetDelayTime(m_siAniType, tempKind);
		instant	= m_pCharDraw->GetFrameNum(m_siAniType) - 1 ;

	}
	else
	{
		SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")))
			tempKind = KIND_HERO1;

		m_pCharDraw->LoadItemCodeData( tempKind, szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, szHorseCode, 0, NULL );

		m_siAniType = WT2R;

		delay	= m_pCharDraw->GetDelayTime(m_siAniType, tempKind);
		instant	= m_pCharDraw->GetFrameNum(m_siAniType) - 1 ;
	}

	clAniInfo.SetAniDelay(delay);
	clAniInfo.SetAniInstant( instant );
}

void CNNewItemMallDlg::SetHaveHorseDraw( SI16 type )
{
	SI32 delay = 0, instant = 0;

	if(type == 0)
	{
		SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")))
			tempKind = KIND_HERO1;

		m_bHaveHorse	= false;
		strcpy(szHorseCode, "");
		m_pCharDraw->LoadItemCodeData( tempKind, szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, NULL, 0, NULL );

		m_siAniType = WT2U;

		delay	= m_pCharDraw->GetDelayTime(m_siAniType, tempKind);
		instant	= m_pCharDraw->GetFrameNum(m_siAniType) - 1 ;
	}
	else
	{
		SI32 tempKind = pclClient->pclCM->CR[1]->GetKind();

		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_GOONZUMASTER")))
			tempKind = KIND_HERO1;

		m_bHaveHorse	= true;

		strcpy(szHorseCode, pclClient->pclHorseManager->pclHorseTypeInfo[type]->szCode);

		m_pCharDraw->LoadItemCodeData( tempKind, szHelmetCode,	szArmourCode,	szWeaponCode, szMantleCode, szHorseCode, 0, NULL );

		// ���� ���ؼ� �ִϸ��̼� ������ ����ǹǷ� �̸� �����ؾ� �Ѵ�. 
		m_siAniType = WT2R;

		delay	= m_pCharDraw->GetDelayTime(m_siAniType, tempKind);
		instant	= m_pCharDraw->GetFrameNum(m_siAniType) - 1 ;

		clAniInfo.SetAniDelay(delay);
		clAniInfo.SetAniInstant( instant );

		//	if ( cldate != NULL && ticket >= 0 )
		//	{
		//		pclCI->clHorseInfo.clHorse.clAddStatusDate.Set(cldate) ;
		//		pclCI->clHorseInfo.clHorse.siStatusTicketNum = ticket ;
		//	}
	}
}

void CNNewItemMallDlg::DrawDialogFirst(SI32 ScreenRX, SI32 ScreenRY )
{
	if(g_bFullScreen)
		GP.PutSpr( &m_SprBack , 80 + ScreenRX, 40 + ScreenRY,0 ); 				//m_SprBack.Pu  tSprT(80 + ScreenRX, 40 + ScreenRY, 0);
}

void CNNewItemMallDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	SI32	delay;
	SI32	framenumber;

	framenumber = m_pCharDraw->GetFrameNum(m_siAniType);

	if ( ((cltClient*)pclClient)->GetFrameDelay() == 25 )
	{
		delay		= clAniInfo.siAniDelay * 2;
	}
	else
	{
		delay		= clAniInfo.siAniDelay;
	}

	clAniInfo.IncreaseAniDelayor(1);

	if(clAniInfo.GetAniDelayor() >= delay)
	{
		clAniInfo.SetAniDelayor(0);


		clAniInfo.IncreaseAniStep(1);

		if(framenumber == 0)
		{
			clAniInfo.SetAniStep(0);
		}
		else
		{
			clAniInfo.SetAniStep(clAniInfo.GetAniStep() % framenumber );
		}
	}

	
	// ���⿡ ���� ��� ��ǥ ���� ( �ε巯�� ȸ���� ����)
 	SI32 offsetX = 0;
	SI32 offsetY = 0;
	
	switch(m_siAniDirection)
	{
	case CCD_SOUTH_EAST:	// �⺻
		offsetX = 0;
		offsetY = 0;
		break;
	case CCD_SOUTH:
		offsetX = 0;
		offsetY = 0;
		break;
	case CCD_SOUTH_WEST:
		offsetX = -5;
		offsetY = 0;
		break;
	case CCD_WEST:
		offsetX = -5;
		offsetY = 0;
		break;
	case CCD_NORTH_WEST:
		offsetX = -5;
		offsetY = 0;
		break;
	case CCD_NORTH:
		offsetX = 0;
		offsetY = 0;
		break;
	case CCD_NORTH_EAST:
		offsetX = 0;
		offsetY = 0;
		break;
	case CCD_EAST:
		offsetX = 3;
		offsetY = 0;
		break;
	}
	
	if( g_SoundOnOffDlg.m_bMaxWindowMode )
	{
		offsetX += 25;
		offsetY += 30;
	}


	//CCD_SOUTH_EAST, CCD_SOUTH, CCD_SOUTH_WEST, CCD_WEST, CCD_NORTH_WEST, CCD_NORTH, CCD_NORTH_EAST, CCD_EAST };

	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{
   		if( pclClient->pclCM->CR[1]->GetKind() == pclClient->GetUniqueFromHash( TEXT("KIND_HERO1") ) || pclClient->pclCM->CR[1]->GetKind() == KIND_HERO2)
			m_pCharDraw->DrawCharacterSelectMode(m_siAniType, (char)m_siAniDirection, clAniInfo.GetAniStep(), 82 + ScreenRX + offsetX, 135 + ScreenRY + offsetY);
		else
  			m_pCharDraw->DrawCharacterSelectMode(m_siAniType, (char)m_siAniDirection, clAniInfo.GetAniStep(), 82 + ScreenRX + offsetX, 125 + ScreenRY + offsetY);

	}
	else
	{
		//m_pCharDraw->DrawCharacterSelectMode(m_siAniType, m_siAniDirection, clAniInfo.GetAniStep(), 240 + ScreenRX, 375 + ScreenRY);
	}

	
}

void CNNewItemMallDlg::SelectItemList( SI16 SelectIndex )
{

	for(SI32 i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		if(i == SelectIndex)
		{
  			m_bSelect[SelectIndex] = !m_bSelect[SelectIndex];					// ���� or ���

			// �̸� ����� 3���� �����ǰ� �ִ�!!
			SI32 tempItemUnique1 = m_ItemDataList[SelectIndex].clItem[0].siUnique;
			SI32 tempItemUnique2 = m_ItemDataList[SelectIndex].clItem[1].siUnique;
			SI32 tempItemUnique3 = m_ItemDataList[SelectIndex].clItem[2].siUnique;

			
			SI16 itemtype1 = pclClient->pclItemManager->GetItemType(tempItemUnique1);
			SI16 itemtype2 = pclClient->pclItemManager->GetItemType(tempItemUnique2);
			SI16 itemtype3 = pclClient->pclItemManager->GetItemType(tempItemUnique3);

			// �ٸ� ������ �������� ���ԵǾ� �ִٸ� �̸����⸦ �������� �ʴ´�.
			if( ( tempItemUnique1 > 0 &&   CheckEquipAble(tempItemUnique1) == FALSE )
				||( tempItemUnique2 > 0 &&   CheckEquipAble(tempItemUnique2) == FALSE )
				||( tempItemUnique3 > 0 &&   CheckEquipAble(tempItemUnique3) == FALSE ) )
			{
				itemtype1 = itemtype2 = itemtype3 = 0;
			}


			SI16 index1 = 0;	// ù��° ������ ��ġ
			SI16 index2 = 0;	// �ι�° ������ ��ġ
			SI16 index3 = 0;	// ���ι�° ������ ��ġ
			SI16 index4 = 0;	// index1 + index2 + index3
			SI16 tempSelect = -1;	// ���������� ������ �������� ����

			if( itemtype1 == ITEMTYPE_DRESS || itemtype1 == ITEMTYPE_HAT || itemtype1 == ITEMTYPE_MANTLE || itemtype1 == ITEMTYPE_ANIMAL )
			{
				if( itemtype1 == ITEMTYPE_HAT )
				{
					index1 = PERSONITEM_HAT;
				}
				else if( itemtype1 == ITEMTYPE_DRESS )
				{
					index1 = PERSONITEM_DRESS;
				}
				else if( itemtype1 == ITEMTYPE_MANTLE )
				{
					index1 = PERSONITEM_MANTLE;
				}
				else if( itemtype1 == ITEMTYPE_ANIMAL )
				{
					// ���� ���⼭ ����
					index1 = 100;	//���� �׳� 100�̶� �Ѵ�(�̰� �˻�Ű���̴�)
					tempSelect = SELECT_ANIMAL;

					m_siSetItemLoad = 2;
				}

			}

			if( itemtype2 == ITEMTYPE_DRESS || itemtype2 == ITEMTYPE_HAT || itemtype2 == ITEMTYPE_MANTLE )
			{
				if( itemtype2 == ITEMTYPE_HAT )
				{
					index2 = PERSONITEM_HAT;
				}
				else if( itemtype2 == ITEMTYPE_DRESS )
				{
					index2 = PERSONITEM_DRESS;
				}
				else if( itemtype2 == ITEMTYPE_MANTLE )
				{
					index2 = PERSONITEM_MANTLE;
				}
			}

			if( itemtype3 == ITEMTYPE_DRESS || itemtype3 == ITEMTYPE_HAT || itemtype3 == ITEMTYPE_MANTLE )
			{
				if( itemtype3 == ITEMTYPE_HAT )
				{
					index3 = PERSONITEM_HAT;
				}
				else if( itemtype3 == ITEMTYPE_DRESS )
				{
					index3 = PERSONITEM_DRESS;
				}
				else if( itemtype3 == ITEMTYPE_MANTLE )
				{
					index3 = PERSONITEM_MANTLE;
				}
			}

			index4 = index1 + index2 + index3;
			// ���� �ϴ� ����!!
			if(index4 ==  PERSONITEM_HAT )
			{
				tempSelect = SELECT_HAT;	// ���� ����
				m_siSetItemLoad = 0;
			}
			else if(index4 ==  PERSONITEM_DRESS )
			{
				tempSelect = SELECT_DRESS;	// �Ǻ� ����
				m_siSetItemLoad = 0;
			}
			else if(index4 ==  PERSONITEM_MANTLE )
			{
				tempSelect = SELECT_MANTLE;	// ����� ����
				m_siSetItemLoad = 3;
			}
			else if(index4 ==  PERSONITEM_HAT + PERSONITEM_DRESS )
			{
				tempSelect = SELECT_SETITEM1;	// 2�� ��Ʈ���� �Ծ���
				m_siSetItemLoad = 1;
			}
			else if(index4 ==  PERSONITEM_HAT + PERSONITEM_DRESS + PERSONITEM_MANTLE )
			{
				tempSelect = SELECT_SETITEM2;	// 3�� ��Ʈ���� �Ծ���
				m_siSetItemLoad = 1;
			}
 
			if(tempSelect >= SELECT_HAT)
			{
				if(m_bSelect[i] == true)
				{
					// �ű� �߰�
					if( m_bPreviewSelect[ tempSelect ] == false )
					{
						// ��Ʈ �������� �ҷ��ö��� �Ǻ��̶� ���ڰ� ������ �����
						if( m_siSetItemLoad == 1 )
						{
							// 2�� ��Ʈ������
							if( index4 ==  PERSONITEM_HAT + PERSONITEM_DRESS )
							{
								SI16 FindIndex = m_pList->DelItem(PERSONITEM_HAT);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_HAT);
									// ���ڴ� 0��
									m_bPreviewSelect[ SELECT_HAT ] = false;
								}

								FindIndex = m_pList->DelItem(PERSONITEM_DRESS);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_DRESS);
									// �Ǻ��� 1��
									m_bPreviewSelect[ SELECT_DRESS ] = false;
								}

								FindIndex = m_pList->DelItem(PERSONITEM_HAT + PERSONITEM_DRESS + PERSONITEM_MANTLE);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_HAT);
									InitSelectEquipItemData(PERSONITEM_DRESS);
									InitSelectEquipItemData(PERSONITEM_MANTLE);
									// 3�� ��Ʈ������ 
									m_bPreviewSelect[ SELECT_SETITEM2 ] = false;
								}
							}
							// 3�� ��Ʈ������
							else if( index4 ==  PERSONITEM_HAT + PERSONITEM_DRESS + PERSONITEM_MANTLE )
							{
								SI16 FindIndex = m_pList->DelItem(PERSONITEM_HAT);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_HAT);
									// ���ڴ� 0��
									m_bPreviewSelect[ SELECT_HAT ] = false;
								}

								FindIndex = m_pList->DelItem(PERSONITEM_DRESS);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_DRESS);
									// �Ǻ��� 1��
									m_bPreviewSelect[ SELECT_DRESS ] = false;
								}

								FindIndex = m_pList->DelItem(PERSONITEM_MANTLE);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_MANTLE);
									// ����� 2��
									m_bPreviewSelect[ SELECT_MANTLE ] = false;
								}

								FindIndex = m_pList->DelItem(PERSONITEM_HAT + PERSONITEM_DRESS);
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_HAT);
									InitSelectEquipItemData(PERSONITEM_DRESS);
									// ���ڴ� 0��
									m_bPreviewSelect[ SELECT_SETITEM1 ] = false;
								}
							}
						}
						else if( m_siSetItemLoad == 0 )	// ���ڳ� �Ǻ��� �ҷ����µ� ��Ʈ �������� ������ �����!!
						{
							SI16 FindIndex = m_pList->DelItem(PERSONITEM_HAT + PERSONITEM_DRESS);
							if( FindIndex >= 0)
							{
								m_pList->Refresh();

								vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

								SI32 j;
								for( j = 0; j < FindIndex; ++j)
									vecIter++;

								m_vecSelectItemMall.erase(vecIter);
								InitSelectEquipItemData(PERSONITEM_HAT);
								InitSelectEquipItemData(PERSONITEM_DRESS);
								// 2�� ��Ʈ�� 
								m_bPreviewSelect[ SELECT_SETITEM1 ] = false;
							}
							else
							{
								SI16 FindIndex = m_pList->DelItem( PERSONITEM_HAT + PERSONITEM_DRESS + PERSONITEM_MANTLE );
								if( FindIndex >= 0)
								{
									m_pList->Refresh();

									vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

									SI32 j;
									for( j = 0; j < FindIndex; ++j)
										vecIter++;

									m_vecSelectItemMall.erase(vecIter);
									InitSelectEquipItemData(PERSONITEM_HAT);
									InitSelectEquipItemData(PERSONITEM_DRESS);
									InitSelectEquipItemData(PERSONITEM_MANTLE);
									// 3�� ��Ʈ�� 
									m_bPreviewSelect[ SELECT_SETITEM2 ] = false;
								}
							}
						}
						else if( m_siSetItemLoad == 3 )	// ���並 �ҷ����µ� ��Ʈ �������� ������ �����!!
						{
							SI16 FindIndex = m_pList->DelItem( PERSONITEM_HAT + PERSONITEM_DRESS + PERSONITEM_MANTLE );
							if( FindIndex >= 0)
							{
								m_pList->Refresh();

								vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

								SI32 j;
								for( j = 0; j < FindIndex; ++j)
									vecIter++;

								m_vecSelectItemMall.erase(vecIter);
								InitSelectEquipItemData(PERSONITEM_HAT);
								InitSelectEquipItemData(PERSONITEM_DRESS);
								InitSelectEquipItemData(PERSONITEM_MANTLE);
								// ��Ʈ�� 4��
								m_bPreviewSelect[ SELECT_SETITEM2 ] = false;
							}
						}

						stListBoxItemData Data;

						Data.siParam[0] = index4;
						Data.Set( 0, m_ItemDataList[i].szName);

						m_pList->AddItem( &Data );
						m_pList->Refresh();

						// �̸�����â�� �߰�
						m_vecSelectItemMall.push_back(m_ItemDataList[i]);
					}
					// �ٲٱ�
					else
					{
						stListBoxItemData Data;
						Data.siParam[0] = index4;
						Data.Set( 0, m_ItemDataList[i].szName);
						SI16 FindIndex = m_pList->ChangeItem( &Data, index4 );
						m_pList->Refresh();
						m_vecSelectItemMall[FindIndex].Set(&m_ItemDataList[i]);
					}

					if(tempSelect == SELECT_HAT || tempSelect == SELECT_DRESS || tempSelect == SELECT_MANTLE || tempSelect == SELECT_SETITEM1 || tempSelect == SELECT_SETITEM2)
						SetCharSetDraw(index1, index2, index3, tempItemUnique1, tempItemUnique2, tempItemUnique3);
					else
					{
						SI32 HorseType = 0;
//						����������ũ üũ �غ�����
						if(tempItemUnique1 == ITEMUNIQUE(5600))				// ������
							HorseType = 1;
						else if(tempItemUnique1 == ITEMUNIQUE(5503))		// �渶
							HorseType = 2;
						else if(tempItemUnique1 == ITEMUNIQUE(5506))		// �鸶
							HorseType = 3;
						else if(tempItemUnique1 == ITEMUNIQUE(5510))	// ���丶
							HorseType = 4;
						else if(tempItemUnique1 == ITEMUNIQUE(5520))		// ��踶 
							HorseType = 5;
						else if(tempItemUnique1 == ITEMUNIQUE(5530))		// �絹��
							HorseType = 6;
						else if(tempItemUnique1 == ITEMUNIQUE(5540))		// õ����
							HorseType = 7;
						else if(tempItemUnique1 == ITEMUNIQUE(5550))	// ö�⸶
							HorseType = 8;
						else if(tempItemUnique1 == ITEMUNIQUE(5560))	// ȣ����
							HorseType = 9;
						else if(tempItemUnique1 == ITEMUNIQUE(5570))	// ��������
							HorseType = 10;
						else if(tempItemUnique1 == ITEMUNIQUE(5580))	// ����ȣ
							HorseType = 11;

						SetHaveHorseDraw(HorseType);
					}

					SI32 j;
					for(  j=0; j < m_pList->GetListNum(); ++j)
					{
						m_pButtonSmallBuy[j]->Show( true );
						m_pButtonSmallNo[j]->Show( true );
					}

					for( j = m_pList->GetListNum(); j < 4; ++j)
					{
						m_pButtonSmallBuy[j]->Show( false );
						m_pButtonSmallNo[j]->Show( false );
					}

					m_bPreviewSelect[ tempSelect ] = true;
				}
				else
				{
					if(tempSelect == SELECT_HAT || tempSelect == SELECT_DRESS || tempSelect == SELECT_MANTLE || tempSelect == SELECT_SETITEM1 || tempSelect == SELECT_SETITEM2)
						SetCharSetDraw(index1, index2, index3, 0, 0, 0);
					else
						SetHaveHorseDraw(0);

					SI16 FindIndex = m_pList->DelItem( index4 );
					m_pList->Refresh();

					// �̸� ����â���� �����
					vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

					SI32 j;
					for( j = 0; j < FindIndex; ++j)
						vecIter++;

					m_vecSelectItemMall.erase(vecIter);
					m_bPreviewSelect[ tempSelect ] = false;

					for( j=3; j >= m_pList->GetListNum(); --j)
					{
						m_pButtonSmallBuy[j]->Show( false );
						m_pButtonSmallNo[j]->Show( false );
					}
				}
			}
		}
		else
			m_bSelect[i] = false;
	}

	SetImageFontIndex(SelectIndex, m_bSelect[SelectIndex]);
}

void CNNewItemMallDlg::ShowBuyWindow(SI16 index)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if( pclClient->IsCountrySwitch(Switch_GlobalItemMall_Limit) )
		{
			// ������ �������� ������ ������ �������� ���� �� �� �ִ�.
			SYSTEMTIME stStart, stEnd;
			memcpy(&stStart, &pclClient->pclCM->CR[1]->pclCI->clBI.siFirstCreatTimeDateVary, sizeof(SYSTEMTIME));
			TimeAddDate(&stStart, 7, &stEnd);
			if( TimeInBounds(&pclclient->sTime, &stStart, &stEnd) == true )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8891 ) );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			SI32 siMaxLimitLevel = 0;
			for(SI32 i=0; i<MAX_ITEMMALL_SET_NUM; i++)
			{
				if( siMaxLimitLevel < m_ItemDataList[index].siLimitLevel[i] )
				{
					siMaxLimitLevel = m_ItemDataList[index].siLimitLevel[i];
				}
			}

			// Ư�� ���� �̻��� �Ǿ�� �������� ������ �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < siMaxLimitLevel )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8889 ) );

				kText.Replace("#level#", SI32ToString(siMaxLimitLevel));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			// �⺻������ ���� 30�̻��� �Ǿ�� �������� ������ �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 30 )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8889 ) );

				kText.Replace("#level#", SI32ToString((SI32)30));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}
		}
	}

	// �����ϱ�â�� �� ������ ��������
	if( pclclient->m_pDialog[ NPRESENTWINDOW_DLG ] )
		pclclient->m_pDialog[ NPRESENTWINDOW_DLG ]->DeleteDialog();

	if( pclclient->IsWhereServiceArea(ConstServiceArea_NHNChina) )
	{
		if( m_ItemDataList[index].siEventPrice > m_siEventCash )
		{
			TCHAR * pTitle = GetTxtFromMgr( 4314 );
			TCHAR * pBody  = GetTxtFromMgr( 7548 );

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pBody );
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

			return;
		}
	}

	if( pclclient->IsWhereServiceArea(ConstServiceArea_China) )
	{
		if( m_ItemDataList[index].bOnlyBuySilverCoin )
		{
			// ����������
			if( m_ItemDataList[index].siPrice > m_siEventCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}
		}
		else
		{
			// ����������
			if( m_ItemDataList[index].siPrice > m_siRealCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}
		}
	}

	if(m_ItemDataList[index].clItem[0].siUnique > 0)
	{
		if( pclclient->m_pDialog[ NBUYWINDOW_DLG ] == NULL )
		{
			pclclient->CreateInterface( NBUYWINDOW_DLG );	

			if(m_ItemDataList[index].siGroupKey > 1)
			{
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetBuyFirstItemInfo(m_ItemDataList[index].szExplain, m_ItemDataList[index].szExplain2, m_ItemDataList[index].szName, m_ItemDataList[index].siImageFont);
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetCashInfo( m_siRealCash, m_siEventCash, m_ItemDataList[index].bOnlyBuySilverCoin );
				cltGameMsgRequest_Get_ItemMall_ItemInfo pclinfo(m_ItemDataList[index].siGroupKey);
				cltMsg clMsg( GAMEMSG_REQUEST_GET_ITEMMALL_ITEMINFO, sizeof( pclinfo ), (BYTE*)&pclinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
			{
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetBuyItemInfo( m_ItemDataList[index] );
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetCashInfo( m_siRealCash, m_siEventCash, m_ItemDataList[index].bOnlyBuySilverCoin );
			}
		}
	}
	else		// �غ����� �������̶��
	{
		TCHAR* pBody  = GetTxtFromMgr( 6077 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pBody );
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
	}
}

void CNNewItemMallDlg::ShowSmallBuyWindow(SI16 index)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if( pclClient->IsCountrySwitch(Switch_GlobalItemMall_Limit) )
		{
			// ������ �������� ������ ������ �������� ���� �� �� �ִ�.
			SYSTEMTIME stStart, stEnd;
			memcpy(&stStart, &pclClient->pclCM->CR[1]->pclCI->clBI.siFirstCreatTimeDateVary, sizeof(SYSTEMTIME));
			TimeAddDate(&stStart, 7, &stEnd);
			if( TimeInBounds(&pclclient->sTime, &stStart, &stEnd) == true )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8891 ) );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			SI32 siMaxLimitLevel = 0;
			for(SI32 i=0; i<MAX_ITEMMALL_SET_NUM; i++)
			{
				if( siMaxLimitLevel < m_ItemDataList[index].siLimitLevel[i] )
				{
					siMaxLimitLevel = m_ItemDataList[index].siLimitLevel[i];
				}
			}

			// Ư�� ���� �̻��� �Ǿ�� �������� ������ �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < siMaxLimitLevel )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8889 ) );

				kText.Replace("#level#", SI32ToString(siMaxLimitLevel));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			// �⺻������ ���� 30�̻��� �Ǿ�� �������� ������ �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 30 )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8889 ) );

				kText.Replace("#level#", SI32ToString((SI32)30));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}
		}
	}

	// �����ϱ�â�� �� ������ ��������
	if( pclclient->m_pDialog[ NPRESENTWINDOW_DLG ] )
		pclclient->m_pDialog[ NPRESENTWINDOW_DLG ]->DeleteDialog();
	
	if( pclclient->IsWhereServiceArea(ConstServiceArea_China) )
	{
		if( 0 < m_ItemDataList[index].siEventPrice )
		{
			if( m_ItemDataList[index].siPrice > m_siRealCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}

			// ����������
			if( m_ItemDataList[index].siEventPrice > m_siEventCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}
		}
		else
		{
			// ����������
			if( m_ItemDataList[index].siPrice > m_siRealCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}
		}
	}


	if(m_vecSelectItemMall[index].clItem[0].siUnique > 0)
	{
		if( pclclient->m_pDialog[ NBUYWINDOW_DLG ] == NULL )
		{
			pclclient->CreateInterface( NBUYWINDOW_DLG );	

			if(m_vecSelectItemMall[index].siGroupKey > 1)
			{
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetBuyFirstItemInfo(m_vecSelectItemMall[index].szExplain, m_vecSelectItemMall[index].szExplain2, m_vecSelectItemMall[index].szName, m_vecSelectItemMall[index].siImageFont);
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetCashInfo( m_siRealCash, m_siEventCash, m_vecSelectItemMall[index].bOnlyBuySilverCoin );

				cltGameMsgRequest_Get_ItemMall_ItemInfo pclinfo(m_vecSelectItemMall[index].siGroupKey);
				cltMsg clMsg( GAMEMSG_REQUEST_GET_ITEMMALL_ITEMINFO, sizeof( pclinfo ), (BYTE*)&pclinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
			{
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetBuyItemInfo(m_vecSelectItemMall[index]);
				((CNBuyWindowDlg*)pclclient->m_pDialog[ NBUYWINDOW_DLG ])->SetCashInfo( m_siRealCash, m_siEventCash, m_vecSelectItemMall[index].bOnlyBuySilverCoin );
			}
		}
	}
	
}

// �����ϱ�â ����
void CNNewItemMallDlg::ShowPresentWindow(SI16 index)
{
   	cltClient *pclclient = (cltClient *)pclClient;

	if( pclClient->siServiceArea == ConstServiceArea_English )
	{
		if( pclClient->IsCountrySwitch(Switch_GlobalItemMall_Limit) )
		{
			// ������ �������� ������ ������ �������� ���� �� �� �ִ�.
			SYSTEMTIME stStart, stEnd;
			memcpy(&stStart, &pclClient->pclCM->CR[1]->pclCI->clBI.siFirstCreatTimeDateVary, sizeof(SYSTEMTIME));
			TimeAddDate(&stStart, 7, &stEnd);
			if( TimeInBounds(&pclclient->sTime, &stStart, &stEnd) == true )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8891 ) );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			SI32 siMaxLimitLevel = 0;
			for(SI32 i=0; i<MAX_ITEMMALL_SET_NUM; i++)
			{
				if( siMaxLimitLevel < m_ItemDataList[index].siLimitLevel[i] )
				{
					siMaxLimitLevel = m_ItemDataList[index].siLimitLevel[i];
				}
			}

			// Ư�� ���� �̻��� �Ǿ�� �������� ������ �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < siMaxLimitLevel )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8889 ) );

				kText.Replace("#level#", SI32ToString(siMaxLimitLevel));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}

			// �⺻������ ���� 30�̻��� �Ǿ�� �������� ������ �� �ִ�.
			if( pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < 30 )
			{
				NTCHARString128	kTitle( GetTxtFromMgr( 5614 ) );
				NTCHARString128	 kText( GetTxtFromMgr( 8889 ) );

				kText.Replace("#level#", SI32ToString((SI32)30));

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(kTitle, kText );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
				return;
			}
		}
	}


//////////////////////////////////////////////

	//����
	if( pclClient->siServiceArea == ConstServiceArea_NHNChina ||
		pclClient->siServiceArea == ConstServiceArea_Japan ||
		pclClient->siServiceArea == ConstServiceArea_Korea ||
		pclClient->siServiceArea == ConstServiceArea_USA	||
		pclClient->siServiceArea == ConstServiceArea_EUROPE)	//[����] �����ϱ� USA �߰� => 2008-7-24
	{
	}
	else
	{
		TCHAR* pBody  = GetTxtFromMgr( 6063 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pBody );
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

 		return;
	}

	if( pclclient->IsWhereServiceArea(ConstServiceArea_China) )
	{
		if( m_ItemDataList[index].bOnlyBuySilverCoin )
		{
			// ����������
			if( m_ItemDataList[index].siPrice > m_siEventCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}
		}
		else
		{
			// ����������
			if( m_ItemDataList[index].siPrice > m_siRealCash )
			{
				TCHAR * pTitle = GetTxtFromMgr( 4314 );
				TCHAR * pBody  = GetTxtFromMgr( 7548 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pBody );
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

				return;
			}
		}
	}


	if( true == m_ItemDataList[index].bOnlyBuySilverCoin )
	{
		TCHAR* pTitle = GetTxtFromMgr( 6071 );
		TCHAR* pText  = GetTxtFromMgr( 10055 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);

		return;
	}

	// ����â�� �� ������ ��������
	if( pclclient->m_pDialog[ NBUYWINDOW_DLG ] )
		pclclient->m_pDialog[ NBUYWINDOW_DLG ]->DeleteDialog();

	if(m_ItemDataList[index].clItem[0].siUnique > 0)
	{
		if( pclclient->m_pDialog[ NPRESENTWINDOW_DLG ] == NULL )
		{
			pclclient->CreateInterface( NPRESENTWINDOW_DLG );	
 
			if(m_ItemDataList[index].siGroupKey > 1)
			{
				((CNPresentWindowDlg*)pclclient->m_pDialog[ NPRESENTWINDOW_DLG ])->SetPresentFirstItemInfo(m_ItemDataList[index].szName, m_ItemDataList[index].szExplain, m_ItemDataList[index].szExplain2, m_ItemDataList[index].siImageFont, m_siRealCash, m_siEventCash);
				cltGameMsgRequest_Get_ItemMall_ItemInfo pclinfo(m_ItemDataList[index].siGroupKey);
				cltMsg clMsg( GAMEMSG_REQUEST_GET_ITEMMALL_ITEMINFO, sizeof( pclinfo ), (BYTE*)&pclinfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			else
			{
				((CNPresentWindowDlg*)pclclient->m_pDialog[ NPRESENTWINDOW_DLG ])->SetPresentItemInfo(m_ItemDataList[index], m_siRealCash, m_siEventCash);
			}
		}
	}
}

// �̸� ���⿡�� ���õ� �ε����� �����
void CNNewItemMallDlg::DeleteItemList(SI16 index)
{
	if(m_pList == NULL)
		return;

	stListBoxItemData tempListBoxItemData(m_pList->GetItemData(index));
	SI16 tempKey = tempListBoxItemData.siParam[0];

	// ������ ��Ʈ �������� 2�����̴�
	if( tempKey == PERSONITEM_HAT + PERSONITEM_DRESS )
		SetCharSetDraw(PERSONITEM_HAT, PERSONITEM_DRESS, 0, 0, 0, 0);
	else if( tempKey == PERSONITEM_HAT + PERSONITEM_DRESS + PERSONITEM_MANTLE )
		SetCharSetDraw(PERSONITEM_HAT, PERSONITEM_DRESS, PERSONITEM_MANTLE, 0, 0, 0);
	// �Ǻ�, ����, ���� ���!!
	else if( tempKey == PERSONITEM_HAT || tempKey == PERSONITEM_DRESS ||  tempKey == PERSONITEM_MANTLE )
		SetCharDraw(tempKey, 0);
	// ���� ���Ƿ� 100�̶� Ű���� ���!!
	else if(tempKey == 100)
		SetHaveHorseDraw(0);

	SI16 i;
	for( i = 0; i < MAX_ITEMMALL_LIST_NUM; ++i)
	{
		m_bSelect[i] = false;
		m_pISItem[i]->SetFontIndex(0);
	}

	for( i = 0; i < SELECT_ITEM_NUM; ++i)
	{
		m_bPreviewSelect[ i ] = false;
	}

	SI16 FindIndex = m_pList->DelItem(tempKey);
	m_pList->Refresh();

	vecIterSelectItemMall vecIter = m_vecSelectItemMall.begin();

	for( i = 0; i < FindIndex; ++i)
		vecIter++;

	m_vecSelectItemMall.erase(vecIter);

	for( i = 3; i >= m_pList->GetListNum(); --i)
	{
		m_pButtonSmallBuy[i]->Show( false );
		m_pButtonSmallNo[i]->Show( false );
	}
}

// �� ĳ�� ������ �����ش�
void CNNewItemMallDlg::SetMyCashInfo(SI32 realcash, SI32 eventcash)
{
	TCHAR tempBuffer[100] = {'\0'};

	m_siRealCash = realcash;
	m_siEventCash = eventcash;

	//TCHAR* pText = GetTxtFromMgr(5944);	// "��"
	g_JYLibFunction.SetNumUnit(realcash, tempBuffer,100, NULL);

	//StringCchPrintf(tempBuffer,100,TEXT("%d"),realcash);
	m_pStaticMyNcoin->SetText(tempBuffer);

	if(pclClient->IsWhereServiceArea( ConstServiceArea_China | ConstServiceArea_Korea | ConstServiceArea_NHNChina))
	{
		g_JYLibFunction.SetNumUnit(eventcash, tempBuffer,100, NULL);
		m_pStaticEvenyMoney->SetText(tempBuffer);
	}
}

// �ű�, �Ŀ�, ����� �ҷ����� code�� �����Ѵ�
void CNNewItemMallDlg::SelectThreeMenu(char* code)
{
	m_dwStratClock	= pclClient->CurrentClock;

	//if(  m_dwStratClock - m_dwLastClock < DELAYTIME )
	//	return;

	m_pButtonSubject1->Show(false);
	m_pButtonSubject2->Show(false);
	m_pButtonSubject3->Show(false);
	m_pButtonSubject4->Show(false);
	m_pButtonSubject5->Show(false);

	m_pButtonCharacter->SetFontIndex(1);
	m_pButtonPower->SetFontIndex(3);
	m_pButtonSummonHorse->SetFontIndex(5);
	m_pButtonMaterialFood->SetFontIndex(7);
//	m_pButtonBeginner->SetFontIndex(9);

	cltClient *pclclient = (cltClient *)pclClient;
	m_siFirstMenuSelect = 0;
	m_siSecondMenuSelect = 0;
	strcpy(m_szType, code);
	cltGameMsgRequest_Get_Sel_PGList pclinfo(m_siFirstMenuSelect, m_siSecondMenuSelect, m_szType, 1);
	cltMsg clMsg( GAMEMSG_REQUEST_GET_SEL_PGLIST, sizeof( pclinfo ), (BYTE*)&pclinfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

}

BOOL CNNewItemMallDlg::ConvertExplain(TCHAR* pText , SI32 size)
{ 
	TCHAR	tempBuffer[512] = {'\0'};
	TCHAR	returnBuffer[512] = {'\0'};
	BOOL	bColorUse = FALSE;

	StringCchCopy(tempBuffer,512, pText);

	SI32 cnt = 0;

	SI32 i;
	SI32 tempBufferLen = _tcslen(tempBuffer);
	

	for( i = 0; i <= tempBufferLen; ++i ) 
	{
		if( tempBuffer[cnt] == '<'		// <C08> �±� üũ
			&& cnt + 4 <= tempBufferLen	// <C08> �� �ڿ� ���� ������ �ǳ� üũ 
			&& ( tempBuffer[cnt+1] == 'c'  || tempBuffer[cnt+1] == 'C' )			
			&& tempBuffer[cnt+4] == '>'
			)
		{
			bColorUse = TRUE;
		}


		if( tempBuffer[cnt] == '<'		// <br> �±� üũ
			&& cnt + 3 <= tempBufferLen	// <br> �� �ڿ� ���� ������ �ǳ� üũ 
			&& ( tempBuffer[cnt+1] == 'b'  || tempBuffer[cnt+1] == 'B' )
			&& ( tempBuffer[cnt+2] == 'r'  || tempBuffer[cnt+2] == 'R' )
			&& tempBuffer[cnt+3] == '>'
			)
		{            
			returnBuffer[i] = '\r';
			returnBuffer[i+1] = ' ';
			cnt += 4;
			i++;
		}/*
		else if(  tempBuffer[cnt] == '<'		// </C> �±� üũ
			&& cnt + 3 <= tempBufferLen	// </C> �� �ڿ� ���� ������ �ǳ� üũ 
			&& ( tempBuffer[cnt+1] == '/')
			&& ( tempBuffer[cnt+2] == 'r'  || tempBuffer[cnt+2] == 'R' )
			&& tempBuffer[cnt+4] == '>'
			)
		{
			returnBuffer[i] = '\r'



			cnt+=5;
		}*/
		else
		{
			returnBuffer[i] = tempBuffer[cnt];
			cnt++;
		}		
	}

	StringCchCopy(pText, size, returnBuffer);

	return bColorUse;
}

/*
void CNNewItemMallDlg::SetBillPageHandle( HWND* hWnd )
{
	m_BillPageHandle = *hWnd;
	m_bBillPageOpend = TRUE;
}*/


void CNNewItemMallDlg::Action()
{
	cltClient* pclclient = (cltClient*)pclClient;


	if(m_bBillPageActive == FALSE && bActiveSwitch == true)	//  �������� ��Ȱ��ȭ�϶� Ŭ���̾�Ʈ�� Ȱ��ȭ�Ǹ�
	{
		// �ڽ��� ĳ������ ��û		
		cltMsg clMsg2( GAMEMSG_REQUEST_GET_MYCASH_INFO, 0, NULL );
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg2);

		m_bBillPageActive = TRUE;
	}
	else if ( bActiveSwitch == FALSE)
	{
		m_bBillPageActive = FALSE;
	}
	

}

BOOL CNNewItemMallDlg::CheckEquipAble(SI32 siItemUnique)
{
	if( siItemUnique <= 0 )
		return FALSE;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(siItemUnique);

	cltItemConition * pclCondition = &pclClient->pclItemManager->pclItemInfo[ref]->clCondition;

	// ���ڸ� ���� ����
	if( pclCondition->bForMan == true && pclCondition->bForWoman == false  )
	{
		SI32 kind = pclClient->pclCM->CR[1]->GetKind();
		if( kind == KIND_HEROINE1 ||  kind == KIND_HEROINE2 || kind == KIND_HEROINE3 || kind == KIND_HEROINE4 )
		{
			return FALSE;	
		}							
	}
	// ���ڸ� ���� ����
	else if( pclCondition->bForMan == false && pclCondition->bForWoman == true )
	{
		SI32 kind = pclClient->pclCM->CR[1]->GetKind();
		if( kind == KIND_HERO1 ||  kind == KIND_HERO2 || kind == KIND_HERO3 || kind == KIND_HERO4 )
		{
			return FALSE;	
		}
	}

	return TRUE;
}

void CNNewItemMallDlg::ShowSiverCoinMark( SI32 Index, bool Flag )
{
	m_pStaticSilverCoin[Index]->Show( Flag );
}

void CNNewItemMallDlg::NHNChinaInit( void )
{
	// �߱��̸� �ش� �������̽����� �׳� ���̰� �д�. �����ϱ� ��ư�� ��Ȱ��.
	if( pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )	
	{
		for( int iCount = 0; iCount < 6; ++iCount )
			m_pButtonPresent[iCount]->Enable( false ); 
		
		return;
	}
	
	m_pButtonGoldItemMall->Show( false );
	m_pButtonEventItemMall->Show( false );
	m_pButtonEventItemMallExplain->Show( false );
	m_pButtonEventMoneyInfo->Show( false );

	
	
	for(int iCount = 0; iCount < 6; ++iCount )
		m_pStaticItemEventPrice[iCount]->Show( false );
}

void CNNewItemMallDlg::NHNChinaGoldItemMall( void )
{
	if( false == pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )	return;

	m_pButtonNewItem->Show( true );
	m_pButtonPopularItem->Show( true );
	m_pButtonSpecialItem->Show( true );

	m_pButtonEventItemMallExplain->Show( false );

	for( int iCount = 0; iCount < 6; ++iCount )
	{
		m_pStaticItemPrice[iCount]->SetY( m_pStaticItemPrice[iCount]->GetY() + (iCount < 3 ? +5 : +12) );      
		m_pStaticItemEventPrice[iCount]->Show( false );
	}
	
	SetFontIndex( 0 );
}

void CNNewItemMallDlg::NHNChinaEventItemMall( void )
{
	if( false == pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina) )	return;

	m_pButtonNewItem->Show( false );
	m_pButtonPopularItem->Show( false );
	m_pButtonSpecialItem->Show( false );

	m_pButtonEventItemMallExplain->Show( true );
	
	for( int iCount = 0; iCount < 6; ++iCount )
	{
		m_pStaticItemPrice[iCount]->SetY( m_pStaticItemPrice[iCount]->GetY() + (iCount < 3 ? -5 : -12) );       
		m_pStaticItemEventPrice[iCount]->Show( true );
	} 

	SetFontIndex( 1 );
}

void CNNewItemMallDlg::NHNChinaShowCategory( bool	_bShow )
{
	m_pButtonCharacter->Show(_bShow);
	m_pButtonPower->Show(_bShow);
	m_pButtonSummonHorse->Show(_bShow);
	m_pButtonMaterialFood->Show(_bShow);
	m_pButtonSubject1->Show(_bShow);
	m_pButtonSubject2->Show(_bShow);
	m_pButtonSubject3->Show(_bShow);
	m_pButtonSubject4->Show(_bShow);
	m_pButtonSubject5->Show(_bShow); 

	if( _bShow )
	{
		m_pButtonCharacter->SetFontIndex(0);
		m_pButtonPower->SetFontIndex(3);		// �Ŀ�
		m_pButtonSummonHorse->SetFontIndex(5);	// ��ȯ/��
		m_pButtonMaterialFood->SetFontIndex(7);	// ���/����
		m_pButtonBeginner->SetFontIndex(9);			// �ʺ���
		m_pButtonSubject1->SetFontIndex(0);
		m_pButtonSubject2->SetFontIndex(3);
		m_pButtonSubject3->SetFontIndex(5);
		m_pButtonSubject4->SetFontIndex(7);
		m_pButtonSubject5->SetFontIndex(39);
	}
}