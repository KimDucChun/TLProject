#include "./TKEngineObjectList.h"
#include "./TKEngineObject.h"

TKEngineObjectList::TKEngineObjectList(TKEngineObject *pOwner, bool OwnerContainer)
:TKBaseObject()
,pOwner(pOwner)
,OwnerContainer(OwnerContainer)
{
    this->EraseList.clear();
    this->ClearDelete();
}

TKEngineObjectList::~TKEngineObjectList(void)
{
    if (OwnerContainer)
    {
        this->ClearDelete();
    }
}

bool TKEngineObjectList::Add(TKEngineObject *pObject)
{
    bool result  = false;

    if (OwnerContainer) return result;

    CSE.Enter();

    itID = EngineObjects_ID.find( pObject->GetID() );
    itName = EngineObjects_Name.find( pObject->GetNodeName() );
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == pObject->GetNodeName() || (*itNew)->GetID() == pObject->GetID() )
        {
            break;
        }
    }

    if ( itID == EngineObjects_ID.end() && itName == EngineObjects_Name.end() && itNew == NewEngineObjects.end() )
    {
        NewEngineObjects.push_back( pObject );
        pObject->SetParent(pOwner);
        result = true;
    }

    CSE.Leave();

    return result;  
}

bool TKEngineObjectList::CompareObjectSeq(TKRenderSideObject *pA, TKRenderSideObject *pB)
{
    return (pA->GetParent()->GetSeq() < pB->GetParent()->GetSeq());
}

bool TKEngineObjectList::Delete(int ID)
{
    CSE.Enter();
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ((*itNew)->GetID() == ID)
        {
            if (OwnerContainer) 
            {
                if ( !(*itNew)->GetReleased() ) (*itNew)->Release();
                delete (*itNew);
            }
            NewEngineObjects.erase(itNew);
            break;
        }
    }
    itID = EngineObjects_ID.find(ID);
    if (itID != EngineObjects_ID.end())
    {
        EraseList.push_back( itID->second );
        if (OwnerContainer) itID->second->SetDeleted( true );        
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

    itID = EngineObjects_ID.find( pObject->GetID() );
    itName = EngineObjects_Name.find( pObject->GetNodeName() );
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == pObject->GetNodeName() || (*itNew)->GetID() == pObject->GetID() )
        {
            break;
        }
    }

    if ( itID == EngineObjects_ID.end() && itName == EngineObjects_Name.end() && itNew == NewEngineObjects.end() )
    {
        NewEngineObjects.push_back( pObject );
        pObject->SetParent(pOwner);
        result = true;
    }

    CSE.Leave();

    return result;
}

bool TKEngineObjectList::RemoveOnList(int ID)
{
    CSE.Enter();
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ((*itNew)->GetID() == ID)
        {
            NewEngineObjects.erase(itNew);
            break;
        }
    }
    itID = EngineObjects_ID.find(ID);
    if (itID != EngineObjects_ID.end())
    {
        EraseList.push_back( itID->second );
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
    CSE.Enter();
    CSF.Enter();
    CSR.Enter();
    if ( !NewEngineObjects.empty() )
    {
        for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
        {
            Rtn_ID = EngineObjects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew) ) );
            if (!Rtn_ID.second) MessageBox(0, "ApplyNew ID fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
            
            if ( (*itNew)->GetNodeName().empty() == false )
            {
                Rtn_Name = EngineObjects_Name.insert( make_pair( (*itNew)->GetNodeName(), (*itNew) ) );
                if (!Rtn_Name.second) MessageBox(0, "ApplyNew Name fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
            }
            if ( (*itNew)->GetFSRaw() != NULL )
            {
                Rtn_FS = FrameSideObjects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew)->GetFSRaw() ) );
                if (!Rtn_FS.second) MessageBox(0, "ApplyNew FSObject fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
            }
            if ( (*itNew)->GetRSRaw() != NULL )
            {
                Rtn_RS = RenderSideObjects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew)->GetRSRaw() ) );
                if (!Rtn_RS.second) MessageBox(0, "ApplyNew RSObject fail", "TKEngineObjectList::ApplyNew", MB_ICONERROR);
                RenderSideObjects_Array.push_back( (*itNew)->GetRSRaw() );
            }
        }
    }
    if ( !NewEngineObjects.empty() )
    {
        sort(RenderSideObjects_Array.begin(), RenderSideObjects_Array.end(), CompareObjectSeq);
        NewEngineObjects.clear();
    }

    if ( !EngineObjects_ID.empty() )
    {
        for (itID = EngineObjects_ID.begin() ; itID != EngineObjects_ID.end() ; ++itID )
        {
            itID->second->ApplyNew();
        }
    }

    CSR.Leave();
    CSF.Leave();
    CSE.Leave();
}

void TKEngineObjectList::ApplyDelete(void)
{
    CSE.Enter();
    CSF.Enter();
    CSR.Enter();

    if ( this->OwnerContainer )
    {
        if ( !DelList.empty() )
        {
            for ( itDel = DelList.begin() ; itDel != DelList.end() ; ++itDel )
            {
                if ( !(*itDel)->GetReleased() ) (*itDel)->Release();
                delete (*itDel);
            }
            DelList.clear();
        }
    }

    ApplyErase();

    CSR.Leave();
    CSF.Leave();
    CSE.Leave();
}

void TKEngineObjectList::ApplyErase(bool ClearEraseList)
{
    int i = 0 ;

    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();

    if (!EngineObjects_ID.empty())
    {
        for (itID = EngineObjects_ID.begin() ; itID != EngineObjects_ID.end() ; ++itID )
        {
            itID->second->ApplyDelete();
        }
    }

    for ( itErase = EraseList.begin() ; itErase != EraseList.end() ; ++itErase )
    {
        itRender = RenderSideObjects_ID.find( (*itErase)->GetID() );
        if (itRender != RenderSideObjects_ID.end())
        {
            RenderSideObjects_ID.erase( itRender );
        }

        itFrame = FrameSideObjects_ID.find( (*itErase)->GetID() );
        if (itFrame != FrameSideObjects_ID.end())
        {
            FrameSideObjects_ID.erase( itFrame );
        }

        itName = EngineObjects_Name.find( (*itErase)->GetNodeName() );
        if (itName != EngineObjects_Name.end())
        {
            EngineObjects_Name.erase( itName );
        }

        itID = EngineObjects_ID.find( (*itErase)->GetID() );
        if (itID != EngineObjects_ID.end())
        {
            EngineObjects_ID.erase( itID );
        }

        for (itArray = RenderSideObjects_Array.begin() ; itArray != RenderSideObjects_Array.end() ; ++itArray )
        {
            if ((*itArray)->GetParent()->GetID() == (*itErase)->GetID())
            {
                RenderSideObjects_Array.erase( itArray );
                break;
            }
        }
        if ((*itErase)->GetDeleted() && ClearEraseList)
        {
            DelList.push_back((*itErase));
        }
    }
    if ( ClearEraseList )
    {
        if ( !EraseList.empty() ) 
            EraseList.clear();
    }

    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
}

int TKEngineObjectList::Count(void)
{
    int RtnValue = 0;
    
    //CSE.Enter();
    RtnValue = (int)EngineObjects_ID.size();
    //CSE.Leave();

    return RtnValue;
}

void TKEngineObjectList::DataSync(void)
{
    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();
    
    if ( !EngineObjects_ID.empty() )
    {
        for ( itID = EngineObjects_ID.begin() ; itID != EngineObjects_ID.end() ; ++itID )
        {
            itID->second->DataSync();
        }
    }

    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
}

void TKEngineObjectList::Init(void)
{
    TKBaseObject::Init();

    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();
    if ( !EngineObjects_ID.empty() )
    {
        for (itID = EngineObjects_ID.begin() ; itID != EngineObjects_ID.end() ; ++itID)
        {
            itID->second->Init();
        }
    }
    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
}

void TKEngineObjectList::Frame(void)
{
    //CSF.Enter();
    if (!FrameSideObjects_ID.empty())
    {
	    TKFrameSideObject *pFSObject;
        for (itFrame = FrameSideObjects_ID.begin() ; 
             itFrame != FrameSideObjects_ID.end() ; 
             ++itFrame)
        {
		    pFSObject = itFrame->second;
            if (itFrame->second->GetIsEnable())
            {
                if (itFrame->second->GetIsFrame())
                {
                    itFrame->second->BeforeFrame();
                    itFrame->second->Frame();
                    itFrame->second->AfterFrame();
                }
            }
        }
    }
    //CSF.Leave();
}

void TKEngineObjectList::Render(const ERenderFuncType RenderType)
{
    //CSR.Enter();
    if ( !RenderSideObjects_Array.empty() )
    {
        for (itArray = RenderSideObjects_Array.begin() ; 
             itArray != RenderSideObjects_Array.end() ; 
             ++itArray )
        {
            if ( (*itArray)->GetIsEnable() )
            {
                if ( (*itArray)->GetIsRender() )
                {
                    (*itArray)->BeforeRender(RenderType);
                    (*itArray)->Render(RenderType);
                    (*itArray)->AfterRender(RenderType);
                }
            }
        }
    }
    //CSR.Leave();
}

void TKEngineObjectList::Release(void)
{
    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();
    if (!EngineObjects_ID.empty())
    {
        for (itID = EngineObjects_ID.begin() ; itID != EngineObjects_ID.end() ; ++itID)
        {
            if ( !itID->second->GetReleased() ) itID->second->Release();
        }
    }
    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
    
    TKBaseObject::Release();
}

TKEngineObject * TKEngineObjectList::Item(int ID)
{
    TKEngineObject *pRtnValue = NULL;

    //CSE.Enter();
    itID = EngineObjects_ID.find(ID);
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetID() == ID )
        {
            break;
        }
    }
    if (itID != EngineObjects_ID.end())
    {
        pRtnValue = itID->second;
    }
    else if (itNew != NewEngineObjects.end() )
    {
        pRtnValue = (*itNew);
    }
    if (pRtnValue)
    {
        if (pRtnValue->GetDeleted())
        {
            pRtnValue = NULL;
        }
    }
    //CSE.Leave();

    return pRtnValue;
}

TKEngineObject * TKEngineObjectList::Item(string & NodeName)
{
    TKEngineObject *pRtnValue = NULL;

    //CSE.Enter();
    itName = EngineObjects_Name.find(NodeName);
    for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == NodeName )
        {
            break;
        }
    }
    if (itName != EngineObjects_Name.end())
    {
        pRtnValue = itName->second;
    }
    else if (itNew != NewEngineObjects.end() )
    {
        pRtnValue = (*itNew);
    }
    if (pRtnValue)
    {
        if (pRtnValue->GetDeleted())
        {
            pRtnValue = NULL;
        }
    }
    //CSE.Leave();

    return pRtnValue;
}

TKEngineObject * TKEngineObjectList::ItemBySeq(int Seq)
{
    return RenderSideObjects_Array[Seq]->GetParent();
}

TKFrameSideObject * TKEngineObjectList::FSObject(int ID)
{
    TKFrameSideObject *pRtnValue = NULL;

    //CSF.Enter();
    itFrame = FrameSideObjects_ID.find(ID);
    if (itFrame != FrameSideObjects_ID.end())
    {
        pRtnValue = itFrame->second;
    }
    //CSF.Leave();

    return pRtnValue;
}

TKRenderSideObject * TKEngineObjectList::RSObject(int ID)
{
    TKRenderSideObject *pRtnValue = NULL;

    //CSR.Enter();
    itRender = RenderSideObjects_ID.find(ID);
    if (itRender != RenderSideObjects_ID.end())
    {
        pRtnValue = itRender->second;
    }
    //CSR.Leave();

    return pRtnValue;
}

TKEngineObject * TKEngineObjectList::operator [](int Seq)
{
    return RenderSideObjects_Array[Seq]->GetParent();
}

void TKEngineObjectList::ClearDelete(void)
{
    //CSE.Enter();
    //CSF.Enter();
    //CSR.Enter();
    if (OwnerContainer) 
    {
        for ( itNew = NewEngineObjects.begin() ; itNew != NewEngineObjects.end() ; ++itNew )
        {
            if ( !(*itNew)->GetReleased() ) (*itNew)->Release();
            delete (*itNew);
        };

        for ( itID = EngineObjects_ID.begin() ; itID != EngineObjects_ID.end() ; ++itID )
        {
            if ( !itID->second->GetReleased() ) itID->second->Release();
            delete itID->second;
        };
    }

    NewEngineObjects.clear();
    EngineObjects_ID.clear();
    EngineObjects_Name.clear();

    FrameSideObjects_ID.clear();
    RenderSideObjects_ID.clear();
    RenderSideObjects_Array.clear();

    //CSR.Leave();
    //CSF.Leave();
    //CSE.Leave();
}
