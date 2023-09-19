#pragma once

#include "Tools.h"

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
    EBT_Ad,
    EBT_Invisible
};

class AColor;
class AsEngine;



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
    static const AColor BG_Color;
    static bool Level_Has_Floor;


    // Engine
    static const int GLOBAL_SCALE = 3;
    static const int FPS = 20;
    static const int MAX_X_POS = LEVEL_X_OFFSET + CELL_WIDTH * LEVEL_WIDTH;
    static const int MAX_Y_POS = 199;
    static const int Max_Movers_Count = 10;
    static const int Max_Modules_Count = 10;
    static const int Initial_Life_Count = 5;
    static const int Max_Life_Count = 12;
    static const double D_GLOBAL_SCALE;
    static int Current_Timer_Tick;
    static HWND HWnd;

    // Border
    static const int BORDER_X_OFFSET = 6;
    static const int BORDER_Y_OFFSET = 4;
    static const int Floor_Y_Pos = MAX_Y_POS - 1;
    static const int Gate_Count = 8;

    // Ball
    static const int Max_Balls_Count = 10;
    static const double Normal_Ball_Speed;
    static const double Moving_STEP_SIZE;
    static const double START_BALL_Y_POS;
    static const double Ball_Accelerate;
    static const double Min_Ball_Angle;

    // Brick
    static const int BRICK_WIDTH = 15;
    static const int BRICK_HEIGHT = 7;
    static const int Max_Active_Bricks_Count = 10;
    static const int Max_Falling_Letters_Count = 10;
    static const int Hits_Per_Letter = 10; // Вероятность выбить букву = 1.0 / Hits_Per_Letter
    
    static const AColor Pink_Color, Blue_Color, White_Color, Letter_Color, Teleport_Portal_Color, 
        Blue_Highlight_Unbreakable, Pink_Highlight_Unbreakable, Advertisement_Pink_Table, Advertisement_Blue_Table, 
        Laser_Color, Gate_Color;


    // Platform
    static const int Platform_Y_POS = 185;
    static const int Meltdown_Speed = 3;
    static const int Platform_NORMAL_WIDTH = 28;
    static const int Platform_CIRCLE_SIZE = 7;
    static const int Platform_NORMAL_INNER_WIDTH = Platform_NORMAL_WIDTH - Platform_CIRCLE_SIZE;
    static const int Platform_Height = 7;
    static const int Platform_Expanding_Inner_Width = 12;

    static void Rect(HDC hdc, RECT& rect, const AColor& color);
    static void Rect(HDC hdc, int x, int y, int width, int height, const AColor& color);
    static void Round_Rect(HDC hdc, RECT& rect, int corner_radius = 2);
    static void Invalidate_Rect(RECT &rect);
    static void Throw();


    static int Rand(int range);
};
