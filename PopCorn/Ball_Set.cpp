#include "Ball_Set.h"

// AsBall_Set
//
void AsBall_Set::Act()
{
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBall_State::On_Platform)
            if (curr_ball->Release_Timer_Tick != 0 && AsConfig::Current_Timer_Tick >= curr_ball->Release_Timer_Tick)
                curr_ball->Release();
    }
}


//
void AsBall_Set::Release_From_Platform(double platform_x_pos)
{
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        if (Balls[i].Get_State() == EBall_State::On_Platform)
            Balls[i].Set_State(EBall_State::Normal, platform_x_pos, AsConfig::START_BALL_Y_POS);
}


//
void AsBall_Set::Set_On_Platform(double platform_x_pos)
{
    int i;
    for (i = 0; i < 1; i++)
    {
        Balls[i].Set_State(EBall_State::Normal);
        Balls[i].Set_State(EBall_State::On_Platform, platform_x_pos, AsConfig::START_BALL_Y_POS);
    }

    for (; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Set_State(EBall_State::Disabled);

}


//
void AsBall_Set::Set_For_Test()
{
    Balls[0].Set_For_Test();
}


//
void AsBall_Set::Tripple_Balls()
{// "–астроить" самый дальний лет€щий от платформы м€чик 
    double curr_ball_x, curr_ball_y;
    double further_ball_x, further_ball_y;
    ABall* further_ball = nullptr;
    ABall* curr_ball;
    ABall* left_candidate = nullptr;
    ABall* right_candidate = nullptr;

    // 1. ¬ыбираем самый дальний по Y м€чик
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() != EBall_State::Normal)
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

    // 2. ≈сли есть "нормальныый" м€чик, то размножаем его
    if (further_ball == nullptr)
        return;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        switch (curr_ball->Get_State())
        {
        case EBall_State::Disabled:
        case EBall_State::Lost:
            if (left_candidate == nullptr)
                left_candidate = curr_ball;
            else
                if (right_candidate == nullptr)
                {
                    right_candidate = curr_ball;
                    break; // ќба кандидата найдены
                }
        }
    }

    // 3. –азводим боковые м€чики в стороны
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
{// ћен€ем направление на обратное у всех м€чиков
    ABall* curr_ball;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        curr_ball = &Balls[i];

        if (curr_ball->Get_State() == EBall_State::Normal)
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

        if (curr_ball->Get_State() == EBall_State::Normal)
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

        if (curr_ball->Get_State() == EBall_State::Normal)
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

        if (curr_ball->Get_State() == EBall_State::On_Platform)
            curr_ball->Forced_Advance(direction, platform_speed, max_speed);
    }
}


//
bool AsBall_Set::Is_Test_Finished()
{
    return Balls[0].Is_Test_Finished(); // ¬ повтор€ющихс€ тестах участвует только 0-й м€чик
}


//
bool AsBall_Set::All_Balls_Are_Lost()
{
    int lost_balls_count = 0;
    int active_balls_count = 0;

    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
    {
        if (Balls[i].Get_State() == EBall_State::Disabled)
            continue;

        active_balls_count++;

        if (Balls[i].Get_State() == EBall_State::Lost)
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
        if (curr_ball->Get_State() == EBall_State::On_Platform)
        {
            curr_ball->Release();
            return true;
        }
    }

    return false;
}


//
bool AsBall_Set::Get_Next_Game_Object(int& index, AGame_Object** game_obj)
{
    if (index < 0 || index >= AsConfig::Max_Balls_Count)
        return false;

    *game_obj = &Balls[index++];

    return true;
}