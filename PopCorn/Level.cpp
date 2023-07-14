#include "Level.h"

char ALevel::level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
    0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0,
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

bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
    double direction = ball->Get_Direction();
    double brick_left_x, brick_right_x;
    double brick_top_y, brick_low_y;
    double min_distance_to_horizontal, min_distance_to_vertical, another_min_distance;
    bool check_horizontal_first;

    for (int i = AsConfig::LEVEL_HEIGHT - 1; i >= 0; i--)
    {
        brick_top_y = AsConfig::LEVEL_Y_OFFSET + i * AsConfig::CELL_HEIGHT;
        brick_low_y = brick_top_y + AsConfig::BRICK_HEIGHT;
        for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
        {
            if (level_01[i][j] == 0)
            {
                continue;
            }

            brick_left_x = AsConfig::LEVEL_X_OFFSET + j * AsConfig::CELL_WIDTH;
            brick_right_x = brick_left_x + AsConfig::BRICK_WIDTH;

            min_distance_to_horizontal = fabs(next_x_pos - brick_left_x);
            another_min_distance = fabs(next_x_pos - brick_right_x);

            if (another_min_distance < min_distance_to_horizontal)
            {
                min_distance_to_horizontal = another_min_distance;
            }

            min_distance_to_vertical = fabs(next_y_pos - brick_top_y);
            another_min_distance = fabs(next_y_pos - brick_low_y);

            if (another_min_distance < min_distance_to_vertical)
            {
                min_distance_to_vertical = another_min_distance;
            }

            if (min_distance_to_horizontal <= min_distance_to_vertical)
            {
                check_horizontal_first = true;
            }
            else
            {
                check_horizontal_first = false;
            }

            if (check_horizontal_first)
            {
                if (Check_Horizontal_Hit(next_x_pos, next_y_pos, brick_left_x, brick_right_x, 
                    brick_top_y, brick_low_y, j, i, ball))
                {
                    return true;
                }

                if (Check_Vertical_Hit(next_x_pos, next_y_pos, brick_left_x, brick_right_x, 
                    brick_top_y, brick_low_y, j, i, ball))
                {
                    return true;
                }                
            }
            else
            {
                if (Check_Vertical_Hit(next_x_pos, next_y_pos, brick_left_x, brick_right_x, 
                    brick_top_y, brick_low_y, j, i, ball))
                {
                    return true;
                }

                if (Check_Horizontal_Hit(next_x_pos, next_y_pos, brick_left_x, brick_right_x, 
                    brick_top_y, brick_low_y, j, i, ball))
                {
                    return true;
                }
            }                     
        }
    }

    return false;
}

bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, double brick_left_x, double brick_right_x, 
    double brick_top_y, double brick_low_y, int level_x, int level_y, ABall* ball)
{
    double direction = ball->Get_Direction();

    // Проверяем попадание в вверхнюю грань
    if (direction >= M_PI && direction <= 2.0 * M_PI)
    {
        if (Hit_Circle_On_Line(next_y_pos - brick_top_y, next_x_pos, brick_left_x, brick_right_x, ball->RADIUS))
        {
            // Проверяем возможность отскока вверх
            if (level_y > 0 && level_01[level_y - 1][level_x] == 0)
            {
                ball->Reflect(true);
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    // Проверяем попадание в нижнюю грань
    if (direction >= 0.0 && direction < M_PI)
    {
        if (Hit_Circle_On_Line(next_y_pos - brick_low_y, next_x_pos, brick_left_x, brick_right_x, ball->RADIUS))
        {
            // Проверяем возможность отскока вниз
            if (level_y < AsConfig::LEVEL_HEIGHT - 1 && level_01[level_y + 1][level_x] == 0)
            {
                ball->Reflect(true);
                return true;
            }
            else
            {
                return false;
            }            
        }
    }

    return false;
}

bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, double brick_left_x, double brick_right_x,
    double brick_top_y, double brick_low_y, int level_x, int level_y, ABall* ball)
{
    double direction = ball->Get_Direction();

    // Проверяем попадание в левую грань
    if (direction >= 0.0 && direction < M_PI_2 || direction >= M_PI + M_PI_2 && direction <= 2.0 * M_PI)
    {
        if (Hit_Circle_On_Line(brick_left_x - next_x_pos, next_y_pos, brick_top_y, brick_low_y, ball->RADIUS))
        {
            // Проверяем возможность отскока влево
            if (level_x > 0 && level_01[level_y][level_x - 1] == 0)
            {
                ball->Reflect(false);
                return true;
            }
            else
            {
                return false;
            }
            
        }
    }

    // Проверяем попадание в правую грань
    if (direction > M_PI_2 && direction < M_PI + M_PI_2)
    {
        if (Hit_Circle_On_Line(brick_right_x - next_x_pos, next_y_pos, brick_top_y, brick_low_y, ball->RADIUS))
        {
            // Проверяем возможность отскока вправо
            if (level_x < AsConfig::LEVEL_WIDTH - 1 && level_01[level_y][level_x + 1] == 0)
            {
                ball->Reflect(false);
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

bool ALevel::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius)
{// Проверяет пересечение горизонтального отрезка (проходящего от left_x до right_x через y) с окружностью радиусом ridius
    double x;
    double min_x, max_x;

    if (y > radius)
    {
        return false;
    }

    x = sqrt(radius * radius - y * y);

    max_x = next_x_pos + x;
    min_x = next_x_pos - x;

    if ((max_x >= left_x && max_x <= right_x) || (min_x >= left_x && min_x <= right_x))
    {
        return true;
    }
    else
    {
        return false;
    }

}

void ALevel::Draw(HDC hdc, RECT& paint_area)// вывод всех кирпичей уровня
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


