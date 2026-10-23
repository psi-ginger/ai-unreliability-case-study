#include "NEquipmentCreateDlg.h"

#include "../../Client.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../../CommonLogic/MsgType-Item.h"

extern cltCommonLogic* pclClient;

CNEquipmentCreateDlg::CNEquipmentCreateDlg()
{
	m_siSelectedIndex = 0;
	m_siWeaponUnique = 0;
	m_siHelmetUnique = 0;
	m_siArmorUnique = 0;
	m_siBeltUnique = 0;
	m_siShoeUnique = 0;
	m_siNeckUnique = 0;
	m_siRingUnique = 0;
	m_siMantleUnique = 0;

	m_siWeaponProPerty = 0;
	m_siHelmetProPerty = 0;
	m_siArmorProPerty = 0;
	m_siBeltProPerty = 0;
	m_siShoeProPerty = 0;

	m_siWeaponEnchant = 0;
	m_siHelmetEnchant = 0;
	m_siArmorEnchant = 0;
	m_siBeltEnchant = 0;
	m_siShoeEnchant = 0;

	m_siWeaponSlot = 0;
	m_siHelmetSlot = 0;
	m_siArmorSlot = 0;
	m_siBeltSlot = 0;
	m_siShoeSlot = 0;

	m_bCheckWeapon = false;
	m_bCheckHelmet = false;
	m_bCheckArmor = false;
	m_bCheckBelt = false;
	m_bCheckShoe = false;
	m_bCheckNeck = false;
	m_bCheckRing = false;
	m_bCheckMantle = false;

	ZeroMemory(m_siaArrItemTypeIndex, sizeof(m_siaArrItemTypeIndex));
}

CNEquipmentCreateDlg::~CNEquipmentCreateDlg()
{

}

void CNEquipmentCreateDlg::Init()
{

}

void CNEquipmentCreateDlg::Create()
{
	if (IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT ( "NInterface/Data/NBalanceTestGMDlg/DLG_EquipmentCreateDlg.ddf" ) );
	file.CreatePopupDialog( this, NEQUIPMENTCREATE_DLG, TEXT("dialog_EquipmentCreate"), StaticCallBackDialogCNCNEquipmentCreateProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_KIND, this ), NEQUIPMENTCREATE_DLG_STATIC_KIND, TEXT("statictext_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_POWER, this ), NEQUIPMENTCREATE_DLG_STATIC_POWER, TEXT("statictext_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_PROPERTY, this ), NEQUIPMENTCREATE_DLG_STATIC_PROPERTY, TEXT("statictext_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_ENCHANT, this ), NEQUIPMENTCREATE_DLG_STATIC_ENCHANT, TEXT("statictext_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_WEAPON, this ), NEQUIPMENTCREATE_DLG_STATIC_WEAPON, TEXT("statictext_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_HELMET, this ), NEQUIPMENTCREATE_DLG_STATIC_HELMET, TEXT("statictext_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_ARMOR, this ), NEQUIPMENTCREATE_DLG_STATIC_ARMOR, TEXT("statictext_NONAME7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_BELT, this ), NEQUIPMENTCREATE_DLG_STATIC_BELT, TEXT("statictext_NONAME8") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_SHOE, this ), NEQUIPMENTCREATE_DLG_STATIC_SHOE, TEXT("statictext_NONAME9") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_MANTLE, this ), NEQUIPMENTCREATE_DLG_STATIC_MANTLE, TEXT("statictext_NONAME10") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_NECKLACE, this ), NEQUIPMENTCREATE_DLG_STATIC_NECKLACE, TEXT("statictext_NONAME11") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_RING, this ), NEQUIPMENTCREATE_DLG_STATIC_RING, TEXT("statictext_NONAME12") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_STATIC_SLOT, this ), NEQUIPMENTCREATE_DLG_STATIC_SLOT, TEXT("statictext_NONAME13") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE, TEXT("combobox_NONAME14") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND, TEXT("combobox_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND, TEXT("combobox_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND, TEXT("combobox_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND, TEXT("combobox_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND, TEXT("combobox_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND, TEXT("combobox_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND, TEXT("combobox_NONAME7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND, TEXT("combobox_NONAME8") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY, TEXT("combobox_NONAME9") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY, TEXT("combobox_NONAME10") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY, TEXT("combobox_NONAME11") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY, TEXT("combobox_NONAME12") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY, TEXT("combobox_NONAME13") );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT, TEXT("combobox_NONAME15") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT, TEXT("combobox_NONAME16") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT, TEXT("combobox_NONAME17") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT, TEXT("combobox_NONAME18") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT, TEXT("combobox_NONAME19") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT, TEXT("combobox_NONAME20") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT, TEXT("combobox_NONAME21") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT, TEXT("combobox_NONAME22") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT, TEXT("combobox_NONAME23") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT, this ), NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT, TEXT("combobox_NONAME24") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1, TEXT("editbox_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2, TEXT("editbox_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3, TEXT("editbox_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4, TEXT("editbox_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1, TEXT("editbox_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2, TEXT("editbox_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3, TEXT("editbox_NONAME7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4, TEXT("editbox_NONAME8") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1, TEXT("editbox_NONAME9") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2, TEXT("editbox_NONAME10") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3, TEXT("editbox_NONAME11") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4, TEXT("editbox_NONAME12") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1, TEXT("editbox_NONAME13") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2, TEXT("editbox_NONAME14") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3, TEXT("editbox_NONAME15") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4, TEXT("editbox_NONAME16") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1, TEXT("editbox_NONAME17") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2, TEXT("editbox_NONAME18") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3, TEXT("editbox_NONAME19") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4, TEXT("editbox_NONAME20") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1, TEXT("editbox_NONAME21") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2, TEXT("editbox_NONAME22") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3, TEXT("editbox_NONAME23") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4, TEXT("editbox_NONAME24") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1, TEXT("editbox_NONAME25") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2, TEXT("editbox_NONAME26") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3, TEXT("editbox_NONAME27") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4, TEXT("editbox_NONAME28") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1, this ), NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1, TEXT("editbox_NONAME29") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2, this ), NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2, TEXT("editbox_NONAME30") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3, this ), NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3, TEXT("editbox_NONAME31") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4, this ), NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4, TEXT("editbox_NONAME32") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON, TEXT("checkbox_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET, TEXT("checkbox_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR, TEXT("checkbox_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_BELT, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_BELT, TEXT("checkbox_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE, TEXT("checkbox_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE, TEXT("checkbox_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE, TEXT("checkbox_NONAME7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX, NEQUIPMENTCREATE_DLG_CHECKBOX_RING, this ), NEQUIPMENTCREATE_DLG_CHECKBOX_RING, TEXT("checkbox_NONAME8") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEQUIPMENTCREATE_DLG_BUTTON_MONCREATE, this ), NEQUIPMENTCREATE_DLG_BUTTON_MONCREATE, TEXT("button_Create") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEQUIPMENTCREATE_DLG_BUTTON_CLOSE, this ), NEQUIPMENTCREATE_DLG_BUTTON_CLOSE, TEXT("button_Close") );

	// 기능설명위한 ui 추가.
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_NONAME19, this ), NEQUIPMENTCREATE_DLG_NONAME19, TEXT("statictext_NONAME19") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_NONAME14, this ), NEQUIPMENTCREATE_DLG_NONAME14, TEXT("statictext_NONAME14") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_NONAME21, this ), NEQUIPMENTCREATE_DLG_NONAME21, TEXT("statictext_NONAME21") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_NONAME20, this ), NEQUIPMENTCREATE_DLG_NONAME20, TEXT("statictext_NONAME20") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_NONAME15, this ), NEQUIPMENTCREATE_DLG_NONAME15, TEXT("statictext_NONAME15") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NEQUIPMENTCREATE_DLG_OUTLINE_1, this ), NEQUIPMENTCREATE_DLG_OUTLINE_1, TEXT("outline_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NEQUIPMENTCREATE_DLG_OUTLINE_2, this ), NEQUIPMENTCREATE_DLG_OUTLINE_2, TEXT("outline_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NEQUIPMENTCREATE_DLG_OUTLINE_3, this ), NEQUIPMENTCREATE_DLG_OUTLINE_3, TEXT("outline_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NEQUIPMENTCREATE_DLG_OUTLINE_4, this ), NEQUIPMENTCREATE_DLG_OUTLINE_4, TEXT("outline_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NEQUIPMENTCREATE_DLG_OUTLINE_5, this ), NEQUIPMENTCREATE_DLG_OUTLINE_5, TEXT("outline_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NEQUIPMENTCREATE_DLG_OUTLINE_7, this ), NEQUIPMENTCREATE_DLG_OUTLINE_7, TEXT("outline_NONAME7") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_ATTACKRATE, this ), NEQUIPMENTCREATE_DLG_ATTACKRATE, TEXT("statictext_attackrate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_MAGRATE, this ), NEQUIPMENTCREATE_DLG_MAGRATE, TEXT("statictext_magrate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_SKILLRATE, this ), NEQUIPMENTCREATE_DLG_SKILLRATE, TEXT("statictext_skillrate") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEQUIPMENTCREATE_DLG_DURRATE, this ), NEQUIPMENTCREATE_DLG_DURRATE, TEXT("statictext_durrate") );

	AddComboItem();
	AddComboProPerty();
	AddComboEnchant();
	AddComboSlot();

	// 기본은 전부 비활성화 상태로 함
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->Enable(false);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->Enable(false);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->Enable(false);

	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT)->Enable(false);
	m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT)->Enable(false);

	// 숫자만 입력가능.
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->SetNumberMode(true);

	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->SetNumberMode(true);
}

void CALLBACK CNEquipmentCreateDlg::StaticCallBackDialogCNCNEquipmentCreateProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNEquipmentCreateDlg *pThis = (CNEquipmentCreateDlg * )pControllerMgr;
	if( pThis )
	{
		pThis->CallBackDialogCNEquipmentCreateDlgProc( nEvent, nControlID, pControl);
	}
}

void CALLBACK CNEquipmentCreateDlg::CallBackDialogCNEquipmentCreateDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32 siTypeIndex = 0;

					if (m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE)->GetSelectedIndex() >= 0)
					{
						siTypeIndex = m_siaArrItemTypeIndex[m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE)->GetSelectedIndex()];

						m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->Clear();

						if ( siTypeIndex != -1 )
						{
							GetItemInfo(m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND), siTypeIndex);
						}

						m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->SetCursel(0);
					}
				}
				break;
			}
		}

	case NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->GetParam(siSelectIndex);
					m_siWeaponUnique = siParam;
				}
				break;
			}
		}

	case NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND)->GetParam(siSelectIndex);
					m_siHelmetUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND)->GetParam(siSelectIndex);
					m_siArmorUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND)->GetParam(siSelectIndex);
					m_siBeltUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND)->GetParam(siSelectIndex);
					m_siShoeUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND)->GetParam(siSelectIndex);
					m_siMantleUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND)->GetParam(siSelectIndex);
					m_siNeckUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND)->GetParam(siSelectIndex);
					m_siRingUnique = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY)->GetParam(siSelectIndex);
					m_siWeaponProPerty = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY)->GetParam(siSelectIndex);
					m_siHelmetProPerty = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY)->GetParam(siSelectIndex);
					m_siArmorProPerty = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY)->GetParam(siSelectIndex);
					m_siBeltProPerty = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY)->GetParam(siSelectIndex);
					m_siShoeProPerty = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT)->GetParam(siSelectIndex);
					m_siWeaponEnchant = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT)->GetParam(siSelectIndex);
					m_siHelmetEnchant = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT)->GetParam(siSelectIndex);
					m_siArmorEnchant = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT)->GetParam(siSelectIndex);
					m_siBeltEnchant = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT)->GetParam(siSelectIndex);
					m_siShoeEnchant = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT)->GetParam(siSelectIndex);
					m_siWeaponSlot = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT)->GetParam(siSelectIndex);
					m_siHelmetSlot = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT)->GetParam(siSelectIndex);
					m_siArmorSlot = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT)->GetParam(siSelectIndex);
					m_siBeltSlot = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					SI32	siSelectIndex = 0;
					SI32	siParam = 0;

					siSelectIndex = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT)->GetSelectedIndex();
					siParam = m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT)->GetParam(siSelectIndex);
					m_siShoeSlot = siParam;
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_BELT:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_BELT );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_CHECKBOX_RING:
		{
			switch( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					SetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_RING );
				}
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_BUTTON_MONCREATE: // 생성버튼
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetItemInfo();
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;

	case NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_CHANGE:
				{
					TCHAR* szInput = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->GetText();

					SI32 siInput = atoi(szInput);

					// 성능부분에 300이상의 값이 들어갈시에 300으로 다시 설정한다.
					if( siInput > MAX_POWER ) 
					{
						TCHAR szMaxValue[128] = { '\0', };
						StringCchPrintf( szMaxValue, sizeof(szMaxValue), TEXT("%I32d"), MAX_POWER);

						m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->SetText( szMaxValue );
					}
				}	
				break;
			}
		}
		break;
	}
}

void CNEquipmentCreateDlg::GetItemInfo(CComboBox* pComboBox, SI32 siTypeindex)
{
	cltClient *pclclient = (cltClient *)pclClient;
	stComboBoxItemData ComboBoxItemData;
	SI32 siItemtypeunique = siTypeindex;

	if(siItemtypeunique >= 0)
	{
		// 콤보 박스에 아이템의 종류를 설정한다. 
		for(SI32 i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{	
			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
			if(pinfo == NULL)continue;
			if(pinfo->siType != siItemtypeunique)	continue;

			ComboBoxItemData.Init();
			TCHAR ptext[256] ={0,};
			StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pinfo->clItem.siUnique, (TCHAR*)pinfo->GetName() );
			ComboBoxItemData.Set( ptext );		
			ComboBoxItemData.siParam = pinfo->clItem.siUnique;
			pComboBox->AddItem( &ComboBoxItemData );
		}
	}
}

void CNEquipmentCreateDlg::AddComboItem()
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 siCount = 0;

	for(SI32 i = 0; i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		cltItemTypeInfo*	pItemTypeInfo = pclclient->pclItemManager->pclItemTypeInfo[i];
		if(pItemTypeInfo == NULL)	continue;

		stComboBoxItemData ComboBoxItemTypeData;
		ComboBoxItemTypeData.Init();
		ComboBoxItemTypeData.Set( (TCHAR*)pItemTypeInfo->GetName() );
		ComboBoxItemTypeData.siParam = i;

		// 무기류 (검,창,도끼,활,총,지팡이)
		if(pItemTypeInfo->siUnique >= 1 && pItemTypeInfo->siUnique <= 6)
		{
			m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE)->AddItem( &ComboBoxItemTypeData );  
		}

		// 투구
		if(pItemTypeInfo->siUnique == 7)
		{
			stComboBoxItemData ComboBoxItemData;
			
			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 7)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND)->AddItem( &ComboBoxItemData );
			}
		}

		// 갑옷
		if(pItemTypeInfo->siUnique == 8)
		{
			stComboBoxItemData ComboBoxItemData;

			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 8)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND)->AddItem( &ComboBoxItemData );
			}
		}

		// 허리띠
		if(pItemTypeInfo->siUnique == 9)
		{
			stComboBoxItemData ComboBoxItemData;

			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 9)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND)->AddItem( &ComboBoxItemData );
			}
		}

		// 신발
		if(pItemTypeInfo->siUnique == 10)
		{
			stComboBoxItemData ComboBoxItemData;

			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 10)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND)->AddItem( &ComboBoxItemData );
			}
		}
		
		// 망토
		if(pItemTypeInfo->siUnique == 41)
		{
			stComboBoxItemData ComboBoxItemData;

			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 41)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND)->AddItem( &ComboBoxItemData );
			}
		}

		// 목걸이
		if(pItemTypeInfo->siUnique == 44)
		{
			stComboBoxItemData ComboBoxItemData;

			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 44)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND)->AddItem( &ComboBoxItemData );
			}
		}

		// 반지
		if(pItemTypeInfo->siUnique == 45)
		{
			stComboBoxItemData ComboBoxItemData;

			for(SI32 j = 0; j < MAX_ITEMINFO_NUMBER; j++)
			{
				cltItemInfo* pItemInfo = pclClient->pclItemManager->pclItemInfo[j];
				if(pItemInfo == NULL)		continue;
				if(pItemInfo->siType != 45)	continue;

				ComboBoxItemData.Init();
				TCHAR ptext[256] ={0,};
				StringCchPrintf( ptext, 256, TEXT("[%d]%s"), pItemInfo->clItem.siUnique, (TCHAR*)pItemInfo->GetName() );
				ComboBoxItemData.Set( ptext );		
				ComboBoxItemData.siParam = pItemInfo->clItem.siUnique;
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND)->AddItem( &ComboBoxItemData );
			}
		}

		m_siaArrItemTypeIndex[siCount] = i;
		siCount++;
	}
}

void CNEquipmentCreateDlg::SetItemInfo()
{
	cltClient* pclclient = (cltClient*)pclClient;

	// 무기
	TCHAR* szWeaponPower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->GetText();
	TCHAR* szWeaponPower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->GetText();
	TCHAR* szWeaponPower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->GetText();
	TCHAR* szWeaponPower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->GetText();

	// 투구
	TCHAR* szHelmetPower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->GetText();
	TCHAR* szHelmetPower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->GetText();
	TCHAR* szHelmetPower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->GetText();
	TCHAR* szHelmetPower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->GetText();

	// 갑옷
	TCHAR* szArmorPower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->GetText();
	TCHAR* szArmorPower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->GetText();
	TCHAR* szArmorPower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->GetText();
	TCHAR* szArmorPower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->GetText();

	// 허리띠
	TCHAR* szBeltPower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->GetText();
	TCHAR* szBeltPower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->GetText();
	TCHAR* szBeltPower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->GetText();
	TCHAR* szBeltPower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->GetText();

	// 신발
	TCHAR* szShoePower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->GetText();
	TCHAR* szShoePower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->GetText();
	TCHAR* szShoePower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->GetText();
	TCHAR* szShoePower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->GetText();

	// 망토
	TCHAR* szMantlePower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->GetText();
	TCHAR* szMantlePower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->GetText();
	TCHAR* szMantlePower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->GetText();
	TCHAR* szMantlePower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->GetText();

	// 목걸이
	TCHAR* szNeckLacePower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->GetText();
	TCHAR* szNeckLacePower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->GetText();
	TCHAR* szNeckLacePower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->GetText();
	TCHAR* szNeckLacePower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->GetText();

	// 반지
	TCHAR* szRingPower1 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->GetText();
	TCHAR* szRingPower2 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->GetText();
	TCHAR* szRingPower3 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->GetText();
	TCHAR* szRingPower4 = m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->GetText();

   	SI32 siWeaponPower1 = atoi(szWeaponPower1);
	SI32 siWeaponPower2 = atoi(szWeaponPower2);
	SI32 siWeaponPower3 = atoi(szWeaponPower3);
	SI32 siWeaponPower4 = atoi(szWeaponPower4);

	SI32 siHelmetPower1 = atoi(szHelmetPower1);
	SI32 siHelmetPower2 = atoi(szHelmetPower2);
	SI32 siHelmetPower3 = atoi(szHelmetPower3);
	SI32 siHelmetPower4 = atoi(szHelmetPower4);
	
	SI32 siArmorPower1 = atoi(szArmorPower1);
	SI32 siArmorPower2 = atoi(szArmorPower2);
	SI32 siArmorPower3 = atoi(szArmorPower3);
	SI32 siArmorPower4 = atoi(szArmorPower4);

	SI32 siBeltPower1 = atoi(szBeltPower1);
	SI32 siBeltPower2 = atoi(szBeltPower2);
	SI32 siBeltPower3 = atoi(szBeltPower3);
	SI32 siBeltPower4 = atoi(szBeltPower4);

	SI32 siShoePower1 = atoi(szShoePower1);
	SI32 siShoePower2 = atoi(szShoePower2);
	SI32 siShoePower3 = atoi(szShoePower3);
	SI32 siShoePower4 = atoi(szShoePower4);

	SI32 siMantlePower1 = atoi(szMantlePower1);
	SI32 siMantlePower2 = atoi(szMantlePower2);
	SI32 siMantlePower3 = atoi(szMantlePower3);
	SI32 siMantlePower4 = atoi(szMantlePower4);

	SI32 siNeckLacePower1 = atoi(szNeckLacePower1);
	SI32 siNeckLacePower2 = atoi(szNeckLacePower2);
	SI32 siNeckLacePower3 = atoi(szNeckLacePower3);
	SI32 siNeckLacePower4 = atoi(szNeckLacePower4);

	SI32 siRingPower1 = atoi(szRingPower1);
	SI32 siRingPower2 = atoi(szRingPower2);
	SI32 siRingPower3 = atoi(szRingPower3);
	SI32 siRingPower4 = atoi(szRingPower4);

	// 1개 이상의 능력치를 설정하지 않으면 장비 생성을 할 수 없음.
	// 체크박스로 체크된것만 확인함.
	if(m_bCheckWeapon)
	{
		if((siWeaponPower1 == 0) || (siWeaponPower2 == 0) || (siWeaponPower3 == 0) || (siWeaponPower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckHelmet)
	{
		if((siHelmetPower1 == 0) || (siHelmetPower2 == 0) || (siHelmetPower3 == 0) || (siHelmetPower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckArmor)
	{
		if((siArmorPower1 == 0) || (siArmorPower2 == 0) || (siArmorPower3 == 0) || (siArmorPower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckBelt)
	{
		if((siBeltPower1 == 0) || (siBeltPower2 == 0) || (siBeltPower3 == 0) || (siBeltPower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckShoe)
	{
		if((siShoePower1 == 0) || (siShoePower2 == 0) || (siShoePower3 == 0) || (siShoePower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckMantle)
	{
		if((siMantlePower1 == 0) || (siMantlePower2 == 0) || (siMantlePower3 == 0) || (siMantlePower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckNeck)
	{
		if((siNeckLacePower1 == 0) || (siNeckLacePower2 == 0) || (siNeckLacePower3 == 0) || (siNeckLacePower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	if(m_bCheckRing)
	{
		if((siRingPower1 == 0) || (siRingPower2 == 0) || (siRingPower3 == 0) || (siRingPower4 == 0))
		{
			NTCHARString256	strTitle	= GetTxtFromMgr(512);
			NTCHARString256	strText		= GetTxtFromMgr(40201);

			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( strTitle, strText );
			return;
		}
	}

	// 장비 생성 요청
	cltGameMsgRequest_EquipmentCreate clEquipmentCreateMsg( m_siWeaponUnique, m_siHelmetUnique, m_siArmorUnique, m_siBeltUnique, m_siShoeUnique, m_siNeckUnique, m_siRingUnique, m_siMantleUnique, 
															siWeaponPower1, siWeaponPower2, siWeaponPower3, siWeaponPower4, 
															siHelmetPower1, siHelmetPower2, siHelmetPower3, siHelmetPower4, siArmorPower1, siArmorPower2, siArmorPower3, siArmorPower4, 
															siBeltPower1, siBeltPower2, siBeltPower3, siBeltPower4, siShoePower1, siShoePower2, siShoePower3, siShoePower4,
															siMantlePower1, siMantlePower2, siMantlePower3, siMantlePower4, siNeckLacePower1, siNeckLacePower2, siNeckLacePower3, siNeckLacePower4,
															siRingPower1, siRingPower2, siRingPower3, siRingPower4, m_siWeaponProPerty, m_siHelmetProPerty, m_siArmorProPerty, m_siBeltProPerty, m_siShoeProPerty,
															m_siWeaponEnchant, m_siHelmetEnchant, m_siArmorEnchant, m_siBeltEnchant, m_siShoeEnchant,
															m_siWeaponSlot, m_siHelmetSlot, m_siArmorSlot, m_siBeltSlot, m_siShoeSlot);
	cltMsg clMsg( GAMEMSG_REQUEST_EQUIPMENTCREATE, sizeof(clEquipmentCreateMsg), (BYTE*)&clEquipmentCreateMsg );
	pclclient->SendMsgToServer( (sPacketHeader*)& clMsg );
}

void CNEquipmentCreateDlg::AddComboProPerty()
{
	cltItemManagerClient* pclItemManager = (cltItemManagerClient*)pclClient->pclItemManager;

	// 무기류
	for(SI32 i = 0; i<=NOW_ENCHANT_NUMBER; i++)
	{
		stComboBoxItemData ComboBoxProPertytData;
		ComboBoxProPertytData.Init();
		TCHAR pText[128] = {0.};
		if(i == 0)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i;
		}
		else if(i == 1)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i;
		}
		else if(i == 2)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+1) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+1;
		}
		else if(i == 3)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+2) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+2;
		}
		else if(i == 4)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+3) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+3;
		}

		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY)->AddItem( &ComboBoxProPertytData );
	}

	// 투구, 갑옷, 허리띠, 신발
	for(SI32 i = 0; i<=NOW_ENCHANT_NUMBER; i++)
	{
		stComboBoxItemData ComboBoxProPertytData;
		ComboBoxProPertytData.Init();
		TCHAR pText[128] = {0.};
		if(i == 0)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i;
		}
		else if(i == 1)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+1) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+1;
		}
		else if(i == 2)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+2) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+2;
		}
		else if(i == 3)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+3) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+3;
		}
		else if(i == 4)
		{
			StringCchPrintf( pText, 256, TEXT("%s"), ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(pText, i+4) );
			ComboBoxProPertytData.Set( pText );
			ComboBoxProPertytData.siParam = i+4;
		}

		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY)->AddItem( &ComboBoxProPertytData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY)->AddItem( &ComboBoxProPertytData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY)->AddItem( &ComboBoxProPertytData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY)->AddItem( &ComboBoxProPertytData );
	}
}

void CNEquipmentCreateDlg::AddComboEnchant()
{
	stComboBoxItemData ComboBoxEnchantData;
	ComboBoxEnchantData.Init();
	TCHAR pText[32] = {0.};

	for(SI32 i = 0; i<= MAX_ENCHANT_GRADE; i++)
	{
		itoa(i, pText, 10);
		ComboBoxEnchantData.Set( pText );
		ComboBoxEnchantData.siParam = i;

		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT)->AddItem( &ComboBoxEnchantData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT)->AddItem( &ComboBoxEnchantData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT)->AddItem( &ComboBoxEnchantData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT)->AddItem( &ComboBoxEnchantData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT)->AddItem( &ComboBoxEnchantData );
	}
}

void CNEquipmentCreateDlg::AddComboSlot()
{
	stComboBoxItemData ComboBoxSlotData;
	ComboBoxSlotData.Init();
	TCHAR pText[32] = {0.};

	for(SI32 i = 0; i<= 5; i++)
	{
		itoa(i, pText, 10);
		ComboBoxSlotData.Set( pText );
		ComboBoxSlotData.siParam = i;

		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT)->AddItem( &ComboBoxSlotData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT)->AddItem( &ComboBoxSlotData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT)->AddItem( &ComboBoxSlotData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT)->AddItem( &ComboBoxSlotData );
		m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT)->AddItem( &ComboBoxSlotData );
	}
}

void CNEquipmentCreateDlg::SetCheckBox(SI32 siControllID)
{
	SI32 siCheckBoxID	= 0;
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( siControllID )
	{
	case NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_WEAPON );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT)->Enable(true);

				m_bCheckWeapon = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONTYPE)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONPROPERTY)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONENCHANT)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_WEAPONSLOT)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_WEAPONPOWER4)->Clear();

				m_siWeaponUnique = 0;
				m_bCheckWeapon = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_HELMET );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT)->Enable(true);

				m_bCheckHelmet = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETPROPERTY)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETENCHANT)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_HELMETSLOT)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_HELMETPOWER4)->Clear();

				m_siHelmetUnique = 0;
				m_bCheckHelmet = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_ARMOR );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT)->Enable(true);

				m_bCheckArmor = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORPROPERTY)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORENCHANT)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_ARMORSLOT)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_ARMORPOWER4)->Clear();

				m_siArmorUnique = 0;
				m_bCheckArmor = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_BELT:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_BELT;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_BELT );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT)->Enable(true);

				m_bCheckBelt = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTPROPERTY)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTENCHANT)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_BELTSLOT)->Enable(false);
				
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_BELTPOWER4)->Clear();

				m_siBeltUnique = 0;
				m_bCheckBelt = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_SHOE );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT)->Enable(true);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT)->Enable(true);

				m_bCheckShoe = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEPROPERTY)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOEENCHANT)->Enable(false);
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_SHOESLOT)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_SHOEPOWER4)->Clear();

				m_siShoeUnique = 0;
				m_bCheckShoe = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_MANTLE );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->Enable(true);

				m_bCheckMantle = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_MANTLEKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_MANTLEPOWER4)->Clear();

				m_siMantleUnique = 0;
				m_bCheckMantle = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_NECKLACE );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->Enable(true);

				m_bCheckNeck = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_NECKLACEKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_NECKLACEPOWER4)->Clear();

				m_siNeckUnique = 0;
				m_bCheckNeck = false;
			}
		}
		break;
	case NEQUIPMENTCREATE_DLG_CHECKBOX_RING:
		{
			siCheckBoxID	= NEQUIPMENTCREATE_DLG_CHECKBOX_RING;
			CCheckBox* pCheckBox = m_InterfaceFactory.GetCheckBox( NEQUIPMENTCREATE_DLG_CHECKBOX_RING );
			if ( pCheckBox->IsCheck() == true ) 
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->Enable(true);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->Enable(true);

				m_bCheckRing = true;
			}
			else
			{
				m_InterfaceFactory.GetComboBox(NEQUIPMENTCREATE_DLG_COMBOBOX_RINGKIND)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->Enable(false);
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->Enable(false);

				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER1)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER2)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER3)->Clear();
				m_InterfaceFactory.GetEdit(NEQUIPMENTCREATE_DLG_EDITBOX_RINGPOWER4)->Clear();

				m_siRingUnique = 0;

				m_bCheckRing = false;
			}
		}
		break;
	default:
		return;
	}

	if ( NULL == pclclient )
	{
		return;
	}

	CCheckBox* pCheckChatSet = m_InterfaceFactory.GetCheckBox(siCheckBoxID);
	if ( NULL == pCheckChatSet )
	{
		return;
	}
}