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


// AsLevel
//
AsLevel::AsLevel()
    :Level_Rect(), Active_Bricks_Count(0), Falling_Letters_Count(0), Falling_Letters(), Current_Level(), Current_Brick_Top_Y(),
    Current_Brick_Right_X(), Current_Brick_Low_Y(), Current_Brick_Left_X(), Active_Briks(), 
    Parachute_Color(AsConfig::Pink_Color, AsConfig::Blue_Color, AsConfig::GLOBAL_SCALE), Teleport_Bricks_Count(0),
    Teleport_Bricks_Pos(0), Advertisement(nullptr), Need_To_Cancel_All(false)
{

}


//
AsLevel::~AsLevel()
{
    Cancel_All_Activity();

    delete[] Teleport_Bricks_Pos;
}


//
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
    if (min_level_x < 0)
        min_level_x = 0;

    max_level_x = (int)((max_ball_x - AsConfig::LEVEL_X_OFFSET) / (double)AsConfig::CELL_WIDTH);
    if (max_level_x >= AsConfig::LEVEL_WIDTH - 1)
        max_level_x = AsConfig::LEVEL_WIDTH - 1;

    min_level_y = (int)((min_ball_y - AsConfig::LEVEL_Y_OFFSET) / (double)AsConfig::CELL_HEIGHT);
    if (min_level_y < 0)
        min_level_y = 0;

    max_level_y = (int)((max_ball_y - AsConfig::LEVEL_Y_OFFSET) / (double)AsConfig::CELL_HEIGHT);
    if (max_level_y >= AsConfig::LEVEL_HEIGHT - 1)
        max_level_y = AsConfig::LEVEL_HEIGHT - 1;

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

            //if (j == AsConfig::LEVEL_WIDTH)
            //    continue;

            Current_Brick_Left_X = AsConfig::LEVEL_X_OFFSET + j * AsConfig::CELL_WIDTH;
            Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::BRICK_WIDTH;

            got_horizontal_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizontal_reflection_pos);

            got_vertical_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflection_pos);


            if (got_horizontal_hit && got_vertical_hit)
            {
                if (On_Hit(j, i, ball, true))
                {
                    if (vertical_reflection_pos < horizontal_reflection_pos)
                    {
                        ball->Reflect(true);
                    }
                    else
                    {
                        ball->Reflect(false);
                    }
                }

                return true;
            }
            else
            {
                if (got_horizontal_hit)
                {
                    if(On_Hit(j, i, ball, false))
                        ball->Reflect(false);                   

                    return true;
                }
                else if (got_vertical_hit)
                {
                    if(On_Hit(j, i, ball, true))
                        ball->Reflect(true);

                    return true;
                }
            }
        }
    }

    return false;
}


//
void AsLevel::Clear(HDC hdc, RECT& paint_area)
{// Стираем движущиеся объекты

    Clear_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);

    if (Advertisement != nullptr)
        Advertisement->Clear(hdc, paint_area);

    if (Need_To_Cancel_All)
    {
        Cancel_All_Activity();
        Need_To_Cancel_All = false;
    }
}


//
void AsLevel::Draw(HDC hdc, RECT& paint_area)
{// вывод всех объектов уровня

    RECT intersection_rect, brick_rect;;

    if (Advertisement != nullptr)
        Advertisement->Draw(hdc, paint_area);

    if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
    {
        for (int i = 0; i < AsConfig::LEVEL_HEIGHT; i++)
        {
            for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
            {

                brick_rect.left = (AsConfig::LEVEL_X_OFFSET + AsConfig::CELL_WIDTH * j) * AsConfig::GLOBAL_SCALE;
                brick_rect.top = (AsConfig::LEVEL_Y_OFFSET + AsConfig::CELL_HEIGHT * i) * AsConfig::GLOBAL_SCALE;
                brick_rect.right = brick_rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
                brick_rect.bottom = brick_rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;

                if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
                    Draw_Brick(hdc, brick_rect, j, i);
            }
        }

        Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Active_Briks, AsConfig::Max_Active_Bricks_Count);
    }

    Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);


}


//
void AsLevel::Act()
{
    Act_Objects((AGraphics_Object**)&Active_Briks, AsConfig::Max_Active_Bricks_Count, Active_Bricks_Count);
    Act_Objects((AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count, Falling_Letters_Count);

    if (Advertisement != nullptr)
        Advertisement->Act();
}


//
bool AsLevel::Is_Finished()
{
    return false; // Заглушка, т.к. этот метод не используется
}


//
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


//
void AsLevel::Set_Current_Level(char level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH])
{
    int index;
    EBrick_Type brick_type;

    memcpy(Current_Level, level, sizeof(Current_Level));

    // 1. Считаем телепорты
    Teleport_Bricks_Count = 0;

    for (int i = 0; i < AsConfig::LEVEL_HEIGHT; i++)
    {
        for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
        {
            brick_type = (EBrick_Type)Current_Level[i][j];

            if (brick_type == EBT_Teleport)
                Teleport_Bricks_Count++;
        }
    }

    delete[] Teleport_Bricks_Pos;
    Teleport_Bricks_Pos = nullptr;

    // 2. Сохранаем координаты телепортов
    if (Teleport_Bricks_Count != 0)
    {
        if (Teleport_Bricks_Count == 1)
            AsConfig::Throw(); // Телепортов должно быть больше 1

        index = 0;
        Teleport_Bricks_Pos = new SPoint[Teleport_Bricks_Count];
        
        for (int i = 0; i < AsConfig::LEVEL_HEIGHT; i++)
        {
            for (int j = 0; j < AsConfig::LEVEL_WIDTH; j++)
            {
                brick_type = (EBrick_Type)Current_Level[i][j];

                if (brick_type == EBT_Teleport)
                {
                    Teleport_Bricks_Pos[index].X = j;
                    Teleport_Bricks_Pos[index].Y = i;
                    index++;
                } 
            }
        }
    }

    Advertisement = new AAdvertisement(9, 6, 2, 3);

}


//
void AsLevel::Stop()
{
    Need_To_Cancel_All = true;
}


//
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


//
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


//
void AsLevel::Clear_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, int objects_max_count)
{
    for (int i = 0; i < objects_max_count; i++)
    {
        if (object_array[i] != 0)
            object_array[i]->Clear(hdc, paint_area);
    }
}


//
void AsLevel::Delete_Objects(AGraphics_Object** object_array, int &objects_count, int objects_max_count)
{
    for (int i = 0; i < objects_max_count; i++)
    {
        if (object_array[i] != 0)
        {
            delete object_array[i];
            object_array[i] = nullptr;
        }
    }

    objects_count = 0;
}


//
void AsLevel::Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, int objects_max_count)
{
    for (int i = 0; i < objects_max_count; i++)
    {
        if (object_array[i] != 0)
            object_array[i]->Draw(hdc, paint_area);
    }
}


//
void AsLevel::Draw_Brick(HDC hdc, RECT brick_rect, int level_x, int level_y)
{
    const AColor* color = 0;
    EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

    switch (brick_type)
    {
    case EBT_None:
        if (Advertisement != nullptr && Advertisement->Has_Brick_At(level_x, level_y))
            break;
        // else - No break

    case EBT_Pink:
    case EBT_Blue:
        AActive_Brick_Pink_Blue::Draw_In_Level(hdc, brick_rect, brick_type);
        break;

    case EBT_Unbreakable:
        AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
        break;

    case EBT_Multihit_1:
    case EBT_Multihit_2:
    case EBT_Multihit_3:
    case EBT_Multihit_4:
        AActive_Brick_Multihit::Draw_In_Level(hdc, brick_rect, brick_type);
        break;

    case EBT_Parachute:
        Draw_Parachute_in_Level(hdc, brick_rect);
        break;

    case EBT_Teleport:
        AActive_Brick_Teleport::Draw_In_Level(hdc, brick_rect);
        break;

    case EBT_Ad:
        AActive_Brick_Ad::Draw_In_Level(hdc, brick_rect);
        break;

    case EBT_Invisible:
        break;

    default:
        AsConfig::Throw();
        return;
    }
}


//
void AsLevel::Draw_Parachute_in_Level(HDC hdc, RECT brick_rect)
{
    int scale = AsConfig::GLOBAL_SCALE;

    Draw_Parachute_Part(hdc, brick_rect, 0, 4);
    Draw_Parachute_Part(hdc, brick_rect, 4, 6);
    Draw_Parachute_Part(hdc, brick_rect, 10, 4);

}


//
void AsLevel::Draw_Parachute_Part(HDC hdc, RECT brick_rect, int offset, int width)
{
    // 1. Верхний сегмент
    int scale = AsConfig::GLOBAL_SCALE;
    RECT rect;

    rect.left = brick_rect.left + offset * scale + 1;
    rect.top = brick_rect.top + 1;
    rect.right = rect.left + width * scale;
    rect.bottom = rect.top + 3 * scale;

    Parachute_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, rect);

    // 2. Нижний сегмент
    rect.top += 3 * scale + 1;
    rect.bottom += 3 * scale + 1;

    AsConfig::Round_Rect(hdc, rect);
}


//
void AsLevel::Redraw_Brick(int brick_x, int brick_y)
{
    RECT brick_rect;

    brick_rect.left = (AsConfig::LEVEL_X_OFFSET + brick_x * AsConfig::CELL_WIDTH) * AsConfig::GLOBAL_SCALE;
    brick_rect.top = (AsConfig::LEVEL_Y_OFFSET + brick_y * AsConfig::CELL_HEIGHT) * AsConfig::GLOBAL_SCALE;
    brick_rect.right = brick_rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    brick_rect.bottom = brick_rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;

    InvalidateRect(AsConfig::HWnd, &brick_rect, FALSE);
}


//
void AsLevel::Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall *ball, bool vertical_hit)
{
    int dest_brick_x, dest_brick_y;
    bool got_direction;
    double pre_teleport_x_pos, pre_teleport_y_pos;
    double curr_ball_x_pos, curr_ball_y_pos;

    AActive_Brick_Teleport* source_teleport, * destination_teleport;
    EDirection_Type direction;

    // Позиция мячика перед входом в телепорт
    ball->Get_Center(pre_teleport_x_pos, pre_teleport_y_pos); 

    destination_teleport = Select_Destination_Teleport(brick_x, brick_y);
    source_teleport = new AActive_Brick_Teleport(brick_x, brick_y, ball, destination_teleport);

    destination_teleport->Get_Level_Pos(dest_brick_x, dest_brick_y);

    // После создания телепорта мячик стал в его центре
    ball->Get_Center(curr_ball_x_pos, curr_ball_y_pos); 

    // Определяем направление, откуда прилетел мячик
    if (vertical_hit)
    {
        if (pre_teleport_y_pos < curr_ball_y_pos)
        {
            direction = EDT_Down;
        }
        else
        {
            direction = EDT_Up;
        }
    }
    else
    {
        if (pre_teleport_x_pos < curr_ball_x_pos)
        {
            direction = EDT_Right;
        }
        else
        {
            direction = EDT_Left;
        }
    }

    // Перебираем все направления в поисках свободного
    got_direction = false;

    for (int i = 0; i < 4; i++)
    {
        switch (direction)
        {
        case EDT_Left:
            if ((dest_brick_x > 0 && Current_Level[dest_brick_y][dest_brick_x - 1] == EBT_None))
                got_direction = true;
            break;

        case EDT_Up:
            if ((dest_brick_y > 0 && Current_Level[dest_brick_y - 1][dest_brick_x] == EBT_None))
                got_direction = true;
            break;

        case EDT_Right:
            if ((dest_brick_x < AsConfig::LEVEL_WIDTH - 1) && (Current_Level[dest_brick_y][dest_brick_x + 1] == EBT_None))
                got_direction = true;
            break;

        case EDT_Down:
            if ((dest_brick_y < AsConfig::LEVEL_HEIGHT) && (Current_Level[dest_brick_y + 1][dest_brick_x] == EBT_None))
                got_direction = true;
            break;

        default:
            AsConfig::Throw();
        }
        
        if (got_direction)
            break;
        
        direction = (EDirection_Type)(direction - 1);
        ball->Set_Direction(ball->Get_Direction() + M_PI_2);


        if (direction < 0)
            direction = EDT_Down;
    }

    destination_teleport->Relese_Direction = direction;
    
    Add_Active_Brick(source_teleport);
    Add_Active_Brick(destination_teleport);

}


//
void AsLevel::Add_Active_Brick(AActive_Brick* active_brick)
{
    for (int i = 0; i < AsConfig::Max_Active_Bricks_Count; i++)
    {
        if (Active_Briks[i] == 0)
        {
            Active_Briks[i] = active_brick;
            Active_Bricks_Count++;
            break;
        }
    }
}


//
void AsLevel::Cancel_All_Activity()
{
    Delete_Objects((AGraphics_Object**)&Active_Briks, Active_Bricks_Count, AsConfig::Max_Active_Bricks_Count);
    Delete_Objects((AGraphics_Object**)&Falling_Letters, Falling_Letters_Count, AsConfig::Max_Falling_Letters_Count);
}


//
bool AsLevel::On_Hit(int brick_x, int brick_y, ABall* ball, bool vertical_hit)
{
    bool can_reflect = true;
    EBrick_Type brick_type;

    if (brick_x == 1 && brick_y == 1)
    {
        int yy = 1;
    }

    brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

    if (brick_type == EBT_Parachute)
    {
        ball->Set_On_Parachute(brick_x, brick_y);
        Current_Level[brick_y][brick_x] = EBT_None;
    }
    else if (Add_Falling_Letter(brick_x, brick_y, brick_type))
        Current_Level[brick_y][brick_x] = EBT_None;
    else
        can_reflect = Create_Active_Brick(brick_x, brick_y, brick_type, ball, vertical_hit);


    Redraw_Brick(brick_x, brick_y);

    return can_reflect;
}


//
bool AsLevel::Create_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type, ABall* ball, bool vertical_hit)
{// Создаем активный кирпич, если можем

    AActive_Brick* active_brick = nullptr;

    if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
        return true;

    switch (brick_type)
    {
    case EBT_None:
        return true;

    case EBT_Pink:
    case EBT_Blue:
        active_brick = new AActive_Brick_Pink_Blue(brick_type, brick_x, brick_y);
        Current_Level[brick_y][brick_x] = EBT_None;
        break;

    case EBT_Unbreakable:
        active_brick = new AActive_Brick_Unbreakable(brick_x, brick_y);
        break;

    case EBT_Multihit_1:
        active_brick = new AActive_Brick_Multihit(brick_x, brick_y);
        Current_Level[brick_y][brick_x] = EBT_None;
        break;

    case EBT_Multihit_2:
    case EBT_Multihit_3:
    case EBT_Multihit_4:
        Current_Level[brick_y][brick_x] = brick_type - 1;
        break;

    case EBT_Parachute:
        AsConfig::Throw(); // Для парашюта активный кирпич не создается
        break;

    case EBT_Teleport:
        Add_Active_Brick_Teleport(brick_x, brick_y, ball, vertical_hit);
        return false;
        break;

    case EBT_Ad:
        active_brick = new AActive_Brick_Ad(brick_x, brick_y, Advertisement);
        Current_Level[brick_y][brick_x] = EBT_Invisible;
        break;

    case EBT_Invisible:
        return true;

    default:
        AsConfig::Throw();
    }

    if(active_brick != nullptr)
        Add_Active_Brick(active_brick);

    return true;
}


//
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


//
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


//
bool AsLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{// Создаем падующую букву, если можем
    
    int letter_x, letter_y;

    ELetter_Type letter_type;
    AFalling_Letter* falling_letter;

    if (!(brick_type == EBT_Blue || brick_type == EBT_Pink))
        return false;

    if (AsConfig::Rand(AsConfig::Hits_Per_Letter) != 0)
        return false;

    
    if (Falling_Letters_Count >= AsConfig::Max_Falling_Letters_Count)
        return false;
    
    for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; i++)
    {
        if (Falling_Letters[i] == 0)
        {
            letter_x = (brick_x * AsConfig::CELL_WIDTH + AsConfig::LEVEL_X_OFFSET) * AsConfig::GLOBAL_SCALE;
            letter_y = (brick_y * AsConfig::CELL_HEIGHT + AsConfig::LEVEL_Y_OFFSET) * AsConfig::GLOBAL_SCALE;

            //letter_type = ELT_P;
            letter_type = AFalling_Letter::Get_Random_Letter_Type();

            falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
            Falling_Letters[i] = falling_letter;
            Falling_Letters_Count++;
            return true;
        }
    }
    return false;
}


//
AActive_Brick_Teleport *AsLevel::Select_Destination_Teleport(int source_x, int source_y)
{
    int dest_index;

    AActive_Brick_Teleport* destination_teleport;

    if (Teleport_Bricks_Count < 2)
    {
        AsConfig::Throw();
        return 0;
    }

    dest_index = AsConfig::Rand(Teleport_Bricks_Count);

    if (Teleport_Bricks_Pos[dest_index].X == source_x && Teleport_Bricks_Pos[dest_index].Y == source_y)
    {// Если случайно выбрали текущий телепорт - переходим к следующему

        dest_index++;

        if (dest_index >= Teleport_Bricks_Count)
            dest_index = 0; // Переходим на начало массива, если вышли за его пределы
    }

    destination_teleport = new AActive_Brick_Teleport(Teleport_Bricks_Pos[dest_index].X, Teleport_Bricks_Pos[dest_index].Y, 0, 0);

    return destination_teleport;
}