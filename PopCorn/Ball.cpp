﻿#include "Ball.h"

// AGraphics_Object
//
AGraphics_Object::~AGraphics_Object()
{

}



// AMover
//
AMover::~AMover()
{

}



// AHit_Checker
//
bool AHit_Checker::Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x)
{// Проверяет пересечение горизонтального отрезка (проходящего от left_x до right_x через y) с окружностью радиусом ridius
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



// ABall
AHit_Checker* ABall::Hit_checkers[] = {};

int ABall::Hit_Checkers_Count = 0;
const double ABall::RADIUS = 2.0 - 0.5 / AsConfig::GLOBAL_SCALE;

//
ABall::ABall()
    :Ball_State(EBS_Disabled), Prev_Ball_State(EBS_Disabled), Ball_Direction(0.0), Testing_Is_Active(false),
    Center_X_Pos(-10.0), Center_Y_Pos(0.0), Ball_Speed(0.0), Ball_Rect(), Prev_Ball_Rect(), Test_Iteration(0), 
    Parachute_Rect(), Prev_Parachute_Rect(), Rest_Test_Distance(), Prev_Ball_Direction(0.0), Prev_Ball_Speed(0.0),
    Release_Timer_Tick(0)
{
    //Set_State(EBS_Normal, -10);
}


//
void ABall::Advance(double max_speed)
{
    HWND hwnd = AsConfig::HWnd;

    bool got_hit = true;
    double next_x_pos, next_y_pos;
    int max_x_pos = AsConfig::MAX_X_POS;
    int max_y_pos = AsConfig::MAX_Y_POS;
    double next_step;
    int prev_hits_count = 0;
    const int max_hits_count = 8;


    if (Ball_State == EBS_Disabled || Ball_State == EBS_Lost || Ball_State == EBS_On_Platform || Ball_State == EBS_Teleporting)
    {
        return;
    }

    next_step = Ball_Speed / max_speed * AsConfig::Moving_STEP_SIZE;

    while (got_hit)
    {
        got_hit = false;

        next_x_pos = Center_X_Pos + next_step * cos(Ball_Direction);
        next_y_pos = Center_Y_Pos - next_step * sin(Ball_Direction);

        // корректируем позицию при отражении:
        for (int i = 0; i < Hit_Checkers_Count; i++)
        {
            got_hit |= Hit_checkers[i]->Check_Hit(next_x_pos, next_y_pos, this); // от рамки
        }

        if (got_hit)
        {
            prev_hits_count++;

            if (prev_hits_count > max_hits_count)
            {
                Ball_Direction += M_PI / 8.0;

                prev_hits_count = 0;
            }
        }
        else
        {
            // Мячик продолжает движение, если не взаимодействовал с другими объектами
            Center_X_Pos = next_x_pos;
            Center_Y_Pos = next_y_pos;

            if (Testing_Is_Active)
                Rest_Test_Distance -= next_step;
        }

        if (Ball_State == EBS_On_Platform)
            break;
    }

    
}


//
void ABall::Begin_Movement()
{
    Prev_Ball_Rect = Ball_Rect;
}


//
void ABall::Finish_Movement()
{
    if (Ball_State == EBS_Disabled || Ball_State == EBS_Lost)
        return;


    Redraw_Ball();
    
    if (Ball_State == EBS_On_Parachute)
    {
        Prev_Parachute_Rect = Parachute_Rect;
    
        Parachute_Rect.bottom = Ball_Rect.bottom;
        Parachute_Rect.top = Parachute_Rect.bottom - Parachute_Size * AsConfig::GLOBAL_SCALE;
    
        Redraw_Parachute();
    }
}


//
double ABall::Get_Speed()
{
    return Ball_Speed;
}


//
void ABall::Act()
{
    // Заглушка, Не используется, т.к. мячик сам ничего не делает (не анимируется)
}


//
void ABall::Clear(HDC hdc, RECT& paint_area)
{// Очищаем фон
    RECT intersection_rect;

    if (Ball_State == EBS_Disabled)
        return;

    if ((Ball_State == EBS_Teleporting || Ball_State == EBS_Lost) && Ball_State == Prev_Ball_State)
        return;

    if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
    {
        AsConfig::BG_Color.Select(hdc);

        Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
    }
}


//
void ABall::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (Ball_State == EBS_Disabled)
        return;

    if ((Ball_State == EBS_Teleporting || Ball_State == EBS_Lost) && Ball_State == Prev_Ball_State)
        return;

    switch (Ball_State)
    {
    case EBS_Normal:
        break;

    case EBS_Lost:
        if (Prev_Ball_State == EBS_On_Parachute)
            Clear_Parachute(hdc);
        return;
        break;

    case EBS_On_Parachute:
        Draw_Parachute(hdc, paint_area);
        break;

    case EBS_Off_Parachute:
        Clear_Parachute(hdc);
        Set_State(EBS_Normal, Center_X_Pos, Center_Y_Pos);
        break;

    case EBS_Teleporting:
        return;

    default:
        break;
    }


    if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
    {// рисуем шарик

        AsConfig::White_Color.Select(hdc);

        Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
    }
}


//
bool ABall::Is_Finished()
{
    return false; // Заглушка, Не используется, т.к. мячик сам ничего не делает (не анимируется)
}


//
void ABall::Draw_Teleporting(HDC hdc, int step)
{
    int top_y = Ball_Rect.top + step / 2;
    int low_y = Ball_Rect.bottom - step / 2 - 1;

    if (top_y >= low_y)
        return;

    AsConfig::White_Color.Select(hdc);
    Ellipse(hdc, Ball_Rect.left, top_y, Ball_Rect.right - 1, low_y);
}


//
void ABall::Set_For_Test()
{
    Testing_Is_Active = true;
    Rest_Test_Distance = 50.0;

    Set_State(EBS_Normal, 100 + Test_Iteration, 100);
    Ball_Direction = M_PI_4;

    Test_Iteration += 1;
}


//
void ABall::Set_State(EBall_State new_state, double x_pos, double y_pos)
{
    switch (new_state)
    {
    case EBS_Disabled:
        //Result_Distance = 0.0;
        Ball_Speed = 0.0;
        break;

    case EBS_Normal:
        Center_X_Pos = x_pos;
        Center_Y_Pos = y_pos;
        Ball_Speed = AsConfig::Normal_Ball_Speed;
        //Result_Distance = 0.0;
        Ball_Direction = M_PI_4;
        Redraw_Ball();
        break;

    case EBS_Lost:
        Redraw_Ball();
        Redraw_Parachute();
        if (!(Ball_State == EBS_Normal || Ball_State == EBS_On_Parachute))
            AsConfig::Throw(); // Только из этих состояний можно потерять мячик

        Ball_Speed = 0.0;
        Center_X_Pos = x_pos;
        Center_Y_Pos = y_pos;
        break;

    case EBS_On_Platform:
        Center_X_Pos = x_pos;
        Center_Y_Pos = y_pos;
        Prev_Ball_Speed = Ball_Speed;
        //Ball_Speed = 0.0;
        //Result_Distance = 0.0;
        Prev_Ball_Direction = Ball_Direction;
        //Ball_Direction = M_PI_4;
        Release_Timer_Tick = AsConfig::Current_Timer_Tick + On_Platform_Timeout;
        Redraw_Ball();
        break;

    case EBS_Off_Parachute:
        if (Ball_State != EBS_On_Parachute)
            AsConfig::Throw(); // В это состояние можно перейти только из EBS_On_Parachute

        Ball_Speed = 0.0;
        //Result_Distance = 0.0;
        Redraw_Ball();
        Redraw_Parachute();
        break;

    case EBS_On_Parachute:
        AsConfig::Throw(); // Для постановки на парашют нужно вызвать специальный метод Set_On_parachute()
        break;

    case EBS_Teleporting:
        if (!(Ball_State == EBS_Normal || Ball_State == EBS_On_Parachute || Ball_State == EBS_Teleporting))
            AsConfig::Throw(); // Только из этих состояний можно войти в телепорт мячик

        Ball_Speed = 0.0;
        //Result_Distance = 0.0;
        Center_X_Pos = x_pos;
        Center_Y_Pos = y_pos;
        Redraw_Ball();

        if (Ball_State == EBS_On_Parachute)
            Redraw_Parachute();

        break;

    default:
        AsConfig::Throw();
        break;
    }

    Prev_Ball_State = Ball_State;
    Ball_State = new_state;
}


//
void ABall::Set_Direction(double new_direction)
{
    const double pi_2 = 2.0 * M_PI;
    while (new_direction > 2.0 * pi_2)
    {
        new_direction -= 2.0 * pi_2;
    }

    while (new_direction < 0.0)
    {
        new_direction += pi_2;
    }

    Ball_Direction = new_direction;
}


//
void ABall::Reflect(bool from_horizontal)
{
    if (from_horizontal)
    {
        Set_Direction(-Ball_Direction);
    }
    else
    {
        Set_Direction(M_PI - Ball_Direction);
    }
}


//
void ABall::Set_On_Parachute(int brick_x, int brick_y)
{
    int cell_x = AsConfig::LEVEL_X_OFFSET + brick_x * AsConfig::CELL_WIDTH;
    int cell_y = AsConfig::LEVEL_Y_OFFSET + brick_y * AsConfig::CELL_HEIGHT;

    Ball_Direction = M_PI + M_PI_2;
    Ball_Speed = 1.0;
    Ball_State = EBS_On_Parachute;

    Parachute_Rect.left = cell_x * AsConfig::GLOBAL_SCALE;
    Parachute_Rect.top = cell_y * AsConfig::GLOBAL_SCALE;
    Parachute_Rect.right = Parachute_Rect.left + Parachute_Size * AsConfig::GLOBAL_SCALE;
    Parachute_Rect.bottom = Parachute_Rect.top + Parachute_Size * AsConfig::GLOBAL_SCALE;

    Prev_Parachute_Rect = Parachute_Rect;

    Center_X_Pos = (double)(cell_x + AsConfig::CELL_WIDTH / 2) - 1.0 / AsConfig::D_GLOBAL_SCALE;
    Center_Y_Pos = (double)(cell_y + Parachute_Size) - RADIUS * 2;

    Redraw_Parachute();
}


//
void ABall::Get_Center(double& x_pos, double& y_pos)
{
    x_pos = Center_X_Pos;
    y_pos = Center_Y_Pos;
}


//
void ABall::Set_Speed(double new_speed)
{
    Ball_Speed = new_speed;
}


//
void ABall::Forced_Advance(double direction, double speed, double max_speed)
{// Принудительное смещение мячика

    double prev_direction = Ball_Direction;
    double prev_speed = Ball_Speed;
    EBall_State prev_state = Ball_State;

    Ball_Direction = direction;
    Ball_Speed = speed;
    Ball_State = EBS_Normal;

    Advance(max_speed);

    Ball_Direction = prev_direction;
    Ball_Speed = prev_speed;
    Ball_State = prev_state;
}


//
void ABall::Release()
{// Продолжить принудительное движение мячика

    Set_State(EBS_Normal, Center_X_Pos, Center_Y_Pos);

    Ball_Speed = Prev_Ball_Speed;

    if (Ball_Speed < AsConfig::Normal_Ball_Speed)
        Ball_Speed = AsConfig::Normal_Ball_Speed;

    Ball_Direction = Prev_Ball_Direction;
    Release_Timer_Tick = 0;
}


//
bool ABall::Is_Test_Finished()
{
    if (Testing_Is_Active)
    {
        if (Rest_Test_Distance <= 0)
        {
            Testing_Is_Active = false;
            Set_State(EBS_Lost);
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}


//
bool ABall::Is_Moving_Up()
{
    if (Ball_Direction >= 0.0 && Ball_Direction < M_PI)
        return true;
    else
        return false;

}


//
bool ABall::Is_Moving_Left()
{
    if (Ball_Direction > M_PI_2 && Ball_Direction < M_PI + M_PI_2)
        return true;
    else
        return false;
}


//
double ABall::Get_Direction()
{
    return Ball_Direction;
}


//
EBall_State ABall::Get_State()
{
    return Ball_State;
}


//
void ABall::Add_Hit_Checker(AHit_Checker* hit_checker)
{
    if(Hit_Checkers_Count >= sizeof(Hit_checkers) / sizeof(Hit_checkers[0]))
    {
        return;
    }
    Hit_checkers[Hit_Checkers_Count++] = hit_checker;
}


//
void ABall::Redraw_Ball()
{
    Ball_Rect.left = (int)((Center_X_Pos - RADIUS) * AsConfig::D_GLOBAL_SCALE);
    Ball_Rect.top = (int)((Center_Y_Pos - RADIUS) * AsConfig::D_GLOBAL_SCALE);
    Ball_Rect.right = (int)((Center_X_Pos + RADIUS) * AsConfig::D_GLOBAL_SCALE);
    Ball_Rect.bottom = (int)((Center_Y_Pos + RADIUS) * AsConfig::D_GLOBAL_SCALE);

    InvalidateRect(AsConfig::HWnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(AsConfig::HWnd, &Ball_Rect, FALSE);
}


//
void ABall::Draw_Parachute(HDC hdc, RECT& paint_area)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    int dome_height = (Parachute_Rect.bottom - Parachute_Rect.top) / 2;
    int arc_height = 4 * scale;
    int arc_x; 
    int line_y;
    int ball_center_x, ball_center_y;
    
    RECT intersection_rect, sub_arc, other_arc;

    if (!IntersectRect(&intersection_rect, &paint_area, &Parachute_Rect))
        return;

    // 0. Очищаем фон
    Clear_Parachute(hdc);

    // 1. Купол
    AsConfig::Blue_Color.Select(hdc);
    Chord(hdc, Parachute_Rect.left, Parachute_Rect.top, Parachute_Rect.right - 1, Parachute_Rect.bottom - 1,
        Parachute_Rect.right, Parachute_Rect.top + dome_height - 1, Parachute_Rect.left, Parachute_Rect.top + dome_height - 1);

    // 2.1 Левая арка
    AsConfig::BG_Color.Select(hdc);

    arc_x = Parachute_Rect.left + 1;

    sub_arc.left = Parachute_Rect.left + 1;
    sub_arc.top = Parachute_Rect.top + dome_height - arc_height / 2;
    sub_arc.right = sub_arc.left + 3 * scale;
    sub_arc.bottom = sub_arc.top + 4 * scale;

    Ellipse(hdc, sub_arc.left, sub_arc.top, sub_arc.right - 1, sub_arc.bottom - 1);

    // 2.2 Средняя арка
    other_arc = sub_arc;

    other_arc.left = arc_x + 3 * scale + 1;
    other_arc.right = arc_x + 11 * scale;

    Ellipse(hdc, other_arc.left, other_arc.top, other_arc.right - 1, other_arc.bottom - 1);


    // 2.3 Правая арка
    other_arc = sub_arc;

    other_arc.left = arc_x + 11 * scale + 1;
    other_arc.right = arc_x + 14 * scale + 1;

    Ellipse(hdc, other_arc.left, other_arc.top, other_arc.right - 1, other_arc.bottom - 1);


    // 3. Стропы
    ball_center_x = (Parachute_Rect.left + Parachute_Rect.right) / 2;
    ball_center_y = Parachute_Rect.bottom + 1 * scale;
    line_y = Parachute_Rect.top + dome_height;
    AsConfig::White_Color.Select(hdc);

    MoveToEx(hdc, Parachute_Rect.left, line_y, 0);
    LineTo(hdc, ball_center_x, ball_center_y);

    MoveToEx(hdc, Parachute_Rect.left + 3 * scale + 1, line_y, 0);
    LineTo(hdc, ball_center_x, ball_center_y);

    MoveToEx(hdc, Parachute_Rect.right - 4 * scale + 1, line_y, 0);
    LineTo(hdc, ball_center_x, ball_center_y);

    MoveToEx(hdc, Parachute_Rect.right, line_y - 1, 0);
    LineTo(hdc, ball_center_x, ball_center_y);

}


//
void ABall::Redraw_Parachute()
{
    InvalidateRect(AsConfig::HWnd, &Prev_Parachute_Rect, FALSE);
    InvalidateRect(AsConfig::HWnd, &Parachute_Rect, FALSE);
}


//
void ABall::Clear_Parachute(HDC hdc)
{// Очищаем фон
    AsConfig::BG_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, Prev_Parachute_Rect);
}