#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Ball.h"
#include "Falling_Letter.h"

enum EPlatform_State
{
    EPS_Missing,
    EPS_Ready,
    EPS_Normal,
    EPS_Meltdown,
    EPS_Roll_In,
    EPS_Expand_Roll_In,

};

class AsPlatform: public AHit_Checker
{
public:
    int X_Pos;
    int Width;

    AsPlatform();
    ~AsPlatform();

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

    void Redraw_Platform();
    void Draw(HDC hdc, RECT& paint_area);
    void Set_State(EPlatform_State new_state);
    void Act();
    void Move(bool to_left);

    bool Hit_by(AFalling_Letter* falling_letter);

    EPlatform_State Get_State();

private:
    int X_Step;
    int Rolling_Step;
    int Inner_width;
    int Meltdown_Y_Pos;
    int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
    int* Normal_Platform_Image;// Пиксели изображения платформы на фоне

    static const int NORMAL_WIDTH = 28; 
    int Meltdown_Platform_Y_Pos[NORMAL_WIDTH * AsConfig::GLOBAL_SCALE];

    EPlatform_State Platform_State;

    RECT Platform_Rect, Prev_Platform_Rect;

    AColor Platform_Circle_Color, Platform_Inner_Color;

    static const int CIRCLE_SIZE = 7;
    static const int Height = 7;
    static const int NORMAL_PLATFORM_INNER_WIDTH = NORMAL_WIDTH - CIRCLE_SIZE;
    static const int MAX_ROLLING_STEP = 16;
    static const int ROLL_IN_PLATFORM_END_X_POS = 99;
    static const int ROLLING_PLATFORM_SPEED = 2;

    void Clear_BG(HDC hdc);
    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);    
    void Draw_Roll_In_State(HDC hdc, RECT paint_area);
    void Draw_Expanding_Roll_In_State(HDC hdc, RECT paint_area);
    
    bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball);
    bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor** color, int& stroke_len);
};