#pragma once

#define _USE_MATH_DEFINES
#include <Windows.h>

#include "Config.h"
#include "Border.h"
#include "Level.h"
#include "Platform.h"
#include "Active_Brick.h"
#include "Ball_Set.h"

// таймер
const int TIMER_ID = WM_USER + 1;

enum EKey_Type
{
    EKT_None,

    EKT_Left,
    EKT_Right,
    EKT_Space,

    EKT_F
};

enum EGame_State
{
    EGS_Test_Ball,

    EGS_Play_Level,
    EGS_Lost_Ball,
    EGS_Restart_Level
};


class AsEngine
{
public:
    AsEngine();

    void Init_Engine(HWND hWnd);
    void Draw_Frame(HDC hdc, RECT& paint_area);

    int On_Key(EKey_Type key_type, bool key_down);
    int On_Timer();  
    void Restart_Level();


private:
    int Life_Count;
    double Rest_Distance;

    EGame_State Game_State;

    AsLevel Level;
    AsPlatform Platform;
    AsBorder Border;
    AsBall_Set Ball_Set;
    AsLaser_Beam_Set Laser_Beam_Set;

    AMover* Movers[AsConfig::Max_Movers_Count]; // Движущиеся в данный момент объекты
    AGraphics_Object* Modules[AsConfig::Max_Modules_Count]; // Главные графические объекты (модули) игры

    void Play_Level();
    void Advance_Movers();
    void Act();
    void On_Falling_Letter(AFalling_Letter* falling_letter);
};





