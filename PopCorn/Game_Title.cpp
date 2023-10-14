#include "Game_Title.h"

// AsGame_Title
const double AsGame_Title::Low_Y_Pos = 135.0;
//
AsGame_Title::AsGame_Title()
    :Game_Title_State(EGame_Title_State::Idle), Start_Tick(0), Destroy_Index(0), Title_Rect()
{

}


//
AsGame_Title::~AsGame_Title()
{
    for (auto* letter : Title_Letters)
        delete letter;

    Title_Letters.erase(Title_Letters.begin(), Title_Letters.end());
}


//
void AsGame_Title::Act()
{
    int curr_tick;
    double y_pos;
    double ratio;

    if (Game_Title_State == EGame_Title_State::Idle || Game_Title_State == EGame_Title_State::Finished)
        return;

    curr_tick = AsConfig::Current_Timer_Tick - Start_Tick;

    switch (Game_Title_State)
    {
    case EGame_Title_State::Game_Over_Descent:
    case EGame_Title_State::Game_Won_Descent:
        if (curr_tick < Descent_Timeout)
            ratio = (double)curr_tick / (double)Descent_Timeout;
        else
        {
            ratio = 1.0;

            if (Game_Title_State == EGame_Title_State::Game_Over_Descent)
                Game_Title_State = EGame_Title_State::Game_Over_Show;
            else
                Game_Title_State = EGame_Title_State::Game_Won_Animate;

            Start_Tick = AsConfig::Current_Timer_Tick;
        }

        y_pos = Low_Y_Pos * ratio;

        for (auto letter : Title_Letters)
            letter->Y_Pos = y_pos;

        Prev_Title_Rect = Title_Rect;

        Title_Rect.top = (int)(y_pos * AsConfig::D_GLOBAL_SCALE);
        Title_Rect.bottom = Title_Rect.top + Height * AsConfig::GLOBAL_SCALE;

        AsTools::Invalidate_Rect(Title_Rect);
        AsTools::Invalidate_Rect(Prev_Title_Rect);
        break;

    case EGame_Title_State::Game_Over_Show:
        if (curr_tick > Game_Over_Show_Timeout)
        {
            Game_Title_State = EGame_Title_State::Game_Over_Destroy;
            Start_Tick = AsConfig::Current_Timer_Tick;
        }
        break;

    case EGame_Title_State::Game_Over_Destroy:
        Destroy_Letters(curr_tick);
        break;
    }
}


//
void AsGame_Title::Destroy_Letters(int curr_tick)
{
    bool can_finish = false;
    bool all_letters_are_finished = true;

    if (Destroy_Index == -1 || curr_tick % Destroy_Delay == 0)
    {// ������� ����������� ������ Destroy_Delay ��� 

        Destroy_Index++;

        if (Destroy_Index >= 0 && Destroy_Index < (int)Title_Letters.size())
            Title_Letters[Destroy_Index]->Destroy();
        else
            can_finish = true;
    }

    for (auto* letter : Title_Letters)
    {
        letter->Act();

        all_letters_are_finished &= letter->Is_Finished();
    }

    if (can_finish && all_letters_are_finished)
        Game_Title_State = EGame_Title_State::Finished;

}


//
void AsGame_Title::Clear(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (Game_Title_State == EGame_Title_State::Idle || Game_Title_State == EGame_Title_State::Finished)
        return;

    if (!IntersectRect(&intersection_rect, &paint_area, &Prev_Title_Rect))
        return;

    AsTools::Rect(hdc, Prev_Title_Rect, AsConfig::BG_Color);
}


//
void AsGame_Title::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;


    if (Game_Title_State == EGame_Title_State::Idle || Game_Title_State == EGame_Title_State::Finished)
        return;

    if (!IntersectRect(&intersection_rect, &paint_area, &Title_Rect))
        return;

    for (auto letter : Title_Letters)
        letter->Draw(hdc, paint_area);
}


//
bool AsGame_Title::Is_Finished()
{
    return false;
}


//
void AsGame_Title::Show(bool game_over)
{
    double title_x, title_end_x;
    double title_y = 0.0;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;

    if (game_over)
    {
        title_x = 31.0;

        Title_Letters.push_back(new AFinal_Letter(title_x, title_y, L'K'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 13.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 29.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 45.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 59.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 82.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 98.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 110.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 122.0, title_y, L'�'));

        Game_Title_State = EGame_Title_State::Game_Over_Descent;
    }
    else
    {
        title_x = 54.0;

        Title_Letters.push_back(new AFinal_Letter(title_x, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 16.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 33.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 46.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 59.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 75.0, title_y, L'�'));
        Title_Letters.push_back(new AFinal_Letter(title_x + 91.0, title_y, L'!'));

        Game_Title_State = EGame_Title_State::Game_Won_Descent;
    }

    title_end_x = Title_Letters[(int)Title_Letters.size() - 1]->X_Pos + 16;

    Title_Rect.left = (int)(title_x * d_scale);
    Title_Rect.top = (int)(title_y * d_scale);
    Title_Rect.right = Title_Rect.left + (int)(title_end_x * d_scale);
    Title_Rect.bottom = Title_Rect.top + Height * AsConfig::GLOBAL_SCALE;

    Start_Tick = AsConfig::Current_Timer_Tick;
    Destroy_Index = -1;

    AsTools::Invalidate_Rect(Title_Rect);
}


//
bool AsGame_Title::Is_Visible()
{
    if (Game_Title_State != EGame_Title_State::Idle)
        return true;
    else
        return false;
}
