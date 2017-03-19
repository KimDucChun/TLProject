#include "TKCommon.h"
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
////INT_VECTOR3
/////////////////////////////////////////////////////////////////////////////////////////////////////
//INT_VECTOR3::INT_VECTOR3(void)
//	:x(0), y(0), z(0)
//{
//}
//
//INT_VECTOR3::INT_VECTOR3(INT_VECTOR3 & AData)
//{
//	(*this) = AData;
//}
//
//INT_VECTOR3::INT_VECTOR3(int x, int y, int z)
//	:x(x), y(y), z(z)
//{
//}
//
//INT_VECTOR3::~INT_VECTOR3(void)
//{
//}
//
//bool INT_VECTOR3::operator == (INT_VECTOR3 & AData)
//{
//	return  (this->x == AData.x) &&
//		(this->y == AData.y) &&
//		(this->z == AData.z);
//}
//
//bool INT_VECTOR3::operator != (INT_VECTOR3 & AData)
//{
//	return  !(*this == AData);
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
////FLOAT_VECTOR3
/////////////////////////////////////////////////////////////////////////////////////////////////////
//INT_VECTOR3::INT_VECTOR3(void)
//	:x(0), y(0), z(0)
//{
//}
//
//INT_VECTOR3::INT_VECTOR3(INT_VECTOR3 & AData)
//{
//	(*this) = AData;
//}
//
//INT_VECTOR3::INT_VECTOR3(int x, int y, int z)
//	:x(x), y(y), z(z)
//{
//}
//
//INT_VECTOR3::~INT_VECTOR3(void)
//{
//}
//
//bool INT_VECTOR3::operator == (INT_VECTOR3 & AData)
//{
//	return  (this->x == AData.x) &&
//		(this->y == AData.y) &&
//		(this->z == AData.z);
//}
//
//bool INT_VECTOR3::operator != (INT_VECTOR3 & AData)
//{
//	return  !(*this == AData);
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
//TKNumGenerator
///////////////////////////////////////////////////////////////////////////////////////////////////
TKIDGenerator::TKIDGenerator(void)
:LastID(0)
{
    InitializeCriticalSection(&cs);
}

TKIDGenerator::~TKIDGenerator(void)
{
    DeleteCriticalSection(&cs);
}

TKIDGenerator & TKIDGenerator::GetInstance(void)
{
    static TKIDGenerator IDGenerator;
    return IDGenerator;
}

int TKIDGenerator::GenID(void)
{
    int RtnID;
    EnterCriticalSection(&cs);
    RtnID = LastID++;
    LeaveCriticalSection(&cs);
    return RtnID;
}

string TKIDGenerator::GenIDStr(void)
{
    return GenIDStr("", "");
}

string TKIDGenerator::GenIDStr(string PreFix, string PostFix)
{
    int RtnID;
    char strTemp[20];
    string RtnStr;
    
    EnterCriticalSection(&cs);
    RtnID = LastID++;
    LeaveCriticalSection(&cs);
    
    itoa(RtnID, strTemp, 10);
    RtnStr = strTemp;
    RtnStr = PreFix + RtnStr + PostFix;
    return RtnStr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//TKCheckExpenses
///////////////////////////////////////////////////////////////////////////////////////////////////
TKCheckExpenses::TKCheckExpenses(void)
{
    InitializeCriticalSection(&cs);
}

TKCheckExpenses::~TKCheckExpenses(void)
{
    DeleteCriticalSection(&cs);
}

TKCheckExpenses & TKCheckExpenses::CurrInstance(void)
{
    static TKCheckExpenses CurrentInstance;
    return CurrentInstance;
}

void TKCheckExpenses::ExpensesCheckStart(void)
{
    EnterCriticalSection(&cs);
    ZeroMemory(&tResult, sizeof(LARGE_INTEGER));
    QueryPerformanceCounter( &tStart );
}

void TKCheckExpenses::ExpensesCheckEnd(void)
{
    QueryPerformanceCounter( &tEnd );
    tResult.QuadPart = tEnd.QuadPart - tStart.QuadPart;
    LeaveCriticalSection(&cs);
}

void TKCheckExpenses::OutputDebugExpenses(char *pString)
{
    char strTemp[255];
    if (pString)
    {
        sprintf(strTemp, "%s Time:%lld\n", pString, tResult.QuadPart);
    }
    else
    {
        sprintf(strTemp, "Time:%lld\n", tResult.QuadPart);
    }
    OutputDebugString(strTemp);    
}

void StringReplace(char *pSrc, const char* pFindStr, const char* pReplaceStr)
{
    char    *pBackUpSrcStr;
    char    *pResultStr;
    char    tmpBuf[SBUFSIZE];
    int     modLen;

    pBackUpSrcStr = pSrc;
    ZeroMemory(tmpBuf, SBUFSIZE);

    while (true)
    {
        pResultStr = strstr(pSrc, pFindStr);
        if (pResultStr == NULL)
        {
            modLen = (int)strlen(pSrc);
            if (strcmp(tmpBuf, "") == 0)
            {
                strncpy(tmpBuf, pSrc, modLen);
            }
            else
            {
                strncat(tmpBuf, pSrc, modLen);
            }
            pSrc += (modLen + strlen(pFindStr));
            break;
        }
        else
        {
            modLen = (int)(pResultStr - pSrc);
            strncat(tmpBuf, pSrc, modLen);
            strncat(tmpBuf, pReplaceStr, strlen(pReplaceStr));
            pSrc += (modLen + strlen(pFindStr));
        }
    }

    pSrc = pBackUpSrcStr;
    strcpy(pSrc, tmpBuf);
}

// 전체 파일 경로에서 파일명 가져오기
void ExtractFileNameWithExt( const char *pSrc, char *pResult )
{
    int sLen = (int)strlen(pSrc);
    for ( int i = sLen-1 ; i >= 0 ; --i )
    {
        if ( pSrc[i] == '\\' || pSrc[i] == '/' )
        {
            if ( i == sLen-1 )
            {
                pResult[0] = 0;
                return;
            }
            else
            {
                strcpy(pResult, &pSrc[i+1]);
                return;
            }
        }
    }
    strcpy(pResult, pSrc);
}

// 전체 파일명에서 확장자만 가져오기
void ExtractFileExt( const char *pSrc, char *pResult )
{
    int sLen = (int)strlen(pSrc);
    for ( int i = sLen-1 ; i >= 0 ; --i )
    {
        if ( pSrc[i] == '.')
        {
            if ( i == sLen-1 )
            {
                pResult[0] = 0;
                return;
            }
            else
            {
                strcpy(pResult, &pSrc[i+1]);
                return;
            }
        }
    }
    pResult[0] = 0;
}

// 전체 파일명에서 경로만 가져오기
void ExtractFileDir( const char *pSrc, char *pResult )
{
    char strTemp[MAX_PATH];
    char strSrc[MAX_PATH];
    
    strcpy(strSrc, pSrc);
    ExtractFileNameWithExt( strSrc, strTemp );
    StringReplace( strSrc, strTemp, "" );
    
    strcpy(pResult, strSrc);
}