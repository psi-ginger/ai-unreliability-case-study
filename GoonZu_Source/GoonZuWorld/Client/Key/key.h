#ifndef _KEY_H
#define _KEY_H


#define MAX_KEY_INPUT	50

#define KEY_MODE_NONE	-1
#define KEY_MODE_TRUE	1
#define KEY_MODE_FALSE	0

class cltKey{
public:
	SI16	siLeftPressMode;
	SI16	siRightPressMode;
	SI16    siDoubleLeftPressMode;

	bool	bMoveSwitch;                   
	SI16	siMouseX;
	SI16	siMouseY;
	DWORD	wParam;


	cltKey()
	{
		siLeftPressMode		= KEY_MODE_NONE;
		siRightPressMode	= KEY_MODE_NONE;
		siDoubleLeftPressMode = KEY_MODE_NONE;
		bMoveSwitch			= false;
		siMouseX			= 0;
		siMouseY			= 0;
		wParam				= 0;
	}

	cltKey(SI16 leftmode, SI16 rightmode, SI16 dleftmode, bool moveswitch, SI16 mousex, SI16 mousey, DWORD wparam)
	{
		siLeftPressMode		= leftmode;
		siRightPressMode	= rightmode;
		siDoubleLeftPressMode = dleftmode;
		bMoveSwitch			= moveswitch;
		siMouseX			= mousex;
		siMouseY			= mousey;
		wParam				= wparam;
	}

	void Set(cltKey* pclkey)
	{
		siLeftPressMode		= pclkey->siLeftPressMode;
		siRightPressMode	= pclkey->siRightPressMode;
		siDoubleLeftPressMode =pclkey->siDoubleLeftPressMode;
		bMoveSwitch			= pclkey->bMoveSwitch;
		siMouseX			= pclkey->siMouseX;
		siMouseY			= pclkey->siMouseY;
		wParam				= pclkey->wParam;
	}
};



class cltKeyManager{
public:

	SI32	siKeyInputIndex;
	cltKey	clKey[MAX_KEY_INPUT];
	
	cltKeyManager();
	~cltKeyManager();

	void Init()
	{
		siKeyInputIndex	= 0;
	}

	void Push(cltKey* pclkey);
	bool Pop(cltKey* pclkey);
};

#endif