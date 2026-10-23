#ifndef _DIALOGCONTROLLER_H_
#define _DIALOGCONTROLLER_H_

#include "ControllerMgr.h"
#include "Interface/Button.h"
#include "Interface/ChatEdit.h"
#include "Interface/CheckBox.h"
#include "Interface/Edit.h"
#include "Interface/ImageStatic.h"
#include "Interface/List.h"
#include "Interface/ListView.h"
#include "Interface/OutLine.h"
#include "Interface/RadioButton.h"
#include "Interface/Static.h"
#include "Interface/Tree.h"
#include "Interface/ComboBox.h"
#include <map>

#define _NEW_INTERFACE_  //[영진] 새로운 인터페이스 테스트용 정의. 2006년 2월 이후 지워주삼
#define SNAPRANGE 15	//[영진] 새로운m 인퍼페이스의 스냅 범위 (픽셀)

class CDialogController : public CControllerMgr
{
public:
	CDialogController();
	virtual ~CDialogController();


	virtual void Create()	{};
	
	void Create( SI16 DialogControllerID, SI16 ControllerKind, CControllerMgr *pParent, GUICallBackEvent pCallBack, TCHAR *Title, TCHAR *FileName, SI16 FontIndex, bool BKTileImg, SI16 BKTileNum, SI16 XPos, SI16 YPos, SI16 Width, SI16 Height, bool Movable = true, bool HaveCaption = true, bool HaveCloseButton = true , bool SavePosToFile = true ); //, bool HaveMinimizeButton = false );
	void SavePosToFile();
	void ReSizeDialog( SI16 XPos, SI16 YPos, SI16 Width, SI16 Height );
	

	virtual void SetData( void *pBuffer )		{};
	virtual BYTE *GetData()						{ return NULL; };
	
	virtual void SetText( TCHAR *Title, TCHAR *strPrintData, ... )	{};
	/*
	virtual void SetText( TCHAR *Text, SI16 Param1, SI16 Param2 )	{};
	*/
protected:
//	CButton *m_pMinimizeButton;
	CButton *m_pCloseButton;				// CloseButton
	BOOL	m_bSavePosToFile;
private:
		
};


//***************************************************************************************************
//
// Class Name 		: InterfaceFactory
//
// Last Modified 	: 2005 / 12 / 25 
// Created		 	: 김광명
//
// Function			: 인터페이스 팩토
//
//***************************************************************************************************
class CButton;
class CStatic;
class CComboBox;
class CImageStatic;
class CTree;
class COutLine;
class CListView;
class CRadioButton;
class CList;
class CChatEdit;
class CCheckBox;
class CEdit;

typedef std::map<SI32, CControllerObj*>  MapInterface;
typedef MapInterface::iterator			  IterMapInterface;

#define BUTTON		0
#define STATIC		1
#define COMBOBOX	2
#define IMAGESTATIC	3
#define TREE		4
#define OUTLINE		5
#define LISTVIEW	6
#define RADIOBUTTON	7
#define LIST		8
#define CHATEDIT	9
#define CHECKBOX	10
#define EDIT		11

class InterfaceFactory
{
public:

	MapInterface m_mapInterfaceFactory_;

	InterfaceFactory()
	{
	}

	~InterfaceFactory()
	{
		for( IterMapInterface i = m_mapInterfaceFactory_.begin(); i != m_mapInterfaceFactory_.end(); ++ i )
		{
			SAFE_DELETE(i->second);
		}
		m_mapInterfaceFactory_.clear();
	}


	CButton*		GetButton( SI32 TypeID );
	CStatic*		GetStatic( SI32 TypeID );
	CComboBox*		GetComboBox( SI32 TypeID );
	CImageStatic*	GetImageStatic( SI32 TypeID );
	CTree*			GetTree( SI32 TypeID );
	COutLine*		GetOutLine( SI32 TypeID );
	CListView*		GetListView( SI32 TypeID );
	CRadioButton*	GetRadioButton( SI32 TypeID );
	CList*			GetList( SI32 TypeID );
	CChatEdit*		GetChatEdit( SI32 TypeID );
	CCheckBox*		GetCheckBox( SI32 TypeID );
	CEdit*			GetEdit( SI32 TypeID );
	void*			GetControl( SI32 TypeID );

	CControllerObj* CreateUserInterface( int Type, SI32 TypeID, CDialogController *pDlg )
	{
		switch( Type )
		{
		case BUTTON:
			{
				CControllerObj* pCControllerObj =  new CButton( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case STATIC:
			{
				CControllerObj* pCControllerObj =  new CStatic( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case COMBOBOX:
			{
				CControllerObj* pCControllerObj =  new CComboBox( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case IMAGESTATIC:
			{
				CControllerObj* pCControllerObj =  new CImageStatic( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case TREE:
			{
				CControllerObj* pCControllerObj =  new CTree( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case OUTLINE:
			{
				CControllerObj* pCControllerObj =  new COutLine( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case LISTVIEW:
			{
				CControllerObj* pCControllerObj =  new CListView( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case RADIOBUTTON:
			{
				CControllerObj* pCControllerObj =  new CRadioButton( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case LIST:
			{
				CControllerObj* pCControllerObj =  new CList( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case CHATEDIT:
			{
				CControllerObj* pCControllerObj =  new CChatEdit( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case CHECKBOX:
			{
				CControllerObj* pCControllerObj =  new CCheckBox( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		case EDIT:
			{
				CControllerObj* pCControllerObj =  new CEdit( pDlg );
				m_mapInterfaceFactory_.insert( MapInterface::value_type( TypeID, pCControllerObj ) );
				return pCControllerObj;
			}
			break;
		default:
			return NULL;
			break;
		}
	}
};


#endif