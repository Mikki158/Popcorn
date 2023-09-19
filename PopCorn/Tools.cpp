#include "Tools.h"


// AGraphics_Object
//
AGraphics_Object::~AGraphics_Object()
{

}



// AMover
//
AMover::~AMover()
{

}



// AHit_Checker
bool AHit_Checker::Check_Hit(double next_x_pos, double next_y_pos)
{
    return false;
}


//
bool AHit_Checker::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
{// ѕровер€ет пересечение горизонтального отрезка (проход€щего от left_x до right_x через y) с окружностью радиусом ridius
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
bool AHit_Checker_List::Add_Hit_Checker(AHit_Checker* hit_checker)
{
    if (Hit_Checkers_Count >= sizeof(Hit_checkers) / sizeof(Hit_checkers[0]))
        return false;

    Hit_checkers[Hit_Checkers_Count++] = hit_checker;

    return true;
}


//
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos, ABall* ball)
{
    for (int i = 0; i < Hit_Checkers_Count; i++)
        if (Hit_checkers[i]->Check_Hit(x_pos, y_pos, ball))
            return true;

    return false;
}


//
bool AHit_Checker_List::Check_Hit(double x_pos, double y_pos)
{
    for (int i = 0; i < Hit_Checkers_Count; i++)
        if (Hit_checkers[i]->Check_Hit(x_pos, y_pos))
            return true;

    return false;
}



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
int AColor::Get_RGB() const
{
    return RGB(R, G, B);
}


//
HBRUSH AColor::Get_Brush() const
{
    return Brush;
}