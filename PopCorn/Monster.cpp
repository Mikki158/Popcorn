#include "Monster.h"

// AMonster
// 
AMonster::AMonster()
    :X_Pos(0), Y_Pos(0), Next_Direction_Switch_Tick(0),
    Alive_Timer_Tick(0), Speed(0.0), Direction(0.0), 
    Monster_State(EMonster_State::Missing), Prev_Monster_Rect{}, Monster_Rect{}
{

}


//
AMonster::~AMonster()
{

}


//
bool AMonster::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{// Возвращаем: true/false - мячик коснулся монстра и направление было скоректировано / не коснулся

    double radius = (double)Width / 2.0;

    if (!(Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive))
        return false;

    if (! AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, X_Pos + radius, Y_Pos + radius, radius, ball))
        return false;
    
    
    Destroy();

    return true;
}


//
bool AMonster::Check_Hit(double next_x_pos, double next_y_pos)
{// Возвра: true, если в позиции (next_x_pos, next_y_pos) луч коснется монстра

    if (!(Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive))
        return false;

    if (next_x_pos >= X_Pos && next_x_pos <= X_Pos + (double)Width
        && next_y_pos >= Y_Pos && next_y_pos <= Y_Pos + (double)Height)
    {
        Destroy();
        return true;
    }
    else
        return false;
}


//
bool AMonster::Check_Hit(RECT& rect)
{// Возвра: true, если в позиции (next_x_pos, next_y_pos) прямоугольник rect коснется монстра

    RECT intersection_rect;

    if (!(Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive))
        return false;

    if (IntersectRect(&intersection_rect, &rect, &Monster_Rect))
    {
        Destroy();
        return true;
    }
    else
        return false;

}


//
void AMonster::Advance(double max_speed)
{
    bool got_new_direction = false;
    double next_step;
    double next_x_pos, next_y_pos;
    double origin_direction;
    RECT monster_rect;

    if (!(Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive))
        return;

    next_step = Speed / max_speed * AsConfig::Moving_STEP_SIZE;

    origin_direction = Direction;

    for (int i = 0; i < 16; i++)
    {
        next_x_pos = X_Pos + next_step * cos(Direction);
        next_y_pos = Y_Pos - next_step * sin(Direction);

        Get_Monster_Rect(next_x_pos, next_y_pos, monster_rect);

        if (AsLevel::Has_Brick_At(monster_rect))
            Direction += M_PI / 8.0;
        else
        {
            got_new_direction = true;
            break;
        }
    }

    if (!got_new_direction)
    {
        Direction = origin_direction - M_PI;
        return;
    }


    // Ограничиваем перемещение рамками уровня
    if (Monster_State == EMonster_State::Alive)
    {
        if (next_x_pos < (double)AsConfig::LEVEL_X_OFFSET)
            next_x_pos = (double)AsConfig::LEVEL_X_OFFSET;

        if (next_x_pos + (double)Width > (double)AsConfig::MAX_X_POS)
            next_x_pos = (double)(AsConfig::MAX_X_POS - Width);

        if (next_y_pos < (double)AsConfig::LEVEL_Y_OFFSET)
            next_y_pos = (double)AsConfig::LEVEL_Y_OFFSET;

        if (next_y_pos + (double)Height > (double)AsConfig::Floor_Y_Pos)
            next_y_pos = (double)(AsConfig::Floor_Y_Pos - Width);
    }

    X_Pos = next_x_pos;
    Y_Pos = next_y_pos;
}


//
void AMonster::Begin_Movement()
{
    // Заглушка, не используется
}


//
void AMonster::Finish_Movement()
{
    if (Is_Finished())
        return;

    Redraw_Monster();
}


//
double AMonster::Get_Speed()
{
    return Speed;
}


//
void AMonster::Act()
{
    switch (Monster_State)
    {
    case EMonster_State::Missing:
        return;

    case EMonster_State::Emitting:
        Act_Alive();

        if (AsConfig::Current_Timer_Tick >= Alive_Timer_Tick)
            Monster_State = EMonster_State::Alive;
        return;

    case EMonster_State::Alive:
        Act_Alive();
        Change_Direction();
        break;

    case EMonster_State::Destroing:
        Act_Destroing();
        break;

    default:
        break;
    }

    AsTools::Invalidate_Rect(Monster_Rect);
}


//
void AMonster::Clear(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Prev_Monster_Rect))
        return;

    AsTools::Ellipse(hdc, Prev_Monster_Rect, AsConfig::BG_Color);
}


//
void AMonster::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Monster_Rect))
        return;

    switch (Monster_State)
    {
    case EMonster_State::Missing:
        break;

    case EMonster_State::Emitting:
    case EMonster_State::Alive:
        Draw_Alive(hdc);
        break;

    case EMonster_State::Destroing:
        Draw_Destroing(hdc, paint_area);
        break;

    default:
        AsConfig::Throw();
        break;
    }
}

//
bool AMonster::Is_Finished()
{
    if (Monster_State == EMonster_State::Missing)
        return true;
    else
        return false;
}


//
void AMonster::Act_Destroing()
{
    bool destroing_is_finished = true;

    for (int i = 0; i < Explosive_Balls_Count; i++)
    {
        Explosive_Balls[i].Act();

        destroing_is_finished &= Explosive_Balls[i].Is_Finished();
    }

    if (destroing_is_finished)
        Monster_State = EMonster_State::Missing;
}


//
void AMonster::Activate(int x_pos, int y_pos, bool moving_right)
{
    int rand_speed;
    int emitting_time_offset;

    Monster_State = EMonster_State::Emitting;

    X_Pos = x_pos;
    Y_Pos = y_pos;

    rand_speed = AsTools::Rand(5) + 1;
    Speed = (double)rand_speed / 10.0;

    emitting_time_offset = (int)((double)AGate::Width / Speed);
    Alive_Timer_Tick = AsConfig::Current_Timer_Tick + emitting_time_offset;

    if (moving_right)
        Direction = 0.0;
    else
        Direction = M_PI;

    On_Activation();

    Redraw_Monster();
}


//
void AMonster::Destroy()
{
    bool is_red;
    int half_width, half_height;
    int x_pos, y_pos;
    int size, half_size, rest_size;
    int x_offset, y_offset;
    int time_offset;

    if (!(Monster_State == EMonster_State::Emitting || Monster_State == EMonster_State::Alive))
        return;

    half_width = Width * AsConfig::GLOBAL_SCALE / 2;
    half_height = Height * AsConfig::GLOBAL_SCALE / 2;
    x_pos = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE) + half_width;
    y_pos = (int)(Y_Pos * AsConfig::D_GLOBAL_SCALE) + half_height;


    half_size = half_width;

    if (half_height < half_size)
        half_size = half_height;

    for (int i = 0; i < Explosive_Balls_Count; i++)
    {
        x_offset = AsTools::Rand(half_width) - half_width / 2;
        y_offset = AsTools::Rand(half_height) - half_height / 2;

        rest_size = half_height - (int)sqrt(x_offset * x_offset + y_offset * y_offset);

        size = AsTools::Rand(rest_size / 2) + rest_size / 2;

        if (size < AsConfig::GLOBAL_SCALE)
            size = AsConfig::GLOBAL_SCALE;

        time_offset = AsTools::Rand(AsConfig::FPS * 3 / 2);

        if (AsTools::Rand(2) == 0)
            is_red = true;
        else
            is_red = false;

        Explosive_Balls[i].Explode(x_pos + x_offset, y_pos + y_offset, size * 2, is_red, time_offset, 10);
    }

    Monster_State = EMonster_State::Destroing;
}


//
void AMonster::Draw_Destroing(HDC hdc, RECT& paint_area)
{
    for (int i = 0; i < Explosive_Balls_Count; i++)
        Explosive_Balls[i].Draw(hdc, paint_area);
}


//
void AMonster::Redraw_Monster()
{
    Prev_Monster_Rect = Monster_Rect;

    Get_Monster_Rect(X_Pos, Y_Pos, Monster_Rect);

    AsTools::Invalidate_Rect(Monster_Rect);
    AsTools::Invalidate_Rect(Prev_Monster_Rect);
}


//
void AMonster::Get_Monster_Rect(double x_pos, double y_pos, RECT &rect)
{
    rect.left = (int)(x_pos * AsConfig::D_GLOBAL_SCALE);
    rect.top = (int)(y_pos * AsConfig::D_GLOBAL_SCALE);
    rect.right = rect.left + Width * AsConfig::GLOBAL_SCALE;
    rect.bottom = rect.top + Height * AsConfig::GLOBAL_SCALE;

}


//
void AMonster::Change_Direction()
{
    double direction_delta;

    if (AsConfig::Current_Timer_Tick > Next_Direction_Switch_Tick)
    {
        Next_Direction_Switch_Tick += AsTools::Rand(AsConfig::FPS);

        // Выбираем случайное направление +/- 45 градусов
        direction_delta = (double)(AsTools::Rand(90) - 45) * M_PI / 180.0;
        Direction += direction_delta;
    }
}



// AMonster_Eye
const double AMonster_Eye::Max_Cornea_Height = 11.0;
const double AMonster_Eye::Blink_Timeouts[Blink_Stage_Count] = { 0.4, 0.2, 0.8, 0.4, 0.4, 0.4, 0.8 };
const EEye_State AMonster_Eye::Blink_States[Blink_Stage_Count] =
{
    EEye_State::Closed, EEye_State::Opening, EEye_State::Starting, EEye_State::Closing,
    EEye_State::Opening, EEye_State::Starting, EEye_State::Closing
};


//
AMonster_Eye::AMonster_Eye()
    : Start_Blink_Timeout(0), Totla_Animation_Timeout(0), Cornea_Height(Max_Cornea_Height), 
    Eye_State(EEye_State::Closed), Blink_Ticks{}
{

}


//
void AMonster_Eye::Draw_Alive(HDC hdc)
{
    HRGN region;
    RECT rect, cornea_rect;

    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    const int half_scale = scale / 2;

    if (Monster_State == EMonster_State::Missing)
        return;


    // 1. Рисуем фон
    // 1.1 Ограничиваем вывод фона
    rect = Monster_Rect;

    rect.right++;
    rect.bottom++;

    region = CreateEllipticRgnIndirect(&rect);
    SelectClipRgn(hdc, region);

    // 1.2 Темный фон
    AsTools::Ellipse(hdc, Monster_Rect, AsConfig::Monster_Dark_Pink_Color);

    // 1.3 Красный фон
    rect = Monster_Rect;

    rect.left -= 2 * scale;
    rect.top -= 3 * scale;
    rect.right -= 2 * scale;
    rect.bottom -= 3 * scale;

    AsTools::Ellipse(hdc, rect, AsConfig::Pink_Color);

    SelectClipRgn(hdc, 0);
    DeleteObject(region);


    // 2. Рисуем сам глаз

    if (Eye_State == EEye_State::Closed)
        return;

    // 2.1 Роговица
    cornea_rect = Monster_Rect;

    cornea_rect.left += scale + half_scale;
    cornea_rect.top += 2 * scale + (int)((Max_Cornea_Height / 2.0 - Cornea_Height / 2.0) * d_scale);
    cornea_rect.right -= scale + half_scale;
    cornea_rect.bottom = cornea_rect.top + (int)(Cornea_Height * d_scale);

    // 2.2 Огранициваем вывод внутренней части глаза
    region = CreateEllipticRgnIndirect(&cornea_rect);
    SelectClipRgn(hdc, region);

    AsTools::Ellipse(hdc, cornea_rect, AsConfig::Monster_Cornea_Color);


    // 2.3 Радужка
    rect = Monster_Rect;

    rect.left += 4 * scale + half_scale;
    rect.top += 4 * scale;
    rect.right -= 4 * scale + half_scale;
    rect.bottom -= 5 * scale;

    AsTools::Ellipse(hdc, rect, AsConfig::Monster_Iris_Color);

    // 2.4 Зрачок
    rect = Monster_Rect;

    rect.left += 7 * scale;
    rect.top += 6 * scale;
    rect.right = rect.left + 2 * scale;
    rect.bottom = rect.top + 3 * scale;

    AsTools::Rect(hdc, rect, AsConfig::BG_Color);


    SelectClipRgn(hdc, 0);
    DeleteObject(region);

    // 2.5 Обводим роговицу
    AsConfig::BG_Outline_Color.Select(hdc);

    Arc(hdc, cornea_rect.left, cornea_rect.top, cornea_rect.right - 1, cornea_rect.bottom - 1, 0, 0, 0, 0);
}


//
void AMonster_Eye::Act_Alive()
{
    int i;
    int curr_tick_offset;
    int prev_tick;
    double ratio;

    if (Monster_State == EMonster_State::Missing)
        return;

    curr_tick_offset = (AsConfig::Current_Timer_Tick - Start_Blink_Timeout) % Totla_Animation_Timeout;

    for (i = 0; i < Blink_Stage_Count; i++)
    {
        if (curr_tick_offset < Blink_Ticks[i])
        {
            Eye_State = Blink_States[i];
            break;
        }
    }

    if (i == 0)
        prev_tick = 0;
    else
        prev_tick = Blink_Ticks[i - 1];

    ratio = (double)(curr_tick_offset - prev_tick) / (double)(Blink_Ticks[i] - prev_tick);

    switch (Eye_State)
    {
    case EEye_State::Closed:
        Cornea_Height = 0.0;
        break;

    case EEye_State::Opening:
        Cornea_Height = Max_Cornea_Height * ratio;
        break;

    case EEye_State::Starting:
        Cornea_Height = Max_Cornea_Height;
        break;

    case EEye_State::Closing:
        Cornea_Height = Max_Cornea_Height * (1.0 - ratio);
        break;

    default:
        AsConfig::Throw();
        break;
    }
}


//
void AMonster_Eye::On_Activation()
{
    double curr_timeout = 0.0;
    int tick_offset = 0;

    // Расчитываем тики анимации
    Start_Blink_Timeout = AsConfig::Current_Timer_Tick;

    for (int i = 0; i < Blink_Stage_Count; i++)
    {
        curr_timeout += Blink_Timeouts[i];
        tick_offset = (int)((double)AsConfig::FPS * curr_timeout);
        Blink_Ticks[i] = tick_offset;
    }

    Totla_Animation_Timeout = tick_offset;
}



// AMonster_Comet
//
AMonster_Comet::AMonster_Comet()
    :Ticks_Per_Rotation(0), Current_Angle(0.0)
{
    int rotation_ticks_delta = Max_Ticks_Per_Rotation - Min_Ticks_Per_Rotation;

    Ticks_Per_Rotation = AsTools::Rand(rotation_ticks_delta) + Min_Ticks_Per_Rotation;
}


//
void AMonster_Comet::Clear(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Prev_Monster_Rect))
        return;

    AsTools::Rect(hdc, Prev_Monster_Rect, AsConfig::BG_Color);
}


//
void AMonster_Comet::Draw_Alive(HDC hdc)
{
    XFORM xform, old_xform;
    RECT rect;
    double alpha;
    double monster_radius;
    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    int ball_size = 4 * scale - scale / 2;

    if (Monster_State == EMonster_State::Missing)
        return;

    //AsTools::Rect(hdc, Monster_Rect, AsConfig::Blue_Color);

    monster_radius = (double)(Width) * d_scale / 2.0 - 1.0;
    alpha = Current_Angle;

    GetWorldTransform(hdc, &old_xform);

    for (int i = 0; i < 2; i++)
    {
        xform.eM11 = (float)cos(alpha);
        xform.eM12 = (float)sin(alpha);
        xform.eM21 = (float)-sin(alpha);
        xform.eM22 = (float)cos(alpha);
        xform.eDx = (float)(X_Pos * d_scale + monster_radius);
        xform.eDy = (float)(Y_Pos * d_scale + monster_radius);
        SetWorldTransform(hdc, &xform);

        alpha += M_PI;

        rect.left = (int)(-monster_radius);
        rect.top = -ball_size / 2;
        rect.right = rect.left + ball_size;
        rect.bottom = rect.top + ball_size;

        AsTools::Ellipse(hdc, rect, AsConfig::White_Color);

        AsConfig::Monster_Comet_Tail.Select_Pen(hdc);

        rect.left = (int)(-monster_radius + 2.0 * d_scale);
        rect.top = (int)(-monster_radius + 2.0 * d_scale);
        rect.right = (int)(monster_radius - 2.0 * d_scale);
        rect.bottom = (int)(monster_radius - 2.0 * d_scale);

        Arc(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, 0, (int)(-monster_radius), (int)(-monster_radius), (int)(-4.0 * d_scale));

        rect.left += scale;
        rect.right -= scale;
        rect.bottom -= scale;

        Arc(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, 0, (int)(-monster_radius), (int)(-monster_radius), (int)(-4.0 * d_scale));
    }


    SetWorldTransform(hdc, &old_xform);
}


//
void AMonster_Comet::Act_Alive()
{
    int time_offset;
    double ratio;

    if (Monster_State == EMonster_State::Missing)
        return;

    time_offset = (AsConfig::Current_Timer_Tick - Alive_Timer_Tick) % Ticks_Per_Rotation;
    ratio = (double)time_offset / (double)Ticks_Per_Rotation;

    Current_Angle = -ratio * 2.0 * M_PI;
}


//
void AMonster_Comet::On_Activation()
{

}