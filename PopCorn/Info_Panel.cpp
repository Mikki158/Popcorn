#include "Info_Panel.h"

// AsInfo_Panel
int AsInfo_Panel::Score = 0;
RECT AsInfo_Panel::Logo_Rect;
RECT AsInfo_Panel::Data_Rect;

//
AsInfo_Panel::AsInfo_Panel()
    :Expecting_User_Name(true), Start_Tick(0), Extra_Lives_Count(AsConfig::Initial_Life_Count), 
    Dark_Blue(0, 190, 190), Dark_Pink(228, 0, 228),
    Letter_P(EBrick_Type::Blue, ELetter_Type::P, 214 * AsConfig::GLOBAL_SCALE + 1, 151 * AsConfig::GLOBAL_SCALE + 1),
    Letter_G(EBrick_Type::Blue, ELetter_Type::G, 256 * AsConfig::GLOBAL_SCALE, 153 * AsConfig::GLOBAL_SCALE),
    Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::GLOBAL_SCALE - 1, 154 * AsConfig::GLOBAL_SCALE - 1),
    Floor_Indicator(EMessage_Type::Floor_Is_Over, Score_X + 8, Score_Y + Indicator_Y_Offset), 
    Monster_Indicator(EMessage_Type::Unfreeze_Monsters, Score_X + 90, Score_Y + Indicator_Y_Offset),
    Player_Name_Label(Score_X + 5, Score_Y + 5, Score_Width - 2 * 5, 18, AsConfig::Name_Font, AsConfig::Blue_Color),
    Score_Label(Score_X + 5, Score_Y + 5 + Score_Value_Offset, Score_Width - 2 * 5, 16, AsConfig::Score_Font, AsConfig::White_Color)
{
    const int scale = AsConfig::GLOBAL_SCALE;

    Logo_Rect.left = Score_X * scale;
    Logo_Rect.top = 5 * scale;
    Logo_Rect.right = Logo_Rect.left + 104 * scale;
    Logo_Rect.bottom = Logo_Rect.top + 100 * scale;

    Data_Rect.left = Score_X * scale;
    Data_Rect.top = Score_Y * scale;
    Data_Rect.right = Data_Rect.left + Score_Width * scale;
    Data_Rect.bottom = Data_Rect.top + Score_Height * scale;

    Letter_P.Show_Full_Size();
    Letter_G.Show_Full_Size();
    Letter_M.Show_Full_Size();
}


//
void AsInfo_Panel::Advance(double max_speed)
{
    // Заглушка, пока не используется
}


//
void AsInfo_Panel::Begin_Movement()
{
    // Заглушка, пока не используется
}


//
void AsInfo_Panel::Finish_Movement()
{
    // Заглушка, пока не используется
}


//
double AsInfo_Panel::Get_Speed()
{
    return 0.0; // Заглушка, пока не используется
}


//
void AsInfo_Panel::Act()
{
    int curr_tick;

    Floor_Indicator.Act();
    Monster_Indicator.Act();

    if (Expecting_User_Name)
    {
        curr_tick = AsConfig::Current_Timer_Tick - Start_Tick;

        if (curr_tick > Blink_Timeout)
        {
            Start_Tick = AsConfig::Current_Timer_Tick;

            if (Player_Name_Label.Content.Get_Lenght() == 0)
                Player_Name_Label.Content = L"ВЕДИТЕ ИМЯ";
            else
                Player_Name_Label.Content = L"";

            AsTools::Invalidate_Rect(Data_Rect);
        }
    }
}


//
void AsInfo_Panel::Clear(HDC hdc, RECT& paint_area)
{
    // !!! Надо сделать!
}


//
void AsInfo_Panel::Draw(HDC hdc, RECT& paint_area)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    const wchar_t* pop_str = L"POP";
    const wchar_t* corn_str = L"CORN";
    AString score_str(L"SCORE:");

    RECT intersection_rect;

    // 1. Логотип
    if (IntersectRect(&intersection_rect, &paint_area, &Logo_Rect))
    {
        AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);

        SetBkMode(hdc, TRANSPARENT);

        // 1.1."POP"
        AsConfig::Logo_Pop_Font.Select(hdc);
        SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
        TextOut(hdc, (Logo_X_Pos + Shadow_X_Offset) * scale, (Logo_Y_Pos + Shadow_Y_Offset) * scale, pop_str, 3);

        SetTextColor(hdc, AsConfig::Pink_Color.Get_RGB());
        TextOut(hdc, Logo_X_Pos * scale, Logo_Y_Pos * scale, pop_str, 3);

        // 1.2 "CORN"
        AsConfig::Logo_Corn_Font.Select(hdc);
        SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
        TextOut(hdc, (Logo_X_Pos + Shadow_X_Offset - 5) * scale, (Logo_Y_Pos + Shadow_Y_Offset + 48) * scale, corn_str, wcslen(corn_str));

        SetTextColor(hdc, AsConfig::Pink_Color.Get_RGB());
        TextOut(hdc, (Logo_X_Pos - 5) * scale, (Logo_Y_Pos + 48) * scale, corn_str, wcslen(corn_str));
    }


    // 2. Таблица счета
    if (IntersectRect(&intersection_rect, &paint_area, &Data_Rect))
    {
        // 2.1 Рамка
        AsTools::Rect(hdc, Score_X, Score_Y, Score_Width, 2, Dark_Pink);
        AsTools::Rect(hdc, Score_X, Score_Y, 2, Score_Height, Dark_Pink);
        AsTools::Rect(hdc, Score_X, Score_Y + Score_Height - 2, Score_Width, 2, Dark_Pink);
        AsTools::Rect(hdc, Score_X + Score_Width - 2, Score_Y, 2, Score_Height, Dark_Pink);

        AsTools::Rect(hdc, Score_X + 2, Score_Y + 2, Score_Width - 4, Score_Height - 4, Dark_Blue);

        // 2.2 Бордюр
        AsConfig::Highlight_Color.Select_Pen(hdc);
        MoveToEx(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale, 0);
        LineTo(hdc, (Score_X + 2) * scale, (Score_Y + 2) * scale);
        LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale);

        AsConfig::Shadow_Color.Select_Pen(hdc);
        MoveToEx(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale, 0);
        LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + Score_Height - 2) * scale);
        LineTo(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale);

        // 2.3 Имя игрока
        AsTools::Rect(hdc, Player_Name_Label.Content_Rect, Dark_Pink); // Выводим плашку фона

        
        Player_Name_Label.Draw(hdc);

        // 3. Счет игрока
        AsTools::Rect(hdc, Score_Label.Content_Rect, Dark_Pink); // Выводим плашку фона

        Score_Label.Content = L"SCORE:";
        Score_Label.Content.Append(Score, 6);
        Score_Label.Draw(hdc);

        // 4. Буквы индикаторов
        Letter_P.Draw(hdc, paint_area);
        Letter_G.Draw(hdc, paint_area);
        Letter_M.Draw(hdc, paint_area);

        // 5. Индикаторы
        Floor_Indicator.Draw(hdc, paint_area);
        Monster_Indicator.Draw(hdc, paint_area);

        // 6. Вывод дополнительных жизней
        Show_Extra_Lives(hdc);
    }
}


//
bool AsInfo_Panel::Is_Finished()
{
    return false; // Заглушка, пока не используется
}


//
void AsInfo_Panel::Increase_Life_Count()
{
    if (Extra_Lives_Count < AsConfig::Max_Life_Count)
    {
        Extra_Lives_Count++;
        AsTools::Invalidate_Rect(Data_Rect);
    }

}


//
bool AsInfo_Panel::Decrease_Life_Count()
{
    if (Extra_Lives_Count == 0)
        return false;

    Extra_Lives_Count--;
    AsTools::Invalidate_Rect(Data_Rect);

    return true;
}


//
bool AsInfo_Panel::Edit_Player_Name(wchar_t symbol)
{
    if (Expecting_User_Name)
    {
        Expecting_User_Name = false;
        Player_Name_Label.Content = L"";
    }

    return Player_Name_Label.Append(symbol);
}


//
void AsInfo_Panel::Update_Score(EScore_Event_Type event_type)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    RECT rect;

    switch (event_type)
    {
    case EScore_Event_Type::Hit_Brick:
        Score += 20;
        break;

    case EScore_Event_Type::Hit_Monster:
        Score += 37;
        break;

    case EScore_Event_Type::Catch_Letter:
        Score += 23;
        break;

    default:
        AsConfig::Throw();
        break;
    }

    rect.left = 211 * scale;
    rect.top = 5 * scale;
    rect.right = rect.left + 104 * scale;
    rect.bottom = AsConfig::MAX_Y_POS * scale;

    AsTools::Invalidate_Rect(Data_Rect);
}


//
void AsInfo_Panel::Choose_Font()
{
    //CHOOSEFONT cf{};
    //LOGFONT lf{};

    //cf.lStructSize = sizeof(CHOOSEFONT);
    //cf.lpLogFont = &lf;
    //cf.Flags = CF_SCREENFONTS;
    //cf.nFontType = SCREEN_FONTTYPE;
    //
    //ChooseFont(&cf);
}


//
//void AsInfo_Panel::Draw_String(HDC hdc, RECT rect, AString& str, bool draw_name)
//{
//    const int scale = AsConfig::GLOBAL_SCALE;
//
//    int str_left_offset, str_top_offset;
//    SIZE str_size;
//
//    // 1. Выводим плашку фона
//    AsTools::Rect(hdc, rect, *Dark_Pink);
//
//    // 2. Выводим строку
//    if (draw_name)
//        AsConfig::Name_Font.Select(hdc);
//    else
//        
//
//    GetTextExtentPoint32(hdc, str.Get_Content(), str.Get_Lenght(), &str_size);
//
//    str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
//    str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;
//
//    // 2.1 Сначала - тень
//    SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
//    TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, str.Get_Content(), str.Get_Lenght());
//
//    // 2.2 Потом - саму строку
//    if (draw_name)
//        SetTextColor(hdc, AsConfig::Blue_Color.Get_RGB());
//    else
//        SetTextColor(hdc, AsConfig::White_Color.Get_RGB());
//
//    TextOut(hdc, str_left_offset, str_top_offset, str.Get_Content(), str.Get_Lenght());
//}


//
void AsInfo_Panel::Draw_Extra_Life(HDC hdc, int x_pos, int y_pos)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    RECT rect;

    rect.left = (Score_X + x_pos) * scale;
    rect.top = (Score_Y + y_pos) * scale;
    rect.right = rect.left + 4 * scale;
    rect.bottom = rect.top + 5 * scale;

    AsTools::Ellipse(hdc, rect, AsConfig::Pink_Color);

    rect.left += 8 * scale;
    rect.right += 8 * scale;

    AsTools::Ellipse(hdc, rect, AsConfig::Pink_Color);

    rect.left = (Score_X + x_pos + 2) * scale;
    rect.top = (Score_Y + y_pos + 1) * scale;
    rect.right = rect.left + 8 * scale;
    rect.bottom = rect.top + 3 * scale;

    AsConfig::Blue_Color.Select(hdc);
    AsTools::Round_Rect(hdc, rect);

}


//
void AsInfo_Panel::Show_Extra_Lives(HDC hdc)
{
    int lives_to_draw = Extra_Lives_Count;

    AsTools::Rect(hdc, Score_X + 27, Score_Y + Indicator_Y_Offset, 56, 30, AsConfig::Teleport_Portal_Color);

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
        {
            if (lives_to_draw <= 0)
                break;

            Draw_Extra_Life(hdc, 27 + 6 + i * 16, 57 + j * 7);

            lives_to_draw--;
        }

}