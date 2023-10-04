#include "Info_Panel.h"

// AsInfo_Panel
//
AsInfo_Panel::AsInfo_Panel()
    :Logo_Pop_Font(0), Logo_Corn_Font(0), Name_Font(0), Score_Font(0), Shadow_Color(nullptr), Highlight_Color(nullptr),
    Dark_Blue(nullptr), Dark_Pink(nullptr),
    Letter_P(EBrick_Type::Blue, ELetter_Type::P, 214 * AsConfig::GLOBAL_SCALE + 1, 151 * AsConfig::GLOBAL_SCALE + 1),
    Letter_G(EBrick_Type::Blue, ELetter_Type::G, 256 * AsConfig::GLOBAL_SCALE, 153 * AsConfig::GLOBAL_SCALE),
    Letter_M(EBrick_Type::Blue, ELetter_Type::M, 297 * AsConfig::GLOBAL_SCALE - 1, 154 * AsConfig::GLOBAL_SCALE - 1)
{
    Letter_P.Show_Full_Size();
    Letter_G.Show_Full_Size();
    Letter_M.Show_Full_Size();
}


//
AsInfo_Panel::~AsInfo_Panel()
{
    delete Shadow_Color;
    delete Highlight_Color;
    delete Dark_Blue;
    delete Dark_Pink;

    if (Logo_Pop_Font != 0)
        DeleteObject(Logo_Pop_Font);

    if (Logo_Corn_Font != 0)
        DeleteObject(Logo_Corn_Font);

    if (Name_Font != 0)
        DeleteObject(Name_Font);

    if (Score_Font != 0)
        DeleteObject(Score_Font);
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
    // !!! Надо сделать!

    //const int scale = AsConfig::GLOBAL_SCALE;
    //RECT rect;
    //
    //rect.left = 211 * scale;
    //rect.top = 5 * scale;
    //rect.right = rect.left + 104 * scale;
    //rect.bottom = 199 * scale;
    //
    //AsTools::Invalidate_Rect(rect);

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
    //const wchar_t* player_str = L"COMPUTER";
    //const wchar_t* score_str = L"SCORE:000000";
    AString score_str(L"SCORE:");

    int logo_x_pos = 212 * scale;
    int logo_y_pos = 0 * scale;
    int shadow_x_offset = 5 * scale;
    int shadow_y_offset = 5 * scale;
    RECT rect;

    // 1. Логотип
    AsTools::Rect(hdc, 211, 5, 104, 100, AsConfig::Blue_Color);


    SetBkMode(hdc, TRANSPARENT);

    // 1.1."POP"
    SelectObject(hdc, Logo_Pop_Font);
    SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
    TextOut(hdc, logo_x_pos + shadow_x_offset, logo_y_pos + shadow_y_offset, pop_str, 3);

    SetTextColor(hdc, AsConfig::Pink_Color.Get_RGB());
    TextOut(hdc, logo_x_pos, logo_y_pos, pop_str, 3);

    // 1.2 "CORN"
    SelectObject(hdc, Logo_Corn_Font);
    SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
    TextOut(hdc, logo_x_pos + shadow_x_offset - 5 * scale, logo_y_pos + shadow_y_offset + 48 * scale, corn_str, wcslen(corn_str));

    SetTextColor(hdc, AsConfig::Pink_Color.Get_RGB());
    TextOut(hdc, logo_x_pos - 5 * scale, logo_y_pos + 48 * scale, corn_str, wcslen(corn_str));


    // 2. Таблица счета
    // 2.1 Рамка
    AsTools::Rect(hdc, Score_X, Score_Y, Score_Width, 2, *Dark_Pink);
    AsTools::Rect(hdc, Score_X, Score_Y, 2, Score_Height, *Dark_Pink);
    AsTools::Rect(hdc, Score_X, Score_Y + Score_Height - 2, Score_Width, 2, *Dark_Pink);
    AsTools::Rect(hdc, Score_X + Score_Width - 2, Score_Y, 2, Score_Height, *Dark_Pink);

    AsTools::Rect(hdc, Score_X + 2, Score_Y + 2, Score_Width - 4, Score_Height - 4, *Dark_Blue);

    // 2.2 Бордюр
    Highlight_Color->Select_Pen(hdc);
    MoveToEx(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale, 0);
    LineTo(hdc, (Score_X + 2) * scale, (Score_Y + 2) * scale);
    LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale);

    Shadow_Color->Select_Pen(hdc);
    MoveToEx(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + 2) * scale, 0);
    LineTo(hdc, (Score_X + Score_Width - 2) * scale, (Score_Y + Score_Height - 2) * scale);
    LineTo(hdc, (Score_X + 2) * scale, (Score_Y + Score_Height - 2) * scale);

    // 2.3 Имя игрока
    rect.left = (Score_X + 5) * scale;
    rect.top = (Score_Y + 5) * scale;
    rect.right = rect.left + (Score_Width - 2 * 5) * scale;
    rect.bottom = rect.top + 16 * scale;

    Player_Name = L"COMPUTER";
    Draw_String(hdc, rect, Player_Name, true);

    // 3. Счет игрока
    rect.top += Score_Value_Offset * scale;
    rect.bottom += Score_Value_Offset * scale;

    score_str.Append(AsConfig::Score);

    Draw_String(hdc, rect, score_str, false);

    // 4. Буквы индикаторов
    Letter_P.Draw(hdc, paint_area); 
    Letter_G.Draw(hdc, paint_area);
    Letter_M.Draw(hdc, paint_area);

    // 5. Индикаторы
    AsTools::Rect(hdc, Score_X + 8, Score_Y + 55, 12, 30, AsConfig::Teleport_Portal_Color); // Левый
    AsTools::Rect(hdc, Score_X + 27, Score_Y + 55, 56, 30, AsConfig::Teleport_Portal_Color); // Средний
    AsTools::Rect(hdc, Score_X + 90, Score_Y + 55, 12, 30, AsConfig::Teleport_Portal_Color); // Правый

    // 6. Вывод дополнительных жизней
    Show_Extra_Lives(hdc);
}


//
bool AsInfo_Panel::Is_Finished()
{
    return false; // Заглушка, пока не используется
}


//
void AsInfo_Panel::Init()
{
    LOGFONT log_font{};

    log_font.lfHeight = -128;
    log_font.lfWeight = 900;
    log_font.lfOutPrecision = 3;
    log_font.lfClipPrecision = 2;
    log_font.lfQuality = 1;
    log_font.lfPitchAndFamily = 34;
    wcscpy_s(log_font.lfFaceName, L"Arial Black");

    Logo_Pop_Font = CreateFontIndirect(&log_font);

    log_font.lfHeight = -96;
    Logo_Corn_Font = CreateFontIndirect(&log_font);


    log_font.lfHeight = -48;
    log_font.lfWeight = 700;
    log_font.lfOutPrecision = 3;
    log_font.lfClipPrecision = 2;
    log_font.lfQuality = 1;
    log_font.lfPitchAndFamily = 49;
    wcscpy_s(log_font.lfFaceName, L"Consolas");

    Name_Font = CreateFontIndirect(&log_font);

    log_font.lfHeight = -44;
    Score_Font = CreateFontIndirect(&log_font);

    Shadow_Color = new AColor(AsConfig::BG_Color, AsConfig::GLOBAL_SCALE);
    Highlight_Color = new AColor(AsConfig::White_Color, AsConfig::GLOBAL_SCALE);
    Dark_Blue = new AColor(0, 190, 190);
    Dark_Pink = new AColor(228, 0, 228);
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
void AsInfo_Panel::Draw_String(HDC hdc, RECT rect, AString& str, bool draw_name)
{
    const int scale = AsConfig::GLOBAL_SCALE;

    int str_left_offset, str_top_offset;
    SIZE str_size;

    // 1. Выводим плашку фона
    AsTools::Rect(hdc, rect, *Dark_Pink);

    // 2. Выводим строку
    if (draw_name)
        SelectObject(hdc, Name_Font);
    else
        SelectObject(hdc, Score_Font);

    GetTextExtentPoint32(hdc, str.Get_Content(), str.Get_Lenght(), &str_size);

    str_left_offset = rect.left + (rect.right - rect.left) / 2 - str_size.cx / 2;
    str_top_offset = rect.top + (rect.bottom - rect.top) / 2 - str_size.cy / 2 - scale;

    // 2.1 Сначала - тень
    SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());
    TextOut(hdc, str_left_offset + 2 * scale, str_top_offset + 2 * scale, str.Get_Content(), str.Get_Lenght());

    // 2.2 Потом - саму строку
    if (draw_name)
        SetTextColor(hdc, AsConfig::Blue_Color.Get_RGB());
    else
        SetTextColor(hdc, AsConfig::White_Color.Get_RGB());

    TextOut(hdc, str_left_offset, str_top_offset, str.Get_Content(), str.Get_Lenght());
}


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
    int lives_to_draw = AsConfig::Extra_Lives_Count;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
        {
            if (lives_to_draw <= 0)
                break;

            Draw_Extra_Life(hdc, 27 + 6 + i * 16, 57 + j * 7);

            lives_to_draw--;
        }

}