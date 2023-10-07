#pragma once
#include "Ball.h"

class AsBall_Set : public AGame_Objects_Set
{
public:
    AsBall_Set();

    virtual void Act();

    void Release_From_Platform(double platform_x_pos);
    void Set_On_Platform(double platform_x_pos);
    void Set_For_Test();
    void Tripple_Balls();
    void Inverse_Balls();
    void Accelerate();
    void Reset_Speed();
    void On_Platform_Advance(double direction, double platform_speed, double max_speed);
    bool Is_Test_Finished();
    bool All_Balls_Are_Lost();
    bool Release_Next_Ball();

private:
    std::vector<ABall> Balls;

    bool Get_Next_Game_Object(int& index, AGame_Object** game_obj);
    void Turn_Tripled_Ball(ABall* ball, bool add_min_angle);
};