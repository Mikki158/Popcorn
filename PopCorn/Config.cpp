﻿#include "Config.h"

int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Level_Has_Floor = false;

HWND AsConfig::HWnd;
HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Pink_Brick_Color(255, 85, 255);
const AColor AsConfig::Blue_Brick_Color(85, 255, 255);

AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
    :R(r), G(g), B(b)
{

}

void AsConfig::Setup_Color()
{
    AsConfig::Create_Pen_Brush(AsConfig::BG_Color, AsConfig::BG_Pen, AsConfig::BG_Brush);
}

void AsConfig::Create_Pen_Brush(const AColor &color, HPEN& hPen, HBRUSH& hBrush)
{
    hPen = CreatePen(PS_SOLID, 0, RGB(color.R, color.G, color.B));
    hBrush = CreateSolidBrush(RGB(color.R, color.G, color.B));
}

void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& hPen, HBRUSH& hBrush)
{
    hPen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    hBrush = CreateSolidBrush(RGB(r, g, b));
}

int AsConfig::Rand(int range)
{
    return rand() * range / RAND_MAX;
}

