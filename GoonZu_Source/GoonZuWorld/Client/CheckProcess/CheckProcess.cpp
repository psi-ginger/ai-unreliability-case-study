//#pragma comment(lib, "psapi.lib")

#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"

#include "../common/Char/CharManager/CharManager.h"
#include "../common/Char/CharServer/Char-Server.h"

#include "../dxutil.h"

#include "CheckProcess.h"
//#include "Psapi.h"
#include "TlHelp32.h"



extern cltCommonLogic* pclClient;


//------------------------------------------------------------------------------------------------------------------------------
// 여기에서 부터는 Server에서만 사용됩니다
//------------------------------------------------------------------------------------------------------------------------------
cltCheckProcessMacroList::cltCheckProcessMacroList()
{
	Init();
}

cltCheckProcessMacroList::~cltCheckProcessMacroList()
{
	for ( SI32 siIndex=0; siIndex<Const_Max_MacroList; ++siIndex )
	{
		SAFE_DELETE( m_pclMacro[siIndex] );
	}
}

void cltCheckProcessMacroList::Init( void )
{
	ZeroMemory( this, sizeof(cltCheckProcessMacroList) );
}

bool cltCheckProcessMacroList::AddMacroName( TCHAR* pszMacroName )
{
	if ( (NULL == pszMacroName) || (_tcscmp(pszMacroName, TEXT("")) == 0) )
	{
		return false;
	}

	if ( Const_Max_MacroList <= m_siListCount )
	{
		return false;
	}

	m_pclMacro[m_siListCount] = new cltCheckProcessMacro;
	if ( NULL == m_pclMacro[m_siListCount] )
	{
		return false;
	}

	m_pclMacro[m_siListCount]->Set( pszMacroName );

	++m_siListCount;

	return true;
}

bool cltCheckProcessMacroList::GetMacroNameFirst( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize )
{
	m_siReadCount = 0; // 현재 읽어야할 리스트의 번호를 처음으로 되돌림

	bool bReturnValue = GetMacorNameNext( pszTextBuffer, siBufferSize );

	return bReturnValue;
}

bool cltCheckProcessMacroList::GetMacorNameNext( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize )
{
	if ( false == GetMacorNameFromIndex( pszTextBuffer, siBufferSize, m_siReadCount ) )
	{
		return false;
	}

	// GetMacroNameNext에서 쓰여질 변수를 하나 늘려줌
	++m_siReadCount;

	return true;
}

bool cltCheckProcessMacroList::GetMacorNameNow( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize )
{
	bool bReturnValue = GetMacorNameFromIndex( pszTextBuffer, siBufferSize, m_siReadCount );

	return bReturnValue;
}

bool cltCheckProcessMacroList::GetMacorNameFromIndex( OUT TCHAR* pszTextBuffer, IN SI32 siBufferSize, IN SI32 siIndex )
{
	if ( (NULL == pszTextBuffer) || (0 >= siBufferSize) )
	{
		return false;
	}

	if ( 0 >= m_siListCount )
	{
		return false;
	}
	
	// 검사하려는 인덱스 번호가 현재 존재하는 리스트 개수보다 크면 안된다
	if ( siIndex >= m_siListCount )
	{
		return false;
	}

	// 혹시 몰라서 NULL 검사
	if ( NULL == m_pclMacro[siIndex] )
	{
		return false;
	}

	StringCchCopy( pszTextBuffer, siBufferSize, m_pclMacro[siIndex]->GetMacroName() );

	return true;

}

void cltCheckProcessMacroList::LoadMacroList( void )
{
	TCHAR		szFileName[]	= TEXT("ServerInfo/MacroList.txt");
	BOOL		bLoadFile		= FALSE;
	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( szFileName );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( szFileName );
	}

	if (!bLoadFile)
	{
		return;
	}

	TCHAR szMacroName[64] = TEXT("");

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_MBSTRING,	szMacroName,	64,
		0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI16 index = 0;

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			AddMacroName( szMacroName );
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
//------------------------------------------------------------------------------------------------------------------------------

cltCheckProcessPersonInfo::cltCheckProcessPersonInfo()
{
	Init();
}

cltCheckProcessPersonInfo::~cltCheckProcessPersonInfo()
{
}

void cltCheckProcessPersonInfo::Init()
{
	ZeroMemory( this, sizeof(cltCheckProcessPersonInfo) );
}

//------------------------------------------------------------------------------------------------------------------------------
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
//------------------------------------------------------------------------------------------------------------------------------

cltCheckProcessManager::cltCheckProcessManager()
{
	Init();
}

cltCheckProcessManager::~cltCheckProcessManager()
{
	DestroyPersonInfo();
}

void cltCheckProcessManager::DestroyPersonInfo()
{
	for ( SI32 siIndex=0; siIndex<MAX_PERSON_NUMBER; ++siIndex )
	{
		SAFE_DELETE( m_pclCP_PersonInfo[siIndex] );
	}

}

void cltCheckProcessManager::Init()
{
	ZeroMemory( this, sizeof(cltCheckProcessManager) );

	// 1분으로 설정
#ifdef _DEBUG
	m_kUpdateTimer_WaitTime.Init(5 * 1000);
#else
	m_kUpdateTimer_WaitTime.Init(60 * 1000);
#endif
}

void cltCheckProcessManager::InitForLog()
{
	DestroyPersonInfo();

	ZeroMemory( m_szModuleName, sizeof(m_szModuleName) );

	m_siPersonCount = 0;

	m_siGMPersonID	= 0;
}

void cltCheckProcessManager::Action()
{
	if ( CHECKPROCESS_STATE_READY == m_siState )
	{
		return;
	}

	if ( m_kUpdateTimer_WaitTime.IsTimed(::GetTickCount(), false) ) 
	{
		// 일단 실행 상태 변경
		m_siState = CHECKPROCESS_STATE_READY;

		switch ( m_siType ) 
		{
			case CHECKPROCESS_TYPE_MULTICLIENT:
				{
					WriteLog();
				}
				break;
			
			// 메크로라면 리스트에 있는 다음 목록을 검사한다
			case CHECKPROCESS_TYPE_MACRO:
				{
					// 로그를 기록하면 모두 초기화가 되기때문에 여기에서 GM에 PersonID를 기록해 놓는다
					SI32 siGMPersonID = m_siGMPersonID;

					WriteLog();

					// 리스트에 있는 다음 모듈을 검사한다
					TCHAR szModuleName[Const_Max_Module_Name] = { '\0', };
					if ( false == m_clMacroList.GetMacorNameNext(szModuleName, sizeof(szModuleName)) )
					{
						return;
					}

					// 저장해 놓아던 GM PersonID로 되돌림
					m_siGMPersonID = siGMPersonID;

					SendOrderToClient( m_siGMPersonID, CHECKPROCESS_TYPE_MACRO, szModuleName );
				}
				break;

			case CHECKPROCESS_TYPE_MODULENAME:
				{
					WriteLog();
				}
				break;
		}	
	}
	
}

bool cltCheckProcessManager::SendOrderToClient( SI32 siGMPersonID, SI32 siType, TCHAR* pszModuleName/* =NULL  */)
{
	// 한번에 하나만 실행될 수 있도록 이미 실행중이라면 실행못한다
	if ( CHECKPROCESS_STATE_PROCESSING == m_siState )
	{
		return false;
	}

	if ( (CHECKPROCESS_TYPE_NONE >= siType) || (CHECKPROCESS_TYPE_MAX <= siType) )
	{
		return false;
	}

	
	ZeroMemory( m_szModuleName, sizeof(m_szModuleName) );


	switch ( siType )
	{
		case CHECKPROCESS_TYPE_MULTICLIENT:
			{
				// 일단은 없음				
			}
			break;

		case CHECKPROCESS_TYPE_MACRO:
			{
				if ( NULL == pszModuleName )
				{
					// 모듈네임이 인수로 들어오지 않을때는 메크로 리스트의 맨 처음 이름을 갖고온다
					if ( false == m_clMacroList.GetMacroNameFirst(m_szModuleName, sizeof(m_szModuleName)) )
					{
						return false;
					}
				}
				else
				{
					// 그렇지 않을경우 인수로 들어온 이름으로
					if ( _tcscmp(pszModuleName, TEXT("")) == 0 )
					{
						return false;
					}

					StringCchCopy( m_szModuleName, sizeof(m_szModuleName), pszModuleName );
				}
			}
			break;

		case CHECKPROCESS_TYPE_MODULENAME:
			{
				// 운영자가 설정한 이름으로 설정
				if ( (NULL == pszModuleName) || (_tcscmp(pszModuleName, TEXT("")) == 0) )
				{
					return false;
				}
				
				StringCchCopy( m_szModuleName, sizeof(m_szModuleName), pszModuleName );
			}
			break;
	}

	// GM ID 저장
	m_siGMPersonID	= siGMPersonID;

	// 실행 타입을 셋한다
	m_siType		= siType;

	// 현재를 실행중으로 변경하고
	m_siState		= CHECKPROCESS_STATE_PROCESSING;

	// 접속한 모든 유저에게 프로세스체크를 보낸다
	cltGameMsgResponse_CheckProcess clInfo( m_szModuleName, siType );
	cltMsg clMsg( GAMEMSG_RESPONSE_CHECKPROCESS, sizeof(clInfo), (BYTE*)&clInfo );
	pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
	
	// 기다릴 시간을 작동한다
	m_kUpdateTimer_WaitTime.SetActive( true , ::GetTickCount() );

	return true;
}

void cltCheckProcessManager::SendResultToGM( SI16* psiList )
{
	if ( NULL == psiList )
	{
		return;
	}
	
	SI32 siGMCharID = pclClient->pclCM->GetIDFromPersonID( m_siGMPersonID );

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siGMCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	// 혹시 지금 저장한 ID가 GM이 아닐때
	if ( BASIC_GAMEMASTER_LEVEL >= pclchar->GetGameMasterLevel() )
	{
		return;
	}


	cltGameMsgResponse_CheckProcess_Result clInfo( m_szModuleName, psiList, m_siType );
	cltMsg clMsg( GAMEMSG_RESPONSE_CHECKPROCESS_RESULT, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );


}

void cltCheckProcessManager::CompareSameUser( IN SI16* psiMultiClientList, OUT SI16* psiSendList )
{
	if ( (NULL == psiMultiClientList) || (NULL == psiSendList) )
	{
		return;
	}

	// 2개부터 21개까지의 인원을 60명 저장
	SI16 siCompareList[Const_Max_SendList][60];
	memset( siCompareList, -1, sizeof(siCompareList) );


	for ( SI32 siIndex=0; siIndex<MAX_PERSON_NUMBER; ++siIndex )
	{
		SI16 siListIndex = psiMultiClientList[siIndex];

		if ( -1 >= siListIndex )
		{
			break;
		}

		cltCheckProcessPersonInfo* pclPersonInfo = m_pclCP_PersonInfo[siListIndex];
		if ( NULL == pclPersonInfo )
		{
			continue;
		}

		SI16 siRealSearchIndex = (pclPersonInfo->GetProcessCount() - 2);

		// 혹시 그럴일은 없겠지만 멀티클라의 갯수가 22개가 넘어가면
		if ( (Const_Max_SendList+2) <= siRealSearchIndex )
		{
			//여기에서 무엇을 할것인가 정해야한다
			continue;
		}
		
		
		for ( SI32 siSearchIndex=0; siSearchIndex<60; ++siSearchIndex )
		{
			if ( -1 >= siCompareList[siRealSearchIndex][siSearchIndex] )
			{
				siCompareList[siRealSearchIndex][siSearchIndex] = siListIndex;
				break;
			}
			else if ( true == pclPersonInfo->IsSame( m_pclCP_PersonInfo[siCompareList[siRealSearchIndex][siSearchIndex]] ) )
			{
				break;
			}
		}
	}

	for ( SI32 siCount=0; siCount<Const_Max_SendList; ++siCount )
	{
		for ( SI32 siListCount=0; siListCount<60; ++siListCount )
		{
			if ( -1 < siCompareList[siCount][siListCount] )
			{
				psiSendList[siCount] += 1;
			}
		}
	}

}

bool cltCheckProcessManager::AddPersonInfo( SI32 siCharID, SI16 siProcessCount, TCHAR* pszMacAddress )
{
	if ( MAX_PERSON_NUMBER <= m_siPersonCount )
	{
		return false;
	}

	cltCharServer* pclchar = pclClient->pclCM->GetCharServer( siCharID );
	if ( NULL == pclchar )
	{
		return false;
	}

	m_pclCP_PersonInfo[ m_siPersonCount ] = new cltCheckProcessPersonInfo;
	if ( NULL == m_pclCP_PersonInfo[m_siPersonCount] )
	{
		return false;
	}

	m_pclCP_PersonInfo[ m_siPersonCount ]->Set( pclchar->pclCI->GetPersonID(), siCharID, siProcessCount, pclchar->GetSession()->GetIP(), pszMacAddress );

	++m_siPersonCount;

	return true;

}

void cltCheckProcessManager::WriteLog( void )
{
	TCHAR szFileName[MAX_PATH] = { '\0', }; 
	if ( false == GetSaveLogFileName(szFileName, sizeof(szFileName)) )
	{
		return;
	}

	// 멀티 클라이언트를 사용하는 유저의 인덱스만 저장
	SI16 siMulticlientList[MAX_PERSON_NUMBER];
	SI16 siListCount = 0;

	// 메크로 사용자 카운트
	SI16 siUserCount = 0;

	// 인덱스는 0부터 시작하니까 -1로 초기화
	memset( siMulticlientList, -1, sizeof(siMulticlientList) );

	for ( SI32 siIndex=0; siIndex<MAX_PERSON_NUMBER; ++siIndex )
	{
		if ( NULL == m_pclCP_PersonInfo[siIndex] )
		{
			break;
		}

		cltCheckProcessPersonInfo* pclPersonInfo = m_pclCP_PersonInfo[siIndex];
		if ( NULL == pclPersonInfo )
		{
			continue;
		}
		
		// 멀티클라이언트 사용자 체크시에만 쓰임
		if ( 2 <= pclPersonInfo->GetProcessCount() )
		{
			siMulticlientList[siListCount] = siIndex;
			
			++siListCount;
		}

		// 메크로/모듈 체크시에만 쓰임
		++siUserCount;

		// 여기서 NULL 나와도 아래에서 체크합니다
		cltCharServer* pclchar = pclClient->pclCM->GetCharServer( pclPersonInfo->GetCharID() );

		NTCHARString512 strLog( TEXT("#personid#\t#noname#\t#count#\t#ip#\t#mac#") );

		strLog.Replace( TEXT("#personid#"),	SI32ToString(pclPersonInfo->GetPersonID()) );
		strLog.Replace( TEXT("#count#"),	SI32ToString(pclPersonInfo->GetProcessCount()) );
		strLog.Replace( TEXT("#ip#"),		pclPersonInfo->GetIP() );
		strLog.Replace( TEXT("#mac#"),		pclPersonInfo->GetMacAddress() );
		
		// 혹시 PersonID가 다를수도 있기때문에 같이 검사
		if ( (pclchar) && (pclchar->pclCI->GetPersonID() == pclPersonInfo->GetPersonID()) )
		{
			strLog.Replace( TEXT("#noname#"), pclchar->GetName() );
		}

		pclClient->pclLog->FilePrint( szFileName, (TCHAR*)strLog );
	}

	// 로그 작성후 결과를 요청한 GM에게 보낸다
	SI16 siSendList[Const_Max_SendList] = { 0, };

	switch ( m_siType )
	{
		case CHECKPROCESS_TYPE_MULTICLIENT:
			{
				CompareSameUser( siMulticlientList, siSendList );
			}
			break;
		case CHECKPROCESS_TYPE_MACRO:
		case CHECKPROCESS_TYPE_MODULENAME:
			{
				siSendList[0] = siUserCount;
			}
			break;
	}

	SendResultToGM( siSendList );

	// 모든 로그 관련 정보를 초기화 한다
	InitForLog();
}

bool cltCheckProcessManager::GetSaveLogFileName( TCHAR* pszTextBuffer, SI32 siBufferSize )
{
	if ( (NULL == pszTextBuffer) || (0 >= siBufferSize) )
	{
		return false;
	}

	cltServer* pclserver = (cltServer*)pclClient;
	if ( NULL == pclserver )
	{
		return false;
	}

	UI16 uiYear		= pclserver->sTime.wYear;
	UI16 uiMonth	= pclserver->sTime.wMonth;
	UI16 uiDay		= pclserver->sTime.wDay;

	switch ( m_siType )
	{
		case CHECKPROCESS_TYPE_MULTICLIENT:
			{
				StringCchPrintf( pszTextBuffer, siBufferSize, TEXT("Config/MulitClient_%04d%02d%02d.log"), uiYear, uiMonth, uiDay );
			}
			break;

		case CHECKPROCESS_TYPE_MACRO:
			{
				StringCchPrintf( pszTextBuffer, siBufferSize, TEXT("Config/Macro_%s_%04d%02d%02d.log"), m_szModuleName, uiYear, uiMonth, uiDay );
			}
			break;

		case CHECKPROCESS_TYPE_MODULENAME:
			{
				StringCchPrintf( pszTextBuffer, siBufferSize, TEXT("Config/Module_%s_%04d%02d%02d.log"), m_szModuleName, uiYear, uiMonth, uiDay );
			}
			break;

		default:
			return false;
	}

	return true;
}

void cltCheckProcessManager::LoadMacroList( void )
{
	m_clMacroList.LoadMacroList();
}

//------------------------------------------------------------------------------------------------------------------------------
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
// 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다 경계선 입니다
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
// 여기에서 부터는 Client에서만 사용됩니다
//------------------------------------------------------------------------------------------------------------------------------

cltCheckProcess::cltCheckProcess()
{
	Init();
}

cltCheckProcess::~cltCheckProcess()
{
}

void cltCheckProcess::Init()
{
	ZeroMemory( this, sizeof(cltCheckProcess) );

	// 내 모듈 이름(군주)가져오기
	GetGoonZuModuleName( m_szGoonZuModuleName, sizeof(m_szGoonZuModuleName) );
}

// 지금 내 모듈 이름을 가져온다
BOOL cltCheckProcess::GetGoonZuModuleName( TCHAR* pszGoonZuName, SI32 siTextSize )
{
	// 98 호환성을 위해 아래 내용 변경
	/*
	HANDLE hThisProcess = GetCurrentProcess();

	if ( hThisProcess )
	{
		if ( 0 < GetModuleBaseName(hThisProcess, NULL, pszGoonZuName, siTextSize) )
		{
			return TRUE;
		}
	}

	return FALSE;
	*/

	if ( (NULL == pszGoonZuName) || (0 >= siTextSize) )
	{
		return FALSE;
	}

	switch ( pclClient->siServiceArea )
	{
	case ConstServiceArea_Japan:
		{
			StringCchCopy( pszGoonZuName, siTextSize, TEXT("Kunshu.exe") );
		}
		break;

	case ConstServiceArea_EUROPE:
	case ConstServiceArea_USA:
		{
			StringCchCopy( pszGoonZuName, siTextSize, TEXT("Luminary.exe") );
		}
		break;

	default:
		{
			StringCchCopy( pszGoonZuName, siTextSize, TEXT("GoonZu.exe") );
		}
		break;
	}

	return TRUE;
}

/* 이녀석은 95,98d은 지원이 안되니까 아래 만든 녀석으로 쓴다
SI32 cltCheckProcess::GetGoonZuProcessCount()
{
	// 내 모듈 이름(군주)가져오기
	TCHAR		szGoonZuModuleName[Const_Max_Module_Name] = { '\0',  };
	if ( FALSE == GetGoonZuModuleName(szGoonZuModuleName, sizeof( szGoonZuModuleName )) )
	{
		return 0;
	}
	
	SI32		siReturnCount			= 0;

	DWORD		dwProcessList[1024]		= {0, };
	DWORD		cb						= 0;
	SI32		siProcessCount			= 0;

	HANDLE		hProcess;
	HMODULE		hModule;
	TCHAR		szProcessName[256]		= {'\0', };

	// 프로세스 목록을 배열에 구하고 개수를 개산.
	::EnumProcesses(dwProcessList, sizeof(dwProcessList), &cb);
	siProcessCount = cb/sizeof(cb);

	// 얻어낸 프로세스 목록에서 현재 내 이름과 같은 프로세스를 검색함
	for ( SI32 siIndex=0; siIndex<siProcessCount; ++siIndex )
	{
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessList[siIndex] );

		if ( (hProcess) && (EnumProcessModules(hProcess, &hModule, sizeof( hModule ), &cb)) )
		{
			GetModuleBaseName( hProcess, hModule, szProcessName, sizeof(szProcessName) );
			if ( (_tcscmp( szProcessName, szGoonZuModuleName ) == 0) )
			{
				++siReturnCount;
			}
		} 
	}

	return siReturnCount;
}
*/

SI32 cltCheckProcess::GetGoonZuProcessCount()
{
	SI32 siReturnCount = GetProcessCountFormModuleName( m_szGoonZuModuleName );

	return siReturnCount;
	
}

// win95/98/2000 이후(nt4.0은 지원못함-설마 여기서 군주를?)
SI32 cltCheckProcess::GetProcessCountFormModuleName( const TCHAR* pszModuleName )
{
	if ( NULL == pszModuleName )
	{
		return 0;
	}

	if ( _tcscmp(pszModuleName, TEXT("")) == 0 )
	{
		return 0;
	}

	SI32	siReturnCount = 0;

	HANDLE	hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( INVALID_HANDLE_VALUE == hSnap )
	{
		return 0;
	}

	PROCESSENTRY32	peEntry;	// 모듈정보를 받아올 구조체
	peEntry.dwSize = sizeof(PROCESSENTRY32);

	SI32 siExitCount	= 0;	// 탈출용

	BOOL bContinue		= Process32First( hSnap, &peEntry );


	while ( bContinue )
	{
		// 혹시 반복문이 무한정 실행될까봐
		if ( Const_Max_Exit_Count <= siExitCount++ )
		{
			break;
		}

		// 인수의 모듈이름과 같은지
		if ( _tcscmp(peEntry.szExeFile, pszModuleName) == 0 )
		{
			++siReturnCount;
		}

		bContinue = Process32Next( hSnap, &peEntry );
	}

	CloseHandle( hSnap );


	return siReturnCount;
}

void cltCheckProcess::ExcuteCheckProcess( SI32 siType, TCHAR* pszModuleName )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
	if ( NULL == pclchar )
	{
		return;
	}

	SI16 siModuleCount = 0;

	// 모듈 체크 타입에 따른 분류
	switch ( siType )
	{
		case CHECKPROCESS_TYPE_MULTICLIENT:
			{
				siModuleCount = GetGoonZuProcessCount();
			}
			break;

		case CHECKPROCESS_TYPE_MACRO:
		case CHECKPROCESS_TYPE_MODULENAME:
			{
				if ( (NULL == pszModuleName) || (_tcscmp(pszModuleName, TEXT("")) == 0) )
				{
					return;
				}
			
				siModuleCount = GetProcessCountFormModuleName( pszModuleName );
			}
			break;

		default:
			{
				return;
			}
	}

	// 체크했는데 이것이 모듈이 0이라면 보낼 필요없음
	if ( 0 >= siModuleCount )
	{
		return;
	}

	// 클라이언트의 MacAddress를 가져온다
	TCHAR szMacAddress[Const_Max_MacAddress] = { '\0', };
	pclclient->GetMacAddress( pclchar->GetSession()->GetSocket(), szMacAddress, sizeof(szMacAddress) );
	

	cltGameMsgRequest_CheckProcess clInfo( szMacAddress, siModuleCount );
	cltMsg clMsg( GAMEMSG_REQUEST_CHECKPROCESS, sizeof(clInfo), (BYTE*)&clInfo );
	pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );
}