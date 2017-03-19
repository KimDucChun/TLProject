#pragma once

#include "../TKObject.h"

#pragma comment (lib, "imm32.lib")

#include <imm.h>
#include <dimm.h>

class TKIMEInput : public TKObject
{
public:
	TKEngine *pEngine;

	HIMC m_hImc;

	int s_nCompCaret;
	char EngStr;
	wchar_t CompStr[10];
	wchar_t ResultStr[10];

public:
	int OnComposition(WPARAM, LPARAM);
	int OnEndComposition(WPARAM, LPARAM);
	int OnStartComposition(WPARAM, LPARAM);
	int OnNotify(WPARAM, LPARAM);
	int OnChar(WPARAM, LPARAM);

	bool PutString(wchar_t* CompStr);

public:
	wchar_t* GetCompStr(void) { return CompStr; }
	wchar_t* GetResultStr(void) { return ResultStr; }
	char* GetEngStr(void) { return &EngStr; }

	bool CleanCompStr(void);
	bool CleanResultStr(void);
	bool CleanEngStr(void);

public:
	TKIMEInput(TKEngine *pEngine);
	~TKIMEInput(void);
};
