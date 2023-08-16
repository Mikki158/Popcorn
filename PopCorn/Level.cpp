#include "Level.h"

char AsLevel::Level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH] =
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

char AsLevel::Test_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH] =
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


AsLevel::AsLevel()
    :Level_Rect(), 
    Active_Bricks_Count(0), Falling_Letters_Count(0)
{

}

void AsLevel::Init()
{    
    Level_Rect.left = AsConfig::LEVEL_X_OFFSET * AsConfig::GLOBAL_SCALE;
    Level_Rect.top = AsConfig::LEVEL_Y_OFFSET * AsConfig::GLOBAL_SCALE;
    Level_Rect.right = AsConfig::LEVEL_X_OFFSET + AsConfig::CELL_WIDTH * AsConfig::LEVEL_WIDTH * AsConfig::GLOBAL_SCALE;
    Level_Rect.bottom = AsConfig::LEVEL_Y_OFFSET + AsConfig::CELL_HEIGHT * AsConfig::LEVEL_HEIGHT * AsConfig::GLOBAL_SCALE;

    memset(Current_Level, 0, sizeof(Current_Level));
    memset(Active_Briks, 0, sizeof(Active_Briks));
    memset(Falling_Letters, 0, sizeof(Falling_Letters));
}

void AsLevel::Act()
{    
    Act_Objects((AGraphics_Object**)Active_Briks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);
    Act_Objects((AGraphics_Object**)Falling_Letters, AsConfig::Max_Falling_Letters_Count, Falling_Letters_Count);
}

void AsLevel::Act_Objects(AGraphics_Object** object_array, int objects_max_count, int &current_count_objects)
{
    for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
    {
        if (object_array[i] != 0)
        {
            object_array[i]->Act();

            if (object_array[i]->Is_Finished())
            {
                delete object_array[i];
                object_array[i] = 0;
                current_count_objects--;
            }
        }
    }
}

void AsLevel::Set_Current_Level(char level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH])
{
    memcpy(Current_Level, level, sizeof(Current_Level));
}

bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
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

bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, 
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

bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, 
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

void AsLevel::On_Hit(int brick_x, int brick_y)
{
    EBrick_Type brick_type;

    brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

    if (Add_Falling_Letter(brick_x, brick_y, brick_type))
        Current_Level[brick_y][brick_x] = EBT_None;
    else
        Add_Active_Brick(brick_x, brick_y, brick_type);    
}

bool AsLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
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

void AsLevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
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


void AsLevel::Draw(HDC hdc, RECT& paint_area)// вывод всех кирпичей уровня
{
    RECT intersection_rect, brick_rect;;

    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
    {
        for (int i = 0; i < AsConfig::LEVEL_HEIGHT; i++)
        {
            for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
            {
                
                brick_rect.left = (AsConfig::LEVEL_X_OFFSET + AsConfig::CELL_WIDTH * j) * AsConfig::GLOBAL_SCALE;
                brick_rect.top = (AsConfig::LEVEL_Y_OFFSET + AsConfig::CELL_HEIGHT * i)* AsConfig::GLOBAL_SCALE;
                brick_rect.right = brick_rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
                brick_rect.bottom = brick_rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;

                if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
                    Draw_Brick(hdc, brick_rect, (EBrick_Type)Current_Level[i][j]);
            }
        }

        Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Active_Briks, AsConfig::Max_Active_Bricks_Count);
    }

    Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}

bool AsLevel::Get_Next_Falling_Leter(AFalling_Letter** falling_letter, int& index)
{
    AFalling_Letter* current_letter;

    if (Falling_Letters_Count == 0)
        return false;

    if (index < 0 || index >= AsConfig::Max_Falling_Letters_Count)
        return false;

    while (index < AsConfig::Max_Falling_Letters_Count)
    {
        current_letter = Falling_Letters[index++];
        if (current_letter != nullptr)
        {
            *falling_letter = current_letter;
            return true;
        }            
    }
        
    return false;   
}

void AsLevel::Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object **object_array, int objects_max_count)
{
    for (int i = 0; i < objects_max_count; i++)
    {
        if (object_array[i] != 0)
            object_array[i]->Draw(hdc, paint_area);
    }
}

void AsLevel::Draw_Brick(HDC hdc, RECT brick_rect, EBrick_Type brick_type)
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

    RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right - 1, brick_rect.bottom - 1,
        2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);
}





