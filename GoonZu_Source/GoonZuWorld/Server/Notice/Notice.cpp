#include "Notice.h"
#include "..\Server.h"

#include "..\..\CommonLogic\CommonLogic.h"

#include "Char\CharManager\CharManager.h"


extern cltCommonLogic* pclClient;

cltNoticeManager::cltNoticeManager()
{
	bLoginNoticeSwitch	= false;
	//	strcpy(szLogInNotice, TEXT(""));
	for ( SI32 i = 0; i <MAX_NOTICE_NUM ; i++) 
	{
		Counting[i]	=	0	;
		Count[i]	=	0	;
	}

}

cltNoticeManager::~cltNoticeManager()
{

}


// 공지를 추가한다. 
void cltNoticeManager::Add(DWORD interval, TCHAR* ptext, SI32 siCount )
{
	SI32 i;

	if(interval  < 100 )		interval = 100;
	if(interval  >  1728000)	interval = 1728000;	// [종호] 30분->24시간 변경

	for(i = 0;i < MAX_NOTICE_NUM;i++)
	{
		if(clNotice[i].dwStartFrame == 0)
		{
			clNotice[i].Set(pclClient->GetFrame(), interval, ptext);
			Count[i]	=	siCount	;
			return ;
		}
	}

}


// 공지를 보여준다. 
void cltNoticeManager::Action()
{
	SI32 i;

	cltServer* pclserver = (cltServer*)pclClient;

	for(i = 0;i < MAX_NOTICE_NUM;i++)
	{
		if(clNotice[i].dwStartFrame)
		{
			if((pclClient->GetFrame() - clNotice[i].dwStartFrame) % clNotice[i].dwInterval == 0)
			{
				if ( Count[i] > 0 && Count[i] >= 100 ) 
				{
					TCHAR* pText = GetTxtFromMgr(5482); 

					// 운영자가 하는것이라면 채팅창에도 나타나게한다.
					//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
					cltGameMsgResponse_Notice clNotice(pText, clNotice[i].szNoticeText,true);
					cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

					pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

					Counting[i]++;
				}
				else if( Count[i] > 0 )	// 카운팅 해서 넘어갔으면 정지 시킨다 카운팅은 초기화
				{
					//pclserver->SendNoticeMsg(-1, 0, 0, clNotice[i].szNoticeText);

					if ( Counting[i] >= Count[i] ) 
					{		
						Counting[i]	=	0	;
                        Count[i]	=	0	;
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(5482); 

						// 운영자가 하는것이라면 채팅창에도 나타나게한다.
						//KHY - 0129 - 하단 채팅창에 공지 출력여부를 결정한다.
						cltGameMsgResponse_Notice clNotice(pText, clNotice[i].szNoticeText,true);
						cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

						pclClient->pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

						Counting[i]++;
					}				
				}
				else
				{
					// 카운팅 초기화
					Counting[i]	=	0	;
					Count[i]	=	0	;

				}
			}
		}
	}
}


// 모든 공지를 지운다. 
void cltNoticeManager::DelAllNotice()
{
	SI32 i;

	for(i = 0;i < MAX_NOTICE_NUM;i++)
	{
		clNotice[i].Init();
	}

}

// 로그인 공지의 on/ff
void cltNoticeManager::OnOffLogInNotice()
{
	if(bLoginNoticeSwitch)
	{
		bLoginNoticeSwitch = false;

	}
	else
	{
		bLoginNoticeSwitch = true;
		/*
		// 공지 내용을 읽어온다. 
		FILE* fp;
		TCHAR buffer[1024];

		strcpy(szLogInNotice , TEXT(""));

		fp = _tfopen(TEXT("Notice\\LogInNotice.txt"), TEXT("rt"));
		if(fp)
		{
		while(_fgetts(buffer, 1024, fp) != NULL)
		{
		for(SI32 i = 0; i < lstrlen(buffer);i++)
		{
		if(buffer[i] == '\n')
		{
		buffer[i] = ' ';
		}
		}

		strcat(szLogInNotice, buffer);
		strcat(szLogInNotice, TEXT("\r\n"));
		}

		fclose(fp);
		}

		*/		

	}
}
