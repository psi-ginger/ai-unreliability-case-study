#ifndef _INTERFACEFILE_H
#define _INTERFACEFILE_H

#include "directives.h"


#define DEFAULT_DIALOGPOSX 150
#define DEFAULT_DIALOGPOSY 150


class CControllerObj;
class CControllerMgr;
class CDialogController;



typedef void (CALLBACK *GUICallBackEvent) ( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );

struct NFDialog
{
	TCHAR	title[ 64 ];
	bool	bTitleVisible;
	bool	bMovable;
	bool	bClosable;
};


struct NFButton
{
	int		groupnum;
	TCHAR	text[ 64 ];
	int		animation;
};

struct NFComboBox
{
	int		groupnum;
	int		popupHeight;
	int		lineHeight;
};

struct NFEditBox
{
	int		groupnum;
	TCHAR	text[ 64 ];
	bool	bMultiLine;
	bool	bReadOnly;
};

struct NFStaticText
{

	int		groupnum;
	TCHAR	text[ 64 ];
};

struct NFImageStatic
{
	int		groupnum;
};

struct NFListBox
{
	int		groupnum;
	bool	bMultiLine;
	bool	bMultiSelect;
	int		lineHeight;
	bool	bFixedRow;
};

struct NFListView
{
	int		groupnum;
	bool	bEmpty;
	bool	bCountVisible;
	bool	bUsable;

	int		tooltipHeight;
	int		tooltipWidth;

	int		startX;
	int		startY;

	int		offsetX;
	int		offsetY;

	int		itemsizeX;
	int		itemsizeY;
};

struct NFCheckBox
{
	int		groupnum;
	TCHAR	text[ 64 ];
};

struct NFRadioButton
{
	int		groupnum;
	int		groupid;
	TCHAR	text[ 64 ];
};

struct NFOutLine
{

	int		groupnum;
	TCHAR	text[ 64 ];
};

struct NFChatEdit
{
	int		groupnum;
	TCHAR	text[ 64 ];
	int		lineHeight;
	bool	bAutoScroll;
	int		offsetX;
	int		offsetY;
};


struct NFControl
{
	int		type;

	TCHAR	id[ 32 ];
	//	TCHAR	id[ 64 ];
	int		x;
	int		y;
	int		width;
	int		height;

	int		surfaceWidth;
	int		surfaceHeight;

	TCHAR	imageFilename[ 256 ];  // Full Path
	int		startFont;

	bool	bImageTile;
	int		imageTileType;

	TCHAR	textFont[ 32 ];
	int		textColor;
	int		textSize;

	union {
		NFDialog		dialog;
		NFButton		button;
		NFComboBox		combobox;
		NFEditBox		editbox;
		NFStaticText	statictext;
		NFImageStatic	imagestatic;
		NFListBox		listbox;
		NFListView		listview;
		NFCheckBox		checkbox;
		NFRadioButton	radiobutton;
		NFOutLine		outline;
		NFChatEdit		chatedit;
	};

};

class CInterfaceFile
{
public:
	CInterfaceFile();
	~CInterfaceFile();


public:
	bool			CreatePopupDialog( CDialogController* pDlg, SI16 id, TCHAR *szID, GUICallBackEvent pCallBack, BOOL readPosFromFile = true); //, BOOL haveMinimizeButton = FALSE );
	bool			CreateChildDialog( CDialogController* pDlg, SI16 id, TCHAR *szID, GUICallBackEvent pCallBack, CControllerMgr *Parent );

	bool			CreateControl( CControllerObj* pControl, SI16 id, TCHAR *szID, int index = 0 );
	bool			CreateControl( CControllerMgr *pParent, CControllerObj** pControlObj, SI16 id, TCHAR *szID, int index = 0 );

	bool			LoadFile( TCHAR *szFilename );

	int				GetControlIndex( TCHAR *szID );

	void			ReadControlInfo( TCHAR *szText );

	int				GetControlKind( TCHAR *szText );

	void			SetBaseInfo( int controlindex, int index, TCHAR *szText );

	void			SetDialogInfo( int controlindex, int index, TCHAR *szText );
	void			SetButtonInfo( int controlindex, int index, TCHAR *szText );
	void			SetComboBoxInfo( int controlindex, int index, TCHAR *szText );
	void			SetEditBoxInfo( int controlindex, int index, TCHAR *szText );
	void			SetStaticTextInfo( int controlindex, int index, TCHAR *szText );
	void			SetImageStaticInfo( int controlindex, int index, TCHAR *szText );
	void			SetListBoxInfo( int controlindex, int index, TCHAR *szText );
	void			SetListViewInfo( int controlindex, int index, TCHAR *szText );
	void			SetCheckBoxInfo( int controlindex, int index, TCHAR *szText );
	void			SetRadioButtonInfo( int controlindex, int index, TCHAR *szText );
	void			SetOutLineInfo( int controlindex, int index, TCHAR *szText );
	void			SetChatEditInfo( int controlindex, int index, TCHAR *szText );

	SI32			GetInfoWithStringID( TCHAR *szID );

	NFControl		m_control[ 100 ];
	int				m_controlNum;

	static bool		m_MustDownload;
	static SI32		m_ClientServiceCode;	// 클라이언트 서비스 지역
};


#endif
