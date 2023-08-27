#pragma once

#include "Active_Brick.h"

enum ELetter_Type
{
    ELT_O, // "������"
    ELT_I, // "��������"
    ELT_C, // "��������"
    ELT_M, // "�������"
    ELT_G, // "�����"
    ELT_K, // "����"
    ELT_W, // "����"

    ELT_P, // "���"
    ELT_L, // "�����"
    ELT_T, // "���"

    ELT_PLUS, // "������� �� ��������� �������"

    ELT_MAX // ��������� ������ ����� ��������� � ������������
};

enum EFalling_Letter_State
{
    EFLS_Normal,
    EFLS_Finalizing, // �������� ������� ������
    EFLS_Finished // ����� ������ ����� �������
};

class AFalling_Letter : public AGraphics_Object
{
public:
    const ELetter_Type letter_type;

    AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y);


    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual void Act();
    virtual bool Is_Finished();

    void Get_Letter_Cell(RECT& rect);
    void Finalize();
    void Test_Draw_All_Steps(HDC hdc);

    static void Init();
    static ELetter_Type Get_Random_Letter_Type();


private:
    int X, Y;
    int Rotation_step;
    int Next_Rotation_Tick;
    EFalling_Letter_State Falling_Latter_State;
    RECT Letter_Cell, Prev_Letter_Cell;

    const EBrick_Type brick_type;

    static int All_Letters_Popularity;
    static int Letters_Popularity[ELT_MAX]; // "���" ������ �����

    static const int Ticks_Per_Step = 4;
    static const int Max_Rotation_Step = 16;
    static const int Brick_Half_Height = AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE / 2;
    

    void Draw_Brick_Letter(HDC hdc);
    void Draw_C(HDC hdc);
    void Set_Brick_Letter_Colors(bool is_switch_color, const AColor** front_color, const AColor** back_color);
    void Draw_Line_To(HDC hdc, int x, int y);
    void Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2);
};
