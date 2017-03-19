#include "./TKTime.h"
#include "./TKEngine.h"

////////////////////////////////////////////////////////////////////////////////
// TKTime
////////////////////////////////////////////////////////////////////////////////
TKTime::TKTime(TKEngine *pEngine)
:TKBaseObject(pEngine)
{
}

TKTime::~TKTime(void)
{
}

void TKTime::Init(void)
{
    TKBaseObject::Init();
    // 1초에 해당하는 클럭카운트를 얻어옴.
    QueryPerformanceFrequency(&m_Frequency);
	memset( &m_End, 0, sizeof( LARGE_INTEGER ) );
	memset( &m_Start, 0, sizeof( LARGE_INTEGER ) );
	m_TimeDelta = 0.0f;
}

void TKTime::Frame(void)
{
	// 1초당 프레임 체크
	m_uiCurrentTime = timeGetTime();
	m_iMilliSecend += (m_uiCurrentTime - m_uiPreTime);
	if(m_iMilliSecend > 1000)
	{
		m_fFramePerSec = (float)m_iFPSCnt;
		m_iFPSCnt = m_iMilliSecend = 0;
	}
    ++m_iFPSCnt;
	m_uiPreTime = m_uiCurrentTime;

	// 1프레임의 시간 체크
	QueryPerformanceCounter( &m_End );

	if( m_Start.QuadPart != 0 )
        // 신형 코어에서 타임델타값이 음수로 나오는 문제가 있음.
		m_TimeDelta = (float)((double)( m_End.QuadPart - m_Start.QuadPart ) / (double)m_Frequency.QuadPart);
	else
		m_TimeDelta = 0.0f;

    //TimeDelta = m_Frequency.LowPart / ;

	m_Start = m_End;

	

    //static FLOAT ftep = 1.0f;
    //static FLOAT fMin;

    //if( g_fSecPerFrame < ftep )
    //{
    //fMin = g_fSecPerFrame;
    //ftep = g_fSecPerFrame;
    //} 
    //return TRUE;

	//static float ftep = 1.0f;
	//static float fMin;
	//if( TimeDelta < ftep )
	//{
	//	fMin = TimeDelta;
	//	ftep = TimeDelta;
	//}
}

float TKTime::GetTimeDelta(void)
{
    return m_TimeDelta;
}

float TKTime::GetFPS(void)
{
    return m_fFramePerSec;
}

