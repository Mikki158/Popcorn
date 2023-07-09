#include "Platform.h"


AsPlatform::AsPlatform()
    : X_Pos(100), X_Step(AsConfig::GLOBAL_SCALE), Width(28), Inner_width(21), Platform_Rect(), Prev_Platform_Rect(),
    Platform_Circle_Pen(), Platform_Inner_Pen(), Platform_Circle_Brush(), Platform_Inner_Brush()
{

}

void AsPlatform::Init()
{
    AsConfig::Create_Pen_Brush(255, 85, 255, Platform_Circle_Pen, Platform_Circle_Brush);
    AsConfig::Create_Pen_Brush(85, 255, 255, Platform_Inner_Pen, Platform_Inner_Brush);
}

void AsPlatform::Redraw_Platform(HWND hwnd)
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = X_Pos * AsConfig::GLOBAL_SCALE;
    Platform_Rect.top = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;
    Platform_Rect.right = Platform_Rect.left + Width * AsConfig::GLOBAL_SCALE;
    Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::GLOBAL_SCALE;

    InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(hwnd, &Platform_Rect, FALSE);
}

void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
    {
        return;
    }


    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);

    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
        Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    SelectObject(hdc, Platform_Circle_Pen);
    SelectObject(hdc, Platform_Circle_Brush);

    Ellipse(hdc, X_Pos * AsConfig::GLOBAL_SCALE, AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE, (X_Pos + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE,
        (AsConfig::Platform_Y_POS + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE);
    Ellipse(hdc, (X_Pos + Inner_width) * AsConfig::GLOBAL_SCALE, AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE,
        (X_Pos + Inner_width + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE, (AsConfig::Platform_Y_POS + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE);

    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);

    RoundRect(hdc, (X_Pos + 4) * AsConfig::GLOBAL_SCALE, (AsConfig::Platform_Y_POS + 1) * AsConfig::GLOBAL_SCALE,
        (X_Pos + 4 + 20) * AsConfig::GLOBAL_SCALE, (AsConfig::Platform_Y_POS + 1 + 5) * AsConfig::GLOBAL_SCALE,
        3 * AsConfig::GLOBAL_SCALE, 3 * AsConfig::GLOBAL_SCALE);
}

