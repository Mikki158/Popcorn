#pragma once

#include "Config.h"

//
enum class ELaser_Beam_State :unsigned char
{
    Disabled,
    Active,
    Stopping,
    Cleanup
};

//
class ALaser_Beam : public AGame_Object
{
public:
    int Release_Timer_Tick;// «начение счетчика времени, после которого надо отпускать прикрепленный м€чик 

    static AHit_Checker_List Hit_Checker_List;

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

private:
    static const int Width = 1;
    static const int Height = 3;


    double X_Pos, Y_Pos;
    double Speed;
    RECT Prev_Beam_Rect, Beam_Rect;

    ELaser_Beam_State Laser_Beam_State;

    void Redraw_Beam();
};