#include "./TKCamera.h"
#include "./TKSGObject.h"

//////////////////////////////////////////////////////////////////////////////
// TKCustomCamera
//////////////////////////////////////////////////////////////////////////////
template <class FRAMESIDEOBJECT, class RENDERSIDEOBJECT>
TKCustomCamera<FRAMESIDEOBJECT, RENDERSIDEOBJECT>::TKCustomCamera(TKEngine *pEngine, TKEngineObject *pParent, int ID, int Seq)
:TKCamera(pEngine, pParent, ID, Seq)
{
    pFSObject = new FRAMESIDEOBJECT(pEngine, this);
    pRSObject = new RENDERSIDEOBJECT(pEngine, this);
}

template <class FRAMESIDEOBJECT, class RENDERSIDEOBJECT>
TKCustomCamera<FRAMESIDEOBJECT, RENDERSIDEOBJECT>::~TKCustomCamera(void)
{
    SAFEDELETE(pFSObject);
    SAFEDELETE(pRSObject);
}

template <class FRAMESIDEOBJECT, class RENDERSIDEOBJECT>
void TKCustomCamera<FRAMESIDEOBJECT, RENDERSIDEOBJECT>::Init(void)
{
    __super::Init();
    
    BeforeInit();
    if ( pFSObject ) pFSObject->Init();
    if ( pRSObject ) pRSObject->Init();
    AfterInit();    
}

template <class FRAMESIDEOBJECT, class RENDERSIDEOBJECT>
void TKCustomCamera<FRAMESIDEOBJECT, RENDERSIDEOBJECT>::Release(void)
{
    if ( pRSObject ) pRSObject->Release();
    if ( pFSObject ) pFSObject->Release();

	__super::Release();
}