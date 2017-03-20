#include "TKWindow.h"
#include "TKObject/TKIMEInput.h"
#include "TKObject/TKEngine.h"

//static TKIMEInput *g_pIMEInput = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( uMsg )
    {
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            break;

        case WM_IME_STARTCOMPOSITION:
            //g_pIMEInput->OnStartComposition(wParam, lParam);
            return 0;
            break;

        case WM_IME_COMPOSITION:
            //g_pIMEInput->OnComposition(wParam, lParam);
            return 0;
            break;

        case WM_IME_ENDCOMPOSITION:
            //g_pIMEInput->OnEndComposition(wParam, lParam);
            return 0;
            break;

        case WM_IME_NOTIFY:
            //g_pIMEInput->OnNotify(wParam, lParam);
            return 0;
            break;

        case WM_CHAR:
            //g_pIMEInput->OnChar(wParam, lParam);
            break;

        case WM_IME_SETCONTEXT:
            lParam = 0;
            break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

TKWindow::TKWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
:nResMenu(0)
{
    ZeroMemory(&wc, sizeof(WNDCLASS));
    ZeroMemory(&msg, sizeof(MSG));

    this->m_hInstance = hInstance;
    this->m_hPrevInstance = hPrevInstance;
    this->m_lpCmdLine = lpCmdLine;
    this->m_nCmdShow = nCmdShow;
}

TKWindow::~TKWindow(void)
{
    UnregisterClass(NULL, wc.hInstance);
}

void TKWindow::InitWndClass()
{
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = WindowProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = m_hInstance;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName     = (LPCTSTR)nResMenu;
    wc.lpszClassName    = "KENGINE";

    RegisterClass(&wc);
}

void TKWindow::InitWndInstance()
{
    m_hWnd = CreateWindow("KENGINE",
                        "KENGINE Sample",
                        WS_CAPTION | WS_SYSMENU,
                        CW_USEDEFAULT, 
                        CW_USEDEFAULT, 
                        SCREEN_WIDTH + GetSystemMetrics(SM_CXBORDER) * 2, 
                        SCREEN_HEIGHT + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2,
                        NULL,
                        NULL,
                        m_hInstance,
                        NULL);

    ShowWindow( m_hWnd, m_nCmdShow );
    UpdateWindow( m_hWnd );
}

void TKWindow::Loop(void)
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (GetEngine())
				GetEngine()->Frame();
		}
	}
}


MSG* TKWindow::GetMSG(void)
{
	return &msg;
}

HWND TKWindow::GetHWND(void)
{
	return m_hWnd;
}

HINSTANCE TKWindow::GetHInstance(void)
{
	return m_hInstance;
}

void TKWindow::SetMainMenu(LPTSTR nResMenu)
{
	this->nResMenu = nResMenu;
}

