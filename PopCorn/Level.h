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

class AFalling_Letter
{
public:
    bool Got_Hit;

    AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

    const ELetter_Type letter_type;
    const EBrick_Type brick_type;

    void Draw(HDC hdc, RECT& paint_area);
    void Act();

    bool Is_Finished();

private:
    int X, Y;
    int Rotation_step;
    int Next_Rotation_Tick;

    static const int Ticks_Per_Step = 4;

    RECT Letter_Cell, Prev_Letter_Cell;
    

    void Draw_Brick_Letter(HDC hdc);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
};


class ALevel: public AHit_Checker
{
public:
    //AActive_Brick Active_Brick;
    static char Level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Test_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];

    ALevel();

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

    void Init();
    void Act();
    void Set_Current_Level(char Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH]);
    void Draw(HDC hdc, RECT& paint_area);

private:
    
    double Current_Brick_Left_X, Current_Brick_Right_X;
    double Current_Brick_Top_Y, Current_Brick_Low_Y;

    char Current_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];

    int Active_Bricks_Count;
    AActive_Brick* Active_Briks[AsConfig::Max_Active_Bricks_Count];
    int Falling_Letters_Count;
    AFalling_Letter* Falling_Letters[AsConfig::Max_Falling_Letters_Count];

    RECT Level_Rect;

    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    //bool Is_Check_Horizontal_First(double next_x_pos, double next_y_pos);
    bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double& reflection_pos);
    bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double& reflection_pos);
    void On_Hit(int brick_x, int brick_y);
    bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);
    void Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type);

};
