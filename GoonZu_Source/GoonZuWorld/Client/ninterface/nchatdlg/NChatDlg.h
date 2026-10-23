//messagebox inserted by taeyang
#ifndef _NCHATDLG_H_
#define _NCHATDLG_H_

#include "../../InterfaceMgr/DialogController.h"

#include "../../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../../Common/Skill/Skill.h"

#include "../../NLib/NProfile.h"
#include "../../NLib/NBitArray.h"

class CChatStatusBarBuffer;
struct stChatStatusBarData;

class CButton;
class CComboBox;
class CStatic;
class CEdit;
class CChatEdit;

#define MSG_CHAT_RECV							12500
#define MSG_RESIDENTSCHAT_RECV					12505
#define MSG_PARTYCHAT_RECV						12510 
#define MSG_MASTERNOTIFYCHAT_RECV				12515
#define MSG_VILLAGENOTIFYCHAT_RECV				12520
#define MSG_RESIDENTSNOTIFYCHAT_RECV			12525
#define MSG_KINGNOTIFYCHAT_RECV					12530
#define MSG_GOODSPRODUCTIONNOTIFYCHAT_RECV		12535
#define MSG_GUILDCHAT_RECV						12540


class CNChatDlg : public CDialogController
{
public:
	CNChatDlg();
	~CNChatDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNChat( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNChat( UINT nEvent, int nControlID, CControllerObj* pControl );

	void CharacterLogin();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void ChatDataSend();
	void Push( SI16 ChatDataKind, TCHAR *ChatText );
	bool Pop( stChatStatusBarData * pChatStatusBarData );
	void SetInputEditBoxFocus();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	void SmallShow();
	void BigShow();

	bool CheckData( TCHAR *PlayerName, TCHAR *ChatText);

	void ChatRecv( TCHAR *PlayerName, TCHAR *ChatText, SI32 Kind = 0 );
	void MasterNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void VillageNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void ResidentsNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void KingNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void GoodsProductionNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );

	void SetOpenedChatDlg( bool Open );
	void ChatMessage(SI16 chatmode, SI32 systemtxt, TCHAR* pmsg);       //client 에 있던 메세지 박스 처리를 위해서 

	SI32 CompareKey(TCHAR *data, SI32 type);

	

private:
	CChatEdit	*m_pChatEdit;
	CEdit		*m_pInputEdit;
	CComboBox	*m_pComboBox;
	CButton		*m_pOpenButton;
	    

	TCHAR		*m_strChatData;
	CChatStatusBarBuffer	*m_pChatStatusBarBuffer;

	bool		m_bOpened;

	
};

#endif
