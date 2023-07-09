#pragma once

#include <Windows.h>

#include "Config.h"


class AsPlatform
{
public:
    int X_Pos;
    int X_Step;
    int Width;

    AsPlatform();

    void Init();

    void Redraw_Platform(HWND hwnd);
    void Draw(HDC hdc, RECT& paint_area);

private:
    int Inner_width;

    RECT Platform_Rect, Prev_Platform_Rect;

    HPEN Platform_Circle_Pen, Platform_Inner_Pen;
    HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

    static const int CIRCLE_SIZE = 7;
    static const int Height = 7;
};