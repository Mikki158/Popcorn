#include "Ball.h"

AHit_Checker* ABall::Hit_checkers[] = {};

int ABall::Hit_Checkers_Count = 0;
const double ABall::RADIUS = 2.0 - 0.5 / AsConfig::GLOBAL_SCALE;
const double ABall::START_BALL_Y_POS = 181.0;

ABall::ABall()
    :Ball_State(EBS_Normal), Ball_Direction(0), Ball_Pen(), Ball_Brush(), Result_Distance(0.0), Testing_Is_Active(false),
    Center_X_Pos(0.0), Center_Y_Pos(START_BALL_Y_POS), Ball_Speed(0.0), Ball_Rect(), Prev_Ball_Rect(), Test_Iteration(0)
{
    Set_State(EBS_Normal, 0);
}

void ABall::Init()
{
    AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}


void ABall::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect))
    {
        // очищаем фон
        SelectObject(hdc, AsConfig::BG_Pen);
        SelectObject(hdc, AsConfig::BG_Brush);

        Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
    }   

    if (Ball_State == EBS_Lost)
        return;

    if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
    {
        // рисуем шарик
        SelectObject(hdc, Ball_Pen);
        SelectObject(hdc, Ball_Brush);

        Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
    }
}

void ABall::Move()
{
    HWND hwnd = AsConfig::HWnd;

    bool got_hit;
    double next_x_pos, next_y_pos;
    int max_x_pos = AsConfig::MAX_X_POS;
    int max_y_pos = AsConfig::MAX_Y_POS;

    Result_Distance += Ball_Speed;

    if (Ball_State != EBS_Normal)
    {
        return;
    }

    while (Result_Distance > AsConfig::STEP_SIZE)
    {
        got_hit = false;

        next_x_pos = Center_X_Pos + AsConfig::STEP_SIZE * cos(Ball_Direction);
        next_y_pos = Center_Y_Pos - AsConfig::STEP_SIZE * sin(Ball_Direction);

        // корректируем позицию при отражении:
        for (int i = 0; i < Hit_Checkers_Count; i++)
        {
            got_hit |= Hit_checkers[i]->Check_Hit(next_x_pos, next_y_pos, this); // от рамки
        }

        if (!got_hit)
        {
            // Мячик продолжает движение, если не взаимодействовал с другими объектами
            Result_Distance -= AsConfig::STEP_SIZE;

            Center_X_Pos = next_x_pos;
            Center_Y_Pos = next_y_pos;

            if(Testing_Is_Active)
                Rest_Test_Distance -= AsConfig::STEP_SIZE;
        }        
    }    

    

    Redraw_Ball();
}

void ABall::Set_For_Test()
{
    Testing_Is_Active = true;
    Rest_Test_Distance = 40.0;

    Set_State(EBS_Normal, 90 + Test_Iteration, 195);
    Ball_Direction = M_PI_4;

    Test_Iteration += 1;
}

bool ABall::Is_Test_Finished()
{
    if (Testing_Is_Active)
    {
        if (Rest_Test_Distance <= 0)
        {
            Testing_Is_Active = false;
            Set_State(EBS_Lost, 0);
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

void ABall::Redraw_Ball()
{
    Prev_Ball_Rect = Ball_Rect;

    Ball_Rect.left = (int)((Center_X_Pos - RADIUS) * AsConfig::GLOBAL_SCALE);
    Ball_Rect.top = (int)((Center_Y_Pos - RADIUS) * AsConfig::GLOBAL_SCALE);
    Ball_Rect.right = (int)((Center_X_Pos + RADIUS) * AsConfig::GLOBAL_SCALE);
    Ball_Rect.bottom = (int)((Center_Y_Pos + RADIUS) * AsConfig::GLOBAL_SCALE);

    InvalidateRect(AsConfig::HWnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(AsConfig::HWnd, &Ball_Rect, FALSE);
}

void ABall::Set_State(EBall_State new_state, double x_pos, double y_pos)
{
    switch (new_state)
    {
    case EBS_Normal:
        Center_X_Pos = x_pos;
        Center_Y_Pos = y_pos;
        Ball_Speed = 3.0; 
        Result_Distance = 0.0;
        Ball_Direction = M_PI_4;
        Redraw_Ball();
        break;

    case EBS_Lost:
        Ball_Speed = 0.0;
        break;

    case EBS_On_Platform:
        Center_X_Pos = x_pos;
        Center_Y_Pos = y_pos;
        Ball_Speed = 0.0;
        Result_Distance = 0.0;
        Ball_Direction = M_PI_4;
        Redraw_Ball();
        break;

    default:
        break;
    }

    Ball_State = new_state;
}

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

double ABall::Get_Direction()
{
    return Ball_Direction;
}

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

bool ABall::Is_Moving_Up()
{
    if (Ball_Direction >= 0.0 && Ball_Direction < M_PI)
        return true;
    else
        return false;
    
}

bool ABall::Is_Moving_Left()
{
    if (Ball_Direction > M_PI_2 && Ball_Direction < M_PI + M_PI_2)
        return true;
    else
        return false;

    
}

void ABall::Add_Hit_Checker(AHit_Checker* hit_checker)
{
    if(Hit_Checkers_Count >= sizeof(Hit_checkers) / sizeof(Hit_checkers[0]))
    {
        return;
    }
    Hit_checkers[Hit_Checkers_Count++] = hit_checker;
}

EBall_State ABall::Get_State()
{
    return Ball_State;
}

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
