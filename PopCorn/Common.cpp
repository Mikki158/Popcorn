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



// AString
//
AString::AString()
{

}

//
AString::AString(const wchar_t* str)
    :Content(str)
{

}


//
void AString::Append(int value, int digits)
{
    wchar_t buf[32];
    wchar_t format[32];

    swprintf(format, 32, L"%%.%ii", digits);
    swprintf(buf, 32, format, value);

    Content += buf;
}


//
void AString::Append(wchar_t symbol)
{
    Content.append(1, symbol);
}


//
void AString::Delete_Last_Symbol()
{
    int len = (int)Content.size();

    if (len > 0)
        Content.resize(len - 1);
}


//
const wchar_t* AString::Get_Content()
{
    return Content.c_str();
}


//
int AString::Get_Lenght()
{
    return Content.length();
}


//
void AString::Clear()
{
    Content = L"";
}



// AsMessage_Menager
std::queue<AMessage*> AsMessage_Menager::Messages_Queue;
//
void AsMessage_Menager::Add_Message(AMessage* message)
{
    Messages_Queue.push(message);
}


//
bool AsMessage_Menager::Get_Message(AMessage** message)
{
    if (Messages_Queue.size() == 0)
        return false;

    *message = Messages_Queue.front();
    Messages_Queue.pop();

    return true;
}