#include "Config.h"

int AsConfig::Current_Timer_Tick = 0;
bool AsConfig::Level_Has_Floor = false;

const double AsConfig::STEP_SIZE = 1.0 / GLOBAL_SCALE;

HWND AsConfig::HWnd;
HPEN AsConfig::BG_Pen;
HBRUSH AsConfig::BG_Brush;

HPEN AsConfig::Brick_Pink_Pen, AsConfig::Brick_Blue_Pen, AsConfig::Letter_Pen, AsConfig::Brick_White_Pen;
HBRUSH AsConfig::Brick_Pink_Brush, AsConfig::Brick_Blue_Brush, AsConfig::Brick_White_Brush;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Pink_Brick_Color(255, 85, 255);
const AColor AsConfig::Blue_Brick_Color(85, 255, 255);
const AColor AsConfig::White_Brick_Color(255, 255, 255);

int AColor::Get_RGB() const
{
    return RGB(R, G, B);
}

AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
    :R(r), G(g), B(b)
{

}

void AsConfig::Setup_Color()
{
    AsConfig::Create_Pen_Brush(AsConfig::BG_Color, AsConfig::BG_Pen, AsConfig::BG_Brush);

    AsConfig::Letter_Pen = CreatePen(PS_SOLID, AsConfig::GLOBAL_SCALE, RGB(255, 255, 255));
    AsConfig::Create_Pen_Brush(AsConfig::Pink_Brick_Color, AsConfig::Brick_Pink_Pen, AsConfig::Brick_Pink_Brush);
    AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, AsConfig::Brick_Blue_Pen, AsConfig::Brick_Blue_Brush);

    AsConfig::Create_Pen_Brush(AsConfig::White_Brick_Color, AsConfig::Brick_White_Pen, AsConfig::Brick_White_Brush);
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
{// Вычисляет псевдослучайное число в диапазоне [0, ... range - 1]
    return rand() * range / RAND_MAX;
}

