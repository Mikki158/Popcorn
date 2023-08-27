#include "Border.h"

// AsBorder
//
AsBorder::AsBorder()
{

}


//
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
    bool got_hit = false;

    // отражение шарика от стен
    if (next_x_pos - ball->RADIUS < AsConfig::BORDER_X_OFFSET)
    {
        got_hit = true;
        ball->Reflect(false);
    }

    if (next_y_pos - ball->RADIUS < AsConfig::BORDER_Y_OFFSET)
    {
        got_hit = true;
        ball->Reflect(true);
    }

    if (next_x_pos + ball->RADIUS > AsConfig::MAX_X_POS + 1)
    {
        got_hit = true;
        ball->Reflect(false);
    }

    if (next_y_pos + ball->RADIUS > AsConfig::MAX_Y_POS)
    {
        if (AsConfig::Level_Has_Floor)
        {
            got_hit = true;
            ball->Reflect(true);
        }
        else
        {
            if (next_y_pos + ball->RADIUS > AsConfig::MAX_Y_POS + ball->RADIUS * 4.0)
            {
                ball->Set_State(EBS_Lost);
            }
        }

    }

    return got_hit;
}


//
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{
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


//
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

