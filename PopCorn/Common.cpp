#include "Common.h"


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



// AGame_Objects_Set
//
void AGame_Objects_Set::Advance(double max_speed)
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Advance(max_speed);
}


//
void AGame_Objects_Set::Begin_Movement()
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Begin_Movement();
}


//
void AGame_Objects_Set::Finish_Movement()
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Finish_Movement();
}


//
double AGame_Objects_Set::Get_Speed()
{
    double max_speed = 0.0;
    double curr_speed;
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
    {
        curr_speed = object->Get_Speed();
        if (curr_speed > max_speed)
            max_speed = curr_speed;
    }

    return max_speed;
}


//
void AGame_Objects_Set::Act()
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Act();

}


//
void AGame_Objects_Set::Clear(HDC hdc, RECT& paint_area)
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Clear(hdc, paint_area);
}


//
void AGame_Objects_Set::Draw(HDC hdc, RECT& paint_area)
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Draw(hdc, paint_area);
}


//
bool AGame_Objects_Set::Is_Finished()
{
    return false; // «аглушка, т.к. этот метод не используетс€
}