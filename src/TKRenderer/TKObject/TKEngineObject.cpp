#include "./TKEngineObject.h"
#include "./TKEngineObjectList.h"

////////////////////////////////////////////////////////////////////////////////
// TKEngineObject
////////////////////////////////////////////////////////////////////////////////
TKEngineObject::TKEngineObject(TKEngineObject *pParent, int ID, int Seq)
	: TKBaseObject(ID)
	, m_pParent(pParent)
	, m_Seq(Seq)
	, m_IsEnable(true)
	, m_IsRender(true)
	, m_IsFrame(true)
{
}

TKEngineObject::~TKEngineObject(void)
{
}