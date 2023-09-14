#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Falling_Letter.h"
#include "Ball_Set.h"

enum class EPlatform_State: unsigned char
{
    Unknow,
    Regular,
    Meltdown,
    Rolling,
    Glue,
    Expanding, 
    Laser
};


enum class EPlatform_Substate_Regular: unsigned char
{
    Unknow,

    Missing,
    Ready,
    Normal
};


enum class EPlatform_Substate_Meltdown: unsigned char
{
    Unknow,

    Init,
    Active
};


enum class EPlatform_Substate_Rolling: unsigned char
{
    Unknow,

    Roll_In,
    Expand_Roll_In
};


enum class EPlatform_Transformation : unsigned char
{
    Unknow,

    Init,
    Active,
    Finalize
};

enum class EPlatform_Moving_State: unsigned char
{
    Stoping,
    Stop,
    Moving_Left,
    Moving_Right
};


enum class EFigure_Type : unsigned char
{
    Ellipse,
    Rectangle,
    Round_Rect_3x
};


class AsPlatform_State
{
public:
    AsPlatform_State();

    operator EPlatform_State() const;
    void operator = (EPlatform_State new_state);

    void Set_Next_State(EPlatform_State nex_state);
    

    EPlatform_State Get_Next_State();
    EPlatform_State Set_State(EPlatform_Substate_Regular new_regular_state);
    EPlatform_State Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state);

    EPlatform_Substate_Regular Regular;
    EPlatform_Substate_Meltdown Meltdown;
    EPlatform_Substate_Rolling Rolling;
    EPlatform_Transformation Glue;
    EPlatform_Transformation Expanding;
    EPlatform_Transformation Laser;

    EPlatform_Moving_State Moving;

private:
    EPlatform_State Current_State;
    EPlatform_State Next_State; // В это состояние переходим из AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
};


class AsPlatform_Glue
{
public:
    AsPlatform_Glue(AsPlatform_State &platform_state);

    bool Act(AsBall_Set* ball_set, EPlatform_State& next_state);

    void Draw_State(HDC hdc, double x_pos);
    void Reset();

private:
    double Glue_Spot_Height_Ratio;
    static const double Min_Glue_Spot_Height_Ratio, Max_Glue_Spot_Height_Ratio, Glue_Spot_Height_Ratio_Step;

    AsPlatform_State* Platform_State;

    void Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height, double x_pos);
};


class AsPlatform_Expanding
{
public:
    AsPlatform_Expanding(AsPlatform_State &platform_state);
    ~AsPlatform_Expanding();

    double Expanding_Platform_Width;

    void Init(AColor &inner_color, AColor &circle_color);
    void Draw_State(HDC hdc, double x);
    void Reset();

    bool Act(double& x_pos, EPlatform_State& next_state, bool& correct_pos);

private:
    static const double Min_Expanding_Platform_Width, Max_Expanding_Platform_Width, Expanding_Platform_Width_Step;

    AsPlatform_State *Platform_State;
    AColor *Truss_Color;
    AColor* Inner_Color; // Use, Not Own
    AColor* Circle_Color; // Use, Not Own

    void Draw_Expanding_Platform_Ball(HDC hdc, bool is_left, double x);
    void Draw_Expanding_Truss(HDC hdc, RECT& inner_rect, bool is_left);
};

enum class ELaser_Beam_State :unsigned char
{
    Disabled,
    Active,
    Stopping,
    Cleanup
};

class ALaser_Beam : public AMover, public AGraphics_Object
{
public:
    int Release_Timer_Tick;// Значение счетчика времени, после которого надо отпускать прикрепленный мячик 

    static const double RADIUS;

    ALaser_Beam();

    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Set_At(double x_pos, double y_pos);
    void Disable();
    bool Is_Active();
    static void Add_Hit_Checker(AHit_Checker* hit_checker);

private:
    static const int Width = 1;
    static const int Height = 3;
    static AHit_Checker* Hit_checkers[3];
    static int Hit_Checkers_Count;


    double X_Pos, Y_Pos;
    double Speed;
    RECT Prev_Beam_Rect, Beam_Rect;

    ELaser_Beam_State Laser_Beam_State;

    void Redraw_Beam();
};


class AsLaser_Beam_Set : public AMover, public AGraphics_Object
{
public:
    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Fire(double left_gun_x_pos, double right_gun_x_pos);

private:
    static const int Max_Laser_Beam_Count = 10;
    ALaser_Beam Laser_Beams[Max_Laser_Beam_Count];

};

class AsPlatform_Laser
{
public:
    AsPlatform_Laser(AsPlatform_State& platform_state);
    ~AsPlatform_Laser();

    void Init(AsLaser_Beam_Set* laser_beam_set, AColor& inner_color, AColor& circle_color);
    bool Act(EPlatform_State& next_state, double x_pos);
    void Draw_State(HDC hdc, double x_pos, RECT &platform_rect);
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

    AsLaser_Beam_Set *Laser_Beam_Set; // Use, Not Own



    void Draw_Laser_Inner_Part(HDC hdc, double x_pos);
    void Draw_Laser_Wing(HDC hdc, double x_pos, bool is_left);
    void Draw_Laser_Leg(HDC hdc, double x_pos, bool is_left);
    void Draw_Laser_Cabin(HDC hdc, double x_pos);


    void Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_tyoe, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height);
    int Get_Expanding_Value(double start, double end, double ratio);
    double Get_Gun_Pos(double platform_x_pos, bool is_left);
};


class AsPlatform: public AHit_Checker, public AMover, public AGraphics_Object
{
public:
    AsPlatform();
    ~AsPlatform();

    static const int NORMAL_WIDTH = 28;
    static const int CIRCLE_SIZE = 7;
    static const int NORMAL_PLATFORM_INNER_WIDTH = NORMAL_WIDTH - CIRCLE_SIZE;
    static const int Height = 7;
    static const int Expanding_Platform_Inner_Width = 12;


    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);
    virtual double Get_Speed();

    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual void Act();
    virtual bool Is_Finished();

    void Init(AsBall_Set* ball_set, AsLaser_Beam_Set* laser_beam_set);
    void Redraw_Platform();
    void Set_State(EPlatform_State new_state);
    void Set_State(EPlatform_Substate_Regular new_regular_state);
    void Move(bool to_left, bool key_down);
    void On_Space_Key(bool key_down);

    bool Hit_by(AFalling_Letter* falling_letter);
    bool Has_State(EPlatform_Substate_Regular regular_state);

    double Get_Middle_Pos();

    EPlatform_State Get_State();

private:
    int Rolling_Step;
    int Last_Redraw_Timer_Tick;
    int Inner_width;
    int Meltdown_Y_Pos;
    int Normal_Platform_Image_Width, Normal_Platform_Image_Height;
    int* Normal_Platform_Image;// Пиксели изображения платформы на фоне
    double X_Pos;
    double Speed;
    bool Left_Key_Down, Right_Key_Down;


    int Meltdown_Platform_Y_Pos[NORMAL_WIDTH * AsConfig::GLOBAL_SCALE];

    
    AsPlatform_State Platform_State;
    AsPlatform_Glue Platform_Glue;
    AsPlatform_Expanding Platform_Expanding;
    AsPlatform_Laser Platform_Laser;

    RECT Platform_Rect, Prev_Platform_Rect;

    AColor Platform_Circle_Color, Platform_Inner_Color, Gun_Color;

    AsBall_Set* Ball_Set;

    static const int MAX_ROLLING_STEP = 16;
    static const int ROLL_IN_PLATFORM_END_X_POS = 99;
    static const int ROLLING_PLATFORM_SPEED = 2;
    static const int X_Step = 6;

    bool Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation& transformatio_state);
    void Draw_Normal_State(HDC hdc, RECT& paint_area);
    void Get_Normal_Platform_Image(HDC hdc);
    void Draw_Meltdown_State(HDC hdc, RECT& paint_area);  
    void Draw_Rolling_State(HDC hdc, RECT& paint_area);
    void Draw_Roll_In_State(HDC hdc, RECT& paint_area);
    void Act_For_Meltdown_State();
    void Act_For_Rolling_State();

    bool Correct_Platform_Pos();
    bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball);
    bool Get_Platform_Image_Stroke_Color(int x, int y, const AColor** color, int& stroke_len);

    double Get_Current_Width();
};