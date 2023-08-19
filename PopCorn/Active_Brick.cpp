#include "Active_Brick.h"

HPEN AActive_Brick_Pink_Blue::Fading_Blue_Brick_Pens[AsConfig::MAX_FADE_STEP];
HBRUSH AActive_Brick_Pink_Blue::Fading_Blue_Brick_Brushes[AsConfig::MAX_FADE_STEP];

HPEN AActive_Brick_Pink_Blue::Fading_Pink_Brick_Pens[AsConfig::MAX_FADE_STEP];
HBRUSH AActive_Brick_Pink_Blue::Fading_Pink_Brick_Brushes[AsConfig::MAX_FADE_STEP];

// AGraphics_Object
//
AGraphics_Object::~AGraphics_Object()
{

}



// AActive_Brick
//
AActive_Brick::~AActive_Brick()
{
}


//
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
    :Brick_Rect(), Brick_Type(brick_type)
{
    Brick_Rect.left = (AsConfig::LEVEL_X_OFFSET + level_x * AsConfig::CELL_WIDTH) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.top = (AsConfig::LEVEL_Y_OFFSET + level_y * AsConfig::CELL_HEIGHT) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.right = Brick_Rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;
}



// AActive_Brick_Pink_Blue
//
AActive_Brick_Pink_Blue::AActive_Brick_Pink_Blue(EBrick_Type brick_type, int level_x, int level_y)
    :AActive_Brick(brick_type, level_x, level_y), fade_Step(0)
{
    if (brick_type != EBT_Blue && brick_type != EBT_Pink)
        throw 13;
}


//
AActive_Brick_Pink_Blue::~AActive_Brick_Pink_Blue()
{
}


//
void AActive_Brick_Pink_Blue::Draw(HDC hdc, RECT& paint_area)
{
    HPEN pen = 0;
    HBRUSH brush = 0;

    switch (Brick_Type)
    {
    case EBT_Pink:
        pen = Fading_Pink_Brick_Pens[fade_Step];
        brush = Fading_Pink_Brick_Brushes[fade_Step];
        break;

    case EBT_Blue:
        pen = Fading_Blue_Brick_Pens[fade_Step];
        brush = Fading_Blue_Brick_Brushes[fade_Step];
        break;

    default:
        break;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
 

    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right - 1, Brick_Rect.bottom - 1,
        2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);
}


//
void AActive_Brick_Pink_Blue::Act()
{    
    if (fade_Step < AsConfig::MAX_FADE_STEP - 1)
    {
        fade_Step += 1;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}

bool AActive_Brick_Pink_Blue::Is_Finished()
{
    if (fade_Step >= AsConfig::MAX_FADE_STEP - 1)
        return true;
    else
        return false;
}


//
void AActive_Brick_Pink_Blue::Setup_Color()
{
    for (int i = 0; i < AsConfig::MAX_FADE_STEP; i++)
    {
        Get_Fading_Color(AsConfig::Blue_Brick_Color, i, Fading_Blue_Brick_Pens[i], Fading_Blue_Brick_Brushes[i]);
        Get_Fading_Color(AsConfig::Pink_Brick_Color, i, Fading_Pink_Brick_Pens[i], Fading_Pink_Brick_Brushes[i]);
    }
}


//
unsigned char AActive_Brick_Pink_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
    return color - step * (color - bg_color) / (AsConfig::MAX_FADE_STEP - 1);
}


//
void AActive_Brick_Pink_Blue::Get_Fading_Color(const AColor &color, int step, HPEN& pen, HBRUSH &brush)
{
    unsigned char r, g, b;

    r = Get_Fading_Channel(color.R, AsConfig::BG_Color.R, step);
    g = Get_Fading_Channel(color.G, AsConfig::BG_Color.G, step);
    b = Get_Fading_Channel(color.B, AsConfig::BG_Color.B, step);

    AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
}



// AActive_Brick_Unbreakable
//
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(int level_x, int level_y)
    :AActive_Brick(EBT_Unbreakable, level_x, level_y), Unbreakable_Animation_Step(0)
{
    Brick_Rect.left = (AsConfig::LEVEL_X_OFFSET + level_x * AsConfig::CELL_WIDTH) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.top = (AsConfig::LEVEL_Y_OFFSET + level_y * AsConfig::CELL_HEIGHT) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.right = Brick_Rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;
}


//
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
}


//
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
    //HPEN pen = 0;
    //HBRUSH brush = 0;

    //switch (Brick_Type)
    //{
    //case EBT_Pink:
    //    pen = Fading_Pink_Brick_Pens[fade_Step];
    //    brush = Fading_Pink_Brick_Brushes[fade_Step];
    //    break;

    //case EBT_Blue:
    //    pen = Fading_Blue_Brick_Pens[fade_Step];
    //    brush = Fading_Blue_Brick_Brushes[fade_Step];
    //    break;

    //default:
    //    break;
    //}

    //SelectObject(hdc, pen);
    //SelectObject(hdc, brush);


    //RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right - 1, Brick_Rect.bottom - 1,
    //    2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);


}


//
void AActive_Brick_Unbreakable::Act()
{
    if (Unbreakable_Animation_Step <= MAX_UNBREAKABLE_ANIMATION_STEP)
    {
        Unbreakable_Animation_Step++;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}


//
bool AActive_Brick_Unbreakable::Is_Finished()
{
    //if (fade_Step >= AsConfig::MAX_FADE_STEP - 1)
    //    return true;
    //else
        return false;
}




