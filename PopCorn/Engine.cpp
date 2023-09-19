#include "Engine.h"





// AsEngine
//
AsEngine::AsEngine()
    :Game_State(EGame_State::Lost_Ball), Rest_Distance(0.0), Modules{}, Life_Count(AsConfig::Initial_Life_Count)
{

}


//
void AsEngine::Init_Engine(HWND hwnd)// Настройка игры при старте
{
    int index = 0;
    SYSTEMTIME sys_time;
    FILETIME file_time;

    GetSystemTime(&sys_time);
    SystemTimeToFileTime(&sys_time, &file_time);
    srand(file_time.dwLowDateTime);

    AsConfig::HWnd = hwnd;
    AActive_Brick_Pink_Blue::Setup_Color();

    
    Level.Init();
    Platform.Init(&Ball_Set, &Laser_Beam_Set);

    AFalling_Letter::Init();
     
    ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
    ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
    ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);

    ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);

    Level.Set_Current_Level(AsLevel::Level_01);

    Platform.Redraw_Platform();

    SetTimer(AsConfig::HWnd, TIMER_ID, 1000 / AsConfig::FPS, 0);

    // Modules
    memset(Modules, 0, sizeof(Modules));

    Add_Next_Module(index, &Level);
    Add_Next_Module(index, &Border);
    Add_Next_Module(index, &Platform);
    Add_Next_Module(index, &Ball_Set);
    Add_Next_Module(index, &Laser_Beam_Set);
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
    case EKey_Type::Left:
        Platform.Move(true, key_down);
        break;

    case EKey_Type::Right:
        Platform.Move(false, key_down);
        break;

    case EKey_Type::Space:
        Platform.On_Space_Key(key_down);
        break;

    case EKey_Type::F:
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
    case EGame_State::Test_Ball:
        Ball_Set.Set_For_Test();
        Game_State = EGame_State::Play_Level;
        break;

    case EGame_State::Play_Level:
        Play_Level();
        break;

    case EGame_State::Lost_Ball:
        if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
            Restart_Level();
        
        break;

    case EGame_State::Restart_Level:
        if (Platform.Has_State(EPlatform_Substate_Regular::Ready))
        {
            Game_State = EGame_State::Play_Level;
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
void AsEngine::Restart_Level()
{
    Game_State = EGame_State::Restart_Level;
    Border.Open_Gate(7, true);
    Border.Open_Gate(5, false);
}


//
void AsEngine::Play_Level()
{
    Advance_Movers();

    if (Ball_Set.All_Balls_Are_Lost())
    {
        Game_State = EGame_State::Lost_Ball;
        Level.Stop();
        Platform.Set_State(EPlatform_State::Meltdown);
    }
    else
        Ball_Set.Accelerate();


    if (Ball_Set.Is_Test_Finished())
        Game_State = EGame_State::Test_Ball;
}


//
void AsEngine::Advance_Movers()
{
    double max_speed = 0.0;
    double curr_speed;

    // 1. Получаем максимальную скорость движущихся объектов
    for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
        if (Modules[i] != nullptr)
        {
            Modules[i]->Begin_Movement();

            curr_speed = fabs(Modules[i]->Get_Speed());

            if (curr_speed > max_speed)
                max_speed = curr_speed;
        }

    // 2. Смещаем все объекты
    Rest_Distance += max_speed;

    while (Rest_Distance > 0.0)
    {
        for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
        {
            if (Modules[i] != nullptr)
                Modules[i]->Advance(max_speed);
        }

        Rest_Distance -= AsConfig::Moving_STEP_SIZE;
    }

    // 3. Заканчиваем все движения на этом кадре
    for (int i = 0; i < AsConfig::Max_Movers_Count; i++)
        if (Modules[i] != nullptr)
            Modules[i]->Finish_Movement();
}


//
void AsEngine::Act()
{
    AFalling_Letter* falling_letter;
    int index = 0;

    // 1. Выполняем вск действия
    for (int i = 0; i < AsConfig::Max_Modules_Count; i++)
        if (Modules[i] != nullptr)
            Modules[i]->Act();

    // 2. Ловим падующие буквы
    while (Level.Get_Next_Falling_Leter(&falling_letter, index))
    {
        if (Platform.Hit_by(falling_letter))
            On_Falling_Letter(falling_letter);
    }

    // 3. Рестарт уровня (если надо)
    if(Game_State == EGame_State::Restart_Level)
        if(Border.Is_Gate_Opened(AsConfig::Gate_Count - 1))
            Platform.Set_State(EPlatform_State::Rolling);
}


//
void AsEngine::On_Falling_Letter(AFalling_Letter* falling_letter)
{
    switch (falling_letter->letter_type)
    {
    case ELetter_Type::O:
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;// !!! Пока отменился только клей

    case ELetter_Type::I:
        Ball_Set.Inverse_Balls();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::C:
        Ball_Set.Reset_Speed();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::M:
        break;

    case ELetter_Type::G:
        if (Life_Count < AsConfig::Max_Life_Count)
            Life_Count++; // !!! Отобразить на индикаторе!
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::K:
        Platform.Set_State(EPlatform_State::Glue);
        break;

    case ELetter_Type::W:
        Platform.Set_State(EPlatform_State::Expanding);
        break;

    case ELetter_Type::P:
        AsConfig::Level_Has_Floor = true;
        Border.Redraw_Floor();
        // !!! Отобразить на индикаторе
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::L:
        Platform.Set_State(EPlatform_State::Laser);
        break;

    case ELetter_Type::T:
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        Ball_Set.Tripple_Balls();
        break;

    case ELetter_Type::PLUS:
        break;

    default:
        AsConfig::Throw();
        break;
    }

    falling_letter->Finalize();
}


//
void AsEngine::Add_Next_Module(int& index, AGame_Object* game_obj)
{
    if (index >= 0 && index < AsConfig::Max_Modules_Count)
        Modules[index++] = game_obj;
    else
        AsConfig::Throw();
}






