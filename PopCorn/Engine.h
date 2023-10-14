﻿#pragma once

#include "Border.h"
#include "Level.h"
#include "Platform.h"
#include "Active_Brick.h"
#include "Ball_Set.h"
#include "Monster_Set.h"
#include "Info_Panel.h"

//
enum class EKey_Type: unsigned char
{
    None,

    Left,
    Right,
    Space,

    F
};

//
enum class EGame_State: unsigned char
{
    Test_Ball,

    Mop_Level,
    Play_Level,
    Lost_Ball,
    Restart_Level,
    Finish_Level,
    Game_Over
};

//
class AsEngine
{
public:
    AsEngine();

    void Init_Engine(HWND hWnd);
    void Draw_Frame(HDC hdc, RECT& paint_area);

    const int TIMER_ID;

    int On_Key(EKey_Type key_type, bool key_down);
    int On_Timer();
    void Restart_Level();
    void Game_Over();


private:
    double Rest_Distance;

    EGame_State Game_State;

    AsLevel Level;
    AsPlatform Platform;
    AsBorder Border;
    AsBall_Set Ball_Set;
    AsLaser_Beam_Set Laser_Beam_Set;
    AsMonster_Set Monster_Set;
    AsInfo_Panel Info_Panel;

    std::vector<AGame_Object*> Modules;// Главные графические объекты (модули) игры

    bool Is_Destroying_Complete();
    void Play_Level();
    void Stop_Play();
    void Advance_Movers();
    void Act();
    void Handle_Message();
    void On_Falling_Letter(AFalling_Letter* falling_letter);

    void Game_Won();
};





