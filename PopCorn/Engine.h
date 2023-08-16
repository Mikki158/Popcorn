﻿#pragma once

#define _USE_MATH_DEFINES
#include <Windows.h>

#include "Config.h"
#include "Border.h"
#include "Level.h"
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

enum EGame_State
{
    EGS_Test_Ball,

    EGS_Play_Level,
    EGS_Lost_Ball,
    EGS_Restart_Level
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
    EGame_State Game_State;

    ABall Ball;
    AsLevel Level;
    AsPlatform Platform;
    AsBorder Border;
    //AActive_Brick Active_Brick;

    void Act();
    void On_Falling_Letter(AFalling_Letter* falling_letter);
};





