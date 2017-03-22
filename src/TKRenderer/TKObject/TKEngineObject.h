#pragma once

#include "TKBaseObject.h"
#include "TKEngineObjectList.h"

class TKEngineObject;
class TKEngineResource;

////////////////////////////////////////////////////////////////////////////////
// TKEngineObject
////////////////////////////////////////////////////////////////////////////////
class TKEngineObject : public TKBaseObject
{
    friend class TKEngineObjectList;
private:
    void Frame(void) {;}        // 사용안함
    
    int					m_Seq;
    TKEngineObject      *m_pParent;

	bool				m_IsEnable;
	bool				m_IsFrame;
	bool				m_IsRender;

    
public:
    TKEngineObject( TKEngineObject *pParent, int ID, int Seq );
    virtual ~TKEngineObject(void);

    int GetSeq(void) { return m_Seq; }

	TKEngineObject * GetParent(void)			{ return m_pParent; }
    void SetParent(TKEngineObject * pParent)	{ this->m_pParent = pParent; }
    
    void ApplyNew(void)
    {
        if ( GetEngineObjectList() ) 
            GetEngineObjectList()->ApplyNew();
    }

    void ApplyDelete(void)
    {   
        if ( GetEngineObjectList() ) 
            GetEngineObjectList()->ApplyDelete();
    }

	virtual bool GetIsRender(void) { return m_IsRender; }
	virtual bool GetIsEnable(void) { return m_IsEnable; }
	virtual bool GetIsFrame(void) { return m_IsFrame; }

	virtual void SetIsRender(bool IsRender) { this->m_IsRender = IsRender; }
	virtual void SetIsEnable(bool IsEnable) { this->m_IsEnable = IsEnable; }
	virtual void SetIsFrame(bool IsFrame) { this->m_IsFrame = IsFrame; }

	virtual float GetCameraDistance(void) { return 0.0f; }
	virtual void SetCameraDistance(float Distance) { ; }
    
    virtual TKEngineObjectList * GetEngineObjectList(void) = 0;

    virtual void SetRes(TKEngineResource * pRes) = 0;
    virtual TKEngineResource * GetRes(void) = 0;

    virtual void BeforeInit(void) = 0;
    virtual void AfterInit(void) = 0;
};
