#include "Explosive_Ball.h"

// AExplosive_Ball
AColor AExplosive_Ball::Fading_Pink_Colors[MAX_FADE_STEP];
AColor AExplosive_Ball::Fading_Blue_Colors[MAX_FADE_STEP];

//
AExplosive_Ball::AExplosive_Ball()
    :Is_Pink(false), X_Pos(0), Y_Pos(0), Step_Count(0), Start_Expanding_Tick(0), Start_Fading_Tick(0), Time_Offset(0),
    Size(0.0), Max_Size(0.0), Size_Step(0.0), Explosive_Ball_State(EExplosive_Ball_State::Idle), Ball_Rect{}
{

}


//
void AExplosive_Ball::Act()
{
    switch (Explosive_Ball_State)
    {
    case EExplosive_Ball_State::Idle:
        break;

    case EExplosive_Ball_State::Charging:
        if (AsConfig::Current_Timer_Tick >= Start_Expanding_Tick)
            Explosive_Ball_State = EExplosive_Ball_State::Expanding;
        break;

    case EExplosive_Ball_State::Expanding:
        Size += Size_Step;

        if (Size >= Max_Size)
        {
            Explosive_Ball_State = EExplosive_Ball_State::Fading;

            Start_Fading_Tick = AsConfig::Current_Timer_Tick;
        }
        else
            Update_Ball_Rect();

        break;

    case EExplosive_Ball_State::Fading:
        if (AsConfig::Current_Timer_Tick > Start_Fading_Tick + Fading_Timeout)
            Explosive_Ball_State = EExplosive_Ball_State::Idle;
        break;

    default:
        break;
    }
}


//
void AExplosive_Ball::Clear(HDC hdc, RECT& paint_area)
{
    // Заглушка, не используется
}


//
void AExplosive_Ball::Draw(HDC hdc, RECT& paint_area)
{
    int curr_timeout;
    int color_index;
    double ratio;
    const AColor* color;

    switch (Explosive_Ball_State)
    {
    case EExplosive_Ball_State::Idle:
        break;

    case EExplosive_Ball_State::Charging:
        break;

    case EExplosive_Ball_State::Expanding:
        if (Is_Pink)
            color = &AsConfig::Explosion_Pink_Color;
        else
            color = &AsConfig::Explosion_Blue_Color;

        AsTools::Ellipse(hdc, Ball_Rect, *color);
        break;

    case EExplosive_Ball_State::Fading:
        curr_timeout = AsConfig::Current_Timer_Tick - Start_Fading_Tick;

        if (curr_timeout > Fading_Timeout)
            curr_timeout = Fading_Timeout;

        if (curr_timeout == Fading_Timeout)
            AsTools::Ellipse(hdc, Ball_Rect, AsConfig::BG_Color);
        else
        {
            ratio = (double)curr_timeout / (double)Fading_Timeout;
            color_index = (int)round(ratio * (double)(MAX_FADE_STEP - 1));

            if (Is_Pink)
                color = &Fading_Pink_Colors[color_index];
            else
                color = &Fading_Blue_Colors[color_index];

            AsTools::Ellipse(hdc, Ball_Rect, *color);
        }

        break;

    default:
        break;
    }

}


//
bool AExplosive_Ball::Is_Finished()
{
    if (Explosive_Ball_State == EExplosive_Ball_State::Idle)
        return true;
    else
        return false;
}


//
void AExplosive_Ball::Setup_Color()
{
    for (int i = 0; i < MAX_FADE_STEP; i++)
    {
        AsTools::Get_Fading_Color(AsConfig::Pink_Color, i, Fading_Pink_Colors[i], MAX_FADE_STEP);
        AsTools::Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Colors[i], MAX_FADE_STEP);
    }
}


//
void AExplosive_Ball::Explode(int x_pos, int y_pos, int size, bool is_pink, int time_offset, int step_count)
{
    Explosive_Ball_State = EExplosive_Ball_State::Expanding;

    X_Pos = x_pos;
    Y_Pos = y_pos;
    Max_Size = size;
    Size = 0.0;
    Time_Offset = time_offset;
    Step_Count = step_count;
    Is_Pink = is_pink;

    Start_Expanding_Tick = AsConfig::Current_Timer_Tick + Time_Offset;
    Explosive_Ball_State = EExplosive_Ball_State::Charging;

    Size_Step = (double)size / (double)Step_Count;

    Update_Ball_Rect();
}


//
void AExplosive_Ball::Update_Ball_Rect()
{
    Ball_Rect.left = X_Pos - (int)(Size / 2.0);
    Ball_Rect.top = Y_Pos - (int)(Size / 2.0);
    Ball_Rect.right = Ball_Rect.left + (int)Size;
    Ball_Rect.bottom = Ball_Rect.top + (int)Size;

    //AsTools::Invalidate_Rect(Ball_Rect);
}