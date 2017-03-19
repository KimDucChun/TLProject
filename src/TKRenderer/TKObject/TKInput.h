#pragma once
#include "TKBaseObject.h"

#define INPUTMOUSEBTNCNT 4
#define INPUTKEYCOUNT 256

enum EMouseButtonIndex
{
    embiLeftButton = 0, 
    embiRightButton = 1,
    embiCenterButton = 2
};

////////////////////////////////////////////////////////////////////////////////
// TKInput
////////////////////////////////////////////////////////////////////////////////
class TKInput : public TKBaseObject
{
protected:
    BYTE            pKeyState[INPUTKEYCOUNT];

    bool            bKeyDown[INPUTKEYCOUNT];
    bool            bLastKeyDown[INPUTKEYCOUNT];
    bool            bKeyPressed[INPUTKEYCOUNT];
    float           fKeyDownedTime[INPUTKEYCOUNT];
    float           fKeyPressedTime[INPUTKEYCOUNT];

    bool            bMouseDown[INPUTMOUSEBTNCNT];
    bool            bLastMouseDown[INPUTMOUSEBTNCNT];
    float           fMouseDownedTime[INPUTMOUSEBTNCNT];

    DIMOUSESTATE    DIMouseState;
    POINT           APIMousePos;
    POINT           LastAPIMousePos;
    POINT           APIMouseMove;

    HRESULT                 hResult;
    LPDIRECTINPUT8          pDI;
    LPDIRECTINPUTDEVICE8    pDIDeviceKey;
    LPDIRECTINPUTDEVICE8    pDIDeviceMouse;
    
    int nOffsetX;
    int nOffsetY;

public:
    TKInput(TKEngine *pEngine);
    virtual ~TKInput(void);

    void Init(void);
    void Frame(void);
    void Release(void);

    BYTE*   KeyState(void)              { return pKeyState; }
    DIMOUSESTATE* GetDIMouseInfo(void)  { return &DIMouseState; }
    POINT* GetAPIMouseInfo(void)        { return &APIMousePos; }
    POINT* GetAPIMouseMove(void)        { return &APIMouseMove; }

    bool KeyDown(const BYTE Key)        { return ( bKeyDown[Key] && !bLastKeyDown[Key]); }
    bool KeyUp(const BYTE Key)          { return (!bKeyDown[Key] && bLastKeyDown[Key]); }
    bool KeyPressed(const BYTE Key)     { return ( bKeyPressed[Key] ); }

    bool MouseButtonDown(const EMouseButtonIndex Index)     { return (bMouseDown[Index] && !bLastMouseDown[Index]); }
    bool MouseButtonUp(const EMouseButtonIndex Index)       { return (!bMouseDown[Index] && bLastMouseDown[Index]); }
    bool MouseButtonPressed(const EMouseButtonIndex Index)  { return (DIMouseState.rgbButtons[Index] != 0); }
};

////////////////////////////////////////////////////////////////////////////////