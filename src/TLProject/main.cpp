#include "../TKRenderer/TKWindow.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	TKWindow    *pWindow;   // ������ �ν��Ͻ�
	//TKEngine    *pEngine;   // ���� �ν��Ͻ�

	//						// ������ �ν��Ͻ� ����
	pWindow = new TKWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	pWindow->SetWndClass();
	pWindow->SetWndInstance();
	pWindow->Loop();

	//// ���� �ν��Ͻ� ����
	pEngine = new TKEngine(hInstance, pWindow->GetHWND());
	//pEngine->SetEngineRootDir(string("D:\\KEngineProject\\KEngine\\"));
	//pEngine->SetUserTextureDir(string("D:\\KEngineProject\\Textures\\"));
	//pEngine->SetModelDir(string("D:\\KEngineProject\\Models\\"));
	//pEngine->SetFXDir(string("D:\\KEngineProject\\KEngine\\FX\\"));

	//pEngine->GetSceneManager()->ChangeState<TKSampleScene>(NULL);   // �⺻ Scene ���� TKSampleScene ����
	//pEngine->Init();

	//// �����쿡 ���� ������ ����
	//pWindow->SetEngine(pEngine);

	//// ���� ���� ��ȸ
	//pEngine->EngineLoop();

	//// ���� ����
	//pEngine->Release();

	//// ��ü ����
	//delete pEngine;
	delete pWindow;

	//_CrtDumpMemoryLeaks();

	return 1;
}


