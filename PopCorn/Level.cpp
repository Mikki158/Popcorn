#include "Level.h"

char ALevel::Level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH] =
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

char ALevel::Test_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


ALevel::ALevel()
    :Level_Rect(), 
    Active_Bricks_Count(0), Falling_Letters_Count(0)
{

}

void ALevel::Init()
{    
    Level_Rect.left = AsConfig::LEVEL_X_OFFSET * AsConfig::GLOBAL_SCALE;
    Level_Rect.top = AsConfig::LEVEL_Y_OFFSET * AsConfig::GLOBAL_SCALE;
    Level_Rect.right = AsConfig::LEVEL_X_OFFSET + AsConfig::CELL_WIDTH * AsConfig::LEVEL_WIDTH * AsConfig::GLOBAL_SCALE;
    Level_Rect.bottom = AsConfig::LEVEL_Y_OFFSET + AsConfig::CELL_HEIGHT * AsConfig::LEVEL_HEIGHT * AsConfig::GLOBAL_SCALE;

    memset(Current_Level, 0, sizeof(Current_Level));
    memset(Active_Briks, 0, sizeof(Active_Briks));
    memset(Falling_Letters, 0, sizeof(Falling_Letters));
}

void ALevel::Act()
{
    for (int i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
    {
        if (Active_Briks[i] != 0)
        {
            Active_Briks[i]->Act();

            if (Active_Briks[i]->Is_Finished())
            {
                Current_Level[Active_Briks[i]->level_y][Active_Briks[i]->level_x] = 0;

                delete Active_Briks[i];
                Active_Briks[i] = 0;
                Active_Bricks_Count--;
            }
        }            
    }

    //!!!

    for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
    {
        if (Falling_Letters[i] != 0)
        {
            Falling_Letters[i]->Act();

            if (Falling_Letters[i]->Is_Finished())
            {
                //Current_Level[Falling_Letters[i]->level_y][Falling_Letters[i]->level_x] = 0;

                delete Falling_Letters[i];
                Falling_Letters[i] = 0;
                Falling_Letters_Count--;
            }
        }
    }
}

void ALevel::Set_Current_Level(char level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH])
{
    memcpy(Current_Level, level, sizeof(Current_Level));
}

bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
    double direction = ball->Get_Direction();

    double min_ball_x, max_ball_x;
    double min_ball_y, max_ball_y;
    int min_level_x, max_level_x;
    int min_level_y, max_level_y;

    bool got_horizontal_hit, got_vertical_hit;
    double horizontal_reflection_pos, vertical_reflection_pos;

    if (next_y_pos + ball->RADIUS > AsConfig::LEVEL_Y_OFFSET + (AsConfig::LEVEL_HEIGHT - 1) * AsConfig::CELL_HEIGHT + AsConfig::BRICK_HEIGHT)
    {
        return false;
    }

    min_ball_x = next_x_pos - ball->RADIUS;
    max_ball_x = next_x_pos + ball->RADIUS;
    min_ball_y = next_y_pos - ball->RADIUS;
    max_ball_y = next_y_pos + ball->RADIUS;

    min_level_x = (int)((min_ball_x - AsConfig::LEVEL_X_OFFSET) / (double)AsConfig::CELL_WIDTH);
    max_level_x = (int)((max_ball_x - AsConfig::LEVEL_X_OFFSET) / (double)AsConfig::CELL_WIDTH);
    min_level_y = (int)((min_ball_y - AsConfig::LEVEL_Y_OFFSET) / (double)AsConfig::CELL_HEIGHT);
    max_level_y = (int)((max_ball_y - AsConfig::LEVEL_Y_OFFSET) / (double)AsConfig::CELL_HEIGHT);

    for (int i = max_level_y; i >= min_level_y; i--)
    {
        Current_Brick_Top_Y = AsConfig::LEVEL_Y_OFFSET + i * AsConfig::CELL_HEIGHT;
        Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::BRICK_HEIGHT;
        for (int j = min_level_x; j <= max_level_x; j++)
        {
            if (Current_Level[i][j] == 0)
            {
                continue;
            }

            Current_Brick_Left_X = AsConfig::LEVEL_X_OFFSET + j * AsConfig::CELL_WIDTH;
            Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::BRICK_WIDTH;

            got_horizontal_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizontal_reflection_pos);

            got_vertical_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflection_pos);


            if(got_horizontal_hit && got_vertical_hit)
            {
                if (vertical_reflection_pos < horizontal_reflection_pos)
                {
                    ball->Reflect(true);
                }
                else
                {
                    ball->Reflect(false);
                }

                On_Hit(j, i);

                return true;
            }
            else
            {
                if (got_horizontal_hit)
                {
                    ball->Reflect(false);

                    On_Hit(j, i);

                    return true;
                }
                else if(got_vertical_hit)
                {
                    ball->Reflect(true);

                    On_Hit(j, i);

                    return true;
                }
            } 
        }
    }

    return false;
}

bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, 
    ABall* ball, double &reflection_pos)
{
    double direction = ball->Get_Direction();

    // Проверяем попадание в нижнюю грань
    if (ball->Is_Moving_Up())
    {
        if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos,
            Current_Brick_Left_X, Current_Brick_Right_X, ball->RADIUS, reflection_pos))
        {
            // Проверяем возможность отскока вниз
            if (level_y < AsConfig::LEVEL_HEIGHT - 1 && Current_Level[level_y + 1][level_x] == 0)
                return true;
            else
                return false;
        }
    }
    else // Проверяем попадание в нижнюю грань
    {
        if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos,
            Current_Brick_Left_X, Current_Brick_Right_X, ball->RADIUS, reflection_pos))
        {
            // Проверяем возможность отскока вверх
            if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
            {
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

bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, 
    ABall* ball, double& reflection_pos)
{
    double direction = ball->Get_Direction();

    
    if (direction >= 0.0 && direction < M_PI_2 || direction >= M_PI + M_PI_2 && direction <= 2.0 * M_PI)
    {
        
    }

    // Проверяем попадание в правую грань
    if (ball->Is_Moving_Left()) 
    {
        if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, 
            Current_Brick_Top_Y, Current_Brick_Low_Y, ball->RADIUS, reflection_pos))
        {
            // Проверяем возможность отскока вправо
            if (level_x < AsConfig::LEVEL_WIDTH - 1 && Current_Level[level_y][level_x + 1] == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else // Проверяем попадание в левую грань
    {
        if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos,
            Current_Brick_Top_Y, Current_Brick_Low_Y, ball->RADIUS, reflection_pos))
        {
            // Проверяем возможность отскока влево
            if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
            {
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

void ALevel::On_Hit(int brick_x, int brick_y)
{
    EBrick_Type brick_type;

    brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

    if (Add_Falling_Letter(brick_x, brick_y, brick_type))
        Current_Level[brick_y][brick_x] = EBT_None;
    else
        Add_Active_Brick(brick_x, brick_y, brick_type);    
}

bool ALevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{// Создаем падующую букву, если можем
    
    int letter_x, letter_y;

    ELetter_Type letter_type;
    AFalling_Letter* falling_letter;

    if (brick_type == EBT_Blue || brick_type == EBT_Pink)
    {
        if (AsConfig::Rand(AsConfig::Hits_Per_Letter) == 0)
        {
            if (Falling_Letters_Count < AsConfig::Max_Falling_Letters_Count)
            {
                letter_type = ELT_O;

                for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
                {
                    if (Falling_Letters[i] == 0)
                    {
                        letter_x = (brick_x * AsConfig::CELL_WIDTH + AsConfig::LEVEL_X_OFFSET) * AsConfig::GLOBAL_SCALE;
                        letter_y = (brick_y * AsConfig::CELL_HEIGHT + AsConfig::LEVEL_Y_OFFSET) * AsConfig::GLOBAL_SCALE;

                        falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
                        Falling_Letters[i] = falling_letter;
                        Falling_Letters_Count++;
                        break;
                    }
                }
                return true;
            }
        }
    }

    return false;
}

void ALevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
{// Создаем активный кирпич, если можем
    AActive_Brick* active_brick;

    if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
        return;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Pink:
    case EBT_Blue:
        active_brick = new AActive_Brick(brick_type, brick_x, brick_y);
        Current_Level[brick_y][brick_x] = EBT_None;
        break;

    default:
        return;
    }

    for (int i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
    {
        if(Active_Briks[i] == 0)
            Active_Briks[i] = active_brick;

        Active_Bricks_Count++;
        break;
    }

    
}


void ALevel::Draw(HDC hdc, RECT& paint_area)// вывод всех кирпичей уровня
{
    RECT intersection_rect;

    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
    {
        for (int i = 0; i < AsConfig::LEVEL_HEIGHT; i++)
        {
            for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
            {
                Draw_Brick(hdc, AsConfig::LEVEL_X_OFFSET + (AsConfig::CELL_WIDTH * j),
                    AsConfig::LEVEL_Y_OFFSET + (AsConfig::CELL_HEIGHT * i), (EBrick_Type)Current_Level[i][j]);
            }
        }

        for (int i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
        {
            if (Active_Briks[i] != 0)
                Active_Briks[i]->Draw(hdc, paint_area);
        }
    }
       

    //!!!
    for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
    {
        if (Falling_Letters[i] != 0)
            Falling_Letters[i]->Draw(hdc, paint_area);
    }
}

void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_None:
        pen = AsConfig::BG_Pen;
        brush = AsConfig::BG_Brush;
        break;

    case EBT_Pink:
        pen = AsConfig::Brick_Pink_Pen;
        brush = AsConfig::Brick_Pink_Brush;
        break;

    case EBT_Blue:
        pen = AsConfig::Brick_Blue_Pen;
        brush = AsConfig::Brick_Blue_Brush;
        break;

    default:
        return;
    }


    SelectObject(hdc, pen);
    SelectObject(hdc, brush);

    RoundRect(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE, (x + AsConfig::BRICK_WIDTH) * AsConfig::GLOBAL_SCALE - 1,
        (y + AsConfig::BRICK_HEIGHT) * AsConfig::GLOBAL_SCALE - 1, 2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);

}





AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
    :brick_type(brick_type), letter_type(letter_type), X(x), Y(y), Rotation_step(2), 
    Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step), Got_Hit(false)
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

    if(IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
        Draw_Brick_Letter(hdc);
}


//
void AFalling_Letter::Act()
{
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

    if (Got_Hit || Letter_Cell.top >= AsConfig::MAX_Y_POS * AsConfig::GLOBAL_SCALE)
        return true;
    else
        return false;
}
