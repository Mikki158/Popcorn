#include "Falling_Letter.h"


AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
    :brick_type(brick_type), letter_type(letter_type), X(x), Y(y), Rotation_step(2),
    Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step), Falling_Latter_State(EFLS_Normal)
{
    Letter_Cell.left = X;
    Letter_Cell.top = Y;
    Letter_Cell.right = Letter_Cell.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    Letter_Cell.bottom = Letter_Cell.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;

    Prev_Letter_Cell = Letter_Cell;
}

void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{// Вывод падающих букв
    double offSet;
    double rotation_angle;
    int brick_Half_height = AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE / 2;
    int back_part_offset;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    XFORM xForm, old_xForm;

    if (!(brick_type == EBT_Blue || brick_type == EBT_Pink))
    {
        return;
    }

    // корректируем шаг вращения и угол поворота
    Rotation_step = Rotation_step % 16;
    if (Rotation_step < 8)
    {
        rotation_angle = 2.0 * M_PI / 16.0 * (double)Rotation_step;
    }
    else
    {
        rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - Rotation_step);
    }

    if (Rotation_step > 4 && Rotation_step <= 12)
    {
        if (brick_type == EBT_Pink)
            switch_color = true;
        else
            switch_color = false;
    }
    else
    {
        if (brick_type == EBT_Blue)
            switch_color = true;
        else
            switch_color = false;
    }

    Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (Rotation_step == 4 || Rotation_step == 12)
    {
        // выводим фон
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);


        Rectangle(hdc, X, Y + brick_Half_height - AsConfig::GLOBAL_SCALE,
            X + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, Y + brick_Half_height);

        // выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, X, Y + brick_Half_height,
            X + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, Y + brick_Half_height + AsConfig::GLOBAL_SCALE);
    }
    else
    {

        xForm.eM11 = 1.0;
        xForm.eM12 = 0.0;
        xForm.eM21 = 0.0;
        xForm.eM22 = (float)-cos(rotation_angle);
        xForm.eDx = (float)X;
        xForm.eDy = (float)Y + (float)(brick_Half_height);

        GetWorldTransform(hdc, &old_xForm);
        SetWorldTransform(hdc, &xForm);


        offSet = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)AsConfig::GLOBAL_SCALE;
        back_part_offset = (int)round(offSet);

        // выводим фон
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, 0, -brick_Half_height - back_part_offset, AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, brick_Half_height - back_part_offset);

        // выводим передний план
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, 0, -brick_Half_height, AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, brick_Half_height);

        if (Rotation_step > 4 && Rotation_step <= 12)
        {
            if (letter_type == ELT_O)
            {
                SelectObject(hdc, AsConfig::Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsConfig::GLOBAL_SCALE, -5 * AsConfig::GLOBAL_SCALE / 2,
                    0 + 10 * AsConfig::GLOBAL_SCALE, 5 * AsConfig::GLOBAL_SCALE / 2);
            }
        }
        SetWorldTransform(hdc, &old_xForm);
    }
}

void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
    if (is_switch_color)
    {
        front_pen = AsConfig::Brick_Pink_Pen;
        front_brush = AsConfig::Brick_Pink_Brush;

        back_pen = AsConfig::Brick_Blue_Pen;
        back_brush = AsConfig::Brick_Blue_Brush;
    }
    else
    {
        front_pen = AsConfig::Brick_Blue_Pen;
        front_brush = AsConfig::Brick_Blue_Brush;

        back_pen = AsConfig::Brick_Pink_Pen;
        back_brush = AsConfig::Brick_Pink_Brush;
    }
}

void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
    {
        // очищаем фон
        SelectObject(hdc, AsConfig::BG_Pen);
        SelectObject(hdc, AsConfig::BG_Brush);

        Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
    }

    if (Falling_Latter_State == EFLS_Finalizing)
    {
        Falling_Latter_State = EFLS_Finished;
        return;
    }

    if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
        Draw_Brick_Letter(hdc);
}


//
void AFalling_Letter::Act()
{
    if (Falling_Latter_State != EFLS_Normal)
        return;

    if (Letter_Cell.top >= AsConfig::MAX_Y_POS * AsConfig::GLOBAL_SCALE)
    {
        Finalize();
        return;
    }

    Prev_Letter_Cell = Letter_Cell;

    Y += AsConfig::GLOBAL_SCALE;

    Letter_Cell.top += AsConfig::GLOBAL_SCALE;
    Letter_Cell.bottom += AsConfig::GLOBAL_SCALE;

    if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
    {
        Rotation_step++;

        Next_Rotation_Tick += Ticks_Per_Step;
    }

    InvalidateRect(AsConfig::HWnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::HWnd, &Letter_Cell, FALSE);
}

bool AFalling_Letter::Is_Finished()
{
    if (Falling_Latter_State == EFLS_Finished)
        return true;
    else
        return false;
}

void AFalling_Letter::Get_Letter_Cell(RECT& rect)
{
    rect = Letter_Cell;
}

void AFalling_Letter::Finalize()
{
    Falling_Latter_State = EFLS_Finalizing;
    InvalidateRect(AsConfig::HWnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::HWnd, &Letter_Cell, FALSE);
}


