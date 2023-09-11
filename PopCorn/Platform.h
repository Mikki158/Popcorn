#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Falling_Letter.h"
#include "Ball_Set.h"

enum class EPlatform_State: unsigned char
{
    Regular,
    Meltdown,
    Rolling,
    Glue
};


enum class EPlatform_Substate_Regular: unsigned char
{
    Unknow,

    Missing,
    Ready,
    Normal
};


enum class EPlatform_Substate_Meltdown: unsigned char
{
    Unknow,

    Init,
    Active
};


enum class EPlatform_Substate_Rolling: unsigned char
{
    Unknow,

    Roll_In,
    Expand_Roll_In
};


enum class EPlatform_Substate_Glue: unsigned char
{
    Unknow,

    Init,
    Active,
    Finalize
};


enum class EPlatform_Moving_State: unsigned char
{
    Stoping,
    Stop,
    Moving_Left,
    Moving_Right
};


class AsPlatform_State
{
public:
    AsPlatform_State();

    operator EPlatform_State() const;
    void operator = (EPlatform_State new_state);

    EPlatform_Substate_Regular Regular;
    EPlatform_Substate_Meltdown Meltdown;
    EPlatform_Substate_Rolling Rolling;
    EPlatform_Substate_Glue Glue;

    EPlatform_Moving_State Moving;

private:
    EPlatform_State Current_State;
};


class AsPlatform: public AHit_Checker, public AMover, public AGraphics_Object
{
public:
    int Width;


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

    void Init(AsBall_Set* ball_set);
    void Redraw_Platform(bool update_rect = true);
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
    int Inner_width;
    int Meltdown_Y_Pos;
    int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
    int* Normal_Platform_Image;// Пиксели изображения платформы на фоне
    double X_Pos;
    double Speed;
    double Glue_Spot_Height_Ratio;
    bool Left_Key_Down, Right_Key_Down;


    static const int NORMAL_WIDTH = 28; 
    int Meltdown_Platform_Y_Pos[NORMAL_WIDTH * AsConfig::GLOBAL_SCALE];

    
    AsPlatform_State Platform_State;

    RECT Platform_Rect, Prev_Platform_Rect;

    AColor Platform_Circle_Color, Platform_Inner_Color;

    AsBall_Set* Ball_Set;

    static const int CIRCLE_SIZE = 7;
    static const int Height = 7;
    static const int NORMAL_PLATFORM_INNER_WIDTH = NORMAL_WIDTH - CIRCLE_SIZE;
    static const int MAX_ROLLING_STEP = 16;
    static const int ROLL_IN_PLATFORM_END_X_POS = 99;
    static const int ROLLING_PLATFORM_SPEED = 2;
    static const int X_Step = 6;
    static const double Max_Glue_Spot_Height_Ratio;
    static const double Min_Glue_Spot_Height_Ratio;
    static const double Glue_Spot_Height_Ratio_Step;

    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Get_Normal_Platform_Image(HDC hdc);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);  
    void Draw_Rolling_State(HDC hdc, RECT& paint_area);
    void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
    void Draw_Glue_State(HDC hdc, RECT &paint_area);
    void Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height);
    void Act_For_Meltdown_State();
    void Act_For_Rolling_State();
    void Act_For_Glue_State();

    bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball);
    bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor** color, int& stroke_len);
};