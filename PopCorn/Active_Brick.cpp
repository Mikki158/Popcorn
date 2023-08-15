#include "Active_Brick.h"

HPEN AActive_Brick::Fading_Blue_Brick_Pens[AsConfig::MAX_FADE_STEP];
HBRUSH AActive_Brick::Fading_Blue_Brick_Brushes[AsConfig::MAX_FADE_STEP];

HPEN AActive_Brick::Fading_Pink_Brick_Pens[AsConfig::MAX_FADE_STEP];
HBRUSH AActive_Brick::Fading_Pink_Brick_Brushes[AsConfig::MAX_FADE_STEP];


//
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
    :Brick_Rect(), Brick_Type(brick_type), Fade_Step(0), level_x(level_x), level_y(level_y)
{
    Brick_Rect.left = (AsConfig::LEVEL_X_OFFSET + level_x * AsConfig::CELL_WIDTH) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.top = (AsConfig::LEVEL_Y_OFFSET + level_y * AsConfig::CELL_HEIGHT) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.right = Brick_Rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;
}


//
void AActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
    HPEN pen = 0;
    HBRUSH brush = 0;

    switch (Brick_Type)
    {
    case EBT_Pink:
        pen = Fading_Pink_Brick_Pens[Fade_Step];
        brush = Fading_Pink_Brick_Brushes[Fade_Step];
        break;

    case EBT_Blue:
        pen = Fading_Blue_Brick_Pens[Fade_Step];
        brush = Fading_Blue_Brick_Brushes[Fade_Step];
        break;

    default:
        break;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
 

    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom,
        2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);
}


//
void AActive_Brick::Act()
{
    HWND hwnd = AsConfig::HWnd;

    if (Fade_Step < AsConfig::MAX_FADE_STEP - 1)
    {
        Fade_Step += 1;
        InvalidateRect(hwnd, &Brick_Rect, FALSE);
    }    
}

bool AActive_Brick::Is_Finished()
{
    if (Fade_Step >= AsConfig::MAX_FADE_STEP - 1)
        return true;
    else
        return false;
}


//
void AActive_Brick::Setup_Color()
{
    for (int i = 0; i < AsConfig::MAX_FADE_STEP; i++)
    {
        Get_Fading_Color(AsConfig::Blue_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
        Get_Fading_Color(AsConfig::Pink_Brick_Color, i, Fading_Pink_Brick_Pens[i], Fading_Pink_Brick_Brushes[i]);
    }
}


//
unsigned char AActive_Brick::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
    return color - step * (color - bg_color) / (AsConfig::MAX_FADE_STEP - 1);
}


//
void AActive_Brick::Get_Fading_Color(const AColor &color, int step, HPEN& pen, HBRUSH &brush)
{
    unsigned char r, g, b;

    r = Get_Fading_Channel(color.R, AsConfig::BG_Color.R, step);
    g = Get_Fading_Channel(color.G, AsConfig::BG_Color.G, step);
    b = Get_Fading_Channel(color.B, AsConfig::BG_Color.B, step);

    AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
}



