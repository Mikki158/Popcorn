#pragma once

#include "Platform_State.h"
#include "Ball_Set.h"

//
class AsPlatform_Glue
{
public:
    AsPlatform_Glue(AsPlatform_State& platform_state);

    bool Act(AsBall_Set* ball_set, EPlatform_State& next_state);

    void Draw_State(HDC hdc, double x_pos);
    void Reset();

private:
    double Glue_Spot_Height_Ratio;
    static const double Min_Glue_Spot_Height_Ratio, Max_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;

    AsPlatform_State* Platform_State;

    void Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height, double x_pos);
};