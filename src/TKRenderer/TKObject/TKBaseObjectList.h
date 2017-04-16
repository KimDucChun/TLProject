#pragma once

#include "TKBaseObject.h"

class TKBaseObjectList : public TKBaseObject
{
private:
    friend class TKEngine;

protected:
    vector<TKBaseObject*>           NewObjects;

    unordered_map<int, TKBaseObject*>    Objects_ID;
    unordered_map<string, TKBaseObject*> Objects_Name;
    vector<TKBaseObject*>           Objects_Array;

    vector<TKBaseObject*>           EraseList;
    vector<TKBaseObject*>           DelList;
    
    TKCriticalSection   CS;

    bool OwnerContainer;

    static bool CompareObjectID(TKBaseObject *pA, TKBaseObject *pB);
    
protected:
    vector<TKBaseObject*>::iterator itNew;
    unordered_map<int, TKBaseObject*>::iterator itID;
    unordered_map<string, TKBaseObject*>::iterator itName;
    vector<TKBaseObject*>::iterator itArray;
    vector<TKBaseObject*>::iterator itDel;
    vector<TKBaseObject*>::iterator itErase;
    
    pair<unordered_map<int, TKBaseObject*>::iterator, bool> Rtn_ID;
    pair<unordered_map<string, TKBaseObject*>::iterator, bool> Rtn_Name;
    
public:
    TKBaseObjectList(bool OwnerContainer);
    virtual ~TKBaseObjectList(void);

    // ��ü ���� �Լ�(OwnerContainer ���� true �� ��츸 ��밡��)
    // ���ϰ� : ������ ��ü ������
    template <typename BASEOBJECT> BASEOBJECT * New(void);            

    // ��ü ���� �Լ�(OwnerContainer ���� true �� ��츸 ��밡��)
    // ID : ��ü ID
    // pNodeName : ��ü ��
    // ���ϰ� : ������ ��ü ������
    template <typename BASEOBJECT> BASEOBJECT * New(int ID, string NodeName);

    // ��ü ���� �Լ�(OwnerContainer ���� true �� ��츸 ��밡��)
    // pNodeName : ��ü ��
    // ���ϰ� : ������ ��ü ������
    template <typename BASEOBJECT> BASEOBJECT * New(string NodeName);

    // ��ü �߰� �Լ�(OwnerContainer ���� false �� ��츸 ��밡��)
    // pObject : �߰��� ��ü�� ������
    // ���ϰ� : �߰� ����/����
    template <typename BASEOBJECT> bool AddObject(BASEOBJECT *pObject);

    bool Delete(int ID);
    inline bool Delete(TKBaseObject *pObject);
    bool Delete(char *pNodeName);
    inline int Count(void);

    void Init(void);
    void Frame(void);
    void Release(void);

    // �߰��� ��ü ����
    void ApplyNew(void);
    // ������ ��ü �޸𸮿��� ����
    void ApplyDelete(void);
    // ������ ��ü �����̳ʿ��� ����
    void ApplyErase(bool ClearEraseList = true);

    TKBaseObject * Item(int ID);
    template <typename BASEOBJECT> BASEOBJECT * Item(int ID);

    TKBaseObject * Item(string & NodeName);
    template <typename BASEOBJECT> BASEOBJECT * Item(string & NodeName);

    inline TKBaseObject * ItemBySeq(int Seq);
    template <typename BASEOBJECT> BASEOBJECT * ItemBySeq(int Seq);

    inline TKBaseObject * operator [] (int Index);
    const TKBaseObjectList & operator = ( const TKBaseObjectList & BaseObjectList );

    void ClearDelete(void);

    void SetOwnerContainer(bool OwnerContainer);
};

///////////////////////////////////////////////////////////////////////////
// ���� ���ø� �Լ�
///////////////////////////////////////////////////////////////////////////
#include "./TKEngine.h"

template <typename BASEOBJECT> BASEOBJECT * TKBaseObjectList::New(void)
{
    BASEOBJECT *pObject = NULL;
    if (!OwnerContainer) return pObject;

    CS.Enter();
    pObject = new BASEOBJECT(GetEngine(), GENID);
    NewObjects.push_back( pObject );
    CS.Leave();
    return pObject;
}

template <typename BASEOBJECT> BASEOBJECT * TKBaseObjectList::New(int ID, string NodeName)
{
    BASEOBJECT *pObject = NULL;
    if (!OwnerContainer) return pObject;

    vector<TKBaseObject*>::iterator itNew;
    unordered_map<int, TKBaseObject*>::iterator itID;
    unordered_map<string, TKBaseObject*>::iterator itName;

    CS.Enter();
    itID = Objects_ID.find( ID );
    itName = Objects_Name.find( NodeName );
    for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
    {
        if ( ((*itNew)->GetNodeName() == NodeName && !NodeName.empty()) || (*itNew)->GetID() == ID )
        {
            break;
        }
    }
    if (itID == Objects_ID.end() && itName == Objects_Name.end() && itNew == NewObjects.end())
    {
        pObject = new BASEOBJECT(pEngine, ID);
        pObject->SetNodeName(NodeName);
        NewObjects.push_back( pObject );
    }
    CS.Leave();

    return pObject;
}

template <typename BASEOBJECT> BASEOBJECT * TKBaseObjectList::New(string NodeName)
{
    BASEOBJECT *pObject = NULL;
    if (!OwnerContainer) return pObject;

    vector<TKBaseObject*>::iterator itNew;
    unordered_map<string, TKBaseObject*>::iterator itName;

    CS.Enter();
    itName = Objects_Name.find( NodeName );
    for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == NodeName )
        {
            break;
        }
    }
    if (itName == Objects_Name.end() && itNew == NewObjects.end())
    {
        pObject = new BASEOBJECT(GetEngine(), GENID);
        pObject->SetNodeName( NodeName );
        NewObjects.push_back( pObject );
    }
    CS.Leave();

    return pObject;
}

template <typename BASEOBJECT> bool TKBaseObjectList::AddObject(BASEOBJECT *pObject)
{
    bool result = false;
    if (OwnerContainer) return result;

    vector<TKBaseObject*>::iterator itNew;
    unordered_map<int, TKBaseObject*>::iterator itID;
    unordered_map<string, TKBaseObject*>::iterator itName;

    CS.Enter();
    itID = Objects_ID.find( pObject->GetID() );
    itName = Objects_Name.find( pObject->GetNodeName() );
    for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
    {
        if ( ( (*itNew)->GetNodeName() == pObject->GetNodeName() && !pObject->GetNodeName().empty() ) ||
               (*itNew)->GetID() == pObject->GetID() )
        {
            break;
        }
    }
    if (itID == Objects_ID.end() && itName == Objects_Name.end() && itNew == NewObjects.end())
    {
        NewObjects.push_back( pObject );
        result = true;
    }
    CS.Leave();

    return result;
}

template <typename BASEOBJECT> BASEOBJECT * TKBaseObjectList::Item(int ID)
{
    return (BASEOBJECT *)this->Item(ID);
}

template <typename BASEOBJECT> BASEOBJECT * TKBaseObjectList::Item(string & NodeName)
{
    return (BASEOBJECT *)this->Item(NodeName);
}

template <typename BASEOBJECT> BASEOBJECT * TKBaseObjectList::ItemBySeq(int Seq)
{
    return (BASEOBJECT *)this->ItemBySeq(Seq);
}

///////////////////////////////////////////////////////////////////////////
// ���� �ζ��� �Լ�
///////////////////////////////////////////////////////////////////////////
TKBaseObject * TKBaseObjectList::ItemBySeq(int Seq)
{
    return Objects_Array[Seq];
}

TKBaseObject * TKBaseObjectList::operator [] (int Seq)
{
    return Objects_Array[Seq];
}

int TKBaseObjectList::Count(void)
{
    return (int)Objects_ID.size();
}

bool TKBaseObjectList::Delete(TKBaseObject *pObject)
{
    return Delete( pObject->GetID() );
}

