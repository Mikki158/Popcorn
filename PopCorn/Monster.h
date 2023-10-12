#pragma once

#include "Explosive_Ball.h"

//
enum class EMonster_State : unsigned char
{
    Missing,
    Emitting,
    Alive,
    Destroing
};

//
enum class EEye_State :unsigned char
{
    Closed,
    Opening,
    Starting,
    Closing
};

//
class AMonster: public AHit_Checker, public AGame_Object
{
public:
    AMonster();
    virtual ~AMonster();

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

    void Act_Destroing();
    void Activate(double x_pos, double y_pos, bool moving_right);
    void Destroy();
    void Set_Freeze_State(bool freeze);

protected:
    EMonster_State Monster_State;
    RECT Prev_Monster_Rect, Monster_Rect;
    double X_Pos, Y_Pos;
    int Next_Direction_Switch_Tick;
    int Alive_Timer_Tick;
    double Direction;
    double Prev_Speed;

    virtual void Draw_Alive(HDC hdc) = 0;
    virtual void Act_Alive() = 0;
    virtual void On_Activation() = 0;

private:
    double Speed;
    bool Need_To_Freeze;
    static const int Explosive_Balls_Count = 20;

    std::vector<AExplosive_Ball> Explosive_Balls;

    void Draw_Destroing(HDC hdc, RECT& paint_area);
    void Redraw_Monster();
    void Get_Monster_Rect(double x_pos, double y_pos, RECT& rect);
    void Change_Direction();
};

//
class AMonster_Eye : public AMonster
{
public:
    AMonster_Eye();

private:
    EEye_State Eye_State;
    double Cornea_Height;
    int Start_Blink_Timeout;
    int Totla_Animation_Timeout;

    static const int Blink_Stage_Count = 7;
    std::vector<int> Blink_Ticks;

    static const double Max_Cornea_Height;
    static const double Blink_Timeouts[Blink_Stage_Count];
    static const EEye_State Blink_States[Blink_Stage_Count];

    virtual void Draw_Alive(HDC hdc);
    virtual void Act_Alive();
    virtual void On_Activation();

};

//
class AMonster_Comet : public AMonster
{
public:
    AMonster_Comet(); 

private:
    static const int Min_Ticks_Per_Rotation = AsConfig::FPS * 2;
    static const int Max_Ticks_Per_Rotation = AsConfig::FPS * 4;

    int Ticks_Per_Rotation;
    double Current_Angle;

    virtual void Clear(HDC hdc, RECT& paint_area);

    virtual void Draw_Alive(HDC hdc);
    virtual void Act_Alive();
    virtual void On_Activation();

};