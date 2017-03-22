#pragma once

#include "TKBaseObject.h"

class TKEngineObject;
class TKFrameSideObject;
class TKRenderSideObject;

class TKEngineObjectList : public TKBaseObject
{
private:
    friend class TKEngine;
    
protected:
    TKEngineObject                 *pOwner;
    
    vector<TKEngineObject*>         NewEngineObjects;

    unordered_map<int, TKEngineObject*>       EngineObjects_ID;
    unordered_map<string, TKEngineObject*>    EngineObjects_Name;

    unordered_map<int, TKFrameSideObject*>    FrameSideObjects_ID;
    unordered_map<int, TKRenderSideObject*>   RenderSideObjects_ID;
    vector<TKRenderSideObject*>     RenderSideObjects_Array;

    vector<TKEngineObject*>         EraseList;
    vector<TKEngineObject*>         DelList;

    TKCriticalSection   CSE;
    TKCriticalSection   CSF;
    TKCriticalSection   CSR;

    bool OwnerContainer;

    static bool CompareObjectSeq(TKRenderSideObject *pA, TKRenderSideObject *pB);

protected:
    vector<TKEngineObject*>::iterator itNew;
    vector<TKEngineObject*>::iterator itDel;
    vector<TKEngineObject*>::iterator itErase;
    
    unordered_map<int, TKEngineObject*>::iterator itID;
    unordered_map<string, TKEngineObject*>::iterator itName;
    unordered_map<int, TKFrameSideObject*>::iterator itFrame;
    unordered_map<int, TKRenderSideObject*>::iterator itRender;
    vector<TKRenderSideObject*>::iterator itArray;
    
    pair<unordered_map<int, TKEngineObject*>::iterator, bool> Rtn_ID;
    pair<unordered_map<string, TKEngineObject*>::iterator, bool> Rtn_Name;
    pair<unordered_map<int, TKFrameSideObject*>::iterator, bool> Rtn_FS;
    pair<unordered_map<int, TKRenderSideObject*>::iterator, bool> Rtn_RS;

public:
    TKEngineObjectList( TKEngine *pEngine, TKEngineObject *pOwner, bool OwnerContainer );
    virtual ~TKEngineObjectList(void);

    // ��ü ���� �Լ�(OwnerContainer ���� true �� ��츸 ��밡��)
    // Seq : ������ ����
    // ���ϰ� : ������ ��ü ������
    template <class ENGINEOBJECT> ENGINEOBJECT * New(int Seq);

    // ��ü ���� �Լ�(OwnerContainer ���� true �� ��츸 ��밡��)
    // Seq : ������ ����
    // ID : ��ü ID
    // pNodeName : ��ü ��
    // ���ϰ� : ������ ��ü ������
    template <class ENGINEOBJECT> ENGINEOBJECT * New(int Seq, int ID, string & NodeName);

    // ��ü ���� �Լ�(OwnerContainer ���� true �� ��츸 ��밡��)
    // Seq : ������ ����
    // pNodeName : ��ü ��
    // ���ϰ� : ������ ��ü ������
    template <class ENGINEOBJECT> ENGINEOBJECT * New(int Seq, string & NodeName);

    // ��ü �߰� �Լ�(OwnerContainer ���� false �� ��츸 ��밡��)
    // pObject : �߰��� ��ü�� ������
    // ���ϰ� : �߰� ����/����
    bool Add(TKEngineObject *pObject);
    
    // ��ü�� �����Ѵ� (OwnerContainer==true �ΰ�� �ν��Ͻ��� �ı��Ѵ�.)
    bool Delete(int ID);
    bool Delete(TKEngineObject *pEngineObject);
    
    // ��ü�� �߰��Ѵ� (OwnerContainer�� ���� ������� �߰��Ѵ�.)
    bool AddOnList(TKEngineObject *pObject);

    // ��ü�� �����Ѵ� (OwnerContainer�� ���� ������� ����Ʈ������ �����Ѵ�.)
    bool RemoveOnList(int ID);
    bool RemoveOnList(TKEngineObject *pEngineObject);
    
    // ��ϵǾ��ִ� ��ü�� ���ڸ� ��ȯ�Ѵ�(ApplyNew �Ǳ� ���� NewObjects�� ���Ե��� �ʴ´�.)
    int Count(void);

    void DataSync(void);

    void Init(void);
    void Frame(void);
    void Render(const ERenderFuncType RenderType);
    void Release(void);

    // �߰��� ��ü ����
    void ApplyNew(void);
    // ������ ��ü �޸𸮿��� ����
    virtual void ApplyDelete(void);
    // ������ ��ü �����̳ʿ��� ����
    virtual void ApplyErase(bool ClearEraseList = true);

    TKEngineObject * Item(int ID);
    template <typename ENGINEOBJECT> ENGINEOBJECT * Item(int ID);

    TKEngineObject * Item(string & NodeName);
    template <typename ENGINEOBJECT> ENGINEOBJECT * Item(string & NodeName);
    
    TKEngineObject * ItemBySeq(int Seq);
    template <typename ENGINEOBJECT> ENGINEOBJECT * ItemBySeq(int Seq);

    TKFrameSideObject * FSObject(int ID);
    template <typename FSOBJECT> FSOBJECT * FSObject(int ID);

    TKRenderSideObject * RSObject(int ID);
    template <typename RSOBJECT> RSOBJECT * RSObject(int ID);

    TKEngineObject * operator [] (int Seq);

    void ClearDelete(void);
    
    void SetOwnerContainer(bool OwnerContainer) { this->OwnerContainer = OwnerContainer; }
    
    TKEngineObject * GetOwner(void)             { return pOwner; }
    void SetOwner(TKEngineObject * pOwner)      { this->pOwner = pOwner; }
};

///////////

#include "./TKEngineObject.h"

template <class ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::New(int Seq)
{
    ENGINEOBJECT *pObject = NULL;
    CSE.Enter();
    pObject = new ENGINEOBJECT(GetEngine(), pOwner, GENID, Seq);
    NewEngineObjects.push_back( pObject );
    CSE.Leave();
    return pObject;   
}

template <class ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::New(int Seq, int ID, string & NodeName)
{
    ENGINEOBJECT *pObject = NULL;
    vector<TKEngineObject*>::iterator itNew;
    unordered_map<int, TKEngineObject*>::iterator itID;
    unordered_map<string, TKEngineObject*>::iterator itName;

    CSE.Enter();

    itID = EngineObjects_ID.find( ID );
    itName = EngineObjects_Name.find( NodeName );
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ( ((*itNew)->GetNodeName() == NodeName && !NodeName.empty()) || (*itNew)->GetID() == ID )
        {
            break;
        }
    }

    if ( itID == EngineObjects_ID.end() && itName == EngineObjects_Name.end() && itNew == NewEngineObjects.end() )
    {
        pObject = new ENGINEOBJECT(GetEngine(), pOwner, GENID, Seq);
        pObject->SetNodeName( NodeName );
        NewEngineObjects.push_back( pObject );
    }

    CSE.Leave();

    return pObject;   
}

template <class ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::New(int Seq, string & NodeName)
{
    ENGINEOBJECT *pObject = NULL;
    vector<TKEngineObject*>::iterator itNew;
    unordered_map<string, TKEngineObject*>::iterator itName;

    CSE.Enter();

    itName = EngineObjects_Name.find( NodeName );
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == NodeName )
        {
            break;
        }
    }

    if ( itName == EngineObjects_Name.end() && itNew == NewEngineObjects.end() )
    {
        pObject = new ENGINEOBJECT(GetEngine(), pOwner, GENID, Seq);
        pObject->SetNodeName( NodeName );
        NewEngineObjects.push_back( pObject );
    }

    CSE.Leave();

    return pObject;   
}

template <typename ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::Item(int ID)
{
    return (ENGINEOBJECT *)this->Item(ID);
}

template <typename ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::Item(string & NodeName)
{
    return (ENGINEOBJECT *)this->Item(NodeName);
}

template <typename ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::ItemBySeq(int Seq)
{
    return (ENGINEOBJECT *)this->ItemBySeq(Seq);
}

template <typename FSOBJECT> FSOBJECT * TKEngineObjectList::FSObject(int ID)
{
    return (FSOBJECT *)this->FSObject(ID);
}

template <typename RSOBJECT> RSOBJECT * TKEngineObjectList::RSObject(int ID)
{
    return (RSOBJECT *)this->RSObject(ID);
}