#include "Engine.h"





// AsEngine
//
AsEngine::AsEngine()
    :Game_State(EGS_Lost_Ball), Rest_Distance(0.0), Movers{}, Modules{}, Life_Count(AsConfig::Initial_Life_Count)
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
    Platform.Init(&Ball_Set);

    AFalling_Letter::Init();
     
    ABall::Add_Hit_Checker(&Border);
    ABall::Add_Hit_Checker(&Level);
    ABall::Add_Hit_Checker(&Platform);

    Level.Set_Current_Level(AsLevel::Level_01);

   //for (int i = 0; i < AsConfig::Max_Balls_Count; i++)
   //    Balls[i].Set_State(EBS_Disabled, -10);

    //Platform.Set_State(EPlatform_State::Glue_Init);

    Platform.Redraw_Platform();

    SetTimer(AsConfig::HWnd, TIMER_ID, 1000 / AsConfig::FPS, 0);

    // Movers
    memset(Movers, 0, sizeof(Movers));
    Movers[0] = &Platform;
    Movers[1] = &Ball_Set;

    // Modules
    memset(Modules, 0, sizeof(Modules));
    Modules[0] = &Level;
    Modules[1] = &Border;
    Modules[2] = &Platform;
    Modules[3] = &Ball_Set;
}


//
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)// отрисовка экрана игры
{  
    SetGraphicsMode(hdc, GM_ADVANCED);

    for (int i = 0; i < AsConfig::Max_Modules_Count; i++)
        if (Modules[i] != nullptr)
            Modules[i]->Clear(hdc, paint_area);

    for (int i = 0; i < AsConfig::Max_Modules_Count; i++)
        if(Modules[i] != nullptr)
            Modules[i]->Draw(hdc, paint_area);
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
        Platform.On_Space_Key(key_down);
        break;

    case EKT_F:
        Ball_Set.Inverse_Balls();
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
        Ball_Set.Set_For_Test();
        Game_State = EGS_Play_Level;
        break;

    case EGS_Play_Level:
        Play_Level();
        break;

    case EGS_Lost_Ball:
        if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
        {
            Game_State = EGS_Restart_Level;
            Platform.Set_State(EPlatform_State::Rolling);
        }
        break;

    case EGS_Restart_Level:
        if (Platform.Has_State(EPlatform_Substate_Regular::Ready))
        {
            Game_State = EGS_Play_Level;
            Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos());
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
    Advance_Movers();

    if (Ball_Set.All_Balls_Are_Lost())
    {
        Game_State = EGS_Lost_Ball;
        Level.Stop();
        Platform.Set_State(EPlatform_State::Meltdown);
    }
    else
        Ball_Set.Accelerate();


    if (Ball_Set.Is_Test_Finished())
        Game_State = EGS_Test_Ball;
}


//
void AsEngine::Advance_Movers()
{
    double max_speed = 0.0;
    double curr_speed;

    // 1. Получаем максимальную скорость движущихся объектов
    for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
        if (Movers[i] != nullptr)
        {
            Movers[i]->Begin_Movement();

            curr_speed = fabs(Movers[i]->Get_Speed());

            if (curr_speed > max_speed)
                max_speed = curr_speed;
        }

    // 2. Смещаем все объекты
    Rest_Distance += max_speed;

    while (Rest_Distance > 0.0)
    {
        for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
        {
            if (Movers[i] != nullptr)
                Movers[i]->Advance(max_speed);
        }

        //Platform.Advance(max_speed);
        Rest_Distance -= AsConfig::Moving_STEP_SIZE;
    }

    // 3. Заканчиваем все движения на этом кадре
    for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
        if (Movers[i] != nullptr)
            Movers[i]->Finish_Movement();
}


//
void AsEngine::Act()
{
    Level.Act();
    Platform.Act();

    if(! Platform.Has_State(EPlatform_Substate_Regular::Ready))
        Ball_Set.Act();

    AFalling_Letter *falling_letter;
    int index = 0;

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
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;// !!! Пока отменился только клей

    case ELT_I:
        Ball_Set.Inverse_Balls();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELT_C:
        Ball_Set.Reset_Speed();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELT_M:
        break;

    case ELT_G:
        if (Life_Count < AsConfig::Max_Life_Count)
            Life_Count++; // !!! Отобразить на индикаторе!
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELT_K:
        Platform.Set_State(EPlatform_State::Glue);
        break;

    case ELT_W:
        break;

    case ELT_P:
        AsConfig::Level_Has_Floor = true;
        Border.Redraw_Floor();
        // !!! Отобразить на индикаторе
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELT_L:
        break;

    case ELT_T:
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        Ball_Set.Tripple_Balls();
        break;

    case ELT_PLUS:
        break;

    default:
        AsConfig::Throw();
        break;
    }

    falling_letter->Finalize();
}







