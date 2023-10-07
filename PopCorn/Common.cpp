#include "Common.h"


// AGraphics_Object
//
AGraphics_Object::~AGraphics_Object()
{

}



// AMover
//
AMover::~AMover()
{

}



// AGame_Objects_Set
//
void AGame_Objects_Set::Advance(double max_speed)
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Advance(max_speed);
}


//
void AGame_Objects_Set::Begin_Movement()
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Begin_Movement();
}


//
void AGame_Objects_Set::Finish_Movement()
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Finish_Movement();
}


//
double AGame_Objects_Set::Get_Speed()
{
    double max_speed = 0.0;
    double curr_speed;
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
    {
        curr_speed = object->Get_Speed();
        if (curr_speed > max_speed)
            max_speed = curr_speed;
    }

    return max_speed;
}


//
void AGame_Objects_Set::Act()
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Act();

}


//
void AGame_Objects_Set::Clear(HDC hdc, RECT& paint_area)
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Clear(hdc, paint_area);
}


//
void AGame_Objects_Set::Draw(HDC hdc, RECT& paint_area)
{
    int index = 0;
    AGame_Object* object;

    while (Get_Next_Game_Object(index, &object))
        object->Draw(hdc, paint_area);
}


//
bool AGame_Objects_Set::Is_Finished()
{
    return false; // «аглушка, т.к. этот метод не используетс€
}