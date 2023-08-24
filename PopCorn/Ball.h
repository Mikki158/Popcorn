#pragma once


#include <math.h>
#include <Windows.h>

#include "Config.h"

enum EBall_State
{
    EBS_Normal,
    EBS_Lost,
    EBS_On_Platform,
    EBS_On_Parachute,
    EBS_Off_Parachute,
    EBS_Teleporting
};

class AHit_Checker;
class ABall;

class AHit_Checker
{
public:
    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball) = 0;

    bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x);    
};

class ABall
{
public:
    static const double RADIUS;


    ABall();

    
    void Draw(HDC hdc, RECT& paint_area);
    void Draw_Teleporting(HDC hdc, int step);
    void Move();
    void Set_For_Test();
    void Set_State(EBall_State new_state, double x_pos, double y_pos = START_BALL_Y_POS);
    void Set_Direction(double new_direction);
    void Reflect(bool from_horizontal);
    void Set_On_Parachute(int brick_x, int brick_y);
    void Get_Center(double& x_pos, double& y_pos);

    bool Is_Test_Finished();
    bool Is_Moving_Up();
    bool Is_Moving_Left();

    double Get_Direction();

    EBall_State Get_State();

    static void Add_Hit_Checker(AHit_Checker* hit_checker);


private:
    double Ball_Direction;
    double Center_X_Pos;
    double Center_Y_Pos;
    double Result_Distance;
    double Ball_Speed;

    int Test_Iteration;
    double Rest_Test_Distance;
    bool Testing_Is_Active;

    EBall_State Ball_State, Prev_Ball_State;

    RECT Ball_Rect, Prev_Ball_Rect;
    RECT Parachute_Rect, Prev_Parachute_Rect; 

    static int Hit_Checkers_Count;

    static AHit_Checker* Hit_checkers[3];

    static const int Parachute_Size = 15;
    static const double START_BALL_Y_POS;

    void Redraw_Ball();
    void Draw_Parachute(HDC hdc, RECT &paint_area);
    void Redraw_Parachute();
    void Clear_Parachute(HDC hdc);
};
