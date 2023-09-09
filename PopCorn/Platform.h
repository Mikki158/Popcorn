#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Falling_Letter.h"
#include "Ball_Set.h"

enum EPlatform_State
{
    EPS_Missing,
    EPS_Ready,
    EPS_Normal,
    EPS_Pre_Meltdown,
    EPS_Meltdown,
    EPS_Roll_In,
    EPS_Expand_Roll_In,
    EPS_Glue_Init,
    EPS_Glue,
    EPS_Glue_Finalize

};


enum EPlatform_Moving_State
{
    EPMS_Stoping,
    EPMS_Stop,
    EPMS_Moving_Left,
    EPMS_Moving_Right
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
    void Move(bool to_left, bool key_down);
    void On_Space_Key(bool key_down);

    bool Hit_by(AFalling_Letter* falling_letter);

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

    EPlatform_State Platform_State;
    EPlatform_Moving_State Platform_Moving_State;

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

    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Get_Normal_Platform_Image(HDC hdc);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);    
    void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
    void Draw_Expanding_Roll_In_State(HDC hdc, RECT& paint_area);
    void Draw_Glue_State(HDC hdc, RECT &paint_area);
    void Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height);
    
    bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball);
    bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor** color, int& stroke_len);
};