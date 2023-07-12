#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Config.h"

enum EPlatform_State
{
    EPS_Missing,
    EPS_Ready,
    EPS_Normal,
    EPS_Meltdown,
    EPS_Roll_In,
    EPS_Expand_Roll_In,

};

class AsPlatform
{
public:
    int X_Pos;
    int X_Step;
    int Width;
    int Rolling_Step;

    AsPlatform();

    void Init();

    void Redraw_Platform();
    void Draw(HDC hdc, RECT& paint_area);
    void Set_State(EPlatform_State new_state);
    void Act();

    EPlatform_State Get_State();

private:
    int Inner_width;
    int Meltdown_Y_Pos;

    static const int NORMAL_WIDTH = 28;
    int Meltdown_Platform_Y_Pos[NORMAL_WIDTH * AsConfig::GLOBAL_SCALE];

    EPlatform_State Platform_State;

    RECT Platform_Rect, Prev_Platform_Rect;

    HPEN Platform_Circle_Pen, Platform_Inner_Pen;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

    static const int CIRCLE_SIZE = 7;
    static const int Height = 7;
    static const int NORMAL_PLATFORM_INNER_WIDTH = NORMAL_WIDTH - CIRCLE_SIZE;
    static const int MAX_ROLLING_STEP = 16;
    static const int ROLL_IN_PLATFORM_END_X_POS = 99;
    static const int ROLLING_PLATFORM_SPEED = 2;

    void Clear_BG(HDC hdc);
    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);
    void Draw_Roll_In_State(HDC hdc, RECT paint_area);
    void Draw_Expanding_Roll_In_State(HDC hdc, RECT paint_area);
};