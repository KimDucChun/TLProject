#pragma once

#include "TKBaseObject.h"

////////////////////////////////////////////////////////////////////////////////
// TKTime
////////////////////////////////////////////////////////////////////////////////
class TKTime : public TKBaseObject
{
private:
	LARGE_INTEGER   m_Start;
    LARGE_INTEGER	m_End;
    LARGE_INTEGER	m_Frequency;
    LARGE_INTEGER   m_FreqOnSec;

    float           m_TimeDelta;
    int				m_iMilliSecend;
    unsigned		m_uiCurrentTime;
    unsigned		m_uiPreTime;
    float			m_fFramePerSec;
    int				m_iFPSCnt;
    
public:
    TKTime(TKEngine *pEngine);
    virtual ~TKTime(void);
    
    void Init(void);
    void Frame(void);
    
    float GetTimeDelta(void);
    float GetFPS(void);
};