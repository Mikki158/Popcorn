#include "Engine.h"

AsEngine::AsEngine()
    :Game_State(EGS_Play_Level)
{

}

void AsEngine::Init_Engine(HWND hwnd)// Настройка игры при старте
{
    AsConfig::HWnd = hwnd;

    //AsConfig::Create_Pen_Brush(AsConfig::BG_Color, BG_Pen, BG_Brush);   

    AActive_Brick::Setup_Color();

    
    Level.Init();
    Platform.Init();
    Ball.Init();
    Border.Init();
     
    ABall::Add_Hit_Checker(&Border);
    ABall::Add_Hit_Checker(&Level);
    ABall::Add_Hit_Checker(&Platform);

    Level.Set_Current_Level(ALevel::Level_01);

    Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);

    Platform.Set_State(EPS_Normal);

    Platform.Redraw_Platform();

    SetTimer(AsConfig::HWnd, TIMER_ID, 1000 / AsConfig::FPS, 0);
}

void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)// отрисовка экрана игры
{   
    Level.Draw(hdc, paint_area); 

    Border.Draw(hdc);

    Platform.Draw(hdc, paint_area);

    Ball.Draw(hdc, paint_area);
    
    //Active_Brick.Draw(hdc, paint_area);

    //Draw_Brick_Letter(hdc, 5);

    /*for (int i = 0; i < 16; i++)
    {
        Draw_Brick_Letter(hdc, 20 + i * CELL_WIDTH * GLOBAL_SCALE, 100, EBT_Blue, ELT_O, i);
        Draw_Brick_Letter(hdc, 20 + i * CELL_WIDTH * GLOBAL_SCALE, 130, EBT_Pink, ELT_O, i);
    }*/
}

int AsEngine::On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.X_Pos -= Platform.X_Step;

        if (Platform.X_Pos <= AsConfig::BORDER_X_OFFSET)
        {
            Platform.X_Pos = AsConfig::BORDER_X_OFFSET;
        }

        Platform.Redraw_Platform();
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= AsConfig::MAX_X_POS - Platform.Width + 1)
        {
            Platform.X_Pos = AsConfig::MAX_X_POS - Platform.Width + 1;
        }

        Platform.Redraw_Platform();
        break;

    case EKT_Space:
        if (Platform.Get_State() == EPS_Ready)
        {
            Platform.Set_State(EPS_Normal);
            Ball.Set_State(EBS_Normal, Platform.X_Pos + Platform.Width / 2);
        }
        break;

    }
    return 0;
}

int AsEngine::On_Timer()
{
    AsConfig::Current_Timer_Tick += 1;

    switch (Game_State)
    {
    case EGS_Test_Ball:
        Ball.Set_For_Test();
        Game_State = EGS_Play_Level;
        break;

    case EGS_Play_Level:
        Ball.Move();

        if (Ball.Get_State() == EBS_Lost)
        {
            Game_State = EGS_Lost_Ball;
            Platform.Set_State(EPS_Meltdown);
        }

        if (Ball.Is_Test_Finished())
        {
            Game_State = EGS_Test_Ball;
        }
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
            Game_State = EGS_Play_Level;
            Ball.Set_State(EBS_On_Platform, Platform.X_Pos + Platform.Width / 2);
        }
        break;

    default:
        break;
    }


    //Level.Active_Brick.Act();
    
    //if (AsConfig::Current_Timer_Tick % 10 == 0)
    {
        Platform.Act();
    }

    

    return 0;
}





