﻿#pragma once

#include <Windows.h>

enum EBrick_Type
{
    EBT_None,

    EBT_Pink,
    EBT_Blue
};

class AColor;
class AsEngine;

class AColor
{
public:
    unsigned char R, G, B;

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

    // Border
    static const int BORDER_X_OFFSET = 6;
    static const int BORDER_Y_OFFSET = 4;

    // Ball
    static const int BALL_SIZE = 4;

    // Brick
    static const int BRICK_WIDTH = 15;
    static const int BRICK_HEIGHT = 7;
    static const int MAX_FADE_STEP = 40;
    
    static const AColor Pink_Brick_Color, Blue_Brick_Color, BG_Color;

    // Platform
    static const int Platform_Y_POS = 185;
    static const int Meltdown_Speed = 3;


    // Engine
    static const int GLOBAL_SCALE = 3;
    static const int FPS = 20;
    static int Current_Timer_Tick;
    static HWND HWnd;
    static HPEN BG_Pen;
    static HBRUSH BG_Brush;

    // Engine
    static const int MAX_X_POS = LEVEL_X_OFFSET + CELL_WIDTH * LEVEL_WIDTH;
    static const int MAX_Y_POS = 199;

    static void Setup_Color();
    static void Create_Pen_Brush(const AColor& color, HPEN& hPen, HBRUSH& hBrush);
    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& hPen, HBRUSH& hBrush);
    static int Rand(int range);
};
