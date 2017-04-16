#include "./TKBaseObjectList.h"
#include "./TKEngine.h"

TKBaseObjectList::TKBaseObjectList(bool OwnerContainer )
:TKBaseObject()
{
    SetObjectType( eotBaseObjectList );

    this->OwnerContainer = OwnerContainer;
    this->ClearDelete();
}

TKBaseObjectList::~TKBaseObjectList(void)
{
    if (OwnerContainer)
    {
        this->ClearDelete();
    }
}

bool TKBaseObjectList::CompareObjectID(TKBaseObject *pA, TKBaseObject *pB)
{
    return (pA->GetID() < pB->GetID());
}

bool TKBaseObjectList::Delete(int ID)
{
    bool result = false;
    //vector<TKBaseObject*>::iterator itNew;
    //map<int, TKBaseObject*>::iterator itID;

    CS.Enter();
    for (itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetID() == ID )
        {
            if (OwnerContainer) 
            {
                if ( !(*itNew)->GetReleased() ) (*itNew)->Release();
                delete (*itNew);
            }
            NewObjects.erase(itNew);
            result = true;
            break;
        }
    }
    if (result == false)
    {
        itID = Objects_ID.find( ID );
        if ( itID != Objects_ID.end() )
        {
            EraseList.push_back( itID->second );
            if (OwnerContainer) itID->second->SetDeleted( true);
            result = true;
        }
    }
    CS.Leave();

    return result;
}

bool TKBaseObjectList::Delete(char *pNodeName)
{
    //map<string, TKBaseObject*>::iterator itName;
    itName = Objects_Name.find( pNodeName );
    if (itName != Objects_Name.end() )
    {
        EraseList.push_back( itName->second );
        if (OwnerContainer) itName->second->SetDeleted( true );
        return true;
    }
    else
    {
        return false;
    }
}

void TKBaseObjectList::Init(void)
{
    //map<int, TKBaseObject*>::iterator itID;
    __super::Init();
    CS.Enter();
    for (itID = Objects_ID.begin() ; itID != Objects_ID.end() ; ++itID )
    {
        itID->second->Init();
    }
    CS.Leave();
}

void TKBaseObjectList::Frame(void)
{
    CS.Enter();

    TKBaseObject *pObject;

    //map<int, TKBaseObject*>::iterator itID;
    for (itID = Objects_ID.begin() ; itID != Objects_ID.end() ; ++itID )
    {
        pObject = itID->second;
        pObject->Frame();
        //OutputDebugString(pObject->GetNodeName());
        //OutputDebugString("\n");
    }
    CS.Leave();
}

void TKBaseObjectList::Release(void)
{
    CS.Enter();
    //map<int, TKBaseObject*>::iterator it;
    for (itID = Objects_ID.begin() ; itID != Objects_ID.end() ; ++itID )
    {
        if ( !itID->second->GetReleased() ) 
            itID->second->Release();
    }
    CS.Leave();
    
    __super::Release();
}


void TKBaseObjectList::ApplyNew(void)
{
    //CS.Enter();
    //pair<map<int, TKBaseObject*>::iterator, bool> Rtn_ID;
    //pair<map<string, TKBaseObject*>::iterator, bool> Rtn_Name;

    if ( !NewObjects.empty() )
    {
        for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
        {
            Rtn_ID = Objects_ID.insert( make_pair( (*itNew)->GetID(), (*itNew) ) );
            if (!Rtn_ID.second) MessageBox(0, "ApplyNew ID fail", "TKBaseObjectList::ApplyNew", MB_ICONERROR);
            if ( (*itNew)->GetNodeName().empty() == false )
            {
                Rtn_Name = Objects_Name.insert( make_pair( (*itNew)->GetNodeName(), (*itNew) ) );
                if (!Rtn_Name.second) MessageBox(0, "ApplyNew Name fail", "TKBaseObjectList::ApplyNew", MB_ICONERROR);
            }
            Objects_Array.push_back( (*itNew) );
        }
        sort(Objects_Array.begin(), Objects_Array.end(), CompareObjectID);
        NewObjects.clear();
    }    
    //CS.Leave();
}

void TKBaseObjectList::ApplyDelete(void)
{
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
}

void TKBaseObjectList::ApplyErase(bool ClearEraseList)
{
    //vector<TKBaseObject*>::iterator itErase, itArray;
    //map<string, TKBaseObject*>::iterator itName;
    //map<int, TKBaseObject*>::iterator itID;

    for ( itErase = EraseList.begin() ; itErase != EraseList.end() ; ++itErase )
    {
        itID = Objects_ID.find( (*itErase)->GetID() );
        if (itID != Objects_ID.end())
        {
            Objects_ID.erase( itID );
        }

        itName = Objects_Name.find( (*itErase)->GetNodeName() );
        if (itName != Objects_Name.end())
        {
            Objects_Name.erase( itName );
        }

        for (itArray = Objects_Array.begin() ; itArray != Objects_Array.end() ; ++itArray )
        {
            if ( (*itArray)->GetID() == (*itErase)->GetID() )
            {
                Objects_Array.erase(itArray);
                break;
            }
        }
        if ((*itErase)->GetDeleted() && ClearEraseList)
        {
            DelList.push_back( (*itErase) );
        }
    }
    if ( ClearEraseList )
    {
        if ( !EraseList.empty() ) 
            EraseList.clear();
    }
}

TKBaseObject * TKBaseObjectList::Item(int ID)
{
    TKBaseObject *pRtnValue = NULL;
    //vector<TKBaseObject*>::iterator itNew;
    //map<int, TKBaseObject*>::iterator itID;

    CS.Enter();

    itID = Objects_ID.find(ID);
    for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetID() == ID )
        {
            break;
        }
    }
    if ( itID != Objects_ID.end() )
    {
        pRtnValue = itID->second;
    }
    else if ( itNew != NewObjects.end() )
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
    CS.Leave();

    return pRtnValue;
}

TKBaseObject * TKBaseObjectList::Item(string & NodeName)
{
    TKBaseObject *pRtnValue = NULL;
    //vector<TKBaseObject*>::iterator itNew;
    //map<string, TKBaseObject*>::iterator itName;

    CS.Enter();

    itName = Objects_Name.find(NodeName);
    for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
    {
        if ( (*itNew)->GetNodeName() == NodeName )
        {
            break;
        }
    }
    if (itName != Objects_Name.end())
    {
        pRtnValue = itName->second;
    }
    else if ( itNew != NewObjects.end() )
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
    CS.Leave();

    return pRtnValue;
}

const TKBaseObjectList & TKBaseObjectList::operator = ( const TKBaseObjectList & BaseObjectList )
{
    this->CS.Enter();
    
    __super::operator=((TKBaseObject&)BaseObjectList);
    
    this->NewObjects     = BaseObjectList.NewObjects;
    this->Objects_ID     = BaseObjectList.Objects_ID;
    this->Objects_Name   = BaseObjectList.Objects_Name;
    this->Objects_Array  = BaseObjectList.Objects_Array;
    this->EraseList      = BaseObjectList.EraseList;
    this->DelList        = BaseObjectList.DelList;
    this->OwnerContainer = BaseObjectList.OwnerContainer;
    
    this->CS.Leave();

    return BaseObjectList;
}

void TKBaseObjectList::ClearDelete(void)
{
    //vector<TKBaseObject*>::iterator itNew;
    //map<int, TKBaseObject*>::iterator itID;

    CS.Enter();

    if (OwnerContainer)
    {
        for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; ++itNew )
        {
            if ( !(*itNew)->GetReleased() ) (*itNew)->Release();
            delete (*itNew);
        }
        for ( itID = Objects_ID.begin() ; itID != Objects_ID.end() ; ++itID )
        {
            if ( !itID->second->GetReleased() ) itID->second->Release();
            delete itID->second;
        }
    }
    NewObjects.clear();
    Objects_ID.clear();
    Objects_Name.clear();
    Objects_Array.clear();

    CS.Leave();
}

void TKBaseObjectList::SetOwnerContainer(bool OwnerContainer)
{
    this->OwnerContainer = OwnerContainer;
}

//int TKBaseObjectList::GenID(void)
//{
//    vector<TKBaseObject*>::iterator itNew;
//    map<int, TKBaseObject*>::iterator itID;
//
//    while ( true )
//    {
//        LastID++;
//        for ( itNew = NewObjects.begin() ; itNew != NewObjects.end() ; itNew++ )
//        {
//            if ((*itNew)->GetID() == LastID )
//            {
//                break;
//            }
//        }
//        itID = Objects_ID.find( LastID );
//        if (itNew == NewObjects.end() && itID == Objects_ID.end())
//        {
//            break;
//        }        
//    }
//
//    return LastID;
//}