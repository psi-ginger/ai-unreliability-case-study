#include "..\Client\Client.h"

#include <mouse.h>

#include "Map\Map.h"

extern cltCommonLogic* pclClient;

extern	int 				MouseMode;							// ���콺�� ó�� ���¸� ���� 
extern	int 				ShiftKeyPressSwitch;
extern	SHORT 				GameMouseX, GameMouseY;
extern	int 				ControlKeyPressSwitch;


SHORT	MouseHotSpotX, MouseHotSpotY;

SHORT	MouseType;       // ���콺�� � ������ ���ΰ�?
SHORT	MouseFont;
SHORT	MouseStep=1;

static int showcursor = 0;


void SetMouse(SHORT mousemode)
{
	switch(mousemode)
	{
	case MOUSE_RIDE:
		MouseType=mousemode;
		MouseFont=69;
		MouseStep=4;
		MouseHotSpotX=9;
		MouseHotSpotY=26;
		break;

	case MOUSE_MAGIC:
		MouseType=mousemode;
		MouseFont=100;
		MouseStep=0;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_ATTACK:
		MouseType=mousemode;
		MouseFont=60;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;

		break;
	case MOUSE_NORMAL:
		MouseType=mousemode;
		MouseFont=80;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;

		break;
	case MOUSE_RESULT:
		MouseType=mousemode;
		MouseFont=183;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_INITMENU:
		MouseType=mousemode;
		MouseFont=180;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_CHOOSEMISSION:
		MouseType=mousemode;
		MouseFont=186;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;


	case MOUSE_SETGAME:
		MouseType=mousemode;
		MouseFont=189;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_PROVIDER:
		MouseType=mousemode;
		MouseFont=192;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;

	case MOUSE_SESSION:
		MouseType=mousemode;
		MouseFont=195;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;


	case MOUSE_CHAR:
		MouseType=mousemode;
		MouseFont=0;
		MouseStep=4;
		MouseHotSpotX=15;
		MouseHotSpotY=15;
		break;

	case MOUSE_HEAL:
		MouseType=mousemode;
		MouseFont=64;
		MouseStep=4;
		MouseHotSpotX=16;
		MouseHotSpotY=31;
		break;

	case MOUSE_HARVEST:
		MouseType=mousemode;
		MouseFont=10;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=15;
		break;

	case MOUSE_EXTINGUISH:
		MouseType=mousemode;
		MouseFont=84;
		MouseStep=14;
		MouseHotSpotX=15;
		MouseHotSpotY=15;
		break;

	case MOUSE_SCROLL_SOUTH:
		MouseType=mousemode;
		MouseFont=20;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_SW:
		MouseType=mousemode;
		MouseFont=21;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_WEST:
		MouseType=mousemode;
		MouseFont=22;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_NW:
		MouseType=mousemode;
		MouseFont=23;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NORTH:
		MouseType=mousemode;
		MouseFont=24;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NE:
		MouseType=mousemode;
		MouseFont=25;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_EAST:
		MouseType=mousemode;
		MouseFont=26;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_SE:
		MouseType=mousemode;
		MouseFont=27;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=31;
		break;

	case MOUSE_SCROLL_SOUTH_X:
		MouseType=mousemode;
		MouseFont=28;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_SW_X:
		MouseType=mousemode;
		MouseFont=29;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=31;
		break;
	case MOUSE_SCROLL_WEST_X:
		MouseType=mousemode;
		MouseFont=30;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_NW_X:
		MouseType=mousemode;
		MouseFont=31;
		MouseStep=1;
		MouseHotSpotX=0;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NORTH_X:
		MouseType=mousemode;
		MouseFont=32;
		MouseStep=1;
		MouseHotSpotX=15;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_NE_X:
		MouseType=mousemode;
		MouseFont=33;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=0;
		break;
	case MOUSE_SCROLL_EAST_X:
		MouseType=mousemode;
		MouseFont=34;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=15;
		break;
	case MOUSE_SCROLL_SE_X:
		MouseType=mousemode;
		MouseFont=35;
		MouseStep=1;
		MouseHotSpotX=31;
		MouseHotSpotY=31;
		break;

	}
}


void ShowHardwareCursor(BOOL mode)
{
	if(pclClient == NULL)return ;
	
	//cyj GetWindowModeSwitch() ������ ����
	//if(pclClient->GetWindowModeSwitch() == FALSE)
		return ;

/*	if(mode == TRUE)
	{
		if(!showcursor)
		{
			while(ShowCursor(TRUE) < 0);
			showcursor = 1;
		}
	}
	else
	{
		if(showcursor)
		{
			while(ShowCursor(FALSE) >= 0);
			showcursor = 0;
		}
	}
*/
}

void DetermineShowCursor(DWORD nhittest)
{

	/*cyj ������ ����
	if(pclClient->GetWindowModeSwitch() == TRUE)
	{
		switch(nhittest)
		{
		case HTCLIENT:
			if(showcursor)
			{
				while(ShowCursor(FALSE) >= 0);
				showcursor = 0;
			}
			break;

		default:
			if(!showcursor)
			{
				while(ShowCursor(TRUE) < 0);
				showcursor = 1;
			}
			break;
		}
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////
//
//	Keyboard and Mouse Interface
//
//	Revision 1.50	
//
///////////////////////////////////////////////////////////////////////////////

// Input Structure
struct SVKEY
{
	BYTE	bKeyStates[256];
	DWORD	dwKeyPressTimes[256];
	BYTE	bRecent;
} gVKey;


///////////////////////////////////////////////////////////////////////////////
//	Keyboard�� ���¸� ��´�. (CDX������)
///////////////////////////////////////////////////////////////////////////////
void ScanVKey()
{
	BYTE bKeyStates[256];
	GetKeyboardState(bKeyStates);

	BYTE	*pbKeyStates = gVKey.bKeyStates;
	DWORD	*pdwKeyPressTimes = gVKey.dwKeyPressTimes;
	BYTE	*bRecent = &gVKey.bRecent;

	for(int key=0; key < 256; ++key)
	{
		if (bKeyStates[key] & 0x80)
		{
			if(pbKeyStates[key] == VK_NONE || pbKeyStates[key] == VK_RELEASE)
			{
				// press event (key was up before but down now)
				pbKeyStates[key] = VK_PRESS;
				pdwKeyPressTimes[key] = GetTickCount();
				*bRecent = key;
			}
			else if(pbKeyStates[key] == VK_PRESS)
			{
				// drag event (key is still down)
				pbKeyStates[key] = VK_REPEAT;
			}
		}
		else // Key is not currently pressed
		{
			if(pbKeyStates[key] == VK_REPEAT || pbKeyStates[key] == VK_PRESS)
			{
				// release event (key was down before but up now)
				pbKeyStates[key] = VK_RELEASE;
			}
			else if(pbKeyStates[key] == VK_RELEASE)
			{
				pbKeyStates[key] = VK_NONE;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//	Virtual Key�� ���¸� ���´�.
///////////////////////////////////////////////////////////////////////////////
void SetVKeyState(int VirtKey)
{
	if( VirtKey > 0 || VirtKey >= 256 )		return;

	gVKey.bKeyStates[VirtKey] = VK_NONE;
}

///////////////////////////////////////////////////////////////////////////////
//	Virtual Key�� ���¸� ���´�.
///////////////////////////////////////////////////////////////////////////////
int GetVKeyState(int VirtKey)
{
	if( VirtKey > 0 || VirtKey >= 256 )		return 0;

	return gVKey.bKeyStates[VirtKey];
}

///////////////////////////////////////////////////////////////////////////////
//	���� �ð��� ���´�.
///////////////////////////////////////////////////////////////////////////////
DWORD GetVKeyPressTime(int VirtKey)
{
	if( VirtKey > 0 || VirtKey >= 256 )		return 0;

	return gVKey.dwKeyPressTimes[VirtKey];
}

///////////////////////////////////////////////////////////////////////////////
//	���� �ֱٿ� ������ Ű
///////////////////////////////////////////////////////////////////////////////
BYTE GetRecentVKey(int fClear)
{
	BYTE temp = gVKey.bRecent;

	if(fClear) gVKey.bRecent = 0;

	return temp;
}

///////////////////////////////////////////////////////////////////////////////
//	Mouse�� ��ġ�� ����ش�.
///////////////////////////////////////////////////////////////////////////////
void GetMousePos(HWND hwnd, LPPOINT lpPoint)
{
	GetCursorPos(lpPoint);
	ScreenToClient(hwnd, lpPoint);
}


