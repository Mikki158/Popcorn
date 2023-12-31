﻿#pragma once

#include "Config.h"

//
enum class EExplosive_Ball_State :unsigned char
{
    Idle,
    Charging,
    Expanding,
    Fading
};

//
class AExplosive_Ball : public AGraphics_Object
{
public:
    AExplosive_Ball();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Explode(int x_pos, int y_pos, int size, bool is_pink, int time_offset, int step_count);
    void Update_Ball_Rect();

private:
    bool Is_Pink;
    int X_Pos, Y_Pos;
    int Step_Count;
    int Start_Expanding_Tick, Start_Fading_Tick;
    int Time_Offset;
    double Size, Max_Size, Size_Step;

    static const int Fading_Timeout = AsConfig::FPS;

    EExplosive_Ball_State Explosive_Ball_State;
    RECT Ball_Rect;
};
