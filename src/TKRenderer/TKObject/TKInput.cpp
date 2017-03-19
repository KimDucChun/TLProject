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

    // ���̷�Ʈ ��ǲ ��ü ����
    if ( FAILED( DirectInput8Create(GetEngine()->GethInstance(),
                                    DIRECTINPUT_VERSION,
                                    IID_IDirectInput8,
                                    (void**)&pDI,
                                    NULL) ) )
    {
        return;
    }

    // Ű���� ��ü ����
    if ( FAILED ( pDI->CreateDevice(GUID_SysKeyboard, &pDIDeviceKey, NULL ) ) ) // Ű���� ��ü����
        return;
    // Ű���� ��ü ������ ���� ����
    if ( FAILED ( pDIDeviceKey->SetDataFormat(&c_dfDIKeyboard) ) )
        return;
    // ��ġ�� �ν��Ͻ��� ���� ���� ������ Ȯ���Ѵ�. ���� ������, �� �ν��Ͻ���, ���� ��ġ�� �ٸ� �ν��Ͻ�, �� �ý����� �̿��� �κа��� ���̷�, ��� ��ȯ�ұ �����Ѵ�.
    // DISCL_FOREGROUND : ���ø����̼��� foreground �׼������� �䱸�Ѵ�. foreground �׼������� �ο��Ǹ� �Ҵ�� �����찡 ��׶���� �̵��� ��, ��ġ�� �ڵ������� ������ �ȴ�.
    // DISCL_NONEXCLUSIVE : ���ø����̼��� ���Ÿ �׼������� �䱸�Ѵ�. ��ġ�� ���� �׼�������, ���� ��ġ�� �׼��� �ϰ� �ִ� �ٸ� ���ø����̼��� �������� �ʴ´�.
    // DISCL_NOWINKEY : Microsoft Windows�� Ű�� ��ȿ�� �Ѵ�. �� �÷��׸� ���� �ϸ�, ����ڴ� �߸��� ���ø����̼��� �����ϴ� ���� ��������. �ٸ�, ����Ʈ�� �׼� ���� ����� �������̽� (UI)�� ǥ�õǰ� �ִ� ������ DISCL_NOWINKEY �÷��״� ��ȿ�̸�, ����Ʈ�� �׼� ���� UI �� �����ϰ� �ִ� ������ Windows Ű�� ���� ��� ����Ѵ�.
    if ( FAILED ( pDIDeviceKey->SetCooperativeLevel(GetEngine()->GethWnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY ) ) )
        return;
    // ��ġ�� ���������� ���ѷ���
    while (pDIDeviceKey->Acquire() == DIERR_INPUTLOST);


    // ���콺 ��ü ����
    if ( FAILED ( pDI->CreateDevice(GUID_SysMouse, &pDIDeviceMouse, NULL ) ) ) // Ű���� ��ü����
        return;
    // Ű���� ��ü ������ ���� ����
    if ( FAILED ( pDIDeviceMouse->SetDataFormat(&c_dfDIMouse) ) )
        return;
    // ��ġ�� �ν��Ͻ��� ���� ���� ������ Ȯ���Ѵ�. ���� ������, �� �ν��Ͻ���, ���� ��ġ�� �ٸ� �ν��Ͻ�, �� �ý����� �̿��� �κа��� ���̷�, ��� ��ȯ�ұ �����Ѵ�.
    // DISCL_FOREGROUND : ���ø����̼��� foreground �׼������� �䱸�Ѵ�. foreground �׼������� �ο��Ǹ� �Ҵ�� �����찡 ��׶���� �̵��� ��, ��ġ�� �ڵ������� ������ �ȴ�.
    // DISCL_NONEXCLUSIVE : ���ø����̼��� ���Ÿ �׼������� �䱸�Ѵ�. ��ġ�� ���� �׼�������, ���� ��ġ�� �׼��� �ϰ� �ִ� �ٸ� ���ø����̼��� �������� �ʴ´�.
    if ( FAILED ( pDIDeviceMouse->SetCooperativeLevel(GetEngine()->GethWnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ) ) )
        return;
    // ��ġ�� ���������� ���ѷ���
    while (pDIDeviceMouse->Acquire() == DIERR_INPUTLOST);
}

void TKInput::Frame(void)
{
    // Ű���� ���� �� ����
    memcpy(bLastKeyDown, bKeyDown, sizeof(bLastKeyDown));

    // ���콺 ���� �� ����
    memcpy(bLastMouseDown, bMouseDown, sizeof(bLastMouseDown));
    memcpy(&LastAPIMousePos, &APIMousePos, sizeof(POINT));
    
    // Ű���� �Է� ����
    ZeroMemory(pKeyState, sizeof(pKeyState));
    hResult = pDIDeviceKey->GetDeviceState(sizeof(pKeyState), pKeyState);
    if ( FAILED (hResult) )
    {
        while( pDIDeviceKey->Acquire() == DIERR_INPUTLOST );
    }

    // ���콺 �Է� ����
    ZeroMemory(&DIMouseState, sizeof(DIMOUSESTATE));
    hResult = pDIDeviceMouse->GetDeviceState(sizeof(DIMouseState), &DIMouseState);
    if ( FAILED (hResult) )
    {
        while (pDIDeviceMouse->Acquire() == DIERR_INPUTLOST );
    }

    // API ���� ���콺 ��ġ ����
    ZeroMemory(&APIMousePos, sizeof(POINT));
    
    GetCursorPos(&APIMousePos);
    ScreenToClient(GetEngine()->GethWnd(), &APIMousePos);
    APIMousePos.x += nOffsetX;
    APIMousePos.y += nOffsetY;

    // �̵��� �� ���
    APIMouseMove.x = APIMousePos.x - LastAPIMousePos.x;
    APIMouseMove.y = APIMousePos.y - LastAPIMousePos.y;

    // Ű���� �� ó��
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

    // ���콺 �� ó��
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