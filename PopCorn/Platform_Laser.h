#pragma once

#include "Platform_State.h"
#include "Laser_Beam_Set.h"

enum class EFigure_Type : unsigned char
{
    Ellipse,
    Rectangle,
    Round_Rect_3x
};

class AsPlatform_Laser
{
public:
    AsPlatform_Laser(AsPlatform_State& platform_state);
    ~AsPlatform_Laser();

    void Init(AsLaser_Beam_Set* laser_beam_set, AColor& inner_color, AColor& circle_color);
    bool Act(EPlatform_State& next_state, double x_pos);
    void Draw_State(HDC hdc, double x_pos, RECT& platform_rect);
    void Reset();
    void Fire(bool fire_on);

private:
    bool Enable_Laser_Firing;
    int Laser_Transformatio_Step;
    int Last_Laser_Shot_Tick;

    static const int Max_Laser_Transformatio_Step = 20;
    static const int Laser_Shot_Timeout = AsConfig::FPS / 2; // 2 раза в секунду

    AsPlatform_State* Platform_State;
    AColor* Inner_Color; // Use, Not Own
    AColor* Circle_Color; // Use, Not Own
    AColor* Gun_Color;

    AsLaser_Beam_Set* Laser_Beam_Set; // Use, Not Own



    void Draw_Laser_Inner_Part(HDC hdc, double x_pos);
    void Draw_Laser_Wing(HDC hdc, double x_pos, bool is_left);
    void Draw_Laser_Leg(HDC hdc, double x_pos, bool is_left);
    void Draw_Laser_Cabin(HDC hdc, double x_pos);


    void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_tyoe, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
    int Get_Expanding_Value(double start, double end, double ratio);
    double Get_Gun_Pos(double platform_x_pos, bool is_left);
};