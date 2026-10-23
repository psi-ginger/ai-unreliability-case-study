/*====================================

클래스 : CNHappyBeanDlg
작성일 : 07 .08 . 22
작성자: 손성웅
변경사항:
차후작업:
=====================================*/
#include "NChristmasEventDlg.h"
#include "../Client/NStructureDlg/StrBaseDlg/StrBaseDlg.h"
#include "../CommonLogic/StructureClass/StructureClass.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Client/UnderMouseObject/UnderMouseObject.h"

NChristmasEventDlg::NChristmasEventDlg()

{
	ZeroMemory(matitemunique,		sizeof(matitemunique));		//필요한 재료 아이템유니크
	ZeroMemory(matitemnum,			sizeof(matitemnum));		//필요한 재료아이템 갯수
	ZeroMemory(matitemEndunique,	sizeof(matitemEndunique));	//가지고 있는 재료아이템 유니크
	ZeroMemory(matitemEndnum,		sizeof(matitemEndnum));		//가지고 있는 재료아이템 갯수

	Initialize();
}

NChristmasEventDlg::~NChristmasEventDlg()
{
	Destroy();
}

void NChristmasEventDlg::init()
{

}


void NChristmasEventDlg::Destroy()
{

}
void NChristmasEventDlg::Create()
{
	if ( IsCreate() )	return;
	
	CInterfaceFile file;
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 id =pclclient->pclUnderMouseObject->GetUnderMouseID();//마우스 아래 있는 것 카인드값을 가저 온다.
	
	//[진성] 영웅의 동상 클릭 후 창. => 2008-6-9
	if (   pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUEBASE"))
		|| pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE"))) 
	{
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_PineTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this )
							,NCHRISTMASEVENT_EDIT_EXPLAIN
							,"editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
								,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
								,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
								,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
								,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
								,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
								,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
								,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
								,buffer);
		}

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
							NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");	//기부하기 버튼 생성
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_MAKETREE , this ), 
							NCHRISTMASEVENT_BUTTON_MAKETREE,   "button_maketree");

		
		//영웅의 동상 완성 설명창
		TCHAR* pTextEditChristmas;
		pTextEditChristmas = GetTxtFromMgr(8458);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);

		TCHAR* pmaketree = GetTxtFromMgr(8459);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->SetText(pmaketree);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->Enable(false);		//기본적으로 영웅의 동상 버튼은 비활성화 시킨다.
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[성웅] 할로윈 호박 촛대 클릭후 창
	if (   pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENBASE"))
		|| pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEEN"))) 
	{
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_PineTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this )
			,NCHRISTMASEVENT_EDIT_EXPLAIN
			,"editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
				,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);
		}

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
			NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");	//기부하기 버튼 생성
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_MAKETREE , this ), 
			NCHRISTMASEVENT_BUTTON_MAKETREE,   "button_maketree");


		//할로윈 이벤트 상 설명창.
		TCHAR* pTextEditChristmas;
		pTextEditChristmas = GetTxtFromMgr(8887);// 텍스트 넣어야 한다.
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);

		TCHAR* pmaketree = GetTxtFromMgr(8720);// 텍스트 넣어야 한다.
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->SetText(pmaketree);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->Enable(false);		//기본적으로 할로윈 촛대 업그레이드 버전은 비활성화
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if (pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_PINE_TREE")))
	{
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_PineTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this ), 
			NCHRISTMASEVENT_EDIT_EXPLAIN,   "editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";

		for(SI32 Count = 1; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);
		}
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
				,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);
		}
		////-----------------------------------------------------------------------------------------------------------------
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM2 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM2,   "listview_item2");//아이템 이미지 2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM3 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM3,   "listview_item3");//아이템 이미지 3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM4 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM4,   "listview_item4");//아이템 이미지 4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM5 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM5,   "listview_item5");//아이템 이미지 5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM6 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM6,   "listview_item6");//아이템 이미지 6
		////-----------------------------------------------------------------------------------------------------------------
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME1,   "editbox_itemname1");//아이템이름1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME2 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME2,   "editbox_itemname2");//아이템이름2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME3 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME3,   "editbox_itemname3");//아이템이름3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME4 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME4,   "editbox_itemname4");//아이템이름4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME5 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME5,   "editbox_itemname5");//아이템이름5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME6 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME6,   "editbox_itemname6");//아이템이름6
		////-----------------------------------------------------------------------------------------------------------------
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX1,   "editbox_itemmaxnum1");//아이템누적갯수1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX2 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX2,   "editbox_itemmaxnum2");//아이템누적갯수2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX3 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX3,   "editbox_itemmaxnum3");//아이템누적갯수3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX4 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX4,   "editbox_itemmaxnum4");//아이템누적갯수4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX5 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX5,   "editbox_itemmaxnum5");//아이템누적갯수5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX6 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX6,   "editbox_itemmaxnum6");//아이템누적갯수6
		////-----------------------------------------------------------------------------------------------------------------
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM1,   "editbox_itemnum1");//아이템기부갯수1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM2 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM2,   "editbox_itemnum2");//아이템기부갯수2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM3 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM3,   "editbox_itemnum3");//아이템기부갯수3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM4 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM3,   "editbox_itemnum4");//아이템기부갯수4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM5 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM5,   "editbox_itemnum5");//아이템기부갯수5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM6 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM6,   "editbox_itemnum6");//아이템기부갯수6
		////-----------------------------------------------------------------------------------------------------------------
	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
			NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");//기부하기 버튼 생성
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_MAKETREE , this ), 
			NCHRISTMASEVENT_BUTTON_MAKETREE,   "button_maketree");//트리 만들기 버튼생성
		
		TCHAR* pmaketree = GetTxtFromMgr(7651);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->SetText(pmaketree);
		//크리스마스 설명창
		TCHAR* pTextEditChristmas;
		pTextEditChristmas = GetTxtFromMgr(7655);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);
		
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->Enable(false);//기본적으로 매이크 트리 버튼은 비활성화 시킨다.
	}

	//KHY - 1113 - 크리스마스 이벤트 - KIND_PINE_TREE , KIND_CHRISTMASTREEBASE = 바뀐게 없음
	if(pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREEBASE")) ) 
	{
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_PineTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this ), 
			NCHRISTMASEVENT_EDIT_EXPLAIN,   "editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";
/*
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);
		}
*/
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
				,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);
		}

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
			NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");//기부하기 버튼 생성
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_MAKETREE , this ), 
			NCHRISTMASEVENT_BUTTON_MAKETREE,   "button_maketree");//트리 만들기 버튼생성
		
		TCHAR* pmaketree = GetTxtFromMgr(6654);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->SetText(pmaketree);
		//크리스마스 설명창
		TCHAR* pTextEditChristmas;
		pTextEditChristmas = GetTxtFromMgr(7655);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);
		
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->Enable(false);//기본적으로 매이크 트리 버튼은 비활성화 시킨다.
	}
	
	// 춘기 - 신년이벤트클릭했을 경우
	if (   pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREEBASE"))) 
	{
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_PineTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this )
			,NCHRISTMASEVENT_EDIT_EXPLAIN
			,"editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
				,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);
		}

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
			NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");	//기부하기 버튼 생성
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_MAKETREE , this ), 
			NCHRISTMASEVENT_BUTTON_MAKETREE,   "button_maketree");


		//신년이벤트 설명창
		TCHAR* pTextEditChristmas;
		pTextEditChristmas = GetTxtFromMgr(9739);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);

		TCHAR* pmaketree = GetTxtFromMgr(9673);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->SetText(pmaketree);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->Enable(false);		//기본적으로 영웅의 동상 버튼은 비활성화 시킨다.
	}

	if (pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"))) 
	{
		//크리스마스 트리일경우
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_CHristmasTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this ), 
			NCHRISTMASEVENT_EDIT_EXPLAIN,   "editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
				,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);
		}
		//--------------------------------------------------------------------------------------------------------------
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM1,   "listview_item1");//아이템 이미지 1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM2 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM2,   "listview_item2");//아이템 이미지 2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM3 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM3,   "listview_item3");//아이템 이미지 3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM4 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM4,   "listview_item4");//아이템 이미지 4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM5 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM5,   "listview_item5");//아이템 이미지 5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM6 , this ), 
		//	NCHRISTMASEVENT_LISTVIEW_ITEM6,   "listview_item6");//아이템 이미지 6

		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME1,   "editbox_itemname1");//아이템이름1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME2 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME2,   "editbox_itemname2");//아이템이름2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME3 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME3,   "editbox_itemname3");//아이템이름3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME4 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME4,   "editbox_itemname4");//아이템이름4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME5 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME5,   "editbox_itemname5");//아이템이름5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME6 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMNAME6,   "editbox_itemname6");//아이템이름6

		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX1,   "editbox_itemmaxnum1");//아이템누적갯수1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX2 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX2,   "editbox_itemmaxnum2");//아이템누적갯수2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX3 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX3,   "editbox_itemmaxnum3");//아이템누적갯수3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX4 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX4,   "editbox_itemmaxnum4");//아이템누적갯수4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX5 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX5,   "editbox_itemmaxnum5");//아이템누적갯수5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX6 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMAX6,   "editbox_itemmaxnum6");//아이템누적갯수6

		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM1,   "editbox_itemnum1");//아이템기부갯수1
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM2 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM2,   "editbox_itemnum2");//아이템기부갯수2
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM3 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM3,   "editbox_itemnum3");//아이템기부갯수3
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM4 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM3,   "editbox_itemnum4");//아이템기부갯수4
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM5 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM5,   "editbox_itemnum5");//아이템기부갯수5
		//file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM6 , this ), 
		//	NCHRISTMASEVENT_EDIT_ITEMMNUM6,   "editbox_itemnum6");//아이템기부갯수6
		//--------------------------------------------------------------------------------------------------------
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
			NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");//기부하기 버튼 생성
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,NCHRISTMASEVENT_BUTTON_HAVEITEM , this ), 
			NCHRISTMASEVENT_BUTTON_HAVEITEM,	"button_haveItem");//선물받기를 생성한다.
		//선물받기
		TCHAR* phaveitem = GetTxtFromMgr(7663);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_HAVEITEM)->SetText(phaveitem);
		TCHAR* pTextEditChristmas;
		pTextEditChristmas = GetTxtFromMgr(7661);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);

		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_HAVEITEM)->Enable(false);//기본적으로는 비활성화.
		
	}

	// [춘기] 크리스마스 이벤트 - 엔펠리스 앞에 있는 크리스 마스 트리를 클릭했을때 (09.10.21)
	if (pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01"))
		|| pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02"))) 
	{
		//크리스마스 트리일경우
		file.LoadFile(TEXT("NInterface/Data/NChristnasEventDlg/NChristnasEventDlg_CHristmasTree.ddf"));
		file.CreatePopupDialog( this, NCHRISTMASEVENT_DLG,TEXT("dialog_christmasevent"), StaticSelectChristmasEventProc);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_EXPLAIN , this ), 
			NCHRISTMASEVENT_EDIT_EXPLAIN,   "editbox_explain");//에디트 설명창

		TCHAR buffer[128] = "";
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; ++Count)
		{
			_stprintf(buffer, TEXT("listview_item%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( LISTVIEW, NCHRISTMASEVENT_LISTVIEW_ITEM1+Count , this )
				,NCHRISTMASEVENT_LISTVIEW_ITEM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemname%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMNAME1+Count , this ) 
				,NCHRISTMASEVENT_EDIT_ITEMNAME1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemmaxnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMAX1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);

			_stprintf(buffer, TEXT("editbox_itemnum%d"), Count+1);
			file.CreateControl(  m_InterfaceFactory.CreateUserInterface( EDIT, NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count , this )
				,NCHRISTMASEVENT_EDIT_ITEMMNUM1+Count
				,buffer);
		}
	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL , this ), 
			NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL,   "button_donationmaterial");//기부하기 버튼 생성

		// 기부하기 버튼 위치 설정
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL)->SetX(109);
		m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL)->SetY(307);
	}

	//현제 보유한 재료 갯수 기본 0으로 셋팅
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText("0");
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText("0");
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText("0");
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText("0");
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText("0");
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText("0");

	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetNumberMode(true);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetNumberMode(true);

	//크리스마스 설명창
	/*TCHAR* pTextEditChristmas;
	pTextEditChristmas = GetTxtFromMgr(7655);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pTextEditChristmas);*/
	//크리스마스 트리 카인드 찾기.
	//처음 만들어지면서 서버로 현제 마을의 트리재료 갯수를 요청.
	////다시 리턴받아서 해당 슬롯에 넣는다.
	//리스트를 요구하는 패킷 날리기.
	//아이템 가운데 정렬
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_CENTER);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_CENTER);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_CENTER);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_CENTER);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_CENTER);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_CENTER);
	//아이템 누적갯수
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX1)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX2)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX3)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX4)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX5)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX6)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);

	//아이템 입력갯수
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);
	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetTextPrintOption(DT_TOP | DT_SINGLELINE | DT_RIGHT);

	//기부하기 버튼텍스트 설정
	TCHAR * pinputmat = GetTxtFromMgr(7662);
	m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL)->SetText(pinputmat);	

	SI32 siVillageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();//마을 유니크
	cltEventStructureUnit* pEventNow = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_CHRISTMASTREE);//크리스마스트리
	cltEventStructureUnit* pEventStruct = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_TREE);//파인트리
	stListViewItemData ListViewItemData;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

//==============================================================================
	//[진성] 영웅의 동상 클릭 . => 2008-6-9
	if(	pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUEBASE")))
	{
		pEventStruct = NULL;
		pEventStruct = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_HERO_BRONZE);
		if(NULL == pEventStruct)
			return;

		EventTreeType = EVENT_STRUCTURE_HERO_BRONZE;
		cltGameMsgRequest_Event_MaterialList clMaterialList(siVillageUnique, EventTreeType);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);
		
		//리스트를 업데이트 하기위한 패킷
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventStruct->clMaterial[i];

			if(pclMaterialInfo == NULL)												
				continue;
			
			SI16 siUnique  = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	
				continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				
				
				//재료아이템유니크와 재료 아이템 갯수를 배열에 저장원 재료 아이템.
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
			}
		}

		// 재료 이름.
		const TCHAR* pitemname[MATERIAL_ITEM_MAX_NUM] = {NULL, }; 
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; Count++)
		{
			pitemname[Count] = pclClient->pclItemManager->GetName(matitemunique[Count]);
			m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1+Count)->SetText((TCHAR*)pitemname[Count]);
			m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1+Count)->ShowScrollBar(false);
		}
				
		SI32 invennum = 0;
		SI32 matnum	  = 0;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0; matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];//재료 아이템
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siItemNum;			//실제 가지고 있는 아이템들의 유니크와 갯수
					
					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	
					{
						matitemEndnum[matnum]	= siMaterialNum;
						siNum					= matitemEndnum[matnum];
					}
					else
					{
						matitemEndnum[matnum]	= siNum;
					}

					if( (0 <= matnum) && (matnum < MATERIAL_ITEM_MAX_NUM) )	
					{
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1+matnum)->SetText(itemnum);//itemnum);
						break;
					}
				}
			}
		}
	}
//=======================================================================================================

	//==============================================================================
	//[성웅] 할로윈 촛대 //Switch_HalloweenEvent
	if(	pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENBASE")))
	{
		pEventStruct = NULL;
		pEventStruct = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_HALLOWEEN);
		if(NULL == pEventStruct)
			return;

		EventTreeType = EVENT_STRUCTURE_HALLOWEEN;
		cltGameMsgRequest_Event_MaterialList clMaterialList(siVillageUnique, EventTreeType);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);

		//리스트를 업데이트 하기위한 패킷
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventStruct->clMaterial[i];

			if(pclMaterialInfo == NULL)												
				continue;

			SI16 siUnique  = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	
				continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				

				//재료아이템유니크와 재료 아이템 갯수를 배열에 저장원 재료 아이템.
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
			}
		}

		// 재료 이름.
		const TCHAR* pitemname[MATERIAL_ITEM_MAX_NUM] = {NULL, }; 
		for(SI32 Count = 0; Count < MATERIAL_ITEM_MAX_NUM; Count++)
		{
			pitemname[Count] = pclClient->pclItemManager->GetName(matitemunique[Count]);
			m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1+Count)->SetText((TCHAR*)pitemname[Count]);
			m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1+Count)->ShowScrollBar(false);
		}

		SI32 invennum = 0;
		SI32 matnum	  = 0;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0; matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];//재료 아이템
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siItemNum;			//실제 가지고 있는 아이템들의 유니크와 갯수

					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	
					{
						matitemEndnum[matnum]	= siMaterialNum;
						siNum					= matitemEndnum[matnum];
					}
					else
					{
						matitemEndnum[matnum]	= siNum;
					}

					if( (0 <= matnum) && (matnum < MATERIAL_ITEM_MAX_NUM) )	
					{
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1+matnum)->SetText(itemnum);//itemnum);
						break;
					}
				}
			}
		}
		// 재료가 4개만 들어가니까 2개는 안보여준다
		m_InterfaceFactory.GetListView(NCHRISTMASEVENT_LISTVIEW_ITEM5)->Show(false);
		m_InterfaceFactory.GetListView(NCHRISTMASEVENT_LISTVIEW_ITEM6)->Show(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->Show(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->Show(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->Show(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->Show(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX5)->Show(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX6)->Show(false);
	}
	//=======================================================================================================


	if(pEventStruct/*파인트리이면*/
		&& pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_PINE_TREE")))
	{
		EventTreeType = EVENT_STRUCTURE_TREE;
		cltGameMsgRequest_Event_MaterialList clMaterialList(siVillageUnique,EVENT_STRUCTURE_TREE);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);//리스트를 업데이트 하기위한 패킷

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventStruct->clMaterial[i];

			if(pclMaterialInfo == NULL)												continue;
			SI16 siUnique = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				
				//재료아이템유니크와 재료 아이템 갯수를 배열에 저장원 재료 아이템.
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
				//아이템의 이름을 가지고 온다.
			}
		}//재료아이템 반복문의 끝.
		//재료아이템 이름 설정
		const TCHAR* pitemname1 = pclClient->pclItemManager->GetName(matitemunique[0]);
		const TCHAR* pitemname2 = pclClient->pclItemManager->GetName(matitemunique[1]);
		const TCHAR* pitemname3 = pclClient->pclItemManager->GetName(matitemunique[2]);
		const TCHAR* pitemname4 = pclClient->pclItemManager->GetName(matitemunique[3]);
		const TCHAR* pitemname5 = pclClient->pclItemManager->GetName(matitemunique[4]);
		const TCHAR* pitemname6 = pclClient->pclItemManager->GetName(matitemunique[5]);
		//아이템 이름 설정
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->SetText((TCHAR*)pitemname1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->SetText((TCHAR*)pitemname2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->SetText((TCHAR*)pitemname3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->SetText((TCHAR*)pitemname4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->SetText((TCHAR*)pitemname5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->SetText((TCHAR*)pitemname6);
		//스크롤바 옵션이름에 보이지 않도록
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->ShowScrollBar(false);

		int		invennum, matnum;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0;matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];//재료 아이템
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siItemNum;//실제 가지고 있는 아이템들의 유니크와 갯수
					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	{
						matitemEndnum[matnum]= siMaterialNum;
						siNum = matitemEndnum[matnum];}
					else{
						matitemEndnum[matnum]= siNum;
					}
					

					if (matnum == 0) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(itemnum);//itemnum);
						break;
					}
					if (matnum == 1) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(itemnum);
						break;
					}
					if (matnum == 2) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(itemnum);
						break;
					}
					if (matnum == 3) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(itemnum);
						break;
					}
					if (matnum == 4) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(itemnum);
						break;
					}
					if (matnum == 5) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(itemnum);
						break;
					}				
				}
			}
		}
	}

	//KHY - 1113 - 크리스마스 이벤트 -
	if( pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREEBASE")))
	{
		pEventStruct = NULL;
		pEventStruct = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_CHRISTMASTREEBASE);
		if(NULL == pEventStruct)
			return;
	
		EventTreeType = EVENT_STRUCTURE_CHRISTMASTREEBASE;
		cltGameMsgRequest_Event_MaterialList clMaterialList(siVillageUnique,EVENT_STRUCTURE_CHRISTMASTREEBASE);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);//리스트를 업데이트 하기위한 패킷

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventStruct->clMaterial[i];

			if(pclMaterialInfo == NULL)												continue;
			SI16 siUnique = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				
				//재료아이템유니크와 재료 아이템 갯수를 배열에 저장원 재료 아이템.
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
				//아이템의 이름을 가지고 온다.
			}
		}//재료아이템 반복문의 끝.
		//재료아이템 이름 설정
		const TCHAR* pitemname1 = pclClient->pclItemManager->GetName(matitemunique[0]);
		const TCHAR* pitemname2 = pclClient->pclItemManager->GetName(matitemunique[1]);
		const TCHAR* pitemname3 = pclClient->pclItemManager->GetName(matitemunique[2]);
		const TCHAR* pitemname4 = pclClient->pclItemManager->GetName(matitemunique[3]);
		const TCHAR* pitemname5 = pclClient->pclItemManager->GetName(matitemunique[4]);
		const TCHAR* pitemname6 = pclClient->pclItemManager->GetName(matitemunique[5]);
		//아이템 이름 설정
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->SetText((TCHAR*)pitemname1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->SetText((TCHAR*)pitemname2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->SetText((TCHAR*)pitemname3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->SetText((TCHAR*)pitemname4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->SetText((TCHAR*)pitemname5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->SetText((TCHAR*)pitemname6);
		//스크롤바 옵션이름에 보이지 않도록
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->ShowScrollBar(false);

		int		invennum, matnum;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0;matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];//재료 아이템
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siItemNum;//실제 가지고 있는 아이템들의 유니크와 갯수
					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	{
						matitemEndnum[matnum]= siMaterialNum;
						siNum = matitemEndnum[matnum];}
					else{
						matitemEndnum[matnum]= siNum;
					}
					

					if (matnum == 0) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(itemnum);//itemnum);
						break;
					}
					if (matnum == 1) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(itemnum);
						break;
					}
					if (matnum == 2) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(itemnum);
						break;
					}
					if (matnum == 3) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(itemnum);
						break;
					}
					if (matnum == 4) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(itemnum);
						break;
					}
					if (matnum == 5) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(itemnum);
						break;
					}				
				}
			}
		}
	}
	Show(true);

	// 춘기 - 신년이벤트
	if( pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREEBASE")))
	{
		pEventStruct = NULL;
		pEventStruct = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_NEWYEARTREEBASE);
		if(NULL == pEventStruct)
			return;

		EventTreeType = EVENT_STRUCTURE_NEWYEARTREEBASE;
		cltGameMsgRequest_Event_MaterialList clMaterialList(siVillageUnique,EVENT_STRUCTURE_NEWYEARTREEBASE);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);//리스트를 업데이트 하기위한 패킷

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventStruct->clMaterial[i];

			if(pclMaterialInfo == NULL)												continue;
			SI16 siUnique = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				
				//재료아이템유니크와 재료 아이템 갯수를 배열에 저장원 재료 아이템.
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
				//아이템의 이름을 가지고 온다.
			}
		}//재료아이템 반복문의 끝.
		//재료아이템 이름 설정
		const TCHAR* pitemname1 = pclClient->pclItemManager->GetName(matitemunique[0]);
		const TCHAR* pitemname2 = pclClient->pclItemManager->GetName(matitemunique[1]);
		const TCHAR* pitemname3 = pclClient->pclItemManager->GetName(matitemunique[2]);
		const TCHAR* pitemname4 = pclClient->pclItemManager->GetName(matitemunique[3]);
		const TCHAR* pitemname5 = pclClient->pclItemManager->GetName(matitemunique[4]);
		const TCHAR* pitemname6 = pclClient->pclItemManager->GetName(matitemunique[5]);
		//아이템 이름 설정
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->SetText((TCHAR*)pitemname1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->SetText((TCHAR*)pitemname2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->SetText((TCHAR*)pitemname3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->SetText((TCHAR*)pitemname4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->SetText((TCHAR*)pitemname5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->SetText((TCHAR*)pitemname6);
		//스크롤바 옵션이름에 보이지 않도록
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->ShowScrollBar(false);

		int		invennum, matnum;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0;matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];//재료 아이템
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siItemNum;//실제 가지고 있는 아이템들의 유니크와 갯수
					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	{
						matitemEndnum[matnum]= siMaterialNum;
						siNum = matitemEndnum[matnum];}
					else{
						matitemEndnum[matnum]= siNum;
					}


					if (matnum == 0) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(itemnum);//itemnum);
						break;
					}
					if (matnum == 1) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(itemnum);
						break;
					}
					if (matnum == 2) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(itemnum);
						break;
					}
					if (matnum == 3) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(itemnum);
						break;
					}
					if (matnum == 4) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(itemnum);
						break;
					}
					if (matnum == 5) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(itemnum);
						break;
					}				
				}
			}
		}
	}

	if(pEventNow/*크리스마스 트리일때*/
		&&pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")))
	{
		EventTreeType = EVENT_STRUCTURE_CHRISTMASTREE;
		cltGameMsgRequest_Event_MaterialList clMaterialList(siVillageUnique,EVENT_STRUCTURE_CHRISTMASTREE);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);//리스트를 업데이트 하기위한 패킷

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventNow->clMaterial[i];

			if(pclMaterialInfo == NULL)												continue;
			SI16 siUnique = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				
				//해당 아이템과 해당아이템갯수를 순차적으로 배열에 저장.;
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
				//아이템의 이름을 가지고 온다.
			}
		}//재료아이템 반복문의 끝.
		//아이템 이름 설정
		const TCHAR* pitemname1 = pclClient->pclItemManager->GetName(matitemunique[0]);
		const TCHAR* pitemname2 = pclClient->pclItemManager->GetName(matitemunique[1]);
		const TCHAR* pitemname3 = pclClient->pclItemManager->GetName(matitemunique[2]);
		const TCHAR* pitemname4 = pclClient->pclItemManager->GetName(matitemunique[3]);
		const TCHAR* pitemname5 = pclClient->pclItemManager->GetName(matitemunique[4]);
		const TCHAR* pitemname6 = pclClient->pclItemManager->GetName(matitemunique[5]);

		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->SetText((TCHAR*)pitemname1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->SetText((TCHAR*)pitemname2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->SetText((TCHAR*)pitemname3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->SetText((TCHAR*)pitemname4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->SetText((TCHAR*)pitemname5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->SetText((TCHAR*)pitemname6);

		//스크롤바 옵션이름에 보이지 않도록
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->ShowScrollBar(false);


		int		invennum, matnum;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0;matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siUnique;//실제 가지고 있는 아이템들의 유니크와 갯수
					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	{
						matitemEndnum[matnum]= siMaterialNum;
						siNum = matitemEndnum[matnum];}
					else{
						matitemEndnum[matnum]= siNum;
					}


					if (matnum == 0) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(itemnum);//itemnum);
						break;
					}
					if (matnum == 1) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(itemnum);
						break;
					}
					if (matnum == 2) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(itemnum);
						break;
					}
					if (matnum == 3) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(itemnum);
						break;
					}
					if (matnum == 4) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(itemnum);
						break;
					}
					if (matnum == 5) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(itemnum);
						break;
					}				
				}
			}
		}
	}

	// [춘기] 크리스마스 이벤트 - 엔펠리스 앞에 있는 크리스 마스 트리 (09.10.21)
	if(pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE01"))
		|| pclClient->pclCM->CR[id]->GetKind() == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASWORLDTREE02")))
	{
		pEventNow = NULL;
		pEventNow = pclClient->pclEventStructure->GetEventStructure(EVENT_STRUCTURE_WORLDCHRISTMASTREE);
		if(NULL == pEventNow)
			return;

		EventTreeType = EVENT_STRUCTURE_WORLDCHRISTMASTREE;
		cltGameMsgRequest_Event_MaterialList clMaterialList(VILLAGE_NONE, EVENT_STRUCTURE_WORLDCHRISTMASTREE);
		cltMsg clMaterialListMsg(GAMEMSG_REQUEST_EVENT_MATERIALLIST, sizeof(cltGameMsgRequest_Event_MaterialList), (BYTE*)&clMaterialList);
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialListMsg);//리스트를 업데이트 하기위한 패킷

		for(int i=0; i<MAX_MATERIAL_NUMBER; i++)
		{
			cltMaterialInfo* pclMaterialInfo = &pEventNow->clMaterial[i];

			if(pclMaterialInfo == NULL)												continue;
			SI16 siUnique = pclMaterialInfo->siUnique;
			SI16 siNeedNum = pclMaterialInfo->siNeedNum;
			if(siUnique <= 0 || siNeedNum <= 0)	continue;

			cltItem clItemMaterial;
			clItemMaterial.Init();

			SI16 brareswitch = 0;
			if(pclClient->pclItemManager->MakeRandItemUnique( pclMaterialInfo->siUnique, &clItemMaterial, 0, 0, &brareswitch,0,0,0,0,0,0) == true)//아이템의 재료를 구해온다.
			{
				SI32 siControlID = NCHRISTMASEVENT_LISTVIEW_ITEM1 + i;
				if(siControlID <= NCHRISTMASEVENT_LISTVIEW_ITEM6)
				{
					clItemMaterial.SetItemNum( pclMaterialInfo->siNeedNum );

					char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
					char *ItemName = (char*)pclClient->pclItemManager->GetName(pclMaterialInfo->siUnique);
					pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclMaterialInfo->siUnique, &GlobalImageIndex, &GlobalImageFont );
					pclClient->pclItemManager->GetExplain(&clItemMaterial, ToolTipMsg);
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 ,pclMaterialInfo->siNeedNum, ItemName, ToolTipMsg );
					CListView* ItemImage1 = m_InterfaceFactory.GetListView(siControlID);
					ItemImage1->InsertItem(&ListViewItemData);
				}				
				//해당 아이템과 해당아이템갯수를 순차적으로 배열에 저장.;
				matitemunique[i]=pclMaterialInfo->siUnique;
				matitemnum[i]= pclMaterialInfo->siNeedNum;
				//아이템의 이름을 가지고 온다.
			}
		}//재료아이템 반복문의 끝.
		//아이템 이름 설정
		const TCHAR* pitemname1 = pclClient->pclItemManager->GetName(matitemunique[0]);
		const TCHAR* pitemname2 = pclClient->pclItemManager->GetName(matitemunique[1]);
		const TCHAR* pitemname3 = pclClient->pclItemManager->GetName(matitemunique[2]);
		const TCHAR* pitemname4 = pclClient->pclItemManager->GetName(matitemunique[3]);
		const TCHAR* pitemname5 = pclClient->pclItemManager->GetName(matitemunique[4]);
		const TCHAR* pitemname6 = pclClient->pclItemManager->GetName(matitemunique[5]);

		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->SetText((TCHAR*)pitemname1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->SetText((TCHAR*)pitemname2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->SetText((TCHAR*)pitemname3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->SetText((TCHAR*)pitemname4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->SetText((TCHAR*)pitemname5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->SetText((TCHAR*)pitemname6);

		//스크롤바 옵션이름에 보이지 않도록
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME1)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME2)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME3)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME4)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME5)->ShowScrollBar(false);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMNAME6)->ShowScrollBar(false);


		int		invennum, matnum;
		// 재료 갯수와 현제 소유 아이템 갯수를 비교 한다.
		for( matnum = 0;matnum < MAX_MATERIAL_NUMBER; matnum++)
		{
			SI32 siMaterialUnique	= matitemunique[matnum];
			SI32 siMaterialNum		= matitemnum[matnum];

			if( siMaterialUnique <= 0 || siMaterialNum <= 0 )		continue;

			// 인벤에서 해당 재료를 찾는다. 
			for( invennum = PERSONITEM_INV0; invennum < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; invennum++ )
			{
				cltItem* pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[invennum];
				if(pclItem == NULL)					continue;

				if( pclItem->siUnique == siMaterialUnique)
				{
					matitemEndunique[matnum] = pclItem->siUnique;//실제 가지고 있는 아이템들의 유니크와 갯수
					SI32 siNum = pclItem->siItemNum;
					if(siNum > siMaterialNum)	{
						matitemEndnum[matnum]= siMaterialNum;
						siNum = matitemEndnum[matnum];}
					else{
						matitemEndnum[matnum]= siNum;
					}


					if (matnum == 0) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(itemnum);//itemnum);
						break;
					}
					if (matnum == 1) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(itemnum);
						break;
					}
					if (matnum == 2) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(itemnum);
						break;
					}
					if (matnum == 3) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(itemnum);
						break;
					}
					if (matnum == 4) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(itemnum);
						break;
					}
					if (matnum == 5) {
						TCHAR itemnum[128]= TEXT("") ;
						StringCchPrintf(itemnum, 128, TEXT("%d"), siNum);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(itemnum);
						break;
					}				
				}
			}
		}
	}
}

void CALLBACK NChristmasEventDlg ::StaticSelectChristmasEventProc(CControllerMgr *pControllerMgr,UINT nEvent, int nControlID,CControllerObj *pControl)
{
	NChristmasEventDlg *pThis = (NChristmasEventDlg*)pControllerMgr;
	pThis->NSelectChristmasEventProc(nEvent, nControlID, pControl);
}


void CALLBACK NChristmasEventDlg ::NSelectChristmasEventProc(UINT nEvent, int nControlID,CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 siVillageUnique = pclClient->pclCM->CR[1]->GetCurrentVillageUnique();

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			if (EVENT_BUTTON_CLICKED == nEvent)
			{
				DeleteDialog();
			}
		}
		break;
	case NCHRISTMASEVENT_EDIT_ITEMMNUM1:
	case NCHRISTMASEVENT_EDIT_ITEMMNUM2:
	case NCHRISTMASEVENT_EDIT_ITEMMNUM3:
	case NCHRISTMASEVENT_EDIT_ITEMMNUM4:
	case NCHRISTMASEVENT_EDIT_ITEMMNUM5:
	case NCHRISTMASEVENT_EDIT_ITEMMNUM6:
		{
			if (EVENT_EDITBOX_CHANGE == nEvent) {//10000이상 적지 못하도록 원래만개를 넣었지만 변경. 현재 필요재료 의 최대 값을 넣는다,
				//먼저 현제 남은 재료의 최대 필요 갯수를 찾아 온다(누적양을 뺀  최대 갯수)
				TCHAR* MaxMatNum1 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX1)->GetText();
				TCHAR* MaxMatNum2 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX2)->GetText();
				TCHAR* MaxMatNum3 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX3)->GetText();
				TCHAR* MaxMatNum4 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX4)->GetText();
				TCHAR* MaxMatNum5 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX5)->GetText();
				TCHAR* MaxMatNum6 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX6)->GetText();
				//필요최대갯수에 사용할 변수 초기화
				SI16 NeedMaxItemNum1 = 0;
				SI16 NeedMaxItemNum2 = 0;
				SI16 NeedMaxItemNum3 = 0;
				SI16 NeedMaxItemNum4 = 0;
				SI16 NeedMaxItemNum5 = 0;
				SI16 NeedMaxItemNum6 = 0;
				//케릭터 형이므로 정수형으로 변환한다.
				NeedMaxItemNum1 = atoi(MaxMatNum1);
				NeedMaxItemNum2 = atoi(MaxMatNum2);
				NeedMaxItemNum3 = atoi(MaxMatNum3);
				NeedMaxItemNum4 = atoi(MaxMatNum4);
				NeedMaxItemNum5 = atoi(MaxMatNum5);
				NeedMaxItemNum6 = atoi(MaxMatNum6);
				//유저가 넣은 갯수를 가저온다.
				TCHAR* Num1 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->GetText();
				TCHAR* Num2 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->GetText();
				TCHAR* Num3 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->GetText();
				TCHAR* Num4 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->GetText();
				TCHAR* Num5 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->GetText();
				TCHAR* Num6 = m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->GetText();
				//정수형변수로 사용할 초기화

				SI16 ItemNum1 = 0;
				SI16 ItemNum2 = 0;
				SI16 ItemNum3 = 0;
				SI16 ItemNum4 = 0;
				SI16 ItemNum5 = 0;			
				SI16 ItemNum6 = 0;	
				//유저가 넣을 재료 갯수가 케릭터 형이므로 정수형으로 변환
				ItemNum1 = atoi(Num1);
				ItemNum2 = atoi(Num2);
				ItemNum3 = atoi(Num3);
				ItemNum4 = atoi(Num4);
				ItemNum5 = atoi(Num5);
				ItemNum6 = atoi(Num6);
				//현제 인벤토리에 가지고 있는 아이템 갯수
				SI16 MaxMatItemNum1 = matitemEndnum[0];
				SI16 MaxMatItemNum2 = matitemEndnum[1];
				SI16 MaxMatItemNum3 = matitemEndnum[2];
				SI16 MaxMatItemNum4 = matitemEndnum[3];
				SI16 MaxMatItemNum5 = matitemEndnum[4];
				SI16 MaxMatItemNum6 = matitemEndnum[5];
				//졍수 형이므로 케릭터형으로 변환
				TCHAR LastTTputMatNum1[10] = "";
				TCHAR LastTTputMatNum2[10] = "";
				TCHAR LastTTputMatNum3[10] = "";
				TCHAR LastTTputMatNum4[10] = "";
				TCHAR LastTTputMatNum5[10] = "";
				TCHAR LastTTputMatNum6[10] = "";
				//현제 인벤토리에 가지고 있는 최대 갯수
				itoa(MaxMatItemNum1, LastTTputMatNum1, 10);
				itoa(MaxMatItemNum2, LastTTputMatNum2, 10);
				itoa(MaxMatItemNum3, LastTTputMatNum3, 10);
				itoa(MaxMatItemNum4, LastTTputMatNum4, 10);
				itoa(MaxMatItemNum5, LastTTputMatNum5, 10);
				itoa(MaxMatItemNum6, LastTTputMatNum6, 10);
                //에디트 상자에 적용한다
				if (ItemNum1 >= NeedMaxItemNum1 
					&& MaxMatItemNum1<=NeedMaxItemNum1)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(LastTTputMatNum1);
				else if (ItemNum1 >= NeedMaxItemNum1 
					&& MaxMatItemNum1>NeedMaxItemNum1)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(MaxMatNum1);
				else if (ItemNum1 < NeedMaxItemNum1
					&& ItemNum1>= MaxMatItemNum1)		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(LastTTputMatNum1);
				
				if (ItemNum2 >= NeedMaxItemNum2 
					&& MaxMatItemNum2<=NeedMaxItemNum2)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(LastTTputMatNum2);
				else if (ItemNum2 >= NeedMaxItemNum2 
					&& MaxMatItemNum2>NeedMaxItemNum2)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(MaxMatNum2);
				else if (ItemNum2 < NeedMaxItemNum2
					&& ItemNum2>= MaxMatItemNum2)		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(LastTTputMatNum2);
				
				if (ItemNum3 >= NeedMaxItemNum3 
					&& MaxMatItemNum1<=NeedMaxItemNum1)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(LastTTputMatNum3);
				else if (ItemNum3 >= NeedMaxItemNum3 
					&& MaxMatItemNum3>NeedMaxItemNum3)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(MaxMatNum3);
				else if (ItemNum3 < NeedMaxItemNum3
					&& ItemNum3>= MaxMatItemNum3)		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(LastTTputMatNum3);
				
				if (ItemNum4 >= NeedMaxItemNum4
					&& MaxMatItemNum4<=NeedMaxItemNum4)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(LastTTputMatNum4);
				else if (ItemNum4 >= NeedMaxItemNum4 
					&& MaxMatItemNum4>NeedMaxItemNum4)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(MaxMatNum4);
				else if (ItemNum4 < NeedMaxItemNum4
					&& ItemNum4>= MaxMatItemNum4)		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(LastTTputMatNum4);
				
				if (ItemNum5 >= NeedMaxItemNum5
					&& MaxMatItemNum5<=NeedMaxItemNum5)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(LastTTputMatNum5);
				else if (ItemNum5 >= NeedMaxItemNum5 
					&& MaxMatItemNum5>NeedMaxItemNum5)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(MaxMatNum5);
				else if (ItemNum5 < NeedMaxItemNum5
					&& ItemNum5>= MaxMatItemNum5)		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(LastTTputMatNum5);
				
				if (ItemNum6 >= NeedMaxItemNum6 
					&& MaxMatItemNum6<=NeedMaxItemNum6)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(LastTTputMatNum6);
				else if (ItemNum6 >= NeedMaxItemNum6 
					&& MaxMatItemNum6>NeedMaxItemNum6)	m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(MaxMatNum6);
				else if (ItemNum6 < NeedMaxItemNum6
					&& ItemNum6>= MaxMatItemNum6)		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(LastTTputMatNum6);
			}
		}
		break;
	case NCHRISTMASEVENT_BUTTON_DONATIONOFMATERIAL://재료 넣기 버튼.
		{
			if (EVENT_BUTTON_CLICKED == nEvent)
			{
				//같은 마을 사람만 재료 등록.
				// [춘기] ConstructType이 월드트리이면 주민여부 무시한다. (09.10.23)
				if(EventTreeType != EVENT_STRUCTURE_WORLDCHRISTMASTREE)
				{
					if(pclClient->pclCM->CR[1]->GetCurrentVillageUnique() != pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage)
					{
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(8464));
						return;
					}
				}

				//현제는 재료 6개만 받도록 하였다.차후 슬롯만 늘리고 MAX_MATERIAL_NUMBER만 늘려 나간다면 
				//다른 이벤트로서 이용도 가능하다.
				SI16	slot1 =_tstoi( m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->GetText());//TCHAR* 형을 16으로 가저온다
				SI16	slot2 =_tstoi( m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->GetText());//현제 이벤토리에 가지고 있는 갯수
				SI16	slot3 =_tstoi( m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->GetText());
				SI16	slot4 =_tstoi( m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->GetText());
				SI16	slot5 =_tstoi( m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->GetText());
				SI16	slot6 =_tstoi( m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->GetText());
				if (slot1 == 0 && slot2 == 0 && slot3 == 0 && slot4 == 0 && slot5 == 0 && slot6 == 0) {
					cltClient *pclclient = (cltClient *)pclClient;
					TCHAR* pText;
					TCHAR* pTitle;
					pText = GetTxtFromMgr(7700);//모든 수가 0이거나 빈자리가 있을때 나오는 설명
					pTitle = GetTxtFromMgr(7651);//모든 수가 0이거나 빈자리가 있을때 나오는 설명
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText);
					DeleteDialog();
					break;
				}
				SI16	itemnum[MAX_MATERIAL_NUMBER] = {slot1,slot2,slot3,slot4,slot5,slot6, 0};//현제는 6개까지만 보내지만 차후 슬롯만 늘리면됨.

				// [춘기] 엔펠리스앞 월드 트리인 경우에는 마을유니크 없이 재료를 넣는다.
				if(EventTreeType == EVENT_STRUCTURE_WORLDCHRISTMASTREE)
				{
					//재료를 날린다
					cltGameMsgRequest_Event_InsertMaterial clMaterial(VILLAGE_NONE, EventTreeType, matitemunique, itemnum, NULL);
					cltMsg clMaterialMsg(GAMEMSG_REQUEST_EVENT_INSERTMATERIAL,sizeof(cltGameMsgRequest_Event_InsertMaterial), (BYTE*)&clMaterial);
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialMsg);
				}
				else
				{
					//재료를 날린다
					cltGameMsgRequest_Event_InsertMaterial clMaterial(siVillageUnique, EventTreeType, matitemunique, itemnum, NULL);
					cltMsg clMaterialMsg(GAMEMSG_REQUEST_EVENT_INSERTMATERIAL,sizeof(cltGameMsgRequest_Event_InsertMaterial), (BYTE*)&clMaterial);
					pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMaterialMsg);
				}


				DeleteDialog();
			}
		}
		break;
	case NCHRISTMASEVENT_BUTTON_MAKETREE :// 크리스마스 트리 만들기 버튼을 누를때
		{
			if (EVENT_BUTTON_CLICKED == nEvent)
			{
				// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
				{
					cltSimpleRank clrank(pclClient->pclCM->CR[1]->pclCI->clCharRank.siType ,siVillageUnique);

					if (pclClient->pclCM->CR[1]->pclCI->clCharRank.siType == 1 &&
						pclClient->pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == TRUE) 
					{
						cltGameMsgRequest_Event_MaterialComplete cllChangeTree( siVillageUnique, EventTreeType );//크리스마스 트리 생성 메세지를 날린다. 
						cltMsg clChangetreeMsg( GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE, sizeof(cltGameMsgRequest_Event_MaterialComplete), (BYTE*)&cllChangeTree );
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clChangetreeMsg);
					}
					else
					{
						TCHAR* pText;
						pText = GetTxtFromMgr(7658);
						m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_EXPLAIN)->SetText(pText);				
						return;
					}
				}
				//DoMsg_GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE 패킷을 사용 한다.
				//크리스마스 트리 변경및 이벤트 완성 아이템을 받을때 이 패킷 이용한다.
				//SI32 PineTreeKind = pclClient->GetUniqueFromHash(TEXT("KIND_PINE_TREE"));//기존의 변신전 나무
				//SI32 ChristmastreeKind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"));//크리스마스 트리 카인드 찾기.
				//이걸로 구분은 하면 될듯 EVENT_STRUCTURE_TREE, EVENT_STRUCTURE_CHRISTMASTREE
				//완성 했다고 트리변경 메세지를 날린다.				
				/*나무를 만들자는 패킷을 보내고는 다이얼로그를 사라지게 한다. 테스트 시에는 주석으로 처리하고
				실제 나갈때는 풀어서 나가야함.*/
				DeleteDialog();
			}
		}
		break;
	case NCHRISTMASEVENT_BUTTON_HAVEITEM :// 크리스마스 트리 버튼이  사라지고 아이템을 받는 버튼이 나왔을때.
		{
			if (EVENT_BUTTON_CLICKED == nEvent)
			{
				//아이템빈공간 검색
				SI16 i;
				cltItem clitem;
				SI32 rtnvalue;
				if(pclClient->pclCM->CR[1]->pclCI->clCharItem.CanAddInv(pclClient->pclItemManager, INVMODE_ALL,
						&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], NULL, &i, &clitem, &rtnvalue)== false)
				{//인벤토리에 아이템 널을 자리가 없다고 표시
					TCHAR* pTitle = GetTxtFromMgr(422);
					TCHAR* pText = GetTxtFromMgr(6981);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
					DeleteDialog();
				}

				//DoMsg_GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE 패킷을 사용 한다.
				//크리스마스 트리 변경및 이벤트 완성 아이템을 받을때 이 패킷 이용한다.
				//SI32 PineTreeKind = pclClient->GetUniqueFromHash(TEXT("KIND_PINE_TREE"));//기존의 변신전 나무
				//SI32 ChristmastreeKind = pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE"));//크리스마스 트리 카인드 찾기.
				//이걸로 구분은 하면 될듯 EVENT_STRUCTURE_TREE, EVENT_STRUCTURE_CHRISTMASTREE
				//완성 했다고 트리변경 메세지를 날린다.
				cltGameMsgRequest_Event_MaterialComplete cllChangeTree( siVillageUnique,EventTreeType/*EVENT_STRUCTURE_TREE*/ );//크리스마스 트리 생성 메세지를 날린다. 
				cltMsg clHAveItemMsg( GAMEMSG_REQUEST_EVENT_MATERIALCOMPLETE, sizeof(cltGameMsgRequest_Event_MaterialComplete), (BYTE*)&cllChangeTree );
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clHAveItemMsg);
				/*나무를 만들자는 패킷을 보내고는 다이얼로그를 사라지게 한다. 테스트 시에는 주석으로 처리하고
				실제 나갈때는 풀어서 나가야함.*/
				DeleteDialog();
			}
		}
		break;
	}
}
void NChristmasEventDlg::changeEditbox(SI16 *itemUnique, SI16* sitemNum, SI16 m_siConstructType)
{
	cltClient *pclclient = (cltClient *)pclClient;
	cltEventStructureUnit* pEventPine = pclClient->pclEventStructure->GetEventStructure(m_siConstructType);
	cltEventStructureUnit* pEventChrisrmas = pclClient->pclEventStructure->GetEventStructure(m_siConstructType);
	SI32 id =pclclient->pclUnderMouseObject->GetUnderMouseID();//마우스 아래 있는 것 카인드값을 가저 온다.
	if (   m_siConstructType == EVENT_STRUCTURE_TREE 
		|| m_siConstructType == EVENT_STRUCTURE_HERO_BRONZE 
		|| m_siConstructType == EVENT_STRUCTURE_HALLOWEEN
		|| m_siConstructType == EVENT_STRUCTURE_CHRISTMASTREEBASE
		|| m_siConstructType == EVENT_STRUCTURE_NEWYEARTREEBASE) 
	{
		//일단은 노가다로..덜덜덜...
		TCHAR Incomeitem1[10] = "" ;
		TCHAR Incomeitem2[10] = "" ;
		TCHAR Incomeitem3[10] = "" ;
		TCHAR Incomeitem4[10] = "" ;
		TCHAR Incomeitem5[10] = "" ;
		TCHAR Incomeitem6[10] = "" ;

		SI16 ItemNum1 = matitemnum[0]-sitemNum[0];
		SI16 ItemNum2 = matitemnum[1]-sitemNum[1];
		SI16 ItemNum3 = matitemnum[2]-sitemNum[2];
		SI16 ItemNum4 = matitemnum[3]-sitemNum[3];
		SI16 ItemNum5 = matitemnum[4]-sitemNum[4];
		SI16 ItemNum6 = matitemnum[5]-sitemNum[5];

		if (ItemNum1 <= 0)	ItemNum1 = 0;
		if (ItemNum2 <= 0)	ItemNum2 = 0;
		if (ItemNum3 <= 0)	ItemNum3 = 0;
		if (ItemNum4 <= 0)	ItemNum4 = 0;
		if (ItemNum5 <= 0)	ItemNum5 = 0;
		if (ItemNum6 <= 0)	ItemNum6 = 0;

		if(ItemNum1 == 0 && ItemNum2 == 0 && ItemNum3 == 0 && ItemNum4 == 0 && ItemNum5 == 0 && ItemNum6 == 0)
		{
			m_InterfaceFactory.GetButton(NCHRISTMASEVENT_BUTTON_MAKETREE)->Enable(true);//현제누적이 꽉 차서 0이 되면 활성화 시킨다
		}

		// 현재 채워져 있는 개수
		itoa(ItemNum1,Incomeitem1 ,10);
		itoa(ItemNum2,Incomeitem2 ,10);
		itoa(ItemNum3,Incomeitem3 ,10);
		itoa(ItemNum4,Incomeitem4 ,10);
		itoa(ItemNum5,Incomeitem5 ,10);
		itoa(ItemNum6,Incomeitem6 ,10);

		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX1)->SetText(Incomeitem1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX2)->SetText(Incomeitem2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX3)->SetText(Incomeitem3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX4)->SetText(Incomeitem4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX5)->SetText(Incomeitem5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX6)->SetText(Incomeitem6);

		// 유저가 넣을 개수
		if(ItemNum1 > matitemEndnum[0])		ItemNum1 = matitemEndnum[0];
		if(ItemNum2 > matitemEndnum[1])		ItemNum2 = matitemEndnum[1];
		if(ItemNum3 > matitemEndnum[2])		ItemNum3 = matitemEndnum[2];
		if(ItemNum4 > matitemEndnum[3])		ItemNum4 = matitemEndnum[3];
		if(ItemNum5 > matitemEndnum[4])		ItemNum5 = matitemEndnum[4];
		if(ItemNum6 > matitemEndnum[5])		ItemNum6 = matitemEndnum[5];

		itoa(ItemNum1,Incomeitem1 ,10);
		itoa(ItemNum2,Incomeitem2 ,10);
		itoa(ItemNum3,Incomeitem3 ,10);
		itoa(ItemNum4,Incomeitem4 ,10);
		itoa(ItemNum5,Incomeitem5 ,10);
		itoa(ItemNum6,Incomeitem6 ,10);

		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(Incomeitem1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(Incomeitem2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(Incomeitem3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(Incomeitem4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(Incomeitem5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(Incomeitem6);

		return;
	}
	else if(m_siConstructType == EVENT_STRUCTURE_WORLDCHRISTMASTREE)
	{
		TCHAR Incomeitem1[10] = "" ;
		TCHAR Incomeitem2[10] = "" ;
		TCHAR Incomeitem3[10] = "" ;
		TCHAR Incomeitem4[10] = "" ;
		TCHAR Incomeitem5[10] = "" ;
		TCHAR Incomeitem6[10] = "" ;

		SI16 ItemNum1 = matitemnum[0]-sitemNum[0];
		SI16 ItemNum2 = matitemnum[1]-sitemNum[1];
		SI16 ItemNum3 = matitemnum[2]-sitemNum[2];
		SI16 ItemNum4 = matitemnum[3]-sitemNum[3];
		SI16 ItemNum5 = matitemnum[4]-sitemNum[4];
		SI16 ItemNum6 = matitemnum[5]-sitemNum[5];

		if (ItemNum1 <= 0)	ItemNum1 = 0;
		if (ItemNum2 <= 0)	ItemNum2 = 0;
		if (ItemNum3 <= 0)	ItemNum3 = 0;
		if (ItemNum4 <= 0)	ItemNum4 = 0;
		if (ItemNum5 <= 0)	ItemNum5 = 0;
		if (ItemNum6 <= 0)	ItemNum6 = 0;

		// 현재 채워져 있는 개수
		itoa(ItemNum1,Incomeitem1 ,10);
		itoa(ItemNum2,Incomeitem2 ,10);
		itoa(ItemNum3,Incomeitem3 ,10);
		itoa(ItemNum4,Incomeitem4 ,10);
		itoa(ItemNum5,Incomeitem5 ,10);
		itoa(ItemNum6,Incomeitem6 ,10);

		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX1)->SetText(Incomeitem1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX2)->SetText(Incomeitem2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX3)->SetText(Incomeitem3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX4)->SetText(Incomeitem4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX5)->SetText(Incomeitem5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMAX6)->SetText(Incomeitem6);

		// 유저가 넣을 개수
		if(ItemNum1 > matitemEndnum[0])		ItemNum1 = matitemEndnum[0];
		if(ItemNum2 > matitemEndnum[1])		ItemNum2 = matitemEndnum[1];
		if(ItemNum3 > matitemEndnum[2])		ItemNum3 = matitemEndnum[2];
		if(ItemNum4 > matitemEndnum[3])		ItemNum4 = matitemEndnum[3];
		if(ItemNum5 > matitemEndnum[4])		ItemNum5 = matitemEndnum[4];
		if(ItemNum6 > matitemEndnum[5])		ItemNum6 = matitemEndnum[5];

		itoa(ItemNum1,Incomeitem1 ,10);
		itoa(ItemNum2,Incomeitem2 ,10);
		itoa(ItemNum3,Incomeitem3 ,10);
		itoa(ItemNum4,Incomeitem4 ,10);
		itoa(ItemNum5,Incomeitem5 ,10);
		itoa(ItemNum6,Incomeitem6 ,10);

		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM1)->SetText(Incomeitem1);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM2)->SetText(Incomeitem2);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM3)->SetText(Incomeitem3);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM4)->SetText(Incomeitem4);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM5)->SetText(Incomeitem5);
		m_InterfaceFactory.GetEdit(NCHRISTMASEVENT_EDIT_ITEMMNUM6)->SetText(Incomeitem6);

		return;
	}
}