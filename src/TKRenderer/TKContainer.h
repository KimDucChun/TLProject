#pragma once

//int Compare(OBJECTTYPE *pA, OBJECTTYPE *pB)
//{
//    if (pA->Value > pB->Value) return 1;
//    else if (pA->Value == pB->Value) return 0;
//    else if (pA->Value < pB->Value) return -1;
//}

#include <ASSERT.h>

namespace KContainer
{
    ///////////////////////////////////////////////////////
    // TKArrayList
    ///////////////////////////////////////////////////////
    template <typename OBJECTTYPE>
    class TKArrayList
    {
    private:
        OBJECTTYPE **pNodes;
        int FCount;
        int FCapacity;
        bool Grow(bool PowerGrowed);
        void QuickSort(int L, int R, int (*Compare)(OBJECTTYPE *, OBJECTTYPE *));
    public:
        TKArrayList(void);
        ~TKArrayList(void);

        int	Count(void);
        OBJECTTYPE * Items(int Index);
        OBJECTTYPE * operator[](int Index);

        void Add(OBJECTTYPE * pObject);
        OBJECTTYPE * AddNew(OBJECTTYPE & AObject);
        void Insert(int Index, OBJECTTYPE *pObject);
        OBJECTTYPE * InsertNew(int Index, OBJECTTYPE & AObject);

        void Erase(int Index);
        void EraseDelete(int Index);
        void Exchange(int Index1, int Index2);
        void Pack(void);
        void Clear(void);
        void ClearDelete(void);

        int Capacity(void);
        void SetCapacity(int NewCapacity);
        int IndexOf(OBJECTTYPE *pObject);
        const OBJECTTYPE * First(void);
        const OBJECTTYPE * Last(void);

        void Sort(int (*Compare)(OBJECTTYPE *, OBJECTTYPE *));
    };

    ///////////////////////////////////////////////////////
    // TKLinkedList
    ///////////////////////////////////////////////////////
    #define TKLINKEDNODE TKLinkedNode<OBJECTTYPE>

    template <class OBJECTTYPE>
    class TKLinkedNode
    {
    private:
        template <class OBJECTTYPE> friend class TKLinkedList;

        TKLINKEDNODE *pPrevNode;
        TKLINKEDNODE *pNextNode;
        OBJECTTYPE *pObject;

        TKLinkedNode(OBJECTTYPE *pObject)
        {
            this->pPrevNode = NULL;
            this->pNextNode = NULL;
            this->pObject   = pObject;
        }
    public:
        TKLINKEDNODE * GetPrevNode(void){ return pPrevNode; }
        TKLINKEDNODE * GetNextNode(void){ return pNextNode; }
        OBJECTTYPE * GetObj(void){ return pObject; }
    };

    template <class OBJECTTYPE>
    class TKLinkedList
    {
    private:
        TKLINKEDNODE *pFirst;
        TKLINKEDNODE *pLast;

        int FCount;
        void QuickSort(OBJECTTYPE **pNodes, int L, int R, int (*Compare)(OBJECTTYPE *, OBJECTTYPE *));

    public:
        TKLinkedList(void);
        ~TKLinkedList(void);

        int	Count(void);

        TKLINKEDNODE * Add(OBJECTTYPE * pObject);
        TKLINKEDNODE * AddNew(OBJECTTYPE & AObject);
        TKLINKEDNODE * Insert(TKLINKEDNODE *pNode, OBJECTTYPE *pObject);
        TKLINKEDNODE * InsertNew(TKLINKEDNODE *pNode, OBJECTTYPE & AObject);
        TKLINKEDNODE * FindNode(OBJECTTYPE * pObject);

        void Erase(TKLINKEDNODE *pNode);
        void EraseDelete(TKLINKEDNODE *pNode);
        void Exchange(TKLINKEDNODE *pNode1, TKLINKEDNODE *pNode2);
        void Clear(void);
        void ClearDelete(void);

        TKLINKEDNODE * First(void);
        TKLINKEDNODE * Last(void);

        void Sort(int (*Compare)(OBJECTTYPE *, OBJECTTYPE *));
    };

    ///////////////////////////////////////////////////////
    // TKMap
    ///////////////////////////////////////////////////////
    #define TKMAPNODE TKMapNode<INDEXTYPE, OBJECTTYPE>

    template <typename INDEXTYPE, typename OBJECTTYPE>
    class TKMapNode
    {
    private:
        template <typename INDEXTYPE, typename OBJECTTYPE> friend class TKMap;

        INDEXTYPE  Index;
        OBJECTTYPE *pObject;

        TKMapNode(void)
        {
            memset(&Index, 0, sizeof(INDEXTYPE));
            pObject = NULL;
        }
        TKMapNode(INDEXTYPE Index, OBJECTTYPE *pObject)
        {
            this->Index = Index;
            this->pObject = pObject;
        }
    public:
        INDEXTYPE GetIndex(void){ return Index; }
        OBJECTTYPE * GetObj(void){ return pObject; }
    };

    template <typename INDEXTYPE, typename OBJECTTYPE>
    class TKMap
    {
    private:
        TKMAPNODE **pNodes;
        int FCount;
        int FCapacity;
        bool Grow(bool PowerGrowed);
        void QuickSort(int L, int R);
        int FindNodeIndex(int L, int R, INDEXTYPE Index);
    public:
        TKMap(void);
        ~TKMap(void);

        int	Count(void);
        TKMAPNODE * Items(INDEXTYPE Index);
        TKMAPNODE * operator[](INDEXTYPE Index);

        TKMAPNODE * Add(INDEXTYPE Index, OBJECTTYPE * pObject);
        TKMAPNODE * AddNew(INDEXTYPE Index, OBJECTTYPE & AObject);

        void Erase(INDEXTYPE Index);
        void EraseDelete(INDEXTYPE Index);
        void Pack(void);
        void Clear(void);
        void ClearDelete(void);

        int Capacity(void);
        TKMAPNODE * IndexOf(OBJECTTYPE *pObject);
        const TKMAPNODE * First(void);
        const TKMAPNODE * Last(void);
    };

    ///////////////////////////////////////////////////////
    // TKMultiMap
    ///////////////////////////////////////////////////////
    #define TKMULTIMAPNODE TKMultiMapNode<INDEXTYPE, OBJECTTYPE>

    template <typename INDEXTYPE, typename OBJECTTYPE>
    class TKMultiMapNode
    {
    private:
        template <typename INDEXTYPE, typename OBJECTTYPE> friend class TKMultiMap;

        INDEXTYPE  Index;
        OBJECTTYPE *pObject;

        TKMULTIMAPNODE(void)
        {
            memset(&Index, 0, sizeof(INDEXTYPE));
            pObject = NULL;
        }
        TKMULTIMAPNODE(INDEXTYPE Index, OBJECTTYPE *pObject)
        {
            this->Index = Index;
            this->pObject = pObject;
        }
    public:
        INDEXTYPE GetIndex(void){ return Index; }
        OBJECTTYPE * GetObj(void){ return pObject; }
    };

    template <typename INDEXTYPE, typename OBJECTTYPE>
    class TKMultiMap
    {
    private:
        TKMULTIMAPNODE **pNodes;
        int FCount;
        int FCapacity;
        bool Grow(bool PowerGrowed);
        void QuickSort(int L, int R);
        int FindNodeIndex(int L, int R, INDEXTYPE Index);
    public:
        TKMultiMap(void);
        ~TKMultiMap(void);

        int	Count(void);
        TKMULTIMAPNODE * Items(INDEXTYPE Index);
        TKMULTIMAPNODE * operator[](INDEXTYPE Index);

        TKMULTIMAPNODE * Add(INDEXTYPE Index, OBJECTTYPE * pObject);
        TKMULTIMAPNODE * AddNew(INDEXTYPE Index, OBJECTTYPE & AObject);

        void Erase(INDEXTYPE Index);
        void EraseDelete(INDEXTYPE Index);
        void Pack(void);
        void Clear(void);
        void ClearDelete(void);

        int Capacity(void);
        TKMULTIMAPNODE * IndexOf(OBJECTTYPE *pObject);
        const TKMULTIMAPNODE * First(void);
        const TKMULTIMAPNODE * Last(void);
    };

    ///////////////////////////////////////////////////////
    // TKStaticArrayList
    ///////////////////////////////////////////////////////
    #define TKSTATICARRAYLIST TKStaticArrayList<OBJECTTYPE, ReservedCapacity>

    template <typename OBJECTTYPE, int ReservedCapacity>
    class TKStaticArrayList
    {
    private:
        class TKStaticArrayNode
        {
            friend class TKStaticArrayList;
            OBJECTTYPE  Object;
            bool        Used;
            TKStaticArrayNode(void)
            {
                //memset(&Object, 0, sizeof(OBJECTTYPE));
                Used = false;
            }
            TKStaticArrayNode(OBJECTTYPE Object)
            {
                this->Object = Object;
                Used = true;
            }
            bool operator == (const TKStaticArrayNode & StaticArrayNode)
            {
                return
                    this->Object = StaticArrayNode.Object &&
                    this->Used = StaticArrayNode.Used;
            }
            bool operator != (const TKStaticArrayNode & StaticArrayNode)
            {
                return !(&this == StaticArrayNode);
            }
            void operator = (const TKStaticArrayNode & StaticArrayNode)
            {
                this->Object = StaticArrayNode.Object;
                this->Used = StaticArrayNode.Used;
            }
        };
        TKStaticArrayNode Nodes[ReservedCapacity];
        int Indexes[ReservedCapacity];
        int FCount;
        int FCapacity;

        void QuickSort(int L, int R, int (*Compare)(OBJECTTYPE *, OBJECTTYPE *));
        OBJECTTYPE * GetNode(int Index);

    public:
        TKStaticArrayList(void);
        TKStaticArrayList(const TKStaticArrayList & StaticArrayList);
        ~TKStaticArrayList(void);

        int	Count(void);
        OBJECTTYPE * Items(int Index);
        OBJECTTYPE * operator[](int Index);

        OBJECTTYPE * New(void);
        OBJECTTYPE * New(OBJECTTYPE & AObject);
        bool Delete(int Index);
        bool Delete(OBJECTTYPE *pObject);
        void Exchange(int Index1, int Index2);
        void Clear(void);

        int Capacity(void);
        int IndexOf(OBJECTTYPE *pObject);
        int IndexOf(OBJECTTYPE & AObject);
        const OBJECTTYPE * First(void);
        const OBJECTTYPE * Last(void);
                
        OBJECTTYPE * GetNodeArrayIndex(int Index);

        void Sort(int (*Compare)(OBJECTTYPE *, OBJECTTYPE *));

        bool operator == (const TKStaticArrayList & StaticArrayList);
        bool operator != (const TKStaticArrayList & StaticArrayList);
        void operator = (const TKStaticArrayList & StaticArrayList);

        bool DumpTo(TKStaticArrayList & StaticArrayList);
    };
}

#include "./TKContainer.hpp"
