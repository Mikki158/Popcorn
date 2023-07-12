#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Config.h"
#include "Level.h"

enum EBall_State
{
    EBS_Normal,
    EBS_Lost,
    EBS_On_Platform
};

class ABall
{
public:
    ABall();

    void Init();
    
    void Draw(HDC hdc, RECT& paint_area);
    void Move(ALevel* level, int platform_x_pos, int platform_width);
    void Set_State(EBall_State new_state, int x_pos);
    EBall_State Get_State();

private:
    double Ball_X_Pos;
    double Ball_Y_Pos;
    double Ball_Direction;
    double Ball_Speed;

    EBall_State Ball_State;

    RECT Ball_Rect, Prev_Ball_Rect;

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;

    static const double START_BALL_Y_POS;

    void Redraw_Ball();
};
