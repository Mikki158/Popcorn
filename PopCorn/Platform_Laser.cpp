#include "Platform_Laser.h"


// AsPlatform_Laser
// 
AsPlatform_Laser::AsPlatform_Laser(AsPlatform_State& platform_state)
    :Platform_State(&platform_state), Circle_Color(nullptr), Inner_Color(nullptr), Gun_Color(nullptr),
    Laser_Transformatio_Step(0), Laser_Beam_Set(nullptr), Enable_Laser_Firing(false), Last_Laser_Shot_Tick(0)
{
    Gun_Color = new AColor(AsConfig::White_Color, AsConfig::GLOBAL_SCALE);
}


//
AsPlatform_Laser::~AsPlatform_Laser()
{
    delete Gun_Color;
}


//
void AsPlatform_Laser::Init(AsLaser_Beam_Set* laser_beam_set, AColor& inner_color, AColor& circle_color)
{
    Laser_Beam_Set = laser_beam_set;
    Circle_Color = &circle_color;
    Inner_Color = &inner_color;
}


//
bool AsPlatform_Laser::Act(EPlatform_State& next_state, double x_pos)
{
    double left_gun_x_pos, right_gun_x_pos;
    next_state = EPlatform_State::Unknown;

    switch (Platform_State->Laser)
    {
    case EPlatform_Transformation::Init:
        if (Laser_Transformatio_Step < Max_Laser_Transformatio_Step)
            Laser_Transformatio_Step++;
        else
            Platform_State->Laser = EPlatform_Transformation::Active;

        return true;

    case EPlatform_Transformation::Active:
        if (Enable_Laser_Firing)
        {
            if (Last_Laser_Shot_Tick + Laser_Shot_Timeout <= AsConfig::Current_Timer_Tick)
            {
                Last_Laser_Shot_Tick = AsConfig::Current_Timer_Tick + Laser_Shot_Timeout;
                left_gun_x_pos = Get_Gun_Pos(x_pos, true) + 0.5;
                right_gun_x_pos = Get_Gun_Pos(x_pos, false) + 0.5;

                Laser_Beam_Set->Fire(left_gun_x_pos, right_gun_x_pos);
            }
        }
        break;

    case EPlatform_Transformation::Finalize:
        if (Laser_Transformatio_Step > 0)
            Laser_Transformatio_Step--;
        else
        {
            Platform_State->Laser = EPlatform_Transformation::Unknown;
            next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
            Enable_Laser_Firing = false;
        }
        return true;

    default:
        AsConfig::Throw();
        break;
    }

    return false;
}


//
void AsPlatform_Laser::Draw_State(HDC hdc, double x_pos, RECT& platform_rect)
{// Рисуем лазерную платформу
    HRGN region;

    region = CreateRectRgnIndirect(&platform_rect);
    SelectClipRgn(hdc, region);

    // 1. Левое крыло
    Draw_Laser_Wing(hdc, x_pos, true);

    // 1. Правое крыло
    Draw_Laser_Wing(hdc, x_pos, false);

    // 3. Центральная чать
    // 3.0  Нормальная средняя чать

    Draw_Laser_Inner_Part(hdc, x_pos);


    // 3.1 Левая нога
    Draw_Laser_Leg(hdc, x_pos, true);

    // 3.2 Правая нога
    Draw_Laser_Leg(hdc, x_pos, false);

    // 3.3 Кабина
    Draw_Laser_Cabin(hdc, x_pos);

    SelectClipRgn(hdc, 0);
    DeleteObject(region);
}


//
void AsPlatform_Laser::Reset()
{
    Laser_Transformatio_Step = 0;
}


//
void AsPlatform_Laser::Fire(bool fire_on)
{

    if (Platform_State->Laser != EPlatform_Transformation::Active)
        return; // Игнорируем выстрел, пока платформа не свормируется

    Enable_Laser_Firing = fire_on;



    /*left_gun_x_pos = Get_Gun_Pos(x_pos, true) + 0.5;
    right_gun_x_pos = Get_Gun_Pos(x_pos, false) + 0.5;

    Laser_Beam_Set->Fire(fire_on, left_gun_x_pos, right_gun_x_pos);*/
}


//
void AsPlatform_Laser::Draw_Laser_Inner_Part(HDC hdc, double x_pos)
{// Рисуем уменьшающуюся часть обычной платформы

    int x, y;
    double ratio = (double)Laser_Transformatio_Step / (double)Max_Laser_Transformatio_Step;

    // Размер: 20 x 5 -> 8 x 1
    x = (int)x_pos;
    y = AsConfig::Platform_Y_POS;

    Inner_Color->Select(hdc);
    Draw_Expanding_Figure(hdc, EFigure_Type::Round_Rect_3x, x + 4, y + 1, 20, 5, ratio, x + 10, y + 3, 8, 1);
}


//
void AsPlatform_Laser::Draw_Laser_Wing(HDC hdc, double x_pos, bool is_left)
{// Рисуем "крыло" лазерной платформы 

    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    double x, y;
    int x_offset;
    double height;
    int half_max_step = Max_Laser_Transformatio_Step / 2;
    double ratio = (double)Laser_Transformatio_Step / (double)Max_Laser_Transformatio_Step;

    x = x_pos;
    y = AsConfig::Platform_Y_POS;

    if (!is_left)
        x += AsConfig::Platform_NORMAL_WIDTH - AsConfig::Platform_CIRCLE_SIZE;

    // 1. Крыло
    Circle_Color->Select(hdc);

    Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x, y, 7, 7, ratio, x, y + 1, 7, 12);

    // 1.1 Перемычка
    // Позиция: (3 : 6) -> (5 : 2)
    // Размер: 1 x 1 -> 6 x 5

    if (is_left)
        x_offset = 5;
    else
        x_offset = -4;

    Draw_Expanding_Figure(hdc, EFigure_Type::Rectangle, x + 3, y + 6, 1, 1, ratio, x + x_offset, y + 2, 6, 5);


    // 1.2 Пушка
    if (Laser_Transformatio_Step >= half_max_step)
    {
        ratio = (double)(Laser_Transformatio_Step - half_max_step) / (double)half_max_step;

        Gun_Color->Select(hdc);


        x = Get_Gun_Pos(x_pos, is_left);

        height = 3.0 * (1.0 - ratio) * d_scale;

        MoveToEx(hdc, (int)(x * d_scale + 1.0), (int)(y * d_scale + 3.0 * d_scale + 1.0), 0);
        LineTo(hdc, (int)(x * d_scale + 1.0), (int)(y * d_scale + height + 1.0));

        // 1.3 Хвост
        Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 1, y + 5, 0, 0, ratio, x - 1, y + 5 + 1.0 / d_scale, 3, 6);
    }
}


void AsPlatform_Laser::Draw_Laser_Leg(HDC hdc, double x_pos, bool is_left)
{// Рисуем "ногу" лазерной платформы 
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    double x, y;
    double x_scale;
    double ratio = (double)Laser_Transformatio_Step / (double)Max_Laser_Transformatio_Step;


    Inner_Color->Select(hdc);

    if (is_left)
    {
        x = (x_pos + 6.0) * d_scale;
        x_scale = d_scale;
    }
    else
    {
        x = x_pos * d_scale + (AsConfig::Platform_NORMAL_WIDTH - 6) * d_scale - 1.0;
        x_scale = -d_scale;
    }

    y = (double)(AsConfig::Platform_Y_POS + 3) * d_scale;
    POINT left_leg_points[7] =
    {
        {(int)x, (int)y},
        {(int)(x + 2 * x_scale), (int)(y - 2 * d_scale)},
        {(int)(x + 4 * x_scale), (int)(y - 2 * d_scale)},
        {(int)(x + 4 * x_scale), (int)y},
        {(int)(x + 2 * x_scale), (int)(y + 2 * d_scale)},
        {(int)(x + 2 * x_scale), (int)(y + (2.0 + 2.0 * ratio) * d_scale)},
        {(int)x, (int)(y + 4.0 * ratio * d_scale)}
    };

    Polygon(hdc, left_leg_points, 7);
}


//
void AsPlatform_Laser::Draw_Laser_Cabin(HDC hdc, double x_pos)
{// Рисуем "кабину" лазерной платформы 

    const int scale = AsConfig::GLOBAL_SCALE;
    int x, y;
    double ratio = (double)Laser_Transformatio_Step / (double)Max_Laser_Transformatio_Step;
    double one_pixel = 1.0 / AsConfig::D_GLOBAL_SCALE;

    x = (int)x_pos;
    y = AsConfig::Platform_Y_POS;

    // 3.3.1 Внешнее кольцо
    // Размер: 2 x 1 -> 10 x 8
    Inner_Color->Select(hdc);
    Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13, y + 1, 2, 1, ratio, x + 9, y - 1, 10, 8 - one_pixel);

    // 3.3.2 Среднее кольцо
    AsConfig::BG_Color.Select(hdc);
    Draw_Expanding_Figure(hdc, EFigure_Type::Rectangle, x + 13, y + 1, 2, 1, ratio, x + 11, y, 6, 1);
    Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13, y + 1, 2, 1, ratio, x + 10, y, 8, 5 - one_pixel);


    // 3.3.3 Внутреннее кольцо
    // РАзмер: 2 x 1 -> 6 x 4
    AsConfig::White_Color.Select(hdc);
    Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13, y + 1, 2, 1, ratio, x + 11, y, 6, 4 - one_pixel);
}


//
void AsPlatform_Laser::Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_tyoe, double start_x, double start_y, double start_width,
    double start_height, double ratio, double end_x, double end_y, double end_width, double end_height)
{
    int x, y;
    int width, height;
    RECT rect;

    x = Get_Expanding_Value(start_x, end_x, ratio);
    y = Get_Expanding_Value(start_y, end_y, ratio);
    width = Get_Expanding_Value(start_width, end_width, ratio);
    height = Get_Expanding_Value(start_height, end_height, ratio);

    switch (figure_tyoe)
    {
    case EFigure_Type::Ellipse:
        Ellipse(hdc, x, y, x + width - 1, y + height);
        break;

    case EFigure_Type::Rectangle:
        Rectangle(hdc, x, y, x + width - 1, y + height);
        break;

    case EFigure_Type::Round_Rect_3x:
        rect.left = x;
        rect.top = y;
        rect.right = rect.left + width + 1;
        rect.bottom = rect.top + height + 1;
        AsTools::Round_Rect(hdc, rect, 3);
        break;

    default:
        AsConfig::Throw();
        break;
    }
}


//
int AsPlatform_Laser::Get_Expanding_Value(double start, double end, double ratio)
{
    int value;
    double delta;

    delta = end - start;
    value = (int)((start + delta * ratio) * AsConfig::D_GLOBAL_SCALE);

    return value;
}


//
double AsPlatform_Laser::Get_Gun_Pos(double platform_x_pos, bool is_left)
{
    double gun_x_pos;

    if (is_left)
        gun_x_pos = (platform_x_pos + 3.0);
    else
        gun_x_pos = platform_x_pos + (AsConfig::Platform_NORMAL_WIDTH - 4);

    return gun_x_pos;
}