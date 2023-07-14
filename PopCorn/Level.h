#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Active_Brick.h"
#include "Ball.h"

enum ELetter_Type
{
    ELT_None,

    ELT_O
};

class ALevel: public AHit_Checker
{
public:
    AActive_Brick Active_Brick;

    ALevel();

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

    void Init();
    void Draw(HDC hdc, RECT& paint_area);

private:

    static char level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];



    HPEN Brick_Pink_Pen, Brick_Blue_Pen, Letter_Pen;
    HBRUSH Brick_Pink_Brush, Brick_Blue_Brush;

    RECT Level_Rect;

    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);
    bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, double brick_left_x, double brick_right_x,
        double brick_top_y, double brick_low_y, int level_x, int level_y, ABall* ball);
    bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, double brick_left_x, double brick_right_x,
        double brick_top_y, double brick_low_y, int level_x, int level_y, ABall* ball);
    bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius);

};
