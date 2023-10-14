#pragma once

#include "Explosive_Ball.h"

//
class AExplosion
{
protected:
    AExplosion();

    void Start_Explosion(RECT& rect);
    bool Act_On_Explosion();
    void Draw_Explosion(HDC hdc, RECT& paint_area);

private:
    static const int Explosive_Balls_Count = 20;
    std::vector<AExplosive_Ball> Explosive_Balls;
};