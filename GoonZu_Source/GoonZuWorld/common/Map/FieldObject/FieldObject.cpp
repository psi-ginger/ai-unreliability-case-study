//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
#include "CommonHeader.h"

#include "FieldObject.h"

#include "..\CommonLogic\CommonLogic.h"

//------------------------------------
// Lib
//------------------------------------
#include <Graphic.h>
#include <Graphic_Capsule.h>
#include "EtcTool\EtcTool.h"
#include "NDataloader.h"
#include "NHashTableString.h"

#include "../../../../patch/unzip.h"

extern cltCommonLogic* pclClient;

struct cltFOT
{	
	int		type;
	TCHAR	typeString[ 64 ];
	int		color;
	TCHAR	atb[ 64 ];
	TCHAR	parentType[ 64 ];
};

//-------------------------------------------------
// Field Object Type Info
//-------------------------------------------------
cltFieldObjectTypeInfo::cltFieldObjectTypeInfo(DWORD type, UI08 color, TCHAR* name, DWORD atb, cltFieldObjectTypeInfo* pclinfo)
{

	Unique		= 0;
	dwType		= type;
	uiColor		= color;
	pParent		= pclinfo;
	dwAtb		= atb;

	if(name == NULL)
	{
		MsgBox(TEXT("vc8923jf"), TEXT("vd23trf:%d"), type);
	}
	pszName		= new TCHAR [lstrlen(name) + 1];
	StringCchCopy(pszName, lstrlen(name) + 1, name);


}

cltFieldObjectTypeInfo::~cltFieldObjectTypeInfo()
{
	if(pszName)
	{
		delete[] pszName;
		pszName = NULL;
	}
}

//-------------------------------------------------
// Field Object Info
//-------------------------------------------------
cltFieldObjectInfo::cltFieldObjectInfo(SHORT unique, DWORD dwtype, DWORD dwimagefilenumber,
									   TCHAR* szname, SI32 transmode,SI32 alphavalue,SI32 shadow,TCHAR* zipname)
{
	pszName		= NULL;
	pszZipName	= NULL;

	if(unique > 30000)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("Not Proper FieldObjectSet Unique. Over 30000"), TEXT("Error"), MB_OK|MB_TOPMOST);
		Unique	= 0;
	}
	else
	{
		Unique	= unique;
	}
	
	dwType	= dwtype;

	if(szname == NULL)
	{
		MsgBox(TEXT("vdxs"), TEXT("V3fds:%d"), unique);
	}
	pszName	= new TCHAR [lstrlen(szname) + 1];
	StringCchCopy(pszName, lstrlen(szname) + 1, szname);

	pszZipName	= new TCHAR [lstrlen(zipname) + 1];
	StringCchCopy(pszZipName, lstrlen(zipname) + 1, zipname);

	siTransMode = transmode;
	if ( alphavalue < 0 || alphavalue > 8 )
		alphavalue = 8 ;
	siAlphaValue = alphavalue ;
	if ( shadow < 0 )
		shadow = 0 ;
	siTreeShadow = shadow ;

	siFOSprRef		= -1;

	// 파일에서 배열 정보를 읽어온다. 
	Init();

	//TCHAR szfilename[128];
	//if(GetInfoFileName(szfilename) == true)
	//{
	//	ReadSetArrayInfo(szfilename);
	//}

	if( pclClient->GameMode != GAMEMODE_CLIENT )
	{
		// 사이즈등의 상세 정보를 하드에서 읽어온다.
		LoadDetailInfo();
	}

}

cltFieldObjectInfo::~cltFieldObjectInfo()
{

	if(pszName)
	{
		delete[] pszName;
		pszName = NULL;
	}

	if(pszZipName)
	{
		delete[] pszZipName;
		pszZipName = NULL;
	}

}

void cltFieldObjectInfo::Init()
{
	TileXsize	= TileYsize		= 0;
	TileCenterX	= TileCenterY	= 0;
	IndentX		= IndentY		= 0;
	AniFrame	= AniDelay		= 1;
}

bool cltFieldObjectInfo::GetInfoFileName(TCHAR* szfilename)
{
	if(szfilename == NULL)return false;

	StringCchPrintf(szfilename, 128, TEXT("FieldObject\\%s\\%s.txt"), pszName, pszName);

	return true;
}

cltFieldObjectInfo* cltFieldObjectManager::GetInfoPointer(TCHAR* szname)
{
	SI32 i;

	// "NULL"이 입력되면 NULL을 리턴한다. 
	if(_tcscmp(szname, TEXT("NULL")) == 0)
		return NULL;

	for(i = 0;i < MAX_FIELDOBJECTSET_NUMBER;i++)
	{
		if(pclInfo[i])
		{
			if(_tcscmp(pclInfo[i]->pszName, szname) == 0)
			{
				return pclInfo[i];
			}
		}
	}

	return NULL;

}

// 이미지를 담고 있는 공간의 포인터를 리턴한다. 
TSpr* cltFieldObjectInfo::GetSpr(cltFieldObjectManager* pFOManager, DWORD dwframe)
{
	if(siFOSprRef < 0)
	{
		//---------------------------------------
		// 만약 로딩이 되어 있지 않다면 로딩한다.
		//---------------------------------------
		pFOManager->LoadSet(dwframe, this);
	}

	// 마지막으로 사용된 프레임을 확인한다.  사용하지 않는 경우에 삭제하기 위해서. 
	SetLastUseFrame(pFOManager, dwframe);

	return &pFOManager->FOSpr[siFOSprRef];
}

// 마지막으로 사용된 프레임을 설정한다. 
void cltFieldObjectInfo::SetLastUseFrame(cltFieldObjectManager* pFOManager, DWORD frame)
{
	if(siFOSprRef < 0)return ;

	pFOManager->dwLastUseFrame[siFOSprRef] = frame;
}

// 배열 정보를 읽어온다. 
bool cltFieldObjectInfo::ReadSetArrayInfo(TCHAR* szfilename)
{

	SI32 i;
	FILE* fp;
	TCHAR buffer[1024];

	fp = _tfopen(szfilename, TEXT("rt"));
	if(fp == NULL)
	{
#ifdef _DEBUG
		if (pclClient->GameMode == GAMEMODE_CLIENT) 
			MessageBox(NULL, szfilename, TEXT("File Read Error"), MB_OK|MB_TOPMOST);
#endif
		return false;
	}

	// 첫번째 3줄은 무시한다. 
	for(i = 0;i< 3;i++)
		_fgetts(buffer, 1024, fp);

	_fgetts(buffer, 1024, fp);

	_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d"), 
			&TileXsize,		&TileYsize,
			&TileCenterX,	&TileCenterY,
			&IndentX,		&IndentY,
			&AniFrame,		&AniDelay);

	fclose(fp);

	return true;

}

bool cltFieldObjectInfo::ReadSetArrayInfoFromMemory(BYTE* pMemory)						// 배열 정보를 읽어온다. 
{
	if( pMemory == NULL )				return false;

	TCHAR* pszString = (TCHAR*)pMemory;

	int ptr = 0;
	int line = 0;
	
	// 첫번째 3줄은 무시한다. 
	//	for(i = 0;i< 3;i++)
	//		_fgetts(buffer, 1024, fp);
	while( pszString[ptr] != '\0' )
	{
		if( _tcsncmp(&pszString[ptr], "\n",   1) == 0 )			{			line++;					}
		else if( _tcsncmp(&pszString[ptr], "\r\n", 2) == 0 )	{			line++;		ptr++;		}

		ptr++;
		if( line == 3 )			break;
	}
	if( pszString[ptr] == '\0' )			return false;

	_stscanf( &pszString[ptr],	TEXT("%d %d %d %d %d %d %d %d"), 
								&TileXsize,		&TileYsize,
								&TileCenterX,	&TileCenterY,
								&IndentX,		&IndentY,
								&AniFrame,		&AniDelay);

	//_stscanf(buffer, TEXT("%d %d %d %d %d %d %d %d"), 
	//			&TileXsize,		&TileYsize,
	//			&TileCenterX,	&TileCenterY,
	//			&IndentX,		&IndentY,
	//			&AniFrame,		&AniDelay);

	return true;
}

// 배열 정보를 저장한다. 
bool cltFieldObjectInfo::WriteSetArrayInfo(TCHAR* szfilename)
{
	// 배열 정보를 읽어온다. 
	FILE* fp;
	TCHAR buffer[1024];

	fp = _tfopen(szfilename, TEXT("wt"));
	if(fp == NULL)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, szfilename, TEXT("File Write Error"), MB_OK|MB_TOPMOST);
		return false;
	}

	// 이름 저장. 
	StringCchPrintf(buffer, 1024, TEXT("%s\n"), pszName);
	_fputts(buffer, fp);

	
	// 빈공간 저장. 
	_fputts(TEXT("\n"), fp);


	// 분류테이블 저장. 
	StringCchPrintf(buffer, 1024, TEXT("%s %s %s %s %s %s %s %s\n"), 
			TEXT("TileXsize"),	TEXT("TileYsize"),
			TEXT("TileCenterX"),	TEXT("TileCenterY"),
			TEXT("IndentX"),		TEXT("IndentY"),
			TEXT("AniFrame"),		TEXT("AniDelay"));

	_fputts(buffer, fp);


	// 실제 데이터 저장. 
	StringCchPrintf(buffer, 1024, TEXT("%d %d %d %d %d %d %d %d\n"), 
			TileXsize,		TileYsize,
			TileCenterX,	TileCenterY,
			IndentX,		IndentY,
			AniFrame,		AniDelay);

	_fputts(buffer, fp);

	fclose(fp);

	return true;
}

void cltFieldObjectInfo::LoadDetailInfo()
{
	if(TileXsize != 0)	return;

	TCHAR szfilename[128];

	if(GetInfoFileName(szfilename) == true)
	{
		BOOL bCompress = FALSE;
		BYTE* pFileBuf = NULL;
		BOOL bCompressLoaded = FALSE;

		//===========================================================================================
		// 파일이 압축파일에 들어있는치 체크한다.
		//===========================================================================================
		TCHAR szZipName[MAX_PATH] = { '\0', };
		TCHAR szRealName[MAX_PATH] = { '\0', };
		if( pszZipName != NULL )	{		StringCchPrintf( szZipName, sizeof(szZipName), "FieldObject\\%s.mz", pszZipName);		}
		if( pszName != NULL )		{		StringCchPrintf( szRealName, sizeof(szRealName), "%s/%s.txt", pszName, pszName );		}

		unzFile uf = unzOpen( szZipName );
		bool bFind = false;
		if( uf )
		{
			unz_global_info gi;
			int ret = unzGetGlobalInfo(uf, &gi);

			for( UI32 j=0; j<gi.number_entry; j++ )
			{
				TCHAR filename[MAX_PATH];
				unz_file_info file_info;
				ret = unzGetCurrentFileInfo( uf, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0);

				_tcsupr(filename);
				_tcsupr(szRealName);

				if( _tcscmp( filename, szRealName ) == 0 )
				{
					bCompress = TRUE;
					pFileBuf = new BYTE[file_info.uncompressed_size + 10];		// 텍스트 파일이므로 여유공간을 조금 준다.
					ZeroMemory(pFileBuf, file_info.uncompressed_size + 10);
					if( pFileBuf != NULL )
					{
						unzOpenCurrentFile( uf );
						UI32 nReaded = unzReadCurrentFile( uf, pFileBuf, file_info.uncompressed_size);
						if( nReaded == file_info.uncompressed_size)		bCompressLoaded = true;
						unzCloseCurrentFile( uf );
					}
					break;
				}

				ret = unzGoToNextFile(uf);
			}
			unzClose( uf );
		}
		//===========================================================================================

		if( bCompress )
		{
			if( bCompressLoaded )
			{
				ReadSetArrayInfoFromMemory( pFileBuf );
			}
			else
			{
#ifdef _DEBUG
				MessageBox( NULL, szfilename, "ERROR", MB_OK );
#endif
			}
			if( pFileBuf != NULL )		delete [] pFileBuf;
		}
		else
		{
			ReadSetArrayInfo(szfilename);
		}
	}
}


//----------------------------------------------------------------
// Field Object Brush
//---------------------------------------------------------------
cltFieldObjectBrush::cltFieldObjectBrush(TCHAR* pname)
{
	SI32 i;

	StringCchCopy(szName, MAX_PATH, pname);

	FielObjectNumber = 0;

	for(i =0; i < MAX_FIELDOBJECT_PER_BRUSH;i++)
	{
		pFieldObjectInfo[i] = NULL;
	}


}

cltFieldObjectBrush::~cltFieldObjectBrush()
{
}


BOOL cltFieldObjectBrush::Add(cltFieldObjectInfo* pfieldobjectinfo)
{
	if(pfieldobjectinfo == NULL)return FALSE;
	if(FielObjectNumber >= MAX_FIELDOBJECT_PER_BRUSH)return FALSE;

	pFieldObjectInfo[FielObjectNumber] = pfieldobjectinfo;
	FielObjectNumber++;

	return TRUE;
}

// 가지고 있는 cltFieldObjectInfo중에서 한개를 랜덤하게 고른 후 이의 unique값을 리턴한다. 
SI32 cltFieldObjectBrush::GetRandUnique()
{
	if(FielObjectNumber == 0)return 0;

	SI32 data = rand() % FielObjectNumber;
	return pFieldObjectInfo[data]->Unique;
}


//-----------------------------------------------------------------
// Field Object Set
//-----------------------------------------------------------------

cltFieldObjectManager::cltFieldObjectManager(SI32 gamemode)
{
	SI32 i;

	siLoadedSize	= 0;

	for( i = 0; i < MAX_FIELDOBJECTSET_NUMBER;i++)
	{
		pclInfo[i] = NULL;
	}

	for( i = 0; i < MAX_FIELDOBJECTTYPE_NUMBER;i++)
	{
		pclTypeInfo[i] = NULL;
	}

	for( i = 0; i < MAX_FIELDOBJECTBRUSH_NUMBER;i++)
	{
		pclFieldObjectBrush[i] = NULL;
	}

	for( i = 0; i < MAX_FOUNIQUE;i++)
	{
		siFORef[i] = -1;
	}

	for( i = 0; i < MAX_FOSPR_NUMBER;i++)
	{
		FOInfoUnique[i]		= 0;
		dwLastUseFrame[i]	= 0;
	}

	InitFieldObjectManager();
	InitTypeSet();
	// }

	// Field Object Type에 대한 초기화를 행한다. 
	InitSet();
	
	if(gamemode == GAMEMODE_MAPEDIT)
	{
		InitBrush();
	}
	
	SI32 a = 0;
}

bool cltFieldObjectManager::InitFieldObjectManager()
{
	return true ;
}

cltFieldObjectManager::~cltFieldObjectManager()
{
	SI32 i;

	for(i = 0;i < MAX_FIELDOBJECTSET_NUMBER;i++)
	{
		if(pclInfo[i])
		{
			delete pclInfo[i];
			pclInfo[i] = NULL;
		}
	}

	for(i = 0;i < MAX_FIELDOBJECTTYPE_NUMBER;i++)
	{
		if(pclTypeInfo[i])
		{
			delete pclTypeInfo[i];
			pclTypeInfo[i] = NULL;
		}
	}

	for(i = 0;i < MAX_FIELDOBJECTBRUSH_NUMBER;i++)
	{
		if(pclFieldObjectBrush[i])
		{
			delete pclFieldObjectBrush[i];
			pclFieldObjectBrush[i] = NULL;
		}
	}

	for(i = 0;i < MAX_FOSPR_NUMBER;i++)
	{
		if(FOSpr[i].pImage)
		{
			GP.FreeSpr( FOSpr[i] );
		}
	}
}

// 브러쉬를 설정한다. 
BOOL cltFieldObjectManager::InitBrush()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	SI32 index = 0;
	TCHAR name[1024];
	TCHAR fieldobject[1024];
	TCHAR buffer[1024];
	TCHAR path[1024];

	FILE* fp;

	hSrch = FindFirstFile(TEXT("FieldObject\\Brush\\*.txt"), &wfd);

	if(GetLastError())
	{
		TCHAR buffer[256];
		TCHAR text[128];
		DWORD errcode = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buffer,  128, NULL);
		StringCchPrintf(text, 128, TEXT("ErrorCode=%d Message=%s"), errcode, buffer);
		
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, text, TEXT("InitBrush"), MB_OK|MB_TOPMOST);
	}

	while(bResult)
	{
		StringCchPrintf(path, 1024, TEXT("FieldObject\\Brush\\%s"), wfd.cFileName);
		// 찾은 파일을 읽어서 값을 초기화한다.
		fp =_tfopen(path, TEXT("rt"));
		if(fp == NULL)
		{
			TCHAR buffer[256];
			StringCchPrintf(buffer, 1024, TEXT("Cannot Find Brush File:%s"), wfd.cFileName);
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox(NULL, buffer, TEXT("Error"), MB_OK|MB_TOPMOST);
			return FALSE;
		}

		// 브러쉬의 이름을 얻어와서 브러시 객체를 만든다. 
		_fgetts(buffer, 1024, fp);
		_stscanf(buffer, TEXT("%s"), name);
		pclFieldObjectBrush[index] = new cltFieldObjectBrush(name);

		// 한줄은 건너 뛴다. 
		_fgetts(buffer, 1024, fp);

		while( _fgetts(buffer, 1024, fp) != NULL)
		{
			_stscanf(buffer, TEXT("%s"), fieldobject);
			pclFieldObjectBrush[index]->Add( GetInfoPointer(fieldobject) );
		}

		fclose(fp);

		// 다음 브러쉬를 가리키게 한다. 
		// 최대 브러쉬 갯수를 초과하지는 못하게 한다. 
		index++;
		if(index >= MAX_FIELDOBJECTBRUSH_NUMBER)
		{
			if (pclClient->GameMode != GAMEMODE_SERVER)
				MessageBox(NULL, TEXT("Exceed Max. Brush Number"), TEXT("Error"), MB_OK|MB_TOPMOST);
			break;
		}


		bResult = FindNextFile(hSrch, &wfd);
	}

	FindClose(hSrch);

	return TRUE;
}

// Field Object Type를 초기화한다. 
BOOL cltFieldObjectManager::InitTypeSet()
{
	NDataAtbParser  atbparser;

	// FOATB 등록
	atbparser.AddAtbInfo( TEXT("FOATB_VILLAGE"), FOATB_VILLAGE );
	atbparser.AddAtbInfo( TEXT("FOATB_TREE"), FOATB_TREE );
	atbparser.AddAtbInfo( TEXT("FOATB_MOUNTINE"), FOATB_MOUNTINE );
	atbparser.AddAtbInfo( TEXT("FOATB_GATE"), FOATB_GATE );
	atbparser.AddAtbInfo( TEXT("FOATB_STATUE"), FOATB_STATUE );
	atbparser.AddAtbInfo( TEXT("FOATB_BUILD"), FOATB_BUILD );
	atbparser.AddAtbInfo( TEXT("FOATB_FLOOR"), FOATB_FLOOR );
	atbparser.AddAtbInfo( TEXT("FOATB_BRIDGE"), FOATB_BRIDGE );

	cltFOT clFot[ MAX_FIELDOBJECTTYPE_NUMBER ];
	int i, index = 0;
	BOOL bLoadFile = FALSE;

	NDataLoader	dataloader;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/FieldObjectTypeInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/FieldObjectTypeInfo.txt") );
	}

	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("FieldObjectTypeInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}
	
	cltFOT aFOT;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&aFOT.type,		4,
			NDATA_MBSTRING,	aFOT.typeString,		64,
			NDATA_INT32,	&aFOT.color,		4,
			NDATA_MBSTRING,	aFOT.atb,		64,
			NDATA_MBSTRING,	aFOT.parentType,		64,
			0,0,0
	};

	dataloader.RegDataType(pDataTypeInfo );

	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())	
		{
			if ( aFOT.typeString[ 0 ] != NULL )
			{
				clFot[index] = aFOT;

				pclClient->pclHashTableString->Insert( clFot[index].typeString , clFot[index].type);
				index++;
			}
		}

	}

	for (i=0; i<index; i++)
	{
		int iType = -1, iAtb = -1, iParentType = -1;
		BOOL bFind = FALSE;

		bFind = pclClient->pclHashTableString->Find(clFot[i].typeString, iType);
		iAtb = atbparser.GetValueByString( clFot[i].atb );

		if (!bFind )
		{
			MsgBox(TEXT("Error"), TEXT("Not Found FieldObjectTypeInfo(%s) In HashTable"), clFot[i].typeString);
		}
				
		cltFieldObjectTypeInfo* aParentInfo;
		if( !pclClient->pclHashTableString->Find(clFot[i].parentType, iParentType) ) 
		{
			aParentInfo = NULL;
		} else {
			aParentInfo = pclTypeInfo[ iParentType ];
		}

		pclTypeInfo[clFot[i].type] = new cltFieldObjectTypeInfo(iType, clFot[i].color, clFot[i].typeString , iAtb, aParentInfo);

	}
	
	
	return TRUE;

}

BOOL cltFieldObjectManager::InitSet()
{
	BOOL bLoadFile = FALSE;
	NDataLoader	dataloader;

	SI32 unique = 0 ;
	SI32 imagenumber = 0 ;
	TCHAR type[128] = TEXT("") ;
	TCHAR name[128] = TEXT("") ;
	SI32 transmode = 0 ;
	SI32 alphavalue = 0 ;
	SI32 shadow = 0 ; // 나무 그림자들
	TCHAR zipname[128] = TEXT("") ;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&unique,			4,
			NDATA_MBSTRING,	type,			128,
			NDATA_INT32,	&imagenumber,	4,
			NDATA_MBSTRING,	name,			128,
			NDATA_INT32,	&transmode,		4,
			NDATA_INT32,	&alphavalue, 	4,
			NDATA_INT32,	&shadow,		4,
			NDATA_MBSTRING,	zipname,		128,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/FieldObjectSetInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/FieldObjectSetInfo.txt") );
	}
	if (!bLoadFile)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("FieldObjectSetInfo Load Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}

	SI32 index = 0;
	while( !dataloader.IsEndOfData() ) 
	{
		if (dataloader.ReadData())
		{
			if (type[ 0 ] != NULL)
			{
				BOOL bFind = FALSE;

				// type문자열을 분석하여 문자여로 되어 있는 텍스트를 게임내에서 의미있는 숫자로 변경하여 리턴한다. 
				SI32 sitype; 

				bFind = pclClient->pclHashTableString->Find(type, sitype);

				if (!bFind )
				{
					MsgBox(TEXT("Error"), TEXT("Not Found FieldObjectSetInfo(%s) In HashTable"), type);
				}


				pclInfo[index] = new cltFieldObjectInfo(unique, sitype, imagenumber, name, transmode,alphavalue,shadow,zipname);
				if(unique > 0 && unique < MAX_FOUNIQUE)
				{
					siFORef[unique] = index;		// 레퍼런스를 구할 수 있도록 보관한다.
				}
				else
				{
					MsgBox(TEXT("vfv93jfdf"), TEXT("vsf8jfd:%d"), unique);
				}


				index++;

				// Field Object 세트를 가질 수 있는 최대 수를 초과하면 에러 메시지를 보낸다. 
				if(index >= MAX_FIELDOBJECTSET_NUMBER)
				{
					if (pclClient->GameMode != GAMEMODE_SERVER)
						MessageBox(NULL, TEXT("Warning Out of FieldObject Number"), TEXT("Warning"), MB_OK|MB_TOPMOST);
				}
			}
		}

	}

	
	return TRUE;
}

// Spr이미지를 저장할 빈 공간을 찾는다. 
SI32 cltFieldObjectManager::FindEmptySpr()
{
	SI32 i;
	DWORD lastframe = 0xffffffff;
	SI32 lastref	= -1;

	// 가장 오래된 프레임을 찾는다. 
	for(i = 0;i < MAX_FOSPR_NUMBER;i++)
	{
		if(FOSpr[i].pImage == NULL)return i;

		if(dwLastUseFrame[i] <= lastframe)
		{
			lastframe	= dwLastUseFrame[i];
			lastref		= i;
		}
	}
	return lastref;
}

// 현재 설정에 맞게 타일 세트를 읽어온다. 
BOOL cltFieldObjectManager::LoadSet(DWORD frame, cltFieldObjectInfo* pclinfo, bool bmainmap )		
{

	if(pclinfo == NULL)
	{
		TCHAR buffer[256];
		StringCchPrintf(buffer, 256, TEXT("Cannot Find required FieldObjectSet"));

		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, buffer, TEXT("Error"), MB_OK|MB_TOPMOST);
		return FALSE;
	}

	BOOL bCompress = FALSE;
	BYTE* pFileBuf = NULL;
	BOOL bCompressLoaded = FALSE;

	//===========================================================================================
	// 파일이 압축파일에 들어있는치 체크한다.
	//===========================================================================================
	TCHAR szZipName[MAX_PATH] = { '\0', };
	TCHAR szRealName[MAX_PATH] = { '\0', };
	if( pclinfo->pszZipName != NULL)	{	StringCchPrintf( szZipName, sizeof(szZipName), "FieldObject\\%s.mz", pclinfo->pszZipName);					}
	if( pclinfo->pszName != NULL)		{	StringCchPrintf( szRealName, sizeof(szRealName), "%s/%s%d.mpr", pclinfo->pszName, pclinfo->pszName, 1 );	}

	unzFile uf = unzOpen( szZipName );
	bool bFind = false;
	if( uf )
	{
		unz_global_info gi;
		int ret = unzGetGlobalInfo(uf, &gi);

		for( UI32 j=0; j<gi.number_entry; j++ )
		{
			TCHAR filename[MAX_PATH];
			unz_file_info file_info;
			ret = unzGetCurrentFileInfo( uf, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0);

			_tcsupr(filename);
			_tcsupr(szRealName);

			if( _tcscmp( filename, szRealName ) == 0 )
			{
				bCompress = TRUE;
				pFileBuf = new BYTE[file_info.uncompressed_size];
				if( pFileBuf != NULL )
				{
					unzOpenCurrentFile( uf );
					UI32 nReaded = unzReadCurrentFile( uf, pFileBuf, file_info.uncompressed_size);
					if( nReaded == file_info.uncompressed_size)		bCompressLoaded = true;
					unzCloseCurrentFile( uf );
				}
				break;
			}

			ret = unzGoToNextFile(uf);
		}
		unzClose( uf );
	}
	//===========================================================================================

	// 이미지를 불러온다.
	TCHAR szfilename[128];
	StringCchPrintf(szfilename, 128, TEXT("FieldObject\\%s\\%s%d.Mpr"), pclinfo->pszName, pclinfo->pszName, 1);
	
	// 적절한 이미지 저장 공간을 찾는다. 
	SI32 imgref = FindEmptySpr();
	if(imgref >= 0)
	{
		// 기존의 정보를 초기화한다. 
		SI32 oldunique	= FOInfoUnique[imgref];
		SI32 oldref		= FindRefFromUniqueMinus(oldunique);
		if(oldref >= 0 )
		{
			pclInfo[ oldref ]->siFOSprRef = -1;
			siLoadedSize -= FOSpr[ imgref ].GetImageDataSize();
		}

		pclinfo->siFOSprRef		= imgref;
		pclinfo->bIsMainMap		= bmainmap;
		dwLastUseFrame[imgref]	= frame;
		FOInfoUnique[imgref]	= pclinfo->Unique;	

		if( bCompress )
		{
			if( bCompressLoaded )
			{
				GP.LoadSprFromMemory(pFileBuf, &FOSpr[ imgref ]);
			}
			else
			{
#ifdef _DEBUG
				MessageBox( NULL, szfilename, "ERROR", MB_OK );
#endif
			}
			if( pFileBuf != NULL )		delete [] pFileBuf;
		}
		else
		{
			GP.LoadSprFromFile(szfilename, &FOSpr[ imgref ]);
		}

		// 로딩된 사이즈를 누적한다. 
		siLoadedSize += FOSpr[ imgref ].GetImageDataSize();

	}


	return TRUE;
}

// 유니크 값을 근거로 레퍼런스를 찾는다. 
SI32 cltFieldObjectManager::FindRefFromUniqueMinus(SI32 siunique)
{

	if(siunique <= 0 || siunique >= MAX_FOUNIQUE)return -1;

	return siFORef[siunique];

}


// FieldObject의 Atb를 찾는다.
SI32 cltFieldObjectManager::GetAtb(SI32 founique)
{
	SI32 ref = FindRefFromUniqueMinus(founique);
	if(ref < 0)return 0;

	SI32 type = pclInfo[ref]->dwType;

	return pclTypeInfo[type]->dwAtb;
}


// 사용되지 않은 Field Object를 메모리에서 삭제한다. 
void cltFieldObjectManager::DeleteIfNotUse(DWORD curframe)
{
	SI32 i;

	for(i = 0;i < MAX_FOSPR_NUMBER;i++)
	{
		if(FOSpr[i].pImage == NULL)continue;

		if( pclClient->IsCountrySwitch(Switch_FieldObjectPreLoad) == false )
		{
			if( TABS(curframe - dwLastUseFrame[i]) <= 500 ) continue;
		}

		SI32 ref = FindRefFromUniqueMinus( FOInfoUnique[i] );

		// 메인맵은 삭제하지 않는다.
		if ( pclInfo[ref]->bIsMainMap )	continue;

		// 이미지 레퍼런스를 삭제한다. 
		pclInfo[ref]->siFOSprRef	= -1;


		// 메모리의 사이즈를 줄여준다. 
		siLoadedSize -= FOSpr[i].GetImageDataSize();
		GP.FreeSpr(FOSpr[i]);
	}
}

// 이름을 근거로 Unique를 찾는다. 
SI32 cltFieldObjectManager::FindUniqueFromName(TCHAR* name)
{
	SI32 i;

	for(i = 0; i < MAX_FIELDOBJECTSET_NUMBER;i++)
	{
		if(pclInfo[i])
		{
			if( _tcscmp(pclInfo[i]->pszName, name)==0 )
			{
				return pclInfo[i]->Unique;
			}
		}
	}

	return 0;
}
