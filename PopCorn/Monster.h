#pragma once

#include "Explosive_Ball.h"

enum class EMonster_State : unsigned char
{
    Missing,
    Emitting,
    Alive,
    Destroing
};

enum class EEye_State :unsigned char
{
    Closed,
    Opening,
    Starting,
    Closing
};


class AMonster: public AHit_Checker, public AGame_Object
{
public:
    AMonster();

    static const int Width = 16;
    static const int Height = 16;

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball);
    virtual bool Check_Hit(double next_x_pos, double next_y_pos);
    virtual bool Check_Hit(RECT& rect);
    

    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Act_Alive();
    void Act_Destroing();
    void Activate(int x_pos, int y_pos, bool moving_right);
    void Destroy();
    bool Is_Active();

private:
    double X_Pos, Y_Pos;

    int Start_Blink_Timeout, Totla_Animation_Timeout;
    int Next_Direction_Switch_Tick, Alive_Timer_Tick;
    double Cornea_Height;

    double Speed, Direction;

    static const int Blink_Stage_Count = 7;
    static const int Explosive_Balls_Count = 20;
    static const double Max_Cornea_Height;
    static const double Blink_Timeouts[Blink_Stage_Count];
    static const EEye_State Blink_States[Blink_Stage_Count];

    EEye_State Eye_State;
    EMonster_State Monster_State;
    RECT Prev_Monster_Rect, Monster_Rect;

    int Blink_Ticks[Blink_Stage_Count];
    AExplosive_Ball Explosive_Balls[Explosive_Balls_Count];

    void Draw_Alive(HDC hdc);
    void Draw_Destroing(HDC hdc, RECT& paint_area);
    void Redraw_Monster();
    void Get_Monster_Rect(double x_pos, double y_pos, RECT& rect);
};