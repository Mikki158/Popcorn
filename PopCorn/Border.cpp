#include "Border.h"

// AsBorder
//
AsBorder::AsBorder()
{
    Floor_Rect.left = AsConfig::LEVEL_X_OFFSET * AsConfig::GLOBAL_SCALE;
    Floor_Rect.top = (AsConfig::Floor_Y_Pos) * AsConfig::GLOBAL_SCALE;
    Floor_Rect.right = AsConfig::MAX_X_POS * AsConfig::GLOBAL_SCALE;
    Floor_Rect.bottom = AsConfig::MAX_Y_POS * AsConfig::GLOBAL_SCALE;
}


//
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{// Корректируем позицию при отражении от рамки
    bool got_hit = false;

    // 1. Левый край
    if (next_x_pos - ball->RADIUS < AsConfig::BORDER_X_OFFSET)
    {
        got_hit = true;
        ball->Reflect(false);
    }

    // 2. Верхний край
    if (next_y_pos - ball->RADIUS < AsConfig::BORDER_Y_OFFSET)
    {
        got_hit = true;
        ball->Reflect(true);
    }

    // 3. Правый край
    if (next_x_pos + ball->RADIUS > AsConfig::MAX_X_POS + 1)
    {
        got_hit = true;
        ball->Reflect(false);
    }

    // 4. Нижний край
    if (AsConfig::Level_Has_Floor && next_y_pos + ball->RADIUS > AsConfig::Floor_Y_Pos)
    {
        got_hit = true;
        ball->Reflect(true);
    }

    // Чтобы шарик смог улететь ниже пола, проверяем его max_y_pos ниже видимой границы
    if (next_y_pos + ball->RADIUS > AsConfig::MAX_Y_POS + ball->RADIUS * 4.0)
        ball->Set_State(EBS_Lost);
    

    

    return got_hit;
}


//
void AsBorder::Act()
{

}


//
void AsBorder::Clear(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!AsConfig::Level_Has_Floor)
        return;

    if (!IntersectRect(&intersection_rect, &paint_area, &Floor_Rect))
        return;

    AsConfig::BG_Color.Select(hdc);

    Rectangle(hdc, Floor_Rect.left, Floor_Rect.top, Floor_Rect.right - 1, Floor_Rect.bottom - 1);
}


//
void AsBorder::Draw(HDC hdc, RECT& paint_area)
{
    // 1. Линия слева
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, paint_area, 2, 1 + i * 4, false);
    }

    // 2. Линия справа
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, paint_area, AsConfig::MAX_X_POS + 2, 1 + i * 4, false);
    }

    // 3. Линия сверху
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, paint_area, 3 + i * 4, 0, true);
    }

    // 4. Пол (если есть)
    if (AsConfig::Level_Has_Floor)
        Draw_Floor(hdc, paint_area);
}


//
bool AsBorder::Is_Finished()
{
    return false; // Заглушка, т.к. этот метод не используется
}


//
void AsBorder::Redraw_Floor()
{
    AsConfig::Invalidate_Rect(Floor_Rect);
}


//
void AsBorder::Draw_Floor(HDC hdc, RECT& paint_area)
{
    int strokes_count;
    int line_len, gap_len;
    int stroke_len;;
    int x_pos, y_pos;
    RECT intersection_rect;
            
    if (!IntersectRect(&intersection_rect, &paint_area, &Floor_Rect))
        return;
    line_len = 6 * AsConfig::GLOBAL_SCALE;
    gap_len = 2 * AsConfig::GLOBAL_SCALE;
    stroke_len = line_len + gap_len;
    strokes_count = (Floor_Rect.right - Floor_Rect.left + AsConfig::GLOBAL_SCALE) / stroke_len;
    x_pos = Floor_Rect.left + 1;
    y_pos = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;

    AsConfig::Letter_Color.Select(hdc);

    for (int i = 0; i < strokes_count; i++)
    {
        MoveToEx(hdc, x_pos, y_pos, 0);
        LineTo(hdc, x_pos + line_len, y_pos);

        x_pos += stroke_len;
    }


}


//
void AsBorder::Draw_Element(HDC hdc, RECT &paint_area, int x, int y, bool top_border)
{// Рисуем элемент рамки уровня
    RECT intersection_rect, rect;

    rect.left = x * AsConfig::GLOBAL_SCALE;
    rect.top = y * AsConfig::GLOBAL_SCALE;
    rect.right = (x + 4) * AsConfig::GLOBAL_SCALE;
    rect.bottom = (y + 4) * AsConfig::GLOBAL_SCALE;

    if (!IntersectRect(&intersection_rect, &paint_area, &rect))
        return;

    // основная линия
    AsConfig::Blue_Color.Select(hdc);

    if (top_border)
    {
        Rectangle(hdc, x * AsConfig::GLOBAL_SCALE, (y + 1) * AsConfig::GLOBAL_SCALE,
            (x + 4) * AsConfig::GLOBAL_SCALE - 1, (y + 4) * AsConfig::GLOBAL_SCALE - 1);
    }
    else
    {
        Rectangle(hdc, (x + 1) * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
            (x + 4) * AsConfig::GLOBAL_SCALE - 1, (y + 4) * AsConfig::GLOBAL_SCALE - 1);
    }

    // белая кайма
    AsConfig::White_Color.Select(hdc);

    if (top_border)
    {
        Rectangle(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
            (x + 4) * AsConfig::GLOBAL_SCALE - 1, (y + 1) * AsConfig::GLOBAL_SCALE - 1);
    }
    else
    {
        Rectangle(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
            (x + 1) * AsConfig::GLOBAL_SCALE - 1, (y + 4) * AsConfig::GLOBAL_SCALE - 1);
    }

    // перфорация
    AsConfig::BG_Color.Select(hdc);

    if (top_border)
    {
        Rectangle(hdc, (x + 2) * AsConfig::GLOBAL_SCALE, (y + 2) * AsConfig::GLOBAL_SCALE,
            (x + 3) * AsConfig::GLOBAL_SCALE - 1, (y + 3) * AsConfig::GLOBAL_SCALE - 1);
    }
    else
    {
        Rectangle(hdc, (x + 2) * AsConfig::GLOBAL_SCALE, (y + 1) * AsConfig::GLOBAL_SCALE,
            (x + 3) * AsConfig::GLOBAL_SCALE - 1, (y + 2) * AsConfig::GLOBAL_SCALE - 1);
    }

}

