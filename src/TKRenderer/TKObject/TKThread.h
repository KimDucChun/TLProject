#pragma once

#include "../TKObject.h"

// 임계영역 클래스
class TKCriticalSection : public TKObject
{
private:
    CRITICAL_SECTION    cs;
public:
    VOID Enter(VOID);
    VOID Leave(VOID);
    TKCriticalSection(VOID);
    ~TKCriticalSection(VOID);
};

// 기본 스레드 추상 클래스
class TKThread : public TKObject
{
protected:
    HANDLE  hThread;
    DWORD   ThreadId;

    int     SuspendCount;
    TKCriticalSection   CS;

    BOOL    FreeOnTerminate;

    BOOL    IsTerminate;

public:
    TKThread(VOID);
    virtual ~TKThread(VOID);

    DWORD Suspend(VOID);
    DWORD Resume(VOID);
    BOOL  Terminate(VOID);
    VOID  SetFreeOnTerminate(const BOOL FreeOnTerminate);

    BOOL GetSuspended(VOID);
    int  GetSuspendCount(void);

    virtual DWORD Execute(VOID) = 0;

    void SetThreadHandle(HANDLE hThread);
    HANDLE GetThreadHandle(void);
};
