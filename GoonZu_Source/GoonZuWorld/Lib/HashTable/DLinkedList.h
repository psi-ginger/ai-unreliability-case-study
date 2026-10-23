// ============================================================================
// DLinkedList.h
//
// This is the Doubly-Linked List class
// ============================================================================
#ifndef _DLINKEDLIST_H_
#define _DLINKEDLIST_H_




// forward declarations 
template<class Datatype> class DListNode;
template<class Datatype> class DLinkedList;
template<class Datatype> class DListIterator;



// -------------------------------------------------------
// Name:        DListNode
// Description: This is the Doubly-linked list node class.
// -------------------------------------------------------
template<class Datatype>
class DListNode
{
public:

// ----------------------------------------------------------------
//  Name:           m_Data
//  Description:    이 Node가 갖고 있는 Data
// ----------------------------------------------------------------
    Datatype m_Data;

// ----------------------------------------------------------------
//  Name:           m_Next
//  Description:    List에서 이 Node가 가리키는 Next Node
// ----------------------------------------------------------------
    DListNode<Datatype>* m_Next;

// ----------------------------------------------------------------
//  Name:           m_Previous
//  Description:    List에서 이 Node가 가리키는 Previous Node
// ----------------------------------------------------------------
    DListNode<Datatype>* m_Previous;

public:

// ----------------------------------------------------------------
//  Name:           DeLink
//  Description:    이 노드를 List에서 Link를 끊어 앞뒤를 연결한다.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Delink()
    {
		// Previous Node가 있다면, Previous Node는 Next Node를 가리키도록 한다.
        if( m_Previous != 0 )
            m_Previous->m_Next = m_Next;

		// Next Node가 있다면, Next Node는 Previous Node를 가리키도록 한다.
        if( m_Next != 0 )
            m_Next->m_Previous = m_Previous;
    }


// ----------------------------------------------------------------
//  Name:           InsertAfter
//  Description:    현재의 Node 바로뒤에 새로운 Node를 추가한다.
//  Arguments:      Data	 - 새로운 Node에 저장되어질 Data
//				    pNewNode - 새로 설정할 Node Pointer
//  Return Value:   None.
// ----------------------------------------------------------------
    bool InsertAfter( Datatype Data, DListNode<Datatype> *pNewNode )
    {
		if(!pNewNode) return false;

		// New Node에 Data를 설정한다.
        pNewNode->m_Data	= Data;

        // New Node의 새로운 Pointer를 설정한다.
        pNewNode->m_Next    = m_Next;
        pNewNode->m_Previous= this;

		// 이 Node 다음에 Node 가 존재한다면 New Node를 가리키도록한다.
        if( m_Next != 0 )
            m_Next->m_Previous = pNewNode;

        // 이 Node의 다음을 New Node로 가리키도록 한다.
        m_Next = pNewNode;
		return true;
    }


// ----------------------------------------------------------------
//  Name:           InsertBefore
//  Description:    현재의 Node앞에 새로운 Node를 추가한다.
//  Arguments:      Data	 - 새로운 Node에 저장되어질 Data
//				    pNewNode - 새로 설정할 Node Pointer
//  Return Value:   None.
// ----------------------------------------------------------------
    bool InsertBefore( Datatype Data, DListNode<Datatype>* pNewNode )
    {
        if(!pNewNode) return false;

		// New Node에 Data를 설정한다.
        pNewNode->m_Data	= Data;

        // New Node의 새로운 Pointer를 설정한다.
        pNewNode->m_Next     = this;
        pNewNode->m_Previous = m_Previous;

		// 이 Node 앞에 Node 가 존재한다면 New Node를 가리키도록한다.
        if( m_Previous != 0 )
            m_Previous->m_Next = pNewNode;

		// 이 Node의 Previous를 New Node로 가리키도록 한다.
        m_Previous = pNewNode;

		return true;
    }


};


// -------------------------------------------------------
// Name:        DLinkedList
// Description: This is the Doubly-linked list container.
// -------------------------------------------------------
template<class Datatype>
class DLinkedList
{
public:
// ----------------------------------------------------------------
//  Name:           m_Head
//  Description:    The first node in the list
// ----------------------------------------------------------------
    DListNode<Datatype>* m_Head;

// ----------------------------------------------------------------
//  Name:           m_Tail
//  Description:    The last node in the list
// ----------------------------------------------------------------
    DListNode<Datatype>* m_Tail;

// ----------------------------------------------------------------
//  Name:           m_siCount
//  Description:    The number of nodes in the list
// ----------------------------------------------------------------
    int m_siCount;

public:

// ----------------------------------------------------------------
//  Name:           DLinkedList
//  Description:    Constructor; creates an empty list
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    DLinkedList()
    {
        m_Head		= 0;
        m_Tail		= 0;
        m_siCount	= 0;
    }

    
// ----------------------------------------------------------------
//  Name:           DLinkedList
//  Description:    Destructor; destroys every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    ~DLinkedList()
    {
        m_Head		= 0;
        m_Tail		= 0;
        m_siCount	= 0;
    }


// ----------------------------------------------------------------
//  Name:           Append
//  Description:    List의 맨뒤에 새로운 Node를 추가한다.
//  Arguments:      Data	 - 추가될 Data
//					pNewNode - 새로 설정할 Node Pointer
//  Return Value:   None.
// ----------------------------------------------------------------
    bool Append( Datatype Data, DListNode<Datatype>* pNewNode)
    {
		if(!pNewNode) return false;

        // List가 비어있는 상태이므로 Head Node가 NULL 일때 
        if( m_Head == 0 )
        {
            // 새로운 Head Node를 설정한다.
            m_Head	= m_Tail	= pNewNode;
            m_Head->m_Data		= Data;
            m_Head->m_Next		= 0;
            m_Head->m_Previous	= 0;
        }
        else
        {
			// Tail 뒤에 새로운 Node를 추가하고 Tail을 재설정한다.
            m_Tail->InsertAfter( Data, pNewNode );
            m_Tail = m_Tail->m_Next;
        }
		// Count를 하나 증가시킨다.
        m_siCount++;
		return true;
    }


// ----------------------------------------------------------------
//  Name:           Prepend
//  Description:    List의 맨앞에 새로운 Node를 추가한다.
//  Arguments:      Data	 - 추가될 Data
//					pNewNode - 새로 설정할 Node Pointer
//  Return Value:   None.
// ----------------------------------------------------------------
    bool Prepend( Datatype Data, DListNode<Datatype> *pNewNode )
    {
		if(!pNewNode) return false;

        // List가 비어있는 상태이므로 Head Node가 NULL 일때 
        if( m_Head == 0 )
        {
            // 새로운 Head Node를 설정한다.
            m_Head	= m_Tail	= pNewNode;
            m_Head->m_Data		= Data;
            m_Head->m_Next		= 0;
            m_Head->m_Previous	= 0;
        }
        else
        {
			// Head앞에 새로운 Node를 추가하고 Head를 재설정한다.
            m_Head->InsertBefore( Data, pNewNode );
            m_Head = m_Head->m_Previous;
        }
		// Count를 하나 증가시킨다.
        m_siCount++;
		return true;
    }


// ----------------------------------------------------------------
//  Name:           RemoveHead
//  Description:    List의 Head Node를 제거한다.
//  Arguments:      pRemovedNode - 제거된 Node
//  Return Value:   None.
// ----------------------------------------------------------------
    DListNode<Datatype> *RemoveHead()
    {
		DListNode<Datatype>* pRemovedNode = 0;
        DListNode<Datatype>* node = 0;

        if( m_Head != 0 )
        {
			// Head의 Next Node를 임시로 저장한다.
            node = m_Head->m_Next;

			// Head를 제거하고 node를 Head로 설정한다.
			pRemovedNode = m_Head;	//  delete m_Head;
            m_Head		 = node;

			// 새로운 Head가 NULL 이라면, Tail 도 NULL로 설정
			// 그렇치 않으면, 새로운 Head의 Previous Node를 NULL로 설정한다.
            if( m_Head == 0 )
                m_Tail = 0;
            else
                m_Head->m_Previous = 0;

			// Count를 하나 줄인다.
            m_siCount--;

			return pRemovedNode;
        }
		return NULL;
    }


// ----------------------------------------------------------------
//  Name:           RemoveTail
//  Description:    List의 Tail Node를 제거한다.
//  Arguments:      pRemovedNode - 제거된 Node.
//  Return Value:   None.
// ----------------------------------------------------------------
    DListNode<Datatype>* RemoveTail()
    {
		DListNode<Datatype>* pRemovedNode = 0;
        DListNode<Datatype>* node = 0;

        if( m_Tail != 0 )
        {
            // Tail의 Previous Node를 임시로 저장한다.
            node = m_Tail->m_Previous;

			// Tail을 제거하고 node를 Tail Node로 설정한다.
            pRemovedNode = m_Tail; //delete m_Tail;
            m_Tail		 = node;

			// 새로운 Tail이 NULL 이라면, Head Node로 NULL로 설정한다.
			// 그렇치 않으면, 새로운 Tail의 Next Node를 NULL로 설정한다.
            if( m_Tail == 0 )
                m_Head = 0;
            else
                m_Tail->m_Next = 0;

			// Count를 하나 줄인다.
            m_siCount--;

			return pRemovedNode;
        }
		return NULL;
    }



// ----------------------------------------------------------------
//  Name:           InsertAfter
//  Description:    iterator 바로 뒤에 Data를 추가하거나 
//	                iterator가 invalid 하면 List의 맨뒤에 추가한다.
//  Arguments:      Iterator - 뒤에 Insert할 Iterator
//					Data	 - 새로운 Node에 저장되어질 Data
//					pNewNode - 새로 설정할 Node Pointer
//  Return Value:   None.
// ----------------------------------------------------------------
    bool InsertAfter( DListIterator<Datatype>& Iterator, Datatype Data, DListNode<Datatype> *pNewNode)
    {
        if( Iterator.m_Node != 0 )
        {
            // Iterator 뒤에 Data를 추가한다.
            Iterator.m_Node->InsertAfter( Data, pNewNode);

			// Iterator가 Tail이라면 Tail을 새로 설정한다.
            if( Iterator.m_Node == m_Tail )
                m_Tail = m_Tail->m_Next;

            // Count를 하나 증가시킨다.
            m_siCount++;
			return true;
        }
        else
        {
            return Append( Data );
        }

		return false;
    }


// ----------------------------------------------------------------
//  Name:           InsertBefore
//  Description:    iterator 바로 앞에 Data를 추가하거나 
//	                iterator가 invalid 하면 List의 맨앞에 추가한다.
//  Arguments:      Iterator - 앞에 Insert할 Iterator
//					Data	 - 새로운 Node에 저장되어질 Data
//					pNewNode - 새로 설정할 Node Pointer
//  Return Value:   None.
// ----------------------------------------------------------------
    bool InsertBefore( DListIterator<Datatype>& Iterator, Datatype Data, DListNode<Datatype> *pNewNode )
    {
        if( Iterator.m_Node != 0 )
        {
            // Iterator 앞에 Data를 추가한다.
            Iterator.m_Node->InsertBefore( Data, pNewNode );

			// Iterator가 Head라면 Head를 새로 설정한다.
            if( Iterator.m_Node == m_Head )
                m_Head = m_Head->m_Previous;

            // Count를 하나 증가시킨다.
            m_siCount++;
			return true;
        }
        else
        {
            return Prepend( Data );
        }
		return false;
    }


// ----------------------------------------------------------------
//  Name:           Remove
//  Description:    Iterator가 가리키는 Node를 제거하고. 
//					Next Node로 Interator Pointer를 설정한다.	
//  Arguments:      Iterator     - 제거할 Iterator
//					pRemovedNode - 제거된 Node
//  Return Value:   None.
// ----------------------------------------------------------------
    DListNode<Datatype>* Remove( DListIterator<Datatype>& Iterator)
    {
        // temporary node pointer.
		DListNode<Datatype>* pRemovedNode =0;
        DListNode<Datatype>* node =0;

        // Iterator가 invalid라면 돌아간다.
        if( Iterator.m_Node == 0 )	return NULL;

        // 제거할 Node를 임시변수에 저장한다.
        node = Iterator.m_Node;

		// 제거할 Node가 Head 거나 Tail 이라면 
		// Head나 Tail의 다음이나 앞의 Node로 제설정한다.
        if( node == m_Head )
        {
            m_Head = m_Head->m_Next;
        }
        else if( node == m_Tail )
        {
            m_Tail = m_Tail->m_Previous;
        }

		// Iterator를 다음의 Valid Node로 옮긴다.
        Iterator.Forth();

        // 제거할 Node를 Delink시킨다.
        node->Delink();

		pRemovedNode = node;// delete node;

		// Head가 NULL이라면 Tail도 NULL로 만든다.
        if( m_Head == 0 )
            m_Tail = 0;
        
        m_siCount--;

		return pRemovedNode;
    }



// ----------------------------------------------------------------
//  Name:           GetIterator
//  Description:    Iterator를 List의 맨앞을 가리키도록 하고 얻어온다.
//  Arguments:      None.
//  Return Value:   List의 맨앞을 가리키는 Iterator
// ----------------------------------------------------------------
    DListIterator<Datatype> GetIterator()
    {
        return DListIterator<Datatype>( this, m_Head );
    }


// ----------------------------------------------------------------
//  Name:           Size
//  Description:    List에 저정된 Count를 얻어온다.
//  Arguments:      None.
//  Return Value:   List의 크기
// ----------------------------------------------------------------
    int Size()
    {
        return m_siCount;
    }


// ----------------------------------------------------------------
//  Name:           SaveToDisk
//  Description:    Linked List를 Disk에 저장한다.
//  Arguments:      pFileName	- 저장할 파일이름 
//  Return Value:   true, if successful
// ----------------------------------------------------------------
    bool SaveToDisk( char* pFileName )
    {
        FILE* fpOutFile = 0;
        DListNode<Datatype>* itr = m_Head;

        // 파일은 연다.
        fpOutFile = _tfopen( pFileName, TEXT("wb") );

        // 파일을 여는데 실패했다면 돌아간다.
        if( fpOutFile == 0 )
            return false;

        // 먼저 List의 Size를 저장한다.
        fwrite( &m_siCount, sizeof( int ), 1, fpOutFile );

        // Loop를 돌면서 List를 저장한다.
        while( itr != 0 )
        {
            fwrite( &(itr->m_Data), sizeof( Datatype ), 1, fpOutFile );
            itr = itr->m_Next;
        }
		// 파일을 닫는다.
        fclose( fpOutFile );

        return true;
    }


// ----------------------------------------------------------------
//  Name:           ReadFromDisk
//  Description:    Reads a linked list from a file.
//  Arguments:      p_filename: the name of the file to read from
//  Return Value:   true if successful.
// ----------------------------------------------------------------
    bool ReadFromDisk( char* pFileName )
    {
        FILE* fpInFile = 0;
        Datatype buffer;
        int count = 0;

        // 파일을 연다.
        fpInFile = _tfopen( pFileName, TEXT("rb") );

        // 파일을 여는데 실패했다면 돌아간다.
        if( fpInFile == 0 )
            return false;

        // 먼저 List의 Size를 읽어온다.
        fread( &count, sizeof( int ), 1, fpInFile );

		// Loop를 돌면서 Data를 읽어온다.
        while( count != 0 )
        {
            fread( &buffer, sizeof( Datatype ), 1, fpInFile );
            Append( buffer );
            count--;
        }

		// 파일을 닫는다.
        fclose( fpInFile );

        return true;
    }

};




// -------------------------------------------------------
// Name:        DListIterator
// Description: This is the basic linked list iterator class.
// -------------------------------------------------------
template<class Datatype>
class DListIterator
{
public:

// ----------------------------------------------------------------
//  Name:           m_Node
//  Description:    현재의 Node를 가리키는 Pointer
// ----------------------------------------------------------------
    DListNode<Datatype>* m_Node;


// ----------------------------------------------------------------
//  Name:           m_List
//  Description:    현재의 List를 가리키는 Pointer
// ----------------------------------------------------------------
    DLinkedList<Datatype>* m_List;
public:


// ----------------------------------------------------------------
//  Name:           DListIterator
//  Description:    Constructor  - 주이진 List와 Node로 Interator를 생성한다. , creates an iterator that points
//  Arguments:      pList	- Iterator가 속한 List의 Pointer 
//                  pNode	- 현재의 Node를 가리키는 Pointer 
//  Return Value:   None.
// ----------------------------------------------------------------
    DListIterator( DLinkedList<Datatype>* pList = 0, DListNode<Datatype>* pNode = 0 )
    {
        m_List = pList;
        m_Node = pNode;
    }


// ----------------------------------------------------------------
//  Name:           Start
//  Description:    Iterator를 List의 맨앞 Node로 설정한다.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Start()
    {
        if( m_List != 0 )	m_Node = m_List->m_Head;
    }

// ----------------------------------------------------------------
//  Name:           End
//  Description:    Iterator를 List의 맨뒤 Node로 설정한다.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void End()
    {
        if( m_List != 0 )	m_Node = m_List->m_Tail;
    }


// ----------------------------------------------------------------
//  Name:           Forth
//  Description:    Iterator를 다음 Node로 이동시킨다.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Forth()
    {
        if( m_Node != 0 )	m_Node = m_Node->m_Next;
    }


// ----------------------------------------------------------------
//  Name:           Back
//  Description:    Iterator를 앞 Node로 이동시킨다.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
    void Back()
    {
        if( m_Node != 0 )	m_Node = m_Node->m_Previous;
    }


// ----------------------------------------------------------------
//  Name:           Item
//  Description:    Iterator가 가리키는 Item을 얻어온다.
//  Arguments:      None.
//  Return Value:   Node에 있는 Data를 얻어온다.
// ----------------------------------------------------------------
    Datatype& Item()
    {
        return m_Node->m_Data;
    }


// ----------------------------------------------------------------
//  Name:           Valid
//  Description:    Node가 valid한지 검사한다.
//  Arguments:      None.
//  Return Value:   valid 하면 true를 돌려준다.
// ----------------------------------------------------------------
    bool Valid()
    {
        return (m_Node != 0);
    }


// ----------------------------------------------------------------
//  Name:           operator==
//  Description:    2개의 Iterator가 같은 Node를 갈리키는지 검사한다.
//  Arguments:      None.
//  Return Value:   같은 Node라면 true를 돌려준다.
// ----------------------------------------------------------------
    bool operator==( DListIterator<Datatype>& p_rhs )
    {
        if( m_Node == p_rhs.m_Node && m_List == p_rhs.m_List )
        {
            return true;
        }
        return false;
    }

};



#endif
