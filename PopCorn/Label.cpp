﻿#include "Label.h"

// ALabel
//
ALabel::ALabel(int x_pos, int y_pos, int width, int height, const AFont& font, const AColor& color)
    :Last_Char(0), X_Pos(x_pos), Y_Pos(y_pos), Width(width), Height(height), Font(font), Color(color)
{
    const int scale = AsConfig::GLOBAL_SCALE;

    Content_Rect.left = X_Pos * scale;
    Content_Rect.top = Y_Pos * scale;
    Content_Rect.right = Content_Rect.left + Width * scale;
    Content_Rect.bottom = Content_Rect.top + Height * scale;
}


//
void ALabel::Draw(HDC hdc)
{
    const int scale = AsConfig::GLOBAL_SCALE;

    int str_left_offset, str_top_offset;
    SIZE str_size;

    // 2. Выводим строку
    SetBkMode(hdc, TRANSPARENT);
    Font.Select(hdc);

    GetTextExtentPoint32(hdc, Content.Get_Content(), Content.Get_Lenght(), &str_size);

    if (str_size.cx > Width * scale)
    {
        if (Last_Char != 0)
        {
            Content.Delete_Last_Symbol();
            Last_Char = 0;
            GetTextExtentPoint32(hdc, Content.Get_Content(), Content.Get_Lenght(), &str_size);
        }
    }

    str_left_offset = Content_Rect.left + (Content_Rect.right - Content_Rect.left) / 2 - str_size.cx / 2;
    str_top_offset = Content_Rect.top + (Content_Rect.bottom - Content_Rect.top) / 2 - str_size.cy / 2 - scale;

    // 2.1 Сначала - тень
    SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
    TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, Content.Get_Content(), Content.Get_Lenght());

    // 2.2 Потом - саму строку
    SetTextColor(hdc, Color.Get_RGB());
    TextOut(hdc, str_left_offset, str_top_offset, Content.Get_Content(), Content.Get_Lenght());
}


//
bool ALabel::Append(wchar_t symbol)
{
    // 1. Поднимаем регистр английских букв
    if (symbol >= L'a' && symbol <= L'z')
        symbol -= L'a' - L'A';

    // 2. Поднимаем регистр кирилических букв
    if (symbol >= 0x430 && symbol <= 0x44f)
        symbol -= 0x20;

    if (symbol >= 0x450 && symbol <= 0x45f)
        symbol -= 0x50;

    // 3. Удаление символа обрабатываем особо 
    if (symbol == 8)
        Content.Delete_Last_Symbol();
    else
    {
        if (symbol >= L' ')
        {
            Last_Char = symbol;
            Content.Append(symbol);
        }
    }

    AsTools::Invalidate_Rect(Content_Rect);

    if (symbol == 0x0d && Content.Get_Lenght() > 0)
        return  true;
    else
        return false;
}
