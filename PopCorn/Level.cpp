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
AsLevel * AsLevel::Level = nullptr;
//
AsLevel::AsLevel()
    :Level_Rect(), Current_Level(), Current_Brick_Top_Y(),
    Current_Brick_Right_X(), Current_Brick_Low_Y(), Current_Brick_Left_X(),
    Parachute_Color(AsConfig::Pink_Color, AsConfig::Blue_Color, AsConfig::GLOBAL_SCALE), Teleport_Bricks_Count(0),
    Teleport_Bricks_Pos(0), Advertisement(nullptr), Need_To_Cancel_All(false)
{
    Level = this;
}


//
AsLevel::~AsLevel()
{
    Cancel_All_Activity();

    delete[] Teleport_Bricks_Pos;
}


//
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{// Корректируем позицию при отражении от кирпичей

    double direction = ball->Get_Direction();

    double min_ball_x, max_ball_x;
    double min_ball_y, max_ball_y;
    int min_level_x, max_level_x;
    int min_level_y, max_level_y;

    bool got_horizontal_hit, got_vertical_hit;
    double horizontal_reflection_pos, vertical_reflection_pos;

    if (next_y_pos + AsConfig::Ball_RADIUS > AsConfig::LEVEL_Y_OFFSET + (AsConfig::LEVEL_HEIGHT - 1) * AsConfig::CELL_HEIGHT + AsConfig::BRICK_HEIGHT)
    {
        return false;
    }

    min_ball_x = next_x_pos - AsConfig::Ball_RADIUS;
    max_ball_x = next_x_pos + AsConfig::Ball_RADIUS;
    min_ball_y = next_y_pos - AsConfig::Ball_RADIUS;
    max_ball_y = next_y_pos + AsConfig::Ball_RADIUS;

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
bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos)
{// Возвра: true, если в позиции (next_x_pos, next_y_pos) луч коснется кирпича

    int level_x_index, level_y_index;

    level_x_index = (int)((next_x_pos - AsConfig::LEVEL_X_OFFSET) / (double)AsConfig::CELL_WIDTH);
    level_y_index = (int)((next_y_pos - AsConfig::LEVEL_Y_OFFSET) / (double)AsConfig::CELL_HEIGHT);

    if(level_x_index < 0 || level_x_index >= AsConfig::LEVEL_WIDTH)
        return false;

    if (level_y_index < 0 || level_y_index >= AsConfig::LEVEL_HEIGHT)
        return false;

    if (Current_Level[level_y_index][level_x_index] == 0)
        return false;

    On_Hit(level_x_index, level_y_index, 0, true);

    return true;
}


//
void AsLevel::Advance(double max_speed)
{
    // Заглушка, т.к. этот метод не используется
}


//
void AsLevel::Begin_Movement()
{
    // Заглушка, т.к. этот метод не используется
}


//
void AsLevel::Finish_Movement()
{
    // Заглушка, т.к. этот метод не используется
}


//
double AsLevel::Get_Speed()
{
    return 0.0; // Заглушка, т.к. этот метод не используется
}


//
void AsLevel::Clear(HDC hdc, RECT& paint_area)
{// Стираем движущиеся объекты

    for (auto* letter : Falling_Letters)
        letter->Clear(hdc, paint_area);

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

        for (auto* brick : Active_Bricks)
            brick->Draw(hdc, paint_area);
    }

    for (auto* letter : Falling_Letters)
        letter->Draw(hdc, paint_area);
}


//
void AsLevel::Act()
{
    Act_Objects(Active_Bricks);
    Act_Objects(Falling_Letters);

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

            if (brick_type == EBrick_Type::Teleport)
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

                if (brick_type == EBrick_Type::Teleport)
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
bool AsLevel::Has_Brick_At(int level_x, int level_y)
{
    EBrick_Type brick_type;

    if (level_x < 0 || level_x >= AsConfig::LEVEL_WIDTH)
        return false;
    if (level_y < 0 || level_y >= AsConfig::LEVEL_HEIGHT)
        return false;

    brick_type = (EBrick_Type)Level->Current_Level[level_y][level_x];

    if (brick_type == EBrick_Type::None)
        return false;
    else
        return true;
}


//
bool AsLevel::Has_Brick_At(RECT monster_rect)
{
    int x_step = AsConfig::CELL_WIDTH * AsConfig::GLOBAL_SCALE;
    int y_step = AsConfig::CELL_HEIGHT * AsConfig::GLOBAL_SCALE;
    int level_x_offset = AsConfig::LEVEL_X_OFFSET * AsConfig::GLOBAL_SCALE;
    int level_y_offset = AsConfig::LEVEL_Y_OFFSET * AsConfig::GLOBAL_SCALE;
    int min_level_x, max_level_x;
    int min_level_y, max_level_y;


    min_level_x = (monster_rect.left - level_x_offset) / x_step;
    max_level_x = (monster_rect.right - level_x_offset) / x_step;

    min_level_y = (monster_rect.top - level_y_offset) / y_step;
    max_level_y = (monster_rect.bottom - level_y_offset) / y_step;

    for (int i = min_level_y; i <= max_level_y; i++)
        for (int j = min_level_x; j <= max_level_x; j++)
            if (Has_Brick_At(j, i))
                return true;

    return false;
}


//
bool AsLevel::Get_Next_Falling_Leter(AFalling_Letter** falling_letter, int& index)
{
    if (index < (int)Falling_Letters.size())
    {
        *falling_letter = (AFalling_Letter *)Falling_Letters[index++];
        return true;
    }

    return false;
}


//
void AsLevel::Act_Objects(std::vector<AGraphics_Object*> &falling_letters)
{
    //std::vector<AFalling_Letter*>::iterator it;
    auto it = falling_letters.begin();

    while( it != falling_letters.end())
    {
        (*it)->Act();

        if ((*it)->Is_Finished())
        {
            delete* it;
            it = falling_letters.erase(it);
        }
        else
            it++;
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
void AsLevel::Delete_Objects(std::vector<AGraphics_Object*>& falling_letters)
{
    for (auto it = falling_letters.begin(); it != falling_letters.end(); it++)
    {
        delete *it;
    }

    falling_letters.erase(falling_letters.begin(), falling_letters.end());
}


//
void AsLevel::Draw_Brick(HDC hdc, RECT brick_rect, int level_x, int level_y)
{
    const AColor* color = 0;
    EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

    switch (brick_type)
    {
    case EBrick_Type::None:
        if (Advertisement != nullptr && Advertisement->Has_Brick_At(level_x, level_y))
            break;
        // else - No break

    case EBrick_Type::Pink:
    case EBrick_Type::Blue:
        AActive_Brick_Pink_Blue::Draw_In_Level(hdc, brick_rect, brick_type);
        break;

    case EBrick_Type::Unbreakable:
        AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
        break;

    case EBrick_Type::Multihit_1:
    case EBrick_Type::Multihit_2:
    case EBrick_Type::Multihit_3:
    case EBrick_Type::Multihit_4:
        AActive_Brick_Multihit::Draw_In_Level(hdc, brick_rect, brick_type);
        break;

    case EBrick_Type::Parachute:
        Draw_Parachute_in_Level(hdc, brick_rect);
        break;

    case EBrick_Type::Teleport:
        AActive_Brick_Teleport::Draw_In_Level(hdc, brick_rect);
        break;

    case EBrick_Type::Ad:
        AActive_Brick_Ad::Draw_In_Level(hdc, brick_rect);
        break;

    case EBrick_Type::Invisible:
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
    AsTools::Round_Rect(hdc, rect);

    // 2. Нижний сегмент
    rect.top += 3 * scale + 1;
    rect.bottom += 3 * scale + 1;

    AsTools::Round_Rect(hdc, rect);
}


//
void AsLevel::Redraw_Brick(int brick_x, int brick_y)
{
    RECT brick_rect;

    brick_rect.left = (AsConfig::LEVEL_X_OFFSET + brick_x * AsConfig::CELL_WIDTH) * AsConfig::GLOBAL_SCALE;
    brick_rect.top = (AsConfig::LEVEL_Y_OFFSET + brick_y * AsConfig::CELL_HEIGHT) * AsConfig::GLOBAL_SCALE;
    brick_rect.right = brick_rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    brick_rect.bottom = brick_rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;

    AsTools::Invalidate_Rect(brick_rect);
}


//
void AsLevel::Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall_Object *ball, bool vertical_hit)
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
            direction = EDirection_Type::Down;
        }
        else
        {
            direction = EDirection_Type::Up;
        }
    }
    else
    {
        if (pre_teleport_x_pos < curr_ball_x_pos)
        {
            direction = EDirection_Type::Right;
        }
        else
        {
            direction = EDirection_Type::Left;
        }
    }

    // Перебираем все направления в поисках свободного
    got_direction = false;

    for (int i = 0; i < 4; i++)
    {
        switch (direction)
        {
        case EDirection_Type::Left:
            if ((dest_brick_x > 0 && (EBrick_Type)Current_Level[dest_brick_y][dest_brick_x - 1] == EBrick_Type::None))
                got_direction = true;
            break;

        case EDirection_Type::Up:
            if ((dest_brick_y > 0 && (EBrick_Type)Current_Level[dest_brick_y - 1][dest_brick_x] == EBrick_Type::None))
                got_direction = true;
            break;

        case EDirection_Type::Right:
            if ((dest_brick_x < AsConfig::LEVEL_WIDTH - 1) && ((EBrick_Type)Current_Level[dest_brick_y][dest_brick_x + 1] == EBrick_Type::None))
                got_direction = true;
            break;

        case EDirection_Type::Down:
            if ((dest_brick_y < AsConfig::LEVEL_HEIGHT) && (EBrick_Type)Current_Level[dest_brick_y + 1][dest_brick_x] == EBrick_Type::None)
                got_direction = true;
            break;

        default:
            AsConfig::Throw();
        }
        
        if (got_direction)
            break;
        
        direction = (EDirection_Type)((int)direction - 1);
        ball->Set_Direction(ball->Get_Direction() + M_PI_2);


        if ((int)direction < 0)
            direction = EDirection_Type::Down;
    }

    destination_teleport->Relese_Direction = direction;
    
    Active_Bricks.push_back(source_teleport);
    Active_Bricks.push_back(destination_teleport);
}


//
void AsLevel::Cancel_All_Activity()
{
    Delete_Objects(Active_Bricks);
    Delete_Objects(Falling_Letters);
}


//
bool AsLevel::On_Hit(int brick_x, int brick_y, ABall_Object* ball, bool vertical_hit)
{
    bool can_reflect = true;
    EBrick_Type brick_type;

    if (brick_x == 1 && brick_y == 1)
    {
        int yy = 1;
    }

    brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

    if (ball == nullptr && brick_type == EBrick_Type::Parachute)
    {
        brick_type = EBrick_Type::Pink;
        Current_Level[brick_y][brick_x] = (char)brick_type;
    }

    if (brick_type == EBrick_Type::Parachute)
    {
        ball->Set_On_Parachute(brick_x, brick_y);
        Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
    }
    else if (Add_Falling_Letter(brick_x, brick_y, brick_type))
        Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
    else
        can_reflect = Create_Active_Brick(brick_x, brick_y, brick_type, ball, vertical_hit);


    Redraw_Brick(brick_x, brick_y);

    return can_reflect;
}


//
bool AsLevel::Create_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type, ABall_Object* ball, bool vertical_hit)
{// Создаем активный кирпич, если можем

    AActive_Brick* active_brick = nullptr;

    if (Active_Bricks.size() >= AsConfig::Max_Active_Bricks_Count)
        return true;

    switch (brick_type)
    {
    case EBrick_Type::None:
        return true;

    case EBrick_Type::Pink:
    case EBrick_Type::Blue:
        active_brick = new AActive_Brick_Pink_Blue(brick_type, brick_x, brick_y);
        Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
        break;

    case EBrick_Type::Unbreakable:
        active_brick = new AActive_Brick_Unbreakable(brick_x, brick_y);
        break;

    case EBrick_Type::Multihit_1:
        active_brick = new AActive_Brick_Multihit(brick_x, brick_y);
        Current_Level[brick_y][brick_x] = (char)EBrick_Type::None;
        break;

    case EBrick_Type::Multihit_2:
    case EBrick_Type::Multihit_3:
    case EBrick_Type::Multihit_4:
        Current_Level[brick_y][brick_x] = (char)brick_type - 1;
        break;

    case EBrick_Type::Parachute:
        AsConfig::Throw(); // Для парашюта активный кирпич не создается
        break;

    case EBrick_Type::Teleport:
        if(ball != nullptr)
            Add_Active_Brick_Teleport(brick_x, brick_y, ball, vertical_hit);
        return false;
        break;

    case EBrick_Type::Ad:
        active_brick = new AActive_Brick_Ad(brick_x, brick_y, Advertisement);
        Current_Level[brick_y][brick_x] = (char)EBrick_Type::Invisible;
        break;

    case EBrick_Type::Invisible:
        return true;

    default:
        AsConfig::Throw();
    }

    if(active_brick != nullptr)
        Active_Bricks.push_back(active_brick);

    return true;
}


//
bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, 
    ABall_Object* ball, double &reflection_pos)
{
    double direction = ball->Get_Direction();

    // Проверяем попадание в нижнюю грань
    if (ball->Is_Moving_Up())
    {
        if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos,
            Current_Brick_Left_X, Current_Brick_Right_X, AsConfig::Ball_RADIUS, reflection_pos))
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
            Current_Brick_Left_X, Current_Brick_Right_X, AsConfig::Ball_RADIUS, reflection_pos))
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
    ABall_Object* ball, double& reflection_pos)
{
    double direction = ball->Get_Direction();

    
    if (direction >= 0.0 && direction < M_PI_2 || direction >= M_PI + M_PI_2 && direction <= 2.0 * M_PI)
    {
        
    }

    // Проверяем попадание в правую грань
    if (ball->Is_Moving_Left()) 
    {
        if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, 
            Current_Brick_Top_Y, Current_Brick_Low_Y, AsConfig::Ball_RADIUS, reflection_pos))
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
            Current_Brick_Top_Y, Current_Brick_Low_Y, AsConfig::Ball_RADIUS, reflection_pos))
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

    if (!(brick_type == EBrick_Type::Blue || brick_type == EBrick_Type::Pink))
        return false;

    if (AsTools::Rand(AsConfig::Hits_Per_Letter) != 0)
        return false;

    
    if (Falling_Letters.size() >= AsConfig::Max_Falling_Letters_Count)
        return false;
    

    letter_x = (brick_x * AsConfig::CELL_WIDTH + AsConfig::LEVEL_X_OFFSET) * AsConfig::GLOBAL_SCALE;
    letter_y = (brick_y * AsConfig::CELL_HEIGHT + AsConfig::LEVEL_Y_OFFSET) * AsConfig::GLOBAL_SCALE;

    letter_type = ELetter_Type::T;
    // 
    //letter_type = AFalling_Letter::Get_Random_Letter_Type();
    // 
    //switch (AsTools::Rand(3))
    //{
    //case 0:
    //    letter_type = ELetter_Type::L;
    //    break;
    //
    //case 1:
    //    letter_type = ELetter_Type::W;
    //    break;
    //
    //case 2:
    //    letter_type = ELetter_Type::K;
    //
    //default:
    //    break;
    //}

    falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
    Falling_Letters.push_back(falling_letter);
    return true;
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

    dest_index = AsTools::Rand(Teleport_Bricks_Count);

    if (Teleport_Bricks_Pos[dest_index].X == source_x && Teleport_Bricks_Pos[dest_index].Y == source_y)
    {// Если случайно выбрали текущий телепорт - переходим к следующему

        dest_index++;

        if (dest_index >= Teleport_Bricks_Count)
            dest_index = 0; // Переходим на начало массива, если вышли за его пределы
    }

    destination_teleport = new AActive_Brick_Teleport(Teleport_Bricks_Pos[dest_index].X, Teleport_Bricks_Pos[dest_index].Y, 0, 0);

    return destination_teleport;
}