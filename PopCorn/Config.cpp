#include "Config.h"

// AsConfig
bool AsConfig::Level_Has_Floor = false;
int AsConfig::Current_Timer_Tick = 0;
HWND AsConfig::HWnd;

const double AsConfig::D_GLOBAL_SCALE = (double)GLOBAL_SCALE;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Moving_STEP_SIZE = 1.0 / GLOBAL_SCALE;
const double AsConfig::START_BALL_Y_POS = 184.0;
const double AsConfig::Ball_Accelerate = 1.001;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;

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

//
void AsConfig::Throw()
{
    throw 14;
}



// AsTools
//
void AsTools::Rect(HDC hdc, RECT& rect, const AColor& color)
{
    color.Select(hdc);
    Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}


//
void AsTools::Rect(HDC hdc, int x, int y, int width, int height, const AColor& color)
{
    int scale = AsConfig::GLOBAL_SCALE;

    color.Select(hdc);
    Rectangle(hdc, x * scale, y * scale, (x + width) * scale - 1, (y + height) * scale - 1);
}


//
void AsTools::Round_Rect(HDC hdc, RECT& rect, int corner_radius)
{
    int radius = corner_radius * AsConfig::GLOBAL_SCALE;

    RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}


//
void AsTools::Ellipse(HDC hdc, RECT& rect, const AColor& color)
{
    color.Select(hdc);

    ::Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}


//
void AsTools::Invalidate_Rect(RECT& rect)
{
    InvalidateRect(AsConfig::HWnd, &rect, FALSE);
}


//
int AsTools::Rand(int range)
{// Вычисляет псевдослучайное число в диапазоне [0, ... range - 1]
    return rand() * range / RAND_MAX;
}

