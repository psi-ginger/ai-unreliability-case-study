
#include "Char/CharManager/CharManager.h"
#include "../Server/Server.h"

#include "../../common/Order/order.h"

#include "../../../DBManager/GameDBManager_World/DBMsg.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "MsgType-Item.h"

#include "../../../DBManager//GameDBManager_World/DBMsg-Family.h"
#include "../../../DBManager//GameDBManager_World/DBMsg-Item.h"

void cltServer::DoMsg_GAMEMSG_REQUEST_FAMILY_REQUEST_ADD_FAMILY(cltMsg* pclMsg, SI32 id )
{	
	cltGameRequest_Request_Add_Children* pclinfo = (cltGameRequest_Request_Add_Children*)pclMsg->cData;
	if ( NULL == pclinfo ){		return;	}
	if(IsCountrySwitch( Switch_We_Are_Family ) == false )	return	;

	cltCharCommon* pParentChar	=	pclCM->GetChar( id )	;
	if( pParentChar == NULL	)	return	;
	// 배우자 
	SI32 siMatePersonID = pParentChar->pclMarriage->GetMatePersonID() ;		if ( siMatePersonID <= 0 )	return	;

	SI32 siMateid = pclCM->GetIDFromPersonID( siMatePersonID )	;
	if ( pclCM->IsValidID(siMateid) == false )
	{
		SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_NOTCONNECT_MATE, 0, 0, pParentChar->GetCharUnique() );	// 잘못된 케릭터 이다		
		return	;
	}
	cltCharCommon* pMateChar	=	pclCM->GetChar( siMateid )	;			if ( pMateChar == NULL )	return	;

	SI32 siChildren_CharID	=	pclCM->GetIDFromName( pclinfo->m_szCharName )	;	// 케릭터 이름으로 케릭터의 아이디를 구한다.
	if ( pclCM->IsValidID(siChildren_CharID)==false )
	{
		SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_ERROR_NO_CHAR, 0, 0, pParentChar->GetCharUnique() );	// 잘못된 케릭터 이다
		return	;
	}

	cltCharCommon* pChildrenChar	=	pclCM->GetChar( siChildren_CharID )	;		// 자녀가 될 케릭터 의 아이디.

	if( pChildrenChar == NULL	)	return	;

	// 이미 가족이 있을 경우에도 신청 실패
	if ( pChildrenChar->IsMarried() == TRUE || pChildrenChar->pclFamily->GetMyClass_InFamily() > 0  )
	{
		SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_BEFORE_IN_FAMILY, 0, 0, pParentChar->GetCharUnique() );	//이미 가정이 있는 케릭터 
		return	;
	}

	// 클라이언트에도 보내준다
	cltGameResponse_Request_Add_Children clinfo( pParentChar->pclCI->GetPersonID(),(TCHAR*)pParentChar->GetName(),
		pMateChar->pclCI->GetPersonID()	,(TCHAR*)pMateChar->GetName(),
		pChildrenChar->pclCI->GetPersonID(),(TCHAR*)pChildrenChar->GetName());
	
	cltMsg clMsg( GAMEMSG_RESPONSE_FAMILY_REQUEST_ADD_CHILDREN, sizeof(clinfo), (BYTE*)&clinfo );
	pChildrenChar->SendNetMsg( (sPacketHeader*)&clMsg );


}
void cltServer::DoMsg_GAMEMSG_REQUEST_FAMILY_ADD_FAMILY(cltMsg* pclMsg, SI32 id )	//요청을 수락하면 가족으로 들어간다.
{
	//자식을 하겠다는 수락을 하였다.
	cltGameRequest_Add_Children* pclinfo = (cltGameRequest_Add_Children*)pclMsg->cData;		if ( NULL == pclinfo ){		return;	}
	if(IsCountrySwitch( Switch_We_Are_Family ) == false )	return	;

	SI32 siParentPersonID = pclinfo->m_siParentPersonID	;
	SI32 siParentID = pclCM->GetIDFromPersonID(siParentPersonID) ;

	cltCharCommon* pParentChar	=	pclCM->GetChar( siParentID )	;						if( pParentChar == NULL	)	return	;

	 SI32 siMatePersonID = pParentChar->pclMarriage->GetMatePersonID()	;
	 SI32 siMateId = pclCM->GetIDFromPersonID( siMatePersonID )	;	
	 cltCharServer* pMateCharServer = pclCM->GetCharServer( siMateId )	;

	 if(pMateCharServer == NULL )	// 배우자가 나갔다 
	 {
		 SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_NOTCONNECT_MATE, 0, 0, pParentChar->GetCharUnique() );	// 배우자가 없다 등록 불가능 
		return	;
	 }

	cltCharCommon* pChildrenChar	=	pclCM->GetChar( id )	;		// 자녀가 될 케릭터 의 아이디.
	if( pChildrenChar == NULL	)	return	;

	// 체크 조건 
	// 1. 가족 등록 이용권이 있는가?
	bool bUsedItem		=	false	;	// 사용했는지 확인
	SI32 siUseItem		=	ITEMUNIQUE( 24121 )	;	// 가족 이용권 
	SI32 siUseItemNum	=	1 ;						// 1장
	cltItem clitem;
	SI32 siref = pclClient->pclItemManager->FindItemRefFromUnique(siUseItem);			if(siref <= 0) return ;
	SI32 siHaveitemNum = pParentChar->pclCI->clCharItem.GetItemNum( siUseItem ) ;	// 아이템을 몇개 가지고 있는지 확인.
	
	if ( siHaveitemNum >= siUseItemNum ) // 충분한 수가 있다면 
	{
		for(SI32 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
		{
			if(pParentChar->pclCI->clCharItem.clItem[i].siUnique == siUseItem)
			{
				if ( siUseItemNum == 0 )	break	;	// 갯수가 0개면 끝낸다
				SI32 invItemNum = pParentChar->pclCI->clCharItem.clItem[i].GetItemNum()	;
				if ( invItemNum  >= siUseItemNum )
				{
					clitem.Set(&pclClient->pclItemManager->pclItemInfo[siref]->clItem);
					clitem.siItemNum = siUseItemNum;
					sDBRequest_UseItem clMsg(pParentChar->GetID(), pParentChar->pclCI->GetPersonID(), i, &clitem, USEITEM_REASON_NONE);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					bUsedItem	=	true	;
					break	;
				}
				else
				{
					clitem.Set(&pclClient->pclItemManager->pclItemInfo[siref]->clItem);
					clitem.siItemNum = invItemNum	;// 양만큼 넣고
					//전체 갯수에서 뺴준다
					siUseItemNum =  siUseItemNum - invItemNum	;
					// 사용해라
					sDBRequest_UseItem clMsg(pParentChar->GetID(), pParentChar->pclCI->GetPersonID(), i, &clitem, USEITEM_REASON_NONE);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					bUsedItem	=	true	;
				}
			}
		}
	}

	if ( bUsedItem == false ) 
	{
		// 아이템이 없어서 사용을 못했다 리턴 메세지를 날려준다.
		SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_No_TICKET, siUseItem, 0, pParentChar->GetCharUnique() );	// 잘못된 케릭터 이다
		return	;
	}

	SYSTEMTIME stAddTime	;

	ZeroMemory(&stAddTime,	sizeof(SYSTEMTIME));

	TimeAddDate(&sTime,30,&stAddTime)	;

	// 가족 구성원을 추가 한다.
	AddFamily(pParentChar->pclCI->GetPersonID(), pChildrenChar->pclCI->GetPersonID() ,stAddTime )	;	//시간저정을 깜빡 핶음

	//DB에 저장 한다.
	sDBRequest_Family_Add_Children clMsg( pParentChar->pclCI->GetPersonID(),(TCHAR*)pParentChar->GetName(),	
		pChildrenChar->pclCI->GetPersonID() , (TCHAR*)pChildrenChar->GetName(),
		stAddTime );// 자식 personID를 날린다.
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}
void cltServer::DoMsg_GAMEMSG_REQUEST_FAMILY_DEL_CHILDREN(cltMsg* pclMsg, SI32 id )	// 가족에서 삭제 
{
	cltGameRequest_Del_Children* pclinfo = (cltGameRequest_Del_Children*)pclMsg->cData;
	if ( NULL == pclinfo ){		return;	}
	if(IsCountrySwitch( Switch_We_Are_Family ))
	{
		cltCharServer* pclcharRequestChar = pclCM->GetCharServer( id)	;				// 삭제를 시도하는 케릭터
		
		SI32 siDetTargetId	=	pclCM->GetIDFromPersonID( pclinfo->m_siPersonID )	;	// 삭제할 타겟 케릭터
		cltCharServer* pclDelTargetchar = pclCM->GetCharServer( siDetTargetId)		;	
		if( pclDelTargetchar == NULL )	// 해당 케릭터가 미접속 
		{
			// 미접속 했는데 탈퇴 타겟이 되려면 무조건 자식일뿐이 없다.
			// 쪽지를 보낸다. 
			cltLetterMsg_DelLetter clletter1;
			SendLetterMsg(pclinfo->m_siPersonID, (cltLetterHeader*)&clletter1);

			sDBRequest_Family_Del_Children clMsg( pclinfo->m_siPersonID );				// 자식 personID를 날린다.
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}
		else	// 해당 케릭터가 접속 해 있다.
		{
			// 접속해 있다면 타겟이 결혼했는지 확인
			if( pclDelTargetchar->IsMarried() == TRUE )	//  기본적으로 결혼 했다면 부모 케릭터 이다.
			{
				if(pclDelTargetchar->pclFamily != NULL && pclDelTargetchar->pclFamily->GetMyClass_InFamily() > MYCLASS_IN_FAMILY_NONE )	// 자식을 둔적이 있다. 가족정보가 있음.
				{
					// 자식이 있다면 해당 케릭터는 가족의 정보를 가지고 있다.
					if( pclDelTargetchar->pclFamily->GetChildPersonID( 0 ) > 0)	// 0번 슬롯에 자식이 있는지 확인 한다.
					{
						// 자식이 있다. 이혼불가능
						SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_DEL_HAVECHILD, 0, 0, pclDelTargetchar->GetCharUnique() );
					}
					else
					{
						SI32 siMatePersonID = pclDelTargetchar->pclFamily->GetMatePersonID( pclDelTargetchar->pclCI->GetPersonID() )	;// 타겟의 배우자 

						SI32 siMateID = pclCM->GetIDFromPersonID( siMatePersonID )	;
						cltCharServer* pclMateChar = pclCM->GetCharServer( siMateID)	;	
						if(pclMateChar != NULL )		// 배우자가 NULL 접속해있지 않다. 특별히 업데이트가 필요 없다.
						{
							pclMateChar->pclFamily->Init()	;	// 초기화 하고 클라이언트를 업데이트 해준다.
							pclMateChar->pclMarriage->Init()	;	// 초기화 하고 클라이언트를 업데이트 해준다.

							// 미접속일때는 그냥 깨졌다는 메세지만 날려주면 된다
							cltLetterMsg_LeaveFamilyLetter clletter( (TCHAR*)pclDelTargetchar->GetName());
							SendLetterMsg( siMatePersonID , (cltLetterHeader*)&clletter );
							
							cltGameResponse_UpdateFamily clinfo( pclMateChar->pclFamily,pclMateChar->pclMarriage );
							cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
							pclMateChar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );
						}
						// 배우자를 업데이트 해줬으면 이제 나를 초기화 한다.
						pclDelTargetchar->pclFamily->Init()	;	// 가족정보 초기화
						pclDelTargetchar->pclMarriage->Init()	;	// 결혼정보 초기화

						SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_DEL_FAMILY, 0, 0, pclDelTargetchar->GetCharUnique() );

						//지운다음에 클라이언트에 초기화하고 클라이언트로 알려준다.
						cltGameResponse_UpdateFamily clinfo( pclDelTargetchar->pclFamily,pclDelTargetchar->pclMarriage );
						cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
						pclDelTargetchar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );
						
						// 디비로 알린다.
						sDBRequest_Family_Del_Children clMsg( pclDelTargetchar->pclCI->GetPersonID() );// 자식 personID를 날린다.
						SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					}
				}
				else	// 결혼은 했지만 가족은 없다.
				{					
					SI32 siMatePersonID = pclDelTargetchar->pclMarriage->GetMatePersonID()	;
					SI32 siMateID = pclCM->GetIDFromPersonID( siMatePersonID )	;
					cltCharServer* pclMate = pclCM->GetCharServer( siMateID )	;
					if ( pclMate == NULL )	// 배우자 미접속 
					{
						// 미접속일때는 그냥 깨졌다는 메세지만 날려주면 된다
						cltLetterMsg_LeaveFamilyLetter clletter( (TCHAR*)pclDelTargetchar->GetName());
						SendLetterMsg(  siMatePersonID , (cltLetterHeader*)&clletter );
					}
					else	// 배우자가 접속 되어 있다.
					{
						pclMate->pclMarriage->Init()	;

						// 미접속일때는 그냥 깨졌다는 메세지만 날려주면 된다
						cltLetterMsg_LeaveFamilyLetter clletter( (TCHAR*)pclDelTargetchar->GetName());
						SendLetterMsg(  siMatePersonID , (cltLetterHeader*)&clletter );

						//지운다음에 클라이언트에 초기화하고 클라이언트로 알려준다.
						cltGameResponse_UpdateFamily clinfo( pclMate->pclFamily,pclMate->pclMarriage );
						cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
						pclMate->SendNetMsg( (sPacketHeader*)&clMsgtoClient );
					}
					pclDelTargetchar->pclMarriage->Init()	;	// 웨딩관계를 청산 한다.

					SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_DEL_FAMILY, 0, 0, pclDelTargetchar->GetCharUnique() );
					//지운다음에 클라이언트에 초기화하고 클라이언트로 알려준다.
					cltGameResponse_UpdateFamily clinfo( pclDelTargetchar->pclFamily ,pclDelTargetchar->pclMarriage);
					cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
					pclDelTargetchar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );

					// 디비로 알린다.
					sDBRequest_Family_Del_Children clMsg( pclDelTargetchar->pclCI->GetPersonID() );// 자식 personID를 날린다.
					SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


				}
			}
			else	// 결혼하지 않았다면 자식 케릭터 
			{
				for ( SI32 siSlot = 0 ; siSlot < MAX_PARENT_NUM; siSlot++) 
				{
					SI32 siParent_PersonID = pclDelTargetchar->pclFamily->GetParent_PersonId(siSlot)	;
					if(siParent_PersonID > 0 )
					{
						cltLetterMsg_LeaveFamilyLetter clletter( (TCHAR*)pclDelTargetchar->GetName());
						SendLetterMsg(  siParent_PersonID , (cltLetterHeader*)&clletter );
					}
				}
				for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM; siSlot++) 
				{
					SI32 PersonID = pclDelTargetchar->pclFamily->GetParent_PersonId(siSlot)	;
					if (PersonID > 0 ) 
					{
						cltLetterMsg_LeaveFamilyLetter clletter( (TCHAR*)pclDelTargetchar->GetName());
						SendLetterMsg(  PersonID , (cltLetterHeader*)&clletter );
					}
				}
				//쪽지 다 날렸으니
				pclDelTargetchar->pclFamily->Init()	;

				//지운다음에 클라이언트에 초기화를 한다.
				cltGameResponse_UpdateFamily clinfo( pclDelTargetchar->pclFamily,pclDelTargetchar->pclMarriage );
				cltMsg clMsgtoClient( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clinfo), (BYTE*)&clinfo );
				pclDelTargetchar->SendNetMsg( (sPacketHeader*)&clMsgtoClient );

				// 가족에서 짤렸따는 메세지를 날려준다.
				SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_DEL_FAMILY, 0, 0, pclDelTargetchar->GetCharUnique() );

				sDBRequest_Family_Del_Children clMsg( pclDelTargetchar->pclCI->GetPersonID() );// 자식 personID를 날린다.
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


			}

		}

	}
}

// 타켓 가족구성원으로 워프 
void cltServer::DoMsg_GAMEMSG_REQUEST_FAMILY_WARP(cltMsg* pclMsg, SI32 id )
{
	cltGameRequest_WarpFamily* pclinfo = (cltGameRequest_WarpFamily*)pclMsg->cData;
	if ( NULL == pclinfo ){		return;	}
	if(IsCountrySwitch( Switch_We_Are_Family ) == false )	return	;

	cltCharServer* pclChar = pclCM->GetCharServer( id );
	if ( NULL == pclChar )	{		return;	}

	// 배우자 캐릭터가 게임하고 있지도 않은데 그르면 안되
	SI32			siTargetFamilyCharID	= pclCM->GetIDFromPersonID( pclinfo->m_siPersonID );
	cltCharServer*	pclTargetFamilyChar		= pclCM->GetCharServer( siTargetFamilyCharID );
	if ( NULL == pclTargetFamilyChar )	{		return	;	}

	SI32 siMateMapIndex = pclTargetFamilyChar->GetMapIndex();
	SI32 siMateXPos		= pclTargetFamilyChar->GetX();
	SI32 siMateYPos		= pclTargetFamilyChar->GetY();

	// 유효하지 않은 맵이라면!!
	if ( (0 > siMateMapIndex) || (MAX_MAP_NUMBER <= siMateMapIndex) )	{		return;	}

	// 해당 맵 지역이 없다면!!!
	if ( NULL == pclMapManager->pclMapArray[siMateMapIndex] )	{		return;	}

	// 유효하지 않은 지역이라면!!
	if ( false == pclMapManager->IsValidArea(siMateMapIndex, siMateXPos, siMateYPos) )	{		return;	}

	// 마을로 워프인지 체크
	bool bVillageWarp = false;
	if ( (0 < siMateMapIndex) && (CUR_VILLAGE_LIMIT >= siMateMapIndex) )
	{
		if ( true == pclMapManager->pclVillageManager->IsValidVillage(siMateMapIndex) )		{			bVillageWarp = true;		}
	}

	// 이지역에서는 워프할수 있을까?
	if ( false == pclChar->CanWarpFrom() )	{		return;	}

	// 해당 지역으로 워프할수있을까?
	if ( false == pclChar->CanWarpTo(siMateMapIndex) ){		return;	}

	// 그 케릭터로 실제로 워프할수있을까?
	if ( false == pclChar->CanWarpToChar(siTargetFamilyCharID) ){		return;	}

	SI32 siWarpUnique	= 0;

	// 마을로 이동일 경우
	if ( bVillageWarp )
	{
		SI32 siGetMapTypeUnique = pclMapManager->pclMapArray[siMateMapIndex]->siMapUnique;
		if (0 > siGetMapTypeUnique){			return;		}
		siWarpUnique = 2300;
	}
	else
	{
		siWarpUnique = 2200;
	}

	if ( id )
	{
		OrderStop( id, BY_COMPUTER );

		// 워프 이팩트 표시
		cltGameMsgResponse_WarpEffect clinfo( id );
		cltMsg clMsg(GAMEMSG_RESPONSE_WARPEFFECT, sizeof(clinfo), (BYTE*)&clinfo);
		pclChar->SendNeighbourMsg(&clMsg, true);

		// 실제로 워프하기
		cltWarpInfo clWarpInfo( 1, siMateMapIndex, false, true, siMateXPos, siMateYPos, -1, -1 );
		pclChar->SetWarpInfo( &clWarpInfo, siWarpUnique );
		pclChar->Warp();
	}
}
void cltServer::DoMsg_GAMEMSG_REQUEST_FAMILY_CHAT(cltMsg* pclMsg, SI32 id )	// 가족 챗팅 
{
	cltGameRequest_ChatFamily* pclinfo = (cltGameRequest_ChatFamily*)pclMsg->cData;
	if ( NULL == pclinfo ){		return;	}
	if(IsCountrySwitch( Switch_We_Are_Family ) == false )	return	;

	// 보낸 사람 이름

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex	=	CHATEDIT_COLOR_VIOLET;
	ChatEditData.siChatEditDataSize		=	sizeof(pclinfo->m_szChatData);
	MStrCpy( ChatEditData.strNameData, pclinfo->m_szCharName, sizeof(TCHAR)*MAX_PLAYER_NAME );	/// 채팅 데이터를 만들고..
	MStrCpy( ChatEditData.strChatData, pclinfo->m_szChatData, sizeof(TCHAR)*MAX_CHAT_DATA_LENGTH );

	cltCharServer* pclCharServer = pclCM->GetCharServer(id)	;

	// 가족들에게 메세지를 날린다.

	for ( SI32 siSlot = 0 ; siSlot< MAX_PARENT_NUM; siSlot++ )
	{
		SI32 siPersonID = pclCharServer->pclFamily->GetParent_PersonId(siSlot) ;
		SI32 siID = pclCM->GetIDFromPersonID( siPersonID)	;
		if ( pclCM->IsValidID( siID) == false ) continue	;
		cltCharServer* pclChar = pclCM->GetCharServer( siID )	;
		// 클라이언트로 해당 케릭터의 정보 업데인트
		cltGameResponse_ChatFamily clInfo( &ChatEditData );
		cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_CHAT, sizeof(clInfo), (BYTE*)&clInfo );
		pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
	}
	for ( SI32 siSlot = 0 ; siSlot< MAX_CHILDREN_NUM; siSlot++ )
	{
		SI32 siPersonID = pclCharServer->pclFamily->GetChildPersonID(siSlot) ;
		SI32 siID = pclCM->GetIDFromPersonID( siPersonID)	;
		if ( pclCM->IsValidID( siID) == false ) continue	;
		cltCharServer* pclChar = pclCM->GetCharServer( siID )	;
		// 클라이언트로 해당 케릭터의 정보 업데인트
		cltGameResponse_ChatFamily clInfo( &ChatEditData );
		cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_CHAT, sizeof(clInfo), (BYTE*)&clInfo );
		pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
	}
}

// 가족 효과 연장 
void cltServer::DoMsg_GAMEMSG_REQUEST_FAMILY_PLUS_FAMILYEFFECT(cltMsg* pclMsg, SI32 id )
{
	cltGameRequest_Plus_FamilyEffectDate* pclinfo = (cltGameRequest_Plus_FamilyEffectDate*)pclMsg->cData;
	if ( NULL == pclinfo ){		return;	}
	if(IsCountrySwitch( Switch_We_Are_Family ) == false )	return	;

	cltCharServer* pParentChar = pclCM->GetCharServer(id)	;	// 기간을 늘리는 부모
	if( pParentChar == NULL ) return	;
	SI32 siTergetPersonId = pclinfo->m_siPersonID	;	// 기간을 늘릴 자식

	// 먼저 티켓검사 부터 
	// 체크 조건 
	// 1. 가족 등록 이용권이 있는가?
	bool bUsedItem		=	false	;	// 사용했는지 확인
	SI32 siUseItem		=	ITEMUNIQUE( 24122 )	;	// 가족 이용권 
	SI32 siUseItemNum	=	1 ;						// 1장
	cltItem clitem;
	SI32 siref = pclClient->pclItemManager->FindItemRefFromUnique(siUseItem);			if(siref <= 0) return ;
	SI32 siHaveitemNum = pParentChar->pclCI->clCharItem.GetItemNum( siUseItem ) ;	// 아이템을 몇개 가지고 있는지 확인.
	if ( siHaveitemNum >= siUseItemNum ) // 충분한 수가 있다면 
	{
		for(SI32 i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON;i++)
		{
			if(pParentChar->pclCI->clCharItem.clItem[i].siUnique == siUseItem)
			{
				if ( siUseItemNum == 0 )	break	;	// 갯수가 0개면 끝낸다
				SI32 invItemNum = pParentChar->pclCI->clCharItem.clItem[i].GetItemNum()	;
				if ( invItemNum  >= siUseItemNum )
				{
					clitem.Set(&pclClient->pclItemManager->pclItemInfo[siref]->clItem);
					clitem.siItemNum = siUseItemNum;
					sDBRequest_UseItem clMsg(id, pParentChar->pclCI->GetPersonID(), i, &clitem, USEITEM_REASON_NONE);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					bUsedItem	=	true	;
					break	;
				}
				else
				{
					clitem.Set(&pclClient->pclItemManager->pclItemInfo[siref]->clItem);
					clitem.siItemNum = invItemNum	;// 양만큼 넣고
					//전체 갯수에서 뺴준다
					siUseItemNum =  siUseItemNum - invItemNum	;
					// 사용해라
					sDBRequest_UseItem clMsg(id, pParentChar->pclCI->GetPersonID(), i, &clitem, USEITEM_REASON_NONE);
					pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
					bUsedItem	=	true	;
				}
			}
		}
	}
	
	if ( bUsedItem == false ) 
	{
		// 아이템이 없어서 사용을 못했다 리턴 메세지를 날려준다.
		SendServerResponseMsg( 0, SRVAL_FAMILY_ADD_WRONG_No_TICKET_PLUSDATE, siUseItem, 0,  pParentChar->GetCharUnique() )	;
		return	;
	}
	
	SYSTEMTIME ChangTime	;
	SYSTEMTIME DatevaryNow = pParentChar->pclFamily->FInd_Add_Date( siTergetPersonId )	;
	TimeAddDate(&DatevaryNow,30,&ChangTime)	;// 30일 늘려준다.

	// 티켓을 검사해서 사용하고
	// 연장 메세지를 날려준다.
	//DB에 저장 한다.
	sDBRequest_Family_Plus_Add_Date clMsg( siTergetPersonId ,ChangTime );
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}