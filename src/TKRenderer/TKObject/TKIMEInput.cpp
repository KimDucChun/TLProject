#include "TKIMEInput.h"
#include "TKEngine.h"

TKIMEInput::TKIMEInput(TKEngine *pEngine)
:pEngine(NULL)
{
    this->pEngine = pEngine;

	memset(CompStr, 0, sizeof(CompStr)); 
	memset(ResultStr, 0, sizeof(ResultStr)); 
	memset(&EngStr, 0, sizeof(&EngStr));
}

TKIMEInput::~TKIMEInput(void)
{
	
}

int TKIMEInput::OnComposition(WPARAM wParam, LPARAM lParam)
{
	m_hImc = ImmGetContext(GetEngine()->GethWnd());

	if(lParam & GCS_COMPSTR)
	{
		int StrSize = ImmGetCompositionStringW(m_hImc, GCS_COMPSTR, NULL, 0);

		ImmGetCompositionStringW(m_hImc, GCS_COMPSTR, CompStr, StrSize);

		CompStr[StrSize] = 0;


	}
	else if(lParam & GCS_RESULTSTR)
	{
		int StrSize = ImmGetCompositionStringW(m_hImc, GCS_RESULTSTR, NULL, 0);

		ImmGetCompositionStringW(m_hImc, GCS_RESULTSTR, ResultStr, StrSize);

		ResultStr[StrSize] = 0;
	}
	else if(lParam & GCS_COMPATTR)
	{

	}

	ImmReleaseContext(GetEngine()->GethWnd(), m_hImc);
	return 0;
}

int TKIMEInput::OnStartComposition(WPARAM wParam, LPARAM lParam)
{
	return 0; 
}

int TKIMEInput::OnEndComposition(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int TKIMEInput::OnNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int TKIMEInput::OnChar(WPARAM wParam, LPARAM lParam)
{
	strcpy(&EngStr, (char*)&wParam);

	return 0;
}

bool TKIMEInput::PutString(wchar_t* CompStr)
{
	return false;
}

bool TKIMEInput::CleanCompStr(void)
{ 
	memset(CompStr, 0, sizeof(CompStr)); 
	return true;
}

bool TKIMEInput::CleanResultStr(void)
{ 
	memset(ResultStr, 0, sizeof(ResultStr)); 
	return true;
}

bool TKIMEInput::CleanEngStr(void)
{ 
	memset(&EngStr, 0, sizeof(&EngStr)); 
	return true;
}