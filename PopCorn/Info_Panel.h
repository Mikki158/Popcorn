#pragma once

#include "Config.h"
#include "Falling_Letter.h"

class AsInfo_Panel : public AGame_Object
{
public:
    AsInfo_Panel();
    ~AsInfo_Panel();

    virtual void Advance(double max_speed);
    virtual void Begin_Movement();
    virtual void Finish_Movement();
    virtual double Get_Speed();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Init();

private:
    HFONT Logo_Pop_Font, Logo_Corn_Font, Name_Font, Score_Font;
    AColor* Shadow_Color, * Highlight_Color, * Dark_Blue, * Dark_Pink;

    AFalling_Letter Letter_P, Letter_G, Letter_M;

    AString Player_Name;

    static const int Score_X = 208;
    static const int Score_Y = 108;
    static const int Score_Width = 110;
    static const int Score_Height = 90;
    static const int Score_Value_Offset = 20;


    void Choose_Font();
    void Draw_String(HDC hdc, RECT rect, AString &str, bool draw_name);
    void Draw_Extra_Life(HDC hdc, int x_pos, int y_pos);
    void Show_Extra_Lives(HDC hdc);

};