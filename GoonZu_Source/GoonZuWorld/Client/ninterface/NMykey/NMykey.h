/* ==========================================================================
클래스 :		 

작성일 :		 
작성자 :		 

변경사항 :		 

차후작업 :		
========================================================================*/
 
#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

#define MYKEY_NUM		 5	// 내가 등록 가능한   키 개수.
#define CHECKBOX_NUM	26	// 내가 사용 가능한 창의 개수.

class CControlMapping
{
public:
	CControlMapping()  { ZeroMemory(this, sizeof(*this) ); }
	CControlMapping(SI32 _siDialogIndex, SI32 _siTextNum)  
	{ 
		m_siDialogIndex  = _siDialogIndex;
		m_siTextNum		 = _siTextNum;
	}
	~CControlMapping() { }
	
	SI32 GetDialogIndex(void)  {	return m_siDialogIndex;		}
	SI32 GetTextNum(void)	   {	return m_siTextNum;			}
	
	void Set( SI32 _siDialogIndex, SI32 _siTextNum )
	{
		m_siDialogIndex  = _siDialogIndex;
		m_siTextNum		 = _siTextNum;
	}

private:
	SI32 m_siDialogIndex;	// 다이얼로그 번호.			
	SI32 m_siTextNum;	    // 텍스트 번호.
};

class CNMykeyDlg : public CDialogController
{
public:

	CNMykeyDlg();
	~CNMykeyDlg();

	void Init();

	void Create();
	void Destroy();

	static void CALLBACK StaticNMykeyDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NMykeyDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void SetDialogKind(SI32* _psiDialogKind);	
	void KeyAction( SI32 _KeyKind );

	void SaveFile();
	void LoadFile();
	void CheckControlMapping();
	SI32 FindTextNum( SI32 _DialogIndex );
	SI32 FindCotrolIndex( SI32 _DialogIndex );					
	void SetKey( void );										// 체크박스에 체크된 키를 m_siDialogKind에 세팅.
	void SetKey( SI32 _DialogIndex );
	void LoadKey( void );										// m_siDialogKind에 세팅된 키를 체크박스 세팅.
	bool IsDestroyWindow( SI32 _DialogIndex );					// 키에 추가된 창이 생성되고 삭제되는 창인가.
	bool IsFullSetkey( int _nControlID );									// 설정 가능한가. 개수 제한.
	void HomePage( void );

private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siDialogKind[MYKEY_NUM];				// 다이얼로그 종류(번호)를 저장.

	CControlMapping		m_clControlMapping[CHECKBOX_NUM];		// 체크박스 컨트롤과 다이얼로그 맵핑.

	bool				m_bCreateComplete;						// 생성완료.
};
 