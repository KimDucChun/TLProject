#pragma once

#include "../TKObject.h"
#include "./TKThread.h"

class TKBaseObject : public TKObject
{
private:
    int         m_ID;
    string      m_NodeName;
    bool        m_Deleted;
    bool        m_Released;
    
public:
    TKBaseObject(void);
    TKBaseObject(int ID);
    virtual ~TKBaseObject(void);
    
    virtual void Init(void);
    virtual void Frame(void);
    virtual void Release(void);    

    bool GetReleased(void);

    int GetID(void);
    void SetID(int ID);
    
    string & GetNodeName(void);
    void SetNodeName(string & NodeName);
    
    bool GetDeleted(void);
    void SetDeleted(bool Deleted);
    
    virtual const TKBaseObject & operator = (const TKBaseObject & BaseObject);
};