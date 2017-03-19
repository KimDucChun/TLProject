#include "TKBaseObject.h"

TKBaseObject::TKBaseObject(void)
    :m_ID(-1)
    ,m_Deleted(false)
    ,m_NodeName("")
    ,m_Released(false)
{
    SetObjectType( eotBaseObject );
}

TKBaseObject::TKBaseObject(int ID)
    :m_ID(ID)
    ,m_Deleted(false)
    ,m_NodeName("")
    ,m_Released(false)
{
    SetObjectType( eotBaseObject );
}

TKBaseObject::~TKBaseObject(void)
{
}

void TKBaseObject::Init(void)
{
    m_Released = false;
}

void TKBaseObject::Frame(void)
{
}

void TKBaseObject::Release(void)
{      
    m_Released = true;
}

bool TKBaseObject::GetReleased(void)
{
    return m_Released;
}

int TKBaseObject::GetID(void)
{
    return m_ID;
}

void TKBaseObject::SetID(int ID) 
{
    m_ID = ID;
}

string & TKBaseObject::GetNodeName(void)
{ 
    return m_NodeName;
}

void TKBaseObject::SetNodeName(string & NodeName)
{
    m_NodeName = NodeName;
}

bool TKBaseObject::GetDeleted(void)
{
    return m_Deleted;
}

void TKBaseObject::SetDeleted(bool Deleted) 
{
    m_Deleted = Deleted;
}

const TKBaseObject & TKBaseObject::operator = (const TKBaseObject & BaseObject)
{
    m_ID        = BaseObject.m_ID;
    m_NodeName  = BaseObject.m_NodeName;
    m_Deleted   = BaseObject.m_Deleted;

    return BaseObject;
}