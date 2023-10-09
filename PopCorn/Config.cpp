#include "Config.h"

// AColor
//
AColor::AColor()
    :R(0), G(0), B(0), Pen(), Brush()
{

}


//
AColor::AColor(unsigned char r, unsigned char g, unsigned char b)
    :R(r), G(g), B(b), Pen(0), Brush(0)
{
    Pen = CreatePen(PS_SOLID, 0, RGB(R, G, B));
    Brush = CreateSolidBrush(RGB(R, G, B));
}


//
AColor::AColor(const AColor& color, int pen_size)
    :R(color.R), G(color.G), B(color.B), Pen(0), Brush(0)
{
    Pen = CreatePen(PS_SOLID, pen_size, color.Get_RGB());
}


//
AColor::AColor(const AColor& pen_color, const AColor& brush_color, int pen_size)
    :R(0), G(0), B(0), Pen(0), Brush(0)
{
    Pen = CreatePen(PS_SOLID, pen_size, pen_color.Get_RGB());
    Brush = CreateSolidBrush(brush_color.Get_RGB());
}


//
AColor::AColor(unsigned char r, unsigned char g, unsigned char b, int pen_size)
    :R(r), G(g), B(b), Pen(0), Brush(0)
{
    Pen = CreatePen(PS_SOLID, pen_size, RGB(R, G, B));
    Brush = CreateSolidBrush(RGB(R, G, B));
}


//
AColor::~AColor()
{
    if (Pen != 0)
        DeleteObject(Pen);

    if (Brush != 0)
        DeleteObject(Brush);
}


//
void AColor::operator = (const AColor& another)
{
    AsConfig::Throw(); // При присваивании нужно создать копии карандаша и кисти. Вместо этого их следует пересоздать методом Set_As()
}


//
void AColor::Select(HDC hdc) const
{
    SelectObject(hdc, Pen);
    SelectObject(hdc, Brush);
}


//
void AColor::Select_Pen(HDC hdc) const
{
    SelectObject(hdc, Pen);
}


//
void AColor::Set_As(unsigned char r, unsigned char g, unsigned char b)
{
    R = r;
    G = g;
    B = b;

    if (Pen != 0)
        DeleteObject(Pen);

    if (Brush != 0)
        DeleteObject(Brush);

    Pen = CreatePen(PS_SOLID, 0, RGB(R, G, B));
    Brush = CreateSolidBrush(RGB(R, G, B));
}


//
int AColor::Get_RGB() const
{
    return RGB(R, G, B);
}


//
HBRUSH AColor::Get_Brush() const
{
    return Brush;
}



// AsConfig
bool AsConfig::Level_Has_Floor = true;
int AsConfig::Current_Timer_Tick = 0;
HWND AsConfig::HWnd;

const double AsConfig::D_GLOBAL_SCALE = (double)GLOBAL_SCALE;
const double AsConfig::Ball_RADIUS = 2.0 - 0.5 / GLOBAL_SCALE;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Moving_STEP_SIZE = 1.0 / GLOBAL_SCALE;
const double AsConfig::START_BALL_Y_POS = 184.0;
const double AsConfig::Ball_Accelerate = 1.001;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Pink_Color(255, 85, 255);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(White_Color, GLOBAL_SCALE);
const AColor AsConfig::Laser_Color(White_Color, GLOBAL_SCALE);
const AColor AsConfig::Teleport_Portal_Color(Blue_Color, BG_Color, GLOBAL_SCALE);
const AColor AsConfig::Blue_Highlight_Unbreakable(Blue_Color, GLOBAL_SCALE);
const AColor AsConfig::Pink_Highlight_Unbreakable(Pink_Color, 3 * GLOBAL_SCALE);
const AColor AsConfig::Advertisement_Pink_Table(Pink_Color, 2 * GLOBAL_SCALE);
const AColor AsConfig::Advertisement_Blue_Table(0, 159, 159, GLOBAL_SCALE);
const AColor AsConfig::Gate_Color(White_Color, GLOBAL_SCALE);
const AColor AsConfig::Monster_Dark_Pink_Color(228, 0, 228);
const AColor AsConfig::Monster_Cornea_Color(BG_Color, White_Color, GLOBAL_SCALE * 2 / 3);
const AColor AsConfig::Monster_Iris_Color(BG_Color, Blue_Color, GLOBAL_SCALE * 2 / 3);
const AColor AsConfig::Monster_Comet_Tail(Monster_Dark_Pink_Color, GLOBAL_SCALE);
const AColor AsConfig::BG_Outline_Color(BG_Color, GLOBAL_SCALE * 2 / 3);
const AColor AsConfig::Explosion_Pink_Color(White_Color, Pink_Color, 0);
const AColor AsConfig::Explosion_Blue_Color(White_Color, Blue_Color, 0);

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
    return rand() * range / (RAND_MAX + 1);
}


//
unsigned char AsTools::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_step)
{
    return color - step * (color - bg_color) / (max_step - 1);
}


//
void AsTools::Get_Fading_Color(const AColor& origin_color, int step, AColor& result_color, int max_step)
{
    unsigned char r, g, b;

    r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step, max_step);
    g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step, max_step);
    b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step, max_step);

    result_color.Set_As(r, g, b);
}


//
bool AsTools::Reflect_On_Circle(double next_x_pos, double next_y_pos, double circle_x, double circle_y, double circle_radius, ABall_Object* ball)
{
    double dx, dy;
    double distance, two_radius;

    double alpha, beta, gamma;
    double realted_ball_direction;

    const double pi_2 = 2.0 * M_PI;

    dx = next_x_pos - circle_x;
    dy = next_y_pos - circle_y;

    distance = sqrt((dx * dx) + (dy * dy));
    two_radius = circle_radius + AsConfig::Ball_RADIUS;

    if (distance - AsConfig::Moving_STEP_SIZE < two_radius)
    {// Мячик коснулся бокового шарика
        beta = atan2(-dy, dx);

        realted_ball_direction = ball->Get_Direction();
        realted_ball_direction -= beta;

        if (realted_ball_direction > pi_2)
            realted_ball_direction -= pi_2;

        if (realted_ball_direction < 0)
            realted_ball_direction += pi_2;

        if (realted_ball_direction > M_PI_2 && realted_ball_direction < M_PI + M_PI_2)
        {
            alpha = beta + M_PI - ball->Get_Direction();
            gamma = beta + alpha;

            ball->Set_Direction(gamma);

            return true;
        }
    }

    return false;
}



// AHit_Checker
bool AHit_Checker::Check_Hit(double next_x_pos, double next_y_pos)
{
    return false;
}


//
bool AHit_Checker::Check_Hit(RECT& rect)
{
    return false;
}


//
bool AHit_Checker::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
{// Проверяет пересечение горизонтального отрезка (проходящего от left_x до right_x через y) с окружностью радиусом ridius
    double min_x, max_x;

    if (y > radius)
    {
        return false;
    }

    x = sqrt(radius * radius - y * y);

    max_x = next_x_pos + x;
    min_x = next_x_pos - x;

    if ((max_x >= left_x && max_x <= right_x) || (min_x >= left_x && min_x <= right_x))
    {
        return true;
    }
    else
    {
        return false;
    }

}



// AHit_Checker_List
//
AHit_Checker_List::AHit_Checker_List()
    :Hit_Checkers_Count(0), Hit_checkers{}
{

}


//
void AHit_Checker_List::Add_Hit_Checker(AHit_Checker* hit_checker)
{
    Hit_checkers.push_back(hit_checker);
}


//
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos, ABall_Object* ball)
{
    for (auto *hit_checker : Hit_checkers)
        if (hit_checker->Check_Hit(x_pos, y_pos, ball))
            return true;

    return false;
}


//
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos)
{
    for (auto* hit_checker : Hit_checkers)
        if (hit_checker->Check_Hit(x_pos, y_pos))
            return true;

    return false;
}


//
bool AHit_Checker_List::Check_Hit(RECT &rect)
{
    for (auto* hit_checker : Hit_checkers)
        if (hit_checker->Check_Hit(rect))
            return true;

    return false;
}



// AString
//
AString::AString()
{

}

//
AString::AString(const wchar_t* str)
    :Content(str)
{

}


//
void AString::Append(int value)
{
    wchar_t buf[32];

    //_itow_s(value, buf, 32, 10);
    swprintf(buf, 32, L"%.6i", value);

    Content += buf;

}


//
const wchar_t* AString::Get_Content()
{
    return Content.c_str();
}


//
int AString::Get_Lenght()
{
    return Content.length();
}



// AMessage
//
AMessage::AMessage(EMessage_Type message_type)
    :Message_Type(message_type)
{

}





