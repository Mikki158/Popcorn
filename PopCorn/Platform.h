#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Falling_Letter.h"
#include "Platform_Glue.h"
#include "Platform_Expanding.h"
#include "Platform_Laser.h"


class AsPlatform: public AHit_Checker, public AMover, public AGraphics_Object
{
public:
    AsPlatform();
    ~AsPlatform();

    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);
    virtual double Get_Speed();

    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual void Act();
    virtual bool Is_Finished();

    void Init(AsBall_Set* ball_set, AsLaser_Beam_Set* laser_beam_set);
    void Redraw_Platform();
    void Set_State(EPlatform_State new_state);
    void Set_State(EPlatform_Substate_Regular new_regular_state);
    void Move(bool to_left, bool key_down);
    void On_Space_Key(bool key_down);

    bool Hit_by(AFalling_Letter* falling_letter);
    bool Has_State(EPlatform_Substate_Regular regular_state);

    double Get_Middle_Pos();

    EPlatform_State Get_State();

private:
    int Rolling_Step;
    int Last_Redraw_Timer_Tick;
    int Inner_width;
    int Meltdown_Y_Pos;
    int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
    int* Normal_Platform_Image;// Пиксели изображения платформы на фоне
    double X_Pos;
    double Speed;
    bool Left_Key_Down, Right_Key_Down;


    int Meltdown_Platform_Y_Pos[AsConfig::Platform_NORMAL_WIDTH * AsConfig::GLOBAL_SCALE];

    
    AsPlatform_State Platform_State;
    AsPlatform_Glue Platform_Glue;
    AsPlatform_Expanding Platform_Expanding;
    AsPlatform_Laser Platform_Laser;

    RECT Platform_Rect, Prev_Platform_Rect;

    AColor Platform_Circle_Color, Platform_Inner_Color, Gun_Color;

    AsBall_Set* Ball_Set;

    static const int MAX_ROLLING_STEP = 16;
    static const int ROLL_IN_PLATFORM_END_X_POS = 99;
    static const int ROLLING_PLATFORM_SPEED = 2;
    static const int X_Step = 6;

    bool Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation& transformatio_state);
    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Get_Normal_Platform_Image(HDC hdc);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);  
    void Draw_Rolling_State(HDC hdc, RECT& paint_area);
    void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
    void Act_For_Meltdown_State();
    void Act_For_Rolling_State();

    bool Correct_Platform_Pos();
    bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball);
    bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor** color, int& stroke_len);

    double Get_Current_Width();
};