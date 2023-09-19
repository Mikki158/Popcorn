#include "Border.h"

// AsBorder
//
AsBorder::AsBorder()
    :Floor_Rect{}, Gates {}
{
    Floor_Rect.left = AsConfig::LEVEL_X_OFFSET * AsConfig::GLOBAL_SCALE;
    Floor_Rect.top = (AsConfig::Floor_Y_Pos) * AsConfig::GLOBAL_SCALE;
    Floor_Rect.right = AsConfig::MAX_X_POS * AsConfig::GLOBAL_SCALE;
    Floor_Rect.bottom = AsConfig::MAX_Y_POS * AsConfig::GLOBAL_SCALE;

    // Гейты
    Gates[0] = new AGate(1, 29);
    Gates[1] = new AGate(AsConfig::MAX_X_POS, 29);
        
    Gates[2] = new AGate(1, 77);
    Gates[3] = new AGate(AsConfig::MAX_X_POS, 77);
        
    Gates[4] = new AGate(1, 129);
    Gates[5] = new AGate(AsConfig::MAX_X_POS, 129);
        
    Gates[6] = new AGate(1, 178);
    Gates[7] = new AGate(AsConfig::MAX_X_POS, 178);
}


//
AsBorder::~AsBorder()
{
    for (int i = 0; i < AsConfig::Gate_Count; i++)
        delete Gates[i];
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
        ball->Set_State(EBall_State::Lost);
    
    return got_hit;
}


//



//
void AsBorder::Advance(double max_speed)
{
    // Заглушка, т.к. этот метод не используется
}


//
void AsBorder::Begin_Movement()
{
    // Заглушка, т.к. этот метод не используется
}


//
void AsBorder::Finish_Movement()
{
    // Заглушка, т.к. этот метод не используется
}


//
double AsBorder::Get_Speed()
{
    return 0.0; // Заглушка, т.к. этот метод не используется
}


//
void AsBorder::Act()
{
    for (int i = 0; i < AsConfig::Gate_Count; i++)
        Gates[i]->Act();
}


//
void AsBorder::Clear(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    // 1. Стираем гейты
    for (int i = 0; i < AsConfig::Gate_Count; i++)
        Gates[i]->Clear(hdc, paint_area);

    // 2. Стираем пол (если надо)
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
        Draw_Element(hdc, paint_area, AsConfig::MAX_X_POS + 1, 1 + i * 4, false);
    }

    // 3. Линия сверху
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, paint_area, 3 + i * 4, 0, true);
    }

    // 4. Пол (если есть)
    if (AsConfig::Level_Has_Floor)
        Draw_Floor(hdc, paint_area);

    // 5. Гейты
    for(int i = 0; i < AsConfig::Gate_Count; i++)
        Gates[i]->Draw(hdc, paint_area);

}


//
bool AsBorder::Is_Finished()
{
    return false; // Заглушка, т.к. этот метод не используется
}


//
void AsBorder::Redraw_Floor()
{
    AsTools::Invalidate_Rect(Floor_Rect);
}


//
void AsBorder::Open_Gate(int gate_index, bool short_open)
{
    if (gate_index != AsConfig::Gate_Count - 1 && short_open)
        AsConfig::Throw();
    
    if (gate_index >= 0 && gate_index < AsConfig::Gate_Count)
        Gates[gate_index]->Open_Gate(short_open);
    else
        AsConfig::Throw();
}


//
bool AsBorder::Is_Gate_Opened(int gate_index)
{
    if (gate_index >= 0 && gate_index < AsConfig::Gate_Count)
        return Gates[gate_index]->Is_Opened();
    else
    {
        AsConfig::Throw();
        return false;
    }
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

    int gate_top_y, gate_low_y;
    RECT intersection_rect, rect;

    rect.left = x * AsConfig::GLOBAL_SCALE;
    rect.top = y * AsConfig::GLOBAL_SCALE;
    rect.right = (x + 4) * AsConfig::GLOBAL_SCALE;
    rect.bottom = (y + 4) * AsConfig::GLOBAL_SCALE;

    if (!top_border)
    {
        for (int i = 0; i < AsConfig::Gate_Count; i++)
        {
            Gates[i]->Get_Y_Size(gate_top_y, gate_low_y);

            if (rect.top >= gate_top_y && rect.bottom <= gate_low_y)
                return; // Гейт целиком перекроет элемент рамки
        }
    }

    if (!IntersectRect(&intersection_rect, &paint_area, &rect))
        return;

    // основная линия
    if (top_border)
        AsTools::Rect(hdc, x, y + 1, 4, 3, AsConfig::Blue_Color);
    else
        AsTools::Rect(hdc, x + 1, y, 3, 4, AsConfig::Blue_Color);

    // белая кайма
    if (top_border)
        AsTools::Rect(hdc, x, y, 4, 1, AsConfig::White_Color);
    else
        AsTools::Rect(hdc, x, y, 1, 4, AsConfig::White_Color);

    // перфорация
    if (top_border)
        AsTools::Rect(hdc, x + 2, y + 2, 1, 1, AsConfig::BG_Color);
    else
        AsTools::Rect(hdc, x + 2, y + 1, 1, 1, AsConfig::BG_Color);
}

