
/* ==========================================================================
	클래스 :		NCityHallSubmitDlg

	작성일 :		05/05/11
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "./NCityHallSubmitDlg.h"

#include <tchar.h>
#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "./NCityHallDlg.h"
#include "..\..\Server\Rank\Rank.h"

#include "MsgType-Structure.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;

//NCityHallSubmitDlg g_cityhallSuggestdlg;

NCityHallSubmitDlg::NCityHallSubmitDlg()
{
	m_bShow = false;
	m_pStatic_chief	 = NULL; 
	m_pStatic_cityhall_candidate	 = NULL; 
	m_pStatic_confirm	 = NULL; 
	m_pButton_suggest	 = NULL; 
	m_pEdit_explain	 = NULL; 
	m_pEdit_candidature	 = NULL; 

}

NCityHallSubmitDlg::~NCityHallSubmitDlg()
{
	if( 	m_pStatic_chief	 )  	delete 	m_pStatic_chief	;
	if( 	m_pStatic_cityhall_candidate	 )  	delete 	m_pStatic_cityhall_candidate	;
	if( 	m_pStatic_confirm	 )  	delete 	m_pStatic_confirm	;
	if( 	m_pButton_suggest	 )  	delete 	m_pButton_suggest	;
	if( 	m_pEdit_explain	 )  	delete 	m_pEdit_explain	;
	if( 	m_pEdit_candidature	 )  	delete 	m_pEdit_candidature	;

}


void NCityHallSubmitDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_CityHallSuggest.ddf"));

		file.CreatePopupDialog( this, NCITYHALLSUBMIT_DLG, TEXT("dialog_cityhallsubmit"), NCityHallSubmitStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_cityhallsubmit") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		
		m_pclStructureManager = pclClient->pclCityHallManager;

		m_pStatic_chief					= new 	CStatic	( this )	;
		m_pStatic_cityhall_candidate	= new 	CStatic	( this )	;
		m_pStatic_confirm				= new 	CStatic	( this )	;
		m_pButton_suggest				= new 	CButton	( this )	;
		m_pEdit_explain					= new 	CEdit	( this )	;
		m_pEdit_candidature				= new 	CEdit	( this )	;

		file.CreateControl( 	m_pStatic_chief	, 					NCITYHALLSUBMIT_STATICTEXT_CHIEF	,				TEXT("statictext_chief") )	;
		file.CreateControl( 	m_pStatic_cityhall_candidate	, 	NCITYHALLSUBMIT_STATICTEXT_CITYHALL_CANDIDATE	,	TEXT("statictext_cityhall_candidate") )	;
		file.CreateControl( 	m_pStatic_confirm	, 				NCITYHALLSUBMIT_STATICTEXT_CONFIRM	,				TEXT("statictext_confirm") )	;
		file.CreateControl( 	m_pButton_suggest	, 				NCITYHALLSUBMIT_BUTTON_SUGGEST	,					TEXT("button_suggest") )	;
		file.CreateControl( 	m_pEdit_explain	, 					NCITYHALLSUBMIT_EDITBOX_EXPLAIN	,					TEXT("editbox_explain") )	;
		file.CreateControl( 	m_pEdit_candidature	, 				NCITYHALLSUBMIT_EDITBOX_CANDIDATURE	,				TEXT("editbox_CANDIDATURE") )	;

		TCHAR buffer[1024];
		TCHAR* pText[5];
		pText[0] = GetTxtFromMgr(4990);
		pText[1] = GetTxtFromMgr(4991);
		pText[2] = GetTxtFromMgr(4992);
		pText[3] = GetTxtFromMgr(4993);
		pText[4] = GetTxtFromMgr(10046);

		StringCchPrintf(buffer, 1024, TEXT("%s\r\n%s\r\n%s\r\n%s\r\n%s"), pText[4],pText[0],pText[1],pText[2], pText[3]);

		m_pEdit_explain->SetText( buffer );
		m_pEdit_candidature->SetMaxEditTextLength( 33 );
	}
	else
	{
		DeleteDialog();
	}
}



void CALLBACK NCityHallSubmitDlg::NCityHallSubmitStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NCityHallSubmitDlg *pThis = (NCityHallSubmitDlg*) pControllerMgr;
	pThis->NCityHallSubmitProc( nEvent, nControlID, pControl );
}

void CALLBACK NCityHallSubmitDlg::NCityHallSubmitProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{ 
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NCITYHALLSUBMIT_BUTTON_SUGGEST:
		{
			switch(nEvent)
			{			
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR candidate[ 32 ];
					TCHAR* pTmpChar = NULL;
					pTmpChar = m_pEdit_candidature->GetText();

					if( pTmpChar == NULL || _tcslen( pTmpChar ) < 1 ) 
						return;
					
					_tcsncpy( candidate, pTmpChar, sizeof(candidate));

					TCHAR buffer[ 1024 ];

										
					//[진성] 영주가 위임되고 1년이 안되서 다시 주총을 열라고 하면 주식을 1% 감소 메시지 띄움. => 2008-7-21
					cltDate clChiefDate;
					clChiefDate.Set( &m_pclStructureManager->clClientStrInfo.clCityhallStrInfo.clChiefDate );
					clChiefDate.AddMonth(12);
					
					cltDate clCurrentDate;
					clCurrentDate.Set(&((cltClient*)pclClient)->clClientDate);
					
					if( clChiefDate.GetDateVary() > clCurrentDate.GetDateVary() )
					{
						StringCchPrintf( buffer, sizeof(buffer), GetTxtFromMgr(8620), candidate );
					}
					else
					{
						StringCchPrintf( buffer, sizeof(buffer), GetTxtFromMgr(1596), candidate );
					}

					stMsgBox MsgBox;
					MsgBox.Set( this, GetTxtFromMgr(1597), buffer, MSGBOX_TYPE_YESNO, 0 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( candidate );
					memcpy( TempBuffer, candidate, sizeof(candidate) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				break;
			}
		}
		break;
	}
}

void NCityHallSubmitDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 주주 총회 발의
		{
			if( DataLen < 1 || pData == NULL ) return;

			if ( RetValue )
			{
				TCHAR candidate[ 32 ];
				memset( candidate, 0, sizeof(candidate));
				memcpy( candidate, pData, sizeof(candidate));

				cltGameMsgRequest_GeneralMeetingSuggest sendMsg( candidate );

				cltMsg clMsg( GAMEMSG_REQUEST_GENERALMEETING_SUGGEST, sizeof(sendMsg), (BYTE*)&sendMsg );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
			}
		}
		break;
	}
}
