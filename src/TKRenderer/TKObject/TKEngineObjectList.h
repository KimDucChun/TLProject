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
    TKEngineObject*								m_pOwner;
    
    vector<TKEngineObject*>						m_NewEngineObjects;

    unordered_map<int, TKEngineObject*>			m_EngineObjects_ID;
    unordered_map<string, TKEngineObject*>		m_EngineObjects_Name;

    vector<TKEngineObject*>						m_EraseList;
    vector<TKEngineObject*>						m_DelList;

    TKCriticalSection							CSE;

    bool										m_OwnerContainer;

    static bool CompareObjectSeq(TKEngineObject *pA, TKEngineObject *pB);

protected:
    vector<TKEngineObject*>::iterator itNew;
    vector<TKEngineObject*>::iterator itDel;
    vector<TKEngineObject*>::iterator itErase;
    
    unordered_map<int, TKEngineObject*>::iterator itID;
    unordered_map<string, TKEngineObject*>::iterator itName;
    vector<TKRenderSideObject*>::iterator itArray;
    
    pair<unordered_map<int, TKEngineObject*>::iterator, bool> Rtn_ID;
    pair<unordered_map<string, TKEngineObject*>::iterator, bool> Rtn_Name;

public:
    TKEngineObjectList(TKEngineObject *pOwner, bool OwnerContainer);
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

    TKEngineObject * operator [] (int Seq);

    void ClearDelete(void);
    
    void SetOwnerContainer(bool OwnerContainer) { this->m_OwnerContainer = OwnerContainer; }
    
    TKEngineObject * GetOwner(void)             { return m_pOwner; }
    void SetOwner(TKEngineObject * pOwner)      { this->m_pOwner = pOwner; }
};

///////////

#include "./TKEngineObject.h"

template <class ENGINEOBJECT> ENGINEOBJECT * TKEngineObjectList::New(int Seq)
{
    ENGINEOBJECT *pObject = NULL;
    CSE.Enter();
    pObject = new ENGINEOBJECT(GetEngine(), m_pOwner, GENID, Seq);
    m_NewEngineObjects.push_back( pObject );
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

    itID = m_EngineObjects_ID.find( ID );
    itName = m_EngineObjects_Name.find( NodeName );
    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    {
        if ( ((*itNew)->GetNodeName() == NodeName && !NodeName.empty()) || (*itNew)->GetID() == ID )
        {
            break;
        }
    }

    if ( itID == m_EngineObjects_ID.end() && itName == m_EngineObjects_Name.end() && itNew == m_NewEngineObjects.end() )
    {
        pObject = new ENGINEOBJECT(GetEngine(), m_pOwner, GENID, Seq);
        pObject->SetNodeName( NodeName );
        m_NewEngineObjects.push_back( pObject );
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

    itName = m_EngineObjects_Name.find( NodeName );
    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == NodeName )
        {
            break;
        }
    }

    if ( itName == m_EngineObjects_Name.end() && itNew == m_NewEngineObjects.end() )
    {
        pObject = new ENGINEOBJECT(GetEngine(), m_pOwner, GENID, Seq);
        pObject->SetNodeName( NodeName );
        m_NewEngineObjects.push_back( pObject );
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