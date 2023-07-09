#include "Ball.h"

ABall::ABall()
    :Ball_Direction(M_PI - M_PI_4), Ball_Pen(), Ball_Brush(), Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0), Ball_Rect(), Prev_Ball_Rect()
{

}

void ABall::Init()
{
    AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}

void ABall::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
    {
        return;
    }
    // очищаем фон
    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);

    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

    // рисуем шарик
    SelectObject(hdc, Ball_Pen);
    SelectObject(hdc, Ball_Brush);

    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);

}

void ABall::Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width)
{
    int next_x_pos, next_y_pos;
    int max_x_pos = AsConfig::MAX_X_POS - AsConfig::BALL_SIZE;
    int platform_y_pos = AsConfig::Platform_Y_POS - AsConfig::BALL_SIZE;

    Prev_Ball_Rect = Ball_Rect;

    next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
    next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

    // отражение шарика от стен
    if (next_x_pos < AsConfig::BORDER_X_OFFSET)
    {
        next_x_pos = AsConfig::LEVEL_X_OFFSET - (next_x_pos - AsConfig::LEVEL_X_OFFSET);
        Ball_Direction = M_PI - Ball_Direction;
    }
    if (next_y_pos < AsConfig::BORDER_Y_OFFSET)
    {
        next_y_pos = AsConfig::BORDER_Y_OFFSET - (next_y_pos - AsConfig::BORDER_Y_OFFSET);
        Ball_Direction = -Ball_Direction;
    }
    if (next_x_pos > max_x_pos)
    {
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
        Ball_Direction = M_PI - Ball_Direction;
    }
    if (next_y_pos > AsConfig::MAX_Y_POS)
    {
        next_y_pos = AsConfig::MAX_Y_POS - (next_y_pos - AsConfig::MAX_Y_POS);
        Ball_Direction = M_PI + (M_PI - Ball_Direction);
    }

    // отражаем шарик от платформы
    if (next_y_pos > platform_y_pos)
    {
        if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
        {
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
            Ball_Direction = M_PI + (M_PI - Ball_Direction);
        }
    }

    // отражаем от киричей

    level->Check_Level_Brick_Heat(next_y_pos, Ball_Direction);

    //Смещаем шарик
    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;

    Ball_Rect.left = Ball_X_Pos * AsConfig::GLOBAL_SCALE;
    Ball_Rect.top = Ball_Y_Pos * AsConfig::GLOBAL_SCALE;
    Ball_Rect.right = Ball_Rect.left + AsConfig::BALL_SIZE * AsConfig::GLOBAL_SCALE;
    Ball_Rect.bottom = Ball_Rect.top + AsConfig::BALL_SIZE * AsConfig::GLOBAL_SCALE;

    InvalidateRect(hwnd, &Ball_Rect, FALSE);
    InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
}