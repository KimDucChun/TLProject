#pragma once

#include "TKBaseObject.h"
#include "TKEngineObjectList.h"

class TKEngineObject;
class TKEngineResource;

////////////////////////////////////////////////////////////////////////////////
// TKRenderSideObject
////////////////////////////////////////////////////////////////////////////////
class TKRenderSideObject : public TKBaseObject
{
    friend class TKEngineObject;
    friend class TKFrameSideObject;
private:
    TKEngineObject  *pParent;

protected:
    bool        IsEnable;
    bool        IsFrame;
    bool        IsRender;

public:
    TKRenderSideObject(TKEngine *pEngine, TKEngineObject *pParent);
    virtual ~TKRenderSideObject(void);

    TKEngineObject * GetParent(void){ return pParent; }
    template <class ENGINEOBJECT> ENGINEOBJECT * GetParent(void){ return (ENGINEOBJECT*)pParent; }
    
    virtual void BeforeRender(const ERenderFuncType RenderType) = 0;
    virtual void Render(const ERenderFuncType RenderType) = 0;
    virtual void AfterRender(const ERenderFuncType RenderType) = 0;

    virtual bool GetIsRender(void)          { return IsRender; }
    virtual bool GetIsEnable(void)          { return IsEnable; }
    virtual bool GetIsFrame(void)           { return IsFrame; }
    
    virtual void SetIsRender(bool IsRender) { this->IsRender = IsRender; }
    virtual void SetIsEnable(bool IsEnable) { this->IsEnable = IsEnable; }
    virtual void SetIsFrame(bool IsFrame)   { this->IsFrame = IsFrame; }    
    
    virtual float GetCameraDistance(void)   { return 0.0f; }
    virtual void SetCameraDistance(float Distance) {;}    

	virtual bool GetAlphaBlend(void)				{ return false; }
	virtual void SetAlphaBlend(bool AlphaBlend)		{  }
};

////////////////////////////////////////////////////////////////////////////////
// TKFrameSideObject
////////////////////////////////////////////////////////////////////////////////
class TKFrameSideObject : public TKBaseObject
{
    friend class TKEngineObject;
private:
    TKEngineObject *pParent;
    
protected:    
    bool        IsEnable;
    bool        IsFrame;
    bool        IsRender;
    
    void        DataSyncFlag( TKRenderSideObject *pRSObject )
    {
        // 멤버변수 위치에 극히 유의한다.
        //memcpy(&pRSObject->IsEnable, &this->IsEnable, sizeof(bool) * 3); 

		// 그냥 원래대로.
		pRSObject->IsEnable	= this->IsEnable;
		pRSObject->IsFrame	= this->IsFrame;
		pRSObject->IsRender	= this->IsRender;
    }

public:
    TKFrameSideObject(TKEngine *pEngine, TKEngineObject *pParent);
    virtual ~TKFrameSideObject(void);
    
    virtual void DataSync( TKRenderSideObject *pRSObject ) = 0;

    TKEngineObject * GetParent(void){ return pParent; }
    template <class ENGINEOBJECT> ENGINEOBJECT * GetParent(void){ return (ENGINEOBJECT*)pParent; }

    virtual void BeforeFrame(void) = 0;
    virtual void AfterFrame(void) = 0;

    virtual bool GetIsRender(void)          { return IsRender; }
    virtual bool GetIsEnable(void)          { return IsEnable; }
    virtual bool GetIsFrame(void)           { return IsFrame; }

    virtual void SetIsRender(bool IsRender) { this->IsRender = IsRender; }
    virtual void SetIsEnable(bool IsEnable) { this->IsEnable = IsEnable; }
    virtual void SetIsFrame(bool IsFrame)   { this->IsFrame = IsFrame; }
    
    virtual float GetCameraDistance(void)   { return 0.0f; }
    virtual void SetCameraDistance(float Distance) {;}    
};

////////////////////////////////////////////////////////////////////////////////
// TKEngineObject
////////////////////////////////////////////////////////////////////////////////
class TKEngineObject : public TKBaseObject
{
    friend class TKEngineObjectList;
private:
    void Frame(void) {;}        // 사용안함
    
    int     Seq;
    TKEngineObject      *pParent;
    
public:
    TKEngineObject( TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq );
    virtual ~TKEngineObject(void);

    int GetSeq(void) { return Seq; }

    virtual TKEngineObject * GetParent(void) { return pParent; }
    template <class ENGINEOBJECT> ENGINEOBJECT * GetParent(void) { return (ENGINEOBJECT*)pParent; }
    void SetParent(TKEngineObject * pParent) { this->pParent = pParent; }
    
    void DataSync(void) // 이 함수는 하위클래스에서 재정의 하지 말것!
    {
        if ( GetFSRaw() ) 
            GetFSRaw()->DataSync( GetRSRaw() );    
        if ( GetEngineObjectList() )
            GetEngineObjectList()->DataSync();
    }

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
    
    virtual TKEngineObjectList * GetEngineObjectList(void) = 0;

    virtual TKFrameSideObject * GetFSRaw(void) = 0;
    virtual TKRenderSideObject * GetRSRaw(void) = 0;
    
    virtual void SetRes(TKEngineResource * pRes) = 0;
    virtual TKEngineResource * GetRes(void) = 0;

    virtual void BeforeInit(void) = 0;
    virtual void AfterInit(void) = 0;
};
