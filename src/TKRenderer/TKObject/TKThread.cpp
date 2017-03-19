#include "./TKThread.h"

VOID TKCriticalSection::Enter(VOID)
{
    EnterCriticalSection(&cs);
}

VOID TKCriticalSection::Leave(VOID)
{
    LeaveCriticalSection(&cs);
}

TKCriticalSection::TKCriticalSection(VOID)
{
    InitializeCriticalSection(&cs);
}

TKCriticalSection::~TKCriticalSection(VOID)
{
    DeleteCriticalSection(&cs);
}

unsigned WINAPI ThreadProc(LPVOID lpParameter)
{
    DWORD result;
    TKThread *pthis;

    pthis = (TKThread *)lpParameter;
    result = pthis->Execute();
    _endthreadex(0);

    return result;
}

TKThread::TKThread(VOID)
:hThread(0)
,SuspendCount(1)
,FreeOnTerminate(TRUE)
,IsTerminate(FALSE)
{
    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, (LPVOID)this, CREATE_SUSPENDED, (unsigned *)&ThreadId);
}

TKThread::~TKThread(VOID)
{
    DWORD ExitCode;

    if (FreeOnTerminate)
    {
        if (hThread != 0)
        {
            IsTerminate = TRUE;
            WaitForSingleObject(hThread, 100);
            GetExitCodeThread(hThread, &ExitCode);
            if (ExitCode == STILL_ACTIVE)
            {
                TerminateThread(hThread, ExitCode);
            }
        }
    }

    if (hThread != 0)
    {
        CloseHandle(hThread);
    }
}

DWORD TKThread::Suspend(VOID)
{
    DWORD result;

    if (hThread != 0)
    {
        SuspendCount = SuspendThread(hThread);
        result = SuspendCount;
    }
    else
    {
        result = -1;
    }

    return result;
}

DWORD TKThread::Resume(VOID)
{
    DWORD result;

    if (hThread != 0)
    {
        SuspendCount = ResumeThread(hThread);
        result = SuspendCount;
    }
    else
    {
        result = -1;
    }
    
    return result;
}

VOID TKThread::SetFreeOnTerminate(const BOOL FreeOnTerminate)
{
    this->FreeOnTerminate = FreeOnTerminate;
}

BOOL TKThread::GetSuspended(VOID)
{
    return (SuspendCount > 0);    
}

int TKThread::GetSuspendCount(void)
{
    return SuspendCount;
}

BOOL TKThread::Terminate(VOID)
{
    DWORD ExitCode;

    if (hThread != 0)
    {
        IsTerminate = TRUE;
        WaitForSingleObject(hThread, 100);
        GetExitCodeThread(hThread, &ExitCode);
        if (ExitCode == STILL_ACTIVE)
        {
            TerminateThread(hThread, ExitCode);
        }
        return 0;
    }
    else
    {
        return 0;
    }
}

void TKThread::SetThreadHandle(HANDLE hThread)
{
    this->hThread = hThread;
}

HANDLE TKThread::GetThreadHandle(void)
{
    return hThread;
}