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

bool BuildOBB(D3DXVECTOR3S & PosList, D3DXVECTOR3 & vecCenter, D3DXVECTOR3 vecAxis[3], float AxisLen[3], D3DXVECTOR3 vecOBB[8])
{
	int nPoints = 0;
	Vector3 *pVector3;
	Vector3 centroid, basis[3], vmin, vmax, OBB;

	memset(&vecCenter, 0, sizeof(D3DXVECTOR3));
	memset(vecAxis, 0, sizeof(vecAxis));
	memset(AxisLen, 0, sizeof(AxisLen));
	memset(vecOBB, 0, sizeof(vecOBB));

	nPoints = (int)PosList.size();

	// 버텍스가 없으면 걍 리턴.
	if (nPoints == 0) return false;

	// 계산된 고유축과 최대최소값을 객체의 멤버에 저장
	pVector3 = new Vector3[nPoints];
	for (int i = 0; i < nPoints; ++i)
	{
		pVector3[i].x = PosList[i].x;
		pVector3[i].y = PosList[i].y;
		pVector3[i].z = PosList[i].z;
	}
	ComputeBoundingBox(pVector3, nPoints, centroid, basis, vmin, vmax);
	delete[nPoints] pVector3;

	// 계산된 고유축과 최대최소값을 객체의 멤버에 저장
	for (int i = 0; i < 3; ++i)
	{
		vecAxis[i].x = basis[i].x;
		vecAxis[i].y = basis[i].y;
		vecAxis[i].z = basis[i].z;
	}
	AxisLen[0] = (vmax.x - vmin.x) / 2.0f;
	AxisLen[1] = (vmax.y - vmin.y) / 2.0f;
	AxisLen[2] = (vmax.z - vmin.z) / 2.0f;

	// 고유축의 방향성을 원래축에 최대한 가깝게 정렬
	for (int i = 0; i < 2; ++i)
	{
		if (vecAxis[0].x < vecAxis[i + 1].x)
		{
			swap(vecAxis[0], vecAxis[i + 1]);
			swap(AxisLen[0], AxisLen[i + 1]);
		}
	}
	if (vecAxis[1].y < vecAxis[2].y)
	{
		swap(vecAxis[1], vecAxis[2]);
		swap(AxisLen[1], AxisLen[2]);
	}

	// 평면을 구해서 중심점 구하기.
	D3DXPLANE plane[6];
	float fMin, fMax, fNow;
	for (int pdix = 0; pdix < 3; ++pdix)
	{
		fMin = 100000.0f;
		fMax = -100000.0f;
		for (int i = 0; i < nPoints; ++i)
		{
			fNow = D3DXVec3Dot(&PosList[i], &vecAxis[pdix]);
			fMin = min(fMin, fNow);
			fMax = max(fMax, fNow);
		}

		plane[pdix * 2] = (D3DXPLANE)vecAxis[pdix];
		plane[pdix * 2].d = -fMin;

		plane[pdix * 2 + 1] = (D3DXPLANE)((-1.0f) * vecAxis[pdix]);
		plane[pdix * 2 + 1].d = fMax;
	}

	float fa, fb, fc;
	fa = float(((-plane[0].d) + plane[1].d) * 0.5f);
	fb = float(((-plane[2].d) + plane[3].d) * 0.5f);
	fc = float(((-plane[4].d) + plane[5].d) * 0.5f);

	vecCenter = (fa * vecAxis[0]) + (fb * vecAxis[1]) + (fc * vecAxis[2]);

	// OBB 8점 위치 설정
	vecOBB[0] = vecCenter + (vecAxis[0] * AxisLen[0] + vecAxis[1] * AxisLen[1] + vecAxis[2] * AxisLen[2]);
	vecOBB[1] = vecCenter + (vecAxis[0] * AxisLen[0] + vecAxis[1] * AxisLen[1] - vecAxis[2] * AxisLen[2]);
	vecOBB[2] = vecCenter + (vecAxis[0] * AxisLen[0] - vecAxis[1] * AxisLen[1] - vecAxis[2] * AxisLen[2]);
	vecOBB[3] = vecCenter + (vecAxis[0] * AxisLen[0] - vecAxis[1] * AxisLen[1] + vecAxis[2] * AxisLen[2]);
	vecOBB[4] = vecCenter + (-vecAxis[0] * AxisLen[0] + vecAxis[1] * AxisLen[1] + vecAxis[2] * AxisLen[2]);
	vecOBB[5] = vecCenter + (-vecAxis[0] * AxisLen[0] + vecAxis[1] * AxisLen[1] - vecAxis[2] * AxisLen[2]);
	vecOBB[6] = vecCenter + (-vecAxis[0] * AxisLen[0] - vecAxis[1] * AxisLen[1] - vecAxis[2] * AxisLen[2]);
	vecOBB[7] = vecCenter + (-vecAxis[0] * AxisLen[0] - vecAxis[1] * AxisLen[1] + vecAxis[2] * AxisLen[2]);

	return true;
}