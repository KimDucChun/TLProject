#include "../TKRenderer/TKWindow.h"
#include "../TKRenderer/TKObject/TKEngine2.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	TKWindow    *pWindow;
	TKEngine2   *pEngine;

	// 윈도우 인스턴스 생성
	pWindow = new TKWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	pWindow->SetWndClass();
	pWindow->SetWndInstance();
	
	// 렌더러 인스턴스 생성
	pEngine = new TKEngine2(hInstance, pWindow->GetHWND());
	//pEngine->SetEngineRootDir(string("D:\\KEngineProject\\KEngine\\"));
	//pEngine->SetUserTextureDir(string("D:\\KEngineProject\\Textures\\"));
	//pEngine->SetModelDir(string("D:\\KEngineProject\\Models\\"));
	//pEngine->SetFXDir(string("D:\\KEngineProject\\KEngine\\FX\\"));

	//pEngine->GetSceneManager()->ChangeState<TKSampleScene>(NULL);   // 기본 Scene 으로 TKSampleScene 선택

	pEngine->Init();
	
	pWindow->Loop();

	pEngine->Release();

	delete pEngine;
	delete pWindow;

	//_CrtDumpMemoryLeaks();

	return 1;
}


