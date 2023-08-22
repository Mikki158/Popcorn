#include "Active_Brick.h"


AColor AActive_Brick_Pink_Blue::Fading_Blue_Brick_Colors[MAX_FADE_STEP];
AColor AActive_Brick_Pink_Blue::Fading_Pink_Brick_Colors[MAX_FADE_STEP];


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
    if (! (brick_type == EBT_Blue || brick_type == EBT_Pink))
        throw 13;
}


//
AActive_Brick_Pink_Blue::~AActive_Brick_Pink_Blue()
{
}


//
void AActive_Brick_Pink_Blue::Draw(HDC hdc, RECT& paint_area)
{
    AColor* color = nullptr;

    switch (Brick_Type)
    {
    case EBT_Pink:
        color = &Fading_Pink_Brick_Colors[fade_Step];
        break;

    case EBT_Blue:
        color = &Fading_Blue_Brick_Colors[fade_Step];
        break;

    default:
        break;
    }

    if (color != nullptr)
        color->Select(hdc); 

    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right - 1, Brick_Rect.bottom - 1,
        2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);
}


//
void AActive_Brick_Pink_Blue::Act()
{    
    if (fade_Step < MAX_FADE_STEP - 1)
    {
        fade_Step += 1;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}

bool AActive_Brick_Pink_Blue::Is_Finished()
{
    if (fade_Step >= MAX_FADE_STEP - 1)
        return true;
    else
        return false;
}


//
void AActive_Brick_Pink_Blue::Setup_Color()
{
    for (int i = 0; i < MAX_FADE_STEP; i++)
    {
        Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
        Get_Fading_Color(AsConfig::Pink_Color, i, Fading_Pink_Brick_Colors[i]);
    }
}


//
void AActive_Brick_Pink_Blue::Draw_In_Level(HDC hdc, RECT brick_rect, EBrick_Type brick_type)
{// Вывод неактивного кирпича на уровень
    const AColor* color = 0;

    switch (brick_type)
    {
    case EBT_None:
        color = &AsConfig::BG_Color;
        break;

    case EBT_Pink:
        color = &AsConfig::Pink_Color;
        break;

    case EBT_Blue:
        color = &AsConfig::Blue_Color;
        break;

    default:
        AsConfig::Throw();
        return;
    }

    if(color != nullptr)
        color->Select(hdc);
    
    AsConfig::Round_Rect(hdc, brick_rect);
}


//
unsigned char AActive_Brick_Pink_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
    return color - step * (color - bg_color) / (MAX_FADE_STEP - 1);
}


//
void AActive_Brick_Pink_Blue::Get_Fading_Color(const AColor &origin_color, int step, AColor& result_color)
{
    unsigned char r, g, b;

    r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
    g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
    b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

    result_color = AColor(r, g, b);
}



// AActive_Brick_Unbreakable
AColor AActive_Brick_Unbreakable::Blue_Highlight(AsConfig::Blue_Color, AsConfig::GLOBAL_SCALE);
AColor AActive_Brick_Unbreakable::Pink_Highlight(AsConfig::Pink_Color, 3 * AsConfig::GLOBAL_SCALE);

//
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(int level_x, int level_y)
    :AActive_Brick(EBT_Unbreakable, level_x, level_y), Animation_Step(0), Region(0)
{
    Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 
        2 * AsConfig::GLOBAL_SCALE - 1, 2 * AsConfig::GLOBAL_SCALE - 1);
}


//
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
    DeleteObject(Region);
}


//
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
    int scale = AsConfig::GLOBAL_SCALE;
    int offset;

    Draw_In_Level(hdc, Brick_Rect);

    AsConfig::Pink_Color.Select(hdc);


    SelectClipRgn(hdc, Region);

    offset = 2 * Animation_Step * scale - AsConfig::BRICK_WIDTH * scale;

    Blue_Highlight.Select_Pen(hdc);
    MoveToEx(hdc, Brick_Rect.left + 4 * scale + offset, Brick_Rect.bottom + scale, 0);
    LineTo(hdc, Brick_Rect.left + 13 * scale - 1 + offset, Brick_Rect.top - 1 * scale);

    
    Pink_Highlight.Select_Pen(hdc);
    MoveToEx(hdc, Brick_Rect.left + 6 * scale + offset, Brick_Rect.bottom + scale, 0);
    LineTo(hdc, Brick_Rect.left + 15 * scale - 1 + offset, Brick_Rect.top - 1 * scale);

    SelectClipRgn(hdc, 0);

}


//
void AActive_Brick_Unbreakable::Act()
{
    if (Animation_Step <= MAX_ANIMATION_STEP)
    {
        Animation_Step++;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}


//
bool AActive_Brick_Unbreakable::Is_Finished()
{
    if (Animation_Step >= MAX_ANIMATION_STEP - 1)
        return true;
    else
        return false;
}


//
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT brick_rect)
{// Вывод неактивного кирпича на уровень

    AsConfig::White_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, brick_rect);
}



// AActive_Brick_Multihit
//AColor AActive_Brick_Multihit::Blue_Highlight(AsConfig::Blue_Color, AsConfig::GLOBAL_SCALE);
//AColor AActive_Brick_Multihit::Pink_Highlight(AsConfig::Pink_Color, 3 * AsConfig::GLOBAL_SCALE);
//
AActive_Brick_Multihit::AActive_Brick_Multihit(int level_x, int level_y)
    :AActive_Brick(EBT_Multihit_1, level_x, level_y), Rotation_Step(0)
{

}


//
AActive_Brick_Multihit::~AActive_Brick_Multihit()
{
    //DeleteObject(Region);
}


//
void AActive_Brick_Multihit::Draw(HDC hdc, RECT& paint_area)
{
    int scale = AsConfig::GLOBAL_SCALE;
    int step;
    double rotation_angle, x_ratio;
    RECT zero_rect;
    XFORM xForm, old_xForm;

    // 1. Очищаем фон
    AsConfig::BG_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, Brick_Rect);

    // 2. Настраиваем матрицу "поворота 100"
    step = Rotation_Step % Steps_Per_Turn;
    rotation_angle = M_PI_4 / 2.0 * (double)step;
    x_ratio = cos(rotation_angle);

    xForm.eM11 = (float)cos(rotation_angle);
    xForm.eM12 = 0.0;
    xForm.eM21 = 0.0;
    xForm.eM22 = 1.0;
    xForm.eDx = (float)Brick_Rect.left + (float)(1.0 - x_ratio) * (float)(AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE) / 2.0f;
    xForm.eDy = (float)Brick_Rect.top;

    GetWorldTransform(hdc, &old_xForm);
    SetWorldTransform(hdc, &xForm);


    // 3. Рисуем "100"
    AsConfig::Letter_Color.Select_Pen(hdc);

    MoveToEx(hdc, 0 + 1 * scale + 1, 0 + 3 * scale, 0);
    LineTo(hdc, 0 + 3 * scale + 1, 0 + 1 * scale);
    LineTo(hdc, 0 + 3 * scale + 1, 0 + 6 * scale - 1);

    zero_rect.left = 0 + 5 * scale + 1;
    zero_rect.top = 0 + 1 * scale;
    zero_rect.right = zero_rect.left + 3 * scale + 1;
    zero_rect.bottom = zero_rect.top + 5 *scale;
    AsConfig::Round_Rect(hdc, zero_rect);


    zero_rect.left += 5 * scale;
    zero_rect.right += 5 * scale;
    AsConfig::Round_Rect(hdc, zero_rect);

    SetWorldTransform(hdc, &old_xForm);
}


//
void AActive_Brick_Multihit::Act()
{
    if (Rotation_Step <= MAX_ROTATION_STEP)
    {
        Rotation_Step++;
      InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}


//
bool AActive_Brick_Multihit::Is_Finished()
{
    if (Rotation_Step >= MAX_ROTATION_STEP)
        return true;
    else
        return false;
}


//
void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, RECT brick_rect, EBrick_Type brick_type)
{// Вывод неактивного кирпича на уровень

    int scale = AsConfig::GLOBAL_SCALE;

    // 1. Рисуем фон
    AsConfig::White_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, brick_rect);

    AsConfig::Pink_Color.Select(hdc);
    Rectangle(hdc, brick_rect.left + scale, brick_rect.top + scale, 
        brick_rect.right - scale - 1, brick_rect.bottom - scale - 1);

    // 2. Рисуем внутренние прямоугольники
    switch (brick_type)
    {
    case EBT_Multihit_1:
        Draw_Stage(hdc, brick_rect, 2, 10);
        break;

    case EBT_Multihit_2:
        Draw_Stage(hdc, brick_rect, 2, 4);
        Draw_Stage(hdc, brick_rect, 8, 4);
        break;

    case EBT_Multihit_3:
        Draw_Stage(hdc, brick_rect, 2, 2);
        Draw_Stage(hdc, brick_rect, 6, 2);
        Draw_Stage(hdc, brick_rect, 10, 2);
        break;

    case EBT_Multihit_4:
        Draw_Stage(hdc, brick_rect, 2, 2);
        Draw_Stage(hdc, brick_rect, 5, 2);
        Draw_Stage(hdc, brick_rect, 8, 2);
        Draw_Stage(hdc, brick_rect, 11, 2);
        break;

    default:
        AsConfig::Throw();
        break;
    }
}


//
void AActive_Brick_Multihit::Draw_Stage(HDC hdc, RECT brick_rect, int x, int width)
{// Рисуем внутренний прямоугольник

    int scale = AsConfig::GLOBAL_SCALE;
    RECT stage_rect;

    stage_rect.left = brick_rect.left + x * scale;
    stage_rect.top = brick_rect.top + 2 * scale;
    stage_rect.right = stage_rect.left + width * scale;
    stage_rect.bottom = stage_rect.top + 3 * scale;

    AsConfig::BG_Color.Select(hdc);
    Rectangle(hdc, stage_rect.left + scale, stage_rect.top + scale, stage_rect.right + scale - 1, stage_rect.bottom + scale - 1);

    AsConfig::Blue_Color.Select(hdc);
    Rectangle(hdc, stage_rect.left, stage_rect.top, stage_rect.right - 1, stage_rect.bottom - 1);

}
