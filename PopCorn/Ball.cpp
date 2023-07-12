#include "Ball.h"

const double ABall::START_BALL_Y_POS = 181.0;

ABall::ABall()
    :Ball_State(EBS_Normal), Ball_Direction(0), Ball_Pen(), Ball_Brush(), 
    Ball_X_Pos(0.0), Ball_Y_Pos(START_BALL_Y_POS), Ball_Speed(0.0), Ball_Rect(), Prev_Ball_Rect()
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

    if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
    {
        // рисуем шарик
        SelectObject(hdc, Ball_Pen);
        SelectObject(hdc, Ball_Brush);

        Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
    }
}

void ABall::Move(ALevel* level, int platform_x_pos, int platform_width)
{
    HWND hwnd = AsConfig::HWnd;

    double next_x_pos, next_y_pos;
    double max_x_pos = AsConfig::MAX_X_POS - AsConfig::BALL_SIZE;
    double max_y_pos = AsConfig::MAX_Y_POS - AsConfig::BALL_SIZE;
    double platform_y_pos = AsConfig::Platform_Y_POS - AsConfig::BALL_SIZE;

    if (Ball_State != EBS_Normal)
    {
        return;
    }

    

    next_x_pos = Ball_X_Pos + (Ball_Speed * cos(Ball_Direction));
    next_y_pos = Ball_Y_Pos - (Ball_Speed * sin(Ball_Direction));

    // отражение шарика от стен
    if (next_x_pos < AsConfig::BORDER_X_OFFSET)
    {
        next_x_pos = AsConfig::LEVEL_X_OFFSET - (next_x_pos - AsConfig::LEVEL_X_OFFSET);
        Ball_Direction = M_PI - Ball_Direction;
    }
    if (next_y_pos < AsConfig::BORDER_Y_OFFSET)
    {
        next_y_pos = AsConfig::BORDER_Y_OFFSET - (next_y_pos - AsConfig::BORDER_Y_OFFSET);
        Ball_Direction = -Ball_Direction;
    }
    if (next_x_pos > max_x_pos)
    {
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
        Ball_Direction = M_PI - Ball_Direction;
    }
    if (next_y_pos > max_y_pos)
    {
        if (level->Has_Floor)
        {
            next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
            Ball_Direction = -Ball_Direction;
        }
        else
        {
            if (next_y_pos > max_y_pos + AsConfig::BALL_SIZE * 2)
            {
                Ball_State = EBS_Lost;
            }            
        }
        
    }

    // отражаем шарик от платформы
    if (next_y_pos > platform_y_pos)
    {
        if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
        {
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
            Ball_Direction = M_PI + (M_PI - Ball_Direction);
        }
    }

    // отражаем от киричей

    level->Check_Level_Brick_Heat(next_y_pos, Ball_Direction);

    //Смещаем шарик
    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;

    Redraw_Ball();
}

void ABall::Redraw_Ball()
{
    HWND hwnd = AsConfig::HWnd;

    Prev_Ball_Rect = Ball_Rect;

    Ball_Rect.left = (int)(Ball_X_Pos * AsConfig::GLOBAL_SCALE);
    Ball_Rect.top = (int)(Ball_Y_Pos * AsConfig::GLOBAL_SCALE);
    Ball_Rect.right = Ball_Rect.left + AsConfig::BALL_SIZE * AsConfig::GLOBAL_SCALE;
    Ball_Rect.bottom = Ball_Rect.top + AsConfig::BALL_SIZE * AsConfig::GLOBAL_SCALE;

    InvalidateRect(AsConfig::HWnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(AsConfig::HWnd, &Ball_Rect, FALSE);
}

void ABall::Set_State(EBall_State new_state, int x_pos)
{
    switch (new_state)
    {
    case EBS_Normal:
        Ball_X_Pos = x_pos - AsConfig::BALL_SIZE / 2;
        Ball_Y_Pos = START_BALL_Y_POS;
        Ball_Speed = 3.0;
        Ball_Direction = M_PI - M_PI_4;
        Redraw_Ball();
        break;

    case EBS_Lost:
        Ball_Speed = 0.0;
        break;

    case EBS_On_Platform:
        Ball_X_Pos = x_pos - AsConfig::BALL_SIZE / 2;
        Ball_Y_Pos = START_BALL_Y_POS;
        Ball_Speed = 0.0;
        Ball_Direction = M_PI - M_PI_4;
        Redraw_Ball();
        break;

    default:
        break;
    }

    Ball_State = new_state;
}

EBall_State ABall::Get_State()
{
    return Ball_State;
}
