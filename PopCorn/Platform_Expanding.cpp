#include "Platform_Expanding.h"


// AsPlatform_Expanding
//
const double AsPlatform_Expanding::Min_Expanding_Platform_Width = (double)AsConfig::Platform_NORMAL_WIDTH;
const double AsPlatform_Expanding::Max_Expanding_Platform_Width = 40.0;
const double AsPlatform_Expanding::Expanding_Platform_Width_Step = 1.0;
//
AsPlatform_Expanding::AsPlatform_Expanding(AsPlatform_State& platform_state)
    :Expanding_Platform_Width(0.0), Platform_State(&platform_state), Truss_Color(nullptr), Inner_Color(nullptr),
    Circle_Color(nullptr)
{

}


//
AsPlatform_Expanding::~AsPlatform_Expanding()
{
    delete Truss_Color;
}


//
void AsPlatform_Expanding::Init(AColor& inner_color, AColor& circle_color)
{
    Truss_Color = new AColor(inner_color, AsConfig::GLOBAL_SCALE);

    Inner_Color = &inner_color;
    Circle_Color = &circle_color;
}


//
void AsPlatform_Expanding::Draw_State(HDC hdc, double x)
{// Рисуем расширяющуюся платформу

    int y = AsConfig::Platform_Y_POS;
    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;

    RECT inner_rect;

    inner_rect.left = (int)((x + (Expanding_Platform_Width - (double)AsConfig::Platform_Expanding_Inner_Width) / 2.0) * d_scale);
    inner_rect.top = (y + 1) * scale;
    inner_rect.right = inner_rect.left + AsConfig::Platform_Expanding_Inner_Width * scale;
    inner_rect.bottom = (y + 1 + 5) * scale;

    // 1. Левая сторона
    // 1.1 Шарик
    Draw_Expanding_Platform_Ball(hdc, true, x);

    // 1.2 Фермы
    Draw_Expanding_Truss(hdc, inner_rect, true);


    // 2. Левая сторона
    // 2.1 Шарик
    Draw_Expanding_Platform_Ball(hdc, false, x);

    // 2.2 Фермы
    Draw_Expanding_Truss(hdc, inner_rect, false);


    // 3. Рисуем среднюю платформу
    AsTools::Rect(hdc, inner_rect, *Inner_Color);
}


//
void AsPlatform_Expanding::Reset()
{
    Expanding_Platform_Width = Min_Expanding_Platform_Width;
}


//
bool AsPlatform_Expanding::Act(double& x_pos, EPlatform_State& next_state, bool& correct_pos)
{
    correct_pos = false;

    next_state = EPlatform_State::Unknown;

    switch (Platform_State->Expanding)
    {
    case EPlatform_Transformation::Init:
        if (Expanding_Platform_Width < Max_Expanding_Platform_Width)
        {
            Expanding_Platform_Width += Expanding_Platform_Width_Step;
            x_pos -= Expanding_Platform_Width_Step / 2.0;

            correct_pos = true;
        }
        else
            Platform_State->Expanding = EPlatform_Transformation::Active;

        return true;

    case EPlatform_Transformation::Active:
        break;

    case EPlatform_Transformation::Finalize:
        if (Expanding_Platform_Width > Min_Expanding_Platform_Width)
        {
            Expanding_Platform_Width -= Expanding_Platform_Width_Step;
            x_pos += Expanding_Platform_Width_Step / 2.0;

            correct_pos = true;
        }
        else
        {
            Platform_State->Expanding = EPlatform_Transformation::Unknown;
            next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
        }

        return true;

    default:
        AsConfig::Throw();
        break;
    }

    return false;
}


//
void AsPlatform_Expanding::Draw_Expanding_Platform_Ball(HDC hdc, bool is_left, double x)
{
    int y = AsConfig::Platform_Y_POS;
    int arc_mid_x;
    int arc_start_y, arc_end_y;
    int arc_right_offset;
    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    RECT rect, arc_rect;

    // 1.1 Шарик
    if (is_left)
        rect.left = (int)(x * d_scale);
    else
        rect.left = (int)((x + Expanding_Platform_Width - (double)AsConfig::Platform_CIRCLE_SIZE) * d_scale);

    rect.top = y * scale;
    rect.right = rect.left + AsConfig::Platform_CIRCLE_SIZE * scale;
    rect.bottom = (y + AsConfig::Platform_CIRCLE_SIZE) * scale;

    AsTools::Ellipse(hdc, rect, *Circle_Color);

    // 1.2 Переходник на ферму
    if (is_left)
        Rectangle(hdc, rect.left + 4 * scale, rect.top, rect.right - scale + 1, rect.bottom - 1);
    else
        Rectangle(hdc, rect.left + 1, rect.top, rect.left + 3 * scale, rect.bottom - 1);


    // 1.3 Дуга фермы на шарике        
    arc_rect.left = rect.left + 4 * scale + 2;
    arc_rect.top = rect.top + scale + 1;
    arc_rect.right = rect.left + (4 + 3) * scale + 2;
    arc_rect.bottom = rect.bottom - scale - 1;

    arc_mid_x = arc_rect.left + (arc_rect.right - arc_rect.left) / 2;

    if (is_left)
    {
        arc_start_y = arc_rect.top;
        arc_end_y = arc_rect.bottom;
    }
    else
    {
        arc_start_y = arc_rect.bottom;
        arc_end_y = arc_rect.top;

        arc_right_offset = (AsConfig::Platform_CIRCLE_SIZE - 2) * scale + 1;

        arc_rect.left -= arc_right_offset;
        arc_rect.right -= arc_right_offset;
        arc_mid_x -= arc_right_offset;
    }

    // 1.3.1 Дырка в шарике под дугой 
    AsTools::Ellipse(hdc, arc_rect, AsConfig::BG_Color);

    // 1.3.2 Сама дуга
    Truss_Color->Select(hdc);
    Arc(hdc, arc_rect.left, arc_rect.top, arc_rect.right - 1, arc_rect.bottom - 1,
        arc_mid_x, arc_start_y, arc_mid_x, arc_end_y);

}


//
void AsPlatform_Expanding::Draw_Expanding_Truss(HDC hdc, RECT& inner_rect, bool is_left)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    int truss_x;
    int truss_top_y, truss_bottom_y;
    int truss_x_offset;
    double extesion_ratio; // [1.0 .. 0.0]

    extesion_ratio = (Max_Expanding_Platform_Width - Expanding_Platform_Width) /
        (Max_Expanding_Platform_Width - Min_Expanding_Platform_Width);
    truss_x_offset = (int)(6.0 * extesion_ratio * AsConfig::D_GLOBAL_SCALE);

    truss_x = inner_rect.left + 1;

    if (is_left)
        truss_x += truss_x_offset;
    else
    {
        truss_x += (AsConfig::Platform_Expanding_Inner_Width + 8 - 1) * scale + 1;
        truss_x -= truss_x_offset;
    }

    truss_top_y = inner_rect.top + 1;
    truss_bottom_y = inner_rect.bottom - scale + 1;

    Truss_Color->Select(hdc);
    MoveToEx(hdc, truss_x, truss_top_y, 0);
    LineTo(hdc, truss_x - 4 * scale - 1, truss_bottom_y);
    LineTo(hdc, truss_x - 8 * scale, truss_top_y);

    MoveToEx(hdc, truss_x, truss_bottom_y, 0);
    LineTo(hdc, truss_x - 4 * scale - 1, truss_top_y);
    LineTo(hdc, truss_x - 8 * scale, truss_bottom_y);
}