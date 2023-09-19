#include "Laser_Beam_Set.h"


//AsLaser_Beam_Set
//
void AsLaser_Beam_Set::Fire(double left_gun_x_pos, double right_gun_x_pos)
{
    ALaser_Beam* left_beam = nullptr, * right_beam = nullptr;

    for (int i = 0; i < Max_Laser_Beam_Count; i++)
    {
        if (Laser_Beams[i].Is_Active())
            continue;

        if (left_beam == nullptr)
            left_beam = &Laser_Beams[i];
        else if (right_beam == nullptr)
        {
            right_beam = &Laser_Beams[i];
            break;
        }

    }

    if (left_beam == nullptr || right_beam == nullptr)
        AsConfig::Throw(); // Ќе хватило "свободных" лазерных лучей

    left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_POS - 1);
    right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_POS - 1);
}


//
bool AsLaser_Beam_Set::Get_Next_Game_Object(int& index, AGame_Object** game_obj)
{
    if (index < 0 || index >= Max_Laser_Beam_Count)
        return false;

    *game_obj = &Laser_Beams[index++];

    return true;
}