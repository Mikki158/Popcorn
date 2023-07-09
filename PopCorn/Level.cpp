#include "Level.h"

char ALevel::level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


ALevel::ALevel()
    :Brick_Pink_Pen(), Brick_Blue_Pen(), Letter_Pen(), Brick_Pink_Brush(), Brick_Blue_Brush(), Level_Rect(), 
    Active_Brick(EBT_Pink)
{

}

void ALevel::Init()
{
    Letter_Pen = CreatePen(PS_SOLID, AsConfig::GLOBAL_SCALE, RGB(255, 255, 255));

    AsConfig::Create_Pen_Brush(AsConfig::Pink_Brick_Color, Brick_Pink_Pen, Brick_Pink_Brush);
    AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, Brick_Blue_Pen, Brick_Blue_Brush);

    Level_Rect.left = AsConfig::LEVEL_X_OFFSET * AsConfig::GLOBAL_SCALE;
    Level_Rect.top = AsConfig::LEVEL_Y_OFFSET * AsConfig::GLOBAL_SCALE;
    Level_Rect.right = AsConfig::LEVEL_X_OFFSET + AsConfig::CELL_WIDTH * AsConfig::LEVEL_WIDTH * AsConfig::GLOBAL_SCALE;
    Level_Rect.bottom = AsConfig::LEVEL_Y_OFFSET + AsConfig::CELL_HEIGHT * AsConfig::LEVEL_HEIGHT * AsConfig::GLOBAL_SCALE;

}

void ALevel::Check_Level_Brick_Heat(int& next_y_pos, double& ball_direction)
{
    int brick_y_pos = AsConfig::LEVEL_Y_OFFSET + AsConfig::LEVEL_HEIGHT * AsConfig::CELL_HEIGHT;
    for (int i = AsConfig::LEVEL_HEIGHT - 1; i >= 0; i--)
    {
        for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
        {
            if (level_01[i][j] == 0)
            {
                continue;
            }

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_direction = -ball_direction;
            }
        }
        brick_y_pos -= AsConfig::CELL_HEIGHT;
    }
}

void ALevel::Draw(HDC hdc, RECT& paint_area, HWND hwnd)// вывод всех кирпичей уровня
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
    {
        return;
    }
    for (int i = 0; i < AsConfig::LEVEL_HEIGHT; i++)
    {
        for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
        {
            Draw_Brick(hdc, AsConfig::LEVEL_X_OFFSET + (AsConfig::CELL_WIDTH * j),
                AsConfig::LEVEL_Y_OFFSET + (AsConfig::CELL_HEIGHT * i), (EBrick_Type)level_01[i][j]);
        }
    }

    Active_Brick.Draw(hdc, paint_area);

    
}

void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Pink:
        pen = Brick_Pink_Pen;
        brush = Brick_Pink_Brush;
        break;

    case EBT_Blue:
        pen = Brick_Blue_Pen;
        brush = Brick_Blue_Brush;
        break;

    default:
        return;
    }


    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    RoundRect(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE, (x + AsConfig::BRICK_WIDTH) * AsConfig::GLOBAL_SCALE,
        (y + AsConfig::BRICK_HEIGHT) * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);

}

void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
    if (is_switch_color)
    {
        front_pen = Brick_Pink_Pen;
        front_brush = Brick_Pink_Brush;

        back_pen = Brick_Blue_Pen;
        back_brush = Brick_Blue_Brush;
    }
    else
    {
        front_pen = Brick_Blue_Pen;
        front_brush = Brick_Blue_Brush;

        back_pen = Brick_Pink_Pen;
        back_brush = Brick_Pink_Brush;
    }
}

void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
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
    rotation_step = rotation_step % 16;
    if (rotation_step < 8)
    {
        rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
    }
    else
    {
        rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);
    }

    if (rotation_step > 4 && rotation_step <= 12)
    {
        switch_color = brick_type == EBT_Blue;
    }
    else
    {
        switch_color = brick_type == EBT_Pink;
    }

    Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (rotation_step == 4 || rotation_step == 12)
    {
        // выводим фон
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);


        Rectangle(hdc, x, y + brick_Half_height - AsConfig::GLOBAL_SCALE, x + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, y + brick_Half_height);

        // выводим передний план
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y + brick_Half_height, x + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, y + brick_Half_height + AsConfig::GLOBAL_SCALE);
    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED);

        xForm.eM11 = 1.0;
        xForm.eM12 = 0.0;
        xForm.eM21 = 0.0;
        xForm.eM22 = (float)-cos(rotation_angle);
        xForm.eDx = (float)x;
        xForm.eDy = (float)y + (float)(brick_Half_height);

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

        if (rotation_step > 4 && rotation_step <= 12)
        {
            if (letter_type == ELT_O)
            {
                SelectObject(hdc, Letter_Pen);
                Ellipse(hdc, 0 + 5 * AsConfig::GLOBAL_SCALE, -5 * AsConfig::GLOBAL_SCALE / 2,
                    0 + 10 * AsConfig::GLOBAL_SCALE, 5 * AsConfig::GLOBAL_SCALE / 2);
            }
        }
        SetWorldTransform(hdc, &old_xForm);
    }
}


