#include "Laser_Beam_Set.h"


//AsLaser_Beam_Set
// 
void AsLaser_Beam_Set::Advance(double max_speed)
{
    for (int i = 0; i < Max_Laser_Beam_Count; i++)
        Laser_Beams[i].Advance(max_speed);
}


//
void AsLaser_Beam_Set::Begin_Movement()
{
    for (int i = 0; i < Max_Laser_Beam_Count; i++)
        Laser_Beams[i].Begin_Movement();
}


//
void AsLaser_Beam_Set::Finish_Movement()
{
    for (int i = 0; i < Max_Laser_Beam_Count; i++)
        Laser_Beams[i].Finish_Movement();
}


//
double AsLaser_Beam_Set::Get_Speed()
{
    double max_speed = 0.0;
    double curr_speed;

    for (int i = 0; i < Max_Laser_Beam_Count; i++)
    {
        curr_speed = Laser_Beams[i].Get_Speed();
        if (curr_speed > max_speed)
            max_speed = curr_speed;
    }

    return max_speed; // !!! Надо сделать
}


//
void AsLaser_Beam_Set::Act()
{
    // Заглушка, Не используется, т.к. лазеры сам ничего не делают (не анимируется)
}


//
void AsLaser_Beam_Set::Clear(HDC hdc, RECT& paint_area)
{
    for (int i = 0; i < Max_Laser_Beam_Count; i++)
        Laser_Beams[i].Clear(hdc, paint_area);
}


//
void AsLaser_Beam_Set::Draw(HDC hdc, RECT& paint_area)
{
    for (int i = 0; i < Max_Laser_Beam_Count; i++)
        Laser_Beams[i].Draw(hdc, paint_area);
}


//
bool AsLaser_Beam_Set::Is_Finished()
{
    return false; // Заглушка, т.к. этот метод не используется
}


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
        AsConfig::Throw(); // Не хватило "свободных" лазерных лучей

    left_beam->Set_At(left_gun_x_pos, AsConfig::Platform_Y_POS - 1);
    right_beam->Set_At(right_gun_x_pos, AsConfig::Platform_Y_POS - 1);
}