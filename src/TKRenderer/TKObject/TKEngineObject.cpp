#include "./TKEngineObject.h"
#include "./TKEngineObjectList.h"

////////////////////////////////////////////////////////////////////////////////
// TKRenderSideObject
////////////////////////////////////////////////////////////////////////////////
TKRenderSideObject::TKRenderSideObject(TKEngine *pEngine, TKEngineObject *pParent)
:TKBaseObject(pEngine, pParent->GetID())
,pParent(pParent)
,IsEnable(true)
,IsRender(true)
,IsFrame(true)
{
}

TKRenderSideObject::~TKRenderSideObject(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// TKFrameSideObject
////////////////////////////////////////////////////////////////////////////////
TKFrameSideObject::TKFrameSideObject(TKEngine *pEngine, TKEngineObject *pParent)
:TKBaseObject(pEngine, pParent->GetID())
,pParent(pParent)
,IsEnable(true)
,IsRender(true)
,IsFrame(true)
{
}

TKFrameSideObject::~TKFrameSideObject(void)
{
}

////////////////////////////////////////////////////////////////////////////////
// TKEngineObject
////////////////////////////////////////////////////////////////////////////////
TKEngineObject::TKEngineObject(TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq)
:TKBaseObject(pEngine, ID)
,pParent(pParent)
,Seq(Seq)
{
}

TKEngineObject::~TKEngineObject(void)
{
}