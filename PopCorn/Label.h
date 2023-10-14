#pragma once

#include "Config.h"

//
class ALabel
{
public:
    ALabel(int x_pos, int y_pos, int width, int height, const AFont& font, const AColor& color);

    AString Content;
    RECT Content_Rect;

    void Draw(HDC hdc);

private:
    int X_Pos, Y_Pos;
    int Width, Height;
    const AFont& Font;
    const AColor& Color;

};