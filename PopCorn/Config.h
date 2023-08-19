#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type
{
    EBT_None,

    EBT_Pink,
    EBT_Blue,
    EBT_Unbreakable,
    EBT_Multihit_1,
    EBT_Multihit_2,
    EBT_Multihit_3,
    EBT_Multihit_4,
    EBT_Parachute,
    EBT_Teleport,
    EBT_Ad
};

class AColor;
class AsEngine;

class AColor
{
public:
    unsigned char R, G, B;

    int Get_RGB() const;

    AColor(unsigned char r, unsigned char g, unsigned char b);
};

class AsConfig
{
public:
    // Lvl
    static const int LEVEL_X_OFFSET = 8;
    static const int LEVEL_Y_OFFSET = 6;
    static const int CELL_WIDTH = 16;
    static const int CELL_HEIGHT = 8;
    static const int LEVEL_WIDTH = 12; // ширина уровня в кирпичах
    static const int LEVEL_HEIGHT = 14; // высота уровня в кирпичах
    static bool Level_Has_Floor;

    // Engine
    static const int GLOBAL_SCALE = 3;
    static const int FPS = 20;
    static const int MAX_X_POS = LEVEL_X_OFFSET + CELL_WIDTH * LEVEL_WIDTH;
    static const int MAX_Y_POS = 199;
    static int Current_Timer_Tick;
    static HWND HWnd;
    static HPEN BG_Pen;
    static HBRUSH BG_Brush;

    // Border
    static const int BORDER_X_OFFSET = 6;
    static const int BORDER_Y_OFFSET = 4;

    // Ball
    static const double STEP_SIZE;

    // Brick
    static const int BRICK_WIDTH = 15;
    static const int BRICK_HEIGHT = 7;
    static const int MAX_FADE_STEP = FPS;
    static const int Max_Active_Bricks_Count = 10;
    static const int Max_Falling_Letters_Count = 10;
    static const int Hits_Per_Letter = 10; // Вероятность выбить букву = 1.0 / Hits_Per_Letter
    
    static const AColor Pink_Brick_Color, Blue_Brick_Color, BG_Color, White_Brick_Color;

    static HPEN Brick_Pink_Pen, Brick_Blue_Pen, Letter_Pen, Brick_White_Pen;
    static HBRUSH Brick_Pink_Brush, Brick_Blue_Brush, Brick_White_Brush;


    // Platform
    static const int Platform_Y_POS = 185;
    static const int Meltdown_Speed = 3;



    static void Setup_Color();
    static void Create_Pen_Brush(const AColor& color, HPEN& hPen, HBRUSH& hBrush);
    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& hPen, HBRUSH& hBrush);
    static int Rand(int range);
};
