#ifndef _DRAWCHATSURFACE_H_
#define _DRAWCHATSURFACE_H_

#include "DrawChatBaseInfo.h"
#include "TSpr.h"

class	CDrawChatSurface
{
public:
	CDrawChatSurface();
	~CDrawChatSurface();

	void						Initialize();
	void						Destroy();
	
	LPDIRECTDRAWSURFACE7		GetChatSurface();

	TSpr						*GetDrawChatImage();
	TSpr						*GetDrawChatTailImage();
    
	TSpr						*GetDrawChatImage1();
	TSpr						*GetDrawChatTailImage1();

	TSpr						*GetDrawChatImage2();
	TSpr						*GetDrawChatTailImage2();

	TSpr						*GetDrawChatImage3();
	TSpr						*GetDrawChatTailImage3();

	TSpr						*GetDrawChatImage4();
	TSpr						*GetDrawChatTailImage4();
	
	TSpr						*GetDrawChatImage5();
	TSpr						*GetDrawChatTailImage5();

	TSpr						*GetDrawShopTitleImage();


private:

	LPDIRECTDRAWSURFACE7		m_lpSurface;

	// 일반 유저 채팅
	TSpr						m_DrawChatSpr;
	TSpr						m_DrawChatTailSpr;

	// 광고
	TSpr						m_DrawChatSpr1;
	TSpr						m_DrawChatTailSpr1;

	// 유저가 하는 말( 제조했다, 실패했다 등등을 보여 줄때 )
	TSpr						m_DrawChatSpr2;
	TSpr						m_DrawChatTailSpr2;

	// NPC 가 하는 말
	TSpr						m_DrawChatSpr3;
	TSpr						m_DrawChatTailSpr3;

	// 개인상점 
	TSpr						m_DrawChatSpr4;
	TSpr						m_DrawChatTailSpr4;
	
	//프리미엄 개인상점 
	TSpr						m_DrawChatSpr5;
	TSpr						m_DrawChatTailSpr5;

	TSpr						m_DrawChatSellBuy;

	TSpr						m_DrawShopTitleImage;
};

#endif