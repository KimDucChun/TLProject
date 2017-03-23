#include "./TKEngineObjectList.h"
#include "./TKEngineObject.h"

TKEngineObjectList::TKEngineObjectList(TKEngineObject *pOwner, bool OwnerContainer)
	:TKBaseObject()
	,m_pOwner(pOwner)
	,m_OwnerContainer(OwnerContainer)
{
    this->m_EraseList.clear();
    this->ClearDelete();
}

TKEngineObjectList::~TKEngineObjectList(void)
{
    if (m_OwnerContainer)
    {
        this->ClearDelete();
    }
}

bool TKEngineObjectList::Add(TKEngineObject *pObject)
{
    bool result  = false;

    if (m_OwnerContainer) return result;

    CSE.Enter();

    itID = m_EngineObjects_ID.find( pObject->GetID() );
    itName = m_EngineObjects_Name.find( pObject->GetNodeName() );
    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == pObject->GetNodeName() || (*itNew)->GetID() == pObject->GetID() )
        {
            break;
        }
    }

    if ( itID == m_EngineObjects_ID.end() && itName == m_EngineObjects_Name.end() && itNew == m_NewEngineObjects.end() )
    {
        m_NewEngineObjects.push_back( pObject );
        pObject->SetParent(m_pOwner);
        result = true;
    }

    CSE.Leave();

    return result;  
}

bool TKEngineObjectList::CompareObjectSeq(TKEngineObject *pA, TKEngineObject *pB)
{
    return (pA->GetParent()->GetSeq() < pB->GetParent()->GetSeq());
}

bool TKEngineObjectList::Delete(int ID)
{
    CSE.Enter();
    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    {
        if ((*itNew)->GetID() == ID)
        {
            if (m_OwnerContainer) 
            {
                if ( !(*itNew)->GetReleased() ) (*itNew)->Release();
                delete (*itNew);
            }
            m_NewEngineObjects.erase(itNew);
            break;
        }
    }
    itID = m_EngineObjects_ID.find(ID);
    if (itID != m_EngineObjects_ID.end())
    {
        m_EraseList.push_back( itID->second );
        if (m_OwnerContainer) itID->second->SetDeleted( true );        
    }
    CSE.Leave();

    return true;
}

bool TKEngineObjectList::Delete(TKEngineObject *pEngineObject)
{
    return Delete( pEngineObject->GetID() );    
}

bool TKEngineObjectList::AddOnList(TKEngineObject *pObject)
{
    bool result  = false;

    CSE.Enter();

    itID = m_EngineObjects_ID.find( pObject->GetID() );
    itName = m_EngineObjects_Name.find( pObject->GetNodeName() );
    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == pObject->GetNodeName() || (*itNew)->GetID() == pObject->GetID() )
        {
            break;
        }
    }

    if ( itID == m_EngineObjects_ID.end() && itName == m_EngineObjects_Name.end() && itNew == m_NewEngineObjects.end() )
    {
        m_NewEngineObjects.push_back( pObject );
        pObject->SetParent(m_pOwner);
        result = true;
    }

    CSE.Leave();

    return result;
}

bool TKEngineObjectList::RemoveOnList(int ID)
{
    CSE.Enter();
    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    {
        if ((*itNew)->GetID() == ID)
        {
            m_NewEngineObjects.erase(itNew);
            break;
        }
    }
    itID = m_EngineObjects_ID.find(ID);
    if (itID != m_EngineObjects_ID.end())
    {
        m_EraseList.push_back( itID->second );
    }
    CSE.Leave();

    return true;
}

bool TKEngineObjectList::RemoveOnList(TKEngineObject *pEngineObject)
{
    return RemoveOnList( pEngineObject->GetID() ); 
}

void TKEngineObjectList::ApplyNew(void)
{
    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();
    //if ( !m_NewEngineObjects.empty() )
    //{
    //    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    //    {
    //        Rtn_ID = m_EngineObjects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew) ) );
    //        if (!Rtn_ID.second) MessageBox(0, "ApplyNew ID fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
    //        
    //        if ( (*itNew)->GetNodeName().empty() == false )
    //        {
    //            Rtn_Name = m_EngineObjects_Name.insert( make_pair( (*itNew)->GetNodeName(), (*itNew) ) );
    //            if (!Rtn_Name.second) MessageBox(0, "ApplyNew Name fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
    //        }
    //        if ( (*itNew)->GetFSRaw() != NULL )
    //        {
    //            Rtn_FS = m_FrameSideObjects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew)->GetFSRaw() ) );
    //            if (!Rtn_FS.second) MessageBox(0, "ApplyNew FSObject fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
    //        }
    //        if ( (*itNew)->GetRSRaw() != NULL )
    //        {
    //            Rtn_RS = m_RenderSideObjects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew)->GetRSRaw() ) );
    //            if (!Rtn_RS.second) MessageBox(0, "ApplyNew RSObject fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
    //            m_RenderSideObjects_Array.push_back( (*itNew)->GetRSRaw() );
    //        }
    //    }
    //}
    //if ( !m_NewEngineObjects.empty() )
    //{
    //    sort(m_RenderSideObjects_Array.begin(), m_RenderSideObjects_Array.end(), CompareObjectSeq);
    //    m_NewEngineObjects.clear();
    //}

    //if ( !m_EngineObjects_ID.empty() )
    //{
    //    for (itID = m_EngineObjects_ID.begin() ; itID != m_EngineObjects_ID.end() ; ++itID )
    //    {
    //        itID->second->ApplyNew();
    //    }
    //}

    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
}

void TKEngineObjectList::ApplyDelete(void)
{
    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();

    //if ( this->m_OwnerContainer )
    //{
    //    if ( !m_DelList.empty() )
    //    {
    //        for ( itDel = m_DelList.begin() ; itDel != m_DelList.end() ; ++itDel )
    //        {
    //            if ( !(*itDel)->GetReleased() ) (*itDel)->Release();
    //            delete (*itDel);
    //        }
    //        m_DelList.clear();
    //    }
    //}

    //ApplyErase();

    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
}

void TKEngineObjectList::ApplyErase(bool ClearEraseList)
{
    //int i = 0 ;

    ////CSE.Enter();
    ////CSF.Enter();
    ////CSR.Enter();

    //if (!m_EngineObjects_ID.empty())
    //{
    //    for (itID = m_EngineObjects_ID.begin() ; itID != m_EngineObjects_ID.end() ; ++itID )
    //    {
    //        itID->second->ApplyDelete();
    //    }
    //}

    //for ( itErase = m_EraseList.begin() ; itErase != m_EraseList.end() ; ++itErase )
    //{
    //    itRender = m_RenderSideObjects_ID.find( (*itErase)->GetID() );
    //    if (itRender != m_RenderSideObjects_ID.end())
    //    {
    //        m_RenderSideObjects_ID.erase( itRender );
    //    }

    //    itFrame = m_FrameSideObjects_ID.find( (*itErase)->GetID() );
    //    if (itFrame != m_FrameSideObjects_ID.end())
    //    {
    //        m_FrameSideObjects_ID.erase( itFrame );
    //    }

    //    itName = m_EngineObjects_Name.find( (*itErase)->GetNodeName() );
    //    if (itName != m_EngineObjects_Name.end())
    //    {
    //        m_EngineObjects_Name.erase( itName );
    //    }

    //    itID = m_EngineObjects_ID.find( (*itErase)->GetID() );
    //    if (itID != m_EngineObjects_ID.end())
    //    {
    //        m_EngineObjects_ID.erase( itID );
    //    }

    //    for (itArray = m_RenderSideObjects_Array.begin() ; itArray != m_RenderSideObjects_Array.end() ; ++itArray )
    //    {
    //        if ((*itArray)->GetParent()->GetID() == (*itErase)->GetID())
    //        {
    //            m_RenderSideObjects_Array.erase( itArray );
    //            break;
    //        }
    //    }
    //    if ((*itErase)->GetDeleted() && ClearEraseList)
    //    {
    //        m_DelList.push_back((*itErase));
    //    }
    //}
    //if ( ClearEraseList )
    //{
    //    if ( !m_EraseList.empty() ) 
    //        m_EraseList.clear();
    //}

    ////CSR.Leave();
    ////CSF.Leave();
    ////CSE.Leave();
}

int TKEngineObjectList::Count(void)
{
    int RtnValue = 0;
    
    ////CSE.Enter();
    //RtnValue = (int)m_EngineObjects_ID.size();
    ////CSE.Leave();

    return RtnValue;
}

void TKEngineObjectList::DataSync(void)
{
    ////CSE.Enter();
    ////CSF.Enter();
    ////CSR.Enter();
    //
    //if ( !m_EngineObjects_ID.empty() )
    //{
    //    for ( itID = m_EngineObjects_ID.begin() ; itID != m_EngineObjects_ID.end() ; ++itID )
    //    {
    //        itID->second->DataSync();
    //    }
    //}

    ////CSR.Leave();
    ////CSF.Leave();
    ////CSE.Leave();
}

void TKEngineObjectList::Init(void)
{
    //TKBaseObject::Init();

    ////CSE.Enter();
    ////CSF.Enter();
    ////CSR.Enter();
    //if ( !m_EngineObjects_ID.empty() )
    //{
    //    for (itID = m_EngineObjects_ID.begin() ; itID != m_EngineObjects_ID.end() ; ++itID)
    //    {
    //        itID->second->Init();
    //    }
    //}
    ////CSR.Leave();
    ////CSF.Leave();
    ////CSE.Leave();
}

void TKEngineObjectList::Frame(void)
{
    ////CSF.Enter();
    //if (!m_FrameSideObjects_ID.empty())
    //{
	   // TKFrameSideObject *pFSObject;
    //    for (itFrame = m_FrameSideObjects_ID.begin() ; 
    //         itFrame != m_FrameSideObjects_ID.end() ; 
    //         ++itFrame)
    //    {
		  //  pFSObject = itFrame->second;
    //        if (itFrame->second->GetIsEnable())
    //        {
    //            if (itFrame->second->GetIsFrame())
    //            {
    //                itFrame->second->BeforeFrame();
    //                itFrame->second->Frame();
    //                itFrame->second->AfterFrame();
    //            }
    //        }
    //    }
    //}
    ////CSF.Leave();
}

void TKEngineObjectList::Render(const ERenderFuncType RenderType)
{
    ////CSR.Enter();
    //if ( !m_RenderSideObjects_Array.empty() )
    //{
    //    for (itArray = m_RenderSideObjects_Array.begin() ; 
    //         itArray != m_RenderSideObjects_Array.end() ; 
    //         ++itArray )
    //    {
    //        if ( (*itArray)->GetIsEnable() )
    //        {
    //            if ( (*itArray)->GetIsRender() )
    //            {
    //                (*itArray)->BeforeRender(RenderType);
    //                (*itArray)->Render(RenderType);
    //                (*itArray)->AfterRender(RenderType);
    //            }
    //        }
    //    }
    //}
    ////CSR.Leave();
}

void TKEngineObjectList::Release(void)
{
    ////CSE.Enter();
    ////CSF.Enter();
    ////CSR.Enter();
    //if (!m_EngineObjects_ID.empty())
    //{
    //    for (itID = m_EngineObjects_ID.begin() ; itID != m_EngineObjects_ID.end() ; ++itID)
    //    {
    //        if ( !itID->second->GetReleased() ) itID->second->Release();
    //    }
    //}
    ////CSR.Leave();
    ////CSF.Leave();
    ////CSE.Leave();
    //
    //TKBaseObject::Release();
}

TKEngineObject * TKEngineObjectList::Item(int ID)
{
    TKEngineObject *pRtnValue = NULL;

    ////CSE.Enter();
    //itID = m_EngineObjects_ID.find(ID);
    //for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    //{
    //    if ( (*itNew)->GetID() == ID )
    //    {
    //        break;
    //    }
    //}
    //if (itID != m_EngineObjects_ID.end())
    //{
    //    pRtnValue = itID->second;
    //}
    //else if (itNew != m_NewEngineObjects.end() )
    //{
    //    pRtnValue = (*itNew);
    //}
    //if (pRtnValue)
    //{
    //    if (pRtnValue->GetDeleted())
    //    {
    //        pRtnValue = NULL;
    //    }
    //}
    ////CSE.Leave();

    return pRtnValue;
}

TKEngineObject * TKEngineObjectList::Item(string & NodeName)
{
    TKEngineObject *pRtnValue = NULL;

    ////CSE.Enter();
    //itName = m_EngineObjects_Name.find(NodeName);
    //for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    //{
    //    if ( (*itNew)->GetNodeName() == NodeName )
    //    {
    //        break;
    //    }
    //}
    //if (itName != m_EngineObjects_Name.end())
    //{
    //    pRtnValue = itName->second;
    //}
    //else if (itNew != m_NewEngineObjects.end() )
    //{
    //    pRtnValue = (*itNew);
    //}
    //if (pRtnValue)
    //{
    //    if (pRtnValue->GetDeleted())
    //    {
    //        pRtnValue = NULL;
    //    }
    //}
    ////CSE.Leave();

    return pRtnValue;
}

TKEngineObject * TKEngineObjectList::ItemBySeq(int Seq)
{
    //return m_RenderSideObjects_Array[Seq]->GetParent();

	return NULL;
}

TKEngineObject * TKEngineObjectList::operator [](int Seq)
{
    //return m_RenderSideObjects_Array[Seq]->GetParent();
	return NULL;
}

void TKEngineObjectList::ClearDelete(void)
{
    ////CSE.Enter();
    ////CSF.Enter();
    ////CSR.Enter();

    //if (m_OwnerContainer) 
    //{
    //    for ( itNew = m_NewEngineObjects.begin() ; itNew != m_NewEngineObjects.end() ; ++itNew )
    //    {
    //        if ( !(*itNew)->GetReleased() ) (*itNew)->Release();
    //        delete (*itNew);
    //    };

    //    for ( itID = m_EngineObjects_ID.begin() ; itID != m_EngineObjects_ID.end() ; ++itID )
    //    {
    //        if ( !itID->second->GetReleased() ) itID->second->Release();
    //        delete itID->second;
    //    };
    //}

    //m_NewEngineObjects.clear();
    //m_EngineObjects_ID.clear();
    //m_EngineObjects_Name.clear();

    //m_FrameSideObjects_ID.clear();
    //m_RenderSideObjects_ID.clear();
    //m_RenderSideObjects_Array.clear();

    ////CSR.Leave();
    ////CSF.Leave();
    ////CSE.Leave();
}
