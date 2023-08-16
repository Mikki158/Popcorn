#pragma once

#include "Active_Brick.h"

enum ELetter_Type
{
    ELT_None,

    ELT_O
};

enum EFalling_Letter_State
{
    EFLS_Normal,
    EFLS_Finalizing, // Начинаем удалять объект
    EFLS_Finished // Когда объект можно удалять
};

class AFalling_Letter : public AGraphics_Object
{
public:

    AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);

    const ELetter_Type letter_type;
    const EBrick_Type brick_type;

    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual void Act();
    virtual bool Is_Finished();

    void Get_Letter_Cell(RECT& rect);
    void Finalize();

private:
    int X, Y;
    int Rotation_step;
    int Next_Rotation_Tick;
    EFalling_Letter_State Falling_Latter_State;

    static const int Ticks_Per_Step = 4;

    RECT Letter_Cell, Prev_Letter_Cell;


    void Draw_Brick_Letter(HDC hdc);
    void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
};
