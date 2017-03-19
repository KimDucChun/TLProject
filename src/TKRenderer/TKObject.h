#pragma once

#include "TKCommon.h"
#include "TKInterface.h"

class TKObject : public ITKObject
{
private:
    ETKObjectType m_ObjectType;
protected:
    inline void SetObjectType( ETKObjectType ObjectType ) { m_ObjectType = ObjectType; }
public:
    TKObject(void);
    virtual ~TKObject(void);

    inline ETKObjectType GetObjectType(void) { return m_ObjectType; }
};
