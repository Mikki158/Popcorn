#include "Engine.h"

AsEngine::AsEngine()
    :HWnd(), Active_Brick(EBT_Pink)
{
}

void AsEngine::Init_Engine(HWND hwnd)// Настройка игры при старте
{
    HWnd = hwnd;

    //AsConfig::Create_Pen_Brush(AsConfig::BG_Color, BG_Pen, BG_Brush);   

    AActive_Brick::setup_Colors();

    Ball.Init();
    Level.Init();
    Platform.Init();
    Border.Init();

    Platform.Redraw_Platform(HWnd);

    SetTimer(HWnd, TIMER_ID, 1000 / AsConfig::FPS, 0);
}

void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)// отрисовка экрана игры
{   
    Level.Draw(hdc, paint_area, HWnd); 
    
    Platform.Draw(hdc, paint_area);
    
    Ball.Draw(hdc, paint_area);

    Border.Draw(hdc);

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

        Platform.Redraw_Platform(HWnd);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= AsConfig::MAX_X_POS - Platform.Width + 1)
        {
            Platform.X_Pos = AsConfig::MAX_X_POS - Platform.Width + 1;
        }

        Platform.Redraw_Platform(HWnd);
        break;

    case EKT_Space:
        break;

    }
    return 0;
}

int AsEngine::On_Timer()
{
    Ball.Move(HWnd, &Level, Platform.X_Pos, Platform.Width);

    //Active_Brick.Act(HWnd);
    Level.Active_Brick.Act(HWnd);

    return 0;
}





