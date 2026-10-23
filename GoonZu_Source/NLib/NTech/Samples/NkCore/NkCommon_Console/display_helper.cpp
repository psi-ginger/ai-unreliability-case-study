//-------------------------------------------------------------------------------------------------
// display_helper.cpp
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

#include "display_helper.h"


class ScreenBlock
{
public:
	static const int COLUMNS = 80;

	ScreenBlock(int iRows)
	{
		m_iRows = iRows;
		m_iBufferSize = iRows * COLUMNS;
		m_pcBuffer = new char[ m_iBufferSize ];
		Clear();
	}

	~ScreenBlock()
	{
		delete [] m_pcBuffer;
	}

	void Clear()
	{
		memset(m_pcBuffer, ' ', m_iBufferSize);
	}
	
	void Put(int x, int y, char* pcStr)
	{
		if( x < 0 ) x = 0;

		int index = y * COLUMNS + x;
		int len = (int)strlen(pcStr);

		if( index + len > m_iBufferSize )
			return;

		memcpy(&m_pcBuffer[ index ], pcStr, len);
	}

	void Display()
	{
		for( int i = 0; i < m_iRows; ++i )
		{
			m_pcBuffer[ (i + 1) * COLUMNS - 1 ] = NULL;
			CDispHelper::Get().Printf( "%s\n", &m_pcBuffer[ i * COLUMNS ] );
		}
	}

private:
	char* m_pcBuffer;
	int m_iBufferSize;
	int m_iRows;

};

CDispHelper* CDispHelper::ms_pkDispHelper = 0;

CDispHelper::CDispHelper()
{
}

void CDispHelper::Init()
{
	ms_pkDispHelper = new CDispHelper();
}

void CDispHelper::Shutdown()
{
	delete ms_pkDispHelper;
}

void CDispHelper::PrintSource(const char* pcStr)
{
	Get().PutStr("Source Code: ");
	Get().PutStr(1, 15, pcStr);
	Get().PutStr("\n\n");
}

void disp_text_up(ScreenBlock& kSB, int iPos, char* pcStr)
{
	int x1 = (iPos * 6) + 2;
	int x2 = x1 - (int)(strlen(pcStr) / 2) + 1;

	kSB.Put( x1, 1, "↓");
	kSB.Put( x2, 0, pcStr);
}

void disp_text_down(ScreenBlock& kSB, int iPos, char* pcStr)
{
	int x1 = (iPos * 6) + 2;
	int x2 = x1 - (int)(strlen(pcStr) / 2) + 1;

	kSB.Put( x1, 0, "↑");
	kSB.Put( x2, 1, pcStr);
}

//-------------------------------------------------------------------------------------------------
// disp_vector
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_vector(NkVector<int>* pkVector)
{
	unsigned int uiAllocedCount = pkVector->GetAllocedCount();
	unsigned int uiCount = pkVector->GetCount();

	Get().Printf( "GetAllocedCount()=%d, GetAllocedBytes()=%d, GetCount()=%d\n",
		pkVector->GetAllocedCount(), 
		pkVector->GetAllocedBytes(), 
		pkVector->GetCount());

	size_t st;
	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_TOP);
	Get().PutStr("\n");

	int iValue;
	NkCharsA< 16 > kValue;
	for( st = 0; st < uiAllocedCount; ++st ) 
	{
		int *piBasePtr = pkVector->getbaseptr();

		iValue = piBasePtr[st];
		if( st < uiCount )
			kValue.Format( "%2d", iValue );
		else
			kValue = "Ｘ";

		if( uiAllocedCount == 1 ) Get().Printf(DISP_NODE_MIDONE, kValue.GetString());
		else if( st == 0 ) Get().Printf(DISP_NODE_MIDLEFT, kValue.GetString());
		else if( st == uiAllocedCount - 1 ) Get().Printf(DISP_NODE_MIDRIGHT, kValue.GetString());
		else Get().Printf(DISP_NODE_MID, kValue.GetString());
	}
	Get().PutStr("\n");

	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_BOTTOM);
	Get().PutStr("\n");

	Get().PutStr("-------------------------------------------------------------------------------\n");
}

//-------------------------------------------------------------------------------------------------
// disp_list
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_list(NkList<int>* pkList)
{

	size_t stCount = pkList->GetCount();
	NkListPos kPos = pkList->GetFrontPos();

	if( stCount == 0 )
		return;

	ScreenBlock kSB(2);

	if( stCount == 1 ) 
		disp_text_up(kSB, 0, "Front,Back");
	else 
	{
		disp_text_up(kSB, 0, "Front");
		disp_text_up(kSB, (int)stCount - 1,"Back");
	}

	kSB.Display();

	size_t st;
	for( st = 0; st < stCount; ++st ) Get().Printf(DISP_NODE_TOP);
	Get().Printf("\n");


	int iValue;
	NkCharsA< 16 > kValue;
	for( st = 0; st < stCount; ++st ) 
	{
		iValue = pkList->GetData(kPos);
		kValue.Format( "%2d", iValue );

		if( stCount == 1 ) Get().Printf(DISP_NODE_MIDONE, kValue.GetString());
		else if( st == 0 ) Get().Printf(DISP_NODE_MIDLEFT, kValue.GetString());
		else if( st == stCount - 1 ) Get().Printf(DISP_NODE_MIDRIGHT, kValue.GetString());
		else Get().Printf(DISP_NODE_MID, kValue.GetString());

		pkList->MoveNext(kPos);
	}
	Get().PutStr("\n");

	for( st = 0; st < stCount; ++st ) Get().Printf(DISP_NODE_BOTTOM);
	Get().PutStr("\n");

	Get().PutStr("-------------------------------------------------------------------------------\n");
}

//-------------------------------------------------------------------------------------------------
// disp_stack
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_stack(NkStack<int>* pkStack)
{
	unsigned int uiAllocedCount = pkStack->GetAllocedCount();
	unsigned int uiCount = pkStack->GetCount();

	Get().Printf( "GetAllocedCount()=%d, GetAllocedBytes()=%d, GetCount()=%d\n",
		pkStack->GetAllocedCount(), 
		pkStack->GetAllocedBytes(), 
		pkStack->GetCount());

	size_t st;
	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_TOP);
	Get().Printf("\n");

	int iValue;
	NkCharsA< 16 > kValue;
	for( st = 0; st < uiAllocedCount; ++st ) 
	{
		int *piBasePtr = pkStack->getbaseptr();

		iValue = piBasePtr[st];
		if( st < uiCount )
			kValue.Format( "%2d", iValue );
		else
			kValue = "Ｘ";

		if( uiAllocedCount == 1 ) Get().Printf(DISP_NODE_MIDONE, kValue.GetString());
		else if( st == 0 ) Get().Printf(DISP_NODE_MIDLEFT, kValue.GetString());
		else if( st == uiAllocedCount - 1 ) Get().Printf(DISP_NODE_MIDRIGHT, kValue.GetString());
		else Get().Printf(DISP_NODE_MID, kValue.GetString());
	}
	Get().PutStr("\n");

	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_BOTTOM);
	Get().PutStr("\n");

	Get().PutStr("-------------------------------------------------------------------------------\n");
}

//-------------------------------------------------------------------------------------------------
// disp_queue
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_queue(NkQueue<int>* pkQueue)
{
	unsigned int uiAllocedCount = pkQueue->GetAllocedCount();
	unsigned int uiCount = pkQueue->GetCount();

	Get().Printf( "GetAllocedCount()=%d, GetAllocedByteSize()=%d, GetCount()=%d\n",
		pkQueue->GetAllocedCount(), 
		pkQueue->GetAllocedBytes(), 
		pkQueue->GetCount());

	ScreenBlock kSB(2);

	disp_text_up(kSB, pkQueue->getfrontpos(), "Front");
	kSB.Display();

	size_t st;
	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_TOP);
	Get().PutStr("\n");

	int iValue;
	NkCharsA< 16 > kValue;
	for( st = 0; st < uiAllocedCount; ++st ) 
	{
		int *piBasePtr = pkQueue->getbaseptr();

		iValue = piBasePtr[st];

		kValue = "Ｘ";
		if( uiCount > 0 )
		{
			if( pkQueue->getfrontpos() <= pkQueue->getbackpos() )
			{
				if( pkQueue->getfrontpos() <= st && st <= pkQueue->getbackpos() )
					kValue.Format( "%2d", iValue );
			}
			else
			{
				if( st <= pkQueue->getbackpos() || pkQueue->getfrontpos() <= st )
					kValue.Format( "%2d", iValue );
			}
		}

		if( uiAllocedCount == 1 ) Get().Printf(DISP_NODE_MIDONE, kValue.GetString());
		else if( st == 0 ) Get().Printf(DISP_NODE_MIDLEFT, kValue.GetString());
		else if( st == uiAllocedCount - 1 ) Get().Printf(DISP_NODE_MIDRIGHT, kValue.GetString());
		else Get().Printf(DISP_NODE_MID, kValue.GetString());
	}
	Get().PutStr("\n");

	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_BOTTOM);
	Get().PutStr("\n");

	kSB.Clear();

	disp_text_down(kSB, pkQueue->getbackpos(), "Back");
	kSB.Display();

	Get().PutStr("-------------------------------------------------------------------------------\n");
}


//-------------------------------------------------------------------------------------------------
// disp_deque
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_deque(NkDeque<int>* pkDeque)
{
	unsigned int uiAllocedCount = pkDeque->GetAllocedCount();
	unsigned int uiCount = pkDeque->GetCount();

	Get().Printf( "GetAllocedCount()=%d, GetAllocedByteSize()=%d, GetCount()=%d\n",
		pkDeque->GetAllocedCount(), 
		pkDeque->GetAllocedBytes(), 
		pkDeque->GetCount());

	ScreenBlock kSB(2);

	disp_text_up(kSB, pkDeque->getfrontpos(), "Front");
	kSB.Display();

	size_t st;
	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_TOP);
	Get().Printf("\n");

	int iValue;
	NkCharsA< 16 > kValue;
	for( st = 0; st < uiAllocedCount; ++st ) 
	{
		int *piBasePtr = pkDeque->getbaseptr();

		iValue = piBasePtr[st];

		kValue = "Ｘ";
		if( uiCount > 0 )
		{
			if( pkDeque->getfrontpos() <= pkDeque->getbackpos() )
			{
				if( pkDeque->getfrontpos() <= st && st <= pkDeque->getbackpos() )
					kValue.Format( "%2d", iValue );
			}
			else
			{
				if( st <= pkDeque->getbackpos() || pkDeque->getfrontpos() <= st )
					kValue.Format( "%2d", iValue );
			}
		}

		if( uiAllocedCount == 1 ) Get().Printf(DISP_NODE_MIDONE, kValue.GetString());
		else if( st == 0 ) Get().Printf(DISP_NODE_MIDLEFT, kValue.GetString());
		else if( st == uiAllocedCount - 1 ) Get().Printf(DISP_NODE_MIDRIGHT, kValue.GetString());
		else Get().Printf(DISP_NODE_MID, kValue.GetString());
	}
	Get().PutStr("\n");

	for( st = 0; st < uiAllocedCount; ++st ) Get().Printf(DISP_NODE_BOTTOM);
	Get().PutStr("\n");

	kSB.Clear();

	disp_text_down(kSB, pkDeque->getbackpos(), "Back");
	kSB.Display();

	Get().PutStr("-------------------------------------------------------------------------------\n");
}

//-------------------------------------------------------------------------------------------------
// disp_string
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_string(char* pcStr, size_t stBufferSize)
{
	if( stBufferSize == 0 )
		stBufferSize = strlen(pcStr) + 1;

	size_t st;

	for( st = 0; st < stBufferSize; ++st ) Get().Printf("  %2d", st);
	Get().PutStr("\n");

	for( st = 0; st < stBufferSize; ++st ) if( st == 0 ) Get().PutStr("┌─"); else Get().PutStr("┬─");
	Get().PutStr("┐\n");

	NkCharsA< 16 > kValue;

	bool bEndString = false;
	for( st = 0; st < stBufferSize; ++st ) 
	{
		if( bEndString ) kValue = "Ｘ";
		else
		{
			if( pcStr[st] == '\0' )
			{
				bEndString = true;
				kValue = "\\0";
			}
			else
				kValue = pcStr[st];
		}

		Get().Printf("│%2s", kValue.GetString());
	}
	Get().PutStr("│\n");

	for( st = 0; st < stBufferSize; ++st ) if( st == 0 ) Get().PutStr("└─"); else Get().PutStr("┴─");
	Get().PutStr("┘\n");

	Get().PutStr("-------------------------------------------------------------------------------\n");
}

//-------------------------------------------------------------------------------------------------
// disp_rbtreemap
//-------------------------------------------------------------------------------------------------
void CDispHelper::disp_rbtreemap(NkRbTreeMap<int,int>* pkRbTreeMap)
{
	unsigned int uiDepth = pkRbTreeMap->GetMaxDepth();

	//Get().Printf("Depth = %d\n", uiDepth);

	NkRbTreeMapPosVector* pkPosVector = pkRbTreeMap->BuildMapPosVector();


	unsigned int i, j, k, l;
	unsigned int iNodeCount = 1;

	// 리프 노드의 총 출력 가로 길이
	unsigned int uiMaxWidth = (unsigned int)pow((double)2,(double)uiDepth-1);

	for( i = 1; i <= uiDepth; ++i )
	{
		unsigned int uiCurrentWidth = (unsigned int)pow((double)2,(double)i-1);
		unsigned int uiSpaceUnit = 
			(uiMaxWidth - uiCurrentWidth) * 4 / (uiCurrentWidth * 2);

		unsigned int uiBaseIndex = uiCurrentWidth - 1;

		if( i > 1 )
		{
			for( j = 0; j < iNodeCount; j+=2 ) 
			{
				for( k = 0; k < uiSpaceUnit; ++k ) Get().PutStr(" ");

				NkRbTreeMapPos kPos[2];
				kPos[0] = (*pkPosVector)[uiBaseIndex + j];
				kPos[1] = (*pkPosVector)[uiBaseIndex + j + 1];

				if( kPos[0] == 0 && kPos[1] == 0 )
				{
					for( k = 0; k < uiSpaceUnit * 2 + 8; ++k ) Get().PutStr(" ");
				}
				else
				{				
					Get().PutStr(" ┌");

					for( k = 0; k < uiSpaceUnit / 2; ++k ) Get().PutStr("─");
					Get().PutStr("┴");
					
					for( k = 0; k < uiSpaceUnit / 2; ++k ) Get().PutStr("─");
					Get().PutStr("┐ ");
				}

				for( k = 0; k < uiSpaceUnit; ++k ) Get().PutStr(" ");
			}
			Get().PutStr("\n");
		}

		uiBaseIndex = uiCurrentWidth - 1;

		if( i > 1 )
		{
			for( j = 0; j < iNodeCount; j+=2 ) 
			{
				//for( k = 0; k < uiSpaceUnit; ++k ) Get().Printf( " ");

				NkRbTreeMapPos kPos[2];
				kPos[0] = (*pkPosVector)[uiBaseIndex + j];
				kPos[1] = (*pkPosVector)[uiBaseIndex + j + 1];

				if( kPos[0] == 0 && kPos[1] == 0 )
				{
					for( k = 0; k < uiSpaceUnit * 4 + 8; ++k ) Get().PutStr(" ");
				}
				else
				{
					for( l = 0; l < 2; ++l )
					{
						for( k = 0; k < uiSpaceUnit; ++k ) Get().PutStr( " ");

						if( kPos[l] )
						{
							if( pkRbTreeMap->IsRedNode(kPos[l]) )
								Get().Printf(4, 15, "%3d", pkRbTreeMap->GetKey(kPos[l]));
							else
								Get().Printf(8, 15, "%3d", pkRbTreeMap->GetKey(kPos[l]));

							Get().PutStr(" ");
						}
						else
							Get().PutStr("    ");

						for( k = 0; k < uiSpaceUnit; ++k ) Get().PutStr(" ");
					}
				}
			}
		}
		else
		{
			NkRbTreeMapPos kPos = (*pkPosVector)[0];
			
			if( kPos )
			{
				for( k = 0; k < uiSpaceUnit; ++k ) Get().Printf( " ");
				Get().Printf(8, 15, "%3d", pkRbTreeMap->GetKey(kPos));
			}
		}
		Get().PutStr("\n");


		iNodeCount *= 2;
	}

	Get().PutStr("-------------------------------------------------------------------------------\n");
}
