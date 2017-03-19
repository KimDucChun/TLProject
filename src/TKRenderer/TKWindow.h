#pragma once

#include "TKCommon.h"

class TKEngine;

class TKWindow
{
protected:
    TKEngine    *pEngine;

    WNDCLASS    wc;
    MSG         msg;
    HWND        hWnd;

    HINSTANCE   hInstance;
    HINSTANCE   hPrevInstance;
    LPSTR       lpCmdLine;
    INT         nCmdShow;

    LPTSTR      nResMenu;

public:
    void SetWndClass();
    void SetWndInstance();
    void SetEngine(TKEngine *pEngine);
    TKEngine * GetEngine(void);
    
    MSG* GetMSG(void);
    HWND GetHWND(void);

    void SetMainMenu(LPTSTR nResMenu);

	void Loop(void);

    TKWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow);
    virtual ~TKWindow(void);
};