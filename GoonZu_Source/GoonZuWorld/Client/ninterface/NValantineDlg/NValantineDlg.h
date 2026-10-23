//-----------------------------------------------------------------------------------------------
//	작성자		:	손성웅
//	작성시작	:	2008-12-10
//	내용		:	발렌타인데이 이벤트로 초기에 자신이 커플인지 솔로인즈 선택하는 다이얼로그
//					이 다이얼로그는 서버로 내용을 보내는 일만 한다. -보여주는 것은 없다.-
//-----------------------------------------------------------------------------------------------


#ifndef _NVALANTINEDLG_H_
#define _NVALANTINEDLG_H_

#include "../Client/client.h"
#include "../Common/Util/Util.h"
#include "../../InterfaceMgr/DialogController.h"

class CNValantineDlg : public CDialogController
{
private:
        
public:
	CNValantineDlg();
	~CNValantineDlg();

	InterfaceFactory m_InterfaceFactory;

	LPDIRECTDRAWSURFACE7		lpBackScreen;	// 게임이 그려질 화면 뒤의 서피스. 

	void Create();
	
	static void CALLBACK StaticNValantineDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )	;
		   void CALLBACK NValantineDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )	;
	void SendMyStatus(SI32 siMyStatus )	;	// 내가 솔로인지 커플인지 서버로 보낸다.
	void MsgBox( )	;	// 내가 솔로인지 커플인지 서버로 보낸다.
};
#endif
