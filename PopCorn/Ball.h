#pragma once


#include <math.h>
#include <Windows.h>

#include "Config.h"

//
class ABall: public ABall_Object, public AGame_Object
{
public:
    int Release_Timer_Tick;// Значение счетчика времени, после которого надо отпускать прикрепленный мячик 

    ABall();

    virtual double Get_Direction();
    virtual void Set_Direction(double new_direction);
    virtual void Set_State(EBall_State new_state, double x_pos = 0, double y_pos = 0);
    virtual EBall_State Get_State();
    virtual void Reflect(bool from_horizontal);
    virtual void Draw_Teleporting(HDC hdc, int step);
    virtual void Set_On_Parachute(int brick_x, int brick_y);
    virtual void Get_Center(double& x_pos, double& y_pos);
    virtual bool Is_Moving_Up();
    virtual bool Is_Moving_Left();


    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();
    
    void Set_For_Test();
    void Set_Speed(double new_speed);
    void Forced_Advance(double direction, double speed, double max_speed);
    void Release();

    bool Is_Test_Finished();



    static AHit_Checker_List Hit_Checker_List;



private:
    double Ball_Direction, Prev_Ball_Direction;
    double Center_X_Pos;
    double Center_Y_Pos;
    double Ball_Speed, Prev_Ball_Speed;

    int Test_Iteration;
    double Rest_Test_Distance;
    bool Testing_Is_Active;

    EBall_State Ball_State, Prev_Ball_State;

    RECT Ball_Rect, Prev_Ball_Rect;
    RECT Parachute_Rect, Prev_Parachute_Rect; 


    static const int On_Platform_Timeout = 10 * AsConfig::FPS;// Время нахождения на платформе
    static const int Parachute_Size = 15;

    void Redraw_Ball();
    void Draw_Parachute(HDC hdc, RECT &paint_area);
    void Redraw_Parachute();
    void Clear_Parachute(HDC hdc);
};
