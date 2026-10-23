#include "KingGuri.h"

#include "../CommonLogic/CommonLogic.h"
#include "../Server/Server.h"

//-----------------------------------------
// Common
//-----------------------------------------
#include "../Common/Char/KindInfo/KindInfo.h"
#include "../Common/Order/Order.h"
#include "Char/CharManager/CharManager.h"

#include "MsgType-System.h"

#include "../../Client/Music/Music.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Village.h"

#include <MsgType-Person.h>


extern cltCommonLogic* pclClient;

CKingGuriEvent::CKingGuriEvent() : cltEvent()
{
	SI32 i;
	for( i = 0; i < MAX_KINGGURI_NUM; ++i )
	{
		if ( i >= MAX_KINGGURI_NUM )	continue	;	// 배열이 넘어서면 다음데이터 읽는대로 넘긴다.	초기화는 다 했잖아.
		m_bKilledKingGuri[i] = false;
		m_siKingGuriID[i] = 0;
	}

	FILE* fp = fopen("ServerData\\KingGuri\\KingGuriInfo.txt", "rt");
	if(fp == NULL)
	{
		MessageBox( NULL, "KingGuri Failed", "KingGuri", MB_OK );
		return;
	}

	TCHAR buffer[256] = {'\0'};
	for( i = 0;i< 7;i++ )
	{
		fgets(buffer, 256, fp);
	}// 데이터와 상관없는 부분을 읽어 들인다.7줄까지는 의미 없음 스타트등등 다 의미 없음

	SI32 count = 0;
	while( fgets( buffer, 256, fp ) != NULL )
	{
		if( _tcscmp( buffer, "<END>") == 0 )	// 가장 중요한 부분 여기에 스페이스바나 엔터 등이 들어가면 엔드로 인식 안합니다 .
		{
			break;
		}
		if ( count >= MAX_POSITION_NUM )	continue	;	// 리소스가 잘못들어갔을때를 대비해서 맥스만큼 읽어 댔으면 더이상 배열에 집어 넣지 않고 컨티뉴 해서 파일을 클로즈 시킵니다
		sscanf( buffer, "%s %d %d", m_szAreaName[count], &m_ptPosition[count].x, &m_ptPosition[count].y );
		count++;
	}

	fclose(fp);
}

CKingGuriEvent::~CKingGuriEvent()
{
}

void CKingGuriEvent::Init()
{
	for( SI32 i = 0; i < MAX_KINGGURI_NUM; ++i )
	{
		if ( i >= MAX_KINGGURI_NUM )	continue	;	// 배열이 넘어서면 다음데이터 읽는대로 넘긴다.	초기화는 다 했잖아.
		m_bKilledKingGuri[i] = false;
		m_siKingGuriID[i] = 0;
	}

	cltEvent::Init();
}

void CKingGuriEvent::Start()
{
	Init();

	bEventSwitch = true;
#ifdef _DEBUG
	dwLeftClock = 1000 * 60 * 50; 
#else
	dwLeftClock = 1000 * 60 * 50; 
#endif	
	dwStartClock = pclClient->CurrentClock;
	dwLastInfoClock = pclClient->CurrentClock;

	//[추가 : 황진성 2007. 11. 13 일본은 대왕 구리댁 이벤트가 50 분동안.]
	if( pclClient->siServiceArea == ConstServiceArea_Japan )
	{
#ifdef _DEBUG
		dwLeftClock = 1000 * 60 * 10; 
#else
		dwLeftClock = 1000 * 60 * 50; 
#endif
	}
}

void CKingGuriEvent::Action()
{
	if( pclClient->IsCountrySwitch( Switch_KingGuriEvent ) )
	{
		if( false == bEventSwitch )
		{
			cltServer* pclserver = (cltServer*)pclClient;

			//[추가 : 황진성 2007. 11. 13 일본은 대왕 구리댁 이벤트는 정각에 발동]
			if( (pclClient->siServiceArea == ConstServiceArea_Japan	||
				pclClient->siServiceArea == ConstServiceArea_USA		)
				)
			{
				if(0 == (pclserver->sTime.wMinute%60))
				{
					MakeKingGuri();
					pclserver->pclLog->FileConfigPrint( "KingGuri", "Start" );
				}
			}
			// [영훈] 2008.01.14 한국은 오전/오후 3,7,11 정시에 발동(하루에 총 6번 발동)
			else if ( pclClient->siServiceArea == ConstServiceArea_Korea	||
				pclClient->siServiceArea == ConstServiceArea_English		||
				pclClient->siServiceArea == ConstServiceArea_NHNChina		||
				pclClient->siServiceArea == ConstServiceArea_EUROPE
				)
			{
				if ( ((pclserver->sTime.wHour == 3) || (pclserver->sTime.wHour == 7) || (pclserver->sTime.wHour == 11) 
					|| (pclserver->sTime.wHour == 15) || (pclserver->sTime.wHour == 19) || (pclserver->sTime.wHour == 23))
					&& (pclserver->sTime.wMinute == 0)
					)
				{
					MakeKingGuri();
					pclserver->pclLog->FileConfigPrint( "KingGuri", "Start" );
				}
			}
			else if( pclClient->siServiceArea == ConstServiceArea_China )
			{
				SI32 totalCount = (sizeof( KingGuriEventStartTime ) / sizeof( WORD )) / 2;

				for ( SI32 i = 0 ; i < totalCount ; ++i )
				{
					if( pclserver->sTime.wHour   == KingGuriEventStartTime[i*2]		&&
						pclserver->sTime.wMinute >= KingGuriEventStartTime[i*2+1]	&&
						pclserver->sTime.wMinute <= KingGuriEventStartTime[i*2+1] + 2 )
					{
						MakeKingGuri();
						pclserver->pclLog->FileConfigPrint( "KingGuri", "Start" );
					}
				}
			}
		}
		else
		{
			if( pclClient->CurrentClock - dwStartClock > dwLeftClock )
			{
				End();
			}
			//[추가 : 황진성 2007. 11. 14 대왕 구리댁 몬스터가 다 죽었으면 이벤트 종료.]
			else if(true == MonsterAllDie())
			{
				End();
			}
			else
			{
				SI32 SendTime = 20 * 1000; //15초 마다

				//[추가 : 황진성 2007. 11. 13 일본은 대왕 구리댁 출현 메시지 전달.]
				if( pclClient->siServiceArea == ConstServiceArea_Japan ||
					pclClient->siServiceArea == ConstServiceArea_USA   ||
					pclClient->siServiceArea == ConstServiceArea_NHNChina	||
					pclClient->siServiceArea == ConstServiceArea_Korea	)
				{
					SendTime = 200000; // 3분20초마다.
				}

				// 전체유저에게 메세지 전송
				if( pclClient->CurrentClock - dwLastInfoClock > (DWORD)SendTime )
				{
					DWORD leftClock = dwLeftClock - ( pclClient->CurrentClock - dwStartClock ); 
					SendInfo( ACTION, leftClock );
					dwLastInfoClock = pclClient->CurrentClock;
				}
			}
		}
	}
}

void CKingGuriEvent::End()
{
	bEventSwitch = false;

	SI32 index = 0;
	SI32 id = 0;
	while( id = pclClient->pclCM->GetCharOrder(index) )
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		if( NULL != pclchar)
		{
			if( pclClient->pclCM->IsValidID(id) == false )		
				continue;

			if( pclchar->siCharMode == CHARMODE_KINGGURI )	
			{
				// 셀 내에서의 정보를 삭제한다. 
				pclClient->pclCM->CR[id]->DeleteInCell();
				if( pclClient->pclCM->DeleteChar( id ) == TRUE )
				{
					index--;
				}
			}
		}
	}

	SendInfo( END, 0 );

	Init();
}


void CKingGuriEvent::MakeKingGuri()
{
	Start();

	// 맵번호 초기하.
	m_siAreaIndex[0] = m_siAreaIndex[1] = m_siAreaIndex[2] = -1;

	// 몬스터를 배치할 적절한 곳을 찾는다. 
	SI32 count		  =  0;
	SI32 randValue	  =  0;
	SI32 oldRandValue = -1;
	bool created	  =  false;

	SI32 siSafeCount	=	0	;	// 만약 만약 만약 완전 만약에 이도저도 안걸리면 최후의 수단


	while( !created )
	{
		randValue = rand() % MAX_POSITION_NUM;

		int i = 0;
		for(	; i < MAX_KINGGURI_NUM; ++i)
		{
			if( randValue == m_siAreaIndex[i])	break;
		}

		if( randValue == oldRandValue || i != MAX_KINGGURI_NUM)	continue;
		if(randValue >= MAX_POSITION_NUM)	continue	;

		oldRandValue = randValue;

		SI32 Kind = 0;
		//[추가 : 황진성 2007. 11. 13 일본은 대왕 구리댁 이벤트는 정각에 발동]
		if( (pclClient->siServiceArea == ConstServiceArea_Japan		||
			pclClient->siServiceArea == ConstServiceArea_USA		||
			pclClient->siServiceArea == ConstServiceArea_Korea		||
			pclClient->siServiceArea == ConstServiceArea_English	||
			pclClient->siServiceArea == ConstServiceArea_NHNChina	||
			pclClient->siServiceArea == ConstServiceArea_EUROPE
			)
			)
		{
			Kind = pclClient->GetUniqueFromHash(TEXT("KIND_BIGRACCOON"));
		}
		else if( pclClient->siServiceArea == ConstServiceArea_China )
		{
			Kind = pclClient->GetUniqueFromHash(TEXT("KIND_RACCOONCHINA"));
		}

		SI32 tempX = m_ptPosition[randValue].x; 
		SI32 tempY = m_ptPosition[randValue].y;

		SI32 x, y;
		if( pclClient->pclMapManager->pclMapArray[0]->FindEmptyAreaForCharLocate(Kind, tempX, tempY, 10, &x, &y, true) == true )
		{

			cltInnerStatusInfo clinnerstatus;
			clinnerstatus.clEnemyType.Set(ENEMYTYPE_ALLPC, 0);

			cltServer* pclserver = (cltServer*)pclClient;

			SI32 id = pclserver->SetNPCChar(Kind, VILLAGE_NONE, NULL, 1, 0, x, y, 0, NPCITEMMODE_NORMAL, &clinnerstatus );
			if(id)
			{
				if(count >= MAX_KINGGURI_NUM) continue	;
				m_siKingGuriID[count] = id;
				StringCchCopy( m_szSelectedAreaName[count], 32, m_szAreaName[randValue] );

				//[추가중 : 황진성 2007. 11. 13 맵 인덱스 번호.]
				m_siAreaIndex[count] = randValue;

				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				pclchar->siCharMode = CHARMODE_KINGGURI;
				count++;

				if( count == MAX_KINGGURI_NUM )
					created = true;
			}
		}
		siSafeCount++;
		if( siSafeCount >= 10000)	break	;	// 만약 만약 만약 만약을 위한 1만번을 돌아서 이걸 못하면 무한루프에 돌았다고 생각 함 그냥 현제 소환한놈에서 넘겨 이건 진짜 만약 만약만약 만약 을 위한 코드
	}


	SendInfo( START, dwLeftClock );
}

void CKingGuriEvent::SendInfo( SI32 Type, DWORD LeftTime, TCHAR* UserNmae )
{
	cltGameMsgResponse_KingGuriInfo clKingGuriInfo( Type, LeftTime, m_szSelectedAreaName, UserNmae, &m_ptPosition[0], &m_siAreaIndex[0]);
	cltMsg clMsg( GAMEMSG_RESPONSE_KINGGURIINFO, sizeof(clKingGuriInfo), (BYTE*)&clKingGuriInfo );

	SI32 index = 0;
	SI32 id = 0;
	while(id = pclClient->pclCM->GetCharOrder(index))
	{
		index++;

		// 사용자인 컴에게만 전달한다. 
		if( true == pclClient->pclCM->CR[id]->IsValidConnection() )
		{
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void CKingGuriEvent::SetDieKingGuri( bool Flag, SI32 KingGuriID, SI32 UserID )
{
	for( SI32 i = 0; i < MAX_KINGGURI_NUM; ++i )
	{
		if( m_siKingGuriID[i] == KingGuriID )
		{
			if( false == m_bKilledKingGuri[i] )
			{
				m_bKilledKingGuri[i] = Flag;

				//[추가 : 황진성 2007. 11. 13. 대왕 구리댁이 죽어서 해당 아이디 제거와 맵이름.]
				m_siKingGuriID[i]	 = 0;
				ZeroMemory(m_szSelectedAreaName[i], sizeof(m_szSelectedAreaName[i]));

				if( true == pclClient->pclCM->IsValidID(UserID) )		
				{
					TCHAR* userName = (TCHAR*)pclClient->pclCM->CR[UserID]->GetName();
					SendInfo( DIE_KINGGURI, 0, userName );
				}
			}
		}
	}

	// 몬스터가 모두 죽었으면 이벤트 종료.
	if(true == MonsterAllDie())
	{
		SendInfo( END, 0 );
	}
	// 몬스터가 살아 있으면 몬스터의 위치를 다시 공지.
	else
	{
		//[추가중 : 황진성 2007. 11. 13 대왕 구리댁이 죽어서 위치를 공지함.]
		DWORD leftClock = dwLeftClock - ( pclClient->CurrentClock - dwStartClock ); 
		SendInfo( ACTION, leftClock );
		dwLastInfoClock = pclClient->CurrentClock;
	}
}

bool CKingGuriEvent::MonsterAllDie(void)
{
	// 몬스터가 모조리 죽었는가 아닌가?
	bool MonAllDie = true;
	for( SI32 i = 0; i < MAX_KINGGURI_NUM; ++i )
	{
		if(0 != m_siKingGuriID[i])	
		{
			MonAllDie = false;
			break;
		}
	}

	return MonAllDie;
}