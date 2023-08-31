#pragma once

#include <Windows.h>

#include "Ball.h"

class AsBorder: public AHit_Checker
{
public:

    AsBorder();

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

    void Draw(HDC hdc, RECT& paint_area);
    void Redraw_Floor();

private:
    RECT Floor_Rect;

    void Draw_Floor(HDC hdc, RECT& paint_area);
    void Draw_Element(HDC hdc, RECT& paint_area, int x, int y, bool top_border);
};