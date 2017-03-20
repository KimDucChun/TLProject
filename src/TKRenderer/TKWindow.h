#pragma once

#include "TKCommon.h"

class TKEngine2;

class TKWindow
{
protected:
    WNDCLASS    wc;
    MSG         msg;
    HWND        m_hWnd;

    HINSTANCE   m_hInstance;
    HINSTANCE   m_hPrevInstance;
    LPSTR       m_lpCmdLine;
    INT         m_nCmdShow;

    LPTSTR      nResMenu;

public:
    void InitWndClass();
    void InitWndInstance();
    
	void Loop(void);

    TKWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow);
    virtual ~TKWindow(void);

	MSG*		GetMSG(void);
	HWND		GetHWND(void);
	HINSTANCE	GetHInstance(void);

	void SetMainMenu(LPTSTR nResMenu);
};