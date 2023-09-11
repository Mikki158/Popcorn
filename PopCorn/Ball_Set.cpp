#include "Ball_Set.h"

// AsBall_Set
//
void AsBall_Set::Advance(double max_speed)
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Advance(max_speed);
}


//
void AsBall_Set::Begin_Movement()
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Begin_Movement();
}


//
void AsBall_Set::Finish_Movement()
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Finish_Movement();
}


//
double AsBall_Set::Get_Speed()
{
    double max_speed = 0.0;
    double curr_speed;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_speed = Balls[i].Get_Speed();
        if (curr_speed > max_speed)
            max_speed = curr_speed;
    }

    return max_speed;
}


//
void AsBall_Set::Act()
{
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBS_On_Platform)
            if (curr_ball->Release_Timer_Tick != 0 && AsConfig::Current_Timer_Tick >= curr_ball->Release_Timer_Tick)
                curr_ball->Release();
    }

}


//
void AsBall_Set::Clear(HDC hdc, RECT& paint_area)
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Clear(hdc, paint_area);
}


//
void AsBall_Set::Draw(HDC hdc, RECT& paint_area)
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Draw(hdc, paint_area);
}


//
bool AsBall_Set::Is_Finished()
{
    return false; // Заглушка, т.к. этот метод не используется
}


//
void AsBall_Set::Release_From_Platform(double platform_x_pos)
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        if (Balls[i].Get_State() == EBS_On_Platform)
            Balls[i].Set_State(EBS_Normal, platform_x_pos, AsConfig::START_BALL_Y_POS);
}


//
void AsBall_Set::Set_On_Platform(double platform_x_pos)
{
    int i;
    for (i = 0; i < 1; i++)
    {
        Balls[i].Set_State(EBS_Normal);
        Balls[i].Set_State(EBS_On_Platform, platform_x_pos, AsConfig::START_BALL_Y_POS);
    }

    for (; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Set_State(EBS_Disabled);

}


//
void AsBall_Set::Set_For_Test()
{
    Balls[0].Set_For_Test();
}


//
void AsBall_Set::Tripple_Balls()
{// "Растроить" самый дальний летящий от платформы мячик 
    double curr_ball_x, curr_ball_y;
    double further_ball_x, further_ball_y;
    ABall* further_ball = nullptr;
    ABall* curr_ball;
    ABall* left_candidate = nullptr;
    ABall* right_candidate = nullptr;

    // 1. Выбираем самый дальний по Y мячик
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() != EBS_Normal)
            continue;

        if (further_ball == nullptr)
            further_ball = curr_ball;
        else
        {
            curr_ball->Get_Center(curr_ball_x, curr_ball_y);
            further_ball->Get_Center(further_ball_x, further_ball_y);

            if (curr_ball_y < further_ball_y)
                further_ball_y = curr_ball_y;
        }
    }

    // 2. Если есть "нормальныый" мячик, то размножаем его
    if (further_ball == nullptr)
        return;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        switch (curr_ball->Get_State())
        {
        case EBS_Disabled:
        case EBS_Lost:
            if (left_candidate == nullptr)
                left_candidate = curr_ball;
            else
                if (right_candidate == nullptr)
                {
                    right_candidate = curr_ball;
                    break; // Оба кандидата найдены
                }
        }
    }

    // 3. Разводим боковые мячики в стороны
    if (left_candidate != nullptr)
    {
        *left_candidate = *further_ball;
        left_candidate->Set_Direction(left_candidate->Get_Direction() + AsConfig::Min_Ball_Angle);
    }

    if (right_candidate != nullptr)
    {
        *right_candidate = *further_ball;
        right_candidate->Set_Direction(right_candidate->Get_Direction() - M_PI / 8.0);
    }
}


//
void AsBall_Set::Inverse_Balls()
{// Меняем направление на обратное у всех мячиков
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBS_Normal)
            curr_ball->Set_Direction(curr_ball->Get_Direction() + M_PI);
    }
}


//
void AsBall_Set::Accelerate()
{
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBS_Normal)
        {
            curr_ball->Set_Speed(curr_ball->Get_Speed() * AsConfig::Ball_Accelerate);
        }
    }
}


//
void AsBall_Set::Reset_Speed()
{
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBS_Normal)
            curr_ball->Set_Speed(AsConfig::Normal_Ball_Speed);
    }
}


//
void AsBall_Set::On_Platform_Advance(double direction, double platform_speed, double max_speed)
{
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBS_On_Platform)
            curr_ball->Forced_Advance(direction, platform_speed, max_speed);
    }
}


//
bool AsBall_Set::Is_Test_Finished()
{
    return Balls[0].Is_Test_Finished(); // В повторяющихся тестах участвует только 0-й мячик
}


//
bool AsBall_Set::All_Balls_Are_Lost()
{
    int lost_balls_count = 0;
    int active_balls_count = 0;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        if (Balls[i].Get_State() == EBS_Disabled)
            continue;

        active_balls_count++;

        if (Balls[i].Get_State() == EBS_Lost)
        {
            lost_balls_count++;

            continue;
        }

        //Balls[i].Move();
    }

    if (active_balls_count == lost_balls_count)
        return true;
    else
        return false;
}


//
bool AsBall_Set::Release_Next_Ball()
{
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];
        if (curr_ball->Get_State() == EBS_On_Platform)
        {
            curr_ball->Release();
            return true;
        }
    }

    return false;
}