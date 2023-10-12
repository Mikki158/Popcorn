#include "Engine.h"


// AsEngine
//
AsEngine::AsEngine()
    :TIMER_ID(WM_USER + 1), Game_State(EGame_State::Mop_Level), Rest_Distance(0.0), Modules{}
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
    
    Level.Init();
    Platform.Init(&Ball_Set, &Laser_Beam_Set);
    Monster_Set.Init(&Border);
    Info_Panel.Init();

    AFalling_Letter::Init();
     
    ABall::Hit_Checker_List.Add_Hit_Checker(&Border);
    ABall::Hit_Checker_List.Add_Hit_Checker(&Level);
    ABall::Hit_Checker_List.Add_Hit_Checker(&Platform);
    ABall::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

    ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Level);
    ALaser_Beam::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);

    AsPlatform::Hit_Checker_List.Add_Hit_Checker(&Monster_Set);


    //Level.Set_Current_Level(1);

    Platform.Redraw_Platform();

    SetTimer(AsConfig::HWnd, TIMER_ID, 1000 / AsConfig::FPS, 0);

    // Modules
    Modules.push_back(&Level);
    Modules.push_back(&Border);
    Modules.push_back(&Platform);
    Modules.push_back(&Ball_Set);
    Modules.push_back(&Laser_Beam_Set);
    Modules.push_back(&Monster_Set);
    Modules.push_back(&Info_Panel);

    Level.Mop_Level(1);
}


//
void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)// отрисовка экрана игры
{  
    SetGraphicsMode(hdc, GM_ADVANCED);

    for (auto* curr_module : Modules)
        curr_module->Clear(hdc, paint_area);

    for (auto* curr_module : Modules)
        curr_module->Draw(hdc, paint_area);
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

    case EGame_State::Mop_Level:
        if (Level.Is_Level_Mopping_Done())
            Restart_Level();
        break;

    case EGame_State::Play_Level:
        Play_Level();
        break;

    case EGame_State::Lost_Ball:
        if (Platform.Has_State(EPlatform_Substate_Regular::Missing))
            if (!Restart_Level())
                Game_Over();
        break;

    case EGame_State::Restart_Level:
        if (Platform.Has_State(EPlatform_Substate_Regular::Ready))
        {
            Game_State = EGame_State::Play_Level;
            Ball_Set.Set_On_Platform(Platform.Get_Middle_Pos());
            Monster_Set.Activate(10);
        }
        break;

    default:
        break;
    }

    Act();

    return 0;
}


//
bool AsEngine::Restart_Level()
{
    if (!Info_Panel.Decrease_Life_Count())
        return false;
    Game_State = EGame_State::Restart_Level;
    Border.Open_Gate(7, true);

    return true;
}


//
void AsEngine::Game_Over()
{
    AsConfig::Throw();// !!! Надо сделать!
}


//
void AsEngine::Play_Level()
{
    Advance_Movers();

    if (Ball_Set.All_Balls_Are_Lost())
    {// Потеряли все мячики 

        Game_State = EGame_State::Lost_Ball;
        Level.Stop();
        Monster_Set.Destroy_All();
        Laser_Beam_Set.Disable_All();
        Platform.Set_State(EPlatform_State::Meltdown);
        Info_Panel.Floor_Indicator.Reset();
        Info_Panel.Monster_Indicator.Reset();
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
    for (auto* curr_module : Modules)
    {
        curr_module->Begin_Movement();

        curr_speed = fabs(curr_module->Get_Speed());

        if (curr_speed > max_speed)
            max_speed = curr_speed;
    }

    // 2. Смещаем все объекты
    Rest_Distance += max_speed;

    while (Rest_Distance > 0.0)
    {
        for (auto* curr_module : Modules)
            curr_module->Advance(max_speed);

        Rest_Distance -= AsConfig::Moving_STEP_SIZE;
    }

    // 3. Заканчиваем все движения на этом кадре
    for (auto* curr_module : Modules)
        curr_module->Finish_Movement();
}


//
void AsEngine::Act()
{
    AFalling_Letter* falling_letter;
    int index = 0;

    // 1. Выполняем вск действия
    for (auto *curr_module : Modules)
        curr_module->Act();

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

    Handle_Message();
}


//
void AsEngine::Handle_Message()
{
    AMessage* message;

    if (AsMessage_Menager::Get_Message(&message))
    {
        switch (message->Message_Type)
        {
        case EMessage_Type::Floor_Is_Over:
            AsConfig::Level_Has_Floor = false;
            Border.Redraw_Floor();
            delete message;
            break;

        case EMessage_Type::Unfreeze_Monsters:
            Monster_Set.Set_Freeze_State(false);
            break;

        default:
            AsConfig::Throw();
            break;
        }
    }

}


//
void AsEngine::On_Falling_Letter(AFalling_Letter* falling_letter)
{
    switch (falling_letter->letter_type)
    {
    case ELetter_Type::O:
        Info_Panel.Floor_Indicator.Cancel();
        Info_Panel.Monster_Indicator.Cancel();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::I:
        Ball_Set.Inverse_Balls();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::C:
        Ball_Set.Reset_Speed();
        Platform.Set_State(EPlatform_Substate_Regular::Normal);
        break;

    case ELetter_Type::M:
        Monster_Set.Set_Freeze_State(true);
        Info_Panel.Monster_Indicator.Restart();
        break;

    case ELetter_Type::G:
        Info_Panel.Increase_Life_Count();
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
        Info_Panel.Floor_Indicator.Restart();
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

    AsInfo_Panel::Update_Score(EScore_Event_Type::Catch_Letter);
}






