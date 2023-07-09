#include "Border.h"

AsBorder::AsBorder()
    :Border_Black_Brush(), Border_Black_Pen(), Border_Blue_Brush(), Border_Blue_Pen(), Border_White_Brush(), Border_White_Pen()
{

}

void AsBorder::Init()
{
    AsConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
    AsConfig::Create_Pen_Brush(AsConfig::BG_Color, Border_Black_Pen, Border_Black_Brush);
    AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{
    // основная линия
    SelectObject(hdc, Border_Blue_Pen);
    SelectObject(hdc, Border_Blue_Brush);

    if (top_border)
    {
        Rectangle(hdc, x * AsConfig::GLOBAL_SCALE, (y + 1) * AsConfig::GLOBAL_SCALE,
            (x + 4) * AsConfig::GLOBAL_SCALE, (y + 4) * AsConfig::GLOBAL_SCALE);
    }
    else
    {
        Rectangle(hdc, (x + 1) * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
            (x + 4) * AsConfig::GLOBAL_SCALE, (y + 4) * AsConfig::GLOBAL_SCALE);
    }

    // белая кайма
    SelectObject(hdc, Border_White_Pen);
    SelectObject(hdc, Border_White_Brush);

    if (top_border)
    {
        Rectangle(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
            (x + 4) * AsConfig::GLOBAL_SCALE, (y + 1) * AsConfig::GLOBAL_SCALE);
    }
    else
    {
        Rectangle(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
            (x + 1) * AsConfig::GLOBAL_SCALE, (y + 4) * AsConfig::GLOBAL_SCALE);
    }

    // перфорация
    SelectObject(hdc, Border_Black_Pen);
    SelectObject(hdc, Border_Black_Brush);

    if (top_border)
    {
        Rectangle(hdc, (x + 2) * AsConfig::GLOBAL_SCALE, (y + 2) * AsConfig::GLOBAL_SCALE,
            (x + 3) * AsConfig::GLOBAL_SCALE, (y + 3) * AsConfig::GLOBAL_SCALE);
    }
    else
    {
        Rectangle(hdc, (x + 2) * AsConfig::GLOBAL_SCALE, (y + 1) * AsConfig::GLOBAL_SCALE,
            (x + 3) * AsConfig::GLOBAL_SCALE, (y + 2) * AsConfig::GLOBAL_SCALE);
    }

}

void AsBorder::Draw(HDC hdc)
{
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 2, 1 + i * 4, false);
    }

    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 202, 1 + i * 4, false);
    }

    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 3 + i * 4, 0, true);
    }
}