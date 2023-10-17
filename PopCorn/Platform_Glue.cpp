#include "Platform_Glue.h"

// AsPlatform_Glue
//
const double AsPlatform_Glue::Max_Glue_Spot_Height_Ratio = 1.0;
const double AsPlatform_Glue::Min_Glue_Spot_Height_Ratio = 0.2;
const double AsPlatform_Glue::Glue_Spot_Height_Ratio_Step = 0.05;

//
AsPlatform_Glue::AsPlatform_Glue(AsPlatform_State& platform_state)
    :Glue_Spot_Height_Ratio(0.0), Platform_State(&platform_state)
{
}

//
bool AsPlatform_Glue::Act(AsBall_Set* ball_set, EPlatform_State& next_state)
{
    next_state = EPlatform_State::Unknown;

    switch (Platform_State->Glue)
    {
    case EPlatform_Transformation::Init:
        if (Glue_Spot_Height_Ratio < Max_Glue_Spot_Height_Ratio)
            Glue_Spot_Height_Ratio += Glue_Spot_Height_Ratio_Step;
        else
            Platform_State->Glue = EPlatform_Transformation::Active;

        return true;

    case EPlatform_Transformation::Active:
        break;

    case EPlatform_Transformation::Finalize:
        if (Glue_Spot_Height_Ratio > Min_Glue_Spot_Height_Ratio)
        {
            Glue_Spot_Height_Ratio -= Glue_Spot_Height_Ratio_Step;

            while (ball_set->Release_Next_Ball())
            {
            }
        }
        else
        {
            Platform_State->Glue = EPlatform_Transformation::Unknown;
            next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
        }

        return true;

    default:
        AsConfig::Throw();
        break;
    }

    return false;
}


//
void AsPlatform_Glue::Draw_State(HDC hdc, double x_pos)
{// Рисуем платформу с растикающим клеем

    HRGN region;
    RECT glue_rect;

    glue_rect.left = (int)((x_pos + 5.0) * AsConfig::D_GLOBAL_SCALE);
    glue_rect.top = (AsConfig::Platform_Y_POS + 1) * AsConfig::GLOBAL_SCALE;
    glue_rect.right = glue_rect.left + AsConfig::Platform_NORMAL_INNER_WIDTH * AsConfig::GLOBAL_SCALE;
    glue_rect.bottom = glue_rect.top + (AsConfig::Platform_Height - 2) * AsConfig::GLOBAL_SCALE;

    region = CreateRectRgnIndirect(&glue_rect);
    SelectClipRgn(hdc, region);

    AsConfig::BG_Color.Select(hdc);

    Draw_Glue_Spot(hdc, 0, 9, 5, x_pos);
    Draw_Glue_Spot(hdc, 6, 6, 5, x_pos);
    Draw_Glue_Spot(hdc, 9, 9, 6, x_pos);

    AsConfig::White_Color.Select(hdc);

    Draw_Glue_Spot(hdc, 0, 9, 4, x_pos);
    Draw_Glue_Spot(hdc, 6, 6, 4, x_pos);
    Draw_Glue_Spot(hdc, 9, 9, 5, x_pos);

    SelectClipRgn(hdc, 0);
    DeleteObject(region);
}


//
void AsPlatform_Glue::Reset()
{
    Glue_Spot_Height_Ratio = Min_Glue_Spot_Height_Ratio;
}


//
void AsPlatform_Glue::Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height, double x_pos)
{// Рисуем полуэлипс как "пятно" клея

    int platform_top = (AsConfig::Platform_Y_POS + 1) * AsConfig::GLOBAL_SCALE;
    int spot_height = (int)((double)height * AsConfig::D_GLOBAL_SCALE * Glue_Spot_Height_Ratio);
    RECT spot_rect;


    spot_rect.left = (int)((x_pos + 5.0 + (double)x_offset) * AsConfig::D_GLOBAL_SCALE);
    spot_rect.top = platform_top - spot_height;
    spot_rect.right = spot_rect.left + width * AsConfig::GLOBAL_SCALE;
    spot_rect.bottom = platform_top + spot_height - AsConfig::GLOBAL_SCALE;


    Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right - 1, spot_rect.bottom - 1,
        spot_rect.left, platform_top - 1, spot_rect.right - 1, platform_top - 1);
}