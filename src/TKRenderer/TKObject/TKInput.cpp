#include "./TKInput.h"
#include "./TKTime.h"
#include "./TKEngine.h"

////////////////////////////////////////////////////////////////////////////////
// TKInput
////////////////////////////////////////////////////////////////////////////////
TKInput::TKInput(TKEngine *pEngine)
:TKBaseObject(pEngine, -1)
{
}

TKInput::~TKInput(void)
{
}

void TKInput::Init(void)
{
    __super::Init();
    
    nOffsetX = 0;
    nOffsetY = 0;
    LONG WindowStyle = GetWindowLong( GetEngine()->GethWnd(), GWL_STYLE );
    if ( WindowStyle | WS_BORDER )
    {
        nOffsetX += GetSystemMetrics(SM_CXBORDER);
        nOffsetY += GetSystemMetrics(SM_CYBORDER);
    }    
    if ( WindowStyle | WS_THICKFRAME )
    {
        nOffsetX += GetSystemMetrics(SM_CXSIZEFRAME);
        nOffsetY += GetSystemMetrics(SM_CYSIZEFRAME);
    }
    if ( WindowStyle | WS_SYSMENU )
    {
        nOffsetY += GetSystemMetrics(SM_CYMENUSIZE);        
    }

    memset(pKeyState, 0, sizeof(pKeyState));

    memset(bKeyDown, 0, sizeof(bKeyDown));
    memset(bLastKeyDown, 0, sizeof(bLastKeyDown));
    memset(bKeyPressed, 0, sizeof(bKeyPressed));
    memset(fKeyDownedTime, 0, sizeof(fKeyDownedTime));
    memset(fKeyPressedTime, 0, sizeof(fKeyPressedTime));

    memset(bMouseDown, 0, sizeof(bMouseDown));
    memset(bLastMouseDown, 0, sizeof(bLastMouseDown));
    memset(fMouseDownedTime, 0 ,sizeof(fMouseDownedTime));
    
    memset(&APIMousePos, 0, sizeof(POINT));
    memset(&LastAPIMousePos, 0, sizeof(POINT));
    memset(&APIMouseMove, 0, sizeof(POINT));

    // 다이렉트 인풋 객체 생성
    if ( FAILED( DirectInput8Create(GetEngine()->GethInstance(),
                                    DIRECTINPUT_VERSION,
                                    IID_IDirectInput8,
                                    (void**)&pDI,
                                    NULL) ) )
    {
        return;
    }

    // 키보드 객체 생성
    if ( FAILED ( pDI->CreateDevice(GUID_SysKeyboard, &pDIDeviceKey, NULL ) ) ) // 키보드 객체생성
        return;
    // 키보드 객체 데이터 형식 결정
    if ( FAILED ( pDIDeviceKey->SetDataFormat(&c_dfDIKeyboard) ) )
        return;
    // 장치의 인스턴스에 대한 협조 레벨을 확립한다. 협조 레벨은, 이 인스턴스가, 같은 장치의 다른 인스턴스, 및 시스템의 이외의 부분과의 사이로, 어떻게 교환할까를 결정한다.
    // DISCL_FOREGROUND : 애플리케이션은 foreground 액세스권을 요구한다. foreground 액세스권이 부여되면 할당된 윈도우가 백그라운드로 이동할 때, 장치는 자동적으로 릴리즈 된다.
    // DISCL_NONEXCLUSIVE : 애플리케이션은 비배타 액세스권을 요구한다. 장치에 대한 액세스권은, 같은 장치에 액세스 하고 있는 다른 애플리케이션을 방해하지 않는다.
    // DISCL_NOWINKEY : Microsoft Windows® 키를 무효로 한다. 이 플래그를 설정 하면, 사용자는 잘못해 애플리케이션을 종료하는 것이 없어진다. 다만, 디폴트의 액션 맵핑 사용자 인터페이스 (UI)가 표시되고 있는 동안은 DISCL_NOWINKEY 플래그는 무효이며, 디폴트의 액션 맵핑 UI 가 존재하고 있는 동안은 Windows 키는 보통 대로 기능한다.
    if ( FAILED ( pDIDeviceKey->SetCooperativeLevel(GetEngine()->GethWnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) )
        return;
    // 장치를 얻을때까지 무한루프
    while (pDIDeviceKey->Acquire() == DIERR_INPUTLOST);


    // 마우스 객체 생성
    if ( FAILED ( pDI->CreateDevice(GUID_SysMouse, &pDIDeviceMouse, NULL ) ) ) // 키보드 객체생성
        return;
    // 키보드 객체 데이터 형식 결정
    if ( FAILED ( pDIDeviceMouse->SetDataFormat(&c_dfDIMouse) ) )
        return;
    // 장치의 인스턴스에 대한 협조 레벨을 확립한다. 협조 레벨은, 이 인스턴스가, 같은 장치의 다른 인스턴스, 및 시스템의 이외의 부분과의 사이로, 어떻게 교환할까를 결정한다.
    // DISCL_FOREGROUND : 애플리케이션은 foreground 액세스권을 요구한다. foreground 액세스권이 부여되면 할당된 윈도우가 백그라운드로 이동할 때, 장치는 자동적으로 릴리즈 된다.
    // DISCL_NONEXCLUSIVE : 애플리케이션은 비배타 액세스권을 요구한다. 장치에 대한 액세스권은, 같은 장치에 액세스 하고 있는 다른 애플리케이션을 방해하지 않는다.
    if ( FAILED ( pDIDeviceMouse->SetCooperativeLevel(GetEngine()->GethWnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
        return;
    // 장치를 얻을때까지 무한루프
    while (pDIDeviceMouse->Acquire() == DIERR_INPUTLOST);
}

void TKInput::Frame(void)
{
    // 키보드 이전 값 복사
    memcpy(bLastKeyDown, bKeyDown, sizeof(bLastKeyDown));

    // 마우스 이전 값 복사
    memcpy(bLastMouseDown, bMouseDown, sizeof(bLastMouseDown));
    memcpy(&LastAPIMousePos, &APIMousePos, sizeof(POINT));
    
    // 키보드 입력 받음
    ZeroMemory(pKeyState, sizeof(pKeyState));
    hResult = pDIDeviceKey->GetDeviceState(sizeof(pKeyState), pKeyState);
    if ( FAILED (hResult) )
    {
        while( pDIDeviceKey->Acquire() == DIERR_INPUTLOST );
    }

    // 마우스 입력 받음
    ZeroMemory(&DIMouseState, sizeof(DIMOUSESTATE));
    hResult = pDIDeviceMouse->GetDeviceState(sizeof(DIMouseState), &DIMouseState);
    if ( FAILED (hResult) )
    {
        while (pDIDeviceMouse->Acquire() == DIERR_INPUTLOST );
    }

    // API 에서 마우스 위치 받음
    ZeroMemory(&APIMousePos, sizeof(POINT));
    
    GetCursorPos(&APIMousePos);
    ScreenToClient(GetEngine()->GethWnd(), &APIMousePos);
    APIMousePos.x += nOffsetX;
    APIMousePos.y += nOffsetY;

    // 이동한 값 계산
    APIMouseMove.x = APIMousePos.x - LastAPIMousePos.x;
    APIMouseMove.y = APIMousePos.y - LastAPIMousePos.y;

    // 키보드 값 처리
    for ( int iKey = 0 ; iKey < INPUTKEYCOUNT ; ++iKey )
    {
        if ( pKeyState[iKey] & (PUSHKEY|HOLDKEY) )
        {
            if ( bKeyDown[iKey] == false )
            {
                if ( fKeyDownedTime[iKey] > 0.01f )
                {
                    fKeyDownedTime[iKey] = 0.0f;
                    bKeyDown[iKey] = true;
                }
            }
            fKeyDownedTime[iKey] += R_TIMEDELTA;
        }
        else
        {
            bKeyDown[iKey] = false;
            fKeyDownedTime[iKey] = 0.0f;
        }        

        if ( pKeyState[iKey] & (HOLDKEY) )
        {
            bKeyPressed[iKey] = true;
            fKeyPressedTime[iKey] += R_TIMEDELTA;
        }
        else
        {
            bKeyPressed[iKey] = false;
            fKeyPressedTime[iKey] = 0.0f;
        }
    }

    // 마우스 값 처리
    for ( int iMouse = 0 ; iMouse < INPUTMOUSEBTNCNT ; ++iMouse )
    {
        if ( DIMouseState.rgbButtons[iMouse] != 0 )
        {
            if ( bMouseDown[iMouse] == false )
            {
                if ( fMouseDownedTime[iMouse] > 0.01f )
                {
                    fMouseDownedTime[iMouse] = 0.0f;
                    bMouseDown[iMouse] = true;
                }
            }
            fMouseDownedTime[iMouse] += R_TIMEDELTA;
        }
        else
        {
            bMouseDown[iMouse] = false;
            fMouseDownedTime[iMouse] = 0.0f;
        }        
    }
}

void TKInput::Release(void)
{
    if (pDIDeviceKey != NULL) pDIDeviceKey->Unacquire();
    SAFERELEASE(pDIDeviceKey);

    if (pDIDeviceMouse != NULL) pDIDeviceMouse->Unacquire();
    SAFERELEASE(pDIDeviceMouse);

    SAFERELEASE(pDI);
    
    __super::Release();
}