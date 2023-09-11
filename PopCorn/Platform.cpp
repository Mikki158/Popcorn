﻿#include "Platform.h"


//AsPlatform_State
//
AsPlatform_State::AsPlatform_State()
    :Current_State(EPlatform_State::Regular), Regular(EPlatform_Substate_Regular::Missing), 
    Meltdown(EPlatform_Substate_Meltdown::Unknow), Rolling(EPlatform_Substate_Rolling::Unknow),
    Glue(EPlatform_Substate_Glue::Unknow), Moving(EPlatform_Moving_State::Stop)
{
}


//
AsPlatform_State::operator EPlatform_State() const
{
    return Current_State;
}


//
void AsPlatform_State::operator = (EPlatform_State new_state)
{
    Current_State = new_state;
}



// AsPlatform
//
const double AsPlatform::Max_Glue_Spot_Height_Ratio = 1.0;
const double AsPlatform::Min_Glue_Spot_Height_Ratio = 0.2;
const double AsPlatform::Glue_Spot_Height_Ratio_Step = 0.05;

//
AsPlatform::AsPlatform()
    : X_Pos(AsConfig::BORDER_X_OFFSET), Width(NORMAL_WIDTH), Inner_width(NORMAL_WIDTH - CIRCLE_SIZE), 
    Platform_Rect(), Prev_Platform_Rect(), Meltdown_Y_Pos(0), Rolling_Step(0), Normal_Platform_Image(nullptr),
    Normal_Platform_Image_Width(0), Normal_Platform_Image_Height(0), Platform_Circle_Color(255, 85, 255), 
    Platform_Inner_Color(85, 255, 255), Meltdown_Platform_Y_Pos(), Speed(0.0),
    Left_Key_Down(false), Right_Key_Down(false), Glue_Spot_Height_Ratio(0.0), Ball_Set(nullptr)
{
    X_Pos = (AsConfig::MAX_X_POS - Width) / 2;
}


//
AsPlatform::~AsPlatform()
{
    delete[] Normal_Platform_Image;
}


//
void AsPlatform::Advance(double max_speed)
{
    double max_platform_x, next_step;

    if (Platform_State.Moving == EPlatform_Moving_State::Stoping || Platform_State.Moving == EPlatform_Moving_State::Stop)
        return;

    max_platform_x = AsConfig::MAX_X_POS - Width + 1;
    next_step = Speed / max_speed * AsConfig::Moving_STEP_SIZE;

    X_Pos += next_step;

    if (X_Pos <= AsConfig::BORDER_X_OFFSET)
    {
        X_Pos = AsConfig::BORDER_X_OFFSET;
        Speed = 0.0;
        Platform_State.Moving = EPlatform_Moving_State::Stoping;
    }

    if (X_Pos >= max_platform_x)
    {
        X_Pos = max_platform_x;
        Speed = 0.0;
        Platform_State.Moving = EPlatform_Moving_State::Stoping;
    }

    // Смещаем прикрепленные мячики

    if ((Platform_State == EPlatform_State::Regular && Platform_State.Regular == EPlatform_Substate_Regular::Ready) || 
        (Platform_State == EPlatform_State::Glue && Platform_State.Glue == EPlatform_Substate_Glue::Active))
    {
        if (Platform_State.Moving == EPlatform_Moving_State::Moving_Left)
            Ball_Set->On_Platform_Advance(M_PI, fabs(Speed), max_speed);
        else
            if (Platform_State.Moving == EPlatform_Moving_State::Moving_Right)
                Ball_Set->On_Platform_Advance(0.0, fabs(Speed), max_speed);
    }
}


//
void AsPlatform::Begin_Movement()
{

}


//
void AsPlatform::Finish_Movement()
{
    if (Platform_State.Moving == EPlatform_Moving_State::Stop)
        return;

    Redraw_Platform();

    if (Platform_State.Moving == EPlatform_Moving_State::Stoping)
        Platform_State.Moving = EPlatform_Moving_State::Stop;
}



//
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
    double inner_top_y, inner_low_y;;
    double inner_left_x, inner_right_x;
    double ball_x, ball_y;
    double inner_y;
    double reflection_pos;

    // отражаем шарик от платформы
    if (next_y_pos + ball->RADIUS < AsConfig::Platform_Y_POS)
        return false;

    inner_top_y = (double)(AsConfig::Platform_Y_POS + 1);
    inner_low_y = (double)(AsConfig::Platform_Y_POS + Height - 1);
    inner_left_x = (double)(X_Pos + (CIRCLE_SIZE - 1));
    inner_right_x = (double)(X_Pos + Width - (CIRCLE_SIZE - 1));

    // Проверяем отражение от боковых шариков
    if (Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball))
        goto _on_hit;// От левого

    if (Reflect_On_Circle(next_x_pos, next_y_pos, Width - CIRCLE_SIZE, ball))
        goto _on_hit;// От правого


    // Проверяем отражение от центральной части
    if (ball->Is_Moving_Up())
        inner_y = inner_low_y; // От нижней грани
    else
        inner_y = inner_top_y; // От верхней грани
    
    if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, ball->RADIUS, reflection_pos))
    {
        ball->Reflect(true);
        goto _on_hit;
    }
    

    return false;

_on_hit:
    if (ball->Get_State() == EBS_On_Parachute)
        ball->Set_State(EBS_Off_Parachute);

    if (Platform_State == EPlatform_State::Glue && Platform_State.Glue == EPlatform_Substate_Glue::Active)
    {
        ball->Get_Center(ball_x, ball_y);
        ball->Set_State(EBS_On_Platform, ball_x, ball_y);
    }

    return true;
}


//
double AsPlatform::Get_Speed()
{
    return Speed;
}


//
void AsPlatform::Clear(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
        return;

    switch (Platform_State)
    {
    case EPlatform_State::Regular:
        if (Platform_State.Regular == EPlatform_Substate_Regular::Missing) //!!! Возможная ошибка с ковычками
            break;

    case EPlatform_State::Rolling:
    case EPlatform_State::Glue:
        AsConfig::BG_Color.Select(hdc);

        Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
            Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
    }
}


//
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
    {
        //return; // !!! не заказывается перерисовка для расплавляющейся платформы
    }

    switch (Platform_State)
    {
    case EPlatform_State::Regular:
        if(Platform_State.Regular == EPlatform_Substate_Regular::Ready || Platform_State.Regular == EPlatform_Substate_Regular::Normal)
            Draw_Normal_State(hdc, paint_area);
        break;

    case EPlatform_State::Meltdown:
        if(Platform_State.Meltdown == EPlatform_Substate_Meltdown::Active)
            Draw_Meltdown_State(hdc, paint_area);
        break;

    case EPlatform_State::Rolling:
        Draw_Rolling_State(hdc, paint_area);
        break;

    case EPlatform_State::Glue:
        Draw_Glue_State(hdc, paint_area);
        break;

    default:
        break;
    }
}


//
void AsPlatform::Act()
{
    HWND hwnd = AsConfig::HWnd;

    switch (Platform_State)
    {
    case EPlatform_State::Meltdown:
        Act_For_Meltdown_State();
        break;

    case EPlatform_State::Rolling:
        Act_For_Rolling_State();
        break;

    case EPlatform_State::Glue:
        Act_For_Glue_State();
        break;

    default:
        break;
    }
}


//
bool AsPlatform::Is_Finished()
{
    return false; // Заглушка, т.к. этот метод не используется
}


//
void AsPlatform::Init(AsBall_Set * ball_set)
{
    Ball_Set = ball_set;
}


//
void AsPlatform::Redraw_Platform(bool update_rect)
{
    HWND hwnd = AsConfig::HWnd;

    int platform_width;

    if (update_rect)
    {
        Prev_Platform_Rect = Platform_Rect;

        if (Platform_State == EPlatform_State::Rolling && Platform_State.Rolling == EPlatform_Substate_Rolling::Roll_In)
        {
            platform_width = CIRCLE_SIZE;
        }
        else
        {
            platform_width = Width;
        }

        Platform_Rect.left = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE);
        Platform_Rect.top = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;
        Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::GLOBAL_SCALE;
        Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::GLOBAL_SCALE;

        if (Platform_State == EPlatform_State::Meltdown)
        {
            Prev_Platform_Rect.bottom = (AsConfig::MAX_X_POS + 1) * AsConfig::GLOBAL_SCALE;
        }
    }

    AsConfig::Invalidate_Rect(Prev_Platform_Rect);
    AsConfig::Invalidate_Rect(Platform_Rect);
}


//
void AsPlatform::Set_State(EPlatform_State new_state)
{
    int len;

    if (Platform_State == new_state)
    {
        return;
    }

    switch (new_state)
    {
    case EPlatform_State::Regular:
        AsConfig::Throw(); //  Состояние EPlatform_State::Regular устанавливается неявно при вызове Set_State(EPlatform_Substate_Regular new_regular_state)
        break;

    case EPlatform_State::Meltdown:
        Speed = 0.0;

        Platform_State.Meltdown = EPlatform_Substate_Meltdown::Init;

        len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

        for (int i = 0; i < len; i++)
        {
            Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
        }
        break;

    case EPlatform_State::Rolling:
        Platform_State.Rolling = EPlatform_Substate_Rolling::Roll_In;
        X_Pos = AsConfig::MAX_X_POS;
        Rolling_Step = MAX_ROLLING_STEP - 2;
        break;

    case EPlatform_State::Glue:
        if (Platform_State.Glue == EPlatform_Substate_Glue::Finalize)
            return;
        else
        {
            Platform_State.Glue = EPlatform_Substate_Glue::Init;
            Glue_Spot_Height_Ratio = Min_Glue_Spot_Height_Ratio;
            
        }
        break;

    default:
        break;
    }

    Platform_State = new_state;
}


//
void AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
{
    if (Platform_State == EPlatform_State::Regular && Platform_State.Regular == new_regular_state)
        return;

    if (new_regular_state == EPlatform_Substate_Regular::Normal)
    {
        if (Platform_State == EPlatform_State::Glue)
        {
            Platform_State.Glue = EPlatform_Substate_Glue::Finalize;

            while (Ball_Set->Release_Next_Ball())
            {
            }

            return;
        }
    }

    Platform_State = EPlatform_State::Regular;
    Platform_State.Regular = new_regular_state;
}


//
void AsPlatform::Move(bool to_left, bool key_down)
{
    if (!(Has_State(EPlatform_Substate_Regular::Normal) || Platform_State == EPlatform_State::Glue))
        return;

    if (to_left)
        Left_Key_Down = key_down;
    else
        Right_Key_Down = key_down;

    if (Left_Key_Down && Right_Key_Down)
        return; // Игнорируем одновременное нажатие двух клавиш

    if (!Left_Key_Down && !Right_Key_Down)
    {
        Speed = 0.0;
        Platform_State.Moving = EPlatform_Moving_State::Stoping;
        return;
    }


    if (Left_Key_Down)
    {
        Platform_State.Moving = EPlatform_Moving_State::Moving_Left;
        Speed = -X_Step;
    }
    
    if(Right_Key_Down)
    {
        Platform_State.Moving = EPlatform_Moving_State::Moving_Right;
        Speed = X_Step;
    }
}


//
void AsPlatform::On_Space_Key(bool key_down)
{
    if (!key_down)
        return;
        
    if (Has_State(EPlatform_Substate_Regular::Ready))
    {
        Ball_Set->Release_From_Platform(Get_Middle_Pos());
        Set_State(EPlatform_Substate_Regular::Normal);
    }
    else if(Platform_State == EPlatform_State::Glue)
        Ball_Set->Release_Next_Ball();

}


//
bool AsPlatform::Hit_by(AFalling_Letter* falling_letter)
{
    RECT intersection_rect, falling_letter_rect;

    falling_letter->Get_Letter_Cell(falling_letter_rect);

    if (IntersectRect(&intersection_rect, &falling_letter_rect, &Platform_Rect))
        return true;
    else
        return false;
}


//
bool AsPlatform::Has_State(EPlatform_Substate_Regular regular_state)
{
    if (Platform_State != EPlatform_State::Regular)
        return false;

    if (Platform_State.Regular == regular_state)
        return true;
    else
        return false;
}


//
double AsPlatform::Get_Middle_Pos()
{
    return X_Pos + (double)Width / 2;
}


//
EPlatform_State AsPlatform::Get_State()
{
    return Platform_State;
}


//
void AsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{
    double x = X_Pos;
    int y = AsConfig::Platform_Y_POS;
    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    int size = (CIRCLE_SIZE - 1) * scale - 1;

    RECT inner_rect, rect;

    // 1. Рисуем боковые шарики
    Platform_Circle_Color.Select(hdc);

    rect.left = (int)(x * d_scale);
    rect.top = y * scale;
    rect.right = (int)((x + (double)CIRCLE_SIZE) * d_scale);
    rect.bottom = (y + CIRCLE_SIZE) * scale;

    Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

    rect.left = (int)((x + Inner_width) * d_scale);
    rect.top = y * scale;
    rect.right = (int)((x + Inner_width + (double)CIRCLE_SIZE) * d_scale);
    rect.bottom = (y + CIRCLE_SIZE) * scale;


    Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);


    // 2. Рисуем среднюю платформу
    Platform_Inner_Color.Select(hdc);

    inner_rect.left = (int)((x + 4) * d_scale);
    inner_rect.top = (y + 1) * scale;
    inner_rect.right = (int)((x + 4 + Inner_width - 1) * d_scale);
    inner_rect.bottom = (y + 1 + 5) * scale;

    AsConfig::Round_Rect(hdc, inner_rect, 3);

    if (Normal_Platform_Image == nullptr && Has_State(EPlatform_Substate_Regular::Ready))
        Get_Normal_Platform_Image(hdc);
}


//
void AsPlatform::Get_Normal_Platform_Image(HDC hdc)
{
    int offset = 0;

    int x = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE);
    int y = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;

    
    Normal_Platform_Image_Width = Width * AsConfig::GLOBAL_SCALE;
    Normal_Platform_Image_Height = Height * AsConfig::GLOBAL_SCALE;

    Normal_Platform_Image = new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height];

    for (int i = 0; i < Normal_Platform_Image_Height; i++)
    {
        for (int j = 0; j < Normal_Platform_Image_Width; j++)
        {
            Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
        }
    }
}


//
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{
    int x, y, j;
    int y_offset;
    int stroke_len;
    int moved_columns_count = 0;
    int max_platform_y = (AsConfig::MAX_Y_POS + 1) * AsConfig::GLOBAL_SCALE;
    const AColor *color = nullptr;


    for (int i = 0; i < Normal_Platform_Image_Width; i++)
    {
        if (Meltdown_Platform_Y_Pos[i] > max_platform_y + 5)
        {
            continue;
        }

        moved_columns_count += 1;

        x = Platform_Rect.left + i;

        y_offset = AsConfig::Rand(AsConfig::Meltdown_Speed) + 1;

        j = 0;
        y = Meltdown_Platform_Y_Pos[i];

        MoveToEx(hdc, x, y, 0);

        // Рисуем последовательность вертикальных штрихов разного цвета (согласно прообразу, сохраненному в Normal_Platform_Image)
        while (Get_Platform_Image_Stroke_Color(i, j, &color, stroke_len))
        {
            color->Select_Pen(hdc);
            LineTo(hdc, x, y + stroke_len);

            y += stroke_len;
            j += stroke_len;
        }

        // Стираем фоном пиксели над штрихом
        y = Meltdown_Platform_Y_Pos[i];
        MoveToEx(hdc, x, y, 0);
        AsConfig::BG_Color.Select_Pen(hdc);
        LineTo(hdc, x, y + y_offset);

        Meltdown_Platform_Y_Pos[i] += y_offset;
    }

    if (moved_columns_count == 0)
    {
        Set_State(EPlatform_Substate_Regular::Missing);
    }
}


//
void AsPlatform::Draw_Rolling_State(HDC hdc, RECT& paint_area)
{// Рисуем выкатывающуюся и расширяющуюся платформу

    switch (Platform_State.Rolling)
    {
    case EPlatform_Substate_Rolling::Roll_In:
        Draw_Roll_In_State(hdc, paint_area);
        break;

    case EPlatform_Substate_Rolling::Expand_Roll_In:
        Draw_Normal_State(hdc, paint_area); // Рисуем расширяющуюся платформу после выкатывания
        break;

    default:
        break;
    }
}


//
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT& paint_area)
{
    int x = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE);
    int y = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;
    int roller_size = CIRCLE_SIZE * AsConfig::GLOBAL_SCALE;
    double alpha;

    XFORM xform, old_xform;

    // 1. Шарик
    Platform_Circle_Color.Select(hdc);

    Ellipse(hdc, x, y, x + roller_size - 1, y + roller_size - 1);

    // 2. Разделительная линия


    alpha = -2.0 * M_PI / (double)MAX_ROLLING_STEP * (double)Rolling_Step;

    xform.eM11 = (float)cos(alpha);
    xform.eM12 = (float)sin(alpha);
    xform.eM21 = (float)-sin(alpha);
    xform.eM22 = (float)cos(alpha);
    xform.eDx = (float)(x + roller_size / 2);
    xform.eDy = (float)(y + roller_size / 2);

    GetWorldTransform(hdc, &old_xform);
    SetWorldTransform(hdc, &xform);

    AsConfig::BG_Color.Select(hdc);

    Rectangle(hdc, -AsConfig::GLOBAL_SCALE / 2, -roller_size / 2, AsConfig::GLOBAL_SCALE / 2, roller_size / 2);

    SetWorldTransform(hdc, &old_xform);
}


//
void AsPlatform::Draw_Glue_State(HDC hdc, RECT &paint_area)
{// Рисуем платформу с растикающим клеем

    HRGN region;
    RECT glue_rect;

    Draw_Normal_State(hdc, paint_area);

    glue_rect.left = (int)((X_Pos + 5.0) * AsConfig::D_GLOBAL_SCALE);
    glue_rect.top = (AsConfig::Platform_Y_POS + 1) * AsConfig::GLOBAL_SCALE;
    glue_rect.right = glue_rect.left + NORMAL_PLATFORM_INNER_WIDTH * AsConfig::GLOBAL_SCALE;
    glue_rect.bottom = glue_rect.top + (Height - 2) * AsConfig::GLOBAL_SCALE;

    region = CreateRectRgnIndirect(&glue_rect);
    SelectClipRgn(hdc, region);

    AsConfig::BG_Color.Select(hdc);

    Draw_Glue_Spot(hdc, 0, 9, 5);
    Draw_Glue_Spot(hdc, 6, 6, 5);
    Draw_Glue_Spot(hdc, 9, 9, 6);

    AsConfig::White_Color.Select(hdc);

    Draw_Glue_Spot(hdc, 0, 9, 4);
    Draw_Glue_Spot(hdc, 6, 6, 4);
    Draw_Glue_Spot(hdc, 9, 9, 5);

    SelectClipRgn(hdc, 0);
    DeleteObject(region);
}


//
void AsPlatform::Draw_Glue_Spot(HDC hdc, int x_offset, int width, int height)
{// Рисуем полуэлипс как "пятно" клея

    int platform_top = (AsConfig::Platform_Y_POS + 1) * AsConfig::GLOBAL_SCALE;
    int spot_height = (int)((double)height * AsConfig::D_GLOBAL_SCALE * Glue_Spot_Height_Ratio);
    RECT spot_rect;


    spot_rect.left = (int)((X_Pos + 5.0 + (double)x_offset) * AsConfig::D_GLOBAL_SCALE);
    spot_rect.top = platform_top - spot_height;
    spot_rect.right = spot_rect.left + width * AsConfig::GLOBAL_SCALE;
    spot_rect.bottom = platform_top + spot_height - AsConfig::GLOBAL_SCALE;


    Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right - 1, spot_rect.bottom - 1,
        spot_rect.left, platform_top - 1, spot_rect.right - 1, platform_top - 1);

}


//
void AsPlatform::Act_For_Meltdown_State()
{
    switch (Platform_State.Meltdown)
    {
    case EPlatform_Substate_Meltdown::Init:
        Platform_State.Meltdown = EPlatform_Substate_Meltdown::Active;
        break;

    case EPlatform_Substate_Meltdown::Active:
        Redraw_Platform();
        break;

    default:
        break;
    }
}


//
void AsPlatform::Act_For_Rolling_State()
{
    switch (Platform_State.Rolling)
    {
    case EPlatform_Substate_Rolling::Roll_In:
        Rolling_Step++;

        if (Rolling_Step > MAX_ROLLING_STEP)
            Rolling_Step -= MAX_ROLLING_STEP;

        X_Pos -= ROLLING_PLATFORM_SPEED;

        if (X_Pos <= ROLL_IN_PLATFORM_END_X_POS)
        {
            X_Pos += ROLLING_PLATFORM_SPEED;
            Platform_State.Rolling = EPlatform_Substate_Rolling::Expand_Roll_In;
            Inner_width = 1;
        }
        break;

    case EPlatform_Substate_Rolling::Expand_Roll_In:
        X_Pos --;

        Inner_width += 2;

        if (Inner_width >= NORMAL_PLATFORM_INNER_WIDTH)
        {
            Inner_width = NORMAL_PLATFORM_INNER_WIDTH;
            Platform_State.Rolling = EPlatform_Substate_Rolling::Unknow;
            Set_State(EPlatform_Substate_Regular::Ready);
            Redraw_Platform();
        }
        break;

    default:
        break;
    }

    


    Redraw_Platform();
}


//
void AsPlatform::Act_For_Glue_State()
{
    switch (Platform_State.Glue)
    {
    case EPlatform_Substate_Glue::Init:
        if (Glue_Spot_Height_Ratio < Max_Glue_Spot_Height_Ratio)
            Glue_Spot_Height_Ratio += Glue_Spot_Height_Ratio_Step;
        else
            Platform_State.Glue = EPlatform_Substate_Glue::Active;

        Redraw_Platform(false);
        break;

    case EPlatform_Substate_Glue::Finalize:
        if (Glue_Spot_Height_Ratio > Min_Glue_Spot_Height_Ratio)
            Glue_Spot_Height_Ratio -= Glue_Spot_Height_Ratio_Step;
        else
        {
            Set_State(EPlatform_Substate_Regular::Normal);
            Platform_State.Glue = EPlatform_Substate_Glue::Unknow;
        }
        Redraw_Platform(false);
        break;

    default:
        break;
    }

}


//
bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball)
{
    double dx, dy;
    double platform_ball_x, platform_ball_y, platform_ball_radius;
    double distance, two_radius;

    double alpha, beta, gamma;
    double realted_ball_direction;

    const double pi_2 = 2.0 * M_PI;

    platform_ball_radius = (double)(CIRCLE_SIZE / 2.0);
    platform_ball_x = (double)(X_Pos + platform_ball_radius + platform_ball_x_offset);
    platform_ball_y = (double)AsConfig::Platform_Y_POS + platform_ball_radius;

    dx = next_x_pos - platform_ball_x;
    dy = next_y_pos - platform_ball_y;

    distance = sqrt((dx * dx) + (dy * dy));
    two_radius = platform_ball_radius + ball->RADIUS;

    //if (fabs(distance - two_radius) < AsConfig::Moving_STEP_SIZE)
    if (distance - AsConfig::Moving_STEP_SIZE < two_radius)
    {// Мячик коснулся бокового шарика
        beta = atan2(-dy, dx);

        realted_ball_direction = ball->Get_Direction();
        realted_ball_direction -= beta;

        if (realted_ball_direction > pi_2)
            realted_ball_direction -= pi_2;

        if (realted_ball_direction < 0)
            realted_ball_direction += pi_2;

        if (realted_ball_direction > M_PI_2 && realted_ball_direction < M_PI + M_PI_2)
        {
            alpha = beta + M_PI - ball->Get_Direction();
            gamma = beta + alpha;

            ball->Set_Direction(gamma);

            return true;
        }
    }

    return false;
}


//
bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, const AColor **color, int &stroke_len)
{// Вычисляет длину очередного вертикального штриха
    int offset = y * Normal_Platform_Image_Width + x; // Позиция в массиве Normal_Platform_Image_Height соответствующая смещению (x, y)
    int color_value;  

    stroke_len = 0;

    if (y >= Normal_Platform_Image_Height)
        return false;

    for (int i = y; i < Normal_Platform_Image_Height; i++)
    {
        if (i == y)
        {
            color_value = Normal_Platform_Image[offset];
            stroke_len = 1;
        }
        else
        {
            if (color_value == Normal_Platform_Image[offset])
                stroke_len++;
            else
                break;
        }

        offset += Normal_Platform_Image_Width; // Переходим на строку ниже
    }

    if (color_value == Platform_Circle_Color.Get_RGB())
        *color = &Platform_Circle_Color;
    else if (color_value == Platform_Inner_Color.Get_RGB())
        *color = &Platform_Inner_Color;
    else if (color_value == AsConfig::BG_Color.Get_RGB())
        *color = &AsConfig::BG_Color;
    else
        AsConfig::Throw();

    return true;
}