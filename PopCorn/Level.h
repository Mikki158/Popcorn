#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include"Config.h"
#include"Active_Brick.h"

enum ELetter_Type
{
    ELT_None,

    ELT_O
};

class ALevel
{
public:
    AActive_Brick Active_Brick;

    ALevel();

    void Init();

    void Check_Level_Brick_Heat(int& next_y_pos, double& ball_direction);
    void Draw(HDC hdc, RECT& paint_area, HWND hwnd);

private:

    static char level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];



    HPEN Brick_Pink_Pen, Brick_Blue_Pen, Letter_Pen;
    HBRUSH Brick_Pink_Brush, Brick_Blue_Brush;

    RECT Level_Rect;

    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
};
