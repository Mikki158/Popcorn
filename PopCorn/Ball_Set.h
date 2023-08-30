#pragma once
#include "Ball.h"

class AsBall_Set : public AMover
{
public:
    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    void Draw(HDC hdc, RECT& paint_area);
    void Release_From_Platform(double platform_x_pos);
    void Set_On_Platform(double platform_x_pos);
    void Set_For_Test();
    void Tripple_Balls();
    void Inverse_Balls();
    void Accelerate();
    void Reset_Speed();
    bool Is_Test_Finished();
    bool All_Balls_Are_Lost();

private:
    ABall Balls[AsConfig::Max_Balls_Count];
};