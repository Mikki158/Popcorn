#include "Final_Letter.h"

// AFinal_Letter
//
AFinal_Letter::AFinal_Letter(double x_pos, double y_pos, const wchar_t letter, int width, int height)
    :X_Pos(x_pos), Y_Pos(y_pos), Got_Letter_Size(false), Final_Letter_State(EFinal_Letter_State::Show_Letter), 
    Letter(letter), Width(width), Height(height), Letter_Rect{}
{

}


//
void AFinal_Letter::Act()
{
    if (Act_On_Explosion())
        Final_Letter_State = EFinal_Letter_State::Finished;

    AsTools::Invalidate_Rect(Letter_Rect);
}


void AFinal_Letter::Clear(HDC hdc, RECT& paint_area)
{

}


//
void AFinal_Letter::Draw(HDC hdc, RECT& paint_area)
{
    SIZE letter_size;

    switch (Final_Letter_State)
    {
    case EFinal_Letter_State::Show_Letter:
        Draw_Letter(hdc, true);

        if (!Got_Letter_Size)
        {
            GetTextExtentPoint32(hdc, &Letter, 1, &letter_size);

            Width = letter_size.cx + letter_size.cx / 5; // 120% ширины
            Height = letter_size.cy;

            Got_Letter_Size = true;
        }
        break;

    case EFinal_Letter_State::Hide_Letter:
        Draw_Letter(hdc, false);
        Final_Letter_State = EFinal_Letter_State::Explosing;
        break;

    case EFinal_Letter_State::Explosing:
        Draw_Explosion(hdc, paint_area);
        break;

    case EFinal_Letter_State::Finished:
        break;

    default:
        AsConfig::Throw();
        break;
    }
}


//
void AFinal_Letter::Draw_Letter(HDC hdc, bool in_color)
{
    SetBkMode(hdc, TRANSPARENT);
    AsConfig::Game_Over_Font.Select(hdc);

    if(in_color)
        SetTextColor(hdc, AsConfig::White_Color.Get_RGB());
    else
        SetTextColor(hdc, AsConfig::BG_Color.Get_RGB());

    TextOut(hdc, (int)(X_Pos * AsConfig::D_GLOBAL_SCALE), (int)(Y_Pos * AsConfig::D_GLOBAL_SCALE), &Letter, 1);
}


//
bool AFinal_Letter::Is_Finished()
{
    if (Final_Letter_State == EFinal_Letter_State::Finished)
        return true;
    else
        return false;
}


//
void AFinal_Letter::Destroy()
{
    Letter_Rect.left = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE);
    Letter_Rect.top = (int)(Y_Pos * AsConfig::D_GLOBAL_SCALE);
    Letter_Rect.right = Letter_Rect.left + Width;
    Letter_Rect.bottom = Letter_Rect.top + Height;

    Final_Letter_State = EFinal_Letter_State::Hide_Letter;

    Start_Explosion(Letter_Rect);
    AsTools::Invalidate_Rect(Letter_Rect);
}
