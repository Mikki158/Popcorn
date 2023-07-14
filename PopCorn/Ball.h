#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Config.h"

enum EBall_State
{
    EBS_Normal,
    EBS_Lost,
    EBS_On_Platform
};

class ABall;

class AHit_Checker
{
public:
    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball) = 0;
};

class ABall
{
public:
    static int Hit_Checkers_Count;   
    static const double RADIUS;

    ABall();

    void Init();
    
    void Draw(HDC hdc, RECT& paint_area);
    void Move();
    void Set_State(EBall_State new_state, int x_pos);
    void Set_Direction(double new_direction);
    void Reflect(bool from_horizontal);

    double Get_Direction();

    static void Add_Hit_Checker(AHit_Checker* hit_checker);
    EBall_State Get_State();

private:
    double Ball_Direction;
    double Ball_Speed;
    double Center_X_Pos;
    double Center_Y_Pos;
    double Result_Distance;

    EBall_State Ball_State;

    RECT Ball_Rect, Prev_Ball_Rect;

    HPEN Ball_Pen;
    HBRUSH Ball_Brush;

    static AHit_Checker* Hit_checkers[3];

    static const double START_BALL_Y_POS;

    void Redraw_Ball();
};
