#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Config.h"
#include "Border.h"
#include "Ball.h"
#include "Platform.h"
#include "Active_Brick.h"

// таймер
const int TIMER_ID = WM_USER + 1;

enum EKey_Type
{
    EKT_None,

    EKT_Left,
    EKT_Right,
    EKT_Space
};

class AsEngine
{
public:
    AsEngine();

    void Init_Engine(HWND hWnd);
    void Draw_Frame(HDC hdc, RECT& paint_area);
    int On_Key_Down(EKey_Type key_type);
    int On_Timer();    

private:
    ABall Ball;
    ALevel Level;
    AsPlatform Platform;
    AsBorder Border;
    AActive_Brick Active_Brick;

    HWND HWnd;

};





