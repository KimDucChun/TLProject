#include "../TKRenderer/TKWindow.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	TKWindow    *pWindow;   // 윈도우 인스턴스
	//TKEngine    *pEngine;   // 엔진 인스턴스

	//						// 윈도우 인스턴스 생성
	pWindow = new TKWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	pWindow->SetWndClass();
	pWindow->SetWndInstance();
	pWindow->Loop();

	//// 엔진 인스턴스 생성
	pEngine = new TKEngine(hInstance, pWindow->GetHWND());
	//pEngine->SetEngineRootDir(string("D:\\KEngineProject\\KEngine\\"));
	//pEngine->SetUserTextureDir(string("D:\\KEngineProject\\Textures\\"));
	//pEngine->SetModelDir(string("D:\\KEngineProject\\Models\\"));
	//pEngine->SetFXDir(string("D:\\KEngineProject\\KEngine\\FX\\"));

	//pEngine->GetSceneManager()->ChangeState<TKSampleScene>(NULL);   // 기본 Scene 으로 TKSampleScene 선택
	//pEngine->Init();

	//// 윈도우에 엔진 포인터 전달
	//pWindow->SetEngine(pEngine);

	//// 엔진 루프 순회
	//pEngine->EngineLoop();

	//// 엔진 해제
	//pEngine->Release();

	//// 객체 해제
	//delete pEngine;
	delete pWindow;

	//_CrtDumpMemoryLeaks();

	return 1;
}


