#pragma once

#include "Falling_Letter.h"
#include "Indicator.h"
#include "Label.h"

//
enum class EScore_Event_Type : unsigned char
{
    Hit_Brick,
    Hit_Monster,
    Catch_Letter
};

//
class AsInfo_Panel : public AGame_Object
{
public:
    AsInfo_Panel();

    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Increase_Life_Count();
    bool Decrease_Life_Count();

    static void Update_Score(EScore_Event_Type event_type);

    AIndicator Floor_Indicator, Monster_Indicator;

private:
    int Extra_Lives_Count;
    //HFONT Logo_Pop_Font, Logo_Corn_Font;
    AColor Dark_Blue, Dark_Pink;

    AFalling_Letter Letter_P, Letter_G, Letter_M;

    //AString Player_Name;
    ALabel Player_Name_Label, Score_Label;

    static RECT Logo_Rect; // Область логотипа
    static RECT Data_Rect; // Область данных (имени игрока, счета и индикаторов)

    static int Score;

    static const int Logo_X_Pos = 212;
    static const int Logo_Y_Pos = 0;
    static const int Shadow_X_Offset = 5;
    static const int Shadow_Y_Offset = 5;
    static const int Score_X = 208;
    static const int Score_Y = 108;
    static const int Score_Width = 110;
    static const int Score_Height = 90;
    static const int Score_Value_Offset = 20;
    static const int Indicator_Y_Offset = 55;


    void Choose_Font();
    //void Draw_String(HDC hdc, RECT rect, AString &str, bool draw_name);
    void Draw_Extra_Life(HDC hdc, int x_pos, int y_pos);
    void Show_Extra_Lives(HDC hdc);

};