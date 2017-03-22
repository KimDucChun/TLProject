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

    // 객체 생성 함수(OwnerContainer 값이 true 일 경우만 사용가능)
    // Seq : 렌더링 순서
    // 리턴값 : 생성된 객체 포인터
    template <class ENGINEOBJECT> ENGINEOBJECT * New(int Seq);

    // 객체 생성 함수(OwnerContainer 값이 true 일 경우만 사용가능)
    // Seq : 렌더링 순서
    // ID : 객체 ID
    // pNodeName : 객체 명
    // 리턴값 : 생성된 객체 포인터
    template <class ENGINEOBJECT> ENGINEOBJECT * New(int Seq, int ID, string & NodeName);

    // 객체 생성 함수(OwnerContainer 값이 true 일 경우만 사용가능)
    // Seq : 렌더링 순서
    // pNodeName : 객체 명
    // 리턴값 : 생성된 객체 포인터
    template <class ENGINEOBJECT> ENGINEOBJECT * New(int Seq, string & NodeName);

    // 객체 추가 함수(OwnerContainer 값이 false 일 경우만 사용가능)
    // pObject : 추가될 객체의 포인터
    // 리턴값 : 추가 성공/실패
    bool Add(TKEngineObject *pObject);
    
    // 객체를 삭제한다 (OwnerContainer==true 인경우 인스턴스도 파괴한다.)
    bool Delete(int ID);
    bool Delete(TKEngineObject *pEngineObject);
    
    // 객체를 추가한다 (OwnerContainer의 값과 상관없이 추가한다.)
    bool AddOnList(TKEngineObject *pObject);

    // 객체를 삭제한다 (OwnerContainer의 값과 상관없이 리스트에서만 삭제한다.)
    bool RemoveOnList(int ID);
    bool RemoveOnList(TKEngineObject *pEngineObject);
    
    // 등록되어있는 객체의 숫자를 반환한다(ApplyNew 되기 전의 NewObjects는 포함되지 않는다.)
    int Count(void);

    void DataSync(void);

    void Init(void);
    void Frame(void);
    void Render(const ERenderFuncType RenderType);
    void Release(void);

    // 추가된 객체 갱신
    void ApplyNew(void);
    // 삭제된 객체 메모리에서 해제
    virtual void ApplyDelete(void);
    // 삭제된 객체 컨테이너에서 제거
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