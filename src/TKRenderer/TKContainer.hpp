#pragma once

namespace KContainer
{

    ///////////////////////////////////////////////////////
    // TKArrayList
    ///////////////////////////////////////////////////////
    template <class OBJECTTYPE>
    TKArrayList<OBJECTTYPE>::TKArrayList(void)
    {
	    FCount = 0;
	    FCapacity = 32;

	    pNodes = NULL;
	    Grow(true);
    }

    template <class OBJECTTYPE>
    TKArrayList<OBJECTTYPE>::~TKArrayList(void)
    {
	    free(pNodes);
	    FCount = 0;
    }

    template <class OBJECTTYPE>
    bool TKArrayList<OBJECTTYPE>::Grow(bool PowerGrowed)
    {
	    int			NewCapacity;
	    bool		isGrowed=false;

	    // 현재 용량보다 요소들의 개수가 더 많을 경우 용량을 현재의 두배로 들려준다.
	    NewCapacity = FCapacity;
	    while (NewCapacity <= FCount)
        {
            if (NewCapacity <= 0)
            {
                NewCapacity = 32;
            }
            NewCapacity = NewCapacity * 2;
		    isGrowed = true;
	    }

	    if (isGrowed || PowerGrowed)
	    {
		    if (pNodes == NULL)
		    {
                pNodes = (OBJECTTYPE **)malloc(sizeof(OBJECTTYPE *) * NewCapacity);
		    }
		    else
		    {
                pNodes = (OBJECTTYPE **)realloc(pNodes, sizeof(OBJECTTYPE *) * NewCapacity);
		    }
		    FCapacity = NewCapacity;
	    }

	    return true;
    }

    template <class OBJECTTYPE>
    int TKArrayList<OBJECTTYPE>::Count(void)
    {
	    return FCount;
    }

    template <class OBJECTTYPE>
    OBJECTTYPE * TKArrayList<OBJECTTYPE>::Items(int Index)
    {
        return ( pNodes[Index] );
    }

    template <class OBJECTTYPE>
    OBJECTTYPE * TKArrayList<OBJECTTYPE>::operator[](int Index)
    {
        return ( pNodes[Index] );
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Add(OBJECTTYPE * pObject)
    {
	    // 현재 용량보다 요소들의 개수가 더 많을 경우 용량을 현재의 두배로 들려준다.
	    while (FCapacity <= FCount)
	    {
		    Grow(false);
	    }

	    // FCount 위치에 요소를 추가한다.
        pNodes[FCount] = pObject;

	    // 갯수를 하나 증가시킨다.
	    FCount++;
    }

    template <class OBJECTTYPE>
    OBJECTTYPE * TKArrayList<OBJECTTYPE>::AddNew(OBJECTTYPE & AObject)
    {
        OBJECTTYPE *pObject = new OBJECTTYPE( AObject );
        *pObject = AObject;
        Add( pObject );

        return pObject;
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Insert(int Index, OBJECTTYPE *pObject)
    {
        if (Index > FCount)
        {
            //MessageBox(0, "", "Insert fail", 0x00000010L);
            return;
        }

	    while (FCapacity <= FCount)
	    {
		    Grow(false);
	    }

        int CopyMemLength = FCount - Index;

	    if (CopyMemLength > 0)
        {
            memcpy(pNodes + Index + 1, pNodes + Index, (sizeof(OBJECTTYPE *) * CopyMemLength));
        }
        pNodes[Index] = pObject;

	    FCount++;
    }

    template <class OBJECTTYPE>
    OBJECTTYPE * TKArrayList<OBJECTTYPE>::InsertNew(int Index, OBJECTTYPE & AObject)
    {
        OBJECTTYPE *pObject = new OBJECTTYPE( AObject );
        Insert( Index, pObject );

        return pObject;
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Erase(int Index)
    {
	    if ((FCount-1) > Index)
        {
		    memcpy(pNodes + Index, pNodes + Index + 1, (sizeof(OBJECTTYPE *) * (FCapacity - Index - 1)));
            FCount--;
	    }
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::EraseDelete(int Index)
    {
        OBJECTTYPE *pObject = pNodes[Index];
        //_ASSERT( _CrtCheckMemory( ) );
        delete pObject;
        Erase(Index);
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Exchange(int Index1, int Index2)
    {
        OBJECTTYPE *pObject;

        pObject = pNodes[Index1];
        pNodes[Index1] = pNodes[Index2];
        pNodes[Index2] = pObject;
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Pack(void)
    {
        pNodes = (OBJECTTYPE **)realloc(pNodes, sizeof(OBJECTTYPE *) * FCount);
        FCapacity = FCount;
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Clear(void)
    {
        int NewCapacity = 32;

        pNodes = (OBJECTTYPE **)realloc(pNodes, sizeof(OBJECTTYPE *) * FCapacity);
        memset(pNodes, 0, sizeof(FCapacity));
        FCapacity = NewCapacity;
        FCount = 0;
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::ClearDelete(void)
    {
        for ( int i = 0 ; i < FCount ;  ++i )
        {
            //_ASSERT( _CrtCheckMemory( ) );
            delete pNodes[i];
            pNodes[i] = NULL;
        }
        Clear();
    }

    template <class OBJECTTYPE>
    int TKArrayList<OBJECTTYPE>::Capacity(void)
    {
        return FCapacity;
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::SetCapacity(int NewCapacity)
    {
        pNodes = (OBJECTTYPE **)realloc(pNodes, sizeof(OBJECTTYPE *) * NewCapacity);
        FCapacity = NewCapacity;
    }

    template <class OBJECTTYPE>
    int TKArrayList<OBJECTTYPE>::IndexOf(OBJECTTYPE * pObject)
    {
        int i = 0;
        while ( i < FCount )
        {
            if ( pObject == pNodes[i] )
            {
                return i;
            }
            ++i;
        };
    }

    template <class OBJECTTYPE>
    const OBJECTTYPE * TKArrayList<OBJECTTYPE>::First(void)
    {
        return pNodes[0];
    }

    template <class OBJECTTYPE>
    const OBJECTTYPE * TKArrayList<OBJECTTYPE>::Last(void)
    {
        return pNodes[FCount-1];
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::Sort(int (*Compare)(OBJECTTYPE *, OBJECTTYPE *))
    {
        if (FCount > 0)
        {
            QuickSort( 0, FCount-1, Compare);
        }
    }

    template <class OBJECTTYPE>
    void TKArrayList<OBJECTTYPE>::QuickSort(int L, int R, int (*Compare)(OBJECTTYPE *, OBJECTTYPE *))
    {
        int I, J;
        OBJECTTYPE *P, *T;

        do
        {
            I = L;
            J = R;
            P = pNodes[ (L + R) >> 1 ];   // 1비트 우측시프트 연산한다. 반으로 나누는것과 동일하다.

            do
            {
                while (Compare( pNodes[I], P) < 0)
                {
                    I++;
                }
                while (Compare( pNodes[J], P) > 0)
                {
                    J--;
                }

                if (I <= J)
                {
                    T = pNodes[I];
                    pNodes[I] = pNodes[J];
                    pNodes[J] = T;
                    I++;
                    J--;
                }

            } while (!(I > J));

            if (L < J)
            {
                QuickSort(L, J, Compare);
            }

            L = I;

        } while (!(I >= R));
    }

    ///////////////////////////////////////////////////////
    // TKLinkedList
    ///////////////////////////////////////////////////////
    template <class OBJECTTYPE>
    TKLinkedList<OBJECTTYPE>::TKLinkedList(void)
    :pFirst(NULL),pLast(NULL),FCount(0)
    {
    }

    template <class OBJECTTYPE>
    TKLinkedList<OBJECTTYPE>::~TKLinkedList(void)
    {
        this->Clear();
    }

    template <class OBJECTTYPE>
    int	TKLinkedList<OBJECTTYPE>::Count(void)
    {
        return FCount;
    }

    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::Add(OBJECTTYPE * pObject)
    {
        TKLINKEDNODE *pNewNode = NULL;
        if (pObject)
        {
            pNewNode = new TKLINKEDNODE( pObject );
            if (pLast)
            {
                pLast->pNextNode = pNewNode;
                pNewNode->pPrevNode = pLast;
                pLast = pNewNode;
            }
            else
            {
                pFirst = pNewNode;
                pLast = pNewNode;
            }
            ++FCount;
        }
        return pNewNode;
    }

    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::AddNew(OBJECTTYPE & AObject)
    {
        OBJECTTYPE *pObject = new OBJECTTYPE( AObject );
        return Add( pObject );
    }

    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::Insert(TKLINKEDNODE *pNode, OBJECTTYPE *pObject)
    {
        TKLINKEDNODE *pNewNode = NULL;
        if (pNode && pObject)
        {
            pNewNode = new TKLINKEDNODE( pObject );
            pNewNode->pPrevNode = pNode->pPrevNode;
            pNewNode->pNextNode = pNode;
            pNode->pPrevNode = pNewNode;
            if (pNode == pFirst)
            {
                pFirst = pNewNode;
            }
            ++FCount;
        }
        return pNewNode;
    }

    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::InsertNew(TKLINKEDNODE *pNode, OBJECTTYPE & AObject)
    {
        TKLINKEDNODE *pNewNode = NULL;
        if (pNode)
        {
            OBJECTTYPE *pObject = new OBJECTTYPE( AObject );
            pNewNode = Insert( pNode, pObject );
        }
        return pNewNode;
    }

    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::FindNode(OBJECTTYPE * pObject)
    {
        TKLINKEDNODE *pNode = NULL;
        for ( pNode = pFirst ; pNode ; pNode = pNode->pNextNode )
        {
            if (pNode->pObject == pObject)
            {
                break;
            }
        }
        return pNode;
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::Erase(TKLINKEDNODE *pNode)
    {
        if (pNode && pFirst && pLast)
        {
            if (pFirst == pNode)
            {
                pFirst = pNode->pNextNode;
            }
            else
            {
                pNode->pPrevNode = pNode->pNextNode;
            }
            delete pNode;

            pNode = NULL;
            --FCount;
        }
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::EraseDelete(TKLINKEDNODE *pNode)
    {
        if (pNode)
        {
            if (pNode->pObject)
            {
                delete pNode->pObject;
                pNode->pObject = NULL;
            }
            Erase(pNode);
        }
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::Exchange(TKLINKEDNODE *pNode1, TKLINKEDNODE *pNode2)
    {
        OBJECTTYPE *pTempObject;

        if (pNode1 && pNode2)
        {
            pTempObject = pNode1->pObject;
            pNode1->pObject = pNode2->pObject;
            pNode2->pObject = pTempObject;
        }
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::Clear(void)
    {
        TKLINKEDNODE *pNode = pFirst, *pNextNode = NULL;
        while ( pNode )
        {
            pNextNode = pNode->pNextNode;
            delete pNode;
            pNode = pNextNode;
        }
        pFirst = NULL;
        pLast = NULL;
        FCount = 0;
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::ClearDelete(void)
    {
        TKLINKEDNODE *pNode;
        for ( pNode = pFirst ; pNode ; pNode = pNode->pNextNode )
        {
            if (pNode->pObject)
            {
                delete pNode->pObject;
                pNode->pObject = NULL;
            }
        }
        Clear();
    }
    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::First(void)
    {
        return pFirst;
    }

    template <class OBJECTTYPE>
    TKLINKEDNODE * TKLinkedList<OBJECTTYPE>::Last(void)
    {
        return pLast;
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::Sort(int (*Compare)(OBJECTTYPE *, OBJECTTYPE *))
    {
        if (FCount <= 0) return;

        int i;
        TKLINKEDNODE *pNode;
        OBJECTTYPE **pNodes;

        pNodes = (OBJECTTYPE **)malloc(sizeof(OBJECTTYPE *) * FCount);

        i = 0;
        for ( pNode = pFirst ; pNode ; pNode = pNode->pNextNode )
        {
            pNodes[i] = pNode->pObject;
            ++i;
        }

        QuickSort(pNodes, 0, FCount-1, Compare);

        i = 0;
        for ( pNode = pFirst ; pNode ; pNode = pNode->pNextNode )
        {
            pNode->pObject = pNodes[i];
            ++i;
        }

        free(pNodes);

        i = 1;
    }

    template <class OBJECTTYPE>
    void TKLinkedList<OBJECTTYPE>::QuickSort(OBJECTTYPE **pNodes, int L, int R, int (*Compare)(OBJECTTYPE *, OBJECTTYPE *))
    {
        int I, J;
        OBJECTTYPE *P, *T;

        do
        {
            I = L;
            J = R;
            P = pNodes[ (L + R) >> 1 ];   // 1비트 우측시프트 연산한다. 반으로 나누는것과 동일하다.

            do
            {
                while (Compare( pNodes[I], P) < 0)
                {
                    I++;
                }
                while (Compare( pNodes[J], P) > 0)
                {
                    J--;
                }

                if (I <= J)
                {
                    T = pNodes[I];
                    pNodes[I] = pNodes[J];
                    pNodes[J] = T;
                    I++;
                    J--;
                }

            } while (!(I > J));

            if (L < J)
            {
                QuickSort(pNodes, L, J, Compare);
            }

            L = I;

        } while (!(I >= R));
    }

    ///////////////////////////////////////////////////////
    // TKMap
    ///////////////////////////////////////////////////////
    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMap<INDEXTYPE, OBJECTTYPE>::TKMap(void)
    {
	    FCount = 0;
	    FCapacity = 32;

        pNodes = NULL;
	    Grow(true);
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMap<INDEXTYPE, OBJECTTYPE>::~TKMap(void)
    {
        Clear();
        free(pNodes);
	    FCount = 0;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    bool TKMap<INDEXTYPE, OBJECTTYPE>::Grow(bool PowerGrowed)
    {
	    int			NewCapacity;
	    bool		isGrowed=false;

	    // 현재 용량보다 요소들의 개수가 더 많을 경우 용량을 현재의 두배로 들려준다.
	    NewCapacity = FCapacity;
	    while (NewCapacity <= FCount)
	    {
            if (NewCapacity <= 0)
            {
                NewCapacity = 32;
            }
            NewCapacity = NewCapacity * 2;
		    isGrowed = true;
	    }

	    if (isGrowed || PowerGrowed)
	    {
		    if (pNodes == NULL)
		    {
			    pNodes = (TKMAPNODE **)malloc(sizeof(TKMAPNODE *) * NewCapacity);
		    }
		    else
		    {
			    pNodes = (TKMAPNODE **)realloc(pNodes, sizeof(TKMAPNODE *) * NewCapacity);
		    }
		    FCapacity = NewCapacity;
	    }

	    return true;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    int TKMap<INDEXTYPE, OBJECTTYPE>::FindNodeIndex(int L, int R, INDEXTYPE Index)
    {
        int Result = -1, H;

        if ( L < 0 || R < 0)
        {
            return -1;
        }

        H = ( L + R ) >> 1;
        TKMAPNODE *P = pNodes[ H ];

        if ( L == R )
        {
            if ( Index == P->Index )
            {
                Result = H;
            }
            else
            {
                Result = -1;
            }
        }
        else if ( L > H || H > R )
        {
            if ( Index == P->Index )
            {
                Result = H;
            }
            else
            {
                Result = -1;
            }
        }
        else
        {
            if ( Index == P->Index )
            {
                Result = H;
            }
            else if ( Index < P->Index )
            {
                Result = FindNodeIndex( L, H-1, Index);
            }
            else if ( Index > P->Index )
            {
                Result = FindNodeIndex( H+1, R, Index);
            }
        }

        return Result;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    int	TKMap<INDEXTYPE, OBJECTTYPE>::Count(void)
    {
        return FCount;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::Items(INDEXTYPE Index)
    {
        TKMAPNODE * pNode = NULL;

        int NodeIndex = FindNodeIndex( 0, FCount-1, Index );
        if (NodeIndex >= 0)
            pNode = pNodes[ NodeIndex ];

        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::operator[](INDEXTYPE Index)
    {
        return Items(Index);
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::Add(INDEXTYPE Index, OBJECTTYPE * pObject)
    {
        // 기존 인덱스와 중복될 수 없음.
        if (FindNodeIndex(0, FCount-1, Index) != -1)
        {
            return NULL;
        }

	    // 현재 용량보다 요소들의 개수가 더 많을 경우 용량을 현재의 두배로 들려준다.
	    while (FCapacity <= FCount)
	    {
		    Grow(false);
	    }

	    // 새로운 노드를 생성한다.
        TKMAPNODE * pNode = new TKMAPNODE;
        pNode->Index = Index;
        pNode->pObject = pObject;

        // FCount 위치에 요소를 추가한다.
        pNodes[FCount] = pNode;

	    // 갯수를 하나 증가시킨다.
	    FCount++;

        // 정렬한다.
        QuickSort(0, FCount-1);

        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::AddNew(INDEXTYPE Index, OBJECTTYPE & AObject)
    {
        OBJECTTYPE *pObject = new OBJECTTYPE( AObject );
        TKMAPNODE *pNode = Add( Index, pObject );
        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMap<INDEXTYPE, OBJECTTYPE>::Erase(INDEXTYPE Index)
    {
        TKMAPNODE * pNode = NULL;
        int NodeIndex = FindNodeIndex( 0, FCount-1, Index );

        if ( 0 <= NodeIndex )
        {
            pNode = pNodes[NodeIndex];
            delete pNode;
            memcpy( pNodes + NodeINdex, pNodes + NodeIndex + 1, (sizeof(TKMapNode *) * (FCapacity - NodeIndex - 1)));
            FCount--;
        }
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMap<INDEXTYPE, OBJECTTYPE>::EraseDelete(INDEXTYPE Index)
    {
        TKMAPNODE * pNode = NULL;
        int NodeIndex = FindNodeIndex( 0, FCount-1, Index );

        if ( 0 <= NodeIndex)
        {
            pNode = pNodes[NodeIndex];
            delete pNode->pObject;
            delete pNode;
            memcpy( pNodes + NodeIndex, pNodes + NodeIndex + 1, (sizeof(TKMapNode *) * (FCapacity - NodeIndex - 1)));
            FCount--;
        }
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMap<INDEXTYPE, OBJECTTYPE>::Pack(void)
    {
        pNodes = (TKMAPNODE **)realloc(pNodes, sizeof(TKMAPNODE *) * FCount);
        FCapacity = FCount;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMap<INDEXTYPE, OBJECTTYPE>::Clear(void)
    {
        int i = 0;
        while ( i < FCount )
        {
            delete pNodes[i];
            ++i;
        }
	    FCapacity = 32;
        pNodes = (TKMAPNODE **)realloc(pNodes, sizeof(TKMAPNODE *) * FCapacity);
	    FCount = 0;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMap<INDEXTYPE, OBJECTTYPE>::ClearDelete(void)
    {
        int i = 0;
        while ( i < FCount )
        {
            delete pNodes[i]->pObject;
            delete pNodes[i];
            ++i;
        }
	    FCapacity = 32;
        pNodes = (TKMAPNODE **)realloc(pNodes, sizeof(TKMAPNODE *) * FCapacity);
	    FCount = 0;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    int TKMap<INDEXTYPE, OBJECTTYPE>::Capacity(void)
    {
        return FCapacity;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::IndexOf(OBJECTTYPE *pObject)
    {
        int i = 0;
        TKMAPNODE *pNode = NULL;
        while ( i < FCount )
        {
            if ( pObject == pNodes[i]->pObject )
            {
                pNode = pNodes[i];
            }
            ++i;
        };
        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    const TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::First(void)
    {
        return pNodes[0];
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    const TKMAPNODE * TKMap<INDEXTYPE, OBJECTTYPE>::Last(void)
    {
        return pNodes[FCount-1];
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMap<INDEXTYPE, OBJECTTYPE>::QuickSort(int L, int R)
    {
        int I, J;
        TKMAPNODE *P, *T;

        do
        {
            I = L;
            J = R;
            P = pNodes[ (L + R) >> 1 ];   // 1비트 우측시프트 연산한다. 반으로 나누는것과 동일하다.

            do
            {
                while ( pNodes[I]->Index < P->Index )
                {
                    I++;
                }
                while ( P->Index < pNodes[J]->Index )
                {
                    J--;
                }

                if (I <= J)
                {
                    T = pNodes[I];
                    pNodes[I] = pNodes[J];
                    pNodes[J] = T;
                    I++;
                    J--;
                }

            } while (!(I > J));

            if (L < J)
            {
                QuickSort(L, J);
            }

            L = I;

        } while (!(I >= R));
    }

    ///////////////////////////////////////////////////////
    // TKMultiMap
    ///////////////////////////////////////////////////////
    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMultiMap<INDEXTYPE, OBJECTTYPE>::TKMultiMap(void)
    {
	    FCount = 0;
	    FCapacity = 32;

        pNodes = NULL;
	    Grow(true);
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMultiMap<INDEXTYPE, OBJECTTYPE>::~TKMultiMap(void)
    {
        Clear();
        free(pNodes);
	    FCount = 0;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    bool TKMultiMap<INDEXTYPE, OBJECTTYPE>::Grow(bool PowerGrowed)
    {
	    int			NewCapacity;
	    bool		isGrowed=false;

	    // 현재 용량보다 요소들의 개수가 더 많을 경우 용량을 현재의 두배로 들려준다.
	    NewCapacity = FCapacity;
	    while (NewCapacity <= FCount)
	    {
            if (NewCapacity <= 0)
            {
                NewCapacity = 32;
            }
            NewCapacity = NewCapacity * 2;
		    isGrowed = true;
	    }

	    if (isGrowed || PowerGrowed)
	    {
		    if (pNodes == NULL)
		    {
			    pNodes = (TKMULTIMAPNODE **)malloc(sizeof(TKMULTIMAPNODE *) * NewCapacity);
		    }
		    else
		    {
			    pNodes = (TKMULTIMAPNODE **)realloc(pNodes, sizeof(TKMULTIMAPNODE *) * NewCapacity);
		    }
		    FCapacity = NewCapacity;
	    }

	    return true;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    int TKMultiMap<INDEXTYPE, OBJECTTYPE>::FindNodeIndex(int L, int R, INDEXTYPE Index)
    {
        int Result = -1, H;

        if ( L < 0 || R < 0)
        {
            return -1;
        }

        H = ( L + R ) >> 1;
        TKMULTIMAPNODE *P = pNodes[ H ];

        if ( L == R )
        {
            if ( Index == P->Index )
            {
                Result = H;
            }
            else
            {
                Result = -1;
            }
        }
        else if ( L > H || H > R )
        {
            if ( Index == P->Index )
            {
                Result = H;
            }
            else
            {
                Result = -1;
            }
        }
        else
        {
            if ( Index == P->Index )
            {
                Result = H;
            }
            else if ( Index < P->Index )
            {
                Result = FindNodeIndex( L, H-1, Index);
            }
            else if ( Index > P->Index )
            {
                Result = FindNodeIndex( H+1, R, Index);
            }
        }

        return Result;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    int	TKMultiMap<INDEXTYPE, OBJECTTYPE>::Count(void)
    {
        return FCount;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::Items(INDEXTYPE Index)
    {
        TKMULTIMAPNODE * pNode = NULL;

        int NodeIndex = FindNodeIndex( 0, FCount-1, Index );
        if (NodeIndex >= 0)
            pNode = pNodes[ NodeIndex ];

        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::operator[](INDEXTYPE Index)
    {
        return Items(Index);
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::Add(INDEXTYPE Index, OBJECTTYPE * pObject)
    {
	    // 현재 용량보다 요소들의 개수가 더 많을 경우 용량을 현재의 두배로 들려준다.
	    while (FCapacity <= FCount)
	    {
		    Grow(false);
	    }

	    // 새로운 노드를 생성한다.
        TKMULTIMAPNODE * pNode = new TKMULTIMAPNODE;
        pNode->Index = Index;
        pNode->pObject = pObject;

        // FCount 위치에 요소를 추가한다.
        pNodes[FCount] = pNode;

	    // 갯수를 하나 증가시킨다.
	    FCount++;

        // 정렬한다.
        QuickSort(0, FCount-1);

        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::AddNew(INDEXTYPE Index, OBJECTTYPE & AObject)
    {
        OBJECTTYPE *pObject = new OBJECTTYPE( AObject );
        TKMULTIMAPNODE *pNode = Add( Index, pObject );
        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMultiMap<INDEXTYPE, OBJECTTYPE>::Erase(INDEXTYPE Index)
    {
        TKMULTIMAPNODE * pNode = NULL;
        int NodeIndex = FindNodeIndex( 0, FCount-1, Index );

        if ( 0 <= NodeIndex )
        {
            pNode = pNodes[NodeIndex];
            delete pNode;
            memcpy( pNodes + NodeINdex, pNodes + NodeIndex + 1, (sizeof(TKMultiMapNode *) * (FCapacity - NodeIndex - 1)));
            FCount--;
        }
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMultiMap<INDEXTYPE, OBJECTTYPE>::EraseDelete(INDEXTYPE Index)
    {
        TKMULTIMAPNODE * pNode = NULL;
        int NodeIndex = FindNodeIndex( 0, FCount-1, Index );

        if ( 0 <= NodeIndex)
        {
            pNode = pNodes[NodeIndex];
            delete pNode->pObject;
            delete pNode;
            memcpy( pNodes + NodeIndex, pNodes + NodeIndex + 1, (sizeof(TKMultiMapNode *) * (FCapacity - NodeIndex - 1)));
            FCount--;
        }
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMultiMap<INDEXTYPE, OBJECTTYPE>::Pack(void)
    {
        pNodes = (TKMULTIMAPNODE **)realloc(pNodes, sizeof(TKMULTIMAPNODE *) * FCount);
        FCapacity = FCount;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMultiMap<INDEXTYPE, OBJECTTYPE>::Clear(void)
    {
        int i = 0;
        while ( i < FCount )
        {
            delete pNodes[i];
            ++i;
        }
	    FCapacity = 32;
        pNodes = (TKMULTIMAPNODE **)realloc(pNodes, sizeof(TKMULTIMAPNODE *) * FCapacity);
	    FCount = 0;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMultiMap<INDEXTYPE, OBJECTTYPE>::ClearDelete(void)
    {
        int i = 0;
        while ( i < FCount )
        {
            delete pNodes[i]->pObject;
            delete pNodes[i];
            ++i;
        }
	    FCapacity = 32;
        pNodes = (TKMULTIMAPNODE **)realloc(pNodes, sizeof(TKMULTIMAPNODE *) * FCapacity);
	    FCount = 0;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    int TKMultiMap<INDEXTYPE, OBJECTTYPE>::Capacity(void)
    {
        return FCapacity;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::IndexOf(OBJECTTYPE *pObject)
    {
        int i = 0;
        TKMULTIMAPNODE *pNode = NULL;
        while ( i < FCount )
        {
            if ( pObject == pNodes[i]->pObject )
            {
                pNode = pNodes[i];
            }
            ++i;
        };
        return pNode;
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    const TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::First(void)
    {
        return pNodes[0];
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    const TKMULTIMAPNODE * TKMultiMap<INDEXTYPE, OBJECTTYPE>::Last(void)
    {
        return pNodes[FCount-1];
    }

    template <typename INDEXTYPE, typename OBJECTTYPE>
    void TKMultiMap<INDEXTYPE, OBJECTTYPE>::QuickSort(int L, int R)
    {
        int I, J;
        TKMULTIMAPNODE *P, *T;

        do
        {
            I = L;
            J = R;
            P = pNodes[ (L + R) >> 1 ];   // 1비트 우측시프트 연산한다. 반으로 나누는것과 동일하다.

            do
            {
                while ( pNodes[I]->Index < P->Index )
                {
                    I++;
                }
                while ( P->Index < pNodes[J]->Index )
                {
                    J--;
                }

                if (I <= J)
                {
                    T = pNodes[I];
                    pNodes[I] = pNodes[J];
                    pNodes[J] = T;
                    I++;
                    J--;
                }

            } while (!(I > J));

            if (L < J)
            {
                QuickSort(L, J);
            }

            L = I;

        } while (!(I >= R));
    }

    ///////////////////////////////////////////////////////
    // TKStaticArrayList
    ///////////////////////////////////////////////////////
    template <typename OBJECTTYPE, int ReservedCapacity>
    TKSTATICARRAYLIST::TKStaticArrayList(void)
    {
        FCapacity = ReservedCapacity;
        FCount = 0;
        //memset(Nodes, 0, sizeof(TKStaticArrayNode) * FCapacity);
        memset(Indexes, -1, sizeof(int) * FCapacity);
    }


    template <typename OBJECTTYPE, int ReservedCapacity>
    TKSTATICARRAYLIST::TKStaticArrayList(const TKSTATICARRAYLIST & StaticArrayList)
    {
        &this == StaticArrayList;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    TKSTATICARRAYLIST::~TKStaticArrayList(void)
    {
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    int	TKSTATICARRAYLIST::Count(void)
    {
        return FCount;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    OBJECTTYPE * TKSTATICARRAYLIST::Items(int Index)
    {
        OBJECTTYPE *pObject = GetNode(Index);
        return pObject;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    OBJECTTYPE * TKSTATICARRAYLIST::operator[](int Index)
    {
        OBJECTTYPE *pObject = GetNode(Index);
        return pObject;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    OBJECTTYPE * TKSTATICARRAYLIST::New(void)
    {
        OBJECTTYPE *pObject = NULL;
        for (int i = 0 ; i < FCapacity ; ++i )
        {
            if (Nodes[i].Used == false)
            {
                Nodes[i].Used = true;
                pObject = &Nodes[i].Object;
                Indexes[FCount] = i;
                ++FCount;
                break;
            }
        }
        return pObject;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    OBJECTTYPE * TKSTATICARRAYLIST::New(OBJECTTYPE &AObject)
    {
        OBJECTTYPE *pObject = NULL;
        pObject = this->New();
        if (pObject != NULL)
        {
            *pObject = AObject;
        }
        return pObject;
    }


    template <typename OBJECTTYPE, int ReservedCapacity>
    bool TKSTATICARRAYLIST::Delete(int Index)
    {
        OBJECTTYPE *pObject = GetNode(Index);
        if (pObject != NULL)
        {
            Nodes[ Indexes[Index] ].Used = false;
            memcpy(&Indexes[Index], &Indexes[Index+1], sizeof(int) * (FCapacity - Index  -1));
            --FCount;
            return true;
        }
        return false;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    bool TKSTATICARRAYLIST::Delete(OBJECTTYPE *pObject)
    {
        int Index = IndexOf(pObject);
        if ( Index >= 0 )
        {
            return Delete(Index);
        }
        return false;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    void TKSTATICARRAYLIST::Exchange(int Index1, int Index2)
    {
        int T;
        T = Indexes[Index1];
        Indexes[Index1] = Indexes[Index2];
        Indexes[Index2] = T;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    void TKSTATICARRAYLIST::Clear(void)
    {
        for (int i = 0 ; i < FCapacity ; ++i )
        {
            Indexes[i] = -1;
        }
        FCount = 0;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    int TKSTATICARRAYLIST::Capacity(void)
    {
        return FCapacity;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    int TKSTATICARRAYLIST::IndexOf(OBJECTTYPE *pObject)
    {
        for (int i = 0 ; i < FCapacity ; ++i )
        {
            if ( &Nodes[i].Object == pObject )
            {
                for ( j = 0 ; j < FCount ; ++j )
                {
                    if (Indexes[j] == i )
                    {
                        return j;
                    }
                }
            }
        }
        return -1;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    int TKSTATICARRAYLIST::IndexOf(OBJECTTYPE &AObject)
    {
        for (int i = 0 ; i < FCapacity ; ++i )
        {
            if ( Nodes[i].Object == AObject )
            {
                for ( j = 0 ; j < FCount ; ++j )
                {
                    if (Indexes[j] == i )
                    {
                        return j;
                    }
                }
            }
        }
        return -1;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    const OBJECTTYPE * TKSTATICARRAYLIST::First(void)
    {
        return &(Nodes[ Indexes[0] ].Object);
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    const OBJECTTYPE * TKSTATICARRAYLIST::Last(void)
    {
        return &(Nodes[ Indexes[ FCount-1 ] ].Object);
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    OBJECTTYPE * TKSTATICARRAYLIST::GetNode(int Index)
    {
        OBJECTTYPE *pObject = NULL;
        if (Index < FCount)
        {
            if (Indexes[Index] >= 0)
            {
                pObject = &Nodes[Indexes[Index]].Object;
            }
        }
        return pObject;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    OBJECTTYPE * TKSTATICARRAYLIST::GetNodeArrayIndex(int Index)
    {
        OBJECTTYPE *pObject = NULL;
        if (Index < FCapacity)
        {
            pObject = &Nodes[Index].Object;
        }
        return pObject;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    void TKSTATICARRAYLIST::Sort(int (*Compare)(OBJECTTYPE *, OBJECTTYPE *))
    {
        if (FCount > 0)
        {
            QuickSort( 0, FCount-1, Compare);
        }
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    bool TKSTATICARRAYLIST::operator == (const TKSTATICARRAYLIST & StaticArrayList)
    {
        if (this->FCount != StaticArrayList.FCount)
        {
            return false;
        }

        if (this->FCapacity != StaticArrayList.FCapacity)
        {
            return false;
        }

        for ( int i = 0 ; i < FCapacity ; ++i )
        {
            if (this->Nodes[i] != StaticArrayList.Nodes[i])
            {
                return false;
            }
        }
        return true;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    bool TKSTATICARRAYLIST::operator != (const TKSTATICARRAYLIST & StaticArrayList)
    {
        return !(&this == StaticArrayList)
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    void TKSTATICARRAYLIST::operator = (const TKSTATICARRAYLIST & StaticArrayList)
    {
        for ( int i = 0 ; i < FCapacity ; ++i )
        {
            this->Nodes[i] = StaticArrayList.Nodes[i];
            this->Indexes[i] = StaticArrayList.Indexes[i];
        }
        this->FCount = StaticArrayList.FCount;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    bool TKSTATICARRAYLIST::DumpTo(TKSTATICARRAYLIST & StaticArrayList)
    {
        if (this->FCapacity != StaticArrayList.FCapacity)
        {
            return false;
        }

        memcpy(&StaticArrayList, this, sizeof(TKSTATICARRAYLIST));

        return true;
    }

    template <typename OBJECTTYPE, int ReservedCapacity>
    void TKSTATICARRAYLIST::QuickSort(int L, int R, int (*Compare)(OBJECTTYPE *, OBJECTTYPE *))
    {
        int I, J;
        int P, T;

        do
        {
            I = L;
            J = R;
            P = (L + R) >> 1 ;   // 1비트 우측시프트 연산한다. 반으로 나누는것과 동일하다.

            do
            {
                while (Compare( &(Nodes[Indexes[I]].Object), &(Nodes[Indexes[P]].Object )) < 0)
                {
                    I++;
                }
                while (Compare( &(Nodes[Indexes[J]].Object), &(Nodes[Indexes[P]].Object )) > 0)
                {
                    J--;
                }

                if (I <= J)
                {
                    T = Indexes[I];
                    Indexes[I] = Indexes[J];
                    Indexes[J] = T;
                    I++;
                    J--;
                }

            } while (!(I > J));

            if (L < J)
            {
                QuickSort(L, J, Compare);
            }

            L = I;

        } while (!(I >= R));
    }

}