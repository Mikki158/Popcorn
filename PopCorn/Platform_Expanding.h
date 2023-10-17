#pragma once

#include "Platform_State.h"

//
class AsPlatform_Expanding
{
public:
    AsPlatform_Expanding(AsPlatform_State& platform_state);
    ~AsPlatform_Expanding();

    double Expanding_Platform_Width;

    void Init(AColor& inner_color, AColor& circle_color);
    void Draw_State(HDC hdc, double x);
    void Reset();

    bool Act(double& x_pos, EPlatform_State& next_state, bool& correct_pos);

private:
    static const double Min_Expanding_Platform_Width, Max_Expanding_Platform_Width, Expanding_Platform_Width_Step;

    AsPlatform_State* Platform_State;
    AColor* Truss_Color;
    AColor* Inner_Color; // Use, Not Own
    AColor* Circle_Color; // Use, Not Own

    void Draw_Expanding_Platform_Ball(HDC hdc, bool is_left, double x);
    void Draw_Expanding_Truss(HDC hdc, RECT& inner_rect, bool is_left);
};