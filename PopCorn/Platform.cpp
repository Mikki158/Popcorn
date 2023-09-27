#include "Platform.h"

// AsPlatform
AHit_Checker_List AsPlatform::Hit_Checker_List;

//
AsPlatform::AsPlatform()
    : X_Pos(AsConfig::BORDER_X_OFFSET), Inner_width(AsConfig::Platform_NORMAL_WIDTH - AsConfig::Platform_CIRCLE_SIZE),
    Platform_Rect(), Prev_Platform_Rect(), Meltdown_Y_Pos(0), Rolling_Step(0), Normal_Platform_Image(nullptr),
    Normal_Platform_Image_Width(0), Normal_Platform_Image_Height(0), Platform_Circle_Color(255, 85, 255), 
    Platform_Inner_Color(85, 255, 255), Meltdown_Platform_Y_Pos(), Speed(0.0), Left_Key_Down(false), 
    Right_Key_Down(false), Ball_Set(nullptr), Last_Redraw_Timer_Tick(0), 
    Gun_Color(AsConfig::White_Color, AsConfig::GLOBAL_SCALE), Platform_Glue(Platform_State), 
    Platform_Expanding(Platform_State), Platform_Laser(Platform_State)
{
    X_Pos = (AsConfig::MAX_X_POS - AsConfig::Platform_NORMAL_WIDTH) / 2;
}


//
AsPlatform::~AsPlatform()
{
    delete[] Normal_Platform_Image;
}


//
void AsPlatform::Advance(double max_speed)
{
    double next_step;

    if (Platform_State.Moving == EPlatform_Moving_State::Stoping || Platform_State.Moving == EPlatform_Moving_State::Stop)
        return;

    next_step = Speed / max_speed * AsConfig::Moving_STEP_SIZE;
    X_Pos += next_step;

    if (Correct_Platform_Pos())
    {
        Speed = 0.0;
        Platform_State.Moving = EPlatform_Moving_State::Stoping;
    }

    // Смещаем прикрепленные мячики
    if ((Platform_State == EPlatform_State::Regular && Platform_State.Regular == EPlatform_Substate_Regular::Ready) || 
        (Platform_State == EPlatform_State::Glue && Platform_State.Glue == EPlatform_Transformation::Active))
    {
        if (Platform_State.Moving == EPlatform_Moving_State::Moving_Left)
            Ball_Set->On_Platform_Advance(M_PI, fabs(Speed), max_speed);
        else
            if (Platform_State.Moving == EPlatform_Moving_State::Moving_Right)
                Ball_Set->On_Platform_Advance(0.0, fabs(Speed), max_speed);
    }

    Hit_Checker_List.Check_Hit(Platform_Rect);

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
bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball)
{
    double inner_top_y, inner_low_y;;
    double inner_left_x, inner_right_x;
    double ball_x, ball_y;
    double inner_y;
    double reflection_pos;
    double circle_x, circle_y, circle_radius;


    // отражаем шарик от платформы
    if (next_y_pos + AsConfig::Ball_RADIUS < AsConfig::Platform_Y_POS)
        return false;

    inner_top_y = (double)(AsConfig::Platform_Y_POS + 1);
    inner_low_y = (double)(AsConfig::Platform_Y_POS + AsConfig::Platform_Height - 1);
    inner_left_x = (double)(X_Pos + (AsConfig::Platform_CIRCLE_SIZE - 1));
    inner_right_x = (double)(X_Pos + Get_Current_Width() - (AsConfig::Platform_CIRCLE_SIZE - 1));

    // Проверяем отражение от боковых шариков
    circle_radius = (double)AsConfig::Platform_CIRCLE_SIZE / 2.0;
    circle_x = (double)X_Pos + circle_radius;
    circle_y = (double)AsConfig::Platform_Y_POS + circle_radius;

    if (AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, circle_x, circle_y, circle_radius, ball))
        goto _on_hit;// От левого

    circle_x += Get_Current_Width() - AsConfig::Platform_CIRCLE_SIZE;


    if (AsTools::Reflect_On_Circle(next_x_pos, next_y_pos, circle_x, circle_y, circle_radius, ball))
        goto _on_hit;// От правого


    // Проверяем отражение от центральной части
    if (ball->Is_Moving_Up())
        inner_y = inner_low_y; // От нижней грани
    else
        inner_y = inner_top_y; // От верхней грани
    
    if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, AsConfig::Ball_RADIUS, reflection_pos))
    {
        ball->Reflect(true);
        goto _on_hit;
    }
    

    return false;

_on_hit:
    if (ball->Get_State() == EBall_State::On_Parachute)
        ball->Set_State(EBall_State::Off_Parachute);

    if (Platform_State == EPlatform_State::Glue && Platform_State.Glue == EPlatform_Transformation::Active)
    {
        ball->Get_Center(ball_x, ball_y);
        ball->Set_State(EBall_State::On_Platform, ball_x, ball_y);
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
    case EPlatform_State::Meltdown:
    case EPlatform_State::Glue:
    case EPlatform_State::Expanding:
    case EPlatform_State::Laser:

        AsTools::Rect(hdc, Prev_Platform_Rect, AsConfig::BG_Color);
    }
}


//
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
        return; // !!! не заказывается перерисовка для расплавляющейся 

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
        Draw_Normal_State(hdc, paint_area);
        Platform_Glue.Draw_State(hdc, X_Pos);
        break;

    case EPlatform_State::Expanding:
        Platform_Expanding.Draw_State(hdc, X_Pos);
        break;

    case EPlatform_State::Laser:
        Platform_Laser.Draw_State(hdc, X_Pos, Platform_Rect);
        break;

    default:
        break;
    }
}


//
void AsPlatform::Act()
{
    bool correct_pos;
    EPlatform_State next_state;
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
        if (Platform_Glue.Act(Ball_Set, next_state))
            Redraw_Platform();

        if (next_state != EPlatform_State::Unknown)
            Set_State(next_state);
        break;

    case EPlatform_State::Expanding:
        if (Platform_Expanding.Act(X_Pos, next_state, correct_pos))
            Redraw_Platform();

        if(correct_pos)
            Correct_Platform_Pos();

        if (next_state != EPlatform_State::Unknown)
            Set_State(next_state);

        break;

    case EPlatform_State::Laser:
        if (Platform_Laser.Act(next_state, X_Pos))
            Redraw_Platform();

        if (next_state != EPlatform_State::Unknown)
            Set_State(next_state);
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
void AsPlatform::Init(AsBall_Set * ball_set, AsLaser_Beam_Set * laser_beam_set)
{
    Ball_Set = ball_set;

    Platform_Expanding.Init(Platform_Inner_Color, Platform_Circle_Color);
    Platform_Laser.Init(laser_beam_set, Platform_Inner_Color, Platform_Circle_Color);
}


//
void AsPlatform::Redraw_Platform()
{
    HWND hwnd = AsConfig::HWnd;

    if (Last_Redraw_Timer_Tick != AsConfig::Current_Timer_Tick)
    {
        Prev_Platform_Rect = Platform_Rect;
        Last_Redraw_Timer_Tick = AsConfig::Current_Timer_Tick;
    }

    Platform_Rect.left = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE);
    Platform_Rect.top = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;
    Platform_Rect.right = (int)((X_Pos + Get_Current_Width()) * AsConfig::D_GLOBAL_SCALE);
    Platform_Rect.bottom = Platform_Rect.top + AsConfig::Platform_Height * AsConfig::GLOBAL_SCALE;

    if (Platform_State == EPlatform_State::Meltdown)
    {
        Prev_Platform_Rect.bottom = (AsConfig::MAX_X_POS + 1) * AsConfig::GLOBAL_SCALE;
    }
    

    AsTools::Invalidate_Rect(Prev_Platform_Rect);
    AsTools::Invalidate_Rect(Platform_Rect);
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
        if (Platform_State != EPlatform_State::Regular)
        {
            Platform_State.Set_Next_State(new_state);
            return;
        }

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
        X_Pos = AsConfig::MAX_X_POS - 1;
        Rolling_Step = MAX_ROLLING_STEP - 1;
        Redraw_Platform();
        break;


    case EPlatform_State::Glue:
        if (Set_Transformation_State(new_state, Platform_State.Glue))
            return;
        else
            Platform_Glue.Reset();

        break;


    case EPlatform_State::Expanding:
        if (Set_Transformation_State(new_state, Platform_State.Expanding))
            return;
        else
            Platform_Expanding.Reset();
        break;

    case EPlatform_State::Laser:
        if (Set_Transformation_State(new_state, Platform_State.Laser))
            return;
        else
            Platform_Laser.Reset();
        break;

    default:
        break;
    }

    Platform_State = new_state;
}


//
void AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
{
    EPlatform_State next_state;

    next_state = Platform_State.Set_State(new_regular_state);

    if (next_state != EPlatform_State::Unknown)
        Set_State(next_state);
}


//
void AsPlatform::Move(bool to_left, bool key_down)
{
    if (!(Has_State(EPlatform_Substate_Regular::Normal) || Platform_State == EPlatform_State::Glue || 
        Platform_State == EPlatform_State::Expanding || Platform_State == EPlatform_State::Laser))
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
    
        
    if (Has_State(EPlatform_Substate_Regular::Ready))
    {
        if (!key_down)
        {
            Ball_Set->Release_From_Platform(Get_Middle_Pos());
            Set_State(EPlatform_Substate_Regular::Normal);
        }
    }
    else if (Platform_State == EPlatform_State::Glue)
    {
        if (!key_down)
            Ball_Set->Release_Next_Ball();
    }
    else if (Platform_State == EPlatform_State::Laser)
        Platform_Laser.Fire(key_down);

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
    return X_Pos + Get_Current_Width() / 2;
}


//
EPlatform_State AsPlatform::Get_State()
{
    return Platform_State;
}


//
bool AsPlatform::Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation& transformatio_state)
{
    if (Platform_State != EPlatform_State::Regular)
    {
        Platform_State.Set_Next_State(new_state);
        return true;
    }

    if (transformatio_state == EPlatform_Transformation::Finalize)
        return true;
    else
    {
        transformatio_state = EPlatform_Transformation::Init;
        return false;
    }
}


//
void AsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{
    double x = X_Pos;
    int y = AsConfig::Platform_Y_POS;
    const int scale = AsConfig::GLOBAL_SCALE;
    const double d_scale = AsConfig::D_GLOBAL_SCALE;
    int size = (AsConfig::Platform_CIRCLE_SIZE - 1) * scale - 1;

    RECT inner_rect, rect;

    // 1. Рисуем боковые шарики
    rect.left = (int)(x * d_scale);
    rect.top = y * scale;
    rect.right = (int)((x + (double)AsConfig::Platform_CIRCLE_SIZE) * d_scale);
    rect.bottom = (y + AsConfig::Platform_CIRCLE_SIZE) * scale;

    AsTools::Ellipse(hdc, rect, Platform_Circle_Color);


    rect.left = (int)((x + Inner_width) * d_scale);
    rect.top = y * scale;
    rect.right = (int)((x + Inner_width + (double)AsConfig::Platform_CIRCLE_SIZE) * d_scale);
    rect.bottom = (y + AsConfig::Platform_CIRCLE_SIZE) * scale;

    AsTools::Ellipse(hdc, rect, Platform_Circle_Color);


    // 2. Рисуем среднюю платформу
    Platform_Inner_Color.Select(hdc);

    inner_rect.left = (int)((x + 4) * d_scale);
    inner_rect.top = (y + 1) * scale;
    inner_rect.right = (int)((x + 4 + Inner_width - 1) * d_scale);
    inner_rect.bottom = (y + 1 + 5) * scale;

    AsTools::Round_Rect(hdc, inner_rect, 3);

    if (Normal_Platform_Image == nullptr && Has_State(EPlatform_Substate_Regular::Ready))
        Get_Normal_Platform_Image(hdc);
}


//
void AsPlatform::Get_Normal_Platform_Image(HDC hdc)
{
    int offset = 0;

    int x = (int)(X_Pos * AsConfig::D_GLOBAL_SCALE);
    int y = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;

    
    Normal_Platform_Image_Width = AsConfig::Platform_NORMAL_WIDTH * AsConfig::GLOBAL_SCALE;
    Normal_Platform_Image_Height = AsConfig::Platform_Height * AsConfig::GLOBAL_SCALE;

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

        y_offset = AsTools::Rand(AsConfig::Meltdown_Speed) + 1;

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
    int roller_size = AsConfig::Platform_CIRCLE_SIZE * AsConfig::GLOBAL_SCALE;
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
void AsPlatform::Act_For_Meltdown_State()
{
    switch (Platform_State.Meltdown)
    {
    case EPlatform_Substate_Meltdown::Init:
        Platform_State.Meltdown = EPlatform_Substate_Meltdown::Active;
        break;

    case EPlatform_Substate_Meltdown::Active:
        break;

    default:
        break;
    }

    Redraw_Platform();
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
            X_Pos = ROLL_IN_PLATFORM_END_X_POS;
            Platform_State.Rolling = EPlatform_Substate_Rolling::Expand_Roll_In;
            Inner_width = 1;
        }
        break;

    case EPlatform_Substate_Rolling::Expand_Roll_In:
        X_Pos --;

        Inner_width += 2;

        if (Inner_width >= AsConfig::Platform_NORMAL_INNER_WIDTH)
        {
            Inner_width = AsConfig::Platform_NORMAL_INNER_WIDTH;
            Platform_State.Rolling = EPlatform_Substate_Rolling::Unknown;
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
bool AsPlatform::Correct_Platform_Pos()
{
    bool got_correction = false;
    double min_platform_x = AsConfig::BORDER_X_OFFSET + 1;
    double max_platform_x = AsConfig::MAX_X_POS - Get_Current_Width();

    if (X_Pos <= min_platform_x)
    {
        X_Pos = min_platform_x;
        got_correction = true;
    }

    if (X_Pos >= max_platform_x)
    {
        X_Pos = max_platform_x;
        got_correction = true;
    }

    return got_correction;
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
        AsConfig::Throw(); // !!! Почему?

    return true;
}


//
double AsPlatform::Get_Current_Width()
{
    if (Platform_State == EPlatform_State::Rolling && Platform_State.Rolling == EPlatform_Substate_Rolling::Roll_In)
        return (double)AsConfig::Platform_CIRCLE_SIZE;
    else if (Platform_State == EPlatform_State::Expanding)
        return Platform_Expanding.Expanding_Platform_Width;
    else
        return (double)AsConfig::Platform_NORMAL_WIDTH;
}