//-------------------------------------------------------------------------------------------------
// NkRbTreeSet.inl
// 엔도어즈 기술개발팀
// 이제완
//-------------------------------------------------------------------------------------------------

template <class T, class TComp> inline
NkRbTreeSet<T,TComp>::NkRbTreeSet()
: m_uiCount(0)
{
	NIL = NkNew NkRbTreeSetNode<T>;

	memset(NIL, 0, sizeof(NkRbTreeSetNode<T>));

	m_pkRoot = NIL;
}

template <class T, class TComp> inline
NkRbTreeSet<T,TComp>::~NkRbTreeSet()
{
	if( m_pkRoot != NIL )
		_RemoveRecursive(m_pkRoot);

	NkDelete NIL;
}


//-------------------------------------------------------------------------------------------------

template <class T, class TComp> inline
unsigned int NkRbTreeSet<T,TComp>::GetCount() const
{
	return m_uiCount;
}

template <class T, class TComp> inline
unsigned int NkRbTreeSet<T,TComp>::GetAllocedBytes() const
{
	return sizeof(NkRbTreeSetNode<T>) * m_uiCount;
}

template <class T, class TComp> inline
unsigned int NkRbTreeSet<T,TComp>::GetMaxDepth() const
{
	NkRbTreeSetNode<T>* pkNode = m_pkRoot;
	unsigned int uiDepth = 0;

	while( pkNode != NIL )
	{
		if( !pkNode->m_bRed )
			uiDepth += 1;
		pkNode = pkNode->m_pkLeft;
	}

	return uiDepth * 2;
}

//-------------------------------------------------------------------------------------------------
template <class T, class TComp> inline
bool NkRbTreeSet<T,TComp>::InvalidPos(NkRbTreeSetPos kPos) const
{
	return kPos == NIL;
}

template <class T, class TComp> inline
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetRootPos() const
{
	return m_pkRoot;
}

template <class T, class TComp> inline
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetParentPos(NkRbTreeSetPos kPos) const
{
	return ((NkRbTreeSetNode<T>*)kPos)->m_pkParent;
}

template <class T, class TComp> inline
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetLeftChildPos(NkRbTreeSetPos kPos) const
{
	return ((NkRbTreeSetNode<T>*)kPos)->m_pkLeft;
}

template <class T, class TComp> inline
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetRightChildPos(NkRbTreeSetPos kPos) const
{
	return ((NkRbTreeSetNode<T>*)kPos)->m_pkRight;
}

template <class T, class TComp>
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetBeginPos() const
{
	if( m_pkRoot == NIL ) return NIL;
	return _FindMin(m_pkRoot);
}

template <class T, class TComp>
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetEndPos() const
{
	if( m_pkRoot == NIL ) return NIL;
	return _FindMax(m_pkRoot);
}

template <class T, class TComp>
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetNextPos(NkRbTreeSetPos kPos) const
{
	return _FindNext((NkRbTreeSetNode<T>*)kPos);
}

template <class T, class TComp>
NkRbTreeSetPos NkRbTreeSet<T,TComp>::GetPrevPos(NkRbTreeSetPos kPos) const
{
	return _FindPrev((NkRbTreeSetNode<T>*)kPos);
}

//-------------------------------------------------------------------------------------------------

template <class T, class TComp> inline
bool NkRbTreeSet<T,TComp>::IsRedNode(NkRbTreeSetPos kPos) const
{
	return ((NkRbTreeSetNode<T>*)kPos)->m_bRed;
}

template <class T, class TComp> inline
T& NkRbTreeSet<T,TComp>::GetData(NkRbTreeSetPos kPos) const
{
	return ((NkRbTreeSetNode<T>*)kPos)->m_data;
}

template <class T, class TComp> inline
T* NkRbTreeSet<T,TComp>::GetDataPtr(NkRbTreeSetPos kPos) const
{
	return &((NkRbTreeSetNode<T>*)kPos)->m_data;
}

//-------------------------------------------------------------------------------------------------

template <class T, class TComp>
NkRbTreeSetPosVector* NkRbTreeSet<T,TComp>::BuildMapPosVector()
{
	unsigned int uiDepth = GetMaxDepth();

	m_kPosVector.Resize((unsigned int)pow((double)2, (double)uiDepth));
	m_kPosVector.ClearData();

	NkRbTreeSetNode<T>* pkNode = m_pkRoot;

	m_kPosVector[0] = pkNode;

	unsigned int uiSizeOfDepth = 1;
	unsigned int uiIndex = 1;
	unsigned int uiBaseIndex = 0;

	for( unsigned int uiCurDepth = 2; uiCurDepth <= uiDepth; ++uiCurDepth )
	{
		for( unsigned int i = 0; i < uiSizeOfDepth; ++i )
		{
			pkNode = (NkRbTreeSetNode<T>*)m_kPosVector[uiBaseIndex++];

			if( pkNode == NIL || pkNode == 0 )
			{
				m_kPosVector[uiIndex++] = 0;
				m_kPosVector[uiIndex++] = 0;
			}
			else
			{
				m_kPosVector[uiIndex++] = (NkRbTreeSetPos)pkNode->m_pkLeft;
				m_kPosVector[uiIndex++] = (NkRbTreeSetPos)pkNode->m_pkRight;
			}
		}
		uiSizeOfDepth *= 2;
	}

	return &m_kPosVector;
}

//-------------------------------------------------------------------------------------------------
// Insert
//-------------------------------------------------------------------------------------------------
template <class T, class TComp>
bool NkRbTreeSet<T,TComp>::Insert(const T& data)
{
	NkRbTreeSetNode<T>* pkNewNode = _NewNode();
	pkNewNode->m_data = data;
	pkNewNode->m_pkLeft = NIL;
	pkNewNode->m_pkRight = NIL;

	if( m_uiCount == 0 )
	{
		pkNewNode->m_pkParent = NIL;
		pkNewNode->m_bRed = false;
		m_pkRoot = pkNewNode;
		m_uiCount = 1;
		return true;
	}

	NkRbTreeSetNode<T>* pkNode = m_pkRoot;
	NkRbTreeSetNode<T>* pkTargetNode = NIL;

	while( pkNode != NIL )
	{
		pkTargetNode = pkNode;

		if( TComp::Equals(data, pkNode->m_data) )
		{
			_DeleteNode(pkNewNode);
			return false;
		}
		else if( TComp::Greater(data, pkNode->m_data) )
			pkNode = pkNode->m_pkRight;			
		else
			pkNode = pkNode->m_pkLeft;
	}

	if( TComp::Greater(data, pkTargetNode->m_data) )
		pkTargetNode->m_pkRight = pkNewNode;
	else
		pkTargetNode->m_pkLeft = pkNewNode;
		

	pkNewNode->m_pkParent = pkTargetNode;
	pkNewNode->m_bRed = true;

	_InsertFixup(pkNewNode);

	++m_uiCount;

	return true;
}

//-------------------------------------------------------------------------------------------------
// Remove
//-------------------------------------------------------------------------------------------------
template <class T, class TComp>
bool NkRbTreeSet<T,TComp>::Remove(const T& data)
{

	NkRbTreeSetNode<T>* pkNode = _Find(data);

	if( pkNode == NIL )
		return false;

	NkRbTreeSetNode<T>* pkTargetNode;
	NkRbTreeSetNode<T>* pkChildNode;

	if( pkNode->m_pkLeft == NIL || pkNode->m_pkRight == NIL )
		pkTargetNode = pkNode;
	else
		pkTargetNode = _FindNext(pkNode);

	if( pkTargetNode->m_pkLeft != NIL )
		pkChildNode = pkTargetNode->m_pkLeft;
	else
		pkChildNode = pkTargetNode->m_pkRight;

	pkChildNode->m_pkParent = pkTargetNode->m_pkParent;

	if( pkTargetNode->m_pkParent == NIL )
		m_pkRoot = pkChildNode;
	else if( pkTargetNode == pkTargetNode->m_pkParent->m_pkLeft )
		pkTargetNode->m_pkParent->m_pkLeft = pkChildNode;
	else
		pkTargetNode->m_pkParent->m_pkRight = pkChildNode;

	if( pkTargetNode != pkNode )
	{
		pkNode->m_data = pkTargetNode->m_data;
	}

	if( pkTargetNode->m_bRed == false )
		_RemoveFixup(pkChildNode);

	_DeleteNode( pkTargetNode );

	--m_uiCount;

	return true;
}

template <class T, class TComp>
void NkRbTreeSet<T,TComp>::RemoveAll()
{
	if( m_pkRoot != NIL )
		_RemoveRecursive(m_pkRoot);
}

//-------------------------------------------------------------------------------------------------
// Find
//-------------------------------------------------------------------------------------------------
template <class T, class TComp>
NkRbTreeSetPos NkRbTreeSet<T,TComp>::FindPos(const T& data) const
{
	return _Find(data);
}

template <class T, class TComp>
bool NkRbTreeSet<T,TComp>::FindData(const T& data) const
{
	NkRbTreeSetNode<T>* pkNode = _Find(key);

	if( pkNode != NIL )
		return true;

	return false;
}

//-------------------------------------------------------------------------------------------------

template <class T, class TComp> inline
void NkRbTreeSet<T,TComp>::_InsertFixup(NkRbTreeSetNode<T>* pkNode)
{

	// 
	//               g(black)                          g(red)
	//              ┌─┴─┐	                     ┌─┴─┐ 
	//            p(red)   u(red)	  =>         p(black)   u(black) 
	//          ┌─┴─┐		                ┌─┴─┐ 
	//     pkNode(red)  		           pkNode(red)   
	//
	//
	//
	//               g(black)                          g(red)
	//              ┌─┴─┐	                     ┌─┴─┐ 
	//            p(red)   u(black)    =>         p(black)   u(black) 
	//          ┌─┴─┐		                ┌─┴─┐ 
	//     pkNode(red)  		           pkNode(red)   
	//    ┌─┴─┐
	//    a       b
	//


	NkRbTreeSetNode<T>* pkParent = pkNode->m_pkParent;
	NkRbTreeSetNode<T>* pkGrandParent = pkParent->m_pkParent; 

	NkRbTreeSetNode<T>* pkUncleNode;

	// 부모 노드가 Red일 경우에 불균형이 발생하므로, 교정 작업을 한다
	while( pkParent->m_bRed )
	{
		// pkParent가 pkGrandParent의 왼쪽 노드인 경우
		if( pkParent == pkGrandParent->m_pkLeft )
		{
			pkUncleNode = pkGrandParent->m_pkRight;

			if( pkUncleNode->m_bRed )
			{
				pkParent->m_bRed = false;
				pkUncleNode->m_bRed = false;
				pkGrandParent->m_bRed = true;

				pkNode = pkGrandParent;
				pkParent = pkNode->m_pkParent;
				pkGrandParent = pkParent->m_pkParent; 
			}
			else 
			{
				if( pkNode == pkParent->m_pkRight )
				{
					pkNode = pkParent;
					_RotateLeft(pkNode);
				}
				pkParent->m_bRed = false;
				pkGrandParent->m_bRed = true;
				_RotateRight(pkGrandParent);
			}
		}
		// pkParent가 pkGrandParent의 오른쪽 노드인 경우
		else
		{
			pkUncleNode = pkGrandParent->m_pkLeft;

			if( pkUncleNode->m_bRed )
			{
				pkParent->m_bRed = false;
				pkUncleNode->m_bRed = false;
				pkGrandParent->m_bRed = true;

				pkNode = pkGrandParent;
				pkParent = pkNode->m_pkParent;
				pkGrandParent = pkParent->m_pkParent; 
			}
			else 
			{
				if( pkNode == pkParent->m_pkLeft )
				{
					pkNode = pkParent;
					_RotateRight(pkNode);
				}
				pkParent->m_bRed = false;
				pkGrandParent->m_bRed = true;
				_RotateLeft(pkGrandParent);
			}
		}
	}

	m_pkRoot->m_bRed = false;
}

template <class T, class TComp> inline
void NkRbTreeSet<T,TComp>::_RemoveFixup(NkRbTreeSetNode<T>* pkNode)
{
	NkRbTreeSetNode<T>* pkParent;
	NkRbTreeSetNode<T>* pkSibling;

	// 삭제된 노드가 Root가 아니고 Black일 경우에 불균형이 발생하므로, 교정 작업을 한다
	while( pkNode != m_pkRoot && pkNode->m_bRed == false )
	{
		pkParent = pkNode->m_pkParent;

		if( pkNode == pkParent->m_pkLeft )
		{
			pkSibling = pkParent->m_pkRight;

			if( pkSibling->m_bRed == true )
			{
				pkSibling->m_bRed = false;
				pkParent->m_bRed = true;
				_RotateLeft(pkParent);

				pkParent = pkNode->m_pkParent;
				pkSibling = pkParent->m_pkRight;
			}

			if( pkSibling->m_pkLeft == 0 || pkSibling->m_pkRight == 0 ) 
				break;

			if( pkSibling->m_pkLeft->m_bRed == false && pkSibling->m_pkRight->m_bRed == false )
			{
				pkSibling->m_bRed = true;
				pkNode = pkParent;
			}
			else 
			{
				if( pkSibling->m_pkRight->m_bRed == false )
				{
					pkSibling->m_pkLeft->m_bRed = false;
					pkSibling->m_bRed = true;
					_RotateRight(pkSibling);

					pkParent = pkNode->m_pkParent;
					pkSibling = pkParent->m_pkRight;
				}

				pkSibling->m_bRed = pkParent->m_bRed;
				pkParent->m_bRed = false;
				pkSibling->m_pkRight->m_bRed = false;
				_RotateLeft(pkParent);
				pkNode = m_pkRoot;
			}
		}
		else
		{
			pkSibling = pkParent->m_pkLeft;

			if( pkSibling->m_bRed == true )
			{
				pkSibling->m_bRed = false;
				pkParent->m_bRed = true;
				_RotateRight(pkParent);

				pkParent = pkNode->m_pkParent;
				pkSibling = pkParent->m_pkLeft;
			}

			if( pkSibling->m_pkLeft == 0 || pkSibling->m_pkRight == 0 ) 
				break;

			if( pkSibling->m_pkLeft->m_bRed == false && pkSibling->m_pkRight->m_bRed == false )
			{
				pkSibling->m_bRed = true;
				pkNode = pkParent;
			}
			else 
			{
				if( pkSibling->m_pkLeft->m_bRed == false )
				{
					pkSibling->m_pkRight->m_bRed = false;
					pkSibling->m_bRed = true;
					_RotateLeft(pkSibling);

					pkParent = pkNode->m_pkParent;
					pkSibling = pkParent->m_pkLeft;
				}

				pkSibling->m_bRed = pkParent->m_bRed;
				pkParent->m_bRed = false;
				pkSibling->m_pkLeft->m_bRed = false;
				_RotateRight(pkParent);
				pkNode = m_pkRoot;
			}
		}
	}

	pkNode->m_bRed = false;	
}

template <class T, class TComp> inline
NkRbTreeSetNode<T>*
NkRbTreeSet<T,TComp>::_FindMin(NkRbTreeSetNode<T>* pkNode) const
{
	NkRbTreeSetNode<T>* pkTemp = pkNode->m_pkLeft;

	while( pkTemp != NIL )
	{
		pkNode = pkTemp;
		pkTemp = pkTemp->m_pkLeft;
	}

	return pkNode;
}

template <class T, class TComp> inline
NkRbTreeSetNode<T>*
NkRbTreeSet<T,TComp>::_FindMax(NkRbTreeSetNode<T>* pkNode) const
{
	NkRbTreeSetNode<T>* pkTemp = pkNode->m_pkRight;

	while( pkTemp != NIL )
	{
		pkNode = pkTemp;
		pkTemp = pkTemp->m_pkRight;
	}

	return pkNode;
}

template <class T, class TComp> inline
NkRbTreeSetNode<T>*
NkRbTreeSet<T,TComp>::_FindPrev(NkRbTreeSetNode<T>* pkNode) const
{
	if( pkNode->m_pkLeft != NIL )
		return _FindMax(pkNode->m_pkLeft);

	NkRbTreeSetNode<T>* pkParent = pkNode->m_pkParent;

	while( pkParent != NIL && pkNode == pkParent->m_pkLeft )
	{
		pkNode = pkParent;
		pkParent = pkParent->m_pkParent;
	}

	return pkParent;
}

template <class T, class TComp> inline
NkRbTreeSetNode<T>*
NkRbTreeSet<T,TComp>::_FindNext(NkRbTreeSetNode<T>* pkNode) const
{
	if( pkNode->m_pkRight != NIL )
		return _FindMin(pkNode->m_pkRight);

	NkRbTreeSetNode<T>* pkParent = pkNode->m_pkParent;

	while( pkParent != NIL && pkNode == pkParent->m_pkRight )
	{
		pkNode = pkParent;
		pkParent = pkParent->m_pkParent;
	}

	return pkParent;
}

template <class T, class TComp> inline
NkRbTreeSetNode<T>*
NkRbTreeSet<T,TComp>::_Find(const T& data) const
{
	NkRbTreeSetNode<T>* pkNode = m_pkRoot;

	while( pkNode != NIL && !TComp::Equals(data, pkNode->m_data) )
	{
		if( TComp::Greater(data, pkNode->m_data) )
			pkNode = pkNode->m_pkRight;
		else
			pkNode = pkNode->m_pkLeft;
			
	}

	return pkNode;
}

template <class T, class TComp> inline
void NkRbTreeSet<T,TComp>::_RotateLeft(NkRbTreeSetNode<T>* pkNode)
{
	NkRbTreeSetNode<T>* pkParent = pkNode->m_pkParent;
	NkRbTreeSetNode<T>* pkRight = pkNode->m_pkRight;
	NkRbTreeSetNode<T>* pkBeta = pkRight->m_pkLeft;

	// 알고리즘 도식
	//
	//    (pkParent)                              (pkParent)
	//        │                                      │
	//        Ｘ(pkNode)           →        (pkRight)Ｙ
	//    ┌─┴─┐             (회전)           ┌─┴─┐
	//    α      Ｙ(pkRight)             (pkNode)Ｘ      γ
	//        ┌─┴─┐                      ┌─┴─┐
	//        β      γ                      α      β


	// Ｘ의 오른쪽 자식 노드를 β로 설정한다
	pkNode->m_pkRight = pkBeta;

	// β가 NIL이 아니면,
	// β의 부모 노드를 Ｘ로 설정한다
	if( pkBeta != NIL )
		pkBeta->m_pkParent = pkNode;

	// Ｙ의 왼쪽 자식 노드를 Ｘ로 설정한다
	pkRight->m_pkLeft = pkNode;

	// Ｘ의 부모 노드를 Ｙ로 설정한다
	pkNode->m_pkParent = pkRight;

	if( pkParent == NIL )
	{
		// pkParent가 NIL이면 최상위 노드이므로 m_pkRoot를 Y로 설정한다
		m_pkRoot = pkRight;
	}
	else
	{
		// pkNode가 부모의 왼쪽 자식 노드인지, 오른쪽 자식 노드인지 판단하여 해당 노드를 Y로 갱신한다
		if( pkParent->m_pkLeft == pkNode )
			pkParent->m_pkLeft = pkRight;
		else
			pkParent->m_pkRight = pkRight;
	}

	// Ｙ의 부모 노드를 pkParent로 설정한다.
	pkRight->m_pkParent = pkParent;
}

template <class T, class TComp> inline
void NkRbTreeSet<T,TComp>::_RotateRight(NkRbTreeSetNode<T>* pkNode)
{
	NkRbTreeSetNode<T>* pkParent = pkNode->m_pkParent;
	NkRbTreeSetNode<T>* pkLeft = pkNode->m_pkLeft;
	NkRbTreeSetNode<T>* pkBeta = pkLeft->m_pkRight;

	// 알고리즘 도식
	//
	//            (pkParent)                (pkParent)
	//                │                        │
	//        (pkNode)Ｘ           →           Ｙ(pkLeft)
	//            ┌─┴─┐     (회전)     ┌─┴─┐
	//    (pkLeft)Ｙ      γ                α      Ｘ(pkNode)
	//        ┌─┴─┐                        ┌─┴─┐
	//        α      β                        β      γ           


	// Ｘ의 왼쪽 자식 노드를 β로 설정한다
	pkNode->m_pkLeft = pkBeta;

	// β가 NULL이 아니면
	// β의 부모 노드를 Ｘ로 설정한다
	if( pkBeta != NIL )
		pkBeta->m_pkParent = pkNode;

	// Ｙ의 오른쪽 자식 노드를 Ｘ로 설정한다
	pkLeft->m_pkRight = pkNode;

	// Ｘ의 부모 노드를 Ｙ로 설정한다
	pkNode->m_pkParent = pkLeft;

	if( pkParent == NIL )
	{
		// pkParent가 NIL이면 최상위 노드이므로 m_pkRoot를 Y로 설정한다
		m_pkRoot = pkLeft;
	}
	else
	{
		// pkNode가 부모의 왼쪽 자식 노드인지, 오른쪽 자식 노드인지 판단하여 해당 노드를 Y로 갱신한다
		if( pkParent->m_pkLeft == pkNode )
			pkParent->m_pkLeft = pkLeft;
		else
			pkParent->m_pkRight = pkLeft;
	}

	// Ｙ의 부모 노드를 pkParent로 설정한다.
	pkLeft->m_pkParent = pkParent;
}

template <class T, class TComp>
void NkRbTreeSet<T,TComp>::_RemoveRecursive(NkRbTreeSetNode<T>* pkNode)
{
	NkRbTreeSetNode<T>* pkParent = pkNode->m_pkParent;
	NkRbTreeSetNode<T>* pkLeft = pkNode->m_pkLeft;
	NkRbTreeSetNode<T>* pkRight = pkNode->m_pkRight;

	if( pkLeft != NIL ) 
		_RemoveRecursive(pkLeft);

	if( pkRight != NIL )
		_RemoveRecursive(pkRight);

	if( pkParent != NIL )
	{
		if( pkParent->m_pkLeft == pkNode )
			pkParent->m_pkLeft = NIL;
		else
			pkParent->m_pkRight = NIL;
	}

	_DeleteNode(pkNode);
}


template <class T, class TComp> inline
NkRbTreeSetNode<T>* NkRbTreeSet<T,TComp>::_NewNode()
{
	return NkNew NkRbTreeSetNode<T>;
}

template <class T, class TComp> inline
void NkRbTreeSet<T,TComp>::_DeleteNode( NkRbTreeSetNode<T>* pkNode )
{
	NkDelete pkNode;
}
