#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Config.h"
#include "Level.h"

class ABall
{
public:
    ABall();

    void Init();
    
    void Draw(HDC hdc, RECT& paint_area);
    void Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width);

private:
    int Ball_X_Pos;
    int Ball_Y_Pos;
    double Ball_Speed;
    double Ball_Direction;

    RECT Ball_Rect, Prev_Ball_Rect;

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;
};
