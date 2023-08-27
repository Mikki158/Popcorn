#include "Engine.h"

// AsEngine
//
AsEngine::AsEngine()
    :Game_State(EGS_Lost_Ball)
{

}


//
void AsEngine::Init_Engine(HWND hwnd)// Настройка игры при старте
{
    SYSTEMTIME sys_time;
    FILETIME file_time;

    GetSystemTime(&sys_time);
    SystemTimeToFileTime(&sys_time, &file_time);
    srand(file_time.dwLowDateTime);

    AsConfig::HWnd = hwnd;
    AActive_Brick_Pink_Blue::Setup_Color();

    
    Level.Init();

    AFalling_Letter::Init();
     
    ABall::Add_Hit_Checker(&Border);
    ABall::Add_Hit_Checker(&Level);
    ABall::Add_Hit_Checker(&Platform);

    Level.Set_Current_Level(AsLevel::Level_01);

   //for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
   //    Balls[i].Set_State(EBS_Disabled, -10);

    Platform.Redraw_Platform();

    SetTimer(AsConfig::HWnd, TIMER_ID, 1000 / AsConfig::FPS, 0);
}


//
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)// отрисовка экрана игры
{  
    SetGraphicsMode(hdc, GM_ADVANCED);


    Level.Draw(hdc, paint_area); 
    Border.Draw(hdc);
    Platform.Draw(hdc, paint_area);
    
    for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Draw(hdc, paint_area);

    //Ball.Draw(hdc, paint_area);
}


//
int AsEngine::On_Key(EKey_Type key_type, bool key_down)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.Move(true, key_down);
        break;

    case EKT_Right:
        Platform.Move(false, key_down);
        break;

    case EKT_Space:
        if(key_down)
            if (Platform.Get_State() == EPS_Ready)
            {
                for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
                    if(Balls[i].Get_State() == EBS_On_Platform)
                        Balls[i].Set_State(EBS_Normal, Platform.Get_Middle_Pos(), AsConfig::START_BALL_Y_POS);

                Platform.Set_State(EPS_Normal);
            }
        break;

    }
    return 0;
}


//
int AsEngine::On_Timer()
{
    AsConfig::Current_Timer_Tick += 1;

    switch (Game_State)
    {
    case EGS_Test_Ball:
        Balls[0].Set_For_Test(); // В повторяющихся тестах участвует только 0-й мячик
        Game_State = EGS_Play_Level;
        break;

    case EGS_Play_Level:
        Play_Level();
        break;

    case EGS_Lost_Ball:
        if (Platform.Get_State() == EPS_Missing)
        {
            Game_State = EGS_Restart_Level;
            Platform.Set_State(EPS_Roll_In);
        }
        break;

    case EGS_Restart_Level:
        if (Platform.Get_State() == EPS_Ready)
        {
            Restart_Level();
        }
        break;

    default:
        break;
    }

    Act();

    return 0;
}


//
void AsEngine::Play_Level()
{
    int lost_balls_count = 0;
    int active_balls_count = 0;
    double rest_distance;
    double max_speed;

    // 1. Смещаем платформу
    max_speed = fabs(Platform.Speed);
    rest_distance = max_speed;

    while (rest_distance > 0.0)
    {
        Platform.Advance(max_speed);
        rest_distance -= AsConfig::STEP_SIZE;
    }

    Platform.Redraw_Platform();


    // 2. Смещаем мячики
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

        Balls[i].Move();
    }

    if (active_balls_count == lost_balls_count)
    {// Потеряли все мячики

        Game_State = EGS_Lost_Ball;
        Level.Stop();
        Platform.Set_State(EPS_Meltdown);
    }

    if(active_balls_count == 1)
        if (Balls[0].Is_Test_Finished())
            Game_State = EGS_Test_Ball; // В повторяющихся тестах участвует только 0-й мячик
}


//
void AsEngine::Restart_Level()
{
    Game_State = EGS_Play_Level;

    for (int i = 0; i < 3; i++)
        Balls[i].Set_State(EBS_On_Platform, Platform.Get_Middle_Pos(), AsConfig::START_BALL_Y_POS);

    for (int i = 3; i < AsConfig::Max_Balls_Count; i++)
        Balls[i].Set_State(EBS_Disabled);
}


//
void AsEngine::Act()
{
    Level.Act();
    Platform.Act();

    AFalling_Letter *falling_letter;
    int index = 0;
    RECT intersection_rect, platform_rect, falling_letter_rect;

    while (Level.Get_Next_Falling_Leter(&falling_letter, index))
    {
        if (Platform.Hit_by(falling_letter))
            On_Falling_Letter(falling_letter);
    }
}


//
void AsEngine::On_Falling_Letter(AFalling_Letter* falling_letter)
{
    switch (falling_letter->letter_type)
    {
    case ELT_O:
        break;

    case ELT_I:
        break;

    case ELT_C:
        break;

    case ELT_M:
        break;

    case ELT_G:
        break;

    case ELT_K:
        break;

    case ELT_W:
        break;

    case ELT_P:
        break;

    case ELT_L:
        break;

    case ELT_T:
        break;

    case ELT_PLUS:
        break;

    default:
        AsConfig::Throw();
        break;
    }

    falling_letter->Finalize();
}







