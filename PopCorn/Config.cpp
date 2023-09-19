#include "Config.h"


HWND AsConfig::HWnd;
//HPEN AsConfig::BG_Pen;
//HBRUSH AsConfig::BG_Brush;


//HPEN AsConfig::Brick_Pink_Pen, AsConfig::Brick_Blue_Pen, AsConfig::Letter_Pen, AsConfig::Brick_White_Pen;
//HBRUSH AsConfig::Brick_Pink_Brush, AsConfig::Brick_Blue_Brush, AsConfig::Brick_White_Brush;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Pink_Color(255, 85, 255);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Teleport_Portal_Color(AsConfig::Blue_Color, AsConfig::BG_Color, AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Blue_Highlight_Unbreakable(AsConfig::Blue_Color, AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Pink_Highlight_Unbreakable(AsConfig::Pink_Color, 3 * AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Advertisement_Pink_Table(AsConfig::Pink_Color, 2 * AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Advertisement_Blue_Table(0, 159, 159, AsConfig::GLOBAL_SCALE);
const AColor AsConfig::Gate_Color(AsConfig::White_Color, AsConfig::GLOBAL_SCALE);



// AsConfig
bool AsConfig::Level_Has_Floor = true;
int AsConfig::Current_Timer_Tick = 0;

const double AsConfig::D_GLOBAL_SCALE = (double)GLOBAL_SCALE;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Moving_STEP_SIZE = 1.0 / GLOBAL_SCALE;
const double AsConfig::START_BALL_Y_POS = 184.0;
const double AsConfig::Ball_Accelerate = 1.001;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;



//
void AsConfig::Rect(HDC hdc, RECT& rect, const AColor &color)
{
    color.Select(hdc);
    Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}


//
void AsConfig::Rect(HDC hdc, int x, int y, int width, int height, const AColor& color)
{
    color.Select(hdc);
    Rectangle(hdc, x * GLOBAL_SCALE, y * GLOBAL_SCALE, (x + width) * GLOBAL_SCALE - 1, (y + height) * GLOBAL_SCALE - 1);
}

 
//
void AsConfig::Round_Rect(HDC hdc, RECT& rect, int corner_radius)
{
    int radius = corner_radius * AsConfig::GLOBAL_SCALE;

    RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}


//
void AsConfig::Invalidate_Rect(RECT& rect)
{
    InvalidateRect(HWnd, &rect, FALSE);
}


//
void AsConfig::Throw()
{
    throw 14;
}


//
int AsConfig::Rand(int range)
{// Вычисляет псевдослучайное число в диапазоне [0, ... range - 1]
    return rand() * range / RAND_MAX;
}

